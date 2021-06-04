/**
* \file  resource.c
*
* \brief This is the implementation of LoRaWAN resource module
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


/****************************** INCLUDES **************************************/
#include "asf.h"
#include "resources.h"
#include "temp_sensor.h"
#include "LED.h"

/*************************STATIC FUNCTIONS ************************************/

/****************************** VARIABLES *************************************/

/*************************FUNCTION PROTOTYPES *********************************/

/*********************************************************************//**
 \brief      Function to get different resource data
 \param[in]  resource - Type of resource (Motor, Light, Temperature)
 \param[out] *data    - Pointer to the data from different resource
*************************************************************************/
void get_resource_data(const resourceType_t resource,uint8_t * data)
{

    switch(resource)
    {

        case TEMP_SENSOR:
        {
            get_temp_sensor_data(data);
            break;
        }
        default:
        {
            *data = UNSUPPORTED_RESOURCE;
            break;
        }
    }
}

/************************************************************************
 \brief      Function to set data to resource
 \param[in]  resourceType_t - Type of resource (Temperature)
 \param[out] *data    - Pointer to the data from different resource
*************************************************************************/
int set_resource_data(const resourceType_t resource, const uint8_t * data)
{
    int status = 0;
    switch(resource)
    {
        case LED_AMBER:
        {
            status = set_LED_data(LED_AMBER,data);
        }
        break;
        case LED_GREEN:
        {
            status = set_LED_data(LED_GREEN,data);
        }
        break;
        default:
        {
            status = UNSUPPORTED_RESOURCE;
        }
    }
    return status;
}
/*********************************************************************//**
 \brief      Function to initialize different resources
*************************************************************************/
void resource_init(void)
{
    temp_sensor_init();
    //LED_init();
}
/*---------------------------------------------------------------------------*/




