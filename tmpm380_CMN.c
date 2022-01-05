#include "tmpm380_CMN.h"

uint8_t  kidou = 0;/* 起動後初期化にてﾃﾞｨｯﾌﾟｽｲｯﾁの確認で使用するﾌﾗｸﾞ変数 */
uint8_t  count01 = 0;/* 起動後初期化時にﾃﾞｨｯﾌﾟｽｲｯﾁの確認で使用するｶｳﾝﾄｱｯﾌﾟ変数 */
uint8_t  ERRFG = 0;/* エラーチェック用のフラグ */
uint16_t HPSHT = 0;/* HACCP運転出力遅延タイマ（チャタリングキャンセル用） */

uint8_t  STSFL0 = 0;/* システム制御フラグ0 */
/*
0…NV-RAM書き込みタイミング（＝0のとき、1はスルー）
1…イニシャライズ完了フラグ（1のとき完了）
2…瞬停動作要求フラグ
3…完了アラーム出力中フラグ
4…表示工程の最小のブザー禁止フラグ
5…準備中（1で準備完了）
6…クリック音
7…設定モードフラグ
*/
uint8_t  STSFL1 = 0;/* システム制御フラグ1 */
/*
0…ｾﾙﾌﾃｽﾄﾓｰﾄﾞ受付フラグ
1…メモリークリア受付フラグ
2…マニュアルモード操作エラー
3…手動排水操作受付中フラグ
4…手動排水受付フラグ
5…予備
6…真空解除中のアラーム禁止フラグ
7…真空解除中のアラーム要求フラグ
*/
uint8_t  D_INISQ = 0;/* 初期化ｱﾌﾟﾘのｼｰｹﾝｽ番号 */
uint8_t  INI_STT = 0;/* 初期化サブシーケンス */
uint8_t  DIPBUF = 0;/* ﾃﾞｨｯﾌﾟｽｲｯﾁのﾊﾞｯﾌｧ */
uint8_t  SYKND0 = 0;/* ﾃﾞｨｯﾌﾟｽｲｯﾁによる機種設定保持ﾊﾞｯﾌｧ */
/*
0〜3…予備
4…0：チラー式、1：蒸気式
5…0：10℃仕様、1：15℃仕様
6…0：35R、1：35R、1：50R、0：100R
7…0　　 　1　　 　0　　 　1
*/

/* I2C通信に使用するﾊﾞｯﾌｧ */
uint8_t NACK5     = 0;/* 埼玉NACK5 */
uint8_t I2C_WCNT  = 0;/* I2C書込み回数 */
uint8_t I2C_RCNT  = 0;/* I2C読込み回数 */
uint8_t I2C_STATE = 0;/* I2C通信割込みのｽﾃｰﾄ番号 */
uint16_t SEC_CNT = 0;/* 3秒カウンタ */
uint8_t I2C_WAIT  = 0;/* 通常通信時確認時の待ち時間 */
uint8_t I2CBUF[32] = {
    /* I2C通信の送信ﾊﾞｯﾌｧ */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0
};
uint8_t I2CCNT  = 0;/* I2C通信回数のカウント */
uint8_t T0BF0 = 0;/* 汎用バッファ0 */
uint8_t T0BF1 = 0;/* 汎用バッファ1 */
uint8_t T0BF2 = 0;/* 汎用バッファ2 */
uint8_t BOOT_SEC = 0;/* 基板起動してからの時間 */
uint8_t WR_RTC = 0;/* RTC書き込みフラグ */
uint8_t RTC_ERR = 0;/* RTCエラーフラグ */
uint8_t RTC_ECT = 0;/* RTCエラー回数 */
uint8_t RTCDT[10] = {
    /* RTCの受信バッファ */
    0,0,0,0,0,0,0,0,0,0
};
uint8_t RTCD_R[10] = {
    /* RTCの受信バッファ(BCDそのまんま) */
    0,0,0,0,0,0,0,0,0,0
};
uint8_t RTCKBF[10] = {
    /* RTCの秒確認バッファ */
    0,0,0,0,0,0,0,0,0,0
};
uint8_t SETST_BUF[10] = {
    /* RTC書き込み用のバッファ */
    0,0,0,0,0,0,0,0,0,0
};
uint8_t RWTMS = 0;/* 運転待機計測ﾀｲﾏ */

uint16_t MSECD = 0;/* ミリ秒のカウント変数 */
uint16_t SECND = 0;/* 秒のカウント変数 */
uint8_t  PDTM = 0;/* POWER DOWN TIMER 1mS */
uint16_t PDTM2 = 0;/* POWER DOWN TIMER(3S) */

/* 入力各ビット格納変数 */
uint8_t MXTM = 0;/* マトリックスタイマー */
uint8_t MXSW = 0;/* キーマトリックスデータ生 */
uint8_t MXSWO0 = 0;/* キーマトリックスデータ確定 */
/*
0…↑SW
1…↓SW
2…→SW
3…設定SW
4…電源SW
5…選択SW
6…運転SW
7…チラー予約SW
*/
uint8_t MXIN0 = 0;/* 入力0生 */
uint8_t MXIN1 = 0;/* 入力1生 */
uint8_t MXINO0 = 0;/* 入力0確定 */
/*
0･･･常温水槽水位L
1･･･常温水槽水位H
2･･･低温水槽水位L
3･･･低温水槽水位H
4･･･常温真空弁閉リミット
5･･･予備
6･･･圧力センサー
7･･･チラー異常信号
*/
uint8_t MXINO1 = 0;/* 入力1確定 */
/*
0･･･サーマルトリップ
1･･･品温センサ断線信号
2･･･空き
3･･･停電信号
4･･･DSW1
5･･･DSW2
6･･･DSW3
7･･･DSW4
*/

/* 出力用変数 */
uint8_t MXOTN0 = 0;/* 出力0 */
/*
0･･･真空弁（常温）出力
1･･･真空弁（低温）出力
2･･･エジェクタP（常温）出力
3･･･エジェクタP（低温）出力
4･･･給水弁（常温）出力
5･･･給水弁（低温）出力
6･･･排水弁（常温）出力
7･･･排水弁（低温）出力
*/
uint8_t MXOTN1 = 0;/* 出力1 */
/*
0･･･使用禁止
1･･･冷却槽排水弁出力（予備）
2･･･エジェクタ弁（常温）出力
3･･･エジェクタ弁（低温）出力
4･･･徐冷弁出力
5･･･真空破壊弁出力
6･･･蒸気弁出力
7･･･チラー運転出力
*/
uint8_t MXOTN2 = 0;/* 出力2 */
/*
0･･･使用禁止
1･･･使用禁止
2･･･ブザー出力（表示基板用）
3･･･チラー循環P出力
4･･･熱交排水弁出力
5･･･エア抜き弁出力
6･･･熱交循環出力
7･･･ブザー出力（リレー出力）
*/
uint8_t MXOTN3 = 0;/* 出力3 */
/*
0･･･使用禁止
1･･･使用禁止
2･･･HACCP用冷却出力
3･･･HACCP用異常出力
4･･･NV-RAM用チップセレクト出力
5･･･予備
6･･･使用禁止
7･･･使用禁止
*/
uint8_t MXOTN4 = 0;/* 出力4 */
/*
0･･･予備
1･･･予備
2･･･予備
3･･･予備
4･･･液晶バックライトON出力
5･･･予備
6･･･予備
7･･･予備
*/
uint8_t MXOTN5 = 0;/* 出力5 */
/*
0･･･使用禁止
1･･･デコーダ―出力1
2･･･デコーダ―出力2
3･･･LED出力1
4･･･LED出力2
5･･･液晶/VFDのE端子出力
6･･･液晶/VFDのRW端子出力
7･･･液晶/VFDのRS端子出力
*/
uint8_t MXOTN6 = 0;/* 出力6 */
/*
0…パターン1LED
1…パターン2LED
2…パターン3LED
3…パターン4LED
4…電源LED
5…準備LED
6…運転/停止LED
7…チラー予約LED
*/
uint8_t MX6BLK = 0;/* 出力6点滅フラグ */
/*
0…パターン1LED
1…パターン2LED
2…パターン3LED
3…パターン4LED
4…電源LED
5…準備LED
6…運転/停止LED
7…チラー予約LED
*/

uint8_t EDDT[6] = {
    /* 編集中データ */
    0, 0, 0, 0, 0, 0
};

/* 〜バックアップデータ(32バイト)〜 */
uint8_t  PASS1 = 0;/* 保存エリアのパスワード下位、正解は0x01 */
uint8_t  PASS2 = 0;/* 保存エリアのパスワード上位、正解は0xAA */
uint8_t  SYSFLG = 0;/* 運転フラグ */
/*
0…常温水槽排水工程フラグ
1…運転中
2…徐冷行程中
3…冷却工程中
4…真空解除待ち
5…チラー予約待ち
6…電源OFF
7…自動排水あり/無し（1で無し）
*/
uint8_t  ABNF = 0;/* 異常フラグ */
/*
0〜3ビット…異常内容1〜F、0は無し
4…チラー異常
5…チラー能力低下
6…継続異常警報要求フラグ
7…異常警報要求フラグ
*/
uint8_t  PAT1 = 0;/* パターン1設定温度 */
uint8_t  PAT2 = 0;/* パターン2設定温度 */
uint8_t  PAT3 = 0;/* パターン3設定温度 */
uint8_t  PAT4 = 0;/* パターン4設定温度 */
uint8_t  RPAT = 0;/* 運転パターン */
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
uint8_t  SWTMP0 = 0;/* 常温水槽の設定温度 */
uint8_t  ABNCUT = 0;/* 異常履歴と異常件数 */
/*
0〜3：異常履歴件数（最大5件）
4〜7：最新異常データ
*/
uint8_t  ABNH0 = 0;/* 異常履歴1 */
/*
0〜3：1個前異常データ
4〜7：2個前異常データ
*/
uint8_t  ABNH1 = 0;/* 異常履歴2 */
/*
0〜3：3個前異常データ
4〜7：4個前異常データ
*/
uint8_t  STIME1 = 0;/* 冷却開始時刻 下位 */
uint8_t  STIME2 = 0;/* 冷却開始時刻 上位 */
uint8_t  ETIME1 = 0;/* 冷却終了時刻 下位 */
uint8_t  ETIME2 = 0;/* 冷却終了時刻 上位 */
uint8_t  RTIMM = 0;/* 積算稼働時間　分 */
uint8_t  RTIMH1 = 0;/* 積算稼働時間　時間 下位 */
uint8_t  RTIMH2 = 0;/* 積算稼働時間　時間 上位 */
uint8_t  SETTM = 0;/* チラー予約タイム　分 */
uint8_t  SETTH = 0;/* チラー予約タイム　時 */
uint8_t  CSEQTS = 0;/* 冷却工程シーケンスタイム（秒） */
uint8_t  CSEQTM = 0;/* 冷却工程シーケンスタイム（分） */
uint8_t  CCSEQ = 0;/* 冷却工程シーケンスナンバー */
uint8_t  SCTM1 = 0;/* パターン1徐冷時間 */
uint8_t  SCTM2 = 0;/* パターン2徐冷時間 */
uint8_t  SCTM3 = 0;/* パターン3徐冷時間 */
uint8_t  SCTM4 = 0;/* パターン4徐冷時間 */
uint8_t  NPAT = 0;/* 現在選択中のパターン */
uint8_t  RWAITT = 0;/* 運転待機時間（分）　MAX＝4時間 */
uint8_t  CSUM = 0;/* NV-RAMのサムチェックデータ */
/* 〜バックアップデータ(32バイト)ここまで〜 */

/* ADｺﾝﾊﾞｰﾀ用の変数類 */
uint8_t  AD_RESLT = 0;/* AD変換実行回数 */
uint8_t  AD_MX = 0;/* どのAD変換を実行するかの指示 */
uint8_t  AD_FLG01 = 0;/* AD変換完了/未完了を判断するフラグ変数 */
uint8_t  AD_FLG02 = 0;/* 2回の平均化完了したかを表すフラグ変数 */
uint8_t  AD_NOW_CNT[3] = {0,0,0};/* 瞬時AD変換カウント */
uint8_t  AD_ONE_CNT[3] = {0,0,0};/* 確定用AD変換カウント */

uint16_t AD_NOW_MAX[3] = {0, 0, 0};/* AD取得時の最大値 */
uint16_t AD_NOW_MIN[3] = {4095, 4095, 4095};/* ADの最小値 */
uint32_t AD_NOW_SUM[3] = {0, 0, 0};/* ADの合算値 */

uint16_t AD_ONE_AVR[3] = {0, 0, 0};/* AD取得時の平均値 */
uint16_t AD_ONE_MAX[3] = {0, 0, 0};/* AD取得時の最大値 */
uint16_t AD_ONE_MIN[3] = {4095, 4095, 4095};/* ADの最小値 */
uint32_t AD_ONE_SUM[3] = {0, 0, 0};/* ADの合算値 */
uint16_t AD0_FVB[5] = {
    /* 品温センサのデジタル値 */
    0,0,0,0,0
};
short TMPDT0 = 0;/* 品温 */
short TMPDT1 = 0;/* 低温槽水温*/
short TMPDT2 = 0;/* 常温槽水温 */
short TOLD0 = 0;/* 旧品温 */
short TOLD1 = 0;/* 旧低温槽水温 */
short TOLD2 = 0;/* 旧常温槽水温 */
uint8_t AD0TM = 0;/* 品温センサ異常検出タイマ */
uint8_t AD1TM = 0;/* 低温槽水温異常検出タイマ */
uint8_t AD2TM = 0;/* 常温槽水温異常検出タイマ */
uint16_t DACOUT = 0;/* 品温の4-20mA出力用 */
uint8_t AD_CTM0 = 0;/* AD変換固着タイマ */

/* 冷却制御固有変数類 */
uint8_t  CCTM0 = 0;/* 冷却工程ﾀｲﾏｰ0 */
uint8_t  CCTM1 = 0;/* 冷却工程ﾀｲﾏｰ1 */
uint16_t CCTM2 = 0;/* 冷却工程ﾀｲﾏｰ2 */
uint8_t  CCTM3 = 0;/* 冷却工程ﾀｲﾏｰ3 */
uint8_t  CCTM4 = 0;/* 瞬停復帰動作用ﾀｲﾏｰ */
uint8_t  CCTM5 = 0;/* 真空解除中のｱﾗｰﾑ用ﾀｲﾏ */

/* 表示アプリで使用する変数類 */
uint8_t DSPNO = 0;/* 表示ナンバー */
uint8_t DSPNOO = 0;/* 旧表示ナンバー */
uint8_t DSPLN1 = 0;/* 表示長1 */
uint8_t DSPLN2 = 0;/* 表示長2 */
const uint8_t *MHEAD1;/* メッセージ1先頭アドレス */
const uint8_t *MHEAD2;/* メッセージ2先頭アドレス */
uint8_t DSPSEQ = 0;/* 画面表示シーケンス */
uint8_t DSPSSQ = 0;/* 画面表示サブシーケンス */
uint8_t DSPLB1[16] = {
    /* DISPLAY DATA BUFFER for LINE 1 */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
uint8_t DSPLB2[16] = {
    /* DISPLAY DATA BUFFER for LINE 2 */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
uint8_t DSPTM1 = 0;/* 画面タイマ1 */
uint8_t DSPTM2 = 0;/* 画面タイマ2 */
uint8_t DCLNF = 0;/* 画面コロン「：」フラグ */
uint8_t DABHP = 0;/* ABNORMAL HISTORY POINT */
uint8_t BZSEQ = 0;/* ブザーシーケンス */
uint8_t BZTM1 = 0;/* ブザータイマー1 */
uint8_t BZTM2 = 0;/* ブザータイマー2 */
uint8_t ABNFO = 0;/* 旧異常フラグ */
uint8_t YOTINO = 0;/* 予知警報表示ﾅﾝﾊﾞｰ */
uint8_t TNO = 0;/* ｾﾙﾌ･ﾃｽﾄ･ﾅﾝﾊﾞｰ */
uint8_t SUBTNO = 0;/* サブテストナンバー */
uint8_t MCLAST = 0;/* ﾒﾓﾘｸﾘｱ受付ﾀｲﾏ */
uint8_t STSAST = 0;/* ｾﾙﾌﾃｽﾄ受付ﾀｲﾏ	*/
uint8_t STSTM1 = 0;/* ｾﾙﾌﾃｽﾄ用ﾀｲﾏ */
uint8_t BITPOS = 0;/* ビットチェックフラグ */
uint8_t CRTCDT = 0;/* RTC CHECK用OLD DATA */
uint8_t MDAST = 0;/* 手動排水操作受付ﾀｲﾏ */
uint8_t DLWSQ = 0;/* DCONTの次回スタート位置を決める場所 */
uint8_t DPOUT = 0;/* LCD/VFDの表示回数 */
uint8_t FLZFG = 0;/* LCD/VFDのフリーズフラグ */
/*
0…ビジーによるフリーズフラグ
1〜7…予備
*/
uint32_t FLZTM = 0;/* LCD/VFDのフリーズタイマ：1〜5分(60000ms〜300000ms) */

/* 操作制御で使用する変数類 */
uint8_t  KFLG = 0;/* ｵﾝﾄﾘｶﾞﾃﾞｰﾀ作成用 */
uint8_t  ONTRG = 0;/* ｵﾝﾄﾘｶﾞﾃﾞｰﾀ */
uint16_t INDPTM = 0;/* 通常表示復帰用ﾀｲﾏｰ */
uint8_t  CLNRTM = 0;/* 洗浄中断表示復帰ﾀｲﾏｰ */
uint8_t  RPTFLG = 0;/* 設定中ﾃﾞｰﾀ */
/*
;		     0〜4 bit = ｵｰﾄﾘﾋﾟｰﾄﾀｲﾏｰ
;		      5,6 bit = 0固定
;			7 bit = ｵｰﾄﾘﾋﾟｰﾄFLAG
*/

/* 外部ﾂｰﾙで使用する通信ﾊﾞｯﾌｧ */
uint8_t  TXFLG0 = 0;/* 外部ﾂｰﾙ通信の送信関連ﾌﾗｸﾞ */
uint8_t  TXSET0 = 0;/* 外部ﾂｰﾙ通信送信ｾｯﾄ数 */
uint8_t  TXLEN0 = 0;/* 外部ﾂｰﾙ通信送信長 */
uint8_t  TXPTR0 = 0;/* 外部ﾂｰﾙ通信送信ﾊﾞｯﾌｧの現在位置 */
uint8_t  USTS0  = 0;/* 外部ﾂｰﾙ通信受信ﾌﾗｸﾞ変数 */
uint8_t  RXLEN0 = 0;/* 外部ﾂｰﾙ通信受信長 */
uint8_t  RXPTR0 = 0;/* 外部ﾂｰﾙ通信受信ﾊﾞｯﾌｧの現在位置 */
uint8_t  TBUF0[256] = {
    /* 送信ﾊﾞｯﾌｧ256ﾊﾞｲﾄ */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
uint8_t  RBUF0[256] = {
    /* 受信ﾊﾞｯﾌｧ256ﾊﾞｲﾄ */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
uint8_t  SCRSTT = 0;/* 通信手順1の通信状態[1byte] */
uint8_t  FLMSTS = 0;/* ｽｺｱ通信受信完了byte数[1byte] */
uint16_t KYNO = 0;/* 局番号[2byte] */
uint16_t PCNO = 0;/* PC番号[2byte] */
uint16_t MEMRY01[16] = {
    /* 通信用Wエリアメモリー */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

/* 水位制御用変数類 */
uint8_t  WLNEW = 0;/* 最新水位ﾚﾍﾞﾙ入力 */
uint8_t  WLOLD = 0;/* １つ前の水位ﾚﾍﾞﾙ入力 */
uint8_t  NHILCT = 0;/* 低温槽HIｷｬﾝｾﾙﾀｲﾏ */
uint8_t  NLOLCT = 0;/* 低温槽LOｷｬﾝｾﾙﾀｲﾏ	*/
uint8_t  CHILCT = 0;/* 常温槽HIｷｬﾝｾﾙﾀｲﾏ	*/
uint8_t  CLOLCT = 0;/* 常温槽LOｷｬﾝｾﾙﾀｲﾏ	*/
uint8_t  WLCTM1 = 0;/* 水位制御用制御ﾀｲﾏ1 */
uint8_t  WLCTM2 = 0;/* 水位制御用制御ﾀｲﾏ2 */
uint8_t  WLCTM3 = 0;/* 水位制御用制御ﾀｲﾏ3 */
uint16_t WLCTM4 = 0;/* 排水工程用ﾀｲﾏｰ */
uint16_t WLCTM5 = 0;/* 電源ｵﾌ時の総排水用ﾀｲﾏ */
uint8_t  WLCWT = 0;/* 水位制御遅延ﾀｲﾏ */
uint16_t WABTM1 = 0;/* 常温槽給水異常検知ﾀｲﾏ */
uint16_t WABTM2 = 0;/* 低温槽給水異常検知ﾀｲﾏ */
uint16_t TIRTM1 = 0;/* ﾁﾗｰ能力低下確認ﾀｲﾏ */
uint8_t  MMODE = 0;/* MANUAL MODE FLAG */
uint8_t  MMDTM = 0;/* MANUAL MODE TIMER	*/
uint8_t  WCTM1 = 0;/* 洗浄中の電動弁動作ﾀｲﾏ	*/
uint8_t  WCTM2 = 0;/* エア抜き弁用ﾀｲﾏ */

/* OS設定値構造体について */
OS_TSKCon1 OSparam = {
    /* TSK_COND[TSKNUM] */
    {
        0x80, 0x80, 0x80, 0x80, 0x80,
        0x80, 0x80, 0x80
    },
    /* NEXT_LOAD_Time[TSKNUM] */
    {
        0,    0,    0,    0,    0,
        0,    0,    0
    }
};

OS_TSKCon1 *OSTSKCon1S(void)
{
    return &OSparam;
}

uint8_t *HENNP[32] = {
    /* NV-RAM保存エリアの先頭アドレス */
    &PASS1,/* 保存エリアのパスワード下位、正解は0x01  +0 */
    &PASS2,/* 保存エリアのパスワード上位、正解は0xAA  +1 */
    &SYSFLG,/* 運転フラグ  +2 */
    &ABNF,/* 異常フラグ  +3 */
    &PAT1,/* パターン1設定温度  +4 */
    &PAT2,/* パターン2設定温度  +5 */
    &PAT3,/* パターン3設定温度  +6 */
    &PAT4,/* パターン4設定温度  +7 */
    &RPAT,/* 運転パターン  +8 */
    &SWTMP0,/* 常温水槽の設定温度  +9 */
    &ABNCUT,/* 異常履歴と異常件数  +10 */
    &ABNH0,/* 異常履歴1  +11 */
    &ABNH1,/* 異常履歴2  +12 */
    &STIME1,/* 冷却開始時刻 下位  +13 */
    &STIME2,/* 冷却開始時刻 上位  +14 */
    &ETIME1,/* 冷却終了時刻 下位  +15 */
    &ETIME2,/* 冷却終了時刻 上位  +16 */
    &RTIMM,/* 積算稼働時間　分  +17 */
    &RTIMH1,/* 積算稼働時間　時間 下位  +18 */
    &RTIMH2,/* 積算稼働時間　時間 上位  +19 */
    &SETTM,/* チラー予約タイム　分  +20 */
    &SETTH,/* チラー予約タイム　時  +21 */
    &CSEQTS,/* 冷却工程シーケンスタイム（秒）  +22 */
    &CSEQTM,/* 冷却工程シーケンスタイム（分）  +23 */
    &CCSEQ,/* 冷却工程シーケンスナンバー  +24 */
    &SCTM1,/* パターン1徐冷時間  +25 */
    &SCTM2,/* パターン2徐冷時間  +26 */
    &SCTM3,/* パターン3徐冷時間  +27 */
    &SCTM4,/* パターン4徐冷時間  +28 */
    &NPAT,/* 現在選択中のパターン  +29 */
    &RWAITT,/* 運転待機時間（分）　MAX＝4時間  +30 */
    &CSUM/* SUM値  +31 */
};