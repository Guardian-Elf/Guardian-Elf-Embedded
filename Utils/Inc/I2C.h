//
// Created by pc on 2025/12/18.
//

#ifndef GUARDIANELF_I2C_H
#define GUARDIANELF_I2C_H
#include "main.h"


// 函数声明
void I2C_Init(void);

/**
  * @brief  I2C起始信号
  */
void I2C_Start(void);

/**
  * @brief  I2C停止信号
  */
void I2C_Stop(void);

/**
  * @brief  接收ACK信号
  */
uint8_t I2C_RecvACK(void);

/**
  * @brief  发送ACK/NACK
  */
void I2C_SendACK(uint8_t ack);

/**
  * @brief  发送一个字节
  */
void I2C_SendByte(uint8_t dat);

/**
  * @brief  接收一个字节
  */
uint8_t I2C_RecvByte(void);

#endif //GUARDIANELF_I2C_H
