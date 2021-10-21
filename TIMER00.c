/**
 *題名：タイマー割り込み処理
 *作成者：浅尾　享一
 *1msごとに呼び出される処理
**/
#include "TIMER00.h"

void INTTB00_IRQHandler(void)
{
    OS_TSKCon1 *OS_LoadTime;/* OS設定値を読むためのポインタ構造体に名前をつける */
    OS_LoadTime = OSTSKCon1S();/* 関数経由でポインタをOSparamにアクセス */
    
    uint8_t  REG_00 = 0;
    uint16_t REG_01 = 0;
    
    TSB_WD->CR = 0x0000004E;/* 内蔵WDTのクリアコードを書き込む */
    
    /* マトリクスの入力生データを吸い上げる */
    if ((TSB_PA->DATA & 0x06) == 0) {
        /* デコーダ0出力の場合 */
        /* マトリクススイッチの情報を取得 */
        MXSW &= 0xFC;
        MXSW |= (uint8_t)((TSB_PE->DATA & 0xC0) >> 6);/* ↑↓SWの情報を更新 */
        
        /* データバスの情報を取得 */
        MXIN0 &= 0xF0;/* 下位4ビットクリア */
        MXIN0 |= (uint8_t)(TSB_PD->DATA & 0x0F);/* データをセット */
        
    } else if ((TSB_PA->DATA & 0x06) == 2) {
        /* デコーダ1出力の場合 */
        /* マトリクススイッチの情報を取得 */
        MXSW &= 0xF3;
        MXSW |= (uint8_t)((TSB_PE->DATA & 0xC0) >> 4);/* →設定SWの情報を更新 */
        
        /* データバスの情報を取得 */
        MXIN0 &= 0x0F;/* 上位4ビットクリア */
        MXIN0 |= (uint8_t)((TSB_PD->DATA & 0x0F) << 4);/* データをセット */
        
    } else if ((TSB_PA->DATA & 0x06) == 4) {
        /* デコーダ2出力の場合 */
        /* マトリクススイッチの情報を取得 */
        MXSW &= 0xCF;
        MXSW |= (uint8_t)((TSB_PE->DATA & 0xC0) >> 2);/* 電源選択SWの情報を更新 */
        
        /* データバスの情報を取得 */
        MXIN1 &= 0xF0;/* 下位4ビットクリア */
        MXIN1 |= (uint8_t)(TSB_PD->DATA & 0x0F);/* データをセット */
        
    } else if ((TSB_PA->DATA & 0x06) == 6) {
        /* デコーダ3出力の場合 */
        /* マトリクススイッチの情報を取得 */
        MXSW &= 0x3F;
        MXSW |= (uint8_t)(TSB_PE->DATA & 0xC0);/* 運転/停止チラー予約SWの情報を更新 */
        
        /* データバスの情報を取得 */
        MXIN1 &= 0x0F;/* 上位4ビットクリア */
        MXIN1 |= (uint8_t)((TSB_PD->DATA & 0x0F) << 4);/* データをセット */
        
    }
    
    REG_00 = (uint8_t)TSB_PA->DATA;/* データレジスタを読み出す */
    REG_00 &= 0xF9;/* マスク */
    REG_00 |= (uint8_t)(MXTM << 1);/* セット */
    TSB_PA->DATA = (uint32_t)REG_00;/* デコーダ―出力を更新 */
    
    MXOTN5 = (uint8_t)TSB_PA->DATA;/* データを持ってくる */
    MXOTN5 &= 0xE7;/* MASKする */
    
    /* デコーダ―を切り替えた後にLED出力をセットする */
    if (MXTM == 0) {
        /* デコーダ―0の場合 */
        MXOTN5 |= ((MXOTN6 & 0x03) << 3);/* セット */
        if (((MX6BLK & 0x01) > 0) && (MSECD >= 600)) {
            /* 点滅フラグがあり、消灯タイミングである場合 */
            MXOTN5 &= 0xF7;/* クリア */
            
        }
        if (((MX6BLK & 0x02) > 0) && (MSECD >= 600)) {
            /* 点滅フラグがあり、消灯タイミングである場合 */
            MXOTN5 &= 0xEF;/* クリア */
            
        }
        
    } else if (MXTM == 1) {
        /* デコーダ―1の場合 */
        MXOTN5 |= ((MXOTN6 & 0x0C) << 1);/* セット */
        if (((MX6BLK & 0x04) > 0) && (MSECD >= 600)) {
            /* 点滅フラグがあり、消灯タイミングである場合 */
            MXOTN5 &= 0xF7;/* クリア */
            
        }
        if (((MX6BLK & 0x08) > 0) && (MSECD >= 600)) {
            /* 点滅フラグがあり、消灯タイミングである場合 */
            MXOTN5 &= 0xEF;/* クリア */
            
        }
        
    } else if (MXTM == 2) {
        /* デコーダ―2の場合 */
        MXOTN5 |= ((MXOTN6 & 0x30) >> 1);/* セット */
        if (((MX6BLK & 0x10) > 0) && (MSECD >= 600)) {
            /* 点滅フラグがあり、消灯タイミングである場合 */
            MXOTN5 &= 0xF7;/* クリア */
            
        }
        if (((MX6BLK & 0x20) > 0) && (MSECD >= 600)) {
            /* 点滅フラグがあり、消灯タイミングである場合 */
            MXOTN5 &= 0xEF;/* クリア */
            
        }
        
    } else {
        /* デコーダ―3の場合 */
        MXOTN5 |= ((MXOTN6 & 0xC0) >> 3);/* セット */
        if (((MX6BLK & 0x40) > 0) && (MSECD >= 600)) {
            /* 点滅フラグがあり、消灯タイミングである場合 */
            MXOTN5 &= 0xF7;/* クリア */
            
        }
        if (((MX6BLK & 0x80) > 0) && (MSECD >= 600)) {
            /* 点滅フラグがあり、消灯タイミングである場合 */
            MXOTN5 &= 0xEF;/* クリア */
            
        }
        
    }
    
    TSB_PA->DATA = (uint32_t)MXOTN5;/* LED出力更新セット */
    
    MXTM++;/* 次回用にマトリックスカウントをインクリメント */
    MXTM &= 0x03;/* 0～3になる様にマスク */
    
    /* HACCP用接点出力 */
    if (TNO > 0) {
        /* セルフテスト中である場合 */
        /* 出力更新しない */
        
    } else if ((CCSEQ >= 1) && (CCSEQ < 10)) {
        /* 冷却シーケンスが1以上、10未満である場合 */
        TSB_PE_DATA_PE2 = 1;/* HACCP用冷却工程出力をオンする */
        
    } else {
        /* 冷却シーケンスがそれら値でない場合 */
        TSB_PE_DATA_PE2 = 0;/* HACCP用冷却工程出力をクリアする */
        
    }
    
    if (TNO > 0) {
        /* セルフテスト中である場合 */
        /* 出力更新しない */
        
    } else if ((ABNF & 0x1F) > 0) {
        /* 異常がセットされている場合 */
        TSB_PE_DATA_PE3 = 1;/* HACCP用異常出力をオンする */
        
    } else {
        /* 解除されている場合 */
        TSB_PE_DATA_PE3 = 0;/* HACCP用異常出力をクリアする */
        
    }
    
    /* 各タスクの次回呼び出し時間をデクリメントしていく処理 */
    for (uint8_t i = 0; i < TSKNUM; i++) {
        if ( ((OS_LoadTime->TSK_COND[i] & 0x20) == 0x20)
        && (OS_LoadTime->NEXT_LOAD_Time[i] > 0) ) {
            /* 遅延ﾌﾗｸﾞがあり、ﾀｲﾏに数値が入っている場合 */
            OS_LoadTime->NEXT_LOAD_Time[i]--;/* 次回呼び出し時間をデクリメント */
            if (OS_LoadTime->NEXT_LOAD_Time[i] == 0) {
                /* 減算した際に0だった場合 */
                OS_LoadTime->NEXT_LOAD_Time[i] = 0;/* 0になったら以降0固定 */
                OS_LoadTime->TSK_COND[i] &= 0xDF;/* 遅延フラグを消去 */
            }
        } else if ( ((OS_LoadTime->TSK_COND[i] & 0x20) == 0x20)
        && (OS_LoadTime->NEXT_LOAD_Time[i] == 0) ) {
            /* 遅延ﾌﾗｸﾞがあり、ﾀｲﾏが0になっている場合 */
            OS_LoadTime->NEXT_LOAD_Time[i] = 0;/* 0になったら以降0固定 */
            OS_LoadTime->TSK_COND[i] &= 0xDF;/* 遅延フラグを消去 */
        }
    }
    
    /* 初回ディップスイッチ確認動作用 */
    if (kidou == 0) {
        /* 起動フラグ立って居ない場合 */
        if (count01 < 5) {
            count01++;/*5ミリ以内であれば5ミリカウント*/
        }
    } else {
        /* 起動フラグが立っている場合 */
        /* 何もしない */
    }
    
    /* 参照用のタイマー */
    MSECD++;/*ミリセカンドカウントアップ*/
    if (MSECD == 1000) {
        /* == 1000ms */
        MSECD = 0;
        SECND++;/*セカンドカウントアップ*/
        BOOT_SEC++;/* 基板起動してからの時間ｶｳﾝﾄｱｯﾌﾟ */
        
        if (BOOT_SEC >= 5) {
            /* 起動から5秒経過する場合 */
            BOOT_SEC = 5;/* MAX FIX */
        }
        
        if (SECND == 60) {
            /* == 60s */
            SECND = 0;
            
        }
    }
    
    if (BOOT_SEC >= 5) {
        /* 起動から5秒経過した場合 */
        SEC_CNT++;/* インクリメント */
        if (SEC_CNT >= 3000) {
            /* 3秒以上になる場合 */
            SEC_CNT = 3000;/* MAX FIX */
            
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
            
        }
        
        RTCKBF[2] = RTCKBF[1];/* KEEP */
        RTCKBF[1] = RTCKBF[0];/* KEEP */
        RTCKBF[0] = RTCDT[0];/* RENEW */
        
        if ((RTCKBF[0] != RTCKBF[1]) || (RTCKBF[0] != RTCKBF[2])) {
            /* 秒の変化があった場合 */
            SEC_CNT = 0;/* クリア */
            
        }
        
    }
    
    /* ～～AD変換の補助処理～～ */
    if ((AD_FLG01 & 0x04) == 0x00) {
        /* AD変換終了している場合 */
        if (AD_RESLT >= 6) {
            /* 6回分、AD変換完了した場合 */
            for (uint8_t i = 0;i < 3;i++) {
                /* 3回繰り返し処理 */
                if (AD_NOW_CNT[i] >= 10) {
                    /* AD変換回数が10回になった場合 */
                    REG_01 = (uint16_t)(uint32_t)((AD_NOW_SUM[i] - ((uint32_t)AD_NOW_MIN[i] + (uint32_t)AD_NOW_MAX[i])) / 8);/* 平均算出 */
                    AD_NOW_CNT[i] = 0;/* 各種値をクリアする */
                    AD_NOW_MAX[i] = 0;
                    AD_NOW_SUM[i] = 0;
                    AD_NOW_MIN[i] = 4095;
                    
                    if ((AD_FLG02 & (0x01 << i)) == 0) {
                        /* 1秒間のAD変換完了していない場合 */
                        AD_ONE_CNT[i]++;/* 1秒間AD変換回数をインクリメントする */
                        AD_ONE_SUM[i] += REG_01;/* 1秒間の平均用に加算する */
                        if (AD_ONE_MIN[i] > REG_01) {
                            /* 最小値未満になる場合 */
                            AD_ONE_MIN[i] = REG_01;/* 最小値を更新 */
                        }
                        
                        if (AD_ONE_MAX[i] < REG_01) {
                            /* 最大値超になる場合 */
                            AD_ONE_MAX[i] = REG_01;/* 最大値を更新 */
                        }
                    }
                }
                
                if ((AD_ONE_CNT[i] >= 10) && ((AD_FLG02 & (0x01 << i)) == 0)) {
                    /* 1秒間のAD変換回数が10回になった場合 */
                    AD_FLG02 |= (0x01 << i);/* 1秒間のAD変換完了フラグをセット */
                    REG_01 = (uint16_t)(uint32_t)((AD_ONE_SUM[i] - ((uint32_t)AD_ONE_MIN[i] + (uint32_t)AD_ONE_MAX[i])) / 8);/* 平均算出 */
                    AD_ONE_AVR[i] = REG_01;/* 平均値に値をセットする */
                    AD_ONE_MAX[i] = 0;/* 各種値をクリアする */
                    AD_ONE_SUM[i] = 0;
                    AD_ONE_MIN[i] = 4095;
                    
                }
                
            }
            
            AD_MX    = 0;/* クリア */
            AD_RESLT = 0;/* クリア */
            
            /* まずは蒸気圧力から */
            TSB_AD->SSET03 = 0x00000080;/* AD変換するレジスタセット */
            TSB_AD->SSET47 = 0x00000000;
            
            AD_FLG01 |= 0x04;/* AD変換中セット */
            
            ADC_Start(ADC_TRG_SW);/* ソフトウェアAD変換開始 */
            AD_CTM0 = 0;/* 固着タイマクリア */
            
        } else if (AD_RESLT == 0) {
            /* AD変換を開始していない場合 */
            AD_MX    = 0;/* クリア */
            AD_RESLT = 0;/* クリア */
            
            for (uint8_t i = 0;i < 3;i++) {
                /* 6回繰り返し処理 */
                AD_NOW_CNT[i] = 0;/* 各種値をクリアする */
                AD_NOW_MAX[i] = 0;
                AD_NOW_SUM[i] = 0;
                AD_NOW_MIN[i] = 4095;
                
            }
            
            /* まずは蒸気圧力から */
            TSB_AD->SSET03 = 0x00000080;/* AD変換するレジスタセット */
            TSB_AD->SSET47 = 0x00000000;
            
            AD_FLG01 |= 0x04;/* AD変換中セット */
            
            ADC_Start(ADC_TRG_SW);/* ソフトウェアAD変換開始 */
            AD_CTM0 = 0;/* 固着タイマクリア */
            
        } else {
            /* 中途半端に終っている場合 */
            AD_CTM0++;/* 固着タイマｲﾝｸﾘﾒﾝﾄ */
            if (AD_CTM0 >= 10) {
                /* 10ミリセック以上経過した場合 */
                AD_CTM0 = 0;/* 固着タイマをクリアする */
                
                AD_MX    = 0;/* クリア */
                AD_RESLT = 0;/* クリア */
                
                for (uint8_t i = 0;i < 6;i++) {
                    /* 6回繰り返し処理 */
                    AD_NOW_CNT[i] = 0;/* 各種値をクリアする */
                    AD_NOW_MAX[i] = 0;
                    AD_NOW_SUM[i] = 0;
                    AD_NOW_MIN[i] = 4095;
                    
                }
                
                /* まずは蒸気圧力から */
                TSB_AD->SSET03 = 0x00000080;/* AD変換するレジスタセット */
                TSB_AD->SSET47 = 0x00000000;
                
                AD_FLG01 |= 0x04;/* AD変換中セット */
                
                ADC_Start(ADC_TRG_SW);/* ソフトウェアAD変換開始 */
                
            }
        }
        
    } else {
        /* AD変換中の場合 */
        AD_CTM0++;/* 固着タイマｲﾝｸﾘﾒﾝﾄ */
        if (AD_CTM0 >= 10) {
            /* 10ミリセック以上経過した場合 */
            AD_CTM0 = 0;/* 固着タイマをクリアする */
            
            AD_MX    = 0;/* クリア */
            AD_RESLT = 0;/* クリア */
            
            for (uint8_t i = 0;i < 6;i++) {
                /* 6回繰り返し処理 */
                AD_NOW_CNT[i] = 0;/* 各種値をクリアする */
                AD_NOW_MAX[i] = 0;
                AD_NOW_SUM[i] = 0;
                AD_NOW_MIN[i] = 4095;
                
            }
            
            /* まずは蒸気圧力から */
            TSB_AD->SSET03 = 0x00000080;/* AD変換するレジスタセット */
            TSB_AD->SSET47 = 0x00000000;
            
            AD_FLG01 |= 0x04;/* AD変換中セット */
            
            ADC_Start(ADC_TRG_SW);/* ソフトウェアAD変換開始 */
            
        }
        
    }
    /* ～～AD変換の補助処理ここまで～～ */
    
    return;
}
