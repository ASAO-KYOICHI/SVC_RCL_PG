/**
 *題名：入力アプリ
 *作成者：浅尾　享一
 *入力検知処理を行うアプリケーション
**/
#include "tmpm380_INPUT.h"


/* ON/OFF_SW入力類の検知ﾀｲﾏ0 */
const uint8_t INP_K0[8] = {
    4 ,4 ,4 ,4 ,4 ,4 ,4, 4
};
/* ON/OFF_PORT入力類の検知ﾀｲﾏ1 */
const uint8_t INP_K1[8] = {
    20,20,20,20,20,20,20,20
};
/* ON/OFF_PORT入力類の検知ﾀｲﾏ2 */
const uint8_t INP_K2[8] = {
    20,20,20,20,20,20,20,20
};

const uint8_t INPUT_ON00 = 0xFF;/* ONとなった時のSW入力値の像 */
/*　全てHighアクティブ
0…上SW
1…下SW
2…→SW
3…設定SW
4…電源SW
5…選択SW
6…運転SW
7…チラー予約SW
*/
const uint8_t INPUT_ON01 = 0xF0;/* ONとなった時のPORT入力値の像 */
/*　水位はLowアクティブ、それ以外はHighアクティブ
0･･･常温水槽水位L
1･･･常温水槽水位H
2･･･低温水槽水位L
3･･･低温水槽水位H
4･･･常温真空弁閉リミット
5･･･予備
6･･･圧力センサー
7･･･チラー異常信号
*/
const uint8_t INPUT_ON02 = 0xFF;/* ONとなった時のPORT入力値の像 */
/*　全てHighアクティブ
0･･･サーマルトリップ
1･･･空き
2･･･空き
3･･･停電信号
4･･･DSW1
5･･･DSW2
6･･･DSW3
7･･･DSW4
*/

/* ON/OFF_SW入力類の検知ｶｳﾝﾀ0 */
uint8_t PORT_CNT00[8] = {
    0,0,0,0,0,0,0,0
};
/* ON/OFF_PORT入力類の検知ｶｳﾝﾀ1 */
uint8_t PORT_CNT01[8] = {
    0,0,0,0,0,0,0,0
};
/* ON/OFF_PORT入力類の検知ｶｳﾝﾀ2 */
uint8_t PORT_CNT02[8] = {
    0,0,0,0,0,0,0,0
};

void INPUT_CONT(void)
{
    OS_TSKCon1 *OS_LoadTime;/* OS設定値を読むためのポインタ構造体に名前をつける */
    OS_LoadTime = OSTSKCon1S();/* 関数経由でポインタをOSparamにアクセス */
    
    uint8_t REG_00 = 0;/* 一時変数 */
    
    /* 以下、入力アプリが実行する処理 */
    /* チャタリングキャンセル処理 */
    for (uint8_t i = 0; i < 8; i++) {
        /* ﾙｰﾌﾟで各ビットをチェックする */
        REG_00 = MXSW;/* スイッチ入力データをセット */
        if ((REG_00 & (0x01 << i)) == (INPUT_ON00 & (0x01 << i))) {
            /* 入力ありである場合 */
            PORT_CNT00[i]++;
            if (PORT_CNT00[i] >= INP_K0[i]) {
                /* ｶｳﾝﾀが一定数以上UPした場合 */
                PORT_CNT00[i] = INP_K0[i];/* MAX FIX */
            }
        } else {
            /* 入力無しである場合 */
            if (PORT_CNT00[i] > 0) {
                /* ｶｳﾝﾀが一定数以上UPした場合 */
                PORT_CNT00[i]--;
            }
        }
        
        REG_00 = MXIN0;
        if ((REG_00 & (0x01 << i)) == (INPUT_ON01 & (0x01 << i))) {
            /* 入力ありである場合 */
            PORT_CNT01[i]++;
            if (PORT_CNT01[i] >= INP_K1[i]) {
                /* ｶｳﾝﾀが一定数以上UPした場合 */
                PORT_CNT01[i] = INP_K1[i];/* MAX FIX */
            }
        } else {
            /* 入力無しである場合 */
            if (PORT_CNT01[i] > 0) {
                /* ｶｳﾝﾀが一定数以上UPした場合 */
                PORT_CNT01[i]--;
            }
        }
        
        REG_00 = MXIN1;
        if ((REG_00 & (0x01 << i)) == (INPUT_ON02 & (0x01 << i))) {
            /* 入力ありである場合 */
            PORT_CNT02[i]++;
            if (PORT_CNT02[i] >= INP_K2[i]) {
                /* ｶｳﾝﾀが一定数以上UPした場合 */
                PORT_CNT02[i] = INP_K2[i];/* MAX FIX */
            }
        } else {
            /* 入力無しである場合 */
            if (PORT_CNT02[i] > 0) {
                /* ｶｳﾝﾀが一定数以上UPした場合 */
                PORT_CNT02[i]--;
            }
        }
        
        if (PORT_CNT00[i] >= INP_K0[i]) {
            /* ﾀｲﾏがﾁｬﾀﾘﾝｸﾞｷｬﾝｾﾙ時間経過した場合 */
            MXSWO0 |= (0x01 << i);/* ﾋﾞｯﾄｾｯﾄ */
        } else if (PORT_CNT00[i] == 0) {
            /* ﾀｲﾏがﾁｬﾀﾘﾝｸﾞｷｬﾝｾﾙ時間経過していない場合 */
            MXSWO0 &= ~(0x01 << i);/* ﾋﾞｯﾄｸﾘｱ */
        }
        
        if (PORT_CNT01[i] >= INP_K1[i]) {
            /* ﾀｲﾏがﾁｬﾀﾘﾝｸﾞｷｬﾝｾﾙ時間経過した場合 */
            MXINO0 |= (0x01 << i);/* ﾋﾞｯﾄｾｯﾄ */
        } else if (PORT_CNT01[i] == 0) {
            /* ﾀｲﾏがﾁｬﾀﾘﾝｸﾞｷｬﾝｾﾙ時間経過していない場合 */
            MXINO0 &= ~(0x01 << i);/* ﾋﾞｯﾄｸﾘｱ */
        }
        
        if (PORT_CNT02[i] >= INP_K2[i]) {
            /* ﾀｲﾏがﾁｬﾀﾘﾝｸﾞｷｬﾝｾﾙ時間経過した場合 */
            MXINO1 |= (0x01 << i);/* ﾋﾞｯﾄｾｯﾄ */
        } else if (PORT_CNT02[i] == 0) {
            /* ﾀｲﾏがﾁｬﾀﾘﾝｸﾞｷｬﾝｾﾙ時間経過していない場合 */
            MXINO1 &= ~(0x01 << i);/* ﾋﾞｯﾄｸﾘｱ */
        }
        
    }/* ループここまで */
    
    if ((((ABNF & 0x0F) == 0) && ((SYSFLG & 0x40) > 0))
    || (((ABNF & 0x0F) == 8)  && ((SYSFLG & 0x42) == 0x42))
    || (((ABNF & 0x0F) == 9)  && ((SYSFLG & 0x42) == 0x42))
    || (((ABNF & 0x0F) == 14) && ((SYSFLG & 0x42) == 0x42))) {
        /* 電源ONで、異常になってない場合、
        又は電源ON・運転中で常温水位電極棒異常である、
        又は電源ON・運転中で常温温度センサー異常である、
        又は電源ON・運転中で真空度異常である場合 */
        if ((MXINO1 & 0x01) > 0) {
            /* サーマル異常入力がある場合 */
            ABNF &= 0x30;/* MASK */
            ABNF |= 0x81;/* ブザーフラグ、サーマル異常をセット */
            
        } else if ((MXINO0 & 0x80) > 0) {
            /* チラー一括異常入力の場合 */
            if ((SYKND0 & 0x10) > 0) {
                /* 蒸気式の設定の場合 */
                ABNF = 0x82;/* ブザーフラグクーリングタワー異常にする */
                
            } else {
                /* チラー式の設定の場合 */
                if ((ABNF & 0x10) == 0) {
                    /* チラー異常がセットされていない場合 */
                    ABNF |= 0x50;/* チラー一括異常セット（運転継続する扱い） */
                    
                }
                
            }
            
        }
        
    }
    
    OS_LoadTime->NEXT_LOAD_Time[1] = 5;/* 次回呼び出し時間 */
    OS_LoadTime->TSK_COND[1] = 0xA0;/* 遅延フラグ建築 */
    
    return;
}