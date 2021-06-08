#include "stm32f0xx.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "app_bsp.h"
#include "stm32f070xb.h"
#include "stm32f0xx_hal_conf.h"
#include "string.h"
#include "stm32f0xx_hal.h"
#include "funciones.h"

/**------------------------------------------------------------------------------------------------
Brief.- Punto de entrada del programa
-------------------------------------------------------------------------------------------------*/

#define strlen_p(x) strlen((const char*)x)


const uint8_t* msgError     =(uint8_t*)"ERROR\n";
const uint8_t* msgOk        =(uint8_t*)"OK\n";
const uint8_t* Comando      =(uint8_t*)"XMAS=";

const uint8_t* secuencias[7] = {(uint8_t*)"blinky,",
                                (uint8_t*)"altern,",
                                (uint8_t*)"rolling,",
                                (uint8_t*)"kit,",
                                (uint8_t*)"car,",
                                (uint8_t*)"none,"
};

void (*prtFunctions[7])(void) = {blinky,altern,rolling,kit,car,none};

UART_HandleTypeDef UartHandle;
__IO ITStatus uartState = RESET;
__IO ITStatus status = RESET;
__IO HAL_StatusTypeDef flag_String = HAL_BUSY;


uint8_t RxByte;
uint8_t RxBuffer[20];
uint32_t tickTimer; 
uint16_t timer = 100;
uint8_t function_list = 5;

void UART_Init(void);

int main( void )
{
    HAL_Init( );
    UART_Init();
    tickTimer = HAL_GetTick();
    for (; ;)
    {

        if (status == SET )
        {
            status = RESET;
            
            if (!memcmp(RxBuffer,Comando,strlen_p(Comando)))
            {
                memcpy(RxBuffer,&RxBuffer[strlen_p(Comando)],(sizeof(RxBuffer) - sizeof(Comando)));

                for (uint8_t i = 0; i < 7; i++)
                {
                    if (!memcmp(RxBuffer,secuencias[i],strlen_p(secuencias[i])))
                    {
                        flag_String = HAL_OK;
                        memcpy(RxBuffer,&RxBuffer[strlen_p(secuencias[i])],(sizeof(RxBuffer)-sizeof(secuencias[i])));
                        function_list = i;
                        break;
                    }  
                    else
                    {
                        flag_String = HAL_ERROR;
                    }
                    
                }
                if (flag_String == HAL_OK)
                {
                    HAL_UART_Transmit_IT(&UartHandle,(uint8_t*)RxBuffer,strlen_p(RxBuffer));
                }
                else
                {
                    HAL_UART_Transmit_IT(&UartHandle,(uint8_t*)msgError,strlen_p(msgError));
                }
                
                
            }
            else
            {
                HAL_UART_Transmit_IT(&UartHandle,(uint8_t*)msgError,strlen_p(msgError));
            }
            
            
            
        }

        if (HAL_GetTick() - tickTimer > timer)
        {
            tickTimer = HAL_GetTick();
            HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
            switch (function_list)
            {
                case 0:
                    blinky();
                    break;
                case 1:
                    altern();
                    break;
                case 2:
                    rolling();
                    break;
                case 3:
                    kit();
                    break;
                case 4:
                    car();
                    break;
                case 5:
                    none();
                    break;
            }
        }
        
        
    } 
    return 0u;
}


void UART_Init()
{
    UartHandle.Instance             = USART2;
    UartHandle.Init.BaudRate        = 115200;
    UartHandle.Init.WordLength      = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits        = UART_STOPBITS_1;
    UartHandle.Init.Parity          = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl       = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode            = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling    = UART_OVERSAMPLING_16;

    HAL_UART_Init(&UartHandle);
    HAL_UART_Receive_IT(&UartHandle,&RxByte,1);
}




void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    uartState = SET;
    memset(RxBuffer,0,sizeof(RxBuffer));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    static uint32_t i = 0;
    RxBuffer[i] = RxByte;
    i++;
    if(RxBuffer[i-1] == '\r')
    {
        status = SET;
        i=0;
    }
    HAL_UART_Receive_IT(&UartHandle,&RxByte,1);
}