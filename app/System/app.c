#include "stm32f10x.h"
#include "OLED.h"
#include "Serial.h"
extern volatile int i;

#define TASK_NUM_MAX 2 

typedef struct
{
    uint8_t run;                  // 调度标志, 1: 调度, 0: 挂起
    uint16_t TimCount;            // 时间片周期, 用于递减计数
    uint16_t TimRload;            // 时间片周期, 用于重载
    void (*TaskFuncCb)(void);     // 函数指针, 保存任务函数地址
} TaskComps_t;



void HmiTask(void)
{
    OLED_ShowHexNum(3, 1, i, 4);
}



void SensorTask(void)
{
    i= i+1 ;

}


static TaskComps_t TaskComps[] =
{
    {0, 1, 100, HmiTask},                 //   {1:调度 0:挂起,  初始化递减值,如果为0 则程序不执行,    重装载值 }
	{0, 1, 1000, SensorTask},
};


void TaskSchedule(void)
{
    for (uint8_t i = 0; i < TASK_NUM_MAX; i++)
    {
        if (TaskComps[i].TimCount) // 判断时间片计数
        {
            TaskComps[i].TimCount--; // 时间片计数递减
            if (TaskComps[i].TimCount == 0)
            {                        //时间片标记为1，并重载计数初值               
                TaskComps[i].TimCount = TaskComps[i].TimRload;
                TaskComps[i].run = 1;
            }
        }
    }
}


void Task_run(void)
{
       for (uint8_t i = 0; i < TASK_NUM_MAX; i++)
        {
            if (TaskComps[i].run)               // 如果任务的调度标志为1
            {
                TaskComps[i].run = 0;           // 清除调度标志
                TaskComps[i].TaskFuncCb();      // 执行任务函数
            }
        }
}


void SysTick_Init(void)
{	
	SysTick->LOAD = (uint32_t)(72000 - 1);				//设置定时器重装值    0xE000E014
	SysTick->VAL = 0x00;					            //清空当前计数值      0xE000E018
	SysTick->CTRL = 0x00000007;				            //设置时钟源为HCLK，启动定时器  0xE000E010
}	
/*Bit 2 (CLKSOURCE) = 1，选择HCLK作为SysTick时钟源。
  Bit 1 (TICKINT) = 1，打开SysTick中断。
  Bit 0 (ENABLE) = 1，启用SysTick定时器。*/	


void SysTick_Handler(void)
{    
	TaskSchedule();	
}


