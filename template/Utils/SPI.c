/*
 * Modified version of SPI master library from Energia IDE
 * source IDE version : energia-0101E0012
 * Changes:
 * * Porting from C++ syntax to C syntax, Removed OOP aspects
 * * SPI_init() overloaded functions (2) split in to two;
 * 		1. SPI_init() take no arguement will initialize default SPI port
 * 		2. SPI_init_module(module) takes SSI module number
 * *added ROM_SysCtlPeripheralEnable(g_ulSSIPort[SSIModule]) line
 *
 */

/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "SPI.h"

#include <stdint.h>

#define SSIBASE g_ulSSIBase[SSIModule]
#define NOT_ACTIVE 0xA

uint8_t SSIModule;

static const unsigned long g_ulSSIBase[4] = {
SSI0_BASE, SSI1_BASE, SSI2_BASE, SSI3_BASE };

//*****************************************************************************
//
// The list of SSI peripherals.
//
//*****************************************************************************
static const unsigned long g_ulSSIPeriph[4] = {
SYSCTL_PERIPH_SSI0, SYSCTL_PERIPH_SSI1,
SYSCTL_PERIPH_SSI2, SYSCTL_PERIPH_SSI3 };

//*****************************************************************************
//
// The list of SSI gpio configurations.
//
//*****************************************************************************
static const unsigned long g_ulSSIConfig[4][4] = { { GPIO_PA2_SSI0CLK,
GPIO_PA3_SSI0FSS, GPIO_PA4_SSI0RX,
GPIO_PA5_SSI0TX }, { GPIO_PF2_SSI1CLK, GPIO_PF3_SSI1FSS,
GPIO_PF0_SSI1RX, GPIO_PF1_SSI1TX }, { GPIO_PB4_SSI2CLK,
GPIO_PB5_SSI2FSS, GPIO_PB6_SSI2RX, GPIO_PB7_SSI2TX }, {
GPIO_PD0_SSI3CLK, GPIO_PD1_SSI3FSS, GPIO_PD2_SSI3RX,
GPIO_PD3_SSI3TX }, };

//*****************************************************************************
//
// The list of SSI gpio port bases.
//
//*****************************************************************************
static const unsigned long g_ulSSIPort[4] = {
GPIO_PORTA_BASE, GPIO_PORTF_BASE, GPIO_PORTB_BASE, GPIO_PORTD_BASE };

//*****************************************************************************
//
// The list of SSI gpio configurations.
//
//*****************************************************************************
static const unsigned long g_ulSSIPins[4] = {
GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5,
GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 };

/*
 * C++ init(uint8_t module) replaced with SPI_init_module(uint8_t module_number)
 */
//void SPI_init_module(uint8_t module) {
//	SSIModule = module;
//}
void SPI_init(void) {
	SSIModule = NOT_ACTIVE;

	SPI_begin();
}

/*
 * Replacement for overloaded SPI->init(uint8_t module) method
 */
void SPI_init_module(uint8_t module_number) {
	SSIModule = module_number;

	SPI_begin();
}
void SPI_begin() {

	unsigned long initialData = 0;

	if (SSIModule == NOT_ACTIVE) {
		SSIModule = BOOST_PACK_SPI;
	}

	ROM_SysCtlPeripheralEnable(g_ulSSIPeriph[SSIModule]);
	ROM_SysCtlPeripheralEnable(g_ulSSIPort[SSIModule]);
	ROM_SSIDisable(SSIBASE);


	ROM_GPIOPinConfigure(g_ulSSIConfig[SSIModule][0]);
	ROM_GPIOPinConfigure(g_ulSSIConfig[SSIModule][1]);
	ROM_GPIOPinConfigure(g_ulSSIConfig[SSIModule][2]);
	ROM_GPIOPinConfigure(g_ulSSIConfig[SSIModule][3]);

	ROM_GPIOPinTypeSSI(g_ulSSIPort[SSIModule], g_ulSSIPins[SSIModule]);

	/*
	 Polarity Phase        Mode
	 0 	   0   SSI_FRF_MOTO_MODE_0
	 0     1   SSI_FRF_MOTO_MODE_1
	 1     0   SSI_FRF_MOTO_MODE_2
	 1     1   SSI_FRF_MOTO_MODE_3
	 */

	/*
	 * Default to
	 * System Clock, SPI_MODE_0, MASTER,
	 * 4MHz bit rate, and 8 bit data
	 */
	ROM_SSIClockSourceSet(SSIBASE, SSI_CLOCK_SYSTEM);
	ROM_SSIConfigSetExpClk(SSIBASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
	SSI_MODE_MASTER, 100000, 8);
	ROM_SSIEnable(SSIBASE);

	//clear out any initial data that might be present in the RX FIFO
	while (ROM_SSIDataGetNonBlocking(SSIBASE, &initialData));

}

void SPI_end() {
	ROM_SSIDisable(SSIBASE);
}
/*
 void SPI_setBitOrder(uint8_t ssPin, uint8_t bitOrder)
 {
 }

 void SPI_setBitOrder(uint8_t bitOrder)
 {
 }
 */
void SPI_setDataMode(uint8_t mode) {

	HWREG(SSIBASE + SSI_O_CR0) &= ~(SSI_CR0_SPO | SSI_CR0_SPH);

	HWREG(SSIBASE + SSI_O_CR0) |= mode;

}

void SPI_setClockDivider(uint8_t divider) {

	//value must be even
	HWREG(SSIBASE + SSI_O_CPSR) = divider;

}

uint8_t SPI_transfer(uint8_t data) {

	unsigned long rxData;

	ROM_SSIDataPut(SSIBASE, data);

	while (ROM_SSIBusy(SSIBASE));

	ROM_SSIDataGet(SSIBASE, &rxData);

	return (uint8_t) rxData;

}

void SPI_setModule(uint8_t module) {
	SSIModule = module;
	SPI_begin();
}
