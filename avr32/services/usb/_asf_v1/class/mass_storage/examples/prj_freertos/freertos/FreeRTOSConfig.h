/*****************************************************************************
 *
 * \file
 *
 * \brief FreeRTOS configuration file.
 *
 * Copyright (c) 2009-2018 Microchip Technology Inc. and its subsidiaries.
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
 ******************************************************************************/
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "board.h"
#include "mass_storage_example.h"


/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION      1
#define configUSE_IDLE_HOOK       0
#define configUSE_TICK_HOOK       0
#define configCPU_CLOCK_HZ        ( FCPU_HZ ) /* Hz clk gen */
#define configPBA_CLOCK_HZ        ( FPBA_HZ )
#define configTICK_RATE_HZ        ( ( portTickType ) 1000 )
#define configMAX_PRIORITIES      ( ( unsigned portBASE_TYPE ) 5 )
#define configMINIMAL_STACK_SIZE  ( ( unsigned portSHORT ) 128 )
/* configTOTAL_HEAP_SIZE is not used when heap_3.c is used. */
#define configTOTAL_HEAP_SIZE     ( ( size_t ) ( 1024*25 ) )
#define configMAX_TASK_NAME_LEN   ( 20 )
#define configUSE_TRACE_FACILITY  0
#define configUSE_16_BIT_TICKS    0
#define configIDLE_SHOULD_YIELD   1

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES     0
#define configMAX_CO_ROUTINE_PRIORITIES ( 1 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet            1
#define INCLUDE_uxTaskPriorityGet           1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_vTaskCleanUpResources       0
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_vTaskDelayUntil             1
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_xTaskGetCurrentTaskHandle   0
#define INCLUDE_xTaskGetSchedulerState      0


/* configTICK_USE_TC is a boolean indicating whether to use a Timer Counter or
   the CPU Cycle Counter for the tick generation.
   Both methods will generate an accurate tick.
   0: Use of the CPU Cycle Counter.
   1: Use of the Timer Counter (configTICK_TC_CHANNEL is the TC channel). */
#define configTICK_USE_TC             0
#define configTICK_TC_CHANNEL         2

/* configHEAP_INIT is a boolean indicating whether to initialize the heap with
   0xA5 in order to be able to determine the maximal heap consumption. */
#define configHEAP_INIT               0

// #define configUSE_MUTEXES                   0
// #define configUSE_RECURSIVE_MUTEXES         0
// #define configUSE_COUNTING_SEMAPHORES       0
// #define configUSE_ALTERNATIVE_API           0
// #define configCHECK_FOR_STACK_OVERFLOW      0
// #define configQUEUE_REGISTRY_SIZE           10
// #define configGENERATE_RUN_TIME_STATS       0

// #define INCLUDE_vResumeFromISR              1
// #define INCLUDE_uxTaskGetStackHighWaterMark 0

/* Debug trace configuration.
   configDBG is a boolean indicating whether to activate the debug trace. */
#if BOARD == EVK1100
#define configDBG                     1
#define configDBG_USART               (&AVR32_USART1)
#define configDBG_USART_RX_PIN        AVR32_USART1_RXD_0_0_PIN
#define configDBG_USART_RX_FUNCTION   AVR32_USART1_RXD_0_0_FUNCTION
#define configDBG_USART_TX_PIN        AVR32_USART1_TXD_0_0_PIN
#define configDBG_USART_TX_FUNCTION   AVR32_USART1_TXD_0_0_FUNCTION
#define configDBG_USART_BAUDRATE      57600
#elif BOARD == EVK1101 || BOARD == EVK1104 || BOARD == EVK1105 || BOARD == UC3C_EK
#define configDBG                     0
#else
#error The board USART configuration should be defined here.
#endif


/* USB task definitions. */
#define configTSK_USB_NAME            ((const signed portCHAR *)"USB")
#define configTSK_USB_STACK_SIZE      256
#define configTSK_USB_PRIORITY        (tskIDLE_PRIORITY + 4)

/* USB device task definitions. */
#define configTSK_USB_DEV_NAME        ((const signed portCHAR *)"USB Device")
#define configTSK_USB_DEV_STACK_SIZE  256
#define configTSK_USB_DEV_PRIORITY    (tskIDLE_PRIORITY + 3)
#define configTSK_USB_DEV_PERIOD      20

/* USB host task definitions. */
#define configTSK_USB_HST_NAME        ((const signed portCHAR *)"USB Host")
#define configTSK_USB_HST_STACK_SIZE  256
#define configTSK_USB_HST_PRIORITY    (tskIDLE_PRIORITY + 3)
#define configTSK_USB_HST_PERIOD      20

/* USB device mass-storage task definitions. */
#define configTSK_USB_DMS_NAME        ((const signed portCHAR *)"USB Device MS")
#define configTSK_USB_DMS_STACK_SIZE  256
#define configTSK_USB_DMS_PRIORITY    (tskIDLE_PRIORITY + 2)
#define configTSK_USB_DMS_PERIOD      2

/* USB host mass-storage task definitions. */
#define configTSK_USB_HMS_NAME        ((const signed portCHAR *)"USB Host MS")
#define configTSK_USB_HMS_STACK_SIZE  256
#define configTSK_USB_HMS_PRIORITY    (tskIDLE_PRIORITY + 2)
#define configTSK_USB_HMS_PERIOD      2

/* uShell task definitions. */
#define configTSK_USHELL_NAME         ((const signed portCHAR *)"uShell")
#define configTSK_USHELL_STACK_SIZE   1000
#define configTSK_USHELL_PRIORITY     (tskIDLE_PRIORITY + 1)
#define configTSK_USHELL_PERIOD       50


#endif /* FREERTOS_CONFIG_H */
