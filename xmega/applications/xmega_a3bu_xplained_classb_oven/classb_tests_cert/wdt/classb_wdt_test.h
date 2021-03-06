/**
 * \file
 *
 * \brief Settings for the watchdog timer test.
 *
 * \par Application note:
 *      AVR1610: Guide to IEC60730 Class B compliance with XMEGA
 *
 * Copyright (c) 2011-2018 Microchip Technology Inc. and its subsidiaries.
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
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef CLASSB_WDT_TEST_H
#define CLASSB_WDT_TEST_H

#include <asf.h>
#include "classb_rtc_common.h"

/**
 * \defgroup classb_wdt Watchdog Timer Test
 *
 * \brief Test to check that the watchdog timer (WDT) is working.
 *
 * The WDT is a system function for monitoring correct program operation that
 * allows recovering from error situations such as runaway or deadlocked code.
 * The self-diagnostic test \ref classb_wdt_test() is executed before the main
 * application and it basically makes sure that:
 * - a system reset is issued after WDT timeout
 * - the WDT can be reset
 * - the device is reset upon untimely WDT reset in window mode
 *
 * The test relies on the Real Time Counter (RTC) to check the timing of the WDT
 * oscillator and, therefore, this library includes a basic RTC driver (see
 * \ref rtc_driver). Note that the RTC has a clock source independent from the
 * CPU and the WDT. The RTC module is implicitly tested as well: if the
 * frequency difference between RTC and WDT is more than 50%, the error state is
 * set.
 *
 * Errors are handled by \ref CLASSB_ERROR_HANDLER_WDT() and there are a number
 * of configurable actions:
 * -# \ref CLASSB_ACTIONS_1_WDT() sets up the WDT for the main application,
 * -# \ref CLASSB_ACTIONS_2_WDT() processes systems resets caused by a working
 * WDT.
 * -# \ref CLASSB_ACTIONS_3_WDT() deals with other reset causes, e.g. brown-out
 * or software reset.
 *
 * In addition to the error handler and configurable actions, the user should
 * configure the WDT periods \ref CLASSB_WDT_WPER and \ref CLASSB_WDT_PER.
 *
 * \note The WDT should be left enabled by this test and be active at all times.
 * There are a number of Class B tests that can potentially take longer time
 * than a WDT, see for example \ref classb_crc. If this is the case, a possible
 * solution would be to increase the WDT period before the Class B test and
 * decrease it afterwards.
 *
 * @{
 */

/**
 * \name Configuration settings
 *
 * @{
 */

/**
 * \brief Open period, during which WDT has to be reset.
 *
 * This should be given as one of the group configuration settings. The total
 * timeout period is the sum of the open and closed periods. In order to comply
 * with the standard, this should be no greater than than 50% of the total
 * period.
 */
#define CLASSB_WDT_PER  WDT_PER_512CLK_gc

/**
 * \brief Closed period, during which WDT cannot be reset.
 *
 * This should be given as one of the group configuration settings. The total
 * timeout period is the sum of the open and closed periods. In order to comply
 * with the standard, this should be at least 50% of the total period.
 */
#define CLASSB_WDT_WPER WDT_WPER_1KCLK_gc

/** @} */

/**
 * \name Constants for internal use
 *
 * \attention These should not be modified by user!
 *
 * @{
 */

/**
 * \brief WDT period in cycles.
 *
 * The WDT frequency is set to 1024 Hz. Considering the information in the
 * datasheet, the period is given by <tt>T_WDT = 8*(2^PER)</tt> where PER is the
 * value of the WDT Timeout register. However, we also have to consider that the
 * group configuration is defined with two bits shifted to the left.
 */
#define CLASSB_WDT_PER_CYCLES (8 * (1 << (CLASSB_WDT_PER >> 2)))

/**
 * \brief Maximum number of WDT cycles to wait for a timeout.
 *
 * The WDT runs from the ULP, which is optimized for power consumption and not
 * accuracy. Here we count with a +50% deviation from the nominal frequency,
 * i.e., <tt>CLASSB_WDT_MAX = 1.5 * CLASSB_WDT_PER_CYCLES</tt>.
 */
#define CLASSB_WDT_MAX (CLASSB_WDT_PER_CYCLES + (CLASSB_WDT_PER_CYCLES >> 1))

/**
 * \brief Minimum number of WDT cycles to wait for a timeout.
 *
 * Here we count with a -50% deviation from the nominal frequency, i.e.,
 * <tt>CLASSB_WDT_MIN = 0.5 * CLASSB_WDT_PER_CYCLES</tt>
 */
#define CLASSB_WDT_MIN (CLASSB_WDT_PER_CYCLES - (CLASSB_WDT_PER_CYCLES >> 1))

/**
 * \brief RTC period in cycles.
 *
 * This setting is only used for the WDT test.
 *
 * The RTC period should be as small as possible to make the estimate reliable.
 * The RTC frequency is set to 1024 Hz. Note that:
 * - if <tt>PER == 0</tt>, only one third of the interrupts are generated
 * - if <tt>PER == 1</tt>, only one half of the interrupts are generated
 * This is not handled by the code, but <tt>PER=2</tt> is small enough. That
 * means approximately 3ms (3 ticks/1024KHz): the value in PER is compared with
 * the counter value, which starts at 0, so the real period will be
 * <tt>CLASSB_WDT_RTC_PER+1</tt>.
 */
#define CLASSB_WDT_RTC_PER 2

/** @} */

/** Enum of valid test states for the WDT test. */
typedef enum classb_preinit_teststate {
	FAULT_WDT,
	TEST_WDT_1,
	TEST_WDT_2,
	TEST_WDT_3,
	TEST_WDT_OK,
} classb_preinit_teststate_t;

/** Macro for telling the compiler not to initialize a variable on restart */
#if defined(__ICCAVR__)
#  define NO_INIT __no_init
#elif defined(__GNUC__)
#  define NO_INIT __attribute__ ((section (".noinit")))
#endif

#if defined(__DOXYGEN__)
/**
 * \name Class B Test
 *
 * @{
 */

void classb_wdt_test(void);

/** @} */
#elif defined(__GNUC__)
/* Pre-init test function prototype for GCC. */
void __attribute__((__naked__, section(".init1"))) classb_wdt_test (void);
/* Pre-init test function type and name for GCC. */
#  define classb_wdt_test void classb_wdt_test
#elif defined(__ICCAVR__)
/* Pre-init test function type and name for IAR. */
#  define classb_wdt_test uint8_t __low_level_init
#else
#  error Unknown compiler!
#endif

#if defined(__GNUC__) && !defined(__OPTIMIZE__)
#  error Optimization must be enabled to successfully write to protected \
	registers, due to timing constraints.
#endif

/** @} */

#endif /* CLASSB_WDT_TEST_H */
