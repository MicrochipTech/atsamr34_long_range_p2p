/**
* \file  radio_driver_SX1276.c
*
* \brief This is the Radio drivers SX1276 source file which contains LoRa-specific
*		 Radio drivers functions declarations and defines for SX1276
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

/************************************************************************/
/*  Includes                                                            */
/************************************************************************/
#ifdef UT
#include "lora_test_main.h"
#endif
#include "radio_interface.h"
#include "radio_registers_SX1276.h"
#include "radio_driver_SX1276.h"
#include "radio_driver_hal.h"
#include "radio_get_set.h"
#include "radio_transaction.h"
#include "sw_timer.h"
#include "sys.h"
#include "stdint.h"
#include "string.h"

/************************************************************************/
/*  Global variables                                                    */
/************************************************************************/
RadioConfiguration_t radioConfiguration;
#ifdef UT
uint16_t testRssi;
#endif // UT

/************************************************************************/
/*  external variables                                                    */
/************************************************************************/
extern volatile RadioCallbackMask_t radioCallbackMask;

/************************************************************************/
/*  Static functions                                                    */
/************************************************************************/

/************************************************************************/
/* Implementations                                                      */
/************************************************************************/

/*********************************************************************//**
\brief	This function sets the radioMode and modulation in order to set
		the transceiver to transmit and receive at a set modulation. 

\param newMode			- Sets the transceiver mode.
\param newModulation	- Sets the modulation.
\param blocking			- Sets if its blocking call or not.
\return					- none.
*************************************************************************/
void Radio_WriteMode(RadioMode_t newMode, RadioModulation_t newModulation, uint8_t blocking)
{
#ifndef UT
    uint8_t opMode;
    uint8_t dioMapping;
    RadioModulation_t currentModulation;
    RadioMode_t currentMode;

    if ((MODULATION_FSK == newModulation) &&
        ((MODE_RXSINGLE == newMode) || (MODE_CAD == newMode)))
    {
        // Unavailable modes for FSK. Just return.
        return;
    }

    // Sanity enforcement on parameters
    newMode &= 0x07;
    newModulation &= 0x01;

    opMode = RADIO_RegisterRead(REG_OPMODE);

    if ((opMode & 0x80) != 0)
    {
        currentModulation = MODULATION_LORA;
    }
    else
    {
        currentModulation = MODULATION_FSK;
    }

    currentMode = opMode & 0x07;

    // If we need to change modulation, we need to do this in sleep mode.
    // Otherwise, we can go straight to changing the current mode to newMode.
    if (newModulation != currentModulation)
    {
        // Go to sleep
        if (MODE_SLEEP != currentMode)
        {
            // Clear mode bits, effectively going to sleep
            RADIO_RegisterWrite(REG_OPMODE, opMode & (~0x07));
            currentMode = MODE_SLEEP;
        }
        // Change modulation
        if (MODULATION_FSK == newModulation)
        {
            // Clear MSB and sleep bits to make it stay in sleep
            opMode = opMode & (~0x87);
        }
        else
        {
            // LoRa mode. Set MSB and clear sleep bits to make it stay in sleep
            opMode = 0x80 | (opMode & (~0x87));
        }
        RADIO_RegisterWrite(REG_OPMODE, opMode);
    }

    // From here on currentModulation is no longer current, we will use
    // newModulation instead as it reflects the chip configuration.
    // opMode reflects the actual configuration of the chip.

    if (newMode != currentMode)
    {
        // If we need to block until the mode switch is ready, configure the
        // DIO5 pin to relay this information.
        if ((MODE_SLEEP != newMode) && (1 == blocking))
        {
            dioMapping = RADIO_RegisterRead(REG_DIOMAPPING2);
            if (MODULATION_FSK == newModulation)
            {
                // FSK mode
                dioMapping |= 0x30;     // DIO5 = 11 means ModeReady in FSK mode
            }
            else
            {
                // LoRa mode
                dioMapping &= ~0x30;    // DIO5 = 00 means ModeReady in LoRa mode
            }
            RADIO_RegisterWrite(REG_DIOMAPPING2, dioMapping);
        }

        // Do the actual mode switch.
        opMode &= ~0x07;                // Clear old mode bits
        opMode |= newMode;              // Set new mode bits
        RADIO_RegisterWrite(REG_OPMODE, opMode);

        // If required and possible, wait for switch to complete
        if (1 == blocking)
        {
            if (MODE_SLEEP != newMode)
            {
               // while (HAL_DIO5PinValue() == 0);
			   /* Wait for Mode change to happen */
			   delay_ms(1);
            }
            else
            {
                SystemBlockingWaitMs(1);
            }
        }
    }
#endif
}

/*********************************************************************//**
\brief	This function sets the receive frequency of the transceiver
		while hopping in FHSS.

\param		- none	
\return		- none.
*************************************************************************/
void RADIO_FHSSChangeChannel(void)
{
    uint32_t freq;
    RADIO_RegisterRead(REG_LORA_IRQFLAGS);

    if (radioConfiguration.frequencyHopPeriod)
    {
        if ((radioConfiguration.radioCallback) &&
            (1 == radioCallbackMask.BitMask.radioFhssfreqCallback))
        {
            radioConfiguration.radioCallback(RADIO_FHSS_NEXT_FREQ_CALLBACK, (void *)&freq);
            Radio_WriteFrequency(freq);
        }
    }

    // Clear FHSSChangeChannel interrupt
    RADIO_RegisterWrite(REG_LORA_IRQFLAGS, 1 << SHIFT1);
}

/*********************************************************************//**
\brief	This function generates random number using the transceiver
		and returns it.

\param		- none	
\return		- returns the random number generated.
*************************************************************************/
uint16_t RADIO_ReadRandom(void)
{
    uint8_t i;
    uint16_t retVal;
    retVal = 0;
	
	// Turn on the RF switch.
	Radio_EnableRfControl(RADIO_RFCTRL_RX); 
	// Enabling Radio Clock
	Radio_SetClockInput();
	
    // Mask all interrupts, do many measurements of RSSI
    Radio_WriteMode(MODE_SLEEP, MODULATION_LORA, 1);
    RADIO_RegisterWrite(REG_LORA_IRQFLAGSMASK, 0xFF);
    Radio_WriteMode(MODE_RXCONT, MODULATION_LORA, 1);
    for (i = 0; i < 16; i++)
    {
        SystemBlockingWaitMs(1);
        retVal <<= SHIFT1;
        retVal |= RADIO_RegisterRead(REG_LORA_RSSIWIDEBAND) & 0x01;
    }
	
	// Turning off the RF switch now.
	Radio_DisableRfControl(RADIO_RFCTRL_RX);
	
    // Return radio to sleep
    Radio_WriteMode(MODE_SLEEP, MODULATION_LORA, 1);
    // Clear interrupts in case any have been generated
    RADIO_RegisterWrite(REG_LORA_IRQFLAGS, 0xFF);
    // Unmask all interrupts
    RADIO_RegisterWrite(REG_LORA_IRQFLAGSMASK, 0x00);
	// Disabling Radio Clock save power
	Radio_ResetClockInput();
	
    return retVal;
}

/*********************************************************************//**
\brief This function reads the RSSI value for LoRa and FSK.

\param rssi	- The RSSI measured in the channel.
\return		- ERR_NONE. Other types are not used now.
*************************************************************************/
RadioError_t Radio_ReadRssi(int16_t *rssi)
{
	if (MODULATION_LORA == radioConfiguration.modulation)
	{
		if (radioConfiguration.frequency >= HF_FREQ_HZ)
		{
		#ifdef UT
			*rssi = testRssi;
		#else // UT
			*rssi = RSSI_HF_OFFSET + RADIO_RegisterRead(REG_LORA_PKTRSSIVALUE);
		#endif // UT
		}
		else
		{
		#ifdef UT
			*rssi = testRssi;
		#else // UT
			*rssi = RSSI_LF_OFFSET + RADIO_RegisterRead(REG_LORA_PKTRSSIVALUE);
		#endif // UT
		}
	}
	else if (MODULATION_FSK == radioConfiguration.modulation)
	{
	#ifdef UT
		*rssi = testRssi;
	#else // UT
		*rssi = -(RADIO_RegisterRead(REG_FSK_RSSIVALUE) >> 1);
	#endif // UT
	}
	else
	{
		return ERR_UNSUPPORTED_ATTR;
	}
	return ERR_NONE;
}

/*********************************************************************//**
\brief This function reads the RSSI value for LoRa.

\param rssi	- The RSSI measured in the channel.
\return		- ERR_NONE. Other types are not used now.
*************************************************************************/
RadioError_t Radio_ReadLoraRssi(int16_t *rssi)
{	
	if (radioConfiguration.frequency >= HF_FREQ_HZ)
	{
#ifdef UT
		*rssi = testRssi;
#else // UT
		*rssi = RSSI_HF_OFFSET + RADIO_RegisterRead(REG_LORA_RSSIVALUE);
#endif // UT		
	}
	else
	{
#ifdef UT
		*rssi = testRssi;
#else // UT
		*rssi = RSSI_LF_OFFSET + RADIO_RegisterRead(REG_LORA_RSSIVALUE);
#endif // UT
	}

	return ERR_NONE;
}

/*********************************************************************//**
\brief This function reads the RSSI value for FSK.

\param rssi	- The RSSI measured in the channel.
\return		- ERR_NONE. Other types are not used now.
*************************************************************************/
RadioError_t Radio_ReadFSKRssi(int16_t *rssi)
{	
#ifdef UT
	*rssi = testRssi;
#else // UT
	*rssi = -(RADIO_RegisterRead(REG_FSK_RSSIVALUE) >> 1);
#endif // UT
	
	return ERR_NONE;
}
/**
 End of File
 */
