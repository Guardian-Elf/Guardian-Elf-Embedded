//
// Created by pc on 2025/12/18.
//

#ifndef GUARDIANELF_I2C_H
#define GUARDIANELF_I2C_H

#include "main.h"

// 基本操作函数

/**
 * @brief  I2C初始化
 */
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

// 通用高级操作函数

/**
 * @brief  检测设备应答
 */
uint8_t I2C_CheckDevice(uint8_t addr);

/**
 * @brief  遍历检测总线上设备
 */
uint8_t I2C_ScanDevice(uint8_t* devices, uint8_t max);

/**
 * @brief  读寄存器数据
 */
uint8_t I2C_ReadReg(uint8_t addr, uint8_t reg);

/**
 * @brief  写寄存器数据
 */
void I2C_WriteReg(uint8_t addr, uint8_t reg, uint8_t data);

/**
 * @brief  连续发送多个字节
 */
void I2C_SendBytes(uint8_t addr, uint8_t *buf, uint8_t len);

/*
 * @brief  连续接收多个字节
 */
void I2C_RecvBytes(uint8_t addr, uint8_t *buf, uint8_t len);


#endif //GUARDIANELF_I2C_H
