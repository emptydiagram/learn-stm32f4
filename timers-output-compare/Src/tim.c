#include "tim.h"
#include "stm32f4xx.h"

#define GPIOAEN            (1U << 0)
#define TIM2EN           (1U << 0)
// TIMER2 chosen somewhat randomly. TIMER2 uses APB1
#define TIMx_CR1_CEN     (1U << 0)
#define TIMx_CCER_CC1E   (1U << 0)

#define SYS_FREQ          (16000000)
#define TIM2_CNT_CLK_FREQ (10000)
#define TIM2_CNT_CLK_DIV  (SYS_FREQ / TIM2_CNT_CLK_FREQ)

void tim2_init(int freq_hz) {
    RCC->APB1ENR |= TIM2EN;

    // counter clock frequency is divided by (PSC + 1)
    // default CNT_CLK_DIV is 16000 / 10 = 1600, so clock frequency goes 16 MHz => 10^4 = 10 KHZ
    TIM2->PSC = TIM2_CNT_CLK_DIV - 1;
    TIM2->ARR = (TIM2_CNT_CLK_FREQ / freq_hz) - 1;
    TIM2->CNT = 0;
    TIM2->CR1 = TIMx_CR1_CEN;
}

void tim2_pa5_output_compare(int freq_hz) {
    RCC->AHB1ENR |= GPIOAEN;

    // set PA5 mode register to alternate function
    GPIOA->MODER &= ~(1U<<10);
    GPIOA->MODER |= (1U<<11);

    // set alt fn type to AF1
    // AFRL5[3:0] = GPIOA_AFRL[23:20]
    // AF1 = 0001
    GPIOA->AFR[0] |= (1U << 20);
    GPIOA->AFR[0] &= ~(1U << 21);
    GPIOA->AFR[0] &= ~(1U << 22);
    GPIOA->AFR[0] &= ~(1U << 23);



    /* timer */

    RCC->APB1ENR |= TIM2EN;

    // counter clock frequency is divided by (PSC + 1)
    // default CNT_CLK_DIV is 16000 / 10 = 1600, so clock frequency goes 16 MHz => 10^4 = 10 KHZ
    TIM2->PSC = TIM2_CNT_CLK_DIV - 1;
    TIM2->ARR = (TIM2_CNT_CLK_FREQ / freq_hz) - 1;

    // Set output compare toggle mode
    TIM2->CCMR1 |= (1U << 4) | (1U << 5);
    TIM2->CCMR1 &= ~(1U << 6);
    // enable tim2 ch1 in compare mode
    TIM2->CCER |= TIMx_CCER_CC1E;

    // clear count, enable timer
    TIM2->CNT = 0;
    TIM2->CR1 = TIMx_CR1_CEN;
}

void tim2_1hz_init(void) {
    // enable clock access for TIM2
    // RCC_APB1ENR: APB1 peripheral clock enable register
    // bit 0 is TIM2EN, enabled = 0b1
    RCC->APB1ENR |= TIM2EN;

    // set prescaler value
    // "The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1)."
    // so resulting CK_CNT is (16 x 10^6 (cycles / s)) / (1600) = 10^4 = 10,000
    // notice also, if clock at 16 MHz, then there are 1600 cycles per 10^{-4} ms
    // 16 x 10^6 (cycles / s) / (1600 (cycles / 0.1ms))
    // => (16 x 10^6) (cycles / s) * 1/1600 (0.1ms / cycles)
    //  = 10^4 (0.1ms / s)

    // when base clock is 16 MHz, this prescales clock to 10 KHz and makes timer count up to 10k, meaning
    // it fires every second?
    TIM2->PSC = 1600 - 1;
    TIM2->ARR = 10000 - 1;
    TIM2->CNT = 0;
    TIM2->CR1 = TIMx_CR1_CEN;
}
