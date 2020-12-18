/**
* \file  main.c
*
* \brief Main file of LoRa Simple Example P2P.
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

/************************ HEADERS ****************************************/
#include "task.h"
#include "asf.h"
#include "sio2host.h"

#if defined(ENABLE_NETWORK_FREEZER)
#include "pdsDataServer.h"
#include "wlPdsTaskManager.h"
#endif

#include "system_task_manager.h"
#include "system_init.h"
#include "sw_timer.h"
#if (BOARD == SAMR34_XPLAINED_PRO)
#include "edbg-eui.h"
#endif

/************************** DEFINITIONS **********************************/

/************************** PROTOTYPES **********************************/
void ReadMacAddress(void);
SYSTEM_TaskStatus_t APP_TaskHandler(void);

int main ( void )
{   
    bool freezer_enable = false;
	irq_initialize_vectors();
	
	system_init();
	delay_init();

	cpu_irq_enable();
	
#if defined (ENABLE_LCD)	
	LCD_Initialize();
#endif

#if defined (ENABLE_CONSOLE)
	sio2host_init();
#endif
	
	// Permanent address for the device is set in miwi_config.h and loaded in global variable myLongAddress
	// Define SYMBOL MACRO to populate myLongAddress from EDBG_EUI or from MODULE_EUI
#if (EDBG_EUI_READ == 1 || MODULE_EUI_READ == 1)
	ReadMacAddress(); 
#endif

	SystemTimerInit();

    // Demo Start Message 
    DemoOutput_Greeting();	

#if (defined EXT_BOARD_OLED1_XPLAINED_PRO)
    Buttons_init();
#endif
    
#if defined(ENABLE_NETWORK_FREEZER)
    nvm_init(INT_FLASH);
    PDS_Init();
    demo_output_freezer_options();
    // User Selection to commission a network or use Freezer
    freezer_enable = freezer_feature();
#endif
    // Commission the network 
    Initialize_Demo(freezer_enable);
    
	/* Initialize demo application */
    Stack_Init();

    while(1)
    {
		SYSTEM_RunTasks();
		Run_Demo();
    }
}

/*********************************************************************//*
 \brief      Application Task Handler
 ************************************************************************/
SYSTEM_TaskStatus_t APP_TaskHandler(void)
{
    Run_Demo();
    return SYSTEM_TASK_SUCCESS;
}

/*********************************************************************
* Function:         void ReadMacAddress()
*
* PreCondition:     none
*
* Input:		    none
*
* Output:		    Reads MAC Address from SAM R34 XPRO EDBG or from 
*					WLR089U0 Internal Flash
* Side Effects:	    none
*
* Overview:		    Uses the MAC Address for addressing
*
* Note:			    
**********************************************************************/
void ReadMacAddress(void)
{
#if (BOARD == SAMR34_XPLAINED_PRO && defined(__SAMR34J18B__))
	// only applicable for SAM R34 Xpro with EDBG on-board
	uint8_t* peui64 = edbg_eui_read_eui64() ;
	for (uint8_t i = 0; i < MY_ADDRESS_LENGTH; i++)
	{
		myLongAddress[i] = peui64[MY_ADDRESS_LENGTH-i-1] ;
	}
#elif (defined(__WLR089U0__))
	// applicable for module with WLR089 Xpro board or custom board
	#define NVM_UID_ADDRESS   ((volatile uint16_t *)(0x0080400AU))
	uint8_t i = 0, j = 0 ;
	uint8_t peui64[8] ;
	for (i = 0; i < MY_ADDRESS_LENGTH; i += 2, j++)
	{
		peui64[i] = (NVM_UID_ADDRESS[j] & 0xFF) ;
		peui64[i + 1] = (NVM_UID_ADDRESS[j] >> 8) ;
	}
	for (i= 0; i < MY_ADDRESS_LENGTH; i++)
	{
		myLongAddress[i] = peui64[MY_ADDRESS_LENGTH-i-1] ;
	}
#endif
}
