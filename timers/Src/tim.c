#include "tim.h"
#include "stm32f4xx.h"

#define TIM2EN           (1U << 0)
// TIMER2 chosen somewhat randomly. TIMER2 uses APB1
#define TIMx_CR1_CEN     (1U << 0)

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
