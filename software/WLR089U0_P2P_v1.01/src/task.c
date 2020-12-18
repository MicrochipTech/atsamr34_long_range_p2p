/**
* \file  task.c
*
* \brief Implementation of Tasks for LoRa Demo Application on MiWi P2P 
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

/***********************Headers***************************************/
#include "task.h"
#include "miwi_api.h"
#include "p2p_demo.h"
#include "asf.h"

#if defined(ENABLE_SLEEP_FEATURE)
#include "sleep_mgr.h"
#endif
#include "sw_timer.h"
#include "phy.h"
/************************** VARIABLES ************************************/
#define LIGHT   0x01
#define SWITCH  0x02

// Software Timer used for Tx timeout
uint8_t TxTimerId = 0;

#if defined(PERIODIC_TX)
uint8_t PeriodicTxTimerId = 0xFF ;
#endif

/*************************************************************************/
// AdditionalNodeID variable array defines the additional 
// information to identify a device on a PAN. This array
// will be transmitted when initiate the connection between 
// the two devices. This  variable array will be stored in 
// the Connection Entry structure of the partner device. The 
// size of this array is ADDITIONAL_NODE_ID_SIZE, defined in 
// miwi_config.h.
// In this demo, this variable array is set to be empty.
/*************************************************************************/
#if ADDITIONAL_NODE_ID_SIZE > 0
    uint8_t AdditionalNodeID[ADDITIONAL_NODE_ID_SIZE] = {LIGHT};
#endif

/*************************************************************************/
// The variable myChannel defines the channel that the device
// is operate on. This variable will be only effective if energy scan
// (ENABLE_ED_SCAN) is not turned on. Once the energy scan is turned
// on, the operating channel will be one of the channels available with
// least amount of energy (or noise).
/*************************************************************************/
uint8_t myChannel = 1;
/* Range for default configuration: 1 to 10
 Note: TX Power and PHY Mode Setting needs to be modified as per the 
 recommendation from Data Sheet for European band (ie.,Channel 0)*/

static void Connection_Confirm(miwi_status_t status);

#ifdef ENABLE_ACTIVE_SCAN	
void Scan_Confirm(uint8_t ActiveScanResultCount, void* PtrActiveScanResults);
#endif // #ifdef ENABLE_ACTIVE_SCAN	
bool freezer_feature(void)
{
    MIWI_TICK tick1, tick2;
    uint8_t switch_val;
    tick1.Val = MiWi_TickGet();
    while(1)
    {
        tick2.Val = MiWi_TickGet();
        if(MiWi_TickGetDiff(tick2, tick1) > (ONE_SECOND * 4))
            break;
        switch_val = ButtonPressed ();
        if(switch_val == 1)
        {
#if defined (ENABLE_LCD)
            LCDDisplay((char *)"Restoring Network !!", 0, false);
			delay_ms(1000);
#endif
            return true;
        }
        else
        {
            return false;
        }

    }
	return false;
}

#ifdef ENABLE_ACTIVE_SCAN	

void Scan_Confirm(uint8_t ActiveScanResultCount, void* PtrActiveScanResults)
{
	ACTIVE_SCAN_RESULT* ActiveScanResult = (ACTIVE_SCAN_RESULT*) PtrActiveScanResults;
#if defined (ENABLE_CONSOLE)	
	for(uint8_t count =0; count <= ActiveScanResultCount-1; count++ )
	{
		printf("\nID\tDevice Address \t\tCapability \tPAN ID \tChannel \n");
		printf("%02x",count+1);
		printf("\t");
		for(uint8_t j = 0; j < 8; j++)
		{
			if( j < MY_ADDRESS_LENGTH )
			{
				printf("%02x",ActiveScanResult[count].Address[MY_ADDRESS_LENGTH-1-j] );
			}
			else
			{
				printf("  ");
			}
		}
		printf("\t");
		printf("%02x",(ActiveScanResult[count].Capability.Val));	
		printf("\t\t");
		printf("%02x",(ActiveScanResult[count].PANID.Val));
		printf("\t");
		printf("%02x",ActiveScanResult[count].Channel);
		printf("\r\n");
		
	} 
	printf("\r\n Scan Operation Status: %d device found",ActiveScanResultCount); 
#endif // #if defined (ENABLE_CONSOLE)
}
#endif // #ifdef ENABLE_ACTIVE_SCAN	

static void Connection_Confirm(miwi_status_t status)
{
#if defined (ENABLE_CONSOLE)	
	printf("\r\nConnect Operation Status: %d\n", status) ;
#endif // #if defined (ENABLE_CONSOLE)

    if ((SUCCESS == status) || (ALREADY_EXISTS == status))
    {
#if defined (ENABLE_CONSOLE)
		printf("\r\nStarted Wireless Communication on Channel %u\r\n", currentChannel) ;
		//DumpConnection(0xFF) ;
#endif // #if defined (ENABLE_CONSOLE)

#if defined(PERIODIC_TX)
		SwTimerCreate(&PeriodicTxTimerId) ;
		SwTimerStart (PeriodicTxTimerId, MS_TO_US(1000), 0/*SW_TIMEOUT_RELATIVE*/, (void *)PeriodicTxCallback, NULL) ;
#endif // #if defined(PERIODIC_TX)
	}
}

bool Initialize_Demo(bool freezer_enable)
{
    uint8_t i;

	bool invalidIEEEAddrFlag = false;
	uint64_t invalidIEEEAddr;

    MiApp_SubscribeDataIndicationCallback(ReceivedDataIndication);

#ifdef ENABLE_SLEEP_FEATURE
	sm_init();
#endif

    if (freezer_enable)
    {
        MiApp_ProtocolInit(NULL, NULL);
		srand(PHY_RandomReq());
		/* Check if a valid IEEE address is available.
		0x0000000000000000 and 0xFFFFFFFFFFFFFFFF is presumed to be invalid */
		/* Check if IEEE address is 0x0000000000000000 */
		memset((uint8_t *)&invalidIEEEAddr, 0x00, LONG_ADDR_LEN);
		if (0 == memcmp((uint8_t *)&invalidIEEEAddr, (uint8_t *)&myLongAddress, LONG_ADDR_LEN))
		{
			invalidIEEEAddrFlag = true;
		}

		/* Check if IEEE address is 0xFFFFFFFFFFFFFFFF */
		memset((uint8_t *)&invalidIEEEAddr, 0xFF, LONG_ADDR_LEN);
		if (0 == memcmp((uint8_t *)&invalidIEEEAddr, (uint8_t *)&myLongAddress, LONG_ADDR_LEN))
		{
			invalidIEEEAddrFlag = true;
		}
		
		if (invalidIEEEAddrFlag)
		{
			/*
			 * In case no valid IEEE address is available, a random
			 * IEEE address will be generated to be able to run the
			 * applications for demonstration purposes.
			 * In production code this can be omitted.
			 */
			uint8_t* peui64 = (uint8_t *)&myLongAddress;
			for(i = 0; i<MY_ADDRESS_LENGTH; i++)
			{
				*peui64++ = (uint8_t)rand();
			}
		}
		PHY_SetIEEEAddr((uint8_t *)&myLongAddress);
#if defined(PROTOCOL_P2P)  
		DemoOutput_Instruction();
#endif
    }
    else
    {
		LED_Off(LED0);

        MiApp_ProtocolInit(NULL, NULL);
		
/*		
		// -- For reference: set RF parameters just after protocol initialization
		// Set Tx Output Power
		uint8_t txPower = 0x1F ;
		PHY_SetTxPower(txPower) ;
		// Read back and print Tx output power
		txPower = 255 ;
		RADIO_GetAttr(OUTPUT_POWER, (void *)&txPower) ;
		printf("\nCurrent Tx output power: %d\r\n", txPower) ;
		// Read and print PA_BOOST status
		uint8_t paBoost = 255 ;
		RADIO_GetAttr(PABOOST, (void *)&paBoost) ;
		printf("PA BOOST status: %d\r\n", paBoost) ;
		// Read and print spreading factor
		RadioDataRate_t sf = 33 ;
		PHY_GetAttribute(SPREADING_FACTOR, (void *)&sf) ;
		printf("Current SF: SF%d\r\n", sf) ;
		// Read and print bandwidth
		RadioLoRaBandWidth_t bw ;
		PHY_GetAttribute(BANDWIDTH, (void *)&bw) ;
		printf("Current BW (125kHz=7, 250kHz=8, 500kHz=9): %d\r\n", bw) ;
		// Modify spreading factor
		sf = SF_12 ;
		printf("Setting SF%d\r\n", sf) ;
		RadioError_t ret = PHY_SetAttribute(SPREADING_FACTOR, (void *)&sf) ;
		if (ret == ERR_NONE)
		{
			printf("Successfully set attribute to SF%d\r\n", sf) ;
		}
		else
		{
			printf("Error to write SF (error %d)\r\n", ret) ;
		}
		// Read back spreading factor
		PHY_GetAttribute(SPREADING_FACTOR, (void *)&sf) ;
		printf("Current SF%d\r\n", sf) ;
*/

		srand(PHY_RandomReq());
		/* Check if a valid IEEE address is available.
		0x0000000000000000 and 0xFFFFFFFFFFFFFFFF is presumed to be invalid */
		/* Check if IEEE address is 0x0000000000000000 */
		memset((uint8_t *)&invalidIEEEAddr, 0x00, LONG_ADDR_LEN);
		if (0 == memcmp((uint8_t *)&invalidIEEEAddr, (uint8_t *)&myLongAddress, LONG_ADDR_LEN))
		{
			invalidIEEEAddrFlag = true;
		}

		/* Check if IEEE address is 0xFFFFFFFFFFFFFFFF */
		memset((uint8_t *)&invalidIEEEAddr, 0xFF, LONG_ADDR_LEN);
		if (0 == memcmp((uint8_t *)&invalidIEEEAddr, (uint8_t *)&myLongAddress, LONG_ADDR_LEN))
		{
			invalidIEEEAddrFlag = true;
		}
		
		if (invalidIEEEAddrFlag)
		{
			/*
			 * In case no valid IEEE address is available, a random
			 * IEEE address will be generated to be able to run the
			 * applications for demonstration purposes.
			 * In production code this can be omitted.
			 */
			uint8_t* peui64 = (uint8_t *)&myLongAddress;
			for(i = 0; i<MY_ADDRESS_LENGTH; i++)
			{
				*peui64++ = (uint8_t)rand();
			}
		}
		PHY_SetIEEEAddr((uint8_t *)&myLongAddress);
        // Set default channel

        /*******************************************************************/
        // Function MiApp_ConnectionMode defines the connection mode. The
        // possible connection modes are:
        //  ENABLE_ALL_CONN:    Enable all kinds of connection
        //  ENABLE_PREV_CONN:   Only allow connection already exists in 
        //                      connection table
        //  ENABL_ACTIVE_SCAN_RSP:  Allow response to Active scan
        //  DISABLE_ALL_CONN:   Disable all connections. 
        /*******************************************************************/
        MiApp_ConnectionMode(ENABLE_ALL_CONN);
        DemoOutput_Channel(myChannel, 0);

#ifdef ENABLE_ED_SCAN		
		uint8_t *NoiseLevel;
		MiApp_NoiseDetection(0xFFFFFFFF, 5, NOISE_DETECT_ENERGY, &NoiseLevel);
#endif // #ifdef ENABLE_ED_SCAN	

#ifdef ENABLE_ACTIVE_SCAN		
		MiApp_SearchConnection(5, 0xFFFFFFFF, Scan_Confirm);
#endif // #ifdef ENABLE_ACTIVE_SCAN

        /*******************************************************************/
        // Function MiApp_EstablishConnection try to establish a new 
        // connection with peer device. 
        // The first parameter is the index to the active scan result, 
        //      which is acquired by discovery process (active scan). If 
        //      the value of the index is 0xFF, try to establish a 
        //      connection with any peer.
        // The second parameter is the mode to establish connection, 
        //      either direct or indirect. Direct mode means connection 
        //      within the radio range; indirect mode means connection 
        //      may or may not in the radio range. 
        /*******************************************************************/
        // Set default channel
        if( MiApp_Set(CHANNEL, &myChannel) == false )
        {
            DemoOutput_ChannelError(myChannel);
        }
				
		uint16_t broadcastAddress = 0xFFFF;
		i = MiApp_EstablishConnection(myChannel, 0, (uint8_t*)&broadcastAddress, 0, Connection_Confirm);

        /*******************************************************************/
        // Display current operation on LCD of demo board, if applicable
        /*******************************************************************/
        if( i != 0xFF )
        {
            DemoOutput_Channel(myChannel, 1);
        }
        else
        {
            /*******************************************************************/
            // If no network can be found and join, we need to start a new 
            // network by calling function MiApp_StartConnection
            //
            // The first parameter is the mode of start connection. There are 
            // two valid connection modes:
            //   - START_CONN_DIRECT        start the connection on current 
            //                              channel
            //   - START_CONN_ENERGY_SCN    perform an energy scan first, 
            //                              before starting the connection on 
            //                              the channel with least noise
            //   - START_CONN_CS_SCN        perform a carrier sense scan 
            //                              first, before starting the 
            //                              connection on the channel with 
            //                              least carrier sense noise. Not
            //                              supported for current radios
            //
            // The second parameter is the scan duration, which has the same 
            //     definition in Energy Scan. 10 is roughly 1 second. 9 is a 
            //     half second and 11 is 2 seconds. Maximum scan duration is 
            //     14, or roughly 16 seconds.
            //
            // The third parameter is the channel map. Bit 0 of the 
            //     double word parameter represents channel 0. For the 2.4GHz 
            //     frequency band, all possible channels are channel 11 to 
            //     channel 26. As the result, the bit map is 0x07FFF800. Stack 
            //     will filter out all invalid channels, so the application 
            //     only needs to pay attention to the channels that are not 
            //     preferred.
            /*******************************************************************/
            MiApp_StartConnection(START_CONN_DIRECT, 10, (1L << myChannel), Connection_Confirm);
        }

        /*******************************************************************/
        // Function DumpConnection is used to print out the content of the
        //  Connection Entry on the hyper terminal. It may be useful in 
        //  the debugging phase.
        // The only parameter of this function is the index of the  
        //  Connection Entry. The value of 0xFF means to print out all
        //  valid Connection Entry; otherwise, the Connection Entry
        //  of the input index will be printed out.
        /*******************************************************************/
#if defined(ENABLE_CONSOLE)
		DumpConnection(0xFF);
#endif

        // Turn on LED 1 to indicate connection established
		LED_On(LED0);
#if defined(PROTOCOL_P2P)
        DemoOutput_Instruction();
#endif    
    }
	
	/* Create SW timer for transmission timeout */
	SwTimerCreate(&TxTimerId) ;
	
	return true;
}

void Run_Demo(void)
{
    P2PTasks();
    run_p2p_demo();   
}