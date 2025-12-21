//
// Created by pc on 2025/12/21.
//

#include "DMA.h"
#include "USART.h"

DMA_HandleTypeDef hdma11;
DMA_HandleTypeDef hdma14;

void DMA1_Channel1_XferCpltCallback(DMA_HandleTypeDef *hdma) {
    DMA1_Channel1_CallBack();
    HAL_DMA_Abort(&hdma11);
}

void DMA1_Channel4_XferCpltCallback(DMA_HandleTypeDef *hdma) {
    DMA1_Channel4_CallBack();
    HAL_DMA_Abort(&hdma14);
}

void DMA_Init11(void) {
    __HAL_RCC_DMA1_CLK_ENABLE();

    hdma11.Instance = DMA1_Channel1;
    hdma11.Init.Direction = DMA_MEMORY_TO_MEMORY;  // 内存到内存
    hdma11.Init.PeriphInc = DMA_PINC_ENABLE;       // 外设地址递增
    hdma11.Init.MemInc = DMA_MINC_ENABLE;          // 内存地址递增
    hdma11.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  // 8位
    hdma11.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     // 8位
    hdma11.Init.Mode = DMA_NORMAL;                 // 普通模式
    hdma11.Init.Priority = DMA_PRIORITY_LOW;      // 低优先级

    /* 初始化DMA */
    HAL_DMA_Init(&hdma11);

    /* 配置中断（可选） */
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

    hdma11.XferCpltCallback = DMA1_Channel1_XferCpltCallback;
}

void DMA_Init14(void) {
    __HAL_RCC_DMA1_CLK_ENABLE();

    hdma14.Instance = DMA1_Channel4;
    hdma14.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma14.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma14.Init.MemInc = DMA_MINC_ENABLE;          // 内存地址递增
    hdma14.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  // 8位
    hdma14.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     // 8位
    hdma14.Init.Mode = DMA_NORMAL;                 // 普通模式
    hdma14.Init.Priority = DMA_PRIORITY_LOW;      // 低优先级

    /* 初始化DMA */
    HAL_DMA_Init(&hdma14);

    /* 配置中断（可选） */
    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

    hdma14.XferCpltCallback = DMA1_Channel4_XferCpltCallback;

    __HAL_LINKDMA(&huart1, hdmatx, hdma14);
}

void DMA_Init(DMA_Channel_TypeDef * DMA_Channel) {

    if (DMA_Channel == DMA1_Channel1) {
        DMA_Init11();
    } else if (DMA_Channel == DMA1_Channel4) {
        DMA_Init14();
    } else {
        return;
    }
}

void DMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma11);

}

void DMA1_Channel4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma14);

}

__weak void DMA1_Channel1_CallBack() {
    USART1_Printf("DMA1_Channel1_IRQHandler\n");
}

__weak void DMA1_Channel4_CallBack() {
    USART1_Printf("DMA1_Channel4_IRQHandler\n");
}
