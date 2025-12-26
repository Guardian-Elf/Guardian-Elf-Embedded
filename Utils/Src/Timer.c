//
// Created by pc on 2025/12/21.
//

#include "Timer.h"
#include "USART.h"


TIM_HandleTypeDef htim2;

// 通用定时器初始化
void Timer_Init_2(void) {
    /* 1. 使能TIM2时钟 */
    __HAL_RCC_TIM2_CLK_ENABLE();

    /* 2. 配置定时器参数 */
    htim2.Instance = TIM2;                  // 使用TIM2
    htim2.Init.Prescaler = 72000 - 1;              // 72MHz/(7199+1) = 1kHz
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;  // 向上计数
    htim2.Init.Period = 19999;                // (9999+1)/1kHz = 1000ms
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // 使能自动重装载

    /* 3. 初始化定时器 */
    HAL_TIM_Base_Init(&htim2);

    /* 4. 配置中断 */
    HAL_NVIC_SetPriority(TIM2_IRQn, 3, 2);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

    /* 5. 启动定时器中断 */
    HAL_TIM_Base_Start_IT(&htim2);
}

/* TIM1中断处理函数 */
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim2);
}

/* 定时器更新中断回调函数 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim2) {
        Timer_Interrupt_2();
    }

}

// 中断函数
__weak void Timer_Interrupt_2(void) {
    USART1_Printf("Timer_Interrupt_2\r\n");
}
