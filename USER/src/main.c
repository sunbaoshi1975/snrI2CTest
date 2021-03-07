/**
  ******************************************************************************
  * @file    snrI2CTest/USER/src/main.c 
  * @author  SBS@DataTellIt Inc.
  * @version V1.0.1
  * @date    Mar 7, 2021
  * @brief   This is the I2C example for DataTellIt noise sensor
  ******************************************************************************
  *
  * <h2><center>&copy; COPYRIGHT 2016 - 2021 DataTellIt Inc.</center></h2>
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main_functions.h"
/* Public variables ---------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* ----------------------- Start implementation ------------------------------*/
/**
  * @brief  Entrace of the User Application
  * @param  None
  * @retval None
  *
  * This is the default main used on systems that have the standard C entry point. 
  *
  */
int main(int argc, char* argv[]) {

  // System initialization and setup
  setup();
  
  // Main loop
  while (1) {
    if( loop() < 0 ) break;
  }
  
  return 0;
}

/***************(C)COPYRIGHT 2016 - 2021 DataTellIt Inc. *****END OF FILE******/
