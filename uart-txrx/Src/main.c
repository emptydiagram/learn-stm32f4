#include <stdio.h>
#include "uart.h"

#define GPIOAEN            (1UL << 0)

// the User LED I/O pin is GPIO PA5
#define PIN5               (1UL << 5)
#define USER_LED_PIN       PIN5

char key;

int main(void)
{

	usart2_txrx_init();

	while(1) {
		RCC->AHB1ENR |= GPIOAEN;

		GPIOA->MODER |= (1U << 10);
		GPIOA->MODER &= ~(1U << 11);

		key = usart2_read();

		printf("Received '%c'\r\n", key);

		if (key == '1' || key == '3' || key == '5' || key == '7' || key == '9') {
			GPIOA->ODR |= USER_LED_PIN;
		} else {
			GPIOA->ODR &= ~USER_LED_PIN;
		}
	}
}
