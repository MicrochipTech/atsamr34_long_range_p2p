/**
 * \file
 *
 * \brief SAM R34 Xplained Pro board configuration.
 *
 * Copyright (c) 2019 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_BOARD_H_INCLUDED
#define CONF_BOARD_H_INCLUDED


/*********** DIO Config **********/
/*Enable the required DIO's*/
#define ENABLE_DIO0
#define ENABLE_DIO1
#define ENABLE_DIO2
/*********************************/

/* RF Switch Enable */
#define RFSWITCH_ENABLE

/******** TCXO Changes***********/
/* Possible Radio clock sources */
#define TCXO_OSC                0
#define XTAL_OSC                1

/* Clock source for SX1276 radio */
#define RADIO_CLK_SRC        TCXO_OSC

#if (RADIO_CLK_SRC == TCXO_OSC)
#define TCXO_ENABLE
#endif

/* Delay in ms for TCXO to stabilize */
#define RADIO_CLK_STABILITATION_DELAY       2
/**********************************/

/************** RSSI Config *******/
// For freq above 862Mhz the RSSI offset to be used
#define RSSI_HF_OFFSET				-157

// For freq below 862Mhz the RSSI offset to be used
#define RSSI_LF_OFFSET				-164
/**********************************/


/* Enable if OLED1 Xplained Pro is connected */
#define EXT_BOARD_OLED1_XPLAINED_PRO

#if (defined EXT_BOARD_OLED1_XPLAINED_PRO)

#define ENABLE_LCD

#define WING_BUTTON_1 EXT1_PIN_9
#define WING_BUTTON_2 EXT1_PIN_3
#define WING_BUTTON_3 EXT1_PIN_4

#define WING_BUTTON_1_ACTIVE      false
#define WING_BUTTON_1_INACTIVE    !WING_BUTTON_1_ACTIVE
#define WING_BUTTON_1_EIC_PIN     PIN_PA22
#define WING_BUTTON_1_EIC_MUX     MUX_PA22A_EIC_EXTINT6
#define WING_BUTTON_1_EIC_PINMUX  PINMUX_PA22A_EIC_EXTINT6
#define WING_BUTTON_1_EIC_LINE    6

#define WING_BUTTON_3_ACTIVE      false
#define WING_BUTTON_3_INACTIVE    !WING_BUTTON_2_ACTIVE
#define WING_BUTTON_3_EIC_PIN     PIN_PA07
#define WING_BUTTON_3_EIC_MUX     MUX_PA07A_EIC_EXTINT7
#define WING_BUTTON_3_EIC_PINMUX  PINMUX_PA07A_EIC_EXTINT7
#define WING_BUTTON_3_EIC_LINE    7

#endif // OLED XPlainedPro Board

#if defined (ENABLE_LCD)
#include "lcd.h"
#endif

#endif /* CONF_BOARD_H_INCLUDED */
