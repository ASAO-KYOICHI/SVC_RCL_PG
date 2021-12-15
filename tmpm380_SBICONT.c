/***
 * 題名：I2C通信アプリ
 * 作成者：浅尾　享一
 * I2C通信制御を行うアプリケーション
***/
#include "tmpm380_SBICONT.h"

const uint8_t WRITE01[9] = {
    /* RTCﾚｼﾞｽﾀｱﾄﾞﾚｽ */
    0x17, 0x31, 0x1F, 0x1F, 0x60, 0x66, 0x6B, 0x6B, 0x1E
};
const uint8_t WRITE02[9] = {
    /* RTCﾚｼﾞｽﾀｱﾄﾞﾚｽに書き込む値 */
    0xA8, 0x00, 0x00, 0x80, 0xD3, 0x03, 0x02, 0x01, 0x00
};
const uint8_t WRITE03[7] = {
    /* RTCﾚｼﾞｽﾀｱﾄﾞﾚｽ2 */
    0x17, 0x1D, 0x1E, 0x1F, 0x18, 0x19, 0x1A
};
const uint8_t WRITE04[7] = {
    /* RTCﾚｼﾞｽﾀｱﾄﾞﾚｽに書き込む値2 */
    0xA8, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
};

/***
  * 題名：SBI0(I2C)割り込み
  * 作成者：浅尾　享一
  * I2C通信 送信、受信時に割り込み
 ***/
void INTSBI0_IRQHandler(void)
{
    TSB_SBI_TypeDef *SBIx;
    SBI_I2CState sbi_sr;

    SBIx = TSB_SBI0;
    sbi_sr = SBI_GetI2CState(SBIx);
    
    uint8_t REG_00, REG_01, REG_02 = 0;
    
    if (I2C_STATE == 0) {
        /** 起動時のRTCからデータ読み出しを行う箇所 ---------------------------------**/
        if (sbi_sr.Bit.TRx) {
            /* 送信モードの際(1は送信) */
            if (sbi_sr.Bit.LastRxBit) {
                /* LRB=1:最終「受信」ビットが1(NACKのとき) */
                NACK5++;
                if (NACK5 < 5) {
                    /* NACKが5回未満である場合 */
                    I2C_WCNT = 0;
                    SBI_GenerateI2CStop(SBIx);/* I2Cを停止 */
                    __NOP();
                    SBI_SetSendData(SBIx, (RTC_ADDR | 0x00));/* 再度RTCアドレスを送る */
                    __NOP();
                    SBI_GenerateI2CStart(SBIx); /* I2Cを開始 */
                    
                } else {
                    NACK5 = 5;/* MAX FIX */
                    SBI_GenerateI2CStop(SBIx); /* I2Cを停止 */
                }
            } else {
                /* LRB = 0:最終「受信」ビットが0(ACKのとき) */
                NACK5 = 0;/* NACK回数をリセットする */
                if (I2C_WCNT == 0) {
                    SBI_SetSendData(SBIx, 0x1E);/* RTCアドレスデータを送信する */
                } else {
                    SBI_GenerateI2CStop(SBIx);/* I2Cを停止 */
                }
                I2C_WCNT++;/* 送信したら送信カウントを加算 */
            }
        } else {
            /* 受信モードの際(0は受信) */
            if (I2C_RCNT >= 2) {
                /* 受信回数が一定数を越えた際 */
                T0BF1 = 1;
                REG_00 = SBI_GetReceiveData(SBIx);/* RTCから読み出したデータ抜き出す */
                SBI_GenerateI2CStop(SBIx);
                
            } else {
                /* 受信回数が一定数を越えていない場合 */
                switch (I2C_RCNT) {/*読込み回数*/
                case 0:
                    /* 一度、様子見を行う。ダミーで読み出し処理 */
                    REG_00 = SBI_GetReceiveData(SBIx);/* RTCから読み出したデータ */
                    
                    break;
                    
                case 1:
                    /*受信したデータ 読み込むデータは、フラグレジスタ*/
                    T0BF0 = SBI_GetReceiveData(SBIx);/* RTCから読み出したデータ */
                    T0BF1 = 1;
                    SBI_SetI2CACK(TSB_SBI0, DISABLE);/* 次のデータ受信時はACKしない */
                    
                    break;
                    
                }
            }
            I2C_RCNT++;/* インクリメント */
            
        }
        
    } else if (I2C_STATE == 1) {
        /** 起動時、RTCへデータ書込み処理 -------------------------------------------**/
        if (sbi_sr.Bit.TRx) {
            /* 送信モードの際(1は送信) */
            if (sbi_sr.Bit.LastRxBit) {
                /* LRB=1:最終「受信」ビットが1(NACKのとき) */
                NACK5++;
                if (NACK5 < 5) {
                    I2C_WCNT = 0;
                    SBI_GenerateI2CStop(SBIx); /* I2Cを停止 */
                    __NOP();
                    SBI_SetSendData(SBIx, (RTC_ADDR | 0x00));/* 再度RTCアドレスを送る */
                    __NOP();
                    SBI_GenerateI2CStart(SBIx); /* I2Cを開始 */
                    
                } else {
                    SBI_GenerateI2CStop(SBIx); /* I2Cを停止 */
                }
            } else {
                /* LRB=0:最終「受信」ビットが0(ACKのとき) */
                NACK5 = 0;/* NACK回数をリセットする */
                if (I2C_WCNT <= 1) {
                    /* カウントがまだデータ送付予定回数以下の際 */
                    if (I2C_WCNT == 0) {
                        /* ｱﾄﾞﾚｽ送信時 */
                        if (T0BF2 <= 8) {
                            /* 8以下の場合 */
                            SBI_SetSendData(SBIx, WRITE01[T0BF2]);/* ｱﾄﾞﾚｽﾃﾞｰﾀ */
                        } else {
                            /* 8超過の場合 */
                            SBI_SetSendData(SBIx, WRITE01[8]);/* ｱﾄﾞﾚｽﾃﾞｰﾀ */
                        }
                        
                    } else if (I2C_WCNT == 1) {
                        if (T0BF2 <= 8) {
                            /* 8以下の場合 */
                            SBI_SetSendData(SBIx, WRITE02[T0BF2]);/* ｱﾄﾞﾚｽﾃﾞｰﾀ */
                        } else {
                            /* 8超過の場合 */
                            SBI_SetSendData(SBIx, WRITE02[8]);/* ｱﾄﾞﾚｽﾃﾞｰﾀ */
                        }
                        
                    } else {
                        /* 何もしない */
                    }
                    I2C_WCNT++;/* 送信したら送信カウントを加算 */
                } else {
                    /* 送信カウントが送信予定回数よりも大きくなった際 */
                    SBI_GenerateI2CStop(SBIx);  /* I2Cを停止 */
                    
                }
            }
        }
        
    } else if (I2C_STATE == 2) {
        /** 起動時、RTCへデータ書込み処理その2 --------------------------------------**/
        if (sbi_sr.Bit.TRx) {
            /* 送信モードの際(1は送信) */
            if (sbi_sr.Bit.LastRxBit) {
                /* LRB=1:最終「受信」ビットが1(NACKのとき) */
                NACK5++;
                if (NACK5 < 5) {
                    I2C_WCNT = 0;
                    SBI_GenerateI2CStop(SBIx); /* I2Cを停止 */
                    __NOP();
                    SBI_SetSendData(SBIx, (RTC_ADDR | 0x00));/* 再度RTCアドレスを送る */
                    __NOP();
                    SBI_GenerateI2CStart(SBIx);/* I2Cを開始 */
                } else {
                    SBI_GenerateI2CStop(SBIx); /* I2Cを停止 */
                }
            } else {
                /* LRB=0:最終「受信」ビットが0(ACKのとき) */
                NACK5 = 0;/* NACK回数をリセットする */
                if (I2C_WCNT < 2) {
                    /* カウントがまだデータ送付予定回数以下の際 */
                    if (I2C_WCNT == 0) {
                        /* RTCアドレスデータを送信する */
                        if (T0BF0 <= 6) {
                            /* 6以下の場合 */
                            SBI_SetSendData(SBIx, WRITE03[T0BF0]);/* アドレスデータ送信 */
                        } else {
                            /* 6超の場合 */
                            SBI_SetSendData(SBIx, WRITE03[6]);/* アドレスデータ送信 */
                        }
                    } else if (I2C_WCNT == 1) {
                        /* RTC設定データを送信する */
                        if (T0BF0 <= 6) {
                            /* 6以下の場合 */
                            SBI_SetSendData(SBIx, WRITE04[T0BF0]);/* アドレスデータ送信 */
                        } else {
                            /* 6超の場合 */
                            SBI_SetSendData(SBIx, WRITE04[6]);/* アドレスデータ送信 */
                        }
                    }
                    I2C_WCNT++;/* 送信したら送信カウントを加算 */
                } else {
                    /* 送信カウントが送信予定回数よりも大きくなった際 */
                    SBI_GenerateI2CStop(SBIx);  /* I2Cを停止 */
                }
            }
        } else {
            /* 受信モードの際(0は受信) */
            /* 何もしない */
        }
        
    } else if (I2C_STATE == 3) {
        /** 起動時のRTCから時刻データ読み出しを行う箇所 -----------------------------**/
        if (sbi_sr.Bit.TRx) {
            /* 送信モードの際(1は送信) */
            if (sbi_sr.Bit.LastRxBit) {
                /* LRB=1:最終「受信」ビットが1(NACKのとき) */
                NACK5++;
                if (NACK5 < 5) {
                    SBI_GenerateI2CStop(SBIx); /* I2Cを停止 */
                    __NOP();
                    SBI_SetSendData(SBIx, (RTC_ADDR | 0x00));/* 再度RTCアドレスを送る */
                    __NOP();
                    SBI_GenerateI2CStart(SBIx); /* I2Cを開始 */
                    I2C_WCNT = 0;
                } else {
                    SBI_GenerateI2CStop(SBIx); /* I2Cを停止 */
                }
                
            } else {
                /* LRB=0:最終「受信」ビットが0(ACKのとき) */
                NACK5 = 0;/* NACK回数をリセットする */
                if (I2C_WCNT == 0) {
                    /* カウントがまだデータ送付予定回数以下の際 */
                    SBI_SetSendData(SBIx, 0x10);/* RTCアドレスデータを送信する */
                    I2C_WCNT++;   /* 送信したら送信カウントを加算 */
                } else { /* 送信カウントが送信予定回数よりも大きくなった際 */
                    SBI_GenerateI2CStop(SBIx);  /* I2Cを停止 */
                    I2C_WCNT++;   /* 送信したら送信カウントを加算 */
                }
            }
            
        } else {
            /* 受信モードの際(0は受信) */
            /* 「秒分時週日月年」データ読込み */
            if (I2C_RCNT == 0) {
                RTCD_R[I2C_RCNT] = SBI_GetReceiveData(SBIx);/* RTCから読み出したデータ */
            } else if (I2C_RCNT >= 1) {
                RTCD_R[I2C_RCNT - 1] = SBI_GetReceiveData(SBIx);/* RTCから読み出したデータ */
            }
            
            if (I2C_RCNT == 7) {
                SBI_SetI2CACK(SBIx, DISABLE);/* 次のデータ受信時はACKしない */
            } else if (I2C_RCNT >= 8) {/* 受信回数が一定数を越えた際 */
                SBI_GenerateI2CStop(SBIx);
                T0BF1 = 1;
            }
            
            I2C_RCNT++;
        }
        
    } else if (I2C_STATE == 4) {
        /* DAｺﾝﾊﾞｰﾀを内部ﾘﾌｧﾚﾝｽﾓｰﾄﾞにする(初回のみ)処理 */
        if (sbi_sr.Bit.MasterSlave) {
            /* マスターモードの際(1はマスターモード) */
            if (sbi_sr.Bit.TRx) {
                /* 送信モードの際(1は送信) */
                if (sbi_sr.Bit.LastRxBit) {
                    /* LRB=1:最終「受信」ビットが1(NACKのとき) */
                    NACK5 = 1;
                    SBI_GenerateI2CStop(SBIx);/* I2Cを停止 */
                    I2C_WCNT = 0;/* 送信回数ｸﾘｱ */
                } else {
                    /* LRB=0:最終「受信」ビットが0(ACKのとき) */
                    NACK5 = 0;
                    if (I2C_WCNT <= 3) {
                        /* カウントがまだデータ長以下の際 */
                        SBI_SetSendData(SBIx, I2CBUF[I2C_WCNT]);/* 送信物をｾｯﾄ */
                        I2C_WCNT++;/* 送信したら送信カウントを加算 */
                    } else {
                        /* 送信カウントが送信回数よりも大きくなった際(データ送信を終了) */
                        SBI_GenerateI2CStop(SBIx);/* I2Cを停止 */
                    }
                }
            } else {
                /* 受信モードの際 */
                /* Do nothing */
            }
        } else {
            /* スレーブモードの際 */
            /* Do nothing */
        }
        
    } else if (I2C_STATE == 5) {
        /* DAｺﾝﾊﾞｰﾀにﾃﾞｼﾞﾀﾙ値を書き込む処理 */
        if (sbi_sr.Bit.MasterSlave) {
            /* マスターモードの際(1はマスターモード) */
            if (sbi_sr.Bit.TRx) {
                /* 送信モードの際(1は送信) */
                if (sbi_sr.Bit.LastRxBit) {
                    /* LRB=1:最終「受信」ビットが1(NACKのとき) */
                    NACK5++;
                    SBI_GenerateI2CStop(SBIx);/* I2Cを停止 */
                    I2C_WCNT = 0;/* 送信回数ｸﾘｱ */
                } else {
                    /* LRB=0:最終「受信」ビットが0(ACKのとき) */
                    NACK5 = 0;
                    /* DAC書き込み処理中である場合 */
                    if (I2C_WCNT <= 3) {
                        /* カウントがまだデータ長以下の際 */
                        SBI_SetSendData(SBIx, I2CBUF[I2C_WCNT]);/* 送信物をｾｯﾄ */
                        I2C_WCNT++;/* 送信したら送信カウントを加算 */
                    } else {
                        /* 送信カウントが送信回数よりも大きくなった際(データ送信を終了) */
                        SBI_GenerateI2CStop(SBIx);/* I2Cを停止 */
                    }
                    
                }
            } else {
                /* 受信モードの際 */
                /* Do nothing */
            }
        } else {
            /* スレーブモードの際 */
            /* Do nothing */
        }
        
    } else {
        /** RTCへデータ書込み処理を行う箇所 -----------------------------------------**/
        if (sbi_sr.Bit.TRx) {
            /* 送信モードの際(1は送信) */
            if (sbi_sr.Bit.LastRxBit) {
                /* LRB=1:最終「受信」ビットが1(NACKのとき) */
                NACK5++;/* NACK回数を加算する */
                if (NACK5 < 5) {
                    /* NACK回数が5回未満である場合 */
                    I2C_WCNT = 0;
                    SBI_GenerateI2CStop(SBIx); /* I2Cを停止 */
                    __NOP();
                    SBI_SetSendData(SBIx, (RTC_ADDR | 0x00));/* 再度RTCアドレスを送る */
                    __NOP();
                    SBI_GenerateI2CStart(SBIx); /* I2Cを開始 */
                    
                } else {
                    SBI_GenerateI2CStop(SBIx); /* I2Cを停止 */
                }
                
            } else {
                /* LRB=0:最終「受信」ビットが0(ACKのとき) */
                NACK5 = 0;/* NACK回数をリセットする */
                
                if (I2C_WCNT <= 6) {
                    /* カウントがまだデータ送付予定回数以下の際 */
                    if (I2C_WCNT == 0) {
                        /* 初回の場合 */
                        SBI_SetSendData(SBIx, 0x11);/* RTCアドレスデータを送信する("分"アドレスから) */
                        
                    } else if (I2C_WCNT == 1) {
                        /* 分ﾃﾞｰﾀの場合 */
                        REG_01 = (SETST_BUF[1] / 10);/* 10の位抽出 */
                        REG_02 = (SETST_BUF[1] % 10);/* 1の位抽出 */
                        REG_00 = ((REG_01 << 4) | REG_02);/* RTC向けの日付ﾃﾞｰﾀ作成 */
                        
                        SBI_SetSendData(SBIx, REG_00);/* 送信物をｾｯﾄ */
                    } else if (I2C_WCNT == 2) {
                        /* 時ﾃﾞｰﾀの場合 */
                        REG_01 = (SETST_BUF[2] / 10);/* 10の位抽出 */
                        REG_02 = (SETST_BUF[2] % 10);/* 1の位抽出 */
                        REG_00 = ((REG_01 << 4) | REG_02);/* RTC向けの日付ﾃﾞｰﾀ作成 */
                        
                        SBI_SetSendData(SBIx, REG_00);/* 送信物をｾｯﾄ */
                    } else if (I2C_WCNT == 3) {
                        /* 週ﾃﾞｰﾀの場合 */
                        REG_01 = (SETST_BUF[3] / 10);/* 10の位抽出 */
                        REG_02 = (SETST_BUF[3] % 10);/* 1の位抽出 */
                        REG_00 = ((REG_01 << 4) | REG_02);/* RTC向けの日付ﾃﾞｰﾀ作成 */
                        
                        SBI_SetSendData(SBIx, REG_00);/* 送信物をｾｯﾄ */
                    } else if (I2C_WCNT == 4) {
                        /* 日ﾃﾞｰﾀの場合 */
                        REG_01 = (SETST_BUF[4] / 10);/* 10の位抽出 */
                        REG_02 = (SETST_BUF[4] % 10);/* 1の位抽出 */
                        REG_00 = ((REG_01 << 4) | REG_02);/* RTC向けの日付ﾃﾞｰﾀ作成 */
                        
                        SBI_SetSendData(SBIx, REG_00);/* 送信物をｾｯﾄ */
                    } else if (I2C_WCNT == 5) {
                        /* 月ﾃﾞｰﾀの場合 */
                        REG_01 = (SETST_BUF[5] / 10);/* 10の位抽出 */
                        REG_02 = (SETST_BUF[5] % 10);/* 1の位抽出 */
                        REG_00 = ((REG_01 << 4) | REG_02);/* RTC向けの日付ﾃﾞｰﾀ作成 */
                        
                        SBI_SetSendData(SBIx, REG_00);/* 送信物をｾｯﾄ */
                    } else if (I2C_WCNT == 6) {
                        /* 年ﾃﾞｰﾀの場合 */
                        REG_01 = (SETST_BUF[6] / 10);/* 10の位抽出 */
                        REG_02 = (SETST_BUF[6] % 10);/* 1の位抽出 */
                        REG_00 = ((REG_01 << 4) | REG_02);/* RTC向けの日付ﾃﾞｰﾀ作成 */
                        
                        SBI_SetSendData(SBIx, REG_00);/* 送信物をｾｯﾄ */
                    }
                    
                    I2C_WCNT++;/* 送信したら送信カウントを加算 */
                } else { /* 送信カウントが送信予定回数よりも大きくなった際 */
                    /* 送信回数は7になるとここになります */
                    SBI_GenerateI2CStop(SBIx);  /* I2Cを停止 */
                }
            }
        } else {
            /* 受信モードの際(0は受信) */
            /* 何もしない */
        }
        
    }/* ここまで */
    
    return;/* 戻る */
}