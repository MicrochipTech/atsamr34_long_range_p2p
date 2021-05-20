/**
* \file  LED.c
*
* \brief This is the implementation of LoRaWAN LED resource module
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

#include "asf.h"
#include "resources.h"

#include "conf_resource.h"

#include "LED.h"

/*********************************************************************//*
 \brief      Function to ON,OFF or Toggle the LED state
 \param[in]  resrc - could be LED_AMBER or LED_GREEN
 \param[in]  data  - could be LOFF, LON, LTOGGLE
             LED_0_PIN - Pin number of the LED0 in the board
 ************************************************************************/

int32_t set_LED_data(const uint8_t resrc,const uint8_t * data)
{
    int8_t status = 0;
    int8_t ledPin = -1;
    uint8_t value = *data;

#if (AMBER_LED == 1)
    if(resrc == LED_AMBER)
    {
        ledPin = LED_0_PIN;
    }
    else
#endif
#if (GREEN_LED == 1)
    if(resrc == LED_GREEN)
    {
        ledPin = LED_1_PIN;
    }
    else
#endif
    {
        ledPin = UNSUPPORTED_RESOURCE;
        value = UNSUPPORTED_RESOURCE;
    }

    switch(value)
    {
        case LOFF:
        {
            LED_Off(ledPin);
            break;
        }
        case LON:
        {
            LED_On(ledPin);
            break;
        }
        case LTOGGLE:
        {
            LED_Toggle(ledPin);
            break;
        }
        default:
        {
            status = UNSUPPORTED_RESOURCE;
            break;
        }
    }

    return status;
}





