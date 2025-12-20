#include "BH1750.h"


uint32_t BH1750_Lux = 0;


/**
  * @brief  向BH1750发送命令
  */
void BH1750_WriteCmd(uint8_t cmd)
{
    I2C_Start();
    I2C_SendByte(BH1750_ADDR_WRITE);
    I2C_RecvACK();
    I2C_SendByte(cmd);
    I2C_RecvACK();
    I2C_Stop();
}

/**
  * @brief  从BH1750读取数据
  */
uint16_t BH1750_ReadData(void)
{
    uint16_t data;

    I2C_Start();
    I2C_SendByte(BH1750_ADDR_READ);
    I2C_RecvACK();

    data = I2C_RecvByte() << 8;
    I2C_SendACK(0);

    data |= I2C_RecvByte();
    I2C_SendACK(1);

    I2C_Stop();
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