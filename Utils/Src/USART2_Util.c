#include <stdarg.h>
#include <stdio.h>
#include "USART2_Util.h"

// USART2 接收缓冲区
uint8_t USART2_buffer[100];
uint8_t USART2_size = 0;

void USART2_Init(void) {
    // 1. 时钟使能

    // 引脚时钟（使用PD5-TX, PD6-RX）
    RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;

    // 配置USART2时钟（USART2在APB1总线上）
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // 复用功能时钟（重映射需要AFIO）
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    // 2. 使能部分重映射（重映射到PD5, PD6）
    AFIO->MAPR |= AFIO_MAPR_USART2_REMAP;

    // 3. 引脚配置 - PD5(TX), PD6(RX)

    // 配置PD5为复用推挽输出（TX）
    GPIOD->CRL &= ~GPIO_CRL_CNF5;  // 清除配置位
    GPIOD->CRL |= GPIO_CRL_CNF5_1; // 复用推挽输出
    GPIOD->CRL |= GPIO_CRL_MODE5;  // 输出模式，最大速度50MHz

    // 配置PD6为浮空输入（RX）
    GPIOD->CRL &= ~GPIO_CRL_MODE6;  // 输入模式
    GPIOD->CRL &= ~GPIO_CRL_CNF6;   // 清除配置位
    GPIOD->CRL |= GPIO_CRL_CNF6_0;  // 浮空输入

    // 4. USART2配置

    // 波特率 115200（系统时钟36MHz）
    USART2->BRR = 0x139;

    // 使能USART2
    USART2->CR1 |= USART_CR1_UE; // 使能USART2
    USART2->CR1 |= USART_CR1_TE; // 发送使能
    USART2->CR1 |= USART_CR1_RE; // 接收使能

    // 其他配置
    USART2->CR1 &= ~USART_CR1_M;   // 数据字长8位
    USART2->CR1 &= ~USART_CR1_PCE; // 禁止奇偶校验
    USART2->CR2 &= ~USART_CR2_STOP; // 1个停止位

    // 中断配置（与USART1保持一致）
    USART2->CR1 |= USART_CR1_IDLEIE | USART_CR1_RXNEIE;
    NVIC_SetPriorityGrouping(0);
    NVIC_SetPriority(USART2_IRQn, 3);
    NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_SendByte(uint8_t byte) {
    // 等待发送缓冲区为空
    while ((USART2->SR & USART_SR_TXE) == 0);

    // 发送数据
    USART2->DR = byte;
}

void USART2_SendString(char *str) {
    while (*str != '\0') {
        USART2_SendByte(*str);
        str++;
    }
}

void USART2_IRQHandler(void) {
    if (USART2->SR & USART_SR_RXNE) {  // 先处理接收
        if (USART2_size < sizeof(USART2_buffer) - 1) {
            USART2_buffer[USART2_size++] = USART2->DR;
        }
    } else if (USART2->SR & USART_SR_IDLE) {  // 再处理空闲中断
        volatile uint32_t temp = USART2->DR;  // 清除IDLE标志
        (void)temp;
        USART2_buffer[USART2_size] = '\0';  // 添加字符串结束符
        USART2_Printf("USART2 Received: %s\r\n", USART2_buffer);
        USART2_size = 0;  // 清零缓冲区
    }
}

int USART2_Printf(const char *format, ...) {
    char buffer[256];
    va_list args;

    va_start(args, format);
    int len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (len > 0) {
        // 发送字符串
        char *p = buffer;
        while (*p) {
            USART2_SendByte(*p++);
        }
    }

    return len;
}