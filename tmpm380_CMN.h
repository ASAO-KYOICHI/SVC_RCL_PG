#ifndef __TMPM380_CMN_H
#define __TMPM380_CMN_H

#ifndef __MAIN_H
#include "main.h"
#endif

/* �ύX���Ȃ��ϐ��l�̒�`---------------------------------------------------- */
#define TSKNUM                   ((uint8_t)8)/* �^�X�N����ݒ� */
#define TMRB_1MS                 0x1388/* fsys = fc = 10MHz * PLL = 40MHz, fphiT0 = fperiph = fgear = fc */
                                 /* ftmrb = 1/8fphiT0 = 1/8 * 40 = 5MHz, Ttmrb = 0.2us, 1000us/0.2us = 5000 = 0x1388 */
#define SELF_ADDR                ((uint8_t)0x70)/* I2C�ʐM�p ���g�̃A�h���X(�K����0b01110000) */
#define DACN_ADDR                ((uint8_t)0x22)/* D/A���ް��̃A�h���X(0b0010001X) */
#define RTC_ADDR                 ((uint8_t)0x64)/*RTC�̃A�h���X(0b0110010X)*/

extern uint8_t  kidou;/* �N���㏉�����ɂ��ި��߽����̊m�F�Ŏg�p�����׸ޕϐ� */
extern uint8_t  count01;/* �N���㏉���������ި��߽����̊m�F�Ŏg�p���鶳�ı��ߕϐ� */
extern uint8_t  ERRFG;/* �G���[�`�F�b�N�p�̃t���O */

extern uint8_t  STSFL0;/* �V�X�e������t���O0 */
extern uint8_t  STSFL1;/* �V�X�e������t���O1 */
extern uint8_t  D_INISQ;/* ���������؂̼��ݽ�ԍ� */
extern uint8_t  INI_STT;/* �������T�u�V�[�P���X */
extern uint8_t  DIPBUF;/* �ި��߽������ޯ̧ */
extern uint8_t  SYKND0;/* �ި��߽����ɂ��@��ݒ�ێ��ޯ̧ */

/* I2C�ʐM�Ɏg�p�����ޯ̧ */
extern uint8_t NACK5;/* ���NACK5 */
extern uint8_t I2C_WCNT;/* I2C�����݉� */
extern uint8_t I2C_RCNT;/* I2C�Ǎ��݉� */
extern uint8_t I2C_STATE;/* I2C�ʐM�����݂̽ðĔԍ� */
extern uint16_t SEC_CNT;/* 3�b�J�E���^ */
extern uint8_t I2C_WAIT;/* �ʏ�ʐM���m�F���̑҂����� */
extern uint8_t I2CBUF[32];/* I2C�ʐM�̑��M�ޯ̧ */
extern uint8_t FRAM_FG;/* �N������F-RAM�ǂݏo���t���O */
extern uint8_t I2CCNT;/* I2C�ʐM�񐔂̃J�E���g */
extern uint8_t T0BF0;/* �ėp�o�b�t�@0 */
extern uint8_t T0BF1;/* �ėp�o�b�t�@1 */
extern uint8_t T0BF2;/* �ėp�o�b�t�@2 */
extern uint8_t BOOT_SEC;/* ��N�����Ă���̎��� */
extern uint8_t WR_RTC;/* RTC�������݃t���O */
extern uint8_t RTC_ERR;/* RTC�G���[�t���O */
extern uint8_t RTC_ECT;/* RTC�G���[�� */
extern uint8_t RTCDT[10];/* RTC�̎�M�o�b�t�@ */
extern uint8_t RTCD_R[10];/* RTC�̎�M�o�b�t�@(BCD���̂܂��) */
extern uint8_t RTCKBF[10];/* RTC�̕b�m�F�o�b�t�@ */
extern uint8_t SETST_BUF[10];/* RTC�������ݗp�̃o�b�t�@ */
extern uint8_t RWTMS;/* �^�]�ҋ@�v����� */

extern uint16_t MSECD;/* �~���b�̃J�E���g�ϐ� */
extern uint16_t SECND;/* �b�̃J�E���g�ϐ� */
extern uint8_t  PDTM;/* POWER DOWN TIMER 1mS */
extern uint16_t PDTM2;/* POWER DOWN TIMER(3S) */

/* ���͊e�r�b�g�i�[�ϐ� */
extern uint8_t MXTM;/* �}�g���b�N�X�^�C�}�[ */
extern uint8_t MXSW;/* �L�[�}�g���b�N�X�f�[�^�� */
extern uint8_t MXSWO0;/* �L�[�}�g���b�N�X�f�[�^�m�� */
extern uint8_t MXIN0;/* ����0�� */
extern uint8_t MXIN1;/* ����1�� */
extern uint8_t MXINO0;/* ����0�m�� */
extern uint8_t MXINO1;/* ����1�m�� */

/* �o�͗p�ϐ� */
extern uint8_t MXOTN0;/* �o��0 */
extern uint8_t MXOTN1;/* �o��1 */
extern uint8_t MXOTN2;/* �o��2 */
extern uint8_t MXOTN3;/* �o��3 */
extern uint8_t MXOTN4;/* �o��4 */
extern uint8_t MXOTN5;/* �o��5 */
extern uint8_t MXOTN6;/* �o��6 */
extern uint8_t MX6BLK;/* �o��6�_�Ńt���O */

extern uint8_t EDDT[6];/* �ҏW���f�[�^ */

/* �`�o�b�N�A�b�v�f�[�^(32�o�C�g)�` */
extern uint8_t  PASS1;/* �ۑ��G���A�̃p�X���[�h���ʁA������0x01 */
extern uint8_t  PASS2;/* �ۑ��G���A�̃p�X���[�h��ʁA������0xAA */
extern uint8_t  SYSFLG;/* �^�]�t���O */
extern uint8_t  ABNF;/* �ُ�t���O */
/*
0�`3�r�b�g�c�ُ���e1�`F�A0�͖���
4�c�`���[�ُ�
5�c�`���[�\�͒ቺ
6�c�p���ُ�x��v���t���O
7�c�ُ�x��v���t���O
*/
extern uint8_t  PAT1;/* �p�^�[��1�ݒ艷�x */
extern uint8_t  PAT2;/* �p�^�[��2�ݒ艷�x */
extern uint8_t  PAT3;/* �p�^�[��3�ݒ艷�x */
extern uint8_t  PAT4;/* �p�^�[��4�ݒ艷�x */
extern uint8_t  RPAT;/* �^�]�p�^�[�� */
/*
0�c�p�^�[��1�@����L��F1�A�����F0
1�c�p�^�[��2�@����L��F1�A�����F0
2�c�p�^�[��3�@����L��F1�A�����F0
3�c�p�^�[��4�@����L��F1�A�����F0
4�c
5�c���Ӱ�ރt���O�@1�F��򃂁[�h�A0�F�ʏ�
6�c�퉷�����̔r����Ԃ�\���t���O
7�c�ቷ�����̔r����Ԃ�\���t���O
*/
extern uint8_t  SWTMP0;/* �퉷�����̐ݒ艷�x */
extern uint8_t  ABNCUT;/* �ُ헚���ƈُ팏�� */
/*
0�`3�F�ُ헚�������i�ő�5���j
4�`7�F�ŐV�ُ�f�[�^
*/
extern uint8_t  ABNH0;/* �ُ헚��1 */
/*
0�`3�F1�O�ُ�f�[�^
4�`7�F2�O�ُ�f�[�^
*/
extern uint8_t  ABNH1;/* �ُ헚��2 */
/*
0�`3�F3�O�ُ�f�[�^
4�`7�F4�O�ُ�f�[�^
*/
extern uint8_t  STIME1;/* ��p�J�n�������� */
extern uint8_t  STIME2;/* ��p�J�n������� */
extern uint8_t  ETIME1;/* ��p�I���������� */
extern uint8_t  ETIME2;/* ��p�I��������� */
extern uint8_t  RTIMM;/* �ώZ�ғ����ԁ@�� */
extern uint8_t  RTIMH1;/* �ώZ�ғ����ԁ@���ԉ��� */
extern uint8_t  RTIMH2;/* �ώZ�ғ����ԁ@���ԏ�� */
extern uint8_t  SETTM;/* �`���[�\��^�C���@�� */
extern uint8_t  SETTH;/* �`���[�\��^�C���@�� */
extern uint8_t  CSEQTS;/* ��p�H���V�[�P���X�^�C���i�b�j */
extern uint8_t  CSEQTM;/* ��p�H���V�[�P���X�^�C���i���j */
extern uint8_t  CCSEQ;/* ��p�H���V�[�P���X�i���o�[ */
extern uint8_t  SCTM1;/* �p�^�[��1���⎞�� */
extern uint8_t  SCTM2;/* �p�^�[��2���⎞�� */
extern uint8_t  SCTM3;/* �p�^�[��3���⎞�� */
extern uint8_t  SCTM4;/* �p�^�[��4���⎞�� */
extern uint8_t  NPAT;/* ���ݑI�𒆂̃p�^�[�� */
extern uint8_t  RWAITT;/* �^�]�ҋ@���ԁi���j�@MAX��4���� */
extern uint8_t  CSUM;/* NV-RAM�̃T���`�F�b�N�f�[�^ */
/* �`�o�b�N�A�b�v�f�[�^(32�o�C�g)�����܂Ł` */

/* AD���ް��p�̕ϐ��� */
extern uint8_t  AD_RESLT;/* AD�ϊ����s�� */
extern uint8_t  AD_MX;/* �ǂ�AD�ϊ������s���邩�̎w�� */
extern uint8_t  AD_FLG01;/* AD�ϊ�����/�������𔻒f����t���O�ϐ� */
extern uint8_t  AD_FLG02;/* 2��̕��ω�������������\���t���O�ϐ� */
extern uint8_t  AD_NOW_CNT[3];/* �u��AD�ϊ��J�E���g */
extern uint8_t  AD_ONE_CNT[3];/* �m��pAD�ϊ��J�E���g */

extern uint16_t AD_NOW_MAX[3];/* AD�擾���̍ő�l */
extern uint16_t AD_NOW_MIN[3];/* AD�̍ŏ��l */
extern uint32_t AD_NOW_SUM[3];/* AD�̍��Z�l */

extern uint16_t AD_ONE_AVR[3];/* AD�擾���̕��ϒl */
extern uint16_t AD_ONE_MAX[3];/* AD�擾���̍ő�l */
extern uint16_t AD_ONE_MIN[3];/* AD�̍ŏ��l */
extern uint32_t AD_ONE_SUM[3];/* AD�̍��Z�l */
extern uint16_t AD0_FVB[5];

extern short TMPDT0;/* �i�� */
extern short TMPDT1;/* �ቷ������*/
extern short TMPDT2;/* �퉷������ */
extern short TOLD0;/* ���i�� */
extern short TOLD1;/* ���ቷ������ */
extern short TOLD2;/* ���퉷������ */
extern uint8_t AD0TM;/* �i���Z���T�ُ팟�o�^�C�} */
extern uint8_t AD1TM;/* �ቷ�������ُ팟�o�^�C�} */
extern uint8_t AD2TM;/* �퉷�������ُ팟�o�^�C�} */
extern uint16_t DACOUT;/* �i����4-20mA�o�͗p */
extern uint8_t AD_CTM0;/* AD�ϊ��Œ��^�C�} */

/* ��p����ŗL�ϐ��� */
extern uint8_t  CCTM0;/* ��p�H����ϰ0 */
extern uint8_t  CCTM1;/* ��p�H����ϰ1 */
extern uint16_t CCTM2;/* ��p�H����ϰ2 */
extern uint8_t  CCTM3;/* ��p�H����ϰ3 */
extern uint8_t  CCTM4;/* �u�╜�A����p��ϰ */
extern uint8_t  CCTM5;/* �^��������̱װїp��� */

/* �\���A�v���Ŏg�p����ϐ��� */
extern uint8_t DSPNO;/* �\���i���o�[ */
extern uint8_t DSPNOO;/* ���\���i���o�[ */
extern uint8_t DSPLN1;/* �\����1 */
extern uint8_t DSPLN2;/* �\����2 */
extern const uint8_t *MHEAD1;/* ���b�Z�[�W1�擪�A�h���X */
extern const uint8_t *MHEAD2;/* ���b�Z�[�W2�擪�A�h���X */
extern uint8_t DSPSEQ;/* ��ʕ\���V�[�P���X */
extern uint8_t DSPSSQ;/* ��ʕ\���T�u�V�[�P���X */
extern uint8_t DSPLB1[16];/* DISPLAY DATA BUFFER for LINE 1 */
extern uint8_t DSPLB2[16];/* DISPLAY DATA BUFFER for LINE 2 */
extern uint8_t DSPTM1;/* ��ʃ^�C�}1 */
extern uint8_t DSPTM2;/* ��ʃ^�C�}2 */
extern uint8_t DCLNF;/* ��ʃR�����u�F�v�t���O */
extern uint8_t DABHP;/* ABNORMAL HISTORY POINT */
extern uint8_t BZSEQ;/* �u�U�[�V�[�P���X */
extern uint8_t BZTM1;/* �u�U�[�^�C�}�[1 */
extern uint8_t BZTM2;/* �u�U�[�^�C�}�[2 */
extern uint8_t ABNFO;/* ���ُ�t���O */
extern uint8_t YOTINO;/* �\�m�x��\�����ް */
extern uint8_t TNO;/* ��̥ýĥ���ް */
extern uint8_t SUBTNO;/* �T�u�e�X�g�i���o�[ */
extern uint8_t MCLAST;/* ��ظر��t��� */
extern uint8_t STSAST;/* ���ýĎ�t���	*/
extern uint8_t STSTM1;/* ���ýėp��� */
extern uint8_t BITPOS;/* �r�b�g�`�F�b�N�t���O */
extern uint8_t CRTCDT;/* RTC CHECK�pOLD DATA */
extern uint8_t MDAST;/* �蓮�r�������t��� */
extern uint8_t DLWSQ;/* DCONT�̎���X�^�[�g�ʒu�����߂�ꏊ */
extern uint8_t DPOUT;/* LCD/VFD�̕\���� */

/* ���쐧��Ŏg�p����ϐ��� */
extern uint8_t  KFLG;/* ���ض��ް��쐬�p */
extern uint8_t  ONTRG;/* ���ض��ް� */
extern uint16_t INDPTM;/* �ʏ�\�����A�p��ϰ */
extern uint8_t  CLNRTM;/* ��򒆒f�\�����A��ϰ */
extern uint8_t  RPTFLG;/* �ݒ蒆�ް� */

/* �O��°قŎg�p����ʐM�ޯ̧ */
extern uint8_t  TXFLG0;/* �O��°ْʐM�̑��M�֘A�׸� */
extern uint8_t  TXSET0;/* �O��°ْʐM���M��Đ� */
extern uint8_t  TXLEN0;/* �O��°ْʐM���M�� */
extern uint8_t  TXPTR0;/* �O��°ْʐM���M�ޯ̧�̌��݈ʒu */
extern uint8_t  USTS0;/* �O��°ْʐM��M�׸ޕϐ� */
extern uint8_t  RXLEN0;/* �O��°ْʐM��M�� */
extern uint8_t  RXPTR0;/* �O��°ْʐM��M�ޯ̧�̌��݈ʒu */
extern uint8_t  TBUF0[256];/* ���M�ޯ̧256�޲� */
extern uint8_t  RBUF0[256];/* ��M�ޯ̧256�޲� */
extern uint8_t  SCRSTT;/* �ʐM�菇1�̒ʐM���[1byte] */
extern uint8_t  FLMSTS;/* ����ʐM��M����byte��[1byte] */
extern uint16_t KYNO;/* �ǔԍ�[2byte] */
extern uint16_t PCNO;/* PC�ԍ�[2byte] */
extern uint16_t MEMRY01[16];/* �ʐM�pW�G���A�������[ */

/* ���ʐ���p�ϐ��� */
extern uint8_t  WLNEW;/* �ŐV�������ٓ��� */
extern uint8_t  WLOLD;/* �P�O�̐������ٓ��� */
extern uint8_t  NHILCT;/* �ቷ��HI��ݾ���� */
extern uint8_t  NLOLCT;/* �ቷ��LO��ݾ����	*/
extern uint8_t  CHILCT;/* �퉷��HI��ݾ����	*/
extern uint8_t  CLOLCT;/* �퉷��LO��ݾ����	*/
extern uint8_t  WLCTM1;/* ���ʐ���p�������1 */
extern uint8_t  WLCTM2;/* ���ʐ���p�������2 */
extern uint8_t  WLCTM3;/* ���ʐ���p�������3 */
extern uint16_t WLCTM4;/* �r���H���p��ϰ */
extern uint16_t WLCTM5;/* �d���̎��̑��r���p��� */
extern uint8_t  WLCWT;/* ���ʐ���x����� */
extern uint16_t WABTM1;/* �퉷�������ُ팟�m��� */
extern uint16_t WABTM2;/* �ቷ�������ُ팟�m��� */
extern uint16_t TIRTM1;/* �װ�\�͒ቺ�m�F��� */
extern uint8_t  MMODE;/* MANUAL MODE FLAG */
extern uint8_t  MMDTM;/* MANUAL MODE TIMER	*/
extern uint8_t  WCTM1;/* ��򒆂̓d���ٓ������	*/
extern uint8_t  WCTM2;/* �G�A�����ٗp��� */

typedef struct
{
    uint8_t  TSK_COND[TSKNUM];/*�A�v���P�[�V�����̃R���f�B�V�����t���O*/
    uint16_t NEXT_LOAD_Time[TSKNUM];/*�A�v���P�[�V��������Ăяo���v������*/
} OS_TSKCon1;

OS_TSKCon1 *OSTSKCon1S(void);

extern uint8_t *HENNP[32];/* NV-RAM�ۑ��G���A�̐擪�A�h���X */
#endif