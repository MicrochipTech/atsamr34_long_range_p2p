/**
* \file  pds_nvm.c
*
* \brief This is the Pds NVM source file which contains NVM abstraction for PDS.
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

#if (ENABLE_PDS == 1)
/******************************************************************************
                   Includes section
******************************************************************************/
#include "pds_interface.h"
#include "pds_common.h"
#include "pds_task_handler.h"
#include "pds_wl.h"


//#define PDS_FLASH_START_ADDRESS        (0x003E000UL)
#define PDS_FLASH_START_ADDRESS        NVMCTRL_RWW_EEPROM_ADDR
/************************************************************************/
/*  Static variables                                                    */
/************************************************************************/

/******************************************************************************
                   Static prototype section
******************************************************************************/
static uint16_t calculate_crc(uint16_t length, uint8_t *data);
static uint16_t Crc16Ccitt(uint16_t initValue, uint8_t byte);
static uint32_t nvmLogicalRowToPhysicalAddr(uint16_t logicalRow);

/******************************************************************************
                   Implementations section
******************************************************************************/

/**************************************************************************//**
\brief	Initializes the NVM by checking the EEPROM size and calling nvm init.

\param[in] none
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t pdsNvmInit(void)
{
	PdsStatus_t status = PDS_OK;
	status_code_t statusCode;
	struct nvm_parameters parameters;

	nvm_get_parameters(&parameters);
	
	statusCode = nvm_init(INT_FLASH);
	if (STATUS_OK != (status_code_genare_t) statusCode)
	{
		return PDS_ERROR;
	}

	if (EEPROM_SIZE > ( (parameters.rww_eeprom_number_of_pages/NVMCTRL_ROW_PAGES) * NVMCTRL_ROW_SIZE) )
	{
		return PDS_NOT_ENOUGH_MEMORY;
	}
	
	return status;
}

/**************************************************************************//**
\brief	This function will calculate the crc of the buffer contents and writes
		the buffer contents to nvm. It will also read the same content to verify
		if its written properly.

\param[in] 	pdsFileItemIdx - The file id to be written.
\param[in] 	buffer - The buffer containing data to be written.
\param[in] 	size - The size of the data in the buffer.
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t pdsNvmWrite(uint16_t rowId, PdsMem_t *buffer, uint16_t size)
{
	PdsStatus_t status = PDS_OK;
	buffer->NVM_Struct.pdsNvmHeader.version = PDS_NVM_VERSION;
	buffer->NVM_Struct.pdsNvmHeader.size = size;
	buffer->NVM_Struct.pdsNvmHeader.crc = calculate_crc(buffer->NVM_Struct.pdsNvmHeader.size, (uint8_t *)(&(buffer->NVM_Struct.pdsNvmData)));
	//buffer->NVM_Struct.pdsNvmHeader.size = size;
	size += sizeof(PdsNvmHeader_t);
	uint32_t addr = nvmLogicalRowToPhysicalAddr(rowId);
	status_code_t statusCode;

	statusCode = nvm_write(INT_FLASH, addr, (uint8_t *const)buffer, size);
	if (STATUS_OK != (status_code_genare_t) statusCode)
	{
		return PDS_ERROR;
	}
	status = pdsNvmRead(rowId, (PdsMem_t *const)buffer, size);
	
	return status;
}

/**************************************************************************//**
\brief	This function will read the contents of NVM and verify the crc.

\param[in] 	pdsFileItemIdx - The file id to be read.
\param[in] 	buffer - The buffer containing data to be read.
\param[in] 	size - The size of the data in the buffer.
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t pdsNvmRead(uint16_t rowId, PdsMem_t *buffer, uint16_t size)
{
	PdsStatus_t status = PDS_OK;
	status_code_genare_t statusCode;
	uint16_t crc;
	uint32_t addr = nvmLogicalRowToPhysicalAddr(rowId);
	if (EEPROM_ROW_SIZE == size)
	{
		do 
		{
			statusCode = nvm_read(INT_FLASH, addr, (uint8_t *const)buffer, size);
		} while (statusCode == STATUS_BUSY);
		
	}
	else
	{
		size += sizeof(PdsNvmHeader_t);
		do 
		{
			statusCode = nvm_read(INT_FLASH, addr, (uint8_t *const)buffer, size);
		} while (statusCode == STATUS_BUSY);		
	}
	
	if (STATUS_OK != statusCode)
	{
		return PDS_ERROR;
	}
	crc = buffer->NVM_Struct.pdsNvmHeader.crc;
	
	if (crc != calculate_crc(buffer->NVM_Struct.pdsNvmHeader.size, (uint8_t *)&(buffer->NVM_Struct.pdsNvmData))) 
	{
		return PDS_CRC_ERROR;
	}
	return status;
}

/**************************************************************************//**
\brief	Will erase the contents of a row.

\param[in] 	rowId - The rowId to be erased.
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t pdsNvmErase(uint16_t rowId)
{
	PdsStatus_t status = PDS_OK;
	uint32_t addr = nvmLogicalRowToPhysicalAddr(rowId);
	status_code_t statusCode;
	do
	{
		statusCode = nvm_erase_row(addr);
	} while (statusCode == ERR_BUSY);
	
	if (STATUS_OK != (status_code_genare_t) statusCode)
	{
		return PDS_ERROR;
	}
	return status;
}

/**************************************************************************//**
\brief	Erases all the contents of NVM of all rows.

\param[in] 	none
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t pdsNvmEraseAll(void)
{
	PdsStatus_t statusCode;
	for(uint8_t row_idx = 0; row_idx< EEPROM_NUM_ROWS; row_idx++)
    {
		statusCode = pdsNvmErase(row_idx);
		if (PDS_OK != statusCode)
		{
			return statusCode;
		}
	}
	
	return PDS_OK;
}

/**************************************************************************//**
\brief	Calculates the CRC in CCITT polynome.

\param[in] 	initValue - The rowId to be erased.
\param[in] 	byte - The rowId to be erased.
\param[out] uint16_t - The calculated 16 bit CRC.
******************************************************************************/
uint16_t Crc16Ccitt(uint16_t initValue, uint8_t byte)
{
  byte ^= initValue & 0xffU;
  byte ^= byte << 4U;

  return ((((uint16_t)byte << 8) | ((initValue & 0xff00U) >> 8))
          ^ (uint8_t)(byte >> 4) ^ ((uint16_t)byte << 3));
}

/**************************************************************************//**
\brief	Calculates the CRC.

\param[in] 	length - The amount of data for which CRC is to be calculated.
\param[in] 	data - The data.
\param[out] uint16_t - The calculated 16 bit CRC.
******************************************************************************/
static uint16_t calculate_crc(uint16_t length, uint8_t *data)
{
  uint16_t eeprom_crc = 0U;
  for (uint16_t i = 0; i < length; i++)
  {
    eeprom_crc = Crc16Ccitt(eeprom_crc, data[i]);
  }
  return eeprom_crc;
}

/**************************************************************************//**
\brief	Converts logical rows to physical address.

\param[in] 	logicalRow - The logical row.
\param[out] uint16_t - The calculated 16 bit CRC.
******************************************************************************/
static uint32_t nvmLogicalRowToPhysicalAddr(uint16_t logicalRow)
{
	//return (NVMCTRL_RWW_EEPROM_ADDR + (NVMCTRL_ROW_SIZE * logicalRow)); // PRVN
	 return (PDS_FLASH_START_ADDRESS + (NVMCTRL_ROW_SIZE * logicalRow));
}
#endif
/* eof pds_nvm.c */
