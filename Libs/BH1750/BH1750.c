#include "BH1750.h"

// 简化的引脚操作宏
#define SCL_H  HAL_GPIO_WritePin(BH1750_PORT, BH1750_SCL_PIN, GPIO_PIN_SET)
#define SCL_L  HAL_GPIO_WritePin(BH1750_PORT, BH1750_SCL_PIN, GPIO_PIN_RESET)
#define SDA_H  HAL_GPIO_WritePin(BH1750_PORT, BH1750_SDA_PIN, GPIO_PIN_SET)
#define SDA_L  HAL_GPIO_WritePin(BH1750_PORT, BH1750_SDA_PIN, GPIO_PIN_RESET)
#define SDA_R  HAL_GPIO_ReadPin(BH1750_PORT, BH1750_SDA_PIN)

uint32_t BH1750_Lux = 0;

/**
  * @brief  初始化BH1750 GPIO引脚
  */
void BH1750_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = BH1750_SCL_PIN | BH1750_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BH1750_PORT, &GPIO_InitStruct);

    SCL_H;
    SDA_H;
    HAL_Delay(100);
}

/**
  * @brief  I2C起始信号
  */
void BH1750_Start(void)
{
    SDA_H; SCL_H;
    SDA_L; SCL_L;
}

/**
  * @brief  I2C停止信号
  */
void BH1750_Stop(void)
{
    SDA_L; SCL_H;
    SDA_H;
}

/**
  * @brief  发送一个字节
  */
void BH1750_SendByte(uint8_t dat)
{
    for (uint8_t i = 0; i < 8; i++) {
        (dat & 0x80) ? SDA_H : SDA_L;
        dat <<= 1;
        SCL_H; SCL_L;
    }
    BH1750_RecvACK();
}

/**
  * @brief  接收一个字节
  */
uint8_t BH1750_RecvByte(void)
{
    uint8_t dat = 0;
    GPIO_InitTypeDef GPIO_InitStruct = {
            .Pin = BH1750_SDA_PIN,
            .Mode = GPIO_MODE_INPUT,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_HIGH
    };
    HAL_GPIO_Init(BH1750_PORT, &GPIO_InitStruct);
    SDA_H;

    for (uint8_t i = 0; i < 8; i++) {
        dat <<= 1;
        SCL_H;
        if(SDA_R == GPIO_PIN_SET) dat |= 1;
        SCL_L;
    }

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init(BH1750_PORT, &GPIO_InitStruct);
    return dat;
}

/**
  * @brief  发送ACK/NACK
  */
void BH1750_SendACK(uint8_t ack)
{
    ack ? SDA_H : SDA_L;
    SCL_H; SCL_L;
}

/**
  * @brief  接收ACK信号
  */
uint8_t BH1750_RecvACK(void)
{
    uint8_t ack = 0;
    GPIO_InitTypeDef GPIO_InitStruct = {
            .Pin = BH1750_SDA_PIN,
            .Mode = GPIO_MODE_INPUT,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_HIGH
    };
    HAL_GPIO_Init(BH1750_PORT, &GPIO_InitStruct);
    SDA_H;

    SCL_H;
    if(SDA_R == GPIO_PIN_SET) ack = 1;
    SCL_L;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init(BH1750_PORT, &GPIO_InitStruct);
    return ack;
}

/**
  * @brief  向BH1750发送命令
  */
void BH1750_WriteCmd(uint8_t cmd)
{
    BH1750_Start();
    BH1750_SendByte(BH1750_ADDR_WRITE);
    BH1750_SendByte(cmd);
    BH1750_Stop();
}

/**
  * @brief  从BH1750读取数据
  */
uint16_t BH1750_ReadData(void)
{
    uint16_t data;

    BH1750_Start();
    BH1750_SendByte(BH1750_ADDR_READ);

    data = BH1750_RecvByte() << 8;
    BH1750_SendACK(0);

    data |= BH1750_RecvByte();
    BH1750_SendACK(1);

    BH1750_Stop();
    return data;
}

/**
  * @brief  获取光照强度
  */
uint32_t BH1750_GetLux(void)
{
    BH1750_WriteCmd(BH1750_PWR_ON);
    HAL_Delay(10);

    BH1750_WriteCmd(BH1750_ONE_H);
    HAL_Delay(180);
    BH1750_Lux = (uint32_t)BH1750_ReadData() * 10 / 12;
    return BH1750_Lux;
}