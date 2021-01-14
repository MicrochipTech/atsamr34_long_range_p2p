/**
* \file  lorawan_na.c
*
* \brief LoRaWAN na file
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
#if(NA_BAND == 1)
/*Init Functions's*/
static void InitDefault915Channels (void);


/*****************************CONSTANTS ***************************************/
static const ChannelParams_t DefaultChannels915 [MAX_CHANNELS_T1] =   {  LC0_915_NA,  LC1_915_NA,  LC2_915_NA,  LC3_915_NA,  LC4_915_NA,  LC5_915_NA,
	                                                                     LC6_915_NA,  LC7_915_NA,  LC8_915_NA,  LC9_915_NA,  LC10_915_NA, LC11_915_NA,
	                                                                     LC12_915_NA, LC13_915_NA, LC14_915_NA, LC15_915_NA, LC16_915_NA, LC17_915_NA,
	                                                                     LC18_915_NA, LC19_915_NA, LC20_915_NA, LC21_915_NA, LC22_915_NA, LC23_915_NA,
	                                                                     LC24_915_NA, LC25_915_NA, LC26_915_NA, LC27_915_NA, LC28_915_NA, LC29_915_NA,
	                                                                     LC30_915_NA, LC31_915_NA, LC32_915_NA, LC33_915_NA, LC34_915_NA, LC35_915_NA,
	                                                                     LC36_915_NA, LC37_915_NA, LC38_915_NA, LC39_915_NA, LC40_915_NA, LC41_915_NA,
	                                                                     LC42_915_NA, LC43_915_NA, LC44_915_NA, LC45_915_NA, LC46_915_NA, LC47_915_NA,
	                                                                     LC48_915_NA, LC49_915_NA, LC50_915_NA, LC51_915_NA, LC52_915_NA, LC53_915_NA,
	                                                                     LC54_915_NA, LC55_915_NA, LC56_915_NA, LC57_915_NA, LC58_915_NA, LC59_915_NA,
	                                                                     LC60_915_NA, LC61_915_NA, LC62_915_NA, LC63_915_NA, LC64_915_NA, LC65_915_NA,
                                                                         LC66_915_NA, LC67_915_NA, LC68_915_NA, LC69_915_NA, LC70_915_NA, LC71_915_NA };

static const DRParams_t DefaultDrParamsNA[] = {
	DR0_PARAMS_NA,
	DR1_PARAMS_NA,
	DR2_PARAMS_NA,
	DR3_PARAMS_NA,
	DR4_PARAMS_NA,
	DR5_PARAMS_NA,
	DR6_PARAMS_NA,
	DR7_PARAMS_NA,
	DR8_PARAMS_NA,
	DR9_PARAMS_NA,
	DR10_PARAMS_NA,
	DR11_PARAMS_NA,
	DR12_PARAMS_NA,
	DR13_PARAMS_NA
};

#if (ENABLE_PDS == 1)

#define PDS_REG_NA_CH_PARAM_ADDR                    ((uint8_t *)&(RegParams.cmnParams.paramsType1.chParams[MAX_CHANNELS_T1 - MAX_CHANNELS_T1]))
#define PDS_REG_NA_LAST_USED_SB_ADDR                ((uint8_t *)&(RegParams.cmnParams.paramsType1.lastUsedSB))

#define PDS_REG_NA_CH_PARAM_SIZE                    sizeof(RegParams.cmnParams.paramsType1.chParams)
#define PDS_REG_NA_LAST_USED_SB_SIZE                sizeof(RegParams.cmnParams.paramsType1.lastUsedSB)

#define PDS_REG_NA_CH_PARAM_OFFSET                  (PDS_FILE_START_OFFSET)
#define PDS_REG_NA_LAST_USED_SB_OFFSET              (PDS_REG_NA_CH_PARAM_OFFSET + PDS_SIZE_OF_ITEM_HDR + PDS_REG_NA_CH_PARAM_SIZE)

/* PDS Reg Params NA Item declaration */

const ItemMap_t pds_reg_na_item_list[] = {

	DECLARE_ITEM(PDS_REG_NA_CH_PARAM_ADDR,
	PDS_FILE_REG_NA_03_IDX,
	(uint8_t)PDS_REG_NA_CH_PARAM,
	PDS_REG_NA_CH_PARAM_SIZE,
	PDS_REG_NA_CH_PARAM_OFFSET),
	DECLARE_ITEM(PDS_REG_NA_LAST_USED_SB_ADDR,
	PDS_FILE_REG_NA_03_IDX,
	(uint8_t)PDS_REG_NA_LAST_USED_SB,
	PDS_REG_NA_LAST_USED_SB_SIZE,
	PDS_REG_NA_LAST_USED_SB_OFFSET)
};

PdsOperations_t aRegNaPdsOps[PDS_REG_NA_MAX_VALUE];

/* PDS Callback */
void LorawanReg_NA_Pds_Cb(void);

#endif
#endif

/****************************** FUNCTIONS *************************************/

StackRetStatus_t LORAReg_InitNA(IsmBand_t ismBand)
{
	StackRetStatus_t result = LORAWAN_SUCCESS;
#if(NA_BAND == 1)
    RegParams.TxCurDataRate = MAC_DEF_TX_CURRENT_DATARATE_NA;
	RegParams.maxChannels = MAX_CHANNELS_T1;
	RegParams.MacTxPower = MAC_DEF_TX_POWER_NA;
	RegParams.maxTxPwr = DEFAULT_EIRP_NA;
	RegParams.pChParams = &RegParams.cmnParams.paramsType1.chParams[0];
	RegParams.pDrParams = &RegParams.cmnParams.paramsType1.DRParams[0];
	RegParams.MinNewChIndex = 0xFF;
	RegParams.DefRx1DataRate = MAC_RX1_WINDOW_DATARATE_NA;
	RegParams.DefRx2DataRate = MAC_RX2_WINDOW_DATARATE_NA;
	RegParams.DefRx2Freq = MAC_RX2_WINDOW_FREQ_NA;
	RegParams.FeaturesSupport = FEATURES_SUPPORTED_NA;
	RegParams.minDataRate = MAC_DATARATE_MIN_NA;
	RegParams.maxDataRate = MAC_DATARATE_MAX_NA;
	RegParams.cmnParams.paramsType1.Max_125khzChan = MAX_CHANNELS_BANDWIDTH_125_AU_NA;
	RegParams.cmnParams.paramsType1.Max_500khzChan = MAX_CHANNELS_BANDWIDTH_500_AU_NA;
	RegParams.cmnParams.paramsType1.minTxDR = DR0;
	RegParams.cmnParams.paramsType1.maxTxDR = DR4;
	RegParams.cmnParams.paramsType1.minRxDR = DR8;
	RegParams.cmnParams.paramsType1.maxRxDR = DR13;
	RegParams.cmnParams.paramsType1.RxParamWindowOffset1 = 10;
	RegParams.pJoinDutyCycleTimer = &RegParams.joinDutyCycleTimer;
	RegParams.pJoinBackoffTimer = &RegParams.joinBackoffTimer;
	RegParams.cmnParams.paramsType1.UpStreamCh0Freq = UPSTREAM_CH0_NA;
	RegParams.cmnParams.paramsType1.UpStreamCh64Freq = UPSTREAM_CH64_NA;
	RegParams.cmnParams.paramsType1.DownStreamCh0Freq = DOWNSTREAM_CH0_NA;
	RegParams.Rx1DrOffset = 3;
	RegParams.maxTxPwrIndx = 10;
	RegParams.defTxPwrIndx = MAC_DEF_TX_POWER_NA;
	RegParams.cmnParams.paramsType1.lastUsedSB = 0;

	RegParams.pJoinBackoffTimer->timerId = regTimerId[0];
	RegParams.pJoinDutyCycleTimer->timerId = regTimerId[1];
	RegParams.pJoinDutyCycleTimer->remainingtime =0;
	RegParams.joinbccount =0;
	RegParams.joinDutyCycleTimeout =0;
	RegParams.band = ismBand;

    InitDefault915Channels ();
	memcpy (RegParams.pDrParams, DefaultDrParamsNA, sizeof(DefaultDrParamsNA) );
	RegParams.cmnParams.paramsType1.alternativeChannel = 0;

#if (ENABLE_PDS == 1)

	/*Fill PDS item id in RegParam Structure */
	RegParams.regParamItems.fileid = PDS_FILE_REG_NA_03_IDX;
	RegParams.regParamItems.ch_param_1_item_id = PDS_REG_NA_CH_PARAM;
	RegParams.regParamItems.ch_param_2_item_id = 0;
	RegParams.regParamItems.band_item_id = 0;
	RegParams.regParamItems.lastUsedSB = PDS_REG_NA_LAST_USED_SB;
	PdsFileMarks_t filemarks;
	/* File ID NA - Register */
	filemarks.fileMarkListAddr = aRegNaPdsOps;
	filemarks.numItems =  (uint8_t)(PDS_REG_NA_MAX_VALUE & 0x00FF);
	filemarks.itemListAddr = (ItemMap_t *)&pds_reg_na_item_list;
	filemarks.fIDcb = LorawanReg_NA_Pds_Cb;
	PDS_RegFile(PDS_FILE_REG_NA_03_IDX,filemarks);
#endif
	
    LORAREG_InitGetAttrFnPtrsNA();
	LORAREG_InitValidateAttrFnPtrsNA();
	LORAREG_InitSetAttrFnPtrsNA();

#else
    result = UNSUPPORTED_BAND;
#endif
    return result;
}


/*
 * \brief Initalized all NA 915 Supported Channels
 */
#if(NA_BAND == 1)
static void InitDefault915Channels (void)
{
	memset (RegParams.pChParams, 0, sizeof(DefaultChannels915) );
	memcpy (RegParams.pChParams, DefaultChannels915, sizeof(DefaultChannels915) );
}

#if (ENABLE_PDS == 1)
void LorawanReg_NA_Pds_Cb(void)
{
	
}
#endif
#endif
