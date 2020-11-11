/********************************************************************
 * Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries
 * (Microchip).  All rights reserved.
 *
 * You are permitted to use the software and its derivatives with Microchip
 * products. See the license agreement accompanying this software, if any, for
 * more info about your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
 * MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP, SMSC, OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH
 * OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY FOR ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES, OR OTHER SIMILAR COSTS. To the fullest
 * extend allowed by law, Microchip and its licensors liability will not exceed
 * the amount of fees, if any, that you paid directly to Microchip to use this
 * software.
 *************************************************************************
 *
 *                           lorawan_reg_params.h
 *
 * LoRaWAN Regional Parameters header file
 *
 ******************************************************************************/

#ifndef _LORAWAN_REG_PARAMS_H
#define	_LORAWAN_REG_PARAMS_H

/******************************MACROS*****************************/

/* Recommended protocol parameters */
/*Note These Parameters are same in all the regulatory defined*/
#define RECEIVE_DELAY1						(1000UL)
#define RECEIVE_DELAY2						2000UL
#define JOIN_ACCEPT_DELAY1					5000UL
#define JOIN_ACCEPT_DELAY2					6000UL
#define ADR_ACK_LIMIT						64
#define ADR_ACK_DELAY						32
#define RETRANSMIT_TIMEOUT							1000+(rand()%2001)
/* Join dutycycle Prescalar for first 1hr*/
#define JOIN_BACKOFF_PRESCALAR_1HR          100
/*Join dutycycle prescalar for 2nd hour from start to 11th hr*/
#define JOIN_BACKOFF_PRESCALAR_10HR         1000
/*join dutycycle prescalar for 12th hr from start to 36th hr*/
#define JOIN_BACKOFF_PRESCALAR_24HR         10000
/* Join backoff timer base time 30 minutes*/
#define BACKOFF_BASE_TIME_IN_MS             1800000
/* Join backoff count For 1 hour is 2 since base timer is 30 minutes*/
#define AGGREGATEDTIME_1HR                  2
#define AGGREGATEDTIME_10HR                 20
#define FCNT_MAX							(UINT32_MAX)
#define SIZE_JOIN_ACCEPT_WITH_CFLIST		33
#define NUMBER_CFLIST_FREQUENCIES			5

#define ALL_CHANNELS						1
#define WITHOUT_DEFAULT_CHANNELS			0

/******************************Type Definitions*****************************/

/*These are the list of attributes used by the Regional Module for GET/SET/Validate*/
typedef enum _LorawanRegionalAttributes
{
	FREQUENCY,
	RX_FREQUENCY,
	TX_FREQUENCY,
	RX_WINDOW_SIZE,
	RX_WINDOW_OFFSET,
	MAX_PAYLOAD_SIZE,
	MAX_CHANNELS,
	MIN_NEW_CH_INDEX,
	DEFAULT_RX1_DATA_RATE,
	DEFAULT_RX2_DATA_RATE,
	DEFAULT_RX2_FREQUENCY,
	DEF_DUTY_CYCLE,
	DEF_DUTY_CYCLE_NEW_CH,
	DEF_DUTY_CYCLE_JOIN_REQUEST,
	SUPPORTED_REGIONAL_FEATURES,
	RX_DATARATE,
	TX_DATARATE,
	SUPPORTED_DR,
	DATA_RANGE,
	DATA_RANGE_CH_BAND,
	MIN_MAX_DR,
	CHANNEL_ID,
	CHANNEL_ID_STATUS,
	RX2_DATARATE,
	RX1_DATARATE_OFFSET,
	RX1_WINDOW_PARAMS,
	CHANNEL_MASK,
	CHANNEL_MASK_CNTL,
	NEW_CHANNELS,
	TX_PWR,
	DUTY_CYCLE,
	DUTY_CYCLE_TIMER,
	JOIN_DUTY_CYCLE_TIMER,
	JOIN_BACK_OFF_TIMER,
	LBT_TIMER,
	MODULATION_ATTR,
	BANDWIDTH_ATTR,
	SPREADING_FACTOR_ATTR,
	MIN_DUTY_CYCLE_TIMER,
	MAC_RECEIVE_DELAY1,
	MAC_RECEIVE_DELAY2,
	MAC_JOIN_ACCEPT_DELAY1,
	MAC_JOIN_ACCEPT_DELAY2,
	MAC_RETRANSMIT_TIMEOUT,
	MAC_ADR_ACK_DELAY,
	MAC_ADR_ACK_LIMIT,
	NEW_TX_CHANNEL_CONFIG,
	FREE_CHANNEL,
	CURRENT_CHANNEL_INDEX,
	DEFAULT_LBT_PARAMS,
	MIN_LBT_CHANNEL_PAUSE_TIMER,
	DL_FREQUENCY,
	TX_PARAMS,
	REG_DEF_TX_POWER,
	REG_DEF_TX_DATARATE,
	CHMASK_CHCNTL,
	JOINBACKOFF_CNTL,
	REG_JOIN_SUCCESS,
	REG_JOIN_ENABLE_ALL,
	CHLIST_DEFAULTS,
	DEF_TX_PWR,
	REG_NUM_ATTRIBUTES	
}LorawanRegionalAttributes_t;

/*
TODO: This should be part of StackRetStatus_t after review.
*/
typedef enum _LORAWAN_ERR_ATTR_e
{
	UNSUPPORTED_BAND = -((CHMASK_CHCNTL) + 2),
    INVLD_ATTR_CHMASK_CHCNTL = -((CHMASK_CHCNTL) + 1),
	INVLD_ATTR_DEF_TX_PWR,
    INVLD_ATTR_REG_DEF_TX_DATARATE,
    INVLD_ATTR_REG_DEF_TX_POWER,
    INVLD_ATTR_TX_PARAMS,
    INVLD_ATTR_DL_FREQUENCY,
    INVLD_ATTR_MIN_LBT_CHANNEL_PAUSE_TIMER,
    INVLD_ATTR_DEFAULT_LBT_PARAMS,
    INVLD_ATTR_CURRENT_CHANNEL_INDEX,
    INVLD_ATTR_FREE_CHANNEL,
    INVLD_ATTR_NEW_TX_CHANNEL_CONFIG,
    INVLD_ATTR_MAC_ADR_ACK_LIMIT,
    INVLD_ATTR_MAC_ADR_ACK_DELAY,
    INVLD_ATTR_MAC_RETRANSMIT_TIMEOUT,
    INVLD_ATTR_MAC_JOIN_ACCEPT_DELAY2,
    INVLD_ATTR_MAC_JOIN_ACCEPT_DELAY1,
    INVLD_ATTR_MAC_RECEIVE_DELAY2,
    INVLD_ATTR_MAC_RECEIVE_DELAY1,
    INVLD_ATTR_MIN_DUTY_CYCLE_TIMER,
    INVLD_ATTR_SPREADING_FACTOR_ATTR,
    INVLD_ATTR_BANDWIDTH_ATTR,
    INVLD_ATTR_MODULATION_ATTR,
    INVLD_ATTR_LBT_TIMER,
    INVLD_ATTR_DUTY_CYCLE_TIMER,
    INVLD_ATTR_DUTY_CYCLE,
    INVLD_ATTR_TX_PWR,
    INVLD_ATTR_NEW_CHANNELS,
    INVLD_ATTR_CHANNEL_MASK_CNTL,
    INVLD_ATTR_CHANNEL_MASK,
    INVLD_ATTR_RX1_WINDOW_PARAMS,
    INVLD_ATTR_RX1_DATARATE_OFFSET,
    INVLD_ATTR_RX2_DATARATE,
    INVLD_ATTR_CHANNEL_ID_STATUS,
    INVLD_ATTR_CHANNEL_ID,
    INVLD_ATTR_MIN_MAX_DR,
    INVLD_ATTR_DATA_RANGE_CH_BAND,
    INVLD_ATTR_DATA_RANGE,
    INVLD_ATTR_SUPPORTED_DR,
    INVLD_ATTR_TX_DATARATE,
    INVLD_ATTR_RX_DATARATE,
    INVLD_ATTR_SUPPORTED_REGIONAL_FEATURES,
    INVLD_ATTR_DEF_DUTY_CYCLE_JOIN_REQUEST,
    INVLD_ATTR_DEF_DUTY_CYCLE_NEW_CH,
    INVLD_ATTR_DEF_DUTY_CYCLE,
    INVLD_ATTR_DEFAULT_RX2_FREQUENCY,
    INVLD_ATTR_DEFAULT_RX2_DATA_RATE,
    INVLD_ATTR_DEFAULT_RX1_DATA_RATE,
    INVLD_ATTR_MIN_NEW_CH_INDEX,
    INVLD_ATTR_MAX_CHANNELS,
    INVLD_ATTR_MAX_PAYLOAD_SIZE,
    INVLD_ATTR_RX_WINDOW_OFFSET,
    INVLD_ATTR_RX_WINDOW_SIZE,
    INVLD_ATTR_TX_FREQUENCY,
    INVLD_ATTR_RX_FREQUENCY,
    INVLD_ATTR_FREQUENCY,
}LorawanErrAttr_t;

/*This structure is used for requesting a new channel/radio configuration for transmission*/
typedef struct
{
	bool transmissionType;
	uint8_t txPwr;
	uint8_t currDr;
}NewTxChannelReq_t;

/*This structure is used for requesting a new free channel for transmission*/
typedef struct
{
	bool transmissionType;
	uint8_t maxChannels;
	uint8_t currDr;
}NewFreeChannelReq_t;

/*This structure is used for getting the data rate which is supported by a certain band
   represented by Channel Mask and Mask control*/
typedef struct
{
	uint8_t chnlMaskCntl;
	uint16_t chnlMask;
}BandDrReq_t;

/*This structure is used forgetting the receive window 1 parameters*/
typedef struct
{
	bool joining;
	uint8_t drOffset;
	uint8_t currDr;
}Rx1WindowParamsReq_t;

/*This receive window 1 parameters are represented by this structure*/
typedef struct
{
	uint32_t rx1Freq;
	uint8_t rx1Dr;
}Rx1WindowParams_t;

/*This structure is used for representing the min and max data rate*/
typedef struct
{
	uint8_t minDr;
	uint8_t maxDr;
}MinMaxDr_t;

/*This structure is used for validating and updating a frequency represented by a channel index*/
typedef struct
{
	uint32_t frequencyNew;
	uint8_t channelIndex;
}ValUpdateFreqTx_t;

/*This structure is used for validating and updating a data range represented by a channel index*/
typedef struct
{
	uint8_t channelIndex;
	uint8_t dataRangeNew;
}ValUpdateDrange_t;

/*This structure is used for Validating the combination of Channel Mask and Mask control*/
typedef struct
{
	uint8_t chnlMaskCntl;
	uint16_t chnlMask;
}ValChMaskCntl_t;

/*This structure is used for validating and updating the dutycycle represented by a channel index*/
typedef struct
{
	uint16_t dutyCycleNew;
	uint8_t channelIndex;
}UpdateDutyCycle_t;

/*This structure is used for  updating a new channel represented by a channel mask and control value*/
typedef struct
{
	uint16_t channelMask;
	uint8_t channelMaskCntl;
}UpdateNewCh_t;

/*This structure is used for  validating the channel index*/
typedef struct
{
	uint8_t channelIndex;
	bool allowedForDefaultChannels;
}ValChId_t;

/*This structure is used for  updating the status of the channel*/
typedef struct
{
	uint8_t channelIndex;
	bool statusNew;
}UpdateChId_t;

/*This structure is used for updating the duty cycle timer with the time on air value*/
typedef struct
{
	uint16_t timeOnAir;
	uint16_t aggDutyCycle;
	bool joining;
}UpdateDutyCycleTimer_t;

typedef struct
{
	uint32_t joinreqTimeonAir;
	bool startJoinDutyCycleTimer;
} UpdateJoinDutyCycleTimer_t;
/*This Structure stores Transmit Params set using TXParamSetupReq*/	
typedef struct _TxParams
{
	uint8_t maxEIRP;	
	uint8_t uplinkDwellTime 	: 1;
	uint8_t downlinkDwellTime   : 1;
	uint8_t reserved 			: 6;	
}TxParams_t;

/******************************PROTOTYPES*****************************/
/**
 * \brief This function initializes the LoRaWAN Regional Parameters Module
 * All the resources handled by the regional module eg Channels will be initialized.
 * \param ismBand The Regional band which is initialized,This is used to validate if
 *	the band is supported by the module and also initializes the module
 *	corresponding to the band
 * \retval LORAWAN_SUCCESS : If initialization is successful
 *	LORAWAN_INVALID_PARAMETER if Initialization is failed
 */
StackRetStatus_t LORAREG_Init(IsmBand_t ismBand);

/**
 * \brief This API provides read access to all the supported Regional Attributes.
 *  The LoRaWAN mac needs to access the regional database via this API.
 * \param[in] attrType Type of Attribute to get from the regional module
 * \param[in] attrInput Input used to get the attribute value
 * \param[out] Result of the GET operation
 * \retval LORAWAN_SUCCESS : If GET is successful
 *	LORAWAN_INVALID_PARAMETER if the input attribute or the attribute type is invalid
 */
StackRetStatus_t LORAREG_GetAttr(LorawanRegionalAttributes_t attrType, void *attrInput, void *attrOutput);

/**
 * \brief This API is used to Update the parameters to the regional parameter data base.
 * \param[in] attrType Type of Attribute to update in the regional module
 * \param[in] attrInput Input used to update the attribute value
 * \retval LORAWAN_SUCCESS : If Update is successful
 *	LORAWAN_INVALID_PARAMETER if the input attribute or the attribute type is invalid
 */
StackRetStatus_t LORAREG_SetAttr(LorawanRegionalAttributes_t attrType, void *attrInput);

/**
 * \brief This API is used to validate if the parameter is according to the
 *  specification defined for that band.
 * \param[in] attrType Type of Attribute to validate in the regional module
 * \param[in] attrInput Input used to validate the attribute value
 * \retval LORAWAN_SUCCESS : If Validate is successful
 *	LORAWAN_INVALID_PARAMETER if the input attribute or the attribute type is invalid
 */
StackRetStatus_t LORAREG_ValidateAttr(LorawanRegionalAttributes_t attrType, void *attrInput);

/**
 * \brief This API is used to un-initialize the regional band parameters and stop the associated timers.
 *	LORAWAN_INVALID_PARAMETER if the input attribute or the attribute type is invalid
 */
StackRetStatus_t LORAREG_UnInit(void);

/**
 * \brief This function returns the supported bands in the LoRaWAN stack ( a compile time feature)
 * \param ismBand The Regional bands supported is updated in this parameter
 * \retval LORAWAN_SUCCESS : If initialization is successful
 *	LORAWAN_INVALID_PARAMETER if Initialization is failed
 */
StackRetStatus_t LORAREG_SupportedBands(uint16_t *bands);
StackRetStatus_t LORAREG_EnableallChannels(IsmBand_t ismBand);
StackRetStatus_t setDefaultTxPower(IsmBand_t ismBand);
#endif	/* _LORAWAN_REG_PARAMS_H */



