/**
* \file  conf_regparams.h
*
* \brief LORAWAN Regional Parameter Configuration
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

 
#ifndef CONF_REGPARAMS_H_INCLUDED
#define CONF_REGPARAMS_H_INCLUDED

/*****************************************************************************/
/* LoRaWAN Regional configuration parameters                                */
/*****************************************************************************/

#if (!(NA_BAND == 1)  && !(AS_BAND == 1) && !(AU_BAND == 1) && !(EU_BAND == 1) && !(IND_BAND == 1) && !(JPN_BAND == 1) && !(KR_BAND == 1) )
#error "Error: Atleast one regional band should be enabled."
#endif

#if (NA_BAND == 1)

#define MAC_DEF_TX_POWER_NA					(7)
#define MAC_DEF_TX_CURRENT_DATARATE_NA		(DR2)
#define MAC_DATARATE_MIN_NA					(DR4)
#define MAC_DATARATE_MAX_NA					(DR0)
#define FEATURES_SUPPORTED_NA               (FHSS_SUPPORT | PA_SUPPORT)

#endif

#if (AS_BAND == 1)

#define MAC_DEF_TX_POWER_AS                  (1)
#define MAC_DEF_TX_CURRENT_DATARATE_AS       (DR3)
#define MAC_DATARATE_MIN_AS                  (DR7)
#define MAC_DATARATE_MAX_AS                  (DR0)
#define FEATURES_SUPPORTED_AS                (PA_SUPPORT | DUTY_CYCLE_SUPPORT)

#endif

#if (AU_BAND == 1)

#define MAC_DEF_TX_POWER_AU                  (7)
#define MAC_DEF_TX_CURRENT_DATARATE_AU       (DR3)
#define MAC_DATARATE_MIN_AU                  (DR6)
#define MAC_DATARATE_MAX_AU                  (DR0)
#define FEATURES_SUPPORTED_AU                (FHSS_SUPPORT | PA_SUPPORT)

#endif

#if (EU_BAND == 1)

#define MAC_DEF_TX_POWER_EU					(1)
#define MAC_DEF_TX_CURRENT_DATARATE_EU		(DR3)
#define MAC_DATARATE_MIN_EU					(DR7)
#define MAC_DATARATE_MAX_EU					(DR0)
#define FEATURES_SUPPORTED_EU               (DUTY_CYCLE_SUPPORT)
#endif

#if (IND_BAND == 1)

#define MAC_DEF_TX_POWER_IN                  (1)
#define MAC_DEF_TX_CURRENT_DATARATE_IN       (DR3)
#define MAC_DATARATE_MIN_IN                  (DR7)
#define MAC_DATARATE_MAX_IN                  (DR0)
#define FEATURES_SUPPORTED_IN                (PA_SUPPORT)
#endif

#if (JPN_BAND == 1)

#define MAC_DEF_TX_POWER_JP                  (1)
#define MAC_DEF_TX_CURRENT_DATARATE_JP       (DR3)
#define MAC_DATARATE_MIN_JP                  (DR7)
#define MAC_DATARATE_MAX_JP                  (DR0)

#define LBT_RSSI_SAMPLES_COUNT_JP		   	 (5)
#define FEATURES_SUPPORTED_JP                (LBT_SUPPORT | PA_SUPPORT)
#endif

#if (KR_BAND == 1)

#define MAC_DEF_TX_POWER_KR                   (1)
#define MAC_DEF_TX_CURRENT_DATARATE_KR        (DR3)
#define MAC_DATARATE_MIN_KR                   (DR5)
#define MAC_DATARATE_MAX_KR                   (DR0)

#define LBT_RSSI_SAMPLES_COUNT_KR		      (10)
#define FEATURES_SUPPORTED_KR                 (LBT_SUPPORT | PA_SUPPORT)
#endif

#if (KR_BAND == 1 || JPN_BAND == 1)

/*Number of Scan retries*/
#define LBT_MAX_RETRY_CHANNELS				  (5)

#endif

#endif /* CONF_REGPARAMS_H_INCLUDED */

/* eof conf_regparams.h */