/**
* \file  sal.h
*
* \brief Header File - Security Abstraction Layer
*
*
* Copyright (c) 2019-2020 Microchip Technology Inc. and its subsidiaries.
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
#ifndef _SAL_H
#define _SAL_H

/* Describes the various items of the MLS Security Layer (Keys/EUIs) */
typedef enum _salItems
{
	/* Application Key */
	SAL_APP_KEY				= 0x00,
	/* Application Session Key */
	SAL_APPS_KEY			= 0x01,
	/* Network Session Key */
	SAL_NWKS_KEY			= 0x02,
	/* Multicast - Application Session Key */
	SAL_MCAST_APPS_KEY		= 0x03,
	/* Multicast - Network Session Key */
	SAL_MCAST_NWKS_KEY		= 0x04,
	/* Key Encryption Key - This key will be used while reading the Session keys 
	 * from ECC608 in a secured way */
	SAL_KEY_ENC_KEY			= 0x05,
	/* JOIN EUI */
	SAL_JOIN_EUI				= 0x06,
	/* Device EUI */
	SAL_DEV_EUI				= 0x07,
	//New key or item should be added before SAL_MAX_KEYS
	SAL_MAX_ITEMS		
}salItems_t;

#define SAL_KEY_LEN				16
#define SAL_EUI_LEN				8

/* Total No of items in SalItems_t */
#define SAL_ITEMS_NUM		SAL_MAX_ITEMS

/* Describes the various status of the Security Layer for the given request */
typedef enum _SalStatus
{
	/* Successfully executed the given request like encrypt, derive etc., */
	SAL_SUCCESS				= 0x00,
	/* Failure in executing the given request */
	SAL_FAILURE 			= 0x01,
	/* Invalid key type is given as input parameter for the particular function */
	SAL_INVALID_KEY_TYPE	= 0x02
	
} SalStatus_t;
 
 /**
 * \brief This function initializes the security modules like AES, ECC608 (If used)
 *
 * \return value of type SalStatus_t
 *         SAL_SUCCESS -- when initialization is successful
 *         SAL_FAILURE -- when initialization of AES/ECC608 is failed
 */
SalStatus_t SAL_Init(void);

/**
 * \brief This function encrypts the given block of data with the key specified/the key stored in ECC608A
 *
 * \param[in]  *buffer  -  pointer to block of data to be encrypted
 * \param[in]  key_type -  Name of the key which is used to encrypt the data 
 *						   (Note: This parameter is used when Key is stored in ECC608)
 * \param[in]  *key		-  Pointer to the key used for Encryption
 *						   (Note: This parameter is used when Key is provided by the upper layer)
 *
 * \return value of type SalStatus_t
 *         SAL_SUCCESS			-- when encryption is successful
 *         SAL_FAILURE			-- when encryption is failed
 *		   SAL_INVALID_KEY_TYPE -- when invalid key_type is given as input parameter
 */
SalStatus_t SAL_AESEncode(unsigned char* buffer, salItems_t key_type, unsigned char* key);

/**
 * \brief This function derives the session key using the Block of data given as input
 *
 * \param[in]  *block		-  pointer to block of data (16 Bytes) used for deriving the session key
 * \param[in]  src_key      -  value of type salItems_t - Name of the key which is used to derive the session key 
 *						       (Note: This parameter is used when src_key is stored in ECC608)
 * \param[in]  target_key   -  value of type salItems_t - Name of the Derived Session Key (NwkSKey/AppSKey)
 * \param[out]  *key		-  Pointer to the derived key (NwkSKey/AppSKey)
 *							   (Note: Incase ECC608 is used as Keystorage device, 
 *								Session keys will be directly stored in the memory slots 
 *								and this pointer will be invalid in that case)
 *						   
 *
 * \return value of type SalStatus_t
 *         SAL_SUCCESS			-- when Session key derivation is successful
 *         SAL_FAILURE			-- when Session key derivation is failed
 *		   SAL_INVALID_KEY_TYPE -- when invalid key_type is given as input parameter
 */
SalStatus_t SAL_DeriveSessionKey(unsigned char* block, salItems_t src_key, unsigned char* key, salItems_t target_key);

/**
 * \brief This function calculates the CMAC value using the key specified
 *
 * \param[in]  *key		    -  Pointer to the key which is used for calculating CMAC value for the given buffer (AppKey/NwkSKey/AppSKey)
 * \param[in]  key_type		-  value of type salItems_t - Name of the key which is used to calculate the CMAC 
 *						       (Note: This parameter is used when key is stored in ECC608)
 * \param[out]  *output		-  Pointer to the 16bytes CMAC value
 * \param[in]   *input		-  Pointer to the data for which CMAC value is being calculated
 * \param[in]	size        -  Length of the data for which CMAC value is being calculated 						   
 *						   
 *
 * \return value of type SalStatus_t
 *         SAL_SUCCESS			-- when CMAC calculation is successful
 *         SAL_FAILURE			-- when CMAC calculation is failed
 *		   SAL_INVALID_KEY_TYPE -- when invalid key_type is given as input parameter
 */
SalStatus_t SAL_AESCmac(uint8_t* key, salItems_t key_type, uint8_t* output, uint8_t* input, uint16_t size);

/**
 * \brief This function reads back the keys from ECC608 device using Encrypted Read
 *
 * \param[in]  key_type		-  value of type salItems_t - Name of the key which is being read back from ECC608
 * \param[in]  *key			-  Pointer to the key which is read back from ECC608				   
 *						   
 *
 * \return value of type SalStatus_t
 *         SAL_SUCCESS			-- when the key is successfully read back from ECC608 device
 *         SAL_FAILURE			-- when the read function got failed
 *		   SAL_INVALID_KEY_TYPE -- when invalid key_type is given as input parameter
 */
SalStatus_t SAL_Read(salItems_t key_type, uint8_t* key);

#endif  // _SAL_H