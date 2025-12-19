//
// Created by pc on 2025/12/18.
//

#ifndef GUARDIANELF_I2C_H
#define GUARDIANELF_I2C_H
#include "main.h"

// 引脚配置
#define I2C_SCL_PIN GPIO_PIN_4
#define I2C_SDA_PIN GPIO_PIN_5
#define I2C_PORT    GPIOA

// 函数声明
void I2C_Init(void);
void I2C_WriteCmd(uint8_t cmd, uint8_t addr);
uint16_t I2C_ReadData(uint8_t addr);

#endif //GUARDIANELF_I2C_H
