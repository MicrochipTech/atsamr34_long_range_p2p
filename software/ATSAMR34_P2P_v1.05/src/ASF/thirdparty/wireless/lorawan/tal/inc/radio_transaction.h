/**
* \file  radio_transaction.h
*
* \brief This is the radio_transaction header file which contains Transmit,
*        Receive and Interrupt handling interfaces.
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


#ifndef _RADIO_TRANSACTION_H
#define _RADIO_TRANSACTION_H

 
/************************************************************************/
/*  Includes                                                            */
/************************************************************************/
#include "radio_interface.h"
#include <stdint.h>
#include <stddef.h>

/************************************************************************/
/*  Defines                                                            */
/************************************************************************/
#define RADIO_LORA_BUFFER_SPACE		271u  
#define RADIO_FSK_BUFFER_SPACE		64u
#define RADIO_BUFFER_SIZE			RADIO_LORA_BUFFER_SPACE

#ifdef UT
#define DEF_UT_TIMEONAIR            20u /* unit: milliseconds */
#endif

#define RADIO_RFCTRL_RX				(0u)
#define RADIO_RFCTRL_TX             (1u)

// FIFO Rx FIFO Threshold set to 32 bytes
#define RADIO_RX_FIFO_LEVEL			(63u)
// FIFO Tx FIFO Threshold set to 20 bytes
#define RADIO_TX_FIFO_LEVEL			(63u)

#define NON_BLOCKING_REQ			0
#define BLOCKING_REQ				1

/************************************************************************/
/*  Global variables                                                    */
/************************************************************************/

/*********************************************************************//**
\brief	Possible events from DIO's and timers
*************************************************************************/
typedef struct _RadioEvents_t
{
    uint16_t LoraTxDoneEvent : 1;
    uint16_t LoraRxDoneEvent : 1;
    uint16_t FskTxDoneEvent : 1;
    uint16_t FskRxDoneEvent : 1;
    uint16_t TxWatchdogTimoutEvent : 1;
    uint16_t RxWatchdogTimoutEvent : 1;
    uint16_t LoraRxTimoutEvent : 1;
    uint16_t FskRxTimoutEvent : 1;
    uint16_t RxError : 1;
	uint16_t LbtScanDoneEvent : 1;
	uint16_t reserved : 6;
} RadioEvents_t;

/*********************************************************************//**
\brief	Possible callback events registration
*************************************************************************/
typedef union _RadioCallbackMask_t
{
    struct _BitMask_
    {
        uint8_t radioRxDoneCallback : 1;
        uint8_t radioTxDoneCallback : 1;
        uint8_t radioTxTimeoutCallback : 1;
        uint8_t radioRxTimeoutCallback : 1;
        uint8_t radioRxErrorCallback : 1;
        uint8_t radioFhssfreqCallback : 1;
		uint8_t reserved : 2;
    } BitMask;
    uint8_t callbackMask;
} RadioCallbackMask_t;

/************************************************************************/
/*  Prototypes                                                    */
/************************************************************************/
/*********************************************************************//**
\brief	This function is the callback function for watchdog timer 
        timeout.

\param time - The watchdog timeout time.
\return     - none
*************************************************************************/
void Radio_WatchdogTimeout(uint8_t time);

/*********************************************************************//**
\brief	This function is triggered by a timer timeout in FSK mode to save
        Rx power. 

\param time - The time for which the timeout was started.
\return     - none
*************************************************************************/
void Radio_RxFSKTimeout(uint8_t time);

/*********************************************************************//**
\brief This function sets the radio state.

\param state - The state of the radio to be set to.
*************************************************************************/
void RadioSetState(RadioState_t state);

/*********************************************************************//**
\brief This function is to Set RF front End Control.

\param type - RX = false/0, TX = true/1.
*************************************************************************/
void Radio_EnableRfControl(bool type);
/*********************************************************************//**
\brief This function is to Reset RF front End Control.

\param type - RX = false/0, TX = true/1.
*************************************************************************/
void Radio_DisableRfControl(bool type);
/*********************************************************************//**
\brief	This function sets the clock input to Radio. 
        It will inform the radio from which clock it is sourced

\param   - None
\return  - None.
*************************************************************************/
void Radio_SetClockInput(void);

/*********************************************************************//**
\brief	This function power off/Resets the clock source of Radio

\param   - None
\return  - None.
*************************************************************************/
void Radio_ResetClockInput(void);

/*********************************************************************//**
\brief	This function handles the payload transfer of bytes from buffer
		to FIFO. 

\param[in] buffer Pointer to the data to be written into the frame buffer
\param[in] bufferLen Length of the data to be written
\return     - none
*************************************************************************/
void Radio_FSKTxPayloadHandler(uint8_t *buffer, uint8_t bufferLen);



#endif  /*_RADIO_TRANSACTION_H*/

// eof radio_transaction.h
