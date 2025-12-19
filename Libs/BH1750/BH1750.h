#ifndef GUARDIANELF_BH1750_H
#define GUARDIANELF_BH1750_H

#include "main.h"
#include "I2C.h"

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
void BH1750_WriteCmd(uint8_t cmd);
uint16_t BH1750_ReadData(void);
uint32_t BH1750_GetLux(void);

#endif //GUARDIANELF_BH1750_H