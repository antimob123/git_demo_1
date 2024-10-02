#ifndef __APP_H
#define __APP_H

void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void Delay_s(uint32_t s);
void SysTick_Init(void);
void TaskSchedule(void);
void Task_run(void);
#endif
