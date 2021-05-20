/**
* \file  wlPdsTypesConverter.c
*
* \brief PDS types converter implementation.
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
#if PDS_ENABLE_WEAR_LEVELING

/******************************************************************************
                    Includes section
******************************************************************************/
#include <wlPdsBindings.h>
#include <wlPdsMemIds.h>
#include <wlPdsTypesConverter.h>
#include <string.h>
#include <assert.h>
//#include "thread_pds.h"
#include "assert.h"
#include "miwi_api.h"

/******************************************************************************
                    Definitions section
******************************************************************************/
#define COMPID "wlPdsTypesConverter"

/******************************************************************************
                    Types section
******************************************************************************/


/******************************************************************************
                    Externals section
******************************************************************************/
MeshGeneralInfotMem_t genInfoMem;

/******************************************************************************
                    Prototypes section
******************************************************************************/

static void verifyItemSizes (S_Nv_ItemId_t id, uint16_t size, uint16_t oldSize);

/******************************************************************************
                    Static variables section
******************************************************************************/


/******************************************************************************
                    Implementation section
******************************************************************************/

/******************************************************************************
\brief Updates BC parameters after restoring taking into account possible size
  changes

\param[in] id      - item id;
\param[in] data    - pointer to data;
\param[in] size    - current item size;
\param[in] oldSize - last size of item

returns true if memory was updated successfully, false - otherwise
******************************************************************************/
bool pdsUpdateMemory(S_Nv_ItemId_t id, void *data, uint16_t size, uint16_t oldSize)
{
  verifyItemSizes(id, size, oldSize);
  
  switch (id)
  {
	  
    default:
      break;
  }
    

  return true;
}

/******************************************************************************
\brief Verifies whether size change is allowed for given item

\param[in] id      - item id;
\param[in] size    - current item size;
\param[in] oldSize - last size of item
******************************************************************************/
static void verifyItemSizes(S_Nv_ItemId_t id, uint16_t size, uint16_t oldSize)
{
  ItemIdToMemoryMapping_t mapItem;

  if (size == oldSize)
    return;

  if (pdsGetItemDescr(id, &mapItem))
  {
    assert (mapItem.flags & SIZE_MODIFICATION_ALLOWED);
  }
}


/******************************************************************************
\brief To get the item descriptor for the given item ID

\param[in] itemId            - item id;
\param[in] itemDescrToGet    - pointer to item descriptor to be loaded;

returns true if descriptor is found out for the given item ID, false - otherwise
******************************************************************************/
bool pdsGetItemDescr(S_Nv_ItemId_t itemId, ItemIdToMemoryMapping_t *itemDescrToGet )
{
  ItemIdToMemoryMapping_t *itemDescrPtr = (ItemIdToMemoryMapping_t *)PDS_FF_START;

  /* Calculate raw data size */
  for (uint8_t fileIdx = 0; fileIdx < PDS_ITEM_AMOUNT; fileIdx++)
  {
    ItemIdToMemoryMapping_t itemDescr;

    memcpy(&itemDescr, (void const *)itemDescrPtr, sizeof(ItemIdToMemoryMapping_t));

    if(itemDescr.itemId == itemId)
    {
      memcpy(itemDescrToGet, &itemDescr, sizeof(ItemIdToMemoryMapping_t));
      return true;
    }
    itemDescrPtr++;

  }
  return false;
}
/******************************************************************************
\brief To get the directory descriptor for the given dir ID

\param[in] itemId           - item id;
\param[in] dirDescrToGet    - pointer to dir descriptor to be loaded;

returns true if descriptor is found out for the given dir ID, false - otherwise
******************************************************************************/
bool pdsGetDirDescr(S_Nv_ItemId_t itemId, PDS_DirDescr_t *dirDescrToGet )
{
  PDS_DirDescr_t *dirDescrPtr = (PDS_DirDescr_t *)PDS_FD_START;

  /* Calculate raw data size */
  for (uint8_t fileIdx = 0; fileIdx < PDS_DIRECTORIES_AMOUNT; fileIdx++)
  {
    PDS_DirDescr_t dirDescr;

    memcpy(&dirDescr, (void const *)dirDescrPtr, sizeof(PDS_DirDescr_t));
    if(dirDescr.memoryId == itemId)
    {
      memcpy(dirDescrToGet, &dirDescr, sizeof(PDS_DirDescr_t));
      return true;
    }
    dirDescrPtr++;
  }
  return false;
}
#endif 

// eof wlPdsTypesConverter.c