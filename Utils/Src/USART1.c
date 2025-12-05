//
// Created by pc on 2025/11/30.
//

#include <stdarg.h>
#include <stdio.h>
#include "USART1.h"

// USART1 接收缓冲区
uint8_t USART1_buffer[USART1_R_SIZE] = {0};
uint32_t USART1_size = 0;

void USART1_Init(void) {
    // 1. 时钟使能 - 使用默认PA9, PA10
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;  // GPIOA时钟
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // 2. 移除重映射代码（不进行重映射）
    // AFIO->MAPR = AFIO_MAPR_USART1_REMAP;  // 删除这行

    // 3. 引脚配置 - PA9(TX), PA10(RX)

    // 配置PA9为复用推挽输出 (USART1_TX)
    GPIOA->CRH &= ~GPIO_CRH_CNF9;  // 清除配置位
    GPIOA->CRH |= GPIO_CRH_CNF9_1;  // 复用推挽输出
    GPIOA->CRH |= GPIO_CRH_MODE9;   // 输出模式，最大速度50MHz

    // 配置PA10为浮空输入 (USART1_RX)
    GPIOA->CRH &= ~GPIO_CRH_MODE10;  // 输入模式
    GPIOA->CRH &= ~GPIO_CRH_CNF10;   // 清除配置位
    GPIOA->CRH |= GPIO_CRH_CNF10_0;  // 浮空输入

    // 4. USART1配置 (保持不变)
    USART1->BRR = 0x271;  // 115200 @72MHz
    USART1->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
    USART1->CR1 &= ~USART_CR1_M;
    USART1->CR1 &= ~USART_CR1_PCE;
    USART1->CR2 &= ~USART_CR2_STOP;

    // 中断配置
    USART1->CR1 |= USART_CR1_IDLEIE | USART_CR1_RXNEIE;
    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART1_IRQn, 3);
    NVIC_EnableIRQ(USART1_IRQn);
}

void USART1_SendByte(uint8_t byte) {
    while ((USART1->SR & USART_SR_TXE) == 0);

    USART1->DR = byte;
}

void USART1_SendString(char *str) {
    while (*str != '\0') {
        USART1_SendByte(*str);
        str++;
    }
}

void USART1_SendStringWithLen(char *str, int len) {
    while (len--) {
        USART1_SendByte(*str);
        str++;
    }
}

void USART1_IRQHandler(void) {
    if (USART1->SR & USART_SR_RXNE) {  // 先处理接收
        USART1_buffer[USART1_size++] = USART1->DR;
        // 防止缓冲区溢出
        if (USART1_size >= sizeof(USART1_buffer) - 1) {
            USART1_size = 0;
        }
    } else if (USART1->SR & USART_SR_IDLE) {  // 再处理空闲中断
        USART1->DR;  // 清除IDLE标志
        USART1_buffer[USART1_size] = '\0';  // 添加字符串结束符
        USART1_size = 0;  // 清零缓冲区
        USART1_ReceiveCallback();
    }
}

int USART1_Printf(const char *format, ...) {
    char buffer[USART1_T_SIZE] = {'\0'};
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // 发送字符串
    char *p = buffer;
    while (*p) {
        USART1_SendByte(*p++);
    }
}

/**
  * @brief  接收回调函数（弱定义）
  * @note   用户需要在其他文件中重写此函数以实现具体功能
  */
__attribute__((weak)) void USART1_ReceiveCallback(void) {
    // 弱定义，用户可在其他文件中重新实现
    USART1_Printf("默认实现打印数据: %s\r\n", USART1_buffer);
}