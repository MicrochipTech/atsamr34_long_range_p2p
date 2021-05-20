/**
* \file  p2p_demo.h
*
* \brief Demo Application for MiWi P2P Interface
*
* Copyright (c) 2019 Microchip Technology Inc. and its subsidiaries. 
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

#ifndef P2P_DEMO_H
#define	P2P_DEMO_H

/*********************************************************************
* Function: void ReceivedDataIndication (RECEIVED_MESSAGE *ind)
*
* Overview: Process a Received Message
*
* PreCondition: MiApp_ProtocolInit
*
* Input:  None
*
* Output: None
*
********************************************************************/
void ReceivedDataIndication (RECEIVED_MESSAGE *ind);

/*********************************************************************
* Function: void run_p2p_demo(void)
*
* Overview: Starts the p2p demo
*
* PreCondition: System Initialization and Miwi Protocol Init
*
* Input:  None
*
* Output: None
*
********************************************************************/
void run_p2p_demo(void);

/*********************************************************************
* Function: void TxToutCallback(void)
*
* Overview: Transmission timeout callback to bring the node back to continious transaction cycle
*
* PreCondition: MiApp_ProtocolInit
*
* Input:  None
*
* Output: None
*
********************************************************************/
void TxToutCallback(void);

/*********************************************************************
* Function: void dataConfcb(uint8_t handle, miwi_status_t status, uint8_t* msgPointer)
*
* Overview: Callback routine which will be called upon the initiated data procedure is performed
*
* PreCondition: MiApp_ProtocolInit
*
* Input:  None
*
* Output: None
*
********************************************************************/
void dataConfcb(uint8_t handle, miwi_status_t status, uint8_t* msgPointer);

void p2p_demo_unicast_to_parent(void) ;
void p2p_demo_send_data(uint8_t msgType, uint8_t targetAddr, uint8_t *data, uint8_t len) ;

#ifdef PERIODIC_TX
void periodicTxCallback(void) ;
#endif

#ifdef DUTY_CYCLING
void dutyCyclingCallback(void) ;
#endif

#endif	/* P2P_DEMO_H */

