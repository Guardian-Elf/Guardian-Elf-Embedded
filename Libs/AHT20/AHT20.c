#include "AHT20.h"
#include "Delay.h"


// AHT20地址
#define AHT20_ADDRESS    0x38  // 7位地址

float AHT20_Temperature, AHT20_Humidity;


void AHT20_Init() {
    uint8_t readBuffer;

    // 延时 40ms
    HAL_Delay(40);

    I2C_RecvBytes(AHT20_ADDRESS, &readBuffer, 1);

    if ((readBuffer & 0x08) != 0x00) {
        return;
    }

    // 发送三个字节数据
    uint8_t sendBuffer[3] = {0xBE, 0x08, 0x00};

    I2C_SendBytes(AHT20_ADDRESS, sendBuffer, 3);
}



void AHT20_ReadData() {
    uint8_t sendBuffer[3] = {0xAC, 0x33, 0x00};
    uint8_t readBuffer[6];

    I2C_SendBytes(AHT20_ADDRESS, sendBuffer, 3);
    HAL_Delay(50);

    I2C_RecvBytes(AHT20_ADDRESS, readBuffer, 6);

    if ((readBuffer[0] & 0x80) != 0x00) {
        return;
    }

    uint32_t data;
    data = (readBuffer[3] >> 4) | (readBuffer[2] << 4) | readBuffer[1] << 12;
    AHT20_Humidity = (float) data * 100.0f / (1 << 20);

    data = ((readBuffer[3] & 0x0F) << 16) | (readBuffer[4] << 8) | (readBuffer[5]);
    AHT20_Temperature= (float) data * 200.0f / (1 << 20) - 50.0f;


}

