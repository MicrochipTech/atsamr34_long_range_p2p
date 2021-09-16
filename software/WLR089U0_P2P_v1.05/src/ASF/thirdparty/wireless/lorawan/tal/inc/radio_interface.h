/**
* \file  radio_interface.h
*
* \brief This is the Radio Interface header file which contains 
*        LoRa-specific Radio interface functions declarations and defines
*
* Copyright (c) 2018-2020 Microchip Technology Inc. and its subsidiaries. 
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


#ifndef RADIO_INTERFACE_H
#define	RADIO_INTERFACE_H

#ifdef	__cplusplus
extern "C" {
#endif

/******************************************************************************
                   Includes section
******************************************************************************/
#include <stdint.h>
#include "stdbool.h"

/******************************************************************************
                   Defines section
******************************************************************************/
#define TIME_ON_AIR_LOAD_VALUE              ((uint32_t)20000)
#define RADIO_WATCHDOG_TIMEOUT              ((uint32_t)15000)
#define RADIO_PHY_PREAMBLE_LENGTH           (8u)
#define RADIO_PHY_FSK_PREAMBLE_BYTES_LENGTH (5u)
#define RADIO_OPMODE_MASK					(0x07)
#define RADIO_RX_DONE_CALLBACK_MASK         (1 << 0)
#define RADIO_TX_DONE_CALLBACK_MASK         (1 << 1)
#define RADIO_TX_TIMEOUT_CALLBACK_MASK      (1 << 2)
#define RADIO_RX_TIMEOUT_CALLBACK_MASK      (1 << 3)
#define RADIO_RX_ERROR_CALLBACK_MASK        (1 << 4)
#define RADIO_FHSS_NEXT_FREQ_CALLBACK_MASK  (1 << 5)
#define RADIO_DEFAULT_CALLBACK_MASK         RADIO_RX_DONE_CALLBACK_MASK		| \
                                            RADIO_TX_DONE_CALLBACK_MASK		| \
                                            RADIO_TX_TIMEOUT_CALLBACK_MASK	| \
                                            RADIO_RX_TIMEOUT_CALLBACK_MASK	| \
                                            RADIO_RX_ERROR_CALLBACK_MASK

/************************************************************************/
/* Types                                                                */
/************************************************************************/
/*********************************************************************//**
\brief	Possible callbacks that can registered to by the upper layer.
*************************************************************************/
typedef enum _RadioCallbackID_t
{
    RADIO_RX_DONE_CALLBACK          = (1 << 0),
    RADIO_TX_DONE_CALLBACK          = (1 << 1),
    RADIO_TX_TIMEOUT_CALLBACK       = (1 << 2),
    RADIO_RX_TIMEOUT_CALLBACK       = (1 << 3),
    RADIO_RX_ERROR_CALLBACK         = (1 << 4),
    RADIO_FHSS_NEXT_FREQ_CALLBACK   = (1 << 5)
} RadioCallbackID_t;

/*********************************************************************//**
\brief	The radio callback typedef used to declare callback functions
		from the radio layer.
*************************************************************************/
typedef void (*RadioCallback_t)(RadioCallbackID_t callback, void *param);

/*********************************************************************//**
\brief	Possible Radio states.
*************************************************************************/
typedef enum _RadioState_t
{
    RADIO_STATE_SLEEP = 0,
    RADIO_STATE_IDLE,
    RADIO_STATE_TX,
	RADIO_STATE_SCAN,
    RADIO_STATE_RX,
    RADIO_STATE_RX_BUSY,
    RADIO_STATE_CAD
} RadioState_t;

/*********************************************************************//**
\brief	Possible modulations supported.
*************************************************************************/
typedef enum _RadioModulation_t
{
    MODULATION_FSK = 0,
    MODULATION_LORA
} RadioModulation_t;

/*********************************************************************//**
\brief	Possible error coding rate supported.
*************************************************************************/
typedef enum _RadioErrorCodingRate_t
{
    CR_4_5 = 1,
    CR_4_6,
    CR_4_7,
    CR_4_8
} RadioErrorCodingRate_t;

/*********************************************************************//**
\brief	Possible spread factors supported.
*************************************************************************/
typedef enum _RadioDataRate_t
{
    SF_7 = 7,
    SF_8,
    SF_9,
    SF_10,
    SF_11,
    SF_12
} RadioDataRate_t;

/*********************************************************************//**
\brief	Possible LoRa bandWidth supported.
*************************************************************************/
typedef enum _RadioLoRaBandWidth_t
{
    BW_125KHZ = 7,
    BW_250KHZ,
    BW_500KHZ,
	BW_UNDEFINED
} RadioLoRaBandWidth_t;

/*********************************************************************//**
\brief	Possible FSK shaping supported.
*************************************************************************/
typedef enum _RadioFSKShaping_t
{
    FSK_SHAPING_NONE            = 0,
    FSK_SHAPING_GAUSS_BT_1_0,
    FSK_SHAPING_GAUSS_BT_0_5,
    FSK_SHAPING_GAUSS_BT_0_3
} RadioFSKShaping_t;

/*********************************************************************//**
\brief	Possible FSK bandWidth supported.
*************************************************************************/
typedef enum _RadioFSKBandWidth_t
{
    FSKBW_250_0KHZ              = 1,
    FSKBW_125_0KHZ,
    FSKBW_62_5KHZ,
    FSKBW_31_3KHZ,
    FSKBW_15_6KHZ,
    FSKBW_7_8KHZ,
    FSKBW_3_9KHZ,
    FSKBW_INVALID_PADDING_0,
    FSKBW_200_0KHZ,
    FSKBW_100_0KHZ,
    FSKBW_50_0KHZ,
    FSKBW_25_0KHZ,
    FSKBW_12_5KHZ,
    FSKBW_6_3KHZ,
    FSKBW_3_1KHZ,
    FSKBW_INVALID_PADDING_1,
    FSKBW_166_7KHZ,
    FSKBW_83_3KHZ,
    FSKBW_41_7KHZ,
    FSKBW_20_8KHZ,
    FSKBW_10_4KHZ,
    FSKBW_5_2KHZ,
    FSKBW_2_6KHZ
} RadioFSKBandWidth_t;

/*********************************************************************//**
\brief	Possible error states returned by the radio.
*************************************************************************/
typedef enum _RadioError_t
{
	/* Success */
	ERR_NONE              = 0,
	/* Errors */
	ERR_NO_DATA,
    ERR_DATA_SIZE,
    ERR_INVALID_REQ,
    ERR_RADIO_BUSY,
    ERR_OUT_OF_RANGE,
    ERR_UNSUPPORTED_ATTR,
	ERR_CHANNEL_BUSY
} RadioError_t;

/*********************************************************************//**
\brief	A list of all the attributes.
*************************************************************************/
typedef enum _RadioAttribute_t
{
    LORA_SYNC_WORD,
    CHANNEL_FREQUENCY,
    CHANNEL_FREQUENCY_DEVIATION,
    PREAMBLE_LEN,
    OUTPUT_POWER,
    CRC,
    IQINVERTED,
    BANDWIDTH,
    PABOOST,
    MODULATION,
    FREQUENCY_HOP_PERIOD,
    ERROR_CODING_RATE,
    WATCHDOG_TIMEOUT,
    FSK_BIT_RATE,
    PACKET_SNR,
    FSK_DATA_SHAPING,
    FSK_RX_BW,
    FSK_AFC_BW,
    FSK_SYNC_WORD,
    FSK_SYNC_WORD_LEN,
    CRC_ON,
    MAX_POWER,
    SPREADING_FACTOR,
    GET_RANDOM,
    GET_MAX_POWER,
    SYMBOL_TIMEOUT,
    RADIO_CALLBACK,
    MODE_MODULATION,
    MAX_RADIO_ATTRIBUTES,
	RADIO_LBT_PARAMS,
	RADIO_CLOCK_STABLE_DELAY,
	PACKET_RSSI_VALUE
} RadioAttribute_t;

/*********************************************************************//**
\brief	Attributes for starting and stopping the receive operation
*************************************************************************/
typedef enum _RadioReceive_t
{
    RECEIVE_START,
    RECEIVE_STOP
} RadioReceive_t;

/*********************************************************************//**
\brief	A structure for storing the receive parameters.
*************************************************************************/
typedef struct _RadioReceiveParam_t
{
    RadioReceive_t action;
    uint16_t rxWindowSize;
} RadioReceiveParam_t;

/*********************************************************************//**
\brief	A structure for storing the transmit parameters.
*************************************************************************/
typedef struct _RadioTransmitParam_t
{
    uint8_t bufferLen;
    uint8_t *bufferPtr;
} RadioTransmitParam_t;

/*********************************************************************//**
\brief	A structure for storing the callback parameters.
*************************************************************************/
typedef struct _RadioCallbackParam_t
{
	RadioError_t status;
	union
	{
		struct _RX
		{
			uint8_t *buffer;
			uint8_t bufferLength;
		} RX;
		struct _TX
		{

			uint32_t timeOnAir;
		} TX;
		struct _FHSS
		{
			uint32_t frequency;
		} FHSS;
	};
} RadioCallbackParam_t;

/*
 * These enumerations need to be set according to the definition in the
 * radio data sheet
 */
typedef enum _RadioMode_t
{
    MODE_SLEEP = 0,
    MODE_STANDBY,
    MODE_FSTX,
    MODE_TX,
    MODE_FSRX,
    MODE_RXCONT,
    MODE_RXSINGLE,
    MODE_CAD
} RadioMode_t;

/*********************************************************************//**
\brief	A structure for storing the modes and modulations supported
		as parameters for get and set.
*************************************************************************/
typedef struct _RadioModeModulation_t
{
    RadioMode_t mode;
    RadioModulation_t modulation;
    uint8_t blocking;
} RadioModeModulation_t;

/*#ifdef LBT*/
/*********************************************************************//**
\brief	A structure for storing the Listen Before Talk parameters
		for get and set.
*************************************************************************/
typedef struct _RadioLBTParams_t
{
    uint16_t lbtScanPeriod;
	int16_t lbtThreshold;
	uint8_t lbtNumOfSamples;
	bool lbtTransmitOn;
} RadioLBTParams_t;

/*********************************************************************//**
\brief	A structure for storing all Listen Before Talk parameters.
*************************************************************************/
typedef struct _RadioLBT_t
{
	RadioLBTParams_t params;
	int32_t	lbtRssiSamples;
	uint32_t lbtScanTimeout;
	int16_t lbtChannelRSSI;
	uint8_t	lbtIrqFlagsBackup;
	uint8_t	lbtRssiSamplesCount;
	uint8_t lbtScanTimerId;
} RadioLBT_t;
/*#endif*/ // LBT

/*********************************************************************//**
\brief	A structure for storing all the attributes of the radio
		and the transceiver.
*************************************************************************/
typedef struct _RadioConfiguration_t
{
	uint32_t frequency;
    uint32_t frequencyDeviation;
    uint32_t bitRate;
	uint32_t watchdogTimerTimeout;
	RadioCallback_t radioCallback;
    uint16_t preambleLen;
	uint16_t frequencyHopPeriod;
    uint8_t syncWordLoRa;
    uint8_t syncWord[8];
    uint8_t syncWordLen;
    int8_t outputPower;
    uint8_t crcOn;
    uint8_t paBoost;
    uint8_t iqInverted;
    uint8_t implicitHeaderMode;
	uint8_t *dataBuffer;
    uint8_t volatile dataBufferLen;
    uint8_t timeOnAirTimerId;
    uint8_t fskRxWindowTimerId;
    uint8_t watchdogTimerId;
    uint8_t initialized;
    uint8_t regVersion;
    int8_t packetSNR;
	RadioErrorCodingRate_t errorCodingRate;
	RadioModulation_t modulation;
    RadioDataRate_t dataRate;
    RadioLoRaBandWidth_t bandWidth;
    RadioFSKShaping_t fskDataShaping;
    RadioFSKBandWidth_t rxBw;
    RadioFSKBandWidth_t afcBw;
	RadioLBT_t lbt;
	uint8_t clockStabilizationDelay;
	uint8_t clockSource;
	int16_t packetRSSI;
	uint8_t volatile fskPayloadIndex;
} RadioConfiguration_t;

/************************************************************************/
/* Extern variables                                                     */
/************************************************************************/

/************************************************************************/
/* Prototypes                                                           */
/************************************************************************/

/*********************************************************************//**
\brief This function gets the radio state.

\return - The current state of the radio.
*************************************************************************/
RadioState_t RADIO_GetState(void);

/*********************************************************************//**
\brief This function gets the radio attribute.

\param attribute - The attribute to get.
\param value     - The pointer that is used to get the value.
\return          - The error condition for getting a given attribute.
*************************************************************************/
RadioError_t RADIO_GetAttr(RadioAttribute_t attribute, void *value);

/*********************************************************************//**
\brief This function sets the radio attribute.

\param attribute - The attribute to set.
\param value     - The pointer that is used to set the value.
\return          - The error condition for setting a given attribute.
*************************************************************************/
RadioError_t RADIO_SetAttr(RadioAttribute_t attribute, void *value);

/*********************************************************************//**
\brief The Radio Init initializes the transceiver
*************************************************************************/
void RADIO_Init(void);

/*********************************************************************//**
\brief	This function transmits the data by doing a task post to the
		RADIO_TxHandler.

\param param 	- Stores the transmission parameters.
\return			- The error condition for the transmit operation.
*************************************************************************/
RadioError_t RADIO_Transmit(RadioTransmitParam_t *param);



/*********************************************************************//**
\brief	This function receives the data and stores it in the buffer
		pointer space by doing a task post to the RADIO_RxHandler.

\param param 	- Stores the receive parameters.
\return			- The error condition for the receive operation.
*************************************************************************/
RadioError_t RADIO_Receive(RadioReceiveParam_t *param);

/*********************************************************************//**
\brief	The function transmits a CW and is supported only in LORA mode.

\return	- returns the status of the operation
*************************************************************************/
RadioError_t RADIO_TransmitCW(void);

/*********************************************************************//**
\brief	The function stops the transmission of a CW.

\return	- returns the status of the operation
*************************************************************************/
RadioError_t RADIO_StopCW(void);

/*********************************************************************//**
\brief	The function enables the callbacks by setting masks.

Setting a bit to 1 means that the callback is enabled
Setting a bit to 0 means that the callback is disabled

\param bitmask - The bitmask for enabling and disabling callbacks.
*************************************************************************/
void RADIO_SetCallbackBitmask(uint8_t bitmask);

/*********************************************************************//**
\brief	The function disables the callbacks by setting masks.

Setting a bit to 1 means that the callback is enabled
Setting a bit to 0 means that the callback is disabled

\param bitmask - The bitmask for disabling callbacks.
*************************************************************************/
void RADIO_ClearCallbackBitmask(uint8_t bitmask);

/*********************************************************************//**
\brief	The function enables or disables the callbacks by setting masks.

Setting a bit to 1 means that the callback is enabled
Setting a bit to 0 means that the callback is disabled

\return	- Returns the bit mask that is stored.
*************************************************************************/
uint8_t RADIO_GetCallbackBitmask(void);

/*********************************************************************//**
\brief	The function initializes the default attributes depending on
		region chosen.
*************************************************************************/
void RADIO_InitDefaultAttributes(void);

/*********************************************************************//**
\brief	This function is called when a hardware radio interrupt DIO0
		occurs.
*************************************************************************/
void RADIO_DIO0(void);

/*********************************************************************//**
\brief	This function is called when a hardware radio interrupt DIO1
		occurs.
*************************************************************************/
void RADIO_DIO1(void);

/*********************************************************************//**
\brief	This function is called when a hardware radio interrupt DIO2
		occurs.
*************************************************************************/
void RADIO_DIO2(void);

/*********************************************************************//**
\brief	This function is called when a hardware radio interrupt DIO3
		occurs.
*************************************************************************/
void RADIO_DIO3(void);

/*********************************************************************//**
\brief	This function is called when a hardware radio interrupt DIO4
		occurs.
*************************************************************************/
void RADIO_DIO4(void);

/*********************************************************************//**
\brief	This function is called when a hardware radio interrupt DIO5
		occurs.
*************************************************************************/
void RADIO_DIO5(void);

/*********************************************************************//**
\brief	This function sets the receive frequency of the transceiver
		while hopping in FHSS.

\param		- none
\return		- none.
*************************************************************************/
void RADIO_FHSSChangeChannel(void);

/*********************************************************************//**
\brief	This function handles the rogue radio interrupts.

\param modulation	- modulation when this function is called.
\return				- none.
*************************************************************************/
void RADIO_UnhandledInterrupt(RadioModulation_t modulation);

/*********************************************************************//**
\brief	This function is triggered by hardware interrupt when a LoRa
        is transmitted.

\param   - none
\return  - none
*************************************************************************/
void RADIO_TxDone(void);

/*********************************************************************//**
\brief	This function is triggered by hardware interrupt when a LoRa
        is received.

\param   - none
\return  - none
*************************************************************************/
void RADIO_RxDone(void);

/*********************************************************************//**
\brief	This function is triggered by hardware interrupt when a FSK
        is received.

\param   - none
\return  - none
*************************************************************************/
void RADIO_FSKPayloadReady(void);

/*********************************************************************//**
\brief	This function is triggered by hardware interrupt when a FSK
        is transmitted.

\param   - none
\return  - none
*************************************************************************/
void RADIO_FSKPacketSent(void);

/*********************************************************************//**
\brief	This function is triggered when the FIFO is above the 
		threshold value. This is triggered by a hardware interrupt. 

\param  - none
\return - none
*************************************************************************/
void RADIO_FSKFifoLevel(void);

/*********************************************************************//**
\brief	This function is called when a sync address match happens or 
		when packet collision happens. In that case we clear the 
		FSKPayload index and datalen variables to download the new packet.  

\param		- none	
\return		- none.
*************************************************************************/
void RADIO_FSKSyncAddr(void);

/*********************************************************************//**
\brief	This function is triggered when the FIFO is empty.
        This is triggered by a hardware interrupt. 

\param  - none
\return - none
*************************************************************************/
void RADIO_FSKFifoEmpty(void);

/*********************************************************************//**
\brief	This function is triggered when no packets are received within
        a specified Rx window. This is triggered by a hardware interrupt.

\param  - none
\return - none
*************************************************************************/
void RADIO_RxTimeout(void);

/*********************************************************************//**
\brief	This function generates random number using the transceiver
		and returns it.

\param		- none
\return		- returns the random number generated.
*************************************************************************/
uint16_t RADIO_ReadRandom(void);

/*********************************************************************//**
\brief	This function can be called by MAC to read radio buffer pointer
		and length of a receive frame(typically in rxdone)

\param   - place holder for data pointer, length
\return  - ERR_NONE. Other types are not used now.
*************************************************************************/
RadioError_t RADIO_GetData(uint8_t **data, uint16_t *dataLen);

#ifdef	__cplusplus
}
#endif

#endif	/* RADIO_INTERFACE_H */

/**
 End of File
*/
