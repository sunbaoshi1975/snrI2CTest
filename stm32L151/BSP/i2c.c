#include "i2c.h"
#include "delay.h"

#define I2C_DELAY 					1
#define I2C_DELAY2					2
#define I2C_HALF_PERIOD 			(I2C_CLOCK_PERIOD_USEC >> 1)

static void I2C_Delay_usec(const uint16_t useconds) {
    if (useconds) delay_us(useconds);
}

static uint8_t I2C_Wait_SCL_Set(void)
{
	/* Maximal timeout of 5ms half polling cycles */
    uint16_t timeout_cycles = 5000 / I2C_HALF_PERIOD;
    while (--timeout_cycles) {
        if (Read_SCL_Pin) return I2C_ST_OK;
        I2C_Delay_usec(I2C_HALF_PERIOD);
    }

    return I2C_ST_ERROR;
}

// ��ʼ��IIC��IO��
void I2C1_Soft_Init(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;				// ����GPIO�ṹ��
	RCC_AHBPeriphClockCmd(	RCC_AHBPeriph_GPIOB, ENABLE );	// ��GPIOB��ʱ��
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = Pin_SCL | Pin_SDA;		// IIC��ӦIO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT ;			// ���
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;			// ��©
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 			// ����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(I2C1_PORT, &GPIO_InitStructure);
	GPIO_SetBits(I2C1_PORT, Pin_SCL | Pin_SDA);
	
    I2C1_Stop();
}
 
// ����IIC��ʼ�ź�
static uint8_t I2C1_Start(void)
{
    Pin_SCL_H; // ����ʱ����
    Pin_SDA_IH; // �����ź���
    I2C_Delay_usec(I2C_DELAY);
    if(!Read_SDA_Pin) return I2C_ST_ERROR;
    Pin_SDA_OL; // �ź����ɸߵ�ƽ��͵�ƽ����
    I2C_Delay_usec(I2C_DELAY2);
	return(Read_SDA_Pin ? I2C_ST_ERROR : I2C_ST_OK);
}
 
// ����IICֹͣ�ź�
static uint8_t I2C1_Stop(void)
{
    Pin_SCL_H; // ����ʱ����
    Pin_SDA_OL; // �����ź���
    I2C_Delay_usec(I2C_DELAY);
    if(Read_SDA_Pin) return I2C_ST_ERROR;
    Pin_SDA_IH; // �ź����ɵ͵�ƽ��ߵ�ƽ����
    I2C_Delay_usec(I2C_DELAY2);
	return(Read_SDA_Pin ? I2C_ST_OK: I2C_ST_ERROR);
}
 
// IIC����ACK�ź�
static void I2C1_Ack(void)
{
    Pin_SCL_L; // ����ʱ����
    I2C_Delay_usec(I2C_DELAY);
    Pin_SDA_OL; // �����ź���
    Pin_SCL_H; // ����ʱ����
    I2C_Delay_usec(I2C_DELAY2); 	// ����
    Pin_SCL_L; // ����ʱ����
    Pin_SDA_IH; // �����ź���
    I2C_Delay_usec(I2C_DELAY);
}
 
// IIC������ACK�ź�: NACK
static void I2C1_NAck(void)
{
    Pin_SCL_L; // ����ʱ����
    I2C_Delay_usec(I2C_DELAY);	
    Pin_SDA_IH; // �����ź���
    Pin_SCL_H; // ����ʱ����
    I2C_Delay_usec(I2C_DELAY2);		// ����
    Pin_SCL_L; // ����ʱ����
    I2C_Delay_usec(I2C_DELAY);
}
 
// IIC�ȴ�ACK�ź�
static uint8_t I2C1_Wait_Ack(void)
{
	uint8_t nack;
    Pin_SCL_L; // ����ʱ����
    Pin_SDA_IH; // �����ź���
    I2C_Delay_usec(I2C_HALF_PERIOD);
    Pin_SCL_H; // ����ʱ����
	// �ȴ�ʱ���߾���
	I2C_Delay_usec(I2C_HALF_PERIOD);
	if( I2C_Wait_SCL_Set() ) return I2C_ST_TIMEOUT;
	nack = (Read_SDA_Pin ? I2C_ST_ERROR : I2C_ST_OK);
    Pin_SCL_L; // ����ʱ����
    I2C_Delay_usec(I2C_DELAY);
    return nack;
}

// IIC����һ���ֽ�
static void I2C1_Send_Byte(uint8_t txd)
{
	Pin_SDA_OUT;
    for(uint8_t i = 0; i < 8; i++) {
		Pin_SCL_L; // ����ʱ����
		I2C_Delay_usec((I2C_HALF_PERIOD >> 1) - 2);
		if(txd & 0x80) {
			Pin_SDA_H;
		} else {
			Pin_SDA_L;
		}
		I2C_Delay_usec(I2C_HALF_PERIOD >> 1);
	    txd <<= 1;
	    Pin_SCL_H; // ����ʱ����
	    I2C_Delay_usec(I2C_HALF_PERIOD);
    }
}
 
// IIC��ȡһ���ֽ�
static uint8_t I2C1_Read_Byte(void)
{
	Pin_SDA_IN;
    uint8_t rxd = 0;
    for(uint8_t i = 0; i < 8; i++) {
		rxd <<= 1;
		Pin_SCL_L; // ����ʱ����
		I2C_Delay_usec(I2C_HALF_PERIOD - 2);
		Pin_SCL_H; // ����ʱ����
		I2C_Delay_usec(I2C_DELAY);
		if( I2C_Wait_SCL_Set() ) {
			rxd = I2C_ST_TIMEOUT;
			break;
		}
		if(Read_SDA_Pin) rxd |= 0x01;
    }
	Pin_SDA_OUT;
    return rxd;
}
 
// ��ӻ�ָ����ַд����
uint8_t I2C1_Write_REG(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t REG_data)
{
    if(I2C1_Start() != I2C_ST_OK) return I2C_ST_ERROR;
    I2C1_Send_Byte(SlaveAddress << 1);
    if(I2C1_Wait_Ack() != I2C_ST_OK) {
		I2C1_Stop();
		return I2C_ST_ERROR;
	}
    I2C1_Send_Byte(REG_Address);
    if(I2C1_Wait_Ack() != I2C_ST_OK) {
		I2C1_Stop();
		return I2C_ST_ERROR;
	}
    I2C1_Send_Byte(REG_data);
    if(I2C1_Wait_Ack() != I2C_ST_OK) {
		I2C1_Stop();
		return I2C_ST_ERROR;
	}
    if(I2C1_Stop() != I2C_ST_OK) return I2C_ST_ERROR;
    return I2C_ST_OK;
}
 
// ���豸�ж�ȡ����
uint8_t I2C1_Read_REG(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t *REG_data, const uint16_t delay_uSec)
{
    if(I2C1_Start() != I2C_ST_OK) return I2C_ST_ERROR;
    I2C1_Send_Byte(SlaveAddress << 1);
    if(I2C1_Wait_Ack() != I2C_ST_OK) {
		I2C1_Stop();
		return I2C_ST_ERROR;
	}
    I2C1_Send_Byte(REG_Address);
    if(I2C1_Wait_Ack() != I2C_ST_OK) {
		I2C1_Stop();
		return I2C_ST_ERROR;
	}
	// Add delay in between
	if( delay_uSec ) I2C_Delay_usec(delay_uSec);
    if(I2C1_Start() != I2C_ST_OK) return I2C_ST_ERROR;
    I2C1_Send_Byte((SlaveAddress << 1) | 0x01);	// Read
    if(I2C1_Wait_Ack() != I2C_ST_OK) {
		I2C1_Stop();
		return I2C_ST_ERROR;
	}
    *REG_data = I2C1_Read_Byte();
    I2C1_NAck();
    if(I2C1_Stop() != I2C_ST_OK) return I2C_ST_ERROR;	
    return I2C_ST_OK;
}
 
// ����дN���ֽ�
uint8_t I2C1_Write_NByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t* buf, uint8_t len)
{
    if(I2C1_Start() != I2C_ST_OK) return I2C_ST_ERROR;
    I2C1_Send_Byte(SlaveAddress << 1);  //�����豸��ַ+д�ź�
    if(I2C1_Wait_Ack() != I2C_ST_OK){I2C1_Stop(); return I2C_ST_ERROR;}
    I2C1_Send_Byte(REG_Address);   
    if(I2C1_Wait_Ack() != I2C_ST_OK){I2C1_Stop(); return I2C_ST_ERROR;}
    for(uint16_t i = 0; i < len; i++) {
        I2C1_Send_Byte(buf[i]);
        if(I2C1_Wait_Ack() != I2C_ST_OK) {
            I2C1_Stop();
            return I2C_ST_ERROR;
        }
    }
    I2C1_Stop();
    return I2C_ST_OK;
}
 
// ������N���ֽ�
uint8_t I2C1_Read_NByte(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t* buf, uint8_t len, const uint16_t delay_uSec)
{
    if(I2C1_Start() != I2C_ST_OK) return I2C_ST_ERROR;
    I2C1_Send_Byte(SlaveAddress << 1);  //�����豸��ַ+д�ź�
    if(I2C1_Wait_Ack() != I2C_ST_OK){ 
		I2C1_Stop();
		return I2C_ST_ERROR;
	}
    I2C1_Send_Byte(REG_Address);   
    if(I2C1_Wait_Ack() != I2C_ST_OK) {
		I2C1_Stop();
		return I2C_ST_ERROR;
	}
	// Add delay in between
	if( delay_uSec ) I2C_Delay_usec(delay_uSec);
    if(I2C1_Start() != I2C_ST_OK) return I2C_ST_ERROR;
    I2C1_Send_Byte((SlaveAddress << 1) | 0x01); // ������
    if(I2C1_Wait_Ack() != I2C_ST_OK) {
		I2C1_Stop();
		return I2C_ST_ERROR;
	}
    for(uint16_t i = 0; i < len; i++) {
        buf[i] = I2C1_Read_Byte();
		if(i < len - 1) I2C1_Ack();
	}
    I2C1_NAck();
    I2C1_Stop();
    return I2C_ST_OK;
}
 
 // ����豸��ַ
uint8_t I2C1_CheckDevice(uint8_t SlaveAddress)
{
    if(I2C1_Start() != I2C_ST_OK) return I2C_ST_ERROR;
    I2C1_Send_Byte(SlaveAddress << 1); //�����豸��ַ+д�ź�
    if(I2C1_Wait_Ack() != I2C_ST_OK) {
		I2C1_Stop();
		return I2C_ST_ERROR;		
    }
    if(I2C1_Stop() != I2C_ST_OK) return I2C_ST_ERROR;	
    return I2C_ST_OK;
}
