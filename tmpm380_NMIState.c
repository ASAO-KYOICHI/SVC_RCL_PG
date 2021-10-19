#include "tmpm380_NMIState.h"

/**
 *���́FNMI���荞��
 *�쐬�ҁF����@����
 *�����FNMI�|�[�g��������G�b�W�����m����Ɣ�������
**/
void INT3_IRQHandler(void)
{
    WDT_Disable();/* ����WDT�֎~ */
    
    ERRFG = 0xA1;/* �G���[�`�F�b�N�p�̃t���O�����Ă� */
    
    NVIC_DisableIRQ(INT3_IRQn);/* INT3�O�����荞�݋֎~ */
    NVIC_DisableIRQ(INTADSFT_IRQn);/* AD�ϊ����荞�݋֎~ */
    NVIC_DisableIRQ(INTTX0_IRQn);/* UART ���M���荞�݋֎~ */
    NVIC_DisableIRQ(INTRX0_IRQn);/* UART ��M���荞�݋֎~ */
    
    return;/* �Ƃ肠�����߂� */
    
}

/**
 *���́F�I���v���O����
 *�쐬�ҁF����@����
 *�����F�ۑ��������āA�@�\���I�t�ɂ��A�d�����؂��܂Ń��Z�b�g�҂����s��
**/
void CPUERR(void)
{
    while (1) {
        for (uint8_t i = 0;i < 250;i++) {
            __NOP();/*WDT�Ń��Z�b�g������܂ŉ������Ȃ��Ń��[�v*/
        }
        WDTCLR();/* �O���Ԍ����v�̃N���A */
    }
    
}


void NMI_Handler(void)
{
    WDT_Disable();/* ����WDT�֎~ */
    
    ERRFG = 0xA1;/* �G���[�`�F�b�N�p�̃t���O�����Ă� */
    
}

void HardFault_Handler(void)
{
    WDT_Disable();/* ����WDT�֎~ */
    
    ERRFG = 0xA1;/* �G���[�`�F�b�N�p�̃t���O�����Ă� */
    
}


void MemManage_Handler(void)
{
    WDT_Disable();/* ����WDT�֎~ */
    
    ERRFG = 0xA1;/* �G���[�`�F�b�N�p�̃t���O�����Ă� */
    
}