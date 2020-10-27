/*
 * @file trx_access.c
 *
 * @brief Performs interface functionalities between the PHY layer and ASF
 * drivers
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
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
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
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
 *
 */

/*
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

#include "board.h"
#if SAMD || SAMR21 || SAML21 || SAMR30 || SAMR34 || (WLR089)
#include "spi.h"
#else
#include "spi_master.h"
#endif

#include "trx_access_1.h"

#include "delay.h"
#include "interrupt.h"
#include "conf_board.h"
#include "asf.h"

#include "radio_driver_hal.h" 


static irq_handler_t irq_hdl_trx = NULL;

#if SAMD || SAMR21 || SAML21 || SAMR30  || SAMR34 || (WLR089)
struct spi_slave_inst_config slave_dev_config;
struct spi_config config;
struct spi_module master;
struct spi_slave_inst slave;
uint16_t dummy_read;

#endif

#if SAMD || SAMR21 || SAML21 || SAMR30 || SAMR34 || (WLR089)
void AT86RFX_ISR(void);

void AT86RFX_ISR(void)
#else
AT86RFX_ISR()
#endif

{
	/*Clearing the RF interrupt*/
	trx_irq_flag_clr();
  	/*Calling the interrupt routines*/
	if (irq_hdl_trx) {
		irq_hdl_trx();
	}
}

void trx_spi_init(void)
{
	/* Initialize SPI in master mode to access the transceiver */
    // do nothing....

}

void PhyReset(void)
{
	RADIO_Reset(); 
}

uint8_t trx_reg_read(uint8_t addr)
{
#if SAMD || SAMR21 || SAML21 || SAMR30  || SAMR34 || (WLR089)
	uint16_t register_value = 0;
#else
	uint8_t register_value = 0;
#endif

	/*Saving the current interrupt status & disabling the global interrupt
	**/
	ENTER_TRX_CRITICAL_REGION();

	/* Prepare the command byte */
	addr |= READ_ACCESS_COMMAND;

	register_value = RADIO_RegisterRead(addr); 

	/*Restoring the interrupt status which was stored & enabling the global
	 * interrupt */
	LEAVE_TRX_CRITICAL_REGION();

	return register_value;
}

void trx_reg_write(uint8_t addr, uint8_t data)
{
	/*Saving the current interrupt status & disabling the global interrupt
	**/
	ENTER_TRX_CRITICAL_REGION();

	RADIO_RegisterWrite(addr, data); 

	/*Restoring the interrupt status which was stored & enabling the global
	 * interrupt */
	LEAVE_TRX_CRITICAL_REGION();
}

void trx_irq_init(FUNC_PTR trx_irq_cb)
{
	/*
	 * Set the handler function.
	 * The handler is set before enabling the interrupt to prepare for
	 * spurious
	 * interrupts, that can pop up the moment they are enabled
	 */
	irq_hdl_trx = (irq_handler_t)trx_irq_cb;
}

uint8_t trx_bit_read(uint8_t addr, uint8_t mask, uint8_t pos)
{
	uint8_t ret;
	ret = trx_reg_read(addr);
	ret &= mask;
	ret >>= pos;
	return ret;
}

void trx_bit_write(uint8_t reg_addr, uint8_t mask, uint8_t pos,
		uint8_t new_value)
{
	uint8_t current_reg_value;
	current_reg_value = trx_reg_read(reg_addr);
	current_reg_value &= ~mask;
	new_value <<= pos;
	new_value &= mask;
	new_value |= current_reg_value;
	trx_reg_write(reg_addr, new_value);
}

void trx_frame_read(uint8_t *data, uint8_t length)
{
	/*Saving the current interrupt status & disabling the global interrupt
	**/
	ENTER_TRX_CRITICAL_REGION();

	RADIO_FrameRead(0,data, length); 

	/*Restoring the interrupt status which was stored & enabling the global
	 * interrupt */
	LEAVE_TRX_CRITICAL_REGION();
}

void trx_frame_write(uint8_t *data, uint8_t length)
{

	/*Saving the current interrupt status & disabling the global interrupt
	**/
	ENTER_TRX_CRITICAL_REGION();

	RADIO_FrameWrite(0, data,  length); 

	/*Restoring the interrupt status which was stored & enabling the global
	 * interrupt */
	LEAVE_TRX_CRITICAL_REGION();
}

/**
 * @brief Writes data into SRAM of the transceiver
 *
 * This function writes data into the SRAM of the transceiver
 *
 * @param addr Start address in the SRAM for the write operation
 * @param data Pointer to the data to be written into SRAM
 * @param length Number of bytes to be written into SRAM
 */
void trx_sram_write(uint8_t addr, uint8_t *data, uint8_t length)
{
	uint8_t temp;

	/*Saving the current interrupt status & disabling the global interrupt
	**/
	ENTER_TRX_CRITICAL_REGION();

#if SAMD || SAMR21 || SAML21 || SAMR30  || SAMR34 || (WLR089)
	/* Start SPI transaction by pulling SEL low */
	spi_select_slave(&master, &slave, true);

	/* Send the command byte */
	temp = TRX_CMD_SW;

	/* Send the command byte */
	while (!spi_is_ready_to_write(&master)) {
	}
	spi_write(&master, temp);
	while (!spi_is_write_complete(&master)) {
	}
	/* Dummy read since SPI RX is double buffered */
	while (!spi_is_ready_to_read(&master)) {
	}
	spi_read(&master, &dummy_read);

	/* Send the address from which the write operation should start */
	while (!spi_is_ready_to_write(&master)) {
	}
	spi_write(&master, addr);
	while (!spi_is_write_complete(&master)) {
	}
	/* Dummy read since SPI RX is double buffered */
	while (!spi_is_ready_to_read(&master)) {
	}
	spi_read(&master, &dummy_read);

	while (length--) {
		while (!spi_is_ready_to_write(&master)) {
		}
		spi_write(&master, *data++);
		while (!spi_is_write_complete(&master)) {
		}
		/* Dummy read since SPI RX is double buffered */
		while (!spi_is_ready_to_read(&master)) {
		}
		spi_read(&master, &dummy_read);
	}

	/* Stop the SPI transaction by setting SEL high */
	spi_select_slave(&master, &slave, false);
#else
	/* Start SPI transaction by pulling SEL low */
	spi_select_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);

	/* Send the command byte */
	temp = TRX_CMD_SW;

	/* Send the command byte */
	spi_write_packet(AT86RFX_SPI, &temp, 1);
	while (!spi_is_tx_empty(AT86RFX_SPI)) {
	}

	/* Send the address from which the write operation should start */
	spi_write_packet(AT86RFX_SPI, &addr, 1);
	while (!spi_is_tx_empty(AT86RFX_SPI)) {
	}

	spi_write_packet(AT86RFX_SPI, data, length);

	/* Stop the SPI transaction by setting SEL high */
	spi_deselect_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);
#endif

	/*Restoring the interrupt status which was stored & enabling the global
	 * interrupt */
	LEAVE_TRX_CRITICAL_REGION();
}

/**
 * @brief Reads data from SRAM of the transceiver
 *
 * This function reads from the SRAM of the transceiver
 *
 * @param[in] addr Start address in SRAM for read operation
 * @param[out] data Pointer to the location where data stored
 * @param[in] length Number of bytes to be read from SRAM
 */
void trx_sram_read(uint8_t addr, uint8_t *data, uint8_t length)
{
	delay_us(1); /* wap_rf4ce */

	/*Saving the current interrupt status & disabling the global interrupt
	**/
	ENTER_TRX_CRITICAL_REGION();
#if SAMD || SAMR21 || SAML21 || SAMR30  || SAMR34 || (WLR089)
	uint16_t temp;
	/* Start SPI transaction by pulling SEL low */
	spi_select_slave(&master, &slave, true);

	temp = TRX_CMD_SR;

	/* Send the command byte */
	while (!spi_is_ready_to_write(&master)) {
	}
	spi_write(&master, temp);
	while (!spi_is_write_complete(&master)) {
	}
	/* Dummy read since SPI RX is double buffered */
	while (!spi_is_ready_to_read(&master)) {
	}
	spi_read(&master, &dummy_read);

	/* Send the address from which the read operation should start */
	while (!spi_is_ready_to_write(&master)) {
	}
	spi_write(&master, addr);
	while (!spi_is_write_complete(&master)) {
	}
	/* Dummy read since SPI RX is double buffered */
	while (!spi_is_ready_to_read(&master)) {
	}
	spi_read(&master, &dummy_read);

	/* Upload the received byte in the user provided location */
	while (length--) {
		while (!spi_is_ready_to_write(&master)) {
		}
		spi_write(&master, 0);
		while (!spi_is_write_complete(&master)) {
		}
		/* Dummy read since SPI RX is double buffered */
		while (!spi_is_ready_to_read(&master)) {
		}
		spi_read(&master, &temp);
		*data = temp;
		data++;
	}

	/* Stop the SPI transaction by setting SEL high */
	spi_select_slave(&master, &slave, false);
#else
	uint8_t temp;
	/* Start SPI transaction by pulling SEL low */
	spi_select_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);

	temp = TRX_CMD_SR;

	/* Send the command byte */
	spi_write_packet(AT86RFX_SPI, &temp, 1);
	while (!spi_is_tx_empty(AT86RFX_SPI)) {
	}

	/* Send the command byte */
	spi_write_packet(AT86RFX_SPI, &addr, 1);
	while (!spi_is_tx_empty(AT86RFX_SPI)) {
	}

	/* Send the address from which the read operation should start */
	/* Upload the received byte in the user provided location */
	spi_read_packet(AT86RFX_SPI, data, length);

	/* Stop the SPI transaction by setting SEL high */
	spi_deselect_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);
#endif

	/*Restoring the interrupt status which was stored & enabling the global
	 * interrupt */
	LEAVE_TRX_CRITICAL_REGION();
}

/**
 * @brief Writes and reads data into/from SRAM of the transceiver
 *
 * This function writes data into the SRAM of the transceiver and
 * simultaneously reads the bytes.
 *
 * @param addr Start address in the SRAM for the write operation
 * @param idata Pointer to the data written/read into/from SRAM
 * @param length Number of bytes written/read into/from SRAM
 */
void trx_aes_wrrd(uint8_t addr, uint8_t *idata, uint8_t length)
{
	
#if SAMR34 || (WLR089)
// do nothing ....  use sal api from lora
#else

	uint8_t *odata;
#if SAMD || SAMR21 || SAML21 || SAMR30   || SAMR34 || (WLR089)
	uint16_t odata_var = 0;
#endif
	uint8_t temp;

	delay_us(1); /* wap_rf4ce */

	ENTER_TRX_REGION();

#ifdef NON_BLOCKING_SPI
	while (spi_state != SPI_IDLE) {
		/* wait until SPI gets available */
	}
#endif
#if SAMD || SAMR21 || SAML21 || SAMR30  || SAMR34 || (WLR089)
	/* Start SPI transaction by pulling SEL low */
	spi_select_slave(&master, &slave, true);

	/* Send the command byte */
	temp = TRX_CMD_SW;

	while (!spi_is_ready_to_write(&master)) {
	}
	spi_write(&master, temp);
	while (!spi_is_write_complete(&master)) {
	}
	/* Dummy read since SPI RX is double buffered */
	while (!spi_is_ready_to_read(&master)) {
	}
	spi_read(&master, &dummy_read);

	/* write SRAM start address */
	while (!spi_is_ready_to_write(&master)) {
	}
	spi_write(&master, addr);
	while (!spi_is_write_complete(&master)) {
	}
	/* Dummy read since SPI RX is double buffered */
	while (!spi_is_ready_to_read(&master)) {
	}
	spi_read(&master, &dummy_read);

	/* now transfer data */
	odata = idata;

	/* write data byte 0 - the obtained value in SPDR is meaningless */
	while (!spi_is_ready_to_write(&master)) {
	}
	spi_write(&master, *idata++);
	while (!spi_is_write_complete(&master)) {
	}
	/* Dummy read since SPI RX is double buffered */
	while (!spi_is_ready_to_read(&master)) {
	}
	spi_read(&master, &dummy_read);

	/* Reading Spi Data for the length specified */
	while (length > 0) {
		while (!spi_is_ready_to_write(&master)) {
		}
		spi_write(&master, *idata++);
		while (!spi_is_write_complete(&master)) {
		}
		while (!spi_is_ready_to_read(&master)) {
		}

#if SAMD || SAMR21 || SAML21 || SAMR30  || SAMR34 || (WLR089)
		spi_read(&master, &odata_var);
		*odata++ = (uint8_t)odata_var;
#else
		spi_read(&master, (uint16_t *)odata++);
#endif
		length--;
	}

	/* To get the last data byte, write some dummy byte */
	while (!spi_is_ready_to_write(&master)) {
	}
	spi_write(&master, 0);
	while (!spi_is_write_complete(&master)) {
	}
	while (!spi_is_ready_to_read(&master)) {
	}
#if SAMD || SAMR21 || SAML21 || SAMR30  || SAMR34 || (WLR089)
	spi_read(&master, &odata_var);
	*odata = (uint8_t)odata_var;
#else
	spi_read(&master, (uint16_t *)odata);
#endif

	/* Stop the SPI transaction by setting SEL high */
	spi_select_slave(&master, &slave, false);
#else
	/* Start SPI transaction by pulling SEL low */
	spi_select_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);

	/* Send the command byte */
	temp = TRX_CMD_SW;
	spi_write_packet(AT86RFX_SPI, &temp, 1);
	while (!spi_is_tx_empty(AT86RFX_SPI)) {
	}

	/* write SRAM start address */
	spi_write_packet(AT86RFX_SPI, &addr, 1);
	while (!spi_is_tx_empty(AT86RFX_SPI)) {
	}

	/* now transfer data */
	odata = idata;

	/* write data byte 0 - the obtained value in SPDR is meaningless */
	spi_write_packet(AT86RFX_SPI, idata++, 1);

	/* Reading Spi Data for the length specified */
	while (length > 0) {
		spi_write_packet(AT86RFX_SPI, idata++, 1);
		while (!spi_is_tx_empty(AT86RFX_SPI)) {
		}
		spi_read_single(AT86RFX_SPI, odata++);
		length--;
	}

	/* To get the last data byte, write some dummy byte */
	spi_read_packet(AT86RFX_SPI, odata, 1);

	/* Stop the SPI transaction by setting SEL high */
	spi_deselect_device(AT86RFX_SPI, &SPI_AT86RFX_DEVICE);
#endif
	LEAVE_TRX_REGION();
#endif 
}
