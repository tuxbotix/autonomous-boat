/*
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

 $Id$
 */

#ifndef _MIRF_H_
#define _MIRF_H_
#include "SPI.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
// Nrf24l settings
/*
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

 $Id$
 */

/* Memory Map */
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define CD          0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17

/* Bit Mnemonics */
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRCO        2
#define PWR_UP      1
#define PRIM_RX     0
#define ENAA_P5     5
#define ENAA_P4     4
#define ENAA_P3     3
#define ENAA_P2     2
#define ENAA_P1     1
#define ENAA_P0     0
#define ERX_P5      5
#define ERX_P4      4
#define ERX_P3      3
#define ERX_P2      2
#define ERX_P1      1
#define ERX_P0      0
#define AW          0
#define ARD         4
#define ARC         0
#define PLL_LOCK    4
#define RF_DR       3
#define RF_PWR      1
#define LNA_HCURR   0
#define RX_DR       6
#define TX_DS       5
#define MAX_RT      4
#define RX_P_NO     1
#define TX_FULL     0
#define PLOS_CNT    4
#define ARC_CNT     0
#define TX_REUSE    6
#define FIFO_FULL   5
#define TX_EMPTY    4
#define RX_FULL     1
#define RX_EMPTY    0

/* Instruction Mnemonics */
#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define REGISTER_MASK 0x1F
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define REUSE_TX_PL   0xE3
#define NOP           0xFF

#define mirf_ADDR_LEN	5
#define mirf_CONFIG ((1<<EN_CRC) | (0<<CRCO) )

void nrf24l01p_init(uint8_t ssi_port_number);
void nrf24l01p_config(void);
void nrf24l01p_send(uint8_t *value);
void nrf24l01p_setRADDR(uint8_t * adr);
void nrf24l01p_setTADDR(uint8_t * adr);
bool nrf24l01p_dataReady(void);
bool nrf24l01p_isSending(void);
bool nrf24l01p_rxFifoEmpty(void);
bool nrf24l01p_txFifoEmpty(void);
void nrf24l01p_getData(uint8_t * data);
uint8_t nrf24l01p_getStatus(void);
void nRF24L01_IRQ(unsigned char *IRQ_Source,unsigned char *recDataBuffer);
void nrf24l01p_transmitSync(uint8_t *dataout, uint8_t len);
void nrf24l01p_transferSync(uint8_t *dataout, uint8_t *datain, uint8_t len);
void nrf24l01p_configRegister(uint8_t reg, uint8_t value);
void nrf24l01p_readRegister(uint8_t reg, uint8_t * value, uint8_t len);
void nrf24l01p_writeRegister(uint8_t reg, uint8_t * value, uint8_t len);
void nrf24l01p_powerUpRx(void);
void nrf24l01p_powerUpTx(void);
void nrf24l01p_powerDown(void);
void nrf24l01p_setPayLoad(uint8_t payload_size);
void nrf24l01p_setChannel(uint8_t channel_number);

void nrf24l01p_csnHi(void);
void nrf24l01p_csnLow(void);

void nrf24l01p_ceHi(void);
void nrf24l01p_ceLow(void);
void nrf24l01p_flushRx(void);

/*
 * In sending mode.
 */

uint8_t PTX;

/*
 * Channel 0 - 127 or 0 - 84 in the US.
 */
uint8_t channel;

/*
 * Payload width in bytes default 16 max 32.
 */

uint8_t payload;

#endif /* _MIRF_H_ */
