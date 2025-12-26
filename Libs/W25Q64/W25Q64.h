#ifndef __W25Q64_H
#define __W25Q64_H

#include "stm32f1xx_hal.h"
#include "stdbool.h"

/**
  * 函    数：W25Q64读取ID号
  * 参    数：MID 工厂ID，使用输出参数的形式返回
  * 参    数：DID 设备ID，使用输出参数的形式返回
  * 返 回 值：无
  */
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID);

void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count);
/**
  * 函    数：W25Q64页擦除
  * 参    数： Page 页号，范围：0~ 32,768
  * 返 回 值：无
  */
void W25Q64_PageErase(uint16_t Page);

/**
  * 函    数：W25Q64页读取(uint8_t)
  * 参    数： Page 页号，范围：0~ 32,768， Offset 偏移量，范围：0~ 255
  * 返 回 值：无
  */
bool W25Q64_ReadPage8(uint16_t Page, uint8_t Offset, uint8_t *DataArray, uint16_t Count);

/**
  * 函    数：W25Q64页写入(uint8_t)
  * 参    数： Page 页号，范围：0~ 32,768， Offset 偏移量，范围：0~ 255
  * 返 回 值：无
  */
bool W25Q64_WritePage8(uint16_t Page, uint8_t Offset, uint8_t *DataArray, uint16_t Count);

/**
  * 函    数：W25Q64页擦除写入(uint8_t)
  * 参    数： Page 页号，范围：0~ 32,768， Offset 偏移量，范围：0~ 255
  * 返 回 值：无
  */
bool W25Q64_WritePageErase8(uint16_t Page, uint8_t Offset, uint8_t *DataArray, uint16_t Count);

#endif
