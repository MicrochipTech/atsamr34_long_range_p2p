/**
* \file  pds_wl.h
*
* \brief     This is the Pds wear levelling header file which contains Pds wear levelling 
*	headers.
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


#ifndef _PDS_WL_H_
#define _PDS_WL_H_

/******************************************************************************
                   Includes section
******************************************************************************/
#include "compiler.h"
#include "pds_nvm.h"
#include "pds_interface.h"

/******************************************************************************
                   Defines section
******************************************************************************/

/******************************************************************************
                               Types section
*******************************************************************************/
typedef struct _RowMap
{
    uint32_t counter;
    uint16_t memId;
    uint16_t previousIdx;
} RowMap_t;

typedef struct _FileMap
{
    uint16_t maxCounterRowIdx;
} FileMap_t;

typedef struct _UpdateFileMap
{
    uint32_t counter;
    uint16_t memId;
    uint16_t rowIdx;
} UpdateFileMap_t;

/******************************************************************************
                   Prototypes section
******************************************************************************/

/**************************************************************************//**
\brief Initializes the WL PDS by updating the row and file map.

\param[in] none
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t pdsWlInit(void);

/**************************************************************************//**
\brief	This function will find the free row index to write to, updates the WL_Struct
		header and writes to NVM. If the nvm write is successful it updates the
		row and file map.

\param[in] 	pdsFileItemIdx - The file id to be written to.
\param[in] 	buffer - The buffer containing data to be written.
\param[in] 	size - The size of the data in the buffer.
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t pdsWlWrite(PdsFileItemIdx_t pdsFileItemIdx, PdsMem_t *buffer, uint16_t size);

/**************************************************************************//**
\brief	This function will find extract the row where the file is stored and 
		read from NVM.

\param[in] 	pdsFileItemIdx - The file id to be read from.
\param[in] 	buffer - The buffer containing data to be written.
\param[in] 	size - The size of the data in the buffer.
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t pdsWlRead(PdsFileItemIdx_t pdsFileItemIdx, PdsMem_t *buffer, uint16_t size);

/**************************************************************************//**
\brief This function checks if a file is found in the file map.

\param[out] - return true or false
******************************************************************************/
bool isFileFound(PdsFileItemIdx_t pdsFileItemIdx);

/**************************************************************************//**
\brief This function Erases Filemap and Rowmap array in WL and Initiates NVM Erase all.

\param[out] - void
******************************************************************************/
void pdsWlDeleteAll(void);

#endif  /* _PDS_WL_H_ */

/* eof pds_wl.h */