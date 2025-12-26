//
// Created by pc on 2025/12/26.
//

#ifndef GUARDIANELF_W25Q64_SERVICE_H
#define GUARDIANELF_W25Q64_SERVICE_H
#include "W25Q64.h"



extern uint8_t W25Q64_Device_Id[24];


// 写入设备id
void W25Q64_Write_Device_ID(uint8_t id[24]);

// 读取设备id
void W25Q64_Read_Device_ID();

#endif //GUARDIANELF_W25Q64_SERVICE_H
