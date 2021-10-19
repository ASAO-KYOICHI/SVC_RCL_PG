/**
 *�薼�F���̓A�v��
 *�쐬�ҁF����@����
 *���͌��m�������s���A�v���P�[�V����
**/
#include "tmpm380_INPUT.h"


/* ON/OFF_SW���͗ނ̌��m���0 */
const uint8_t INP_K0[8] = {
    4 ,4 ,4 ,4 ,4 ,4 ,4, 4
};
/* ON/OFF_PORT���͗ނ̌��m���1 */
const uint8_t INP_K1[8] = {
    20,20,20,20,20,20,20,20
};
/* ON/OFF_PORT���͗ނ̌��m���2 */
const uint8_t INP_K2[8] = {
    20,20,20,20,20,20,20,20
};

const uint8_t INPUT_ON00 = 0xFF;/* ON�ƂȂ�������SW���͒l�̑� */
/*�@�S��High�A�N�e�B�u
0�c��SW
1�c��SW
2�c��SW
3�c�ݒ�SW
4�c�d��SW
5�c�I��SW
6�c�^�]SW
7�c�`���[�\��SW
*/
const uint8_t INPUT_ON01 = 0xF0;/* ON�ƂȂ�������PORT���͒l�̑� */
/*�@���ʂ�Low�A�N�e�B�u�A����ȊO��High�A�N�e�B�u
0����퉷��������L
1����퉷��������H
2����ቷ��������L
3����ቷ��������H
4����퉷�^��ٕ��~�b�g
5����\��
6������̓Z���T�[
7����`���[�ُ�M��
*/
const uint8_t INPUT_ON02 = 0xFF;/* ON�ƂȂ�������PORT���͒l�̑� */
/*�@�S��High�A�N�e�B�u
0����T�[�}���g���b�v
1�����
2�����
3�����d�M��
4���DSW1
5���DSW2
6���DSW3
7���DSW4
*/

/* ON/OFF_SW���͗ނ̌��m����0 */
uint8_t PORT_CNT00[8] = {
    0,0,0,0,0,0,0,0
};
/* ON/OFF_PORT���͗ނ̌��m����1 */
uint8_t PORT_CNT01[8] = {
    0,0,0,0,0,0,0,0
};
/* ON/OFF_PORT���͗ނ̌��m����2 */
uint8_t PORT_CNT02[8] = {
    0,0,0,0,0,0,0,0
};

void INPUT_CONT(void)
{
    OS_TSKCon1 *OS_LoadTime;/* OS�ݒ�l��ǂނ��߂̃|�C���^�\���̂ɖ��O������ */
    OS_LoadTime = OSTSKCon1S();/* �֐��o�R�Ń|�C���^��OSparam�ɃA�N�Z�X */
    
    uint8_t REG_00 = 0;/* �ꎞ�ϐ� */
    
    /* �ȉ��A���̓A�v�������s���鏈�� */
    /* �`���^�����O�L�����Z������ */
    for (uint8_t i = 0; i < 8; i++) {
        /* ٰ�߂Ŋe�r�b�g���`�F�b�N���� */
        REG_00 = MXSW;/* �X�C�b�`���̓f�[�^���Z�b�g */
        if ((REG_00 & (0x01 << i)) == (INPUT_ON00 & (0x01 << i))) {
            /* ���͂���ł���ꍇ */
            PORT_CNT00[i]++;
            if (PORT_CNT00[i] >= INP_K0[i]) {
                /* ��������萔�ȏ�UP�����ꍇ */
                PORT_CNT00[i] = INP_K0[i];/* MAX FIX */
            }
        } else {
            /* ���͖����ł���ꍇ */
            if (PORT_CNT00[i] > 0) {
                /* ��������萔�ȏ�UP�����ꍇ */
                PORT_CNT00[i]--;
            }
        }
        
        REG_00 = MXIN0;
        if ((REG_00 & (0x01 << i)) == (INPUT_ON01 & (0x01 << i))) {
            /* ���͂���ł���ꍇ */
            PORT_CNT01[i]++;
            if (PORT_CNT01[i] >= INP_K1[i]) {
                /* ��������萔�ȏ�UP�����ꍇ */
                PORT_CNT01[i] = INP_K1[i];/* MAX FIX */
            }
        } else {
            /* ���͖����ł���ꍇ */
            if (PORT_CNT01[i] > 0) {
                /* ��������萔�ȏ�UP�����ꍇ */
                PORT_CNT01[i]--;
            }
        }
        
        REG_00 = MXIN1;
        if ((REG_00 & (0x01 << i)) == (INPUT_ON02 & (0x01 << i))) {
            /* ���͂���ł���ꍇ */
            PORT_CNT02[i]++;
            if (PORT_CNT02[i] >= INP_K2[i]) {
                /* ��������萔�ȏ�UP�����ꍇ */
                PORT_CNT02[i] = INP_K2[i];/* MAX FIX */
            }
        } else {
            /* ���͖����ł���ꍇ */
            if (PORT_CNT02[i] > 0) {
                /* ��������萔�ȏ�UP�����ꍇ */
                PORT_CNT02[i]--;
            }
        }
        
        if (PORT_CNT00[i] >= INP_K0[i]) {
            /* ��ς�����ݸ޷�ݾَ��Ԍo�߂����ꍇ */
            MXSWO0 |= (0x01 << i);/* �ޯľ�� */
        } else if (PORT_CNT00[i] == 0) {
            /* ��ς�����ݸ޷�ݾَ��Ԍo�߂��Ă��Ȃ��ꍇ */
            MXSWO0 &= ~(0x01 << i);/* �ޯĸر */
        }
        
        if (PORT_CNT01[i] >= INP_K1[i]) {
            /* ��ς�����ݸ޷�ݾَ��Ԍo�߂����ꍇ */
            MXINO0 |= (0x01 << i);/* �ޯľ�� */
        } else if (PORT_CNT01[i] == 0) {
            /* ��ς�����ݸ޷�ݾَ��Ԍo�߂��Ă��Ȃ��ꍇ */
            MXINO0 &= ~(0x01 << i);/* �ޯĸر */
        }
        
        if (PORT_CNT02[i] >= INP_K2[i]) {
            /* ��ς�����ݸ޷�ݾَ��Ԍo�߂����ꍇ */
            MXINO1 |= (0x01 << i);/* �ޯľ�� */
        } else if (PORT_CNT02[i] == 0) {
            /* ��ς�����ݸ޷�ݾَ��Ԍo�߂��Ă��Ȃ��ꍇ */
            MXINO1 &= ~(0x01 << i);/* �ޯĸر */
        }
        
    }/* ���[�v�����܂� */
    
    if ((((ABNF & 0x0F) == 0) && ((SYSFLG & 0x40) > 0))
    || (((ABNF & 0x0F) == 8)  && ((SYSFLG & 0x42) == 0x42))
    || (((ABNF & 0x0F) == 9)  && ((SYSFLG & 0x42) == 0x42))
    || (((ABNF & 0x0F) == 14) && ((SYSFLG & 0x42) == 0x42))) {
        /* �d��ON�ŁA�ُ�ɂȂ��ĂȂ��ꍇ�A
        ���͓d��ON�E�^�]���ŏ퉷���ʓd�ɖ_�ُ�ł���A
        ���͓d��ON�E�^�]���ŏ퉷���x�Z���T�[�ُ�ł���A
        ���͓d��ON�E�^�]���Ő^��x�ُ�ł���ꍇ */
        if ((MXINO1 & 0x01) > 0) {
            /* �T�[�}���ُ���͂�����ꍇ */
            ABNF &= 0x30;/* MASK */
            ABNF |= 0x81;/* �u�U�[�t���O�A�T�[�}���ُ���Z�b�g */
            
        } else if ((MXINO0 & 0x80) > 0) {
            /* �`���[�ꊇ�ُ���͂̏ꍇ */
            if ((SYKND0 & 0x10) > 0) {
                /* ���C���̐ݒ�̏ꍇ */
                ABNF = 0x82;/* �u�U�[�t���O�N�[�����O�^���[�ُ�ɂ��� */
                
            } else {
                /* �`���[���̐ݒ�̏ꍇ */
                if ((ABNF & 0x10) == 0) {
                    /* �`���[�ُ킪�Z�b�g����Ă��Ȃ��ꍇ */
                    ABNF |= 0x50;/* �`���[�ꊇ�ُ�Z�b�g�i�^�]�p�����鈵���j */
                    
                }
                
            }
            
        }
        
    }
    
    OS_LoadTime->NEXT_LOAD_Time[1] = 5;/* ����Ăяo������ */
    OS_LoadTime->TSK_COND[1] = 0xA0;/* �x���t���O���z */
    
    return;
}