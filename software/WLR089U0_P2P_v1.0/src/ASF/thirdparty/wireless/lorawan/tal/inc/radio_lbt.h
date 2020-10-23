/**
* \file  radio_lbt.h
*
* \brief This is the Radio LBT header file which contains listen before talk specific
*		Radio functions declarations and defines for SX1276
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


/*#ifdef LBT*/

#ifndef RADIO_LBT_H_
#define RADIO_LBT_H_

/******************************************************************************
                   Includes section
******************************************************************************/
#include "system_task_manager.h"
#include "stdint.h"

/******************************************************************************
                   Defines section
******************************************************************************/

/******************************************************************************
                   Prototypes section
******************************************************************************/
/*********************************************************************//**
\brief	This function is triggered by the scan done event and it sets up
		the radio for scanning.

\param 	- none
\return	- returns the success or failure of a task
*************************************************************************/
SYSTEM_TaskStatus_t radioScanDoneHandler(void);

/*********************************************************************//**
\brief	This function is the callback function for LBT scan timer 
        timeout.

\param time - not used.
\return     - none
*************************************************************************/
void Radio_LBTScanTimeout(uint8_t time);

/*#endif LBT*/
#endif /* RADIO_LBT_H_ */