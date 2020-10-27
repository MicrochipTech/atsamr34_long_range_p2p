/**
* \file  p2p_demo.c
*
* \brief Demo Application for MiWi P2P Implementation
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
#include "miwi_api.h"
#include "miwi_p2p.h"
#include "task.h"
#include "p2p_demo.h"
#include "mimem.h"
#include "asf.h"
#if defined(ENABLE_SLEEP_FEATURE)
#include "sleep_mgr.h"
#endif
#if defined (ENABLE_CONSOLE)
#include "sio2host.h"
#endif

#include "phy.h"								
#include "sw_timer.h"					

#if defined(PROTOCOL_P2P)

uint8_t i;
uint8_t TxSynCount = 0;
uint8_t TxSynCount2 = 0;
uint8_t TxNum = 0;
uint8_t RxNum = 0;
bool chk_sel_status = true;  // Check _select_Status
uint8_t NumOfActiveScanResponse;
bool update_ed;
uint8_t select_ed;
uint8_t msghandledemo = 0;
	
#if defined(ENABLE_SLEEP_FEATURE)
volatile uint8_t send_data = 0;	// Flag used to put a sleeping device(RFD) in sleep - wakeup - transmit cycle
#endif // #if defined(ENABLE_SLEEP_FEATURE)

void TxToutCallback(void)
{
	#if defined(ENABLE_SLEEP_FEATURE)
		send_data = 0;
	#endif // #if defined(ENABLE_SLEEP_FEATURE)
	/* That bring the node back to continuous transaction cycle */
	PHY_DataConf(TRANSACTION_EXPIRED);
	SwTimerStop(TxTimerId);
}

void dataConfcb(uint8_t handle, miwi_status_t status, uint8_t* msgPointer)
{
	#if defined (ENABLE_CONSOLE)
        printf("\nData Confirm: Handle: %d status:%d \r\n", handle, status);
	#endif // #if defined (ENABLE_CONSOLE)
	#if defined(ENABLE_SLEEP_FEATURE)
		send_data = 0;
	#endif // #if defined(ENABLE_SLEEP_FEATURE)
	/* Stop transmission timeout timer */
		SwTimerStop(TxTimerId);
	/* Free payload buffer allocated */
	MiMem_Free(msgPointer);
}

void run_p2p_demo(void)
{
#if defined(ENABLE_SLEEP_FEATURE)
	if (Total_Connections())
	{
		/* At least device is connected to one peer device */
		if(!(P2PStatus.bits.DataRequesting || P2PStatus.bits.RxHasUserData || send_data))
		{
			/* Put the transceiver into sleep */
			MiApp_TransceiverPowerState(POWER_STATE_SLEEP);
			#if defined (ENABLE_CONSOLE)
			printf("\r\nDevice is sleeping");
			/* Disable UART */
			sio2host_disable();
			#endif // #if defined (ENABLE_CONSOLE)
			// Turned OFF RF Switch
			struct port_config pin_conf;
			port_get_config_defaults(&pin_conf);
			pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
			port_pin_set_config(RF_SWITCH_PIN, &pin_conf);
			port_pin_set_output_level(RF_SWITCH_PIN, RF_SWITCH_INACTIVE);
			
			/* Put the MCU into sleep */
			sm_sleep((RFD_WAKEUP_INTERVAL - 2));

			#if defined (ENABLE_CONSOLE)
			/* Enable UART */
			sio2host_enable();
			printf("\r\nDevice WokeUp");
			#endif // #if defined (ENABLE_CONSOLE)
			/* Wakeup the transceiver and send data request*/
			MiApp_TransceiverPowerState(POWER_STATE_WAKEUP_DR);
		}
	}
#endif // #if defined(ENABLE_SLEEP_FEATURE)
	{
		/*******************************************************************/
		// If no packet received, now we can check if we want to send out
		// any information.
		// Function ButtonPressed will return if any of the two buttons
		// has been pushed.
		/*******************************************************************/

		/* Read the button */
		uint8_t PressedButton = ButtonPressed();
		switch( PressedButton )
		{
			case 1:
			{
				/*******************************************************************/
				// Button 1 pressed. We need to send out the bitmap of word "MiWi".
				// First call MiApp_FlushTx to reset the Transmit buffer. Then fill
				// the buffer one byte by one byte by calling function
				// MiApp_WriteData
				/*******************************************************************/
				uint8_t* dataPtr = NULL;
				uint8_t dataLen = 0;
				uint16_t broadcastAddress = 0xFFFF;
				#if defined(ENABLE_SECURITY)
					/* Allocate buffer for secured payload */
					dataPtr = MiMem_Alloc(CALC_SEC_PAYLOAD_SIZE(MAX_SEC_BCAST_PAYLOAD)); 
					if (NULL == dataPtr)
					    return;
                    for(i = 0; i < MAX_SEC_BCAST_PAYLOAD; i++) 
				#else
					/* Allocate buffer for non-secured payload */
					dataPtr = MiMem_Alloc(CALC_SEC_PAYLOAD_SIZE(MAX_NSEC_BCAST_PAYLOAD)); 
					if (NULL == dataPtr)
					return;
					for(i = 0; i < MAX_NSEC_BCAST_PAYLOAD; i++) 
				#endif // #if defined(ENABLE_SECURITY)
					{
						// Fill TX buffer User DATA
						dataPtr[dataLen++] = 0x42;	// 'B'
                    }
                    TxSynCount++;
				#if defined(ENABLE_SLEEP_FEATURE)
		 			send_data = 1;
				#endif // #if defined(ENABLE_SLEEP_FEATURE)
				
				/* Broadcast the message */
				if(MiApp_SendData(SHORT_ADDR_LEN, (uint8_t *)&broadcastAddress, dataLen, dataPtr, msghandledemo++, true, dataConfcb)== false)
				{
					#if defined(ENABLE_SLEEP_FEATURE)
						PHY_DataConf(FAILURE);
					#endif // #if defined(ENABLE_SLEEP_FEATURE)
					DemoOutput_BroadcastFail();
				}
				else
				{
					/* Start timer for transmission timeout */
					SwTimerStart (TxTimerId, MS_TO_US(5000), 0/*SW_TIMEOUT_RELATIVE*/, (void *)TxToutCallback, NULL);
				}

				/* Update display/console */
				DemoOutput_UpdateTxRx(++TxNum, RxNum);
				DemoOutput_Instruction();
			}
			break;
			case 2:
				/* Button 2 pressed */
				chk_sel_status = true;
				select_ed = 0;
				update_ed = true;
				while(update_ed == true)
				{
					// Peer Device Info
					#if defined (ENABLE_LCD)
						LCD_Erase();
						snprintf(LCDText, sizeof(LCDText),(char*)"SW:%02d-%02x%02x%02x \nBUTTON1: Change node",select_ed,ConnectionTable[select_ed].Address[0],
                               ConnectionTable[select_ed].Address[1],ConnectionTable[select_ed].Address[2]);
                        LCD_Update();
					#endif // #if defined (ENABLE_LCD)
					// Display another Peer Device Address
					chk_sel_status = true;

					while(chk_sel_status)
					{
						uint8_t switch_val = ButtonPressed();
						// While waiting in TX , RX will process if any message was available
						if(switch_val == 1)
						{
							uint8_t* dataPtr = NULL;
							uint8_t dataLen = 0;
							update_ed = false;
							chk_sel_status = false;
							#if defined(ENABLE_SECURITY)
								/* Allocate buffer for secured payload */
								dataPtr = MiMem_Alloc(CALC_SEC_PAYLOAD_SIZE(MAX_SEC_UCAST_PAYLOAD));
							    if (NULL == dataPtr)
								    return;
                                for(i = 0; i < MAX_SEC_UCAST_PAYLOAD; i++)        
							#else
								/* Allocate buffer for non-secured payload */
                                dataPtr = MiMem_Alloc(CALC_SEC_PAYLOAD_SIZE(MAX_NSEC_UCAST_PAYLOAD));    
                                if (NULL == dataPtr)
                                return;
                                for(i = 0; i < MAX_NSEC_UCAST_PAYLOAD; i++)        
							#endif // #if defined(ENABLE_SECURITY)
                                {
                                    // Fill TX buffer User DATA
									dataPtr[dataLen++] = 0x55;	// 'U'
                                }
                                TxSynCount2++;
							#if defined(ENABLE_SLEEP_FEATURE)
								send_data = 1;
							#endif // #if defined(ENABLE_SLEEP_FEATURE)
							
							/* Unicast the message to select_ed node */
							if (MiApp_SendData(LONG_ADDR_LEN, ConnectionTable[select_ed].Address, dataLen, dataPtr, msghandledemo++, 1, dataConfcb) == false)
							{
								/* That bring the node back to continuous transaction cycle */
								#if defined(ENABLE_SLEEP_FEATURE)
									PHY_DataConf(FAILURE);
								#endif // #if defined(ENABLE_SLEEP_FEATURE)
								DemoOutput_UnicastFail();
							}
							else
							{
								// Successful Transmission
								TxNum++;
								/* Start timer for transmission timeout */
								SwTimerStart (TxTimerId, MS_TO_US(5000), 0/*SW_TIMEOUT_RELATIVE*/, (void *)TxToutCallback, NULL);
							}
							// Update the display/console
							DemoOutput_UpdateTxRx(TxNum, RxNum);
							DemoOutput_Instruction();

							break;
						} // if(switch_val == 1)
						else if(switch_val == 2)
						{
							if (select_ed > conn_size-2)
							{
								// Last Peer Device
								select_ed = 0;
							}
							else
							{
								// Update the Display
								select_ed = select_ed+1;
							}
							chk_sel_status = false;
						} // else if(switch_val == 2)
					} // while(chk_sel_status), end of Peer Device selection
				} // while(update_ed == true), end of Display
			break;
			default:
			break;
		}
	}
}

void ReceivedDataIndication (RECEIVED_MESSAGE *ind)
{
    /*******************************************************************/
    // If a packet has been received, handle the information available
    // in rxMessage.
    /*******************************************************************/
	if ((myPANID.v[1] == ind->SourcePANID.v[1]) && (myPANID.v[0] == ind->SourcePANID.v[0]))
	{
		#if defined(ENABLE_CONSOLE)
		DemoOutput_HandleMessage();
		#endif
		DemoOutput_UpdateTxRx(TxNum, ++RxNum);
		// Toggle LED2 to indicate receiving a packet.
		LED_Toggle(LED0);
		DemoOutput_Instruction();
	}
	else
	{
		printf("Packet received from other Source PAN ID : 0x%x%x\r\n",ind->SourcePANID.v[1],ind->SourcePANID.v[0]);
	}
}

void p2p_demo_unicast_to_parent(void)
{
	uint8_t* dataPtr = NULL;
	uint8_t dataLen = 0;
	update_ed = false;
	chk_sel_status = false;
	#if defined(ENABLE_SECURITY)
		dataPtr = MiMem_Alloc(CALC_SEC_PAYLOAD_SIZE(MAX_SEC_UCAST_PAYLOAD));    //PL_fix
		if (NULL == dataPtr)
		return;
		for(i = 0; i < MAX_SEC_UCAST_PAYLOAD; i++)        //PL_fix
	#else
		dataPtr = MiMem_Alloc(CALC_SEC_PAYLOAD_SIZE(MAX_NSEC_UCAST_PAYLOAD));    //PL_fix
		if (NULL == dataPtr)
		return;
		for(i = 0; i < MAX_NSEC_UCAST_PAYLOAD; i++)        //PL_fix
	#endif
		{
		// Tx Buffer User Data
		dataPtr[dataLen++] = 0x55;
		}
		TxSynCount2++;
	#if defined(ENABLE_SLEEP_FEATURE)
	// to not enter to the sleep loop until data has been sent
	send_data = 1;
	#endif // #if defined(ENABLE_SLEEP_FEATURE)

	select_ed = 0 ;    // parent addr: ConnectionTable[0].Address
	printf("\r\nTry to unicast to %02d-%02x%02x%02x\r\n", select_ed, ConnectionTable[select_ed].Address[0], ConnectionTable[select_ed].Address[1], ConnectionTable[select_ed].Address[2]) ;

	if( MiApp_SendData(LONG_ADDR_LEN, ConnectionTable[select_ed].Address, dataLen, dataPtr, msghandledemo++, 1, dataConfcb) == false)
	{
		#if defined(ENABLE_SLEEP_FEATURE)
			PHY_DataConf(FAILURE);
		#endif // #if defined(ENABLE_SLEEP_FEATURE)
		DemoOutput_UnicastFail();
	}
	else
	{
		// Successful Transmission
		TxNum++;
		SwTimerStart (TxTimerId, MS_TO_US(5000), 0/*SW_TIMEOUT_RELATIVE*/, (void *)TxToutCallback, NULL);
	}
	// Update the LCD
	DemoOutput_UpdateTxRx(TxNum, RxNum);
	DemoOutput_Instruction();
	printf("Tx Messages: %d - Rx Messages: %d\r\n", TxNum, RxNum) ;	
}

#endif // #if defined(PROTOCOL_P2P)