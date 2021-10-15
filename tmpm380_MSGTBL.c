/***
 * �薼�FLCD/VFD�̕\�����b�Z�[�W�e�[�u��
 * �쐬�ҁF����@����
 * LCD/VFD�\�����b�Z�[�W�ꗗ
***/
#include "tmpm380_MSGTBL.h"

const uint8_t SPACCE[17] = {
    /* �X�y�[�X�݂̂̃��b�Z�[�W */
    16, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
};

/* ***** MEMORY CLEAR MESSAGE ***** */
const uint8_t MCLMS0[17] = {
    /* NV-RAM������ MESSAGE */
    16, 'N', 'V', '-', 'R', 'A', 'M', ' ', '�', ' ', '�', '�', '�', '�', ' ', '�', ' '
};
const uint8_t MCLMS1[17] = {
    /* NV-RAM������ MESSAGE */
	16, ' ', ' ', ' ', ' ', ' ', ' ', ' ', '�', '�', '�', '�', ' ', '�', '�', '�', '�'
};

/* *** TOP MESSAGE *** */
const uint8_t TOPMS1[17] = {
    16, '<', '�', '�', '�', '�', ' ', 'S', 'V', 'C', ' ', '-', ' ', '3', '5', 'R', '>'
};
const uint8_t TOPMS2[20] = {
    16, ' ', '�', '�', '�', '�', '�', ' ', '�', '�', '�', ' ', '�', '�', '�', '�', '�', '.', '.', '.'
};
const uint8_t ALMSG[17] = {
    16, ' ', '�', '�', ' ', '�', '�', '�', '�', ' ', '�', '�', ' ', '�', '�', '�', '�'
};

/* *** �d���̎��� MESSAGE *** */
const uint8_t CLKMS1[17] = {
    16, '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', ' ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t CLKMS2[17] = {
    16, ' ', '\'', ' ', ' ', '/', ' ', ' ', '/', ' ', ' ', ' ', ' ', ' ', ':', ' ', ' '
};

/* *** �ݒ莞�� MESSAGE *** */
const uint8_t DSMDHD[17] = {
    16, '<', '�', '�', '�', '�', ' ', '�', '�', '�', '�', '>', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD01[17] = {
    /* DSPSEQ = 1 */
    16, ' ', '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD02[17] = {
    /* DSPSEQ = 2 */
    16, ' ', '�', '�', '�', '�', ' ', '�', '�', '�', '�', ' ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD02A[17] = {
    /* DSPSEQ = 3 */
    16, ' ', '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD02B[17] = {
    /* DSPSEQ = 4 */
    16, ' ', '�', '�', '�', '�', '�', '�', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD03[17] = {
    /* DSPSEQ = 5 */
    16, ' ', '�', '�', '�', ' ', '�', '�', '�', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD04[17] = {
    /* DSPSEQ = 6 */
    16, ' ', '�', '�', '�', '�', ' ', '�', '�', '�', '�', ' ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD05[17] = {
    /* DSPSEQ = 7 */
    16, ' ', '�', '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�', ' ', ' ', ' ', ' '
};
const uint8_t DSMD06[17] = {
    16, '<', '�', '�', '�', '�', '�', ' ', ' ', '>', ' ', ' ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD07[17] = {
    /* DSPSSQ = 1 */
    16, '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�', '=', ' ', ' ', ' ', '�', 'C'
};
const uint8_t DSMD08[17] = {
    /* DSPSSQ = 2 */
    16, '�', '�', '�', '�', '�', ' ', ' ', ' ', ' ', ' ', '=', ' ', ' ', '�', '�', ' '
};
const uint8_t DSMD09[17] = {
    /* DSPSSQ = 3 */
    16, '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�', '=', ' ', ' ', ' ', '�', '�'
};
const uint8_t DSMD10[17] = {
    16, '<', '�', '�', '�', '�', ' ', '�', '�', '�', '�', '>', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD11[17] = {
    /* DSPSSQ = 1 */
    16, ' ', '�', '�', '�', '�', ' ', '�', '�', '�', '�', '=', ' ', ' ', ' ', '�', 'C'
};
const uint8_t DSMD11A[17] = {
    16, '<', '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�', '>', ' ', ' ', ' ', ' '
};
const uint8_t DSMD11B[17] = {
    /* DSPSSQ = 1 */
    16, ' ', '0', ':', '0', '0', ' ', '�', ' ', '�', '�', '�', '�', ' ', '�', '�', ' '
};
const uint8_t DSMD11C[17] = {
    16, ' ', '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�', ' ', '�', '�', '�', ' '
};
const uint8_t DSMD12[17] = {
    16, '<', '�', '�', '�', ' ', '�', '�', '�', '>', ' ', ' ', ' ', ' ', ':', ' ', ' '
};
const uint8_t DSMD13[17] = {
    /* DSPSSQ = 1,2 */
    16, ' ', '�', '�', '�', ' ', '�', '�', '�', '�', ' ', '=', ' ', ' ', ':', ' ', ' '
};
const uint8_t DSMD14[17] = {
    16, '<', '�', '�', '�', '�', ' ', '�', '�', '�', '�', '>', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD15[17] = {
    /* DSPSSQ = 1,2,3,4,5 */
    16, ' ', '\'', ' ', ' ', '/', ' ', ' ', '/', ' ', ' ', ' ', ' ', ' ', ':', ' ', ' '
};
const uint8_t DSMD16[17] = {
    16, '<', '�', '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�', '>', ' ', ' ', ' '
};
const uint8_t DSMD17[17] = {
    /* DSPSSQ = 1 */
    16, ' ', '�', '�', '�', '�', '�', ' ', '�', '�', '�', '=', ' ', ' ', ' ', '�', '�'
};
const uint8_t DSMD18[17] = {
    /* DSPSSQ = 2 */
    16, ' ', '�', '�', ' ', '�', '�', '�', '�', ' ', ' ', '=', ' ', ' ', '�', '�', ' '
};
const uint8_t DSMD19[17] = {
    /* DSPSSQ = 3 */
    16, ' ', '�', '�', '�', '�', '�', '�', '�', '�', ' ', '=', ' ', ' ', ' ', ' ', ' '
};
const uint8_t DSMD20[17] = {
    /* DSPSSQ = 4 */
    16, ' ', '�', '�', '�', '�', '�', '�', '�', '�', ' ', '=', ' ', ' ', ' ', '�', 'C'
};
const uint8_t DSMD21[17] = {
    /* DSPSSQ = 5 */
    16, ' ', '�', '�', '�', '�', '�', '�', ' ', ' ', ' ', '=', ' ', ' ', ' ', '�', 'C'
};
const uint8_t DSMD22[17] = {
    /* DSPSSQ = 6 */
    16, ' ', '�', '�', '�', '�', ' ', ' ', ' ', ' ', ' ', '=', ' ', ' ', ' ', '�', 'C'
};
const uint8_t DSMD23[17] = {
    /* DSPSSQ = 7 (0) */
    16, ' ', '�', '�', '�', '�', ' ', '=', ' ', '�', '�', '�', '�', '�', '�', '�', ' '
};
const uint8_t DSMD231[17] = {
    /* DSPSSQ = 7 (1) */
    16, ' ', '�', '�', '�', '�', ' ', '=', ' ', '�', '�', '�', '�', '�', '�', '�', '�'
};
const uint8_t DSMD232[17] = {
    /* DSPSSQ = 7 (2) */
    16, ' ', '�', '�', '�', '�', ' ', '=', ' ', '�', '�', '�', '�', '�', '�', ' ', ' '
};
const uint8_t DSMD23E[17] = {
    /* DSPSSQ = 7 (E) */
    16, ' ', '�', '�', '�', '�', ' ', '�', '�', '�', ' ', '�', '�', '�', '�', '�', '�'
};

const uint8_t *DSMDSTBL_1[] = {
    /* �ݒ��ʗp�\���e�[�u�� */
    DSMD01, DSMD02, DSMD02A, DSMD02B, DSMD03, DSMD04, DSMD05, DSMD06, DSMD07, DSMD08,
    DSMD09, DSMD10, DSMD11,  DSMD11A, DSMD11B, DSMD11C, DSMD12, DSMD13, DSMD14, DSMD15,
    DSMD16, DSMD17, DSMD18,  DSMD19,  DSMD20, DSMD20, DSMD21, DSMD22, DSMD23, DSMD231,
    DSMD232, DSMD23E
};

/* *** �^�]�ҋ@����MESSAGE *** */
const uint8_t RDYMS1[17] = {
    16, ' ', '<', '�', '�', '�', '�', '�', ' ', ' ', '�', '�', '�', '�', ' ', '>', ' '
};
const uint8_t RDYMS2[17] = {
    16, '�', '�', '�', '�', ' ', ' ', '�', 'C', '/', '�', '�', '�', '�', '�', '�', '�'
};

/* *** �^�]���� MESSAGE *** */
const uint8_t RNMS1[17] = {
    16, '�', '�', '�', '�', '�', ' ', '/', '�', '�', '�', '�', ' ', ' ', ' ', '�', 'C'
};
const uint8_t RNMS2[17] = {
    16, '�', '�', '�', '�', '�', ' ', '/', '�', '�', '�', '�', ' ', ' ', ' ', '�', 'C'
};
const uint8_t RNSTM1[17] = {
    16, ' ', '�', '�', '�', '�', ' ', '=', ' ', ' ', ' ', ' ', ' ', ' ', '�', 'C', ' '
};
const uint8_t RNSTM2[17] = {
    16, ' ', '�', '�', '�', ' ', ' ', '=', ' ', ' ', ' ', ' ', ' ', ':', ' ', ' ', ' '
};
const uint8_t RNSTM3[17] = {
    16, ' ', '�', '�', '�', ' ', ' ', '=', ' ', ' ', ' ', ' ', ' ', ' ', '�', '�', ' '
};

/* *** �I�� MESSAGE *** */
const uint8_t ENDMS1[17] = {
    16, '�', '�', '�', '�', '�', '�', '/', '�', '�', '�', '�', ' ', ' ', ' ', '�', 'C'
};
const uint8_t ENDMS2[17] = {
    16, ' ', '�', '�', '�', '�', '�', '�', '=', ' ', ' ', ' ', ' ', ':', ' ', ' ', ' '
};

/* *** �^��������� MESSAGE *** */
const uint8_t DESVC1[17] = {
    16, '�', '�', '�', '�', ' ', '�', '�', '�', '�', '�', ' ', '�', '�', '�', ' ', ' '
};

/* *** �r������ *** */
const uint8_t DRAIN1[17] = {
    16, '�', '�', '�', '�', ' ', '�', '�', '�', ' ', '�', '�', '�', '�', '�', '�', '�'
};
const uint8_t DRAIN2[17] = {
    16, '�', '�', '�', '�', ' ', '�', '�', '�', ' ', '�', '�', '�', ' ', ' ', ' ', ' '
};

/* *** ��򓮍� *** */
const uint8_t CLEAN1[17] = {
    /* ���m�Fү���� */
    16, '�', '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�', ' ', '?', ' ', ' ', ' '
};
const uint8_t CLEAN2[17] = {
    /* ���J�n�I��ү���� */
    16, ' ', ' ', ' ', '�', '�', ' ', ' ', ' ', ' ', '(', '�', '�', '�', ')', ' ', ' '
};
const uint8_t CLEAN3[17] = {
    /* ��򒆒f�I��ү���� */
    16, ' ', ' ', '�', '�', '�', '�', ' ', ' ', '(', '�', '�', '�', '�', '�', ')', ' '
};
const uint8_t CLEAN4[17] = {
    16, '<', '�', '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�', '>', ' ', ' ', ' '
};
const uint8_t CLEAN5[17] = {
    /* ���H���\�� */
    16, ' ', '�', '�', '�', '�', '�', '�', ' ', '�', '�', '�', ' ', ' ', ' ', ' ', ' '
};
const uint8_t CLEAN6[17] = {
    /* �������H���P�\�� */
    16, ' ', '1', '�', '�', '�', ' ', '�', ' ', '�', '�', '�', '�', '�', '�', '�', ' '
};
const uint8_t CLEAN7[17] = {
    /* �������H���Q�\�� */
    16, ' ', '2', '�', '�', '�', ' ', '�', ' ', '�', '�', '�', '�', '�', '�', '�', ' '
};
const uint8_t CLEAN8[17] = {
    /* ���I���\�� */
    16, ' ', '�', '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�', '�', '�', ' ', ' '
};

/* *** �\�m�x�� MESSAGE *** */
const uint8_t DALM1[17] = {
    16, ' ', '�', '�', '�', ' ', '�', ' ', '�', '�', '�', '�', '�', ' ', '�', '�', '�'
};
/* *** �ُ�\�� *** */
const uint8_t ABNTOP[17] = {
    16, '�', '�', '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�', '�', '�', '�', '�'
};
const uint8_t ABN01[17] = {
    16, ' ', '�', '�', '�', '�', '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�', '�'
};
const uint8_t ABN02[17] = {
    16, ' ', '�', '�', '�', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '�', '�', '�', '�', '�'
};
const uint8_t ABN03[17] = {
    16, ' ', '�', '�', '�', '�', '�', '�', '�', '�', ' ', ' ', '�', '�', '�', '�', '�'
};
const uint8_t ABN04[17] = {
    16, ' ', '�', '�', '�', '�', '�', ' ', ' ', ' ', ' ', ' ', '�', '�', '�', '�', '�'
};
const uint8_t ABN05[17] = {
    16, ' ', '�', '�', '�', '�', '�', ' ', ' ', ' ', ' ', ' ', '�', '�', '�', '�', '�'
};
const uint8_t ABN06[17] = {
    16, ' ', '�', '�', '�', '�', '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�', '�'
};
const uint8_t ABN07[17] = {
    16, ' ', '�', '�', '�', '�', '�', '�', '�', '�', '�', '1', '�', '�', '�', '�', '�'
};
const uint8_t ABN08[17] = {
    16, ' ', '�', '�', '�', '�', '�', '�', '�', '�', '�', '2', '�', '�', '�', '�', '�'
};
const uint8_t ABN09[17] = {
    16, ' ', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�'
};
const uint8_t ABN10[17] = {
    16, ' ', '�', '�', '�', '�', '�', '�', '�', '�', ' ', ' ', '�', '�', '�', '�', '�'
};
const uint8_t ABN11[17] = {
    16, ' ', '�', '�', '�', '�', '�', ' ', ' ', ' ', ' ', ' ', '�', '�', '�', '�', '�'
};
const uint8_t ABN12[17] = {
    16, ' ', '�', '�', '�', ' ', '�', '�', '�', ' ', ' ', ' ', '�', '�', '�', '�', '�'
};
const uint8_t ABN13[17] = {
    16, ' ', '�', '�', '�', '�', '�', '�', '�', '�', ' ', ' ', '�', '�', '�', '�', '�'
};
const uint8_t ABN14[17] = {
    16, ' ', '�', '�', '�', '�', '�', '�', ' ', ' ', ' ', ' ', '�', '�', '�', '�', '�'
};
const uint8_t ABN15[17] = {
    16, ' ', '�', '�', '�', '�', '�', '�', ' ', ' ', ' ', ' ', '�', '�', '�', '�', '�'
};
const uint8_t ABN16[17] = {
    16, ' ', '�', '�', '�', '�', '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�', '�'/* *���C���̂�* */
};
const uint8_t ABN17[17] = {
    16, ' ', '�', '�', '�', '�', '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�', '�'/* *���C���̂�* */
};
const uint8_t ABN18[17] = {
    16, ' ', '�', '�', '�', '�', ' ', '�', '�', '�', '�', ' ', '�', '�', '�', '�', '�'/* *���C���̂�* */
};

/* �ُ�̐擪�A�h���X�܂Ƃ� */
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
    /* LCD�̕\���m�F MESSAGE */
    16, 'L', 'C', 'D', ' ', '�', ' ', '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�'
};
const uint8_t TSTMS1[17] = {
    /* LED�̕\���m�F MESSAGE */
    16, 'L', 'E', 'D', ' ', '�', ' ', '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�'
};
const uint8_t TSTMS2[17] = {
    /* ���ٽ����̊m�F MESSAGE */
    16, 'S', 'W', '1', ' ', '�', ' ', '�', '�', '�', ' ', '�', '�', '�', '�', '�', ' '
};
const uint8_t TSTMS3[17] = {
    /* ����o�͂̊m�F MESSAGE */
    16, '�', '�', '�', '�', '�', '�', ' ', '�', ' ', '�', '�', '�', '�', ' ', ' ', ' '
};
const uint8_t TSTMS4[17] = {
    /* ���͂̊m�F MESSAGE */
    16, '�', '�', '�', '�', '�', '�', ' ', '�', ' ', '�', '�', '�', '�', ' ', ' ', ' '
};
const uint8_t TSTMS5[17] = {
    /* �i���ݻ��f���m�F MESSAGE */
    16, '�', '�', '�', '�', '�', '�', '�', '�', ' ', '�', '�', '�', '�', '�', ' ', ' '
};
const uint8_t TSTMS6[17] = {
    /* �ʐM�߰Ă̊m�F MESSAGE */
    16, '�', '�', '�', '�', ' ', '�', '�', '�', '�', ' ', '�', ' ', '�', '�', '�', '�'
};
const uint8_t TSTMS7[17] = {
    /* RTC�̊m�F MESSAGE */
    16, 'R', 'T', 'C', ' ', '�', ' ', '�', '�', '�', '�', ' ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t TSTMS8[17] = {
    /* NV-RAM�̊m�F MESSAGE */
    16, 'N', 'V', '-', 'R', 'A', 'M', ' ', '�', ' ', '�', '�', '�', '�', ' ', ' ', ' '
};
const uint8_t TSTMS9[17] = {
    /* OK MESSAGE */
    16, 'N', 'V', '-', 'R', 'A', 'M', ' ', ':', ' ', 'G', 'O', 'O', 'D', ' ', ' ', ' '
};
const uint8_t TSTMSA[17] = {
    /* �ُ� MESSAGE */
    16, 'N', 'V', '-', 'R', 'A', 'M', ' ', ':', ' ', '�', '�', '�', '�', '�', ' ', ' '
};
const uint8_t TSTMSB[17] = {
    /* DIP SW�̊m�F MESSAGE */
    16, ' ', 'D', 'I', 'P', ' ', '�', '�', '�', '�', ' ', ' ', ' ', ' ', ' ', ' ', ' '
};
const uint8_t TSTMSC[17] = {
    /* �d����/�ݗv�� */
    16, 'P', 'O', 'W', 'E', 'R', ' ', 'O', 'F', 'F', ' ', '-', '-', '>', ' ', 'O', 'N'
};
const uint8_t TSTMSD[17] = {
    /* HACCP�p�ړ_�o�͊m�F */
    16, 'H', 'A', 'C', 'C', 'P', ' ', '�', '�', '�', '�', ' ', ' ', ' ', ' ', ' ', ' '
};