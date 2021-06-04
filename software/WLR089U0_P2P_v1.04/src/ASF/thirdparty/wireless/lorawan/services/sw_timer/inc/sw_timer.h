/**
* \file  sw_timer.h
*
* \brief This is the implementation of LoRaWAN SW Timer module
*		
*
* Copyright (c) 2018-2020 Microchip Technology Inc. and its subsidiaries. 
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


/* Prevent double inclusion */
#ifndef COMMON_SW_TIMER_H
#define COMMON_SW_TIMER_H

/******************************************************************************
                     Includes section
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "sys.h"
#include "stack_common.h"
#include "system_task_manager.h"

/******************************************************************************
                     Types section
******************************************************************************/
/*
* Type definition for callbacks for timer functions
*/
typedef void (*SwTimerCallbackFunc_t)(void *);

/* Timeout type */
typedef enum _SwTimeoutType {
	/** The timeout is relative to the current time. */
	SW_TIMEOUT_RELATIVE,
	/** The timeout is an absolute value. */
	SW_TIMEOUT_ABSOLUTE
} SwTimeoutType_t;

/*
* This defines the structure of the time type.
*/
typedef struct _SwTimer {
	/* Timeout in microseconds */
	uint32_t absoluteExpiryTime;

	/* Callback function to be executed on expiry of the timer */
	void (*timerCb)(void*);

	/* Parameter to be passed to callback function of the expired timer */
	void *paramCb;

	/* Next timer which was started or has expired */
	uint8_t nextTimer;

	/* Whether this time is loaded is actually loaded into timer or not? */
	bool loaded;
} SwTimer_t;

/*
* This defines the type of the system timestamp
*/
typedef uint64_t SwTimestamp_t;

/******************************************************************************
                     Macros section
******************************************************************************/
/*
* Value to indicate end of timer in the array or queue
*/
#define SWTIMER_INVALID              (0xFF)

/*
* Shift mask to obtain the 16-bit system time out of a 32-bit timeout
*/
#define SWTIMER_SYSTIME_SHIFTMASK    (16)

/*
* Mask to obtain the 16-bit H/W time out of a 32-bit timeout
*/
#define SWTIMER_HWTIME_MASK          (0xFFFF)

/*
* The smallest timeout in microseconds
*/
#define SWTIMER_MIN_TIMEOUT          (0xff)

/*
* The largest timeout in microseconds
*/
#define SWTIMER_MAX_TIMEOUT          (0x7FFFFFFF)

/*
* Invalid timeout
*/
#define SWTIMER_INVALID_TIMEOUT      (0xFFFFFFFF)

/**
* Adds two time values
*/
#define ADD_TIME(a, b)               ((a) + (b))

/**
* Subtracts two time values
*/
#define SUB_TIME(a, b)               ((a) - (b))

/*
* Macro to convert milliseconds to microseconds
*/
#define MS_TO_US(m)                  ((m) * (1000))

/*
* Macro to convert microseconds to milliseconds
*/
#define US_TO_MS(u)                  ((u) / (1000))

/*
* Invalid value for the timestamp
*/
#define SW_TIMESTAMP_INVALID         (UINT64_MAX)

/*
* Maximum value in the timestamp
*/
#define SW_TIMESTAMP_MAX             (UINT64_MAX)

/******************************************************************************
                     Prototypes section
******************************************************************************/

/**************************************************************************//**
\brief Returns a timer id to be used before starting a timer

\param[out] timerId Value of the id returned by the function

\return LORAWAN_SUCCESS if new timerId is allocated
        LORAWAN_RESOURCE_UNAVAILABLE if there is no more timerId to allocate
******************************************************************************/
StackRetStatus_t SwTimerCreate(uint8_t *timerId);

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
  SwTimeoutType_t timeoutType, void *timerCb, void *paramCb);

/**************************************************************************//**
\brief Stops a running timer. It stops a running timer with specified timerId
\param timer_id Timer identifier
\return
        LORAWAN_INVALID_PARAMETER if timerId is not valid
        LORAWAN_INVALID_REQUEST if timerId was not started before
        LORAWAN_SUCCESS if it is successfully stopped
******************************************************************************/
StackRetStatus_t SwTimerStop(uint8_t timerId);

/**************************************************************************//**
\brief Get current system time.
\return Returns current system time in microseconds
******************************************************************************/
uint64_t SwTimerGetTime(void);

/**************************************************************************//**
\brief Checks whether a given timer is running or not
\param[in] timerId Timer ID to be checked for running
\return True if the timer is running else False
******************************************************************************/
bool SwTimerIsRunning(uint8_t timerid);

/**************************************************************************//**
\brief Handles Queues and Callbacks for Expired Timers
******************************************************************************/
void SwTimersExecute(void);

/**************************************************************************//**
\brief Initializes the Software Timer module
******************************************************************************/
void SystemTimerInit(void);

/**************************************************************************//**
\brief Resets the Software Timer module
******************************************************************************/
void SwTimerReset(void);

/**************************************************************************//**
\brief Returns the remaining timeout for the given timerId
\param[in] timerId Timer ID to get the remaining time
\return Remaining time until expiry in microseconds
******************************************************************************/
uint32_t SwTimerReadValue(uint8_t timerId);

/**************************************************************************//**
\brief Run the running timer for the given offset
\param[in] offset New time duration for the running timer
******************************************************************************/
void SwTimerRunRemainingTime(uint32_t offset);

/**************************************************************************//**
\brief Returns the duration until the next timer expiry
\return Returns the duration until the next timeout in microseconds
******************************************************************************/
uint32_t SwTimerNextExpiryDuration(void);

/**************************************************************************//**
\brief Handler for the timer tasks
\return SYSTEM_TASK_SUCCESS after servicing the timer triggers
******************************************************************************/
SYSTEM_TaskStatus_t TIMER_TaskHandler(void);

/**************************************************************************//**
\brief Suspends the software timer
******************************************************************************/
void SystemTimerSuspend(void);

/**************************************************************************//**
\brief Resumes the software timer by offseting it with given time
\param[in] timeToSync Amount of duration to offset from known system time
******************************************************************************/
void SystemTimerSync(uint64_t timeToSync);

/**************************************************************************//**
\brief Returns a timestamp id to be used before using it

\param[out] timestampId Value of the id returned by the function

\return LORAWAN_SUCCESS if new timerId is allocated
        LORAWAN_RESOURCE_UNAVAILABLE if there is no more timerId to allocate
******************************************************************************/
StackRetStatus_t SwTimerTimestampCreate(uint8_t *timestampId);

/**************************************************************************//**
\brief Returns the timestamp stored in the given timestamp index
\param[in] index Index of the system timestamp
\param[out] *timestamp Pointer-to-timestamp to be read from the given index
******************************************************************************/
void SwTimerReadTimestamp(uint8_t index, SwTimestamp_t *timestamp);

/**************************************************************************//**
\brief Stores the timestamp in the given timestamp index
\param[in] index Index of the system timestamp
\param[in] *timestamp Pointer-to-timestamp to be stored in the given index
******************************************************************************/
void SwTimerWriteTimestamp(uint8_t index, SwTimestamp_t *timestamp);

/**************************************************************************//**
\brief Difference between the given timestamps
\param[in] *earlier  Pointer-to-timestamp of known-to-be-earlier time
\param[in] *later    Pointer-to-timestamp of known-to-be-later time
\return SwTimestamp_t - time difference between later and earlier timestamps
******************************************************************************/
SwTimestamp_t SwTimerTimeDiff(SwTimestamp_t *earlier, SwTimestamp_t *later);

#endif /* COMMON_SW_TIMER_H */

/* eof sw_timer.h */
