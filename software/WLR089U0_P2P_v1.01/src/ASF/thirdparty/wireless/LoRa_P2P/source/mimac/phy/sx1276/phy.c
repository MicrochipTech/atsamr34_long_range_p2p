/**
* \file  phy.c
*
* \brief Physical Layer Abstraction for AT86RF212B implementation
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
/*
* Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
*/

#include <string.h>
#include <stdbool.h>
#include <trx_access_1.h>

#include "miwi_config.h" 
#if defined(PROTOCOL_P2P)
#include "miwi_config_p2p.h"      //MiWi Protocol layer configuration file
#endif

#include "delay.h"
#include "sal.h"
#include "phy.h"

#include "radio_registers_SX1276.h"
#include "radio_driver_hal.h"
#include "radio_interface.h"
#include "power.h"
#include "radio_get_set.h"
#include "radio_driver_SX1276.h"
#include "radio_transaction.h"
#include "stack_common.h"
#include "radio_task_manager.h"	

#include "aes_engine.h"

extern void Radio_WritePower(int8_t power); 


/*- Definitions ------------------------------------------------------------*/
#define PHY_CRC_SIZE    2

/*- Types ------------------------------------------------------------------*/
typedef enum {
	PHY_STATE_INITIAL,
	PHY_STATE_IDLE,
	PHY_STATE_SLEEP,
	PHY_STATE_TX_WAIT_END,
} PhyState_t;

/*- Prototypes -------------------------------------------------------------*/
static void phySetChannel(void);
static void phySetRxState(void);

/*- Variables --------------------------------------------------------------*/
static PhyState_t phyState = PHY_STATE_INITIAL;
static bool phyRxState;
static uint8_t phyBand;
static uint8_t phyChannel;// TODO
static uint8_t phyModulation;
RxBuffer_t RxBuffer[BANK_SIZE];

volatile uint8_t DevPanId[2];
volatile uint8_t DevShortAddress[2];
volatile uint8_t DevIEEEaddr[8];

void radioCallback(RadioCallbackID_t callback, void *param); 
/*************************************************************************//**
*****************************************************************************/
void PHY_Init(void)
{
    /* Initialize the Radio Hardware */
    HAL_RadioInit();
    /* Initialize the AES Hardware Engine */
    AESInit();	
	RADIO_Init();
	RADIO_SetAttr(RADIO_CALLBACK, (void *)&radioCallback);
	
   RADIO_InitDefaultAttributes();				
}


/*************************************************************************//**
*****************************************************************************/
// Set Radio to Receive State
void PHY_SetRxState(bool rx)
{
	phyRxState = rx;
	phySetRxState();
}

/*************************************************************************//**
*****************************************************************************/
// Set Channel
void PHY_SetChannel(uint8_t channel)
{
	phyChannel = channel;
	if (PHY_STATE_SLEEP == phyState)
	{
		PHY_Wakeup();
	}
	phySetChannel();
}

/*************************************************************************//**
*****************************************************************************/
void PHY_SetBand(uint8_t band)
{
	phyBand = band;
	phySetChannel();
}

/*************************************************************************//**
*****************************************************************************/
void PHY_SetModulation(uint8_t modulation)
{
	phyModulation = modulation;
	phySetChannel();
}

/*************************************************************************/


/*****************************************************************************/
void PHY_SetPanId(uint16_t panId)
{
	uint8_t *d = (uint8_t *)&panId;
	DevPanId[0]=d[0];
	DevPanId[1]=d[1];
}

/*************************************************************************//**
*****************************************************************************/


// Set Short Address
void PHY_SetShortAddr(uint16_t addr)
{
	uint8_t *d = (uint8_t *)&addr;
	DevShortAddress[0]=d[0];
	DevShortAddress[1]=d[1];
}

/*************************************************************************//**
*****************************************************************************/

void PHY_SetTxPower(uint8_t txPower)
{
	uint8_t reg;
	uint8_t PaBoost ;
	reg = txPower;

	if(reg > 15)
	{
		PaBoost = ENABLED;
		RADIO_SetAttr(PABOOST, (void *)&PaBoost);
	}
	else
	{
		PaBoost = DISABLED;
		RADIO_SetAttr(PABOOST, (void *)&PaBoost);
		reg &= 0x0F ;
	}
	RADIO_SetAttr(OUTPUT_POWER, &reg);
}

RadioError_t PHY_SetAttribute(RadioAttribute_t RadioAttr,uint8_t* AttrVal)
{
	RadioError_t Status;
	switch(RadioAttr)
	{
		case SPREADING_FACTOR:
		case BANDWIDTH:
			Status = RADIO_SetAttr(RadioAttr,AttrVal);
		break;	
		default:
			// Do nothing
			Status = ERR_INVALID_REQ;
		break;
	}
	return Status;
}

RadioError_t PHY_GetAttribute(RadioAttribute_t RadioAttr,uint8_t* AttrVal)
{
	RadioError_t Status;
	switch(RadioAttr)
	{
		case SPREADING_FACTOR:
		case BANDWIDTH:
			Status = RADIO_GetAttr(RadioAttr,AttrVal);
			break;
		default:
			// Do nothing
			Status = ERR_INVALID_REQ;
		break;
	}
	return Status;
}
/*************************************************************************//**
*****************************************************************************/
// Radio Sleep
void PHY_Sleep(void)
{
	if (PHY_STATE_SLEEP != phyState)
	{
		Radio_WriteMode(MODE_SLEEP, radioConfiguration.modulation, 0);
		HAL_TCXOPowerOff();
		/* Disable the SPI interface */
		HAL_RadioDeInit();
		phyState = PHY_STATE_SLEEP;	
	}
}

/*************************************************************************//**
*****************************************************************************/
// Radio Wake Up
void PHY_Wakeup(void)
{
	if (PHY_STATE_SLEEP == phyState)
	{
		HAL_TCXOPowerOn();
		/* Initialize the Radio Hardware */
		HAL_RadioInit();
		/* Reset the radio */
		RADIO_Reset();
		Radio_WriteMode(MODE_STANDBY, radioConfiguration.modulation, 0);
	 	phySetRxState();
	 	phyState = PHY_STATE_IDLE;
	}
}

/*************************************************************************//**
*****************************************************************************/
extern uint8_t AckReqData;
extern MIWI_TICK DataTxAckTimer;
volatile RadioCallbackID_t callbackBackup;
void radioCallback(RadioCallbackID_t callback, void *param)
{
	uint8_t i,RxBank=0xFF;
	RadioCallbackParam_t * callbackRx;
	callbackBackup = callback;
	RadioReceiveParam_t RadioReceiveParam;	
	callbackRx = param;
	int16_t rssi_readVal=0;
	Radio_ReadRssi(&rssi_readVal);
	switch (callback)
	{
		case RADIO_TX_DONE_CALLBACK:
		case RADIO_TX_TIMEOUT_CALLBACK:
			if((!AckReqData)||(callbackRx->status != ERR_NONE))
				PHY_DataConf(callbackRx->status); // Need to convert LoRa status to MiWi status
			else
				DataTxAckTimer.Val = MiWi_TickGet();

			RadioReceiveParam.action = RECEIVE_START;
			RadioReceiveParam.rxWindowSize = 0;
			RADIO_Receive(&RadioReceiveParam);
		break;

		case RADIO_RX_DONE_CALLBACK:
		case RADIO_RX_TIMEOUT_CALLBACK:
				for (i = 0; i < BANK_SIZE; i++)
				{
					if (RxBuffer[i].PayloadLen == 0)
					{
						RxBank = i;
						break;
					}
				}
					callbackRx = (RadioCallbackParam_t *)param;

						
				RxBuffer[RxBank].PayloadLen = callbackRx->RX.bufferLength;
				if (RxBuffer[RxBank].PayloadLen < (RX_PACKET_SIZE - 4))			
				{

					//copy all of the data from the FIFO into the TxBuffer, plus RSSI and LQI
					//TODO Where are we copying rssi and lqi?
					for (i = 0; i <= callbackRx->RX.bufferLength; i++)
					{
						RxBuffer[RxBank].Payload[i] = callbackRx->RX.buffer[i];
					}
				}
				RxBuffer[RxBank].PayloadLen +=4;
				rssi_readVal = radioConfiguration.packetRSSI;
				RxBuffer[RxBank].radioRSSI = rssi_readVal;						
				//If Ack request bit in FCF is set send Ack or Make Radio Receive
				if(RxBuffer[RxBank].Payload[0] & 0x20)
				{
					// Create Ack Packet and send Ack using PHY_DataReq
					RadioReceiveParam.action = RECEIVE_STOP;					
					RadioReceiveParam.rxWindowSize = 0;
					RADIO_Receive(&RadioReceiveParam);
				}
				else
				{
					RadioReceiveParam.action = RECEIVE_START;
					RadioReceiveParam.rxWindowSize = 0;
					RADIO_Receive(&RadioReceiveParam);	
				}
		break;
		case RADIO_RX_ERROR_CALLBACK:
				RadioReceiveParam.action = RECEIVE_START;
				RadioReceiveParam.rxWindowSize = 0;
				PHY_DataConf(FAILURE);											
				RADIO_Receive(&RadioReceiveParam);
				break;
		case RADIO_FHSS_NEXT_FREQ_CALLBACK:
		/*TODO : Add api to register function*/
		//if(fhssCallback)
		//{
			//*(uint32_t *)param = fhssCallback();
		//}
		break;

		default:
		break;
	}
}

uint8_t DataRequestBuffer[255];       
	
uint8_t PHY_DataReq(uint8_t *data)		
{
		RadioTransmitParam_t RadioTransmitParam;	
		RadioError_t status;
		uint8_t count = 0;
		RadioTransmitParam.bufferLen = data[0];
		for (count = 0; count < data[0]; count++)
		{
			DataRequestBuffer[count] = data[1+count];
		}
		RadioTransmitParam.bufferPtr = (uint8_t *)&DataRequestBuffer;
		
		RadioSetState(RADIO_STATE_IDLE);
		phySetChannel();

		RadioLBTParams_t localRadioLBTParams;
		localRadioLBTParams.lbtScanPeriod = 15;
		localRadioLBTParams.lbtThreshold = 255;
		localRadioLBTParams.lbtNumOfSamples = 5;
		localRadioLBTParams.lbtTransmitOn = 1;
		status = RADIO_SetAttr(RADIO_LBT_PARAMS, &localRadioLBTParams);
		status = RADIO_Transmit (&RadioTransmitParam);
		
		return (!status);							
}

/*************************************************************************//**
*****************************************************************************/
uint16_t PHY_RandomReq(void)
{
	uint16_t rnd = 0;

	rnd = RADIO_ReadRandom();
	phySetRxState();

	return rnd;
}

/*************************************************************************//**
*****************************************************************************/
void PHY_EncryptReq(uint8_t *text, uint8_t *key)
{	
	AESEncode(text,key);
}

void PHY_EncryptReqCBC(uint8_t *text, uint8_t *key)
{	
	AESEncode(text,key);	
}

/*************************************************************************//**
*****************************************************************************/
// Decrypt Block
void PHY_DecryptReq(uint8_t *text, uint8_t *key)
{
	AESEncode(text,key);
}


/*************************************************************************//**
*****************************************************************************/
uint8_t PHY_EdReq(void) //TODO datatype int8 or uint8
{
	int16_t rssi_value;
	
	Radio_ReadFSKRssi(&rssi_value);
	phySetRxState();
	
	return ((uint8_t)rssi_value);
}

/*************************************************************************//**
*****************************************************************************/
static void phySetChannel(void)
{
	uint32_t frequncy_set;
	if(phyChannel) // Based on channel we need to set frequency 
	{
		switch(phyChannel)
		{
			//case 0:
				//frequncy_set = FREQ_200KHZ;
				//break;
			//case 1:
				//frequncy_set = FREQ_500KHZ;
				//break;			
			//case 2:
				//frequncy_set = FREQ_600KHZ;
				//break;
			//case 3:
				//frequncy_set = FREQ_1600KHZ;
				//break;
			//case 0:
				//frequncy_set = FREQ_137000KHZ;
				//break;
			//case 1:
				//frequncy_set = FREQ_175000KHZ;
				//break;
			//case 2:
				//frequncy_set = FREQ_410000KHZ;
				//break;
			//case 3:
				//frequncy_set = FREQ_433050KHZ;
				//break;
			//case 4:
				//frequncy_set = FREQ_433300KHZ;
				//break;
			//case 5:
				//frequncy_set = FREQ_434665KHZ;
				//break;
			//case 6:
				//frequncy_set = FREQ_434790KHZ;
				//break;
			//case 7:
				//frequncy_set = FREQ_525000KHZ;
				//break;
			//case 8:
				//frequncy_set = FREQ_786000KHZ;
				//break;
			case 0:
				frequncy_set = FREQ_862000KHZ;
				break;
			case 1:
				frequncy_set = FREQ_863000KHZ;
				break;
			case 2:
				frequncy_set = FREQ_865000KHZ;
				break;
			case 3:
				frequncy_set = FREQ_866550KHZ;
				break;
			case 4:
				frequncy_set = FREQ_867000KHZ;
				break;			
			case 5:
				frequncy_set = FREQ_868100KHZ;
				break;
			case 6:
				frequncy_set = FREQ_869525KHZ;
				break;			
			case 7:
				frequncy_set = FREQ_870000KHZ;
				break;
			case 8:
				frequncy_set = FREQ_902300KHZ;
				break;
			case 9:
				frequncy_set = FREQ_903000KHZ;
				break;
			case 10:
				frequncy_set = FREQ_915000KHZ;
				break;
			case 11:
				frequncy_set = FREQ_915200KHZ;
				break;
			case 12:
				frequncy_set = FREQ_915900KHZ;
				break;
			case 13:
				frequncy_set = FREQ_920000KHZ;
				break;
			case 14:
				frequncy_set = FREQ_920900KHZ;
				break;
			case 15:
				frequncy_set = FREQ_921900KHZ;
				break;
			case 16:
				frequncy_set = FREQ_922000KHZ;
				break;
			case 17:
				frequncy_set = FREQ_922100KHZ;
				break;
			case 18:
				frequncy_set = FREQ_923000KHZ;
				break;
			case 19:
				frequncy_set = FREQ_923200KHZ;
				break;
			case 20:
				frequncy_set = FREQ_923300KHZ;
				break;
			case 21:
				frequncy_set = FREQ_925000KHZ;
				break;		
				
			case 22:
				frequncy_set = FREQ_927500KHZ;
				break;
			case 23:
				frequncy_set = FREQ_928000KHZ;
				break;
			case 24:
				frequncy_set = FREQ_928500KHZ;
				break;
			case 25:
				frequncy_set = FREQ_1020000KHZ;
				break;
			default:
				frequncy_set = FREQ_868100KHZ;
				break;				
																									
		}
	}
		
	if(RADIO_SetAttr(CHANNEL_FREQUENCY,(void *)&frequncy_set) == 0)  
	{
	}
}

/*************************************************************************//**
*****************************************************************************/
static void phySetRxState(void)
{
	RADIO_RxHandler();
}

/*************************************************************************//**
*****************************************************************************/
void PHY_SetIEEEAddr(uint8_t *ieee_addr)
{
	uint8_t *ptr_to_reg = ieee_addr;

	for (uint8_t i = 0; i < 8; i++)
	{
		DevIEEEaddr[i] = ptr_to_reg[i];
	}
}


/*************************************************************************//**
*****************************************************************************/
// Handle Packet Received
void PHY_TaskHandler(void)
{
    if (PHY_STATE_SLEEP == phyState)
    {
        return;
    }
	// Handled in DIO interrupts 
}
