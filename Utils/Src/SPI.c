#include "stm32f1xx_hal.h"                  // Device header

/*引脚配置层*/
/* 引脚定义 */
#define SPI_CS_PIN      GPIO_PIN_4
#define SPI_MISO_PIN    GPIO_PIN_5    // DO
#define SPI_SCK_PIN     GPIO_PIN_6    // SLK
#define SPI_MOSI_PIN    GPIO_PIN_7    // DI

#define SPI_PORT        GPIOA
#define HAL_RCC_GPIOx_CLK_ENABLE()          __HAL_RCC_GPIOA_CLK_ENABLE()

/*引脚配置层*/

/**
  * 函    数：SPI写SS引脚电平
  * 参    数：BitValue 协议层传入的当前需要写入SS的电平，范围0~1
  * 返 回 值：无
  */
void SPI_W_SS(uint8_t BitValue)
{
    HAL_GPIO_WritePin(SPI_PORT, SPI_CS_PIN, (GPIO_PinState)BitValue);
}

/**
  * 函    数：SPI写SCK引脚电平
  * 参    数：BitValue 协议层传入的当前需要写入SCK的电平，范围0~1
  * 返 回 值：无
  */
void SPI_W_SCK(uint8_t BitValue)
{
    HAL_GPIO_WritePin(SPI_PORT, SPI_SCK_PIN, (GPIO_PinState)BitValue);
}

/**
  * 函    数：SPI写MOSI引脚电平
  * 参    数：BitValue 协议层传入的当前需要写入MOSI的电平，范围0~1
  * 返 回 值：无
  */
void SPI_W_MOSI(uint8_t BitValue)
{
    HAL_GPIO_WritePin(SPI_PORT, SPI_MOSI_PIN, (GPIO_PinState)BitValue);
}

/**
  * 函    数：SPI读MISO引脚电平
  * 参    数：无
  * 返 回 值：协议层需要得到的当前MISO的电平，范围0~1
  */
uint8_t SPI_R_MISO(void)
{
    return (uint8_t)HAL_GPIO_ReadPin(SPI_PORT, SPI_MISO_PIN);
}

/**
  * 函    数：SPI初始化
  * 参    数：无
  * 返 回 值：无
  */
void SPI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 使能GPIOA时钟 */
    HAL_RCC_GPIOx_CLK_ENABLE();

    /* 配置CS、SCK、MOSI为输出模式 */
    GPIO_InitStruct.Pin = SPI_CS_PIN | SPI_SCK_PIN | SPI_MOSI_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SPI_PORT, &GPIO_InitStruct);

    /* 配置MISO为输入模式（上拉） */
    GPIO_InitStruct.Pin = SPI_MISO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(SPI_PORT, &GPIO_InitStruct);

    /* 设置默认电平 */
    SPI_W_SS(1);      // CS默认高电平
    SPI_W_SCK(0);     // SCK默认低电平
    SPI_W_MOSI(0);    // MOSI默认低电平
}

/*协议层*/

/**
  * 函    数：SPI起始
  * 参    数：无
  * 返 回 值：无
  */
void SPI_Start(void)
{
    SPI_W_SS(0);				//拉低SS，开始时序
}

/**
  * 函    数：SPI终止
  * 参    数：无
  * 返 回 值：无
  */
void SPI_Stop(void)
{
    SPI_W_SS(1);				//拉高SS，终止时序
}

/**
  * 函    数：SPI交换传输一个字节，使用SPI模式0
  * 参    数：ByteSend 要发送的一个字节
  * 返 回 值：接收的一个字节
  */
uint8_t SPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i, ByteReceive = 0x00;					//定义接收的数据，并赋初值0x00，此处必须赋初值0x00，后面会用到
	
	for (i = 0; i < 8; i ++)						//循环8次，依次交换每一位数据
	{
		/*两个!可以对数据进行两次逻辑取反，作用是把非0值统一转换为1，即：!!(0) = 0，!!(非0) = 1*/
        SPI_W_MOSI(!!(ByteSend & (0x80 >> i)));	//使用掩码的方式取出ByteSend的指定一位数据并写入到MOSI线
        SPI_W_SCK(1);								//拉高SCK，上升沿移出数据
		if (SPI_R_MISO()){ ByteReceive |= (0x80 >> i);}	//读取MISO数据，并存储到Byte变量
															//当MISO为1时，置变量指定位为1，当MISO为0时，不做处理，指定位为默认的初值0
        SPI_W_SCK(0);								//拉低SCK，下降沿移入数据
	}
	
	return ByteReceive;								//返回接收到的一个字节数据
}
