/*****************************************************************************
 *
 * \file
 *
 * \brief 32 bits FIR filtering
 *
 * This file show a way to use the 32 bits FIR filter function and print the output signal
 * through the USART.
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
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

/*! \mainpage
 * \section intro Introduction
 * This example demonstrates how to use the 32-bit FIR Filter function.\n
 * To visualize the result, you may use a terminal configured as described in the section <a href="#configinfo">Configuration Information</a>.\n
 * Through this page, you can access the \ref group_dsplib documentation.
 *
 * \section compilinfo Compilation Information
 * This software is written for GNU GCC for 32-bit AVRs and for IAR Embedded Workbench
 * for Atmel 32-bit AVRs. Other compilers may or may not work.
 *
 * \section deviceinfo Device Info
 * All 32-bit AVR devices with a USART module can be used. This example has been tested
 * with the following setup:<BR>
 * - EVK1100 evaluation kit,
 * - EVK1101 evaluation kit.
 *
 * \section configinfo Configuration Information
 * This example has been tested with the following configuration:
 * - CPU clock: 12 MHz;
 * - USART1 connected to a PC serial port via a standard RS232 DB9 cable;
 * - PC terminal settings:
 *   - 9600 bps,
 *   - 8 data bits,
 *   - no parity bit,
 *   - 1 stop bit,
 *   - no flow control.
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <avr32/io.h>

#include "compiler.h"
#include "board.h"

#include "dsp.h"
#include "dsp_debug.h"

#include "pm.h"
#include "cycle_counter.h"

//! The size of the input signal
#define SIZE  64
//! The number of tap
#define FIR_COEF_SIZE  25

//! The output buffer
A_ALIGNED dsp32_t y[SIZE - FIR_COEF_SIZE + 1];

//! The input signal resulting from a multiplication between a cosine and a sine.
A_ALIGNED dsp32_t x[SIZE] = {
  DSP32_Q(0.00000000000),
  DSP32_Q(0.05971733275),
  DSP32_Q(0.10139671590),
  DSP32_Q(0.11013997328),
  DSP32_Q(0.07684940150),
  DSP32_Q(0.00000000009),
  DSP32_Q(-0.11375674268),
  DSP32_Q(-0.25026678813),
  DSP32_Q(-0.38974690925),
  DSP32_Q(-0.50960156480),
  DSP32_Q(-0.58778525220),
  DSP32_Q(-0.60622623913),
  DSP32_Q(-0.55381024235),
  DSP32_Q(-0.42847700897),
  DSP32_Q(-0.23810168698),
  DSP32_Q(-0.00000000072),
  DSP32_Q(0.26091167702),
  DSP32_Q(0.51508014225),
  DSP32_Q(0.73202046178),
  DSP32_Q(0.88426998520),
  DSP32_Q(0.95105651622),
  DSP32_Q(0.92117732728),
  DSP32_Q(0.79468707991),
  DSP32_Q(0.58315039108),
  DSP32_Q(0.30840722126),
  DSP32_Q(0.00000000147),
  DSP32_Q(-0.30840721848),
  DSP32_Q(-0.58315038876),
  DSP32_Q(-0.79468707829),
  DSP32_Q(-0.92117732653),
  DSP32_Q(-0.95105651640),
  DSP32_Q(-0.88426998626),
  DSP32_Q(-0.73202046355),
  DSP32_Q(-0.51508014451),
  DSP32_Q(-0.26091167948),
  DSP32_Q(-0.00000000167),
  DSP32_Q(0.23810168494),
  DSP32_Q(0.42847700747),
  DSP32_Q(0.55381024151),
  DSP32_Q(0.60622623898),
  DSP32_Q(0.58778525267),
  DSP32_Q(0.50960156576),
  DSP32_Q(0.38974691050),
  DSP32_Q(0.25026678946),
  DSP32_Q(0.11375674388),
  DSP32_Q(0.00000000082),
  DSP32_Q(-0.07684940098),
  DSP32_Q(-0.11013997317),
  DSP32_Q(-0.10139671616),
  DSP32_Q(-0.05971733325),
  DSP32_Q(-0.00000000059),
  DSP32_Q(0.05971733225),
  DSP32_Q(0.10139671565),
  DSP32_Q(0.11013997338),
  DSP32_Q(0.07684940203),
  DSP32_Q(0.00000000100),
  DSP32_Q(-0.11375674148),
  DSP32_Q(-0.25026678680),
  DSP32_Q(-0.38974690800),
  DSP32_Q(-0.50960156384),
  DSP32_Q(-0.58778525172),
  DSP32_Q(-0.60622623927),
  DSP32_Q(-0.55381024319),
  DSP32_Q(-0.42847701047)
};

// The impulse response coefficients of the filter
A_ALIGNED dsp32_t fir_coef[FIR_COEF_SIZE] = {
  DSP32_Q(-0.01559148806),
  DSP32_Q(-0.00894210585),
  DSP32_Q(0.00000000000),
  DSP32_Q(0.01092924048),
  DSP32_Q(0.02338723209),
  DSP32_Q(0.03678830106),
  DSP32_Q(0.05045511524),
  DSP32_Q(0.06366197724),
  DSP32_Q(0.07568267286),
  DSP32_Q(0.08583936913),
  DSP32_Q(0.09354892838),
  DSP32_Q(0.09836316431),
  DSP32_Q(0.10000000000),
  DSP32_Q(0.09836316431),
  DSP32_Q(0.09354892838),
  DSP32_Q(0.08583936913),
  DSP32_Q(0.07568267286),
  DSP32_Q(0.06366197724),
  DSP32_Q(0.05045511524),
  DSP32_Q(0.03678830106),
  DSP32_Q(0.02338723209),
  DSP32_Q(0.01092924048),
  DSP32_Q(0.00000000000),
  DSP32_Q(-0.00894210585),
  DSP32_Q(-0.01559148806)
};


//! The main function
int main(int argc, char *argv[])
{
  unsigned int cycle_count;

  // Switch to external Oscillator 0.
  pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);

  // Initialize the DSP debug module
  dsp_debug_initialization(FOSC0);

  // Get the actual cycle count
  cycle_count = Get_sys_count();

  // Perform a 25-taps FIR
  dsp32_filt_fir(y, x, SIZE, fir_coef, FIR_COEF_SIZE);

  // Calculate the number of cycles
  cycle_count = Get_sys_count() - cycle_count;

  // Print the number of cycles
  dsp32_debug_printf("Cycle count: %d\n", cycle_count);
  // Print the output signal
  dsp32_debug_print_vect(y, SIZE - FIR_COEF_SIZE + 1);

  while(1);
}

