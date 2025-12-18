//
// Created by pc on 2025/12/5.
//

#include "app.h"
#include "Inc/OLED_Service.h"
#include "Inc/USART_Service.h"
#include "BH1750.h"

void App(void) {
    USART_Service_Init();
    OLED_Show_BaseInfo();

}

