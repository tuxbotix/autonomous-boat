/* nrf24l01_config.h
 * Configuration of nRF24L01+ Connection pins and other parameters to the MCU.
 * 
 * Written for Jks Liu(chinatianma@gmail.com)'s nrf24l01+ library.
 * Inspired by Eric Brundick (spirilis@linux.com)'s nrf_userconfig.h
 *
 * Copyright (c) 2014, Darshana Adikari <darshana@tuxbotix.net>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose
 * with or without fee is hereby granted, provided that the above copyright notice
 * and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT,
 * OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#define RF_CHANNEL 1
/* GPIO pin connections for CE, IRQ, CSN pins
 */
#define TX_ADR_LENGTH   5                         // 5 bytes TX(RX) address width **** max is 5
#define MAX_TX_PLOAD_WIDTH  32                        // 32 bytes TX payload max

/*
 * CE Pin controls RX / TX, default 8.
 */


#define CE_BASE GPIO_PORTA_BASE
#define CE_PIN  GPIO_PIN_6
#define CS_BASE GPIO_PORTA_BASE
#define CS_PIN  GPIO_PIN_7

#define IRQ_BASE GPIO_PORTA_BASE
#define IRQ_PIN GPIO_PIN_5
