/*
* Copyright (c) 2012, Mauro Scomparin
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Mauro Scomparin nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY Mauro Scomparin ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL Mauro Scomparin BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* File:			LM4F_startup.c.
* Author:		Mauro Scomparin <http://scompoprojects.worpress.com>.
* Version:		1.0.0.
* Description:	LM4F120H5QR startup code.
*/

//-----------------------------------------------------------------------------
// 							 Functions declarations
//-----------------------------------------------------------------------------

// Main should be defined on your main file so it's extern.
extern int main(void);

extern int uart3InturruptHandle(void);

extern int uart4InturruptHandle(void);
// rst_handler contains the code to run on reset.
void rst_handler(void);
// nmi_handler it's the code for an non maskerable interrupt.
void nmi_handler(void);
// this is just the default handler.
void empty_def_handler(void);
// this is the code for an hard fault.
void hardfault_handler(void);
void TimerIntHandlerServos(void);
void SysTickIntHandler(void);
void portAIntHandler(void);

//-----------------------------------------------------------------------------
// 						     Variables declarations
//-----------------------------------------------------------------------------

// defined by the linker it's the stack top variable (End of ram)
extern unsigned long _stack_top;
// defined by the liker, this are just start and end marker for each section.
// .text (code)
extern unsigned long _start_text;
extern unsigned long _end_text;
// .data (data to be copied on ram)
extern unsigned long _start_data;
extern unsigned long _end_data;
// .bss (uninitialized data to set to 0);
extern unsigned long _start_bss;
extern unsigned long _end_bss;

// NVIC ISR table
// the funny looking void(* myvectors[])(void) basically it's a way to make cc accept an array of function pointers.
__attribute__ ((section(".nvic_table")))
void(* myvectors[])(void) = {
	// This are the fixed priority interrupts and the stack pointer loaded at startup at R13 (SP).
	//												VECTOR N (Check Datasheet)
	// here the compiler it's boring.. have to figure that out
    (void (*)) &_stack_top,
    						// stack pointer should be
							// placed here at startup.			0
    rst_handler,			// code entry point					1
    nmi_handler,			// NMI handler.						2
    hardfault_handler,		// hard fault handler.				3
    // Configurable priority interruts handler start here.
    empty_def_handler,                      // The MPU fault handler
    empty_def_handler,                      // The bus fault handler
    empty_def_handler,                      // The usage fault handler
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    empty_def_handler,                      // SVCall handler
    empty_def_handler,                      // Debug monitor handler
    0,                                      // Reserved
    empty_def_handler,                      // The PendSV handler
    SysTickIntHandler,                      // The SysTick handler
    portAIntHandler,                      // GPIO Port A
    empty_def_handler,                      // GPIO Port B
    empty_def_handler,                      // GPIO Port C
    empty_def_handler,                      // GPIO Port D
    empty_def_handler,                      // GPIO Port E
    empty_def_handler,                      // UART0 Rx and Tx
    uart4InturruptHandle,// UART1 Rx and Tx
    empty_def_handler,                      // SSI0 Rx and Tx
    empty_def_handler,                      // I2C0 Master and Slave
    empty_def_handler,                      // PWM Fault
    empty_def_handler,                      // PWM Generator 0
    empty_def_handler,                      // PWM Generator 1
    empty_def_handler,                      // PWM Generator 2
    empty_def_handler,                      // Quadrature Encoder 0
    empty_def_handler,                      // ADC Sequence 0
    empty_def_handler,                      // ADC Sequence 1
    empty_def_handler,                      // ADC Sequence 2
    empty_def_handler,                      // ADC Sequence 3
    empty_def_handler,                      // Watchdog timer
    empty_def_handler,                      // Timer 0 subtimer A
    empty_def_handler,                      // Timer 0 subtimer B
    empty_def_handler,                      // Timer 1 subtimer A
    empty_def_handler,                      // Timer 1 subtimer B
    TimerIntHandlerServos,                      // Timer 2 subtimer A
    empty_def_handler,                      // Timer 2 subtimer B
    empty_def_handler,                      // Analog Comparator 0
    empty_def_handler,                      // Analog Comparator 1
    empty_def_handler,                      // Analog Comparator 2
    empty_def_handler,                      // System Control (PLL, OSC, BO)
    empty_def_handler,                      // FLASH Control
    empty_def_handler,                      // GPIO Port F
    empty_def_handler,                      // GPIO Port G
    empty_def_handler,                      // GPIO Port H
           empty_def_handler,                      // UART2 Rx and Tx
    empty_def_handler,                      // SSI1 Rx and Tx
    empty_def_handler,                      // Timer 3 subtimer A
    empty_def_handler,                      // Timer 3 subtimer B
    empty_def_handler,                      // I2C1 Master and Slave
    empty_def_handler,                      // Quadrature Encoder 1
    empty_def_handler,                      // CAN0
    empty_def_handler,                      // CAN1
    empty_def_handler,                      // CAN2
    empty_def_handler,                      // Ethernet
    empty_def_handler,                      // Hibernate
    empty_def_handler,                      // USB0
    empty_def_handler,                      // PWM Generator 3
    empty_def_handler,                      // uDMA Software Transfer
    empty_def_handler,                      // uDMA Error
    empty_def_handler,                      // ADC1 Sequence 0
    empty_def_handler,                      // ADC1 Sequence 1
    empty_def_handler,                      // ADC1 Sequence 2
    empty_def_handler,                      // ADC1 Sequence 3
    empty_def_handler,                      // I2S0
    empty_def_handler,                      // External Bus Interface 0
    empty_def_handler,                      // GPIO Port J
    empty_def_handler,                      // GPIO Port K
    empty_def_handler,                      // GPIO Port L
    empty_def_handler,                      // SSI2 Rx and Tx
    empty_def_handler,                      // SSI3 Rx and Tx
    uart3InturruptHandle,          // UART3 Rx and Tx
    empty_def_handler,                     // UART4 Rx and Tx
    empty_def_handler,                      // UART5 Rx and Tx
    empty_def_handler,                      // UART6 Rx and Tx
    empty_def_handler,                      // UART7 Rx and Tx
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    empty_def_handler,                      // I2C2 Master and Slave
    empty_def_handler,                      // I2C3 Master and Slave
    empty_def_handler,                      // Timer 4 subtimer A
    empty_def_handler,                      // Timer 4 subtimer B
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    empty_def_handler,                      // Timer 5 subtimer A
    empty_def_handler,                      // Timer 5 subtimer B
    empty_def_handler,                      // Wide Timer 0 subtimer A
    empty_def_handler,                      // Wide Timer 0 subtimer B
    empty_def_handler,                      // Wide Timer 1 subtimer A
    empty_def_handler,                      // Wide Timer 1 subtimer B
    empty_def_handler,                      // Wide Timer 2 subtimer A
    empty_def_handler,                      // Wide Timer 2 subtimer B
    empty_def_handler,                      // Wide Timer 3 subtimer A
    empty_def_handler,                      // Wide Timer 3 subtimer B
    empty_def_handler,                      // Wide Timer 4 subtimer A
    empty_def_handler,                      // Wide Timer 4 subtimer B
    empty_def_handler,                      // Wide Timer 5 subtimer A
    empty_def_handler,                      // Wide Timer 5 subtimer B
    empty_def_handler,                      // FPU
    empty_def_handler,                      // PECI 0
    empty_def_handler,                      // LPC 0
    empty_def_handler,                      // I2C4 Master and Slave
    empty_def_handler,                      // I2C5 Master and Slave
    empty_def_handler,                      // GPIO Port M
    empty_def_handler,                      // GPIO Port N
    empty_def_handler,                      // Quadrature Encoder 2
    empty_def_handler,                      // Fan 0
    0,                                      // Reserved
    empty_def_handler,                      // GPIO Port P (Summary or P0)
    empty_def_handler,                      // GPIO Port P1
    empty_def_handler,                      // GPIO Port P2
    empty_def_handler,                      // GPIO Port P3
    empty_def_handler,                      // GPIO Port P4
    empty_def_handler,                      // GPIO Port P5
    empty_def_handler,                      // GPIO Port P6
    empty_def_handler,                      // GPIO Port P7
    empty_def_handler,                      // GPIO Port Q (Summary or Q0)
    empty_def_handler,                      // GPIO Port Q1
    empty_def_handler,                      // GPIO Port Q2
    empty_def_handler,                      // GPIO Port Q3
    empty_def_handler,                      // GPIO Port Q4
    empty_def_handler,                      // GPIO Port Q5
    empty_def_handler,                      // GPIO Port Q6
    empty_def_handler,                      // GPIO Port Q7
    empty_def_handler,                      // GPIO Port R
    empty_def_handler,                      // GPIO Port S
    empty_def_handler,                      // PWM 1 Generator 0
    empty_def_handler,                      // PWM 1 Generator 1
    empty_def_handler,                      // PWM 1 Generator 2
    empty_def_handler,                      // PWM 1 Generator 3
    empty_def_handler                       // PWM 1 Fault
};

//-----------------------------------------------------------------------------
// 							Function implementations.
//-----------------------------------------------------------------------------

/*
* System on reset code. NVIC 1
* Here I prepare the memory for the c compiler.
* The stack pointer should be set at the beginning with the NVIC table already.
* Copy the .data segment from flash into ram.
* 0 to the .bss segment
*/

void rst_handler(void){
	// Copy the .data section pointers to ram from flash.
	// Look at LD manual (Optional Section Attributes).

	// source and destination pointers
	unsigned long *src;
	unsigned long *dest;

	//this should be good!
	src = &_end_text;
	dest = &_start_data;

	//this too
    while(dest < &_end_data)
    {
        *dest++ = *src++;
    }

    // now set the .bss segment to 0!
    dest = &_start_bss;
	while(dest < &_end_bss){
		*dest++ = 0;
	}

	// after setting copying .data to ram and "zero-ing" .bss we are good
	// to start the main() method!
	// There you go!
	main();
}

// NMI Exception handler code NVIC 2
void nmi_handler(void){
	// Just loop forever, so if you want to debug the processor it's running.
    while(1){
    }
}

// Hard fault handler code NVIC 3
void hardfault_handler(void){
	// Just loop forever, so if you want to debug the processor it's running.
    while(1){
    }
}

// Empty handler used as default.
void empty_def_handler(void){
	// Just loop forever, so if you want to debug the processor it's running.
    while(1){
    }
}
