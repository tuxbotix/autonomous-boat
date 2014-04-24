#include "QC_I2C.h"

#define ACCEL_PHERIPHERAL SYSCTL_PERIPH_GPIOA
#define ACCEL_I2C_MASTER_BASE I2C1_MASTER_BASE
#define ACCEL_I2C_PORT GPIO_PORTA_BASE
#define ACCEL_I2C_PHERIPHERAL SYSCTL_PERIPH_I2C1
#define ACCEL_SCL_PIN GPIO_PIN_6
#define ACCEL_SDA_PIN GPIO_PIN_7
#define ACCEL_SCL_PIN_CONF GPIO_PA6_I2C1SCL
#define ACCEL_SDA_PIN_CONF GPIO_PA7_I2C1SDA
/*
 #ifndef ACCEL_PHERIPHERAL
 #define ACCEL_PHERIPHERAL SYSCTL_PERIPH_GPIOA
 #ifndef ACCEL_I2C_MASTER_BASE
 #define ACCEL_I2C_MASTER_BASE I2C1_MASTER_BASE
 #ifndef ACCEL_I2C_PORT
 #define ACCEL_I2C_PORT GPIO_PORTA_BASE
 #ifndef ACCEL_I2C_PHERIPHERAL
 #define ACCEL_I2C_PHERIPHERAL SYSCTL_PERIPH_I2C1
 #ifndef ACCEL_SCL_PIN
 #define ACCEL_SCL_PIN GPIO_PIN_6
 #ifndef ACCEL_SDA_PIN
 #define ACCEL_SDA_PIN GPIO_PIN_7
 #ifndef ACCEL_SCL_PIN_CONF
 #define ACCEL_SCL_PIN_CONF GPIO_PA6_I2C1SCL
 #ifndef ACCEL_SDA_PIN_CONF
 #define ACCEL_SDA_PIN_CONF GPIO_PA7_I2C1SDA*/

//*****************************************************************************
//
//! Initializes and enables the specified I2C block.
//!
//! \param ACCEL_I2C_MASTER_BASE is the I2C peripheral to be used.
//! \param ulI2CSpeed defines the normal (100kbps) or fast (400kbps) I2C mode.
//!
//! This function enables the specified I2C block and sets it up to run at
//! the either 100kbps or 400kbps.  If the \e ulI2CSpeed is false, the I2C will
//! run at 100kbps and if true, then the I2C will run at 400kbps.  The
//! \e ACCEL_I2C_MASTER_BASE parameter can be one of the following values:
//!
//! \return None.
//
//*****************************************************************************
void i2c_Config(void) {

	// I2C1 is used with PortA[7:6].

	SysCtlPeripheralEnable(ACCEL_I2C_PHERIPHERAL);
	SysCtlPeripheralEnable(ACCEL_PHERIPHERAL);

                    GPIOPinConfigure(ACCEL_SCL_PIN_CONF);
                    GPIOPinTypeI2CSCL(ACCEL_I2C_PORT, ACCEL_SCL_PIN);
                    GPIOPinConfigure(ACCEL_SDA_PIN_CONF);
                    GPIOPinTypeI2CSCL(ACCEL_I2C_PORT, ACCEL_SDA_PIN);

//                    GPIOPinTypeI2CSCL(ACCEL_I2C_PORT, ACCEL_SCL_PIN);
//                    ROM_GPIOPinTypeI2C(ACCEL_I2C_PORT,ACCEL_SDA_PIN );
//
//                    ROM_GPIOPinConfigure(ACCEL_SCL_PIN_CONF);
//                    ROM_GPIOPinConfigure(ACCEL_SDA_PIN_CONF);
//
//                        ROM_SysCtlPeripheralEnable(ACCEL_I2C_PHERIPHERAL);
//
                        ROM_I2CMasterInitExpClk(ACCEL_I2C_MASTER_BASE, SysCtlClockGet(), 1); // 1 : 400Khz, 0 : 100Khz
}

//*****************************************************************************
//
//! Reads the I2C slave register.
//!
//! \param ACCEL_I2C_MASTER_BASE is the base for the I2C module.
//! \param SlaveID is the 7-bit address of the slave to be addressed.
//! \param addr is the register to read from.
//!
//! This function initiates a read from the slave device.
//! The ACCEL_I2C_MASTER_BASE parameter is the I2C modules master base address.
//! \e ACCEL_I2C_MASTER_BASE parameter can be one of the following values:
//!
//! \return Register value in an unsigned long format.  Note that 0 will be
//! returned if there is ever an error, 1 if there was not.
//
//*****************************************************************************
uint8_t i2c_ReadByte(uint8_t SlaveID, uint8_t addr) {
	unsigned long ulRegValue = 0;

//      LED_TOGGLE(BLUE);

// Wait until master module is done transferring.
//
	while (ROM_I2CMasterBusy(ACCEL_I2C_MASTER_BASE)) {
	};

	//
	// Tell the master module what address it will place on the bus when
	// writing to the slave.
	//
	ROM_I2CMasterSlaveAddrSet(ACCEL_I2C_MASTER_BASE, SlaveID, 0);

	//
	// Place the command to be sent in the data register.
	//
	ROM_I2CMasterDataPut(ACCEL_I2C_MASTER_BASE, addr);

	//
	// Initiate send of data from the master.
	//
	ROM_I2CMasterControl(ACCEL_I2C_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);

	//
	// Wait until master module is done transferring.
	//
	while (ROM_I2CMasterBusy(ACCEL_I2C_MASTER_BASE)) {
	};

	//
	// Check for errors.
	//
	if (ROM_I2CMasterErr(ACCEL_I2C_MASTER_BASE) != I2C_MASTER_ERR_NONE) {
		return 2;
	}

	//
	// Tell the master module what address it will place on the bus when
	// reading from the slave.
	//
	ROM_I2CMasterSlaveAddrSet(ACCEL_I2C_MASTER_BASE, SlaveID, 1);

	//
	// Tell the master to read data.
	//
	ROM_I2CMasterControl(ACCEL_I2C_MASTER_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

	//
	// Wait until master module is done receiving.
	//
	while (ROM_I2CMasterBusy(ACCEL_I2C_MASTER_BASE)) {
	};

	//
	// Check for errors.
	//
	if (ROM_I2CMasterErr(ACCEL_I2C_MASTER_BASE) != I2C_MASTER_ERR_NONE) {
		return 1;
	}

	//
	// Read the data from the master.
	//
	ulRegValue = ROM_I2CMasterDataGet(ACCEL_I2C_MASTER_BASE);

	//
	// Return the register value.
	//
	return 3;
//	return ulRegValue;
}

//*****************************************************************************
//
//! Writes to the specified I2C slave register.
//!
//! \param ACCEL_I2C_MASTER_BASE is the base for the I2C module.
//! \param SlaveID is the 7-bit address of the slave to be addressed.
//! \param addr is the register to write data to.
//! \param data is the 8-bit data to be written.
//!
//! This function initiates a read from the I2C slave device.
//! The ACCEL_I2C_MASTER_BASE parameter is the I2C modules master base address.
//! \e ACCEL_I2C_MASTER_BASE parameter can be one of the following values:
//!
//! \return Register value in an unsigned long format.  Note that 0 will be
//! returned if there is ever an error, 1 if there was not.
//
//int32_t  i2c_XmtByte(uint8_t SlaveID, uint8_t addr, uint8_t data);
//*****************************************************************************
int32_t i2c_WriteByte(uint8_t SlaveID, uint8_t addr, uint8_t data) {

//      LED_TOGGLE(GREEN);

	//
	// Wait until master module is done transferring.
	//
	while (ROM_I2CMasterBusy(ACCEL_I2C_MASTER_BASE)) {
	};

	//
	// Tell the master module what address it will place on the bus when
	// writing to the slave.
	//
	ROM_I2CMasterSlaveAddrSet(ACCEL_I2C_MASTER_BASE, SlaveID, 0);

	//
	// Place the command to be sent in the data register.
	//
	ROM_I2CMasterDataPut(ACCEL_I2C_MASTER_BASE, addr);

	//
	// Initiate send of data from the master.
	//
	ROM_I2CMasterControl(ACCEL_I2C_MASTER_BASE,
			I2C_MASTER_CMD_BURST_SEND_START);

	//
	// Wait until master module is done transferring.
	//
	while (ROM_I2CMasterBusy(ACCEL_I2C_MASTER_BASE)) {
	};

	//
	// Check for errors.
	//
	if (ROM_I2CMasterErr(ACCEL_I2C_MASTER_BASE) != I2C_MASTER_ERR_NONE) {
		return 0;
	}

	//
	// Place the value to be sent in the data register.
	//
	ROM_I2CMasterDataPut(ACCEL_I2C_MASTER_BASE, data);

	//
	// Initiate send of data from the master.
	//
	ROM_I2CMasterControl(ACCEL_I2C_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);

	//
	// Wait until master module is done transferring.
	//
	while (ROM_I2CMasterBusy(ACCEL_I2C_MASTER_BASE)) {
	};

	//
	// Check for errors.
	//
	if (ROM_I2CMasterErr(ACCEL_I2C_MASTER_BASE) != I2C_MASTER_ERR_NONE) {
		return 0;
	}

	//
	// Initiate send of data from the master.
	//
	ROM_I2CMasterControl(ACCEL_I2C_MASTER_BASE,
			I2C_MASTER_CMD_BURST_SEND_FINISH);

	//
	// Wait until master module is done transferring.
	//
	while (ROM_I2CMasterBusy(ACCEL_I2C_MASTER_BASE)) {
	};

	//
	// Check for errors.
	//
	if (ROM_I2CMasterErr(ACCEL_I2C_MASTER_BASE) != I2C_MASTER_ERR_NONE) {
		return 0;
	}

	//
	// Return 1 if there is no error.
	//
	return 1;
}

//*****************************************************************************
//
//! Reads one/multiple bytes of data from an I2C slave device.
//!
//! \param ACCEL_I2C_MASTER_BASE is the base for the I2C module.
//! \param SlaveID is the 7-bit address of the slave to be addressed.
//! \param addr is the register to start reading from.
//! \param pBuf is a pointer to the array to store the data.
//! \param nBytes is the number of bytes to read from the slave.
//!
//! This function reads one/multiple bytes of data from an I2C slave device.
//! The ACCEL_I2C_MASTER_BASE parameter is the I2C modules master base address.
//! \e ACCEL_I2C_MASTER_BASE parameter can be one of the following values:
//!
//!
//! \return 0 if there was an error or 1 if there was not.
//
//int32_t  i2c_RcvBuf(uint8_t SlaveID, uint8_t addr, int32_t nBytes, uint8_t* pBuf);
//*****************************************************************************
int32_t i2c_ReadBuf(uint8_t SlaveID, uint8_t addr, int32_t nBytes,
		uint8_t* pBuf) {
	uint8_t nBytesCount; // local variable used for byte counting/state determination
	uint16_t MasterOptionCommand; // used to assign the commands for ROM_I2CMasterControl() function

	//
	// Wait until master module is done transferring.
	//
	while (ROM_I2CMasterBusy(ACCEL_I2C_MASTER_BASE)) {
	};

	//
	// Tell the master module what address it will place on the bus when
	// writing to the slave.
	//
	ROM_I2CMasterSlaveAddrSet(ACCEL_I2C_MASTER_BASE, SlaveID, 0);

	//
	// Place the command to be sent in the data register.
	//
	ROM_I2CMasterDataPut(ACCEL_I2C_MASTER_BASE, addr);

	//
	// Initiate send of data from the master.
	//
	ROM_I2CMasterControl(ACCEL_I2C_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);

	//
	// Wait until master module is done transferring.
	//
	while (ROM_I2CMasterBusy(ACCEL_I2C_MASTER_BASE)) {
	};

	//
	// Check for errors.
	//
	if (ROM_I2CMasterErr(ACCEL_I2C_MASTER_BASE) != I2C_MASTER_ERR_NONE) {
		return 0;
	}

	//
	// Tell the master module what address it will place on the bus when
	// reading from the slave.
	//
	ROM_I2CMasterSlaveAddrSet(ACCEL_I2C_MASTER_BASE, SlaveID, true);

	//
	// Start with BURST with more than one byte to write
	//
	MasterOptionCommand = I2C_MASTER_CMD_BURST_RECEIVE_START;

	for (nBytesCount = 0; nBytesCount < nBytes; nBytesCount++) {
		//
		// The second and intermittent byte has to be read with CONTINUE control word
		//
		if (nBytesCount == 1)
			MasterOptionCommand = I2C_MASTER_CMD_BURST_RECEIVE_CONT;

		//
		// The last byte has to be send with FINISH control word
		//
		if (nBytesCount == nBytes - 1)
			MasterOptionCommand = I2C_MASTER_CMD_BURST_RECEIVE_FINISH;

		//
		// Re-configure to SINGLE if there is only one byte to read
		//
		if (nBytes == 1)
			MasterOptionCommand = I2C_MASTER_CMD_SINGLE_RECEIVE;

		//
		// Initiate read of data from the slave.
		//
		ROM_I2CMasterControl(ACCEL_I2C_MASTER_BASE, MasterOptionCommand);

		//
		// Wait until master module is done reading.
		//
		while (ROM_I2CMasterBusy(ACCEL_I2C_MASTER_BASE)) {
		};

		//
		// Check for errors.
		//
		if (ROM_I2CMasterErr(ACCEL_I2C_MASTER_BASE) != I2C_MASTER_ERR_NONE) {
			return 0;
		}

		//
		// Move byte from register
		//
		pBuf[nBytesCount] = I2CMasterDataGet(ACCEL_I2C_MASTER_BASE);
	}

	// send number of received bytes
	return nBytesCount;
}

//*****************************************************************************
//
//! Writes one/multiple bytes of data to an I2C slave device.
//! Ensure to use auto-increment options on some devices
//! (Control Registers, refer to data sheet).
//! I.e. store related command in the first position of your data array.
//!
//! \param ACCEL_I2C_MASTER_BASE is the base for the I2C module.
//! \param SlaveID is the 7-bit address of the slave to be addressed.
//! \param addr is the register to start writing to.
//! \param pBuf is a pointer to the array to be send.
//! \param nBytes is the number of bytes to send from array pBuf[].
//!
//! This function writes multiple bytes of data an I2C slave device.
//! The ACCEL_I2C_MASTER_BASE parameter is the I2C modules master base address.
//! \e ACCEL_I2C_MASTER_BASE parameter can be one of the following values:
//!
//!
//! \return 0 if there was an error or 1 if there was not.

//int32_t  i2c_XmtBuf(uint8_t SlaveID, uint8_t addr, int32_t nBytes, uint8_t* pBuf );
//
//*****************************************************************************

int32_t i2c_WriteBuf(uint8_t SlaveID, uint8_t addr, int32_t nBytes,
		uint8_t* pBuf) {
	uint8_t nBytesCount; // local variable used for byte counting/state determination
	uint16_t MasterOptionCommand; // used to assign the commands for ROM_I2CMasterControl() function

	//
	// Wait until master module is done transferring.
	//
	while (ROM_I2CMasterBusy(ACCEL_I2C_MASTER_BASE)) {
	};

	//
	// Tell the master module what address it will place on the bus when
	// writing to the slave.
	//
	ROM_I2CMasterSlaveAddrSet(ACCEL_I2C_MASTER_BASE, SlaveID, false);

	//
	// Place the value to be sent in the data register.
	//
	ROM_I2CMasterDataPut(ACCEL_I2C_MASTER_BASE, addr);

	//
	// Initiate send of data from the master.
	//
	ROM_I2CMasterControl(ACCEL_I2C_MASTER_BASE,
			I2C_MASTER_CMD_BURST_SEND_START);

	//
	// Wait until master module is done transferring.
	//
	while (ROM_I2CMasterBusy(ACCEL_I2C_MASTER_BASE)) {
	};

	//
	// Check for errors.
	//
	if (ROM_I2CMasterErr(ACCEL_I2C_MASTER_BASE) != I2C_MASTER_ERR_NONE) {
		return 0;
	}

	//
	// Start with CONT for more than one byte to write
	//
	MasterOptionCommand = I2C_MASTER_CMD_BURST_SEND_CONT;

	for (nBytesCount = 0; nBytesCount < nBytes; nBytesCount++) {
		//
		// The second and intermittent byte has to be send with CONTINUE control word
		//
		if (nBytesCount == 1)
			MasterOptionCommand = I2C_MASTER_CMD_BURST_SEND_CONT;

		//
		// The last byte has to be send with FINISH control word
		//
		if (nBytesCount == nBytes - 1)
			MasterOptionCommand = I2C_MASTER_CMD_BURST_SEND_FINISH;

		//
		// Re-configure to SINGLE if there is only one byte to write
		//
		if (nBytes == 1)
			MasterOptionCommand = I2C_MASTER_CMD_SINGLE_SEND;

		//
		// Send data byte
		//
		ROM_I2CMasterDataPut(ACCEL_I2C_MASTER_BASE, pBuf[nBytesCount]);

		//
		// Initiate send of data from the master.
		//
		ROM_I2CMasterControl(ACCEL_I2C_MASTER_BASE, MasterOptionCommand);

		//
		// Wait until master module is done transferring.
		//
		while (ROM_I2CMasterBusy(ACCEL_I2C_MASTER_BASE)) {
		};

		//
		// Check for errors.
		//
		if (ROM_I2CMasterErr(ACCEL_I2C_MASTER_BASE) != I2C_MASTER_ERR_NONE) {
			return 0;
		}
	}

	//
	// Return 1 if there is no error.
	//
	return 1;
}
