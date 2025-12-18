#ifndef GUARDIANELF_BH1750_H
#define GUARDIANELF_BH1750_H

#include "main.h"

// 引脚配置 - 根据您的要求改为PA4、PA5
#define BH1750_SCL_PIN GPIO_PIN_4
#define BH1750_SDA_PIN GPIO_PIN_5
#define BH1750_PORT    GPIOA

// I2C地址
#define BH1750_ADDR_WRITE  0x46  // ADDR接地时的写地址
#define BH1750_ADDR_READ   0x47  // ADDR接地时的读地址

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

// 储存光照数据
extern uint32_t BH1750_Lux;

// 函数声明
void BH1750_Init(void);
void BH1750_Start(void);
void BH1750_Stop(void);
void BH1750_SendByte(uint8_t dat);
uint8_t BH1750_RecvByte(void);
void BH1750_SendACK(uint8_t ack);
uint8_t BH1750_RecvACK(void);
void BH1750_WriteCmd(uint8_t cmd);
uint16_t BH1750_ReadData(void);
uint32_t BH1750_GetLux(void);

#endif //GUARDIANELF_BH1750_H