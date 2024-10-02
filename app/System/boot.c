#include "stm32f10x.h"                  // Device header

#define BOOTLOADER_ADDRESS 0x08000000    // // 定义 Bootloader 地址，通常为 Flash 的起始地址

void JumpToBootloader(void)
{
    typedef void (*pFunction)(void);  // 定义函数指针类型
    pFunction Jump_To_Bootloader;
    uint32_t BootloaderStackPointer = *(__IO uint32_t*)BOOTLOADER_ADDRESS;  // 获取 Bootloader 的栈顶地址
    uint32_t BootloaderEntryPoint = *(__IO uint32_t*)(BOOTLOADER_ADDRESS + 4);  // 获取 Bootloader 的入口地址

    // 关闭中断
    __disable_irq();

    // 重置主堆栈指针
    __set_MSP(BootloaderStackPointer);

    // 设置向量表偏移寄存器指向 Bootloader 的向量表
    SCB->VTOR = BOOTLOADER_ADDRESS;

    // 跳转到 Bootloader 的入口点
    Jump_To_Bootloader = (pFunction)BootloaderEntryPoint;    //将 BootloaderEntryPoint 地址转换为一个函数指针，并赋值给 Jump_To_Bootloader 变量。
    Jump_To_Bootloader();
}


