/**
* \file  demo_output.c
*
* \brief Demo output Implementation.
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


#include "system.h"
#include "miwi_config.h"
#include "miwi_config_p2p.h"
#include "miwi_api.h"
#include "delay.h"
#include "demo_output.h"
#include "sio2host.h"
#include "asf.h" 

#define DEBOUNCE_TIME 0x00003FFF

bool PUSH_BUTTON_pressed;
MIWI_TICK PUSH_BUTTON_press_time;

void DemoOutput_Greeting(void)
{    
#if defined (ENABLE_LCD)
	#if defined(PROTOCOL_P2P)
		LCDDisplay((char *)"Simple LoRa P2P Demo on \n SAMR34 Node ", 0, true);
	#endif
#endif
    #if defined (ENABLE_CONSOLE)
        #if defined(PROTOCOL_P2P)
            printf("\r\nStarting Node 1 of Simple Demo for LoRa P2P Stack ...");  
        #endif
		#if defined(ENABLE_SLEEP_FEATURE)
			printf("\r\nReduced Function Device (RFD) operations") ;
		#else
			printf("\r\nFull Function Device (FFD) operations") ;
		#endif
        printf("\r\n     RF Transceiver: SX_1276");
        printf("\r\n     Demo Instruction:");
        printf("\r\n                     Power on the board until LED 1 lights up");
        printf("\r\n                     to indicate connecting with peer.");
        printf("\r\n                     Push Button 1 to broadcast message.");
        printf("\r\n                     LED 1 will be toggled upon receiving messages. ");
        printf("\r\n\r\n");		
    #endif 
}        

void demo_output_freezer_options(void)
{ 
#if defined (ENABLE_LCD)
	LCDDisplay((char *)"SW: Use Nwk Freezer \nPress in 5 sec", 0, false);	
	delay_ms(1000);
#endif
}
void DemoOutput_Channel(uint8_t channel, uint8_t Step)
{
    if( Step == 0 )
    {
#if defined (ENABLE_LCD)       
        LCDDisplay((char *)"Connecting Peer on \n Channel ", channel, true);
#endif
#if defined (ENABLE_CONSOLE)
        #if !defined(MIWIKIT)
        printf("\r\nConnecting Peer on Channel ");
        printf("%d",channel);
        printf("\r\n");
        #endif
#endif
    }
    else
    { 
#if defined (ENABLE_LCD)
        LCDDisplay((char *)"Connected Peer on \n Channel ", channel, true);
#endif
#if defined (ENABLE_CONSOLE)
        #if !defined(MIWIKIT)
        printf("\r\nConnected Peer on Channel ");
        printf("%d",channel);
        printf("\r\n");
        #endif
#endif
    }
}    

void DemoOutput_Instruction(void)
{
#if defined (ENABLE_LCD)
	#if defined (EXT_BOARD_OLED1_XPLAINED_PRO)
		LCDDisplay((char *)"SW     : Broadcast \nBUTTON1: Unicast", 0, false);
	#else
		LCDDisplay((char *)"SW: Broadcast", 0, false);
	#endif
#endif
}


void DemoOutput_HandleMessage(void)
{
    uint8_t i;

    if( rxMessage.flags.bits.secEn )
    {
        sio2host_tx((uint8_t *)"Secured ", sizeof("Secured "));
    }

    if( rxMessage.flags.bits.broadcast )
    {
        sio2host_tx((uint8_t *)"Broadcast Packet with RSSI ", sizeof("Broadcast Packet with RSSI "));
    }
    else
    {
        sio2host_tx((uint8_t *)"Unicast Packet with RSSI ",sizeof("Unicast Packet with RSSI "));
    }
    printf("%02x", rxMessage.PacketRSSI);
    if( rxMessage.flags.bits.srcPrsnt )
    {
        sio2host_tx((uint8_t *)" from ", sizeof(" from "));
        if( rxMessage.flags.bits.altSrcAddr )
        {
            printf( "%x", rxMessage.SourceAddress[1]);
            printf( "%x", rxMessage.SourceAddress[0]);
        }
        else
        {    
            for(i = 0; i < MY_ADDRESS_LENGTH; i++)
            {
                printf("%x", rxMessage.SourceAddress[MY_ADDRESS_LENGTH-1-i]);
            }    
        }
    }
    sio2host_tx((uint8_t *)": ",sizeof(": "));
    
    for(i = 0; i < rxMessage.PayloadSize; i++)
    {
        sio2host_putchar(rxMessage.Payload[i]);
    }   
	printf("\r\n");    
} 

void DemoOutput_UpdateTxRx(uint8_t TxNum, uint8_t RxNum)
{
#if defined (ENABLE_LCD)
    LCDTRXCount(TxNum, RxNum);  
#endif
}

void DemoOutput_ChannelError(uint8_t channel)
{
    #if defined (ENABLE_CONSOLE)
        printf("\r\nSelection of channel ");
        printf("%d", channel);
        printf(" is not supported in current configuration.\r\n");
    #endif
}

void DemoOutput_UnicastFail(void)
{
    #if defined (ENABLE_CONSOLE)
        printf("\r\nUnicast Failed\r\n");
    #endif
#if defined (ENABLE_LCD)
    LCDDisplay((char *)" Unicast Failed", 0, true);
#endif
}    

void DemoOutput_BroadcastFail(void)
{
	#if defined (ENABLE_CONSOLE)
	printf("\r\nBroadcast Failed\r\n");
	#endif
	#if defined (ENABLE_LCD)
	LCDDisplay((char *)" Broadcast Failed", 0, true);
	#endif
}    

/*********************************************************************
 * Function:        uint8_t ButtonPressed(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          Return value to indicate which button has been pressed.
 *                  Return 0 if no button pressed.
 *
 * Side Effects:
 *
 * Overview:        This function check if any button has been pressed
 *
 * Note:
 ********************************************************************/

uint8_t ButtonPressed(void)
{
    MIWI_TICK tickDifference;
    if(!port_pin_get_input_level(BUTTON_0_PIN))
    {
        //if the button was previously not pressed
        if(PUSH_BUTTON_pressed == false)
        {
            PUSH_BUTTON_pressed = true;
            PUSH_BUTTON_press_time.Val = MiWi_TickGet();
            return 1;
        }
    }
#if defined (EXT_BOARD_OLED1_XPLAINED_PRO)
    else if(!port_pin_get_input_level(WING_BUTTON_1))
    {
	    //if the button was previously not pressed
	    if(PUSH_BUTTON_pressed == false)
	    {
		    PUSH_BUTTON_pressed = true;
		    PUSH_BUTTON_press_time.Val = MiWi_TickGet();
		    return 2;
	    }
    }
#endif	
    else
    {
        //get the current time
        MIWI_TICK t;
		t.Val = MiWi_TickGet();

        //if the button has been released long enough
        tickDifference.Val = MiWi_TickGetDiff(t,PUSH_BUTTON_press_time);

        //then we can mark it as not pressed
        if(tickDifference.Val > DEBOUNCE_TIME)
        {
            PUSH_BUTTON_pressed = false;
        }
    }
    return 0;
}

#if (defined EXT_BOARD_OLED1_XPLAINED_PRO)
/**
 * \brief Configures buttons
 */
void Buttons_init(void)
{
	struct port_config conf;
	port_get_config_defaults(&conf);

	conf.direction = PORT_PIN_DIR_INPUT;
	conf.input_pull = PORT_PIN_PULL_UP;

	/* Configure all three buttons as inputs */
	port_pin_set_config(WING_BUTTON_1, &conf);
	port_pin_set_config(WING_BUTTON_2, &conf);
	port_pin_set_config(WING_BUTTON_3, &conf);
}
#endif



