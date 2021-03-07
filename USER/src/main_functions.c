/**
  ******************************************************************************
  * @file    MSensor/main_functions.c
  * @file    snrI2CTest/USER/src/main.c 
  * @author  SBS@DataTellIt Inc.
  * @version V1.0.1
  * @date    Mar 7, 2021
  * @brief   This is the implementation of main functions of noise sensor I2C example
  ******************************************************************************
  *
  * <h2><center>&copy; COPYRIGHT 2016-2021 DataTellIt Inc.</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "delay.h"
#include "global.h"
#include "i2c.h"
#include "nsrAPICmd.h"
#include "sensorMIC.h"
#include "timer.h"
#include "usart2.h"
#include <string.h>

/* Public constants ----------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
volatile uint8_t gCollectFlag = 0;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t m_collectTick = 0;

/* Private function prototypes -----------------------------------------------*/
void tmrProcess(void);

/* --------------------- Start implementation --------------------------------*/
// Noise sensor I2C function example
void nsrI2C_Example() {
    uint16_t lv_data;
  
    // Read command
    /// 读传感器固件版本
    if( nsr_i2_read_version(&lv_data) == NSR_STATUS_OK ) {
        LogOutput("NSR version: 0x%X\r\n", lv_data);
    }

    /// 读噪声告警门限
    if( nsr_i2_read(SRAPI_CMD_SET_TH_WARNING, &lv_data, NSR_CMD_DURATION_USEC) == NSR_STATUS_OK ) {
        LogOutput("NSR warning threshold: %d\r\n", lv_data);
    }
    
    /// 读告警保持时长（秒）
    if( nsr_i2_read(SRAPI_CMD_SET_WARN_SW_ON_DUR, &lv_data, NSR_CMD_DURATION_USEC) == NSR_STATUS_OK ) {
        LogOutput("NSR warning duration: %d seconds\r\n", lv_data);
    }

    // Write command
    /// 修改噪声告警门限
    lv_data = 55;
    if( nsr_i2_change_config(SRAPI_CMD_SET_TH_WARNING, lv_data) == NSR_STATUS_OK ) {
        LogOutput("NSR warning threshold changed to %d\r\n", lv_data);
    }

    /// 修改告警保持时长（秒）
    lv_data = 10;
    if( nsr_i2_change_config(SRAPI_CMD_SET_WARN_SW_ON_DUR, lv_data) == NSR_STATUS_OK ) {
        LogOutput("NSR warning duration changed to %d seconds\r\n", lv_data);
    }
}

/**
  * @brief  System initialization
  * @param  None
  * @retval 0 - Succeeded
            1 - Init error
  */
uint8_t sys_init(void) {
	
	// Init clock, timer, button etc.
	RCC_HSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	/* Set HSI as sys clock: 8M */
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	
    delay_init();                      	// 延时函数初始化

	// 设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	Timer_Init();						// 定时器初始化

    /* Initialize the i2c bus for the current platform */
	I2C1_Soft_Init();

    return 0;
}

/**
  * @brief  Device initialization
  * @param  None
  * @retval 0 - Succeeded
            1 - Init error
  */
uint8_t dev_init(void) {
	// Enable Interrupt in case it was disabled anyhow
	__enable_irq();

	// Init Sensor modules
	// Noise sensor
	mic_init();
    
	return 0;
}

/**
  * @brief  Multi-sensor runtime setup, Arduino compatible
  * @param  None
  * @retval None
  *
  */
void setup() {
    // System Init
    sys_init();
	
	/* serial log */
	Serial_Usart2_Init(9600, FALSE);
	Serial_PutString("\r\n-------------APP START ");
	Serial_PutString("-------------\r\n");
	
	/* Initialize Devices */
	dev_init();
	
	// Install timer handler
	Timer_10ms_handler = tmrProcess;
    
    // Noise sensor I2C function example
    nsrI2C_Example();
}

/**
  * @brief  Multi-sensor main loop, Arduino compatible
  * @param  None
  * @retval reboot if return -1
  *
  */
int loop() {
    if( gCollectFlag ) {
        if( (gCollectFlag & MULTI_COLLECT_FLAG_MIC) == MULTI_COLLECT_FLAG_MIC ) {
            // Collect noise (dB)
            mic_checkData();
        }	
        gCollectFlag = 0;
    }    
    return 0;
}

// Execute every second
void OneSecondProcess(void)
{
}

/**
* @brief  10ms timer process
* @param  None
* @retval None
*/
void tmrProcess(void)
{
	static uint8_t lv_second_tick = 0;
	
	// Check data-collection timer
	if( gCollectFlag == 0 ) {
        if( ++m_collectTick >= MULTI_COLLECT_INTERVAL_10MS ) {
            m_collectTick = 0;
            gCollectFlag |= MULTI_COLLECT_FLAG_MIC;
        }
	}
	
	// Check other slow timers every one second
	if( ++lv_second_tick >= 100 ) {
		lv_second_tick = 0;
		OneSecondProcess();
	}
}

/***************(C)COPYRIGHT 2016 - 2021 DataTellIt Inc. *****END OF FILE******/
