/**
* \file  pds_nvm.h
*
* \brief This is the PDS NVM header file which containes NVM abstractions for PDS.
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

#ifndef _PDS_NVM_H_
#define _PDS_NVM_H_

/******************************************************************************
                   Includes section
******************************************************************************/
#include "compiler.h"
#include "pds_interface.h"
#include "common_nvm.h"
#include "nvm.h"
#include "conf_nvm.h"

/******************************************************************************
                   Defines section
******************************************************************************/
/* Default EEPROM Size - 4K Bytes */
#ifndef EEPROM_SIZE
#define EEPROM_SIZE			4096
#endif

#define EEPROM_PAGE_SIZE        (NVMCTRL_PAGE_SIZE)
#define EEPROM_PAGE_PER_ROW     (NVMCTRL_ROW_PAGES)
#define EEPROM_ROW_SIZE         (EEPROM_PAGE_SIZE*EEPROM_PAGE_PER_ROW)
#define EEPROM_NUM_ROWS         (EEPROM_SIZE/EEPROM_ROW_SIZE)


/******************************************************************************
                               Types section
*******************************************************************************/

/******************************************************************************
                   Prototypes section
******************************************************************************/

/**************************************************************************//**
\brief	Initializes the NVM by checking the EEPROM size and calling nvm init.

\param[in] none
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t pdsNvmInit(void);

/**************************************************************************//**
\brief	This function will calculate the crc of the buffer contents and writes
		the buffer contents to nvm. It will also read the same content to verify
		if its written properly.

\param[in] 	pdsFileItemIdx - The file id to be written.
\param[in] 	buffer - The buffer containing data to be written.
\param[in] 	size - The size of the data in the buffer.
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t pdsNvmWrite(uint16_t rowId, PdsMem_t *buffer, uint16_t size);

/**************************************************************************//**
\brief	This function will read the contents of NVM and verify the crc.

\param[in] 	pdsFileItemIdx - The file id to be read.
\param[in] 	buffer - The buffer containing data to be read.
\param[in] 	size - The size of the data in the buffer.
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t pdsNvmRead(uint16_t rowId, PdsMem_t *buffer, uint16_t size);

/**************************************************************************//**
\brief	Will erase the contents of a row.

\param[in] 	rowId - The rowId to be erased.
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t pdsNvmErase(uint16_t rowId);

/**************************************************************************//**
\brief	Erases all the contents of NVM of all rows.

\param[in] 	none
\param[out] status - The return status of the function's operation of type PdsStatus_t.
******************************************************************************/
PdsStatus_t pdsNvmEraseAll(void);

#endif  /* _PDS_NVM_H_ */

/* eof pds_nvm.h */