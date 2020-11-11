/**
* \file  lorawan_au.c
*
* \brief LoRaWAN au file
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

#if(AU_BAND == 1)
/*Init Functions's*/
static void InitDefault915ChannelsAU (void);


/*****************************CONSTANTS ***************************************/
static const ChannelParams_t DefaultChannels915AU[MAX_CHANNELS_AU_NA] =   {  LC0_915_AU,  LC1_915_AU,  LC2_915_AU,  LC3_915_AU,  LC4_915_AU,  LC5_915_AU,
	                                                                     LC6_915_AU,  LC7_915_AU,  LC8_915_AU,  LC9_915_AU,  LC10_915_AU, LC11_915_AU,
	                                                                     LC12_915_AU, LC13_915_AU, LC14_915_AU, LC15_915_AU, LC16_915_AU, LC17_915_AU,
	                                                                     LC18_915_AU, LC19_915_AU, LC20_915_AU, LC21_915_AU, LC22_915_AU, LC23_915_AU,
	                                                                     LC24_915_AU, LC25_915_AU, LC26_915_AU, LC27_915_AU, LC28_915_AU, LC29_915_AU,
	                                                                     LC30_915_AU, LC31_915_AU, LC32_915_AU, LC33_915_AU, LC34_915_AU, LC35_915_AU,
	                                                                     LC36_915_AU, LC37_915_AU, LC38_915_AU, LC39_915_AU, LC40_915_AU, LC41_915_AU,
	                                                                     LC42_915_AU, LC43_915_AU, LC44_915_AU, LC45_915_AU, LC46_915_AU, LC47_915_AU,
	                                                                     LC48_915_AU, LC49_915_AU, LC50_915_AU, LC51_915_AU, LC52_915_AU, LC53_915_AU,
	                                                                     LC54_915_AU, LC55_915_AU, LC56_915_AU, LC57_915_AU, LC58_915_AU, LC59_915_AU,
	                                                                     LC60_915_AU, LC61_915_AU, LC62_915_AU, LC63_915_AU, LC64_915_AU, LC65_915_AU,
                                                                         LC66_915_AU, LC67_915_AU, LC68_915_AU, LC69_915_AU, LC70_915_AU, LC71_915_AU };

static const DRParams_t DefaultDrParamsAU[] = {
	DR0_PARAMS_AU,
	DR1_PARAMS_AU,
	DR2_PARAMS_AU,
	DR3_PARAMS_AU,
	DR4_PARAMS_AU,
	DR5_PARAMS_AU,
	DR6_PARAMS_AU,
	DR7_PARAMS_AU,
	DR8_PARAMS_AU,
	DR9_PARAMS_AU,
	DR10_PARAMS_AU,
	DR11_PARAMS_AU,
	DR12_PARAMS_AU,
	DR13_PARAMS_AU
};

#if (ENABLE_PDS == 1)
#define PDS_REG_AU_CH_PARAM_ADDR                    ((uint8_t *)&(RegParams.cmnParams.paramsType1.chParams[MAX_CHANNELS_T1 - MAX_CHANNELS_T1]))
#define PDS_REG_AU_LAST_USED_SB_ADDR                ((uint8_t *)&(RegParams.cmnParams.paramsType1.lastUsedSB))

#define PDS_REG_AU_CH_PARAM_SIZE                    sizeof(RegParams.cmnParams.paramsType1.chParams)
#define PDS_REG_AU_LAST_USED_SB_SIZE                sizeof(RegParams.cmnParams.paramsType1.lastUsedSB)

#define PDS_REG_AU_CH_PARAM_OFFSET                  (PDS_FILE_START_OFFSET)
#define PDS_REG_AU_LAST_USED_SB_OFFSET              (PDS_REG_AU_CH_PARAM_OFFSET + PDS_SIZE_OF_ITEM_HDR + PDS_REG_AU_CH_PARAM_SIZE)
/* PDS Reg Params AU Item declaration */

const ItemMap_t pds_reg_au_item_list[] = {
	DECLARE_ITEM(PDS_REG_AU_CH_PARAM_ADDR,
	PDS_FILE_REG_AU_09_IDX,
	(uint8_t)PDS_REG_AU_CH_PARAM,
	PDS_REG_AU_CH_PARAM_SIZE,
	PDS_REG_AU_CH_PARAM_OFFSET),
	DECLARE_ITEM(PDS_REG_AU_LAST_USED_SB_ADDR,
	PDS_FILE_REG_AU_09_IDX,
	(uint8_t)PDS_REG_AU_LAST_USED_SB,
	PDS_REG_AU_LAST_USED_SB_SIZE,
	PDS_REG_AU_LAST_USED_SB_OFFSET),
};

PdsOperations_t aRegAuPdsOps[PDS_REG_AU_MAX_VALUE];

/* PDS Callback function */
void LorawanReg_AU_Pds_Cb(void);

#endif
#endif


/****************************** FUNCTIONS *************************************/

StackRetStatus_t LORAReg_InitAU(IsmBand_t ismBand)
{
    StackRetStatus_t result = LORAWAN_SUCCESS;
#if(AU_BAND == 1)
    RegParams.TxCurDataRate = MAC_DEF_TX_CURRENT_DATARATE_AU;
	RegParams.maxChannels = MAX_CHANNELS_AU_NA;
	RegParams.MacTxPower = MAC_DEF_TX_POWER_AU;
	RegParams.pChParams = &RegParams.cmnParams.paramsType1.chParams[0];
	RegParams.pDrParams = &RegParams.cmnParams.paramsType1.DRParams[0];
	RegParams.MinNewChIndex = 0xFF;
	RegParams.DefRx1DataRate = MAC_RX1_WINDOW_DATARATE_AU;
	RegParams.DefRx2DataRate = MAC_RX2_WINDOW_DATARATE_AU;
	RegParams.DefRx2Freq = MAC_RX2_WINDOW_FREQ_AU;
	RegParams.FeaturesSupport = FEATURES_SUPPORTED_AU;
	RegParams.minDataRate = MAC_DATARATE_MIN_AU;
	RegParams.maxDataRate = MAC_DATARATE_MAX_AU;
	RegParams.cmnParams.paramsType1.Max_125khzChan = MAX_CHANNELS_BANDWIDTH_125_AU_NA;
	RegParams.cmnParams.paramsType1.Max_500khzChan = MAX_CHANNELS_BANDWIDTH_500_AU_NA;
	RegParams.cmnParams.paramsType1.minTxDR = DR0;
	RegParams.cmnParams.paramsType1.maxTxDR = DR6;
	RegParams.cmnParams.paramsType1.minRxDR = DR8;
	RegParams.cmnParams.paramsType1.maxRxDR = DR13;
	RegParams.cmnParams.paramsType1.RxParamWindowOffset1 = 8;
	RegParams.cmnParams.paramsType1.UpStreamCh0Freq = UPSTREAM_CH0_AU;
	RegParams.cmnParams.paramsType1.UpStreamCh64Freq = UPSTREAM_CH64_AU;
	RegParams.cmnParams.paramsType1.DownStreamCh0Freq = DOWNSTREAM_CH0_AU;
    RegParams.pJoinDutyCycleTimer = &RegParams.joinDutyCycleTimer;
	RegParams.pJoinBackoffTimer = &RegParams.joinBackoffTimer;
    RegParams.Rx1DrOffset = 5;
	RegParams.maxTxPwrIndx = 10;
	RegParams.defTxPwrIndx = MAC_DEF_TX_POWER_AU;
	RegParams.maxTxPwr = DEFAULT_EIRP_AU;
	RegParams.cmnParams.paramsType1.lastUsedSB = 0;

	RegParams.pJoinBackoffTimer->timerId = regTimerId[0];	
	RegParams.pJoinDutyCycleTimer->timerId = regTimerId[1];
	RegParams.pJoinDutyCycleTimer->remainingtime = 0;
	RegParams.joinbccount =0;
	RegParams.joinDutyCycleTimeout =0;
	
	RegParams.band = ismBand;
	
    InitDefault915ChannelsAU ();
	memcpy (RegParams.pDrParams, DefaultDrParamsAU, sizeof(DefaultDrParamsAU) );
	RegParams.cmnParams.paramsType1.alternativeChannel = 0;
#if (ENABLE_PDS == 1)

	/*Fill PDS item id in RegParam Structure */
	RegParams.regParamItems.fileid = PDS_FILE_REG_AU_09_IDX;
	RegParams.regParamItems.ch_param_1_item_id = PDS_REG_AU_CH_PARAM;
	RegParams.regParamItems.ch_param_2_item_id = 0;
	RegParams.regParamItems.band_item_id = 0;
	RegParams.regParamItems.lastUsedSB = PDS_REG_AU_LAST_USED_SB;
	PdsFileMarks_t filemarks;
	/* File ID AU - Register */
	filemarks.fileMarkListAddr = aRegAuPdsOps;
	filemarks.numItems =  (uint8_t)(PDS_REG_AU_MAX_VALUE & 0x00FF);
	filemarks.itemListAddr = (ItemMap_t *)&pds_reg_au_item_list;
	filemarks.fIDcb = LorawanReg_AU_Pds_Cb;
	PDS_RegFile(PDS_FILE_REG_AU_09_IDX,filemarks);
#endif	
    LORAREG_InitGetAttrFnPtrsAU();
	LORAREG_InitValidateAttrFnPtrsAU();
	LORAREG_InitSetAttrFnPtrsAU();
#else
    result = UNSUPPORTED_BAND;
#endif
	return result;
}


/*
 * \brief Initalized all NA 915 Supported Channels
 */
#if(AU_BAND == 1)
static void InitDefault915ChannelsAU (void)
{
	memset (RegParams.pChParams, 0, sizeof(DefaultChannels915AU) );
	memcpy (RegParams.pChParams, DefaultChannels915AU, sizeof(DefaultChannels915AU) );
}
#if (ENABLE_PDS == 1)
void LorawanReg_AU_Pds_Cb(void)
{
	
}
#endif
#endif
