/**
 *******************************************************************************
 * ﾌｧｲﾙ名：main.c
 * 作成者：浅尾　享一
 * 用途　：Operating System
 *******************************************************************************
 */
#include "main.h"

SBI_InitI2CTypeDef myI2C;
UART_InitTypeDef   myUART;

int main(void)
{
    /*****内蔵ウォッチドッグの設定*********************************************/
    WDT_SetDetectTime(WDT_DETECT_TIME_EXP_19);/* 約52msで発動する */
    WDT_SetOverflowOutput(WDT_WDOUT);/* ﾀｲﾏｵｰﾊﾞｰﾌﾛｰ時はﾏｲｺﾝﾘｾｯﾄ */
    WDT_Enable();/* ｳｫｯﾁﾄﾞｯｸﾞの許可 */
    /*****内蔵ウォッチドッグの設定ここまで*************************************/
    
    /* ポートの初期化ここから開始-------------------------------------------- */
    /* 入力ポートに設定****************************************************** */
    TSB_PA->IE  = 0x01;/* PA0を入力ﾎﾟｰﾄに設定 */
    TSB_PD->IE  = 0x4F;/* PD0~PD3,PD6を入力ﾎﾟｰﾄに設定する */
    TSB_PE->IE  = 0xC2;/* PE1を入力ﾎﾟｰﾄに設定する */
    
    /* 出力ポートに設定****************************************************** */
    TSB_PA->DATA |= 0xC0;/* LCD/VFDの制御線（E除く）をHIGHにする */
    TSB_PA->DATA &= 0xC7;/* LED出力をマスクする */
    TSB_PA->CR |= 0xFE;/* PA1~PA7を出力ﾎﾟｰﾄにする */
    
    TSB_PC->CR |= 0xFC;/* PC2~PC7を出力ﾎﾟｰﾄにする */
    TSB_PC->DATA &= 0x03;/* 出力クリア */
    
    TSB_PE->CR |= 0x1C;/* PE2~PE4を出力ﾎﾟｰﾄにする */
    TSB_PE_DATA_PE4 = 0;/* NV-RAMチップセレクト出力初期値(Disable)セット */
    
    TSB_PD->CR |= 0x30;/* PD4,5を出力ポートにする */
    TSB_PD->DATA &= 0xCF;/* PD4,5(NV-RAMへのDOUTラインとSKをLOWにする) */
    
    TSB_PF->CR  = 0x10;/* PF4を出力ﾎﾟｰﾄにする */
    TSB_PF_DATA_PF4 = 0;/* LCDバックライト出力をOFFにする */
    
    TSB_PG->CR |= 0xFF;/* PG0~PG7を出力ポートに設定 */
    TSB_PG->DATA = 0;/* 出力をクリア */
    
    TSB_PN->CR |= 0xFF;/* PN0~PN7を出力ポートに設定 */
    TSB_PN->DATA = 0;/* 出力をクリア */
    
    /*************ADコンバーター設定*******************************************/
    /* ADCクロックのセット */
    ADC_SetClk(ADC_HOLD_FIX, ADC_FC_DIVIDE_LEVEL_NONE);
    /* ソフトウェアトリガ用プログラムによるAD変換結果レジスタを選択 */
    /* TRG_ENABLE()で選んだADチャネルを許可する */
    /* レジスタ0 */
    ADC_SetSWTrg(ADC_REG0, TRG_ENABLE(ADC_AIN0));
    /* レジスタ1 */
    ADC_SetSWTrg(ADC_REG1, TRG_ENABLE(ADC_AIN1));
    /* レジスタ2 */
    ADC_SetSWTrg(ADC_REG2, TRG_ENABLE(ADC_AIN2));
    
    ADC_SetResolution(ADC_12BITS);/* AD変換の分解能を12ビットにする */
    /* AVREFH～AVREFL間のリファレンス電流の制御。DISABLEでリセット時以外は常時通電となる */
    ADC_SetLowPowerMode(DISABLE);
    /* AD変換モジュールの有効化 */
    ADC_Enable();
    /*********ADコンバーター設定ここまで***************************************/
    
    /* SBI0をI2Cモードにする初期設定 */
    /****************** PGピン0～1をI2Cモードに設定する ***********************/
    /*PC0～1ポートの出力許可*/
    GPIO_SetOutputEnableReg(GPIO_PC, GPIO_BIT_0, ENABLE);/* PC0、SDA0ライン */
    GPIO_SetOutputEnableReg(GPIO_PC, GPIO_BIT_1, ENABLE);/* PC1、SCL0ライン */
    
    if (TSB_PC_DATA_PC0 == 0) {
        /* SDAであるPC0がLow張り付きであった場合 */
        /* SDA,SCLラインをカチカチする */
        TSB_PC_DATA_PC1 = 1;/* PC1をHighにする */
        for (uint8_t j=0;j<=5;j++) {
            __NOP();
        }
        for (uint8_t i=0;i<=9;i++) {
            TSB_PC->CR &= 0xFE;/* PC0をHI-Zにする */
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
            TSB_PC->CR |= 0x01;/* PC0を出力許可する */
            TSB_PC_DATA_PC0 = 0;/* PC0をLowにする */
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
            TSB_PC_DATA_PC1 = 0;/* PC1をLowにする */
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
            TSB_PC->CR &= 0xFE;/* PC0をHI-Zにする */
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
            TSB_PC_DATA_PC1 = 1;/* PC1をHighにする */
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
        }
        TSB_PC->CR |= 0x01;/* PC0の出力許可 */
    }
    
    /*PC0～1ポートの入力許可*/
    GPIO_SetInputEnableReg(GPIO_PC, GPIO_BIT_0, ENABLE);/* PC0、SDA0ライン */
    GPIO_SetInputEnableReg(GPIO_PC, GPIO_BIT_1, ENABLE);/* PC1、SCL0ライン */
    
    /*ファンクションレジスタの設定(SDA＆SCLにしている)*/
    GPIO_EnableFuncReg(GPIO_PC, GPIO_FUNC_REG_3, GPIO_BIT_0);/*SDAに設定*/
    GPIO_EnableFuncReg(GPIO_PC, GPIO_FUNC_REG_3, GPIO_BIT_1);/*SCLに設定*/
    /*PG0～1をオープンドレイン設定*/
    GPIO_SetOpenDrain(GPIO_PC, GPIO_BIT_0, ENABLE);/*PG0オープンドレイン許可*/
    GPIO_SetOpenDrain(GPIO_PC, GPIO_BIT_1, ENABLE);/*PG1オープンドレイン許可*/
    /*どうしてこんな設定しているかはデータシートを参照*/
    /*******************I2C設定************************************************/
    /* SBI0初期設定 */
    myI2C.I2CSelfAddr = SELF_ADDR;/*自分のアドレス設定*/
    myI2C.I2CDataLen  = SBI_I2C_DATA_LEN_8; /* I2Cモードの転送ビット長。8ビットにする */
    myI2C.I2CACKState = ENABLE;/* ACKを有効に設定する */
    myI2C.I2CClkDiv   = SBI_I2C_CLK_DIV_200;/* I2Cの転送ｸﾛｯｸ = fsys/200 = 約200kHz */
    SBI_Enable(TSB_SBI0);/*シリアルバスインターフェースの許可*/
    SBI_SWReset(TSB_SBI0);/*SBI回路を初期化する*/
    SBI_InitI2C(TSB_SBI0, &myI2C);/*I2C通信の初期化*/
    /***************I2C設定ここまで********************************************/
    
    /************タイマー割り込みの設定****************************************/
    TMRB_InitTypeDef m_tmrb;/*TMRB設定用構造体にmain内で扱うために名前を付ける*/
    
    /* ～ TMRB設定用構造体の定義 ～ */
    m_tmrb.Mode = TMRB_INTERVAL_TIMER;/*モードはインターバルタイマモード*/
    m_tmrb.ClkDiv = TMRB_CLK_DIV_8;/*インターバルタイマのソースクロックをf/8*/
    m_tmrb.TrailingTiming = TMRB_1MS;/*タイマRG1に書き込む値。アップカウンタと比較する*/
    /*これ↑は要計算箇所 1ms=0x1388　tmpm380_CMN.hの中で定義している*/
    m_tmrb.UpCntCtrl = TMRB_AUTO_CLEAR;/*アップカウンタの動作設定.*/
    /*Trailing Timingで設定した値とアップカウンタが一致した際に0クリアして再スタートする*/
    m_tmrb.LeadingTiming = TMRB_1MS;/*タイマRG0に書き込む値。アップカウンタと比較する*/
    /*これ↑は要計算箇所 1ms=0x1388　main.hの中で定義している*/
    
    /*ここでタイマー割り込みにより1ms置きにデコーダー出力をカウントアップする*/
    TMRB_Enable(TSB_TB0);/*TSB_TB0チャネルのTMRBタイマの動作許可*/
    TMRB_Init(TSB_TB0, &m_tmrb);/*TSB_TB0チャネルの初期化*/
    /********タイマー割り込みの設定ここまで************************************/
    
    /********外部ﾂｰﾙ用UARTの有効化処理*****************************************/
    TSB_PE->FR1 |= 0x03;/* UART機能をON */
    TSB_PE->CR  |= 0x01;/* 出力にセット */
    TSB_PE->IE  |= 0x02;/* 入力にセット */
    
    myUART.BaudRate = (uint32_t)9600;/* ボーレート設定 */
    myUART.DataBits = UART_DATA_BITS_7;/* ビット幅設定 */
    myUART.StopBits = UART_STOP_BITS_1;/* ストップビット設定 */
    myUART.Parity   = UART_EVEN_PARITY;/* パリティビット設定 */
    myUART.Mode     = UART_ENABLE_TX|UART_ENABLE_RX;/* 送受信モードに設定 */
    myUART.FlowCtrl = UART_NONE_FLOW_CTRL;/* フロー制御なし */
    
    UART_Enable(UART0);/* UART0の有効化 */
    UART_Init(UART0, &myUART);/* 上記で設定したUART項目の有効化 */

    UART_TRxAutoDisable(UART0, UART_RXTXCNT_AUTODISABLE);/* 送受信の自動禁止に設定
    　　　 　　　　　　　　　　　　　　　　　　　　　 (データシート参照) */
    UART_FIFOConfig(UART0, DISABLE);/* UART0のFIFOを不許可とする */
    /********外部ﾂｰﾙ用UARTの有効化処理ここまで*********************************/
    
    /******* 割り込み優先順位設定 *********************************************/
    NVIC_SetPriority(INT3_IRQn, 0x00);/* INT3割り込み優先順位変更 */
    NVIC_SetPriority(INTADSFT_IRQn, 0x02);/* AD変換割り込み優先順位変更 */
    NVIC_SetPriority(INTTB00_IRQn, 0x02);/* タイマ割り込み優先順位変更 */
    NVIC_SetPriority(INTSBI0_IRQn, 0x02);/* I2C割り込み優先順位変更 */
    NVIC_SetPriority(INTTX0_IRQn, 0x02);/* UART0 送信割り込み優先順位変更 */
    NVIC_SetPriority(INTRX0_IRQn, 0x02);/* UART0 受信割り込み優先順位変更 */
    
    /* システムハンドラ割り込み優先順位変更(使用しないんだけどね) */
    NVIC_SetPriority(MemoryManagement_IRQn, 0x01);
    NVIC_SetPriority(BusFault_IRQn, 0x01);
    NVIC_SetPriority(UsageFault_IRQn, 0x01);
    NVIC_SetPriority(SVCall_IRQn, 0x01);
    NVIC_SetPriority(DebugMonitor_IRQn, 0x01);
    NVIC_SetPriority(PendSV_IRQn, 0x01);
    NVIC_SetPriority(SysTick_IRQn, 0x01);
    
    /******* INT3割り込み許可設定 *********************************************/
    CG_SetSTBYReleaseINTSrc(CG_INT_SRC_3, CG_INT_ACTIVE_STATE_FALLING, DISABLE);/*INT3は立下りエッジで割り込み発生に設定*/
    CG_SetSTBYReleaseINTSrc(CG_INT_SRC_3, CG_INT_ACTIVE_STATE_FALLING, ENABLE);/*INT3解除入力許可*/
    CG_ClearINTReq(CG_INT_SRC_3);/*割り込み要求INT3の解除*/
    NVIC_ClearPendingIRQ(INT3_IRQn);/*INT3割り込み保留ラッチのクリア*/
    NVIC_EnableIRQ(INT3_IRQn);/*INT3割り込み許可*/
    /***** INT3割り込み許可設定ここまで ***************************************/
    
    /** タイマー割り込み許可 **/
    NVIC_EnableIRQ(INTTB00_IRQn);/* INTTB00の割り込み許可 */
    TMRB_SetRunState(TSB_TB0, TMRB_RUN);/*タイマーのアップカウンタの起動*/
    
    /* Main Roop */
    while (1) {
        /*-- OS処理部分 --*/
        if (ERRFG == 0xA1) {
            /* NMIが入っていた場合 */
            break;/* 戻る */
            
        }
        
        /** AD割り込み(ソフトウェアAD変換完了時) **/
        NVIC_EnableIRQ(INTADSFT_IRQn);/* INTADSFTの割り込み許可 */
        /** タイマー割り込み許可 **/
        NVIC_EnableIRQ(INTTB00_IRQn);/*  INTTB00の割り込み許可  */
        /** I2C割り込み許可 **/
        NVIC_EnableIRQ(INTSBI0_IRQn);/*  INTSBI0の割り込み許可  */
        /** 外部ツール通信の割り込み許可 **/
        NVIC_EnableIRQ(INTTX0_IRQn);/*   UART0 送信割り込み許可 */
        NVIC_EnableIRQ(INTRX0_IRQn);/*   UART0 受信割り込み許可 */
        
        TMRB_SetRunState(TSB_TB0, TMRB_RUN);/*タイマーのアップカウンタの起動*/
        
        OS_TSKCon1 *OS1;/* OS設定値を読むためのポインタ構造体に名前をつける */
        OS1 = OSTSKCon1S();/* 関数経由でポインタをOSparamにアクセス */

        static uint8_t NextRunTask = (TSKNUM - 1);/* タスク番号の初期値 */
        
        NextRunTask++;/* 次回起動タスク番号をインクリメント */
        if (NextRunTask <= (TSKNUM - 1)) {
            /* 範囲内であれば何もしない */
        } else {
            /* 範囲外であれば0クリアする */
            NextRunTask = 0;
        }
        
        if (((OS1->TSK_COND[NextRunTask]) & 0x80) == 0x80) {
            /* TSK_COND[NextRunTask]の7ビット目が1のとき */
            NVIC_DisableIRQ(INTADSFT_IRQn);/* AD変換割り込み禁止 */
            NVIC_DisableIRQ(INTTB00_IRQn);/* タイマー割り込み禁止 */
            NVIC_DisableIRQ(INTSBI0_IRQn);/* I2C割り込み許可 */
            NVIC_DisableIRQ(INTTX0_IRQn);/* UART0 送信割り込み禁止 */
            NVIC_DisableIRQ(INTRX0_IRQn);/* UART0 受信割り込み禁止 */
            
            if (((OS1->TSK_COND[NextRunTask]) & 0x20) == 0x00) {
                /* TSK_COND[NextRunTask]の5ビット目が0のとき */
                /* OSが言ったタスクをセレクトする処理 */
                switch (NextRunTask) {
                case 0:
                    DEVICE_INIT();/*初期化アプリ*/
                    break;
                case 1:
                    INPUT_CONT();/*入力処理アプリ*/
                    break;
                case 2:
                    AD_CONVERTER();/*AD変換処理アプリ*/
                    break;
                case 3:
                    CCONT();/* 真空冷却アプリ */
                    break;
                case 4:
                    WCONT();/* 水位制御アプリ */
                    break;
                case 5:
                    DCONT();/* 表示制御アプリ */
                    break;
                case 6:
                    OPRCNT();/* 操作制御ｱﾌﾟﾘ */
                    break;
                case 7:
                    COMCNT();/* 通信制御ｱﾌﾟﾘ */
                    break;
                default:
                    break;/*範囲外の値が来た場合はブレークする*/
                }
            } else {
                /* 何もしない */
            }
        } else {
            /* 何もしない */
        }
        /*-- OS処理部分 ここまで --*/
    }
    /* Main Loop End */
    
    if (D_INISQ > 0) {
        /* データ読み出し完了していた場合 */
        KEEP();/* 抜けた後、データ保存処理 */
        
    }
    CPUERR();/* "Good bye!" */
    
}

/* --外付けWDTのクリア------------------------------------------------------- */
void WDTCLR(void)
{
    TSB_PN_DATA_PN0 = (TSB_PN_DATA_PN0 ^ 1);/* ビットの反転処理 */
    return;
}

/**
 *名称：外部ツール用の通信ポート0送信割り込み処理
 *作成者：浅尾　享一
 *説明：通信ポート0の送信処理
**/
void INTTX0_IRQHandler(void)
{
    if (((TXFLG0 & 0x02) == 0x02) || (TXLEN0 == 0) || (TXSET0 == 0)) {
        /* 送信キャンセルの場合 or 送信終了 or 送信バッファが空の場合 */
        TXFLG0 &= 0xEE;/* 送信中と送信フラグを削除 */
        TXFLG0 |= 0x20;/* 送信バッファエンプティフラグセット */
        
    } else {
        /* そうではない場合 */
        TSB_SC0->BUF = TBUF0[(TXPTR0 & 0x0FF)];/* 送信物をセット */
        TXPTR0++;/* 送信ポインタをインクリメント */
        TXPTR0 &= 0x0FF;/* MAX => 255(送信ポート0のバッファ制限)になるようにマスクする */
        TXSET0--;/* 送信バッファセット数 - 1 */
        TXLEN0--;/* 送信長 - 1 */
        
    }
    
    return;/* 戻る */
}

/**
 *名称：外部ツール用の通信ポート0受信割り込み処理
 *作成者：浅尾　享一
 *説明：通信ポート0の受信処理
**/
void INTRX0_IRQHandler(void)
{
    uint8_t REG_00 = 0;
    if ((TSB_SC0->CR & 0x1C) > 0x00) {
        /* 受信時何かしらのエラーがあれば何もしない処理 */
        REG_00 = TSB_SC0->BUF;/* ダミー読み出し */
        REG_00 = 0x04;
        USTS0 |= REG_00;/* 何か受信時エラーセット */
        
    } else {
        /* エラーが無い場合 */
        if (RXLEN0 >= 255) {
            /* 受信長が受信バッファより大きくなってしまった場合 */
            REG_00 = TSB_SC0->BUF;/* ダミー読み出し */
            
            REG_00 = 0x08;
            USTS0 |= REG_00;/* バッファオーバーランエラーセット */
            
        } else {
            /* 受信長が受信バッファ以内の場合 */
            RBUF0[(RXPTR0 + RXLEN0) & 0x0FF] = (TSB_SC0->BUF & 0x7F);/* 受信バッファに受信データを格納する */
            RXLEN0++;/* 受信データ長インクリメント */
            
        }
    }
    
    return;/* 戻る */
}