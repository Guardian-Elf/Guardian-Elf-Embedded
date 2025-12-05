#include <stdarg.h>
#include <stdio.h>
#include "USART2.h"
#include "USART1.h"

// USART2 接收缓冲区
uint8_t USART2_buffer[USART2_R_SIZE] = {0};
uint32_t USART2_size = 0;

void USART2_Init(void) {
    // 1. 时钟使能 - 使用默认PA2, PA3
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;  // GPIOA时钟

    // USART2时钟（APB1总线）
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // 2. 移除重映射代码
    // AFIO->MAPR |= AFIO_MAPR_USART2_REMAP;  // 删除这行

    // 3. 引脚配置 - PA2(TX), PA3(RX)

    // 配置PA2为复用推挽输出（TX）
    GPIOA->CRL &= ~GPIO_CRL_CNF2;  // 清除配置位
    GPIOA->CRL |= GPIO_CRL_CNF2_1; // 复用推挽输出
    GPIOA->CRL |= GPIO_CRL_MODE2;  // 输出模式，最大速度50MHz

    // 配置PA3为浮空输入（RX）
    GPIOA->CRL &= ~GPIO_CRL_MODE3;  // 输入模式
    GPIOA->CRL &= ~GPIO_CRL_CNF3;   // 清除配置位
    GPIOA->CRL |= GPIO_CRL_CNF3_0;  // 浮空输入

    // 4. USART2配置
    USART2->BRR = 0xEA6;  // 9600 @36MHz

    USART2->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
    USART2->CR1 &= ~USART_CR1_M;
    USART2->CR1 &= ~USART_CR1_PCE;
    USART2->CR2 &= ~USART_CR2_STOP;

    // 中断配置
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
        USART2_size = 0;  // 清零缓冲区
        USART2_ReceiveCallback();
    }
}

int USART2_Printf(const char *format, ...) {
    char buffer[USART2_T_SIZE] = {"\0"};
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

/**
  * @brief  接收回调函数（弱定义）
  * @note   用户需要在其他文件中重写此函数以实现具体功能
  */
__attribute__((weak)) void USART2_ReceiveCallback(void) {
    // 弱定义，用户可在其他文件中重新实现
    USART2_Printf("默认实现打印数据: %s\r\n", USART2_buffer);
}