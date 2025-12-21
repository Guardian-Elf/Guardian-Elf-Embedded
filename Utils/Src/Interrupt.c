//
// Created by pc on 2025/12/19.
//

#include "Interrupt.h"
#include "USART.h"

#include "stm32f1xx_hal.h"

/**
  * @brief  GPIO中断初始化
  * @param  GPIO_PORT: GPIO端口（只允许ABC）
  * @param  GPIO_PIN: 引脚号（0-15）
  * @param  active_level: 0=低电平有效（下降沿），1=高电平有效（上升沿）
  * @param  priority: 中断优先级 (0-15)
  */
void Interrupt_Init(GPIO_TypeDef *GPIO_PORT, uint32_t GPIO_PIN,
                    uint8_t active_level, uint32_t priority)
{
    uint32_t mode, pull;
    IRQn_Type IRQn;
    uint8_t pin_index = 0;

    // 确定触发方式和上下拉
    if (active_level == 0) {
        mode = GPIO_MODE_IT_FALLING;  // 下降沿触发（按键按下为低）
        pull = GPIO_PULLUP;           // 默认上拉，未按下时为高
    } else {
        mode = GPIO_MODE_IT_RISING;   // 上升沿触发
        pull = GPIO_PULLDOWN;         // 默认下拉，未按下时为低
    }

    // 使能GPIO时钟
    if (GPIO_PORT == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (GPIO_PORT == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (GPIO_PORT == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else {
        return; // 非法端口，直接返回
    }

    // 配置GPIO为中断模式
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN;
    GPIO_InitStruct.Mode = mode;
    GPIO_InitStruct.Pull = pull;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIO_PORT, &GPIO_InitStruct);

    // 关键步骤：配置EXTI中断线与GPIO端口的映射
    // 先找到引脚号（0-15）
    while (!(GPIO_PIN & (1 << pin_index))) {
        pin_index++;
        if (pin_index > 15) return; // 非法引脚，返回
    }

    // 获取中断号
    if (pin_index == 0) IRQn = EXTI0_IRQn;
    else if (pin_index == 1) IRQn = EXTI1_IRQn;
    else if (pin_index == 2) IRQn = EXTI2_IRQn;
    else if (pin_index == 3) IRQn = EXTI3_IRQn;
    else if (pin_index == 4) IRQn = EXTI4_IRQn;
    else if (pin_index >= 5 && pin_index <= 9) IRQn = EXTI9_5_IRQn;
    else IRQn = EXTI15_10_IRQn;

    // 配置中断优先级并使能
    HAL_NVIC_SetPriority(IRQn, priority, 0);
    HAL_NVIC_EnableIRQ(IRQn);
}


// 辅助函数，检测是否中断，指定传入的回调函数
void Interrupt_Check(uint16_t GPIO_Pin, void (*callback)()) {
    static uint32_t last_time = 0;

    if (__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) == 0x00u)
    {
        return;

    }

    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);

    uint32_t current_time = HAL_GetTick();
    if (current_time - last_time < 10) {

        return;
    }
    last_time = current_time;
    USART1_Printf("%lu\r\n", last_time);
    callback();
}

// 6种中断函数
// 配置多个引脚使用不同的EXTI线
void EXTI0_IRQHandler(void)    {
    Interrupt_Check(GPIO_PIN_0, Interrupt_0);
}
void EXTI1_IRQHandler(void)    {
    Interrupt_Check(GPIO_PIN_1, Interrupt_1);
}
void EXTI2_IRQHandler(void)    {
    Interrupt_Check(GPIO_PIN_2, Interrupt_2);
}
void EXTI3_IRQHandler(void)    {
    Interrupt_Check(GPIO_PIN_3, Interrupt_3);
}
void EXTI4_IRQHandler(void)    {
    Interrupt_Check(GPIO_PIN_4, Interrupt_4);
}
void EXTI9_5_IRQHandler(void)  {
    Interrupt_Check(GPIO_PIN_5, Interrupt_5);
    Interrupt_Check(GPIO_PIN_6, Interrupt_6);
    Interrupt_Check(GPIO_PIN_7, Interrupt_7);
    Interrupt_Check(GPIO_PIN_8, Interrupt_8);
    Interrupt_Check(GPIO_PIN_9, Interrupt_9);
}
void EXTI15_10_IRQHandler(void) {
    Interrupt_Check(GPIO_PIN_10, Interrupt_10);
    Interrupt_Check(GPIO_PIN_11, Interrupt_11);
    Interrupt_Check(GPIO_PIN_12, Interrupt_12);
    Interrupt_Check(GPIO_PIN_13, Interrupt_13);
    Interrupt_Check(GPIO_PIN_14, Interrupt_14);
    Interrupt_Check( GPIO_PIN_15, Interrupt_15);
}

// 16个引脚的中断函数的默认实现
__weak void Interrupt_0(void) {
    USART1_Printf("Interrupt_0\r\n");
}
__weak void Interrupt_1(void) {
    USART1_Printf("Interrupt_1\r\n");
}
__weak void Interrupt_2(void) {
    USART1_Printf("Interrupt_2\r\n");
}
__weak void Interrupt_3(void) {
    USART1_Printf("Interrupt_3\r\n");
}
__weak void Interrupt_4(void) {
    USART1_Printf("Interrupt_4\r\n");
}
__weak void Interrupt_5(void) {
    USART1_Printf("Interrupt_5\r\n");
}
__weak void Interrupt_6(void) {
    USART1_Printf("Interrupt_6\r\n");
}
__weak void Interrupt_7(void) {
    USART1_Printf("Interrupt_7\r\n");
}
__weak void Interrupt_8(void) {
    USART1_Printf("Interrupt_8\r\n");
}
__weak void Interrupt_9(void) {
    USART1_Printf("Interrupt_9\r\n");
}
__weak void Interrupt_10(void) {
    USART1_Printf("Interrupt_10\r\n");
}
__weak void Interrupt_11(void) {
    USART1_Printf("Interrupt_11\r\n");
}
__weak void Interrupt_12(void) {
    USART1_Printf("Interrupt_12\r\n");
}
__weak void Interrupt_13(void) {
    USART1_Printf("Interrupt_13\r\n");
}
__weak void Interrupt_14(void) {
    USART1_Printf("Interrupt_14\r\n");
}
__weak void Interrupt_15(void) {
    USART1_Printf("Interrupt_15\r\n");
}
