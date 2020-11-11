/**
* \file  lorawan_multiband.c
*
* \brief LoRaWAN Multiband file
*		
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


/****************************** INCLUDES **************************************/

#include "lorawan.h"
#include "conf_regparams.h"
#include <math.h>
#include "radio_interface.h"
#include "sw_timer.h"
#include "conf_stack.h"
#include "lorawan_reg_params.h"
#include "lorawan_multiband.h"


/****************************** VARIABLES *************************************/
RegParams_t RegParams;
uint8_t regTimerId[REG_PARAMS_TIMERS_COUNT];


/************************ PRIVATE FUNCTION PROTOTYPES *************************/
/*Init Functions's*/
//static void InitDefault915Channels (void);
void JoinDutyCycleCallback(uint8_t param);
static StackRetStatus_t setJoinDutyCycleTimer(LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t setJoinBackoffCntl(LorawanRegionalAttributes_t attr,void *attrInput);
static StackRetStatus_t setJoinBackOffTimer(LorawanRegionalAttributes_t attr, void *attrInput);

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_FreqT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_DutyCycleT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_RxWindowSizeT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_RxWindowOffsetT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_MaxPayloadT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_DRangeChBandT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_Rx1WindowparamsType1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_ModulationAttrT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_BandwidthAttrT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_SpreadFactorT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_MinDutyCycleTimer(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_NewTxChConfigT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_FreeChannel1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);


static StackRetStatus_t ValidateRxFreqT1 (LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t ValidateTxFreqT1 (LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t ValidateDataRateRxT1 (LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t ValidateDataRateTxT1 (LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t ValidateDataRange(LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t ValidateChannelId (LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t ValidateChannelMask (LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t ValidateChannelMaskCntl (LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t ValidateChMaskChCntl (LorawanRegionalAttributes_t attr, void *attrInput);

static StackRetStatus_t setDataRange (LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t setChannelIdStatus (LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t setNewChannelsT1 (LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t setJoinSuccess(LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t setEnableAllChs(LorawanRegionalAttributes_t attr, void *attrInput);

static StackRetStatus_t GetTxChannelConfig1 (bool transmissionType,uint8_t txPwr,uint8_t currDr,radioConfig_t* radioConfig);
static StackRetStatus_t SearchAvailableChannel1 (uint8_t maxChannels, bool transmissionType,uint8_t currDr, uint8_t* channelIndex);
static void getRx1WindowparamsType1(Rx1WindowParamsReq_t* rx1WindowParamReq ,Rx1WindowParams_t* rx1WindowParams);
static uint32_t GenerateFrequencyReception (uint8_t channelIndex);
static uint32_t GenerateFrequency1 (uint8_t channelIndex);
static uint32_t GenerateFrequency2 (uint8_t channelIndex);
static DataRange_t getChBandDrT1(uint8_t chMaskCntl,uint16_t channelMask);

static void EnableChannels2(uint8_t startIndx, uint8_t endIndx, uint16_t chMask);
#endif

#if (EU_BAND == 1 || IND_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_FreqT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_MaxPayloadT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
#endif

#if (EU_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_Rx1WindowparamsType2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);

static void getRx1WindowparamsType2(Rx1WindowParamsReq_t* rx1WindowParamReq ,Rx1WindowParams_t* rx1WindowParams);
#endif

#if (EU_BAND == 1) || (AS_BAND == 1) || (JPN_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_DutyCycleT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_DutyCycleTimer(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);

static StackRetStatus_t setDutyCycle(LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t setDutyCycleTimer(LorawanRegionalAttributes_t attr, void *attrInput);
#endif

#if (EU_BAND == 1)
static StackRetStatus_t ValidateFreq (LorawanRegionalAttributes_t attr, void *attrInput);
static void UpdateChannelIdStatusT2(uint8_t chid, bool statusNew);
#endif

#if (AS_BAND == 1 || JPN_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_FreqT3(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_MaxPayloadT3(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_Rx1WindowparamsType4(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);


static StackRetStatus_t ValidateDataRateRxT2 (LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t ValidateDataRateTxT2 (LorawanRegionalAttributes_t attr, void *attrInput);

static void getRx1WindowparamsType4(Rx1WindowParamsReq_t* rx1WindowParamReq,Rx1WindowParams_t* rx1WindowParams);
static void UpdateChannelIdStatusT3(uint8_t chid, bool statusNew);

#endif


#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_RxWindowSizeT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_RxWindowOffsetT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_DRangeChBandT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_ModulationAttrT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_BandwidthAttrT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_SpreadFactorT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_NewTxChConfigT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_FreeChannel2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_DlFrequency(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);


static StackRetStatus_t ValidateTxFreqT2 (LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t ValidateDataRangeT2(LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t ValidateChannelIdT2 (LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t ValidateChannelMaskT2 (LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t ValidateChannelMaskCntlT2 (LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t ValidateChMaskChCntlT2 (LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t LORAREG_GetAttr_DefTxPwr(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t setDataRangeT2 (LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t setFrequency(LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t setNewChannel(LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t setDlFrequency(LorawanRegionalAttributes_t attr, void *attrInput);

static StackRetStatus_t GetTxChannelConfig2 (bool transmissionType,uint8_t txPwr,uint8_t currDr,radioConfig_t* radioConfig);
static StackRetStatus_t SearchAvailableChannel2 (uint8_t maxChannels, bool transmissionType,uint8_t currDr, uint8_t* channelIndex);
static StackRetStatus_t ValidateDataRate (LorawanRegionalAttributes_t attr, void *attrInput);
static DataRange_t getChBandDrT2(uint8_t chMaskCntl,uint16_t channelMask);
// used only in EU band, but included in a common function
static uint8_t getSubBandId(uint32_t frequency);
static StackRetStatus_t setChlistDefaultState(LorawanRegionalAttributes_t attr, void *attrInput);

#endif

#if (IND_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_Rx1WindowparamsType3(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);

static StackRetStatus_t ValidateFreqIN (LorawanRegionalAttributes_t attr, void *attrInput);

static void getRx1WindowparamsType3(Rx1WindowParamsReq_t* rx1WindowParamReq ,Rx1WindowParams_t* rx1WindowParams);
#endif

#if (JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_DefLBTParams(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_minLBTChPauseTimer(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);

static StackRetStatus_t setLBTTimer(LorawanRegionalAttributes_t attr, void *attrInput);
static void LBTChannelPauseCallback (uint8_t param);
#endif

#if (AS_BAND == 1)
static StackRetStatus_t ValidateFrequencyAS (LorawanRegionalAttributes_t attr, void *attrInput);
#endif

#if (JPN_BAND == 1)
static StackRetStatus_t ValidateFreqJP (LorawanRegionalAttributes_t attr, void *attrInput);
#endif

#if( KR_BAND == 1)
static StackRetStatus_t ValidateFreqKR (LorawanRegionalAttributes_t attr, void *attrInput);
static void UpdateChannelIdStatusT4(uint8_t chid, bool statusNew);
#endif

#if( EU_BAND == 1 || IND_BAND == 1)
static StackRetStatus_t setChannelIdStatusT2 (LorawanRegionalAttributes_t attr, void *attrInput);
#endif

#if( AS_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t setChannelIdStatusT3 (LorawanRegionalAttributes_t attr, void *attrInput);
#endif

#if ( AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t setTxParams(LorawanRegionalAttributes_t attr, void *attrInput);
#endif

#if (NA_BAND == 1 || AU_BAND == 1 || IND_BAND == 1)
static void UpdateChannelIdStatus(uint8_t chid, bool statusNew);
#endif

static StackRetStatus_t LORAREG_GetAttr_MaxChannel(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_MinNewChIndex(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_DefRx1DataRate(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_DefRx2DataRate(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_DefRx2Freq(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_RegFeatures(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_DataRange(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_MinMaxDr(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_ChIdStatus(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_MacRecvDelay1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_MacRecvDelay2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_MacJoinAcptDelay1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_MacJoinAcptDelay2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_MacRetransmitTimeout(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_MacAdrAckDelay(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_MacAdrAckLimit(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_RegDefTxPwr(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_RegDefTxDR(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_CurChIndx(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
static StackRetStatus_t LORAREG_GetAttr_JoinDutyCycleRemainingTime(LorawanRegionalAttributes_t attr,void *attrInput, void *attrOutput);


static void UpdateMinMaxChDataRate (MinMaxDr_t* minmax_val);


static StackRetStatus_t ValidateSupportedDr (LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t ValidateTxPower (LorawanRegionalAttributes_t attr, void *attrInput);
static StackRetStatus_t ValidateRx1DataRateOffset(LorawanRegionalAttributes_t attr, void *attrInput);


/*StackRetStatus_t CreateAllRegSoftwareTimers (void);
void SetCallbackRegSoftwareTimers (void);

void StopAllRegSoftwareTimers (void);*/

#if (EU_BAND == 1) || (AS_BAND == 1) || (JPN_BAND == 1)
static void DutyCycleCallback (uint8_t param);
#endif

static pLoraRegGetAttr_t pGetAttr[REG_NUM_ATTRIBUTES];
static pLoraRegValidateAttr_t pValidateAttr[REG_NUM_ATTRIBUTES];
static pLoraRegSetAttr_t pSetAttr[REG_NUM_ATTRIBUTES];

/*****************************CONSTANTS ***************************************/

/****************************** FUNCTIONS *************************************/

StackRetStatus_t InValidGetAttr(LorawanRegionalAttributes_t attr, void * attrInput, void * attrOutput)
{
	return LORAWAN_INVALID_REQUEST;
}

StackRetStatus_t InValidAttr(LorawanRegionalAttributes_t attr, void * attrInput)
{
	return LORAWAN_INVALID_REQUEST;
}

#if (NA_BAND == 1)
void LORAREG_InitGetAttrFnPtrsNA(void)
{
    pGetAttr[FREQUENCY] = LORAREG_GetAttr_FreqT1;
    pGetAttr[RX_WINDOW_SIZE] = LORAREG_GetAttr_RxWindowSizeT1;
    pGetAttr[RX_WINDOW_OFFSET] = LORAREG_GetAttr_RxWindowOffsetT1;
    pGetAttr[MAX_PAYLOAD_SIZE] = LORAREG_GetAttr_MaxPayloadT1;
    pGetAttr[MAX_CHANNELS] = LORAREG_GetAttr_MaxChannel;
    pGetAttr[MIN_NEW_CH_INDEX] = LORAREG_GetAttr_MinNewChIndex;
    pGetAttr[DEFAULT_RX1_DATA_RATE] = LORAREG_GetAttr_DefRx1DataRate;
    pGetAttr[DEFAULT_RX2_DATA_RATE] = LORAREG_GetAttr_DefRx2DataRate;
    pGetAttr[DEFAULT_RX2_FREQUENCY] = LORAREG_GetAttr_DefRx2Freq;
    pGetAttr[SUPPORTED_REGIONAL_FEATURES] = LORAREG_GetAttr_RegFeatures;
    pGetAttr[DATA_RANGE] = LORAREG_GetAttr_DataRange;
    pGetAttr[DATA_RANGE_CH_BAND] = LORAREG_GetAttr_DRangeChBandT1;
    pGetAttr[MIN_MAX_DR] = LORAREG_GetAttr_MinMaxDr;
    pGetAttr[CHANNEL_ID_STATUS] = LORAREG_GetAttr_ChIdStatus;
    pGetAttr[RX1_WINDOW_PARAMS] = LORAREG_GetAttr_Rx1WindowparamsType1;
	pGetAttr[JOIN_DUTY_CYCLE_TIMER] = LORAREG_GetAttr_JoinDutyCycleRemainingTime;
    pGetAttr[DUTY_CYCLE] = LORAREG_GetAttr_DutyCycleT1;
    pGetAttr[MODULATION_ATTR] = LORAREG_GetAttr_ModulationAttrT1;
    pGetAttr[BANDWIDTH_ATTR] = LORAREG_GetAttr_BandwidthAttrT1;
    pGetAttr[SPREADING_FACTOR_ATTR] = LORAREG_GetAttr_SpreadFactorT1;
    pGetAttr[MIN_DUTY_CYCLE_TIMER] = LORAREG_GetAttr_MinDutyCycleTimer;
    pGetAttr[MAC_RECEIVE_DELAY1] = LORAREG_GetAttr_MacRecvDelay1;
    pGetAttr[MAC_RECEIVE_DELAY2] = LORAREG_GetAttr_MacRecvDelay2;
    pGetAttr[MAC_JOIN_ACCEPT_DELAY1] = LORAREG_GetAttr_MacJoinAcptDelay1;
    pGetAttr[MAC_JOIN_ACCEPT_DELAY2] = LORAREG_GetAttr_MacJoinAcptDelay2;
    pGetAttr[MAC_RETRANSMIT_TIMEOUT] = LORAREG_GetAttr_MacRetransmitTimeout;
    pGetAttr[MAC_ADR_ACK_DELAY] = LORAREG_GetAttr_MacAdrAckDelay;
    pGetAttr[MAC_ADR_ACK_LIMIT] = LORAREG_GetAttr_MacAdrAckLimit;
    pGetAttr[NEW_TX_CHANNEL_CONFIG] = LORAREG_GetAttr_NewTxChConfigT1;
    pGetAttr[FREE_CHANNEL] = LORAREG_GetAttr_FreeChannel1;
    pGetAttr[CURRENT_CHANNEL_INDEX] = LORAREG_GetAttr_CurChIndx;
    pGetAttr[REG_DEF_TX_POWER] = LORAREG_GetAttr_RegDefTxPwr;
	pGetAttr[DEF_TX_PWR] = LORAREG_GetAttr_DefTxPwr;
    pGetAttr[REG_DEF_TX_DATARATE] = LORAREG_GetAttr_RegDefTxDR;
}
#endif

#if(EU_BAND == 1)
void LORAREG_InitGetAttrFnPtrsEU(void)
{
    pGetAttr[FREQUENCY] = LORAREG_GetAttr_FreqT2;
    pGetAttr[RX_WINDOW_SIZE] = LORAREG_GetAttr_RxWindowSizeT2;
    pGetAttr[RX_WINDOW_OFFSET] = LORAREG_GetAttr_RxWindowOffsetT2;
    pGetAttr[MAX_PAYLOAD_SIZE] = LORAREG_GetAttr_MaxPayloadT2;
    pGetAttr[MAX_CHANNELS] = LORAREG_GetAttr_MaxChannel;
    pGetAttr[MIN_NEW_CH_INDEX] = LORAREG_GetAttr_MinNewChIndex;
    pGetAttr[DEFAULT_RX1_DATA_RATE] = LORAREG_GetAttr_DefRx1DataRate;
    pGetAttr[DEFAULT_RX2_DATA_RATE] = LORAREG_GetAttr_DefRx2DataRate;
    pGetAttr[DEFAULT_RX2_FREQUENCY] = LORAREG_GetAttr_DefRx2Freq;
    pGetAttr[SUPPORTED_REGIONAL_FEATURES] = LORAREG_GetAttr_RegFeatures;
    pGetAttr[DATA_RANGE] = LORAREG_GetAttr_DataRange;
    pGetAttr[DATA_RANGE_CH_BAND] = LORAREG_GetAttr_DRangeChBandT2;
    pGetAttr[MIN_MAX_DR] = LORAREG_GetAttr_MinMaxDr;
    pGetAttr[CHANNEL_ID_STATUS] = LORAREG_GetAttr_ChIdStatus;
    pGetAttr[RX1_WINDOW_PARAMS] = LORAREG_GetAttr_Rx1WindowparamsType2;
	pGetAttr[JOIN_DUTY_CYCLE_TIMER]= LORAREG_GetAttr_JoinDutyCycleRemainingTime;
    pGetAttr[DUTY_CYCLE] = LORAREG_GetAttr_DutyCycleT2;
    pGetAttr[MODULATION_ATTR] = LORAREG_GetAttr_ModulationAttrT2;
    pGetAttr[BANDWIDTH_ATTR] = LORAREG_GetAttr_BandwidthAttrT2;
    pGetAttr[SPREADING_FACTOR_ATTR] = LORAREG_GetAttr_SpreadFactorT2;
    pGetAttr[MIN_DUTY_CYCLE_TIMER] = LORAREG_GetAttr_DutyCycleTimer;
    pGetAttr[MAC_RECEIVE_DELAY1] = LORAREG_GetAttr_MacRecvDelay1;
    pGetAttr[MAC_RECEIVE_DELAY2] = LORAREG_GetAttr_MacRecvDelay2;
    pGetAttr[MAC_JOIN_ACCEPT_DELAY1] = LORAREG_GetAttr_MacJoinAcptDelay1;
    pGetAttr[MAC_JOIN_ACCEPT_DELAY2] = LORAREG_GetAttr_MacJoinAcptDelay2;
    pGetAttr[MAC_RETRANSMIT_TIMEOUT] = LORAREG_GetAttr_MacRetransmitTimeout;
    pGetAttr[MAC_ADR_ACK_DELAY] = LORAREG_GetAttr_MacAdrAckDelay;
    pGetAttr[MAC_ADR_ACK_LIMIT] = LORAREG_GetAttr_MacAdrAckLimit;
    pGetAttr[NEW_TX_CHANNEL_CONFIG] = LORAREG_GetAttr_NewTxChConfigT2;
    pGetAttr[FREE_CHANNEL] = LORAREG_GetAttr_FreeChannel2;
    pGetAttr[CURRENT_CHANNEL_INDEX] = LORAREG_GetAttr_CurChIndx;
    pGetAttr[DL_FREQUENCY] = LORAREG_GetAttr_DlFrequency;
    pGetAttr[REG_DEF_TX_POWER] = LORAREG_GetAttr_RegDefTxPwr;
	pGetAttr[DEF_TX_PWR] = LORAREG_GetAttr_DefTxPwr;
    pGetAttr[REG_DEF_TX_DATARATE] = LORAREG_GetAttr_RegDefTxDR;
}
#endif


#if(AS_BAND == 1)
void LORAREG_InitGetAttrFnPtrsAS(void)
{
    pGetAttr[FREQUENCY] = LORAREG_GetAttr_FreqT3;
    pGetAttr[RX_WINDOW_SIZE] = LORAREG_GetAttr_RxWindowSizeT2;
    pGetAttr[RX_WINDOW_OFFSET] = LORAREG_GetAttr_RxWindowOffsetT2;
    pGetAttr[MAX_PAYLOAD_SIZE] = LORAREG_GetAttr_MaxPayloadT3;
    pGetAttr[MAX_CHANNELS] = LORAREG_GetAttr_MaxChannel;
    pGetAttr[MIN_NEW_CH_INDEX] = LORAREG_GetAttr_MinNewChIndex;
    pGetAttr[DEFAULT_RX1_DATA_RATE] = LORAREG_GetAttr_DefRx1DataRate;
    pGetAttr[DEFAULT_RX2_DATA_RATE] = LORAREG_GetAttr_DefRx2DataRate;
    pGetAttr[DEFAULT_RX2_FREQUENCY] = LORAREG_GetAttr_DefRx2Freq;
    pGetAttr[SUPPORTED_REGIONAL_FEATURES] = LORAREG_GetAttr_RegFeatures;
    pGetAttr[DATA_RANGE] = LORAREG_GetAttr_DataRange;
    pGetAttr[DATA_RANGE_CH_BAND] = LORAREG_GetAttr_DRangeChBandT2;
    pGetAttr[MIN_MAX_DR] = LORAREG_GetAttr_MinMaxDr;
    pGetAttr[CHANNEL_ID_STATUS] = LORAREG_GetAttr_ChIdStatus;
    pGetAttr[RX1_WINDOW_PARAMS] = LORAREG_GetAttr_Rx1WindowparamsType4;
	pGetAttr[JOIN_DUTY_CYCLE_TIMER]= LORAREG_GetAttr_JoinDutyCycleRemainingTime;
    pGetAttr[MODULATION_ATTR] = LORAREG_GetAttr_ModulationAttrT2;
    pGetAttr[BANDWIDTH_ATTR] = LORAREG_GetAttr_BandwidthAttrT2;
    pGetAttr[SPREADING_FACTOR_ATTR] = LORAREG_GetAttr_SpreadFactorT2;
    pGetAttr[MAC_RECEIVE_DELAY1] = LORAREG_GetAttr_MacRecvDelay1;
    pGetAttr[MAC_RECEIVE_DELAY2] = LORAREG_GetAttr_MacRecvDelay2;
    pGetAttr[MAC_JOIN_ACCEPT_DELAY1] = LORAREG_GetAttr_MacJoinAcptDelay1;
    pGetAttr[MAC_JOIN_ACCEPT_DELAY2] = LORAREG_GetAttr_MacJoinAcptDelay2;
    pGetAttr[MAC_RETRANSMIT_TIMEOUT] = LORAREG_GetAttr_MacRetransmitTimeout;
    pGetAttr[MAC_ADR_ACK_DELAY] = LORAREG_GetAttr_MacAdrAckDelay;
    pGetAttr[MAC_ADR_ACK_LIMIT] = LORAREG_GetAttr_MacAdrAckLimit;
    pGetAttr[NEW_TX_CHANNEL_CONFIG] = LORAREG_GetAttr_NewTxChConfigT2;
    pGetAttr[FREE_CHANNEL] = LORAREG_GetAttr_FreeChannel2;
    pGetAttr[CURRENT_CHANNEL_INDEX] = LORAREG_GetAttr_CurChIndx;
    pGetAttr[DL_FREQUENCY] = LORAREG_GetAttr_DlFrequency;
    pGetAttr[REG_DEF_TX_POWER] = LORAREG_GetAttr_RegDefTxPwr;
	pGetAttr[DEF_TX_PWR] = LORAREG_GetAttr_DefTxPwr;
    pGetAttr[REG_DEF_TX_DATARATE] = LORAREG_GetAttr_RegDefTxDR;
    pGetAttr[DUTY_CYCLE] = LORAREG_GetAttr_DutyCycleT2;
    pGetAttr[MIN_DUTY_CYCLE_TIMER] = LORAREG_GetAttr_DutyCycleTimer;
}
#endif

#if(AU_BAND == 1)
void LORAREG_InitGetAttrFnPtrsAU(void)
{
    pGetAttr[FREQUENCY] = LORAREG_GetAttr_FreqT1;
    pGetAttr[RX_WINDOW_SIZE] = LORAREG_GetAttr_RxWindowSizeT1;
    pGetAttr[RX_WINDOW_OFFSET] = LORAREG_GetAttr_RxWindowOffsetT1;
    pGetAttr[MAX_PAYLOAD_SIZE] = LORAREG_GetAttr_MaxPayloadT1;
    pGetAttr[MAX_CHANNELS] = LORAREG_GetAttr_MaxChannel;
    pGetAttr[MIN_NEW_CH_INDEX] = LORAREG_GetAttr_MinNewChIndex;
    pGetAttr[DEFAULT_RX1_DATA_RATE] = LORAREG_GetAttr_DefRx1DataRate;
    pGetAttr[DEFAULT_RX2_DATA_RATE] = LORAREG_GetAttr_DefRx2DataRate;
    pGetAttr[DEFAULT_RX2_FREQUENCY] = LORAREG_GetAttr_DefRx2Freq;
    pGetAttr[SUPPORTED_REGIONAL_FEATURES] = LORAREG_GetAttr_RegFeatures;
    pGetAttr[DATA_RANGE] = LORAREG_GetAttr_DataRange;
    pGetAttr[DATA_RANGE_CH_BAND] = LORAREG_GetAttr_DRangeChBandT1;
    pGetAttr[MIN_MAX_DR] = LORAREG_GetAttr_MinMaxDr;
    pGetAttr[CHANNEL_ID_STATUS] = LORAREG_GetAttr_ChIdStatus;
    pGetAttr[RX1_WINDOW_PARAMS] = LORAREG_GetAttr_Rx1WindowparamsType1;
	pGetAttr[JOIN_DUTY_CYCLE_TIMER]= LORAREG_GetAttr_JoinDutyCycleRemainingTime;
    pGetAttr[DUTY_CYCLE] = LORAREG_GetAttr_DutyCycleT1;
    pGetAttr[DUTY_CYCLE_TIMER] = LORAREG_GetAttr_MinDutyCycleTimer;	
    pGetAttr[MODULATION_ATTR] = LORAREG_GetAttr_ModulationAttrT1;
    pGetAttr[BANDWIDTH_ATTR] = LORAREG_GetAttr_BandwidthAttrT1;
    pGetAttr[SPREADING_FACTOR_ATTR] = LORAREG_GetAttr_SpreadFactorT1;
    pGetAttr[MAC_RECEIVE_DELAY1] = LORAREG_GetAttr_MacRecvDelay1;
    pGetAttr[MAC_RECEIVE_DELAY2] = LORAREG_GetAttr_MacRecvDelay2;
    pGetAttr[MAC_JOIN_ACCEPT_DELAY1] = LORAREG_GetAttr_MacJoinAcptDelay1;
    pGetAttr[MAC_JOIN_ACCEPT_DELAY2] = LORAREG_GetAttr_MacJoinAcptDelay2;
    pGetAttr[MAC_RETRANSMIT_TIMEOUT] = LORAREG_GetAttr_MacRetransmitTimeout;
    pGetAttr[MAC_ADR_ACK_DELAY] = LORAREG_GetAttr_MacAdrAckDelay;
    pGetAttr[MAC_ADR_ACK_LIMIT] = LORAREG_GetAttr_MacAdrAckLimit;
    pGetAttr[NEW_TX_CHANNEL_CONFIG] = LORAREG_GetAttr_NewTxChConfigT1;
    pGetAttr[FREE_CHANNEL] = LORAREG_GetAttr_FreeChannel1;
    pGetAttr[CURRENT_CHANNEL_INDEX] = LORAREG_GetAttr_CurChIndx;
    pGetAttr[REG_DEF_TX_POWER] = LORAREG_GetAttr_RegDefTxPwr;
	pGetAttr[DEF_TX_PWR] = LORAREG_GetAttr_DefTxPwr;
    pGetAttr[REG_DEF_TX_DATARATE] = LORAREG_GetAttr_RegDefTxDR;
}
#endif

#if(IND_BAND == 1)
void LORAREG_InitGetAttrFnPtrsIN(void)
{
    pGetAttr[FREQUENCY] = LORAREG_GetAttr_FreqT2;
    pGetAttr[RX_WINDOW_SIZE] = LORAREG_GetAttr_RxWindowSizeT2;
    pGetAttr[RX_WINDOW_OFFSET] = LORAREG_GetAttr_RxWindowOffsetT2;
    pGetAttr[MAX_PAYLOAD_SIZE] = LORAREG_GetAttr_MaxPayloadT2;
    pGetAttr[MAX_CHANNELS] = LORAREG_GetAttr_MaxChannel;
    pGetAttr[MIN_NEW_CH_INDEX] = LORAREG_GetAttr_MinNewChIndex;
    pGetAttr[DEFAULT_RX1_DATA_RATE] = LORAREG_GetAttr_DefRx1DataRate;
    pGetAttr[DEFAULT_RX2_DATA_RATE] = LORAREG_GetAttr_DefRx2DataRate;
    pGetAttr[DEFAULT_RX2_FREQUENCY] = LORAREG_GetAttr_DefRx2Freq;
    pGetAttr[SUPPORTED_REGIONAL_FEATURES] = LORAREG_GetAttr_RegFeatures;
    pGetAttr[DATA_RANGE] = LORAREG_GetAttr_DataRange;
    pGetAttr[DATA_RANGE_CH_BAND] = LORAREG_GetAttr_DRangeChBandT2;
    pGetAttr[MIN_MAX_DR] = LORAREG_GetAttr_MinMaxDr;
    pGetAttr[CHANNEL_ID_STATUS] = LORAREG_GetAttr_ChIdStatus;
    pGetAttr[RX1_WINDOW_PARAMS] = LORAREG_GetAttr_Rx1WindowparamsType3;
	pGetAttr[JOIN_DUTY_CYCLE_TIMER]= LORAREG_GetAttr_JoinDutyCycleRemainingTime;
    pGetAttr[MODULATION_ATTR] = LORAREG_GetAttr_ModulationAttrT2;
    pGetAttr[BANDWIDTH_ATTR] = LORAREG_GetAttr_BandwidthAttrT2;
    pGetAttr[SPREADING_FACTOR_ATTR] = LORAREG_GetAttr_SpreadFactorT2;
    pGetAttr[MAC_RECEIVE_DELAY1] = LORAREG_GetAttr_MacRecvDelay1;
    pGetAttr[MAC_RECEIVE_DELAY2] = LORAREG_GetAttr_MacRecvDelay2;
    pGetAttr[MAC_JOIN_ACCEPT_DELAY1] = LORAREG_GetAttr_MacJoinAcptDelay1;
    pGetAttr[MAC_JOIN_ACCEPT_DELAY2] = LORAREG_GetAttr_MacJoinAcptDelay2;
    pGetAttr[MAC_RETRANSMIT_TIMEOUT] = LORAREG_GetAttr_MacRetransmitTimeout;
    pGetAttr[MAC_ADR_ACK_DELAY] = LORAREG_GetAttr_MacAdrAckDelay;
    pGetAttr[MAC_ADR_ACK_LIMIT] = LORAREG_GetAttr_MacAdrAckLimit;
    pGetAttr[NEW_TX_CHANNEL_CONFIG] = LORAREG_GetAttr_NewTxChConfigT2;
    pGetAttr[FREE_CHANNEL] = LORAREG_GetAttr_FreeChannel2;
    pGetAttr[CURRENT_CHANNEL_INDEX] = LORAREG_GetAttr_CurChIndx;
    pGetAttr[DL_FREQUENCY] = LORAREG_GetAttr_DlFrequency;
    pGetAttr[REG_DEF_TX_POWER] = LORAREG_GetAttr_RegDefTxPwr;
	pGetAttr[DEF_TX_PWR] = LORAREG_GetAttr_DefTxPwr;
    pGetAttr[REG_DEF_TX_DATARATE] = LORAREG_GetAttr_RegDefTxDR;
}
#endif

#if(JPN_BAND  == 1)
void LORAREG_InitGetAttrFnPtrsJP(void)
{
    pGetAttr[FREQUENCY] = LORAREG_GetAttr_FreqT3;
    pGetAttr[RX_WINDOW_SIZE] = LORAREG_GetAttr_RxWindowSizeT2;
    pGetAttr[RX_WINDOW_OFFSET] = LORAREG_GetAttr_RxWindowOffsetT2;
    pGetAttr[MAX_PAYLOAD_SIZE] = LORAREG_GetAttr_MaxPayloadT3;
    pGetAttr[MAX_CHANNELS] = LORAREG_GetAttr_MaxChannel;
    pGetAttr[MIN_NEW_CH_INDEX] = LORAREG_GetAttr_MinNewChIndex;
    pGetAttr[DEFAULT_RX1_DATA_RATE] = LORAREG_GetAttr_DefRx1DataRate;
    pGetAttr[DEFAULT_RX2_DATA_RATE] = LORAREG_GetAttr_DefRx2DataRate;
    pGetAttr[DEFAULT_RX2_FREQUENCY] = LORAREG_GetAttr_DefRx2Freq;
    pGetAttr[SUPPORTED_REGIONAL_FEATURES] = LORAREG_GetAttr_RegFeatures;
    pGetAttr[DATA_RANGE] = LORAREG_GetAttr_DataRange;
    pGetAttr[DATA_RANGE_CH_BAND] = LORAREG_GetAttr_DRangeChBandT2;
    pGetAttr[MIN_MAX_DR] = LORAREG_GetAttr_MinMaxDr;
    pGetAttr[CHANNEL_ID_STATUS] = LORAREG_GetAttr_ChIdStatus;
    pGetAttr[RX1_WINDOW_PARAMS] = LORAREG_GetAttr_Rx1WindowparamsType4;
	pGetAttr[JOIN_DUTY_CYCLE_TIMER]= LORAREG_GetAttr_JoinDutyCycleRemainingTime;
    pGetAttr[MODULATION_ATTR] = LORAREG_GetAttr_ModulationAttrT2;
    pGetAttr[BANDWIDTH_ATTR] = LORAREG_GetAttr_BandwidthAttrT2;
    pGetAttr[SPREADING_FACTOR_ATTR] = LORAREG_GetAttr_SpreadFactorT2;
    pGetAttr[MAC_RECEIVE_DELAY1] = LORAREG_GetAttr_MacRecvDelay1;
    pGetAttr[MAC_RECEIVE_DELAY2] = LORAREG_GetAttr_MacRecvDelay2;
    pGetAttr[MAC_JOIN_ACCEPT_DELAY1] = LORAREG_GetAttr_MacJoinAcptDelay1;
    pGetAttr[MAC_JOIN_ACCEPT_DELAY2] = LORAREG_GetAttr_MacJoinAcptDelay2;
    pGetAttr[MAC_RETRANSMIT_TIMEOUT] = LORAREG_GetAttr_MacRetransmitTimeout;
    pGetAttr[MAC_ADR_ACK_DELAY] = LORAREG_GetAttr_MacAdrAckDelay;
    pGetAttr[MAC_ADR_ACK_LIMIT] = LORAREG_GetAttr_MacAdrAckLimit;
    pGetAttr[NEW_TX_CHANNEL_CONFIG] = LORAREG_GetAttr_NewTxChConfigT2;
    pGetAttr[FREE_CHANNEL] = LORAREG_GetAttr_FreeChannel2;
    pGetAttr[CURRENT_CHANNEL_INDEX] = LORAREG_GetAttr_CurChIndx;
    pGetAttr[DEFAULT_LBT_PARAMS] = LORAREG_GetAttr_DefLBTParams;
    pGetAttr[MIN_LBT_CHANNEL_PAUSE_TIMER] = LORAREG_GetAttr_minLBTChPauseTimer;
    pGetAttr[DL_FREQUENCY] = LORAREG_GetAttr_DlFrequency;
    pGetAttr[REG_DEF_TX_POWER] = LORAREG_GetAttr_RegDefTxPwr;
	pGetAttr[DEF_TX_PWR] = LORAREG_GetAttr_DefTxPwr;
    pGetAttr[REG_DEF_TX_DATARATE] = LORAREG_GetAttr_RegDefTxDR;
	pGetAttr[DUTY_CYCLE] = LORAREG_GetAttr_DutyCycleT2;
	pGetAttr[MIN_DUTY_CYCLE_TIMER] = LORAREG_GetAttr_DutyCycleTimer;
}
#endif

#if(KR_BAND == 1)
void LORAREG_InitGetAttrFnPtrsKR(void)
{
    pGetAttr[FREQUENCY] = LORAREG_GetAttr_FreqT2;
    pGetAttr[RX_WINDOW_SIZE] = LORAREG_GetAttr_RxWindowSizeT2;
    pGetAttr[RX_WINDOW_OFFSET] = LORAREG_GetAttr_RxWindowOffsetT2;
    pGetAttr[MAX_PAYLOAD_SIZE] = LORAREG_GetAttr_MaxPayloadT2;
    pGetAttr[MAX_CHANNELS] = LORAREG_GetAttr_MaxChannel;
    pGetAttr[MIN_NEW_CH_INDEX] = LORAREG_GetAttr_MinNewChIndex;
    pGetAttr[DEFAULT_RX1_DATA_RATE] = LORAREG_GetAttr_DefRx1DataRate;
    pGetAttr[DEFAULT_RX2_DATA_RATE] = LORAREG_GetAttr_DefRx2DataRate;
    pGetAttr[DEFAULT_RX2_FREQUENCY] = LORAREG_GetAttr_DefRx2Freq;
    pGetAttr[SUPPORTED_REGIONAL_FEATURES] = LORAREG_GetAttr_RegFeatures;
    pGetAttr[DATA_RANGE] = LORAREG_GetAttr_DataRange;
    pGetAttr[DATA_RANGE_CH_BAND] = LORAREG_GetAttr_DRangeChBandT2;
    pGetAttr[MIN_MAX_DR] = LORAREG_GetAttr_MinMaxDr;
    pGetAttr[CHANNEL_ID_STATUS] = LORAREG_GetAttr_ChIdStatus;
    pGetAttr[RX1_WINDOW_PARAMS] = LORAREG_GetAttr_Rx1WindowparamsType2;
	pGetAttr[JOIN_DUTY_CYCLE_TIMER]=LORAREG_GetAttr_JoinDutyCycleRemainingTime;
    pGetAttr[MODULATION_ATTR] = LORAREG_GetAttr_ModulationAttrT2;
    pGetAttr[BANDWIDTH_ATTR] = LORAREG_GetAttr_BandwidthAttrT2;
    pGetAttr[SPREADING_FACTOR_ATTR] = LORAREG_GetAttr_SpreadFactorT2;
    pGetAttr[MAC_RECEIVE_DELAY1] = LORAREG_GetAttr_MacRecvDelay1;
    pGetAttr[MAC_RECEIVE_DELAY2] = LORAREG_GetAttr_MacRecvDelay2;
    pGetAttr[MAC_JOIN_ACCEPT_DELAY1] = LORAREG_GetAttr_MacJoinAcptDelay1;
    pGetAttr[MAC_JOIN_ACCEPT_DELAY2] = LORAREG_GetAttr_MacJoinAcptDelay2;
    pGetAttr[MAC_RETRANSMIT_TIMEOUT] = LORAREG_GetAttr_MacRetransmitTimeout;
    pGetAttr[MAC_ADR_ACK_DELAY] = LORAREG_GetAttr_MacAdrAckDelay;
    pGetAttr[MAC_ADR_ACK_LIMIT] = LORAREG_GetAttr_MacAdrAckLimit;
    pGetAttr[NEW_TX_CHANNEL_CONFIG] = LORAREG_GetAttr_NewTxChConfigT2;
    pGetAttr[FREE_CHANNEL] = LORAREG_GetAttr_FreeChannel2;
    pGetAttr[CURRENT_CHANNEL_INDEX] = LORAREG_GetAttr_CurChIndx;
    pGetAttr[DEFAULT_LBT_PARAMS] = LORAREG_GetAttr_DefLBTParams;
    pGetAttr[MIN_LBT_CHANNEL_PAUSE_TIMER] = LORAREG_GetAttr_minLBTChPauseTimer;
    pGetAttr[DL_FREQUENCY] = LORAREG_GetAttr_DlFrequency;
    pGetAttr[REG_DEF_TX_POWER] = LORAREG_GetAttr_RegDefTxPwr;
	pGetAttr[DEF_TX_PWR] = LORAREG_GetAttr_DefTxPwr;
    pGetAttr[REG_DEF_TX_DATARATE] = LORAREG_GetAttr_RegDefTxDR;
}
#endif

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_FreqT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	uint8_t  channelId;
	
	StackRetStatus_t result = LORAWAN_SUCCESS;

	channelId = *(uint8_t *)attrInput;
	if(channelId > RegParams.maxChannels)
	{
		return LORAWAN_INVALID_PARAMETER;
	}
	if (channelId < RegParams.cmnParams.paramsType1.Max_125khzChan)
	{
		*(uint32_t *)attrOutput = GenerateFrequency1 (channelId);
	}
	else if ( (channelId < RegParams.maxChannels) && (channelId >= RegParams.cmnParams.paramsType1.Max_125khzChan) )
	{
		*(uint32_t *)attrOutput = GenerateFrequency2 (channelId);
	}
	else
	{
		result = LORAWAN_INVALID_PARAMETER;
	}

	return result;
}
#endif

#if (EU_BAND == 1 || IND_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_FreqT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	uint8_t  channelId;
	StackRetStatus_t result = LORAWAN_SUCCESS;

	channelId = *(uint8_t *)attrInput;
	if(channelId > RegParams.maxChannels)
	{
		return LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		memcpy(attrOutput,(&RegParams.pOtherChParams[channelId].ulfrequency),sizeof(uint32_t));
	}
	
	return result;
}
#endif

#if (AS_BAND == 1 || JPN_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_FreqT3(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	uint8_t  channelId;
	StackRetStatus_t result = LORAWAN_SUCCESS;

	channelId = *(uint8_t *)attrInput;
	if(channelId > RegParams.maxChannels) 
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		memcpy(attrOutput,(&RegParams.pOtherChParams[channelId].ulfrequency),sizeof(uint32_t));
	}
	
	return result;
}
#endif

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_RxWindowSizeT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint8_t  dataRate;
	
	dataRate = *(uint8_t *)attrInput;
	if((ValidateDataRateTxT1(TX_DATARATE, attrInput) != LORAWAN_SUCCESS) && (ValidateDataRateRxT1(RX_DATARATE, attrInput) != LORAWAN_SUCCESS))
	{
		return LORAWAN_INVALID_PARAMETER;
	}
	else
	{
	    *(uint16_t *)attrOutput = RegParams.pDrParams[dataRate].rxWindowSize;	
	}
	return result;
}
#endif

#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_RxWindowSizeT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint8_t  dataRate;
	
	dataRate = *(uint8_t *)attrInput;
	if((ValidateDataRate(RX_DATARATE, attrInput) != LORAWAN_SUCCESS))
	{
		return LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		*(uint16_t *)attrOutput = RegParams.pDrParams[dataRate].rxWindowSize;
	}
	return result;
}
#endif

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_RxWindowOffsetT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint8_t  dataRate;
	
	dataRate = *(uint8_t *)attrInput;
	if((ValidateDataRateTxT1(TX_DATARATE, attrInput) != LORAWAN_SUCCESS) && (ValidateDataRateRxT1(RX_DATARATE, attrInput) != LORAWAN_SUCCESS))
	{
		return LORAWAN_INVALID_PARAMETER;
	}
    else
	{
		*(int8_t *)attrOutput = RegParams.pDrParams[dataRate].rxWindowOffset;
	}
	
	return result;
}
#endif

#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_RxWindowOffsetT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint8_t  dataRate;
	
	dataRate = *(uint8_t *)attrInput;
	if((ValidateDataRate(RX_DATARATE, attrInput) != LORAWAN_SUCCESS))
	{
		return LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		*(int8_t *)attrOutput = RegParams.pDrParams[dataRate].rxWindowOffset;
	}
	
	return result;
}
#endif

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_MaxPayloadT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint8_t  dataRate;
	
	dataRate = *(uint8_t *)attrInput;
	if((ValidateDataRateTxT1(TX_DATARATE, attrInput) != LORAWAN_SUCCESS) && (ValidateDataRateRxT1(RX_DATARATE, attrInput) != LORAWAN_SUCCESS))
	{
		result =  LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		*(uint8_t *)attrOutput = RegParams.pDrParams[dataRate].maxPayloadSizeDt0;
	}
	return result;
}
#endif

#if (EU_BAND == 1 || IND_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_MaxPayloadT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint8_t  dataRate;
	
	dataRate = *(uint8_t *)attrInput;
	if((ValidateDataRate(RX_DATARATE, attrInput) != LORAWAN_SUCCESS))
	{
		result =  LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		*(uint8_t *)attrOutput = RegParams.pDrParams[dataRate].maxPayloadSizeDt0;
	}
	return result;
}
#endif

#if (AS_BAND == 1 || JPN_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_MaxPayloadT3(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint8_t  dataRate;
	
	dataRate = *(uint8_t *)attrInput;
	if((ValidateDataRate(RX_DATARATE, attrInput) != LORAWAN_SUCCESS))
	{
		result =  LORAWAN_INVALID_PARAMETER;
	}
	if(RegParams.cmnParams.paramsType2.txParams.uplinkDwellTime == 1)
	{
		*(uint8_t *)attrOutput = RegParams.pDrParams[dataRate].maxPayloadSizeDt1;
	}
	else
	{
		*(uint8_t *)attrOutput = RegParams.pDrParams[dataRate].maxPayloadSizeDt0;
	}
	return result;
}
#endif

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_SpreadFactorT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint8_t  dataRate;
	
	dataRate = *(uint8_t *)attrInput;
	if((ValidateDataRateTxT1(TX_DATARATE, attrInput) != LORAWAN_SUCCESS) && (ValidateDataRateRxT1(RX_DATARATE, attrInput) != LORAWAN_SUCCESS))
	{
		result =  LORAWAN_INVALID_PARAMETER;
	}
    else
	{
		*(RadioDataRate_t *)attrOutput = RegParams.pDrParams[dataRate].spreadingFactor;
	}
	return result;
}
#endif

#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_SpreadFactorT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint8_t  dataRate;
	
	dataRate = *(uint8_t *)attrInput;
	if((ValidateDataRate(RX_DATARATE, attrInput) != LORAWAN_SUCCESS))
	{
		result =  LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		*(RadioDataRate_t *)attrOutput = RegParams.pDrParams[dataRate].spreadingFactor;
	}
	return result;
}
#endif


static StackRetStatus_t LORAREG_GetAttr_MaxChannel(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	*(uint8_t *)attrOutput = RegParams.maxChannels;
	return LORAWAN_SUCCESS;
}


static StackRetStatus_t LORAREG_GetAttr_MinNewChIndex(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	*(uint8_t *)attrOutput = (uint8_t)RegParams.MinNewChIndex;
	if(RegParams.MinNewChIndex == 0xFF)
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	
	return result;
}

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_ModulationAttrT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint8_t  dataRate;
	dataRate = *(uint8_t *)attrInput;
	if((ValidateDataRateTxT1(TX_DATARATE, attrInput) != LORAWAN_SUCCESS) && (ValidateDataRateRxT1(RX_DATARATE, attrInput) != LORAWAN_SUCCESS))
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	else
	{
	    *(uint8_t *)attrOutput = RegParams.pDrParams[dataRate].modulation;
	}

	return result;
}
#endif

#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_ModulationAttrT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint8_t  dataRate;
	dataRate = *(uint8_t *)attrInput;
	if((ValidateDataRate(RX_DATARATE, attrInput) != LORAWAN_SUCCESS))
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		*(uint8_t *)attrOutput = RegParams.pDrParams[dataRate].modulation;
	}

	return result;
}
#endif

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_BandwidthAttrT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint8_t  dataRate;
	dataRate = *(uint8_t *)attrInput;
	if((ValidateDataRateTxT1(TX_DATARATE, attrInput) != LORAWAN_SUCCESS) && (ValidateDataRateRxT1(RX_DATARATE, attrInput) != LORAWAN_SUCCESS))
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		*(uint8_t *)attrOutput = RegParams.pDrParams[dataRate].bandwidth;
	}
	return result;
}
#endif

#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_BandwidthAttrT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint8_t  dataRate;
	dataRate = *(uint8_t *)attrInput;
	if((ValidateDataRate(RX_DATARATE, attrInput) != LORAWAN_SUCCESS))
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		*(uint8_t *)attrOutput = RegParams.pDrParams[dataRate].bandwidth;
	}
	return result;
}
#endif

static StackRetStatus_t LORAREG_GetAttr_DefRx1DataRate(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	*(uint8_t *)attrOutput = RegParams.DefRx1DataRate;
	return LORAWAN_SUCCESS;
}

static StackRetStatus_t LORAREG_GetAttr_DefRx2DataRate(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	*(uint8_t *)attrOutput = RegParams.DefRx2DataRate;
	return LORAWAN_SUCCESS;
}

static StackRetStatus_t LORAREG_GetAttr_DefRx2Freq(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	memcpy(attrOutput,&RegParams.DefRx2Freq,sizeof(uint32_t));
	//*(uint32_t *)attrOutput = DefRx2Freq;
	return LORAWAN_SUCCESS;
}

static StackRetStatus_t LORAREG_GetAttr_RegFeatures(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	*(uint32_t *)attrOutput = RegParams.FeaturesSupport;
	return LORAWAN_SUCCESS;
}

static StackRetStatus_t LORAREG_GetAttr_DataRange(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint8_t  channelId;
	ValChId_t valChid;
	valChid.channelIndex = *(uint8_t *)attrInput;
	valChid.allowedForDefaultChannels = ALL_CHANNELS;
	channelId = *(uint8_t *)attrInput;
	if (pValidateAttr[CHANNEL_ID](CHANNEL_ID, &valChid) == LORAWAN_SUCCESS)
	{
	    *(uint8_t *)attrOutput = RegParams.pChParams[channelId].dataRange.value;
	}
	else
	{
		*(uint8_t *)attrOutput = 0xFF;
	    result = LORAWAN_INVALID_PARAMETER;
	}
	return result;
}

static StackRetStatus_t LORAREG_GetAttr_MinMaxDr(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	MinMaxDr_t minmaxDr;
	UpdateMinMaxChDataRate(&minmaxDr);
	memcpy(attrOutput,&minmaxDr,sizeof(MinMaxDr_t));
	return result;
}

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_DRangeChBandT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	BandDrReq_t bandDrReq;
	DataRange_t chBandDr;

	memcpy(&bandDrReq,(BandDrReq_t *)attrInput,sizeof(BandDrReq_t));
	if(ValidateChannelMaskCntl(CHANNEL_MASK_CNTL, (void *)&bandDrReq.chnlMaskCntl)	!= LORAWAN_SUCCESS)
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		chBandDr = getChBandDrT1(bandDrReq.chnlMaskCntl,bandDrReq.chnlMask);
		memcpy(attrOutput,&chBandDr,sizeof(DataRange_t));
	}
	return result;
}
#endif

#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_DRangeChBandT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	BandDrReq_t bandDrReq;
	DataRange_t chBandDr;

	memcpy(&bandDrReq,(BandDrReq_t *)attrInput,sizeof(BandDrReq_t));
	/*if(ValidateChannelMaskCntl((void *)&bandDrReq.chnlMaskCntl)	!= LORAWAN_SUCCESS)
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	else
	{*/
		chBandDr = getChBandDrT2(bandDrReq.chnlMaskCntl,bandDrReq.chnlMask);
		memcpy(attrOutput,&chBandDr,sizeof(DataRange_t));
	//}
	return result;
}
#endif

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_Rx1WindowparamsType1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	
	Rx1WindowParamsReq_t rx1WindowParamsReq;
			
	Rx1WindowParams_t rx1WindowParamsResp;

	memcpy(&rx1WindowParamsReq,(Rx1WindowParamsReq_t *)attrInput,sizeof(Rx1WindowParamsReq_t));
			
	getRx1WindowparamsType1(&rx1WindowParamsReq,&rx1WindowParamsResp);

	memcpy(attrOutput,&rx1WindowParamsResp,sizeof(Rx1WindowParams_t));
	
	return result;
}
#endif
static StackRetStatus_t LORAREG_GetAttr_JoinDutyCycleRemainingTime(LorawanRegionalAttributes_t attr,void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	
	uint32_t timeremaining =0;
	if(SwTimerIsRunning(RegParams.pJoinDutyCycleTimer->timerId))
	{
		timeremaining = US_TO_MS(SwTimerReadValue (RegParams.pJoinDutyCycleTimer->timerId));
		timeremaining = timeremaining + RegParams.pJoinDutyCycleTimer->remainingtime;
	}
	memcpy(attrOutput,&timeremaining,sizeof(uint32_t));
	return result;
}

#if (EU_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_Rx1WindowparamsType2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	
	Rx1WindowParamsReq_t rx1WindowParamsReq;
	
	Rx1WindowParams_t rx1WindowParamsResp;

	memcpy(&rx1WindowParamsReq,(Rx1WindowParamsReq_t *)attrInput,sizeof(Rx1WindowParamsReq_t));
	
	getRx1WindowparamsType2(&rx1WindowParamsReq,&rx1WindowParamsResp);

	memcpy(attrOutput,&rx1WindowParamsResp,sizeof(Rx1WindowParams_t));
	
	return result;
}
#endif

#if (IND_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_Rx1WindowparamsType3(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	
	Rx1WindowParamsReq_t rx1WindowParamsReq;
	
	Rx1WindowParams_t rx1WindowParamsResp;

	memcpy(&rx1WindowParamsReq,(Rx1WindowParamsReq_t *)attrInput,sizeof(Rx1WindowParamsReq_t));
	
	getRx1WindowparamsType3(&rx1WindowParamsReq,&rx1WindowParamsResp);

	memcpy(attrOutput,&rx1WindowParamsResp,sizeof(Rx1WindowParams_t));
	
	return result;
}
#endif

#if (AS_BAND == 1 || JPN_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_Rx1WindowparamsType4(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	
	Rx1WindowParamsReq_t rx1WindowParamsReq;
	
	Rx1WindowParams_t rx1WindowParamsResp;

	memcpy(&rx1WindowParamsReq,(Rx1WindowParamsReq_t *)attrInput,sizeof(Rx1WindowParamsReq_t));
	
	getRx1WindowparamsType4(&rx1WindowParamsReq,&rx1WindowParamsResp);

	memcpy(attrOutput,&rx1WindowParamsResp,sizeof(Rx1WindowParams_t));
	
	return result;
}
#endif

#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_DlFrequency(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint8_t  channelId;
	channelId = *(uint8_t *)attrInput;
	if (channelId > RegParams.maxChannels)
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		memcpy(attrOutput,&(RegParams.pOtherChParams[channelId].rx1Frequency),sizeof(uint32_t));
	}
	return result;
}
#endif


static StackRetStatus_t LORAREG_GetAttr_ChIdStatus(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	ValChId_t val_chid;
	uint8_t  channelId;
	channelId = *(uint8_t *)attrInput;
	val_chid.channelIndex = *(uint8_t *)attrInput;
	val_chid.allowedForDefaultChannels = ALL_CHANNELS;
	
	if (pValidateAttr[CHANNEL_ID](CHANNEL_ID, &val_chid) == LORAWAN_SUCCESS)
	{
		*(uint8_t *)attrOutput = RegParams.pChParams[channelId].status;
	}
	else
	{
		*(uint8_t *)attrOutput = DISABLED;
		result = LORAWAN_INVALID_PARAMETER;
	}
	return result;
}

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_DutyCycleT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	*(uint16_t *)attrOutput = UINT16_MAX;
	return LORAWAN_INVALID_PARAMETER;
}
#endif

#if (EU_BAND == 1) || (AS_BAND == 1) || (JPN_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_DutyCycleT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
    uint8_t  channelId;
    uint8_t subBandId;
	ValChId_t valChid;
	
	valChid.channelIndex = *(uint8_t *)attrInput;
	valChid.allowedForDefaultChannels = ALL_CHANNELS;
    channelId = *(uint8_t *)attrInput;
    if (ValidateChannelIdT2(CHANNEL_ID, &valChid) == LORAWAN_SUCCESS)
    {
	    subBandId = RegParams.cmnParams.paramsType2.othChParams[channelId].subBandId;
	    *(uint16_t *)attrOutput = RegParams.cmnParams.paramsType2.subBandDutyCycle[subBandId];
    }
    else
    {
	    result = LORAWAN_INVALID_PARAMETER;
    }
	return result;
}
#endif

#if (EU_BAND == 1) || (AS_BAND == 1) || (JPN_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_DutyCycleTimer(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
    //To get the next free channel when duty cycle is enabled
    uint32_t minimSubBandTimer = UINT32_MAX;
    uint32_t minDutyCycleTimer,ticks,delta;
    uint8_t bandId;
    uint8_t  currentDataRate;
    currentDataRate = *(uint8_t *)attrInput;

    for (uint8_t i = 0; i < RegParams.maxChannels; i++)
    {
	    if ( (RegParams.pChParams[i].status == ENABLED) )
	    {
		    bandId = RegParams.cmnParams.paramsType2.othChParams[i].subBandId;
		        
		    if((RegParams.pSubBandParams[bandId].subBandTimeout != 0) && 
			   (RegParams.pSubBandParams[bandId].subBandTimeout <= minimSubBandTimer) && 
			   (currentDataRate >= RegParams.pChParams[i].dataRange.min) && 
			   (currentDataRate <= RegParams.pChParams[i].dataRange.max) )
		    {
			    minimSubBandTimer = RegParams.pSubBandParams[bandId].subBandTimeout;
		    }
	    }
    }
        
    if((UINT32_MAX != minimSubBandTimer) && (minimSubBandTimer >= RegParams.cmnParams.paramsType2.aggregatedDutyCycleTimeout))
    {
	    minDutyCycleTimer = minimSubBandTimer;
    }
    else
    {
	    minDutyCycleTimer = RegParams.cmnParams.paramsType2.aggregatedDutyCycleTimeout;
    }
        
    ticks = SwTimerReadValue (RegParams.pDutyCycleTimer->timerId);
    delta = RegParams.pDutyCycleTimer->lastTimerValue - US_TO_MS(ticks);
    
	if( minDutyCycleTimer != 0)
	{
		/*Get the time left for the band timer which supports the requested data rate to expire*/
		minDutyCycleTimer = minDutyCycleTimer - delta; //Logically delta will not be greater than minDcTimer
	}
        
    memcpy(attrOutput,&minDutyCycleTimer,sizeof(uint32_t));
	
	return result;
}
#endif

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_MinDutyCycleTimer(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	*(uint32_t *)attrOutput = UINT32_MAX;
	return LORAWAN_INVALID_PARAMETER;
}
#endif

static StackRetStatus_t LORAREG_GetAttr_MacRecvDelay1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	*(uint16_t *)attrOutput = RECEIVE_DELAY1;
	return LORAWAN_SUCCESS;
}

static StackRetStatus_t LORAREG_GetAttr_MacRecvDelay2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	*(uint16_t *)attrOutput = RECEIVE_DELAY2;
	return LORAWAN_SUCCESS;
}

static StackRetStatus_t LORAREG_GetAttr_MacJoinAcptDelay1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	*(uint16_t *)attrOutput = JOIN_ACCEPT_DELAY1;
	return LORAWAN_SUCCESS;
}

static StackRetStatus_t LORAREG_GetAttr_MacJoinAcptDelay2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	*(uint16_t *)attrOutput = JOIN_ACCEPT_DELAY2;
	return LORAWAN_SUCCESS;
}

static StackRetStatus_t LORAREG_GetAttr_MacRetransmitTimeout(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	*(uint16_t *)attrOutput = RETRANSMIT_TIMEOUT;
	return LORAWAN_SUCCESS;
}

static StackRetStatus_t LORAREG_GetAttr_MacAdrAckDelay(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	*(uint8_t *)attrOutput = ADR_ACK_DELAY;
	return LORAWAN_SUCCESS;
}

static StackRetStatus_t LORAREG_GetAttr_MacAdrAckLimit(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	*(uint8_t *)attrOutput = ADR_ACK_LIMIT;
	return LORAWAN_SUCCESS;
}

static StackRetStatus_t LORAREG_GetAttr_RegDefTxPwr(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	*(uint8_t *)attrOutput = RegParams.MacTxPower;
	return LORAWAN_SUCCESS;
}

static StackRetStatus_t LORAREG_GetAttr_DefTxPwr(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	*(uint8_t *)attrOutput = RegParams.defTxPwrIndx;
	return LORAWAN_SUCCESS;
}

static StackRetStatus_t LORAREG_GetAttr_RegDefTxDR(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	*(uint8_t *)attrOutput = RegParams.TxCurDataRate;
	return LORAWAN_SUCCESS;
}

static StackRetStatus_t LORAREG_GetAttr_CurChIndx(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	*(uint8_t *)attrOutput = RegParams.lastUsedChannelIndex;
	return LORAWAN_SUCCESS;
}

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_NewTxChConfigT1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	NewTxChannelReq_t newTxChannelReq;

	memcpy(&newTxChannelReq,(NewTxChannelReq_t *)attrInput,sizeof(NewTxChannelReq_t));
	if((ValidateDataRateTxT1(TX_DATARATE, (void *)&newTxChannelReq.currDr) != LORAWAN_SUCCESS) || (ValidateTxPower(TX_PWR, (void *)&newTxChannelReq.txPwr) != LORAWAN_SUCCESS))
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		result = GetTxChannelConfig1(newTxChannelReq.transmissionType,newTxChannelReq.txPwr,newTxChannelReq.currDr,(radioConfig_t*)attrOutput);
	}
	return result;
}
#endif

#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_NewTxChConfigT2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	NewTxChannelReq_t newTxChannelReq;

	memcpy(&newTxChannelReq,(NewTxChannelReq_t *)attrInput,sizeof(NewTxChannelReq_t));
	if((ValidateDataRate(RX_DATARATE, (void *)&newTxChannelReq.currDr) != LORAWAN_SUCCESS) || (ValidateTxPower(TX_PWR,  (void *)&newTxChannelReq.txPwr) != LORAWAN_SUCCESS))
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		result = GetTxChannelConfig2(newTxChannelReq.transmissionType,newTxChannelReq.txPwr,newTxChannelReq.currDr,(radioConfig_t*)attrOutput);
	}
	return result;

}
#endif

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_FreeChannel1(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	NewFreeChannelReq_t newFreeChannelReq;

	memcpy(&newFreeChannelReq,(NewFreeChannelReq_t *)attrInput,sizeof(NewFreeChannelReq_t));
	
	if(newFreeChannelReq.maxChannels > RegParams.maxChannels)
	{
		return LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		result = SearchAvailableChannel1(newFreeChannelReq.maxChannels,newFreeChannelReq.transmissionType,newFreeChannelReq.currDr,(uint8_t*)attrOutput);
	}
	
	return result;
}
#endif

#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_FreeChannel2(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	NewFreeChannelReq_t newFreeChannelReq;

	memcpy(&newFreeChannelReq,(NewFreeChannelReq_t *)attrInput,sizeof(NewFreeChannelReq_t));
	
	if(newFreeChannelReq.maxChannels > RegParams.maxChannels)
	{
		return LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		result = SearchAvailableChannel2(newFreeChannelReq.maxChannels,newFreeChannelReq.transmissionType,newFreeChannelReq.currDr,(uint8_t*)attrOutput);
	}
	
	return result;
}
#endif

#if (JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_DefLBTParams(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	LorawanLBTParams_t* lorawanLBTParams;
	lorawanLBTParams = (LorawanLBTParams_t *)attrOutput;
			
	lorawanLBTParams->lbtNumOfSamples	= RegParams.cmnParams.paramsType2.LBT_RSSISamplesCount;
	lorawanLBTParams->lbtScanPeriod		= RegParams.cmnParams.paramsType2.LBTScanPeriod;
	lorawanLBTParams->lbtThreshold		= RegParams.cmnParams.paramsType2.LBTSignalThreshold;
	lorawanLBTParams->lbtTransmitOn		= LBT_ENABLE;
	lorawanLBTParams->maxRetryChannels	= LBT_MAX_RETRY_CHANNELS;
	return 0;
}
#endif

#if (JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t LORAREG_GetAttr_minLBTChPauseTimer(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput)
{
	StackRetStatus_t retVal = LORAWAN_SUCCESS;
	uint32_t minim = UINT32_MAX;
	uint8_t  currentDataRate;
	currentDataRate = *(uint8_t *)attrInput;
	for (uint8_t i = 0; i < RegParams.maxChannels; i++)
	{
		if ( (RegParams.pChParams[i].status == ENABLED) && (RegParams.cmnParams.paramsType2.channelTimer[i] != 0) 
		     && (RegParams.cmnParams.paramsType2.channelTimer[i] <= minim) 
			 && (currentDataRate >= RegParams.pChParams[i].dataRange.min) 
			 && (currentDataRate <= RegParams.pChParams[i].dataRange.max) )
		{
			minim = RegParams.cmnParams.paramsType2.channelTimer[i];
		}
	}
	memcpy(attrOutput,&minim,sizeof(uint32_t));
	return retVal;
}
#endif

#if(NA_BAND == 1)
void LORAREG_InitValidateAttrFnPtrsNA(void)
{
    pValidateAttr[RX_FREQUENCY] = ValidateRxFreqT1;
    pValidateAttr[TX_FREQUENCY] = ValidateTxFreqT1;
    pValidateAttr[RX_DATARATE] = ValidateDataRateRxT1;
    pValidateAttr[TX_DATARATE] = ValidateDataRateTxT1;
    pValidateAttr[SUPPORTED_DR] = ValidateSupportedDr;
    pValidateAttr[DATA_RANGE] = ValidateDataRange;
    pValidateAttr[CHANNEL_ID] = ValidateChannelId;
    pValidateAttr[RX2_DATARATE] = ValidateDataRateRxT1;
    pValidateAttr[RX1_DATARATE_OFFSET] = ValidateRx1DataRateOffset;
    pValidateAttr[CHANNEL_MASK] = ValidateChannelMask;
    pValidateAttr[CHANNEL_MASK_CNTL] = ValidateChannelMaskCntl;
    pValidateAttr[TX_PWR] = ValidateTxPower;
    pValidateAttr[CHMASK_CHCNTL] = ValidateChMaskChCntl;
}
#endif

#if(EU_BAND == 1)
void LORAREG_InitValidateAttrFnPtrsEU(void)
{
	pValidateAttr[RX_FREQUENCY] = ValidateFreq;
	pValidateAttr[TX_FREQUENCY] = ValidateTxFreqT2;
	pValidateAttr[RX_DATARATE] = ValidateDataRate;
	pValidateAttr[TX_DATARATE] = ValidateDataRate;
	pValidateAttr[SUPPORTED_DR] = ValidateSupportedDr;
	pValidateAttr[DATA_RANGE] = ValidateDataRangeT2;
	pValidateAttr[CHANNEL_ID] = ValidateChannelIdT2;
	pValidateAttr[RX2_DATARATE] = ValidateDataRate;
	pValidateAttr[RX1_DATARATE_OFFSET] = ValidateRx1DataRateOffset;
	pValidateAttr[CHANNEL_MASK] = ValidateChannelMaskT2;
	pValidateAttr[CHANNEL_MASK_CNTL] = ValidateChannelMaskCntlT2;
	pValidateAttr[TX_PWR] = ValidateTxPower;
	pValidateAttr[CHMASK_CHCNTL] = ValidateChMaskChCntlT2;
}
#endif

#if(AS_BAND == 1 )
void LORAREG_InitValidateAttrFnPtrsAS(void)
{
	pValidateAttr[RX_FREQUENCY] = ValidateFrequencyAS;
	pValidateAttr[TX_FREQUENCY] = ValidateTxFreqT2;
	pValidateAttr[RX_DATARATE] = ValidateDataRateRxT2;
	pValidateAttr[TX_DATARATE] = ValidateDataRateTxT2;
	pValidateAttr[SUPPORTED_DR] = ValidateSupportedDr;
	pValidateAttr[DATA_RANGE] = ValidateDataRangeT2;
	pValidateAttr[CHANNEL_ID] = ValidateChannelIdT2;
	pValidateAttr[RX2_DATARATE] = ValidateDataRateRxT2;
	pValidateAttr[RX1_DATARATE_OFFSET] = ValidateRx1DataRateOffset;
	pValidateAttr[CHANNEL_MASK] = ValidateChannelMaskT2;
	pValidateAttr[CHANNEL_MASK_CNTL] = ValidateChannelMaskCntlT2;
	pValidateAttr[TX_PWR] = ValidateTxPower;
	pValidateAttr[CHMASK_CHCNTL] = ValidateChMaskChCntlT2;
}
#endif

#if(AU_BAND == 1)
void LORAREG_InitValidateAttrFnPtrsAU(void)
{
	pValidateAttr[RX_FREQUENCY] = ValidateRxFreqT1;
	pValidateAttr[TX_FREQUENCY] = ValidateTxFreqT1;
	pValidateAttr[RX_DATARATE] = ValidateDataRateRxT1;
	pValidateAttr[TX_DATARATE] = ValidateDataRateTxT1;
	pValidateAttr[SUPPORTED_DR] = ValidateSupportedDr;
	pValidateAttr[DATA_RANGE] = ValidateDataRange;
	pValidateAttr[CHANNEL_ID] = ValidateChannelId;
	pValidateAttr[RX2_DATARATE] = ValidateDataRateRxT1;
	pValidateAttr[RX1_DATARATE_OFFSET] = ValidateRx1DataRateOffset;
	pValidateAttr[CHANNEL_MASK] = ValidateChannelMask;
	pValidateAttr[CHANNEL_MASK_CNTL] = ValidateChannelMaskCntl;
	pValidateAttr[TX_PWR] = ValidateTxPower;
	pValidateAttr[CHMASK_CHCNTL] = ValidateChMaskChCntl;
}
#endif

#if(IND_BAND == 1)
void LORAREG_InitValidateAttrFnPtrsIN(void)
{
	pValidateAttr[RX_FREQUENCY] = ValidateFreqIN;
	pValidateAttr[TX_FREQUENCY] = ValidateTxFreqT2;
	pValidateAttr[RX_DATARATE] = ValidateDataRate;
	pValidateAttr[TX_DATARATE] = ValidateDataRate;
	pValidateAttr[SUPPORTED_DR] = ValidateSupportedDr;
	pValidateAttr[DATA_RANGE] = ValidateDataRangeT2;
	pValidateAttr[CHANNEL_ID] = ValidateChannelIdT2;
	pValidateAttr[RX2_DATARATE] = ValidateDataRate;
	pValidateAttr[RX1_DATARATE_OFFSET] = ValidateRx1DataRateOffset;
	pValidateAttr[CHANNEL_MASK] = ValidateChannelMaskT2;
	pValidateAttr[CHANNEL_MASK_CNTL] = ValidateChannelMaskCntlT2;
	pValidateAttr[TX_PWR] = ValidateTxPower;
	pValidateAttr[CHMASK_CHCNTL] = ValidateChMaskChCntlT2;
}
#endif

#if( JPN_BAND == 1)
void LORAREG_InitValidateAttrFnPtrsJP(void)
{
	pValidateAttr[RX_FREQUENCY] = ValidateFreqJP;
	pValidateAttr[TX_FREQUENCY] = ValidateTxFreqT2;
	pValidateAttr[RX_DATARATE] = ValidateDataRateRxT2;
	pValidateAttr[TX_DATARATE] = ValidateDataRateTxT2;
	pValidateAttr[SUPPORTED_DR] = ValidateSupportedDr;
	pValidateAttr[DATA_RANGE] = ValidateDataRangeT2;
	pValidateAttr[CHANNEL_ID] = ValidateChannelIdT2;
	pValidateAttr[RX2_DATARATE] = ValidateDataRateRxT2;
	pValidateAttr[RX1_DATARATE_OFFSET] = ValidateRx1DataRateOffset;
	pValidateAttr[CHANNEL_MASK] = ValidateChannelMaskT2;
	pValidateAttr[CHANNEL_MASK_CNTL] = ValidateChannelMaskCntlT2;
	pValidateAttr[TX_PWR] = ValidateTxPower;
	pValidateAttr[CHMASK_CHCNTL] = ValidateChMaskChCntlT2;
}
#endif

#if(KR_BAND == 1)
void LORAREG_InitValidateAttrFnPtrsKR(void)
{
	pValidateAttr[RX_FREQUENCY] = ValidateFreqKR;
	pValidateAttr[TX_FREQUENCY] = ValidateTxFreqT2;
	pValidateAttr[RX_DATARATE] = ValidateDataRate;
	pValidateAttr[TX_DATARATE] = ValidateDataRate;
	pValidateAttr[SUPPORTED_DR] = ValidateSupportedDr;
	pValidateAttr[DATA_RANGE] = ValidateDataRangeT2;
	pValidateAttr[CHANNEL_ID] = ValidateChannelIdT2;
	pValidateAttr[RX2_DATARATE] = ValidateDataRate;
	pValidateAttr[RX1_DATARATE_OFFSET] = ValidateRx1DataRateOffset;
	pValidateAttr[CHANNEL_MASK] = ValidateChannelMaskT2;
	pValidateAttr[CHANNEL_MASK_CNTL] = ValidateChannelMaskCntlT2;
	pValidateAttr[TX_PWR] = ValidateTxPower;
	pValidateAttr[CHMASK_CHCNTL] = ValidateChMaskChCntlT2;
}
#endif

#if(NA_BAND == 1)
void LORAREG_InitSetAttrFnPtrsNA(void)
{
	pSetAttr[DATA_RANGE] = setDataRange;
	pSetAttr[CHANNEL_ID_STATUS] = setChannelIdStatus;
	pSetAttr[NEW_CHANNELS] = setNewChannelsT1;
	pSetAttr[REG_JOIN_SUCCESS] = setJoinSuccess;
	pSetAttr[REG_JOIN_ENABLE_ALL] = setEnableAllChs;
	pSetAttr[JOIN_DUTY_CYCLE_TIMER] = setJoinDutyCycleTimer;
	pSetAttr[JOIN_BACK_OFF_TIMER] = setJoinBackOffTimer;
	pSetAttr[JOINBACKOFF_CNTL] = setJoinBackoffCntl;
}
#endif

#if(EU_BAND == 1)
void LORAREG_InitSetAttrFnPtrsEU(void)
{
	pSetAttr[DATA_RANGE] = setDataRangeT2;
	pSetAttr[CHANNEL_ID_STATUS] = setChannelIdStatusT2;
	pSetAttr[DUTY_CYCLE] = setDutyCycle;
	pSetAttr[DUTY_CYCLE_TIMER] = setDutyCycleTimer;
	pSetAttr[FREQUENCY] = setFrequency;
	pSetAttr[NEW_CHANNELS] = setNewChannel;
	pSetAttr[JOIN_DUTY_CYCLE_TIMER]= setJoinDutyCycleTimer;
	pSetAttr[JOIN_BACK_OFF_TIMER] = setJoinBackOffTimer;
	pSetAttr[DL_FREQUENCY] = setDlFrequency;
	pSetAttr[JOINBACKOFF_CNTL] = setJoinBackoffCntl;
	pSetAttr[CHLIST_DEFAULTS] = setChlistDefaultState;
}
#endif

#if(AS_BAND == 1)
void LORAREG_InitSetAttrFnPtrsAS(void)
{
	pSetAttr[DATA_RANGE] = setDataRangeT2;
	pSetAttr[CHANNEL_ID_STATUS] = setChannelIdStatusT3;
	pSetAttr[FREQUENCY] = setFrequency;
	pSetAttr[DL_FREQUENCY] = setDlFrequency;
    pSetAttr[NEW_CHANNELS] = setNewChannel;
	pSetAttr[JOIN_DUTY_CYCLE_TIMER] = setJoinDutyCycleTimer;
	pSetAttr[JOIN_BACK_OFF_TIMER] = setJoinBackOffTimer;
	pSetAttr[TX_PARAMS] = setTxParams;
	pSetAttr[DUTY_CYCLE] = setDutyCycle;
	pSetAttr[DUTY_CYCLE_TIMER] = setDutyCycleTimer;
	pSetAttr[JOINBACKOFF_CNTL] = setJoinBackoffCntl;
	pSetAttr[CHLIST_DEFAULTS] = setChlistDefaultState;
}
#endif

#if(AU_BAND == 1)
void LORAREG_InitSetAttrFnPtrsAU(void)
{
	pSetAttr[DATA_RANGE] = setDataRange;
	pSetAttr[CHANNEL_ID_STATUS] = setChannelIdStatus;
	pSetAttr[NEW_CHANNELS] = setNewChannelsT1;
	pSetAttr[REG_JOIN_SUCCESS] = setJoinSuccess;
	pSetAttr[REG_JOIN_ENABLE_ALL] = setEnableAllChs;
	pSetAttr[JOIN_DUTY_CYCLE_TIMER]= setJoinDutyCycleTimer;
	pSetAttr[JOIN_BACK_OFF_TIMER] = setJoinBackOffTimer;
	pSetAttr[JOINBACKOFF_CNTL] = setJoinBackoffCntl;
}
#endif

#if(IND_BAND == 1)
void LORAREG_InitSetAttrFnPtrsIN(void)
{
	pSetAttr[DATA_RANGE] = setDataRangeT2;
	pSetAttr[CHANNEL_ID_STATUS] = setChannelIdStatusT2;
	pSetAttr[FREQUENCY] = setFrequency;
	pSetAttr[NEW_CHANNELS] = setNewChannel;
	pSetAttr[JOIN_DUTY_CYCLE_TIMER]= setJoinDutyCycleTimer;
	pSetAttr[JOIN_BACK_OFF_TIMER] = setJoinBackOffTimer;
	pSetAttr[DL_FREQUENCY] = setDlFrequency;
	pSetAttr[TX_PARAMS] = setTxParams;
	pSetAttr[JOINBACKOFF_CNTL] = setJoinBackoffCntl;
	pSetAttr[CHLIST_DEFAULTS] = setChlistDefaultState;
}
#endif

#if(JPN_BAND == 1)
void LORAREG_InitSetAttrFnPtrsJP(void)
{
	pSetAttr[DATA_RANGE] = setDataRangeT2;
	pSetAttr[CHANNEL_ID_STATUS] = setChannelIdStatusT3;
	pSetAttr[LBT_TIMER] = setLBTTimer;
	pSetAttr[FREQUENCY] = setFrequency;
	pSetAttr[DL_FREQUENCY] = setDlFrequency;
	pSetAttr[NEW_CHANNELS] = setNewChannel;
	pSetAttr[JOIN_DUTY_CYCLE_TIMER]= setJoinDutyCycleTimer;
	pSetAttr[JOIN_BACK_OFF_TIMER] = setJoinBackOffTimer;
	pSetAttr[TX_PARAMS] = setTxParams;
	pSetAttr[DUTY_CYCLE] = setDutyCycle;
	pSetAttr[DUTY_CYCLE_TIMER] = setDutyCycleTimer;
	pSetAttr[JOINBACKOFF_CNTL] = setJoinBackoffCntl;
	pSetAttr[CHLIST_DEFAULTS] = setChlistDefaultState;
}
#endif

#if(KR_BAND == 1)
void LORAREG_InitSetAttrFnPtrsKR(void)
{
	pSetAttr[DATA_RANGE] = setDataRangeT2;
	pSetAttr[CHANNEL_ID_STATUS] = setChannelIdStatusT3;
	pSetAttr[LBT_TIMER] = setLBTTimer;
	pSetAttr[FREQUENCY] = setFrequency;
	pSetAttr[DL_FREQUENCY] = setDlFrequency;
	pSetAttr[NEW_CHANNELS] = setNewChannel;
	pSetAttr[JOIN_DUTY_CYCLE_TIMER]= setJoinDutyCycleTimer;
	pSetAttr[JOIN_BACK_OFF_TIMER] = setJoinBackOffTimer;
	pSetAttr[TX_PARAMS] = setTxParams;
	pSetAttr[JOINBACKOFF_CNTL] = setJoinBackoffCntl;
	pSetAttr[CHLIST_DEFAULTS] = setChlistDefaultState;
}
#endif

/** 
 * \brief This function initializes the LoRaWAN Regional Parameters Module
 * All the resources handled by the regional module eg Channels will be initialized.
 * \param ismBand The Regional band which is initialized,This is used to validate if
 *	the band is supported by the module and also initializes the module
 *	corresponding to the band
 * \retval LORAWAN_SUCCESS : If initialization is successful
 *	LORAWAN_INVALID_PARAMETER if Initialization is failed
 */
StackRetStatus_t LORAREG_Init(IsmBand_t ismBand)
{	
	StackRetStatus_t result = LORAWAN_SUCCESS;
	static bool initialized  = false;
	
	if(!initialized)
	{		
		if (LORAWAN_SUCCESS == CreateAllRegSoftwareTimers())
		{
//			SetCallbackRegSoftwareTimers();		
			initialized = true;
		}
		else
		{
			result = LORAWAN_RESOURCE_UNAVAILABLE;
		}
	}
	else
	{
		StopAllRegSoftwareTimers();	
	}

	for(int i = 0; i < REG_NUM_ATTRIBUTES; i++)
	{
		pGetAttr[i] = InValidGetAttr;
		pValidateAttr[i] = InValidAttr;
		pSetAttr[i] = InValidAttr;
	}
	
	if(ismBand == ISM_NA915)
	{
	    result = LORAReg_InitNA(ismBand);
	}
	else if(ismBand == ISM_EU868 || ismBand == ISM_EU433)
	{
	    result = LORAReg_InitEU(ismBand);		
	}
	else if(ismBand >= ISM_BRN923 && ismBand <= ISM_VTM923)
	{
		result = LORAReg_InitAS(ismBand);
	}
	else if(ismBand == ISM_AU915)
	{
		result = LORAReg_InitAU(ismBand);
	}
	else if(ismBand == ISM_IND865)
	{
		result = LORAReg_InitIN(ismBand);
	}
	else if(ismBand == ISM_JPN923)
	{
		result = LORAReg_InitJP(ismBand);
	}
	else if(ismBand == ISM_KR920)
	{
		result = LORAReg_InitKR(ismBand);
	}
	
	return result;
}

/** 
 * \brief This API provides read access to all the supported Regional Attributes.
 *  The LoRaWAN mac needs to access the regional database via this API.
 * \param[in] attrType Type of Attribute to get from the regional module
 * \param[in] attrInput Input used to get the attribute value
 * \param[out] Result of the GET operation
 * \retval LORAWAN_SUCCESS : If GET is successful
 *	LORAWAN_INVALID_PARAMETER if the input attribute or the attribute type is invalid
 */
StackRetStatus_t LORAREG_GetAttr(LorawanRegionalAttributes_t attrType, void *attrInput, void *attrOutput)
{
	StackRetStatus_t result = LORAWAN_INVALID_PARAMETER;
	if(attrType < REG_NUM_ATTRIBUTES)
	{
	    result = pGetAttr[attrType](attrType, attrInput,attrOutput);	
	}

	return result;
}

/** 
 * \brief This API is used to validate if the parameter is according to the
 *  specification defined for that band.
 * \param[in] attrType Type of Attribute to validate in the regional module
 * \param[in] attrInput Input used to validate the attribute value
 * \retval LORAWAN_SUCCESS : If Validate is successful
 *	LORAWAN_INVALID_PARAMETER if the input attribute or the attribute type is invalid
 */
StackRetStatus_t LORAREG_ValidateAttr(LorawanRegionalAttributes_t attrType, void *attrInput)
{
	StackRetStatus_t result = LORAWAN_INVALID_PARAMETER;
	
	if(attrType < REG_NUM_ATTRIBUTES)
	{
	    result = pValidateAttr[attrType](attrType, attrInput);		
	}

	return result;
}

/** 
 * \brief This API is used to Update the parameters to the regional parameter data base. 
 * \param[in] attrType Type of Attribute to update in the regional module
 * \param[in] attrInput Input used to update the attribute value
 * \retval LORAWAN_SUCCESS : If Update is successful
 *	LORAWAN_INVALID_PARAMETER if the input attribute or the attribute type is invalid
 */
StackRetStatus_t LORAREG_SetAttr(LorawanRegionalAttributes_t attrType, void *attrInput)
{
	StackRetStatus_t result = LORAWAN_INVALID_PARAMETER;
	if(attrType < REG_NUM_ATTRIBUTES)
	{
	    result = pSetAttr[attrType](attrType, attrInput);		
	}

	return result;
}

/*
 * \brief  Generates a frequency for US upstream,
 * utilizing LoRa 125 kHz BW varying from SF7 to SF10 (DR0..DR3) starting at 902.3 MHz 
 * and incrementing linearly by 200 kHz to 914.9 MHz
 */
#if (NA_BAND == 1 || AU_BAND == 1)
static uint32_t GenerateFrequency1 (uint8_t channelIndex)   // channelIndex should be a random number between 0 and 63, generated with the random function
{
    uint32_t channelFrequency;

    channelFrequency = RegParams.cmnParams.paramsType1.UpStreamCh0Freq + FREQ_200KHZ * channelIndex;
    return channelFrequency;
}
#endif

/*
 * \brief  Generates a frequency for US upstream,
 * utilizing LoRa 500 kHz BW at SF8 (DR4) starting at 903.0 MHz and 
 * incrementing linearly by 1.6 MHz to 914.2 MHz
 */
#if (NA_BAND == 1 || AU_BAND == 1)
static uint32_t GenerateFrequency2 (uint8_t channelIndex)
{
    uint32_t channelFrequency;

    channelFrequency = RegParams.cmnParams.paramsType1.UpStreamCh64Freq + 
	                   FREQ_1600KHZ * (channelIndex - RegParams.cmnParams.paramsType1.Max_125khzChan);
    return channelFrequency;
}
#endif


/*
 * \brief Validates if the TX Datarate is supported by the regional band
 * \param[in] dataRate Data rate to be validated
 * \retval LORAWAN_SUCCESS If the validation is successful
 *		   LORAWAN_INVALID_PARAMETER If the data rate is not supported
 */
#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t ValidateDataRateTxT1 (LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	
	uint8_t dataRate = *(uint8_t *)attrInput;

	if (dataRate > RegParams.cmnParams.paramsType1.maxTxDR)
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	
	return result;
}
#endif

#if (AS_BAND == 1 || JPN_BAND == 1)
static StackRetStatus_t ValidateDataRateTxT2(LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	
	uint8_t dataRate = *(uint8_t *)attrInput;
	
	if(dataRate > RegParams.minDataRate ||
	(RegParams.cmnParams.paramsType2.txParams.uplinkDwellTime == 1 && dataRate < RegParams.DefRx1DataRate))
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	
	return result;
}
#endif

/*
 * \brief Validates if the RX Datarate is supported by the regional band
 * \param[in] dataRate Data rate to be validated
 * \retval LORAWAN_SUCCESS If the validation is successful
 *		   LORAWAN_INVALID_PARAMETER If the data rate is not supported
 */

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t ValidateDataRateRxT1 (LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	
	uint8_t dataRate = *(uint8_t *)attrInput;

	if((dataRate > RegParams.cmnParams.paramsType1.maxRxDR) || (dataRate < RegParams.cmnParams.paramsType1.minRxDR) )
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	return result;
}
#endif

#if (AS_BAND == 1 || JPN_BAND == 1)
static StackRetStatus_t ValidateDataRateRxT2 (LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	
	uint8_t dataRate = *(uint8_t *)attrInput;

    if(dataRate > RegParams.minDataRate || (
	RegParams.cmnParams.paramsType2.txParams.downlinkDwellTime == 1 && dataRate < RegParams.DefRx1DataRate))
	{
		result = LORAWAN_INVALID_PARAMETER;
	}

	return result;
}
#endif

/*
 * \brief Validates if the Channel Id is supported by the regional band
 * \param[in] channelId Index of the channel to be validated 
 * \param[in] allowedForDefaultChannels If the Channel Id is default and changing the channel is allowed
 * \retval LORAWAN_SUCCESS If the validation is successful
 *		   LORAWAN_INVALID_PARAMETER If the Channel Id is not supported
 */
#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t ValidateChannelId (LorawanRegionalAttributes_t attr, void *attrInput)  //if allowedForDefaultChannels is 1, all the channels can be modified, if it is 0 channels 0, 1, 2 and 16, 17, and 18 (dual band) cannot be modified
{
    StackRetStatus_t result = LORAWAN_SUCCESS;
	
    uint8_t channelId = *(uint8_t *)attrInput;
	
    if (channelId >= RegParams.maxChannels)
    {
        result = LORAWAN_INVALID_PARAMETER ;
    }
	
    return result;
}
#endif

#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t ValidateChannelIdT2 (LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t retVal = LORAWAN_SUCCESS;
	ValChId_t val_chid;
	
	memcpy(&val_chid,attrInput,sizeof(ValChId_t));
	if((val_chid.channelIndex >= RegParams.maxChannels) || (val_chid.allowedForDefaultChannels == WITHOUT_DEFAULT_CHANNELS &&
	 val_chid.channelIndex < RegParams.cmnParams.paramsType2.minNonDefChId))
	 {
		 retVal = LORAWAN_INVALID_PARAMETER;
	 }
	 return retVal;
}
#endif
/*
 * \brief Updates the Min and Max Channel datarate of all the enbled channels
 * \param[out] minmax_val Min MAX Datarate of the Enabled Channels
 */
static void UpdateMinMaxChDataRate (MinMaxDr_t* minmax_val)
{
	uint8_t i;
	
	// after updating the data range of a channel we need to check if the minimum dataRange has changed or not.
	// The user cannot set the current data rate outside the range of the data range
	uint8_t minDataRate = RegParams.minDataRate;
	uint8_t maxDataRate = RegParams.maxDataRate;

	for (i = 0; i < RegParams.maxChannels; i++)
	{
		if ( (RegParams.pChParams[i].dataRange.min < minDataRate) && (RegParams.pChParams[i].status == ENABLED) )
		{
			minDataRate = RegParams.pChParams[i].dataRange.min;
		}
		if ( (RegParams.pChParams[i].dataRange.max > maxDataRate) && (RegParams.pChParams[i].status == ENABLED) )
		{
			maxDataRate = RegParams.pChParams[i].dataRange.max;
		}
	}
	
	if ((RegParams.cmnParams.paramsType2.txParams.uplinkDwellTime == 1) && ((((1 << RegParams.band) & (ISM_ASBAND)) || ((1 << RegParams.band) & (1 << ISM_JPN923))) != 0))
	{
		minDataRate = DR2;
	}
	
	minmax_val->minDr = minDataRate;
	minmax_val->maxDr = maxDataRate;
}

/*
 * \brief Validates if the Channel Mask Control is supported by the regional band
 * \param[in] channelMaskCntl channelMask Cntrol to be validated 
 * \retval LORAWAN_SUCCESS If the validation is successful
 *		   LORAWAN_INVALID_PARAMETER If the channelMask Cntrol is not supported
 */
#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t ValidateChannelMaskCntl (LorawanRegionalAttributes_t attr, void *attrInput)
{
    StackRetStatus_t result = LORAWAN_SUCCESS;
	
	uint8_t channelMaskCntl = *(uint8_t *)attrInput;

    // 5 is RFU for channel mask for US
    if ((channelMaskCntl == 5) || (channelMaskCntl > 7))
    {
        result = LORAWAN_INVALID_PARAMETER;
    }

    return result;
}
#endif



/*
 * \brief Validates if the Channel Mask and Control is supported by the regional band
 * \param[in] chMaskchCntl channelMask and Control to be validated
 * \retval LORAWAN_SUCCESS If the validation is successful
 *		   LORAWAN_INVALID_PARAMETER If the channelMask Cntrol is not supported
 */
#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t ValidateChMaskChCntl (LorawanRegionalAttributes_t attr, void *attrInput)
{
    StackRetStatus_t result = LORAWAN_INVALID_PARAMETER;
	
	ValChMaskCntl_t chMaskchCntl = * (ValChMaskCntl_t *)attrInput;
    if(chMaskchCntl.chnlMask == 0 && chMaskchCntl.chnlMaskCntl == 7)
    {
        result = LORAWAN_INVALID_PARAMETER;
    }
	else
	{
		result = ((ValidateChannelMask(CHANNEL_MASK, (void *)&chMaskchCntl.chnlMask)) | (ValidateChannelMaskCntl(CHANNEL_MASK_CNTL,  (void *)&chMaskchCntl.chnlMaskCntl)));
	}
    return result;
}
#endif

/*
 * \brief Validates if the Channel Mask and Control is supported by the regional band
 * \param[in] chMaskchCntl channelMask and Control to be validated
 * \retval LORAWAN_SUCCESS If the validation is successful
 *		   LORAWAN_INVALID_PARAMETER If the channelMask Cntrol is not supported
 */
#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t ValidateChMaskChCntlT2 (LorawanRegionalAttributes_t attr, void *attrInput)
{
//     StackRetStatus_t result = LORAWAN_INVALID_PARAMETER;
// 	
	ValChMaskCntl_t chMaskchCntl = * (ValChMaskCntl_t *)attrInput;
// 
//     if ( chMaskchCntl.chnlMask || chMaskchCntl.chnlMaskCntl )
// 	{
// 	    result = ((ValidateChannelMaskT2(CHANNEL_MASK, (void *)&chMaskchCntl.chnlMask)) | (ValidateChannelMaskCntlT2(CHANNEL_MASK_CNTL, (void *)&chMaskchCntl.chnlMaskCntl)));
// 	}
// 
//     return result;
	/*If Both Mask and Cntl are 0 then its an invalid combination*/
    if ( chMaskchCntl.chnlMask || chMaskchCntl.chnlMaskCntl )
	{
		if(((ValidateChannelMaskT2(CHANNEL_MASK, (void *)&chMaskchCntl.chnlMask)) == LORAWAN_SUCCESS) && (ValidateChannelMaskCntlT2(CHANNEL_MASK_CNTL, (void *)&chMaskchCntl.chnlMaskCntl)) == LORAWAN_SUCCESS)
		{
			return LORAWAN_SUCCESS;
	}
		else
		{
			return LORAWAN_INVALID_PARAMETER;
}
	}
	else
	{
		return LORAWAN_INVALID_PARAMETER;
	}
}
#endif

/*
 * \brief This function gets the minimum and maximum data rate or the data range
 * supported by all the channels 
 * \retval Data Range supported by the Channel band
 */
#if (NA_BAND == 1 || AU_BAND == 1)
static DataRange_t getChBandDrT1(uint8_t chMaskCntl,uint16_t channelMask)
{

	uint8_t auxMinDataRate = DR7, auxMaxDataRate = DR0, i;
	uint16_t auxChannelMask;
	uint8_t startingIndex, endingIndex;
	DataRange_t bandDr;
	auxChannelMask = channelMask;
	
	if( RegParams.band == ISM_AU915)
	{
		auxMinDataRate = DR6;
	}
	
	switch (chMaskCntl)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		{
			startingIndex = chMaskCntl << SHIFT4;
			endingIndex = startingIndex + 16;
			for (i = startingIndex; i < endingIndex; i++)
			{
				if ((RegParams.pChParams[i].dataRange.min < auxMinDataRate) && (((auxChannelMask & 0x0001) == 0x0001) || (auxChannelMask == 0)))
				{
					auxMinDataRate = RegParams.pChParams[i].dataRange.min;
				}
				if ((RegParams.pChParams[i].dataRange.max > auxMaxDataRate) && (((auxChannelMask & 0x0001) == 0x0001) || (auxChannelMask == 0)))
				{
					auxMaxDataRate = RegParams.pChParams[i].dataRange.max;
				}
				auxChannelMask = auxChannelMask >> SHIFT1;
			}
		} break;

		case 4:
		case 7:
		{
			auxMinDataRate = RegParams.cmnParams.paramsType1.maxTxDR;
			auxMaxDataRate = RegParams.cmnParams.paramsType1.maxTxDR;
		} break;

		case 6:
		{
			// verify channels 0 to 63 for min/max datarate
			for (i = 0; i < 64; i++)
			{
				if (RegParams.pChParams[i].dataRange.min < auxMinDataRate)
				{
					auxMinDataRate = RegParams.pChParams[i].dataRange.min;
				}
				if (RegParams.pChParams[i].dataRange.max > auxMaxDataRate)
				{
					auxMaxDataRate = RegParams.pChParams[i].dataRange.max;
				}
			}
			if (channelMask != 0)    // if there is at least one channel enabled with DR4
			{
				auxMaxDataRate = RegParams.cmnParams.paramsType1.maxTxDR;
			}
		} break;
	}
	
	bandDr.min = auxMinDataRate;
	bandDr.max = auxMaxDataRate;
	
	return bandDr;
	
}
#endif

#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static DataRange_t getChBandDrT2(uint8_t chMaskCntl,uint16_t channelMask)
{
	uint8_t auxMinDataRate = DR7, auxMaxDataRate = DR0, i;
	uint16_t auxChannelMask;
	DataRange_t bandDr;
	auxChannelMask = channelMask;
	
	switch (chMaskCntl)
	{
		case 0:
		{
			for (i = 0; i < RegParams.maxChannels; i++)
			{
				if ((RegParams.pChParams[i].dataRange.min < auxMinDataRate) && ((auxChannelMask & 0x0001) == 0x0001))
				{
					auxMinDataRate = RegParams.pChParams[i].dataRange.min;
				}
				if ((RegParams.pChParams[i].dataRange.max > auxMaxDataRate) && ((auxChannelMask & 0x0001) == 0x0001))
				{
					auxMaxDataRate = RegParams.pChParams[i].dataRange.max;
				}
				auxChannelMask = auxChannelMask >> SHIFT1;
			}
			break;
		}

		case 6:
		{
			for (i = 0; i < RegParams.maxChannels; i++)
			{
				if (RegParams.pChParams[i].dataRange.min < auxMinDataRate)
				{
					auxMinDataRate = RegParams.pChParams[i].dataRange.min;
				}
				if (RegParams.pChParams[i].dataRange.max > auxMaxDataRate)
				{
					auxMaxDataRate = RegParams.pChParams[i].dataRange.max;
				}
			}
			break;
		}
		
		default:
		{
			auxMinDataRate = 0xFF;
			auxMaxDataRate = 0xFF;
			break;
		}
	}
	
	bandDr.min = auxMinDataRate;
	bandDr.max = auxMaxDataRate;
	
	return bandDr;
}
#endif
/*
 * \brief THis function is used to get the rx1 window parameters such as the data rate and frequency
 * \param[in] rx1WindowParamReq Structure containg the members required for getting the rx1 window parameters
 * \param[out] rx1WindowParams Structure containing the datarate and frequency used for rx1 window
 */

#if (NA_BAND == 1 || AU_BAND == 1)
static void getRx1WindowparamsType1(Rx1WindowParamsReq_t* rx1WindowParamReq ,Rx1WindowParams_t* rx1WindowParams)
{
	
	if(rx1WindowParamReq->joining)
	{
		if (RegParams.lastUsedChannelIndex <= MAX_CHANNELS_BANDWIDTH_125_AU_NA) 
		{
			rx1WindowParamReq->currDr = RegParams.cmnParams.paramsType1.minTxDR;
		}
		else
		{
			rx1WindowParamReq->currDr = RegParams.cmnParams.paramsType1.maxTxDR;
		}
	}
	rx1WindowParams->rx1Dr = rx1WindowParamReq->currDr + 
		            RegParams.cmnParams.paramsType1.RxParamWindowOffset1 - rx1WindowParamReq->drOffset;
		
	if(rx1WindowParams->rx1Dr > RegParams.cmnParams.paramsType1.maxRxDR)
	{
		rx1WindowParams->rx1Dr = RegParams.cmnParams.paramsType1.maxRxDR;
	}
	else if(rx1WindowParams->rx1Dr < RegParams.cmnParams.paramsType1.minRxDR)
	{
		rx1WindowParams->rx1Dr = RegParams.cmnParams.paramsType1.minRxDR;
	}
	
	rx1WindowParams->rx1Freq = GenerateFrequencyReception(RegParams.lastUsedChannelIndex % 8);
}
#endif

/*
 * \brief THis function is used to get the rx1 window parameters such as the data rate and frequency
 * \param[in] rx1WindowParamReq Structure containg the members required for getting the rx1 window parameters
 * \param[out] rx1WindowParams Structure containing the datarate and frequency used for rx1 window
 */
#if (EU_BAND == 1 || KR_BAND == 1)
static void getRx1WindowparamsType2(Rx1WindowParamsReq_t* rx1WindowParamReq,Rx1WindowParams_t* rx1WindowParams)
{

    if (rx1WindowParamReq->currDr >= rx1WindowParamReq->drOffset)
    {
        rx1WindowParams->rx1Dr = rx1WindowParamReq->currDr - rx1WindowParamReq->drOffset;
    }
    else
    {
        rx1WindowParams->rx1Dr = DR0;
    }

	rx1WindowParams->rx1Freq = RegParams.pOtherChParams[RegParams.lastUsedChannelIndex].rx1Frequency;			

}
#endif

/*
 * \brief THis function is used to get the rx1 window parameters such as the data rate and frequency
 * \param[in] rx1WindowParamReq Structure containg the members required for getting the rx1 window parameters
 * \param[out] rx1WindowParams Structure containing the datarate and frequency used for rx1 window
 */
#if (IND_BAND == 1)
static void getRx1WindowparamsType3(Rx1WindowParamsReq_t* rx1WindowParamReq,Rx1WindowParams_t* rx1WindowParams)
{
	uint8_t maxDR = DR5;
	uint8_t minDR = DR0;
	int8_t effectiveDROffset;
	
	effectiveDROffset = (rx1WindowParamReq->drOffset > maxDR) ? (maxDR - rx1WindowParamReq->drOffset) : (rx1WindowParamReq->drOffset);
	
    if (rx1WindowParamReq->currDr >= effectiveDROffset)
    {
        rx1WindowParams->rx1Dr = rx1WindowParamReq->currDr - effectiveDROffset;
		if(rx1WindowParams->rx1Dr < minDR)
		{
			rx1WindowParams->rx1Dr = minDR;
		}
		else if(rx1WindowParams->rx1Dr > maxDR)
		{
			rx1WindowParams->rx1Dr = maxDR;
		}
    }
    else
    {
        rx1WindowParams->rx1Dr = minDR;
    }

	rx1WindowParams->rx1Freq = RegParams.pOtherChParams[RegParams.lastUsedChannelIndex].rx1Frequency;			

}
#endif

#if (AS_BAND == 1 || JPN_BAND == 1)
static void getRx1WindowparamsType4(Rx1WindowParamsReq_t* rx1WindowParamReq,Rx1WindowParams_t* rx1WindowParams)
{
	uint8_t maxDR = DR5;
	uint8_t minDR = DR0;
	int8_t effectiveDROffset;
	
	if((((1 << RegParams.band) & (ISM_ASBAND)) || ((1 << RegParams.band) & (1 << ISM_JPN923)) != 0) &&  rx1WindowParamReq->joining)
	{
		rx1WindowParams->rx1Dr = DR2;
		rx1WindowParams->rx1Freq = RegParams.pOtherChParams[RegParams.lastUsedChannelIndex].rx1Frequency;
		return;
	}
	
	if(RegParams.cmnParams.paramsType2.txParams.downlinkDwellTime == 1)
	{
		minDR = DR2;
	}
	
	effectiveDROffset = (rx1WindowParamReq->drOffset > maxDR) ? (maxDR - rx1WindowParamReq->drOffset) : (rx1WindowParamReq->drOffset);
	
	if (rx1WindowParamReq->currDr >= effectiveDROffset)
	{
		rx1WindowParams->rx1Dr = rx1WindowParamReq->currDr - effectiveDROffset;
		if(rx1WindowParams->rx1Dr < minDR)
		{
			rx1WindowParams->rx1Dr = minDR;
		}
		else if(rx1WindowParams->rx1Dr > maxDR)
		{
			rx1WindowParams->rx1Dr = maxDR;
		}
	}
	else
	{
		rx1WindowParams->rx1Dr = minDR;
	}

	rx1WindowParams->rx1Freq = RegParams.pOtherChParams[RegParams.lastUsedChannelIndex].rx1Frequency;

}
#endif


/*
 * \brief Validates if the Transmit Power is supported by the regional band
 * \param[in] txPowerNew Transmit Power to be validated
 * \retval LORAWAN_SUCCESS If the validation is successful
 *		   LORAWAN_INVALID_PARAMETER If the Transmit Power is not supported
 */
static StackRetStatus_t ValidateTxPower (LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint8_t txPowerNew = *(uint8_t *)attrInput;
	
	//if ((txPowerNew < 5) || (txPowerNew > 10) || (txPowerNew == 6))
    if (0xf != txPowerNew)
    { /* 0xF means keep the current settings & ignore this cmd */
	    if (txPowerNew > RegParams.maxTxPwrIndx)
	    {
		    result = LORAWAN_INVALID_PARAMETER;
	    }
    }    

	return result;
}

/** 
 * \brief This function is used to select a free channel for transmission and
 * get the corresponding radio configuration for transmission 
 * \param[in] transmissionType Type of transmission - True : Data Frame ,False : Join Request
 * \param[in] txPwr Transmit Power which needs to be converted to the corresponding power 
 *	in dBm according to the regional specification
 * \param[in] currDr : CUrrent Data rate maintained in the MAC which is used to derive the radio configuration
 * \param[out] radioConfig : Radio Configuration to be set to the Radio registers
 * \retval LORAWAN_SUCCESS : If Channel is found successfully is successful
 *	LORAWAN_NO_CHANNELS_FOUND if No free channel is available
 */
#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t GetTxChannelConfig1 (bool transmissionType,uint8_t txPwrIndx,uint8_t currDr,radioConfig_t* radioConfig)  // transmission type is 0 means join request, transmission type is 1 means data message mode
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint8_t channelIndex;
	uint8_t txPower = RegParams.maxTxPwr - 2 * txPwrIndx;

	if (transmissionType == 1) // data message (not join request), it should search in all
	{
		result = SearchAvailableChannel1 (RegParams.maxChannels, transmissionType,currDr, &channelIndex);
		radioConfig->sf = RegParams.pDrParams[currDr].spreadingFactor;

	}
	else   // join request message, first should search between channels 0 - 63 using DR0, then 64 - 71 using DR4, and so on
	{
		result = SearchAvailableChannel1 (RegParams.cmnParams.paramsType1.Max_125khzChan, transmissionType,DR0, &channelIndex);
		if(channelIndex < MAX_CHANNELS_BANDWIDTH_125_AU_NA)
		{
			if( RegParams.band == ISM_NA915)
			{
				radioConfig->sf = SF_10;
			}
			else
			{
				radioConfig->sf = SF_12;
			}
		}
		else
		{
			radioConfig->sf = SF_8;
		}

#if (ENABLE_PDS == 1)
		PDS_STORE(RegParams.regParamItems.lastUsedSB);
#endif

	}


	if (result == LORAWAN_SUCCESS)
	{
		if (channelIndex < RegParams.cmnParams.paramsType1.Max_125khzChan)
		{
			radioConfig->bandwidth = BW_125KHZ;
			radioConfig->frequency = GenerateFrequency1 (channelIndex);
			radioConfig->txPower = txPower;
			if(RegParams.band == ISM_NA915)
			{
    			radioConfig->ecrConfig.override = true;
			    radioConfig->ecrConfig.ecr = CR_4_5;	 
			}
		}
		else
		{
			radioConfig->bandwidth = BW_500KHZ;
			radioConfig->frequency = GenerateFrequency2 (channelIndex);
			if( RegParams.band == ISM_NA915)
			{
			    radioConfig->ecrConfig.override = false;
			}
			//devices, when transmitting with 500 kHz BW may use a maximum of 26 dBm
			if (txPower <= 26)
			{
				radioConfig->txPower = txPower;
			}
			else
			{
				radioConfig->txPower = 26;
			}
		}
		radioConfig->freq_hop_period = DISABLED;
		radioConfig->modulation = MODULATION_LORA;
		//ConfigureRadioTx(radioConfig);
		RegParams.lastUsedChannelIndex = channelIndex;
	}

	return result;
}
#endif

/** 
 * \brief This function is used to select a free channel for transmission and
 * get the corresponding radio configuration for transmission 
 * \param[in] transmissionType Type of transmission - True : Data Frame ,False : Join Request
 * \param[in] txPwr Transmit Power which needs to be converted to the corresponding power 
 *	in dBm according to the regional specification
 * \param[in] currDr : CUrrent Data rate maintained in the MAC which is used to derive the radio configuration
 * \param[out] radioConfig : Radio Configuration to be set to the Radio registers
 * \retval LORAWAN_SUCCESS : If Channel is found successfully is successful
 *	LORAWAN_NO_CHANNELS_FOUND if No free channel is available
 */
#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t GetTxChannelConfig2 (bool transmissionType,uint8_t txPwrIndx,uint8_t currDr,radioConfig_t* radioConfig)  // transmission type is 0 means join request, transmission type is 1 means data message mode
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint8_t channelIndex;

	
	if(((((1 << RegParams.band) & (ISM_ASBAND)) || ((1 << RegParams.band) & (1 << ISM_JPN923))) != 0) && (transmissionType == 0))
	{
		/*DR2 is the default Join Data rate*/
		currDr = DR2;
	}
	
	
	result = SearchAvailableChannel2 (RegParams.maxChannels, transmissionType,currDr, &channelIndex);

	if (result == LORAWAN_SUCCESS)
	{
		RegParams.lastUsedChannelIndex = channelIndex;

		radioConfig->frequency = RegParams.pOtherChParams[channelIndex].ulfrequency;

		radioConfig->txPower = RegParams.maxTxPwr - 2 *txPwrIndx;
		
		radioConfig->freq_hop_period = DISABLED ;
		
		radioConfig->modulation = RegParams.pDrParams[currDr].modulation;
		
		radioConfig->bandwidth = RegParams.pDrParams[currDr].bandwidth;
		
		radioConfig->sf = RegParams.pDrParams[currDr].spreadingFactor;
		
		/*IND Regulatory does not prescribe specific ecr*/
		radioConfig->ecrConfig.override = false;
		
	}
	return result;
}
#endif

/** 
 * \brief This function is used to select a free available channel 
 * \param[in] transmissionType Type of transmission - True : Data Frame ,False : Join Request
 * \param[in] maxChannels Maximum number of channels to search
 * \param[in] currDr : CUrrent Data rate maintained in the MAC which is used to derive the radio configuration
 * \param[out] channelIndex : Index of the selected free channel
 * \retval LORAWAN_SUCCESS : If Channel is found successfully is successful
 *	LORAWAN_NO_CHANNELS_FOUND if No free channel is available
 */
//Pass current data rate into this function
#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t SearchAvailableChannel1 (uint8_t maxChannels, bool transmissionType,uint8_t currDr, uint8_t* channelIndex)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint8_t i = 0,j = 0,k = 0;
	/* Channel list is a 2-dimensional array, which stores the Channel 
	 * in first index and corresponding Sub-band in second index
	 * For eg: Channel ID 1 and corresponding sub-band is 1.
	 * Channel ID 55 and corresponding sub-band is 6 */
	uint8_t ChList[MAX_CHANNELS_BANDWIDTH_125_AU_NA + MAX_CHANNELS_BANDWIDTH_500_AU_NA][2];
	uint8_t num = 0;
	uint8_t randomNumber = 0;
	memset(ChList, 0, (MAX_CHANNELS_BANDWIDTH_125_AU_NA + MAX_CHANNELS_BANDWIDTH_500_AU_NA) * 2);  

	if(transmissionType == false)
	{   
		if(RegParams.FeaturesSupport & JOIN_BACKOFF_SUPPORT)
		{
			if(SwTimerIsRunning(RegParams.pJoinDutyCycleTimer->timerId)) 
			{
					return LORAWAN_NO_CHANNELS_FOUND;
			}
			}
	} 
	  
	for (i = 0, k = 0; i < (MAX_CHANNELS_BANDWIDTH_125_AU_NA + MAX_CHANNELS_BANDWIDTH_500_AU_NA); i += 8, k++)
	{
			for (j = 0; j < NO_OF_CH_IN_SUBBAND; j++)
			{
				/* 1. Check and see, the Channel status is ENABLED.
				* 2. If the request is for data packet,
				*		2a. Check and see both data rate input is between range min and max Dr of the Channel.
				*		2b. Check and see, if Channel is not as same as previous packet.
				* 3. If the request is for join procedure,
				*		3a. Check and see  transmissionType value is false.
				*		3b. Check and see the channel falls between the next sub-band of last used sub-band.
				*			3b1. Channel ID is less than 63 and lastUsedSB is equals to k.
				*			3b2. Channel ID is greater than 63 and lastUsedSB is equals to Channel ID - 63.
				*		Here k value is index and goes from 0-7.
				*		lastUsedSB is value and goes from 1-8. 
				*		Need to get channels only from next sub-band of previously used one.
				*/
				if (((transmissionType) && (currDr >= RegParams.pChParams[i + j].dataRange.min) && (currDr <= RegParams.pChParams[i + j].dataRange.max) 
					&& ((RegParams.pChParams[i + j].status == ENABLED) && ((i+j) != RegParams.lastUsedChannelIndex))) 
					||
					((!transmissionType) &&((RegParams.pChParams[i + j].status == ENABLED) && ((i+j) != RegParams.lastUsedChannelIndex))
	#if (RANDOM_NW_ACQ == 1)
					&&
					((((i+j) < MAX_CHANNELS_BANDWIDTH_125_AU_NA) && (RegParams.cmnParams.paramsType1.lastUsedSB == k))
					|| 
					(((i+j) > MAX_CHANNELS_BANDWIDTH_125_AU_NA) && 
					(RegParams.cmnParams.paramsType1.lastUsedSB == ((i + j) - MAX_CHANNELS_BANDWIDTH_125_AU_NA))))
                                  
	#endif        
					))
				{
						// Mark this channel i+j
						// Mark this sub band k
						// increment number of channel;
						ChList[num][0] = i+j;
						if((i+j) >= MAX_CHANNELS_BANDWIDTH_125_AU_NA)
						{
								/* The channel falls under 500KHz range. So need to calculate sub-band correctly */
								/* Sub-band values are stored in range of 1-8. So adding 1 to the index */
								ChList[num][1] = ((i + j) - MAX_CHANNELS_BANDWIDTH_125_AU_NA + 1);
						}
						else
						{
								/* Channel is in the range of 0-63 125Khz range, Store the k value as sub-band */
								/* Sub-band values are stored in range of 1-8. So adding 1 to the index */
								ChList[num][1] = k + 1;    
						}
						num++;
					}					
		}
	}
	/* Get a random number and select a channel */
	if(0 != num)
	{
		randomNumber = rand() % num;
		*channelIndex = ChList[randomNumber][0];
	#if (RANDOM_NW_ACQ == 1)          
		/* Update the lastUsedSB value based on the channel selected */
		RegParams.cmnParams.paramsType1.lastUsedSB = ChList[randomNumber][1];
		/* If the lastUsedSB value is 8, then it means roll over has to happen.
		* So changing the value to 1
		*/
		if(RegParams.cmnParams.paramsType1.lastUsedSB >= MAX_SUBBANDS)
		{
				RegParams.cmnParams.paramsType1.lastUsedSB = 0;
			
		}
	#endif 
	}
	else
	{
		if ((RegParams.pChParams[RegParams.lastUsedChannelIndex].status == ENABLED) &&
		(currDr >= RegParams.pChParams[RegParams.lastUsedChannelIndex].dataRange.min) &&
		(currDr <= RegParams.pChParams[RegParams.lastUsedChannelIndex].dataRange.max))
		{
			*channelIndex = RegParams.lastUsedChannelIndex;
		}
		else
		{
			result = LORAWAN_NO_CHANNELS_FOUND;
		}

	}
	return result;	
}
#endif

/** 
 * \brief This function is used to select a free available channel 
 * \param[in] transmissionType Type of transmission - True : Data Frame ,False : Join Request
 * \param[in] maxChannels Maximum number of channels to search
 * \param[in] currDr : CUrrent Data rate maintained in the MAC which is used to derive the radio configuration
 * \param[out] channelIndex : Index of the selected free channel
 * \retval LORAWAN_SUCCESS : If Channel is found successfully is successful
 *	LORAWAN_NO_CHANNELS_FOUND if No free channel is available
 */
//Pass current data rate into this function
#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t SearchAvailableChannel2 (uint8_t maxChannels, bool transmissionType,uint8_t currDr, uint8_t* channelIndex)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;

	uint8_t i = 0;
	uint8_t num = 0;
	uint8_t ChList[maxChannels];
	uint8_t randomNumber = 0;
	memset(ChList, 0, sizeof(ChList));
	bool bandWithoutDutyCycle = (((1 << RegParams.band) & (ISM_EUBAND | ISM_ASBAND | (1 << ISM_JPN923))) == 0);
	
    if(transmissionType == false)
    {
	    if(RegParams.FeaturesSupport & JOIN_BACKOFF_SUPPORT)
	    {
		    if(SwTimerIsRunning(RegParams.pJoinDutyCycleTimer->timerId)) //check timerrunning
		    {
			    return LORAWAN_NO_CHANNELS_FOUND;
		    }
	    }
    }
    else
    {
	    if(RegParams.cmnParams.paramsType2.aggregatedDutyCycleTimeout)
	    {
		    return LORAWAN_NO_CHANNELS_FOUND;
	    }
    }
	
	for (i = 0; i < maxChannels; i++)
	{
			if ((RegParams.pChParams[i].status == ENABLED) &&
				(currDr >= RegParams.pChParams[i].dataRange.min) &&
				(currDr <= RegParams.pChParams[i].dataRange.max)&&

				(bandWithoutDutyCycle || RegParams.pSubBandParams[RegParams.pOtherChParams[i].subBandId].subBandTimeout == 0))
			{
				if(((transmissionType == 0)  && (RegParams.pOtherChParams[i].joinRequestChannel == 1)) || (transmissionType != 0)) 
				{
					ChList[num] = i;
					num++;
					
				}
				
				
			}
	}
	if(0 != num)
	{
		randomNumber = rand() % num;
		*channelIndex = ChList[randomNumber];
	}
	else
	{
		result = LORAWAN_NO_CHANNELS_FOUND;
	}
	return result;
}
#endif


/*
 * \brief Verifies if the channel mask has all channels enabled
 * \param[in] channelMask Channel Mask to be validated
 * \retval LORAWAN_SUCCESS If the validation is successful
 *		   LORAWAN_INVALID_PARAMETER If the channelMask is invalid
 */
#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t ValidateChannelMask (LorawanRegionalAttributes_t attr, void *attrInput)
{
	return LORAWAN_SUCCESS;	
}
#endif

#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t ValidateChannelMaskT2 (LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t retVal = LORAWAN_SUCCESS;
	
	uint16_t channelMask = * (uint16_t *)attrInput;
	
	if(channelMask != 0x0000U)
	{
		for(uint8_t i = 0; i< RegParams.maxChannels;i++)
		{
			if(((channelMask && BIT0) == BIT0) && ((RegParams.pOtherChParams[i].parametersDefined & (FREQUENCY_DEFINED | DATA_RANGE_DEFINED)) != (FREQUENCY_DEFINED | DATA_RANGE_DEFINED)))
			{
				retVal = LORAWAN_INVALID_PARAMETER;
				break;
			}
			else
			{
				channelMask = channelMask >> SHIFT1;
			}
		}
		return retVal = LORAWAN_SUCCESS;
	}
	else
	{
		////ChMask can be set to 0 if ChMaskCtrl is set to 6
		return retVal = LORAWAN_SUCCESS;
	}
	
	return retVal;
}
#endif

#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t ValidateChannelMaskCntlT2 (LorawanRegionalAttributes_t attr, void *attrInput)
{
	uint8_t channelMaskCntl = * (uint16_t *)attrInput;
	StackRetStatus_t result = LORAWAN_SUCCESS;
	
    if ( (channelMaskCntl != 0) && (channelMaskCntl != 6) )
    {
	    result = LORAWAN_INVALID_PARAMETER;
    }
	return result;
}
#endif

#if (NA_BAND == 1 || AU_BAND == 1)
static uint32_t GenerateFrequencyReception (uint8_t channelIndex)
{
	uint32_t channelFrequency;
	
	channelFrequency = RegParams.cmnParams.paramsType1.DownStreamCh0Freq + FREQ_600KHZ * channelIndex;
	return channelFrequency;
}
#endif

/*
 * \brief This function Creates all the timers required for the regional module
 */
StackRetStatus_t CreateAllRegSoftwareTimers (void)
{
    /* For other bands, regTimerId[0] is used for required purpose. */
	StackRetStatus_t status  = LORAWAN_SUCCESS;
	for(uint8_t i=0; i < REG_PARAMS_TIMERS_COUNT; i++)
	{
		if(LORAWAN_SUCCESS == status)
		{
			status = SwTimerCreate(&regTimerId[i]);
		}
		else
		{
			break;
		}
	}
    return status;
}

/*
 * \brief This function Stops  all the timers of the regional module
 */

void StopAllRegSoftwareTimers (void)
{
	for (uint8_t i=0; i< REG_PARAMS_TIMERS_COUNT; i++)
	{
		SwTimerStop(regTimerId[i]);
	}
	
}


/*
 * \brief Timer callback for updating the duty cycle timer
 * \param Timer Parameters
 */
#if (EU_BAND == 1) || (AS_BAND == 1) || (JPN_BAND == 1)
static void DutyCycleCallback (uint8_t param)
{
    uint32_t minimSubBandTimer = UINT32_MAX;
	uint32_t nextTimer;
    bool found = 0;
    uint8_t i;
	// todo : ASIA DCYCLE / this code has potential for bug; it can be a garbage value. never initialized
	uint32_t DutyCycleTimeout = RegParams.cmnParams.paramsType2.aggregatedDutyCycleTimeout;

    for (i=0; i < RegParams.maxSubBands; i++)
    {
        //Validate this only for enabled channels
        if (( RegParams.pSubBandParams[i].subBandTimeout != 0 ))
        {
            if ( RegParams.pSubBandParams[i].subBandTimeout > RegParams.pDutyCycleTimer->lastTimerValue )
            {
                RegParams.pSubBandParams[i].subBandTimeout = RegParams.pSubBandParams[i].subBandTimeout - RegParams.pDutyCycleTimer->lastTimerValue;
            }
            else
            {
                RegParams.pSubBandParams[i].subBandTimeout = 0;
            } 
            if ( (RegParams.pSubBandParams[i].subBandTimeout <= minimSubBandTimer) && (RegParams.pSubBandParams[i].subBandTimeout != 0) )
            {
                minimSubBandTimer  = RegParams.pSubBandParams[i].subBandTimeout;
                found = 1;
            }
        }
    }
	
    if (( DutyCycleTimeout != 0 ))
    {
	    if (DutyCycleTimeout > RegParams.pDutyCycleTimer->lastTimerValue)
	    {
		    RegParams.cmnParams.paramsType2.aggregatedDutyCycleTimeout = DutyCycleTimeout - RegParams.pDutyCycleTimer->lastTimerValue;
	    }
	    else
	    {
		    RegParams.cmnParams.paramsType2.aggregatedDutyCycleTimeout = 0;
	    }
		
		DutyCycleTimeout = RegParams.cmnParams.paramsType2.aggregatedDutyCycleTimeout;
		
		if(DutyCycleTimeout)
		{
			found = 1;
		}
    }
		
			
    if ( found == 1 )
    {		
		if ((DutyCycleTimeout >= minimSubBandTimer))
		{
			nextTimer  = DutyCycleTimeout;			
		}
		else
		{
			nextTimer = minimSubBandTimer;
		}

        RegParams.pDutyCycleTimer->lastTimerValue = nextTimer;
		SwTimerStart (RegParams.pDutyCycleTimer->timerId, MS_TO_US(nextTimer), SW_TIMEOUT_RELATIVE, (void *)DutyCycleCallback, NULL);
        
    }
}
#endif
void JoinDutyCycleCallback (uint8_t param)
{   
	
	if(RegParams.pJoinDutyCycleTimer->remainingtime!=0)
	{
		if(RegParams.pJoinDutyCycleTimer->remainingtime>US_TO_MS(SWTIMER_MAX_TIMEOUT))
		{
			RegParams.pJoinDutyCycleTimer->remainingtime = RegParams.pJoinDutyCycleTimer->remainingtime-(US_TO_MS(SWTIMER_MAX_TIMEOUT));
			SwTimerStart(RegParams.pJoinDutyCycleTimer->timerId, SWTIMER_MAX_TIMEOUT, SW_TIMEOUT_RELATIVE, (void *)JoinDutyCycleCallback, NULL);
			
		}
		else
		{
			SwTimerStart(RegParams.pJoinDutyCycleTimer->timerId, MS_TO_US(RegParams.pJoinDutyCycleTimer->remainingtime), SW_TIMEOUT_RELATIVE, (void *)JoinDutyCycleCallback, NULL);
			RegParams.pJoinDutyCycleTimer->remainingtime =0;
		}
	}
	else
	{
	SwTimerStop(RegParams.pJoinDutyCycleTimer->timerId);
	RegParams.joinDutyCycleTimeout = 0;
	
	}
}

void JoinBackoffCallback (uint8_t param)
{
	RegParams.joinbccount ++;
	SwTimerStart (RegParams.pJoinBackoffTimer->timerId, MS_TO_US(BACKOFF_BASE_TIME_IN_MS), SW_TIMEOUT_RELATIVE, (void *)JoinBackoffCallback, NULL);
}
/*
 * \brief Validates if the Datarate is supported by the regional band
 * \param[in] dataRate Data rate to be validated
 * \retval LORAWAN_SUCCESS If the validation is successful
 *		   LORAWAN_INVALID_PARAMETER If the data rate is not supported
 */
#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t ValidateDataRate (LorawanRegionalAttributes_t attr, void *attrInput)
{
    StackRetStatus_t result = LORAWAN_SUCCESS;
	
	uint8_t dataRate = *(uint8_t *)attrInput;

    if (0xf != dataRate)
    { /* 0xF means keep the current settings & ignore this cmd */
        if ( dataRate > RegParams.minDataRate )
        {
            result = LORAWAN_INVALID_PARAMETER;
        }        
    }

    return result;
}
#endif

static StackRetStatus_t ValidateSupportedDr (LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t result = LORAWAN_INVALID_PARAMETER;
	uint8_t  dataRate;
	dataRate = *(uint8_t *)attrInput;
	
	for(uint8_t i = 0; i <RegParams.maxChannels; i++)
	{
		if(RegParams.pChParams[i].status == ENABLED && dataRate >= RegParams.pChParams[i].dataRange.min &&
		   dataRate <= RegParams.pChParams[i].dataRange.max)
		{
			result = LORAWAN_SUCCESS;
			break;
		}
	}
	return result;	
}

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t ValidateRxFreqT1 (LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint32_t FreqNew = *(uint32_t *)attrInput;
	
	if(FreqNew < FREQ_923300KHZ || FreqNew > FREQ_927500KHZ || (FreqNew - FREQ_923300KHZ) % FREQ_600KHZ != 0)
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	return result;
}

static StackRetStatus_t ValidateTxFreqT1(LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	ValUpdateFreqTx_t  val_freqTx;
	uint8_t ChIndex;
	
	memcpy(&val_freqTx,attrInput,sizeof(ValUpdateFreqTx_t));
	ChIndex = val_freqTx.channelIndex;
	if(ChIndex > RegParams.maxChannels)
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	if(val_freqTx.frequencyNew != GenerateFrequency1(ChIndex) && 
	   val_freqTx.frequencyNew != GenerateFrequency2(ChIndex))
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	return result;
}
#endif

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t ValidateDataRange(LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t retVal = LORAWAN_SUCCESS;
	ValUpdateDrange_t val_drange;
	uint8_t chId;
	uint8_t dataRateMin, dataRateMax;
	uint8_t maxTxDR = RegParams.cmnParams.paramsType1.maxTxDR;
	memcpy(&val_drange,attrInput,sizeof(ValUpdateDrange_t));
	
	dataRateMin = val_drange.dataRangeNew & LAST_NIBBLE;
	dataRateMax = (val_drange.dataRangeNew & FIRST_NIBBLE) >> SHIFT4;
	chId = val_drange.channelIndex;
	
	if((ValidateDataRateTxT1(TX_DATARATE, (void *)&dataRateMax) != LORAWAN_SUCCESS ) || (ValidateDataRateTxT1(TX_DATARATE, (void *)&dataRateMin) != LORAWAN_SUCCESS ) || dataRateMax < dataRateMin || 
	   ((chId < RegParams.cmnParams.paramsType1.Max_125khzChan) && ((dataRateMin == maxTxDR) || (dataRateMax == maxTxDR))) || 
	   ((chId >= RegParams.cmnParams.paramsType1.Max_125khzChan) && (chId < RegParams.maxChannels) && (dataRateMin != maxTxDR)) ||
	   ((chId >= RegParams.cmnParams.paramsType1.Max_125khzChan) && (chId < RegParams.maxChannels) && (dataRateMax != maxTxDR)))
    {
	    retVal = LORAWAN_INVALID_PARAMETER;	   
	}
	return retVal;
}
#endif

#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t ValidateDataRangeT2(LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t retVal = LORAWAN_SUCCESS;
	ValUpdateDrange_t val_drange;
	uint8_t dataRateMin, dataRateMax;
	memcpy(&val_drange,attrInput,sizeof(ValUpdateDrange_t));
	
	dataRateMin = val_drange.dataRangeNew & LAST_NIBBLE;
	dataRateMax = (val_drange.dataRangeNew & FIRST_NIBBLE) >> SHIFT4;
	
	if((ValidateDataRate(RX_DATARATE, (void *)&dataRateMin) != LORAWAN_SUCCESS) || (ValidateDataRate(RX_DATARATE, (void *)&dataRateMax) != LORAWAN_SUCCESS)
	   || (dataRateMax < dataRateMin) )
	{
		retVal = LORAWAN_INVALID_PARAMETER;
	}
	return retVal;
}
#endif

static StackRetStatus_t ValidateRx1DataRateOffset(LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t retVal = LORAWAN_INVALID_PARAMETER;
	
	uint8_t rx1DrOffset = *(uint8_t *)attrInput;
	
	if(rx1DrOffset <= RegParams.Rx1DrOffset)
	{
		retVal = LORAWAN_SUCCESS;
	}
	
	return retVal;
}

#if(EU_BAND == 1)
static StackRetStatus_t ValidateFreq (LorawanRegionalAttributes_t attr, void *attrInput)
{
	uint32_t frequencyNew = *(uint32_t *)attrInput;
	StackRetStatus_t retVal = LORAWAN_SUCCESS;

    if(getSubBandId(frequencyNew) == 0xFF)
	{
		retVal = LORAWAN_INVALID_PARAMETER;
	}
	return retVal;
}
#endif

#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t ValidateTxFreqT2 (LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t retVal = LORAWAN_SUCCESS;
	ValUpdateFreqTx_t  val_freqTx;
	
	memcpy(&val_freqTx,attrInput,sizeof(ValUpdateFreqTx_t));
	
	retVal = pValidateAttr[RX_FREQUENCY](RX_FREQUENCY,(void *)&val_freqTx.frequencyNew);

	return retVal;
}
#endif

#if( IND_BAND == 1)
static StackRetStatus_t ValidateFreqIN (LorawanRegionalAttributes_t attr, void *attrInput)
{
	uint32_t newFreq = 0;
	StackRetStatus_t result = LORAWAN_SUCCESS;
	
	memcpy(&newFreq,attrInput,sizeof(uint32_t));
	
	if(newFreq > FREQ_867000KHZ || newFreq < FREQ_865000KHZ)
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	
	return result;
}
#endif

#if( JPN_BAND == 1 )
static StackRetStatus_t ValidateFreqJP (LorawanRegionalAttributes_t attr, void *attrInput)
{
	uint32_t newFreq = 0;
	StackRetStatus_t result = LORAWAN_SUCCESS;
	
	memcpy(&newFreq,attrInput,sizeof(uint32_t));
	
	if(newFreq > FREQ_928000KHZ || newFreq < FREQ_920000KHZ)
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	
	return result;
}
#endif

#if( KR_BAND == 1)
static StackRetStatus_t ValidateFreqKR (LorawanRegionalAttributes_t attr, void *attrInput)
{
    StackRetStatus_t result = LORAWAN_INVALID_PARAMETER;
	uint32_t freqwidth = 200000;
	uint32_t freq = 0, freqNew = 0;
	
	memcpy(&freqNew,attrInput,sizeof(uint32_t));
	
	for(freq = FREQ_920900KHZ; freq <= FREQ_923300KHZ; freq += freqwidth)
	{
		if(freq == freqNew)
		{
			result = LORAWAN_SUCCESS;
			break;
		}
	}
	return result;	
}
#endif

#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static uint8_t getSubBandId(uint32_t frequency)
{
	uint8_t subBandId = 0xFF;
#if (EU_BAND == 1)	|| (JPN_BAND == 1)
	for(uint8_t i = 0; i < RegParams.maxSubBands; i++)
	{
		if(frequency >= RegParams.pSubBandParams[i].freqMin && frequency <= RegParams.pSubBandParams[i].freqMax)
		{
			subBandId = i;
			break;
		}
	}
#elif (AS_BAND == 1)
	// whole ASIA 923 spectrum is one band
	subBandId = 0;
#endif
	return subBandId;
}
#endif

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t setDataRange (LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t retVal = LORAWAN_SUCCESS;
	ValUpdateDrange_t  update_dr;
	
	memcpy((void *)&update_dr,attrInput,sizeof(ValUpdateDrange_t));
	if((ValidateDataRange(DATA_RANGE, attrInput) != LORAWAN_SUCCESS) || (ValidateChannelId(CHANNEL_ID, &update_dr.channelIndex) != LORAWAN_SUCCESS))
	{
		retVal = LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		RegParams.pChParams[update_dr.channelIndex].dataRange.value = update_dr.dataRangeNew;
#if (ENABLE_PDS == 1)
		PDS_STORE(RegParams.regParamItems.ch_param_1_item_id);
#endif
	}
	
	return retVal;
}
#endif
StackRetStatus_t setDefaultTxPower(IsmBand_t ismBand)
{   
	StackRetStatus_t result = LORAWAN_SUCCESS;
	if(ismBand == ISM_EU868)
	{
			RegParams.MacTxPower = MAC_DEF_TX_POWER_EU;
	}
	else if(ismBand == ISM_JPN923)
	{
		RegParams.MacTxPower = MAC_DEF_TX_POWER_JP;
	}
	else if (ismBand == ISM_KR920)
	{
		RegParams.MacTxPower = MAC_DEF_TX_POWER_KR;
	}
	else if (ismBand == ISM_ASBAND)
	{
		RegParams.MacTxPower = MAC_DEF_TX_POWER_AS;
	}
	else if(ismBand == ISM_AU915)
	{
		RegParams.MacTxPower = MAC_DEF_TX_POWER_AU;
	}
	else if(ismBand == ISM_NA915)
	{
		RegParams.MacTxPower = MAC_DEF_TX_POWER_NA;
	}
	else
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
#if (ENABLE_PDS==1)
	PDS_STORE(RegParams.MacTxPower);
#endif
	return result;
}
#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t setDataRangeT2 (LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t retVal = LORAWAN_SUCCESS;
	ValUpdateDrange_t  update_dr;
	ValChId_t val_chid;
	
	memcpy((void *)&update_dr,attrInput,sizeof(ValUpdateDrange_t));
	
	val_chid.channelIndex = update_dr.channelIndex;
	val_chid.allowedForDefaultChannels = ALL_CHANNELS;
	
	if((ValidateDataRangeT2(DATA_RANGE, attrInput) != LORAWAN_SUCCESS) || (ValidateChannelIdT2(CHANNEL_ID, &val_chid) != LORAWAN_SUCCESS))
	{
		retVal = LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		RegParams.pChParams[update_dr.channelIndex].dataRange.value = update_dr.dataRangeNew;
#if (ENABLE_PDS == 1)
		PDS_STORE(RegParams.regParamItems.ch_param_1_item_id);
#endif
		RegParams.pOtherChParams[update_dr.channelIndex].parametersDefined |= DATA_RANGE_DEFINED;
#if (ENABLE_PDS == 1)
		PDS_STORE(RegParams.regParamItems.ch_param_2_item_id);
#endif
	}
	
	return retVal;
}
#endif

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t setChannelIdStatus (LorawanRegionalAttributes_t attr, void *attrInput)
{
	UpdateChId_t updateChid;
	StackRetStatus_t retVal = LORAWAN_SUCCESS;
	if(ValidateChannelId(CHANNEL_ID, attrInput) == LORAWAN_SUCCESS)
	{
		memcpy(&updateChid,attrInput,sizeof(UpdateChId_t));
		UpdateChannelIdStatus(updateChid.channelIndex,updateChid.statusNew);
	}
	else
	{
		retVal = LORAWAN_INVALID_PARAMETER;
	}
	return retVal;
}
#endif

#if( EU_BAND == 1 || IND_BAND == 1)
static StackRetStatus_t setChannelIdStatusT2 (LorawanRegionalAttributes_t attr, void *attrInput)
{
	UpdateChId_t updateChid;
	StackRetStatus_t retVal = LORAWAN_SUCCESS;
	ValChId_t valChid;
	
	memcpy(&updateChid,attrInput,sizeof(UpdateChId_t));
	
	valChid.channelIndex = updateChid.channelIndex;
	valChid.allowedForDefaultChannels = /*WITHOUT_DEFAULT_CHANNELS*/ALL_CHANNELS;
	
	if(ValidateChannelIdT2(CHANNEL_ID, &valChid) == LORAWAN_SUCCESS)
	{
		if(RegParams.band ==  ISM_IND865)
		{
#if ((IND_BAND) == 1)
			UpdateChannelIdStatus(updateChid.channelIndex,updateChid.statusNew);
#endif
		}
		else
		{
#if ((EU_BAND) == 1)
		    UpdateChannelIdStatusT2(updateChid.channelIndex,updateChid.statusNew);
#endif
		}

	}
	else
	{
		retVal = LORAWAN_INVALID_PARAMETER;
	}
	return retVal;
}
#endif

#if( AS_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t setChannelIdStatusT3 (LorawanRegionalAttributes_t attr, void *attrInput)
{
	UpdateChId_t updateChid;
	StackRetStatus_t retVal = LORAWAN_SUCCESS;
	ValChId_t valChid;
	
	memcpy(&updateChid,attrInput,sizeof(UpdateChId_t));
	
	valChid.channelIndex = updateChid.channelIndex;
	valChid.allowedForDefaultChannels = ALL_CHANNELS;
	
	if(ValidateChannelIdT2(CHANNEL_ID, &valChid) == LORAWAN_SUCCESS)
	{
		if( RegParams.band == ISM_KR920)
		{
#if (KR_BAND == 1)
			UpdateChannelIdStatusT4(updateChid.channelIndex,updateChid.statusNew);
#endif
		}
		else
		{
#if (((AS_BAND) == 1) || ((JPN_BAND) == 1))
		    UpdateChannelIdStatusT3(updateChid.channelIndex,updateChid.statusNew);
#endif
		}
	}
	else
	{
		retVal = LORAWAN_INVALID_PARAMETER;
	}
	return retVal;
}
#endif

#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t setNewChannelsT1 (LorawanRegionalAttributes_t attr, void *attrInput)
{
	UpdateNewCh_t updateNewCh;
	uint16_t chMask = 0;
	uint8_t chMaskCtrl = 0;
	StackRetStatus_t retVal = LORAWAN_SUCCESS;
	memcpy(&updateNewCh,attrInput,sizeof(UpdateNewCh_t));
	chMask = updateNewCh.channelMask;
	chMaskCtrl = updateNewCh.channelMaskCntl;

	if(ValidateChannelMaskCntl(CHANNEL_MASK_CNTL, &chMaskCtrl) == LORAWAN_SUCCESS)
	{
		uint8_t i = 0;
		uint8_t channel = 0;

		channel = chMaskCtrl << SHIFT4;
		switch(chMaskCtrl)
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			{
				EnableChannels2(channel, channel + 15, chMask);
				break;
			}
			case 5:
			    break;
			case 6:
			{
				for(i = 0; i < 63; i++)
				{
					UpdateChannelIdStatus(i, ENABLED);
				}
				EnableChannels2(64, 71, chMask);
			}
			    break;
			case 7:
			{
				for(i = 0; i< 63; i++)
				{
					UpdateChannelIdStatus(i, DISABLED);
				}
				EnableChannels2(64, 71, chMask);
				break;
			}
		}
	}
	else
	{
		retVal = LORAWAN_INVALID_PARAMETER;
	}
	return retVal;
}
#endif

#if (NA_BAND == 1 || AU_BAND == 1 || IND_BAND == 1)
static void UpdateChannelIdStatus(uint8_t chid, bool statusNew)
{
	if(chid < RegParams.maxChannels || ((((1 << RegParams.band) & (ISM_NAAUBAND)) == 0) && chid >= RegParams.cmnParams.paramsType2.minNonDefChId))
	{
		RegParams.pChParams[chid].status = statusNew;
#if (ENABLE_PDS == 1)
		PDS_STORE(RegParams.regParamItems.ch_param_1_item_id);
#endif

		
	}
}
#endif

#if ((EU_BAND) == 1)
static void UpdateChannelIdStatusT2(uint8_t chid, bool statusNew)
{
	if(chid < RegParams.maxChannels && /* chid >= RegParams.cmnParams.paramsType2.minNonDefChId && */
	   (RegParams.pOtherChParams[chid].parametersDefined & (FREQUENCY_DEFINED | DATA_RANGE_DEFINED)) == (FREQUENCY_DEFINED | DATA_RANGE_DEFINED))
	{
		RegParams.pChParams[chid].status = statusNew;
#if (ENABLE_PDS == 1)
		PDS_STORE(RegParams.regParamItems.ch_param_1_item_id);
#endif

		
		if(((1 << RegParams.band) & (ISM_EUBAND)) != 0 && statusNew == DISABLED)
		{
			uint8_t subBandId;
			subBandId = RegParams.pOtherChParams[chid].subBandId;
			
			for(uint8_t i = 0; i < RegParams.maxChannels; i++)
			{
				if(RegParams.pChParams[i].status == ENABLED &&
				 subBandId == RegParams.pOtherChParams[i].subBandId)
				 {
					 return;
				 }
			}
			RegParams.pSubBandParams[subBandId].subBandTimeout = 0;
		}
	}
}
#endif

#if (AS_BAND == 1 || JPN_BAND == 1)
static void UpdateChannelIdStatusT3(uint8_t chid, bool statusNew)
{
	RegParams.pChParams[chid].status = statusNew;
#if (ENABLE_PDS == 1)
	PDS_STORE(RegParams.regParamItems.ch_param_1_item_id);
#endif
	
#if (ENABLE_PDS == 1)
	PDS_STORE(RegParams.regParamItems.ch_param_2_item_id);
#endif
	if( RegParams.band == ISM_JPN923)
	{
		RegParams.cmnParams.paramsType2.channelTimer[chid] = 0;
	}

}
#endif

#if (KR_BAND == 1)
static void UpdateChannelIdStatusT4(uint8_t chid, bool statusNew)
{
	RegParams.pChParams[chid].status = statusNew;
	
#if (ENABLE_PDS == 1)
	PDS_STORE(RegParams.regParamItems.ch_param_1_item_id);
#endif
	
	if(statusNew == ENABLED)
	{
		if(RegParams.pOtherChParams[chid].ulfrequency < FREQ_922100KHZ)
		{
			RegParams.pOtherChParams[chid].maxEIRP = DEFAULT_EIRP_KR_LF;
		}
		else
		{
			RegParams.pOtherChParams[chid].maxEIRP = DEFAULT_EIRP_KR_HF;
		}
#if (ENABLE_PDS == 1)
		PDS_STORE(RegParams.regParamItems.ch_param_2_item_id);
#endif

	}
}
#endif

#if ((NA_BAND) == 1 || (AU_BAND) == 1)
static void EnableChannels2(uint8_t startIndx, uint8_t endIndx, uint16_t chMask)
{
	uint8_t i = 0;
	
	for(i = startIndx; i <= endIndx; i++)
	{
		if((chMask & (0x0001)) == 0x0001)
		{
			UpdateChannelIdStatus(i, ENABLED);
		}
		else
		{
			UpdateChannelIdStatus(i, DISABLED);
		}
		chMask = chMask >> SHIFT1;
	}
}
#endif

/*
 * \brief Validates if the Frequency is supported by the regional band
 * \param[in] frequencyNew Frequency to be validated
 * \retval LORAWAN_SUCCESS If the validation is successful
 *		   LORAWAN_INVALID_PARAMETER If the Frequency is not supported
 */
#if (AS_BAND == 1)
static StackRetStatus_t ValidateFrequencyAS (LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	uint32_t frequencyNew = * (uint32_t *)attrInput;

    /*Bands Supporting Freq 923-925MHz*/
    if(RegParams.band == ISM_BRN923 || RegParams.band == ISM_CMB923 || RegParams.band == ISM_INS923 || RegParams.band == ISM_LAOS923)
	{
		if ( (frequencyNew < FREQ_923000KHZ) || (frequencyNew > FREQ_925000KHZ) )
		{
			result = LORAWAN_INVALID_PARAMETER;
		}
	}
	/*Bands Supporting Freq 915 - 928MHz*/
	else if(RegParams.band == ISM_NZ923)
	{
		if ( (frequencyNew < FREQ_915000KHZ) || (frequencyNew > FREQ_928000KHZ) )
		{
			result = LORAWAN_INVALID_PARAMETER;
		}		
	}
	/*Bands Supporting Freq 920 - 925MHz*/
	else if(RegParams.band == ISM_SP923 || RegParams.band == ISM_THAI923 || RegParams.band == ISM_VTM923)
	{
		if ( (frequencyNew < FREQ_920000KHZ) || (frequencyNew > FREQ_925000KHZ) )
		{
			result = LORAWAN_INVALID_PARAMETER;
		}
	}
	/*Bands Supporting Freq 922 - 928MHz*/
	else if(RegParams.band == ISM_TWN923)
	{
		if ( (frequencyNew < FREQ_922000KHZ) || (frequencyNew > FREQ_928000KHZ) )
		{
			result = LORAWAN_INVALID_PARAMETER;
		}
	}
	else
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	return result;
}
#endif

#if (EU_BAND == 1) || (AS_BAND == 1) || (JPN_BAND == 1)
static StackRetStatus_t setDutyCycle(LorawanRegionalAttributes_t attr, void *attrInput)
{
    UpdateDutyCycle_t updateDCycle;
	ValChId_t val_chid;
	StackRetStatus_t result = LORAWAN_SUCCESS;
    memcpy(&updateDCycle,attrInput,sizeof(UpdateDutyCycle_t));
	
	val_chid.channelIndex = updateDCycle.channelIndex;
	val_chid.allowedForDefaultChannels = ALL_CHANNELS;
	
	if(ValidateChannelIdT2(CHANNEL_ID, &val_chid) == LORAWAN_SUCCESS)
	{
		uint8_t bandId;
		bandId = RegParams.pOtherChParams[updateDCycle.channelIndex].subBandId;
		RegParams.cmnParams.paramsType2.subBandDutyCycle[bandId] = updateDCycle.dutyCycleNew;
		RegParams.pSubBandParams[bandId].subBandTimeout = 0;
		RegParams.pOtherChParams[updateDCycle.channelIndex].parametersDefined |= DUTY_CYCLE_DEFINED;
#if (ENABLE_PDS == 1)
		PDS_STORE(RegParams.regParamItems.ch_param_2_item_id);
#endif
	}
	else
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	return result;
}
#endif

#if (EU_BAND == 1) || (AS_BAND == 1) || (JPN_BAND == 1)
static StackRetStatus_t setDutyCycleTimer(LorawanRegionalAttributes_t attr, void *attrInput)
{
	UpdateDutyCycleTimer_t updateDCTimer;
	StackRetStatus_t result = LORAWAN_SUCCESS;
    bool found = 0;
    uint8_t i,bandId;
    
    uint32_t delta = 0, minimSubBandTimer = UINT32_MAX, ticks,nextTimer;
	
	memcpy(&updateDCTimer,attrInput,sizeof(UpdateDutyCycleTimer_t));
	
    //i = RegParams.lastUsedChannelIndex;
	
	// step1: find the sub band of the last used channel
    bandId = RegParams.pOtherChParams[RegParams.lastUsedChannelIndex].subBandId;
	// Return immediately if the last channel used in not under Dutycycle restrictions as per ARIB Spec
	// Band 0 => 920.6 MHz to 922.2 MHz -> Following LBT
	// Band 1 => 922.4 Mhz to 928.0 MHz -> Follwoing both LBT and Dutycycle 
	if (bandId == 0 && RegParams.band == ISM_JPN923)
	{
		return result;
	}
	
	// this duty cycle setting applies only for data frames; if join frame was latest, then return immediately
	if(updateDCTimer.joining != 1)
	{
		// find the new timeout for the subband used for last TX
		RegParams.pSubBandParams[bandId].subBandTimeout = ((uint32_t)updateDCTimer.timeOnAir * ((uint32_t)RegParams.cmnParams.paramsType2.subBandDutyCycle[bandId] - 1));
		// find the new aggregated timeout over all bands
		RegParams.cmnParams.paramsType2.aggregatedDutyCycleTimeout = (uint32_t)updateDCTimer.timeOnAir * ((uint32_t) updateDCTimer.aggDutyCycle - 1);
	}
	else
	{
		//return result;
		RegParams.joinDutyCycleTimeout = (uint32_t)updateDCTimer.timeOnAir * ((uint32_t) updateDCTimer.aggDutyCycle - 1);
	}
	
	// find how much time from last timeout is due yet
	if(SwTimerIsRunning(RegParams.pDutyCycleTimer->timerId))
	{
		SwTimerStop(RegParams.pDutyCycleTimer->timerId);
		ticks = SwTimerReadValue(RegParams.pDutyCycleTimer->timerId);
		
		delta = RegParams.pDutyCycleTimer->lastTimerValue - US_TO_MS(ticks);
	}
	// assume that last-used-subband has the minimum most timeout
	minimSubBandTimer = RegParams.pSubBandParams[bandId].subBandTimeout;
	found = 1;
	
	// walk over all available sub-bands
	for(i = 0; i < RegParams.maxSubBands; i++)
	{ // ignore if this the last used sub-band -- bandId stores it's index
		// cond #1: it is a sub-band other than last used sub-band
		// cond #2: it is a sub-band that cannot be used right now
		// BOTH HAS TO HOLD TRUE
		if((i != bandId) && (RegParams.pSubBandParams[i].subBandTimeout != 0))
		{
			if(RegParams.pSubBandParams[i].subBandTimeout > delta)
			{
				// this sub-band has timeout left yet
				RegParams.pSubBandParams[i].subBandTimeout = 
				          RegParams.pSubBandParams[i].subBandTimeout - delta;
			}
			else
			{// this sub-band timeout has elapsed already
				RegParams.pSubBandParams[i].subBandTimeout = 0;
			}
			
			if(RegParams.pSubBandParams[i].subBandTimeout <= minimSubBandTimer && RegParams.pSubBandParams[i].subBandTimeout != 0)
			{
				// if this is smaller time than last used subband and it has still timeout to elapse then this is the new minimum
				minimSubBandTimer = RegParams.pSubBandParams[i].subBandTimeout;
				found = 1;
			}
		}
	}
	
	// following block works if DutyCycleReq command imposed specific restrictions in addition to the regional parameters regulations
	if(RegParams.cmnParams.paramsType2.aggregatedDutyCycleTimeout != 0)
	{
		if(RegParams.cmnParams.paramsType2.aggregatedDutyCycleTimeout > delta)
		{
			RegParams.cmnParams.paramsType2.aggregatedDutyCycleTimeout = RegParams.cmnParams.paramsType2.aggregatedDutyCycleTimeout - delta;
		}
		else
		{
			RegParams.cmnParams.paramsType2.aggregatedDutyCycleTimeout = 0;
		}
		
		if(RegParams.cmnParams.paramsType2.aggregatedDutyCycleTimeout)
		{
			found = 1;
		}
	}
	
	
	if(found == 1)
	{
		if(RegParams.cmnParams.paramsType2.aggregatedDutyCycleTimeout >= minimSubBandTimer)
		{
			nextTimer = RegParams.cmnParams.paramsType2.aggregatedDutyCycleTimeout;
		}
		else
		{
			nextTimer = minimSubBandTimer;
		}
		
		RegParams.pDutyCycleTimer->lastTimerValue = nextTimer;
		SwTimerStart (RegParams.pDutyCycleTimer->timerId, MS_TO_US(nextTimer), SW_TIMEOUT_RELATIVE, (void *)DutyCycleCallback, NULL);
		//SwTimerSetTimeout(RegParams.pDutyCycleTimer->timerId, MS_TO_TICKS(nextTimer));
		//SwTimerStart(RegParams.pDutyCycleTimer->timerId);
	}
	return result;
}
#endif

static StackRetStatus_t setJoinDutyCycleTimer(LorawanRegionalAttributes_t attr, void *attrInput)
{
	UpdateJoinDutyCycleTimer_t UpdateJoinDutyCycleTimer;
	StackRetStatus_t result = LORAWAN_SUCCESS;
	
	memcpy(&UpdateJoinDutyCycleTimer,attrInput,sizeof(UpdateJoinDutyCycleTimer_t));
	
	uint32_t delta = 0,ticks;
	
	if(UpdateJoinDutyCycleTimer.startJoinDutyCycleTimer == true)
	{
			if(RegParams.joinbccount < AGGREGATEDTIME_1HR)
			{
				RegParams.joinDutyCycleTimeout = (uint32_t)UpdateJoinDutyCycleTimer.joinreqTimeonAir * (JOIN_BACKOFF_PRESCALAR_1HR - 1);
				
			}
			else if (RegParams.joinbccount < AGGREGATEDTIME_10HR)
			{
				RegParams.joinDutyCycleTimeout = (uint32_t)UpdateJoinDutyCycleTimer.joinreqTimeonAir * (JOIN_BACKOFF_PRESCALAR_10HR - 1);
				
			}
			else
			{
				RegParams.joinDutyCycleTimeout = (uint32_t)UpdateJoinDutyCycleTimer.joinreqTimeonAir * (JOIN_BACKOFF_PRESCALAR_24HR - 1);
				
			}
		
				
		// find how much time from last timeout is due yet
		if(SwTimerIsRunning(RegParams.pJoinDutyCycleTimer->timerId))
		{
			
			ticks = SwTimerReadValue(RegParams.pJoinDutyCycleTimer->timerId);
			SwTimerStop(RegParams.pJoinDutyCycleTimer->timerId);
			
			delta = RegParams.pJoinDutyCycleTimer->lastTimerInterval- US_TO_MS(ticks);
		}
		
		// following block works if DutyCycleReq command imposed specific restrictions in addition to the regional parameters regulations
		if(RegParams.joinDutyCycleTimeout != 0)
		{
			if(RegParams.joinDutyCycleTimeout > delta)
			{
				RegParams.joinDutyCycleTimeout = RegParams.joinDutyCycleTimeout - delta;
				if(RegParams.joinDutyCycleTimeout > US_TO_MS(SWTIMER_MAX_TIMEOUT))
				{
					RegParams.pJoinDutyCycleTimer->remainingtime =RegParams.joinDutyCycleTimeout - (US_TO_MS(SWTIMER_MAX_TIMEOUT)) ;
					SwTimerStart(RegParams.pJoinDutyCycleTimer->timerId, SWTIMER_MAX_TIMEOUT, SW_TIMEOUT_RELATIVE, (void *)JoinDutyCycleCallback, NULL);
					RegParams.joinDutyCycleTimeout = RegParams.joinDutyCycleTimeout - (US_TO_MS(SWTIMER_MAX_TIMEOUT));
				
				}
				else
				{
				SwTimerStart (RegParams.pJoinDutyCycleTimer->timerId, MS_TO_US(RegParams.joinDutyCycleTimeout), SW_TIMEOUT_RELATIVE, (void *)JoinDutyCycleCallback, NULL);
				}
			}
			else
			{
				RegParams.joinDutyCycleTimeout = 0;
			}
			
			
		}
	}
	else
	{
		SwTimerStop(RegParams.pJoinDutyCycleTimer->timerId);
		RegParams.joinDutyCycleTimeout = 0;
		RegParams.pJoinDutyCycleTimer->remainingtime =0;
	}
return result;
	
}
static StackRetStatus_t setJoinBackOffTimer(LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	bool startJoinBackOffTimer;
	memcpy(&startJoinBackOffTimer,attrInput,sizeof(bool));
	if(startJoinBackOffTimer == true)
	{
		SwTimerStart (RegParams.pJoinBackoffTimer->timerId, MS_TO_US(BACKOFF_BASE_TIME_IN_MS), SW_TIMEOUT_RELATIVE, (void *)JoinBackoffCallback, NULL);
	}
	else
	{
		SwTimerStop(RegParams.pJoinBackoffTimer->timerId);
		RegParams.joinbccount = 0;
	}
 return result;
}
static StackRetStatus_t setJoinBackoffCntl(LorawanRegionalAttributes_t attr,void *attrInput)
{   
	StackRetStatus_t result = LORAWAN_SUCCESS;
	bool joinbackoffcntl;
	memcpy(&joinbackoffcntl,attrInput,sizeof(bool));
	if(joinbackoffcntl == false)
	{
		if(RegParams.FeaturesSupport & JOIN_BACKOFF_SUPPORT)
		{
			/*Disable joinbackoff */
			RegParams.FeaturesSupport &= ~JOIN_BACKOFF_SUPPORT;
			
			/* Stop Join Backoff timer */
			if (SwTimerIsRunning(RegParams.pJoinBackoffTimer->timerId))
			{
				SwTimerStop(RegParams.pJoinBackoffTimer->timerId);
			}
			/* Stop Join Dutycycle timer */
			if (SwTimerIsRunning(RegParams.pJoinDutyCycleTimer->timerId))
			{
				SwTimerStop(RegParams.pJoinDutyCycleTimer->timerId);
			}
			
		}
	
	}
	else
	{   /*Enable join backoff */
		RegParams.FeaturesSupport |= JOIN_BACKOFF_SUPPORT;
	}
return result;
}
#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t setFrequency(LorawanRegionalAttributes_t attr, void *attrInput)
{
	ValUpdateFreqTx_t updateTxFreq;
	ValChId_t valChid;
	StackRetStatus_t result = LORAWAN_SUCCESS;

	
	memcpy(&updateTxFreq,attrInput,sizeof(ValUpdateFreqTx_t));
	
	valChid.channelIndex = updateTxFreq.channelIndex;
	valChid.allowedForDefaultChannels = WITHOUT_DEFAULT_CHANNELS;

	
	if(pValidateAttr[RX_FREQUENCY](RX_FREQUENCY,&updateTxFreq.frequencyNew) != LORAWAN_SUCCESS || ValidateChannelIdT2(CHANNEL_ID, &valChid) != LORAWAN_SUCCESS)
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		uint8_t chIndx = updateTxFreq.channelIndex;
		if((((1 << RegParams.band) & ((ISM_EUBAND) | (1 << ISM_JPN923))) != 0))
		{
		    RegParams.pOtherChParams[chIndx].subBandId = getSubBandId(updateTxFreq.frequencyNew);
		}
		RegParams.pOtherChParams[chIndx].ulfrequency = updateTxFreq.frequencyNew;
		RegParams.pOtherChParams[chIndx].rx1Frequency = updateTxFreq.frequencyNew;
		RegParams.pOtherChParams[chIndx].parametersDefined |= FREQUENCY_DEFINED;
#if (ENABLE_PDS == 1)
		PDS_STORE(RegParams.regParamItems.ch_param_2_item_id);
#endif

	}
	return result;
}
#endif

#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t setNewChannel(LorawanRegionalAttributes_t attr, void *attrInput)
{
	UpdateNewCh_t newCh;
	StackRetStatus_t result = LORAWAN_INVALID_PARAMETER;
	pUpdateChIdStatus_t pUpdateChidStatus = NULL;
	
	if(((ISM_ASBAND) & (1 << RegParams.band)) != 0 || ((ISM_JPN923) == RegParams.band))
	{
#if (((AS_BAND) == 1) || ((JPN_BAND) == 1))		
		pUpdateChidStatus = UpdateChannelIdStatusT3;
#endif
	}
	else if( ISM_KR920 == RegParams.band)
	{
#if (KR_BAND == 1)
		pUpdateChidStatus = UpdateChannelIdStatusT4;
#endif
	}
	else if( ISM_IND865 == RegParams.band)
	{
#if ((IND_BAND) == 1)
		pUpdateChidStatus = UpdateChannelIdStatus;
#endif
	}
	else
	{
#if ((EU_BAND) == 1)
		pUpdateChidStatus = UpdateChannelIdStatusT2;
#endif
	}
	
	memcpy(&newCh, attrInput,sizeof(UpdateNewCh_t));
	
	ValChMaskCntl_t valchMaskCntl;
	
	valchMaskCntl.chnlMask = newCh.channelMask;
	valchMaskCntl.chnlMaskCntl = newCh.channelMaskCntl;
	
	if(LORAREG_ValidateAttr(CHMASK_CHCNTL,&valchMaskCntl) != LORAWAN_SUCCESS)
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		uint8_t i = 0;
		result = LORAWAN_SUCCESS;
		
		if(newCh.channelMaskCntl == 6)
		{
			for(i = 0; i < RegParams.maxChannels; i++)
			{
				pUpdateChidStatus(i,ENABLED);
			}
		}
		else if(newCh.channelMaskCntl == 0)
		{
			for(i = 0; i < RegParams.maxChannels; i++)
			{
				if((newCh.channelMask & BIT0) == BIT0)
				{
					pUpdateChidStatus(i, ENABLED);
				}
				else
				{
					pUpdateChidStatus(i,DISABLED);
				}
				newCh.channelMask = newCh.channelMask >> SHIFT1;
			}
		}
	}
	return result;
}
#endif

#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t setDlFrequency(LorawanRegionalAttributes_t attr, void *attrInput)
{
	ValUpdateFreqTx_t updateDlFreq;
	StackRetStatus_t result = LORAWAN_SUCCESS;
	ValChId_t Chid;

		
	memcpy(&updateDlFreq,attrInput,sizeof(ValUpdateFreqTx_t));
	
	Chid.channelIndex = updateDlFreq.channelIndex;
		Chid.allowedForDefaultChannels = ALL_CHANNELS;
// 	
// 	if(((1 << (ISM_EUBAND)) & RegParams.band) != 0)
// 	{
// 		Chid.allowedForDefaultChannels = ALL_CHANNELS;
// 	}
// 	
	

	if(pValidateAttr[RX_FREQUENCY](RX_FREQUENCY, &updateDlFreq.frequencyNew) != LORAWAN_SUCCESS || ValidateChannelIdT2(CHANNEL_ID, &Chid) != LORAWAN_SUCCESS)
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	else
	{
		RegParams.pOtherChParams[updateDlFreq.channelIndex].rx1Frequency = updateDlFreq.frequencyNew;
#if (ENABLE_PDS == 1)
		PDS_STORE(RegParams.regParamItems.ch_param_2_item_id);
#endif
	}
	
	return result;
}
#endif

#if ( AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t setTxParams(LorawanRegionalAttributes_t attr, void *attrInput)
{
	TxParams_t updateTxParams;

	memcpy(&updateTxParams,attrInput,sizeof(TxParams_t));
	
	RegParams.cmnParams.paramsType2.txParams.uplinkDwellTime = updateTxParams.uplinkDwellTime;
	if (RegParams.cmnParams.paramsType2.txParams.uplinkDwellTime == 1)
	{
		RegParams.maxDataRate = DR2;
	}
	else
	{
		RegParams.maxDataRate = DR0;
	}
	RegParams.cmnParams.paramsType2.txParams.downlinkDwellTime = updateTxParams.downlinkDwellTime;
	RegParams.maxTxPwr = updateTxParams.maxEIRP;
	
	return LORAWAN_SUCCESS;
}
#endif

#if (JPN_BAND == 1 || KR_BAND == 1)

/*
 * \brief Timer callback for updating the duty cycle timer
 * \param Timer Parameters
 */

static void LBTChannelPauseCallback (uint8_t param)
{
    uint32_t minim = UINT32_MAX;
    bool found = 0;
    uint8_t i;
	LBTTimer_t *pLBTTimer;
	
	pLBTTimer = &RegParams.cmnParams.paramsType2.LBTTimer;

    for (i=0; i < RegParams.maxChannels; i++)
    {
        //Validate this only for enabled channels
        if ((RegParams.pChParams[i].status == ENABLED) && ( RegParams.cmnParams.paramsType2.channelTimer[i] != 0 ))
        {
            if ( RegParams.cmnParams.paramsType2.channelTimer[i] > pLBTTimer->lastTimerValue)
            {
                RegParams.cmnParams.paramsType2.channelTimer[i] = RegParams.cmnParams.paramsType2.channelTimer[i] - pLBTTimer->lastTimerValue;
            }
            else
            {
                RegParams.cmnParams.paramsType2.channelTimer[i] = 0;
            }
            if ( (RegParams.cmnParams.paramsType2.channelTimer[i] <= minim) && (RegParams.cmnParams.paramsType2.channelTimer[i] != 0) )
            {
                minim  = RegParams.cmnParams.paramsType2.channelTimer[i];
                found = 1;
            }
        }
    }
    if ( found == 1 )
    {
        pLBTTimer->lastTimerValue = minim;
        SwTimerStart (pLBTTimer->timerId, MS_TO_US(minim), SW_TIMEOUT_RELATIVE, (void *)LBTChannelPauseCallback, NULL);
    }
}


static StackRetStatus_t setLBTTimer(LorawanRegionalAttributes_t attr, void *attrInput)
{
    bool found = 0;
    uint8_t i;
    uint32_t delta = 0, minim = UINT32_MAX, ticks;
	LBTTimer_t *pLBTTimer;

    i = RegParams.lastUsedChannelIndex;
	pLBTTimer = &RegParams.cmnParams.paramsType2.LBTTimer;
	
	RegParams.cmnParams.paramsType2.channelTimer[i] = LBT_TRANSMIT_CHANNEL_PAUSE_DURATION;
	
	if(SwTimerIsRunning(pLBTTimer->timerId))
	{
		SwTimerStop(pLBTTimer->timerId);
		
		ticks = SwTimerReadValue(pLBTTimer->timerId);
		delta = pLBTTimer->lastTimerValue - US_TO_MS(ticks);
	}
	else
	{
		minim = RegParams.cmnParams.paramsType2.channelTimer[i];
		found = 1;
	}
	
	for(i = 0; i < RegParams.maxChannels; i++)
	{
		if(i != RegParams.lastUsedChannelIndex)
		{
			if((RegParams.pChParams[i].status == ENABLED) && (RegParams.cmnParams.paramsType2.channelTimer[i] != 0))
			{
				if(RegParams.cmnParams.paramsType2.channelTimer[i] > delta)
				{
					RegParams.cmnParams.paramsType2.channelTimer[i] = RegParams.cmnParams.paramsType2.channelTimer[i] - delta;
				}
				else
				{
					RegParams.cmnParams.paramsType2.channelTimer[i] = 0;
				}
				if((RegParams.cmnParams.paramsType2.channelTimer[i] <= minim) && (RegParams.cmnParams.paramsType2.channelTimer[i] != 0))
				{
					minim = RegParams.cmnParams.paramsType2.channelTimer[i];
					found = 1;
				}
			}
		}
	}
	if((found == 1) && minim)
	{
		pLBTTimer->lastTimerValue = minim;
		SwTimerStart (pLBTTimer->timerId, MS_TO_US(minim), SW_TIMEOUT_RELATIVE, (void *)LBTChannelPauseCallback, NULL);
	}
	return LORAWAN_SUCCESS;
}
#endif

StackRetStatus_t LORAREG_SupportedBands(uint16_t *bands)
{
	StackRetStatus_t  status = LORAWAN_SUCCESS;
	uint16_t value = 0;
	
	value  = value |
	#if (AS_BAND == 1)
	    (ISM_ASBAND) |
	#endif
	#if (AU_BAND == 1)
	    (1 << (ISM_AU915)) |
	#endif
	#if (EU_BAND == 1)
	    (ISM_EUBAND) |
	#endif
	#if (IND_BAND == 1)
	    (1 <<(ISM_IND865)) |
	#endif
	#if (JPN_BAND == 1)
	(1 <<(ISM_JPN923)) |
	#endif
	#if (KR_BAND == 1)
	    (1 <<(ISM_KR920)) |
	#endif
	#if (NA_BAND == 1)
	    (1 <<(ISM_NA915)) |
	#endif
	    0 ;
	*bands = value;
	
	return status;
}

StackRetStatus_t LORAREG_UnInit(void)
{
	StackRetStatus_t result = LORAWAN_INVALID_PARAMETER;
	
	StopAllRegSoftwareTimers();

#if (ENABLE_PDS == 1)
	/* Do not reset the mac fild id1 */
	if(RegParams.regParamItems.fileid)
	{
	    PDS_UnRegFile(RegParams.regParamItems.fileid);
		if(RegParams.band == ISM_EU868)
		{
			PDS_UnRegFile(PDS_FILE_REG_EU868_12_IDX);
		}
	}
#endif	
	memset(&RegParams,0,sizeof(RegParams_t));
	
	return result;
}

/*
 * \brief Sets the channel update status after successful Join procedure.
 * \param[in] None
 * \retval LORAWAN_SUCCESS for both NA915 and AU915.
 *		   LORAWAN_INVALID_PARAMETER for all other regions
 */
#if (NA_BAND == 1 || AU_BAND == 1)
static StackRetStatus_t setJoinSuccess(LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t status = LORAWAN_SUCCESS;
#if (RANDOM_NW_ACQ == 1)
	uint8_t lastUsedSB;
	/* Temporarily making it 8 to disable the remaining channels and after the check, will be changed it to 0 */
	if(RegParams.cmnParams.paramsType1.lastUsedSB == 0)
	{
		lastUsedSB = 8;
	}
	else
	{
		lastUsedSB = RegParams.cmnParams.paramsType1.lastUsedSB;
	}
		
	for(uint8_t i = 0; i < (NO_OF_CH_IN_SUBBAND * (MAX_SUBBANDS + 1)); i++)
	{
		if(((i < MAX_CHANNELS_BANDWIDTH_125_AU_NA) && ((i < ((lastUsedSB - 1) * NO_OF_CH_IN_SUBBAND)) 
			|| (i >= (((lastUsedSB - 1) * NO_OF_CH_IN_SUBBAND) + 8) )))
			|| ((i >= MAX_CHANNELS_BANDWIDTH_125_AU_NA) && (i != lastUsedSB + MAX_CHANNELS_BANDWIDTH_125_AU_NA - 1)))
		{
			RegParams.pChParams[i].status = DISABLED;	
		}
	}
#if (ENABLE_PDS == 1)
	PDS_STORE(RegParams.regParamItems.ch_param_1_item_id);
#endif
#endif
	return status;
}

static StackRetStatus_t setEnableAllChs(LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t status = LORAWAN_SUCCESS;
#if (RANDOM_NW_ACQ == 1)		
	for(uint8_t i = 0; i < (NO_OF_CH_IN_SUBBAND * (MAX_SUBBANDS + 1)); i++)
	{
		RegParams.pChParams[i].status = ENABLED;	
	}
	RegParams.cmnParams.paramsType1.lastUsedSB = 0;
#if (ENABLE_PDS == 1)
	PDS_STORE(RegParams.regParamItems.ch_param_1_item_id);
	PDS_STORE(RegParams.regParamItems.lastUsedSB);
#endif
#endif	
	return status;
}
#endif
#if (EU_BAND == 1 || AS_BAND == 1 || IND_BAND == 1 || JPN_BAND == 1 || KR_BAND == 1)
static StackRetStatus_t setChlistDefaultState(LorawanRegionalAttributes_t attr, void *attrInput)
{
	StackRetStatus_t status = LORAWAN_SUCCESS;
	/* Traverse the entire channel list and disable the all the Channel status except Default channels */
	for (uint8_t i = 0; i <= RegParams.maxChannels; i++)
	{
		if(RegParams.pOtherChParams[i].joinRequestChannel != true)
		{
			RegParams.pChParams[i].status = DISABLED;
		}
	}
	return status;
}
#endif
void Enableallchannels()
{
	for(uint8_t i = 0; i < (NO_OF_CH_IN_SUBBAND * (MAX_SUBBANDS + 1)); i++)
	{
		RegParams.pChParams[i].status = ENABLED;
	}
	RegParams.cmnParams.paramsType1.lastUsedSB = 0;
#if (ENABLE_PDS == 1)
	PDS_STORE(RegParams.regParamItems.ch_param_1_item_id);
	PDS_STORE(RegParams.regParamItems.lastUsedSB);
#endif
}
StackRetStatus_t LORAREG_EnableallChannels(IsmBand_t ismBand)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
	if(ismBand == ISM_EU868)
	{
		 InitDefault868Channels();
	}
	else if (ismBand == ISM_EU433)
	{
		 InitDefault433Channels();
	}
	else if(ismBand == ISM_JPN923)
	{
		 InitDefault920Channels();	
	}
	else if (ismBand == ISM_KR920)
	{
		 InitDefault920ChannelsKR();
	}
	else if (ismBand == ISM_ASBAND)
	{
		 InitDefault923Channels ();
	}
	else if(ismBand == ISM_AU915 || ismBand == ISM_NA915)
	{
		 Enableallchannels();
	}
	return result;
}


