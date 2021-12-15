#ifndef __TMPM380_CMN_H
#define __TMPM380_CMN_H

#ifndef __MAIN_H
#include "main.h"
#endif

/* 変更しない変数値の定義---------------------------------------------------- */
#define TSKNUM                   ((uint8_t)8)/* タスク数を設定 */
#define TMRB_1MS                 0x1388/* fsys = fc = 10MHz * PLL = 40MHz, fphiT0 = fperiph = fgear = fc */
                                 /* ftmrb = 1/8fphiT0 = 1/8 * 40 = 5MHz, Ttmrb = 0.2us, 1000us/0.2us = 5000 = 0x1388 */
#define SELF_ADDR                ((uint8_t)0x70)/* I2C通信用 自身のアドレス(適当に0b01110000) */
#define DACN_ADDR                ((uint8_t)0x22)/* D/Aｺﾝﾊﾞｰﾀのアドレス(0b0010001X) */
#define RTC_ADDR                 ((uint8_t)0x64)/*RTCのアドレス(0b0110010X)*/

extern uint8_t  kidou;/* 起動後初期化にてﾃﾞｨｯﾌﾟｽｲｯﾁの確認で使用するﾌﾗｸﾞ変数 */
extern uint8_t  count01;/* 起動後初期化時にﾃﾞｨｯﾌﾟｽｲｯﾁの確認で使用するｶｳﾝﾄｱｯﾌﾟ変数 */
extern uint8_t  ERRFG;/* エラーチェック用のフラグ */

extern uint8_t  STSFL0;/* システム制御フラグ0 */
extern uint8_t  STSFL1;/* システム制御フラグ1 */
extern uint8_t  D_INISQ;/* 初期化ｱﾌﾟﾘのｼｰｹﾝｽ番号 */
extern uint8_t  INI_STT;/* 初期化サブシーケンス */
extern uint8_t  DIPBUF;/* ﾃﾞｨｯﾌﾟｽｲｯﾁのﾊﾞｯﾌｧ */
extern uint8_t  SYKND0;/* ﾃﾞｨｯﾌﾟｽｲｯﾁによる機種設定保持ﾊﾞｯﾌｧ */

/* I2C通信に使用するﾊﾞｯﾌｧ */
extern uint8_t NACK5;/* 埼玉NACK5 */
extern uint8_t I2C_WCNT;/* I2C書込み回数 */
extern uint8_t I2C_RCNT;/* I2C読込み回数 */
extern uint8_t I2C_STATE;/* I2C通信割込みのｽﾃｰﾄ番号 */
extern uint16_t SEC_CNT;/* 3秒カウンタ */
extern uint8_t I2C_WAIT;/* 通常通信時確認時の待ち時間 */
extern uint8_t I2CBUF[32];/* I2C通信の送信ﾊﾞｯﾌｧ */
extern uint8_t FRAM_FG;/* 起動時のF-RAM読み出しフラグ */
extern uint8_t I2CCNT;/* I2C通信回数のカウント */
extern uint8_t T0BF0;/* 汎用バッファ0 */
extern uint8_t T0BF1;/* 汎用バッファ1 */
extern uint8_t T0BF2;/* 汎用バッファ2 */
extern uint8_t BOOT_SEC;/* 基板起動してからの時間 */
extern uint8_t WR_RTC;/* RTC書き込みフラグ */
extern uint8_t RTC_ERR;/* RTCエラーフラグ */
extern uint8_t RTC_ECT;/* RTCエラー回数 */
extern uint8_t RTCDT[10];/* RTCの受信バッファ */
extern uint8_t RTCD_R[10];/* RTCの受信バッファ(BCDそのまんま) */
extern uint8_t RTCKBF[10];/* RTCの秒確認バッファ */
extern uint8_t SETST_BUF[10];/* RTC書き込み用のバッファ */
extern uint8_t RWTMS;/* 運転待機計測ﾀｲﾏ */

extern uint16_t MSECD;/* ミリ秒のカウント変数 */
extern uint16_t SECND;/* 秒のカウント変数 */
extern uint8_t  PDTM;/* POWER DOWN TIMER 1mS */
extern uint16_t PDTM2;/* POWER DOWN TIMER(3S) */

/* 入力各ビット格納変数 */
extern uint8_t MXTM;/* マトリックスタイマー */
extern uint8_t MXSW;/* キーマトリックスデータ生 */
extern uint8_t MXSWO0;/* キーマトリックスデータ確定 */
extern uint8_t MXIN0;/* 入力0生 */
extern uint8_t MXIN1;/* 入力1生 */
extern uint8_t MXINO0;/* 入力0確定 */
extern uint8_t MXINO1;/* 入力1確定 */

/* 出力用変数 */
extern uint8_t MXOTN0;/* 出力0 */
extern uint8_t MXOTN1;/* 出力1 */
extern uint8_t MXOTN2;/* 出力2 */
extern uint8_t MXOTN3;/* 出力3 */
extern uint8_t MXOTN4;/* 出力4 */
extern uint8_t MXOTN5;/* 出力5 */
extern uint8_t MXOTN6;/* 出力6 */
extern uint8_t MX6BLK;/* 出力6点滅フラグ */

extern uint8_t EDDT[6];/* 編集中データ */

/* ～バックアップデータ(32バイト)～ */
extern uint8_t  PASS1;/* 保存エリアのパスワード下位、正解は0x01 */
extern uint8_t  PASS2;/* 保存エリアのパスワード上位、正解は0xAA */
extern uint8_t  SYSFLG;/* 運転フラグ */
extern uint8_t  ABNF;/* 異常フラグ */
/*
0～3ビット…異常内容1～F、0は無し
4…チラー異常
5…チラー能力低下
6…継続異常警報要求フラグ
7…異常警報要求フラグ
*/
extern uint8_t  PAT1;/* パターン1設定温度 */
extern uint8_t  PAT2;/* パターン2設定温度 */
extern uint8_t  PAT3;/* パターン3設定温度 */
extern uint8_t  PAT4;/* パターン4設定温度 */
extern uint8_t  RPAT;/* 運転パターン */
/*
0…パターン1　徐冷有り：1、無し：0
1…パターン2　徐冷有り：1、無し：0
2…パターン3　徐冷有り：1、無し：0
3…パターン4　徐冷有り：1、無し：0
4…
5…洗浄ﾓｰﾄﾞフラグ　1：洗浄モード、0：通常
6…常温水槽の排水状態を表すフラグ
7…低温水槽の排水状態を表すフラグ
*/
extern uint8_t  SWTMP0;/* 常温水槽の設定温度 */
extern uint8_t  ABNCUT;/* 異常履歴と異常件数 */
/*
0～3：異常履歴件数（最大5件）
4～7：最新異常データ
*/
extern uint8_t  ABNH0;/* 異常履歴1 */
/*
0～3：1個前異常データ
4～7：2個前異常データ
*/
extern uint8_t  ABNH1;/* 異常履歴2 */
/*
0～3：3個前異常データ
4～7：4個前異常データ
*/
extern uint8_t  STIME1;/* 冷却開始時刻下位 */
extern uint8_t  STIME2;/* 冷却開始時刻上位 */
extern uint8_t  ETIME1;/* 冷却終了時刻下位 */
extern uint8_t  ETIME2;/* 冷却終了時刻上位 */
extern uint8_t  RTIMM;/* 積算稼働時間　分 */
extern uint8_t  RTIMH1;/* 積算稼働時間　時間下位 */
extern uint8_t  RTIMH2;/* 積算稼働時間　時間上位 */
extern uint8_t  SETTM;/* チラー予約タイム　分 */
extern uint8_t  SETTH;/* チラー予約タイム　時 */
extern uint8_t  CSEQTS;/* 冷却工程シーケンスタイム（秒） */
extern uint8_t  CSEQTM;/* 冷却工程シーケンスタイム（分） */
extern uint8_t  CCSEQ;/* 冷却工程シーケンスナンバー */
extern uint8_t  SCTM1;/* パターン1徐冷時間 */
extern uint8_t  SCTM2;/* パターン2徐冷時間 */
extern uint8_t  SCTM3;/* パターン3徐冷時間 */
extern uint8_t  SCTM4;/* パターン4徐冷時間 */
extern uint8_t  NPAT;/* 現在選択中のパターン */
extern uint8_t  RWAITT;/* 運転待機時間（分）　MAX＝4時間 */
extern uint8_t  CSUM;/* NV-RAMのサムチェックデータ */
/* ～バックアップデータ(32バイト)ここまで～ */

/* ADｺﾝﾊﾞｰﾀ用の変数類 */
extern uint8_t  AD_RESLT;/* AD変換実行回数 */
extern uint8_t  AD_MX;/* どのAD変換を実行するかの指示 */
extern uint8_t  AD_FLG01;/* AD変換完了/未完了を判断するフラグ変数 */
extern uint8_t  AD_FLG02;/* 2回の平均化完了したかを表すフラグ変数 */
extern uint8_t  AD_NOW_CNT[3];/* 瞬時AD変換カウント */
extern uint8_t  AD_ONE_CNT[3];/* 確定用AD変換カウント */

extern uint16_t AD_NOW_MAX[3];/* AD取得時の最大値 */
extern uint16_t AD_NOW_MIN[3];/* ADの最小値 */
extern uint32_t AD_NOW_SUM[3];/* ADの合算値 */

extern uint16_t AD_ONE_AVR[3];/* AD取得時の平均値 */
extern uint16_t AD_ONE_MAX[3];/* AD取得時の最大値 */
extern uint16_t AD_ONE_MIN[3];/* ADの最小値 */
extern uint32_t AD_ONE_SUM[3];/* ADの合算値 */
extern uint16_t AD0_FVB[5];

extern short TMPDT0;/* 品温 */
extern short TMPDT1;/* 低温槽水温*/
extern short TMPDT2;/* 常温槽水温 */
extern short TOLD0;/* 旧品温 */
extern short TOLD1;/* 旧低温槽水温 */
extern short TOLD2;/* 旧常温槽水温 */
extern uint8_t AD0TM;/* 品温センサ異常検出タイマ */
extern uint8_t AD1TM;/* 低温槽水温異常検出タイマ */
extern uint8_t AD2TM;/* 常温槽水温異常検出タイマ */
extern uint16_t DACOUT;/* 品温の4-20mA出力用 */
extern uint8_t AD_CTM0;/* AD変換固着タイマ */

/* 冷却制御固有変数類 */
extern uint8_t  CCTM0;/* 冷却工程ﾀｲﾏｰ0 */
extern uint8_t  CCTM1;/* 冷却工程ﾀｲﾏｰ1 */
extern uint16_t CCTM2;/* 冷却工程ﾀｲﾏｰ2 */
extern uint8_t  CCTM3;/* 冷却工程ﾀｲﾏｰ3 */
extern uint8_t  CCTM4;/* 瞬停復帰動作用ﾀｲﾏｰ */
extern uint8_t  CCTM5;/* 真空解除中のｱﾗｰﾑ用ﾀｲﾏ */

/* 表示アプリで使用する変数類 */
extern uint8_t DSPNO;/* 表示ナンバー */
extern uint8_t DSPNOO;/* 旧表示ナンバー */
extern uint8_t DSPLN1;/* 表示長1 */
extern uint8_t DSPLN2;/* 表示長2 */
extern const uint8_t *MHEAD1;/* メッセージ1先頭アドレス */
extern const uint8_t *MHEAD2;/* メッセージ2先頭アドレス */
extern uint8_t DSPSEQ;/* 画面表示シーケンス */
extern uint8_t DSPSSQ;/* 画面表示サブシーケンス */
extern uint8_t DSPLB1[16];/* DISPLAY DATA BUFFER for LINE 1 */
extern uint8_t DSPLB2[16];/* DISPLAY DATA BUFFER for LINE 2 */
extern uint8_t DSPTM1;/* 画面タイマ1 */
extern uint8_t DSPTM2;/* 画面タイマ2 */
extern uint8_t DCLNF;/* 画面コロン「：」フラグ */
extern uint8_t DABHP;/* ABNORMAL HISTORY POINT */
extern uint8_t BZSEQ;/* ブザーシーケンス */
extern uint8_t BZTM1;/* ブザータイマー1 */
extern uint8_t BZTM2;/* ブザータイマー2 */
extern uint8_t ABNFO;/* 旧異常フラグ */
extern uint8_t YOTINO;/* 予知警報表示ﾅﾝﾊﾞｰ */
extern uint8_t TNO;/* ｾﾙﾌ･ﾃｽﾄ･ﾅﾝﾊﾞｰ */
extern uint8_t SUBTNO;/* サブテストナンバー */
extern uint8_t MCLAST;/* ﾒﾓﾘｸﾘｱ受付ﾀｲﾏ */
extern uint8_t STSAST;/* ｾﾙﾌﾃｽﾄ受付ﾀｲﾏ	*/
extern uint8_t STSTM1;/* ｾﾙﾌﾃｽﾄ用ﾀｲﾏ */
extern uint8_t BITPOS;/* ビットチェックフラグ */
extern uint8_t CRTCDT;/* RTC CHECK用OLD DATA */
extern uint8_t MDAST;/* 手動排水操作受付ﾀｲﾏ */
extern uint8_t DLWSQ;/* DCONTの次回スタート位置を決める場所 */
extern uint8_t DPOUT;/* LCD/VFDの表示回数 */

/* 操作制御で使用する変数類 */
extern uint8_t  KFLG;/* ｵﾝﾄﾘｶﾞﾃﾞｰﾀ作成用 */
extern uint8_t  ONTRG;/* ｵﾝﾄﾘｶﾞﾃﾞｰﾀ */
extern uint16_t INDPTM;/* 通常表示復帰用ﾀｲﾏｰ */
extern uint8_t  CLNRTM;/* 洗浄中断表示復帰ﾀｲﾏｰ */
extern uint8_t  RPTFLG;/* 設定中ﾃﾞｰﾀ */

/* 外部ﾂｰﾙで使用する通信ﾊﾞｯﾌｧ */
extern uint8_t  TXFLG0;/* 外部ﾂｰﾙ通信の送信関連ﾌﾗｸﾞ */
extern uint8_t  TXSET0;/* 外部ﾂｰﾙ通信送信ｾｯﾄ数 */
extern uint8_t  TXLEN0;/* 外部ﾂｰﾙ通信送信長 */
extern uint8_t  TXPTR0;/* 外部ﾂｰﾙ通信送信ﾊﾞｯﾌｧの現在位置 */
extern uint8_t  USTS0;/* 外部ﾂｰﾙ通信受信ﾌﾗｸﾞ変数 */
extern uint8_t  RXLEN0;/* 外部ﾂｰﾙ通信受信長 */
extern uint8_t  RXPTR0;/* 外部ﾂｰﾙ通信受信ﾊﾞｯﾌｧの現在位置 */
extern uint8_t  TBUF0[256];/* 送信ﾊﾞｯﾌｧ256ﾊﾞｲﾄ */
extern uint8_t  RBUF0[256];/* 受信ﾊﾞｯﾌｧ256ﾊﾞｲﾄ */
extern uint8_t  SCRSTT;/* 通信手順1の通信状態[1byte] */
extern uint8_t  FLMSTS;/* ｽｺｱ通信受信完了byte数[1byte] */
extern uint16_t KYNO;/* 局番号[2byte] */
extern uint16_t PCNO;/* PC番号[2byte] */
extern uint16_t MEMRY01[16];/* 通信用Wエリアメモリー */

/* 水位制御用変数類 */
extern uint8_t  WLNEW;/* 最新水位ﾚﾍﾞﾙ入力 */
extern uint8_t  WLOLD;/* １つ前の水位ﾚﾍﾞﾙ入力 */
extern uint8_t  NHILCT;/* 低温槽HIｷｬﾝｾﾙﾀｲﾏ */
extern uint8_t  NLOLCT;/* 低温槽LOｷｬﾝｾﾙﾀｲﾏ	*/
extern uint8_t  CHILCT;/* 常温槽HIｷｬﾝｾﾙﾀｲﾏ	*/
extern uint8_t  CLOLCT;/* 常温槽LOｷｬﾝｾﾙﾀｲﾏ	*/
extern uint8_t  WLCTM1;/* 水位制御用制御ﾀｲﾏ1 */
extern uint8_t  WLCTM2;/* 水位制御用制御ﾀｲﾏ2 */
extern uint8_t  WLCTM3;/* 水位制御用制御ﾀｲﾏ3 */
extern uint16_t WLCTM4;/* 排水工程用ﾀｲﾏｰ */
extern uint16_t WLCTM5;/* 電源ｵﾌ時の総排水用ﾀｲﾏ */
extern uint8_t  WLCWT;/* 水位制御遅延ﾀｲﾏ */
extern uint16_t WABTM1;/* 常温槽給水異常検知ﾀｲﾏ */
extern uint16_t WABTM2;/* 低温槽給水異常検知ﾀｲﾏ */
extern uint16_t TIRTM1;/* ﾁﾗｰ能力低下確認ﾀｲﾏ */
extern uint8_t  MMODE;/* MANUAL MODE FLAG */
extern uint8_t  MMDTM;/* MANUAL MODE TIMER	*/
extern uint8_t  WCTM1;/* 洗浄中の電動弁動作ﾀｲﾏ	*/
extern uint8_t  WCTM2;/* エア抜き弁用ﾀｲﾏ */

typedef struct
{
    uint8_t  TSK_COND[TSKNUM];/*アプリケーションのコンディションフラグ*/
    uint16_t NEXT_LOAD_Time[TSKNUM];/*アプリケーション次回呼び出し要求時間*/
} OS_TSKCon1;

OS_TSKCon1 *OSTSKCon1S(void);

extern uint8_t *HENNP[32];/* NV-RAM保存エリアの先頭アドレス */
#endif