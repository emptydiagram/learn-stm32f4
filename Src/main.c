#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
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
#define LED_ON()           (GPIOA->BSRR = USER_LED_PIN)
#define LED_OFF()          (GPIOA->BSRR = USER_LED_PIN << 16)
#define LED_TOGGLE()       (GPIOA->ODR ^= USER_LED_PIN)

#define BLINK_COUNT    (240000)

#define ACTIONS_PER_PERIOD  (16)

#define    __IO            volatile

static inline bool user_btn_pressed(void) {
    return (GPIOC->IDR & USER_BTN_PIN) == 0;
}

void toggle_odr_spin(int spin_count) {
    LED_TOGGLE();
    for (int i = 0; i < spin_count; i++);
}

void blink_odr_spin(int spin_count_1, int spin_count_2) {
    toggle_odr_spin(spin_count_1);
    toggle_odr_spin(spin_count_2);
}

void blink_bsrr_spin(int spin_count_1, int spin_count_2) {
    LED_ON();
    for (int i = 0; i < spin_count_1; i++);
    LED_OFF();
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
    int count = 0;
    while (1) {

        if (user_btn_pressed()) {
            LED_OFF();
            for (int i = 0; i < BLINK_COUNT; i++);
        } else {
            // TODO: Fibonacci blink
            if (count < ACTIONS_PER_PERIOD) {
                LED_TOGGLE();
                for (int i = 0; i < 2 * BLINK_COUNT; i++);
            } else if (count < 2 * ACTIONS_PER_PERIOD) {
                LED_TOGGLE();
                for (int i = 0; i < BLINK_COUNT; i++);
                LED_TOGGLE();
                for (int i = 0; i < BLINK_COUNT; i++);
            }
            count += 1;
            if (count >= 2 * ACTIONS_PER_PERIOD) {
                count = 0;
            }
        }
    }
}
