/**
* \file  radio_task_manager.h
*
* \brief This is the Radio Driver Task Manager header file which contains Radio task
*        scheduler of the Radio Driver
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

#ifndef _RADIO_DRIVER_TASKMANAGER_H
#define _RADIO_DRIVER_TASKMANAGER_H

/******************************************************************************
                   Includes section
******************************************************************************/
#include "system_task_manager.h"

/******************************************************************************
                   Defines section
******************************************************************************/
#define RADIO_TASKS_COUNT               5u

/******************************************************************************
                               Types section
*******************************************************************************/
typedef enum
{
  RADIO_TX_DONE_TASK_ID = (1 << 0),
  RADIO_RX_DONE_TASK_ID = (1 << 1),
  RADIO_TX_TASK_ID      = (1 << 2),
  RADIO_RX_TASK_ID      = (1 << 3),
  RADIO_SCAN_TASK_ID = (1 << 4),
  RADIO_SLEEP_TASK_ID = (1 << 5)
} RadioTaskIds_t;

/******************************************************************************
                   Prototypes section
******************************************************************************/
extern SYSTEM_TaskStatus_t RADIO_TxHandler(void);
extern SYSTEM_TaskStatus_t RADIO_RxHandler(void);
extern SYSTEM_TaskStatus_t RADIO_TxDoneHandler(void);
extern SYSTEM_TaskStatus_t RADIO_RxDoneHandler(void);
extern SYSTEM_TaskStatus_t RADIO_ScanHandler(void);
/* SYSTEM_TaskStatus_t RADIO_SleepHandler(void); */

/**************************************************************************//**
\brief Set task for RADIO task manager.

\param[in] id - a single value from the type RadioTaskIds_t
******************************************************************************/
void radioPostTask(RadioTaskIds_t id);

/**************************************************************************//**
\brief Clear task for RADIO task manager.

\param[in] id - a single value from the type RadioTaskIds_t
******************************************************************************/
void radioClearTask(RadioTaskIds_t id);

#endif  /*_RADIO_DRIVER_TASKMANAGER_H*/

/* eof radio_task_manager.h */
