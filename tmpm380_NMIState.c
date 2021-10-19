#include "tmpm380_NMIState.h"

/**
 *名称：NMI割り込み
 *作成者：浅尾　享一
 *説明：NMIポートが立下りエッジを感知すると発生する
**/
void INT3_IRQHandler(void)
{
    WDT_Disable();/* 内蔵WDT禁止 */
    
    ERRFG = 0xA1;/* エラーチェック用のフラグを建てる */
    
    NVIC_DisableIRQ(INT3_IRQn);/* INT3外部割り込み禁止 */
    NVIC_DisableIRQ(INTADSFT_IRQn);/* AD変換割り込み禁止 */
    NVIC_DisableIRQ(INTTX0_IRQn);/* UART 送信割り込み禁止 */
    NVIC_DisableIRQ(INTRX0_IRQn);/* UART 受信割り込み禁止 */
    
    return;/* とりあえず戻る */
    
}

/**
 *名称：終了プログラム
 *作成者：浅尾　享一
 *説明：保存処理して、機能をオフにし、電源が切れるまでリセット待ちを行う
**/
void CPUERR(void)
{
    while (1) {
        for (uint8_t i = 0;i < 250;i++) {
            __NOP();/*WDTでリセットかかるまで何もしないでループ*/
        }
        WDTCLR();/* 外部番犬時計のクリア */
    }
    
}


void NMI_Handler(void)
{
    WDT_Disable();/* 内蔵WDT禁止 */
    
    ERRFG = 0xA1;/* エラーチェック用のフラグを建てる */
    
}

void HardFault_Handler(void)
{
    WDT_Disable();/* 内蔵WDT禁止 */
    
    ERRFG = 0xA1;/* エラーチェック用のフラグを建てる */
    
}


void MemManage_Handler(void)
{
    WDT_Disable();/* 内蔵WDT禁止 */
    
    ERRFG = 0xA1;/* エラーチェック用のフラグを建てる */
    
}