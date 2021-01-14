/**
* \file  lorawan_as.c
*
* \brief LoRaWAN as file
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

#if (AS_BAND == 1)
/*Init Functions's*/



/*****************************CONSTANTS ***************************************/
static const ChannelParams_t DefaultChannels923[] = {
	LC0_923,
	LC1_923
};

static const OthChannelParams_t AdvChannels923[] = {
	ADV_LC0_923,
	ADV_LC1_923
};

static const DRParams_t DefaultDrParamsAS[] = {
	DR0_PARAMS_AS,
	DR1_PARAMS_AS,
	DR2_PARAMS_AS,
	DR3_PARAMS_AS,
	DR4_PARAMS_AS,
	DR5_PARAMS_AS,
	DR6_PARAMS_AS,
	DR7_PARAMS_AS,
};

static const SubBandParams_t SubBandParams923[] =
{
	SB0_923
};

static const uint16_t SubBandDutyCycle923[] =
{
	SB0_923_DC_0
};

#if (ENABLE_PDS == 1)
#define PDS_REG_AS_CH_PARAM_1_ADDR                      ((uint8_t *)&(RegParams.cmnParams.paramsType2.chParams[MAX_CHANNELS_T2 - MAX_CHANNELS_T2]))
#define PDS_REG_AS_CH_PARAM_2_ADDR                      ((uint8_t *)&(RegParams.cmnParams.paramsType2.othChParams[MAX_CHANNELS_T2 - MAX_CHANNELS_T2]))
#define PDS_REG_AS_BAND_ADDR                            ((uint8_t *)&(RegParams.band))

#define PDS_REG_AS_CH_PARAM_1_SIZE					    sizeof(RegParams.cmnParams.paramsType2.chParams)
#define PDS_REG_AS_CH_PARAM_2_SIZE					    sizeof(RegParams.cmnParams.paramsType2.othChParams)
#define PDS_REG_AS_BAND_SIZE                            sizeof(RegParams.band)

#define PDS_REG_AS_CH_PARAM_1_OFFSET                    (PDS_FILE_START_OFFSET)
#define PDS_REG_AS_CH_PARAM_2_OFFSET                    (PDS_REG_AS_CH_PARAM_1_OFFSET + PDS_SIZE_OF_ITEM_HDR + PDS_REG_AS_CH_PARAM_1_SIZE)
#define PDS_REG_AS_BAND_OFFSET                          (PDS_REG_AS_CH_PARAM_2_OFFSET + PDS_SIZE_OF_ITEM_HDR + PDS_REG_AS_CH_PARAM_2_SIZE)

/* PDS Reg Params NA Item declaration */

const ItemMap_t pds_reg_as_item_list[] = {
	DECLARE_ITEM(PDS_REG_AS_CH_PARAM_1_ADDR,
	PDS_FILE_REG_AS_05_IDX,
	(uint8_t)PDS_REG_AS_CH_PARAM_1,
	PDS_REG_AS_CH_PARAM_1_SIZE,
	PDS_REG_AS_CH_PARAM_1_OFFSET),
	DECLARE_ITEM(PDS_REG_AS_CH_PARAM_2_ADDR,
	PDS_FILE_REG_AS_05_IDX,
	(uint8_t)PDS_REG_AS_CH_PARAM_2,
	PDS_REG_AS_CH_PARAM_2_SIZE,
	PDS_REG_AS_CH_PARAM_2_OFFSET),	
	DECLARE_ITEM(PDS_REG_AS_BAND_ADDR,
	PDS_FILE_REG_AS_05_IDX,
	(uint8_t)PDS_REG_AS_BAND,
	PDS_REG_AS_BAND_SIZE,
	PDS_REG_AS_BAND_OFFSET)
};

PdsOperations_t aRegAsPdsOps[PDS_REG_AS_MAX_VALUE];

/* PDS Callback function */
void LorawanReg_AS_Pds_Cb(void);
#endif
#endif
/****************************** FUNCTIONS *************************************/

StackRetStatus_t LORAReg_InitAS(IsmBand_t ismBand)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
#if(AS_BAND == 1)
	static bool initialized = false;
	
	RegParams.TxCurDataRate = MAC_DEF_TX_CURRENT_DATARATE_AS;
	RegParams.maxChannels = MAX_CHANNELS_AS;
	RegParams.maxSubBands = MAX_NUM_SUBBANDS_AS;
	RegParams.MacTxPower = MAC_DEF_TX_POWER_AS;
	RegParams.pChParams = &RegParams.cmnParams.paramsType2.chParams[0];
	RegParams.pDrParams = &RegParams.cmnParams.paramsType2.DRParams[0];
	RegParams.pOtherChParams = &RegParams.cmnParams.paramsType2.othChParams[0];
	RegParams.pDutyCycleTimer = &RegParams.cmnParams.paramsType2.DutyCycleTimer;
	RegParams.pJoinDutyCycleTimer = &RegParams.joinDutyCycleTimer;
	RegParams.pJoinBackoffTimer = &RegParams.joinBackoffTimer;
	RegParams.pSubBandParams = &RegParams.cmnParams.paramsType2.SubBands[0];
	RegParams.DefRx1DataRate = MAC_RX1_WINDOW_DATARATE_AS;
	RegParams.DefRx2DataRate = MAC_RX2_WINDOW_DATARATE_AS;
	RegParams.DefRx2Freq = MAC_RX2_WINDOW_FREQ_AS;	
	RegParams.MinNewChIndex = NEW_CHANNEL_INDEX_AS;
	RegParams.FeaturesSupport = FEATURES_SUPPORTED_AS;
	RegParams.minDataRate = MAC_DATARATE_MIN_AS;
	RegParams.maxDataRate = MAC_DATARATE_MAX_AS;
	RegParams.Rx1DrOffset = 7;
	RegParams.maxTxPwrIndx = MAX_TX_PWR_INDEX_AS;
	RegParams.defTxPwrIndx = MAC_DEF_TX_POWER_AS;
	RegParams.maxTxPwr = DEFAULT_EIRP_AS;
	RegParams.cmnParams.paramsType2.minNonDefChId = 2;
	RegParams.pDutyCycleTimer->timerId = regTimerId[0];
	RegParams.pJoinBackoffTimer->timerId = regTimerId[1];
    RegParams.pJoinDutyCycleTimer->timerId = regTimerId[2];
	RegParams.pJoinDutyCycleTimer->remainingtime = 0;
	RegParams.joinbccount =0;
	RegParams.joinDutyCycleTimeout =0;
	RegParams.cmnParams.paramsType2.txParams.uplinkDwellTime = 1;
	RegParams.cmnParams.paramsType2.txParams.downlinkDwellTime = 1;
	
	RegParams.band = ismBand;
	
	if(ismBand >= ISM_BRN923 && ismBand <= ISM_VTM923)
	{
		InitDefault923Channels ();
		RegParams.cmnParams.paramsType2.txParams.maxEIRP = DEFAULT_EIRP_AS;
		memcpy (RegParams.pDrParams, DefaultDrParamsAS, sizeof(DefaultDrParamsAS) );
#if (ENABLE_PDS == 1)
		/*Fill PDS item id in RegParam Structure */
		RegParams.regParamItems.fileid = PDS_FILE_REG_AS_05_IDX;
		RegParams.regParamItems.ch_param_1_item_id = PDS_REG_AS_CH_PARAM_1;
		RegParams.regParamItems.ch_param_2_item_id = PDS_REG_AS_CH_PARAM_2;
		RegParams.regParamItems.band_item_id = PDS_REG_AS_BAND;
		RegParams.regParamItems.lastUsedSB = 0;
		/* File ID AS923 - Register */
		PdsFileMarks_t filemarks;
		filemarks.fileMarkListAddr = aRegAsPdsOps;
		filemarks.numItems =  (uint8_t)(PDS_REG_AS_MAX_VALUE & 0x00FF);
		filemarks.itemListAddr = (ItemMap_t *)&pds_reg_as_item_list;
		filemarks.fIDcb = LorawanReg_AS_Pds_Cb;
		PDS_RegFile(PDS_FILE_REG_AS_05_IDX,filemarks);
#endif		
	}
	else
	{
		result =  LORAWAN_INVALID_PARAMETER;
	}
	
	if(!initialized)
	{
		initialized = true;
	}

    LORAREG_InitGetAttrFnPtrsAS();	
	LORAREG_InitValidateAttrFnPtrsAS();
	LORAREG_InitSetAttrFnPtrsAS();
#if (ENABLE_PDS == 1)
	PDS_STORE(RegParams.regParamItems.band_item_id);
#endif
	
#else
	result = UNSUPPORTED_BAND;
#endif
	return result;
}


/*
 * \brief This function initializes all the AS923 Channels to default values
 */
#if(AS_BAND == 1)
void InitDefault923Channels (void)
{
	uint8_t i;

	memset (RegParams.pChParams, 0, sizeof(DefaultChannels923) );
	memset (RegParams.pOtherChParams, 0, sizeof(AdvChannels923) );
	memcpy (RegParams.pChParams, DefaultChannels923, sizeof(DefaultChannels923));
	memcpy (RegParams.pOtherChParams, AdvChannels923, sizeof(AdvChannels923));
	memset (RegParams.pSubBandParams, 0, sizeof(SubBandParams923) );
	memcpy (RegParams.pSubBandParams, SubBandParams923, sizeof(SubBandParams923) );
	memset(RegParams.cmnParams.paramsType2.subBandDutyCycle,0,sizeof(SubBandDutyCycle923));
	memcpy(RegParams.cmnParams.paramsType2.subBandDutyCycle,SubBandDutyCycle923,sizeof(SubBandDutyCycle923));
	for (i = 2; i < RegParams.maxChannels; i++)
	{
		RegParams.pChParams[i].dataRange.value = UINT8_MAX;
	}
}
#if (ENABLE_PDS == 1)
void LorawanReg_AS_Pds_Cb(void)
{
	; // nothing to do
}
#endif
#endif
