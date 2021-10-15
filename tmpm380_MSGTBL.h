#ifndef __TMPM380_MSGTBL_H
#define __TMPM380_MSGTBL_H

#ifndef __MAIN_H
#include "main.h"
#endif

extern const uint8_t SPACCE[17];/* �X�y�[�X�݂̂̃��b�Z�[�W */

extern const uint8_t MCLMS0[17];/* NV-RAM������ MESSAGE */
extern const uint8_t MCLMS1[17];/* NV-RAM������ MESSAGE */

/* *** TOP MESSAGE *** */
extern const uint8_t TOPMS1[17];
extern const uint8_t TOPMS2[20];
extern const uint8_t  ALMSG[17];

/* *** �d���̎��� MESSAGE *** */
extern const uint8_t CLKMS1[17];
extern const uint8_t CLKMS2[17];

/* *** �ݒ莞�� MESSAGE *** */
extern const uint8_t DSMDHD[17];
extern const uint8_t DSMD01[17];/* DSPSEQ = 1 */
extern const uint8_t DSMD02[17];/* DSPSEQ = 2 */
extern const uint8_t DSMD02A[17];/* DSPSEQ = 3 */
extern const uint8_t DSMD02B[17];/* DSPSEQ = 4 */
extern const uint8_t DSMD03[17];/* DSPSEQ = 5 */
extern const uint8_t DSMD04[17];/* DSPSEQ = 6 */
extern const uint8_t DSMD05[17];/* DSPSEQ = 7 */
extern const uint8_t DSMD06[17];
extern const uint8_t DSMD07[17];/* DSPSSQ = 1 */
extern const uint8_t DSMD08[17];/* DSPSSQ = 2 */
extern const uint8_t DSMD09[17];/* DSPSSQ = 3 */
extern const uint8_t DSMD10[17];
extern const uint8_t DSMD11[17];/* DSPSSQ = 1 */
extern const uint8_t DSMD11A[17];
extern const uint8_t DSMD11B[17];/* DSPSSQ = 1 */
extern const uint8_t DSMD11C[17];
extern const uint8_t DSMD12[17];
extern const uint8_t DSMD13[17];/* DSPSSQ = 1,2 */
extern const uint8_t DSMD14[17];
extern const uint8_t DSMD15[17];/* DSPSSQ = 1,2,3,4,5 */
extern const uint8_t DSMD16[17];
extern const uint8_t DSMD17[17];/* DSPSSQ = 1 */
extern const uint8_t DSMD18[17];/* DSPSSQ = 2 */
extern const uint8_t DSMD19[17];/* DSPSSQ = 3 */
extern const uint8_t DSMD20[17];/* DSPSSQ = 4 */
extern const uint8_t DSMD21[17];/* DSPSSQ = 5 */
extern const uint8_t DSMD22[17];/* DSPSSQ = 6 */
extern const uint8_t DSMD23[17];/* DSPSSQ = 7 (0) */
extern const uint8_t DSMD231[17];/* DSPSSQ = 7 (1) */
extern const uint8_t DSMD232[17];/* DSPSSQ = 7 (2) */
extern const uint8_t DSMD23E[17];/* DSPSSQ = 7 (E) */

extern const uint8_t *DSMDSTBL_1[];/* �ݒ��ʗp�\���e�[�u�� */

/* *** �^�]�ҋ@����MESSAGE *** */
extern const uint8_t RDYMS1[17];
extern const uint8_t RDYMS2[17];

/* *** �^�]���� MESSAGE *** */
extern const uint8_t RNMS1[17];
extern const uint8_t RNMS2[17];
extern const uint8_t RNSTM1[17];
extern const uint8_t RNSTM2[17];
extern const uint8_t RNSTM3[17];

/* *** �I�� MESSAGE *** */
extern const uint8_t ENDMS1[17];
extern const uint8_t ENDMS2[17];

/* *** �^��������� MESSAGE *** */
extern const uint8_t DESVC1[17];

/* *** �r������ *** */
extern const uint8_t DRAIN1[17];
extern const uint8_t DRAIN2[17];

/* *** ��򓮍� *** */
extern const uint8_t CLEAN1[17];/* ���m�Fү���� */
extern const uint8_t CLEAN2[17];/* ���J�n�I��ү���� */
extern const uint8_t CLEAN3[17];/* ��򒆒f�I��ү���� */
extern const uint8_t CLEAN4[17];
extern const uint8_t CLEAN5[17];/* ���H���\�� */
extern const uint8_t CLEAN6[17];/* �������H���P�\�� */
extern const uint8_t CLEAN7[17];/* �������H���Q�\�� */
extern const uint8_t CLEAN8[17];/* ���I���\�� */

/* *** �\�m�x�� MESSAGE *** */
extern const uint8_t DALM1[17];
/* *** �ُ�\�� *** */
extern const uint8_t ABNTOP[17];
extern const uint8_t ABN01[17];
extern const uint8_t ABN02[17];
extern const uint8_t ABN03[17];
extern const uint8_t ABN04[17];
extern const uint8_t ABN05[17];
extern const uint8_t ABN06[17];
extern const uint8_t ABN07[17];
extern const uint8_t ABN08[17];
extern const uint8_t ABN09[17];
extern const uint8_t ABN10[17];
extern const uint8_t ABN11[17];
extern const uint8_t ABN12[17];
extern const uint8_t ABN13[17];
extern const uint8_t ABN14[17];
extern const uint8_t ABN15[17];
extern const uint8_t ABN16[17];
extern const uint8_t ABN17[17];
extern const uint8_t ABN18[17];

/* �ُ�̐擪�A�h���X�܂Ƃ� */
extern const uint8_t *ABNFSTBL[];

/* ***** SELF TEST MESSAGE ***** */
extern const uint8_t TSTTOP[17];/* TOP MESSAGE of SELF TEST MODE */
extern const uint8_t TSTMS0[17];/* LCD�̕\���m�F MESSAGE */
extern const uint8_t TSTMS1[17];/* LED�̕\���m�F MESSAGE */
extern const uint8_t TSTMS2[17];/* ���ٽ����̊m�F MESSAGE */
extern const uint8_t TSTMS3[17];/* ����o�͂̊m�F MESSAGE */
extern const uint8_t TSTMS4[17];/* ���͂̊m�F MESSAGE */
extern const uint8_t TSTMS5[17];/* �i���ݻ��f���m�F MESSAGE */
extern const uint8_t TSTMS6[17];/* �ʐM�߰Ă̊m�F MESSAGE */
extern const uint8_t TSTMS7[17];/* RTC�̊m�F MESSAGE */
extern const uint8_t TSTMS8[17];/* NV-RAM�̊m�F MESSAGE */
extern const uint8_t TSTMS9[17];/* OK MESSAGE */
extern const uint8_t TSTMSA[17];/* �ُ� MESSAGE */
extern const uint8_t TSTMSB[17];/* DIP SW�̊m�F MESSAGE */
extern const uint8_t TSTMSC[17];/* �d����/�ݗv�� */
extern const uint8_t TSTMSD[17];/* HACCP�p�ړ_�o�͊m�F */

#endif