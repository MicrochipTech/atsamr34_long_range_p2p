/**
* \file  sw_timer.c
*
* \brief This is the implementation of LoRaWAN SW Timer module
*		
*
* Copyright (c) 2019-2020 Microchip Technology Inc. and its subsidiaries. 
*
* \asf_license_start
*
* \page License
*
* Subject to your compliance with these terms, you may use Microchip
* software and any derivatives exclusively with Microchip products. 
* It is your responsibility to comply with third party license terms applicable 
* to your use of third party software (including open source software) that 
* may accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, 
* WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, 
* INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, 
* AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE 
* LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL 
* LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE 
* SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE 
* POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY 
* RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, 
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*
* \asf_license_stop
*
*/
/*
* Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
*/

/******************************************************************************
                     Includes section
******************************************************************************/
#include <string.h>
#include "atomic.h"
#include "system_assert.h"
#include "conf_sw_timer.h"
#include "common_hw_timer.h"
#include "sw_timer.h"

#ifndef TOTAL_NUMBER_SW_TIMESTAMPS
#define TOTAL_NUMBER_SW_TIMESTAMPS (2u)
#endif /* #ifndef TOTAL_NUMBER_SW_TIMESTAMPS */

#if (TOTAL_NUMBER_OF_SW_TIMERS > 0)

  /****************************************************************************
                     Prototypes section
******************************************************************************/
static inline uint64_t gettime(void);
static void hwTimerExpiryCallback(void);
static void hwTimerOverflowCallback(void);
static void loadHwTimer(uint8_t timer_id);
static void swtimerProcessOverflow(void);
static void swtimerInternalHandler(void);
static inline bool swtimerCompareTime(uint32_t t1, uint32_t t2);
static void swtimerStartAbsoluteTimer(uint8_t timer_id,
    uint32_t point_in_time, void * handler_cb, void *parameter);

/******************************************************************************
                     Global variables section
******************************************************************************/

/******************************************************************************
             +-------------------+----------+-----------+
             | SYS_TIME_OVERFLOW | SYS_TIME | TC0_COUNT |
             +-------------------+----------+-----------+
             63                  31         15          0
******************************************************************************/
/*
* This represents the bits 15 to 31 of the low word of system time.
* And, it is incremented whenever the TC0_COUNT overflows.
*/
volatile uint16_t sysTime;

/*
* This represents the bits 31 to 63 of the high word of system time.
* And, it is incremented whenever the \sysTime overflows.
*/
volatile uint32_t sysTimeOvf;

/* This flag indicates an expired timer. */
volatile bool isTimerTriggered;

/* This is the timer array. */
SwTimer_t swTimers[TOTAL_NUMBER_OF_SW_TIMERS];
 
/* This is the timestamp array */
SwTimestamp_t swTimestamp[TOTAL_NUMBER_SW_TIMESTAMPS];

/******************************************************************************
                     Static variables section
******************************************************************************/
/* This is the counter of all running timers. */
static volatile uint8_t runningTimers;

/* This is the reference to the head of the running timer queue. */
static uint_fast8_t runningTimerQueueHead;

/* This is the reference to the head of the expired timer queue. */
static uint_fast8_t expiredTimerQueueHead;

/* This is the reference to the tail of the expired timer queue. */
static uint_fast8_t expiredTimerQueueTail;

/* This is the count of timers that are allocated at the instance. */
static uint8_t allocatedTimerId = 0;

/* This is the count of timestamps that are allocated at the instance. */
static uint8_t allocatedTimestampId = 0;

/* This is the last known system time saved before sleep */
static uint64_t sysTimeLastKnown = 0;

/******************************************************************************
                     Interrupt service routines
******************************************************************************/
//m16946 required
extern void SYS_HwOverflow_Cb(void);
/* ISR to handle OVF interrupt from TC0 */
static void hwTimerOverflowCallback(void)
{
    uint16_t temp = sysTime;
    if (++sysTime < temp)
    {
        sysTimeOvf++;
    }

    swtimerProcessOverflow();
//m16946 required for LoRa P2P
	SYS_HwOverflow_Cb();
}

/* ISR to handle CC0 interrupt from TC0 */
static void hwTimerExpiryCallback(void)
{
    if (0 < runningTimers)
    {
        isTimerTriggered = true;
        SYSTEM_PostTask(TIMER_TASK_ID);
    }
}

/******************************************************************************
                     Implementation section
******************************************************************************/

/**************************************************************************//**
\brief Inserts the timer in running timers queue in sorted order
******************************************************************************/
static void swtimerStartAbsoluteTimer(uint8_t timerId, uint32_t pointInTime,
    void *handlerCb, void *parameter)
{
    uint8_t flags = cpu_irq_save();

    /* Check is done to see if any timer has expired */
    swtimerInternalHandler();

    swTimers[timerId].absoluteExpiryTime = pointInTime;
    swTimers[timerId].timerCb = (void (*)(void*))handlerCb;
    swTimers[timerId].paramCb = parameter;
    swTimers[timerId].loaded = false;
    runningTimers++;

    if (SWTIMER_INVALID == runningTimerQueueHead)
    {
        runningTimerQueueHead = timerId;
        loadHwTimer(runningTimerQueueHead);
    }
    else
    {
        uint8_t index;
        bool isTimerInserted = false;
        uint8_t currIndex = runningTimerQueueHead;
        uint8_t prevIndex = runningTimerQueueHead;

        /*
        * Loop through the running timers in the array of swTimers.
        * Find the correct position for timer identified by 'timerId'
        * to be inserted in the runningTimersQueue.
        */
        for (index = 0; index < runningTimers; index++)
        {
            if (SWTIMER_INVALID != currIndex)
            {
                if (swtimerCompareTime(swTimers[currIndex].absoluteExpiryTime,
                                       pointInTime))
                {
                    /*
                    * Requested absolute time value is greater than the time
                    * value pointed by the curr_index in the timer array
                    */
                    prevIndex = currIndex;
                    currIndex = swTimers[currIndex].nextTimer;
                }
                else
                {
                /*
                * New timer 'timerId' to be inserted is having expiryDuration
                * that is lesser than the timer pointed by 'currentIndex'.
                */
                    swTimers[timerId].nextTimer = currIndex;
                    swTimers[currIndex].loaded = false;
                    if (runningTimerQueueHead == currIndex)
                    {
                        /* Insertion at the head of the timer queue. */
                        runningTimerQueueHead = timerId;

                        loadHwTimer(runningTimerQueueHead);
                    }
                    else
                    {
                    /* Insertion at somewhere in the middle of timer queue. */
                        swTimers[prevIndex].nextTimer = timerId;
                    }

                    isTimerInserted = true;
                    break;
                }
            }
        }

        if (!isTimerInserted)
        {
            /* Insertion at the tail of the timer queue. */
            swTimers[prevIndex].nextTimer = timerId;
            swTimers[timerId].nextTimer = SWTIMER_INVALID;
        }
    }

    cpu_irq_restore(flags);
}

/**************************************************************************//**
\brief Sets the timer compare value for the given timer, if it is
       within the overflow limit, else it will not load compare
******************************************************************************/
static void loadHwTimer(uint8_t timerId)
{
    if (SWTIMER_INVALID != timerId)
    {
        uint32_t now = gettime();
        uint32_t expiryTime = swTimers[timerId].absoluteExpiryTime;
        if (swtimerCompareTime(now, expiryTime))
        {
            if (!swTimers[timerId].loaded)
            {
                uint32_t timeDiff = expiryTime - now;

                if (SWTIMER_MIN_TIMEOUT >= timeDiff)
                {
                    isTimerTriggered = true;
                    SYSTEM_PostTask(TIMER_TASK_ID);
                }
                else  if ((uint32_t)TIMER_PERIOD >= timeDiff)
                {
                    common_tc_delay((uint16_t)timeDiff);
                    swTimers[timerId].loaded = true;
                }
                else
                {
                    swTimers[timerId].loaded = false;
                }
            }
        }
        else
        {
            isTimerTriggered = true;
            SYSTEM_PostTask(TIMER_TASK_ID);
        }
    }
    else
    {
        common_tc_compare_stop();
    }
}

/**************************************************************************//**
\brief Compares two time values

\param[in] t1   First timer's value in microseconds
\param[in] t2   Second timer's value in microseconds

\return True if "t1 is lesser than or equal to t2" else False
******************************************************************************/
static inline bool swtimerCompareTime(uint32_t t1, uint32_t t2)
{
    return ((t2 - t1) < INT32_MAX);
}

/**************************************************************************//**
\brief Returns the system time in microseconds
\return System time in us
******************************************************************************/
static inline uint64_t gettime(void)
{
    uint64_t time = 0uL;
    time |= ((uint64_t) sysTimeOvf) << 32;
    time |= ((uint64_t) sysTime) << 16;
    time |= (uint64_t) common_tc_read_count();
    return time;
}

/**************************************************************************//**
\brief Process the overflow interrupt of TC0
******************************************************************************/
static void swtimerProcessOverflow(void)
{
    uint32_t tmo32;
    uint16_t tmoHigh16, tmoLow16;
    uint8_t flags = cpu_irq_save();

    if (SWTIMER_INVALID != runningTimerQueueHead && !swTimers[runningTimerQueueHead].loaded)
    {
        tmo32 = swTimers[runningTimerQueueHead].absoluteExpiryTime;
        tmoHigh16 = (uint16_t)(tmo32 >> SWTIMER_SYSTIME_SHIFTMASK);

        if (tmoHigh16 == sysTime)
        {
            tmoLow16 = (uint16_t)(tmo32 & SWTIMER_HWTIME_MASK);
            if (SWTIMER_MIN_TIMEOUT < tmoLow16)
            {
                common_tc_delay(tmoLow16);
                swTimers[runningTimerQueueHead].loaded = true;
            }
            else
            {
                isTimerTriggered = true;
                SYSTEM_PostTask(TIMER_TASK_ID);
            }
        }
    }

    cpu_irq_restore(flags);
}

/**************************************************************************//**
\brief Internal handler for the timer trigger
******************************************************************************/
static void swtimerInternalHandler(void)
{
    if (isTimerTriggered)
    {
        isTimerTriggered = false;

        if (0 < runningTimers)
        { /* Holds the number of running timers */
            if ((expiredTimerQueueHead == SWTIMER_INVALID) && \
                (expiredTimerQueueTail == SWTIMER_INVALID))
            { /* in case of this is the only timer that has expired so far */
                expiredTimerQueueHead = runningTimerQueueHead;
                expiredTimerQueueTail = runningTimerQueueHead;
            }
            else
            { /* there were already some timers expired before this one */
                swTimers[expiredTimerQueueTail].nextTimer = runningTimerQueueHead;
                expiredTimerQueueTail = runningTimerQueueHead;
            }

            runningTimerQueueHead = swTimers[runningTimerQueueHead].nextTimer;

            swTimers[expiredTimerQueueTail].nextTimer = SWTIMER_INVALID;

            if ((--runningTimers) > 0)
            { /* keep the ball rolling! load the next head timer from the queue */
                loadHwTimer(runningTimerQueueHead);
            }
        }
    }
}


/**************************************************************************//**
\brief Handler for the timer tasks
\return SYSTEM_TASK_SUCCESS after servicing the timer triggers
******************************************************************************/
SYSTEM_TaskStatus_t TIMER_TaskHandler(void)
{
    SwTimersExecute();
    return SYSTEM_TASK_SUCCESS;
}

/**************************************************************************//**
\brief Resets the Software Timer module
******************************************************************************/
void SwTimerReset(void)
{
    /*
    * Initialize the timer resources like timer arrays queues, timer registers
    */
    uint8_t index;

    runningTimers = 0u;
    isTimerTriggered = false;

    runningTimerQueueHead = SWTIMER_INVALID;
    expiredTimerQueueHead = SWTIMER_INVALID;
    expiredTimerQueueTail = SWTIMER_INVALID;

    for (index = 0; index < TOTAL_NUMBER_OF_SW_TIMERS; index++)
    {
        swTimers[index].nextTimer = SWTIMER_INVALID;
        swTimers[index].timerCb = NULL;
    }

    allocatedTimerId = 0u;
}

/**************************************************************************//**
\brief Initializes the Software Timer module
******************************************************************************/
void SystemTimerInit(void)
{
    SwTimerReset();

    /* initialize system time parameters */
    sysTimeOvf = 0x00000000;
    sysTime = 0x0000;

    common_tc_init();
    set_common_tc_overflow_callback(hwTimerOverflowCallback);
    set_common_tc_expiry_callback(hwTimerExpiryCallback);
}

/**************************************************************************//**
\brief Get current system time.
\return Returns current system time in microseconds
******************************************************************************/
uint64_t SwTimerGetTime(void)
{
    return gettime();
}

/**************************************************************************//**
\brief Returns a timer id to be used before starting a timer

\param[out] timerId Value of the id returned by the function

\return LORAWAN_SUCCESS if new timerId is allocated
        LORAWAN_RESOURCE_UNAVAILABLE if there is no more timerId to allocate
******************************************************************************/
StackRetStatus_t SwTimerCreate(uint8_t *timerId)
{
    StackRetStatus_t retVal = LORAWAN_SUCCESS;

    if (allocatedTimerId < TOTAL_NUMBER_OF_SW_TIMERS)
    {
        ATOMIC_SECTION_ENTER
        *timerId = allocatedTimerId;
        allocatedTimerId++;
        ATOMIC_SECTION_EXIT
    }
    else
    {
        /*
        * If you reach this spot it means the TOTAL_NUMBER_OF_SW_TIMERS
        * is #defined to a lower value than the number of timers that have
        * been SwTimerCreate()
        */
        SYS_ASSERT_FATAL(ASSERT_HAL_TIMERID_EXHAUSTED);
        retVal = LORAWAN_RESOURCE_UNAVAILABLE;
    }

    return retVal;
}

/**************************************************************************//**
\brief Starts a  timer

       This function starts a regular timer and installs the corresponding
       callback function handle the timeout event.

\param[in] timerId Timer identifier
\param[in] timerCount Timeout in microseconds
\param[in] timeoutType \ref SW_TIMEOUT_RELATIVE or \ref SW_TIMEOUT_ABSOLUTE
\param[in] timerCb Callback handler invoked upon timer expiry
\param[in] paramCb Argument for the callback handler

\return LORAWAN_INVALID_PARAMETER if at least one input parameter in invalid
        LORAWAN_INVALID_REQUEST if \timerId is already running
        LORAWAN_SUCCESS if \timerId is successfully queued for running
******************************************************************************/
StackRetStatus_t SwTimerStart(uint8_t timerId, uint32_t timerCount,
    SwTimeoutType_t timeoutType, void *timerCb, void *paramCb)
{
    uint32_t now = 0;
    uint32_t pointInTime;

    if (TOTAL_NUMBER_OF_SW_TIMERS <= timerId || NULL == timerCb)
    {
        SYS_ASSERT_INFO(ASSERT_HAL_TIMER_START_FAILURE);
        return LORAWAN_INVALID_PARAMETER;
    }

    if (NULL != swTimers[timerId].timerCb)
    {
        /*
        * Timer is already running if the callback function of the
        * corresponding timer index in the timer array is not NULL.
        */
        SYS_ASSERT_INFO(ASSERT_HAL_TIMER_START_FAILURE);
        return LORAWAN_INVALID_REQUEST;
    }

    now = (uint32_t) gettime();

    switch (timeoutType)
    {
        case SW_TIMEOUT_RELATIVE:
        {
            if ((timerCount > SWTIMER_MAX_TIMEOUT) || \
            (timerCount < SWTIMER_MIN_TIMEOUT))
            {
                SYS_ASSERT_INFO(ASSERT_HAL_TIMER_START_FAILURE);
                return LORAWAN_INVALID_PARAMETER;
            }

            pointInTime = ADD_TIME(timerCount, now);
        }
        break;

        case SW_TIMEOUT_ABSOLUTE:
        {
            uint32_t timeout;
            timeout = SUB_TIME(timerCount, now);

            if ((timeout > SWTIMER_MAX_TIMEOUT) || \
            (timeout < SWTIMER_MIN_TIMEOUT))
            {
                SYS_ASSERT_INFO(ASSERT_HAL_TIMER_START_FAILURE);
                return LORAWAN_INVALID_PARAMETER;
            }

            pointInTime = timerCount;
        }
        break;

        default:
        {
            SYS_ASSERT_INFO(ASSERT_HAL_TIMER_START_FAILURE);
            return LORAWAN_INVALID_PARAMETER;
        }
    }

    swtimerStartAbsoluteTimer(timerId, pointInTime, timerCb, paramCb);
    return LORAWAN_SUCCESS;
}

/**************************************************************************//**
\brief Checks whether a given timer is running or not
\param[in] timerId Timer ID to be checked for running
\return True if the timer is running else False
******************************************************************************/
bool SwTimerIsRunning(uint8_t timerId)
{
    if (NULL == swTimers[timerId].timerCb)
    {
        return false;
    }

    return true;
}

/**************************************************************************//**
\brief Returns the remaining timeout for the given timerId
\param[in] timerId Timer ID to get the remaining time
\return Remaining time until expiry in microseconds
******************************************************************************/
uint32_t SwTimerReadValue(uint8_t timerId)
{
    uint32_t remainingTime = 0u;
    uint32_t timerExpiryTime = 0u;
    uint32_t currentSysTime = 0u;
    if ( NULL != swTimers[timerId].timerCb )
    {
	    timerExpiryTime = swTimers[timerId].absoluteExpiryTime;
	    currentSysTime = (uint32_t) gettime();
	    if ( currentSysTime <= timerExpiryTime )
	    {
		    remainingTime = timerExpiryTime - currentSysTime;
	    }
	    else if ( currentSysTime > timerExpiryTime )
	    {
		    remainingTime = (UINT32_MAX - currentSysTime) + timerExpiryTime;
	    }

        if (remainingTime >= SWTIMER_MAX_TIMEOUT)
        {
            /* Diff cannot be more than max timeout */
            remainingTime = 0;
        }
    }
    return remainingTime;
}

/**************************************************************************//**
\brief Returns the duration until the next timer expiry
\return Returns the duration until the next timeout in microseconds
******************************************************************************/
uint32_t SwTimerNextExpiryDuration(void)
{
    uint32_t duration = SWTIMER_INVALID_TIMEOUT;

    if (SWTIMER_INVALID != runningTimerQueueHead)
    {
        duration = SwTimerReadValue(runningTimerQueueHead);
    }

    return duration;
}

/**************************************************************************//**
\brief Run the running timer for the given offset
\param[in] offset New time duration for the running timer
******************************************************************************/
void SwTimerRunRemainingTime(uint32_t offset)
{
    void * timerCb = (void*)(swTimers[runningTimerQueueHead].timerCb);
    void *paramCb = swTimers[runningTimerQueueHead].paramCb;
    uint8_t timerId = runningTimerQueueHead;

    if (LORAWAN_SUCCESS == SwTimerStop(runningTimerQueueHead))
    {
        SwTimerStart(timerId, offset, SW_TIMEOUT_RELATIVE, timerCb, paramCb);
    }
}

/**************************************************************************//**
\brief Handles Queues and Callbacks for Expired Timers
******************************************************************************/
void SwTimersExecute(void)
{
    uint64_t now = gettime();

    uint8_t flags = cpu_irq_save();
    swtimerInternalHandler();
    cpu_irq_restore(flags);

    /*
    * Process expired timers.
    * Call the callback functions of the expired timers in the order of
    * their expiry.
    */
    {
        SwTimerCallbackFunc_t callback;
        void *cbParam;
        uint8_t nextExpiredTimer;

        /* Expired timer if any will be processed here */
        while (SWTIMER_INVALID != expiredTimerQueueHead)
        {
            flags = cpu_irq_save();

            nextExpiredTimer = swTimers[expiredTimerQueueHead].nextTimer;

            /* Callback is stored */
            callback = (SwTimerCallbackFunc_t)swTimers[expiredTimerQueueHead].timerCb;

            /* Callback parameter is stored */
            cbParam = swTimers[expiredTimerQueueHead].paramCb;

            /*
            * The expired timer's structure elements are updated
            * and the timer is taken out of expired timer queue
            */
            swTimers[expiredTimerQueueHead].nextTimer = SWTIMER_INVALID;
            swTimers[expiredTimerQueueHead].timerCb = NULL;
            swTimers[expiredTimerQueueHead].paramCb = NULL;
            swTimers[expiredTimerQueueHead].loaded = false;

            /*
            * The expired timer queue head is updated with the next
            * timer in the expired timer queue.
            */
            expiredTimerQueueHead = nextExpiredTimer;

            if (SWTIMER_INVALID == expiredTimerQueueHead)
            {
                expiredTimerQueueTail = SWTIMER_INVALID;
            }

            cpu_irq_restore(flags);

            if (NULL != callback)
            {
                /* Callback function is called */
                callback(cbParam);
                (void)now;
            }
        }
    }
}

/**************************************************************************//**
\brief Stops a running timer. It stops a running timer with specified timerId
\param timer_id Timer identifier
\return
        LORAWAN_INVALID_PARAMETER if timerId is not valid
        LORAWAN_INVALID_REQUEST if timerId was not started before
        LORAWAN_SUCCESS if it is successfully stopped
******************************************************************************/
StackRetStatus_t SwTimerStop(uint8_t timerId)
{
    bool timerStopReqStatus = false;
    uint8_t currIndex;
    uint8_t prevIndex;

    if (TOTAL_NUMBER_OF_SW_TIMERS <= timerId)
    {
        SYS_ASSERT_INFO(ASSERT_HAL_TIMER_STOP_FAILURE);
        return LORAWAN_INVALID_PARAMETER;
    }

    uint8_t flags = cpu_irq_save();

    /* Check if any timer has expired. */
    swtimerInternalHandler();

    /* The requested timer is first searched in the running timer queue */
    if (runningTimers > 0)
    {
        uint8_t timer_count = runningTimers;
        prevIndex = currIndex = runningTimerQueueHead;
        while (timer_count > 0)
        {
            if (timerId == currIndex)
            {
                timerStopReqStatus = true;
                if (timerId == runningTimerQueueHead)
                {
                    common_tc_compare_stop();
                    runningTimerQueueHead = swTimers[timerId].nextTimer;

                    /*
                    * The value in OVF corresponds to the timeout pointed
                    * by the 'running_timer_queue_head'. As the head has
                    * changed here, OVF needs to be loaded by the new
                    * timeout value, if any.
                    */
                    loadHwTimer(runningTimerQueueHead);
                }
                else
                {
                    swTimers[prevIndex].nextTimer = swTimers[timerId].nextTimer;
                }

                /*
                * The next timer element of the stopped timer is updated
                * to its default value.
                */
                swTimers[timerId].nextTimer  = SWTIMER_INVALID;
                break;
            }
            else
            {
                prevIndex = currIndex;
                currIndex = swTimers[currIndex].nextTimer;
            }

            timer_count--;
        }

        if (timerStopReqStatus)
        {
            runningTimers--;
        }
    }

    /*
    * The requested timer is not present in the running timer queue.
    * It will be now searched in the expired timer queue
    */
    if (!timerStopReqStatus)
    {
        prevIndex = currIndex = expiredTimerQueueHead;
        while (SWTIMER_INVALID != currIndex)
        {
            if (timerId == currIndex)
            {
                if (timerId == expiredTimerQueueHead)
                {
                    /* The requested timer is the head of the expired timer queue */
                    if (expiredTimerQueueHead == expiredTimerQueueTail)
                    {
                        /* Only one timer in expired timer queue */
                        expiredTimerQueueHead = expiredTimerQueueTail = SWTIMER_INVALID;
                    }
                    else
                    {
                        /*
                        * The head of the expired timer queue is moved to next
                        * timer in the expired timer queue.
                        */
                        expiredTimerQueueHead = swTimers[expiredTimerQueueHead].nextTimer;
                    }
                }
                else
                {
                    /*
                    * The requested timer is present in the middle or at the
                    * end of the expired timer queue.
                    */
                    swTimers[prevIndex].nextTimer = swTimers[timerId].nextTimer;

                    /*
                    * If the stopped timer is the one which is at the tail of
                    * the expired timer queue, then the tail is updated.
                    */
                    if (timerId == expiredTimerQueueTail)
                    {
                        expiredTimerQueueTail = prevIndex;
                    }
                }

                timerStopReqStatus = true;
                break;
            }
            else
            {
                prevIndex = currIndex;
                currIndex = swTimers[currIndex].nextTimer;
            }
        }
    }

    if (timerStopReqStatus)
    {
        /*
        * The requested timer is stopped, hence the structure elements
        * of the timer are updated.
        */
        swTimers[timerId].timerCb = NULL;
        swTimers[timerId].paramCb = NULL;
        swTimers[timerId].loaded = false;
    }

    cpu_irq_restore(flags);

    if (timerStopReqStatus)
    {
        return LORAWAN_SUCCESS;
    }

    SYS_ASSERT_INFO(ASSERT_HAL_TIMER_STOP_FAILURE);
    return LORAWAN_INVALID_REQUEST;
}

/**************************************************************************//**
\brief Suspends the software timer
******************************************************************************/
void SystemTimerSuspend(void)
{
    sysTimeLastKnown = gettime();
    common_tc_stop();
}

/**************************************************************************//**
\brief Resumes the software timer by offseting it with given time
\param[in] timeToSync Amount of duration to offset from known system time
******************************************************************************/
void SystemTimerSync(uint64_t timeToSync)
{
    uint8_t timerId;
    uint16_t adjustOffset;

    sysTimeLastKnown += timeToSync;

    /* 1. Update system time */
    sysTimeOvf = (uint32_t) (sysTimeLastKnown >> 32);
    sysTime = (uint16_t) ((sysTimeLastKnown >> SWTIMER_SYSTIME_SHIFTMASK) & 0xffff);

    /* 2. Adjust expiration of running timers */
    adjustOffset = (uint16_t) sysTimeLastKnown;
    timerId = runningTimerQueueHead;
    for (uint8_t index = 0; index < runningTimers; index++)
    {
        if (SWTIMER_INVALID != timerId)
        {
            swTimers[timerId].absoluteExpiryTime -= adjustOffset;
            timerId = swTimers[timerId].nextTimer;
        }
    }

    /* 3. Start hardware timer */
    common_tc_init();
    set_common_tc_overflow_callback(hwTimerOverflowCallback);
    set_common_tc_expiry_callback(hwTimerExpiryCallback);

    /* 4. Resume timer queue operations */
    if (runningTimers && (SWTIMER_INVALID != runningTimerQueueHead))
    {
        uint32_t remainingTime = SwTimerNextExpiryDuration();

        if (SWTIMER_MIN_TIMEOUT > remainingTime)
        {
            /*
            * Since head timer has very less duration...
            * It cannot be restarted. So, expire head timer.
            */
            isTimerTriggered = true;
            SYSTEM_PostTask(TIMER_TASK_ID);
        }
        else
        {
            /*
            * There is some time left in head timer, so restart it.
            */
            SwTimerRunRemainingTime(remainingTime);
        }
    }
}


/**************************************************************************//**
\brief Returns a timestamp id to be used before using it

\param[out] timestampId Value of the id returned by the function

\return LORAWAN_SUCCESS if new timerId is allocated
        LORAWAN_RESOURCE_UNAVAILABLE if there is no more timerId to allocate
******************************************************************************/
StackRetStatus_t SwTimerTimestampCreate(uint8_t *timestampId)
{
    StackRetStatus_t retVal = LORAWAN_SUCCESS;

    if (allocatedTimestampId < TOTAL_NUMBER_SW_TIMESTAMPS)
    {
        ATOMIC_SECTION_ENTER
        *timestampId = allocatedTimestampId;
        allocatedTimestampId++;
        ATOMIC_SECTION_EXIT
    }
    else
    {
        /*
        * If you reach this spot it means the TOTAL_NUMBER_SW_TIMESTAMPS
        * is #defined to a lower value than the number of timers that have
        * been SwTimerTimestampCreate()
        */
        SYS_ASSERT_FATAL(ASSERT_HAL_TIMESTAMPID_EXHAUSTED);
        retVal = LORAWAN_RESOURCE_UNAVAILABLE;
    }

    return retVal;
}

/**************************************************************************//**
\brief Returns the timestamp stored in the given timestamp index
\param[in] index Index of the system timestamp
\param[out] *timestamp Pointer-to-timestamp to be read from the given index
******************************************************************************/
void SwTimerReadTimestamp(uint8_t index, SwTimestamp_t *timestamp)
{
	*timestamp = swTimestamp[index];
}

/**************************************************************************//**
\brief Stores the timestamp in the given timestamp index
\param[in] index Index of the system timestamp
\param[in] *timestamp Pointer-to-timestamp to be stored in the given index
******************************************************************************/
void SwTimerWriteTimestamp(uint8_t index, SwTimestamp_t *timestamp)
{
    swTimestamp[index] = *timestamp;
}

/**************************************************************************//**
\brief Difference between the given timestamps
\param[in] *earlier  Pointer-to-timestamp of known-to-be-earlier time
\param[in] *later    Pointer-to-timestamp of known-to-be-later time
\return SwTimestamp_t - time difference between later and earlier timestamps
******************************************************************************/
SwTimestamp_t SwTimerTimeDiff(SwTimestamp_t *earlier, SwTimestamp_t *later)
{
	SwTimestamp_t diff = SW_TIMESTAMP_INVALID;

    if (*later >= *earlier)
	{
		diff = *later - *earlier;
	}
	else
	{
		diff = (UINT64_MAX - *earlier) + *later;
	}
	
	return diff;
}

#endif /* #if (TOTAL_NUMBER_OF_SW_TIMERS > 0) */

/* EOF sw_timer.c */
