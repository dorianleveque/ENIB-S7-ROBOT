

#ifndef ASF_H
#define ASF_H

/*
 * This file includes all API header files for the selected drivers from ASF.
 * Note: There might be duplicate includes required by more than one driver.
 *
 * The file is automatically generated and will be re-written when
 * running the ASF driver selector tool. Any changes will be discarded.
 */

// From module: ADC - Analog-to-digital Converter
#include <adc.h>

// From module: Common SAM compiler driver
#include <compiler.h>
#include <status_codes.h>

// From module: FreeRTOS mini Real-Time Kernel
#include <FreeRTOS.h>
#include <StackMacros.h>
#include <croutine.h>
#include <list.h>
#include <mpu_wrappers.h>
#include <portable.h>
#include <projdefs.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>
#include <timers.h>

// From module: GPIO - General purpose Input/Output
#include <gpio.h>

// From module: Generic board support
#include <board.h>

// From module: IOPORT - General purpose I/O service
#include <ioport.h>

// From module: Interrupt management - SAM implementation
#include <interrupt.h>

// From module: PDC - Peripheral DMA Controller Example
#include <pdc.h>

// From module: PIO - Parallel Input/Output Controller
#include <pio.h>

// From module: PMC - Power Management Controller
#include <pmc.h>
#include <sleep.h>

// From module: PWM - Pulse Width Modulation
#include <pwm.h>

// From module: Part identification macros
#include <parts.h>

// From module: SAM3N EK LED support enabled
#include <led.h>

// From module: SAM3N startup code
#include <exceptions.h>

// From module: Standard serial I/O (stdio) - SAM implementation
#include <stdio_serial.h>

// From module: System Clock Control - SAM3N implementation
#include <sysclk.h>

// From module: TC - Timer Counter
#include <tc.h>

// From module: UART - Univ. Async Rec/Trans
#include <uart.h>

// From module: USART - Serial interface - SAM implementation for devices with both UART and USART
#include <serial.h>

// From module: USART - Univ. Syn Async Rec/Trans
#include <usart.h>

// From module: pio_handler support enabled
#include <pio_handler.h>

#endif // ASF_H
