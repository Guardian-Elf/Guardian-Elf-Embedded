//
// Created by pc on 2025/12/26.
//

#include "W25Q64_Service.h"

// id存储地址宏(页与偏移量)
#define W25Q64_DEVICE_ID_PAGE 0
#define W25Q64_DEVICE_ID_OFFSET 0

// 存储id值
uint8_t W25Q64_Device_Id[24] = {0};

// 写入设备id
void W25Q64_Write_Device_ID(uint8_t id[24]) {
    W25Q64_WritePageErase8(W25Q64_DEVICE_ID_PAGE, W25Q64_DEVICE_ID_OFFSET, id, 24);
}

// 读取设备id
void W25Q64_Read_Device_ID() {
    W25Q64_ReadPage8(W25Q64_DEVICE_ID_PAGE, W25Q64_DEVICE_ID_OFFSET, W25Q64_Device_Id, 24);
}
