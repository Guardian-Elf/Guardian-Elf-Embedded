#include "main.h"
#include "Delay.h"


static uint32_t fac_us = 0;  // 微秒延时倍乘数

/**
  * @brief  初始化延时函数
  * @param  无
  * @retval 无
  */
void Delay_Init(void)
{
    // SystemCoreClock在system_stm32f1xx.c中定义
    fac_us = SystemCoreClock / 8000000;  // 72MHz时，fac_us=9
}

/**
  * @brief  微秒级延时（基于SysTick）
  * @param  nus 延时时长
  * @retval 无
  */
void Delay_us(uint16_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;

    ticks = nus * fac_us;
    told = SysTick->VAL;

    while(1)
    {
        tnow = SysTick->VAL;
        if(tnow != told)
        {
            if(tnow < told)
                tcnt += told - tnow;
            else
                tcnt += reload - tnow + told;
            told = tnow;
            if(tcnt >= ticks) break;
        }
    }
}

/**
  * @brief  毫秒级延时（基于SysTick）
  * @param  nms 延时时长
  * @retval 无
  */
  void Delay_ms(uint16_t nms) {
      while(nms--) {
          Delay_us(1000);
      }
  }
