#include <stdbool.h>
#include "stm32f1xx_hal.h"                  // Device header
#include "SPI.h"
#include "W25Q64_Ins.h"

// 地址最大宏
#define W25Q64_MAX_ADDRESS	0x7FFFFF

/**
  * 函    数：W25Q64读取ID号
  * 参    数：MID 工厂ID，使用输出参数的形式返回
  * 参    数：DID 设备ID，使用输出参数的形式返回
  * 返 回 值：无
  */
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	SPI_Start();								//SPI起始
	SPI_SwapByte(W25Q64_JEDEC_ID);			//交换发送读取ID的指令
	*MID = SPI_SwapByte(W25Q64_DUMMY_BYTE);	//交换接收MID，通过输出参数返回
	*DID = SPI_SwapByte(W25Q64_DUMMY_BYTE);	//交换接收DID高8位
	*DID <<= 8;									//高8位移到高位
	*DID |= SPI_SwapByte(W25Q64_DUMMY_BYTE);	//或上交换接收DID的低8位，通过输出参数返回
	SPI_Stop();								//SPI终止
}

/**
  * 函    数：W25Q64写使能
  * 参    数：无
  * 返 回 值：无
  */
void W25Q64_WriteEnable(void)
{
	SPI_Start();								//SPI起始
	SPI_SwapByte(W25Q64_WRITE_ENABLE);		//交换发送写使能的指令
	SPI_Stop();								//SPI终止
}

/**
  * 函    数：W25Q64等待忙
  * 参    数：无
  * 返 回 值：无
  */
void W25Q64_WaitBusy(void)
{
	uint32_t Timeout;
	SPI_Start();								//SPI起始
	SPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);				//交换发送读状态寄存器1的指令
	Timeout = 100000;							//给定超时计数时间
	while ((SPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)	//循环等待忙标志位
	{
		Timeout --;								//等待时，计数值自减
		if (Timeout == 0)						//自减到0后，等待超时
		{
			/*超时的错误处理代码，可以添加到此处*/
			break;								//跳出等待，不等了
		}
	}
	SPI_Stop();								//SPI终止
}

/**
  * 函    数：W25Q64页编程
  * 参    数：Address 页编程的起始地址，范围：0x000000~0x7FFFFF
  * 参    数：DataArray	用于写入数据的数组
  * 参    数：Count 要写入数据的数量，范围：0~256
  * 返 回 值：无
  * 注意事项：写入的地址范围不能跨页
  */
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;

	W25Q64_WriteEnable();						//写使能

	SPI_Start();								//SPI起始
	SPI_SwapByte(W25Q64_PAGE_PROGRAM);		//交换发送页编程的指令
	SPI_SwapByte(Address >> 16);				//交换发送地址23~16位
	SPI_SwapByte(Address >> 8);				//交换发送地址15~8位
	SPI_SwapByte(Address);					//交换发送地址7~0位
	for (i = 0; i < Count; i ++)				//循环Count次
	{
		SPI_SwapByte(DataArray[i]);			//依次在起始地址后写入数据
	}
	SPI_Stop();								//SPI终止

	W25Q64_WaitBusy();							//等待忙
}

/**
  * 函    数：W25Q64扇区擦除（4KB）
  * 参    数：Address 指定扇区的地址，范围：0x000000~0x7FFFFF
  * 返 回 值：无
  */
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();						//写使能

	SPI_Start();								//SPI起始
	SPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);	//交换发送扇区擦除的指令
	SPI_SwapByte(Address >> 16);				//交换发送地址23~16位
	SPI_SwapByte(Address >> 8);				//交换发送地址15~8位
	SPI_SwapByte(Address);					//交换发送地址7~0位
	SPI_Stop();								//SPI终止

	W25Q64_WaitBusy();							//等待忙
}

/**
  * 函    数：W25Q64读取数据
  * 参    数：Address 读取数据的起始地址，范围：0x000000~0x7FFFFF
  * 参    数：DataArray 用于接收读取数据的数组，通过输出参数返回
  * 参    数：Count 要读取数据的数量，范围：0~0x800000
  * 返 回 值：无
  */
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	SPI_Start();								//SPI起始
	SPI_SwapByte(W25Q64_READ_DATA);			//交换发送读取数据的指令
	SPI_SwapByte(Address >> 16);				//交换发送地址23~16位
	SPI_SwapByte(Address >> 8);				//交换发送地址15~8位
	SPI_SwapByte(Address);					//交换发送地址7~0位
	for (i = 0; i < Count; i ++)				//循环Count次
	{
		DataArray[i] = SPI_SwapByte(W25Q64_DUMMY_BYTE);	//依次在起始地址后读取数据
	}
	SPI_Stop();								//SPI终止
}


void W25Q64_PageErase(uint16_t Page) {
    W25Q64_SectorErase(Page << 2);
}


// 辅助函数判断是否超出范围
bool W25Q64_IsOutOfRange(uint16_t Page, uint8_t Offset, uint16_t Count) {
    return ((Page << 2) | Offset + Count) > W25Q64_MAX_ADDRESS;
}

bool W25Q64_ReadPage8(uint16_t Page, uint8_t Offset, uint8_t *DataArray, uint16_t Count) {
    if (W25Q64_IsOutOfRange(Page, Offset, Count)) return false;
    W25Q64_ReadData((Page << 2) | Offset, DataArray, Count);
    return true;
}


bool W25Q64_WritePage8(uint16_t Page, uint8_t Offset, uint8_t *DataArray, uint16_t Count) {
    if (W25Q64_IsOutOfRange(Page, Offset, Count)) return  false;
    W25Q64_PageProgram((Page << 2) | Offset, DataArray, Count);
    return true;
}


bool W25Q64_WritePageErase8(uint16_t Page, uint8_t Offset, const uint8_t *DataArray, uint16_t Count) {
    if (W25Q64_IsOutOfRange(Page, Offset, Count)) return  false;
    // 先读取一页数据
    uint8_t PageData[256];
    W25Q64_ReadPage8(Page, 0, PageData, 256);

    // 更新数据
    for (int i = 0; i < Count; i++) {
        PageData[Offset + i] = DataArray[i];
    }

    // 擦除一页
    W25Q64_PageErase(Page);

    // 写一页数据
    W25Q64_WritePage8(Page, 0, PageData, 256);

    return true;
}

