#include <stdio.h>
#include "tim.h"
#include "uart.h"

#define GPIOAEN            (1U << 0)
#define PIN5               (1U << 5)
#define USER_LED_PIN       PIN5


int main(void)
{
    // user LED (PA5) setup
    RCC->AHB1ENR |= GPIOAEN;
    GPIOA->MODER |= (1U<<10);
    GPIOA->MODER &= ~(1U<<11);

    usart2_txrx_init();
    tim2_1hz_init();

    printf("Starting\n\r");

    while (1) {
        if (TIM2->SR & TIMx_SR_UIF) {
            TIM2->SR &= ~TIMx_SR_UIF;
            GPIOA->ODR ^= USER_LED_PIN;
            printf("One more second gone\n\r");
        }
    }
}
