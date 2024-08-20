/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.6.0
  * @date    13-October-2021
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2014 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "led.h"
#include "key.h"
#include "ws2812_app.h"
#include "stm32f0xx_it.h"
    
uint32_t systick_cnt=0;
RCC_ClocksTypeDef sys_freq;
void systick_init(void);
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
 int main(void)
{
    RCC_GetClocksFreq(&sys_freq);
    systick_init();
    HardwareInit_Led();
    HardwareInit_Key();
    bsp_ws2812_init();
    while (1)
    {
       task_SystemFunction();
    }
}


#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

void task_SystemFunction(void)
{
    if (Struct_TaksBaseTime.basetime_1ms >= 1)
    {
        Struct_TaksBaseTime.basetime_1ms = 0;
        task_Key();
        task_rgb_ch(&Front_RGB);
    }
    if (Struct_TaksBaseTime.basetime_5ms >= 5)
    {
        Struct_TaksBaseTime.basetime_5ms = 0;
    }
    if (Struct_TaksBaseTime.basetime_500ms >= 500)
    {
        Struct_TaksBaseTime.basetime_500ms = 0;
        Task_Led();
    }
}

void systick_init(void)
{
    if (SysTick_Config(SystemCoreClock / 1000))
    { 
        /* Capture error */ 
        while (1);
    }
}

uint32_t sys_TimeGet(void)
{
    return systick_cnt;
}
