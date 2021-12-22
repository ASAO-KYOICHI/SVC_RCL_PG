/***
 * 題名：通信制御アプリ
 * 作成者：浅尾　享一
 * 通信制御を行うアプリケーション
***/
#include "tmpm380_COMCNT.h"

const uint8_t ABNHEAD[] = {
    /* 異常時に返信するデータ */
    0x15, '0', '0', 'F', 'F', '0', '6'
};

void COMCNT(void)
{
    OS_TSKCon1 *OS_LoadTime;/* OS設定値を読むためのポインタ構造体に名前をつける */
    OS_LoadTime = OSTSKCon1S();/* 関数経由でポインタをOSparamにアクセス */
    
    uint8_t  REG_00[4] = {
        /* 一時変数 */
        0, 0, 0, 0
    };
    uint16_t REG_01 = 0;/* 一時変数 */
    uint16_t REG_02 = 0;/* 一時変数 */
    uint16_t REG_03 = 0;/* 一時変数 */
    
    if (((STSFL1 & 0x03) > 0) || ((STSFL0 & 0x02) == 0)) {
        /* セルフテストか、メモリークリア中である、又は初期化中である場合 */
        OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* 次回呼び出し時間 */
        OS_LoadTime->TSK_COND[7] = 0xA0;/* 遅延フラグ建築 */
        
        return;
        
    }
    
    if ((USTS0 & 0x04) > 0) {
        /* 何かしらエラーであった場合 */
        RXPTR0 += RXLEN0;/* 受信ポインタを移動 */
        RXLEN0 = 0;/* 受信長をクリア */
        USTS0 &= 0xFB;/* フラグクリア */
        
        if ((TXFLG0 & 0x11) == 0) {
            /* 送信中ではない場合 */
            for (uint8_t i = 0;i < 7;i++) {
                /* 7回繰り返し処理 */
                TBUF0[(TXPTR0 + i) & 0x0FF] = ABNHEAD[i];/* データを入れます */
                
            }
            
            TXLEN0 = 7;
            TXSET0 = 7;/* 送信長、データ長セット */
            TXFLG0 &= 0xDF;/* エンプティフラグをクリア */
            TXFLG0 |= 0x11;/* 送信開始セット */
            
            /* データを格納し送信処理 */
            TSB_SC0->BUF = TBUF0[TXPTR0];/* バッファにセット */
            
            TXPTR0++;/* ﾎﾟｲﾝﾀをｲﾝｸﾘﾒﾝﾄ */
            TXLEN0--;/* ﾃﾞｸﾘﾒﾝﾄ */
            TXSET0--;/* ﾃﾞｸﾘﾒﾝﾄ */
            
            OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* 次回呼び出し時間 */
            OS_LoadTime->TSK_COND[7] = 0xA0;/* 遅延フラグ建築 */
            
            return;
            
        }
        
    }/* エラーが無い場合はここに抜ける */
    
    
    while (RXLEN0 > 0) {
        /* 受信長が0以上ある場合はループします */
        if ((USTS0 & 0x08) > 0) {
            /* バッファｵｰﾊﾞｰﾗﾝであった場合 */
            RXLEN0 = 0;/* 受信データ長クリア */
            RXPTR0 = 0;/* 受信ﾎﾟｲﾝﾀクリア */
            SCRSTT = 0;/* 工程ｸﾘｱ */
            USTS0 &= 0xF7;/* バッファｵｰﾊﾞｰﾗﾝクリア */
            
        }
        
        if ((SCRSTT == 0) && (RXLEN0 == 0)) {
            /* フレームﾁｪｯｸ中ではなく、受信ﾃﾞｰﾀがない場合 */
            OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* 次回呼び出し時間 */
            OS_LoadTime->TSK_COND[7]       = 0xA0;/* 遅延フラグ建築 */
            
            return;/* 戻ります */
        }
        
        if (SCRSTT == 0) {
            /* フレームﾁｪｯｸ中ではない場合 */
            if (RXLEN0 < 10) {
                /* 受信長が10バイト未満である場合 */
                OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[7]       = 0xA0;/* 遅延フラグ建築 */
                
                return;/* 戻ります */
                
            }
            
            if (RBUF0[RXPTR0] != 0x05) {
                /* ENQではない場合 */
                SCRSTT = 0;/* 工程ｸﾘｱ */
                RXPTR0++;/* ｲﾝｸﾘﾒﾝﾄ */
                RXLEN0--;/* 受信長をﾃﾞｸﾘﾒﾝﾄ */
                continue;/* ループの頭へ */
                
            }
            
            /* ENQを発見した場合 */
            KYNO = (((uint16_t)RBUF0[(RXPTR0 + 1) & 0x0FF] << 8) | ((uint16_t)RBUF0[(RXPTR0 + 2) & 0x0FF]));/* 局番号を保存 */
            PCNO = (((uint16_t)RBUF0[(RXPTR0 + 3) & 0x0FF] << 8) | ((uint16_t)RBUF0[(RXPTR0 + 4) & 0x0FF]));/* PC番号を保存 */
            
            if ((RBUF0[(RXPTR0 + 5) & 0x0FF] == 0x50) && (RBUF0[(RXPTR0 + 6) & 0x0FF] == 0x43)) {
                /* PCコマンドである場合 */
                if (RBUF0[(RXPTR0 + 7) & 0x0FF] != 0x30) {
                    /* 伝文ウェイトが0ms以外の場合 */
                    SCRSTT = 0;/* 工程ｸﾘｱ */
                    RXPTR0++;/* ｲﾝｸﾘﾒﾝﾄ */
                    RXLEN0--;/* 受信長をﾃﾞｸﾘﾒﾝﾄ */
                    continue;/* ループの頭へ */
                    
                }
                
                /* SUM値をロード */
                REG_01 = (((uint16_t)RBUF0[(RXPTR0 + 8) & 0x0FF] << 8) | ((uint16_t)RBUF0[(RXPTR0 + 9) & 0x0FF]));/* SUM値を保存 */
                
                /* 受信したﾃﾞｰﾀのSUMを算出 */
                REG_00[0]  = (uint8_t)(KYNO & 0x00FF);
                REG_00[0] += (uint8_t)(KYNO >> 8);
                REG_00[0] += (uint8_t)(PCNO & 0x00FF);
                REG_00[0] += (uint8_t)(PCNO >> 8);
                REG_00[0] += 0x50;
                REG_00[0] += 0x43;
                REG_00[0] += 0x30;
                
                if (REG_01 != ASCICV(REG_00[0])) {
                    /* SUM値が一致しない場合 */
                    SCRSTT = 0;/* 工程ｸﾘｱ */
                    RXPTR0++;/* ｲﾝｸﾘﾒﾝﾄ */
                    RXLEN0--;/* 受信長をﾃﾞｸﾘﾒﾝﾄ */
                    continue;/* ループの頭へ */
                    
                }
                
                TBUF0[TXPTR0] = 0x02;/* STXをセット */
                TBUF0[(TXPTR0 + 1) & 0x0FF] = (uint8_t)(KYNO >> 8);/* 上位をセット */
                TBUF0[(TXPTR0 + 2) & 0x0FF] = (uint8_t)(KYNO & 0x00FF);/* 下位をセット */
                TBUF0[(TXPTR0 + 3) & 0x0FF] = (uint8_t)(PCNO >> 8);/* 上位をセット */
                TBUF0[(TXPTR0 + 4) & 0x0FF] = (uint8_t)(PCNO & 0x00FF);/* 下位をセット */
                TBUF0[(TXPTR0 + 5) & 0x0FF] = 0x38;/* PC型名"0x3832"をセット */
                TBUF0[(TXPTR0 + 6) & 0x0FF] = 0x32;
                TBUF0[(TXPTR0 + 7) & 0x0FF] = 0x03;/* ETXをセット */
                
                REG_00[0] = 0;/* 一度クリアする */
                for (uint8_t i = 1;i < 8;i++) {
                    /* 7回繰り返し実行 */
                    REG_00[0] += TBUF0[(TXPTR0 + i) & 0x0FF];/* バッファの値を加算する */
                    
                }
                
                REG_01 = ASCICV(REG_00[0]);/* SUMをｱｽｷｰ化する */
                TBUF0[(TXPTR0 + 8) & 0x0FF] = (uint8_t)(REG_01 & 0x00FF);/* SUMセット1 */
                TBUF0[(TXPTR0 + 9) & 0x0FF] = (uint8_t)(REG_01 >> 8);/* SUMセット2 */
                TBUF0[(TXPTR0 + 10) & 0x0FF] = 0x0D;/* CRセット*/
                TBUF0[(TXPTR0 + 11) & 0x0FF] = 0x0A;/* LFセット */
                
                TXLEN0 += 12;/* 送信長セット */
                TXSET0 += 12;/* 送信長セット */
                TXFLG0 |= 0x11;/* 送信要求、開始セット */
                TXFLG0 &= 0xDF;/* 送信ｷｬﾝｾﾙをクリアする */
                
                TSB_SC0->BUF = TBUF0[TXPTR0];/* 送信物セット */
                TXPTR0++;/* ｲﾝｸﾘﾒﾝﾄ */
                TXPTR0 &= 0xFF;/* MAX FIX */
                TXLEN0--;/* ﾃﾞｸﾘﾒﾝﾄ */
                TXSET0--;/* ﾃﾞｸﾘﾒﾝﾄ */
                
                RXLEN0 = 0;/* 受信長クリア */
                RXPTR0 = 0;/* 受信ﾎﾟｲﾝﾀクリア */
                
                SCRSTT = 0;/* 工程ｸﾘｱ */
                
                OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[7]       = 0xA0;/* 遅延フラグ建築 */
                
                return;/* 戻ります */
                
            } else {
                /* PCコマンドではない場合 */
                /* フレーム開始の判定 */
                if (RXLEN0 < 17) {
                    /* 受信長が17バイト未満である場合 */
                    OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* 次回呼び出し時間 */
                    OS_LoadTime->TSK_COND[7]       = 0xA0;/* 遅延フラグ建築 */
                    
                    return;/* 戻ります */
                    
                }
                
                if ((RBUF0[(RXPTR0 + 5) & 0x0FF] == 'W') && (RBUF0[(RXPTR0 + 6) & 0x0FF] == 'R')) {
                    /* コマンド正常"WR"である場合 */
                    if ( ((RBUF0[(RXPTR0 + 7) & 0x0FF] >= 0x30) && (RBUF0[(RXPTR0 + 7) & 0x0FF] <= 0x39))
                    || ((RBUF0[(RXPTR0 + 7) & 0x0FF] >= 0x41) && (RBUF0[(RXPTR0 + 7) & 0x0FF] <= 0x46)) ) {
                        /* 伝文ウェイトがASCIIで0～Fの範囲である場合 */
                        /* 何もせず通過する */
                    } else {
                        /* 伝文ウェイトが違反する場合 */
                        SCRSTT = 0;/* 工程ｸﾘｱ */
                        RXPTR0++;/* ｲﾝｸﾘﾒﾝﾄ */
                        RXLEN0--;/* 受信長をﾃﾞｸﾘﾒﾝﾄ */
                        continue;/* ループの頭へ */
                        
                    }
                    
                    if (RBUF0[(RXPTR0 + 8) & 0x0FF] != 'W') {
                        /* W領域以外である場合 */
                        SCRSTT = 0;/* 工程ｸﾘｱ */
                        RXPTR0++;/* ｲﾝｸﾘﾒﾝﾄ */
                        RXLEN0--;/* 受信長をﾃﾞｸﾘﾒﾝﾄ */
                        continue;/* ループの頭へ */
                        
                    }
                    
                    for (REG_00[1] = 9;REG_00[1] < 15;REG_00[1]++) {
                        /* 4回繰り返し */
                        if ( ((RBUF0[(RXPTR0 + REG_00[1]) & 0x0FF] >= 0x30) && (RBUF0[(RXPTR0 + REG_00[1]) & 0x0FF] <= 0x39))
                            /* ASCIIで0～9の範囲である場合 */
                        || ((RBUF0[(RXPTR0 + REG_00[1]) & 0x0FF] >= 0x41) && (RBUF0[(RXPTR0 + REG_00[1]) & 0x0FF] <= 0x46)) ) {
                            /* ASCIIでA～Fの範囲である場合 */
                            /* 何もしない */
                            
                        } else {
                            /* 範囲外の場合 */
                            break;
                            
                        }
                        
                    }
                    
                    if (REG_00[1] <= 14) {
                        /* どっかで範囲外によってブレークした場合 */
                        SCRSTT = 0;/* 工程ｸﾘｱ */
                        RXPTR0++;/* ｲﾝｸﾘﾒﾝﾄ */
                        RXLEN0--;/* 受信長をﾃﾞｸﾘﾒﾝﾄ */
                        continue;/* ループの頭へ */
                        
                    } else {
                        /* 最後まで確認した後の場合(REG_00[1]が15) */
                        REG_00[1]--;/* 14にする */
                        
                    }
                    
                    FLMSTS = 17;/* コマンドのﾊﾞｲﾄ数セット */
                    SCRSTT = 0x01;/* WRコマンドセット */
                    
                    continue;/* ループの頭へ */
                    
                } else {
                    /* コマンド異常の場合 */
                    SCRSTT = 0;/* 工程ｸﾘｱ */
                    RXPTR0++;/* ｲﾝｸﾘﾒﾝﾄ */
                    RXLEN0--;/* 受信長をﾃﾞｸﾘﾒﾝﾄ */
                    continue;/* ループの頭へ */
                    
                }
                
            }
            
        } else {
            /* フレームﾁｪｯｸ中の場合 */
            /* コマンド採用の判定 */
            if (RXLEN0 < FLMSTS) {
                /* フレーム終了までデータを取り込めていない場合 */
                OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* 次回呼び出し時間 */
                OS_LoadTime->TSK_COND[7]       = 0xA0;/* 遅延フラグ建築 */
                
                return;
                
            }
            
            REG_00[0] = 0;/* クリア */
            for (uint8_t i = 0;i < FLMSTS;i++) {
                /* フレーム終了値 - 2まで繰り返し */
                if ((i + 3) < FLMSTS) {
                    /* SUM算出エリアの場合 */
                    REG_00[0] += RBUF0[(RXPTR0 + 1 + i) & 0x0FF];/* SUM値を算出します */
                    
                } else if ((i + 3) == FLMSTS) {
                    /* SUM算出エリア超1回目（受信ﾃﾞｰﾀのSUM値） */
                    REG_00[1] = RBUF0[(RXPTR0 + 1 + i) & 0x0FF];/* SUM値をセットします */
                    
                } else if ((i + 2) == FLMSTS) {
                    /* SUM算出エリア超1回目（受信ﾃﾞｰﾀのSUM値） */
                    REG_00[2] = RBUF0[(RXPTR0 + 1 + i) & 0x0FF];/* SUM値をセットします */
                    
                }
                
            }
            
            REG_01 = ASCICV(REG_00[0]);/* アスキー化します */
            if ((REG_00[1] != (uint8_t)(REG_01 & 0x00FF)) || (REG_00[2] != (uint8_t)(REG_01 >> 8))) {
                /* SUM値が一致しない場合 */
                SCRSTT &= 0x00;/* 工程ｸﾘｱ */
                RXLEN0 -= 17;/* 減算する */
                RXPTR0 += 17;
                
                continue;
                
            }
            
            break;/* SUMが正常ならループを抜ける */
            
        }
        
    }/* whileによる受信データ確認はここまで */
    
    if ((RXLEN0 == 0) || (SCRSTT == 0)) {
        /* 受信長が０、又は工程がクリアされている場合 */
        OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* 次回呼び出し時間 */
        OS_LoadTime->TSK_COND[7]       = 0xA0;/* 遅延フラグ建築 */
        
        return;
        
    }
    
    /* 各通信毎の処理 */
    TBUF0[(TXPTR0 + 1) & 0x0FF] = (uint8_t)(KYNO >> 8);/* 上位8ビットをセット */
    TBUF0[(TXPTR0 + 2) & 0x0FF] = (uint8_t)(KYNO & 0x00FF);/* 下位8ビットをセット */
    TBUF0[(TXPTR0 + 3) & 0x0FF] = (uint8_t)(PCNO >> 8);/* 上位8ビットをセット */
    TBUF0[(TXPTR0 + 4) & 0x0FF] = (uint8_t)(PCNO & 0x00FF);/* 下位8ビットをセット */
    
    /* WRコマンド */
    REG_00[0] = RBUF0[(RXPTR0 + 9) & 0x0FF];/* 受信バッファ_アドレスの1000の位 */
    REG_00[1] = RBUF0[(RXPTR0 + 10) & 0x0FF];/* 受信バッファ_アドレスの100の位 */
    REG_00[2] = BNRCV(REG_00[0], REG_00[1]);/* ASCII -> Binary変換 */
    REG_01 = ((uint16_t)REG_00[2] * 100);/* ×100したものをセット */
    
    REG_00[0] = RBUF0[(RXPTR0 + 11) & 0x0FF];/* 受信バッファ_アドレスの10の位 */
    REG_00[1] = RBUF0[(RXPTR0 + 12) & 0x0FF];/* 受信バッファ_アドレスの1の位 */
    REG_00[2] = BNRCV(REG_00[0], REG_00[1]);/* ASCII -> Binary変換 */
    REG_01 += (uint16_t)REG_00[2];/* 10と1の位を加算セット */
    
    if (REG_01 > 0x0F) {
        /* アドレスを確認し、読み込み範囲外である場合 */
        RXLEN0 = RXPTR0 = 0;/* 受信長、受信ﾎﾟｲﾝﾀクリアする */
        SCRSTT = 0;/* 工程ｸﾘｱ */
        
        TBUF0[TXPTR0] = 0x15;/* "NAK"をセット */
        TBUF0[(TXPTR0 + 5) & 0x0FF] = 0x30;
        TBUF0[(TXPTR0 + 6) & 0x0FF] = 0x36;
        TBUF0[(TXPTR0 + 7) & 0x0FF] = 0x0D;
        TBUF0[(TXPTR0 + 8) & 0x0FF] = 0x0A;
        TXLEN0 += 9;/* 送信数セット */
        TXSET0 += 9;/* 送信数セット */
        TXFLG0 |= 0x11;/* 送信要求＆開始をセット */
        TXFLG0 &= 0xDF;/* 送信ｷｬﾝｾﾙ解除 */
        
        TSB_SC0->BUF = TBUF0[TXPTR0];/* バッファにセット */
        
        TXPTR0++;/* ﾎﾟｲﾝﾀをｲﾝｸﾘﾒﾝﾄ */
        TXPTR0 &= 0xFF;/* MAX FIX */
        TXLEN0--;/* ﾃﾞｸﾘﾒﾝﾄ */
        TXSET0--;/* ﾃﾞｸﾘﾒﾝﾄ */
        
        OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* 次回呼び出し時間 */
        OS_LoadTime->TSK_COND[7]       = 0xA0;/* 遅延フラグ建築 */
        
        return;
        
    }
    
    REG_02 = REG_01;/* 先頭アドレスをキープしとく */
    REG_01 = 0;/* クリア */
    REG_00[0] = RBUF0[(RXPTR0 + 13) & 0x0FF];/* 受信バッファ_読み込みデータ数の10の位 */
    REG_00[1] = RBUF0[(RXPTR0 + 14) & 0x0FF];/* 受信バッファ_読み込みデータ数の1の位 */
    REG_00[2] = HEXCV(REG_00[0], REG_00[1]);/* ASCII -> HEX16進(ほぼBinary)変換 */
    REG_01 = (uint16_t)REG_00[2];/* 10と1の位をセット */
    
    if ((REG_01 == 0) || (REG_01 > 16) || ((REG_01 + REG_02) > 0x10)) {
        /* 読み込みﾃﾞｰﾀ数を確認し、読み込み範囲外の数値である場合 */
        /* 先頭アドレス　+　読み込みﾃﾞｰﾀ数が15以上になる場合 */
        RXLEN0 = RXPTR0 = 0;/* 受信長、受信ﾎﾟｲﾝﾀクリアする */
        SCRSTT = 0;/* 工程ｸﾘｱ */
        
        TBUF0[TXPTR0] = 0x15;/* "NAK"をセット */
        TBUF0[(TXPTR0 + 5) & 0x0FF] = 0x30;
        TBUF0[(TXPTR0 + 6) & 0x0FF] = 0x36;
        TBUF0[(TXPTR0 + 7) & 0x0FF] = 0x0D;
        TBUF0[(TXPTR0 + 8) & 0x0FF] = 0x0A;
        TXLEN0 += 9;/* 送信数セット */
        TXSET0 += 9;/* 送信数セット */
        TXFLG0 |= 0x11;/* 送信要求＆開始をセット */
        TXFLG0 &= 0xDF;/* 送信ｷｬﾝｾﾙ解除 */
        
        TSB_SC0->BUF = TBUF0[TXPTR0];/* バッファにセット */
        
        TXPTR0++;/* ﾎﾟｲﾝﾀをｲﾝｸﾘﾒﾝﾄ */
        TXPTR0 &= 0xFF;/* MAX FIX */
        TXLEN0--;/* ﾃﾞｸﾘﾒﾝﾄ */
        TXSET0--;/* ﾃﾞｸﾘﾒﾝﾄ */
        
        OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* 次回呼び出し時間 */
        OS_LoadTime->TSK_COND[7]       = 0xA0;/* 遅延フラグ建築 */
        
        return;
        
    }
    
    /* メモリーマップ更新 */
    for (uint8_t i = 0;i < 14;i++) {
        /* 14回繰り返し */
        if (i == 5) {
            /* W0005(異常履歴1上位と冷却開始時刻「分」下位）のメモリであった場合 */
            /* 下位のみBCDに直す必要があるメモリの場合 */
            REG_00[0] = *HENNP[(2 * i) + 3];/* 下位セット */
            REG_00[1] = REG_00[0];/* コピー */
            REG_00[0] = ((REG_00[0] % 100) / 10);/* 10の位のみ抽出 */
            REG_00[0] <<= 4;/* 上位に4ビットシフト */
            REG_00[1] = (REG_00[1] % 10);/* 1の位のみ抽出 */
            REG_00[0] |= REG_00[1];/* 10と1の位を合わせてBCD完成 */
            MEMRY01[i] = (uint16_t)REG_00[0];/* まず下位セット */
            
            REG_03 = (uint16_t)*HENNP[(2 * i) + 2];
            MEMRY01[i] |= (REG_03 << 8);/* がっちゃんこで上位セット */
            
        } else if (i == 7) {
            /* W0007(冷却終了時刻「時」上位と積算稼働時間「分」下位)のメモリであった場合 */
            /* 上位のみBCDに直す必要があるメモリの場合 */
            MEMRY01[i] = (uint16_t)*HENNP[(2 * i) + 3];/* 下位セット */
            
            REG_00[0] = *HENNP[(2 * i) + 2];/* 上位セット */
            REG_00[1] = REG_00[0];/* コピー */
            REG_00[0] = ((REG_00[0] % 100) / 10);/* 10の位のみ抽出 */
            REG_00[0] <<= 4;/* 上位に4ビットシフト */
            REG_00[1] = (REG_00[1] % 10);/* 1の位のみ抽出 */
            REG_00[0] |= REG_00[1];/* 10と1の位を合わせてBCD完成 */
            REG_03 = (uint16_t)REG_00[0];/* 次に上位セット */
            MEMRY01[i] |= (REG_03 << 8);/* がっちゃんこで上位セット */
            
        } else if ( (i == 6) || (i == 9) ) {
            /* 上位/下位ともにBCDに直す必要があるメモリの場合 */
            REG_00[0] = *HENNP[(2 * i) + 3];/* 下位セット */
            REG_00[1] = REG_00[0];/* コピー */
            REG_00[0] = ((REG_00[0] % 100) / 10);/* 10の位のみ抽出 */
            REG_00[0] <<= 4;/* 上位に4ビットシフト */
            REG_00[1] = (REG_00[1] % 10);/* 1の位のみ抽出 */
            REG_00[0] |= REG_00[1];/* 10と1の位を合わせてBCD完成 */
            MEMRY01[i] = (uint16_t)REG_00[0];/* まず下位セット */
            
            REG_00[0] = *HENNP[(2 * i) + 2];/* 上位セット */
            REG_00[1] = REG_00[0];/* コピー */
            REG_00[0] = ((REG_00[0] % 100) / 10);/* 10の位のみ抽出 */
            REG_00[0] <<= 4;/* 上位に4ビットシフト */
            REG_00[1] = (REG_00[1] % 10);/* 1の位のみ抽出 */
            REG_00[0] |= REG_00[1];/* 10と1の位を合わせてBCD完成 */
            REG_03 = (uint16_t)REG_00[0];/* 次に上位セット */
            MEMRY01[i] |= (REG_03 << 8);/* がっちゃんこで上位セット */
            
        } else if (i == 8) {
            /* W0008 積算稼働時間「時」のエリアである場合 */
            /* ここだけ上位と下位が逆になる */
            MEMRY01[i] = (uint16_t)*HENNP[(2 * i) + 2];/* 下位セット */
            REG_03 = (uint16_t)*HENNP[(2 * i) + 3];
            MEMRY01[i] |= (REG_03 << 8);/* 上位セット */
            
        } else {
            /* 例外の箇所はそのまま突っ込む */
            MEMRY01[i] = (uint16_t)*HENNP[(2 * i) + 3];/* 下位セット */
            REG_03 = (uint16_t)*HENNP[(2 * i) + 2];
            MEMRY01[i] |= (REG_03 << 8);/* 上位セット */
            
        }
        
    }/* 繰り返しここまで */
    
    MEMRY01[14] = (uint16_t)TMPDT0;/* 下位セット */
    REG_03 = (uint16_t)*HENNP[30];
    MEMRY01[14] |= (REG_03 << 8);/* 上位セット */
    MEMRY01[15] = (uint16_t)TMPDT2;/* 下位セット */
    REG_03 = (uint16_t)TMPDT1;
    MEMRY01[15] |= (REG_03 << 8);/* 上位セット */
    
    for (uint16_t i = 0;i < REG_01;i++) {
        /* 読み込みワード数分だけ繰り返し */
        REG_00[0] = (uint8_t)(MEMRY01[REG_02 + i] >> 8);/* 取得したデータの上位をセットする */
        
        REG_00[1] = (REG_00[0] >> 4);/* 上位4ビットをセット */
        REG_00[1] += 0x30;
        if (REG_00[1] >= 0x3A) {
            /* 10以上の場合 */
            REG_00[1] += 0x07;/* 7加算 */
            
        }
        TBUF0[(TXPTR0 + 5 + (i * 4)) & 0x0FF] = REG_00[1];
        
        REG_00[1] = (REG_00[0] & 0x0F);/* 下位4ビットをセット */
        REG_00[1] += 0x30;
        if (REG_00[1] >= 0x3A) {
            /* 10以上の場合 */
            REG_00[1] += 0x07;/* 7加算 */
            
        }
        TBUF0[(TXPTR0 + 6 + (i * 4)) & 0x0FF] = REG_00[1];
        
        REG_00[0] = (uint8_t)(MEMRY01[REG_02 + i] & 0x00FF);/* 取得したデータの上位をセットする */
        
        REG_00[1] = (REG_00[0] >> 4);/* 上位4ビットをセット */
        REG_00[1] += 0x30;
        if (REG_00[1] >= 0x3A) {
            /* 10以上の場合 */
            REG_00[1] += 0x07;/* 7加算 */
            
        }
        TBUF0[(TXPTR0 + 7 + (i * 4)) & 0x0FF] = REG_00[1];
        
        REG_00[1] = (REG_00[0] & 0x0F);/* 下位4ビットをセット */
        REG_00[1] += 0x30;
        if (REG_00[1] >= 0x3A) {
            /* 10以上の場合 */
            REG_00[1] += 0x07;/* 7加算 */
            
        }
        TBUF0[(TXPTR0 + 8 + (i * 4)) & 0x0FF] = REG_00[1];
        
        if ((i + 1) == REG_01) {
            /* ループ最後の場合 */
            REG_03 = (9 + (i * 4));/* TBUF0のポインタ場所を保存 */
        }
        
    }/* 繰り返しここまで */
    
    TBUF0[(TXPTR0 + REG_03) & 0x0FF] = 0x03;/* ETXをセットする */
    
    REG_00[1] = 0;/* ここでクリア */
    for (uint16_t i = 1;i <= REG_03;i++) {
        /* 繰り返し加算 */
        REG_00[1] += TBUF0[(TXPTR0 + i) & 0x0FF];/* 加算する */
        
    }
    
    REG_01 = ASCICV(REG_00[1]);/* ｱｽｷｰ変換 */
    TBUF0[(TXPTR0 + REG_03 + 1) & 0x0FF] = (uint8_t)(REG_01 & 0x00FF);/* SUMをセットする */
    TBUF0[(TXPTR0 + REG_03 + 2) & 0x0FF] = (uint8_t)(REG_01 >> 8);/* SUMをセットする */
    TBUF0[(TXPTR0 + REG_03 + 3) & 0x0FF] = 0x0D;/* CRをセットする */
    TBUF0[(TXPTR0 + REG_03 + 4) & 0x0FF] = 0x0A;/* LFをセットする */
    TBUF0[TXPTR0] = 0x02;
    TXLEN0 += (REG_03 + 5);/* 送信長を加算する */
    TXSET0 += (REG_03 + 5);/* 送信長を加算する */
    TXFLG0 |= 0x11;/* 送信要求、開始セット */
    TXFLG0 &= 0xDF;/* 送信ｷｬﾝｾﾙセット */
    
    TSB_SC0->BUF = TBUF0[TXPTR0];/* バッファにセット */
    
    TXPTR0++;/* ﾎﾟｲﾝﾀをｲﾝｸﾘﾒﾝﾄ */
    TXPTR0 &= 0xFF;/* MAX FIX */
    TXLEN0--;/* ﾃﾞｸﾘﾒﾝﾄ */
    TXSET0--;/* ﾃﾞｸﾘﾒﾝﾄ */
    
    RXLEN0 = 0;/* 受信長クリア */
    RXPTR0 = 0;/* 受信ポインタクリア */
    SCRSTT = 0;/* クリア */
    
    /* WRコマンドここまで */
    
    OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* 次回呼び出し時間 */
    OS_LoadTime->TSK_COND[7] = 0xA0;/* 遅延フラグ建築 */
    
    return;
    
}

/**
 *題名：ｱｽｷｰ変換処理
 *作成者：浅尾　享一
 *説明：ｱｽｷｰ変換した値を戻す処理
**/
uint16_t ASCICV(uint8_t A_REG)
{
    uint16_t REG_00 = 0;/* 一時バッファ */
    uint8_t  REG_01 = 0;/* 一時バッファ */
    
    REG_01 = (A_REG >> 4);/* 上位セット */
    REG_01 += 0x30;
    if (REG_01 >= 0x3A) {
        /* 0x3A未満の場合 */
        REG_01 += 0x07;/* 7加算 */
        
    }
    
    REG_00 = (uint16_t)REG_01;/* 下位にセット */
    
    REG_01 = (A_REG & 0x0F);/* 下位セット */
    REG_01 += 0x30;
    if (REG_01 >= 0x3A) {
        /* 0x3A未満の場合 */
        REG_01 += 0x07;/* 7加算 */
        
    }
    
    REG_00 |= ((uint16_t)REG_01 << 8);/* 上位にセット */
    
    return REG_00;
    
}


/**
 *題名：ｱｽｷｰ→ﾊﾞｲﾅﾘ変換処理
 *作成者：浅尾　享一
 *説明：ｱｽｷｰ→ﾊﾞｲﾅﾘ変換した値を戻す処理
**/
uint8_t BNRCV(uint8_t W_REG, uint8_t A_REG)
{
    uint8_t REG_00 = 0;
    
    W_REG -= 0x30;
    if (W_REG >= 0x0A) {
        /* 10以上の場合 */
        W_REG -= 7;
    }
    A_REG -= 0x30;
    if (A_REG >= 0x0A) {
        /* 10以上の場合 */
        A_REG -= 7;
    }
    
    A_REG <<= 4;/* 4bit左シフト */
    W_REG &= 0x0F;
    A_REG |= W_REG;
    W_REG <<= 2;
    W_REG += A_REG;
    A_REG &= 0xF0;
    W_REG -= A_REG;
    W_REG <<= 1;
    A_REG >>= 4;
    REG_00 = ((A_REG) + (W_REG));
    
    return REG_00;
}

/**
 *題名：ｱｽｷｰ→HEX16進(ﾊﾞｲﾅﾘ)変換処理
 *作成者：浅尾　享一
 *説明：ｱｽｷｰ→HEX16進(ﾊﾞｲﾅﾘ)変換した値を戻す処理
**/
uint8_t HEXCV(uint8_t W_REG, uint8_t A_REG)
{
    W_REG -= 0x30;
    if (W_REG >= 0x0A) {
        W_REG -= 0x07;
    }
    
    A_REG -= 0x30;
    if (A_REG >= 0x0A) {
        A_REG -= 0x07;
    }
    
    W_REG <<= 4;
    A_REG |= W_REG;/* ORします */
    
    return A_REG;
}