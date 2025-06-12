#include <stdint.h>
#include "stm32f4xx.h"

#define GPIOAEN            (1U << 0)
#define USART2EN           (1U << 17)
//#define USART_CR1_TE       (1U << 3)
//#define USART_CR1_UE       (1U << 13)
//#define USART_SR_TXE       (1U << 7)

// 16 MHz internal RC oscillator ??
#define DEFAULT_SYS_FREQ   (16000000)
#define APB1_CLK           DEFAULT_SYS_FREQ

#define USART_BAUDRATE     115200

void usart2_tx_init();
static void usart_set_baudrate(USART_TypeDef *usartx, uint32_t periph_clk, uint32_t baud_rate);
static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baud_rate);
void usart2_write(int ch);

int main(void)
{

	usart2_tx_init();

    int chars1[] = {'H','E','L','L','O',' ','W','O','R','L','D', ' '};
    int chars_len = sizeof(chars) / sizeof(chars[0]);
    int char_idx = 0;
	while(1) {
		int count_multiples = (char_idx / 6 < 1) ? 1 : 5;
		usart2_write(chars[char_idx]);
		for (int i = 0; i < (count_multiples * 100000); i++);
		char_idx = (char_idx + 1) % chars_len;
	}
}

void usart2_tx_init() {
	/*
	 * # Config UART GPIO pin
	 *  - enable clock access to GPIO A
	 *  - set PA2 mode to alternate function mode
	 *  - set PA2 alt. fn. type to UART Tx, AF7
	 */
	RCC->AHB1ENR |= GPIOAEN;

	// set MODER2 on GPIOA (PA2) to (b4 = 0, b5 = 1)
	GPIOA->MODER &= ~(1U << 4);
	GPIOA->MODER |= (1U << 5);

	// set bits 8-11 to 0b0111
	GPIOA->AFR[0] |= (7U << 8);
	GPIOA->AFR[0] &= ~(1U << 11);


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

void usart2_write(int ch) {
    // ensure transmit data register is empty
	// spins until TXE bit is high.
	// "This bit is set by hardware when the content of the TDR register has been transferred into the shift register."
	while (!(USART2->SR & USART_SR_TXE));

	// write to Transmit DR
	USART2->DR = ch & 0xFF;
}
