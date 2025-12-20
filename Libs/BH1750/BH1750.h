#ifndef GUARDIANELF_BH1750_H
#define GUARDIANELF_BH1750_H

#include "main.h"
#include "I2C.h"

// 储存光照数据
extern uint32_t BH1750_Lux;

// 函数声明
void BH1750_WriteCmd(uint8_t cmd);
uint16_t BH1750_ReadData(void);
uint32_t BH1750_GetLux(void);

#endif //GUARDIANELF_BH1750_H