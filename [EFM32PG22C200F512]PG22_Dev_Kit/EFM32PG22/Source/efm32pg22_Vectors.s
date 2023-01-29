/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*            (c) 2014 - 2020 SEGGER Microcontroller GmbH             *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
* All rights reserved.                                               *
*                                                                    *
* Redistribution and use in source and binary forms, with or         *
* without modification, are permitted provided that the following    *
* condition is met:                                                  *
*                                                                    *
* - Redistributions of source code must retain the above copyright   *
*   notice, this condition and the following disclaimer.             *
*                                                                    *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND             *
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,        *
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF           *
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
* DISCLAIMED. IN NO EVENT SHALL SEGGER Microcontroller BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR           *
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT  *
* OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;    *
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF      *
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT          *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE  *
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH   *
* DAMAGE.                                                            *
*                                                                    *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File      : efm32pg22_Vectors.s
Purpose   : Exception and interrupt vectors for efm32pg22 devices.

Additional information:
  Preprocessor Definitions
    __NO_EXTERNAL_INTERRUPTS
      If defined,
        the vector table will contain only the internal exceptions
        and interrupts.
    __VECTORS_IN_RAM
      If defined,
        an area of RAM, large enough to store the vector table,
        will be reserved.

    __OPTIMIZATION_SMALL
      If defined,
        all weak definitions of interrupt handlers will share the
        same implementation.
      If not defined,
        all weak definitions of interrupt handlers will be defined
        with their own implementation.
*/
        .syntax unified

/*********************************************************************
*
*       Macros
*
**********************************************************************
*/

//
// Directly place a vector (word) in the vector table
//
.macro VECTOR Name=
        .section .vectors, "ax"
        .code 16
        .word \Name
.endm

//
// Declare an exception handler with a weak definition
//
.macro EXC_HANDLER Name=
        //
        // Insert vector in vector table
        //
        .section .vectors, "ax"
        .word \Name
        //
        // Insert dummy handler in init section
        //
        .section .init.\Name, "ax"
        .thumb_func
        .weak \Name
        .balign 2
\Name:
        1: b 1b   // Endless loop
.endm

//
// Declare an interrupt handler with a weak definition
//
.macro ISR_HANDLER Name=
        //
        // Insert vector in vector table
        //
        .section .vectors, "ax"
        .word \Name
        //
        // Insert dummy handler in init section
        //
#if defined(__OPTIMIZATION_SMALL)
        .section .init, "ax"
        .weak \Name
        .thumb_set \Name,Dummy_Handler
#else
        .section .init.\Name, "ax"
        .thumb_func
        .weak \Name
        .balign 2
\Name:
        1: b 1b   // Endless loop
#endif
.endm

//
// Place a reserved vector in vector table
//
.macro ISR_RESERVED
        .section .vectors, "ax"
        .word 0
.endm

//
// Place a reserved vector in vector table
//
.macro ISR_RESERVED_DUMMY
        .section .vectors, "ax"
        .word Dummy_Handler
.endm

/*********************************************************************
*
*       Externals
*
**********************************************************************
*/
        .extern __stack_end__
        .extern Reset_Handler
        .extern HardFault_Handler

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*  Setup of the vector table and weak definition of interrupt handlers
*
*/
        .section .vectors, "ax"
        .code 16
        .balign 256
        .global _vectors
        .global __Vectors
_vectors:
__Vectors:
        //
        // Internal exceptions and interrupts
        //
        VECTOR __stack_end__
        VECTOR Reset_Handler
        EXC_HANDLER NMI_Handler
        VECTOR HardFault_Handler
        ISR_RESERVED
        ISR_RESERVED
        ISR_RESERVED
        ISR_RESERVED
        ISR_RESERVED
        ISR_RESERVED
        ISR_RESERVED
        EXC_HANDLER SVC_Handler
        ISR_RESERVED
        ISR_RESERVED
        EXC_HANDLER PendSV_Handler
        EXC_HANDLER SysTick_Handler
        //
        // External interrupts
        //
#ifndef __NO_EXTERNAL_INTERRUPTS
        ISR_HANDLER CRYPTOACC_IRQHandler
        ISR_HANDLER TRNG_IRQHandler
        ISR_HANDLER PKE_IRQHandler
        ISR_HANDLER SMU_SECURE_IRQHandler
        ISR_HANDLER SMU_PRIVILEGED_IRQHandler
        ISR_HANDLER SMU_NS_PRIVILEGED_IRQHandler
        ISR_HANDLER EMU_IRQHandler
        ISR_HANDLER TIMER0_IRQHandler
        ISR_HANDLER TIMER1_IRQHandler
        ISR_HANDLER TIMER2_IRQHandler
        ISR_HANDLER TIMER3_IRQHandler
        ISR_HANDLER TIMER4_IRQHandler
        ISR_HANDLER RTCC_IRQHandler
        ISR_HANDLER USART0_RX_IRQHandler
        ISR_HANDLER USART0_TX_IRQHandler
        ISR_HANDLER USART1_RX_IRQHandler
        ISR_HANDLER USART1_TX_IRQHandler
        ISR_HANDLER ICACHE0_IRQHandler
        ISR_HANDLER BURTC_IRQHandler
        ISR_HANDLER LETIMER0_IRQHandler
        ISR_HANDLER SYSCFG_IRQHandler
        ISR_HANDLER LDMA_IRQHandler
        ISR_HANDLER LFXO_IRQHandler
        ISR_HANDLER LFRCO_IRQHandler
        ISR_HANDLER ULFRCO_IRQHandler
        ISR_HANDLER GPIO_ODD_IRQHandler
        ISR_HANDLER GPIO_EVEN_IRQHandler
        ISR_HANDLER I2C0_IRQHandler
        ISR_HANDLER I2C1_IRQHandler
        ISR_HANDLER EMUDG_IRQHandler
        ISR_HANDLER EMUSE_IRQHandler
        ISR_HANDLER AGC_IRQHandler
        ISR_HANDLER BUFC_IRQHandler
        ISR_HANDLER FRC_PRI_IRQHandler
        ISR_HANDLER FRC_IRQHandler
        ISR_HANDLER MODEM_IRQHandler
        ISR_HANDLER PROTIMER_IRQHandler
        ISR_HANDLER RAC_RSM_IRQHandler
        ISR_HANDLER RAC_SEQ_IRQHandler
        ISR_HANDLER RDMAILBOX_IRQHandler
        ISR_HANDLER RFSENSE_IRQHandler
        ISR_HANDLER PRORTC_IRQHandler
        ISR_HANDLER SYNTH_IRQHandler
        ISR_HANDLER WDOG0_IRQHandler
        ISR_HANDLER HFXO0_IRQHandler
        ISR_HANDLER HFRCO0_IRQHandler
        ISR_HANDLER CMU_IRQHandler
        ISR_HANDLER AES_IRQHandler
        ISR_HANDLER IADC_IRQHandler
        ISR_HANDLER MSC_IRQHandler
        ISR_HANDLER DPLL0_IRQHandler
        ISR_HANDLER PDM_IRQHandler
        ISR_HANDLER SW0_IRQHandler
        ISR_HANDLER SW1_IRQHandler
        ISR_HANDLER SW2_IRQHandler
        ISR_HANDLER SW3_IRQHandler
        ISR_HANDLER KERNEL0_IRQHandler
        ISR_HANDLER KERNEL1_IRQHandler
        ISR_HANDLER M33CTI0_IRQHandler
        ISR_HANDLER M33CTI1_IRQHandler
        ISR_HANDLER EMUEFP_IRQHandler
        ISR_HANDLER DCDC_IRQHandler
        ISR_HANDLER EUART0_RX_IRQHandler
        ISR_HANDLER EUART0_TX_IRQHandler
#endif
        //
        .section .vectors, "ax"
_vectors_end:

#ifdef __VECTORS_IN_RAM
        //
        // Reserve space with the size of the vector table
        // in the designated RAM section.
        //
        .section .vectors_ram, "ax"
        .balign 256
        .global _vectors_ram

_vectors_ram:
        .space _vectors_end - _vectors, 0
#endif

/*********************************************************************
*
*  Dummy handler to be used for reserved interrupt vectors
*  and weak implementation of interrupts.
*
*/
        .section .init.Dummy_Handler, "ax"
        .thumb_func
        .weak Dummy_Handler
        .balign 2
Dummy_Handler:
        1: b 1b   // Endless loop


/*************************** End of file ****************************/
