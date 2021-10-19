/**
 *題名：初期化アプリ
 *作成者：浅尾　享一
 *初期化に必要な処理を実行するアプリケーション
**/
#include "tmpm380_DEVICEINI.h"

void DEVICE_INIT(void)
{
    OS_TSKCon1 *OS_LoadTime;/* OS設定値を読むためのポインタ構造体に名前をつける */
    OS_LoadTime = OSTSKCon1S();/* 関数経由でポインタをOSparamにアクセス */
    
    uint8_t REG_00, REG_01 = 0;/* 一時処理変数 */
    
    /* 以下、初期化アプリが実行する処理 */
    WDTCLR();/* 外部ｳｫｯﾁﾄﾞｯｸﾞﾀｲﾏのｸﾘｱ処理 */
    
    if (D_INISQ == 0) {
        /* 初回起動時動作 */
        /* ﾃﾞｨｯﾌﾟｽｲｯﾁの読み出し処理 */
        while (1) {
            /* ループここから */
            NVIC_EnableIRQ(INTTB00_IRQn);/* タイマ割り込み許可 */
            
            while (count01 < 5);/* 5ms待ちます */
            TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
            count01 = 0;/* カウントの0クリア */
            while (count01 < 5);/* 5ms待ちます */
            TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
            count01 = 0;
            WDTCLR();/* 外付けウォッチドッグタイマーのクリア */
            
            DIPBUF = (MXIN1 & 0xF0);/* 現在ディップスイッチの値を代入する */
            
            while (count01 < 5);/* 5ms待ちます */
            TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
            count01 = 0;/* カウントの0クリア */
            while (count01 < 5);/* 5ms待ちます */
            TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
            count01 = 0;
            WDTCLR();/* 外付けウォッチドッグタイマーのクリア */
            while (count01 < 5);/* 5ms待ちます */
            TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
            count01 = 0;/* カウントの0クリア */
            while (count01 < 5);/* 5ms待ちます */
            TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
            count01 = 0;
            WDTCLR();/* 外付けウォッチドッグタイマーのクリア */
            
            if ((MXIN1 & 0xF0) == DIPBUF) {
                /* ﾃﾞｨｯﾌﾟｽｲｯﾁの取り込みが正常である場合 */
                SYKND0 = DIPBUF;/* 機器情報に保存する */
                break;/* 抜けます */
                
            }
            
        }/* ループここまで */
        
        TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
        count01 = 0;
        WDTCLR();/* 外付けウォッチドッグタイマーのクリア */
        
        /* NV-RAMからのデータ読み出し処理 */
        SETUP();/* 読み出し処理 */
        
        ABNFO = (ABNF & 0x0F);/* オールドに保存する */
        if ((SYSFLG & 0x40) > 0) {
            /* 電源ONフラグがある場合 */
            DSPNO = 1;/* トップ画面にセット */
            
        }
        
        if ((SYSFLG & 0x02) > 0) {
            /* 運転中フラグがあった場合 */
            if (((ABNF & 0x0F) == 0) || ((ABNF & 0x0F) == 11)) {
                /* 正常で電源が入ってきた、または停電異常であった場合 */
                ABNF |= 0x0B;/* 11（0x0B）停電異常をセット */
                ABNF |= 0x80;/* ブザーフラグをON */
                STSFL0 |= 0x20;/* 準備中のフラグをセット */
                
            }/* 停電異常以外の異常であった場合はここに抜ける */
            
        }/* 運転中フラグがあった場合はここに抜ける */
        
        if ((RPAT & 0x20) > 0) {
            /* 洗浄工程中であった場合 */
            DSPSEQ = 4;/* 洗浄モードセット */
            DSPSSQ = 2;/* 洗浄工程表示をセット */
            STSFL0 |= 0x80;/* 設定モードをセット */
            
        }
        
        if (NPAT == 11) {
            /* NPATの値が11であった場合 */
            TNO = 11;/* セルフテストナンバーを11にする */
            
        }
        
        WDTCLR();/* 外付けウォッチドッグタイマーのクリア */
        TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
        count01 = 0;/* カウントクリア */
        
        /* I2CバスでRTCとDACの確認を行う処理 */
        I2C_WCNT = 0;/* I2C書込み回数をｸﾘｱ */
        I2C_RCNT = 0;/* I2C読み込み処理回数をクリア */
        I2C_STATE = 0;/* I2C割り込みｽﾃｰﾄを初回用にする */
        
        SBI_I2CState i2c_state;/* I2Cバス状態を見る構造体に名前を付ける */
        
        NVIC_EnableIRQ(INTSBI0_IRQn);/* I2C通信割り込みの許可 */
        SBI_GenerateI2CStop(TSB_SBI0);/* I2C通信のｽﾄｯﾌﾟをｾｯﾄ */
        
        i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
        while (i2c_state.Bit.BusState == 1) {
            /* バスビジーである場合 */
            SBI_GenerateI2CStop(TSB_SBI0);/* 最初にｽﾄｯﾌﾟｺﾝﾃﾞｨｼｮﾝをｾｯﾄ */
            while (count01 < 5);/* 5ms待つ */
            WDTCLR();/* 外付けウォッチドッグタイマーのクリア */
            TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
            count01 = 0;/* カウントクリア */
            
            I2C_RSTE();/* I2Cﾊﾞｽﾗｲﾝをｸﾘｱ */
            
            i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
            
        }
        
        /* RTCの初期設定などの確認処理 */
        /* ダミーリード処理 */
        T0BF1 = 0;/* 読み出しﾁｪｯｸ用にクリア */
        I2C_RCNT = 1;/* 受信回数ｾｯﾄ(直ぐにストップする処理) */
        
        i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
        for (uint8_t j = 0;j < 10;j++) {
            /* ダミーリードを10回トライアル */
            SBI_SetSendData(TSB_SBI0, ((RTC_ADDR) | 0x01));/* RTC読出し */
            SBI_GenerateI2CStart(TSB_SBI0);/* I2Cスタートさせる。送信終了後、割りこみ発生 */
            
            count01 = 0;/* カウントクリア */
            while (count01 < 5);/* 5ms待つ */
            count01 = 0;/* カウントクリア */
            while (count01 < 5);/* 5ms待つ */
            TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
            WDTCLR();/* 外付けウォッチドッグタイマーのクリア */
            count01 = 0;/* カウントクリア */
            
            if ((I2C_RCNT > 2) && (T0BF1 == 1)) {
                /* ダミー読出しできた場合 */
                SBI_SetI2CACK(TSB_SBI0, ENABLE);/* ACKを無効化しているため、ここで有効にする */
                break;
                
            } else {
                /* ダミー読出し出来ていない場合 */
                i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
                while (i2c_state.Bit.BusState == 1) {
                    /* バスビジーである場合 */
                    SBI_GenerateI2CStop(TSB_SBI0);/* 最初にｽﾄｯﾌﾟｺﾝﾃﾞｨｼｮﾝをｾｯﾄ */
                    count01 = 0;/* カウントクリア */
                    while (count01 < 5);/* 10ms待つ */ 
                    TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
                    WDTCLR();/* 外付けウォッチドッグタイマーのクリア */
                    
                    I2C_RSTE();/* I2Cﾊﾞｽﾗｲﾝをｸﾘｱ */
                    
                    i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
                    
                }
                
            }
            
            SBI_SetI2CACK(TSB_SBI0, ENABLE);/* ACKを無効化しているため、ここで有効にする */
            
        }
        
        T0BF1 = 0;/* ｸﾘｱ */
        SBI_GenerateI2CStop(TSB_SBI0);/* ｽﾄｯﾌﾟｺﾝﾃﾞｨｼｮﾝをｾｯﾄ */
        
        /* VLFﾋﾞｯﾄの確認 */
        I2C_STATE = 0;/* 割り込みステート変更 */
        I2C_WCNT = 0;/* 初回書込みカウントを0リセット */
        I2C_RCNT = 0;/* 初回読込みカウントを0リセット */
        T0BF0 |= 0x02;/* 仮でVLFﾌﾗｸﾞをｾｯﾄ(RTC準備未完了の意) */
        
        count01 = 0;/* カウントクリア */
        for (uint8_t i = 0;i < 10;i++) {
            i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
            if (i2c_state.Bit.BusState == 1) {
                /* バスビジーである場合 */
                SBI_GenerateI2CStop(TSB_SBI0);/* ｽﾄｯﾌﾟｺﾝﾃﾞｨｼｮﾝをｾｯﾄ */
                count01 = 0;/* カウントクリア */
                while (count01 < 5);/* 5ms待つ */
                TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
                WDTCLR();/* 外付けウォッチドッグタイマーのクリア */
                count01 = 0;/* カウントクリア */
                
                I2C_RSTE();/* I2Cﾊﾞｽﾗｲﾝをｸﾘｱ */
                
            } else {
                /* バスフリー確認が出来た場合 */
                break;
                
            }
            
            if (i == 9) {
                /* 最後の場合 */
                RTC_ERR |= 0x01;/* RTC異常があったフラグをセット */
            }
            
        }
        
        i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
        if (i2c_state.Bit.BusState == 0) {
            /* バスフリーである場合 */
            T0BF1 = 0;/* 使用するﾊﾞｯﾌｧクリア */
            SBI_SetSendData(TSB_SBI0, ((RTC_ADDR) | 0x00));/* ダミー書込み */
            SBI_GenerateI2CStart(TSB_SBI0);/* I2Cスタートさせる。送信終了後、割りこみ発生 */
            
            count01 = 0;/* カウントクリア */
            while (count01 < 5);/* 5ms待ちます */
            TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
            count01 = 0;/* カウントクリア */
            
            i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
            if ((I2C_WCNT >= 2) && (i2c_state.Bit.BusState == 0)) {
                /* RTCと通信可能である場合 */
                SBI_SetSendData(TSB_SBI0, (RTC_ADDR | 0x01));/* スタート後、スレーブアドレスを送る。続いて読込み */
                SBI_GenerateI2CStart(TSB_SBI0);/* I2Cスタート状態にさせる。送信終了後、割りこみ発生 */
                count01 = 0;/* カウントクリア */
                while (count01 < 5);/* 5ms待ちます */
                TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
                WDTCLR();/* クリアー */
                count01 = 0;/* カウントクリア */
                
                if ((I2C_RCNT >= 3) && (T0BF1 == 1)) {
                    /* 受信完了した場合 */
                    RTC_ERR = 0;/* RTC異常をクリア */
                } else {
                    RTC_ERR |= 0x01;/* RTC異常があったフラグをセット */
                }
            } else {
                /* RTCと通信出来ない場合 */
                RTC_ERR |= 0x01;/* RTC異常があったフラグをセット */
            }
        } else {
            /* バスビジーである場合 */
            RTC_ERR |= 0x01;/* RTC異常があったフラグをセット */
        }
        
        SBI_SetI2CACK(TSB_SBI0, ENABLE);/* ACKを無効化しているため、ここで有効にする */
        
        while ((T0BF0 & 0x02) == 0x02) {
            /* 読出したVLFビットが1だった場合 */
            I2C_STATE = 1;/* 初回読み出し1終了 割り込みステートの変更 */
            T0BF2 = 0;/* ｸﾘｱ */
            
            /* 初期化コマンドおよび、VLFレジスタに0書き込み */
            for (;T0BF2 <= 8;T0BF2++) {
                I2C_WCNT = 0;/* 書込みカウントを0リセット */
                I2C_RCNT = 0;/* 読込みカウントを0リセット */
                /* I2Cバス状態の確認を行う */
                count01 = 0;/* カウントクリア */
                while (count01 < 5);/* 5ms待つ */
                TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
                WDTCLR();/* WDT clear */
                count01 = 0;/* カウントクリア */
                
                i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
                if (i2c_state.Bit.BusState == 1) {
                    /* バスビジーである場合 */
                    SBI_GenerateI2CStop(TSB_SBI0);/* 最初にｽﾄｯﾌﾟｺﾝﾃﾞｨｼｮﾝをｾｯﾄ */
                    count01 = 0;/* カウントクリア */
                    while (count01 < 5);/* 5ms待つ */
                    TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
                    WDTCLR();/* WDT clear */
                    count01 = 0;/* カウントクリア */
                    
                }
                
                i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
                if (i2c_state.Bit.BusState == 0) {
                    /* バスフリーである場合 */
                    SBI_SetSendData(TSB_SBI0, ((RTC_ADDR) | 0x00));/* RTCへ書込み */
                    SBI_GenerateI2CStart(TSB_SBI0);/* I2Cスタートさせる。送信終了後、割りこみ発生 */
                    
                    count01 = 0;/* カウントクリア */
                    for (uint8_t i = 0;i < 4;i++) {
                        /* 20ms待つ処理 */
                        while (count01 < 5);/* 5ms待つ */
                        TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
                        WDTCLR();/* WDT clear */
                        count01 = 0;/* カウントクリア */
                        
                    }
                    
                    if (I2C_WCNT >= 2) {
                        /* 書き込み完了である場合 */
                        RTC_ERR = 0;/* ｸﾘｱｰ */
                    } else {
                        RTC_ERR |= 0x01;/* RTC通信異常をセット */
                    }
                } else {
                    /* バスフリーにならない場合 */
                    RTC_ERR |= 0x01;/* RTC通信異常をセット */
                }
            }
            
            I2C_STATE = 0;/* 割り込みステートの変更 */
            I2C_WCNT = 0;/* 初回書込みカウントを0リセット */
            I2C_RCNT = 0;/* 初回読込みカウントを0リセット */
            T0BF0 |= 0x02;/* 仮でVLFﾌﾗｸﾞｾｯﾄ */
            
            i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
            while (i2c_state.Bit.BusState == 1) {
                /* バスビジーである場合は繰り返し */
                SBI_GenerateI2CStop(TSB_SBI0);/* ｽﾄｯﾌﾟｺﾝﾃﾞｨｼｮﾝをｾｯﾄ */
                count01 = 0;/* カウントクリア */
                while (count01 < 5);/* 5ms待つ */
                TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
                WDTCLR();/* WD clear */
                count01 = 0;/* カウントクリア */
                i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
                
            }
            
            i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
            if (i2c_state.Bit.BusState == 0) {
                /* バスフリーである場合 */
                T0BF1 = 0;/* 使用するﾊﾞｯﾌｧクリア */
                SBI_SetSendData(TSB_SBI0, ((RTC_ADDR) | 0x00));/* ダミー書込み */
                SBI_GenerateI2CStart(TSB_SBI0);/* I2Cスタートさせる。送信終了後、割りこみ発生 */
                
                count01 = 0;/* カウントクリア */
                while (count01 < 5);/* 5ms待つ */
                count01 = 0;/* カウントクリア */
                while (count01 < 5);/* 5ms待つ */
                
                TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
                WDTCLR();/* WDT clear */
                count01 = 0;/* カウントクリア */
                i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
                
                if ((I2C_WCNT >= 2) && (i2c_state.Bit.BusState == 0)) {
                    /* RTCと通信可能である場合 */
                    SBI_SetSendData(TSB_SBI0, (RTC_ADDR | 0x01));/* スタート後、スレーブアドレスを送る。(読込み) */
                    SBI_GenerateI2CStart(TSB_SBI0);/* I2Cスタート状態にさせる。送信終了後、割りこみ発生 */
                    count01 = 0;/* カウントクリア */
                    while (count01 < 5);/* 5ms待つ */
                    count01 = 0;/* カウントクリア */
                    while (count01 < 5);/* 5ms待つ */
                    
                    TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
                    WDTCLR();/* WDT clear */
                    count01 = 0;/* カウントクリア */
                    
                    if ((I2C_RCNT >= 3) && (T0BF1 == 1)) {
                        /* 受信完了した場合 */
                        RTC_ERR = 0;
                    } else {
                        RTC_ERR |= 0x01;
                    }
                } else {
                    /* RTCと通信出来ない場合 */
                    RTC_ERR |= 0x01;
                }
            } else {
                /* バスビジーである場合 */
                RTC_ERR |= 0x01;
            }
            
            SBI_SetI2CACK(TSB_SBI0, ENABLE);/* ACKを無効化しているため、ここで有効にする */
            
            if ((RTC_ERR & 0x01) == 0x01) {
                /* 通信不良があった場合は抜けます */
                break;
            }
            
        }/* ループここまで */
        
        I2C_STATE = 2;/* 初回RTC状態確認終了 割り込みステートの変更 */
        i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
        /* ↓RTCのレジスタに設定値を入力していく↓ */
        if (i2c_state.Bit.BusState == 1) {
            /* バスビジーである場合 */
            SBI_GenerateI2CStop(TSB_SBI0);/* I2C通信終了をｾｯﾄ */
            count01 = 0;/* カウントクリア */
            while (count01 < 5);/* 5ms待つ */
            TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
            WDTCLR();/* WDT clear */
            count01 = 0;/* カウントクリア */
            
        }
        T0BF0 = 0;/* 初期化用作業領域のクリア */
        
        if ((RTC_ERR & 0x01) == 0x00) {
            /* バスフリーである場合 */
            for (;T0BF0 <= 6; T0BF0++) {
                I2C_WCNT = 0;/* 初回書込みカウントを0リセット */
                I2C_RCNT = 0;/* 初回読込みカウを0リセット */
                /* 待ちます */
                count01 = 0;/* カウントクリア */
                while (count01 < 5);/* 5ms待つ */
                TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
                WDTCLR();/* WDT clear */
                count01 = 0;/* カウントクリア */
                
                /* I2Cバス状態の確認を行う */
                i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
                if (i2c_state.Bit.BusState == 0) {
                    /* I2Cバスの状態確認(利用可になるまで繰り返す) */
                    SBI_SetSendData(TSB_SBI0, ((RTC_ADDR) | 0x00));/* RTCへ書込み */
                    SBI_GenerateI2CStart(TSB_SBI0);/* I2Cスタートさせる。送信終了後、割りこみ発生 */
                    count01 = 0;/* カウントクリア */
                    while (count01 < 5);/* 5ms待つ */
                    count01 = 0;/* カウントクリア */
                    while (count01 < 5);/* 5ms待つ */
                    
                    TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
                    WDTCLR();/* WDT clear */
                    count01 = 0;/* カウントクリア */
                    
                    if (I2C_WCNT < 2) {
                        /* 書き込み終了していない場合 */
                        RTC_ERR |= 0x01;
                    } else {
                        /* 書き込み終了した場合 */
                        RTC_ERR = 0;
                    }
                } else {
                    /* 利用不可である場合 */
                    RTC_ERR |= 0x01;
                }
                
            }
            
            SBI_GenerateI2CStop(TSB_SBI0);/* ストップコンディションｾｯﾄ */
        }
        
        I2C_STATE = 3;/* 初回RTCレジスタ書込み終了 割り込みステートの変更 */
        
        /* ↓RTCの時刻読み込み処理↓ */
        T0BF0 = 0;/* 初期化アプリ作業領域のクリア */
        T0BF1 = 0;
        
        I2C_WCNT = 0;/* 初回書込みカウントを0リセット */
        I2C_RCNT = 0;/* 初回読込みカウントを0リセット */
        
        /* I2Cバス状態の確認を行う */
        if (RTC_ERR == 0) {
            /* 利用可能である場合 */
            i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
            if (i2c_state.Bit.BusState == 0) {
                /* I2Cバスの状態確認(利用可になるまで繰り返す) */
                SBI_SetSendData(TSB_SBI0, ((RTC_ADDR) | 0x00));/* ダミー書込み */
                SBI_GenerateI2CStart(TSB_SBI0);/* I2Cスタートさせる。送信終了後、割りこみ発生 */
                
                count01 = 0;/* カウントクリア */
                while (count01 < 5);/* 5ms待つ */
                count01 = 0;/* カウントクリア */
                while (count01 < 5);/* 5ms待つ */
                
                TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
                WDTCLR();/* WDT clear */
                count01 = 0;/* カウントクリア */
                
                i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
                if ((I2C_WCNT >= 2) && (i2c_state.Bit.BusState == 0)) {
                    /* ダミーライトできており、バスフリーである場合 */
                    SBI_SetSendData(TSB_SBI0, (RTC_ADDR | 0x01));/* スタート後、スレーブアドレスを送る。続いて読込み */
                    SBI_GenerateI2CStart(TSB_SBI0);/* I2Cスタート状態にさせる。送信終了後、割りこみ発生 */
                    
                    count01 = 0;/* カウントクリア */
                    while (count01 < 5);/* 5ms待つ */
                    count01 = 0;/* カウントクリア */
                    while (count01 < 5);/* 5ms待つ */
                    
                    TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
                    WDTCLR();
                    count01 = 0;/* カウントクリア */
                    
                    if (T0BF1 == 1) {
                        /* 読出し完了した場合 */
                        RTC_ERR = 0;/* クリア */
                    } else {
                        /* 読出し未完了の場合 */
                        RTC_ERR |= 0x01;/* セット */
                    }
                } else {
                    /* 通信できない場合 */
                    RTC_ERR |= 0x01;/* セット */
                }
            } else {
                /* バスフリーにならない場合 */
                RTC_ERR |= 0x01;/* セット */
            }
            
        }
        
        SBI_SetI2CACK(TSB_SBI0, ENABLE);/* 読込みの最後でACKを無効化しているため、ここで有効にする */
        T0BF1 = 0;
        
        REG_00 = (RTCDT[0] & 0x0F);
        REG_01 = ((RTCDT[0] & 0xF0) >> 4) * 10;
        RTCDT[0] = REG_00 + REG_01;/* 秒　BCD→バイナリ変換 */
        REG_00 = (RTCDT[1] & 0x0F);
        REG_01 = ((RTCDT[1] & 0xF0) >> 4) * 10;
        RTCDT[1] = REG_00 + REG_01;/* 分　BCD→バイナリ変換 */
        REG_00 = (RTCDT[2] & 0x0F);
        REG_01 = ((RTCDT[2] & 0xF0) >> 4) * 10;
        RTCDT[2] = REG_00 + REG_01;/* 時　BCD→バイナリ変換 */
        REG_00 = (RTCDT[4] & 0x0F);
        REG_01 = ((RTCDT[4] & 0xF0) >> 4) * 10;
        RTCDT[4] = REG_00 + REG_01;/* 日　BCD→バイナリ変換 */
        REG_00 = (RTCDT[5] & 0x0F);
        REG_01 = ((RTCDT[5] & 0xF0) >> 4) * 10;
        RTCDT[5] = REG_00 + REG_01;/* 月　BCD→バイナリ変換 */
        REG_00 = (RTCDT[6] & 0x0F);
        REG_01 = ((RTCDT[6] & 0xF0) >> 4) * 10;
        RTCDT[6] = REG_00 + REG_01;/* 年　BCD→バイナリ変換 */
        
        RTC_VAL_CHK();/* 取り出した時刻がおかしくないかチェック */
        
        if (RTC_ERR > 0) {
            /* 一度でもバスフリーにならなかった、もしくは日付に異常が見られた場合 */
            if (((ABNF & 0x0F) == 0)
            || (((SYSFLG & 0x02) > 0) && ((ABNF & 0x0F) == 8))
            || (((SYSFLG & 0x02) > 0) && ((ABNF & 0x0F) == 9))
            || (((SYSFLG & 0x02) > 0) && ((ABNF & 0x0F) == 14))) {
                /* 異常になってない場合、又は運転中で常温水位電極棒異常である
                又は運転中で常温温度センサー異常である、又は真空度異常である場合 */
                ABNF &= 0x60;/* MASK */
                ABNF |= 0x8C;/* RTCエラーセット、ブザーオン */
                SYSFLG |= 0x40;/* パワーオンフラグセット */
                
            }
            
            if ((RTC_ERR & 0x02) > 0) {
                /* 読み込み日付がおかしい場合 */
                SETST_BUF[6] = RTCDT[6] = 21;/* 年 */
                SETST_BUF[5] = RTCDT[5] = 4;/* 月 */
                SETST_BUF[4] = RTCDT[4] = 1;/* 日 */
                SETST_BUF[2] = RTCDT[2] = 12;/* 時 */
                SETST_BUF[1] = RTCDT[1] = 0;/* 分 */
                WR_RTC  |= 0x01;/* 書込みの意をセット */
                
            }
            
            RTC_ERR  = 0;/* クリア */
            
        }
        
        /* 年月日から曜日を算出する */
        SETST_BUF[3] = RTCDT[3] = GETWEEKDAY(RTCDT[6], RTCDT[5], RTCDT[4]);/* 曜日セット */
        
        /* D/Aコンバーターの初期設定（内部リファレンス処理） */
        I2C_WCNT = 0;/* I2C書込み回数をｸﾘｱ */
        I2C_STATE = 4;/* I2C割り込みｽﾃｰﾄを初回用にする */
        SBI_GenerateI2CStop(TSB_SBI0);/* I2C通信のｽﾄｯﾌﾟをｾｯﾄ */
        
        /* I2C通信によるDAｺﾝﾊﾞｰﾀの初期化処理 */
        I2CBUF[0] = (DACN_ADDR | 0x00);/* DACｱﾄﾞﾚｽ(write)ｾｯﾄ */
        I2CBUF[1] = 0x6F;/* DAC_A/B共に内部ﾘﾌｧﾚﾝｽﾓｰﾄﾞに設定する */
        I2CBUF[2] = 0xFF;/* Don't Care 1ﾊﾞｲﾄｾｯﾄ */
        I2CBUF[3] = 0xFF;/* Don't Care 1ﾊﾞｲﾄｾｯﾄ */
        
        i2c_state = SBI_GetI2CState(TSB_SBI0);
        while (i2c_state.Bit.BusState) {
            /*I2Cバスの状態確認(利用可になるまで繰り返す)*/
            count01 = 0;/* クリア */
            while(count01 < 5);/* 5ms待つ */
            
            TSB_WD->CR = 0x0000004E;/* 内蔵WDTのクリアコードを書き込む */
            WDTCLR();
            SBI_GenerateI2CStop(TSB_SBI0);/* I2C通信のｽﾄｯﾌﾟをｾｯﾄ */
            i2c_state = SBI_GetI2CState(TSB_SBI0);
            
        }
        
        count01 = 0;/* タイマーカウントクリア */
        
        for (uint8_t i = 0;i < 10;i++) {
            /* 10回繰り返しﾙｰﾌﾟ構文 */
            I2C_WCNT = 1;
            SBI_SetSendData(TSB_SBI0, I2CBUF[0]);/* スタート後、DACアドレスを送る。書込み */
            SBI_GenerateI2CStart(TSB_SBI0);/* I2Cスタート状態にさせる。送信終了後、割りこみ発生 */
            
            for (uint8_t j = 0;j < 5;j++) {
                /* 10回繰り返しﾙｰﾌﾟ構文(約100ms待つ処理) */
                while (count01 < 5);/* 5ms待ちます */
                TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
                count01 = 0;/* カウントの0クリア */
                while (count01 < 5);/* 5ms待ちます */
                TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
                count01 = 0;
                while (count01 < 5);/* 5ms待ちます */
                TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
                count01 = 0;/* カウントの0クリア */
                while (count01 < 5);/* 5ms待ちます */
                TSB_WD->CR = 0x0000004E;/*WDTのクリアコードを書き込む*/
                count01 = 0;
                WDTCLR();/* 外付けウォッチドッグタイマーのクリア */
                
            }
            
            if (I2C_WCNT > 3) {
                /* 全てのﾃﾞｰﾀが送信完了した場合 */
                /* DAｺﾝﾊﾞｰﾀの内部ﾘﾌｧﾚﾝｽ処理完了 */
                SBI_GenerateI2CStop(TSB_SBI0);/* I2C通信のｽﾄｯﾌﾟをｾｯﾄ */
                I2C_WCNT  = 0;
                I2C_STATE = 5;/* I2C割り込みｽﾃｰﾄの変更 */
                break;
                
            } else {
                /* データが送信出来ていない場合 */
                SBI_GenerateI2CStop(TSB_SBI0);/* I2C通信のｽﾄｯﾌﾟをｾｯﾄ */
                I2C_WCNT = 0;
                i2c_state = SBI_GetI2CState(TSB_SBI0);
                
                while (i2c_state.Bit.BusState) {
                    /*I2Cバスの状態確認(利用可になるまで繰り返す)*/
                    count01 = 0;/* クリア */
                    while(count01 < 5);/* 5ms待つ */
                    
                    TSB_WD->CR = 0x0000004E;/* 内蔵WDTのクリアコードを書き込む */
                    WDTCLR();
                    SBI_GenerateI2CStop(TSB_SBI0);/* I2C通信のｽﾄｯﾌﾟをｾｯﾄ */
                    i2c_state = SBI_GetI2CState(TSB_SBI0);
                    
                }
                
            }
            
        }
        
        NVIC_DisableIRQ(INTTB00_IRQn);/* タイマ割り込み不許可 */
        NVIC_DisableIRQ(INTSBI0_IRQn);/* I2C通信割り込み不許可 */
        
        count01 = 0;/* カウントクリア */
        kidou = 1;/* 起動完了ｾｯﾄ */
        D_INISQ = 1;/* 初回ﾃﾞｨｯﾌﾟｽｲｯﾁ取り込み、NV-RAM取り込み、RTC設定・時刻、DAｺﾝﾊﾞｰﾀ設定書込み完了ｾｯﾄ */
        INI_STT = 0;/* サブシーケンスクリア */
        
    } else {
        /* ～～～～～初期化完了後の処理～～～～～ */
        /* 共通処理 */
        if ((SYSFLG & 0x5E) == 0x40) {
            /* 運転待ち状態である場合 */
            if (RWTMS >= 240) {
                /* 60秒経過している場合 */
                RWTMS -= 240;/* 60秒を減算 */
                RWAITT++;/* 分をインクリメント */
                
                if (RWAITT >= 240) {
                    /* 4時間経過した場合 */
                    RWAITT = 0;/* 分をクリア */
                    SYSFLG &= 0xA1;/* パワーオフにする */
                    
                }
                
            }
            
        } else {
            /* 運転待ち状態ではない場合 */
            RWTMS = 0;
            RWAITT = 0;
            
        }
        
        SBI_I2CState i2c_state;/* I2Cバス状態を見る構造体に名前を付ける */
        
        STSFL0 |= 0x02;/* 初期化完了セット */
        
        if (D_INISQ == 1) {
            /* ～～～～～RTC読み出し処理～～～～～ */
            /* ここから分岐 */
            if (INI_STT == 0) {
                /* 初回 */
                I2C_STATE = 3;/* I2C割り込みステートの変更 */
                
                /* RTCの時刻読み込み処理 */
                T0BF0 = 0;/* 初期化アプリ作業領域のクリア */
                I2C_WCNT = 0;/* 初回書込みカウントを0リセット */
                I2C_RCNT = 0;/* 初回読込みカウントを0リセット */
                
                /* I2Cバス状態の確認を行う */
                for (uint8_t i = 0;i < 200;i++) {
                    i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
                    __NOP();
                    RTC_ERR |= 0x80;
                    
                    if (!i2c_state.Bit.BusState) {
                        /* 通信していない場合 */
                        RTC_ERR &= 0x7F;/* RTC異常クリア */
                        SBI_SetSendData(TSB_SBI0, ((RTC_ADDR) | 0x00));/* ダミー書込み */
                        SBI_GenerateI2CStart(TSB_SBI0);/* I2Cスタートさせる。送信終了後、割りこみ発生 */
                        
                        break;
                    } else if (i == 199) {
                        /* 最後までバスフリーを確認できなかった場合 */
                        I2C_RSTE();/* バスチェック・クリア */
                    }
                }
                
                INI_STT++;/* 次回の処理位置インクリメント */
                
                OS_LoadTime->NEXT_LOAD_Time[0] = 5;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[0] = 0xA0;/* 遅延フラグ建築 */
                
            } else if (INI_STT == 1) {
                /* ダミーライト終了確認 */
                if (I2C_WCNT > 1) {
                    /* ダミーライト待ち終わった際 */
                    for (uint8_t i = 0;i < 200;i++) {
                        i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態の取得 */
                        __NOP();
                        RTC_ERR |= 0x80;
                        if (!i2c_state.Bit.BusState) {
                            /* I2Cバスの状態確認(バスが空きであれば) */
                            RTC_ERR &= 0x7F;/* RTC異常クリア */
                            SBI_SetSendData(TSB_SBI0, (RTC_ADDR | 0x01));/* スタート後、スレーブアドレスを送る。続いて読込み */
                            SBI_GenerateI2CStart(TSB_SBI0);/* I2Cスタート状態にさせる。送信終了後、割りこみ発生 */
                            break;
                            
                        }
                        
                    }
                    
                } else if (NACK5 > 5) {
                    /* NACKが5回に達した場合 */
                    SBI_GenerateI2CStop(TSB_SBI0);/* I2Cを停止させる */
                    RTC_ERR |= 0x80;/* RTC異常ｾｯﾄ */
                    
                } else {
                    /* 何もせず待つ */
                    SBI_GenerateI2CStop(TSB_SBI0);/* I2Cを停止させる */
                    RTC_ERR |= 0x80;/* RTC異常ｾｯﾄ */
                    
                }
                
                INI_STT++;/* 次回処理位置インクリメント */
                
                OS_LoadTime->NEXT_LOAD_Time[0] = 10;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[0] = 0xA0;/* 遅延フラグ建築 */
                
            } else if (INI_STT == 2) {
                /* 読み込み待ち状態 */
                if (I2C_RCNT >= 9) {
                    /* 読込み回数が読込み予定データ数を超えた場合 */
                    RTC_ERR &= 0x7F;/* 受信できたので一旦RTC異常をクリアする */
                    
                    SBI_SetI2CACK(TSB_SBI0, ENABLE);/* 読込みの最後でACKを無効化しているため、ここで有効にする */
                    REG_00 = (RTCDT[0] & 0x0F);
                    REG_01 = (RTCDT[0] >> 4) * 10;
                    RTCDT[0] = REG_00 + REG_01;/* 秒　BCD→バイナリ変換 */
                    REG_00 = (RTCDT[1] & 0x0F);
                    REG_01 = ((RTCDT[1] & 0xF0) >> 4) * 10;
                    RTCDT[1] = REG_00 + REG_01;/* 分　BCD→バイナリ変換 */
                    REG_00 = (RTCDT[2] & 0x0F);
                    REG_01 = ((RTCDT[2] & 0xF0) >> 4) * 10;
                    RTCDT[2] = REG_00 + REG_01;/* 時　BCD→バイナリ変換 */
                    REG_00 = (RTCDT[4] & 0x0F);
                    REG_01 = ((RTCDT[4] & 0xF0) >> 4) * 10;
                    RTCDT[4] = REG_00 + REG_01;/* 日　BCD→バイナリ変換 */
                    REG_00 = (RTCDT[5] & 0x0F);
                    REG_01 = ((RTCDT[5] & 0xF0) >> 4) * 10;
                    RTCDT[5] = REG_00 + REG_01;/* 月　BCD→バイナリ変換 */
                    REG_00 = (RTCDT[6] & 0x0F);
                    REG_01 = ((RTCDT[6] & 0xF0) >> 4) * 10;
                    RTCDT[6] = REG_00 + REG_01;/* 年　BCD→バイナリ変換 */
                    
                    RTC_VAL_CHK();/* RTCの値チェック実施 */
                    
                    if ((RTC_ERR & 0x02) > 0) {
                        /* データに異常があった場合 */
                        RTC_ECT++;/* カウントをインクリメント */
                        if (RTC_ECT >= 5) {
                            /* カウントが5以上になった場合 */
                            RTC_ERR |= 0x80;/* 時計IC自体だめフラグ */
                            RTC_ECT = 0;/* クリア */
                            
                        }
                        
                    } else {
                        /* データに異常がない場合 */
                        RTC_ECT = 0;/* クリア */
                        RTC_ERR &= 0x7F;/* 時計IC問題なし */
                    }
                    
                    /* 年月日から曜日を算出する */
                    RTCDT[3] = GETWEEKDAY(RTCDT[6], RTCDT[5], RTCDT[4]);/* 曜日セット */
                    
                    
                    
                } else if (NACK5 >= 5) {
                    /* NACK回数が一定数を超えた場合 */
                    SBI_SetI2CACK(TSB_SBI0, ENABLE);/* 読込みの最後でACKを無効化しているため、ここで有効にする */
                    RTC_ERR |= 0x80;
                    
                } else {
                    /* それ以外の場合 */
                    SBI_SetI2CACK(TSB_SBI0, ENABLE);/* 読込みの最後でACKを無効化しているため、ここで有効にする */
                    RTC_ECT++;/* エラーカウントをインクリメント */
                    if (RTC_ECT >= 5) {
                        /* 5回以上失敗した場合 */
                        RTC_ECT = 0;/* 失敗回数クリア */
                        RTC_ERR |= 0x80;
                        
                    }
                    
                }
                
                /* 終了処理 */
                SBI_GenerateI2CStop(TSB_SBI0);/* I2Cを停止させる */
                INI_STT = 0;/* 次回処理位置を変更 */
                I2C_RCNT = I2C_WCNT = 0;/* 値をクリアする */
                T0BF0 = T0BF1 = 0;/* 予定数以上の受信をした事を検知したフラグをクリアする */
                
                if ((WR_RTC & 0x01) == 0x01) {
                    /* RTCの書き換えが発生した場合 */
                    D_INISQ = 4;/* RTC書き込みに切り替える */
                    
                } else {
                    /* RTCの書き換えが発生していない場合 */
                    D_INISQ = 2;/* NV-RAM書き込みに切り替える */
                    
                }
                
                OS_LoadTime->NEXT_LOAD_Time[0] = 15;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[0] = 0xA0;/* 遅延フラグ建築 */
                
            }
            
            if (RTC_ERR > 0) {
                /* 一度でもバスフリーにならなかった、もしくは日付に異常が見られた場合 */
                if (((ABNF & 0x0F) == 0)
                || (((SYSFLG & 0x02) > 0) && ((ABNF & 0x0F) == 8))
                || (((SYSFLG & 0x02) > 0) && ((ABNF & 0x0F) == 9))
                || (((SYSFLG & 0x02) > 0) && ((ABNF & 0x0F) == 14))) {
                    /* 異常になってない場合、又は運転中で常温水位電極棒異常である
                    又は運転中で常温温度センサー異常である、又は真空度異常である場合 */
                    ABNF &= 0x60;/* MASK */
                    ABNF |= 0x8C;/* RTCエラーセット、ブザーオン */
                    SYSFLG |= 0x40;/* パワーオンフラグセット */
                    
                }
                
                RTC_ERR = 0;
            }
            
        } else if (D_INISQ == 2) {
            /* ～～～～～NV-RAM書き込み処理～～～～～ */
            STSFL0 ^= 0x01;/* XOR　0ビット目 */
            if ((STSFL0 & 0x01) == 0) {
                /* NV-RAM書き込みタイミングである場合 */
                MOMCHK();/* メモリー検査 */
                KEEP();/* NV-RAMへバックアップ処理 */
                
            }
            
            D_INISQ = 3;/* DAC書き込みに切り替える */
            INI_STT = 0;/* 初期値にする */
            
            OS_LoadTime->NEXT_LOAD_Time[0] = 30;/* 次回呼び出し時間 */
            OS_LoadTime->TSK_COND[0] = 0xA0;/* 遅延フラグ建築 */
            
        } else if (D_INISQ == 3) {
            /* ～～～～～DAC書き込み処理～～～～～ */
            if (INI_STT == 0) {
                /* 送信ﾀｲﾐﾝｸﾞである場合 */
                I2C_STATE = 5;/* I2C割り込みステートの変更 */
                
                T0BF0 = 0;/* 初期化アプリ作業領域のクリア */
                I2C_WCNT = 0;/* 初回書込みカウントを0リセット */
                I2C_RCNT = 0;/* 初回読込みカウントを0リセット */
                
                i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
                
                for (uint8_t i = 0;i < 200;i++) {
                    /* 200回ループ */
                    if (!i2c_state.Bit.BusState) {
                        /* I2Cバスがバスフリーである場合 */
                        REG_00 = DACOUT;/* DAC出力用変数値をセットする */
                        
                        I2CBUF[0] = (DACN_ADDR | 0x00);/* DACｱﾄﾞﾚｽ(write)ｾｯﾄ */
                        I2CBUF[1] = 0x30;/* DAC_Aのレジスタに書き込む命令 */
                        I2CBUF[2] = (uint8_t)((0x0FF0 & REG_00) >> 4);/* DACに送る上位8ビットｾｯﾄ */
                        I2CBUF[3] = (uint8_t)((0x000F & REG_00) << 4);/* DACに送る下位4ビットｾｯﾄ */
                        
                        I2C_WCNT = 1;/* 送信前にカウントを加算する */
                        SBI_SetSendData(TSB_SBI0, I2CBUF[0]);/* スタート後、DACアドレスを送る。書込み */
                        SBI_GenerateI2CStart(TSB_SBI0);/* I2Cスタート状態にさせる。送信終了後、割りこみ発生 */
                        
                        break;/* ループを抜けます */
                        
                    } else {
                        /* I2Cバスがバスビジーである場合 */
                        SBI_GenerateI2CStop(TSB_SBI0);/* I2C通信のｽﾄｯﾌﾟをｾｯﾄ */
                        __NOP();
                        __NOP();
                        i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
                        
                    }
                    
                }
                
                INI_STT = 1;/* 処理シーケンスを次のシーケンスにする */
                
                OS_LoadTime->NEXT_LOAD_Time[0] = 15;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[0] = 0xA0;/* 遅延フラグ建築 */
                
            } else {
                /* 全送信完了確認タイミングである場合 */
                if (I2C_WCNT > 3) {
                    /* DAコンバータ書込みの全てのﾃﾞｰﾀが送信完了した場合 */
                    I2CCNT = 0;/* クリア */
                    
                } else {
                    /* データが送信出来ていない場合 */
                    I2CCNT++;/* 待ち時間ｲﾝｸﾘﾒﾝﾄ */
                    if (I2CCNT >= 5) {
                        /* 5回以上待ちぼうけの場合 */
                        I2CCNT = 5;/* 5でFIX */
                        I2C_RSTE();/* F-RAMリセット処理 */
                        
                    }
                    
                }
                
                SBI_GenerateI2CStop(TSB_SBI0);/* I2C通信のｽﾄｯﾌﾟをｾｯﾄ */
                I2C_WCNT = 0;/* 書込み回数クリア */
                I2C_STATE = 3;/* 次回割り込みステート変更 */
                INI_STT = 0;/* 初期値 */
                D_INISQ = 1;/* 次はRTC時刻読み込み */
                
                OS_LoadTime->NEXT_LOAD_Time[0] = 25;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[0] = 0xA0;/* 遅延フラグ建築 */
                
            }
            
        } else if (D_INISQ == 4) {
            /* RTC書き込み処理 */
            if (INI_STT == 0) {
                /* 初回は書き込み処理 */
                I2C_STATE = 6;/* 割り込みステートの変更 */
                
                /* RTCの時刻読み込み処理 */
                T0BF0 = 0;/* 初期化アプリ作業領域のクリア */
                T0BF2 = 0;
                I2C_WCNT = 0;/* 書込みカウントを0リセット */
                I2C_RCNT = 0;/* 読込みカウントを0リセット */
                
                /* I2Cバス状態の確認を行う */
                for (uint8_t i = 0;i < 200;i++) {
                    i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2Cバス状態取得 */
                    __NOP();/* NOP */
                    SBI_GenerateI2CStop(TSB_SBI0);/* I2Cを停止させる */
                    if (!i2c_state.Bit.BusState) {
                        /* I2Cバスが空きである場合 */
                        SBI_SetSendData(TSB_SBI0, ((RTC_ADDR) | 0x00));/* 書込みｾｯﾄ */
                        SBI_GenerateI2CStart(TSB_SBI0);/* I2Cスタートさせる。送信終了後、割りこみ発生 */
                        INI_STT++;/* 次回の処理位置インクリメント */
                        break;
                        
                    } else if (i == 199) {
                        /* 最後までバスフリーを検出できなかった場合 */
                        D_INISQ = 1;/* 時刻読み込み処理へ */
                        I2C_RSTE();/* バスチェック・クリア */
                    }
                    
                }
                
                OS_LoadTime->NEXT_LOAD_Time[0] = 15;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[0] = 0xA0;/* 遅延フラグ建築 */
                
            } else if (INI_STT == 1) {
                /* 書き込み待ち */
                if (I2C_WCNT > 6) {
                    /* 書き込み待ち終わった際 */
                    RTC_ERR &= 0x7F;/* エラーフラグをクリアする */
                    
                } else if (NACK5 >= 5) {
                    /* NACKが5回に達した場合 */
                    RTC_ERR |= 0x80;/* 時計IC自体だめフラグ */
                    SBI_GenerateI2CStop(TSB_SBI0);/* I2Cを停止させる */
                    
                } else {
                    /* 何もせず待つ */
                    RTC_ERR |= 0x80;/* 時計IC自体だめフラグ */
                    
                }
                
                /* 終了処理 */
                SBI_GenerateI2CStop(TSB_SBI0);/* I2Cを停止させる */
                INI_STT  = 0;/* 次回処理位置を変更 */
                I2C_WCNT = 0;/* 書き込み回数クリア */
                D_INISQ  = 2;/* NV-RAM書き込みに切り替える */
                WR_RTC  &= 0xFE;/* 時計書込み終了 */
                
                for (uint8_t i = 0;i < 7;i++) {
                    SETST_BUF[i] = 0;
                }
                
                OS_LoadTime->NEXT_LOAD_Time[0] = 15;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[0] = 0xA0;/* 遅延フラグ建築 */
                
            }
            
        }
        
        /* 初期化完了後の処理ここまで */
    }
    
    if (OS_LoadTime->NEXT_LOAD_Time[0] == 0) {
        /* 入力されていない場合 */
        OS_LoadTime->NEXT_LOAD_Time[0] = 20;/* 次回呼び出し時間 */
        OS_LoadTime->TSK_COND[0] = 0xA0;/* 遅延フラグ建築 */
        
    }
    
    return;
}

/**
 *題名：I2Cバスのリセットルーチン
 *作成者：浅尾　享一
 *説明：I2CバスのSDAラインがLow張り付きした場合、強制リセットを行う。
**/
void I2C_RSTE(void)
{
    
    if (TSB_PC_DATA_PC0 == 0) {
        /* PC0がLow張り付きであった場合 */
        /* SCLラインをカチカチする */
        for (uint8_t i=0;i<=9;i++) {
            TSB_PC_DATA_PC1 = 1;
            TSB_PC->CR &= 0xFE;/* PC0をHI-Zにする */
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
            TSB_PC->CR |= 0x01;
            TSB_PC_DATA_PC0 = 0;
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
            TSB_PC_DATA_PC1 = 0;
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
            TSB_PC->CR &= 0xFE;/* PG0をHI-Zにする */
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
            TSB_PC_DATA_PC1 = 1;
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
        }
        TSB_PC->CR |= 0x01;
    }
    
    return;
}

/**
 *題名：NV-RAMのデータ取得ルーチン
 *作成者：浅尾　享一
 *説明：初回、NV-RAMからデータを取得する
**/
void SETUP(void)
{
    for (uint8_t i = 0;i < 2;i++) {
        /* 2回確認を行います */
        ECMD(0x85);/* リコールコマンド(NV-RAMの内蔵E2PROMから内蔵RAMに読み出す処理) */
        
        TSB_PE_DATA_PE4 = 0;/* NV-RAM チップノットセレクト */
        TSB_PD->DATA &= 0xCF;/* DIとSKをクリア */
        
        count01 = 0;/* タイマクリア */
        while(count01 < 1);/* 1ms待ちます */
        
        ERD();/* 1度読み捨て */
        ERD();/* 2回目を採用 */
        
        if ((PASS1 == 0x01) && (PASS2 == 0xAA)) {
            /* パスワードOKである場合 */
            break;/* 抜けます */
            
        } else {
            /* パスワードが見つからない場合 */
            if (i == 1) {
                /* 2回目であった場合 */
                BLCLR();/* 初期値セットする */
                ABNF = 0x0D;/* NV-RAM異常 */
                ABNF |= 0x80;/* ブザーフラグセット */
                SYSFLG |= 0x40;/* 電源ONフラグセット */
                
            }
            
        }
    }
    
    return;/* 戻ります */
    
}

/**
 *題名：NV-RAM保存するメモリー状態の確認
 *作成者：浅尾　享一
 *説明：保存メモリーのチェックをしておかしければ初期値にする
**/
void MOMCHK(void)
{
    if ((PASS1 == 0x01) && (PASS2 == 0xAA)) {
        /* パスワードが設定されている場合 */
        /* 何もしない */
        
    } else {
        /* パスワードが設定されていない場合 */
        BLCLR();/* 初期値にする */
        ABNF = 0x0D;
        
    }
    
    return;/* 戻る */
    
}

/**
 *題名：NV-RAMへデータ保存するルーチン
 *作成者：浅尾　享一
 *説明：NV-RAMへデータを書き込み・保存する処理
**/
void KEEP(void)
{
    uint16_t REG_00 = 0;/* 一時バッファエリア */
    
    ECMD(0x84);/* 書き込み許可コマンドを送信 */
    
    TSB_PE_DATA_PE4 = 0;/* NV-RAM チップノットセレクト */
    TSB_PD->DATA &= 0xCF;/* DIとSKをクリア */
    
    for (uint8_t i = 0;i < 16;i++) {
        /* 16回繰り返し（16ワード分） */
        ECMD((0x83 | (i << 3)));/* NV-RAM書き込みコマンド */
        TSB_PE_DATA_PE4 = 1;/* NV-RAM チップセレクト */
        
        REG_00 = 0;/* 一旦クリア */
        REG_00 = (uint16_t)*HENNP[i * 2];/* セット */
        REG_00 |= ((uint16_t)*HENNP[(i * 2) + 1] << 8);/* セット */
        
        for (uint8_t j = 0;j < 16;j++) {
            /* 16bit分繰り返し確認 */
            if ((REG_00 & 0x0001) > 0) {
                /* NV-RAMへ出力するデータがHighである場合 */
                TSB_PD_DATA_PD5 = 1;/* セット */
                
            } else {
                /* NV-RAMへ出力するデータがLowである場合 */
                TSB_PD_DATA_PD5 = 0;/* セット */
                
            }
            REG_00 >>= 1;/* 1ビット右シフト */
            
            /* クロックデータを出力 */
            TSB_PD_DATA_PD4 = 1;/* SKをHigh */
            for (uint8_t k = 0;k < 4;k++) {
                /* 少しだけ待ちます */
                __NOP();/* 待つ */
            }
            TSB_PD_DATA_PD4 = 0;/* SKをLow */
            __NOP();/* 待つ */
            
        }
        
        TSB_PE_DATA_PE4 = 0;/* NV-RAM チップノットセレクト */
        TSB_PD->DATA &= 0xCF;/* DIとSKをクリア */
        __NOP();/* 待つ */
        
    }
    
    return;
}

/**
 *題名：NV-RAMのコマンド書き込みルーチン
 *作成者：浅尾　享一
 *説明：初回、NV-RAMへコマンドを送信する
**/
void ECMD(uint8_t CoMD)
{
    TSB_PE_DATA_PE4 = 0;/* NV-RAM チップノットセレクト */
    TSB_PD->DATA &= 0xCF;/* DIとSKをクリア */
    
    TSB_PE_DATA_PE4 = 1;/* NV-RAM チップセレクト */
    
    for (uint8_t i = 0;i < 8;i++) {
        /* 8回ループする */
        /* まずはコマンドビットを出力する */
        if ((CoMD & 0x80) == 0) {
            /* コマンドの最上位ビットが0の場合 */
            TSB_PD_DATA_PD5 = 0;/* Low */
            
        } else {
            /* コマンドの最上位ビットが1の場合 */
            TSB_PD_DATA_PD5 = 1;/* High */
            
        }
        CoMD <<= 1;/* 1ﾋﾞｯﾄ左シフト */
        
        /* クロックデータを出力 */
        TSB_PD_DATA_PD4 = 1;/* SKをHigh */
        for (uint8_t j = 0;j < 4;j++) {
            /* 少しだけ待ちます */
            __NOP();/* 待つ */
        }
        TSB_PD_DATA_PD4 = 0;/* SKをLow */
        
    }
    
    return;/* 戻る */
}

/**
 *題名：NV-RAMからデータ読み出しルーチン
 *作成者：浅尾　享一
 *説明：初回、NV-RAMからデータを取り込む処理
**/
void ERD(void)
{
    uint16_t REG_00 = 0;/* ここでしか使わないもの定義 */
    
    TSB_PE_DATA_PE4 = 0;/* NV-RAM チップノットセレクト */
    TSB_PD->DATA &= 0xCF;/* DIとSKをクリア */
    
    for (uint8_t i = 0;i < 16;i++) {
        /* 16回繰り返し（16ワード分） */
        ECMD((0x86 | (i << 3)));/* NV-RAM読み出しコマンド */
        TSB_PE_DATA_PE4 = 1;/* NV-RAM チップセレクト */
        
        for (uint8_t j = 0;j < 16;j++) {
            /* 16bit分繰り返し確認 */
            REG_00 >>= 1;/* 1ビット右シフト */
            if (TSB_PD_DATA_PD6 == 1) {
                /* NV-RAMの出力データがHighである場合 */
                REG_00 |= 0x8000;/* セット */
                
            }
            
            /* クロックデータを出力 */
            TSB_PD_DATA_PD4 = 1;/* SKをHigh */
            for (uint8_t k = 0;k < 4;k++) {
                /* 少しだけ待ちます */
                __NOP();/* 待つ */
            }
            TSB_PD_DATA_PD4 = 0;/* SKをLow */
            __NOP();/* 待つ */
            
        }
        
        TSB_PE_DATA_PE4 = 0;/* NV-RAM チップノットセレクト */
        /* データを反映する */
        *HENNP[i * 2] = (uint8_t)REG_00;/* 下位セット */
        *HENNP[(i * 2) + 1] = (uint8_t)(REG_00 >> 8);/* 上位セット */
        
    }
    
    return;/* 戻る */
}

/**
 *題名：NV-RAMのデータ初期化ルーチン
 *作成者：浅尾　享一
 *説明：初回、NV-RAMのパスワードデータがおかしかった場合にデータをクリアする処理
**/
void BLCLR(void)
{
    for (uint8_t i = 0;i < 32;i++) {
        /* 32回繰り返し処理 */
        *HENNP[i] = 0;/* データをクリアする */
        
    }
    
    *HENNP[0] = 0x01;/* PASS1 */
    *HENNP[1] = 0xAA;/* PASS2 */
    *HENNP[4] = 0x0A;/* パターン1設定温度　10℃ */
    *HENNP[5] = 0x0A;/* パターン2設定温度　10℃ */
    *HENNP[6] = 0x0A;/* パターン3設定温度　10℃ */
    *HENNP[7] = 0x0A;/* パターン4設定温度　10℃ */
    *HENNP[9] = 20;/* 常温槽設定水温　20℃ */
    *HENNP[20] = 0;/* チラー予約時間（分）　0分 */
    *HENNP[21] = 8;/* チラー予約時間（時）　8時 */
    *HENNP[25] = 5;/* パターン1徐冷温度　5分 */
    *HENNP[26] = 5;/* パターン2徐冷温度　5℃ */
    *HENNP[27] = 5;/* パターン3徐冷温度　5分 */
    *HENNP[28] = 5;/* パターン4徐冷温度　5℃ */
    *HENNP[29] = 0;/* 現在のパターン選択値　0 */
    
    return;
    
}

/**
 *題名：RTCデータの正常性確認
 *作成者：浅尾　享一
 *説明：RTCから読み出した値が正常な値であるか判定する
**/
void RTC_VAL_CHK(void)
{
    if (RTCDT[0] > 59) {
        /* 秒数エラー */
        RTC_ERR |= 0x02;/* エラーセット */
        RTCDT[0] = 0;
    }
    
    if (RTCDT[1] > 59) {
        /* 分数エラー */
        RTC_ERR |= 0x02;/* エラーセット */
        RTCDT[1] = 0;
    }
    
    if (RTCDT[2] > 23) {
        /* 時数エラー */
        RTC_ERR |= 0x02;/* エラーセット */
        RTCDT[2] = 12;
    }
    
    if ((RTCDT[5] > 12) || (RTCDT[5] < 1)) {
        /* 月数エラー */
        RTC_ERR |= 0x02;/* エラーセット */
        RTCDT[5] = 4;
    }
    
    if (RTCDT[6] > 99) {
        /* 年数エラー */
        RTC_ERR |= 0x02;/* エラーセット */
        RTCDT[6] = 22;
    }
    
    if (
    (RTCDT[4] > 31) || (RTCDT[4] < 1)
    || ((RTCDT[4] > 30) && ((RTCDT[5] == 4) || (RTCDT[5] == 6) || (RTCDT[5] == 9) || (RTCDT[5] == 11)))
    || ((RTCDT[4] > (28 + (1 / (RTCDT[6] % 4 + 1)) * (1 - 1 / (RTCDT[6] % 100 + 1)) + (1 / (RTCDT[6] % 400 + 1))))
        && (RTCDT[5] == 2))
    ) {
        /* 日数エラー */
        RTC_ERR |= 0x02;/* エラーセット */
        RTCDT[4] = 1;
    }
    
    return;
}

/*
 年、月、日を指定すると、曜日を返す。(0:日～6:土)
*/
uint8_t GETWEEKDAY(uint8_t nYear, uint8_t nMonth, uint8_t nDay)
{
    int nWeekday, nTmp, Year2;
    
    Year2 = (2000 + nYear);/* 2000年加算 */
    if ((nMonth == 1) || (nMonth == 2)) {
        /* 1月、2月の場合 */
        Year2--;/* 年をﾃﾞｸﾘﾒﾝﾄ */
        nMonth += 12;/* 12ヶ月加算 */
    }
    
    nTmp     = (Year2 / 100);/* 年値を100で割った値をセット */
    nWeekday = ((Year2 + (Year2 >> 2) - nTmp + (nTmp >> 2) + (13 * (int)nMonth + 8) / 5 + (int)nDay) % 7);/* 曜日算出 */
    nWeekday &= 0x00000007;/* 必要な値のみをマスク */
    
    return (uint8_t)nWeekday;/* 1バイトでリターン */
}