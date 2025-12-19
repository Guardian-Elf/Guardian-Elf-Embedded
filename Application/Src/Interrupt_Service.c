//
// Created by pc on 2025/12/19.
//

#include "Interrupt_Service.h"

void Interrupt_Service_Init(void) {
    Interrupt_Init(GPIOB, GPIO_PIN_0, 1, 3);
    Interrupt_Init(GPIOB, GPIO_PIN_1, 1, 3);
}

void Interrupt_0(void) {
    OLED_NextPage();
}
void Interrupt_1(void) {
    OLED_PrevPage();
}