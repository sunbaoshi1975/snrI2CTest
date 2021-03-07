#ifndef __DTIT_I2C_H
#define __DTIT_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l1xx.h"

#define	I2C1_PORT    		GPIOB
#define Pin_SCL				GPIO_Pin_6
#define Pin_SDA				GPIO_Pin_7
#define Pin_Mode_SCL_MSK	((uint32_t)0x00001000)
#define Pin_Mode_SDA_MSK	((uint32_t)0x00004000)
 
#define Pin_SCL_L			I2C1_PORT->ODR &= ~Pin_SCL
#define Pin_SCL_H			I2C1_PORT->ODR |= Pin_SCL
 
#define Pin_SDA_L			I2C1_PORT->ODR &= ~Pin_SDA
#define Pin_SDA_H			I2C1_PORT->ODR |= Pin_SDA
 
#define Pin_SDA_IN			I2C1_PORT->MODER &= ~Pin_Mode_SDA_MSK
#define Pin_SDA_OUT			I2C1_PORT->MODER |= Pin_Mode_SDA_MSK

#define Pin_SDA_OL			Pin_SDA_OUT;Pin_SDA_L
#define Pin_SDA_IH			Pin_SDA_IN;Pin_SDA_H

#define Read_SDA_Pin		(I2C1_PORT->IDR & Pin_SDA)
#define Read_SCL_Pin		(I2C1_PORT->IDR & Pin_SCL)

/**
 * The clock period of the i2c bus in microseconds. Increase this, if your GPIO
 * ports cannot support a 100 kHz output rate. (2 * 1 / 20usec == 100Khz)
 *
 * This is only relevant for the sw-i2c HAL (bit-banging on GPIO pins). The
 * pulse length is half the clock period, the number should thus be even.
 */
//#define I2C_CLOCK_PERIOD_USEC 	10			// 200KHz
//#define I2C_CLOCK_PERIOD_USEC 	20			// 100KHz
//#define I2C_CLOCK_PERIOD_USEC 		40			// 50KHz
#define I2C_CLOCK_PERIOD_USEC 		24			// 25KHz, calibration
//#define I2C_CLOCK_PERIOD_USEC 		12			// 50KHz, calibration	

#define I2C_ST_OK 					0
#define I2C_ST_ERROR 				1
#define I2C_ST_TIMEOUT 				0xFF

void I2C1_Soft_Init(void);
static uint8_t I2C1_Start(void);						
static uint8_t I2C1_Stop(void);			
static void I2C1_Send_Byte(uint8_t txd);
static uint8_t I2C1_Read_Byte(void);
static uint8_t I2C1_Wait_Ack(void); 		
static void I2C1_Ack(void);							
static void I2C1_NAck(void);						
static void I2C_Delay_usec(const uint16_t useconds);
static uint8_t I2C_Wait_SCL_Set(void);

uint8_t I2C1_Write_REG(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t REG_data);
uint8_t I2C1_Read_REG(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t *REG_data, const uint16_t delay_uSec);
uint8_t I2C1_Write_NByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t* buf, uint8_t len);
uint8_t I2C1_Read_NByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t* buf, uint8_t len, const uint16_t delay_uSec);
uint8_t I2C1_CheckDevice(uint8_t SlaveAddress);

#ifdef __cplusplus
}
#endif

#endif /*__DTIT_I2C_H*/
