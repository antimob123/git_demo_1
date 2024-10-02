#include "stm32f10x.h"                  // Device header

#define BOOTLOADER_ADDRESS 0x08000000    // ���� Bootloader ��ַ��ͨ��Ϊ Flash ����ʼ��ַ

void JumpToBootloader(void)
{
    typedef void (*pFunction)(void);  // ���庯��ָ������
    pFunction Jump_To_Bootloader;
    uint32_t BootloaderStackPointer = *(__IO uint32_t*)BOOTLOADER_ADDRESS;  // ��ȡ Bootloader ��ջ����ַ
    uint32_t BootloaderEntryPoint = *(__IO uint32_t*)(BOOTLOADER_ADDRESS + 4);  // ��ȡ Bootloader ����ڵ�ַ

    // �ر��ж�
    __disable_irq();

    // ��������ջָ��
    __set_MSP(BootloaderStackPointer);

    // ����������ƫ�ƼĴ���ָ�� Bootloader ��������
    SCB->VTOR = BOOTLOADER_ADDRESS;

    // ��ת�� Bootloader ����ڵ�
    Jump_To_Bootloader = (pFunction)BootloaderEntryPoint;   //�� BootloaderEntryPoint ��ַת��Ϊһ������ָ�룬����ֵ�� Jump_To_Bootloader ������
    Jump_To_Bootloader();
}


