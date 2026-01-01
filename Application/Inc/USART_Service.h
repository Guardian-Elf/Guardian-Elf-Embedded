//
// Created by pc on 2025/12/6.
//

#ifndef GUARDIANELF_USART_SERVICE_H
#define GUARDIANELF_USART_SERVICE_H
#include "USART.h"
#include "GP10.h"
#include "Enum.h"

void USART_Service_Init();

// 向esp发送
void USART_Service_Set2ESP(enum SerialDataOperationCode code, char *data);

#endif //GUARDIANELF_USART_SERVICE_H
