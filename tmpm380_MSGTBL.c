/***
 * 題名：LCD/VFDの表示メッセージテーブル
 * 作成者：浅尾　享一
 * LCD/VFD表示メッセージ一覧
***/
#include "tmpm380_MSGTBL.h"

const uint8_t SPACCE[17] = {
    /* スペースのみのメッセージ */
    16, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
};

/* ***** MEMORY CLEAR MESSAGE ***** */
const uint8_t MCLMS0[17] = {
    /* NV-RAM初期化 MESSAGE */
    16, 'N', 'V', '-', 'R', 'A', 'M', ' ', 'ﾉ', ' ', 'ﾃ', 'ﾞ', 'ｰ', 'ﾀ', ' ', 'ｦ', ' '
};
const uint8_t MCLMS1[17] = {
    /* NV-RAM初期化 MESSAGE */
	16, ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'ｼ', 'ｮ', 'ｷ', 'ｶ', ' ', 'ｼ', 'ﾏ', 'ｼ', 'ﾀ'
};

/* *** TOP MESSAGE *** */
const uint8_t TOPMS1[17] = {
    16, '<', 'ｻ', 'ﾑ', 'ｿ', 'ﾝ', ' ', 'S', 'V', 'C', ' ', '-', ' ', '3', '5', 'R', '>'
};
const uint8_t TOPMS2[20] = {
    16, ' ', 'ｼ', 'ﾊ', 'ﾞ', 'ﾗ', 'ｸ', ' ', 'ｵ', 'ﾏ', 'ﾁ', ' ', 'ｸ', 'ﾀ', 'ﾞ', 'ｻ', 'ｲ', '.', '.', '.'
};
const uint8_t ALMSG[17] = {
    16, ' ', 'ﾖ', 'ﾁ', ' ', 'ｹ', 'ｲ', 'ﾎ', 'ｳ', ' ', 'ｶ', 'ﾞ', ' ', 'ｱ', 'ﾘ', 'ﾏ', 'ｽ'
};

/* *** 電源ｵﾌ時の MESSAGE *** */
const uint8_t CLKMS1[17] = {
    16, 'ｹ', 'ﾞ', 'ﾝ', 'ｻ', 'ﾞ', 'ｲ', 'ｼ', 'ﾞ', 'ｺ', 'ｸ', ' ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t CLKMS2[17] = {
    16, ' ', '\'', ' ', ' ', '/', ' ', ' ', '/', ' ', ' ', ' ', ' ', ' ', ':', ' ', ' '
};

/* *** 設定時の MESSAGE *** */
const uint8_t DSMDHD[17] = {
    16, '<', 'ｾ', 'ｯ', 'ﾃ', 'ｲ', ' ', 'ｺ', 'ｳ', 'ﾓ', 'ｸ', '>', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD01[17] = {
    /* DSPSEQ = 1 */
    16, ' ', 'ﾊ', 'ﾟ', 'ﾀ', 'ｰ', 'ﾝ', ' ', 'ﾄ', 'ｳ', 'ﾛ', 'ｸ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD02[17] = {
    /* DSPSEQ = 2 */
    16, ' ', 'ｽ', 'ｲ', 'ｿ', 'ｳ', ' ', 'ｵ', 'ﾝ', 'ﾄ', 'ﾞ', ' ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD02A[17] = {
    /* DSPSEQ = 3 */
    16, ' ', 'ｼ', 'ﾞ', 'ﾄ', 'ﾞ', 'ｳ', ' ', 'ﾊ', 'ｲ', 'ｽ', 'ｲ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD02B[17] = {
    /* DSPSEQ = 4 */
    16, ' ', 'ｾ', 'ﾝ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD03[17] = {
    /* DSPSEQ = 5 */
    16, ' ', 'ﾁ', 'ﾗ', 'ｰ', ' ', 'ﾖ', 'ﾔ', 'ｸ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD04[17] = {
    /* DSPSEQ = 6 */
    16, ' ', 'ｼ', 'ﾞ', 'ｺ', 'ｸ', ' ', 'ｾ', 'ｯ', 'ﾃ', 'ｲ', ' ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD05[17] = {
    /* DSPSEQ = 7 */
    16, ' ', 'ﾒ', 'ﾝ', 'ﾃ', 'ﾅ', 'ﾝ', 'ｽ', ' ', 'ﾓ', 'ｰ', 'ﾄ', 'ﾞ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD06[17] = {
    16, '<', 'ﾊ', 'ﾟ', 'ﾀ', 'ｰ', 'ﾝ', ' ', ' ', '>', ' ', ' ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD07[17] = {
    /* DSPSSQ = 1 */
    16, 'ﾚ', 'ｲ', 'ｷ', 'ｬ', 'ｸ', ' ', 'ｵ', 'ﾝ', 'ﾄ', 'ﾞ', '=', ' ', ' ', ' ', 'ﾟ', 'C'
};
const uint8_t DSMD08[17] = {
    /* DSPSSQ = 2 */
    16, 'ｼ', 'ﾞ', 'ｮ', 'ﾚ', 'ｲ', ' ', ' ', ' ', ' ', ' ', '=', ' ', ' ', 'ｱ', 'ﾘ', ' '
};
const uint8_t DSMD09[17] = {
    /* DSPSSQ = 3 */
    16, 'ｼ', 'ﾞ', 'ｮ', 'ﾚ', 'ｲ', ' ', 'ｼ', 'ﾞ', 'ｶ', 'ﾝ', '=', ' ', ' ', ' ', 'ﾌ', 'ﾝ'
};
const uint8_t DSMD10[17] = {
    16, '<', 'ｽ', 'ｲ', 'ｿ', 'ｳ', ' ', 'ｵ', 'ﾝ', 'ﾄ', 'ﾞ', '>', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD11[17] = {
    /* DSPSSQ = 1 */
    16, ' ', 'ｽ', 'ｲ', 'ｿ', 'ｳ', ' ', 'ｵ', 'ﾝ', 'ﾄ', 'ﾞ', '=', ' ', ' ', ' ', 'ﾟ', 'C'
};
const uint8_t DSMD11A[17] = {
    16, '<', 'ｼ', 'ﾞ', 'ﾄ', 'ﾞ', 'ｳ', ' ', 'ﾊ', 'ｲ', 'ｽ', 'ｲ', '>', ' ', ' ', ' ', ' '
};
const uint8_t DSMD11B[17] = {
    /* DSPSSQ = 1 */
    16, ' ', '0', ':', '0', '0', ' ', 'ﾆ', ' ', 'ﾊ', 'ｲ', 'ｽ', 'ｲ', ' ', 'ｽ', 'ﾙ', ' '
};
const uint8_t DSMD11C[17] = {
    16, ' ', 'ｼ', 'ﾞ', 'ﾄ', 'ﾞ', 'ｳ', ' ', 'ﾊ', 'ｲ', 'ｽ', 'ｲ', ' ', 'ｼ', 'ﾅ', 'ｲ', ' '
};
const uint8_t DSMD12[17] = {
    16, '<', 'ﾁ', 'ﾗ', 'ｰ', ' ', 'ﾖ', 'ﾔ', 'ｸ', '>', ' ', ' ', ' ', ' ', ':', ' ', ' '
};
const uint8_t DSMD13[17] = {
    /* DSPSSQ = 1,2 */
    16, ' ', 'ﾖ', 'ﾔ', 'ｸ', ' ', 'ｼ', 'ﾞ', 'ｺ', 'ｸ', ' ', '=', ' ', ' ', ':', ' ', ' '
};
const uint8_t DSMD14[17] = {
    16, '<', 'ｼ', 'ﾞ', 'ｺ', 'ｸ', ' ', 'ｾ', 'ｯ', 'ﾃ', 'ｲ', '>', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD15[17] = {
    /* DSPSSQ = 1,2,3,4,5 */
    16, ' ', '\'', ' ', ' ', '/', ' ', ' ', '/', ' ', ' ', ' ', ' ', ' ', ':', ' ', ' '
};
const uint8_t DSMD16[17] = {
    16, '<', 'ﾒ', 'ﾝ', 'ﾃ', 'ﾅ', 'ﾝ', 'ｽ', ' ', 'ﾓ', 'ｰ', 'ﾄ', 'ﾞ', '>', ' ', ' ', ' '
};
const uint8_t DSMD17[17] = {
    /* DSPSSQ = 1 */
    16, ' ', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ', ' ', 'ﾘ', 'ﾚ', 'ｷ', '=', ' ', ' ', ' ', 'ｹ', 'ﾝ'
};
const uint8_t DSMD18[17] = {
    /* DSPSSQ = 2 */
    16, ' ', 'ﾖ', 'ﾁ', ' ', 'ｹ', 'ｲ', 'ﾎ', 'ｳ', ' ', ' ', '=', ' ', ' ', 'ｱ', 'ﾘ', ' '
};
const uint8_t DSMD19[17] = {
    /* DSPSSQ = 3 */
    16, ' ', 'ｶ', 'ﾄ', 'ﾞ', 'ｳ', 'ｼ', 'ﾞ', 'ｶ', 'ﾝ', ' ', '=', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD20[17] = {
    /* DSPSSQ = 4 */
    16, ' ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ', 'ｵ', 'ﾝ', 'ｿ', 'ｳ', ' ', '=', ' ', ' ', ' ', 'ﾟ', 'C'
};
const uint8_t DSMD21[17] = {
    /* DSPSSQ = 5 */
    16, ' ', 'ﾃ', 'ｲ', 'ｵ', 'ﾝ', 'ｿ', 'ｳ', ' ', ' ', ' ', '=', ' ', ' ', ' ', 'ﾟ', 'C'
};
const uint8_t DSMD22[17] = {
    /* DSPSSQ = 6 */
    16, ' ', 'ﾋ', 'ﾝ', 'ｵ', 'ﾝ', ' ', ' ', ' ', ' ', ' ', '=', ' ', ' ', ' ', 'ﾟ', 'C'
};
const uint8_t DSMD23[17] = {
    /* DSPSSQ = 7 (0) */
    16, ' ', 'ﾊ', 'ｲ', 'ｽ', 'ｲ', ' ', '=', ' ', 'ｽ', 'ﾍ', 'ﾞ', 'ﾃ', 'ﾉ', 'ｿ', 'ｳ', ' '
};
const uint8_t DSMD231[17] = {
    /* DSPSSQ = 7 (1) */
    16, ' ', 'ﾊ', 'ｲ', 'ｽ', 'ｲ', ' ', '=', ' ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ', 'ｵ', 'ﾝ', 'ｿ', 'ｳ'
};
const uint8_t DSMD232[17] = {
    /* DSPSSQ = 7 (2) */
    16, ' ', 'ﾊ', 'ｲ', 'ｽ', 'ｲ', ' ', '=', ' ', 'ﾃ', 'ｲ', 'ｵ', 'ﾝ', 'ｿ', 'ｳ', ' ', ' '
};
const uint8_t DSMD23E[17] = {
    /* DSPSSQ = 7 (E) */
    16, ' ', 'ﾊ', 'ｲ', 'ｽ', 'ｲ', ' ', 'ｿ', 'ｳ', 'ｻ', ' ', 'ﾃ', 'ﾞ', 'ｷ', 'ﾏ', 'ｾ', 'ﾝ'
};

const uint8_t *DSMDSTBL_1[] = {
    /* 設定画面用表示テーブル */
    DSMD01, DSMD02, DSMD02A, DSMD02B, DSMD03, DSMD04, DSMD05, DSMD06, DSMD07, DSMD08,
    DSMD09, DSMD10, DSMD11,  DSMD11A, DSMD11B, DSMD11C, DSMD12, DSMD13, DSMD14, DSMD15,
    DSMD16, DSMD17, DSMD18,  DSMD19,  DSMD20, DSMD20, DSMD21, DSMD22, DSMD23, DSMD231,
    DSMD232, DSMD23E
};

/* *** 運転待機時のMESSAGE *** */
const uint8_t RDYMS1[17] = {
    16, ' ', '<', 'ﾊ', 'ﾟ', 'ﾀ', 'ｰ', 'ﾝ', ' ', ' ', 'ｾ', 'ﾝ', 'ﾀ', 'ｸ', ' ', '>', ' '
};
const uint8_t RDYMS2[17] = {
    16, 'ﾋ', 'ﾝ', 'ｵ', 'ﾝ', ' ', ' ', 'ﾟ', 'C', '/', 'ｼ', 'ﾞ', 'ｮ', 'ﾚ', 'ｲ', 'ｱ', 'ﾘ'
};

/* *** 運転中の MESSAGE *** */
const uint8_t RNMS1[17] = {
    16, 'ｼ', 'ﾞ', 'ｮ', 'ﾚ', 'ｲ', ' ', '/', 'ｾ', 'ｯ', 'ﾃ', 'ｲ', ' ', ' ', ' ', 'ﾟ', 'C'
};
const uint8_t RNMS2[17] = {
    16, 'ﾚ', 'ｲ', 'ｷ', 'ｬ', 'ｸ', ' ', '/', 'ｾ', 'ｯ', 'ﾃ', 'ｲ', ' ', ' ', ' ', 'ﾟ', 'C'
};
const uint8_t RNSTM1[17] = {
    16, ' ', 'ﾋ', 'ﾝ', 'ｵ', 'ﾝ', ' ', '=', ' ', ' ', ' ', ' ', ' ', ' ', 'ﾟ', 'C', ' '
};
const uint8_t RNSTM2[17] = {
    16, ' ', 'ｶ', 'ｲ', 'ｼ', ' ', ' ', '=', ' ', ' ', ' ', ' ', ' ', ':', ' ', ' ', ' '
};
const uint8_t RNSTM3[17] = {
    16, ' ', 'ｹ', 'ｲ', 'ｶ', ' ', ' ', '=', ' ', ' ', ' ', ' ', ' ', ' ', 'ﾌ', 'ﾝ', ' '
};

/* *** 終了 MESSAGE *** */
const uint8_t ENDMS1[17] = {
    16, 'ｼ', 'ｭ', 'ｳ', 'ﾘ', 'ｮ', 'ｳ', '/', 'ｾ', 'ｯ', 'ﾃ', 'ｲ', ' ', ' ', ' ', 'ﾟ', 'C'
};
const uint8_t ENDMS2[17] = {
    16, ' ', 'ｼ', 'ｭ', 'ｳ', 'ﾘ', 'ｮ', 'ｳ', '=', ' ', ' ', ' ', ' ', ':', ' ', ' ', ' '
};

/* *** 真空解除中の MESSAGE *** */
const uint8_t DESVC1[17] = {
    16, 'ｼ', 'ﾝ', 'ｸ', 'ｳ', ' ', 'ｶ', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', ' ', 'ﾁ', 'ｭ', 'ｳ', ' ', ' '
};

/* *** 排水動作 *** */
const uint8_t DRAIN1[17] = {
    16, 'ﾊ', 'ｲ', 'ｽ', 'ｲ', ' ', 'ｿ', 'ｳ', 'ｻ', ' ', 'ｳ', 'ｹ', 'ﾂ', 'ｹ', 'ﾁ', 'ｭ', 'ｳ'
};
const uint8_t DRAIN2[17] = {
    16, 'ﾊ', 'ｲ', 'ｽ', 'ｲ', ' ', 'ﾁ', 'ｭ', 'ｳ', ' ', 'ﾃ', 'ﾞ', 'ｽ', ' ', ' ', ' ', ' '
};

/* *** 洗浄動作 *** */
const uint8_t CLEAN1[17] = {
    /* 洗浄確認ﾒｯｾｰｼﾞ */
    16, 'ｾ', 'ﾝ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ', ' ', 'ｼ', 'ﾏ', 'ｽ', 'ｶ', ' ', '?', ' ', ' ', ' '
};
const uint8_t CLEAN2[17] = {
    /* 洗浄開始選択ﾒｯｾｰｼﾞ */
    16, ' ', ' ', ' ', 'ﾊ', 'ｲ', ' ', ' ', ' ', ' ', '(', 'ｲ', 'ｲ', 'ｴ', ')', ' ', ' '
};
const uint8_t CLEAN3[17] = {
    /* 洗浄中断選択ﾒｯｾｰｼﾞ */
    16, ' ', ' ', 'ﾁ', 'ｭ', 'ｳ', 'ｼ', ' ', ' ', '(', 'ｹ', 'ｲ', 'ｿ', 'ﾞ', 'ｸ', ')', ' '
};
const uint8_t CLEAN4[17] = {
    16, '<', 'ｾ', 'ﾝ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ', ' ', 'ﾓ', 'ｰ', 'ﾄ', 'ﾞ', '>', ' ', ' ', ' '
};
const uint8_t CLEAN5[17] = {
    /* 洗浄工程表示 */
    16, ' ', 'ｾ', 'ﾝ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ', ' ', 'ﾁ', 'ｭ', 'ｳ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t CLEAN6[17] = {
    /* すすぎ工程１表示 */
    16, ' ', '1', 'ｶ', 'ｲ', 'ﾒ', ' ', 'ﾉ', ' ', 'ｽ', 'ｽ', 'ｷ', 'ﾞ', 'ﾁ', 'ｭ', 'ｳ', ' '
};
const uint8_t CLEAN7[17] = {
    /* すすぎ工程２表示 */
    16, ' ', '2', 'ｶ', 'ｲ', 'ﾒ', ' ', 'ﾉ', ' ', 'ｽ', 'ｽ', 'ｷ', 'ﾞ', 'ﾁ', 'ｭ', 'ｳ', ' '
};
const uint8_t CLEAN8[17] = {
    /* 洗浄終了表示 */
    16, ' ', 'ｾ', 'ﾝ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ', ' ', 'ｼ', 'ｭ', 'ｳ', 'ﾘ', 'ｮ', 'ｳ', ' ', ' '
};

/* *** 予知警報 MESSAGE *** */
const uint8_t DALM1[17] = {
    16, ' ', 'ﾁ', 'ﾗ', 'ｰ', ' ', 'ﾉ', ' ', 'ﾉ', 'ｳ', 'ﾘ', 'ｮ', 'ｸ', ' ', 'ﾃ', 'ｲ', 'ｶ'
};
/* *** 異常表示 *** */
const uint8_t ABNTOP[17] = {
    16, 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ', 'ｶ', 'ﾞ', ' ', 'ﾊ', 'ｯ', 'ｾ', 'ｲ', 'ｼ', 'ﾏ', 'ｼ', 'ﾀ'
};
const uint8_t ABN01[17] = {
    16, ' ', 'ｻ', 'ｰ', 'ﾏ', 'ﾙ', 'ﾄ', 'ﾘ', 'ｯ', 'ﾌ', 'ﾟ', ' ', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ'
};
const uint8_t ABN02[17] = {
    16, ' ', 'ﾁ', 'ﾗ', 'ｰ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ'
};
const uint8_t ABN03[17] = {
    16, ' ', 'ﾋ', 'ﾝ', 'ｵ', 'ﾝ', 'ｾ', 'ﾝ', 'ｻ', 'ｰ', ' ', ' ', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ'
};
const uint8_t ABN04[17] = {
    16, ' ', 'ﾚ', 'ｲ', 'ｷ', 'ｬ', 'ｸ', ' ', ' ', ' ', ' ', ' ', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ'
};
const uint8_t ABN05[17] = {
    16, ' ', 'ｷ', 'ｭ', 'ｳ', 'ｽ', 'ｲ', ' ', ' ', ' ', ' ', ' ', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ'
};
const uint8_t ABN06[17] = {
    16, ' ', 'ﾃ', 'ﾞ', 'ﾝ', 'ﾄ', 'ﾞ', 'ｳ', 'ﾍ', 'ﾞ', 'ﾝ', ' ', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ'
};
const uint8_t ABN07[17] = {
    16, ' ', 'ｽ', 'ｲ', 'ｲ', 'ﾃ', 'ﾞ', 'ﾝ', 'ｷ', 'ｮ', 'ｸ', '1', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ'
};
const uint8_t ABN08[17] = {
    16, ' ', 'ｽ', 'ｲ', 'ｲ', 'ﾃ', 'ﾞ', 'ﾝ', 'ｷ', 'ｮ', 'ｸ', '2', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ'
};
const uint8_t ABN09[17] = {
    16, ' ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ', 'ｵ', 'ﾝ', 'ｾ', 'ﾝ', 'ｻ', 'ｰ', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ'
};
const uint8_t ABN10[17] = {
    16, ' ', 'ﾃ', 'ｲ', 'ｵ', 'ﾝ', 'ｾ', 'ﾝ', 'ｻ', 'ｰ', ' ', ' ', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ'
};
const uint8_t ABN11[17] = {
    16, ' ', 'ﾃ', 'ｲ', 'ﾃ', 'ﾞ', 'ﾝ', ' ', ' ', ' ', ' ', ' ', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ'
};
const uint8_t ABN12[17] = {
    16, ' ', 'ﾄ', 'ｹ', 'ｲ', ' ', 'ｷ', 'ﾉ', 'ｳ', ' ', ' ', ' ', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ'
};
const uint8_t ABN13[17] = {
    16, ' ', 'ﾊ', 'ﾞ', 'ｯ', 'ｸ', 'ｱ', 'ｯ', 'ﾌ', 'ﾟ', ' ', ' ', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ'
};
const uint8_t ABN14[17] = {
    16, ' ', 'ｼ', 'ﾝ', 'ｸ', 'ｳ', 'ﾄ', 'ﾞ', ' ', ' ', ' ', ' ', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ'
};
const uint8_t ABN15[17] = {
    16, ' ', 'ｺ', 'ﾝ', 'ﾄ', 'ﾛ', 'ｰ', 'ﾗ', ' ', ' ', ' ', ' ', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ'
};
const uint8_t ABN16[17] = {
    16, ' ', 'ｸ', 'ｰ', 'ﾘ', 'ﾝ', 'ｸ', 'ﾞ', 'ﾀ', 'ﾜ', 'ｰ', ' ', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ'/* *蒸気式のみ* */
};
const uint8_t ABN17[17] = {
    16, ' ', 'ｽ', 'ｲ', 'ｲ', 'ﾃ', 'ﾞ', 'ﾝ', 'ｷ', 'ｮ', 'ｸ', ' ', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ'/* *蒸気式のみ* */
};
const uint8_t ABN18[17] = {
    16, ' ', 'ｽ', 'ｲ', 'ｵ', 'ﾝ', ' ', 'ｾ', 'ﾝ', 'ｻ', 'ｰ', ' ', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ'/* *蒸気式のみ* */
};

/* 異常の先頭アドレスまとめ */
const uint8_t *ABNFSTBL[] = {
    ABNTOP, ABN01, ABN02, ABN03, ABN04, ABN05, ABN06, ABN07, ABN08, ABN09,
    ABN10,  ABN11, ABN12, ABN13, ABN14, ABN15, ABN16, ABN17, ABN18
};

/* ***** SELF TEST MESSAGE ***** */
const uint8_t TSTTOP[17] = {
    /* TOP MESSAGE of SELF TEST MODE */
    16, '<', 'S', 'E', 'L', 'F', ' ', 'T', 'E', 'S', 'T', ' ', 'M', 'O', 'D', 'E', '>'
};
const uint8_t TSTMS0[17] = {
    /* LCDの表示確認 MESSAGE */
    16, 'L', 'C', 'D', ' ', 'ﾉ', ' ', 'ﾋ', 'ｮ', 'ｳ', 'ｼ', 'ﾞ', ' ', 'ｶ', 'ｸ', 'ﾆ', 'ﾝ'
};
const uint8_t TSTMS1[17] = {
    /* LEDの表示確認 MESSAGE */
    16, 'L', 'E', 'D', ' ', 'ﾉ', ' ', 'ﾋ', 'ｮ', 'ｳ', 'ｼ', 'ﾞ', ' ', 'ｶ', 'ｸ', 'ﾆ', 'ﾝ'
};
const uint8_t TSTMS2[17] = {
    /* ﾊﾟﾈﾙｽｲｯﾁの確認 MESSAGE */
    16, 'S', 'W', '1', ' ', 'ｦ', ' ', 'ｵ', 'ｼ', 'ﾃ', ' ', 'ｸ', 'ﾀ', 'ﾞ', 'ｻ', 'ｲ', ' '
};
const uint8_t TSTMS3[17] = {
    /* 制御出力の確認 MESSAGE */
    16, 'ｼ', 'ｭ', 'ﾂ', 'ﾘ', 'ｮ', 'ｸ', ' ', 'ﾉ', ' ', 'ｶ', 'ｸ', 'ﾆ', 'ﾝ', ' ', ' ', ' '
};
const uint8_t TSTMS4[17] = {
    /* 入力の確認 MESSAGE */
    16, 'ﾆ', 'ｭ', 'ｳ', 'ﾘ', 'ｮ', 'ｸ', ' ', 'ﾉ', ' ', 'ｶ', 'ｸ', 'ﾆ', 'ﾝ', ' ', ' ', ' '
};
const uint8_t TSTMS5[17] = {
    /* 品温ｾﾝｻｰ断線確認 MESSAGE */
    16, 'ﾋ', 'ﾝ', 'ｵ', 'ﾝ', 'ｾ', 'ﾝ', 'ｻ', 'ｰ', ' ', 'ｵ', 'ｰ', 'ﾌ', 'ﾟ', 'ﾝ', ' ', ' '
};
const uint8_t TSTMS6[17] = {
    /* 通信ﾎﾟｰﾄの確認 MESSAGE */
    16, 'ﾂ', 'ｳ', 'ｼ', 'ﾝ', ' ', 'ﾎ', 'ﾟ', 'ｰ', 'ﾄ', ' ', 'ﾉ', ' ', 'ｶ', 'ｸ', 'ﾆ', 'ﾝ'
};
const uint8_t TSTMS7[17] = {
    /* RTCの確認 MESSAGE */
    16, 'R', 'T', 'C', ' ', 'ﾉ', ' ', 'ｶ', 'ｸ', 'ﾆ', 'ﾝ', ' ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t TSTMS8[17] = {
    /* NV-RAMの確認 MESSAGE */
    16, 'N', 'V', '-', 'R', 'A', 'M', ' ', 'ﾉ', ' ', 'ｶ', 'ｸ', 'ﾆ', 'ﾝ', ' ', ' ', ' '
};
const uint8_t TSTMS9[17] = {
    /* OK MESSAGE */
    16, 'N', 'V', '-', 'R', 'A', 'M', ' ', ':', ' ', 'G', 'O', 'O', 'D', ' ', ' ', ' '
};
const uint8_t TSTMSA[17] = {
    /* 異常 MESSAGE */
    16, 'N', 'V', '-', 'R', 'A', 'M', ' ', ':', ' ', 'ｲ', 'ｼ', 'ﾞ', 'ｮ', 'ｳ', ' ', ' '
};
const uint8_t TSTMSB[17] = {
    /* DIP SWの確認 MESSAGE */
    16, ' ', 'D', 'I', 'P', ' ', 'ｽ', 'ｲ', 'ｯ', 'ﾁ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t TSTMSC[17] = {
    /* 電源ｵﾌ/ｵﾝ要求 */
    16, 'P', 'O', 'W', 'E', 'R', ' ', 'O', 'F', 'F', ' ', '-', '-', '>', ' ', 'O', 'N'
};
const uint8_t TSTMSD[17] = {
    /* HACCP用接点出力確認 */
    16, 'H', 'A', 'C', 'C', 'P', ' ', 'ｶ', 'ｸ', 'ﾆ', 'ﾝ', ' ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t TSTMSE[17] = {
    /* DAC出力/停電入力確認 */
    16, 'D', 'A', 'C', ':', ' ', ' ', ' ', ' ', ',', 'T', 'D', ':', 'O', 'F', 'F', ' '
};