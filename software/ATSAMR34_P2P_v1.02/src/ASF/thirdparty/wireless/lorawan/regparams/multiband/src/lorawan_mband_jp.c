/**
* \file  lorawan_jp.c
*
* \brief LoRaWAN JAPAN file
*		
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

/************************ PRIVATE FUNCTION PROTOTYPES *************************/
#include "lorawan_multiband.h"

#if (JPN_BAND == 1)
/*Init Functions's*/



/*****************************CONSTANTS ***************************************/
static const ChannelParams_t DefaultChannels923JP[] = {
	LC0_923_JP,
	LC1_923_JP
};

static const OthChannelParams_t AdvChannels923JP[] = {
	ADV_LC0_923_JP,
	ADV_LC1_923_JP
};

static const DRParams_t DefaultDrParamsJP[] = {
	DR0_PARAMS_JP,
	DR1_PARAMS_JP,
	DR2_PARAMS_JP,
	DR3_PARAMS_JP,
	DR4_PARAMS_JP,
	DR5_PARAMS_JP,
	DR6_PARAMS_JP,
	DR7_PARAMS_JP,
};

static const SubBandParams_t SubBandParamsJP923[] =
{
	SB0_923_JP,
	SB1_923_JP
};

static const uint16_t SubBandDutyCycleJP923[] =
{
	SB0_923_JP_DC_0,
	SB1_923_JP_DC_0
};


#if (ENABLE_PDS == 1)

#define PDS_REG_JPN_CH_PARAM_1_ADDR                      ((uint8_t *)&(RegParams.cmnParams.paramsType2.chParams[MAX_CHANNELS_T2 - MAX_CHANNELS_T2]))
#define PDS_REG_JPN_CH_PARAM_2_ADDR                      ((uint8_t *)&(RegParams.cmnParams.paramsType2.othChParams[MAX_CHANNELS_T2 - MAX_CHANNELS_T2]))

#define PDS_REG_JPN_CH_PARAM_1_SIZE					     sizeof(RegParams.cmnParams.paramsType2.chParams)
#define PDS_REG_JPN_CH_PARAM_2_SIZE					     sizeof(RegParams.cmnParams.paramsType2.othChParams)

#define PDS_REG_JPN_CH_PARAM_1_OFFSET                     (PDS_FILE_START_OFFSET)
#define PDS_REG_JPN_CH_PARAM_2_OFFSET                     (PDS_REG_JPN_CH_PARAM_1_OFFSET + PDS_SIZE_OF_ITEM_HDR + PDS_REG_JPN_CH_PARAM_1_SIZE)

/* PDS Reg Params JPN Item declaration */

const ItemMap_t pds_reg_jpn_fid1_item_list[] = {
	DECLARE_ITEM(PDS_REG_JPN_CH_PARAM_1_ADDR,
	PDS_FILE_REG_JPN_08_IDX,
	(uint8_t)PDS_REG_JPN_CH_PARAM_1,
	PDS_REG_JPN_CH_PARAM_1_SIZE,
	PDS_REG_JPN_CH_PARAM_1_OFFSET),
	DECLARE_ITEM(PDS_REG_JPN_CH_PARAM_2_ADDR,
	PDS_FILE_REG_JPN_08_IDX,
	(uint8_t)PDS_REG_JPN_CH_PARAM_2,
	PDS_REG_JPN_CH_PARAM_2_SIZE,
	PDS_REG_JPN_CH_PARAM_2_OFFSET)
};

PdsOperations_t aRegJpnFid1PdsOps[PDS_REG_JPN_FID1_MAX_VALUE];


/* PDS Callback function */
void LorawanReg_JPN_Pds_Cb(void);

#endif
#endif

/****************************** FUNCTIONS *************************************/

StackRetStatus_t LORAReg_InitJP(IsmBand_t ismBand)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;

#if (JPN_BAND == 1)

	RegParams.TxCurDataRate = MAC_DEF_TX_CURRENT_DATARATE_JP;
	RegParams.maxChannels = MAX_CHANNELS_JP;
	RegParams.maxSubBands = MAX_NUM_SUBBANDS_JP;
	RegParams.MacTxPower = MAC_DEF_TX_POWER_JP;
	RegParams.pChParams = &RegParams.cmnParams.paramsType2.chParams[0];
	RegParams.pDrParams = &RegParams.cmnParams.paramsType2.DRParams[0];
	RegParams.pSubBandParams = &RegParams.cmnParams.paramsType2.SubBands[0];
	RegParams.pOtherChParams = &RegParams.cmnParams.paramsType2.othChParams[0];
	RegParams.pDutyCycleTimer = &RegParams.cmnParams.paramsType2.DutyCycleTimer;
    RegParams.pJoinDutyCycleTimer = &RegParams.joinDutyCycleTimer;
	RegParams.pJoinBackoffTimer = &RegParams.joinBackoffTimer;
	RegParams.DefRx1DataRate = MAC_RX1_WINDOW_DATARATE_JP;
	RegParams.DefRx2DataRate = MAC_RX2_WINDOW_DATARATE_JP;
	RegParams.DefRx2Freq = MAC_RX2_WINDOW_FREQ_JP;	
	RegParams.MinNewChIndex = MIN_CHANNEL_INDEX_JP;
	RegParams.FeaturesSupport = FEATURES_SUPPORTED_JP;
	RegParams.minDataRate = MAC_DATARATE_MIN_JP;
	RegParams.maxDataRate = MAC_DATARATE_MAX_JP;
	RegParams.cmnParams.paramsType2.LBTScanPeriod = LBT_SCAN_PERIOD_JP;
	RegParams.cmnParams.paramsType2.LBTSignalThreshold = LBT_SIGNAL_THRESHOLD_JP;
	RegParams.cmnParams.paramsType2.LBT_RSSISamplesCount = LBT_RSSI_SAMPLES_COUNT_JP;
	RegParams.cmnParams.paramsType2.minNonDefChId = 2;
	RegParams.Rx1DrOffset = 7;
	RegParams.maxTxPwrIndx = 7;
	RegParams.defTxPwrIndx = MAC_DEF_TX_POWER_JP;
	RegParams.maxTxPwr = DEFAULT_EIRP_JP;
	RegParams.cmnParams.paramsType2.LBTTimer.timerId = regTimerId[0];
	RegParams.pDutyCycleTimer->timerId = regTimerId[1];
	RegParams.pJoinBackoffTimer->timerId = regTimerId[2];
    RegParams.pJoinDutyCycleTimer->timerId = regTimerId[3];
	RegParams.pJoinDutyCycleTimer->remainingtime =0;
	RegParams.joinbccount =0;
	RegParams.joinDutyCycleTimeout =0;
	RegParams.cmnParams.paramsType2.txParams.uplinkDwellTime = 1;
	RegParams.cmnParams.paramsType2.txParams.downlinkDwellTime = 1;
	RegParams.band = ismBand;
	
	if(ismBand == ISM_JPN923)
	{
		InitDefault920Channels();
		RegParams.cmnParams.paramsType2.txParams.maxEIRP = DEFAULT_EIRP_JP;//MAX_EIRP_JP;
		memcpy (RegParams.pDrParams, DefaultDrParamsJP, sizeof(DefaultDrParamsJP) );
#if (ENABLE_PDS == 1)

		/*Fill PDS item id in RegParam Structure */
		RegParams.regParamItems.fileid = PDS_FILE_REG_JPN_08_IDX;
		RegParams.regParamItems.ch_param_1_item_id = PDS_REG_JPN_CH_PARAM_1;
		RegParams.regParamItems.ch_param_2_item_id = PDS_REG_JPN_CH_PARAM_2;
		RegParams.regParamItems.band_item_id = 0;
		RegParams.regParamItems.lastUsedSB = 0;
		
		PdsFileMarks_t filemarks;
		/* File ID JPN - Register */
		filemarks.fileMarkListAddr = aRegJpnFid1PdsOps;
		filemarks.numItems =  (uint8_t)(PDS_REG_JPN_FID1_MAX_VALUE & 0x00FF);
		filemarks.itemListAddr = (ItemMap_t *)&pds_reg_jpn_fid1_item_list;
		filemarks.fIDcb = LorawanReg_JPN_Pds_Cb;
		PDS_RegFile(PDS_FILE_REG_JPN_08_IDX,filemarks);

#endif		
	}
	else
	{
		result = LORAWAN_INVALID_PARAMETER;
	}
	
	
    LORAREG_InitGetAttrFnPtrsJP();
	LORAREG_InitValidateAttrFnPtrsJP();
	LORAREG_InitSetAttrFnPtrsJP();
#else
    result = UNSUPPORTED_BAND;
#endif
	return result;
}


/*
 * \brief This function initializes all the IN865 Channels to default values
 */
#if (JPN_BAND == 1)
void InitDefault920Channels (void)
{
    uint8_t i;
    memset (RegParams.pChParams, 0, sizeof(DefaultChannels923JP) );
	memset (RegParams.pOtherChParams, 0, sizeof(AdvChannels923JP) );
    memcpy (RegParams.pChParams, DefaultChannels923JP, sizeof(DefaultChannels923JP) );
	memcpy (RegParams.pOtherChParams, AdvChannels923JP, sizeof(AdvChannels923JP) );
	memset (RegParams.pSubBandParams, 0, sizeof(SubBandParamsJP923) );
	memcpy (RegParams.pSubBandParams, SubBandParamsJP923, sizeof(SubBandParamsJP923) );
	memset (RegParams.cmnParams.paramsType2.subBandDutyCycle,0,sizeof(SubBandDutyCycleJP923));
	memcpy (RegParams.cmnParams.paramsType2.subBandDutyCycle,SubBandDutyCycleJP923,sizeof(SubBandDutyCycleJP923));
    for (i = 2; i < RegParams.maxChannels; i++)
    {
	    RegParams.pChParams[i].dataRange.value = UINT8_MAX;
		RegParams.cmnParams.paramsType2.channelTimer[i] = 0;
    }
	RegParams.lastUsedChannelIndex = UINT8_MAX;
}
#if (ENABLE_PDS == 1)
/* PDS Callback function */
void LorawanReg_JPN_Pds_Cb(void)
{

}
#endif
#endif
