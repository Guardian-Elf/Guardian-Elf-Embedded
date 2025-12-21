//
// Created by pc on 2025/12/21.
//

#ifndef GUARDIANELF_DMA_H
#define GUARDIANELF_DMA_H
#include "stm32f1xx_hal.h"

extern DMA_HandleTypeDef hdma11;

void DMA_Init(DMA_Channel_TypeDef * DMA_Channel);

void DMA1_Channel1_CallBack();
void DMA1_Channel4_CallBack();


#endif //GUARDIANELF_DMA_H
