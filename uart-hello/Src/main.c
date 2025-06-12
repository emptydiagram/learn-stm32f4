#include "stm32f4xx.h"

#define GPIOAEN            (1UL << 0)
#define USART2EN           (1UL << 17)
#define USART_CR1_TE       (1UL << 3)
#define USART_CR1_UE       (1UL << 13)

// 16 MHz internal RC oscillator ??
#define DEFAULT_SYS_FREQ   (16000000)
#define APB1_CLK           DEFAULT_SYS_FREQ

#define USART_BAUDRATE     115200

static void usart_set_baudrate(USART_TypeDef *usartx, uint32_t periph_clk, uint32_t baud_rate);
static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baud_rate);

int main(void)
{
    /* Loop forever */
	while(1) {
		RCC->APB1ENR |= USART2EN;
	}
}

void uart2_tx_init() {
	/*
	 * # Config UART GPIO pin
	 *  - enable clock access to GPIO A
	 *  - set PA2 mode to alternate function mode
	 *  - set PA2 alt. fn. type to UART Tx, AF7
	 */
	RCC->AHB1ENR |= GPIOAEN;

	// set MODER2 on GPIOA (PA2) to (b4 = 0, b5 = 1)
	GPIOA->MODER &= ~(1UL << 4);
	GPIOA->MODER |= (1UL << 5);

	// set bits 8-11 to 0b0111
	GPIOA->AFR[0] |= (7U << 8);
	GPIOA->AFR[0] &= ~(1UL << 11);



	/*
	 * # Configure UART Module
	 *  - enable USART2 on APB1
	 *  - config baud rate
	 *  - config transfer direction
	 *  - enable UART module
	 */

	RCC->APB1ENR |= USART2EN;

	usart_set_baudrate(USART2, APB1_CLK, USART_BAUDRATE);

	// not logical OR, "want to clean" the CR1
	USART2->CR1 = USART_CR1_TE;

	USART2->CR1 |= USART_CR1_UE;

}

static void usart_set_baudrate(USART_TypeDef *usartx, uint32_t periph_clk, uint32_t baud_rate) {
	usartx->BRR = compute_uart_bd(periph_clk, baud_rate);
}


static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baud_rate) {
	// "empirically derived" formula from Video 15 ~16:40, of "Embedded Systems Bare-Metal Programming Ground Up™ (STM32)"
	return (periph_clk + (baud_rate / 2U)) / baud_rate;
}
