/**
 * \file
 *
 * \brief Nv Bindings Interface
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
#define S_Nv_ItemInit S_Nv_ItemInit_Impl
#define S_Nv_Write S_Nv_Write_Impl
#define S_Nv_Read S_Nv_Read_Impl
#define S_Nv_EraseAll S_Nv_EraseAll_Impl
#define S_Nv_SetPowerSupplyCheckingFunction S_Nv_SetPowerSupplyCheckingFunction_Impl
#define S_Nv_ItemLength S_Nv_ItemLength_Impl
#define S_Nv_Delete S_Nv_Delete_Impl
#define S_Nv_SetPowerSupplyCheckingFunction S_Nv_SetPowerSupplyCheckingFunction_Impl
#define S_Nv_IsItemAvailable S_Nv_IsItemAvailable_Impl

// used interfaces
#if defined(TESTHARNESS)
// ...bind to stubs...
#define D_Nv_Write D_Nv_Stub_Write
#define D_Nv_Read D_Nv_Stub_Read
#define D_Nv_EraseSector D_Nv_Stub_EraseSector
#define D_Nv_IsEmpty D_Nv_Stub_IsEmpty
#define D_Nv_IsEqual D_Nv_Stub_IsEqual

// Use actual timers
#define N_Timer_Stop N_Timer_Stop_Impl
#define N_Timer_Start16 N_Timer_Start16_Impl
#define N_Timer_IsRunning N_Timer_IsRunning_Impl

#else
// ...bind to implementation...
#define D_Nv_Write D_Nv_Write_Impl
#define D_Nv_Read D_Nv_Read_Impl
#define D_Nv_EraseSector D_Nv_EraseSector_Impl
#define D_Nv_IsEmpty D_Nv_IsEmpty_Impl
#define D_Nv_IsEqual D_Nv_IsEqual_Impl

#define N_Timer_Stop N_Timer_Stop_Impl
#define N_Timer_Start16 N_Timer_Start16_Impl
#define N_Timer_IsRunning N_Timer_IsRunning_Impl

#endif
#define S_XNv_SetPowerSupplyCheckingFunction S_XNv_SetPowerSupplyCheckingFunction_Impl
