#include <stdbool.h>
#include <stdio.h>
#include "uart.h"

#define GPIOCEN            (1U << 2)
#define PIN13              (1UL << 13)
#define USER_BTN_PIN       PIN13

static inline bool user_btn_pressed(void) {
    return (GPIOC->IDR & USER_BTN_PIN) == 0;
}

int main(void)
{
    RCC->AHB1ENR |= GPIOCEN;

    GPIOC->MODER &= ~(1UL<<26);
    GPIOC->MODER &= ~(1UL<<27);

    usart2_tx_init();

    char *hello_strs[] = {"HELLO", " ", "W", "O", "R", "L", "D", ".", ".", "."};
    char *goodbye_strs[] = {"GOOD", " ", "BYE", " ", "S", "U", "N", ".", ".", "."};

    char **strs[] = { hello_strs, goodbye_strs };
    int strs0_len = sizeof(hello_strs) / sizeof(hello_strs[0]);
    int strs1_len = sizeof(goodbye_strs) / sizeof(goodbye_strs[0]);
    int strs_lens[] = { strs0_len, strs1_len };
    int strs_idxes[] = { 0, 0 };
    int base_count = 400000;


    int mod_count;

    while(1) {
        int curr_bp = user_btn_pressed();
        mod_count = curr_bp == 1 ? base_count : 2 * base_count;

        printf(strs[curr_bp][strs_idxes[curr_bp]]);
        fflush(stdout);

        for (int i = 0; i < mod_count; i++);

        strs_idxes[curr_bp] = (strs_idxes[curr_bp] + 1) % strs_lens[curr_bp];
    }
}
