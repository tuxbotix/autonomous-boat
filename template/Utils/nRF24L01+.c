/**
 * nRF24L01+ : Mirf base library ported to Stellaris Launchpad [LM4FXXX]
 *
 * Ported from Arduino to Stellaris Launchpad [LM4FXXX]
 * 	20/04/2014:
 * 		Using Hardware SPI through ported Energia SPI master library (SPI.CPP)
 *		Added basic IRQ support
 * By: Darshana Adikari <darshana@tuxbotix.net>
 *
 * Additional bug fixes and improvements
 *  11/03/2011:
 *   Switched spi library.
 *  07/13/2010:
 *   Added example to read a register
 *  11/12/2009:
 *   Fix dataReady() to work correctly
 *   Renamed keywords to keywords.txt ( for IDE ) and updated keyword list
 *   Fixed client example code to timeout after one second and try again
 *    when no response received from server
 * By: Nathan Isburgh <nathan@mrroot.net>
 * $Id: mirf.cpp 67 2010-07-13 13:25:53Z nisburgh $
 *
 * An Ardunio port of:
 * http://www.tinkerer.eu/AVRLib/nRF24L01
 *
 * Significant changes to remove depencence on interupts and auto ack support.
 *
 * Aaron Shrimpton <aaronds@gmail.com>
 *
 *************************************************************************

 Copyright (c) 2007 Stefan Engelke <mbox@stefanengelke.de>

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use, copy,
 modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.


 */

#include "nRF24L01+.h"
#include "nRF24L01_config.h" // User configurations
#include "SPI.h"
#include <stdbool.h>
#include <stdint.h>

char recDataBuffer[32];
void nrf24l01p_transferSync(uint8_t *dataout, uint8_t *datain, uint8_t len) {
	uint8_t i = 0;
	for (i = 0; i < len; i++) {
		datain[i] = SPI_transfer(dataout[i]);
	}
}

void nrf24l01p_transmitSync(uint8_t *dataout, uint8_t len) {
	uint8_t i;
	for (i = 0; i < len; i++) {
		SPI_transfer(dataout[i]);
	}
}

void nrf24l01p_init(uint8_t ssi_port_number) {
	channel = 1;
	payload = 16;
	GPIOPinTypeGPIOOutput(CE_BASE, CE_PIN);
	GPIOPinTypeGPIOOutput(CS_BASE, CS_PIN);
	GPIOPinTypeGPIOInput(IRQ_BASE, IRQ_PIN);
	GPIOPinWrite(CE_BASE, CE_PIN, 0x00);
	GPIOPinWrite(CS_BASE, CS_PIN, CS_PIN);

	nrf24l01p_ceLow();
	nrf24l01p_csnHi();

	// Initialize spi module;     SPI->begin();

	SPI_init_module(ssi_port_number);

}

void nrf24l01p_config()
// Sets the important registers in the nRF module and powers the module
// in receiving mode
// NB: channel and payload must be set now.
{

	// Set RF channel
	nrf24l01p_configRegister(RF_CH, channel);

	// Set length of incoming payload
	nrf24l01p_configRegister(RX_PW_P0, payload);
	nrf24l01p_configRegister(RX_PW_P1, payload);
	nrf24l01p_configRegister(SETUP_RETR, 0x13); //changing ADR to 500 microsecs.
	//#define SETUP_RETR  0x04
	// Read and print RF_SETUP

	unsigned char rf_setup = 0;
	nrf24l01p_readRegister( RF_SETUP, &rf_setup, sizeof(rf_setup));

	UARTprintf("retr setup %x", rf_setup);
	// Start receiver
	nrf24l01p_powerUpRx();
	nrf24l01p_flushRx();
}

void nrf24l01p_setRADDR(uint8_t * adr)
// Sets the receiving address
{
	nrf24l01p_ceLow();
	nrf24l01p_writeRegister(RX_ADDR_P1, adr, mirf_ADDR_LEN);
	nrf24l01p_ceHi();
}

void nrf24l01p_setTADDR(uint8_t * adr)
// Sets the transmitting address
{
	/*
	 * RX_ADDR_P0 must be set to the sending addr for auto ack to work.
	 */

	nrf24l01p_writeRegister(RX_ADDR_P0, adr, mirf_ADDR_LEN);
	nrf24l01p_writeRegister(TX_ADDR, adr, mirf_ADDR_LEN);
}
void nrf24l01p_setPayLoad(uint8_t payload_size) {
	payload = payload_size;
}
void nrf24l01p_setChannel(uint8_t channel_number) {
	channel = channel_number;
}
extern bool nrf24l01p_dataReady()
// Checks if data is available for reading
{
	// See note in getData() function - just checking RX_DR isn't good enough
	uint8_t status = nrf24l01p_getStatus();

	// We can short circuit on RX_DR, but if it's not set, we still need
	// to check the FIFO for any pending packets
	if (status & (1 << RX_DR))
		return 1;
	return !nrf24l01p_rxFifoEmpty();
}

extern bool nrf24l01p_rxFifoEmpty() {
	uint8_t fifoStatus = 0;

	nrf24l01p_readRegister(FIFO_STATUS, &fifoStatus, sizeof(fifoStatus));
	return (fifoStatus & (1 << RX_EMPTY));
}

extern void nrf24l01p_getData(uint8_t * data)
// Reads payload bytes into data array
{
	nrf24l01p_csnLow();                               // Pull down chip select
	SPI_transfer( R_RX_PAYLOAD);            // Send cmd to read rx payload
	nrf24l01p_transferSync(data, data, payload); // Read payload
	nrf24l01p_csnHi();                               // Pull up chip select
	// NVI: per product spec, p 67, note c:
	//  "The RX_DR IRQ is asserted by a new packet arrival event. The procedure
	//  for handling this interrupt should be: 1) read payload through SPI,
	//  2) clear RX_DR IRQ, 3) read FIFO_STATUS to check if there are more
	//  payloads available in RX FIFO, 4) if there are more data in RX FIFO,
	//  repeat from step 1)."
	// So if we're going to clear RX_DR here, we need to check the RX FIFO
	// in the dataReady() function
	nrf24l01p_configRegister(STATUS, (1 << RX_DR));   // Reset status register
}

void nrf24l01p_configRegister(uint8_t reg, uint8_t value)
// Clocks only one byte into the given MiRF register
{
	nrf24l01p_csnLow();
	SPI_transfer(W_REGISTER | (REGISTER_MASK & reg));
	SPI_transfer(value);
	nrf24l01p_csnHi();
}

void nrf24l01p_readRegister(uint8_t reg, uint8_t * value, uint8_t len)
// Reads an array of bytes from the given start position in the MiRF registers.
{
	nrf24l01p_csnLow();
	SPI_transfer(R_REGISTER | (REGISTER_MASK & reg));
	nrf24l01p_transferSync(value, value, len);
	nrf24l01p_csnHi();
}

extern void nrf24l01p_writeRegister(uint8_t reg, uint8_t * value, uint8_t len)
// Writes an array of bytes into inte the MiRF registers.
{
	nrf24l01p_csnLow();
	SPI_transfer(W_REGISTER | (REGISTER_MASK & reg));
	nrf24l01p_transmitSync(value, len);
	nrf24l01p_csnHi();
}

void nrf24l01p_send(uint8_t * value)
// Sends a data package to the default address. Be sure to send the correct
// amount of bytes as configured as payload on the receiver.
{
	uint8_t status;
	status = nrf24l01p_getStatus();

	while (PTX) {
		status = nrf24l01p_getStatus();

		if ((status & ((1 << TX_DS) | (1 << MAX_RT)))) {
			PTX = 0;
			break;
		}
	}                  // Wait until last packet is sent

	nrf24l01p_ceLow();

	nrf24l01p_powerUpTx();       // Set to transmitter mode , Power up

	nrf24l01p_csnLow();                    // Pull down chip select
	SPI_transfer( FLUSH_TX);     // Write cmd to flush tx fifo
	nrf24l01p_csnHi();                    // Pull up chip select

	nrf24l01p_csnLow();                    // Pull down chip select
	SPI_transfer( W_TX_PAYLOAD); // Write cmd to write payload
	nrf24l01p_transmitSync(value, payload);   // Write payload
	nrf24l01p_csnHi();                    // Pull up chip select

	nrf24l01p_ceHi();                     // Start transmission
}

/**
 * isSending.
 *
 * Test if chip is still sending.
 * When sending has finished return chip to listening.
 *
 */

bool nrf24l01p_isSending() {
	uint8_t status;
	if (PTX) {
		status = nrf24l01p_getStatus();

		/*
		 *  if sending successful (TX_DS) or max retries exceded (MAX_RT).
		 */

		if ((status & ((1 << TX_DS) | (1 << MAX_RT)))) {
			nrf24l01p_powerUpRx();
			return false;
		}

		return true;
	}
	return false;
}

void nRF24L01_IRQ(unsigned char *IRQ_Source, unsigned char * recDataBuffer) // interrupt EXT_INT0
{
	unsigned char temp = 0;
	temp = nrf24l01p_getStatus();

	if (!nrf24l01p_isSending()
			&& ((temp & (1 << RX_DR)) || !nrf24l01p_rxFifoEmpty())) {
		nrf24l01p_csnLow();                             // Pull down chip select
		SPI_transfer( R_RX_PAYLOAD);            // Send cmd to read rx payload
		nrf24l01p_transferSync(recDataBuffer, recDataBuffer, payload); // Read payload
		nrf24l01p_csnHi();                               // Pull up chip select
		*IRQ_Source |= (1 << RX_DR);
	}
	if (temp & (1 << TX_DS)) {
		*IRQ_Source |= (1 << TX_DS);
	}
	if (temp & (1 << MAX_RT)) {
		*IRQ_Source |= (1 << MAX_RT);
	}
	nrf24l01p_configRegister(STATUS, *IRQ_Source);

}

uint8_t nrf24l01p_getStatus() {
	uint8_t rv = 0;
	nrf24l01p_readRegister(STATUS, &rv, 1);
	return rv;
}

void nrf24l01p_powerUpRx() {
	PTX = 0;
	nrf24l01p_ceLow();
	nrf24l01p_configRegister(CONFIG,
	mirf_CONFIG | ((1 << PWR_UP) | (1 << PRIM_RX)));
	nrf24l01p_ceHi();
	nrf24l01p_configRegister(STATUS, (1 << TX_DS) | (1 << MAX_RT));
}

void nrf24l01p_flushRx() {
	nrf24l01p_csnLow();
	SPI_transfer( FLUSH_RX);
	nrf24l01p_csnHi();
}

void nrf24l01p_powerUpTx() {
	PTX = 1;
	nrf24l01p_configRegister(CONFIG,
	mirf_CONFIG | ((1 << PWR_UP) | (0 << PRIM_RX)));
}

void nrf24l01p_ceHi() {
	GPIOPinWrite(CE_BASE, CE_PIN, CE_PIN);
}

void nrf24l01p_ceLow() {
	GPIOPinWrite(CE_BASE, CE_PIN, 0x00);

}

void nrf24l01p_csnHi() {
	GPIOPinWrite(CS_BASE, CS_PIN, CS_PIN);
}

void nrf24l01p_csnLow() {
	GPIOPinWrite(CS_BASE, CS_PIN, 0x00);

}

void nrf24l01p_powerDown() {
	nrf24l01p_ceLow();
	nrf24l01p_configRegister(CONFIG, mirf_CONFIG);
}
