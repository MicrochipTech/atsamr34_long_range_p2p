/**
* \file  pmm.c
*
* \brief This is the implementation of LoRaWAN power management module
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

/******************************************************************************
                   Includes section
******************************************************************************/
#ifndef _PDS_INTERFACE_H
#define	_PDS_INTERFACE_H
#include "compiler.h"



/******************************************************************************
                   Defines section
******************************************************************************/
#define DECLARE_ITEM(RAMADDR, FILEID, ITEMID, SIZE, MEMOFFSET)  {.ramAddress = RAMADDR, .fileId = FILEID, .itemId = (ITEMID & 0xFF), .size = SIZE, .itemOffset = MEMOFFSET}

#define ITEM_HERADER_MAGIC_NUM		0xa5
//&& 0xFF);
#define PDS_STORE(item)				do {	\
				PdsFileItemIdx_t file = item >> 8; \
				uint8_t itemNum =  (uint8_t)(item & 0x00FF);\
				PDS_Store(file, itemNum); \
				} while(0)

#define PDS_DELETE(item)				do {	\
				PdsFileItemIdx_t file = item >> 8; \
				uint8_t itemNum =  (uint8_t)item & 0xFF; \
				PDS_Delete(file, itemNum); \
				} while(0)
					
#define PDS_RESTORE(item)				do {	\
				PdsFileItemIdx_t file = item >> 8; \
				uint8_t itemNum =  (uint8_t)item & 0xFF; \
				PDS_Restore(file, itemNum); \
				} while(0)

#define PDS_FILE_START_OFFSET     	0x00

#define PDS_NVM_VERSION				0x01	
#define PDS_WL_VERSION				0x01
#define PDS_FILES_VERSION			0x01

#define PDS_MAGIC					0xa5

/******************************************************************************
                               Types section
*******************************************************************************/
/* PDS Status codes */
typedef enum _PdsStatus
{
	PDS_OK = 0, 
	PDS_CRC_ERROR, // If CRC fails
	PDS_ERROR, // An error from the nvm driver
	PDS_NOT_FOUND, // If file or item cannot be found
	PDS_NOT_ENOUGH_MEMORY, // If Not enough space is alloted for wear leveling, there must be at least one other row available for each row used 
	PDS_INVLIAD_FILE_IDX,
	PDS_ITEM_DELETED
} PdsStatus_t;

/* PDS Item Operations */
typedef enum _PdsOperations
{
	PDS_OP_NONE = 0x00,
	PDS_OP_STORE = 0x01,
	PDS_OP_DELETE = 0x02
} PdsOperations_t;

/* PDS File IDs*/
typedef enum _PdsFileItemIdx
{
	PDS_FILE_MAC_01_IDX = 0U,
	PDS_FILE_MAC_02_IDX,
	PDS_FILE_REG_NA_03_IDX,
	PDS_FILE_REG_EU868_04_IDX,
	PDS_FILE_REG_AS_05_IDX,
	PDS_FILE_REG_KR_06_IDX,
	PDS_FILE_REG_IND_07_IDX,
	PDS_FILE_REG_JPN_08_IDX,
	PDS_FILE_REG_AU_09_IDX,
	PDS_FILE_REG_KR2_10_IDX,
	PDS_FILE_REG_JPN2_11_IDX,
	PDS_FILE_REG_EU868_12_IDX,
	PDS_FILE_APP_DATA1_13_IDX,
	PDS_MAX_FILE_IDX
} PdsFileItemIdx_t;

/* PDS item header structure */
typedef struct _ItemHeader
{
	uint8_t magic;
	uint8_t version;
	uint8_t size;
	uint8_t itemId;
	bool delete;
} ItemHeader_t;

/* PDS item map structure */
typedef struct _ItemMap
{
	uint8_t *ramAddress;
	uint8_t fileId;
	uint8_t itemId;
	uint8_t size;
	uint8_t itemOffset;
} ItemMap_t;

/* PDS File Marks structure */
typedef struct _PdsFileMarks
{
	PdsOperations_t* fileMarkListAddr;
	uint8_t     	 numItems;
	ItemMap_t* 	 itemListAddr;
	void         (*fIDcb)(void);
} PdsFileMarks_t;

#define PDS_SIZE_OF_ITEM_HDR         sizeof(ItemHeader_t)

/******************************************************************************
                   Prototypes section
******************************************************************************/

/**************************************************************************//**
\brief Initializes the PDS.

\param[in] none
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t PDS_Init(void);

/**************************************************************************//**
\brief Disables storing to PDS.

\param[in] none
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t PDS_UnInit(void);

/**************************************************************************//**
\brief 	This function will set the store operation bit in the filemarks for the
		item in PDS

\param[in] pdsFileItemIdx - The file id to regsiter file to PDS.
\param[in] item - The item id of the item in PDS.
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t PDS_Store(PdsFileItemIdx_t pdsFileItemIdx, uint8_t item);

/**************************************************************************//**
\brief This function will restore an item from PDS to RAM.

\param[in] pdsFileItemIdx - The file id to regsiter file to PDS.
\param[in] item - The item id of the item in PDS.
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t PDS_Restore(PdsFileItemIdx_t pdsFileItemIdx, uint8_t item);

/**************************************************************************//**
\brief	This function will set the delete operation for the item
		in the filemask.

\param[in] pdsFileItemIdx - The file id to regsiter file to PDS.
\param[in] item - The item id of the item in PDS.
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t PDS_Delete(PdsFileItemIdx_t pdsFileItemIdx, uint8_t item);

/**************************************************************************//**
\brief	This function checks if all the registered files are restorable.

\param[out] status - The return status of the function's operation.
******************************************************************************/
bool PDS_IsRestorable(void);

/**************************************************************************//**
\brief This function will erase all the items stored in the PDS.

\param[out] status - The return status of the function's operation.
******************************************************************************/
PdsStatus_t PDS_DeleteAll(void);

/**************************************************************************//**
\brief 	This function will restore all the items from the PDS to RAM
		from all registered files.

\param[out] status - The return status of the function's operation.
******************************************************************************/
PdsStatus_t PDS_RestoreAll(void);

/**************************************************************************//**
\brief	This function will set the store operation to all the items stored 
		in all the registered files in PDS.

\param[in] none
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t PDS_StoreAll(void);

/**************************************************************************//**
\brief This function registers a file to the PDS.

\param[in] argFileId - The file id to register file to PDS.
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t PDS_RegFile(PdsFileItemIdx_t argFileId, PdsFileMarks_t argFileMarks);

/**************************************************************************//**
\brief This function un-registers a file to the PDS.

\param[in] argFileId - The file id to un-register file to PDS.
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t PDS_UnRegFile(PdsFileItemIdx_t argFileId);

#endif  /*_PDS_INTERFACE_H */

/* eof pds_interface.h */
