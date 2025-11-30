//
// Created by pc on 2025/11/30.
//


#include "Interrupt_demo.h"
#include "stm32f103xe.h"

void Interrupt_demo01(void) {
    // 1. 时钟使能
    // 引脚A时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // AFIO(引脚复用选择器，将A~G的相同引脚封装成一个总线发给EXTI)时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    // 2. 引脚设置

    // 设置引脚模式,上下拉模式
    GPIOA->CRL &= ~GPIO_CRL_MODE1;
    GPIOA->CRL |= GPIO_CRL_CNF1_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF1_0;

    GPIOA->CRL &= ~GPIO_CRL_MODE2;
    GPIOA->CRL |= GPIO_CRL_CNF2_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF2_0;


    // 置0表述下拉
    GPIOA->ODR &= ~GPIO_ODR_ODR1;
    GPIOA->ODR &= ~GPIO_ODR_ODR2;

    // led引脚配置为推挽输出
    GPIOA->CRL &= ~GPIO_CRL_CNF3;
    GPIOA->CRL |= GPIO_CRL_MODE3;



    // 3. 配置中断

    // AFIO配置引脚复用选择A1.2引脚
    AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI1_PA;
    AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI2_PA;

    // 配置中断触发方式(上升沿触发)
    EXTI->RTSR |= EXTI_RTSR_TR1;
    EXTI->RTSR |= EXTI_RTSR_TR2;


    // 事件屏蔽寄存器（开发中断事件请求）
    EXTI->IMR |= EXTI_IMR_MR1;
    EXTI->IMR |= EXTI_IMR_MR2;

    // 4. 配置NVIC

    // 全部都是抢占优先级
    NVIC_SetPriorityGrouping(3);

    // 设置优先级
    NVIC_SetPriority(EXTI1_IRQn, 3);
    NVIC_SetPriority(EXTI2_IRQn, 3);

    // NVIC启动中断
    NVIC_EnableIRQ(EXTI1_IRQn);
    NVIC_EnableIRQ(EXTI2_IRQn);
}





// 中断服务函数(名称固定)
void EXTI1_IRQHandler(void) {
    // 清除中断信号
    EXTI->PR |= EXTI_PR_PR1;


    GPIOA->ODR |= GPIO_ODR_ODR3;
}

void EXTI2_IRQHandler(void) {
    // 清除中断信号
    EXTI->PR |= EXTI_PR_PR2;


    GPIOA->ODR &= ~GPIO_ODR_ODR3;
}