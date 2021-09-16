/**
* \file  pmm.c
*
* \brief This is the implementation of LoRaWAN power management module
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

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
/* Standard headers */
#include <stdint.h>

/* PMM headers */
#include "pmm.h"
#include "conf_pmm.h"

/* Timer headers */
#include "hw_timer.h"
#include "sw_timer.h"
#include "sleep_timer.h"

/* HAL sleep header */
#include "sleep.h"

/* Other required headers */
#include "atomic.h"
#include "system_task_manager.h"

#ifdef CONF_PMM_ENABLE
/************************************************************************/
/* Defines                                                              */
/************************************************************************/
#define	SW_TIMER_ERROR_CORRECTION_TICKS		2

/******************************************************************************
                     Prototypes section
******************************************************************************/
static inline bool validateSleepDuration(uint32_t durationMs);

/************************************************************************/
/* Static variables                                                     */
/************************************************************************/
static PMM_SleepReq_t *sleepReq = NULL;
static PMM_State_t pmmState = PMM_STATE_ACTIVE;

/************************************************************************/
/* Function definitions                                                 */
/************************************************************************/
static inline bool validateSleepDuration(uint32_t durationMs)
{
    return (PMM_SLEEPTIME_MIN_MS <= durationMs) && \
        (PMM_SLEEPTIME_MAX_MS >= durationMs) && \
        (SWTIMER_INVALID_TIMEOUT != durationMs);
}

/**
* \brief This function puts the system to sleep if possible
*
* \param[in]  *req  -  pointer to PMM_SleepReq_t request structure
*
* \return value of type PMM_Status_t
*         PMM_SLEEP_REQ_DENIED -- when sleep is not possible at the instance
*         PMM_SLEEP_REQ_PROCESSED -- when sleep is possible and have already done
*/
PMM_Status_t PMM_Sleep(PMM_SleepReq_t *req)
{
    PMM_Status_t status = PMM_SLEEP_REQ_DENIED;
    uint32_t sysSleepTime = ~0u; /* 0xffFFffFF is invalid */
    bool canSleep;

    if ( req && (PMM_STATE_ACTIVE == pmmState) )
    {
        canSleep = SYSTEM_ReadyToSleep();
        canSleep = canSleep && validateSleepDuration( req->sleepTimeMs );

        if ( false == canSleep )
        {
            return status;
        }

        if ( SLEEP_MODE_BACKUP == req->sleep_mode )
        {
            canSleep = canSleep && ( SWTIMER_INVALID_TIMEOUT == SwTimerNextExpiryDuration() );
            sysSleepTime = req->sleepTimeMs;
        }
        else if ( SLEEP_MODE_STANDBY == req->sleep_mode )
        {
            sysSleepTime = SwTimerNextExpiryDuration();
            sysSleepTime = (SWTIMER_INVALID_TIMEOUT == sysSleepTime) ? PMM_SLEEPTIME_MAX_MS : US_TO_MS( sysSleepTime );
            canSleep = canSleep && validateSleepDuration( sysSleepTime );
            if ( canSleep && (req->sleepTimeMs < sysSleepTime) )
            {
                sysSleepTime = req->sleepTimeMs;
            }
        }

        if ( canSleep && SYSTEM_ReadyToSleep() )
        {
            /* Start of sleep preparation */
            SystemTimerSuspend();
            SleepTimerStart( MS_TO_SLEEP_TICKS( sysSleepTime - PMM_WAKEUPTIME_MS ), PMM_Wakeup );
            pmmState = PMM_STATE_SLEEP;
            sleepReq = req;
            /* End of sleep preparation */

            /* Put the system to sleep */
            HAL_Sleep(req->sleep_mode);

            status = PMM_SLEEP_REQ_PROCESSED;
        }
    }

    return status;
}

/**
* \brief Wakeup from sleep
*/
void PMM_Wakeup(void)
{
    uint64_t sleptTimeUs = 0;

    if (PMM_STATE_SLEEP == pmmState)
    {

		pmmState = PMM_STATE_ACTIVE;
        sleptTimeUs = SLEEP_TICKS_TO_US(SleepTimerGetElapsedTime());
        SleepTimerStop();

        SystemTimerSync(sleptTimeUs);
        if (sleepReq && sleepReq->pmmWakeupCallback)
        {
            sleepReq->pmmWakeupCallback(US_TO_MS(sleptTimeUs));
            sleepReq = NULL;
        }
    }
}

#endif /* CONF_PMM_ENABLE */

/* eof pmm.c */
