/**
* \file  conf_stack.h
*
* \brief LORAWAN STack Configuration File
*		
*
* Copyright (c) 2018 Microchip Technology Inc. and its subsidiaries. 
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
 

#ifndef CONF_STACK_H_INCLUDED
#define CONF_STACK_H_INCLUDED

/*****************************************************************************/
/* LoRaWAN MAC layer configuration parameters                                */
/*****************************************************************************/
#define FEATURE_CLASSC 1

#if (FEATURE_CLASSC == 1)
#define FEATURE_DL_MCAST 1
#else
#define FEATURE_DL_MCAST 0
#endif

#if (FEATURE_CLASSC == 1)
#define LORAWAN_SUPPORTED_ED_CLASSES                (CLASS_A | CLASS_C)
#else
#define LORAWAN_SUPPORTED_ED_CLASSES                CLASS_A
#endif

#endif /* CONF_STACK_H_INCLUDED */

/* eof conf_stack.h */
