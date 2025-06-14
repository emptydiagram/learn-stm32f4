#ifndef TIM_H_
#define TIM_H_
void tim2_init(int freq_hz);
void tim2_1hz_init(void);
void tim2_pa5_output_compare(int freq_hz);
#define TIMx_SR_UIF        (1U << 0)
#endif
