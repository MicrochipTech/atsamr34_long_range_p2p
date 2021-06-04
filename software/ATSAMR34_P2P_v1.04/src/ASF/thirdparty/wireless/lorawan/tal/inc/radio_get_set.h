/**
* \file  radio_get_set.h
*
* \brief This is the radio_get_set header file which contains interfaces for
*	Getting and Setting the Radio and Transciver parameters.
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


#ifndef _RADIO_GET_SET_H
#define _RADIO_GET_SET_H

 
/************************************************************************/
/*  Includes                                                            */
/************************************************************************/

/************************************************************************/
/*  Includes                                                            */
/************************************************************************/

#define RADIO_DEFAULT_FREQ DEFAULT_CALIBRATION_FREQ

/************************************************************************/
/*  Global variables                                                    */
/************************************************************************/

/************************************************************************/
/*  Function prototypes                                                 */
/************************************************************************/

/*********************************************************************//**
\brief	This function sets the transmit frequency. 

\param frequency	- Sets the transmit radio frequency.
\return				- none.
*************************************************************************/
void Radio_WriteFrequency(uint32_t frequency);

/*********************************************************************//**
\brief	This function prepares the transceiver for transmit and receive
		according to modulation set.

\param symbolTimeout	- Sets the symbolTimeout parameter.
\return					- none.
*************************************************************************/
void Radio_WriteConfiguration(uint16_t symbolTimeout);

/*********************************************************************//**
\brief	This function sets the radioMode and modulation in order to set
		the transceiver to transmit and receive at a set modulation. 

\param newMode			- Sets the transceiver mode.
\param newModulation	- Sets the modulation.
\param blocking			- Sets if its blocking call or not.
\return					- none.
*************************************************************************/
void Radio_WriteMode(RadioMode_t newMode, RadioModulation_t newModulation, uint8_t blocking);

#endif  /*_RADIO_GET_SET_H*/

// eof radio_get_set.h