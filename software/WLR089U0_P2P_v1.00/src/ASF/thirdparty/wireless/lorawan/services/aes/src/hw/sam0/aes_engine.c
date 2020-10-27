/**
* \file  aes_engine.c
*
* \brief This is the implementation of AES Module
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

/**************************************** INCLUDES****************************/
#include <stdlib.h>
#include <stdint.h>
#include "aes_engine.h"
#ifndef UT
#include "asf.h"
#endif

/**************************************** MACROS******************************/
/* 32bit array of size 4 used as input/argument for aes drivers*/
#define SUB_BLOCK_COUNT 4

/**************************************** GLOBALS****************************/
#ifndef UT
/* AES configuration */
struct aes_config g_aes_cfg;

/* Data array used as input/output for aes engine */
static uint32_t io_data[SUB_BLOCK_COUNT];

/* AES instance*/
struct aes_module aes_instance;
#endif

/*************************************IMPLEMENTATION****************************/
/**
 * \brief Encrypts the given block of data
 * \param[in,out] block Block of input data to be encrypted
 * \param[in] key Cryptographic key to be used in AES encryption
 */
void AESEncode(unsigned char* block, unsigned char* masterKey)
{
#ifndef UT	
	
	/* Configure the AES. */
	g_aes_cfg.encrypt_mode = AES_ENCRYPTION;
	g_aes_cfg.key_size = AES_KEY_SIZE_128;
	g_aes_cfg.start_mode = AES_AUTO_START;
	g_aes_cfg.opmode = AES_ECB_MODE;
	g_aes_cfg.cfb_size = AES_CFB_SIZE_128;
	g_aes_cfg.lod = false;
	aes_set_config(&aes_instance,AES, &g_aes_cfg);
	
	for(uint8_t i=0;i<SUB_BLOCK_COUNT;i++)
	{
		io_data[i] = convert_byte_array_to_32_bit(masterKey+(i*(sizeof(uint32_t))));
	}
	
	/* Set the cryptographic key. */
	aes_write_key(&aes_instance, io_data);

	/* The initialization vector is not used by the ECB cipher mode. */
	
	aes_set_new_message(&aes_instance);
	
	for(uint8_t i=0;i<SUB_BLOCK_COUNT;i++)
	{
		io_data[i] = convert_byte_array_to_32_bit(block+(i*(sizeof(uint32_t))));			
	}
		
	/* Write the data to be ciphered to the input data registers. */
	aes_write_input_data(&aes_instance, io_data);
	aes_clear_new_message(&aes_instance);
	/* Wait for the end of the encryption process. */
	while (!(aes_get_status(&aes_instance) & AES_ENCRYPTION_COMPLETE)) {
	}
	aes_read_output_data(&aes_instance,io_data);
	
	memcpy(block,io_data,BLOCKSIZE);
#endif	
}

/**
 * \brief Initializes the AES Engine.
 */
void AESInit(void)
{
#ifndef UT	
	//! [setup_config]
	aes_get_config_defaults(&g_aes_cfg);
	//! [setup_config]

	//! [setup_config_defaults]
	aes_init(&aes_instance,AES, &g_aes_cfg);
	//! [setup_config_defaults]
	//! [module_enable]
	aes_enable(&aes_instance);
#endif	
}

