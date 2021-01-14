/**
* \file  sal.c
*
* \brief Security Abstraction Layer file
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

/**************************************** INCLUDES****************************/
/* ASF Header */
#include <asf.h>

/* AES Headers */
#include "aes_engine.h"

/* SAL Headers */
#include "conf_sal.h"
#include "sal.h"

/* CryptoAuth Library Headers */
#ifdef CRYPTO_DEV_ENABLED
#include "conf_atcad.h"
#include "cryptoauthlib.h"
#endif
/**************************************** MACROS******************************/

/**************************************** GLOBALS****************************/
#ifdef CRYPTO_DEV_ENABLED
/* List of Key Slot numbers in ECC608 where in LoRAWAN keys are stored */
static const uint8_t keySlots[SAL_ITEMS_NUM] = {
	/* Key slot where in Application Key is stored */
	APP_KEY_SLOT,
	/* Key Slot where in Application Session Key is stored */
	APPS_KEY_SLOT,
	/* Key Slot where in Network Session Key is stored */
	NWKS_KEY_SLOT,
	/* Key Slot where in Multicast - Application Session Key is stored */
	MCAST_APPS_KEY_SLOT,
	/* Key Slot where in Multicast - Network Session Key is stored */
	MCAST_NWKS_KEY_SLOT,
	/* Key Slot where Key Encryption Key is stored */
	KEK_SLOT,	
	/* Key Slot where Application EUI is stored */
	APP_EUI_SLOT,
	/* Key Slot where Device EUI is stored */
	DEV_EUI_SLOT
};

/* Variable array to hold the Key Encryption Key */
static uint8_t keyEncryptionKey[32];
/* Default configuration for an ECCx08A device on the I2C bus */
static ATCAIfaceCfg cfg_atecc608a_i2c_default;

/**************************FUNCION DEFINITION***********************************/
/* Function to generate random 32 bytes key and write that to Key Encryption Key Slot */
static SalStatus_t sal_WriteKeyEncryptionKey(void);
#endif

static void sal_GenerateSubkey (uint8_t* key, salItems_t key_type, uint8_t* k1, uint8_t* k2);
static void sal_FillSubKey( uint8_t *source, uint8_t *key, uint8_t size);
/*************************************IMPLEMENTATION****************************/
 /**
 * \brief This function initializes the security modules like AES, ECC608 (If used)
 *
 * \return value of type SalStatus_t
 *         SAL_SUCCESS -- when initialization is successful
 *         SAL_FAILURE -- when initialization of AES/ECC608 is failed
 */
SalStatus_t SAL_Init(void)
{
	SalStatus_t sal_status = SAL_SUCCESS;
#ifndef UT	
	/* Initialize the AES Engine */
	AESInit();
#ifdef CRYPTO_DEV_ENABLED

	ATCA_STATUS status = ATCA_SUCCESS;

	/* Initialize the communication to the ECC608.  
	   Set the I2C address */
	cfg_atecc608a_i2c_default .iface_type            = ATCA_I2C_IFACE;
	cfg_atecc608a_i2c_default.devtype                = ATECC608A;
	cfg_atecc608a_i2c_default.atcai2c.slave_address  = ATCAD_SLAVE_I2C_ADDRESS;
	cfg_atecc608a_i2c_default.atcai2c.bus            = ATCAD_HOST_I2C_SERCOM;
	cfg_atecc608a_i2c_default.atcai2c.baud           = ATCAD_HOST_I2C_BAUDRATE;
	cfg_atecc608a_i2c_default.wake_delay             = 1500;
	cfg_atecc608a_i2c_default.rx_retries             = 20;
	status = atcab_init( &cfg_atecc608a_i2c_default );

	if (ATCA_SUCCESS == status )
	{
		/* Write Key Encryption Key to KEK Slot in ECC608A */
		sal_status = sal_WriteKeyEncryptionKey();
	}
#endif	
#endif
   return sal_status;	
}

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
SalStatus_t SAL_AESEncode(unsigned char* buffer, salItems_t key_type, unsigned char* key)
{
	SalStatus_t sal_status = SAL_SUCCESS;
	
	uint8_t useKey[16];	
#ifndef CRYPTO_DEV_ENABLED // If Keys are provide by the MAC for encrypting the data
	memcpy(useKey, key, sizeof(useKey));
	/* Encrypt the block using AES (HW/SW) Engine */
	AESEncode(buffer, useKey);
	key_type = key_type;
#else // If Keys are stored inside the ECC608A device
	ATCA_STATUS atcab_status = ATCA_SUCCESS;
	/* Get the Key slot number based on the Key type parameter */
	uint8_t keySlot = keySlots[key_type];
	uint8_t encData[16];
	
	switch(key_type)
	{
		case SAL_APP_KEY:
		{
			/* If the key_type is APP Key, Encryption Should have done inside ECC608,
			 * since AppKey is not readable from it */
			atcab_status = atcab_aes_encrypt(keySlot, APP_KEY_SLOT_BLOCK, buffer, encData);
			if (atcab_status == ATCA_SUCCESS)
			{
				memcpy(buffer, encData, sizeof(encData));
				sal_status = SAL_SUCCESS;
			}
			else
			{
				sal_status = SAL_FAILURE;
			}
			
		}
		break;
		/* If Encryption is using Session Keys, Higher layer should provide key details */
		case SAL_APPS_KEY:
		case SAL_NWKS_KEY:
		case SAL_MCAST_APPS_KEY:
		case SAL_MCAST_NWKS_KEY:
		{
			memcpy(useKey, key, sizeof(useKey));
			/* Encrypt the block using AES (HW/SW) Engine */
			AESEncode(buffer, useKey);
		}
		break;
		
		default:
		{
			sal_status = SAL_INVALID_KEY_TYPE;
		}
		break;
	}
#endif	
	return sal_status;
}

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
SalStatus_t SAL_DeriveSessionKey(unsigned char* block, salItems_t src_key, unsigned char* key, salItems_t target_key)
{
	SalStatus_t sal_status = SAL_SUCCESS;
	
#ifndef CRYPTO_DEV_ENABLED // If Key is provided by the MAC layer for deriving the session keys
	sal_status = SAL_AESEncode(block, src_key, key);
#else	// If Session keys are derived inside ECC608 and to be stored inside it
	ATCA_STATUS atcad_status = ATCA_SUCCESS;
	/* Get the Key slot number based on the Key type parameter */
	uint8_t source_slot = keySlots[src_key];   // Slot of Source key which is used to derive the session key
	uint8_t target_slot = keySlots[target_key];// Slot of Target Session Key which is being derived
	uint8_t derive_mode    = KDF_MODE_ALG_AES | KDF_MODE_SOURCE_SLOT | KDF_MODE_TARGET_SLOT;
	uint16_t key_id = (target_slot << 8) | source_slot; //2 Byte key_id denotes Source and Target slot values (Target Slot(1 Byte) | Source Slot (1Byte))
	uint32_t aes_details = 0;
	if (SAL_APP_KEY == src_key)
	{
		aes_details = KDF_DETAILS_AES_KEY_LOC_MASK && (1 << APP_KEY_SLOT_BLOCK);
	}
	/** \brief Executes the KDF command, which derives a new key in AES mode
	 *
	 * This function combines a source key which is stored inside ECC608 data slot with an input string and
     * creates a result key which will be stored into the data slot
	 *
	 * \param[in]  derive_mode       Mode determines KDF algorithm (PRF,AES,HKDF), source
	 *								 key location, and target key locations.
	 * \param[in]  key_id			 Source and target key slots if locations are in the
	 *								 EEPROM. Source key slot is the LSB and target key
	 *								 slot is the MSB.
	 * \param[in]  details           Further information about the computation, depending
	 *                               on the algorithm (4 bytes).
	 * \param[in]  block           Input value from system (up to 128 bytes). Actual size
	 *                               of message is 16 bytes for AES algorithm or is encoded
	 *                               in the MSB of the details parameter for other
	 *                               algorithms.
	 * \param[out] out_data          Output of the KDF function is returned here. If the
	 *                               result remains in the device, this can be NULL.
	 * \param[out] out_nonce         If the output is encrypted, a 32 byte random nonce
	 *                               generated by the device is returned here. If output
	 *                               encryption is not used, this can be NULL.
	 *
	 * \return ATCA_SUCCESS on success, otherwise an error code.
	 */
	 atcad_status = atcab_kdf(derive_mode, key_id, aes_details, block, NULL, NULL);
	
							
	
	 if(ATCA_SUCCESS != atcad_status)
	 {
		 sal_status = SAL_FAILURE;
	 }
#endif	
	 return sal_status;
}

/**
 * \brief This function reads back the keys from ECC608 device using Encrypted Read/Plain read based on key type
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
SalStatus_t SAL_Read(salItems_t key_type, uint8_t* key)
{
	SalStatus_t sal_status = SAL_SUCCESS;
#ifdef CRYPTO_DEV_ENABLED	
	ATCA_STATUS status = ATCA_SUCCESS;
	
	/* Get the Key slot number based on the Key type parameter */	
	uint8_t keyId = keySlots[key_type];
	uint8_t block = 0;
	switch(key_type)
	{
		case SAL_NWKS_KEY:
		case SAL_APPS_KEY:
		case SAL_MCAST_APPS_KEY:
		case SAL_MCAST_NWKS_KEY:
		{
			/** \brief Executes Read command on a slot configured for encrypted reads and
			 *          decrypts the data to return it as plaintext.
			 *
			 * Data zone must be locked for this command to succeed. Can only read 32 byte
			 * blocks.
			 *
			 *  \param[in]  keyId                The slot ID to read from.
			 *  \param[in]  block                Index of the 32 byte block within the slot to read.
			 *  \param[out] key                  Decrypted (plaintext) data from the read is returned
			 *                                   here (32 bytes).
			 *  \param[in]  keyEncryptionKey     32 byte ReadKey for the slot being read.
			 *  \param[in]  KEK_SLOT             KeyID of the ReadKey being used.
			 *
			 *  returns ATCA_SUCCESS on success, otherwise an error code.
			 */
			status = atcab_read_enc(keyId, block, key, &keyEncryptionKey[0], KEK_SLOT);
			
		}
		break;
		case SAL_JOIN_EUI:
		{	
			/** \brief Used to read an arbitrary number of bytes from any zone configured
			 *          for clear reads.
			 *
			 * This function will issue the Read command as many times as is required to
			 * read the requested data.
			 *
			 *  \param[in]  zone    ATCA_ZONE_DATA - Reading App EUI which is stored in the data slot
			 *  \param[in]  key_id  Slot number to read from
			 *  \param[in]  offset  Byte offset within the zone to read from.
			 *  \param[out] key     Read data is returned here.
			 *  \param[in]  length  Number of bytes to read starting from the offset.
			 *
			 *  \return ATCA_SUCCESS on success, otherwise an error code.
			 */
			status = atcab_read_bytes_zone(ATCA_ZONE_DATA, keyId, 0, key, SAL_EUI_LEN);
		}
		break;
		case SAL_DEV_EUI:
		{
#if (SERIAL_NUM_AS_DEV_EUI == 1)
			status = atcab_read_serial_number(key);
#else
			status = atcab_read_bytes_zone(ATCA_ZONE_DATA, keyId, 0, key, SAL_EUI_LEN);
#endif
		}
		break;
		default:
			sal_status = SAL_INVALID_KEY_TYPE;
		break;
	}
	
	if (ATCA_SUCCESS != status)
	{
		sal_status = SAL_FAILURE;
	}
	
#else	
	/* Keep Compiler Happy */
	key_type = key_type;
	key = key;
#endif
    return sal_status;
}

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
SalStatus_t SAL_AESCmac(uint8_t* key, salItems_t key_type, uint8_t* output, uint8_t* input, uint16_t size)
{
	SalStatus_t sal_status = SAL_SUCCESS;
	uint16_t n = 0, i = 0, j =0;
	bool flag = false;
	uint8_t k1[16], k2[16];
	uint8_t x[16], y[16], mLast[16], padded[16];
	uint8_t *ptr = NULL;

	sal_GenerateSubkey(key, key_type, k1, k2);

	n = (size + 15) >> 4;
	if (n == 0)
	{
		n = 1;
		flag = 0;
	}
	else
	{
		flag = !(size % 16);
	}

	if ( flag == 1 )
	{
		j = 0;
		for (i=((n-1) << 4); i<(n << 4); i++)
		{
			mLast[j] = input[i] ^ k1[j];
			j++;
		}
	}
	else
	{
		// padding
		ptr = &input[size - (size%16)];
		for (i=0; i<16; i++)
		{
			if ( i < (size%16) )
			{
				padded[i] = ptr[i];
			}
			else
			{
				if ( i == (size%16) )
				{
					padded[i] = 0x80;
				}
				else
				{
					padded[i] = 0x00;
				}
			}
		}

		// XOR
		for (i=0; i<sizeof(mLast); i++)
		{
			mLast[i] = padded[i] ^ k2[i];
		}
	}

	memset(x, 0, sizeof(x));

	for (i=0; i<(n-1); i++)
	{
		for (j=0; j<16; j++)
		{
			y[j] = x[j] ^ input[(i << 4)+j];
		}
		memcpy(x, y, sizeof(y));
		SAL_AESEncode(x, key_type, key);
	}

	for (i=0; i<sizeof(x); i++)
	{
		y[i] = x[i] ^ mLast[i];
	}

	SAL_AESEncode(y, key_type, key);

	memcpy(output, y, sizeof(y));
	
	return sal_status;
}

/****************************** PRIVATE FUNCTIONS *****************************/
static void sal_GenerateSubkey (uint8_t* key, salItems_t key_type, uint8_t* k1, uint8_t* k2)
{
	uint8_t i = 0;
	uint8_t l[16];
	uint8_t const_Rb[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87};


	memset(l, 0, sizeof(l));

	SAL_AESEncode(l, key_type, key);

	// compute k1 sub-key
	if ( (l[0] & 0x80) == 0x00 )  // MSB( bufferLocal[0] ) is '0'
	{
		sal_FillSubKey( l, k1, (sizeof(l) ) );
	}
	else
	{
		sal_FillSubKey( l, k1, (sizeof(l) ) );

		for (i=0; i<sizeof(l); i++)
		{
			k1[i] ^= const_Rb[i];
		}
	}

	// compute k2 sub-key
	if ( (k1[0] & 0x80) == 0x00 )   // MSB( k1[0] ) is '0'
	{
		sal_FillSubKey( k1, k2, (sizeof(l)) );
	}
	else
	{
		sal_FillSubKey( k1, k2, (sizeof(l) ) );

		for (i=0; i<sizeof(l); i++)
		{
			k2[i] = k2[i] ^ const_Rb[i];
		}
	}
}

static void sal_FillSubKey( uint8_t *source, uint8_t *key, uint8_t size)
{
	uint8_t i = 0;
	uint8_t carry = 0;

	i=size;

	while(i--)
	{
		key[i] = (source[i] << 1) | carry;
		carry = !!(source[i] & 0x80);
	}
}

/**
 * \brief Function to generate random 32 bytes key and write that to Key Encryption Key Slot				   
 *		  This key will be used while reading the Session Keys in a Encrypted way		   
 *
 * \return value of type SalStatus_t
 *         SAL_SUCCESS			-- when Key Generation and writing to a ECC608 Slot is successful
 *         SAL_FAILURE			-- when Key Generation and writing to a ECC608 Slot is failed
 */
#ifdef CRYPTO_DEV_ENABLED
static SalStatus_t sal_WriteKeyEncryptionKey(void)
{
	SalStatus_t sal_status = SAL_SUCCESS;
	ATCA_STATUS status = ATCA_SUCCESS;
	
	/** \brief Executes Random command, which generates a 32 byte (Key Encryption Key) random number
	 *          from the CryptoAuth device.
	 *
     * \param[out] rand_out  32 bytes of random data is returned here.
     *
     * \return ATCA_SUCCESS on success, otherwise an error code.
     */
	status = atcab_random((uint8_t *)&keyEncryptionKey[0]);

	if (ATCA_SUCCESS == status)
	{
		/* Write Key Encryption Key of 32 Bytes in KEK_SLOT */
		status = atcab_write_zone(ATCA_ZONE_DATA, KEK_SLOT, 0, 0, keyEncryptionKey, 32);
	}
	
	if (ATCA_SUCCESS != status)
	{
		sal_status = SAL_FAILURE;
	}
	
	return sal_status;
}
#endif


