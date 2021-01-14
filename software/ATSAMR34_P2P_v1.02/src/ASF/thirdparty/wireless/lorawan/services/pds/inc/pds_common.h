/**
* \file  pmm.c
*
* \brief This is the implementation of LoRaWAN power management module
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

#ifndef _PDS_COMMON_H
#define _PDS_COMMON_H

/******************************************************************************
                   Includes section
******************************************************************************/
#include "string.h"
#include "limits.h"

/******************************************************************************
                               Defines and Types section
*******************************************************************************/
#define PDS_MEM_SIZE			8192U

typedef struct _PdsNvmHeader_t
{
	uint16_t crc;
	uint8_t size;
	uint8_t version;
} PdsNvmHeader_t;

#define PDS_NVM_MEM_SIZE		NVMCTRL_ROW_SIZE
#define PDS_NVM_HEADER_SIZE		sizeof(PdsNvmHeader_t)
#define PDS_NVM_DATA_SIZE		PDS_NVM_MEM_SIZE - PDS_NVM_HEADER_SIZE


COMPILER_PACK_SET(1)
typedef struct _PdsWlHeader_t
{
	uint8_t magicNo;
	uint8_t version;
	uint8_t size;
	uint16_t memId;
	uint32_t counter;
} PdsWlHeader_t;
COMPILER_PACK_RESET()

typedef uint8_t PdsWlData_t;
typedef uint8_t PdsWlMem_t;

#define PDS_WL_MEM_SIZE			PDS_NVM_DATA_SIZE
#define PDS_WL_HEADER_SIZE		sizeof(PdsWlHeader_t)
#define PDS_WL_DATA_SIZE		PDS_WL_MEM_SIZE - PDS_WL_HEADER_SIZE

COMPILER_PACK_SET(1)
typedef union _PdsWl_t
{
	struct _WL_Struct
	{
		PdsWlHeader_t pdsWlHeader;
		PdsWlData_t pdsWlData[PDS_WL_DATA_SIZE];
	} WL_Struct;
	struct _WL_Mem
	{
		PdsWlMem_t pdsWlMem[PDS_WL_MEM_SIZE];
	} WL_Mem;
} PdsWl_t;
COMPILER_PACK_RESET()

typedef PdsWl_t PdsNvmData_t;
typedef uint8_t PdsNvmMem_t;

COMPILER_PACK_SET(1)
typedef union _PdsNvm_t
{
	struct _NVM_Struct
	{
		PdsNvmHeader_t pdsNvmHeader;
		PdsNvmData_t pdsNvmData;
	} NVM_Struct;
	struct _NVM_Mem
	{
		PdsNvmMem_t pdsNvmMem[PDS_NVM_MEM_SIZE];
	} NVM_Mem;
} PdsNvm_t;
COMPILER_PACK_RESET()


typedef PdsNvm_t PdsMem_t;





/******************************************************************************
                   Prototypes section
******************************************************************************/

#endif  /*_PDS_COMMON_H */

/* eof pds_common.h */
