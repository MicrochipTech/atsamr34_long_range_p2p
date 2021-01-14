/**
* \file  system_assert.h
*
* \brief This has all the dependencies for LoRaWAN system assertion
*
* Copyright (c) 2018-2020 Microchip Technology Inc. and its subsidiaries. 
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

/************************************************************************/
/* Includes                                                             */
/************************************************************************/
#include <stdint.h>
#include <stddef.h>
/******************************************************************************/
/* Type definitions                                                            */
/******************************************************************************/
typedef enum _SystemAssertLevel_t
{
    ASSERT_INFO,
    ASSERT_ERROR,
    ASSERT_FATAL
} SystemAssertLevel_t;

typedef void(*SystemAssertCb_t)(SystemAssertLevel_t level, uint16_t assertNum);

#if (_DEBUG_ == 1)
#define SYS_ASSERT_FATAL(num)   SYSTEM_Assert(ASSERT_FATAL, num)
#define SYS_ASSERT_ERROR(num)   SYSTEM_Assert(ASSERT_ERROR, num)
#define SYS_ASSERT_INFO(num)    SYSTEM_Assert(ASSERT_INFO, num)
#else /* #if (_DEBUG_ == 1) */
#define SYS_ASSERT_FATAL(num)
#define SYS_ASSERT_ERROR(num)
#define SYS_ASSERT_INFO(num)
#endif /* #if (_DEBUG_ == 1) */

/* 4-bits - Module id, 12-bits - assert number */

/* Module ID */
#define ASSERT_APP      (0x0000)
#define ASSERT_MAC      (0x0100)
#define ASSERT_RADIO    (0x0200)
#define ASSERT_HAL      (0x0300)

/* Module specific asserts */
/* APP */
#define ASSERT_APP_GPSTIMEUNKNOWN_FAIL          (ASSERT_APP|1)
/* MAC */
#define ASSERT_MAC_RX1CALLBACK_RXFAIL           (ASSERT_MAC|1)
#define ASSERT_MAC_TXRETRY_RXSTOPFAIL           (ASSERT_MAC_RX1CALLBACK_RXFAIL+1)
#define ASSERT_MAC_AUTOREPLY_RXSTATEFAIL        (ASSERT_MAC_TXRETRY_RXSTOPFAIL+1)
#define ASSERT_MAC_UNCNFTXRETRY_RXSTATEFAIL     (ASSERT_MAC_AUTOREPLY_RXSTATEFAIL+1)
#define ASSERT_MAC_UNCNFTXRETRY_RXSTOPFAIL      (ASSERT_MAC_UNCNFTXRETRY_RXSTATEFAIL+1)
#define ASSERT_MAC_CONFIGRX2_RXFAIL             (ASSERT_MAC_UNCNFTXRETRY_RXSTOPFAIL+1)
#define ASSERT_MAC_SETEDCLASS_RXSTOPFAIL        (ASSERT_MAC_CONFIGRX2_RXFAIL+1)
#define ASSERT_MAC_PAUSE_RXSTOPFAIL             (ASSERT_MAC_SETEDCLASS_RXSTOPFAIL+1)
#define ASSERT_MAC_RXCALLBACK_RXSTOPFAIL        (ASSERT_MAC_PAUSE_RXSTOPFAIL+1)
#define ASSERT_MAC_CLASSCRX2TIMEOUT_STATEFAIL   (ASSERT_MAC_RXCALLBACK_RXSTOPFAIL+1)
#define ASSERT_MAC_CLASSCJOIN_STATEFAIL			(ASSERT_MAC_CLASSCRX2TIMEOUT_STATEFAIL+1)
/* HAL */
#define ASSERT_HAL_TIMERID_EXHAUSTED            (ASSERT_HAL|1)
#define ASSERT_HAL_TIMER_START_FAILURE          (ASSERT_HAL|2)
#define ASSERT_HAL_TIMER_STOP_FAILURE           (ASSERT_HAL|3)

/******************************************************************************/
/* Function declarations                                                         */
/******************************************************************************/
#if (_DEBUG_ == 1)
void SYSTEM_Assert(SystemAssertLevel_t level, uint16_t assertNum);
void SYSTEM_AssertSubscribe(SystemAssertCb_t cb);
#endif /* #if (_DEBUG_ == 1) */
/* eof system_assert.h */


