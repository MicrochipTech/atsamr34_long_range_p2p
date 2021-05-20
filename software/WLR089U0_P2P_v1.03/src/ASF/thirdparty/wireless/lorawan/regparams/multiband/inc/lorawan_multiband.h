/**
* \file  lorawan_multiband.h
*
* \brief LORAWAN Regional multiband common inlcude file
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


#ifndef COMMON_MULTIBAND_H_INCLUDED
#define COMMON_MULTIBAND_H_INCLUDED

#include "lorawan.h"
#include "radio_interface.h"

#include "lorawan_reg_params.h"
#include "conf_regparams.h"

#if (ENABLE_PDS == 1)
#include "pds_interface.h"
#endif

/* Macros to identify which of the parameters are defined in a particular band */
#define FREQUENCY_DEFINED						0x01
#define DATA_RANGE_DEFINED						0x02
#define DUTY_CYCLE_DEFINED						0x04

#define MAX_DR_COUNT							14
#define MAX_TX_PWR_CNT							11

#define ALL_CHANNELS							1
#define WITHOUT_DEFAULT_CHANNELS				0

#if ((NA_BAND == 1) || (AU_BAND == 1))
/* This MACRO defines number of channels in a sub-band */
#define NO_OF_CH_IN_SUBBAND                     8
/* This macro defines number of total sub-bands in NA and AU band */
#define MAX_SUBBANDS							8
#endif

#if (JPN_BAND == 1)
/*
* JPN923 falls under AS923 region, so it has duty cycle support.
* Also, JPN923 has LBT support. Since, both feature require a timer
* for each, it needs 2 timers.
* Specifically...
* regTimerId[0] --> LBT timer
* regTimerId[1] --> Dutycycle Timer
* regTimerId[2] --> Join backoff timer
* regTimerId[3] --> Join dutycycle timer
* dutycycle timer might get added
*/
#define REG_PARAMS_TIMERS_COUNT                 (4u)
#else
/*
* Bands (KR, AS, EU) other than JPN923 uses 1 timer from regional params.
* Specifically...
* In KR920, regTimerId[0] --> LBT timer
* In EU and AS, regTimerId[0] --> duty cycle timer
* regTimerId[1] -->Join backoff timer
* regTimerId[2] -->Join dutycycle timer
*/
#define REG_PARAMS_TIMERS_COUNT                 (3u)
#endif

/**************************Band wise macros ******************************************/
#if (AS_BAND == 1)

//maximum number of channels
#define MAX_CHANNELS_AS             (16) // 16 channels numbered from 0 to 15
#define MAX_NUM_SUBBANDS_AS         (1)  // currently all ASIA 923 spectrum is considered as ONE band

//AS923 default channels
#define LC0_923									{ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) }}
#define LC1_923									{ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) }}

#define ADV_LC0_923              {923200000,923200000, 0, 1, 16, 0xFF}
#define ADV_LC1_923              {923400000,923400000, 0, 1, 16, 0xFF}
	
//                                min_freq,  max_freq,  timeout
#define SB0_923                  {902000000, 928000000, 0/*, 100*/} // <1%
#define SB0_923_DC_0             (100) // <1%

/*AS Default Parameters Based on DataRate*/
/*rxWindowSize , maxPayloadSixeDT0,maxPayloadSixeDT1,rxWindowOffset,spreadingFactor,bandwidth,modulation*/
#define DR0_PARAMS_AS                 {8,    51,     0,   -33   , 12,    BW_125KHZ,         MODULATION_LORA}
#define DR1_PARAMS_AS                 {10,   51,     0,   -50   , 11,    BW_125KHZ,        MODULATION_LORA}
#define DR2_PARAMS_AS                 {14,   51,    11,   -58  , 10,        BW_125KHZ,        MODULATION_LORA}
#define DR3_PARAMS_AS				  {26,   115,   53,  -62   , 9,        BW_125KHZ,        MODULATION_LORA}
#define DR4_PARAMS_AS                 {49,   242,   125,  -66  , 8,        BW_125KHZ,        MODULATION_LORA}
#define DR5_PARAMS_AS                 {83,   242,   242,  -68  , 7,        BW_125KHZ,        MODULATION_LORA}
#define DR6_PARAMS_AS                 {60,   242,   242,  -15  , 7,        BW_250KHZ,        MODULATION_LORA}
#define DR7_PARAMS_AS                 {100,    242,   242,   -50  , 0,        BW_UNDEFINED,    MODULATION_FSK}

/*Default EIRP set by the AS923 Specification*/
#define DEFAULT_EIRP_AS                        16 //dBm
/*Default RX1 WIndow Data rate.Note this value will be changed based on the selected DR for TX*/
#define MAC_AS923_RX1_WINDOW_DATARATE        (DR2)

/*Default DR for RX2*/
#define MAC_AS923_RX2_WINDOW_DATARATE       (DR2)

/*Default Frequency for RX2*/
#define MAC_AS923_RX2_WINDOW_FREQ           (FREQ_923200KHZ)

/*Default RX1 WIndow Data rate.Note this value will be changed based on the selected DR for TX*/
#define MAC_RX1_WINDOW_DATARATE_AS                 MAC_AS923_RX1_WINDOW_DATARATE

/*Default DR for RX2*/
#define MAC_RX2_WINDOW_DATARATE_AS              MAC_AS923_RX2_WINDOW_DATARATE

/*Default Frequency for RX2*/
#define MAC_RX2_WINDOW_FREQ_AS                     MAC_AS923_RX2_WINDOW_FREQ

#define NEW_CHANNEL_INDEX_AS                    2
#define MAX_TX_PWR_INDEX_AS                        7

#endif

#if (AU_BAND == 1|| NA_BAND == 1)

#define MAX_CHANNELS_AU_NA                         72
#define MAX_CHANNELS_BANDWIDTH_125_AU_NA           64
#define MAX_CHANNELS_BANDWIDTH_500_AU_NA            8

#endif


#if (AU_BAND == 1)

#define MAC_RX1_WINDOW_DATARATE_AU                (DR8) // DC

#define MAC_RX2_WINDOW_DATARATE_AU               (DR8)
#define MAC_RX2_WINDOW_FREQ_AU                   (FREQ_923300KHZ)


#define UPSTREAM_CH0_AU                      (FREQ_915200KHZ)
#define UPSTREAM_CH64_AU                     (FREQ_915900KHZ)
#define DOWNSTREAM_CH0_AU                    (FREQ_923300KHZ)

#define DEFAULT_EIRP_AU				 30

/* AU channels for 915-928 Mhz */
#define LC0_915_AU                    { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC1_915_AU                    { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC2_915_AU                    { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC3_915_AU                    { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC4_915_AU                    { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC5_915_AU                    { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC6_915_AU                    { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC7_915_AU                    { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC8_915_AU                    { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC9_915_AU                    { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC10_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC11_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC12_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC13_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC14_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC15_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC16_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC17_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC18_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC19_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC20_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC21_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC22_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC23_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC24_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC25_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC26_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC27_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC28_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC29_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC30_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC31_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC32_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC33_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC34_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC35_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC36_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC37_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC38_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC39_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC40_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC41_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC42_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC43_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC44_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC45_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC46_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC47_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC48_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC49_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC50_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC51_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC52_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC53_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC54_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC55_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC56_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC57_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC58_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC59_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC60_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC61_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC62_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC63_915_AU                   { ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) } }
#define LC64_915_AU                   { ENABLED,  { ( ( DR6 << SHIFT4 ) | DR6 ) } }
#define LC65_915_AU                   { ENABLED,  { ( ( DR6 << SHIFT4 ) | DR6 ) } }
#define LC66_915_AU                   { ENABLED,  { ( ( DR6 << SHIFT4 ) | DR6 ) } }
#define LC67_915_AU                   { ENABLED,  { ( ( DR6 << SHIFT4 ) | DR6 ) } }
#define LC68_915_AU                   { ENABLED,  { ( ( DR6 << SHIFT4 ) | DR6 ) } }
#define LC69_915_AU                   { ENABLED,  { ( ( DR6 << SHIFT4 ) | DR6 ) } }
#define LC70_915_AU                   { ENABLED,  { ( ( DR6 << SHIFT4 ) | DR6 ) } }
#define LC71_915_AU                   { ENABLED,  { ( ( DR6 << SHIFT4 ) | DR6 ) } }


/*AU Default Parameters Based on DataRate*/
/*rxWindowSize , maxPayloadSize0,maxPayloadSize1,rxWindowOffset,spreadingFactor,bandwidth,modulation*/
#define DR0_PARAMS_AU                 {8,      51,      0,        -33,     12,     BW_125KHZ,        MODULATION_LORA}
#define DR1_PARAMS_AU                 {11,      51,      0,        -50,      11,     BW_125KHZ,        MODULATION_LORA}
#define DR2_PARAMS_AU                 {18,      51,     0,        -58,      10,     BW_125KHZ,        MODULATION_LORA}
#define DR3_PARAMS_AU               {28,      115,     0,        -62,         9,      BW_125KHZ,        MODULATION_LORA}
#define DR4_PARAMS_AU               {61,      242,     0,        -66,         8,      BW_125KHZ,        MODULATION_LORA}
#define DR5_PARAMS_AU               {83      ,242,     0,         -68,      7,      BW_125KHZ,        MODULATION_LORA}
#define DR6_PARAMS_AU               {61      ,242,     0,         -66,      8,      BW_500KHZ,        MODULATION_LORA}
#define DR7_PARAMS_AU               {0,      0,       0,        0,      0,      BW_UNDEFINED,     MODULATION_LORA}
#define DR8_PARAMS_AU               {7,      53,      0,        12,     12,     BW_500KHZ,         MODULATION_LORA}
#define DR9_PARAMS_AU               {5,      129,     0,        6,      11,     BW_500KHZ,         MODULATION_LORA}
#define DR10_PARAMS_AU              {7,      242,     0,        1,      10,     BW_500KHZ,         MODULATION_LORA}
#define DR11_PARAMS_AU              {12      ,242,    0,        -2,     9,      BW_500KHZ,         MODULATION_LORA}
#define DR12_PARAMS_AU              {22      ,242,    0,        -4,     8,      BW_500KHZ,         MODULATION_LORA}
#define DR13_PARAMS_AU              {42      ,242,    0,        -4,     7,      BW_500KHZ,         MODULATION_LORA}

#endif

#if (EU_BAND == 1)
#define MAX_NUM_SUBBANDS_EU                    (6)

/*SubBands and Params Freqmin,Freqmax,Timer,DC*/
#define SB0_868                                {863000000, 865000000, 0/*, 1000*/} // 0.1%
#define SB1_868                                {865000001, 868000000, 0/*, 100*/}  // 1%
#define SB2_868                                {868000001, 868600000, 0/*, 100*/}  // 1%
#define SB3_868                                {868700000, 869200000, 0/*, 1000*/} // 0.1%
#define SB4_868                                {869400000, 869650000, 0/*, 10*/}   // 10%
#define SB5_868                                {869700000, 870000000, 0/*, 100*/}  // 1%

#define SB0_DT                        1000
#define SB1_DT                        100
#define SB2_DT                        100
#define SB3_DT                        1000
#define SB4_DT                        10
#define SB5_DT                        100

/*This is the subband which belongs to the default channels*/
#define DEFAULT_CH_BAND_EU                        1

#define DEFAULT_EIRP_EU							  16

//EU default channels for 868 Mhz
#define LC0_868                                {ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) }}
#define LC1_868                                {ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) }}
#define LC2_868                                {ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) }}

//EU default channels for 433 Mhz (the same channels are for join request)
#define LC0_433                                {ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) }}
#define LC1_433                                {ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) }}
#define LC2_433                                {ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) }}

//EU default channels for 868 Mhz
#define Adv_LC0_868                            {868100000, 868100000, DEFAULT_CH_BAND , 1, 16,0xFF}
#define Adv_LC1_868                            {868300000, 868300000, DEFAULT_CH_BAND , 1, 16,0xFF}
#define Adv_LC2_868                            {868500000, 868500000, DEFAULT_CH_BAND , 1, 16,0xFF}

//EU default channels for 433 Mhz (the same channels are for join request)
#define Adv_LC0_433                            {433175000, 433175000, DEFAULT_CH_BAND , 1, 12, 0xFF}
#define Adv_LC1_433                            {433375000, 433375000, DEFAULT_CH_BAND , 1, 12, 0xFF}
#define Adv_LC2_433                            {433575000, 433575000, DEFAULT_CH_BAND , 1, 12, 0xFF}

/*This is the subband which belongs to the default channels*/
#define DEFAULT_CH_BAND        1

/* //TODO Remove EU433 and have it as a separate file*/
//SubBands and Params Freqmin,Freqmax,Timer,DC
#define SB0_433                                {433000000, 434800000, 0/*, 100*/}

/*EU Default Parameters Based on DataRate*/
/*rxWindowSize , maxPayloadSize,rxWindowOffset,spreadingFactor,bandwidth,modulation*/
#define DR0_PARAMS_EU                            {8,51,0,-33,12,BW_125KHZ,MODULATION_LORA}
#define DR1_PARAMS_EU                            {11,51,0,-45,11,BW_125KHZ,MODULATION_LORA}
#define DR2_PARAMS_EU                            {23,51,0,-52,10,BW_125KHZ,MODULATION_LORA}
#define DR3_PARAMS_EU                            {28,115,0,-62,9,BW_125KHZ,MODULATION_LORA}
#define DR4_PARAMS_EU                            {61,242,0,-66,8,BW_125KHZ,MODULATION_LORA}
#define DR5_PARAMS_EU                            {83,242,0,-68,7,BW_125KHZ,MODULATION_LORA}
#define DR6_PARAMS_EU                            {60,242,0,-15,7,BW_250KHZ,MODULATION_LORA}
#define DR7_PARAMS_EU                            {100,242,0,-50,0,BW_UNDEFINED,MODULATION_FSK}

#define MAC_868_RX1_WINDOW_DATARATE            DR0
#define MAC_868_RX2_WINDOW_DATARATE            DR0
#define MAC_868_RX2_WINDOW_FREQ                FREQ_869525KHZ
#define MAC_433_RX1_WINDOW_DATARATE            DR5
#define MAC_433_RX2_WINDOW_DATARATE            DR0
#define MAC_433_RX2_WINDOW_FREQ                FREQ_433050KHZ
#define MAC_RX1_WINDOW_DATARATE                MAC_868_RX1_WINDOW_DATARATE
#define MAC_RX2_WINDOW_DATARATE                MAC_868_RX2_WINDOW_DATARATE
#define MAC_RX2_WINDOW_FREQ                    MAC_868_RX2_WINDOW_FREQ

#endif


#if (IND_BAND == 1)
// Number of sub-bands in the available spectrum
#define MAX_NUM_SUBBANDS_IN                    (1)

//maximum number of channels
#define MAX_CHANNELS_IN             16 // 16 channels numbered from 0 to 15


//IND default channels for 868=5 Mhz
#define LC0_865_IN                   {ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) }}
#define LC1_865_IN                   {ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) }}
#define LC2_865_IN                   {ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) }}

//IND default channels for 868=5 Mhz
#define ADV_LC0_865_IN                   {865062500, 865062500,  0, 1, DEFAULT_EIRP_IN, 0xFF}
#define ADV_LC1_865_IN                   {865402500, 865402500,  0, 1, DEFAULT_EIRP_IN, 0xFF}
#define ADV_LC2_865_IN                   {865985000, 865985000,  0, 1, DEFAULT_EIRP_IN, 0xFF}

/*THis is the minimum channel index which can be modified.Channel index below this are default channels*/
#define MIN_CHANNEL_INDEX_IN    3

/*IND Default Parameters Based on DataRate*/
/*rxWindowSize , maxPayloadSize0, maxPayloadSize1,rxWindowOffset,spreadingFactor,bandwidth,modulation*/
#define DR0_PARAMS_IN                 {8,51,0,-33,12,BW_125KHZ,MODULATION_LORA}
#define DR1_PARAMS_IN                 {10,51,0,-50,11,BW_125KHZ,MODULATION_LORA}
#define DR2_PARAMS_IN                 {14,51,0,-58,10,BW_125KHZ,MODULATION_LORA}
#define DR3_PARAMS_IN               {26,115,0,-62,9,BW_125KHZ,MODULATION_LORA}
#define DR4_PARAMS_IN               {49,242,0,-66,8,BW_125KHZ,MODULATION_LORA}
#define DR5_PARAMS_IN               {83,242,0,-68,7,BW_125KHZ,MODULATION_LORA}
#define DR6_PARAMS_IN               {0,0,0,0,0,BW_UNDEFINED,MODULATION_LORA}
#define DR7_PARAMS_IN               {8,242,0,-2,0,BW_UNDEFINED,MODULATION_FSK}

/*Default MAX EIRP value as mentioned in India Regional Spec*/
#define DEFAULT_EIRP_IN                 30

/*Default RX1 WIndow Data rate.Note this value will be changed based on the selected DR for TX*/
#define MAC_RX1_WINDOW_DATARATE_IN                 (DR0)

/*Default DR for RX2*/
#define MAC_RX2_WINDOW_DATARATE_IN               (DR2)

/*Default Frequency for RX2*/
#define MAC_RX2_WINDOW_FREQ_IN                   (FREQ_866550KHZ)

#endif


#if (JPN_BAND == 1)
// Number of sub-bands in the available spectrum
#define MAX_NUM_SUBBANDS_JP                   (2)
//maximum number of channels
#define MAX_CHANNELS_JP             16 // 16 channels numbered from 0 to 15

#define DEFAULT_CH_SUBBAND_JP       1

//JP920 default channels
#define LC0_923_JP                   {ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) }}
#define LC1_923_JP                   {ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) }}

//JP920 default channels
//										 ulFreq,    rx1Freq,   chSubBand,  IsJoinChannel, maxEIRP, parametersDefined
#define ADV_LC0_923_JP                   {923200000,923200000, DEFAULT_CH_SUBBAND_JP, 1, DEFAULT_EIRP_JP, 0xFF}
#define ADV_LC1_923_JP                   {923400000,923400000, DEFAULT_CH_SUBBAND_JP, 1, DEFAULT_EIRP_JP, 0xFF}

//JP920 subbands
//                                    min_freq,  max_freq,  timeout
#define SB0_923_JP                   {920000000, 922200000, 0}
#define SB0_923_JP_DC_0              (100) // <1%

#define SB1_923_JP                   {922400000, 928000000, 0}
#define SB1_923_JP_DC_0              (10) // <10%

/*JP Default Parameters Based on DataRate*/
/*rxWindowSize , maxPayloadSixeDT0,maxPayloadSixeDT1,rxWindowOffset,spreadingFactor,bandwidth,modulation*/
#define DR0_PARAMS_JP               {8,    51,     0,  -33   , 12,    BW_125KHZ,         MODULATION_LORA}
#define DR1_PARAMS_JP               {10,   51,     0,  -50   , 11,    BW_125KHZ,        MODULATION_LORA}
#define DR2_PARAMS_JP               {14,   51,    11,   -58  , 10,    BW_125KHZ,        MODULATION_LORA}
#define DR3_PARAMS_JP               {26,   115,   53,  -62   , 9,        BW_125KHZ,        MODULATION_LORA}
#define DR4_PARAMS_JP               {49,   242,   125,  -66  , 8,        BW_125KHZ,        MODULATION_LORA}
#define DR5_PARAMS_JP               {83,   242,   242,  -68  , 7,        BW_125KHZ,        MODULATION_LORA}
#define DR6_PARAMS_JP               {60,   242,   242,  -15  , 7,        BW_250KHZ,        MODULATION_LORA}
#define DR7_PARAMS_JP               {100,    242,   242,  -50  , 0,        BW_UNDEFINED,    MODULATION_FSK}

#define DATARATE_COUNT_JP     8

#define DEFAULT_EIRP_JP                        16 //dBm

#define MAC_RX1_WINDOW_DATARATE_JP        (DR2)

#define MAC_RX2_WINDOW_DATARATE_JP       (DR2)

#define MAC_RX2_WINDOW_FREQ_JP           (FREQ_923200KHZ)

#define MIN_CHANNEL_INDEX_JP             2

/*LBT Parameters - From JP Specification*/
#define LBT_SCAN_PERIOD_JP                             5    /*ms*/
#define LBT_SIGNAL_THRESHOLD_JP                       -80  /*dBm*/

#endif

#define LBT_TRANSMIT_CHANNEL_PAUSE_DURATION            50    /*ms*/
/*LBT is mandatory for JP*/
#define LBT_ENABLE                                    true

#if (KR_BAND == 1)
// Number of sub-bands in the available spectrum
#define MAX_NUM_SUBBANDS_KR                   (1)

//maximum number of channels
/*Note : Spec mentions 16 channels to be supported ,but only 13 channels are possible from 920.9 to 922.3*/
#define MAX_CHANNELS_KR             16 // 13 channels numbered from 0 to 12 only possilbe based on the table,
//but setting this value as 16 as per the specification

//KR default channels for 920Band
#define LC0_920_KR                   {ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) }}
#define LC1_920_KR                   {ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) }}
#define LC2_920_KR                   {ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) }}

#define ADV_LC0_920_KR                   {922100000,922100000, 0, 1, 14, 0xFF}
#define ADV_LC1_920_KR                   {922300000,922300000, 0, 1, 14, 0xFF}
#define ADV_LC2_920_KR                   {922500000,922500000, 0, 1, 14, 0xFF}

//KR Default Parameters Based on DataRate
//rxWindowSize,maxPayloadSize0,maxPayloadSize1,rxWindowOffset,spreadingFactor
/*Note Values of rxWindowSize and rxOffset to be revisited*/

#define DR0_PARAMS_KR         {8,     51,  0,    -33,      12,BW_125KHZ,MODULATION_LORA}
#define DR1_PARAMS_KR         {10,    51, 0, -50,     11,BW_125KHZ,MODULATION_LORA}
#define DR2_PARAMS_KR         {14,    51, 0,    -58,     10,BW_125KHZ,MODULATION_LORA}
#define DR3_PARAMS_KR       {26,    115, 0,    -62,     9,BW_125KHZ,MODULATION_LORA}
#define DR4_PARAMS_KR       {49,    242, 0,    -66,     8,BW_125KHZ,MODULATION_LORA}
#define DR5_PARAMS_KR       {83,    242, 0,    -68,     7,BW_125KHZ,MODULATION_LORA}

#define MAC_RX1_WINDOW_DATARATE_KR                (DR0)

#define MAC_RX2_WINDOW_DATARATE_KR               (DR0)

#define MAC_RX2_WINDOW_FREQ_KR                   (FREQ_921900KHZ)

#define MIN_CHANNEL_INDEX_KR                     3

// FREQ < 922.1
#define DEFAULT_EIRP_KR_LF    10

//FREQ >=922.1
#define DEFAULT_EIRP_KR_HF    14

/*LBT Parameters - From KR Specification*/
#define LBT_SCAN_PERIOD_KR                                10    /*ms*/
#define LBT_SIGNAL_THRESHOLD_KR                       -65  /*dBm*/

#endif


#if (NA_BAND == 1)

#define MAC_RX1_WINDOW_DATARATE_NA                DR10
#define MAC_RX2_WINDOW_DATARATE_NA                DR8
#define MAC_RX2_WINDOW_FREQ_NA                    FREQ_923300KHZ

/* US channels for 915 Mhz */
#define LC0_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC1_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC2_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC3_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC4_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC5_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC6_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC7_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC8_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC9_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC10_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC11_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC12_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC13_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC14_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC15_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC16_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC17_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC18_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC19_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC20_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC21_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC22_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC23_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC24_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC25_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC26_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC27_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC28_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC29_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC30_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC31_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC32_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC33_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC34_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC35_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC36_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC37_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC38_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC39_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC40_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC41_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC42_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC43_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC44_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC45_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC46_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC47_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC48_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC49_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC50_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC51_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC52_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC53_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC54_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC55_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC56_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC57_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC58_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC59_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC60_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC61_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC62_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC63_915_NA                                { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC64_915_NA                                { ENABLED,  { ( ( DR4 << SHIFT4 ) | DR4 ) } }
#define LC65_915_NA                                { ENABLED,  { ( ( DR4 << SHIFT4 ) | DR4 ) } }
#define LC66_915_NA                                { ENABLED,  { ( ( DR4 << SHIFT4 ) | DR4 ) } }
#define LC67_915_NA                                { ENABLED,  { ( ( DR4 << SHIFT4 ) | DR4 ) } }
#define LC68_915_NA                                { ENABLED,  { ( ( DR4 << SHIFT4 ) | DR4 ) } }
#define LC69_915_NA                                { ENABLED,  { ( ( DR4 << SHIFT4 ) | DR4 ) } }
#define LC70_915_NA                                { ENABLED,  { ( ( DR4 << SHIFT4 ) | DR4 ) } }
#define LC71_915_NA                                { ENABLED,  { ( ( DR4 << SHIFT4 ) | DR4 ) } }


/*NA Default Parameters Based on DataRate*/
/*rxWindowSize , maxPayloadSize0, maxPayloadSize1,rxWindowOffset,spreadingFactor,bandwidth,modulation*/
#define DR0_PARAMS_NA                             {18,11,0,-58,10,BW_125KHZ,MODULATION_LORA}
#define DR1_PARAMS_NA                             {5,53,0,6,9,BW_125KHZ,MODULATION_LORA}
#define DR2_PARAMS_NA                             {7,125,0,1,8,BW_125KHZ,MODULATION_LORA}
#define DR3_PARAMS_NA							{12,242,0,-2,7,BW_125KHZ,MODULATION_LORA}
#define DR4_PARAMS_NA                            {22,242,0,-4,8,BW_500KHZ,MODULATION_LORA}
#define DR5_PARAMS_NA                            {0,0,0,0,0,BW_UNDEFINED,MODULATION_LORA}
#define DR6_PARAMS_NA                            {0,0,0,0,0,BW_UNDEFINED,MODULATION_LORA}
#define DR7_PARAMS_NA                            {0,0,0,0,0,BW_UNDEFINED,MODULATION_LORA}
#define DR8_PARAMS_NA                            {35,53,0,-43,12,BW_500KHZ,MODULATION_LORA}
#define DR9_PARAMS_NA                            {7,129,0,6,11,BW_500KHZ,MODULATION_LORA}
#define DR10_PARAMS_NA                            {7,242,0,1,10,BW_500KHZ,MODULATION_LORA}
#define DR11_PARAMS_NA                            {12,242,0,-2,9,BW_500KHZ,MODULATION_LORA}
#define DR12_PARAMS_NA                            {22,242,0,-4,8,BW_500KHZ,MODULATION_LORA}
#define DR13_PARAMS_NA                            {42,242,0,-4,7,BW_500KHZ,MODULATION_LORA}

#define DEFAULT_EIRP_NA                           30

#define UPSTREAM_CH0_NA                            FREQ_902300KHZ
#define UPSTREAM_CH64_NA                           FREQ_903000KHZ
#define DOWNSTREAM_CH0_NA                          FREQ_923300KHZ

#endif


typedef StackRetStatus_t (*pLoraRegGetAttr_t)(LorawanRegionalAttributes_t attr, void *attrInput, void *attrOutput);
typedef StackRetStatus_t (*pLoraRegValidateAttr_t)(LorawanRegionalAttributes_t attr, void *attrInput);
typedef StackRetStatus_t (*pLoraRegSetAttr_t)(LorawanRegionalAttributes_t attr, void *attrInput);
typedef void (*pUpdateChIdStatus_t)(uint8_t chid, bool statusNew);

/************** PDS Parameters *********************/
#if (ENABLE_PDS == 1)
/************ NA915 **********************/
/* NA Reg Params Items Start Index */
#define REG_NA_PDS_FID1_START_INDEX    PDS_FILE_REG_NA_03_IDX << 8


/* PDS Reg NA Items - List*/
typedef enum _pds_reg_na_items
{
    PDS_REG_NA_CH_PARAM = REG_NA_PDS_FID1_START_INDEX,
	PDS_REG_NA_LAST_USED_SB,
    PDS_REG_NA_MAX_VALUE  /* Always add new items above this value */
}pds_reg_na_items_t;

/************ EU868 **********************/

/* EU868 Reg Params Items Start Index */
#define REG_EU868_PDS_FID1_START_INDEX    PDS_FILE_REG_EU868_04_IDX << 8
#define REG_EU868_PDS_FID2_START_INDEX    PDS_FILE_REG_EU868_12_IDX << 8

#define PDS_REG_EU868_FID1_MAX_VALUE 1
/* PDS Reg EU868 Items - List*/
typedef enum _pds_reg_fid1_eu868_items
{
    PDS_REG_EU868_CH_PARAM_1 = REG_EU868_PDS_FID1_START_INDEX /* Channel Param structure for all 16 channels */
      /* Always add new items above this value */
}pds_reg_eu868_fid1_items_t;

#define PDS_REG_EU868_FID2_MAX_VALUE 1
typedef enum _pds_reg_eu868_fid2_items
{
    PDS_REG_EU868_CH_PARAM_2 = REG_EU868_PDS_FID2_START_INDEX /* Channel Param structure for all 16 channels */
    /* Always add new items above this value */
}pds_reg_eu868_fid2_items_t;

/************ AU915 **********************/

/* AU Reg Params Items Start Index */
#define REG_AU_PDS_FID1_START_INDEX    PDS_FILE_REG_AU_09_IDX << 8

#define PDS_REG_AU_MAX_VALUE 2

/* PDS Reg AU Items - List*/
typedef enum _pds_reg_au_items
{
    PDS_REG_AU_CH_PARAM = REG_AU_PDS_FID1_START_INDEX,
	PDS_REG_AU_LAST_USED_SB
      /* Always add new items above this value */
}pds_reg_au_items_t;

/************ AS923 **********************/

/* AS923 Reg Params Items Start Index */
#define REG_AS_PDS_START_INDEX    PDS_FILE_REG_AS_05_IDX << 8

#define PDS_REG_AS_MAX_VALUE 3

/* PDS Reg AS923 Items - List*/
typedef enum _pds_reg_as_items
{
    PDS_REG_AS_CH_PARAM_1= REG_AS_PDS_START_INDEX, /* Channel Param structure for all 16 channels */
    PDS_REG_AS_CH_PARAM_2,
    PDS_REG_AS_BAND                               /* Band selected while initialized */
      /* Always add new items above this value */
}pds_reg_as_items_t;


/************ JPN923 **********************/

/* JPN Reg Params Items Start Index */
#define REG_JPN_PDS_FID1_START_INDEX    PDS_FILE_REG_JPN_08_IDX << 8

#define PDS_REG_JPN_FID1_MAX_VALUE 2

/* PDS Reg JPN Items - List*/
typedef enum _pds_reg_jpn_fid1_items
{
    PDS_REG_JPN_CH_PARAM_1 = REG_JPN_PDS_FID1_START_INDEX, /* Channel Param structure for all 16 channels */
    PDS_REG_JPN_CH_PARAM_2
      /* Always add new items above this value */
}pds_reg_jpn_fid1_items_t;

/************ KR923 **********************/

/* Korea Reg Params Items Start Index */
#define REG_KR_PDS_FID1_START_INDEX    PDS_FILE_REG_KR_06_IDX << 8

#define PDS_REG_KR_FID1_MAX_VALUE 2

/* PDS Reg Korea Items - List*/
typedef enum _pds_reg_kr_fid1_items
{
    PDS_REG_KR_CH_PARAM_1 = REG_KR_PDS_FID1_START_INDEX, /* Channel Param structure for all 16 channels */
    PDS_REG_KR_CH_PARAM_2
     /* Always add new items above this value */
}pds_reg_kr_fid1_items_t;

/************ IND865 **********************/

/* IND865 Reg Params Items Start Index */
#define REG_IND_PDS_START_INDEX    PDS_FILE_REG_IND_07_IDX << 8

#define PDS_REG_IND_MAX_VALUE 2

/* PDS Reg IND865 Items - List*/
typedef enum _pds_reg_ind_items
{
    PDS_REG_IND_CH_PARAM_1 = REG_IND_PDS_START_INDEX, /* Channel Param structure for all 16 channels */
    PDS_REG_IND_CH_PARAM_2
     /* Always add new items above this value */
}pds_reg_ind_items_t;
#endif
/**************************************************/


COMPILER_PACK_SET(1)

#define MAX_DRPARAMS_NA                         14
#define MAX_CHANNELS_NA                            72

#define MAX_CHANNELS_EU                            16 // 16 channels numbered from 0 to 15
#define MAX_DRPARAMS_EU                          8

#define MAX_DRPARAMS_T1                         MAX_DRPARAMS_NA
#define MAX_CHANNELS_T1                         MAX_CHANNELS_NA

#define MAX_DRPARAMS_T2                         MAX_DRPARAMS_EU
#define MAX_CHANNELS_T2                         MAX_CHANNELS_EU

// Find out the maximum number of sub bands among multiple bands
#define MAX_NUM_SUBBANDS                        (0)
#ifdef MAX_NUM_SUBBANDS_EU
#if (MAX_NUM_SUBBANDS_EU > MAX_NUM_SUBBANDS)
#undef MAX_NUM_SUBBANDS
#define MAX_NUM_SUBBANDS	(MAX_NUM_SUBBANDS_EU)
#endif
#endif
#ifdef MAX_NUM_SUBBANDS_AS
#if (MAX_NUM_SUBBANDS_AS > MAX_NUM_SUBBANDS)
#undef MAX_NUM_SUBBANDS
#define MAX_NUM_SUBBANDS	(MAX_NUM_SUBBANDS_AS)
#endif
#endif
#ifdef MAX_NUM_SUBBANDS_IN
#if (MAX_NUM_SUBBANDS_IN > MAX_NUM_SUBBANDS)
#undef MAX_NUM_SUBBANDS
#define MAX_NUM_SUBBANDS	(MAX_NUM_SUBBANDS_IN)
#endif
#endif
#ifdef MAX_NUM_SUBBANDS_JP
#if (MAX_NUM_SUBBANDS_JP > MAX_NUM_SUBBANDS)
#undef MAX_NUM_SUBBANDS
#define MAX_NUM_SUBBANDS	(MAX_NUM_SUBBANDS_JP)
#endif
#endif
#ifdef MAX_NUM_SUBBANDS_KR
#if (MAX_NUM_SUBBANDS_KR > MAX_NUM_SUBBANDS)
#undef MAX_NUM_SUBBANDS
#define MAX_NUM_SUBBANDS	(MAX_NUM_SUBBANDS_KR)
#endif
#endif

#if (ENABLE_PDS == 1)
typedef struct _RegPdsItems
{
    uint8_t  fileid;
    uint16_t lastUsedSB;
    uint16_t ch_param_1_item_id;
    uint16_t ch_param_2_item_id;
    uint16_t band_item_id;
}RegPdsItems_t;
#endif
/*This Structure stores dutycycle timer related information*/
typedef struct _DutyCycleTimer
{
    /* Duty cycle timers last value */
    uint32_t lastTimerValue;
    /* The unique Id given to the timer */
    uint8_t timerId;
}    DutyCycleTimer_t;
/*This Structure stores Joinreq dutycycle timer related information*/
typedef struct _JoinDutyCycleTimer
{
	/* Duty cycle timers last value */
	uint32_t lastTimerInterval;
	/* Remaining time if dutycycle time is greater than swtimer max */
	uint32_t remainingtime;
	/* The unique Id given to the timer */
	uint8_t timerId;
	
}    JoinDutyCycleTimer_t;
/*This Structure stores Joinreq backoff timer related information*/
typedef struct _JoinBackoffTimer
{
	/* The unique Id given to the timer */
	uint8_t timerId;
}    JoinBackoffTimer_t;

/*This Structure stores LBT timer related information*/
typedef struct _LBTTimer
{
    uint32_t lastTimerValue;
    uint8_t timerId;
}    LBTTimer_t;

/* Stores Sub-Band specific Parameters */
typedef struct _SubBandParams
{
    /*Start of Frequency Range of the Subband*/
    uint32_t freqMin;
    /*End of Frequency Range of the Subband*/
    uint32_t freqMax;
    /*Time left for the subband to be available for next transmission*/
    uint32_t subBandTimeout;
}SubBandParams_t;

typedef struct _channelParams
{
    /* Indicates if the channel is enabled or disabled */
    bool status;
    /* Indicates the min and max dataRate supported by the channel */
    DataRange_t dataRange ;
}ChannelParams_t;

/* Stores Channel related parameters*/
typedef struct _OthChannelParams
{
    /* The uplink frequency in Hz configured for this channel */
    uint32_t ulfrequency;
    /* The receive frequency in Hz corresponding to RX1 window the device must listen to when the channel was used for the last uplink */
    uint32_t rx1Frequency;
    /* Indicates the subBand id to which the channel belongs to */
    uint8_t subBandId;
    /* Indicates if the corresponding channel can be used for joining */
    bool joinRequestChannel;
    uint8_t maxEIRP;
    /* Indicates if Frequency and DataRange of the corresponding channel is defined or not
     * meaning if they are added to the database or absent.
    Used as an identifier to verify the parameters defined for a specific channel
       like frequency, dataRange -> FREQUENCY_DEFINED, DATA_RANGE_DEFINED
    */
    uint8_t parametersDefined;
} OthChannelParams_t;

/** Regional parameters which are controlled by Data Rate */
typedef struct _DRParams
{
    uint16_t rxWindowSize;
    /* Maximum payload size when downlink Dwell time is zero - Dwell time 0 means no restriction on dwell time */
    uint8_t maxPayloadSizeDt0;
    /* Maximum payload size when downlink Dwell time is zero - Dwell time 0 means 400ms  */
    uint8_t maxPayloadSizeDt1;
    int8_t rxWindowOffset;
    RadioDataRate_t spreadingFactor;
    RadioLoRaBandWidth_t bandwidth;
    RadioModulation_t modulation;
} DRParams_t;

typedef struct _RegParamsType1
{
    DRParams_t DRParams[MAX_DRPARAMS_T1];
    ChannelParams_t chParams[MAX_CHANNELS_T1];
    /* Variables used in NA and AU bands to make the functions common */
    uint32_t UpStreamCh0Freq;
    uint32_t UpStreamCh64Freq;
    uint32_t DownStreamCh0Freq;
    uint8_t minTxDR;
    uint8_t maxTxDR;
    uint8_t minRxDR;
    uint8_t maxRxDR;
    uint8_t Max_125khzChan;
    uint8_t Max_500khzChan;
    uint8_t RxParamWindowOffset1;
    uint8_t alternativeChannel;
	/* Used to store the sub-band from which the channel is used for Transmission */
	uint8_t lastUsedSB;
}RegParamsType1_t;

typedef struct _RegParamsType2
{
    DRParams_t DRParams[MAX_DRPARAMS_T2];
    ChannelParams_t chParams[MAX_CHANNELS_T2];
    OthChannelParams_t othChParams[MAX_CHANNELS_T2];
    DutyCycleTimer_t DutyCycleTimer;
	uint32_t channelTimer[MAX_CHANNELS_T2]; /* LBT Channel timer array */
    LBTTimer_t LBTTimer;
    /*DutyCycle multiplier calculated based on the regulatory defined DutyCycle*/
    uint16_t subBandDutyCycle[MAX_NUM_SUBBANDS];
    /*This filed stores the info of aggregated duty cycle*/
    uint32_t  aggregatedDutyCycleTimeout;
	uint8_t LBTScanPeriod;
    int8_t LBTSignalThreshold;
    uint8_t LBT_RSSISamplesCount;
    uint8_t minNonDefChId;
    /*Stores Tx params, maxEIRP element is not used */
    TxParams_t txParams;
    /*Information on the Sub band related Parameters are stored here*/
    SubBandParams_t SubBands[MAX_NUM_SUBBANDS];
}RegParamsType2_t;

typedef union _CmnParams
{
    RegParamsType1_t paramsType1;
    RegParamsType2_t paramsType2;
}CmnParams_t;


/* All the parameters related to multiband region are stored using this structure*/
typedef struct _RegParams
{
    DRParams_t *pDrParams;
    ChannelParams_t *pChParams;
    OthChannelParams_t *pOtherChParams;
    SubBandParams_t *pSubBandParams;
    DutyCycleTimer_t *pDutyCycleTimer;
	JoinDutyCycleTimer_t *pJoinDutyCycleTimer;
	JoinBackoffTimer_t *pJoinBackoffTimer;
    uint32_t DefRx2Freq;
    FeaturesSupported_t FeaturesSupport;
    uint8_t MinNewChIndex;
    uint8_t DefRx1DataRate;
    uint8_t DefRx2DataRate;
    uint8_t minDataRate;
    uint8_t maxDataRate;
    int8_t TxCurDataRate;
    int8_t MacTxPower;
    uint8_t maxTxPwr;
	uint8_t maxSubBands;
    int8_t maxChannels;
    int8_t maxDrParams;
    int8_t Rx1DrOffset;
    int8_t maxTxPwrIndx;
	int8_t defTxPwrIndx;
    uint8_t band;


    /*All Parameters which are related to data rate are stored here*/
    /*The supported tx pwr values are stored here*/
    //TXPower_t txPower[MAX_TX_PWR_CNT];
    /*The last channel which was used for transmission is used here*/
    uint8_t lastUsedChannelIndex;
    uint32_t aggregatedDutyCycleTimeout;
	JoinDutyCycleTimer_t joinDutyCycleTimer;
	JoinBackoffTimer_t joinBackoffTimer;
	/* Join request dutycycle timeout*/
	uint32_t  joinDutyCycleTimeout;
	uint8_t joinbccount;
    CmnParams_t cmnParams;
#if (ENABLE_PDS == 1)
    RegPdsItems_t regParamItems;
#endif

} RegParams_t;
COMPILER_PACK_RESET()

StackRetStatus_t LORAReg_InitEU(IsmBand_t ismBand);
StackRetStatus_t LORAReg_InitNA(IsmBand_t ismBand);
StackRetStatus_t LORAReg_InitAS(IsmBand_t ismBand);
StackRetStatus_t LORAReg_InitAU(IsmBand_t ismBand);
StackRetStatus_t LORAReg_InitIN(IsmBand_t ismBand);
StackRetStatus_t LORAReg_InitJP(IsmBand_t ismBand);
StackRetStatus_t LORAReg_InitKR(IsmBand_t ismBand);

StackRetStatus_t CreateAllRegSoftwareTimers (void);
void StopAllRegSoftwareTimers (void);
//void SetCallbackRegSoftwareTimers (void);

void LORAREG_InitGetAttrFnPtrsNA(void);
void LORAREG_InitGetAttrFnPtrsEU(void);
void LORAREG_InitGetAttrFnPtrsAS(void);
void LORAREG_InitGetAttrFnPtrsAU(void);
void LORAREG_InitGetAttrFnPtrsIN(void);
void LORAREG_InitGetAttrFnPtrsJP(void);
void LORAREG_InitGetAttrFnPtrsKR(void);

StackRetStatus_t InValidGetAttr(LorawanRegionalAttributes_t attr, void * attrInput, void * attrOutput);
StackRetStatus_t InValidAttr(LorawanRegionalAttributes_t attr, void * attrInput);

void LORAREG_InitValidateAttrFnPtrsNA(void);
void LORAREG_InitValidateAttrFnPtrsEU(void);
void LORAREG_InitValidateAttrFnPtrsAS(void);
void LORAREG_InitValidateAttrFnPtrsAU(void);
void LORAREG_InitValidateAttrFnPtrsIN(void);
void LORAREG_InitValidateAttrFnPtrsJP(void);
void LORAREG_InitValidateAttrFnPtrsKR(void);
void InitDefault868Channels (void);
void InitDefault433Channels (void);
void InitDefault920Channels (void);
void InitDefault923Channels (void);
void InitDefault920ChannelsKR (void);
void Enableallchannels(void);

void LORAREG_InitSetAttrFnPtrsNA(void);
void LORAREG_InitSetAttrFnPtrsEU(void);
void LORAREG_InitSetAttrFnPtrsAS(void);
void LORAREG_InitSetAttrFnPtrsAU(void);
void LORAREG_InitSetAttrFnPtrsIN(void);
void LORAREG_InitSetAttrFnPtrsJP(void);
void LORAREG_InitSetAttrFnPtrsKR(void);


void JoinBackoffCallback(uint8_t param);
extern RegParams_t RegParams;

extern uint8_t regTimerId[REG_PARAMS_TIMERS_COUNT];

#endif /* COMMON_MULTIBAND_H_INCLUDED */

/* eof common_multiband.h */
