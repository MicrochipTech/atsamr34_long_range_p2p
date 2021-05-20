/**
* \file  pds_task_handler.h
*
* \brief     This is the PDS Driver Task Manager header file which calls PDS task
*	scheduler.
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


#ifndef _PDS_DRIVER_TASKMANAGER_H
#define _PDS_DRIVER_TASKMANAGER_H

/******************************************************************************
                   Includes section
******************************************************************************/
#include "system_task_manager.h"

/******************************************************************************
                   Defines section
******************************************************************************/
#define PDS_TASKS_COUNT               1u

/******************************************************************************
                               Types section
*******************************************************************************/
typedef enum
{
  PDS_STORE_DELETE_TASK_ID = (1 << 0)
} PdsTaskIds_t;

/******************************************************************************
                   Prototypes section
******************************************************************************/
SYSTEM_TaskStatus_t PDS_TaskHandler(void);

/**************************************************************************//**
\brief Set task for PDS task manager.

\param[in] id - a single value from the type PdsTaskIds_t
******************************************************************************/
void pdsPostTask(PdsTaskIds_t id);

/**************************************************************************//**
\brief Clear task for PDS task manager.

\param[in] id - a single value from the type PdsTaskIds_t
******************************************************************************/
void pdsClearTask(PdsTaskIds_t id);

#endif  /*_PDS_DRIVER_TASKMANAGER_H*/

/* eof pds_task_handler.h */
