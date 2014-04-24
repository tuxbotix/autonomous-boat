
#ifndef ACCEL_PHERIPHERAL
#define ACCEL_PHERIPHERAL SYSCTL_PERIPH_GPIOA
#endif
#ifndef ACCEL_I2C_MASTER_BASE
#define ACCEL_I2C_MASTER_BASE I2C1_MASTER_BASE
#endif
#ifndef ACCEL_I2C_PORT
#define ACCEL_I2C_PORT GPIO_PORTA_BASE
#endif
#ifndef ACCEL_I2C_PHERIPHERAL
#define ACCEL_I2C_PHERIPHERAL SYSCTL_PERIPH_I2C1
#endif
#ifndef ACCEL_SCL_PIN
#define ACCEL_SCL_PIN GPIO_PIN_6
#endif
#ifndef ACCEL_SDA_PIN
#define ACCEL_SDA_PIN GPIO_PIN_7
#endif
#ifndef ACCEL_SCL_PIN_CONF
#define ACCEL_SCL_PIN_CONF GPIO_PA6_I2C1SCL
#endif
#ifndef ACCEL_SDA_PIN_CONF
#define ACCEL_SDA_PIN_CONF GPIO_PA7_I2C1SDA
#endif
#ifndef QC_I2C_H_
#define QC_I2C_H_


//Standard C header files
#include "stdint-gcc.h"

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/i2c.h"
#include "driverlib/fpu.h"
#include "driverlib/ssi.h"


// setup code for all I2C modules present on the LM4F120E5QR
extern void i2c_Config(void);

// functions for writing/reading single bytes of data
extern uint8_t i2c_ReadByte(uint8_t devId, uint8_t addr);

extern int32_t i2c_WriteByte(uint8_t devId, uint8_t addr, uint8_t data);

// functions for writing/reading multiple bytes of data
extern int32_t i2c_ReadBuf(uint8_t devId, uint8_t addr, int32_t nBytes , uint8_t* pBuf );

extern int32_t i2c_WriteBuf(uint8_t devId, uint8_t addr, int32_t nBytes , uint8_t* pBuf);


#endif /* QC_I2C_H_ */
