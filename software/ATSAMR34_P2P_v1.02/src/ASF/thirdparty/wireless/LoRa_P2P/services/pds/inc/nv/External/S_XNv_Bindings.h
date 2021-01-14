/**
 * \file
 *
 * \brief XNv Bindings Interface
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
 */
// implemented interface(s)
#define S_XNv_ItemInit S_XNv_ItemInit_Impl
#define S_XNv_Write S_XNv_Write_Impl
#define S_XNv_Read S_XNv_Read_Impl
#define S_XNv_EraseAll S_XNv_EraseAll_Impl
#define S_XNv_SetPowerSupplyCheckingFunction S_XNv_SetPowerSupplyCheckingFunction_Impl
#define S_XNv_ItemLength S_XNv_ItemLength_Impl
#define S_XNv_Delete S_XNv_Delete_Impl
#define S_XNv_SetPowerSupplyCheckingFunction S_XNv_SetPowerSupplyCheckingFunction_Impl
#define S_XNv_IsItemAvailable S_XNv_IsItemAvailable_Impl

// used interfaces
#if defined(TESTHARNESS)
// ...bind to stubs...
#define D_XNv_Write D_XNv_Stub_Write
#define D_XNv_Read D_XNv_Stub_Read
#define D_XNv_EraseSector D_XNv_Stub_EraseSector
#define D_XNv_IsEmpty D_XNv_Stub_IsEmpty
#define D_XNv_IsEqual D_XNv_Stub_IsEqual

// Use actual timers
#define N_Timer_Stop N_Timer_Stop_Impl
#define N_Timer_Start16 N_Timer_Start16_Impl
#define N_Timer_IsRunning N_Timer_IsRunning_Impl

#else
// ...bind to implementation...
#define D_XNv_Write D_XNv_Write_Impl
#define D_XNv_Read D_XNv_Read_Impl
#define D_XNv_EraseSector D_XNv_EraseSector_Impl
#define D_XNv_IsEmpty D_XNv_IsEmpty_Impl
#define D_XNv_IsEqual D_XNv_IsEqual_Impl

#define N_Timer_Stop N_Timer_Stop_Impl
#define N_Timer_Start16 N_Timer_Start16_Impl
#define N_Timer_IsRunning N_Timer_IsRunning_Impl

#endif
