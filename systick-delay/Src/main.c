#include <stdint.h>
#include <stdio.h>
#include "systick.h"
#include "uart.h"

#define GPIOAEN            (1UL << 0)
#define PIN5               (1UL << 5)
#define USER_LED_PIN       PIN5


int main(void)
{
	// user LED (PA5) setup
    RCC->AHB1ENR |= GPIOAEN;
    GPIOA->MODER |= (1U<<10);
    GPIOA->MODER &= ~(1U<<11);

	usart2_txrx_init();

	printf("Starting\n\r");

	while(1) {
		systick_delay_ms(1000);
		GPIOA->ODR ^= USER_LED_PIN;
		printf("One more second gone\n\r");
	}
}
