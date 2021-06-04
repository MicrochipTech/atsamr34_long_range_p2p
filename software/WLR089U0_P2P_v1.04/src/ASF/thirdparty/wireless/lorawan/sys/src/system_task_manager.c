/**
* \file  system_task_manager.c
*
* \brief This is the implementation of LoRaWAN system task manager
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

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include "system_init.h"
#include "atomic.h"
#include "system_task_manager.h"
/************************************************************************/
/* Defines                                                              */
/************************************************************************/
#define SYSTEM_TASK_ID_COUNT 5u

/************************************************************************/
/* Externals                                                            */
/************************************************************************/
//! This function is called to process RADIO task. SHOULD be defined in RADIO.
extern SYSTEM_TaskStatus_t RADIO_TaskHandler(void);

//! This function is called to process LORAWAN task. SHOULD be defined in LORAWAN.
extern SYSTEM_TaskStatus_t LORAWAN_TaskHandler(void);

//! This function is called to process system timer task. SHOULD be defined in TIMER.
extern SYSTEM_TaskStatus_t TIMER_TaskHandler(void);

//! This function is called to process pds task. SHOULD be defined in PDS.
extern SYSTEM_TaskStatus_t PDS_TaskHandler(void);

//! This function is called to process APP task. SHOULD be defined in APP.
extern SYSTEM_TaskStatus_t APP_TaskHandler(void);

/************************************************************************/
/*  Static variables                                                    */
/************************************************************************/
static SYSTEM_TaskStatus_t (*taskHandlers[SYSTEM_TASK_ID_COUNT])(void) ={
  /* In the order of descending priority */
    TIMER_TaskHandler,
    RADIO_TaskHandler,
    LORAWAN_TaskHandler,
    PDS_TaskHandler,
    APP_TaskHandler,
};

static volatile uint16_t sysTaskFlag = 0u;

/************************************************************************/
/* Implementations                                                      */
/************************************************************************/
/*********************************************************************//**
\brief System tasks execution entry point
*************************************************************************/
void SYSTEM_RunTasks(void)
{
    if ((1 << SYSTEM_TASK_ID_COUNT) > sysTaskFlag)
    { /* Only valid task bits are set */
        while (sysTaskFlag)
        { /* One or more task are pending to execute */
            for (uint16_t taskId = 0; taskId < SYSTEM_TASK_ID_COUNT; taskId++)
            {
                if ((1 << taskId) & sysTaskFlag)
                {
                    /*
                    * Reset the task bit since it is to be executed now.
                    * It is done inside atomic section to avoid any interrupt context
                    * corrupting the bits.
                    */
                    ATOMIC_SECTION_ENTER
                    sysTaskFlag &= ~(1 << taskId);
                    ATOMIC_SECTION_EXIT

                    /* Return value is not used now, can be used later */
                    taskHandlers[taskId]();

                    /* Break here so that higher priority task executes next, if any */
                    break;
                }
            }
        }
    }
    else
    {
        /*
        * Invalid task bits are set i.e., bits other than 4 LSB are set
        * Can happen only due to corruption, so halt
        * TODO : replace this with assert implementation
        */
        while(1);
    }
}

/*********************************************************************//**
\brief Posts a task to the task manager, which is later processed by the
       task handler of the corresponding stack layer. A task is processed
       when the SYSTEM_RunTasks() function. \n

       IDs of the tasks are listed in the SYSTEM_Task_t enum. Each task has its
       own priority and is called only if there is no any task with higher
       priority. \n

       A handler is called when respective task can be run. Each task has its
       own task handler. \n

       Correspondence between tasks and handlers is listed below:  \n
       RADIO RADIO_TaskHandler() \n
       HAL - HAL_TaskHandler() \n
       LORAWAN - LORAWAN_TaskHandler() \n
       APP - APP_TaskHandler() \n

\param[in] task - ID of the posted task.
*************************************************************************/

void SYSTEM_PostTask(SYSTEM_Task_t task)
{
    ATOMIC_SECTION_ENTER
    sysTaskFlag |= task;
    ATOMIC_SECTION_EXIT
}

/*********************************************************************//**
\brief Returns the readiness of the system for sleep

\return 'true' if the system is ready, 'false' otherwise
*************************************************************************/
bool SYSTEM_ReadyToSleep(void)
{
    return !(sysTaskFlag & 0xffff);
}

/* eof system_task_manager.c */

