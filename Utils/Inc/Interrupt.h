//
// Created by pc on 2025/12/19.
//

#ifndef GUARDIANELF_INTERRUPT_H
#define GUARDIANELF_INTERRUPT_H
#include "main.h"

// 初始化中断(适配STM32F103C8T6,只有A B C)
void Interrupt_Init(GPIO_TypeDef *GPIO_PORT, uint32_t GPIO_PIN,
                    uint8_t active_level, uint32_t priority);

// 16个引脚的中断函数
void Interrupt_0(void);
void Interrupt_1(void);
void Interrupt_2(void);
void Interrupt_3(void);
void Interrupt_4(void);
void Interrupt_5(void);
void Interrupt_6(void);
void Interrupt_7(void);
void Interrupt_8(void);
void Interrupt_9(void);
void Interrupt_10(void);
void Interrupt_11(void);
void Interrupt_12(void);
void Interrupt_13(void);
void Interrupt_14(void);
void Interrupt_15(void);


#endif //GUARDIANELF_INTERRUPT_H
