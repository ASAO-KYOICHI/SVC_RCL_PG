/***
 * 題名：LCD表示制御アプリ
 * 作成者：浅尾　享一
 * LCD/VFD表示の制御を行うアプリケーション
***/
#include "tmpm380_DCONT.h"

const uint8_t NUMDEG[]  = "0123456789";
const uint8_t NUMDEG2[] = " 123456789";

/* セルフテスト用NV-RAMチェック時 */
const uint8_t NVRMITB[] = {
    0x01, 0xAA,/* パスワード */
    0x00,/* SYSFLG */
    0x00,/* ABNF */
    0x0A,/* パターン1設定温度 */
    0x0A,/* パターン2設定温度 */
    0x0A,/* パターン3設定温度 */
    0x0A,/* パターン4設定温度 */
    0x00,/* 冷却パターン */
    20,/* 常温槽設定水温20℃ */
    0x00,/* 異常履歴と異常件数 */
    0x00,/* 異常履歴 */
    0x00,/* 異常履歴 */
    0x00,/* 冷却開始時刻（分） */
    0x00,/* 冷却開始時刻（時） */
    0x00,/* 冷却終了時刻（分） */
    0x00,/* 冷却終了時刻（時） */
    0x00,/* 積算稼働時間（分） */
    0x00,/* 積算稼働時間（時）下位 */
    0x00,/* 積算稼働時間（時）上位 */
    0x00,/* チラー予約時間（分） */
    0x08,/* チラー予約時間（時） */
    0x00,/* 冷却工程時間（秒） */
    0x00,/* 冷却工程時間（分） */
    0x00,/* 冷却工程番号 */
    0x05,/* パターン1徐冷時間 */
    0x05,/* パターン2徐冷時間 */
    0x05,/* パターン3徐冷時間 */
    0x05,/* パターン4徐冷時間 */
    0x00,/* 現在のパターン選択値 */
    0x00/*  運転待機時間（分） */
};

void DCONT(void)
{
    OS_TSKCon1 *OS_LoadTime;/* OS設定値を読むためのポインタ構造体に名前をつける */
    OS_LoadTime = OSTSKCon1S();/* 関数経由でポインタをOSparamにアクセス */
    
    uint8_t  REG_00, REG_01 = 0;/* 一時変数セット */
    uint32_t REG_02 = 0;
    
    for (uint8_t i = 0;i < 2;i++) {
        /* わざと繰り返し構文を設置する（continue;で戻る様にする為） */
        i = 1;/* 次回、必ずループから抜けられるように値をセット */
        
        switch (DLWSQ) {
        case 0:
            /* 初回である場合 */
            DLWSQ = 1;/* 次回スタート位置を1にする */
            OS_LoadTime->NEXT_LOAD_Time[5] = 100;/* 次回呼び出し時間 */
            OS_LoadTime->TSK_COND[5] = 0xA0;/* 遅延フラグ建築 */
                
            return;/* 戻る */
            break;
            
        case 1:
            /* 2回目である場合 */
            TSB_PA->DATA &= 0x1F;/* LCD,VFDのE/RW/RSをオフする */
            TSB_PF->IE &= 0xF0;/* LCDデータバスを非入力にする */
            TSB_PF->CR |= 0x0F;/* LCDデータバスを出力にする */
            TSB_PF->DATA &= 0xF0;/* 一旦マスク */
            TSB_PF->DATA |= 0x03;/* LCDデータバス出力をセット、初期化（4ビットバス設定）コマンド */
            
            EPLS();/* Eをパルス出力する */
            DLWSQ++;/* 次回スタート位置をインクリメント */
            
            OS_LoadTime->NEXT_LOAD_Time[5] = 5;/* 次回呼び出し時間 */
            OS_LoadTime->TSK_COND[5] = 0xA0;/* 遅延フラグ建築 */
                
            return;/* 戻る */
            break;
            
        case 2:
        case 3:
            /* 3,4回目である場合 */
            EPLS();/* Eをパルス出力する */
            DLWSQ++;/* 次回スタート位置をインクリメント */
            
            OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* 次回呼び出し時間 */
            OS_LoadTime->TSK_COND[5] = 0xA0;/* 遅延フラグ建築 */
                
            return;/* 戻る */
            break;
            
        case 4:
            /* 5回目である場合 */
            TSB_PF->CR &= 0xF0;/* 出力機能をオフ */
            TSB_PF->IE |= 0x0F;/* 入力機能をオン */
            TSB_PA_DATA_PA6 = 1;/* RWをオンする */
            
            if ((EPBSY() & 0x08) > 0) {
                /* ビジーフラグがオンである場合 */
                OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[5] = 0xA0;/* 遅延フラグ建築 */
                
                FLZFG |= 0x01;/* フラグセット */
                if (FLZTM >= 60000) {
                    /* 1分以上経過した場合 */
                    FLZFG = DLWSQ = 0;/* やり直し */
                    FLZTM = 0;/* タイマクリア */
                    
                }
                
                return;/* 戻る */
                
            }
            
            FLZFG = 0;/* クリア */
            DLWSQ++;/* 次回スタート位置をインクリメント */
            
            TSB_PA_DATA_PA6 = 0;/* RWをオフする */
            TSB_PF->IE &= 0xF0;/* LCDデータバスを非入力にする */
            TSB_PF->CR |= 0x0F;/* LCDデータバスを出力にする */
            TSB_PF->DATA &= 0xF0;/* 一旦マスク */
            TSB_PF->DATA |= 0x02;/* LCDデータバス出力をセット、2行コマンド */
            
            EPLS();/* Eをパルス出力する */
            
            OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* 次回呼び出し時間 */
            OS_LoadTime->TSK_COND[5] = 0xA0;/* 遅延フラグ建築 */
                
            return;/* 戻る */
            break;
            
        case 5:
            /* 6回目である場合 */
            TSB_PF->CR &= 0xF0;/* 出力機能をオフ */
            TSB_PF->IE |= 0x0F;/* 入力機能をオン */
            TSB_PA_DATA_PA6 = 1;/* RWをオンする */
            
            if ((EPBSY() & 0x08) > 0) {
                /* ビジーフラグがオンである場合 */
                OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[5] = 0xA0;/* 遅延フラグ建築 */
                
                FLZFG |= 0x01;/* フラグセット */
                if (FLZTM >= 60000) {
                    /* 1分以上経過した場合 */
                    FLZFG = DLWSQ = 0;/* やり直し */
                    FLZTM = 0;/* タイマクリア */
                    
                }
                
                return;/* 戻る */
                
            }
            
            FLZFG = 0;/* クリア */
            DLWSQ++;/* 次回スタート位置をインクリメント */
            
            TSB_PA_DATA_PA6 = 0;/* RWをオフする */
            TSB_PF->IE &= 0xF0;/* LCDデータバスを非入力にする */
            TSB_PF->CR |= 0x0F;/* LCDデータバスを出力にする */
            TSB_PF->DATA &= 0xF0;/* 一旦マスク */
            TSB_PF->DATA |= 0x02;/* LCDデータバス出力をセット、？コマンド */
            
            EPLS();/* Eをパルス出力する */
            
            TSB_PF->DATA &= 0xF0;/* 一旦マスク */
            TSB_PF->DATA |= 0x08;/* LCDデータバス出力をセット、2行コマンド */
            
            EPLS();/* Eをパルス出力する */
            
            OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* 次回呼び出し時間 */
            OS_LoadTime->TSK_COND[5] = 0xA0;/* 遅延フラグ建築 */
                
            return;/* 戻る */
            break;
            
        case 6:
            /* 7回目である場合 */
            TSB_PF->CR &= 0xF0;/* 出力機能をオフ */
            TSB_PF->IE |= 0x0F;/* 入力機能をオン */
            TSB_PA_DATA_PA6 = 1;/* RWをオンする */
            
            if ((EPBSY() & 0x08) > 0) {
                /* ビジーフラグがオンである場合 */
                OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[5] = 0xA0;/* 遅延フラグ建築 */
                
                FLZFG |= 0x01;/* フラグセット */
                if (FLZTM >= 60000) {
                    /* 1分以上経過した場合 */
                    FLZFG = DLWSQ = 0;/* やり直し */
                    FLZTM = 0;/* タイマクリア */
                    
                }
                
                return;/* 戻る */
                
            }
            
            FLZFG = 0;/* クリア */
            DLWSQ++;/* 次回スタート位置をインクリメント */
            
            TSB_PA_DATA_PA6 = 0;/* RWをオフする */
            TSB_PF->IE &= 0xF0;/* LCDデータバスを非入力にする */
            TSB_PF->CR |= 0x0F;/* LCDデータバスを出力にする */
            TSB_PF->DATA &= 0xF0;/* 一旦マスク */
            TSB_PF->DATA |= 0x00;/* LCDデータバス出力をセット、？コマンド */
            
            EPLS();/* Eをパルス出力する */
            
            TSB_PF->DATA |= 0x08;/* LCDデータバス出力をセット、？コマンド */
            
            EPLS();/* Eをパルス出力する */
            
            OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* 次回呼び出し時間 */
            OS_LoadTime->TSK_COND[5] = 0xA0;/* 遅延フラグ建築 */
                
            return;/* 戻る */
            break;
            
        case 7:
            /* 8回目である場合 */
            TSB_PF->CR &= 0xF0;/* 出力機能をオフ */
            TSB_PF->IE |= 0x0F;/* 入力機能をオン */
            TSB_PA_DATA_PA6 = 1;/* RWをオンする */
            
            if ((EPBSY() & 0x08) > 0) {
                /* ビジーフラグがオンである場合 */
                OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[5] = 0xA0;/* 遅延フラグ建築 */
                
                FLZFG |= 0x01;/* フラグセット */
                if (FLZTM >= 60000) {
                    /* 1分以上経過した場合 */
                    FLZFG = DLWSQ = 0;/* やり直し */
                    FLZTM = 0;/* タイマクリア */
                    
                }
                
                return;/* 戻る */
                
            }
            
            FLZFG = 0;/* クリア */
            DLWSQ++;/* 次回スタート位置をインクリメント */
            
            TSB_PA_DATA_PA6 = 0;/* RWをオフする */
            TSB_PF->IE &= 0xF0;/* LCDデータバスを非入力にする */
            TSB_PF->CR |= 0x0F;/* LCDデータバスを出力にする */
            TSB_PF->DATA &= 0xF0;/* 一旦マスク */
            TSB_PF->DATA |= 0x00;/* LCDデータバス出力をセット、？コマンド */
            
            EPLS();/* Eをパルス出力する */
            
            TSB_PF->DATA |= 0x01;/* LCDデータバス出力をセット、？コマンド */
            
            EPLS();/* Eをパルス出力する */
            
            OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* 次回呼び出し時間 */
            OS_LoadTime->TSK_COND[5] = 0xA0;/* 遅延フラグ建築 */
                
            return;/* 戻る */
            break;
            
        case 8:
            /* 9回目である場合 */
            TSB_PF->CR &= 0xF0;/* 出力機能をオフ */
            TSB_PF->IE |= 0x0F;/* 入力機能をオン */
            TSB_PA_DATA_PA6 = 1;/* RWをオンする */
            
            if ((EPBSY() & 0x08) > 0) {
                /* ビジーフラグがオンである場合 */
                OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[5] = 0xA0;/* 遅延フラグ建築 */
                
                FLZFG |= 0x01;/* フラグセット */
                if (FLZTM >= 60000) {
                    /* 1分以上経過した場合 */
                    FLZFG = DLWSQ = 0;/* やり直し */
                    FLZTM = 0;/* タイマクリア */
                    
                }
                
                return;/* 戻る */
                
            }
            
            FLZFG = 0;/* クリア */
            DLWSQ++;/* 次回スタート位置をインクリメント */
            
            TSB_PA_DATA_PA6 = 0;/* RWをオフする */
            TSB_PF->IE &= 0xF0;/* LCDデータバスを非入力にする */
            TSB_PF->CR |= 0x0F;/* LCDデータバスを出力にする */
            TSB_PF->DATA &= 0xF0;/* 一旦マスク */
            TSB_PF->DATA |= 0x00;/* LCDデータバス出力をセット、？コマンド */
            
            EPLS();/* Eをパルス出力する */
            
            TSB_PF->DATA |= 0x06;/* LCDデータバス出力をセット、？コマンド */
            
            EPLS();/* Eをパルス出力する */
            
            OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* 次回呼び出し時間 */
            OS_LoadTime->TSK_COND[5] = 0xA0;/* 遅延フラグ建築 */
                
            return;/* 戻る */
            break;
            
        case 9:
            /* 10回目である場合 */
            TSB_PF->CR &= 0xF0;/* 出力機能をオフ */
            TSB_PF->IE |= 0x0F;/* 入力機能をオン */
            TSB_PA_DATA_PA6 = 1;/* RWをオンする */
            
            if ((EPBSY() & 0x08) > 0) {
                /* ビジーフラグがオンである場合 */
                OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[5] = 0xA0;/* 遅延フラグ建築 */
                
                FLZFG |= 0x01;/* フラグセット */
                if (FLZTM >= 60000) {
                    /* 1分以上経過した場合 */
                    FLZFG = DLWSQ = 0;/* やり直し */
                    FLZTM = 0;/* タイマクリア */
                    
                }
                
                return;/* 戻る */
                
            }
            
            FLZFG = 0;/* クリア */
            DLWSQ++;/* 次回スタート位置をインクリメント */
            
            TSB_PA_DATA_PA6 = 0;/* RWをオフする */
            TSB_PF->IE &= 0xF0;/* LCDデータバスを非入力にする */
            TSB_PF->CR |= 0x0F;/* LCDデータバスを出力にする */
            TSB_PF->DATA &= 0xF0;/* 一旦マスク */
            TSB_PF->DATA |= 0x00;/* LCDデータバス出力をセット、？コマンド */
            
            EPLS();/* Eをパルス出力する */
            
            TSB_PF->DATA |= 0x0C;/* LCDデータバス出力をセット、？コマンド */
            
            EPLS();/* Eをパルス出力する */
            
            OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* 次回呼び出し時間 */
            OS_LoadTime->TSK_COND[5] = 0xA0;/* 遅延フラグ建築 */
                
            return;/* 戻る */
            break;
            
        case 10:
            /* 11回目である場合 */
            if ((STSFL0 & 0x02) == 0) {
                /* 初期化完了していない場合 */
                OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[5] = 0xA0;/* 遅延フラグ建築 */
                
                return;/* 戻る */
                
            }
            
            if ((STSFL1 & 0x02) > 0) {
                /* メモリークリアフラグがオンしている場合 */
                if (MCLAST >= 60) {
                    /* メモリークリア受付完了である場合 */
                    MCLRMS();/* 飛びます */
                    DLWSQ = 29;/* LCD表示制御を「29」選択 */
                    
                    i = 0;/* 繰り返しの変数を一旦クリア */
                    continue;/* ループの先頭へ */
                    
                } else {
                    /* メモリークリア受付未完了である場合 */
                    OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* 次回呼び出し時間 */
                    OS_LoadTime->TSK_COND[5] = 0xA0;/* 遅延フラグ建築 */
                    
                    return;/* 戻る */
                    
                }
                
            }/* メモリークリアフラグがオンしていない場合はここに抜ける */
            
            if (TNO > 0) {
                /* セルフテストモードである場合 */
                DLWSQ = 19;/* LCD表示制御を「19」選択、ｾﾙﾌﾃｽﾄの処理へ */
                i = 0;/* 繰り返しの変数を一旦クリア */
                continue;/* ループの先頭へ */
                
            }
            
            if (((SYSFLG & 0x40) > 0) && ((RPAT & 0x20) == 0) && ((STSFL0 & 0x20) == 0)) {
                /* 電源ONで、洗浄工程中ではなく、準備中である場合 */
                MXOTN6 |= 0x20;/* 準備LEDをオンする */
                MX6BLK |= 0x20;/* 準備LEDの点滅をセット */
                
            } else {
                /* 電源OFFか、洗浄工程中であるか、準備中でない場合 */
                MXOTN6 &= 0xDF;/* 準備LEDをオフする */
                MX6BLK &= 0xDF;/* 準備LEDの点滅をクリア */
                
            }
            
            if ((SYSFLG & 0x40) > 0) {
                /* 電源オンである場合 */
                MXOTN6 |= 0x10;/* 電源LEDをセットする */
                
            } else {
                /* 電源オフである場合 */
                MXOTN6 &= 0xEF;/* 電源LEDをクリアする */
                
            }
            
            if ((SYKND0 & 0x10) > 0) {
                /* 蒸気式である場合 */
                SYSFLG &= 0xDF;/* チラー予約フラグを強制オフにする */
                
            }
            
            if ((SYSFLG & 0x20) > 0) {
                /* チラー予約されている場合 */
                MXOTN6 |= 0x80;/* チラー予約LEDをオン */
                
            } else {
                /* チラー予約されていない場合 */
                MXOTN6 &= 0x7F;/* チラー予約LEDをクリアする */
                
            }
            
            if ((SYSFLG & 0x02) == 0) {
                /* 運転がオフである場合 */
                MXOTN6 &= 0xBF;/* 運転LEDをオフ */
                MX6BLK &= 0xBF;/* 点滅フラグをオフ */
                
            } else {
                /* 運転がオンである場合 */
                MXOTN6 |= 0x40;/* 運転LEDをオン */
                if (CCSEQ >= 10) {
                    /* 真空解除工程である場合 */
                    MX6BLK |= 0x40;/* 点滅フラグをオン */
                    
                }
                
            }
            
            if (((RPAT & 0x20) > 0)
            || ((RPAT & 0x20) == 0) && ((SYSFLG & 0x40) == 0) && ((STSFL0 & 0x80) == 0)) {
                /* 洗浄工程中か、洗浄工程中ではなく電源がオフで、設定ﾓｰﾄﾞではない場合 */
                MXOTN6 &= 0xF0;/* パターンLEDをクリア */
                
            } else {
                /* 洗浄工程中ではないとき、電源オンか、設定モードのとき */
                MXOTN6 &= 0xF0;/* マスク */
                if (NPAT == 0) {
                    /* パターンが1の場合 */
                    MXOTN6 |= 0x01;/* 1セット */
                    
                } else if (NPAT == 1) {
                    /* パターンが2の場合 */
                    MXOTN6 |= 0x02;/* 2セット */
                    
                } else if (NPAT == 2) {
                    /* パターンが3の場合 */
                    MXOTN6 |= 0x04;/* 3セット */
                    
                } else {
                    /* パターンが4の場合 */
                    MXOTN6 |= 0x08;/* 4セット */
                    
                }
                
            }
            
            if ((SYSFLG & 0x40) == 0) {
                /* 電源オフである場合 */
                ABNFO = 0;/* 旧異常フラグをリセット */
                
                if ((STSFL0 & 0x80) == 0) {
                    /* 設定中ではない場合 */
                    DSPNO = 0;/* 時計表示をセット */
                    
                } else if ((DSPSEQ != 4) || ((DSPSEQ == 4) && (DSPSSQ == 0))) {
                    /* 洗浄モードでない、又は洗浄モードであり、メインメニューである場合 */
                    if ((DSPNO != 7) && (DSPSEQ > 0)) {
                        /* 設定画面へ変更時であり、画面シーケンスが進んでいた場合 */
                        DSPSEQ = 0;/* 一度クリア */
                        
                    }
                    DSPNO = 7;/* 設定モード表示セット */
                    
                } else {
                    /* これ以外の場合 */
                    DSPNO = 5;/* 洗浄モード表示をセット */
                    
                }
                
            } else {
                /* 電源オンである場合 */
                if ((ABNF & 0x0F) > 0) {
                    /* 異常がセットされている場合 */
                    STSFL0 &= 0x7F;/* 設定モードフラグを強制リセット */
                    STSFL0 |= 0x20;/* 準備中フラグをリセット */
                    
                    if ((ABNF & 0x0F) != ABNFO) {
                        /* 異常が変化していた場合 */
                        DSPSEQ = 0;/* 画面シーケンスをクリアする */
                        
                        /* 異常履歴の更新 */
                        ABNFO = (ABNF & 0x0F);/* オールドを更新する */
                        
                        REG_00 = ABNF;/* 最新異常データ */
                        REG_00 &= 0x0F;/* マスク */
                        REG_01 = ABNCUT;/* 一個前の異常データ */
                        REG_01 &= 0xF0;/* マスク */
                        
                        ABNCUT &= 0x0F;/* 4～7bit目をクリア */
                        ABNCUT |= (REG_00 << 4);/* 最新データをセットします */
                        
                        ABNH1 <<= 4;/* 一番古い異常データを押し出し、4番目のデータをセット */
                        REG_00 = ABNH0;/* データロード */
                        REG_00 >>= 4;/* シフト */
                        ABNH1 |= REG_00;/* 3番目の異常をロード */
                        ABNH0 <<= 4;/* 4bitシフト */
                        ABNH0 |= (REG_01 >> 4);/* 2番目のデータセット */
                        
                        REG_00 = ABNCUT;/* データロード */
                        REG_00 &= 0x0F;/* 異常件数のみマスク */
                        REG_00 ++;/* 異常件数をインクリメント */
                        
                        if (REG_00 >= 5) {
                            /* 5件以上になる場合 */
                            REG_00 = 5;/* MAX FIX */
                            
                        }
                        
                        ABNCUT &= 0xF0;
                        ABNCUT |= REG_00;/* 異常件数セット */
                        
                    }/* 異常が変化していた場合ここまで */
                    
                    if (((SYSFLG & 0x02) == 0)
                    || (((SYSFLG & 0x02) > 0) && ((ABNF & 0x0F) != 8) && ((ABNF & 0x0F) != 9) && ((ABNF & 0x0F) != 14))) {
                        /* 運転中ではない、又は運転中で常温槽水位電極棒異常ではなく、常温温度センサー異常ではなく、真空度異常ではない場合 */
                        DSPNO = 6;/* 異常表示をセットする */
                        
                    } else {
                        /* 運転中に、常温槽水位電極棒異常か、常温温度センサー異常か、真空度異常である場合 */
                        if ((STSFL0 & 0x20) == 0) {
                            /* 準備中の場合 */
                            DSPNO = 1;/* 準備中のメッセージセット */
                            
                        } else if ((SYSFLG & 0x1E) == 0) {
                            /* 運転工程ではない場合 */
                            DSPNO = 2;/* 待機表示セット */
                            
                        } else if ((SYSFLG & 0x0C) > 0) {
                            /* 冷却工程であった場合 */
                            DSPNO = 3;/* 冷却工程セット */
                            
                        } else {
                            /* 冷却工程ではない場合 */
                            DSPNO = 4;/* 冷却工程完了セット */
                            
                        }
                        
                    }
                    
                } else {
                    /* 異常がセットされていない場合 */
                    if ((STSFL0 & 0x80) == 0) {
                        /* 設定モードでない場合 */
                        if ((STSFL0 & 0x20) == 0) {
                            /* 準備中の場合 */
                            if ((DSPNO == 0) && (DSPSEQ > 0)) {
                                /* OFF -> ON時であり、画面シーケンスが進んでいた場合 */
                                DSPSEQ = 0;/* 一度クリア */
                                
                            }
                            DSPNO = 1;/* 準備中のメッセージセット */
                            
                        } else if ((SYSFLG & 0x1E) == 0) {
                            /* 運転工程ではない場合 */
                            DSPNO = 2;/* 待機表示セット */
                            
                        } else if ((SYSFLG & 0x0C) > 0) {
                            /* 冷却工程であった場合 */
                            DSPNO = 3;/* 冷却工程セット */
                            
                        } else {
                            /* 冷却工程ではない場合 */
                            DSPNO = 4;/* 冷却工程完了セット */
                            
                        }
                        
                    } else {
                        /* 設定モードの場合 */
                        if ((DSPSEQ != 4) || ((DSPSEQ == 4) && (DSPSSQ == 0))) {
                            /* 洗浄モードではない場合、又は洗浄モードでメインメニューの場合 */
                            if ((DSPNO != 7) && (DSPSEQ > 0)) {
                                /* 設定画面へ変更時であり、画面シーケンスが進んでいた場合 */
                                DSPSEQ = 0;/* 一度クリア */
                                
                            }
                            DSPNO = 7;/* 設定モード表示をセット */
                            
                        } else {
                            /* 洗浄モードでメインメニューでない場合 */
                            DSPNO = 5;/* 洗浄モード表示セット */
                            
                        }
                        
                    }
                    
                }
                /* 電源オンである場合ここまで */
                
            }
            
            DLWSQ++;/* インクリメント */
            DLWSQ += DSPNO;/* 画面番号を加算 */
            i = 0;/* 再度ループに入りなおすため、ループ値をクリア */
            
            break;
            
        case 11:
            /* 時計表示 */
            STSFL0 &= 0xEF;/* ブザー音禁止フラグ */
            TSB_PF_DATA_PF4 = 0;/* バックライトオフ */
            
            if (DSPSEQ == 0) {
                /* 初回の場合 */
                DSPSEQ = 1;/* 1にする */
                DSPTM2 = 0;/* タイマクリア */
                
                memcpy(DSPLB1, CLKMS1 + 1, 16);/* 16バイトコピー */
                memcpy(DSPLB2, CLKMS2 + 1, 16);/* 16バイトコピー */
                DSPLB2[2] = NUMDEG[((RTCDT[6] % 100) / 10)];/* 年10の位 */
                DSPLB2[3] = NUMDEG[(RTCDT[6] % 10)];/* 年１の位 */
                DSPLB2[5] = NUMDEG[((RTCDT[5] % 100) / 10)];/* 月10の位 */
                DSPLB2[6] = NUMDEG[(RTCDT[5] % 10)];/* 月１の位 */
                DSPLB2[8] = NUMDEG[((RTCDT[4] % 100) / 10)];/* 日10の位 */
                DSPLB2[9] = NUMDEG[(RTCDT[4] % 10)];/* 日１の位 */
                DSPLB2[11] = NUMDEG[((RTCDT[2] % 100) / 10)];/* 時10の位 */
                DSPLB2[12] = NUMDEG[(RTCDT[2] % 10)];/* 時1の位 */
                DSPLB2[14] = NUMDEG[((RTCDT[1] % 100) / 10)];/* 分10の位 */
                DSPLB2[15] = NUMDEG[(RTCDT[1] % 10)];/* 分1の位 */
                
                DSPLN1 = 16;/* データ長セット */
                MHEAD1 = DSPLB1;/* メッセージの先頭アドレスをセット */
                MHEAD1 --;/* アドレスをデクリメント */
                DSPLN2 = 16;/* データ長16をセット */
                MHEAD2 = DSPLB2;/* メッセージの先頭アドレスをセット */
                MHEAD2 --;/* アドレスをデクリメント */
                
                DLWSQ = 25;/* 表示クリアから開始 */
                i = 0;/* 再度ループに入りなおし可 */
                continue;/* 先頭に戻る */
                
            } else {
                /* 初回以降である場合 */
                if ((STSFL1 & 0x08) > 0) {
                    /* 手動排水受付中である場合 */
                    TSB_PF_DATA_PF4 = 1;/* バックライトオン */
                    memcpy(DSPLB1, DRAIN1 + 1, 16);/* 16バイトコピー */
                    
                } else if ((STSFL1 & 0x10) > 0) {
                    /* 排水中である場合 */
                    TSB_PF_DATA_PF4 = 1;/* バックライトオン */
                    memcpy(DSPLB1, DRAIN2 + 1, 16);/* 16バイトコピー */
                    
                } else {
                    /* 排水関係が無い場合 */
                    TSB_PF_DATA_PF4 = 0;/* バックライトオフ */
                    memcpy(DSPLB1, CLKMS1+ 1, 16);/* 16バイトコピー */
                    
                }
                
                DSPTM2++;/* タイマインクリメント */
                if ((DSPTM2 < 10) && ((STSFL1 & 0x18) == 0)) {
                    /* タイマが0.5秒未満で、排水関係のフラグが無い場合 */
                    /* 何もしない */
                    
                } else {
                    /* タイマが0.5秒以上か、排水関係のフラグがある場合 */
                    if (DSPTM2 >= 10) {
                        /* タイマが1秒経過した場合 */
                        DSPTM2 = 0;/* クリア */
                        DCLNF ^= 0x01;/* 「：」反転フラグ構築 */
                        
                    }
                    
                    memcpy(DSPLB2, CLKMS2 + 1, 16);/* 16バイトコピー */
                    DSPLB2[2] = NUMDEG[((RTCDT[6] % 100) / 10)];/* 年10の位 */
                    DSPLB2[3] = NUMDEG[(RTCDT[6] % 10)];/* 年１の位 */
                    DSPLB2[5] = NUMDEG[((RTCDT[5] % 100) / 10)];/* 月10の位 */
                    DSPLB2[6] = NUMDEG[(RTCDT[5] % 10)];/* 月１の位 */
                    DSPLB2[8] = NUMDEG[((RTCDT[4] % 100) / 10)];/* 日10の位 */
                    DSPLB2[9] = NUMDEG[(RTCDT[4] % 10)];/* 日１の位 */
                    DSPLB2[11] = NUMDEG[((RTCDT[2] % 100) / 10)];/* 時10の位 */
                    DSPLB2[12] = NUMDEG[(RTCDT[2] % 10)];/* 時1の位 */
                    DSPLB2[14] = NUMDEG[((RTCDT[1] % 100) / 10)];/* 分10の位 */
                    DSPLB2[15] = NUMDEG[(RTCDT[1] % 10)];/* 分1の位 */
                    
                    if ((DCLNF & 0x01) > 0) {
                        /* コロンを消すタイミングである場合 */
                        DSPLB2[13] = 0x20;/* コロン　→　スペース */
                        
                    }
                    
                    DSPLN1 = 16;/* データ長セット */
                    MHEAD1 = DSPLB1;/* メッセージの先頭アドレスをセット */
                    MHEAD1 --;/* アドレスをデクリメント */
                    DSPLN2 = 16;/* データ長16をセット */
                    MHEAD2 = DSPLB2;/* メッセージの先頭アドレスをセット */
                    MHEAD2 --;/* アドレスをデクリメント */
                    
                    DLWSQ = 29;/* 1～２行目表示処理から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                }
                
            }/* 初回以降の処理ここまで */
            
            break;
            
        case 12:
            /* TOPメッセージ表示 */
            TSB_PF_DATA_PF4 = 1;/* バックライトオン */
            
            if (DSPSEQ == 0) {
                /* 表示シーケンスが0である場合 */
                DSPSEQ = 1;/* 1にする */
                
                memcpy(DSPLB1, TOPMS1 + 1, 16);/* 16バイトコピー */
                
                if ((SYKND0 & 0xC0) == 0) {
                    /* 35RCLである場合 */
                    /* 何もしない */
                } else if ((SYKND0 & 0xC0) == 0xC0) {
                    /* 40RCLである場合 */
                    DSPLB1[12] = '4';/* shift-jis */
                    DSPLB1[13] = '0';/* shift-jis */
                    
                } else if ((SYKND0 & 0xC0) == 0x40) {
                    /* 50RCLである場合 */
                    DSPLB1[12] = '5';/* shift-jis */
                    DSPLB1[13] = '0';/* shift-jis */
                    
                } else {
                    /* 100RCLである場合 */
                    DSPLB1[11] = '1';/* shift-jis */
                    DSPLB1[12] = '0';/* shift-jis */
                    DSPLB1[13] = '0';/* shift-jis */
                    
                }
                
                DSPLN1 = 16;/* データ長セット */
                MHEAD1 = DSPLB1;/* メッセージの先頭アドレスをセット */
                MHEAD1 --;/* アドレスをデクリメント */
                
                memset(DSPLB2, 0x20, 16);
                DSPLN2 = 16;/* データ長16をセット */
                MHEAD2 = DSPLB2;/* メッセージの先頭アドレスをセット */
                MHEAD2 --;/* アドレスをデクリメント */
                
                DSPTM1 = 0;/* タイマをクリアする */
                
                DLWSQ = 25;/* 表示クリアから開始 */
                i = 0;/* 再度ループに入りなおし可 */
                continue;/* 先頭に戻る */
                
            } else {
                /* 初回以降である場合 */
                DSPTM1++;/* タイマインクリメント */
                if (DSPTM1 >= 4) {
                    /* タイマが200ms経過した場合 */
                    DSPTM1 = 0;/* タイマークリア */
                    
                    memcpy(DSPLB2, DSPLB2 + 1, 15);/* 15バイトシフト */
                    
                    if (DSPSEQ < 20) {
                        /* 2つ目のメッセージ表示タイミングではない場合 */
                        DSPLB2[15] = TOPMS2[DSPSEQ];
                        
                    } else if (DSPSEQ < 21) {
                        /* スペース表示タイミングである場合 */
                        DSPLB2[15] = 0x20;/* スペース */
                        
                    } else if (DSPSEQ < 37) {
                        /* スペースエリアである場合 */
                        if ((ABNF & 0x30) == 0) {
                            /* 予知警報・チラー異常なしである場合 */
                            DSPLB2[15] = SPACCE[(DSPSEQ - 20)];
                            
                        } else if ((ABNF & 0x30) == 0x20) {
                            /* 予知警報あり、チラー異常なしである場合 */
                            DSPLB2[15] = ALMSG[(DSPSEQ - 20)];
                            
                        } else {
                            /* チラー異常が発生している場合 */
                            DSPLB2[15] = ABN02[(DSPSEQ - 20)];
                            if (((ABNF & 0x40) > 0) && (DSPSEQ < 23)) {
                                /* ブザーフラグがあり、0.4秒以上経過していない場合 */
                                TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                                TSB_PC_DATA_PC7 = 1;/* 外部警報出力をオンする */
                                MXOTN2 |= 0x80;/* ビットをセット */
                                
                            } else {
                                /* ブザーフラグが無い、又は0.4以上経過した場合 */
                                TSB_PC_DATA_PC2 = 0;/* ブザーオフ */
                                TSB_PC_DATA_PC7 = 0;/* 外部警報出力をオフする */
                                MXOTN2 &= 0x7F;/* ビットクリア */
                                
                            }
                            
                        }
                        
                    } else if (DSPSEQ < 39) {
                        /* メッセージエンドである場合 */
                        DSPLB2[15] = 0x20;/* スペース */
                        
                    } else {
                        /* それ以外(最終) */
                        DSPLB2[15] = 0x20;/* スペース */
                        DSPSEQ = 0;/* クリア */
                        
                    }
                    
                    DSPSEQ++;/* インクリメント */
                    DSPLN2 = 16;/* データ長をセット */
                    MHEAD2 = DSPLB2;/* メッセージの先頭アドレスをセット */
                    MHEAD2 --;/* アドレスをデクリメント */
                    
                    DLWSQ = 33;/* 2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                }/* タイマが200ms経過していない場合はここに抜ける */
                
            }
            
            break;
            
        case 13:
            /* 運転待機表示 */
            TSB_PF_DATA_PF4 = 1;/* バックライトオン */
            
            if (DSPSEQ == 0) {
                /* 1回目である場合 */
                BZTM1 = 0;/* ブザータイマをクリアする */
                if ((STSFL0 & 0x10) == 0) {
                    /* ブザー音禁止では無い場合 */
                    STSFL0 |= 0x40;/* セット　クリック */
                    
                }
                
                DSPSEQ = 1;/* 次回2回目に変更 */
                DSPTM1 = 0;/* タイマクリア */
                
                DSPLN1 = RDYMS1[0];/* 1行目の文字数セット */
                MHEAD1 = RDYMS1;/* 1行目の先頭アドレスをセット */
                
                memcpy(DSPLB2, RDYMS2 + 1, 16);/* 16バイトメモリーコピーする */
                
                if (NPAT == 0) {
                    /* パターン1である場合 */
                    REG_00 = PAT1;/* パターン1の設定温度 */
                    REG_01 = (RPAT & 0x01);/* 徐冷の有無をセット（0なら無し） */
                    
                } else if (NPAT == 1) {
                    /* パターン2である場合 */
                    REG_00 = PAT2;/* パターン2の設定温度 */
                    REG_01 = (RPAT & 0x02);/* 徐冷の有無をセット（0なら無し） */
                    
                } else if (NPAT == 2) {
                    /* パターン3である場合 */
                    REG_00 = PAT3;/* パターン3の設定温度 */
                    REG_01 = (RPAT & 0x04);/* 徐冷の有無をセット（0なら無し） */
                    
                } else {
                    /* パターン4である場合 */
                    REG_00 = PAT4;/* パターン4の設定温度 */
                    REG_01 = (RPAT & 0x08);/* 徐冷の有無をセット（0なら無し） */
                    
                }
                
                DSPLB2[4] = NUMDEG2[((REG_00 % 100) / 10)];/* 温度10の位(ゼロは表示しない) */
                DSPLB2[5] =  NUMDEG[(REG_00 % 10)];/* 温度1の位 */
                
                if (REG_01 == 0) {
                    /* 徐冷が無い場合 */
                    DSPLB2[14] = 'ﾅ';/* shift-jis */
                    DSPLB2[15] = 'ｼ';/* shift-jis */
                    
                }
                
                DSPLN2 = 16;/* データ長をセット */
                MHEAD2 = DSPLB2;/* メッセージの先頭アドレスをセット */
                MHEAD2 --;/* アドレスをデクリメント */
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                continue;/* 先頭に戻る */
                
            } else if (DSPSEQ == 1) {
                /* 2回目である場合 */
                BZTM1++;/* ブザータイマをインクリメント */
                
                if (BZTM1 >= 4) {
                    /* 200ms以上経過していた場合 */
                    if (BZTM1 == 4) {
                        /* ジャスト200msであった場合 */
                        if ((STSFL0 & 0x10) == 0) {
                            /* ブザー音禁止では無い場合 */
                            STSFL0 |= 0x40;/* クリックをセット */
                            
                        }
                        
                    }
                    
                    STSFL0 &= 0xEF;/* ブザー禁止フラグをリセットする */
                    BZTM1 = 4;/* FIX */
                    
                }/* 200ms未満の場合はここに抜ける */
                
                DSPTM1++;/* タイマをインクリメント */
                
                if (DSPTM1 < 40) {
                    /* 2秒未満の場合 */
                    DSPLN1 = RDYMS1[0];/* 1行目の文字数セット */
                    MHEAD1 = RDYMS1;/* 1行目の先頭アドレスをセット */
                    
                    memcpy(DSPLB2, RDYMS2 + 1, 16);/* 16バイトメモリーコピーする */
                    
                    if (NPAT == 0) {
                        /* パターン1である場合 */
                        REG_00 = PAT1;/* パターン1の設定温度 */
                        REG_01 = (RPAT & 0x01);/* 徐冷の有無をセット（0なら無し） */
                        
                    } else if (NPAT == 1) {
                        /* パターン2である場合 */
                        REG_00 = PAT2;/* パターン2の設定温度 */
                        REG_01 = (RPAT & 0x02);/* 徐冷の有無をセット（0なら無し） */
                        
                    } else if (NPAT == 2) {
                        /* パターン3である場合 */
                        REG_00 = PAT3;/* パターン3の設定温度 */
                        REG_01 = (RPAT & 0x04);/* 徐冷の有無をセット（0なら無し） */
                        
                    } else {
                        /* パターン4である場合 */
                        REG_00 = PAT4;/* パターン4の設定温度 */
                        REG_01 = (RPAT & 0x08);/* 徐冷の有無をセット（0なら無し） */
                        
                    }
                    
                    DSPLB2[4] = NUMDEG2[((REG_00 % 100) / 10)];/* 温度10の位(ゼロは表示しない) */
                    DSPLB2[5] =  NUMDEG[(REG_00 % 10)];/* 温度1の位 */
                    
                    if (REG_01 == 0) {
                        /* 徐冷が無い場合 */
                        DSPLB2[14] = 'ﾅ';/* shift-jis */
                        DSPLB2[15] = 'ｼ';/* shift-jis */
                        
                    }
                    
                    DSPLN2 = 16;/* データ長をセット */
                    MHEAD2 = DSPLB2;/* メッセージの先頭アドレスをセット */
                    MHEAD2 --;/* アドレスをデクリメント */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                } else {
                    /* 2秒経過した場合 */
                    DSPSEQ = 2;/* シーケンスを3回目にする */
                    DSPTM1 = 0;/* タイマをクリア */
                    /* 再度DLWSQ＝13にて処理やり直しとなる */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* ループ先頭に戻る */
                    
                }
                
            } else if (DSPSEQ == 2) {
                /* 3回目である場合 */
                if ( ((ABNF & 0x30) == 0)
                || (DSPTM1++ >= 40) ) {
                    /* チラー異常と予知警報が無い、又は2秒以上経過した場合 */
                    DSPSEQ = 1;/* 次回2回目に変更 */
                    DSPTM1 = 0;/* タイマクリア */
                    
                    DSPLN1 = RDYMS1[0];/* 1行目の文字数セット */
                    MHEAD1 = RDYMS1;/* 1行目の先頭アドレスをセット */
                    
                    memcpy(DSPLB2, RDYMS2 + 1, 16);/* 16バイトメモリーコピーする */
                    
                    if (NPAT == 0) {
                        /* パターン1である場合 */
                        REG_00 = PAT1;/* パターン1の設定温度 */
                        REG_01 = (RPAT & 0x01);/* 徐冷の有無をセット（0なら無し） */
                        
                    } else if (NPAT == 1) {
                        /* パターン2である場合 */
                        REG_00 = PAT2;/* パターン2の設定温度 */
                        REG_01 = (RPAT & 0x02);/* 徐冷の有無をセット（0なら無し） */
                        
                    } else if (NPAT == 2) {
                        /* パターン3である場合 */
                        REG_00 = PAT3;/* パターン3の設定温度 */
                        REG_01 = (RPAT & 0x04);/* 徐冷の有無をセット（0なら無し） */
                        
                    } else {
                        /* パターン4である場合 */
                        REG_00 = PAT4;/* パターン4の設定温度 */
                        REG_01 = (RPAT & 0x08);/* 徐冷の有無をセット（0なら無し） */
                        
                    }
                    
                    DSPLB2[4] = NUMDEG2[((REG_00 % 100) / 10)];/* 温度10の位(ゼロは表示しない) */
                    DSPLB2[5] =  NUMDEG[(REG_00 % 10)];/* 温度1の位 */
                    
                    if (REG_01 == 0) {
                        /* 徐冷が無い場合 */
                        DSPLB2[14] = 'ﾅ';/* shift-jis */
                        DSPLB2[15] = 'ｼ';/* shift-jis */
                        
                    }
                    
                    DSPLN2 = 16;/* データ長をセット */
                    MHEAD2 = DSPLB2;/* メッセージの先頭アドレスをセット */
                    MHEAD2 --;/* アドレスをデクリメント */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                }/* チラー異常か予知警報があり、時間が2秒未満である場合はここに抜ける */
                
                if ((ABNF & 0x10) == 0) {
                    /* チラー異常ではなく予知警報がある場合 */
                    DSPLN2 = ALMSG[0];/* データ長をセット */
                    MHEAD2 = ALMSG;/* 文字列の先頭アドレスをセット */
                    
                    DLWSQ = 33;/* 2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                } else {
                    /* チラー異常がある場合 */
                    if (((ABNF & 0x40) > 0) && (DSPTM1 < 10)) {
                        /* ブザー要求があり、0.5秒未満である場合 */
                        TSB_PC_DATA_PC2 = 1;/* ブザー出力をオン */
                        TSB_PC_DATA_PC7 = 1;/* 外部警報出力をオン */
                        MXOTN2 |= 0x80;/* ビットセット */
                        
                    } else {
                        /* ブザー要求が無いか、0.5秒以上経過した場合 */
                        TSB_PC_DATA_PC2 = 0;/* ブザー出力をオフ */
                        TSB_PC_DATA_PC7 = 0;/* 外部警報出力をオフ */
                        MXOTN2 &= 0x7F;/* ビットクリア */
                        
                    }
                    
                    DSPLN2 = ABN02[0];/* データ長をセット */
                    MHEAD2 = ABN02;/* 文字列の先頭アドレスをセット */
                    
                    DLWSQ = 33;/* 2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                }
                /* 3回目ここまで */
                
            }
            
            break;
            
        case 14:
            /* 徐冷・冷却工程表示 */
            TSB_PF_DATA_PF4 = 1;/* バックライトオン */
            
            if ((DSPSEQ == 0) || ((DSPSEQ == 1) && (DSPTM1++ < 40))) {
                /* 1回目である、又は2回目以降で2秒未満である場合 */
                if (DSPSEQ == 0) {
                    /* 1回目に限り */
                    DSPSEQ = 1;/* セット、次回2回目にする */
                    DSPTM1 = 0;/* タイマをクリア */
                    
                }
                
                if ((SYSFLG & 0x08) == 0) {
                    /* 冷却工程ではない場合 */
                    memcpy(DSPLB1, RNMS1 + 1, 16);/* 16バイトメモりーコピー */
                    
                } else {
                    /* 冷却工程中である場合 */
                    memcpy(DSPLB1, RNMS2 + 1, 16);/* 16バイトメモりーこぴー */
                    
                }
                
                memcpy(DSPLB2, RNSTM1 + 1, 16);/* 16バイトメモリーコピー */
                
                if (NPAT == 0) {
                    /* パターン1である場合 */
                    REG_00 = PAT1;/* パターン1の設定温度 */
                    
                } else if (NPAT == 1) {
                    /* パターン2である場合 */
                    REG_00 = PAT2;/* パターン2の設定温度 */
                    
                } else if (NPAT == 2) {
                    /* パターン3である場合 */
                    REG_00 = PAT3;/* パターン3の設定温度 */
                    
                } else {
                    /* パターン4である場合 */
                    REG_00 = PAT4;/* パターン4の設定温度 */
                    
                }
                
                DSPLB1[12] = NUMDEG2[((REG_00 % 100) / 10)];/* 温度10の位(ゼロは表示しない) */
                DSPLB1[13] =  NUMDEG[(REG_00 % 10)];/* 温度1の位 */
                
                DSPLB2[11] = NUMDEG2[((TMPDT0 % 100) / 10)];/* 温度10の位(ゼロは表示しない) */
                DSPLB2[12] =  NUMDEG[(TMPDT0 % 10)];/* 温度1の位 */
                
                DSPLN1 = 16;/* データ長をセット */
                MHEAD1 = DSPLB1;/* メッセージの先頭アドレスをセット */
                MHEAD1 --;/* アドレスをデクリメント */
                
                DSPLN2 = 16;/* データ長をセット */
                MHEAD2 = DSPLB2;/* メッセージの先頭アドレスをセット */
                MHEAD2 --;/* アドレスをデクリメント */
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                continue;/* 先頭に戻る */
                
            } else if ((DSPSEQ == 1) && (DSPTM1 >= 40)) {
                /* 2回目以降で2秒以上経過した場合 */
                DSPSEQ = 2;/* 3回目にセット */
                DSPTM1 = 0;/* タイマクリア */
                
            }
            
            if (DSPSEQ == 2) {
                /* 3回目である場合 */
                DSPTM1++;/* タイマをインクリメント */
                if (DSPTM1 < 40) {
                    /* 2秒経過していない場合 */
                    memcpy(DSPLB2, RNSTM2 + 1, 16);/* メモリーコピー */
                    
                    DSPLB2[10] = NUMDEG2[((STIME2 % 100) / 10)];/* 開始時刻「時」10の位(ゼロは表示しない) */
                    DSPLB2[11] =  NUMDEG[(STIME2 % 10)];/* 開始時刻「時」1の位 */
                    DSPLB2[13] = NUMDEG2[((STIME1 % 100) / 10)];/* 開始時刻「分」10の位(ゼロは表示しない) */
                    DSPLB2[14] =  NUMDEG[(STIME1 % 10)];/* 開始時刻「分」1の位 */
                    
                    DSPLN2 = 16;/* データ長をセット */
                    MHEAD2 = DSPLB2;/* メッセージの先頭アドレスをセット */
                    MHEAD2 --;/* アドレスをデクリメント */
                    
                    DLWSQ = 33;/* 2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                } else {
                    /* 2秒経過した場合 */
                    DSPSEQ = 3;/* 次の4回目表示へ */
                    DSPTM1 = 0;/* タイマークリア */
                    
                }
                
            }
            
            if (DSPSEQ == 3) {
                /* 4回目である場合 */
                DSPTM1++;/* タイマをインクリメント */
                if (DSPTM1 < 40) {
                    /* 2秒経過していない場合 */
                    memcpy(DSPLB2, RNSTM3 + 1, 16);/* メモリーコピー */
                    
                    DSPLB2[10] = NUMDEG2[((CSEQTM % 1000) / 100)];/* シーケンス時間の100の位(ゼロは表示しない) */
                    if (CSEQTM >= 10) {
                        /* 10の位ありの場合 */
                        DSPLB2[11] = NUMDEG[((CSEQTM % 100) / 10)];/* シーケンス時間の10の位 */
                        
                    }/* 10の位無しの場合は何もしない */
                    DSPLB2[12] =  NUMDEG[(CSEQTM % 10)];/* シーケンス時間の1の位 */
                    
                    DSPLN2 = 16;/* データ長をセット */
                    MHEAD2 = DSPLB2;/* メッセージの先頭アドレスをセット */
                    MHEAD2 --;/* アドレスをデクリメント */
                    
                    DLWSQ = 33;/* 2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                } else {
                    /* 2秒経過した場合 */
                    DSPSEQ = 4;/* 次の5回目の表示へ */
                    DSPTM1 = 0;/* タイマークリア */
                    
                }
                
            }
            
            if (DSPSEQ == 4) {
                /* 5回目である場合 */
                if (((ABNF & 0x3F) == 0) || (DSPTM1++ >= 40)) {
                    /* 異常か予知警報が無い場合、又は2秒以上経過した場合 */
                    i = 0;/* 再度ループ入りなおし可 */
                    DSPTM1 = 0;/* タイマクリア */
                    DSPSEQ = 1;/* シーケンス番号を戻す */
                    continue;/* ループの先頭へ戻る */
                    
                }/* 異常があって、タイマが2秒未満の場合はここに抜ける */
                
                if ((ABNF & 0x1F) == 0) {
                    /* 予知警報であった場合 */
                    DSPLN2 = ALMSG[0];/* データ長をセット */
                    MHEAD2 = ALMSG;/* 先頭アドレスをセット */
                    
                    DLWSQ = 33;/* 2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                }
                
                if (((ABNF & 0x40) > 0) && (DSPTM1 < 10)) {
                    /* ブザー要求があり、0.5秒未満である場合 */
                    TSB_PC_DATA_PC2 = 1;/* ブザー出力をオン */
                    TSB_PC_DATA_PC7 = 1;/* 外部警報出力をオン */
                    MXOTN2 |= 0x80;/* ビットセット */
                    
                } else {
                    /* ブザー要求が無いか、0.5秒以上経過した場合 */
                    TSB_PC_DATA_PC2 = 0;/* ブザー出力をオフ */
                    TSB_PC_DATA_PC7 = 0;/* 外部警報出力をオフ */
                    MXOTN2 &= 0x7F;/* ビットクリア */
                    
                }
                
                if ((ABNF & 0x0F) == 0) {
                    /* チラー異常(継続)であった場合 */
                    DSPLN2 = ABN02[0];/* データ長をセット */
                    MHEAD2 = ABN02;/* 先頭アドレスをセット */
                    
                    DLWSQ = 33;/* 2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                } else {
                    /* それ以外の異常であった場合 */
                    if (((SYKND0 & 0x10) > 0) && ((ABNF & 0x0F) == 8)) {
                        /* 蒸気式であり、常温槽水位電極棒異常である場合 */
                        DSPLN2 = ABNFSTBL[17][0];/* データ長 */
                        MHEAD2 = ABNFSTBL[17];/* 先頭アドレス */
                        
                    } else if (((SYKND0 & 0x10) > 0) && ((ABNF & 0x0F) == 9)) {
                        /* 蒸気式であり、常温槽水槽温度センサー異常である場合 */
                        DSPLN2 = ABNFSTBL[18][0];/* データ長 */
                        MHEAD2 = ABNFSTBL[18];/* 先頭アドレス */
                        
                    } else {
                        /* それ以外の場合 */
                        DSPLN2 = ABNFSTBL[(ABNF & 0x0F)][0];/* データ長 */
                        MHEAD2 = ABNFSTBL[(ABNF & 0x0F)];/* 先頭アドレス */
                        
                    }
                    
                    DLWSQ = 33;/* 2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                }
                /* 5回目ここまで */
                
            }
            
            break;
            
        case 15:
            /* 冷却工程完了表示 */
            TSB_PF_DATA_PF4 = 1;/* バックライト出力オン */
            
            if ((DSPSEQ == 0) || ((DSPSEQ == 1) && (DSPTM1++ < 40))) {
                /* 1回目である、又は2回目以降で2秒未満である場合 */
                if (DSPSEQ == 0) {
                    /* 1回目に限り */
                    DSPSEQ = 1;/* セット、次回2回目にする */
                    DSPTM1 = 0;/* タイマをクリア */
                    
                }
                
                if (CCSEQ == 15) {
                    /* 完了表示タイミングである場合 */
                    memcpy(DSPLB1, ENDMS1 + 1, 16);/* メモリーコピー */
                    
                } else {
                    /* 完了以外のタイミングである場合 */
                    memcpy(DSPLB1, DESVC1 + 1, 16);/* メモリーコピー */
                    
                }
                
                memcpy(DSPLB2, RNSTM1 + 1, 16);/* 品温表示メモリーコピー */
                
                if (CCSEQ == 15) {
                    /* 完了表示タイミングである場合 */
                    if (NPAT == 0) {
                        /* パターン1の場合 */
                        REG_00 = PAT1;/* パターン1の温度をセット */
                        
                    } else if (NPAT == 1) {
                        /* パターン2の場合 */
                        REG_00 = PAT2;/* パターン2の温度をセット */
                        
                    } else if (NPAT == 2) {
                        /* パターン3の場合 */
                        REG_00 = PAT3;/* パターン3の温度をセット */
                        
                    } else {
                        /* パターン4の場合 */
                        REG_00 = PAT4;/* パターン4の温度をセット */
                        
                    }
                    
                    DSPLB1[12] = NUMDEG2[((REG_00 % 100) / 10)];/* 温度10の位(ゼロは表示しない) */
                    DSPLB1[13] =  NUMDEG[(REG_00 % 10)];/* 温度1の位 */
                    
                }
                
                DSPLB2[11] = NUMDEG2[((TMPDT0 % 100) / 10)];/* 温度10の位(ゼロは表示しない) */
                DSPLB2[12] =  NUMDEG[(TMPDT0 % 10)];/* 温度1の位 */
                
                DSPLN1 = 16;/* データ長をセット */
                MHEAD1 = DSPLB1;/* メッセージの先頭アドレスをセット */
                MHEAD1 --;/* アドレスをデクリメント */
                
                DSPLN2 = 16;/* データ長をセット */
                MHEAD2 = DSPLB2;/* メッセージの先頭アドレスをセット */
                MHEAD2 --;/* アドレスをデクリメント */
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                continue;/* 先頭に戻る */
                
            } else if ((DSPSEQ == 1) && (DSPTM1 >= 40)) {
                /* 2回目以降で2秒以上経過した場合 */
                DSPSEQ = 2;/* 3回目の処理へ */
                DSPTM1 = 0;/* タイマクリア */
                
            }
            
            if (DSPSEQ == 2) {
                /* 3回目の処理である場合 */
                if (DSPTM1++ < 40) {
                    /* タイムアップしていない場合 */
                    memcpy(DSPLB2, RNSTM2 + 1, 16);/* 開始時刻表示をメモリーコピー */
                    
                    DSPLB2[10] = NUMDEG2[((STIME2 % 100) / 10)];/* 開始時刻「時」10の位(ゼロは表示しない) */
                    DSPLB2[11] =  NUMDEG[(STIME2 % 10)];/* 開始時刻「時」1の位 */
                    DSPLB2[13] = NUMDEG2[((STIME1 % 100) / 10)];/* 開始時刻「分」10の位(ゼロは表示しない) */
                    DSPLB2[14] =  NUMDEG[(STIME1 % 10)];/* 開始時刻「分」1の位 */
                    
                    DSPLN2 = 16;/* データ長をセット */
                    MHEAD2 = DSPLB2;/* メッセージの先頭アドレスをセット */
                    MHEAD2 --;/* アドレスをデクリメント */
                    
                    DLWSQ = 33;/* 2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                } else {
                    /* タイムアップした場合 */
                    DSPSEQ = 3;/* 4回目の処理へ */
                    DSPTM1 = 0;/* タイマクリア */
                    
                }
                
            }
            
            if (DSPSEQ == 3) {
                /* 4回目の処理である場合 */
                if (DSPTM1++ < 40) {
                    /* タイムアップしていない場合 */
                    memcpy(DSPLB2, ENDMS2 + 1, 16);/* 終了時刻表示をメモリーコピー */
                    
                    DSPLB2[10] = NUMDEG2[((ETIME2 % 100) / 10)];/* 終了時刻「時」10の位(ゼロは表示しない) */
                    DSPLB2[11] =  NUMDEG[(ETIME2 % 10)];/* 終了時刻「時」1の位 */
                    DSPLB2[13] = NUMDEG2[((ETIME1 % 100) / 10)];/* 終了時刻「分」10の位(ゼロは表示しない) */
                    DSPLB2[14] =  NUMDEG[(ETIME1 % 10)];/* 終了時刻「分」1の位 */
                    
                    DSPLN2 = 16;/* データ長をセット */
                    MHEAD2 = DSPLB2;/* メッセージの先頭アドレスをセット */
                    MHEAD2 --;/* アドレスをデクリメント */
                    
                    DLWSQ = 33;/* 2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                } else {
                    /* タイムアップした場合 */
                    DSPSEQ = 4;/* 5回目の処理へ */
                    DSPTM1 = 0;/* タイマクリア */
                    
                }
                
            }
            
            if (DSPSEQ == 4) {
                /* 5回目の処理であった場合 */
                if (((ABNF & 0x3F) == 0) || (DSPTM1++ >= 40)) {
                    /* 異常か予知警報が無い場合、又は2秒以上経過した場合 */
                    i = 0;/* 再度ループ入りなおし可 */
                    DSPTM1 = 0;/* タイマクリア */
                    DSPSEQ = 1;/* シーケンス番号を戻す */
                    continue;/* ループの先頭へ戻る */
                    
                }/* 異常があって、タイマが2秒未満の場合はここに抜ける */
                
                if ((ABNF & 0x1F) == 0) {
                    /* 予知警報であった場合 */
                    DSPLN2 = ALMSG[0];/* データ長をセット */
                    MHEAD2 = ALMSG;/* 先頭アドレスをセット */
                    
                    DLWSQ = 33;/* 2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                }
                
                ABNF &= 0xBF;/* 継続異常警報(ブザー)フラグをリセットする */
                
                if ((ABNF & 0x0F) == 0) {
                    /* チラー異常(継続)であった場合 */
                    DSPLN2 = ABN02[0];/* データ長をセット */
                    MHEAD2 = ABN02;/* 先頭アドレスをセット */
                    
                    DLWSQ = 33;/* 2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                } else {
                    /* それ以外の異常であった場合 */
                    if (((SYKND0 & 0x10) > 0) && ((ABNF & 0x0F) == 8)) {
                        /* 蒸気式であり、常温槽水位電極棒異常である場合 */
                        DSPLN2 = ABNFSTBL[17][0];/* データ長 */
                        MHEAD2 = ABNFSTBL[17];/* 先頭アドレス */
                        
                    } else if (((SYKND0 & 0x10) > 0) && ((ABNF & 0x0F) == 9)) {
                        /* 蒸気式であり、常温槽水槽温度センサー異常である場合 */
                        DSPLN2 = ABNFSTBL[18][0];/* データ長 */
                        MHEAD2 = ABNFSTBL[18];/* 先頭アドレス */
                        
                    } else {
                        /* それ以外の場合 */
                        DSPLN2 = ABNFSTBL[(ABNF & 0x0F)][0];/* データ長 */
                        MHEAD2 = ABNFSTBL[(ABNF & 0x0F)];/* 先頭アドレス */
                        
                    }
                    
                    DLWSQ = 33;/* 2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                }
                /* 5回目ここまで */
                
            }
            
            break;
            
        case 16:
            /* 洗浄モード表示 */
            TSB_PF_DATA_PF4 = 1;/* バックライトオンする */
            
            if (DSPSSQ == 1) {
                /* 洗浄モード選択表示 */
                DSPLN1 = CLEAN1[0];/* データ長をセット */
                MHEAD1 = CLEAN1;/* 1行目先頭アドレスをセット */
                
                memcpy(DSPLB2, CLEAN2 + 1, 16);/* メモリーコピー */
                DSPLN2 = 16;/* データ長セット */
                if (EDDT[0] > 0) {
                    /* 「ハイ」選択中である場合 */
                    DSPLB2[2] = '(';
                    DSPLB2[5] = ')';
                    DSPLB2[9] = ' ';
                    DSPLB2[13] = ' ';/* shift-jis */
                    
                }
                
                MHEAD2 = DSPLB2;/* ｱﾄﾞﾚｽセット */
                MHEAD2--;/* アジャスト */
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                continue;/* 先頭に戻る */
                
            } else if (DSPSSQ == 2) {
                /* 洗浄工程表示である場合 */
                DSPLN1 = CLEAN4[0];/* データ長をセット */
                MHEAD1 = CLEAN4;/* 1行目先頭アドレスをセット */
                
                if (CCSEQ < 23) {
                    /* 洗浄工程中である場合 */
                    MHEAD2 = CLEAN5;/* 2行目の先頭アドレスをセット */
                    
                } else if (CCSEQ < 25) {
                    /* 1回目のすすぎ工程である場合 */
                    MHEAD2 = CLEAN6;/* 2行目の先頭アドレスをセット */
                    
                } else if (CCSEQ < 27) {
                    /* 2回目のすすぎ工程である場合 */
                    MHEAD2 = CLEAN7;/* 2行目の先頭アドレスをセット */
                    
                } else {
                    /* 洗浄終了である場合 */
                    MHEAD2 = CLEAN8;/* 2行目の先頭アドレスをセット */
                    
                }
                DSPLN2 = 16;/* データ長をセットする */
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                continue;/* 先頭に戻る */
                
            } else {
                /* 継続/中止選択画面である場合 */
                DSPLN1 = CLEAN4[0];/* データ長をセット */
                MHEAD1 = CLEAN4;/* 1行目先頭アドレスをセット */
                
                memcpy(DSPLB2, CLEAN3 + 1, 16);/* メモリーコピー */
                DSPLN2 = 16;/* データ長セット */
                if (EDDT[0] > 0) {
                    /* 「ハイ」選択中である場合 */
                    DSPLB2[1] = '(';
                    DSPLB2[6] = ')';
                    DSPLB2[8] = ' ';
                    DSPLB2[14] = ' ';/* shift-jis */
                    
                }
                
                MHEAD2 = DSPLB2;/* ｱﾄﾞﾚｽセット */
                MHEAD2--;/* アジャスト */
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                continue;/* 先頭に戻る */
                
            }
            
            break;
            
        case 17:
            /* 異常表示 */
            if (DSPSEQ == 0) {
                /* 最初である場合 */
                DSPSEQ = 1;/* 次のシーケンスにする */
                BZTM1 = BZTM2 = 0;/* ブザー制御用タイマをクリア */
                DSPTM1 = 0;/* タイマ―クリア */
                STSFL0 &= 0x7F;/* 設定モードの強制解除 */
                TSB_PC_DATA_PC7 = 1;/* 外部警報出力をオンする */
                MXOTN2 |= 0x80;/* ビットセット */
                
            }
            
            TSB_PF_DATA_PF4 = 0;/* バックライトオフ */
            DSPTM1++;/* タイマインクリメント */
            if (DSPTM1 >= 20) {
                /* タイマが1秒経過した場合 */
                DSPTM1 = 0;/* タイマクリア */
                
            }
            
            if (DSPTM1 < 10) {
                /* タイマが0.5秒未満である場合 */
                TSB_PF_DATA_PF4 = 1;/* バックライトオン */
                
            }
            
            DSPLN2 = ABNTOP[0];/* 異常表示2行目のデータ長をセット */
            MHEAD2 = ABNTOP;/* 先頭アドレスをセット */
            
            if (((SYKND0 & 0x10) > 0) && ((ABNFO & 0x0F) == 2)) {
                /* 蒸気式であり、チラー一括異常である場合 */
                DSPLN1 = ABNFSTBL[16][0];/* クーリングタワー異常のデータ長 */
                memcpy(DSPLB1, ABNFSTBL[16] + 2, 15);/* オフセット先頭アドレス */
                
            } else if (((SYKND0 & 0x10) > 0) && ((ABNFO & 0x0F) == 8)) {
                /* 蒸気式であり、常温槽水位電極棒異常である場合 */
                DSPLN1 = ABNFSTBL[17][0];/* 水位電極棒異常データ長 */
                memcpy(DSPLB1, ABNFSTBL[17] + 2, 15);/* オフセット先頭アドレス */
                
            } else if (((SYKND0 & 0x10) > 0) && ((ABNFO & 0x0F) == 9)) {
                /* 蒸気式であり、常温槽水槽温度センサー異常である場合 */
                DSPLN1 = ABNFSTBL[18][0];/* 温度センサー異常データ長 */
                memcpy(DSPLB1, ABNFSTBL[18] + 2, 15);/* オフセット先頭アドレス */
                
            } else {
                /* それ以外の場合 */
                DSPLN1 = ABNFSTBL[(ABNFO & 0x0F)][0];/* データ長 */
                memcpy(DSPLB1, ABNFSTBL[(ABNFO & 0x0F)] + 2, 15);/* オフセット先頭アドレス */
                
            }
            
            DSPLB1[10] = DSPLB1[11] = ' ';/* スペース文字 */
            DSPLB1[12] = DSPLB1[13] = ' ';/* スペース文字 */
            DSPLB1[14] = DSPLB1[15] = ' ';/* スペース文字 */
            
            MHEAD1 = DSPLB1;/* 先頭アドレスをセットする */
            MHEAD1 --;/* デクリメント */
            
            if ((ABNF & 0x80) == 0) {
                /* ブザー出力がオフである場合 */
                TSB_PC_DATA_PC7 = 0;/* 警報外部出力をオフにする */
                MXOTN2 &= 0x7F;/* ビットクリア */
                
            } else {
                /* ブザー出力がオンになっている場合 */
                if (BZSEQ == 0) {
                    /* ブザーシーケンスが0である場合 */
                    BZTM1 = BZTM2 = 0;/* タイマ―クリア */
                    BZSEQ++;/* 次のシーケンスへ */
                    
                }
                
                if (BZSEQ == 1) {
                    /* ブザーシーケンスが1の場合 */
                    TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                    BZTM1++;/* タイマをインクリメント */
                    if (BZTM1 >= 20) {
                        /* 1秒経過した場合 */
                        BZTM1 = 0;/* クリア */
                        BZSEQ++;/* 次へ */
                        
                    }
                    
                } else {
                    /* ブザーシーケンスが2の場合 */
                    TSB_PC_DATA_PC2 = 0;/* ブザーオフ */
                    BZTM1++;/* タイマをインクリメント */
                    if (BZTM1 >= 20) {
                        /* 1秒経過した場合 */
                        BZTM1 = 0;/* クリア */
                        BZSEQ = 1;/* 次へ */
                        
                    }
                    
                }
                
            }
            
            DLWSQ = 29;/* 1,2行目表示から開始 */
            i = 0;/* 再度ループに入りなおし可 */
            continue;/* 先頭に戻る */
            
            break;
            
        case 18:
            /* 設定表示 */
            TSB_PF_DATA_PF4 = 1;/* バックライトオンする */
            
            if (DSPSEQ == 0) {
                /* 1回目であった場合 */
                DSPTM1 = 0;/* タイマクリア */
                DSPSEQ = 1;/* 2回目の処理へ */
                
                DSPLN1 = DSMDHD[0];/* データ長をセット */
                MHEAD1 = DSMDHD;/* 先頭アドレスをセット */
                
                DSPLN2 = DSMD01[0];/* データ長をセット */
                MHEAD2 = DSMD01;/* 先頭アドレスをセット */
                
                DLWSQ = 25;/* 画面クリアと1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                continue;/* 先頭に戻る */
                
            }/* 2回目以降はここに抜ける */
            
            if (DSPSSQ == 0) {
                /* サブシーケンス番号が0である場合 */
                DSPLN2 = 16;/* データ長をセット */
                MHEAD2 = DSMDSTBL_1[(DSPSEQ - 1)];/* 先頭アドレスをセット */
                
                DLWSQ = 33;/* 2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                continue;/* 先頭に戻る */
                
            }/* サブシーケンス番号が1以上の場合はここに抜ける */
            
            if (DSPSEQ == 1) {
                /* パターン設定である場合 */
                DSPLN1 = DSMD06[0];/* データ長をセット */
                memcpy(DSPLB1, DSMD06 + 1, 16);/* メモリーコピー */
                
                DSPLN2 = 16;/* データ長をセット */
                memcpy(DSPLB2, DSMDSTBL_1[(DSPSSQ - 1) + 8] + 1, 16);/* メモリーコピー */
                
                DSPLB1[7] = NUMDEG[(NPAT + 1) % 10];/* 1行目データ（パターン番号）をセット */
                
                if (DSPSSQ == 2) {
                    /* 徐冷あり/なしの設定である場合 */
                    if ((EDDT[0] & (0x01 << NPAT)) == 0) {
                        /* 現在選択中のパターンが徐冷無しである場合 */
                        DSPLB2[13] = 'ﾅ';/* shift-jis */
                        DSPLB2[14] = 'ｼ';/* shift-jis */
                        
                    }
                    
                } else {
                    /* それ以外の設定である場合 */
                    DSPLB2[12] = NUMDEG2[((EDDT[0] % 100) / 10)];/* 設定値10の位(ゼロは表示しない) */
                    DSPLB2[13] =  NUMDEG[(EDDT[0] % 10)];/* 設定値1の位 */
                    
                }
                
                MHEAD1 = DSPLB1;/* メッセージの先頭アドレスをセット */
                MHEAD1 --;/* アドレスをデクリメント */
                MHEAD2 = DSPLB2;/* メッセージの先頭アドレスをセット */
                MHEAD2 --;/* アドレスをデクリメント */
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                continue;/* 先頭に戻る */
                
            } else if (DSPSEQ == 2) {
                /* 常温槽水槽温度設定である場合 */
                DSPLN1 = DSMD10[0];/* データ長 */
                MHEAD1 = DSMD10;/* 先頭アドレスセット */
                
                DSPLN2 = DSMD11[0];/* データ長 */
                memcpy(DSPLB2, DSMD11 + 1, 16);/* 常温槽設定水温セット */
                DSPLB2[12] = NUMDEG2[((EDDT[0] % 100) / 10)];/* 設定値10の位(ゼロは表示しない) */
                DSPLB2[13] =  NUMDEG[(EDDT[0] % 10)];/* 設定値1の位 */
                
                MHEAD2 = DSPLB2;/* 先頭アドレスセット */
                MHEAD2 --;/* アドレスをデクリメント */
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                continue;/* 先頭に戻る */
                
            } else if (DSPSEQ == 3) {
                /* 自動排水設定である場合 */
                DSPLN1 = DSMD11A[0];/* データ長をセットする */
                MHEAD1 = DSMD11A;/* 先頭アドレスをセット */
                
                if ((EDDT[0] & 0x80) == 0) {
                    /* 自動排水ありの場合 */
                    DSPLN2 = DSMD11B[0];/* データ長セット */
                    MHEAD2 = DSMD11B;/* 先頭アドレスセット */
                    
                } else {
                    /* 自動排水無しの場合 */
                    DSPLN2 = DSMD11C[0];/* データ長セット */
                    MHEAD2 = DSMD11C;/* 先頭アドレスセット */
                    
                }
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                continue;/* 先頭に戻る */
                
            } else if (DSPSEQ == 5) {
                /* チラー予約設定である場合 */
                DSPTM2++;/* タイマインクリメント */
                
                if (DSPTM2 >= 10) {
                    /* 0.5秒経過したとき */
                    DSPTM2 = 0;/* タイマクリア */
                    DCLNF ^= 0x01;/* XOR 0x01 */
                    
                }
                
                DSPLN1 = DSMD12[0];/* データ長 */
                memcpy(DSPLB1, DSMD12 + 1, 16);/* 現在時刻設定セット */
                
                DSPLN2 = DSMD13[0];/* データ長 */
                memcpy(DSPLB2, DSMD13 + 1, 16);/* チラー予約時刻設定セット */
                
                /* 現在時刻 */
                DSPLB1[11] = NUMDEG2[((RTCDT[2] % 100) / 10)];/* 10の位(ゼロは表示しない) */
                DSPLB1[12] =  NUMDEG[(RTCDT[2] % 10)];/* 1の位 */
                DSPLB1[14] = NUMDEG2[((RTCDT[1] % 100) / 10)];/* 10の位(ゼロは表示しない) */
                DSPLB1[15] =  NUMDEG[(RTCDT[1] % 10)];/* 1の位 */
                
                /* チラー予約設定時刻 */
                DSPLB2[11] = NUMDEG2[((EDDT[1] % 100) / 10)];/* 10の位(ゼロは表示しない) */
                DSPLB2[12] =  NUMDEG[(EDDT[1] % 10)];/* 1の位 */
                DSPLB2[14] =  NUMDEG[((EDDT[0] % 100) / 10)];/* 10の位 */
                DSPLB2[15] =  NUMDEG[(EDDT[0] % 10)];/* 1の位 */
                
                if ((DCLNF & 0x01) == 0) {
                    /* コロン点灯タイミングである場合 */
                    if (DSPSSQ == 1) {
                        /* 時の編集時である場合 */
                        DSPLB2[11] = DSPLB2[12] = ' ';/* 空白 */
                        
                    } else {
                        /* 分の編集時である場合 */
                        DSPLB2[14] = DSPLB2[15] = ' ';/* 空白 */
                        
                    }
                    
                } else {
                    /* コロン消灯タイミングである場合 */
                    DSPLB1[13] = ' ';/* 空白 */
                    
                }
                
                MHEAD1 = DSPLB1;/* 先頭アドレスセット */
                MHEAD2 = DSPLB2;
                MHEAD1 --;/* アドレスデクリメント */
                MHEAD2 --;/* アドレスデクリメント */
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                continue;/* 先頭に戻る */
                
            } else if (DSPSEQ == 6) {
                /* 時刻設定である場合 */
                DSPTM2++;/* タイマインクリメント */
                
                if (DSPTM2 >= 10) {
                    /* 0.5秒経過したとき */
                    DSPTM2 = 0;/* タイマクリア */
                    DCLNF ^= 0x01;/* XOR 0x01 */
                    
                }
                
                DSPLN1 = DSMD14[0];/* データ長をセット */
                MHEAD1 = DSMD14;/* 1行目のメッセージセット */
                
                DSPLN2 = DSMD15[0];/* データ長セット */
                memcpy(DSPLB2, DSMD15 + 1, 16);/* メモリーコピー */
                
                /* 年 */
                DSPLB2[2] =  NUMDEG[((EDDT[4] % 100) / 10)];/* 10の位 */
                DSPLB2[3] =  NUMDEG[(EDDT[4] % 10)];/* 1の位 */
                /* 月 */
                DSPLB2[5] =  NUMDEG[((EDDT[3] % 100) / 10)];/* 10の位 */
                DSPLB2[6] =  NUMDEG[(EDDT[3] % 10)];/* 1の位 */
                /* 日 */
                DSPLB2[8] =  NUMDEG[((EDDT[2] % 100) / 10)];/* 10の位 */
                DSPLB2[9] =  NUMDEG[(EDDT[2] % 10)];/* 1の位 */
                /* 時 */
                DSPLB2[11] = NUMDEG2[((EDDT[1] % 100) / 10)];/* 10の位 */
                DSPLB2[12] =  NUMDEG[(EDDT[1] % 10)];/* 1の位 */
                /* 分 */
                DSPLB2[14] =  NUMDEG[((EDDT[0] % 100) / 10)];/* 10の位 */
                DSPLB2[15] =  NUMDEG[(EDDT[0] % 10)];/* 1の位 */
                
                if ((DCLNF & 0x01) > 0) {
                    /* 消灯タイミングである場合 */
                    DSPLB2[((DSPSSQ - 1) * 3) + 2] = ' ';/* スペース */
                    DSPLB2[((DSPSSQ - 1) * 3) + 3] = ' ';/* スペース */
                    
                }
                
                MHEAD2 = DSPLB2;/* 先頭アドレスセット */
                MHEAD2 --;/* アドレスデクリメント */
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                continue;/* 先頭に戻る */
                
            } else {
                /* メンテナンスモードでの設定 */
                DSPLN1 = DSMD16[0];/* データ長をセットする */
                MHEAD1 = DSMD16;/* 先頭アドレスをセット */
                
                if (DSPSSQ == 1) {
                    /* 異常履歴表示モードである場合 */
                    if (DABHP == 0) {
                        /* 単純表示の場合 */
                        DSPLN2 = DSMD17[0];/* データ長をセット */
                        memcpy(DSPLB2, DSMD17 + 1, 16);/* メモリーコピー */
                        
                        DSPLB2[13] = NUMDEG[(ABNCUT & 0x0F) % 10];/* 異常履歴件数の数字をセット */
                        
                        MHEAD2 = DSPLB2;/* 先頭アドレスをセット */
                        MHEAD2 --;/* アドレスをデクリメント */
                        
                        DLWSQ = 29;/* 1,2行目表示から開始 */
                        i = 0;/* 再度ループに入りなおし可 */
                        continue;/* 先頭に戻る */
                        
                    } else {
                        /* 詳細表示である場合 */
                        REG_00 = 0;/* 一旦クリア */
                        if (DABHP == 1) {
                            /* 最新表示の場合 */
                            REG_00 = ABNCUT;/* 最新異常をセット */
                            REG_00 >>= 4;/* 4bit右シフト */
                            
                        } else if (DABHP == 2) {
                            /* 2番目の表示の場合 */
                            REG_00 = ABNH0;/* 2番目の異常をセット */
                            REG_00 &= 0x0F;/* マスク */
                            
                        } else if (DABHP == 3) {
                            /* 3番目の表示の場合 */
                            REG_00 = ABNH0;/* 3番目の異常をセット */
                            REG_00 >>= 4;/* 4bit右シフト */
                            
                        } else if (DABHP == 4) {
                            /* 4番目の表示の場合 */
                            REG_00 = ABNH1;/* 4番目の異常をセット */
                            REG_00 &= 0x0F;/* マスク */
                            
                        } else {
                            /* 5番目の表示の場合 */
                            REG_00 = ABNH1;/* 5番目の異常をセット */
                            REG_00 >>= 4;/* 4bit右シフト */
                            
                        }
                        
                        if (((SYKND0 & 0x10) > 0) && (REG_00 == 2)) {
                            /* 蒸気式で、チラー一括異常である場合 */
                            DSPLN2 = ABN16[0];
                            memcpy(DSPLB2, ABN16 + 1, 16);/* クーリングタワー一括異常をロード */
                            
                        } else if (((SYKND0 & 0x10) > 0) && (REG_00 == 8)) {
                            /* 蒸気式で、常温槽水位電極棒異常である場合 */
                            DSPLN2 = ABN17[0];
                            memcpy(DSPLB2, ABN17 + 1, 16);/* 水位電極棒異常をロード */
                            
                        } else if (((SYKND0 & 0x10) > 0) && (REG_00 == 9)) {
                            /* 蒸気式で、常温槽水温温度センサー異常である場合 */
                            DSPLN2 = ABN18[0];
                            memcpy(DSPLB2, ABN18 + 1, 16);/* 水温センサ異常をロード */
                            
                        } else {
                            /* それ以外の場合 */
                            DSPLN2 = ABNFSTBL[REG_00][0];/* データ長セット */
                            memcpy(DSPLB2, ABNFSTBL[REG_00] + 1, 16);/* 異常の文字列ロード */
                            
                        }
                        
                        DSPLB2[0] = NUMDEG[(DABHP % 10)];/* 履歴番号をshift-jisでセットする */
                        
                        MHEAD2 = DSPLB2;/* 先頭アドレスをセット */
                        MHEAD2 --;/* アドレスをデクリメント */
                        
                        DLWSQ = 29;/* 1,2行目表示から開始 */
                        i = 0;/* 再度ループに入りなおし可 */
                        continue;/* 先頭に戻る */
                        
                    }
                    
                } else if (DSPSSQ == 2) {
                    /* 予知警報画面である場合 */
                    if ((ABNF & 0x20) == 0) {
                        /* 予知警報ビットがクリアされている場合 */
                        YOTINO = 0;/* 予知表示ナンバーをクリア */
                        DSPLN2 = DSMD18[0];/* データ長セット */
                        memcpy(DSPLB2, DSMD18 + 1, 16);/* データをロード */
                        
                        DSPLB2[13] = 'ﾅ';
                        DSPLB2[14] = 'ｼ';/* shift-jis */
                        
                    } else if (((ABNF & 0x20) > 0) && (YOTINO == 0)) {
                        /* 予知警報ありで、詳細表示ではない場合 */
                        YOTINO = 0;/* 予知表示ナンバーをクリア */
                        DSPLN2 = DSMD18[0];/* データ長セット */
                        memcpy(DSPLB2, DSMD18 + 1, 16);/* データをロード */
                        
                    } else {
                        /* それ以外の場合 */
                        DSPLN2 = DALM1[0];/* データ長セット */
                        memcpy(DSPLB2, DALM1 + 1, 16);/* チラー能力低下を表示 */
                        
                    }
                    
                    MHEAD2 = DSPLB2;/* 先頭アドレスをセット */
                    MHEAD2 --;/* アドレスをデクリメント */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                } else if (DSPSSQ == 3) {
                    /* 稼働時間表示である場合 */
                    DSPLN2 = DSMD19[0];/* データ長をロード */
                    memcpy(DSPLB2, DSMD19 + 1, 16);/* 設備稼働時間文字列をメモリーコピー */
                    
                    REG_02 = (uint32_t)RTIMH2;/* 設備稼働時間の上位ロード */
                    REG_02 <<= 8;/* 8bit 左シフト */
                    REG_02 |= (uint32_t)RTIMH1;/* 設備稼働時間の下位ロード */
                    
                    DSPLB2[11] = NUMDEG[(REG_02 % 100000) / 10000];/* 1万の位セット */
                    DSPLB2[12] = NUMDEG[(REG_02 % 10000) / 1000];/* 1000の位セット */
                    DSPLB2[13] = NUMDEG[(REG_02 % 1000) / 100];/* 100の位セット */
                    DSPLB2[14] = NUMDEG[(REG_02 % 100) / 10];/* 10の位セット */
                    DSPLB2[15] = NUMDEG[REG_02 % 10];/* 1の位セット */
                    
                    MHEAD2 = DSPLB2;/* 先頭アドレスをセット */
                    MHEAD2 --;/* アドレスをデクリメント */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                } else if (DSPSSQ == 4) {
                    /* 常温槽温度表示である場合 */
                    DSPLN2 = DSMD20[0];/* データ長セット */
                    memcpy(DSPLB2, DSMD20 + 1, 16);/* メモリーコピー */
                    
                    DSPLB2[12] = NUMDEG2[(TMPDT2 % 100) / 10];/* 10の位 */
                    DSPLB2[13] = NUMDEG[TMPDT2 % 10];/* 1の位 */
                    
                    MHEAD2 = DSPLB2;/* 先頭アドレスをセット */
                    MHEAD2 --;/* アドレスをデクリメント */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                } else if (DSPSSQ == 5) {
                    /* 低温槽温度表示である場合 */
                    DSPLN2 = DSMD21[0];/* データ長セット */
                    memcpy(DSPLB2, DSMD21 + 1, 16);/* メモリーコピー */
                    
                    DSPLB2[12] = NUMDEG2[(TMPDT1 % 100) / 10];/* 10の位 */
                    DSPLB2[13] = NUMDEG[TMPDT1 % 10];/* 1の位 */
                    
                    MHEAD2 = DSPLB2;/* 先頭アドレスをセット */
                    MHEAD2 --;/* アドレスをデクリメント */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                } else if (DSPSSQ == 6) {
                    /* 品温表示である場合 */
                    DSPLN2 = DSMD22[0];/* データ長セット */
                    memcpy(DSPLB2, DSMD22 + 1, 16);/* メモリーコピー */
                    
                    DSPLB2[12] = NUMDEG2[(TMPDT0 % 100) / 10];/* 10の位 */
                    DSPLB2[13] = NUMDEG[TMPDT0 % 10];/* 1の位 */
                    
                    MHEAD2 = DSPLB2;/* 先頭アドレスをセット */
                    MHEAD2 --;/* アドレスをデクリメント */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                } else if (MMODE == 0) {
                    /* 総排水モードである場合 */
                    DSPLN2 = DSMD23[0];/* データ長セット */
                    MHEAD2 = DSMD23;/* 先頭アドレスをセットする */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                } else if (MMODE == 1) {
                    /* 常温槽排水モードである場合 */
                    DSPLN2 = DSMD231[0];/* データ長セット */
                    MHEAD2 = DSMD231;/* 先頭アドレスをセットする */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                } else if (MMODE == 2) {
                    /* 低温槽排水モードである場合 */
                    DSPLN2 = DSMD232[0];/* データ長セット */
                    MHEAD2 = DSMD232;/* 先頭アドレスをセットする */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    continue;/* 先頭に戻る */
                    
                } else {
                    /* それ以外の表示の場合 */
                    MMDTM++;/* タイマインクリメント */
                    if (MMDTM < 10) {
                        /* 0.5秒経過していない場合 */
                        TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                        STSFL1 |= 0x04;/* マニュアルモード操作エラーフラグをセット */
                        
                    } else {
                        /* 0.5秒以上経過した場合 */
                        TSB_PC_DATA_PC2 = 0;/* ブザーオフ */
                        STSFL1 &= 0xFB;/* マニュアルモード操作エラーフラグをクリア */
                        
                        if (MMDTM >= 20) {
                            /* 2秒以上経過した場合 */
                            MMODE = 0;/* マニュアルモードをクリア */
                            MMDTM = 0;/* タイマクリア */
                            INDPTM = 0;/* 通常表示復帰タイマをクリア */
                            
                            STSFL0 &= 0x7F;/* 設定モードをクリア */
                            DSPSEQ = 0;/* ディスプレーシーケンスをクリアする */
                            DSPSSQ = 0;/* サブシーケンスをクリア */
                            
                        }
                        
                    }
                    
                    DSPLN2 = DSMD23E[0];/* データ長セット */
                    MHEAD2 = DSMD23E;/* 先頭アドレスをセットする */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    
                }
                
            }
            
            break;
            
        case 19:
            /* セルフテストモードの表示 */
            TSB_PF_DATA_PF4 = 1;/* バックライトオン */
            
            if (TNO == 1) {
                /* セルフテストスタート時 */
                MXOTN6 = 0;/* LED出力を全てオフにする */
                MX6BLK = 0;/* LEDの点滅要求をリセット */
                MXOTN0 = 0;/* 出力をクリアする */
                MXOTN1 = 0;/* 出力をクリアする */
                MXOTN2 = 0;/* 出力をクリアする */
                
                TSB_PG->DATA = MXOTN0;/* 出力更新 */
                
                REG_00 = TSB_PN->DATA;/* データレジスタをロード */
                REG_00 &= 0x01;/* 0ビット目のみ残す */
                MXOTN1 |= REG_00;/* ORする */
                TSB_PN->DATA = MXOTN1;/* 出力更新 */
                
                REG_00 = TSB_PC->DATA;/* データレジスタをロード */
                REG_00 &= 0x07;/* 0～2ビット目のみ残す */
                MXOTN2 |= REG_00;/* ORする */
                TSB_PC->DATA = MXOTN2;/* 出力更新 */
                
                DSPLN1 = TSTTOP[0];/* データ長をセット */
                MHEAD1 = TSTTOP;/* 先頭アドレスセット */
                
                DSPLN2 = TSTMS0[0];/* データ長をセット */
                MHEAD2 = TSTMS0;/* 先頭アドレスセット */
                
                TSB_PC_DATA_PC2 = 1;/* ブザー出力をオンする */
                
                STSTM1++;/* タイマインクリメント */
                
                if (STSTM1 >= 10) {
                    /* 0.5秒経過した場合 */
                    TSB_PC_DATA_PC2 = 0;/* ブザー出力をオフする */
                    TNO = 2;/* LCD（VFD）確認の項目番号をセット */
                    SUBTNO = 0;/* サブテストナンバーをクリア */
                    
                }
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                
            } else if (TNO == 2) {
                /* LCD（VFD）の確認である場合 */
                REG_00 = SUBTNO;/* サブテスト番号をセット */
                REG_00 &= 0x0F;/* 必要な分だけマスク */
                DSPLB1[0] = NUMDEG[REG_00];/* 数値化…shift-jisセット */
                memcpy(DSPLB1 + 1, DSPLB1, 15);/* メモリーコピーで全部数字にする */
                
                STSTM1++;/* タイマをインクリメント */
                if (STSTM1 >= 10) {
                    /* 0.5秒経過した場合 */
                    STSTM1 = 0;/* タイマクリア */
                    SUBTNO++;/* インクリメント */
                    if ((SUBTNO & 0x0F) >= 10) {
                        /* 10以上になる場合 */
                        SUBTNO &= 0xF0;/* 下をクリア */
                        SUBTNO += 0x10;/* 加算する */
                        
                        if (SUBTNO >= 0x20) {
                            /* 2回終了時であった場合 */
                            SUBTNO = 0;/* サブ番号をクリア */
                            TNO = 3;/* 次の確認へ */
                            
                        }
                        
                    }
                    
                }
                
                DSPLN1 = DSPLN2 = 16;/* データ長をセットする */
                MHEAD1 = DSPLB1;/* 先頭アドレスセット */
                MHEAD2 = DSPLB1;/* 先頭アドレスセット（上と同じ） */
                MHEAD1 --;/* アドレスデクリメント */
                MHEAD2 --;/* アドレスデクリメント */
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                
            } else if (TNO == 3) {
                /* LEDの確認である場合 */
                if (SUBTNO > 0) {
                    /* 最初のシーケンスではない場合 */
                    TSB_PC_DATA_PC2 = 0;/* ブザーオフ */
                    
                } else {
                    /* 最初のシーケンスである場合 */
                    TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                    MXOTN6 |= 0x80;/* チラー予約LEDをオン */
                    SUBTNO = 1;/* サブ番号を次へ */
                    
                }
                
                STSTM1++;/* タイマインクリメント */
                if (STSTM1 >= 10) {
                    /* 0.5秒経過した場合 */
                    STSTM1 = 0;
                    MXOTN6 >>= 1;/* 1bitシフト */
                    
                    if (MXOTN6 == 0) {
                        /* LED出力終わりの場合 */
                        SUBTNO = 0;/* サブ番号をクリア */
                        TNO = 4;/* 次の確認項目へ */
                        
                    }
                    
                }
                
                DSPLN1 = TSTTOP[0];/* データ長をセット */
                MHEAD1 = TSTTOP;/* 先頭アドレスをセット */
                
                DSPLN2 = TSTMS1[0];/* データ長をセット */
                MHEAD2 = TSTMS1;/* 先頭アドレスをセット */
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                
            } else if (TNO == 4) {
                /* スイッチの確認である場合 */
                if (SUBTNO > 0) {
                    /* 最初のシーケンスではない場合 */
                    TSB_PC_DATA_PC2 = 0;/* ブザーオフ */
                    
                } else {
                    /* 最初のシーケンスである場合 */
                    TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                    BITPOS = 0x01;/* スイッチ入力のベリファイ用 */
                    SUBTNO = 1;/* サブ番号を次へ */
                    
                }
                
                if (MXSWO0 == BITPOS) {
                    /* スイッチ入力とベリファイ用の値が同じである場合 */
                    TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                    SUBTNO++;/* インクリメント */
                    BITPOS <<= 1;/* 1ﾋﾞｯﾄシフト */
                    
                    if (BITPOS == 0) {
                        /* シフト後の値がゼロの場合 */
                        SUBTNO = 0;/* クリア */
                        TNO = 5;/* 次の確認へ */
                        
                    }
                    
                }
                
                memcpy(DSPLB2, TSTMS2 + 1, 16);/* メモリーコピー */
                DSPLB2[2] = NUMDEG[SUBTNO];/* 数字を文字列化してイン */
                DSPLN2 = 16;/* データ長セット */
                
                DSPLN1 = TSTTOP[0];/* データ長をセット */
                MHEAD1 = TSTTOP;/* 先頭アドレスセット */
                MHEAD2 = DSPLB2;/* 先頭アドレスをセット */
                MHEAD2 --;/* デクリメント */
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                
            } else if (TNO == 5) {
                /* 出力の確認である場合 */
                TSB_PC_DATA_PC2 = 0;/* ブザー出力をクリアする */
                
                if (SUBTNO == 0) {
                    /* 最初のシーケンスであった場合 */
                    SUBTNO = 1;/* 1をセット */
                    MXOTN0 = 0x01;/* 常温真空弁出力をセットする */
                    
                }
                
                STSTM1++;/* タイマをインクリメント */
                if (STSTM1 >= 10) {
                    /* 0.5秒経過した場合 */
                    TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                    STSTM1 = 0;/* タイマクリア */
                    if (MXOTN0 > 0) {
                        /* 何かビットが立っている場合 */
                        MXOTN0 <<= 1;/* 1ﾋﾞｯﾄ左シフト */
                        if (MXOTN0 == 0) {
                            /* ゼロになる場合 */
                            MXOTN1 = 0x02;/* 冷却槽排水弁出力セット */
                            
                        }
                        
                    } else if ((MXOTN1 & 0xFE) > 0) {
                        /* 何かビットが立っている場合 */
                        MXOTN1 <<= 1;/* 1ﾋﾞｯﾄシフト */
                        
                        if (MXOTN1 == 0) {
                            /* ゼロになる場合 */
                            MXOTN2 = 0x08;/* チラー循環Pをセット */
                            
                        }
                        
                    } else if ((MXOTN2 & 0xF8) > 0) {
                        /* 何かビットが立っている場合 */
                        MXOTN2 <<= 1;/* 1ビットシフト */
                        
                        if (MXOTN2 == 0) {
                            /* ゼロになる場合 */
                            SUBTNO = 0;/* テストナンバークリア */
                            TNO = 6;/* 次の確認へ */
                            
                        }
                        
                    }
                    
                }/* 0.5秒経過していない場合はここに抜ける */
                
                /* 出力の更新 */
                TSB_PG->DATA = MXOTN0;/* 出力更新 */
                
                REG_00 = TSB_PN->DATA;/* 現在の出力を読み出す */
                REG_00 &= 0x01;/* 0ビット目は残すようにマスク */
                REG_00 |= (MXOTN1 & 0xFE);/* ORする */
                TSB_PN->DATA = REG_00;/* 出力更新 */
                
                REG_00 = TSB_PC->DATA;/* 現在の出力を読み出す */
                REG_00 &= 0x07;/* 0ビット目は残すようにマスク */
                REG_00 |= (MXOTN2 & 0xF8);/* ORする */
                TSB_PC->DATA = REG_00;/* 出力更新 */
                
                /* 画面データをセットする */
                DSPLN1 = TSTTOP[0];/* データ長をセット */
                MHEAD1 = TSTTOP;/* 先頭アドレスをセット */
                DSPLN2 = TSTMS3[0];/* データ長をセット */
                MHEAD2 = TSTMS3;/* 先頭アドレスをセット */
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                
            } else if (TNO == 6) {
                /* 外部入力の確認である場合 */
                TSB_PC_DATA_PC2 = 0;/* ブザー出力をクリアする */
                
                if (SUBTNO == 0) {
                    /* 最初のシーケンスであった場合 */
                    SUBTNO = 1;/* 1をセット */
                    BITPOS = 0x01;/* 比較用変数セット */
                    
                }
                
                if (SUBTNO < 9) {
                    /* MXINO0の確認の場合 */
                    if (((MXINO0 ^ 0x0F) == BITPOS) && ((MXINO1 ^ 0xF0) == 0)) {
                        /* 一致する場合(反転が必要な箇所はXORで反転させている) */
                        TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                        
                        SUBTNO++;/* テストナンバーインクリメント */
                        if (SUBTNO == 6) {
                            /* ナンバーが6になる場合 */
                            SUBTNO++;/* 更にインクリメント */
                            BITPOS <<= 1;/* 1ﾋﾞｯﾄ左シフト */
                            
                        }
                        BITPOS <<= 1;/* 1ビット左シフト */
                        
                        if (BITPOS == 0) {
                            /* ゼロになった場合 */
                            BITPOS = 1;/* 再度1セット */
                            
                        }
                        
                    }/* 一致しなければここに抜ける */
                    
                } else {
                    /* MXINO1の確認の場合 */
                    if (((MXINO1 ^ 0xF0) == BITPOS) && ((MXINO0 ^ 0x0F) == 0)) {
                        /* 一致する場合 */
                        TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                        
                        SUBTNO++;/* テストナンバーインクリメント */
                        if (SUBTNO == 10) {
                            /* ナンバーが10になる場合 */
                            SUBTNO += 3;/* +3する */
                            BITPOS = 0x08;/* ディップスイッチ確認位置-1のポジションにする */
                            
                        }
                        BITPOS <<= 1;/* 1ﾋﾞｯﾄ左シフト */
                        
                        if (BITPOS == 0) {
                            /* ゼロになった場合 */
                            SUBTNO = 0;/* サブ番号をクリア */
                            TNO = 7;/* 次の確認へ */
                            
                        }
                        
                    }/* 一致しない場合はここに抜ける */
                    
                }
                
                DSPLN1 = TSTTOP[0];/* データ長セット */
                MHEAD1 = TSTTOP;/* 先頭アドレスセット */
                
                if (SUBTNO < 13) {
                    /* ディップスイッチの確認以前である場合 */
                    DSPLN2 = TSTMS4[0];/* データ長をセット */
                    MHEAD2 = TSTMS4;/* 先頭アドレスセット */
                    
                } else {
                    /* ディップスイッチの確認である場合 */
                    DSPLN2 = TSTMSB[0];/* データ長セット */
                    memcpy(DSPLB2, TSTMSB + 1, 16);/* メモリーコピー */
                    DSPLB2[14] = NUMDEG[ (SUBTNO - 12)/*4 - (SUBTNO - 13)*/ ];/* 数字を文字にコンバート */
                    
                    MHEAD2 = DSPLB2;/* 先頭アドレスセット */
                    MHEAD2 --;/* アドレスデクリメント */
                    
                }
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                
            } else if (TNO == 7) {
                /* A/D595 ALARMの確認である場合 */
                TSB_PC_DATA_PC2 = 0;/* ブザー出力をクリアする */
                
                if (((MXINO1 ^ 0xF0) == 0x02) && ((MXINO0 ^ 0x0F) == 0)) {
                    /* 断線信号が入った場合 */
                    TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                    SUBTNO = 0;/* クリア */
                    TNO = 8;/* 次の確認へ */
                    
                }
                
                DSPLN1 = TSTTOP[0];/* データ長セット */
                MHEAD1 = TSTTOP;/* 先頭アドレスセット */
                
                DSPLN2 = TSTMS5[0];/* データ長セット */
                MHEAD2 = TSTMS5;/* 先頭アドレスセット */
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                
            } else if (TNO == 8) {
                /* 通信ポートの確認である場合 */
                TSB_PC_DATA_PC2 = 0;/* ブザー出力をクリアする */
                
                if (SUBTNO == 0) {
                    /* 最初の場合 */
                    TBUF0[0] = 0x0F;/* 送信データセット */
                    TBUF0[1] = 0x70;/* 送信データセット */
                    
                    /* 送信前準備 */
                    TXPTR0 = 0;/* ポインタゼロ */
                    TXLEN0 = TXSET0 = 2;/* 2バイトセット */
                    TXFLG0 |= 0x11;/* 送信中、送信フラグをセット */
                    TXFLG0 &= 0xDF;/* 送信バッファエンプティをクリア */
                    
                    /* データを格納し送信処理 */
                    TSB_SC0->BUF = TBUF0[TXPTR0];/* バッファにセット */
                    
                    TXPTR0++;/* ﾎﾟｲﾝﾀをｲﾝｸﾘﾒﾝﾄ */
                    TXLEN0--;/* ﾃﾞｸﾘﾒﾝﾄ */
                    TXSET0--;/* ﾃﾞｸﾘﾒﾝﾄ */
                    
                    SUBTNO++;/* インクリメント */
                    
                } else if (SUBTNO == 1) {
                    /* 受信待ち */
                    STSTM1++;/* タイマーをインクリメント */
                    if (STSTM1 >= 10) {
                        /* 0.5秒経過した場合 */
                        STSTM1 = 0;/* タイマクリア */
                        SUBTNO++;/* インクリメント */
                        
                    }
                    
                } else {
                    /* 結果確認 */
                    if (((TXFLG0 & 0x20) > 0) && ((TXFLG0 & 0x11) == 0) && (RXLEN0 == 2)
                    && (RBUF0[(RXPTR0 + 0) & 0x0FF] == 0x0F) && (RBUF0[(RXPTR0 + 1) & 0x0FF] == 0x70)) {
                        /* 送信バッファエンプティで、送信中送信フラグがオフであり、受信数が2で
                        期待通りの受信データであった場合 */
                        TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                        SUBTNO = 0;/* クリア */
                        TNO = 9;/* 次の確認処理へ */
                        
                    } else {
                        /* おかしかった場合 */
                        SUBTNO = 0;/* クリア */
                        
                    }
                    
                    RXPTR0 = 0;/* 受信ポインタクリア */
                    RXLEN0 = 0;/* 受信長クリア */
                    
                    TXFLG0 &= 0xEE;/* 送信フラグクリア */
                    TXFLG0 |= 0x20;/* 送信バッファエンプティをセット */
                    TXPTR0 = 0;/* 送信ポインタクリア */
                    TXLEN0 = TXSET0 = 0;/* 送信長、送信セット数クリア */
                    
                }
                
                DSPLN1 = TSTTOP[0];/* データ長セット */
                MHEAD1 = TSTTOP;/* 先頭アドレスセット */
                DSPLN2 = TSTMS6[0];/* データ長セット */
                MHEAD2 = TSTMS6;/* 先頭アドレスセット */
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                
            } else if (TNO == 9) {
                /* RTCの確認である場合 */
                TSB_PC_DATA_PC2 = 0;/* ブザー出力をクリアする */
                
                if (SUBTNO == 0) {
                    /* 1回目確認である場合 */
                    CRTCDT = RTCDT[0];/* 秒をロード */
                    SUBTNO = 1;/* 1をセット */
                    
                } else {
                    /* 2回目の確認である場合 */
                    STSTM1++;/* タイマをインクリメント */
                    if (STSTM1 >= 30) {
                        /* 1.5秒以上経過した場合 */
                        STSTM1 = 0;/* タイマークリア */
                        SUBTNO = 0;/* サブテストナンバークリア */
                        
                        if (RTCDT[0] >= CRTCDT) {
                            /* 現在秒の方が過去秒以上である場合 */
                            REG_00 = (RTCDT[0] - CRTCDT);/* 引きます */
                            
                        } else {
                            /* 逆の場合 */
                            REG_00 = (RTCDT[0] + 60);/* 60秒加算セット */
                            REG_00 -= CRTCDT;/* 過去秒を引く */
                            
                        }
                        
                        if ((REG_00 >= 1) && (REG_00 < 3)) {
                            /* 秒の差が1以上、3未満である場合 */
                            TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                            TNO = 10;/* 次の確認処理へ */
                            
                        }
                        
                    }
                    
                }
                
                DSPLN1 = TSTTOP[0];/* データ長セット */
                MHEAD1 = TSTTOP;/* 先頭アドレスセット */
                DSPLN2 = TSTMS7[0];/* データ長セット */
                MHEAD2 = TSTMS7;/* 先頭アドレスセット */
                
                DLWSQ = 29;/* 1,2行目表示から開始 */
                i = 0;/* 再度ループに入りなおし可 */
                
            } else if (TNO == 10) {
                /* NV-RAMの確認1である場合 */
                TSB_PC_DATA_PC2 = 0;/* ブザー出力をクリアする */
                
                if (SUBTNO == 0) {
                    /* サブテストナンバーが０の場合 */
                    STSTM1++;/* タイマインクリメント */
                    if (STSTM1 >= 20) {
                        /* 1秒経過した場合 */
                        TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                        STSTM1 = 0;/* タイマクリア */
                        SUBTNO = 1;/* 次の処理へ */
                        
                    }
                    
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
                    *HENNP[29] = 11;/* ☆現在のパターン選択値を"11"にする（テスト用）☆ */
                    
                    DSPLN1 = TSTTOP[0];/* データ長セット */
                    MHEAD1 = TSTTOP;/* 先頭アドレスセット */
                    DSPLN2 = TSTMS8[0];/* データ長セット */
                    MHEAD2 = TSTMS8;/* 先頭アドレスセット */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    
                } else {
                    /* サブテストナンバーが1の場合 */
                    DSPLN2 = TSTMSC[0];/* データ長セット */
                    MHEAD2 = TSTMSC;/* 先頭アドレスセット */
                    
                    DLWSQ = 33;/* 2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    
                }
                
            } else if (TNO == 11) {
                /* NV-RAMの確認2である場合 */
                TSB_PC_DATA_PC2 = 0;/* ブザー出力をクリアする */
                
                if (SUBTNO == 0) {
                    /* 最初のシーケンスである場合 */
                    STSTM1++;/* タイマインクリメント */
                    if (STSTM1 >= 10) {
                        /* 0.5秒経過した場合 */
                        TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                        STSTM1 = 0;/* タイマクリア */
                        SUBTNO = 1;/* 次へ */
                        
                    }
                    
                    DSPLN1 = TSTTOP[0];/* データ長をセット */
                    MHEAD1 = TSTTOP;/* 先頭アドレスをセット */
                    DSPLN2 = TSTMS8[0];/* データ長をセット */
                    MHEAD2 = TSTMS8;/* 先頭アドレスをセット */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループ入りなおし可 */
                    
                } else {
                    /* データ読み出しチェック時である場合 */
                    NPAT = 0;/* "0"にする */
                    
                    REG_00 = 0;/* 一旦クリア */
                    for (uint8_t j = 0;j < 31;j++) {
                        /* 31回繰り返し */
                        if (NVRMITB[j] == *HENNP[j]) {
                            /* 値が一致する場合 */
                            /* 何もしない */
                            
                        } else {
                            /* 値が一致しない場合 */
                            REG_00 ++;/* インクリメント */
                            
                        }
                        
                    }/* 繰り返しここまで */
                    
                    if (REG_00 == 0) {
                        /* 何も異常が無かった場合 */
                        STSTM1++;/* タイマインクリメント */
                        if (STSTM1 >= 10) {
                            /* 0.5秒経過した場合 */
                            TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                            STSTM1 = 0;/* タイマクリア */
                            SUBTNO = 0;/* サブシーケンスクリア */
                            TNO = 12;/* 次の確認へ */
                            
                        }
                        
                        
                        DSPLN2 = TSTMS9[0];/* データ長セット */
                        MHEAD2 = TSTMS9;/* 先頭アドレスセット */
                        
                    } else {
                        /* 何か異常があった場合 */
                        DSPLN2 = TSTMSA[0];/* データ長セット */
                        MHEAD2 = TSTMSA;/* 先頭アドレスセット */
                        
                    }
                    
                    DLWSQ = 33;/* 2行目表示から開始 */
                    i = 0;/* 再度ループに入りなおし可 */
                    
                }
                
            } else if (TNO == 12) {
                /* HACCP出力接点の確認である場合 */
                TSB_PC_DATA_PC2 = 0;/* ブザー出力をクリアする */
                
                if (SUBTNO == 0) {
                    /* 最初のシーケンスである場合 */
                    STSTM1++;/* タイマインクリメント */
                    if (STSTM1 >= 10) {
                        /* 0.5秒経過した場合 */
                        TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                        STSTM1 = 0;/* タイマクリア */
                        SUBTNO = 1;/* 次へ */
                        
                    }
                    
                    TSB_PE->DATA &= 0xF3;/* 冷却運転・異常出力をオフにする */
                    
                    DSPLN1 = TSTTOP[0];/* データ長をセット */
                    MHEAD1 = TSTTOP;/* 先頭アドレスをセット */
                    DSPLN2 = TSTMSD[0];/* データ長をセット */
                    MHEAD2 = TSTMSD;/* 先頭アドレスをセット */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループ入りなおし可 */
                    
                } else if (SUBTNO == 1) {
                    /* 2番目のシーケンスである場合 */
                    STSTM1++;/* タイマインクリメント */
                    if (STSTM1 >= 40) {
                        /* 2秒経過した場合 */
                        TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                        STSTM1 = 0;/* タイマクリア */
                        SUBTNO = 2;/* 次へ */
                        
                    }
                    
                    TSB_PE->DATA |= 0x04;/* 冷却運転出力をオンにする */
                    
                    DSPLN1 = TSTTOP[0];/* データ長をセット */
                    MHEAD1 = TSTTOP;/* 先頭アドレスをセット */
                    DSPLN2 = TSTMSD[0];/* データ長をセット */
                    
                    memcpy(DSPLB2, TSTMSD + 1, 16);/* メモリーコピー */
                    DSPLB2[11] = NUMDEG[1];/* 文字データ「１」セット */
                    MHEAD2 = DSPLB2;/* 先頭アドレスをセット */
                    MHEAD2 --;/* アドレスデクリメント */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループ入りなおし可 */
                    
                } else if (SUBTNO == 2) {
                    /* 3番目のシーケンスである場合 */
                    STSTM1++;/* タイマインクリメント */
                    if (STSTM1 >= 40) {
                        /* 2秒経過した場合 */
                        TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                        STSTM1 = 0;/* タイマクリア */
                        SUBTNO = 3;/* 次の確認へ */
                        
                    }
                    
                    TSB_PE->DATA |= 0x08;/* 異常出力をオンにする */
                    
                    DSPLN1 = TSTTOP[0];/* データ長をセット */
                    MHEAD1 = TSTTOP;/* 先頭アドレスをセット */
                    DSPLN2 = TSTMSD[0];/* データ長をセット */
                    
                    memcpy(DSPLB2, TSTMSD + 1, 16);/* メモリーコピー */
                    DSPLB2[11] = NUMDEG[2];/* 文字データ「２」セット */
                    MHEAD2 = DSPLB2;/* 先頭アドレスをセット */
                    MHEAD2 --;/* アドレスデクリメント */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループ入りなおし可 */
                    
                } else if (SUBTNO == 3) {
                    /* 4番目のシーケンスである場合 */
                    STSTM1++;/* タイマインクリメント */
                    if (STSTM1 >= 40) {
                        /* 2秒経過した場合 */
                        TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                        STSTM1 = 0;/* タイマクリア */
                        SUBTNO = 4;/* 次の確認へ */
                        
                    }
                    
                    TSB_PE->DATA &= 0xFB;/* 冷却出力をオフにする */
                    
                    DSPLN1 = TSTTOP[0];/* データ長をセット */
                    MHEAD1 = TSTTOP;/* 先頭アドレスをセット */
                    DSPLN2 = TSTMSD[0];/* データ長をセット */
                    
                    memcpy(DSPLB2, TSTMSD + 1, 16);/* メモリーコピー */
                    DSPLB2[11] = NUMDEG[3];/* 文字データ「３」セット */
                    MHEAD2 = DSPLB2;/* 先頭アドレスをセット */
                    MHEAD2 --;/* アドレスデクリメント */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループ入りなおし可 */
                    
                } else if (SUBTNO == 4) {
                    /* 5番目のシーケンスである場合 */
                    STSTM1++;/* タイマインクリメント */
                    if (STSTM1 >= 50) {
                        /* 2.5秒経過した場合 */
                        TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                        STSTM1 = 0;/* タイマクリア */
                        SUBTNO = 0;/* サブシーケンスクリア */
                        TNO = 13;/* 次の確認へ */
                        
                    }
                    
                    TSB_PE->DATA &= 0xF3;/* 冷却・異常出力をオフにする */
                    
                    DSPLN1 = TSTTOP[0];/* データ長をセット */
                    MHEAD1 = TSTTOP;/* 先頭アドレスをセット */
                    DSPLN2 = TSTMSD[0];/* データ長をセット */
                    
                    memcpy(DSPLB2, TSTMSD + 1, 16);/* メモリーコピー */
                    DSPLB2[11] = NUMDEG[4];/* 文字データ「４」セット */
                    MHEAD2 = DSPLB2;/* 先頭アドレスをセット */
                    MHEAD2 --;/* アドレスデクリメント */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループ入りなおし可 */
                    
                }
                
            } else if (TNO == 13) {
                /* DAC出力値 / 停電入力の確認である場合 */
                TSB_PC_DATA_PC2 = 0;/* ブザー出力をクリアする */
                
                if (SUBTNO == 0) {
                    /* 最初のシーケンスである場合 */
                    STSTM1++;/* タイマインクリメント */
                    if (STSTM1 >= 10) {
                        /* 0.5秒経過した場合 */
                        TSB_PC_DATA_PC2 = 1;/* ブザーオン */
                        STSTM1 = 0;/* タイマクリア */
                        SUBTNO = 1;/* 次へ */
                        
                    }
                    
                    DSPLN1 = TSTTOP[0];/* データ長をセット */
                    MHEAD1 = TSTTOP;/* 先頭アドレスをセット */
                    DSPLN2 = TSTMSE[0];/* データ長をセット */
                    MHEAD2 = TSTMSE;/* 先頭アドレスをセット */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループ入りなおし可 */
                    
                } else {
                    /* 2番目以降のシーケンスである場合 */
                    DSPLN1 = TSTTOP[0];/* データ長をセット */
                    MHEAD1 = TSTTOP;/* 先頭アドレスをセット */
                    DSPLN2 = TSTMSE[0];/* データ長をセット */
                    
                    memcpy(DSPLB2, TSTMSE + 1, 16);/* メモリーコピー */
                    DSPLB2[4] = NUMDEG[((DACOUT % 10000) / 1000)];/* 数字の文字データセット1 */
                    DSPLB2[5] = NUMDEG[((DACOUT % 1000) / 100)];/* 数字の文字データセット2 */
                    DSPLB2[6] = NUMDEG[((DACOUT % 100) / 10)];/* 数字の文字データセット3 */
                    DSPLB2[7] = NUMDEG[(DACOUT % 10)];/* 数字の文字データセット4 */
                    
                    if ((MXINO1 & 0x08) > 0) {
                        /* 停電入力がある場合 */
                        DSPLB2[13] = 'N';/* ONのN */
                        DSPLB2[14] = ' ';/* OFFのFを消す */
                        
                    }
                    
                    MHEAD2 = DSPLB2;/* 先頭アドレスをセット */
                    MHEAD2 --;/* アドレスデクリメント */
                    
                    DLWSQ = 29;/* 1,2行目表示から開始 */
                    i = 0;/* 再度ループ入りなおし可 */
                    
                }
                
            }
            
            break;/* 抜けます */
            
        case 25:
            /* 1行目2行目の表示をクリアする処理 */
            TSB_PF->CR &= 0xF0;/* 出力機能をオフ */
            TSB_PF->IE |= 0x0F;/* 入力機能をオン */
            TSB_PA_DATA_PA6 = 1;/* RWをオンする */
            
            DLWSQ++;/* 番号インクリメント */
            /* ブレークは書かない */
            
        case 26:
            /* ビジーフラグチェック */
            if ((EPBSY() & 0x08) > 0) {
                /* ビジーフラグがオンである場合 */
                OS_LoadTime->NEXT_LOAD_Time[5] = 0;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[5] = 0x80;/* 遅延フラグ建築 */
                
                FLZFG |= 0x01;/* フラグセット */
                if (FLZTM >= 60000) {
                    /* 1分以上経過した場合 */
                    FLZFG = DLWSQ = 0;/* やり直し */
                    FLZTM = 0;/* タイマクリア */
                    
                }
                
                return;/* 戻る */
                
            }
            
            FLZFG = 0;/* クリア */
            DLWSQ++;/* 番号インクリメント */
            /* ブレークは書かない */
            
        case 27:
            /* 画面クリアコマンドの送信 */
            TSB_PA_DATA_PA6 = 0;/* RWオフにする */
            TSB_PF->IE &= 0xF0;/* LCDデータバスを非入力にする */
            TSB_PF->CR |= 0x0F;/* LCDデータバスを出力にする */
            TSB_PF->DATA &= 0xF0;/* 一旦マスク */
            TSB_PF->DATA |= 0x00;/* LCDデータバス出力をセット、クリアコマンド */
            
            EPLS();/* Eをパルス出力する */
            
            TSB_PF->DATA |= 0x01;/* LCDデータバス出力をセット、クリアコマンド */
            
            EPLS();/* Eをパルス出力する */
            
            TSB_PF->CR &= 0xF0;/* 出力機能をオフ */
            TSB_PF->IE |= 0x0F;/* 入力機能をオン */
            TSB_PA_DATA_PA6 = 1;/* RWをオンする */
            
            DLWSQ++;/* 番号インクリメント */
            /* ブレークは書かない */
            
        case 28:
            /* ビジーフラグチェック */
            if ((EPBSY() & 0x08) > 0) {
                /* ビジーフラグがオンである場合 */
                OS_LoadTime->NEXT_LOAD_Time[5] = 0;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[5] = 0x80;/* 遅延フラグ建築 */
                
                FLZFG |= 0x01;/* フラグセット */
                if (FLZTM >= 60000) {
                    /* 1分以上経過した場合 */
                    FLZFG = DLWSQ = 0;/* やり直し */
                    FLZTM = 0;/* タイマクリア */
                    
                }
                
                return;/* 戻る */
                
            }
            
            FLZFG = 0;/* クリア */
            DLWSQ = 31;/* 番号を31にする */
            i = 0;/* クリア */
            continue;/* ループ先頭に戻る */
            /* ブレークは書かない */
            
        case 29:
            /* 1,2行目を表示する処理 */
            /* まずは1行目の表示カーソルを合わせる */
            TSB_PA_DATA_PA6 = 0;/* RWオフにする */
            TSB_PF->IE &= 0xF0;/* LCDデータバスを非入力にする */
            TSB_PF->CR |= 0x0F;/* LCDデータバスを出力にする */
            TSB_PF->DATA &= 0xF0;/* 一旦マスク */
            TSB_PF->DATA |= 0x08;/* LCDデータバス出力をセット、1行目コマンド */
            
            EPLS();/* Eをパルス出力する */
            
            TSB_PF->DATA &= 0xF0;/* 一旦マスク */
            TSB_PF->DATA |= 0x00;/* LCDデータバス出力をセット、1行目コマンド */
            
            EPLS();/* Eをパルス出力する */
            
            TSB_PF->CR &= 0xF0;/* 出力機能をオフ */
            TSB_PF->IE |= 0x0F;/* 入力機能をオン */
            TSB_PA_DATA_PA6 = 1;/* RWをオンする */
            
            DLWSQ++;/* 番号インクリメント */
            /* ブレークは書かない */
            
        case 30:
            /* ビジーフラグの確認 */
            if ((EPBSY() & 0x08) > 0) {
                /* ビジーフラグがオンである場合 */
                OS_LoadTime->NEXT_LOAD_Time[5] = 0;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[5] = 0x80;/* 遅延フラグ建築 */
                
                FLZFG |= 0x01;/* フラグセット */
                if (FLZTM >= 60000) {
                    /* 1分以上経過した場合 */
                    FLZFG = DLWSQ = 0;/* やり直し */
                    FLZTM = 0;/* タイマクリア */
                    
                }
                
                return;/* 戻る */
                
            }
            
            FLZFG = 0;/* クリア */
            DLWSQ++;/* 番号インクリメント */
            /* ブレークは書かない */
            
        case 31:
            /* 1行目に文字を表示する処理 */
            TSB_PA_DATA_PA6 = 0;/* R/Wをオフ */
            TSB_PA_DATA_PA7 = 1;/* RSをオン */
            
            MHEAD1++;/* アドレスインクリメント */
            
            REG_00 = *MHEAD1;/* 実体の数字をセット */
            REG_01 = REG_00;/* もうひとつにもコピー */
            REG_00 >>= 4;/* 4ビット右シフト */
            REG_00 &= 0x0F;/* 必要な分だけマスク */
            REG_01 &= 0x0F;/* 必要な分だけマスク */
            
            TSB_PF->IE &= 0xF0;/* LCDデータバスを非入力にする */
            TSB_PF->CR |= 0x0F;/* LCDデータバスを出力にする */
            TSB_PF->DATA &= 0xF0;/* 一旦マスク */
            TSB_PF->DATA |= REG_00;/* LCDデータバス出力をセット、データ上位 */
            
            EPLS();/* Eをパルス出力する */
            
            TSB_PF->DATA &= 0xF0;/* 一旦マスク */
            TSB_PF->DATA |= REG_01;/* LCDデータバス出力をセット、データ下位 */
            
            EPLS();/* Eをパルス出力する */
            
            TSB_PF->CR &= 0xF0;/* 出力機能をオフ */
            TSB_PF->IE |= 0x0F;/* 入力機能をオン */
            TSB_PA_DATA_PA6 = 1;/* RWをオンする */
            TSB_PA_DATA_PA7 = 0;/* RSをオフする */
            
            DLWSQ++;/* 番号インクリメント */
            /* ブレークは書かない */
            
        case 32:
            /* ビジーフラグの確認とまだ文字表示残があるか確認 */
            if ((EPBSY() & 0x08) > 0) {
                /* ビジーフラグがオンである場合 */
                OS_LoadTime->NEXT_LOAD_Time[5] = 0;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[5] = 0x80;/* 遅延フラグ建築 */
                
                FLZFG |= 0x01;/* フラグセット */
                if (FLZTM >= 60000) {
                    /* 1分以上経過した場合 */
                    FLZFG = DLWSQ = 0;/* やり直し */
                    FLZTM = 0;/* タイマクリア */
                    
                }
                
                return;/* 戻る */
                
            }
            
            FLZFG = 0;/* クリア */
            if (DSPLN1 > 0) {
                /* 1以上の場合 */
                DSPLN1--;/* デクリメント */
                
            }
            
            if (DSPLN1 == 0) {
                /* 送信するものが0である場合 */
                DLWSQ++;/* 番号インクリメント */
                
            } else {
                /* 送信するものがまだある場合 */
                DLWSQ = 31;/* 再度31にセット */
                i = 0;/* 繰り返しできる様に値をクリア */
                continue;/* ループの先頭へ */
                
            }
            /* ブレークは書かない */
            
        case 33:
            /* 2行目を表示する処理 */
            /* 2行目の表示カーソルを合わせる */
            TSB_PA_DATA_PA6 = 0;/* RWオフにする */
            TSB_PA_DATA_PA7 = 0;/* RSオフにする */
            TSB_PF->IE &= 0xF0;/* LCDデータバスを非入力にする */
            TSB_PF->CR |= 0x0F;/* LCDデータバスを出力にする */
            TSB_PF->DATA &= 0xF0;/* 一旦マスク */
            TSB_PF->DATA |= 0x0C;/* LCDデータバス出力をセット、2行目コマンド */
            
            EPLS();/* Eをパルス出力する */
            
            TSB_PF->DATA &= 0xF0;/* 一旦マスク */
            TSB_PF->DATA |= 0x00;/* LCDデータバス出力をセット、2行目コマンド */
            
            EPLS();/* Eをパルス出力する */
            
            TSB_PF->CR &= 0xF0;/* 出力機能をオフ */
            TSB_PF->IE |= 0x0F;/* 入力機能をオン */
            TSB_PA_DATA_PA6 = 1;/* RWをオンする */
            
            DLWSQ++;/* 番号インクリメント */
            /* ブレークは書かない */
            
        case 34:
            /* ビジーフラグの確認 */
            if ((EPBSY() & 0x08) > 0) {
                /* ビジーフラグがオンである場合 */
                OS_LoadTime->NEXT_LOAD_Time[5] = 0;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[5] = 0x80;/* 遅延フラグ建築 */
                
                FLZFG |= 0x01;/* フラグセット */
                if (FLZTM >= 60000) {
                    /* 1分以上経過した場合 */
                    FLZFG = DLWSQ = 0;/* やり直し */
                    FLZTM = 0;/* タイマクリア */
                    
                }
                
                return;/* 戻る */
                
            }
            
            FLZFG = 0;/* クリア */
            DLWSQ++;/* 番号インクリメント */
            /* ブレークは書かない */
            
        case 35:
            /* 2行目に文字を表示する処理 */
            TSB_PA_DATA_PA6 = 0;/* R/Wをオフ */
            TSB_PA_DATA_PA7 = 1;/* RSをオン */
            
            MHEAD2++;/* アドレスインクリメント */
            
            REG_00 = *MHEAD2;/* 実体の数字をセット */
            REG_01 = REG_00;/* もうひとつにもコピー */
            REG_00 >>= 4;/* 4ビット右シフト */
            REG_00 &= 0x0F;/* 必要な分だけマスク */
            REG_01 &= 0x0F;/* 必要な分だけマスク */
            
            TSB_PF->IE &= 0xF0;/* LCDデータバスを非入力にする */
            TSB_PF->CR |= 0x0F;/* LCDデータバスを出力にする */
            TSB_PF->DATA &= 0xF0;/* 一旦マスク */
            TSB_PF->DATA |= REG_00;/* LCDデータバス出力をセット、データ上位 */
            
            EPLS();/* Eをパルス出力する */
            
            TSB_PF->DATA &= 0xF0;/* 一旦マスク */
            TSB_PF->DATA |= REG_01;/* LCDデータバス出力をセット、データ下位 */
            
            EPLS();/* Eをパルス出力する */
            
            TSB_PF->CR &= 0xF0;/* 出力機能をオフ */
            TSB_PF->IE |= 0x0F;/* 入力機能をオン */
            TSB_PA_DATA_PA6 = 1;/* RWをオンする */
            TSB_PA_DATA_PA7 = 0;/* RSをオフする */
            
            DLWSQ++;/* 番号インクリメント */
            /* ブレークは書かない */
            
        case 36:
            /* ビジーフラグの確認とまだ文字表示残があるか確認 */
            if ((EPBSY() & 0x08) > 0) {
                /* ビジーフラグがオンである場合 */
                OS_LoadTime->NEXT_LOAD_Time[5] = 0;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[5] = 0x80;/* 遅延フラグ建築 */
                
                FLZFG |= 0x01;/* フラグセット */
                if (FLZTM >= 60000) {
                    /* 1分以上経過した場合 */
                    FLZFG = DLWSQ = 0;/* やり直し */
                    FLZTM = 0;/* タイマクリア */
                    
                }
                
                return;/* 戻る */
                
            }
            
            FLZFG = 0;/* クリア */
            if (DSPLN2 > 0) {
                /* 1以上の場合 */
                DSPLN2--;/* デクリメント */
                
            }
            
            if (DSPLN2 == 0) {
                /* 送信するものが0である場合 */
                DLWSQ = 10;/* 次回の開始位置をセットする */
                i = 1;/* 明示的に繰り返しを終わらせる */
                
            } else {
                /* 送信するものがまだある場合 */
                DLWSQ = 35;/* 再度35にセット */
                i = 0;/* 繰り返しできる様に値をクリア */
                continue;/* ループの先頭へ */
                
            }
            
            break;/* 抜けます */
            
        default:
            /* 当てはまらない場合はここに抜ける */
            DLWSQ = 0;/* リセットするぜ */
            break;/* 抜ける */
            
        }/* スイッチ文はここまで */
        
    }/* ループはここまで */
    
    OS_LoadTime->NEXT_LOAD_Time[5] = 50;/* 次回呼び出し時間 */
    OS_LoadTime->TSK_COND[5] = 0xA0;/* 遅延フラグ建築 */
    
    return;/* 戻る */
    
}

/***
 * 題名：Eパルス出力ルーチン
 * 作成者：浅尾　享一
 * LCD/VFD表示時に必要なEパルス処理
***/
void EPLS(void)
{
    TSB_PA_DATA_PA5 = 1;/* E信号をHIGHにする */
    for (uint8_t i = 0;i < 5;i++) {
        __NOP();/* タイミングずらし */
        
    }
    TSB_PA_DATA_PA5 = 0;/* E信号をLOWにする */
    
    return;/* 戻る */
    
}

/***
 * 題名：Pビジーフラグのチェックルーチン
 * 作成者：浅尾　享一
 * LCD/VFD処理時に必要なPビジーフラグチェックルーチン
***/
uint8_t EPBSY(void)
{
    uint8_t REG_00 = 0;/* 一時変数をセット */
    
    TSB_PA_DATA_PA5 = 1;/* E信号をHIGHにする */
    for (uint8_t i = 0;i < 5;i++) {
        __NOP();/* タイミングずらし */
        
    }
    REG_00 = TSB_PF->DATA;/* PFポートのデータをロード */
    REG_00 &= 0x0F;/* 4bitにマスク */
    TSB_PA_DATA_PA5 = 0;/* E信号をLOWにする */
    
    for (uint8_t i = 0;i < 5;i++) {
        __NOP();/* タイミングずらし */
        
    }
    TSB_PA_DATA_PA5 = 1;/* E信号をHIGHにする */
    for (uint8_t i = 0;i < 5;i++) {
        __NOP();/* タイミングずらし */
        
    }
    TSB_PA_DATA_PA5 = 0;/* E信号をLOWにする */
    
    return REG_00;/* 値をセットして戻る */
    
}

/***
 * 題名：メモリークリア受付時の表示
 * 作成者：浅尾　享一
 * メモリークリア受付中表示を出す
***/
void MCLRMS(void)
{
    TSB_PF_DATA_PF4 = 1;/* バックライト出力をオンする */
    DSPLN1 = MCLMS0[0];/* 1行目のデータ長をセットする */
    MHEAD1 = &(MCLMS0[0]);/* メッセージの先頭アドレスをセットする */
    
    DSPLN2 = MCLMS1[0];/* 1行目のデータ長をセットする */
    MHEAD2 = &(MCLMS1[0]);/* メッセージの先頭アドレスをセットする */
    
    TSB_PC_DATA_PC2 = 1;/* ブザーオン */
    
    STSTM1++;/* タイマインクリメント */
    if (STSTM1 >= 10) {
        /* 0.5秒経過した場合 */
        STSTM1 = 0;/* タイマクリア */
        TSB_PC_DATA_PC2 = 0;/* ブザーオフ */
        STSFL1 &= 0xFD;/* メモリークリア受付中をクリア */
        
        MCLAST = 0;/* メモリークリア受付タイマをクリアする */
        
    }
    
    return;/* 戻る */
    
}