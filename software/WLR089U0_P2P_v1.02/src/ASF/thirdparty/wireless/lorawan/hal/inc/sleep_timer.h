/**
* \file  sleep_timer.h
*
* \brief Sleep timer interface
*		
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
 

#ifndef SLEEP_TIMER_H_INCLUDED
#define SLEEP_TIMER_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONF_PMM_ENABLE
/**************************************** INCLUDES*****************************/
#include <stdint.h>

#define     COMPARE_COUNT_MAX_VALUE     (0xFFFFFFFF)

#define     US_TO_SLEEP_TICKS(u)        ((u) * (0.03278f))
#define     SLEEP_TICKS_TO_US(s)        ((s) * (30.5175f))

#define     MS_TO_SLEEP_TICKS(m)        ((m) * (32.769f))
#define     SLEEP_TICKS_TO_MS(s)        ((s) * (0.0306f))

/***************************************PROTOTYPES**************************/
/**
* \brief Initializes the sleep timer module
*/
void SleepTimerInit(void);

/**
* \brief Start the sleep timer
*/
void SleepTimerStart(uint32_t time, void (*cb)(void));

/**
* \brief Stop the sleep timer
*/
void SleepTimerStop(void);

/**
* \brief Calculate the Elapsed Time from the previous call of this function
* \retval Elapsed time in ticks
*/
uint32_t SleepTimerGetElapsedTime(void);

#endif /* CONF_PMM_ENABLE */

#ifdef  __cplusplus
}
#endif

#endif /* SLEEP_TIMER_H_INCLUDED */

/* eof sleep_timer.h */

