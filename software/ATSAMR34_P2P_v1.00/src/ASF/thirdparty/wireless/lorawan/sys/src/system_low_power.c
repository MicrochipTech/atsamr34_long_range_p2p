/**
* \file  system_low_power.c
*
* \brief Low power management file
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

#include <stdint.h>
#include <stdbool.h>
#include "system_low_power.h"
#include "sw_timer.h"
#include "sys.h"

static void OnSleepTimerEvent(uint8_t param) __attribute__((unused));
static void SysCfgLowPower(void) __attribute__((unused));
static void SysCfgRun(void);

static volatile SysSleepState_t goToSleep = SLEEP_OFF;
static volatile SysSleepState_t eusart1WakeUpState = SLEEP_ON;
static uint8_t sleepTimerId;
static exitFromSleepCb_t pExitFromSleepCb;
static volatile bool exitFromSleep = false;


void SysSleepInit(exitFromSleepCb_t exitFromSleepCb)
{
    // Turn off everything the project won't ever use
/*
    UART2MD = 1;
    TMR6MD = 1;
    TMR5MD = 1;
    TMR4MD = 1;
    TMR3MD = 1;
    TMR2MD = 1;
    CCP5MD = 1;
    CCP4MD = 1;
    CCP3MD = 1;
    CCP2MD = 1;
    CCP1MD = 1;
    CTMUMD = 1;
    CMP2MD = 1;
    CMP1MD = 1;
    ADCMD = 1;

    IDLEN = 0;
    
    SwTimerCreate(&sleepTimerId);
    SwTimerSetCallback(sleepTimerId, OnSleepTimerEvent, 0U);
    pExitFromSleepCb = exitFromSleepCb;*/
}

void SysSleepStart(uint32_t ms)
{
/*
    SysSetGoToSleepState(SLEEP_ON);
    SwTimerSetTimeout(sleepTimerId, MS_TO_TICKS(ms));*/
}

void SysGoToSleep(void)
{
#if 0  //CHN_TODO: Replace the code with RTC ASF based code.
   /* if(goToSleep == SLEEP_ON)
    {
        // wait for sleep to be possible. Make sure no timers are running,
        // no interrupts are pending service.
        INTERRUPT_GlobalInterruptDisable();
        if ( ( INTERRUPT_PeekDioStatus() == 0 ) && ( TXIE == 0 ) && ( TXSTA1bits.TRMT == 1 )  && (SwTimersCanSleep( ) == 1))
        {
            // MCU going to sleep. Start sleep timer.
            SysCfgLowPower();
            INTERRUPT_GlobalInterruptEnable();
            SwTimerStart(sleepTimerId);

            //Radio default state after init is "sleep". There is no need to chage radio state        
            while (goToSleep == SLEEP_ON || goToSleep == SLEEP_TRANSITION_TO_OFF)
            {
                //if a wake-up is received eusart interrupt will increment the buffer head and eusart1RxCount
                if(eusart1RxCount > 0)
                {
                    EUSART1WakeUp();
                    
                    //EUSART1_Read is needed in order to not consider the received wake-up sequence as part
                    //of the future command. It will decrease the eusart1RxCount back to 0 and will increment
                    //the buffer tail at the same level as buffer head
                    EUSART1_Read();
                    continue;
                }
                BAUDCON1bits.WUE = 1;
                SLEEP();
                SwTimersExecute();
            }
            BAUDCON1bits.WUE = 0;
            
            //executes only on a break command
            if(eusart1WakeUpState == SLEEP_OFF)
            {
                //a blank read is needed because at some point a receive interrupt will occur during the reply
                //from sleep and will consider the \n from the reply as an input and will be introduced in buffer.
                //In this way we ignore the \n introduced during interrupt.
                EUSART1_Read();
                
                //prepare for next sleep time;
                eusart1WakeUpState = SLEEP_ON;
            }
                
            
        }
        INTERRUPT_GlobalInterruptEnable();
    }*/
#endif	
}

void EUSART1WakeUp(void)
{
#if 0	////CHN_TODO: Replace the code with RTC ASF based code.
    if(eusart1WakeUpState == SLEEP_ON)
    {
        eusart1WakeUpState = SLEEP_TRANSITION_TO_OFF;
        SysSetGoToSleepState(SLEEP_TRANSITION_TO_OFF);
        BAUDCON1bits.ABDEN = 1;   
        
    }
    else
    {
        if (RCSTA1bits.FERR == 1)
        {
            if (RCREG1 == 0x00)
            {
                RCSTA1bits.SPEN = 0; / * Force clear FERR by resetting the EUSART * /
                NOP();
                RCSTA1bits.SPEN = 1; / * Enable EUSART * /
                NOP();
                BAUDCON1bits.ABDEN = 1;
            }
        }
        else
        {
            if(eusart1WakeUpState == SLEEP_TRANSITION_TO_OFF)
            {     
                //Woke up from sleep using UART and new baud detected, reply can be sent now
                eusart1WakeUpState = SLEEP_OFF;
                SysSetGoToSleepState(SLEEP_OFF);

            }
        }
    }*/
#endif	
}

SysSleepState_t SysGetGoToSleepState(void)
{
    return goToSleep;
}

void SysSetGoToSleepState(SysSleepState_t sleepState)
{
    if(sleepState == SLEEP_OFF)
    {
        exitFromSleep = true;
    }
    goToSleep = sleepState;
}

void SysExitFromSleep(void)
{   
    if(exitFromSleep == true)
    {
        SwTimerStop(sleepTimerId);
        
        exitFromSleep = false;
        
        SysCfgRun();

        if (pExitFromSleepCb)
        {
            pExitFromSleepCb();
        }
    }
}

static void OnSleepTimerEvent(uint8_t param)
{
    SysSetGoToSleepState(SLEEP_OFF);
}

static void SysCfgLowPower(void)
{    
  /*  SPI2_DeInitialize();
    
    //Disable MSSP2 module
    MSSP2MD = 1;
    
    //Make sure SPI2 pins are not left in floating state during sleep
    //NCS
    RADIO_nCS_ANS = 0;
    RADIO_nCS_TRIS = 0;
    RADIO_nCS_LAT = 1;

    //MISO
	ANSD1 = 0;
	TRISD1 = 0;
	LATD1 = 0;
		
    //MOSI
	ANSD4 = 0;
	TRISD4 = 0;
	LATD4 = 0;
		
    //CLK
	ANSD0 = 0;
	TRISD0 = 0;
	LATD0 = 0;*/
}

static void SysCfgRun(void)
{    
   /* //Enable MSSP2 module
    MSSP2MD = 0;    
    SPI2_Initialize();*/
}