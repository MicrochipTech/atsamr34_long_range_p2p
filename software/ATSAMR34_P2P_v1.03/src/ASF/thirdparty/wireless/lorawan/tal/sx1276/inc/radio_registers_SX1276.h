/**
* \file  radio_registers_SX1276.h
*
* \brief This is the Radio register SX1276 source file which contains LoRa-specific
*		 Radio register functions declarations and defines for SX1276
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

#ifndef RADIO_REGISTERS_H
#define	RADIO_REGISTERS_H

#ifdef	__cplusplus
extern "C" {
#endif


#define REG_WRITE                                       0x80

// Common registers
#define REG_FIFO                                        0x00
#define REG_OPMODE                                      0x01
#define REG_FRFMSB                                      0x06
#define REG_FRFMID                                      0x07
#define REG_FRFLSB                                      0x08
#define REG_PACONFIG                                    0x09
#define REG_PARAMP                                      0x0A
#define REG_OCP                                         0x0B
#define REG_LNA                                         0x0C
#define REG_DIOMAPPING1                                 0x40
#define REG_DIOMAPPING2                                 0x41
#define REG_VERSION                                     0x42
#define REG_TCXO                                        0x4B
#define REG_PADAC                                       0x4D
#define REG_FORMERTEMP                                  0x5B
#define REG_AGCREF                                      0x61
#define REG_AGCTHRESH1                                  0x62
#define REG_AGCTHRESH2                                  0x63
#define REG_AGCTHRESH3                                  0x64
#define REG_PLL                                         0x70

// FSK only registers
#define REG_FSK_BITRATEMSB                              0x02
#define REG_FSK_BITRATELSB                              0x03
#define REG_FSK_FDEVMSB                                 0x04
#define REG_FSK_FDEVLSB                                 0x05
#define REG_FSK_RXCONFIG                                0x0D
#define REG_FSK_RSSICONFIG                              0x0E
#define REG_FSK_RSSICOLLISION                           0x0F
#define REG_FSK_RSSITHRESH                              0x10
#define REG_FSK_RSSIVALUE                               0x11
#define REG_FSK_RXBW                                    0x12
#define REG_FSK_AFCBW                                   0x13
#define REG_FSK_OOKPEAK                                 0x14
#define REG_FSK_OOKFIX                                  0x15
#define REG_FSK_OOKAVG                                  0x16
#define REG_FSK_RES17                                   0x17
#define REG_FSK_RES18                                   0x18
#define REG_FSK_RES19                                   0x19
#define REG_FSK_AFCFEI                                  0x1A
#define REG_FSK_AFCMSB                                  0x1B
#define REG_FSK_AFCLSB                                  0x1C
#define REG_FSK_FEIMSB                                  0x1D
#define REG_FSK_FEILSB                                  0x1E
#define REG_FSK_PREAMBLEDETECT                          0x1F
#define REG_FSK_RXTIMEOUT1                              0x20
#define REG_FSK_RXTIMEOUT2                              0x21
#define REG_FSK_RXTIMEOUT3                              0x22
#define REG_FSK_RXDELAY                                 0x23
#define REG_FSK_OSC                                     0x24
#define REG_FSK_PREAMBLEMSB                             0x25
#define REG_FSK_PREAMBLELSB                             0x26
#define REG_FSK_SYNCCONFIG                              0x27
#define REG_FSK_SYNCVALUE1                              0x28
#define REG_FSK_SYNCVALUE2                              0x29
#define REG_FSK_SYNCVALUE3                              0x2A
#define REG_FSK_SYNCVALUE4                              0x2B
#define REG_FSK_SYNCVALUE5                              0x2C
#define REG_FSK_SYNCVALUE6                              0x2D
#define REG_FSK_SYNCVALUE7                              0x2E
#define REG_FSK_SYNCVALUE8                              0x2F
#define REG_FSK_PACKETCONFIG1                           0x30
#define REG_FSK_PACKETCONFIG2                           0x31
#define REG_FSK_PAYLOADLENGTH                           0x32
#define REG_FSK_NODEADRS                                0x33
#define REG_FSK_BROADCASTADRS                           0x34
#define REG_FSK_FIFOTHRESH                              0x35
#define REG_FSK_SEQCONFIG1                              0x36
#define REG_FSK_SEQCONFIG2                              0x37
#define REG_FSK_TIMERRESOL                              0x38
#define REG_FSK_TIMER1COEF                              0x39
#define REG_FSK_TIMER2COEF                              0x3A
#define REG_FSK_IMAGECAL                                0x3B
#define REG_FSK_TEMP                                    0x3C
#define REG_FSK_LOWBAT                                  0x3D
#define REG_FSK_IRQFLAGS1                               0x3E
#define REG_FSK_IRQFLAGS2                               0x3F
#define REG_FSK_PLLHOP                                  0x44
#define REG_FSK_BITRATEFRAC                             0x5D

// LoRa only registers
#define REG_LORA_FIFOADDRPTR                            0x0D
#define REG_LORA_FIFOTXBASEADDR                         0x0E
#define REG_LORA_FIFORXBASEADDR                         0x0F
#define REG_LORA_FIFORXCURRENTADDR                      0x10
#define REG_LORA_IRQFLAGSMASK                           0x11
#define REG_LORA_IRQFLAGS                               0x12
#define REG_LORA_RXNBBYTES                              0x13
#define REG_LORA_RXHEADERCNTVALUEMSB                    0x14
#define REG_LORA_RXHEADERCNTVALUELSB                    0x15
#define REG_LORA_RXPACKETCNTVALUEMSB                    0x16
#define REG_LORA_RXPACKETCNTVALUELSB                    0x17
#define REG_LORA_MODEMSTAT                              0x18
#define REG_LORA_PKTSNRVALUE                            0x19
#define REG_LORA_PKTRSSIVALUE                           0x1A
#define REG_LORA_RSSIVALUE                              0x1B
#define REG_LORA_HOPCHANNEL                             0x1C
#define REG_LORA_MODEMCONFIG1                           0x1D
#define REG_LORA_MODEMCONFIG2                           0x1E
#define REG_LORA_SYMBTIMEOUTLSB                         0x1F
#define REG_LORA_PREAMBLEMSB                            0x20
#define REG_LORA_PREAMBLELSB                            0x21
#define REG_LORA_PAYLOADLENGTH                          0x22
#define REG_LORA_PAYLOADMAXLENGTH                       0x23
#define REG_LORA_HOPPERIOD                              0x24
#define REG_LORA_FIFORXBYTEADDR                         0x25
#define REG_LORA_MODEMCONFIG3                           0x26
#define REG_LORA_FEIMSB                                 0x28
#define REG_LORA_FEIMID                                 0x29
#define REG_LORA_FEILSB                                 0x2A
#define REG_LORA_RSSIWIDEBAND                           0x2C
#define REG_LORA_DETECTOPTIMIZE                         0x31
#define REG_LORA_INVERTIQ                               0x33
#define REG_LORA_DETECTIONTHRESHOLD                     0x37


#ifdef	__cplusplus
}
#endif

#endif	/* RADIO_REGISTERS_H */

/**
 End of File
*/