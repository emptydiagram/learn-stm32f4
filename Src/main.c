#include <stdint.h>
#include <stddef.h>
#include "stm32f4xx.h"

// User LED: PA5, (port, pin) = (A, 5)
// User Push Button: B1, connected to I/O pin PC13: (port, pin) = (C, 13)

//// Masks
// mask for bit 0 of RCC_AHB1ENR register
#define GPIOAEN            (1UL << 0)
#define GPIOCEN            (1UL << 2)

// the User LED I/O pin is GPIO PA5
#define PIN5               (1UL << 5)
#define PIN13              (1UL << 13)
#define USER_LED_PIN       PIN5
#define USER_BTN_PIN       PIN13

#define COUNT_SIZE         (80000)
#define FIB_BLINK_COUNT    (230000)
#define FIB_PAUSE_COUNT    (5*FIB_BLINK_COUNT)

#define    __IO            volatile


void toggle_odr_spin(int spin_count) {
    GPIOA->ODR ^= USER_LED_PIN;
    for (int i = 0; i < spin_count; i++);
}

void blink_odr_spin(int spin_count_1, int spin_count_2) {
    toggle_odr_spin(spin_count_1);
    toggle_odr_spin(spin_count_2);
}

void blink_bsrr_spin(int spin_count_1, int spin_count_2) {
    GPIOA->BSRR = USER_LED_PIN;
    for (int i = 0; i < spin_count_1; i++);
    GPIOA->BSRR = (1UL << (16 + 5));
    for (int i = 0; i < spin_count_2; i++);
}

int main(void) {
    // enable clock access to GPIOA and GPIOC
    RCC->AHB1ENR |= GPIOAEN;
    RCC->AHB1ENR |= GPIOCEN;

    // set PA5 as output pin
    GPIOA->MODER |= (1UL<<10);
    GPIOA->MODER &= ~(1UL<<11);

    // set PC13 as input pin
    GPIOC->MODER &= ~(1UL<<26);
    GPIOC->MODER &= ~(1UL<<27);

    // process input, produce output, etc.
    GPIOA->ODR &= ~(USER_LED_PIN);
    while (1) {
        if (GPIOC->IDR & USER_BTN_PIN) {
            GPIOA->ODR ^= USER_LED_PIN;
            for (int i = 0; i < FIB_BLINK_COUNT; i++);
        } else {
            GPIOA->BSRR = (1UL << (16 + 5));
            for (int i = 0; i < FIB_BLINK_COUNT; i++);
        }
    }
}
