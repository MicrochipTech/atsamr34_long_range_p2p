/**
* \file  pds_wl.c
*
* \brief This is the Pds wear levelling source file which contains Pds wear levelling
*        implimentation.
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
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "pds_interface.h"
#include "pds_common.h"
#include "pds_task_handler.h"
#include "pds_wl.h"

/************************************************************************/
/*  Static variables                                                    */
/************************************************************************/
static RowMap_t rowMap[EEPROM_NUM_ROWS];
static FileMap_t fileMap[PDS_MAX_FILE_IDX];

/******************************************************************************
                   Static prototype section
******************************************************************************/
static void pdsUpdateRowMap(void);
static void pdsUpdateFileMap(UpdateFileMap_t *updateFileMap);
static uint16_t pdsReturnFreeRowIdx(void);

/******************************************************************************
                   Implementations section
******************************************************************************/

/**************************************************************************//**
\brief Initializes the WL PDS by updating the row and file map.

\param[in] none
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t pdsWlInit(void)
{

	PdsStatus_t status = pdsNvmInit();
	if (PDS_OK != status)
	{
		return status;
	}
	PdsMem_t buffer;
	memset(&rowMap, UCHAR_MAX, EEPROM_NUM_ROWS * sizeof(RowMap_t));
    memset(&fileMap, UCHAR_MAX, PDS_MAX_FILE_IDX * sizeof(FileMap_t));
	memset(&buffer, 0, sizeof(PdsMem_t));
    UpdateFileMap_t updateFileMap;
	
    for(uint8_t rowIdx = 0; rowIdx< EEPROM_NUM_ROWS; rowIdx++)
    {
		status = pdsNvmRead(rowIdx, &buffer, EEPROM_ROW_SIZE);
		if (PDS_OK == status)
		{
			rowMap[rowIdx].counter = buffer.NVM_Struct.pdsNvmData.WL_Struct.pdsWlHeader.counter;
            rowMap[rowIdx].memId = buffer.NVM_Struct.pdsNvmData.WL_Struct.pdsWlHeader.memId;
            rowMap[rowIdx].previousIdx = USHRT_MAX;
            updateFileMap.counter = buffer.NVM_Struct.pdsNvmData.WL_Struct.pdsWlHeader.counter;
            updateFileMap.memId = buffer.NVM_Struct.pdsNvmData.WL_Struct.pdsWlHeader.memId;
            updateFileMap.rowIdx = rowIdx;
            pdsUpdateFileMap(&updateFileMap);
		}
    }
    pdsUpdateRowMap();
	
	return PDS_OK;
}

/**************************************************************************//**
\brief	This function will find the free row index to write to, updates the WL_Struct
		header and writes to NVM. If the nvm write is successful it updates the
		row and file map.

\param[in] 	pdsFileItemIdx - The file id to be written to.
\param[in] 	buffer - The buffer containing data to be written.
\param[in] 	size - The size of the data in the buffer.
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t pdsWlWrite(PdsFileItemIdx_t pdsFileItemIdx, PdsMem_t *buffer, uint16_t size)
{
	UpdateFileMap_t updateFileMap;
	PdsStatus_t status = PDS_OK;
	uint32_t counter;
    uint16_t rowIdx = pdsReturnFreeRowIdx();
	buffer->NVM_Struct.pdsNvmData.WL_Struct.pdsWlHeader.counter++;
	buffer->NVM_Struct.pdsNvmData.WL_Struct.pdsWlHeader.memId = pdsFileItemIdx;
	buffer->NVM_Struct.pdsNvmData.WL_Struct.pdsWlHeader.magicNo = PDS_MAGIC;
	buffer->NVM_Struct.pdsNvmData.WL_Struct.pdsWlHeader.version = PDS_WL_VERSION;
	buffer->NVM_Struct.pdsNvmData.WL_Struct.pdsWlHeader.size = size;
	counter = buffer->NVM_Struct.pdsNvmData.WL_Struct.pdsWlHeader.counter;
	size += sizeof(PdsWlHeader_t);

	status = pdsNvmWrite(rowIdx, buffer, size);
	if (PDS_OK == status)
	{
		rowMap[rowIdx].counter = counter;
		rowMap[rowIdx].memId = pdsFileItemIdx;
		rowMap[rowIdx].previousIdx = USHRT_MAX;
		updateFileMap.counter = rowMap[rowIdx].counter;
		updateFileMap.memId = rowMap[rowIdx].memId;
		updateFileMap.rowIdx = rowIdx;
		pdsUpdateFileMap(&updateFileMap);
	}
	
	return status;
}

/**************************************************************************//**
\brief	This function will find extract the row where the file is stored and 
		read from NVM.

\param[in] 	pdsFileItemIdx - The file id to be read from.
\param[in] 	buffer - The buffer containing data to be written.
\param[in] 	size - The size of the data in the buffer.
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t pdsWlRead(PdsFileItemIdx_t pdsFileItemIdx, PdsMem_t *buffer, uint16_t size)
{
	PdsStatus_t status = PDS_OK;
	uint16_t rowIdx = fileMap[pdsFileItemIdx].maxCounterRowIdx;
	if (USHRT_MAX == rowIdx)
	{
		return PDS_NOT_FOUND;
	}
	
	size += sizeof(PdsWlHeader_t);
	status = pdsNvmRead(rowIdx, buffer, size);
	
	return status;
}

/**************************************************************************//**
\brief	Updates the row map so that the old entries of the file are erased
		in the row map.

\param[in] - return none
******************************************************************************/
static void pdsUpdateRowMap()
{
	uint16_t rowIdx = USHRT_MAX;
    for(uint16_t memId = 0, rowIdxLocal = 0 ; memId < PDS_MAX_FILE_IDX; memId++)
    {
		
        // make all the previous indexes of the max_counter_row_idx invalid by writing 0xFF
		if(USHRT_MAX != fileMap[memId].maxCounterRowIdx)
		{
			rowIdx = rowMap[fileMap[memId].maxCounterRowIdx].previousIdx;
			rowMap[fileMap[memId].maxCounterRowIdx].previousIdx = USHRT_MAX;
			while(rowIdx != USHRT_MAX)
			{
				rowIdxLocal = rowMap[rowIdx].previousIdx;
				rowMap[rowIdx].previousIdx = USHRT_MAX;
				rowMap[rowIdx].counter = UINT_MAX;
				rowMap[rowIdx].memId = USHRT_MAX;
				rowIdx = rowIdxLocal;
			}
		}
    }
}

/**************************************************************************//**
\brief Updates the file map so that it points to the latest row index for a file id.

\param[out] - returns none
******************************************************************************/
static void pdsUpdateFileMap(UpdateFileMap_t *updateFileMap)
{
    uint16_t *presentRowIdx;
    uint16_t *previousRowIdx;
    uint16_t *lastRowIdx;
    uint16_t prevTemp;
    if(USHRT_MAX == fileMap[updateFileMap->memId].maxCounterRowIdx)
    {   
		/* If there is no entry in filemap update current rowidx as maxcounteridx*/
        fileMap[updateFileMap->memId].maxCounterRowIdx = updateFileMap->rowIdx;

    }
    else if(rowMap[fileMap[updateFileMap->memId].maxCounterRowIdx].counter < rowMap[updateFileMap->rowIdx].counter)
    {
        /* If the entry in filemap is lesser than  current rowidx update current idx as maxcounteridx*/
		// update the max counter rowIdx of the memId as previous_idx in the present row
        rowMap[updateFileMap->rowIdx].previousIdx = fileMap[updateFileMap->memId].maxCounterRowIdx;
        fileMap[updateFileMap->memId].maxCounterRowIdx = updateFileMap->rowIdx;

    }
	else if (rowMap[fileMap[updateFileMap->memId].maxCounterRowIdx].counter == rowMap[updateFileMap->rowIdx].counter)
	{

		return;
	}
    else
    {
        previousRowIdx = &rowMap[fileMap[updateFileMap->memId].maxCounterRowIdx].previousIdx;
        presentRowIdx = &updateFileMap->rowIdx;
        lastRowIdx = &fileMap[updateFileMap->memId].maxCounterRowIdx;
		do
		{
			if (USHRT_MAX == *previousRowIdx)
			{
				*previousRowIdx = *presentRowIdx;
				break;
			}
			if (rowMap[*previousRowIdx].counter > rowMap[*presentRowIdx].counter)
			{
				lastRowIdx = previousRowIdx;
				previousRowIdx = &rowMap[*previousRowIdx].previousIdx;
			}
			else
			{
			    prevTemp = *previousRowIdx;
				rowMap[*lastRowIdx].previousIdx = *presentRowIdx;
				rowMap[*presentRowIdx].previousIdx = prevTemp;
				break;
			}
		} while(true);

    }
}

/**************************************************************************//**
\brief Finds free row index by scanning the row map.

\param[out] - returns free row index
******************************************************************************/
static uint16_t pdsReturnFreeRowIdx(void)
{
    uint8_t rowIdx;
    bool found = 0;
    for(rowIdx = 0; rowIdx < EEPROM_NUM_ROWS; rowIdx++)
    {
        if( (UINT_MAX == rowMap[rowIdx].counter) &&
            (USHRT_MAX == rowMap[rowIdx].previousIdx) &&
            (USHRT_MAX == rowMap[rowIdx].memId) )
        {
            found = 1;
            break;
        }
    }

    if(!found)
    {
        pdsUpdateRowMap();
        for(rowIdx = 0; rowIdx < EEPROM_NUM_ROWS; rowIdx++)
        {
            if( (UINT_MAX == rowMap[rowIdx].counter) &&
                (USHRT_MAX == rowMap[rowIdx].previousIdx) &&
                (USHRT_MAX == rowMap[rowIdx].memId))
            {
                break;
            }
        }
    }
	return rowIdx;
}

/**************************************************************************//**
\brief This function checks if a file is found in the file map.

\param[out] - return true or false
******************************************************************************/
bool isFileFound(PdsFileItemIdx_t pdsFileItemIdx)
{
	uint16_t rowIdx = fileMap[pdsFileItemIdx].maxCounterRowIdx;
	if (USHRT_MAX == rowIdx)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void pdsWlDeleteAll(void)
{
	/* Clear Filemap array */
	memset(&rowMap, UCHAR_MAX, EEPROM_NUM_ROWS * sizeof(RowMap_t));
	/* Clear Row Map Array */
    memset(&fileMap, UCHAR_MAX, PDS_MAX_FILE_IDX * sizeof(FileMap_t));
	/* Call NVM Erase All */
	pdsNvmEraseAll();
}

#endif
/* eof pds_wl.c */
