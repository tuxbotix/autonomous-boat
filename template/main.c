/*
 * Copyright (c) 2012, Mauro Scomparin : The template
 * Copyright (c) 2012, The A-Team : The other code
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
 * File:			main.c.
 * Author:		Mauro Scomparin <http://scompoprojects.worpress.com>.
 * Version:		1.0.0.
 * Description:	Main sample file.
 */
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

#include "driverlib/uart.h"
#include "driverlib/i2c.h"
#include "driverlib/systick.h"
#include "utils/mpu_accl.h"
#include "utils/I2C_Stellaris_API_r02/I2C_Stellaris_API.h"
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"
#include "Utils/delay.h"

#define SERVO_MAX_COUNT 1
//#define wlan
#include "utils/servo.h"
#include "Utils/nRF24L01+.h"
#include "Utils/nRF24L01_config.h"
//#include "control.h"

#define SERIAL_BUFFER_MAX 40
#define DATA_BUFFER_MAX 32

#define MAXFIELD    25                // maximum field length
#define dt 1
#define TICKFREQ 1/dt
#define GYROSCOPE_SENSITIVITY 3

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define ACCL_GAIN 0.612915625
#define MAG_GAIN 1
unsigned char ACC_Data[6];
int16_t Ax, Ay, Az, Mx, My, Mz;
unsigned char *pucBuffer;
volatile bool message = false;

volatile bool toggle = false;
unsigned char gpsSentence[2][83]; // max nmea length = 82, with /0 null, its 83
unsigned int gpsTime;
float gpsLattitude;
float gpsLongitude;
unsigned char gpsBufferSize[2];
unsigned char gpsSentenceRMC[83];
unsigned char gpsSentenceVTG[83];
unsigned char recDataBuffer[DATA_BUFFER_MAX];
volatile int j = 0;
volatile short i;
char gpsLatestSentenceType=0; //1=GPRMC, 2= GPVTG,
int gpsLat[2] = { 0, 0 };
int gpsLon[2] = { 0, 0 };
unsigned char IRQ_Source;
servo_t *servo;
// An interrupt function.
bool isGPSChecksumPass(unsigned char *s);
void readAccel(void);
void sendAcclData(void);
void sendGPSData(void);
void sortGPSSentence(void);
void sendMagnetoData(void);
void readMagneto(void);

float getHeading(void);
void sendHeading(void);
double getRoll(void);
double getPitch(void);
void setupServos(void);
void portAIntHandler(void);
float my_strtof(unsigned char* input, int* outputPieces);
bool GetField(unsigned char *pData, unsigned char *pField, int nFieldNum,
		int nMaxFieldLen);
unsigned char* my_strcpy(unsigned char *dest, unsigned const char *src,
		size_t size);
void ComplementaryFilter(short accData[3], short gyrData[3], float *pitch,
		float *roll);
#define DEBUG
#ifdef DEBUG
void InitConsole(void) {
	//
	// Enable GPIO port A which is used for UART0 pins.
	// TODO: change this to whichever GPIO port you are using.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	//
	// Configure the pin muxing for UART0 functions on port A0 and A1.
	// This step is not necessary if your part does not support pin muxing.
	// TODO: change this to select the port/pin you are using.
	//
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);

	//
	// Select the alternate (UART) function for these pins.
	// TODO: change this to select the port/pin you are using.
	//
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	//
	// Initialize the UART for console I/O.
	//
	UARTStdioInit(0);
}
#endif
int main(void) {

	ROM_FPUEnable();
	ROM_FPULazyStackingEnable();

	ROM_SysCtlClockSet(
	SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	SysTickPeriodSet(SysCtlClockGet() / TICKFREQ); //tickfreq in Hz
	SysTickIntEnable();
	SysTickEnable();
#ifdef DEBUG
	InitConsole();
#endif
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

	ROM_IntMasterEnable();

	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,
	GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

	GPIOPinConfigure(GPIO_PC6_U3RX);
	GPIOPinConfigure(GPIO_PC7_U3TX);
	GPIOPinConfigure(GPIO_PB0_U1RX);
	GPIOPinConfigure(GPIO_PB1_U1TX);

	ROM_GPIOPinTypeUART(GPIO_PORTC_BASE,
	GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	ROM_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	ROM_UARTFIFOEnable(UART1_BASE);
	ROM_UARTFIFOEnable(UART3_BASE);

	ROM_UARTConfigSetExpClk(UART3_BASE, ROM_SysCtlClockGet(), 115200,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
			UART_CONFIG_PAR_NONE));
	ROM_UARTConfigSetExpClk(UART1_BASE, ROM_SysCtlClockGet(), 38400,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
			UART_CONFIG_PAR_NONE));

	ROM_IntEnable(INT_UART1);
	ROM_UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT); // ONLY recieve inturrupt

	ROM_IntEnable(INT_UART3);
	ROM_UARTIntEnable(UART3_BASE, UART_INT_RX | UART_INT_RT); // ONLY recieve inturrupt

	I2CSetup(I2C0_MASTER_BASE, true);

	setupServos();
	UARTprintf("init");
	nrf24l01p_init(3);
	nrf24l01p_setRADDR((uint8_t *) "Boat");
	nrf24l01p_setPayLoad(DATA_BUFFER_MAX);
	nrf24l01p_setChannel(1);
	nrf24l01p_config();
	nrf24l01p_setTADDR((uint8_t *) "Base");

//	nrf24l01p_send((unsigned char *) &"Init");
	GPIOPortIntRegister(GPIO_PORTA_BASE, portAIntHandler);
	GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_FALLING_EDGE);
	GPIOPinIntEnable(GPIO_PORTA_BASE, GPIO_PIN_5);
	unsigned char p = 0;
	nrf24l01p_readRegister(0x00, &p, 1);
	UARTprintf("   ");

	while (1) {

		sortGPSSentence();
	}
}

// The interrupt function definition.
/*
 * systick - All the IMU calculations called through this**
 *
 */
void SysTickIntHandler(void) {
	//nrf24l01p_send("p");
//	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);

	//Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.

	//Turn off the LED

	//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);

}
void portAIntHandler(void) {
	GPIOPinIntClear(GPIO_PORTA_BASE, GPIO_PIN_5);

	unsigned char irq = 0;
	nRF24L01_IRQ(&irq, recDataBuffer);
	if (irq & (1 << RX_DR)) {

		delay_ms(1); //IMPORTANT, delay > max retransmission delay

		if (strncmp((char*) recDataBuffer, "gps", 3) == 0) { // && (strncmp((char*) ble, "accl", 4) == 0)

			//nrf24l01p_send((unsigned char*) "lablala");
			//					UARTCharPutNonBlocking(UART3_BASE, 'p');
			sendGPSData();

			//
		} else if (strncmp((char*) recDataBuffer, "accl", 4) == 0) {
			sendAcclData();

		} else if (strncmp((char*) recDataBuffer, "mag", 3) == 0) {
			//sendMagnetoData();
			sendHeading();

		} else if (strncmp((char*) recDataBuffer, "ser", 3) == 0) {
			if (recDataBuffer[3] == '+') {
				servoSet(servo, 1800);
			} else if (recDataBuffer[3] == '-') {
				servoSet(servo, 1388);
			} else {
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

				//Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.

				SysCtlDelay(SysCtlClockGet() / (1000 * 3));

				//Turn off the LED

				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);

				servoSet(servo, 1500);
			}

		}

	}

	//UARTprintf("irq %x", irq);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
	SysCtlDelay(SysCtlClockGet() / (1000 * 2));
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);

#ifdef DEBUG
	if (irq & (1 << MAX_RT)) {
		UARTprintf("Max_RT.\n");
	}
	if (irq & (1 << TX_DS)) {
		//UARTprintf("TX_DS.\n");
	}
	if (irq & (1 << RX_DR)) {
		UARTprintf("RX_DR\n");
	}
#endif

}
/**
 * UART 3 - for wifi
 */
void uart3InturruptHandle(void) {
	unsigned long ulStatus;
	unsigned char ble[15];
	ble[14] = '\0';
	unsigned long val;
	bool msg = false;

//
// Get the interrrupt status.
//
	ulStatus = ROM_UARTIntStatus(UART3_BASE, true);

//
// Clear the asserted interrupts.
//
	ROM_UARTIntClear(UART3_BASE, ulStatus);

//
// Loop while there are characters in the receive FIFO.
//
	while (UARTCharsAvail(UART3_BASE)) {
		//
		// Read the next character from the UART and write it back to the UART.
		//
		val = UARTCharGetNonBlocking(UART3_BASE);
		ble[i] = val;
		if (val == '\n' || val == '\r' || i == 13) {
			i = -1;
			msg = true;
		}

//		UARTCharPutNonBlocking(UART3_BASE, val);
		i++;
	}
	if (msg) {
		if (strncmp((char*) ble, "gps", 3) == 0) {// && (strncmp((char*) ble, "accl", 4) == 0)
//					UARTCharPutNonBlocking(UART3_BASE, 'p');
			sendGPSData();
			msg = false;
//
		} else if (strncmp((char*) ble, "accl", 4) == 0) {
			sendAcclData();
			msg = false;
		} else if (strncmp((char*) ble, "mag", 3) == 0) {
			//sendMagnetoData();
			sendHeading();
			msg = false;
		} else if (strncmp((char*) ble, "ser", 3) == 0) {
			if (ble[3] == '+') {
				servoSet(servo, 1800);
			} else if (ble[3] == '-') {
				servoSet(servo, 1388);
			} else {
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

				//Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.

				SysCtlDelay(SysCtlClockGet() / (1000 * 3));

				//Turn off the LED

				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);

				servoSet(servo, 1500);
			}

		}
		msg = false;
	}

}
/**
 * UART 1 - for gps module
 */
void uart4InturruptHandle(void) {
	unsigned long ulStatus1, val;
	ulStatus1 = ROM_UARTIntStatus(UART1_BASE, true);
	ROM_UARTIntClear(UART1_BASE, ulStatus1);

	while (UARTCharsAvail(UART1_BASE)) {
		val = UARTCharGetNonBlocking(UART1_BASE);
//    inputBuffer[j]=val;

		if ((val == '$')) {
			//gpsSentence[1][82] = '\0';
			gpsSentence[!toggle][j] = '\0';
			gpsBufferSize[!toggle] = j;
//			gpsSentence[!toggle][0] = j;
			j = 0;	//0 index to hold end point
			toggle = !toggle;
			message = true;

		}
		gpsSentence[!toggle][j] = val;

		if (val == '\n' || val == '\r' || val == ' ' || val == '\t') {
			j--; //if some newline is in the message, simply ignore that charater by
			//shifting the increment backwardsso
		}

		j++;
	}

}

/**
 * is gps checksum pass
 */
bool isGPSChecksumPass(unsigned char *s) {

	int calculatedCheckSum = 0;

	unsigned char curChecksum[3];
	curChecksum[0] = s[(gpsBufferSize[toggle] - 2)];
	curChecksum[1] = s[(gpsBufferSize[toggle] - 1)];
	curChecksum[2] = '\0';

	int givenCheckSum = (int) ustrtoul((char*) curChecksum, NULL, 16);
	//if (givenCheckSum <= 0) {
	for (int i = 1; i < (gpsBufferSize[toggle] - 3); i++) {
		calculatedCheckSum ^= s[i];

	}

	if (calculatedCheckSum == givenCheckSum) {
		return true;
	} else
		return false;
	/*} else {
	 return false;
	 }*/
}

/**
 * Read magnetometer (raw values)
 */
void readMagneto(void) {
	int e, f;
	unsigned char temp, MR_Data[6];
	e = I2CRegWrite(I2C0_MASTER_BASE, 0x1E, 0x00, 0x14);//set CRA_REG_M register
	f = I2CRegWrite(I2C0_MASTER_BASE, 0x1E, 0x01, 0x20);
	f = I2CRegWrite(I2C0_MASTER_BASE, 0x1E, 0x02, 0x00);//set MR_REG_M register

	temp = I2CRegRead(I2C0_MASTER_BASE, 0x1E, 0x02); //read MR_REG_M
	MR_Data[0] = I2CRegRead(I2C0_MASTER_BASE, 0x1E, 0x03); //read OUT_X_H_M (MSB)
	MR_Data[1] = I2CRegRead(I2C0_MASTER_BASE, 0x1E, 0x04); //read OUT_X_L_M (LSB)
	MR_Data[2] = I2CRegRead(I2C0_MASTER_BASE, 0x1E, 0x07); //read OUT_Y_H_M (MSB)
	MR_Data[3] = I2CRegRead(I2C0_MASTER_BASE, 0x1E, 0x08); //read OUT_Y_L_M (LSB)
	MR_Data[4] = I2CRegRead(I2C0_MASTER_BASE, 0x1E, 0x05); //read OUT_Z_H_M (MSB)
	MR_Data[5] = I2CRegRead(I2C0_MASTER_BASE, 0x1E, 0x06); //read OUT_Z_L_M (LSB)
	Mx = (int16_t) (MR_Data[0] << 8) + MR_Data[1];
	My = (int16_t) (MR_Data[2] << 8) + MR_Data[3];
	Mz = (int16_t) (MR_Data[4] << 8) + MR_Data[5];

	if (e == 0 || f == 0 || temp != 0) {
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);

		//Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.

		SysCtlDelay(SysCtlClockGet() / (1000 * 3));

		//Turn off the LED

		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
	}

}
/*
 *  Read Accelerometer (raw values)
 */
void readAccel(void) {

	I2CRegWrite(I2C0_MASTER_BASE, 0x18, 0x20, 0x27); //set CTRL_REG1_A register
	I2CRegWrite(I2C0_MASTER_BASE, 0x18, 0x23, 0x40); //set CTRL_REG1_A register

	ACC_Data[0] = I2CRegRead(I2C0_MASTER_BASE, 0x18, 0x28);
	ACC_Data[1] = I2CRegRead(I2C0_MASTER_BASE, 0x18, 0x29);
	ACC_Data[2] = I2CRegRead(I2C0_MASTER_BASE, 0x18, 0x2A);
	ACC_Data[3] = I2CRegRead(I2C0_MASTER_BASE, 0x18, 0x2B);
	ACC_Data[4] = I2CRegRead(I2C0_MASTER_BASE, 0x18, 0x2C);
	ACC_Data[5] = I2CRegRead(I2C0_MASTER_BASE, 0x18, 0x2D);
	Ax = (int16_t) (ACC_Data[0] << 8) + ACC_Data[1];
	Ay = (int16_t) (ACC_Data[2] << 8) + ACC_Data[3];
	Az = (int16_t) (ACC_Data[4] << 8) + ACC_Data[5];

	Ax = (int16_t) Ax * ACCL_GAIN;
	Ay = (int16_t) Ay * ACCL_GAIN;
	Az = (int16_t) Az * ACCL_GAIN;

}

/*
 * send mag. values to uart 3
 */
void sendMagnetoData(void) {
	readMagneto();
	short b;
	unsigned char mag[20];
	mag[19] = '\0';
	b = usprintf((char*) mag, "%i,%i,%i\n", Mx, My, Mz);
#ifndef wlan
	nrf24l01p_send(mag);
#else
	for (int i = 0; i < b; i++) {
		ROM_UARTCharPut(UART3_BASE, mag[i]);
	}

#endif
}
/*
 * send acc. data to uart 3
 */
void sendAcclData(void) {
	readAccel();
	int aMagnitude = abs(Ax) + abs(Ay) + abs(Az);
	short b;
	unsigned char accl[20];
	accl[19] = '\0';
	b = usprintf((char*) accl, "%i,%i,%i\n", Ax, Ay, Az);
#ifndef wlan
	nrf24l01p_send(accl);
#else
	for (int i = 0; i < b; i++) {
		ROM_UARTCharPut(UART3_BASE, accl[i]);
	}
	//ROM_UARTCharPut(UART3_BASE, '\n');

#endif
	b = usprintf((char*) accl, "%i\n", aMagnitude);
#ifndef wlan
	nrf24l01p_send(accl);
#else
	for (int i = 0; i < b; i++) {
		ROM_UARTCharPut(UART3_BASE, accl[i]);
	}
	//ROM_UARTCharPut(UART3_BASE, '\n');

#endif

}
/*
 * send gps sentences to uart
 */
void sendGPSData(void) {

	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
	SysCtlDelay(SysCtlClockGet() / (1000 * 2));
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);

	if (gpsLatestSentenceType == 1) {
		char t[50] = "";
		if (gpsTime != 0) {

			usprintf(t, "$GPCO,%i,%i.%i,%i.%i*\n", gpsTime, gpsLat[0],
					gpsLat[1], gpsLon[0], gpsLon[1]);

		} else {
			usprintf(t, "$GPCO,N,N.N,N.N*\n");
		}
#ifndef wlan

		nrf24l01p_send((unsigned char*) t);
#else
		for (int i = 0; i < 50; i++) {
			if (t[i] == '\0') {
				break;
			}
			UARTCharPut(UART3_BASE, t[i]);

		}
		//UARTCharPut(UART3_BASE, '\n');
#endif
	} else if(gpsLatestSentenceType==2){
#ifndef wlan
		nrf24l01p_send((unsigned char*) gpsSentenceVTG);
		nrf24l01p_send((unsigned char*) '\n');
#else
		for (int i = 0; i < 83; i++) {
			if (gpsSentenceVTG[i] == '\0') {
				break;
			}
			UARTCharPut(UART3_BASE, gpsSentenceVTG[i]);

		}
		//UARTCharPut(UART3_BASE, '\n');
#endif

	}else{
#ifndef wlan
		nrf24l01p_send((unsigned char*)"$GPCO,N,N.N,N.N*\n");
		//nrf24l01p_send((unsigned char*) '\n');
#else
		for (int i = 0; i < 83; i++) {
			if (gpsSentenceVTG[i] == '\0') {
				break;
			}
			UARTCharPut(UART3_BASE, gpsSentenceVTG[i]);

		}
		//UARTCharPut(UART3_BASE, '\n');
#endif

	}

}
/*
 * send gps sentences to uart
 */
void sortGPSSentence(void) {

	unsigned char pField[MAXFIELD];
	if (message && isGPSChecksumPass(gpsSentence[toggle])) {
		if (strncmp((char*) gpsSentence[toggle], "$GPVTG", 6) == 0) {

			my_strcpy(gpsSentenceVTG, gpsSentence[toggle], 83);
			gpsLatestSentenceType = 2;

		} else if (strncmp((char*) gpsSentence[toggle], "$GPRMC", 6) == 0) {

			if (GetField(gpsSentence[toggle], pField, 2, MAXFIELD)
					&& pField[0] == 'A') {
				my_strcpy(gpsSentenceRMC, gpsSentence[toggle], 83);
				GetField(gpsSentenceRMC, pField, 1, MAXFIELD);
				gpsTime = ustrtoul((char*) pField, NULL, 10);

				GetField(gpsSentenceRMC, pField, 4, MAXFIELD);
				if (pField[0] == 'N') {
					GetField(gpsSentenceRMC, pField, 3, MAXFIELD);
					gpsLattitude = my_strtof(pField, gpsLat);
				} else { //south
					GetField(gpsSentenceRMC, pField, 3, MAXFIELD);
					gpsLattitude = -my_strtof(pField, gpsLat);
					gpsLat[0] = -gpsLat[0];
				}

				GetField(gpsSentenceRMC, pField, 6, MAXFIELD);
				if (pField[0] == 'E') {
					GetField(gpsSentenceRMC, pField, 5, MAXFIELD);
					gpsLongitude = my_strtof(pField, gpsLon);
				} else { //west
					GetField(gpsSentenceRMC, pField, 5, MAXFIELD);
					gpsLongitude = -my_strtof(pField, gpsLon);
					gpsLon[0] = -gpsLon[0];
				}

				gpsLatestSentenceType = 1;
			} else {
				gpsSentenceRMC[0] = pField[0];

				gpsLatestSentenceType = 1;
			}

		}
		message = false;
	}
}

/*
 * send calculated heading to uart3
 */
void sendHeading(void) {
	short b;
	char accl[20];
	accl[19] = '\0';
	b = usprintf(accl, "$HEAD,%i*\n", (int) getHeading()); //$HEAD,value*
#ifndef wlan
	nrf24l01p_send(accl);
#else
	for (int i = 0; i < b && accl[i] != '\0'; i++) {
		ROM_UARTCharPut(UART3_BASE, accl[i]);
	}

#endif
}

/*
 * read and calculate heading with basic tilt compensation
 * @returns float containing heading value -> -180 to +180
 *
 */
float getHeading(void) {

	readMagneto();
	readAccel();
	double pitch, roll;
	if ((Az > 9500 && Az < 10050) || (Az < -9500 && Az > -10050)) {
		pitch = roll = 0;
	} else {
		pitch = getPitch();
		roll = getRoll();
	}
	int Xh = Mx * cos(pitch) + Mz * sin(pitch);	// Virtual X and Virtual Y (corrected)
	int Yh = Mx * sin(pitch) * sin(roll) + My * cos(roll)
			- Mz * sin(roll) * cos(pitch);
	float heading = (atan2f(Yh, Xh) * 180 / M_PI);
	if (heading < 0) {
		heading = 360 + heading;
	}
	return heading;
}
double getRoll(void) { //rotation around x axis **in radians

	return (atan2(Ay, Az));
}
double getPitch(void) { //rotation around y axis **in radians

	return (atan2(Ax, Az));
}
/*
 * setup servos on system
 */
void setupServos(void) {
// PA2 & PA3
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4);

// Zero out configs
	servoInit();

// Bind pads to servo configs
	servo = servoAttach(GPIO_PORTB_BASE, GPIO_PIN_4);

// Start the servo timers
	servoStart();
}
/*
 * complimentary filter - useful to combine acc. & gyro
 */
void ComplementaryFilter(short accData[3], short gyrData[3], float *pitch,
		float *roll) {
	float pitchAcc, rollAcc;

// Integrate the gyroscope data -> int(angularSpeed) = angle
	*pitch += ((float) gyrData[0] / GYROSCOPE_SENSITIVITY) * dt; // Angle around the X-axis
	*roll -= ((float) gyrData[1] / GYROSCOPE_SENSITIVITY) * dt; // Angle around the Y-axis

// Compensate for drift with accelerometer data if !bullshit
// Sensitivity = -2 to 2 G at 16Bit -> 2G = 32768 && 0.5G = 8192
	int forceMagnitudeApprox = abs(accData[0]) + abs(accData[1])
			+ abs(accData[2]);
	if (forceMagnitudeApprox > 8192 && forceMagnitudeApprox < 32768) {
		// Turning around the X axis results in a vector on the Y-axis
		pitchAcc = atan2f((float) accData[1], (float) accData[2]) * 180 / M_PI;
		*pitch = *pitch * 0.98 + pitchAcc * 0.02;

		// Turning around the Y axis results in a vector on the X-axis
		rollAcc = atan2f((float) accData[0], (float) accData[2]) * 180 / M_PI;
		*roll = *roll * 0.98 + rollAcc * 0.02;
	}
}

unsigned char *my_strcpy(unsigned char *dest, unsigned const char *src,
		size_t size) {
	unsigned i;
	for (i = 0; src[i] != '\0' && i < size; ++i)
		dest[i] = src[i];
	dest[i] = '\0';
	return dest;
}
/*
 * @Name:        GetField
 * @Description:    This function will get the specified field in a NMEA string.
 * @Entry:        char *pData -        Pointer to NMEA string
 *                char *pField -        pointer to returned field
 *                int nfieldNum -        Field offset to get
 *                int nMaxFieldLen -    Maximum of chars pFiled can handle
 */

bool GetField(unsigned char *pData, unsigned char *pField, int nFieldNum,
		int nMaxFieldLen) {
	int i = 0;
	int nField = 0;
	int i2 = 0;

	//
	// Validate params
	//
	if (pData == NULL || pField == NULL || nMaxFieldLen <= 0) {
		return false;
	}

	//
	// Go to the beginning of the selected field
	//
	i = 0;
	nField = 0;
	while (nField != nFieldNum && pData[i]) {
		if (pData[i] == ',') {
			nField++;
		}

		i++;

		if (pData[i] == '\0') {
			pField[0] = '\0';
			return false;
		}
	}

	if (pData[i] == ',' || pData[i] == '*') {
		pField[0] = '\0';
		return false;
	}

	//
	// copy field from pData to Field
	//
	i2 = 0;
	while (pData[i] != ',' && pData[i] != '*' && pData[i]) {
		pField[i2] = pData[i];
		i2++;
		i++;

		//
		// check if field is too big to fit on passed parameter. If it is,
		// crop returned field to its max length.
		//
		if (i2 >= nMaxFieldLen) {
			i2 = nMaxFieldLen - 1;
			break;
		}
	}
	pField[i2] = '\0';

	return true;
}
float my_strtof(unsigned char* input, int* outputPieces) {
	float output = 0;
	bool mantissa = false;
	bool negativeValue = false;
	char inputInteger[MAXFIELD];
	char inputFraction[MAXFIELD];
	short i = 0, j = 0, k = 0;

	short strLength = ustrlen((char*) input);

	for (i = 0; i < strLength; i++, k++) {

		if (input[i] == '.') {
			mantissa = true;
			inputInteger[k] = '\0';
			j = 0;
			continue;		//skip the dec.
		}
		if (input[i] == '-') {
			negativeValue = true;
			k--;
			continue;		//skip the dec.
		}
		if (!mantissa) {
			inputInteger[k] = input[i];
		} else {
			inputFraction[j] = input[i];
			j++;
		}
	}

	inputFraction[j] = '\0';
	int integerPart = ustrtoul(inputInteger, NULL, 0);
	int fractionPart = ustrtoul(inputFraction, NULL, 0);
	outputPieces[0] = integerPart;
	outputPieces[1] = fractionPart;
	output = integerPart + (float) (fractionPart / (pow(10, (j - 1))));
	if (negativeValue) {
		return -output;
	}
	return output;

}
