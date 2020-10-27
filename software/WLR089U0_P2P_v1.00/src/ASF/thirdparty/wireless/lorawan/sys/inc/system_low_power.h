/**
* \file  system_low_power.h
*
* \brief System low power header file
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

#ifndef _SYSTEM_LOW_POWER_H
#define _SYSTEM_LOW_POWER_H

#include <stdint.h>
#include <stdbool.h>

typedef void (*exitFromSleepCb_t)(void);

typedef enum
{
    // SLEEP_OFF = state used when the module is in running mode (default state)
    // SLEEP_OFF = the module exited from sleep mode either with sleep timeout or UART after auto baud detection ended
    SLEEP_OFF                                   = 0,
    // SLEEP_TRANSITION_TO_OFF = intermediary state between SLEEP_ON and SLEEP_OFF
    // SLEEP_TRANSITION_TO_OFF = state used after the MCU exits from sleep mode and the UART Auto Baud detection process is in progress
    SLEEP_TRANSITION_TO_OFF                     , 
    // SLEEP_ON = state used when in module in sleep mode
    SLEEP_ON                                    ,
} SysSleepState_t;

void SysSleepInit(exitFromSleepCb_t exitFromSleepCb);
void SysSleepStart(uint32_t ms);
void SysGoToSleep(void);
SysSleepState_t SysGetGoToSleepState(void);
void SysSetGoToSleepState(SysSleepState_t sleepState);
void SysExitFromSleep(void);
void EUSART1WakeUp(void);

#endif /* _SYSTEM_LOW_POWER_H */