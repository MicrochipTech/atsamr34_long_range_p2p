/**
* \file  pmm.h
*
* \brief This is the interface of LoRaWAN power management module
*
* Copyright (c) 2018 Microchip Technology Inc. and its subsidiaries. 
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

#ifndef PMM_H
#define PMM_H
#include <asf.h>
#include "sleep.h"
#ifdef	__cplusplus
extern "C" {
#endif

#ifdef CONF_PMM_ENABLE

/************************************************************************/
/* Constants                                                            */
/************************************************************************/

#define  PMM_WAKEUPTIME_MS         10u         // 10ms
#define  PMM_SLEEPTIME_MIN_MS      1000u       // 1s
#define  PMM_SLEEPTIME_MAX_MS      130990000u  // 36h23m10s

/************************************************************************/
/* Types                                                                */
/************************************************************************/

/* Describes the status of power manager for a sleep request */
typedef enum _PMM_Status_t
{
	/*
	 * PMM denies the request because system is not ready to sleep at
	 * the instance of sleep call.
	 */
	PMM_SLEEP_REQ_DENIED = 0,

	/*
	 * Power manager accepted and have already processed the request.
	 * i.e., sleep was done.
	 */
	PMM_SLEEP_REQ_PROCESSED
} PMM_Status_t;

/* Describes the states of power manager */
typedef enum _PMM_State_t
{
	/* PMM has come out of sleep */
	PMM_STATE_ACTIVE = 0,

	/* PMM is starting to sleep */
	PMM_STATE_SLEEP
} PMM_State_t;

/* Structure of sleep request */
typedef struct _PMM_SleepReq_t
{
	/* Sleep time requested to PMM. Unit is milliseconds */
	uint32_t sleepTimeMs;
    /*  Sleep Modes */
	HAL_SleepMode_t sleep_mode;
	/* Callback from sleep request */
	void (*pmmWakeupCallback)(uint32_t sleptDuration);
} PMM_SleepReq_t;


/************************************************************************/
/* Function declarations                                                */
/************************************************************************/

/**
 * \brief This function puts the system to sleep if possible
 *
 * \param[in]  *req  -  pointer to PMM_SleepReq_t request structure
 *
 * \return value of type PMM_Status_t
 *  PMM_SLEEP_REQ_DENIED when sleep is not possible at the instance
 *  PMM_SLEEP_REQ_PROCESSED when sleep is possible and have already done
 */
PMM_Status_t PMM_Sleep(PMM_SleepReq_t *req);

/**
 * \brief Wakeup from sleep
 */
void PMM_Wakeup(void);

#ifdef	__cplusplus
}
#endif

#endif /* CONF_PMM_ENABLE */

#endif /* PMM_H */

/* eof pmm.h */
