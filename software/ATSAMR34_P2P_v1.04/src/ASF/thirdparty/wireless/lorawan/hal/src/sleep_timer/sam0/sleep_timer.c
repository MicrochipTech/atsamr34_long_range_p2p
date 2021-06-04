/**
* \file  sleep_timer.c
*
* \brief Sleep timer implementation
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
 

/**************************************** INCLUDES*****************************/
#include "sleep_timer.h"
#include <rtc_count.h>
#include <rtc_count_interrupt.h>

#ifdef CONF_PMM_ENABLE
/**************************************** EXTERNS ****************************/
struct rtc_module rtc;

/************************************** IMPLEMENTATION************************/
/**
* \brief Initializes the sleep timer module
*/
void SleepTimerInit(void)
{
	struct rtc_count_config rtc_config;
	rtc_count_get_config_defaults(&rtc_config);
	
	rtc_config.prescaler = RTC_COUNT_PRESCALER_OFF;
	rtc_config.enable_read_sync = true;
	rtc_config.compare_values[0] = COMPARE_COUNT_MAX_VALUE;
	rtc_config.compare_values[1] = COMPARE_COUNT_MAX_VALUE;
	rtc_count_init(&rtc, RTC, &rtc_config);
	rtc_count_enable(&rtc);
}

/**
* \brief Calculate the Elapsed Time from the previous call of this function
* \retval Elapsed time in ticks
*/
uint32_t SleepTimerGetElapsedTime(void)
{
	return rtc_count_get_count(&rtc);
}

/**
* \brief Initializes the sleep timer
*/
void SleepTimerStart(uint32_t sleepTicks, void (*cb)(void))
{
	rtc_count_set_count(&rtc, 0);
	rtc_count_register_callback(&rtc, cb, RTC_COUNT_CALLBACK_COMPARE_0);
	rtc_count_set_compare(&rtc, sleepTicks, RTC_COUNT_COMPARE_0);
	rtc_count_enable_callback(&rtc, RTC_COUNT_CALLBACK_COMPARE_0);
}

/**
* \brief Stop the sleep timer
*/
void SleepTimerStop(void)
{
	rtc_count_disable_callback(&rtc, RTC_COUNT_CALLBACK_COMPARE_0);
}

#endif /* CONF_PMM_ENABLE */

/* eof sleep_timer.c */
