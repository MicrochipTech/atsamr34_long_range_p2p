/**
* \file  stack_common.h
*
* \brief LORAWAN Stack common include file
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


#ifndef STACK_COMMON_H_INCLUDED
#define STACK_COMMON_H_INCLUDED


#ifdef	__cplusplus
extern "C" {
#endif


/* Stack version string */
#define PRODUCT                   "MLS_SDK"
#define MAJOR_NUM                 "1"
#define MINOR_NUM                 "0"
#define BUILD_TYPE                "P"                /* E or P */
#define BUILD_NUM                 "5"

/* Release Version Information */
#define STACK_VER    PRODUCT"_"MAJOR_NUM"_"MINOR_NUM"_"BUILD_TYPE"_"BUILD_NUM

/* Release version information in 32-bit bitfield */
/******************************************************************************
Bit field information: This must be updated manually

  /---------------------------------------------------\
  | bit pos |    field name   |        meaning        |
  |---------------------------------------------------|
  |   0-13  |   reserved      |         NA            |
  |         |                 |                       |
  |---------------------------------------------------|
  |  14-17  |     build       | running version of    |
  |         |   iteration     | this release          |
  |         |                 |                       |
  |---------------------------------------------------|
  |  18-19  |    qualifier    | 00 - reserved         |
  |         |                 | 01 - Production  (P)  |
  |         |                 | 10 - Engineering (E)  |
  |         |                 | 11 - reserved         |
  |---------------------------------------------------|
  |  20-23  |  stack minor    | minor version         |
  |         |                 |                       |
  |---------------------------------------------------|
  |  24-27  |  stack major    | major version         |
  |         |                 |                       |
  |---------------------------------------------------|
  |  28-31  |   reserved      |         NA            |
  |         |                 |                       |
  \---------------------------------------------------/

Example:
  MLS_SDK_1_1_E_3 is represented as 0x0118C000

  0000     0001    0001   10      0011   00000000000000
    |       |       |      |       |            |
 Reserved  stack  stack  qual. iteration    Reserved
           major  minor 
******************************************************************************/
#define STACK_VERSION_VALUE     (0x01054000) // MLS_SDK_1_0_P_4

/***************************** TYPEDEFS ***************************************/
typedef enum _StackRetStatus
{
    LORAWAN_RADIO_SUCCESS                          = 0,
    LORAWAN_RADIO_NO_DATA                       ,
    LORAWAN_RADIO_DATA_SIZE                     ,
    LORAWAN_RADIO_INVALID_REQ                   ,
    LORAWAN_RADIO_BUSY                          ,
    LORAWAN_RADIO_OUT_OF_RANGE                  ,
    LORAWAN_RADIO_UNSUPPORTED_ATTR              ,
    LORAWAN_RADIO_CHANNEL_BUSY                  ,            
    LORAWAN_SUCCESS                             ,
    LORAWAN_NWK_NOT_JOINED                      ,
    LORAWAN_INVALID_PARAMETER                   ,
    LORAWAN_KEYS_NOT_INITIALIZED                ,
    LORAWAN_SILENT_IMMEDIATELY_ACTIVE           ,
    LORAWAN_FCNTR_ERROR_REJOIN_NEEDED           ,
    LORAWAN_INVALID_BUFFER_LENGTH               ,
    LORAWAN_MAC_PAUSED                          ,
    LORAWAN_NO_CHANNELS_FOUND                   ,
    LORAWAN_BUSY								,
    LORAWAN_NO_ACK                              ,
    LORAWAN_NWK_JOIN_IN_PROGRESS                ,
    LORAWAN_RESOURCE_UNAVAILABLE				,
    LORAWAN_INVALID_REQUEST                     ,
	LORAWAN_UNSUPPORTED_BAND                    ,
    LORAWAN_FCNTR_ERROR                         ,
    LORAWAN_MIC_ERROR                           ,
    LORAWAN_INVALID_MTYPE                       ,    
    LORAWAN_MCAST_HDR_INVALID                   ,
	LORAWAN_TX_TIMEOUT                    		,
	LORAWAN_RADIO_TX_TIMEOUT                    ,
	LORAWAN_MAX_MCAST_GROUP_REACHED				,
	LORAWAN_INVALID_PACKET				,
	LORAWAN_RXPKT_ENCRYPTION_FAILED				,
	LORAWAN_TXPKT_ENCRYPTION_FAILED				,
	LORAWAN_SKEY_DERIVATION_FAILED				,
	LORAWAN_MIC_CALCULATION_FAILED				,
	LORAWAN_SKEY_READ_FAILED      ,
    LORAWAN_JOIN_NONCE_ERROR
} StackRetStatus_t;

/* ISM Band Types*/
typedef enum _IsmBand
{   /* EU 863 - 870MHz ISM Band */
	ISM_EU868 = 0,
	/* EU 433MHz ISM Band */
	ISM_EU433,
	/*NA*/
	ISM_NA915,
	/*AU*/
	ISM_AU915,
	/*KR*/
	ISM_KR920,
	/*Japan*/
	ISM_JPN923,
	/*AS932 BANDS*/
	/*Brunei*/
	ISM_BRN923,
	/*Cambodia*/
	ISM_CMB923,
	/*Indonesia*/
	ISM_INS923,
	/*Laos*/
	ISM_LAOS923,
	/*New Zealand*/
	ISM_NZ923,
	/*Singapore*/
	ISM_SP923,
	/*Taiwan*/
	ISM_TWN923,
	/*Thailand*/
	ISM_THAI923,
	/*Vietnam*/
	ISM_VTM923,
	/*India*/
	ISM_IND865
	
} IsmBand_t;

#define ISM_EUBAND         ((1 << ISM_EU868) | (1 << ISM_EU433))
#define ISM_NAAUBAND       ((1 << ISM_NA915) | (1 << ISM_AU915))
#define ISM_ASBAND         ((1 << ISM_BRN923) | (1 << ISM_CMB923) | (1 << ISM_INS923) | (1 << ISM_NZ923) |(1 << ISM_SP923) | (1 << ISM_TWN923) | (1 << ISM_THAI923) | (1 << ISM_VTM923) | (1 << ISM_LAOS923))

#define ENABLED                                 1
#define DISABLED                                0

#define ACCEPTED                                1
#define REJECTED                                0

#define SHIFT0                                  (0)
#define SHIFT1                                  (1)
#define SHIFT2                                  (2)
#define SHIFT3                                  (3)
#define SHIFT4                                  (4)
#define SHIFT5                                  (5)
#define SHIFT6                                  (6)
#define SHIFT7                                  (7)
#define SHIFT8                                  (8)
#define SHIFT9                                  (9)
#define SHIFT10                                 (10)
#define SHIFT11                                 (11)
#define SHIFT12                                 (12)
#define SHIFT13                                 (13)
#define SHIFT14                                 (14)
#define SHIFT15                                 (15)
#define SHIFT16                                 (16)
#define SHIFT17                                 (17)
#define SHIFT18                                 (18)
#define SHIFT19                                 (19)
#define SHIFT20                                 (20)
#define SHIFT21                                 (21)
#define SHIFT22                                 (22)
#define SHIFT23                                 (23)
#define SHIFT24                                 (24)
#define SHIFT25                                 (25)
#define SHIFT26                                 (26)
#define SHIFT27                                 (27)
#define SHIFT28                                 (28)
#define SHIFT29                                 (29)
#define SHIFT30                                 (30)
#define SHIFT31                                 (31)

#define BIT0                                    (1 << SHIFT0)
#define BIT1                                    (1 << SHIFT1)
#define BIT2                                    (1 << SHIFT2)
#define BIT3                                    (1 << SHIFT3)
#define BIT4                                    (1 << SHIFT4)
#define BIT5                                    (1 << SHIFT5)
#define BIT6                                    (1 << SHIFT6)
#define BIT7                                    (1 << SHIFT7)
#define BIT8                                    (1 << SHIFT8)
#define BIT9                                    (1 << SHIFT9)
#define BIT10                                   (1 << SHIFT10)
#define BIT11                                   (1 << SHIFT11)
#define BIT12                                   (1 << SHIFT12)
#define BIT13                                   (1 << SHIFT13)
#define BIT14                                   (1 << SHIFT14)
#define BIT15                                   (1 << SHIFT15)
#define BIT16                                   (1 << SHIFT16)
#define BIT17                                   (1 << SHIFT17)
#define BIT18                                   (1 << SHIFT18)
#define BIT19                                   (1 << SHIFT19)
#define BIT20                                   (1 << SHIFT20)
#define BIT21                                   (1 << SHIFT21)
#define BIT22                                   (1 << SHIFT22)
#define BIT23                                   (1 << SHIFT23)
#define BIT24                                   (1 << SHIFT24)
#define BIT25                                   (1 << SHIFT25)
#define BIT26                                   (1 << SHIFT26)
#define BIT27                                   (1 << SHIFT27)
#define BIT28                                   (1 << SHIFT28)
#define BIT29                                   (1 << SHIFT29)
#define BIT30                                   (1 << SHIFT30)
#define BIT31                                   (1 << SHIFT31)

#define FREQ_200KHZ                             200000
#define FREQ_500KHZ                             500000
#define FREQ_600KHZ                             600000
#define FREQ_1600KHZ                            1600000

#define FREQ_137000KHZ                          137000000
#define FREQ_175000KHZ                          175000000
#define FREQ_410000KHZ                          410000000
#define FREQ_433050KHZ                          433050000
#define FREQ_433300KHZ                          433300000
#define FREQ_434665KHZ                          434665000
#define FREQ_434790KHZ                          434790000
#define FREQ_525000KHZ                          525000000
#define FREQ_786000KHZ                          862000000

#ifndef FREQ_862000KHZ
#define FREQ_862000KHZ                          862000000
#endif
#ifndef FREQ_863000KHZ
#define FREQ_863000KHZ                          863000000
#endif
#ifndef FREQ_867000KHZ
#define FREQ_867000KHZ                          867000000
#endif
#ifndef FREQ_865000KHZ
#define FREQ_865000KHZ                          865000000
#endif
#ifndef FREQ_866550KHZ
#define FREQ_866550KHZ                          866550000
#endif
#ifndef FREQ_868100KHZ
#define FREQ_868100KHZ                          868100000
#endif
#ifndef FREQ_869525KHZ
#define FREQ_869525KHZ                          869525000
#endif
#ifndef FREQ_870000KHZ
#define FREQ_870000KHZ                          870000000
#endif

#ifndef FREQ_902300KHZ
#define FREQ_902300KHZ                          902300000
#endif
#ifndef FREQ_903000KHZ
#define FREQ_903000KHZ                          903000000
#endif
#ifndef FREQ_923300KHZ
#define FREQ_923300KHZ                          923300000
#endif

#ifndef FREQ_915200KHZ
#define FREQ_915200KHZ                          915200000
#endif

#ifndef FREQ_915900KHZ
#define FREQ_915900KHZ                          915900000
#endif

#ifndef FREQ_927500KHZ
#define FREQ_927500KHZ                          927500000
#endif

#ifndef FREQ_928500KHZ
#define FREQ_928500KHZ                          928500000
#endif

#ifndef FREQ_920900KHZ
#define FREQ_920900KHZ                          920900000
#endif

#ifndef FREQ_923300KHZ
#define FREQ_923300KHZ                          923300000
#endif

#ifndef FREQ_922100KHZ
#define FREQ_922100KHZ                          922100000
#endif


#ifndef FREQ_921900KHZ
#define FREQ_921900KHZ                          921900000
#endif

#ifndef FREQ_923000KHZ
#define FREQ_923000KHZ                          923000000
#endif

#ifndef FREQ_925000KHZ
#define FREQ_925000KHZ                          925000000
#endif

#ifndef FREQ_920000KHZ
#define FREQ_920000KHZ                          920000000
#endif

#ifndef FREQ_922000KHZ
#define FREQ_922000KHZ                          922000000
#endif


#ifndef FREQ_928000KHZ
#define FREQ_928000KHZ                          928000000
#endif

#ifndef FREQ_915000KHZ
#define FREQ_915000KHZ                          915000000
#endif

#ifndef FREQ_923200KHZ
#define FREQ_923200KHZ                          923200000
#endif

#ifndef FREQ_866550KHZ
#define FREQ_866550KHZ                          866550000
#endif

#define FREQ_1020000KHZ                         1020000000

#define DEFAULT_CALIBRATION_FREQ                (FREQ_868100KHZ)

#define BATTERY_LEVEL_INVALID                   (0xFF)
#define EXTERNALLY_POWERED						(0x00)

#ifdef	__cplusplus
}
#endif

#endif /* STACK_COMMON_H_INCLUDED */

/* eof stack_common.h */
