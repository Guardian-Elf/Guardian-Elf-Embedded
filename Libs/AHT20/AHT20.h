#ifndef GUARDIANELF__AHT20_H
#define GUARDIANELF__AHT20_H

#include "stm32f1xx_hal.h"
#include "I2C.h"

extern float AHT20_Temperature, AHT20_Humidity;

void AHT20_Init();

void AHT20_ReadData();


#endif //GUARDIANELF__AHT20_H