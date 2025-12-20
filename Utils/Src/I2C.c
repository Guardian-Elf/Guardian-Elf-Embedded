//
// Created by pc on 2025/12/18.
//

#include "I2C.h"
#include "Delay.h"

// 引脚配置
#define I2C_SCL_PIN GPIO_PIN_4
#define I2C_SDA_PIN GPIO_PIN_5
#define I2C_PORT    GPIOA

/**
  * @brief  初始化BH1750 GPIO引脚
  */
void I2C_Init(void)
{
    /* GPIO端口时钟使能 */
    __HAL_RCC_GPIOB_CLK_ENABLE();  // 开启I2C_PORT的时钟

    /* GPIO初始化结构体配置 */
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* SCL引脚配置 (PB10) */
    GPIO_InitStruct.Pin = I2C_SCL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;  // 开漏输出
    GPIO_InitStruct.Pull = GPIO_NOPULL;          // 不使用上下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);

    /* SDA引脚配置 (PB11) */
    GPIO_InitStruct.Pin = I2C_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;  // 开漏输出
    GPIO_InitStruct.Pull = GPIO_NOPULL;          // 不使用上下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);

    /* 设置默认电平 */
    HAL_GPIO_WritePin(I2C_PORT, I2C_SCL_PIN | I2C_SDA_PIN, GPIO_PIN_SET);
}

void I2C_W_SCL(uint8_t BitValue) {
    // 使用HAL_GPIO_WritePin函数
    HAL_GPIO_WritePin(I2C_PORT, I2C_SCL_PIN, (GPIO_PinState)BitValue); // 根据BitValue设置SCL引脚电平
    Delay_us(10);
}

void I2C_W_SDA(uint8_t BitValue) {
    // 使用HAL_GPIO_WritePin函数
    HAL_GPIO_WritePin(I2C_PORT, I2C_SDA_PIN, (GPIO_PinState)BitValue); // 根据BitValue设置SDA引脚电平
    Delay_us(10);
}

uint8_t I2C_R_SDA(void) {
    GPIO_PinState pinState;
    pinState = HAL_GPIO_ReadPin(I2C_PORT, I2C_SDA_PIN); // 读取SDA电平
    Delay_us(10);  // 改为微秒级延时
    return (uint8_t)pinState; // 返回SDA电平
}

/**
  * @brief  I2C起始信号
  */
void I2C_Start(void)
{
    I2C_W_SDA(1);							//释放SDA，确保SDA为高电平
    I2C_W_SCL(1);							//释放SCL，确保SCL为高电平
    I2C_W_SDA(0);							//在SCL高电平期间，拉低SDA，产生起始信号
    I2C_W_SCL(0);							//起始后把SCL也拉低，即为了占用总线，也为了方便总线时序的拼接

}

/**
  * @brief  I2C停止信号
  */
void I2C_Stop(void)
{
    I2C_W_SDA(0);							//拉低SDA，确保SDA为低电平
    I2C_W_SCL(1);							//释放SCL，使SCL呈现高电平
    I2C_W_SDA(1);							//在SCL高电平期间，释放SDA，产生终止信号

}

/**
  * @brief  接收ACK信号
  */
uint8_t I2C_RecvACK(void)
{
    uint8_t AckBit;							//定义应答位变量
    I2C_W_SDA(1);							//接收前，主机先确保释放SDA，避免干扰从机的数据发送
    I2C_W_SCL(1);							//释放SCL，主机机在SCL高电平期间读取SDA
    AckBit = I2C_R_SDA();					//将应答位存储到变量里
    I2C_W_SCL(0);							//拉低SCL，开始下一个时序模块
    return AckBit;							//返回定义应答位变量
}

/**
  * @brief  发送ACK/NACK
  */
void I2C_SendACK(uint8_t ack)
{
    I2C_W_SDA(ack);					//主机把应答位数据放到SDA线
    I2C_W_SCL(1);							//释放SCL，从机在SCL高电平期间，读取应答位
    I2C_W_SCL(0);							//拉低SCL，开始下一个时序模块
}

/**
  * @brief  发送一个字节
  */
void I2C_SendByte(uint8_t dat)
{
    uint8_t i;
    for (i = 0; i < 8; i ++)				//循环8次，主机依次发送数据的每一位
    {
        /*两个!可以对数据进行两次逻辑取反，作用是把非0值统一转换为1，即：!!(0) = 0，!!(非0) = 1*/
        I2C_W_SDA(!!(dat & (0x80 >> i)));//使用掩码的方式取出Byte的指定一位数据并写入到SDA线
        I2C_W_SCL(1);						//释放SCL，从机在SCL高电平期间读取SDA
        I2C_W_SCL(0);						//拉低SCL，主机开始发送下一位数据
    }
}

/**
  * @brief  接收一个字节
  */
uint8_t I2C_RecvByte(void)
{
    uint8_t i, Byte = 0x00;					//定义接收的数据，并赋初值0x00，此处必须赋初值0x00，后面会用到
    I2C_W_SDA(1);							//接收前，主机先确保释放SDA，避免干扰从机的数据发送
    for (i = 0; i < 8; i ++)				//循环8次，主机依次接收数据的每一位
    {
        I2C_W_SCL(1);						//释放SCL，主机机在SCL高电平期间读取SDA
        if (I2C_R_SDA()){Byte |= (0x80 >> i);}	//读取SDA数据，并存储到Byte变量
        //当SDA为1时，置变量指定位为1，当SDA为0时，不做处理，指定位为默认的初值0
        I2C_W_SCL(0);						//拉低SCL，从机在SCL低电平期间写入SDA
    }
    return Byte;							//返回接收到的一个字节数据
}