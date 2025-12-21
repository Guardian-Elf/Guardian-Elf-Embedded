#include "BH1750.h"
#include "Delay.h"

// I2C地址
#define BH1750_ADDR  0x23  // ADDR接地时的写地址

// BH1750命令集
#define BH1750_PWR_DOWN     0x00 //关闭模块
#define BH1750_PWR_ON       0x01 //打开模块等待测量命令
#define BH1750_RST          0x07 //重置数据寄存器值在PowerOn模式下有效
#define BH1750_CON_H        0x10 // 连续高分辨率模式,1lx,120ms
#define BH1750_CON_H2       0x11 // 连续高分辨率模式.5lx,120ms
#define BH1750_CON_L        0x13 // 连续低分辨率模式,4lx,16ms
#define BH1750_ONE_H        0x20 // 一次高分辨率模式,1lx,120ms,测量后模块转到PowerDown模式
#define BH1750_ONE_H2       0x21 // 一次高分辨率模式,0.5lx,120ms,测量后模块转到PowerDown模式
#define BH1750_ONE_L        0x23 // 一次低分辨率模式,4lx,16ms,测量后模块转到PowerDown模式


uint32_t BH1750_Lux = 0;


/**
  * @brief  向BH1750发送命令
  */
void BH1750_WriteCmd(uint8_t cmd)
{
    I2C_SendBytes(BH1750_ADDR, &cmd, 1);
}

/**
  * @brief  从BH1750读取数据
  */
uint16_t BH1750_ReadData(void)
{
    uint16_t data;
    uint8_t  buf[2];

    I2C_RecvBytes(BH1750_ADDR, buf, 2);
    data = buf[0] << 8 | buf[1];

    return data;
}

/**
  * @brief  获取光照强度
  */
uint32_t BH1750_GetLux(void)
{
    BH1750_WriteCmd(BH1750_PWR_ON);

    BH1750_WriteCmd(BH1750_ONE_H);
    HAL_Delay(80);
    BH1750_Lux = (uint32_t)BH1750_ReadData() * 10 / 12;
    return BH1750_Lux;
}