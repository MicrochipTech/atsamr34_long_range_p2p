/**
* \file  system_task_manager.h
*
* \brief This is the interface of LoRaWAN system task manager
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

#ifndef SYSTEM_TASK_MANAGER_H
#define SYSTEM_TASK_MANAGER_H
/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include <stdbool.h>
#include <stdint.h>

/************************************************************************/
/* Defines                                                              */
/************************************************************************/

/************************************************************************/
/* Types                                                                */
/************************************************************************/

/*! \brief Possible results of task handler execution */
typedef enum _SYSTEM_TaskStatus_t
{
  SYSTEM_TASK_SUCCESS = 0x00,
  SYSTEM_TASK_FAILURE = 0x01
} SYSTEM_TaskStatus_t;

/*! The list of task IDs. The IDs are sorted according to descending
priority. For each task ID there is the corresponding task handler function. */
typedef enum _SYSTEM_Task_t
{
  TIMER_TASK_ID   = 1 << 0,
  RADIO_TASK_ID   = 1 << 1,
  LORAWAN_TASK_ID = 1 << 2,
  PDS_TASK_ID     = 1 << 3,
  APP_TASK_ID     = 1 << 4,
} SYSTEM_Task_t;

/************************************************************************/
/* Prototypes                                                           */
/************************************************************************/
/********************************************************************//**
\brief  This function is called by the stack or from the main()

If several tasks have been posted by the moment of the function's call,
they are executed in order of layers' priority: a task of the layer with the
highest priority is executed first.
*************************************************************************/
void SYSTEM_RunTasks(void);

/*********************************************************************//**
\brief Posts a task to the task manager, which is later processed by the
       task handler of the corresponding stack layer. A task is processed
       when the SYSTEM_RunTasks() function.

\param[in] task - ID of the posted task.
*************************************************************************/
/*
IDs of the tasks are listed in the SYSTEM_Task_t enum. Each task has its
own priority and is called only if there is no any task with higher priority.
A handler is called when respective task can be run. Each task has its
own task handler.
Correspondence between tasks and handlers is listed below:  \n
RADIO RADIO_TaskHandler()
HAL - HAL_TaskHandler()
LORAWAN - LORAWAN_TaskHandler()
APP - APP_TaskHandler()
 */
void SYSTEM_PostTask(SYSTEM_Task_t task);

/*********************************************************************//**
\brief Returns the readiness of the system for sleep

\return '1' if the system is ready, '0' otherwise
*************************************************************************/
bool SYSTEM_ReadyToSleep(void);

#endif /* SYSTEM_TASK_MANAGER_H */

/* eof system_task_manager.h */

