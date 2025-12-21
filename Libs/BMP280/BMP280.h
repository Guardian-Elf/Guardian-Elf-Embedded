//
// Created by pc on 2025/12/20.
//

#ifndef GUARDIANELF_BMP280_H
#define GUARDIANELF_BMP280_H

#include "stm32f1xx_hal.h"
#include "I2C.h"

extern double BMP280_Temp, BMP280_Press;


uint8_t BMP280_GetDeviceID();

void BMP280_Init();

void BMP280_ReadData();

#endif //GUARDIANELF_BMP280_H
