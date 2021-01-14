/**
* \file  resource.h
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


/* Prevent double inclusion */
#ifndef RESOURCE_H
#define RESOURCE_H


/****************************** INCLUDES **************************************/
#include "asf.h"

/***************************** DEFINES ****************************************/
/* Resource type */
typedef enum _resourceType
{
    TEMP_SENSOR = 0x1,
    LED_AMBER = 0x2,
    LED_GREEN = 0x3,
    UNSUPPORTED_RESOURCE = 0xFF
}resourceType_t;


/*************************** FUNCTIONS PROTOTYPE ******************************/

/*********************************************************************//**
 \brief      Function to get different resource data
 \param[in]  resourceType_t - Type of resource (Temperature)
 \param[out] *data    - Pointer to the data from different resource
*************************************************************************/
void get_resource_data(const resourceType_t resource, uint8_t * data);

/*********************************************************************//**
 \brief      Function to set data to resource
 \param[in]  resourceType_t - Type of resource (Temperature)
 \param[out] *data    - Pointer to the data from different resource
*************************************************************************/
int set_resource_data(const resourceType_t resource,const uint8_t * data);

/*********************************************************************//**
 \brief      Function to initialize the different resources
*************************************************************************/
void resource_init(void);

#endif



