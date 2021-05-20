/**
* \file  sleep.h
*
* \brief This is the hardware interface for sleep
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
 
#ifndef SLEEP_H
#define SLEEP_H

/************************************************************************/
/* Types                                                                */
/************************************************************************/
/* Possible sleep modes */
typedef enum _HAL_SleepMode_t
{
#if defined(SAMR34) || defined(SAMR35) || defined(WLR089)
  /* HAL Sleep modes for SAMR34, SAML21-compatible MCUs */
  SLEEP_MODE_IDLE    = 0,
  SLEEP_MODE_STANDBY,
  SLEEP_MODE_BACKUP,
  SLEEP_MODE_OFF
#else
  SLEEP_MODE_NONE
#endif
} HAL_SleepMode_t;

/************************************************************************/
/* Function declarations                                                */
/************************************************************************/

/**
 * \brief Puts the system in given sleep mode
 *
 * \param[in] mode - sleep mode
 */
void HAL_Sleep(HAL_SleepMode_t mode);

#endif /* SLEEP_H */

/* eof sleep.h */
