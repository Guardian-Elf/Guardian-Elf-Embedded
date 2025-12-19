//
// Created by pc on 2025/12/18.
//

#include "I2C.h"

// 简化的引脚操作宏
#define SCL_H  HAL_GPIO_WritePin(I2C_PORT, I2C_SCL_PIN, GPIO_PIN_SET)
#define SCL_L  HAL_GPIO_WritePin(I2C_PORT, I2C_SCL_PIN, GPIO_PIN_RESET)
#define SDA_H  HAL_GPIO_WritePin(I2C_PORT, I2C_SDA_PIN, GPIO_PIN_SET)
#define SDA_L  HAL_GPIO_WritePin(I2C_PORT, I2C_SDA_PIN, GPIO_PIN_RESET)
#define SDA_R  HAL_GPIO_ReadPin(I2C_PORT, I2C_SDA_PIN)

/**
  * @brief  初始化BH1750 GPIO引脚
  */
void I2C_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = I2C_SCL_PIN | I2C_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);

    SCL_H;
    SDA_H;
    HAL_Delay(100);
}

/**
  * @brief  I2C起始信号
  */
void I2C_Start(void)
{
    SDA_H; SCL_H;
    SDA_L; SCL_L;
}

/**
  * @brief  I2C停止信号
  */
void I2C_Stop(void)
{
    SDA_L; SCL_H;
    SDA_H;
}

/**
  * @brief  接收ACK信号
  */
uint8_t I2C_RecvACK(void)
{
    uint8_t ack = 0;
    GPIO_InitTypeDef GPIO_InitStruct = {
            .Pin = I2C_SDA_PIN,
            .Mode = GPIO_MODE_INPUT,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_HIGH
    };
    HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);
    SDA_H;

    SCL_H;
    if(SDA_R == GPIO_PIN_SET) ack = 1;
    SCL_L;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);
    return ack;
}

/**
  * @brief  发送ACK/NACK
  */
void I2C_SendACK(uint8_t ack)
{
    ack ? SDA_H : SDA_L;
    SCL_H; SCL_L;
}

/**
  * @brief  发送一个字节
  */
void I2C_SendByte(uint8_t dat)
{
    for (uint8_t i = 0; i < 8; i++) {
        (dat & 0x80) ? SDA_H : SDA_L;
        dat <<= 1;
        SCL_H; SCL_L;
    }
    I2C_RecvACK();
}

/**
  * @brief  接收一个字节
  */
uint8_t I2C_RecvByte(void)
{
    uint8_t dat = 0;
    GPIO_InitTypeDef GPIO_InitStruct = {
            .Pin = I2C_SDA_PIN,
            .Mode = GPIO_MODE_INPUT,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_HIGH
    };
    HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);
    SDA_H;

    for (uint8_t i = 0; i < 8; i++) {
        dat <<= 1;
        SCL_H;
        if(SDA_R == GPIO_PIN_SET) dat |= 1;
        SCL_L;
    }

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);
    return dat;
}





/**
  * @brief  向BH1750发送命令
  */
void I2C_WriteCmd(uint8_t cmd, uint8_t addr)
{
    I2C_Start();
    I2C_SendByte(addr);
    I2C_SendByte(cmd);
    I2C_Stop();
}

/**
  * @brief  从BH1750读取数据
  */
uint16_t I2C_ReadData(uint8_t addr)
{
    uint16_t data;

    I2C_Start();
    I2C_SendByte(addr);

    data = I2C_RecvByte() << 8;
    I2C_SendACK(0);

    data |= I2C_RecvByte();
    I2C_SendACK(1);

    I2C_Stop();
    return data;
}
