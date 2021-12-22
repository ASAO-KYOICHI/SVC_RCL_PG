/***
 * �薼�F�ʐM����A�v��
 * �쐬�ҁF����@����
 * �ʐM������s���A�v���P�[�V����
***/
#include "tmpm380_COMCNT.h"

const uint8_t ABNHEAD[] = {
    /* �ُ펞�ɕԐM����f�[�^ */
    0x15, '0', '0', 'F', 'F', '0', '6'
};

void COMCNT(void)
{
    OS_TSKCon1 *OS_LoadTime;/* OS�ݒ�l��ǂނ��߂̃|�C���^�\���̂ɖ��O������ */
    OS_LoadTime = OSTSKCon1S();/* �֐��o�R�Ń|�C���^��OSparam�ɃA�N�Z�X */
    
    uint8_t  REG_00[4] = {
        /* �ꎞ�ϐ� */
        0, 0, 0, 0
    };
    uint16_t REG_01 = 0;/* �ꎞ�ϐ� */
    uint16_t REG_02 = 0;/* �ꎞ�ϐ� */
    uint16_t REG_03 = 0;/* �ꎞ�ϐ� */
    
    if (((STSFL1 & 0x03) > 0) || ((STSFL0 & 0x02) == 0)) {
        /* �Z���t�e�X�g���A�������[�N���A���ł���A���͏��������ł���ꍇ */
        OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* ����Ăяo������ */
        OS_LoadTime->TSK_COND[7] = 0xA0;/* �x���t���O���z */
        
        return;
        
    }
    
    if ((USTS0 & 0x04) > 0) {
        /* ��������G���[�ł������ꍇ */
        RXPTR0 += RXLEN0;/* ��M�|�C���^���ړ� */
        RXLEN0 = 0;/* ��M�����N���A */
        USTS0 &= 0xFB;/* �t���O�N���A */
        
        if ((TXFLG0 & 0x11) == 0) {
            /* ���M���ł͂Ȃ��ꍇ */
            for (uint8_t i = 0;i < 7;i++) {
                /* 7��J��Ԃ����� */
                TBUF0[(TXPTR0 + i) & 0x0FF] = ABNHEAD[i];/* �f�[�^�����܂� */
                
            }
            
            TXLEN0 = 7;
            TXSET0 = 7;/* ���M���A�f�[�^���Z�b�g */
            TXFLG0 &= 0xDF;/* �G���v�e�B�t���O���N���A */
            TXFLG0 |= 0x11;/* ���M�J�n�Z�b�g */
            
            /* �f�[�^���i�[�����M���� */
            TSB_SC0->BUF = TBUF0[TXPTR0];/* �o�b�t�@�ɃZ�b�g */
            
            TXPTR0++;/* �߲����ݸ���� */
            TXLEN0--;/* �޸���� */
            TXSET0--;/* �޸���� */
            
            OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* ����Ăяo������ */
            OS_LoadTime->TSK_COND[7] = 0xA0;/* �x���t���O���z */
            
            return;
            
        }
        
    }/* �G���[�������ꍇ�͂����ɔ����� */
    
    
    while (RXLEN0 > 0) {
        /* ��M����0�ȏ゠��ꍇ�̓��[�v���܂� */
        if ((USTS0 & 0x08) > 0) {
            /* �o�b�t�@���ް�݂ł������ꍇ */
            RXLEN0 = 0;/* ��M�f�[�^���N���A */
            RXPTR0 = 0;/* ��M�߲���N���A */
            SCRSTT = 0;/* �H���ر */
            USTS0 &= 0xF7;/* �o�b�t�@���ް�݃N���A */
            
        }
        
        if ((SCRSTT == 0) && (RXLEN0 == 0)) {
            /* �t���[���������ł͂Ȃ��A��M�ް����Ȃ��ꍇ */
            OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* ����Ăяo������ */
            OS_LoadTime->TSK_COND[7]       = 0xA0;/* �x���t���O���z */
            
            return;/* �߂�܂� */
        }
        
        if (SCRSTT == 0) {
            /* �t���[���������ł͂Ȃ��ꍇ */
            if (RXLEN0 < 10) {
                /* ��M����10�o�C�g�����ł���ꍇ */
                OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[7]       = 0xA0;/* �x���t���O���z */
                
                return;/* �߂�܂� */
                
            }
            
            if (RBUF0[RXPTR0] != 0x05) {
                /* ENQ�ł͂Ȃ��ꍇ */
                SCRSTT = 0;/* �H���ر */
                RXPTR0++;/* �ݸ���� */
                RXLEN0--;/* ��M�����޸���� */
                continue;/* ���[�v�̓��� */
                
            }
            
            /* ENQ�𔭌������ꍇ */
            KYNO = (((uint16_t)RBUF0[(RXPTR0 + 1) & 0x0FF] << 8) | ((uint16_t)RBUF0[(RXPTR0 + 2) & 0x0FF]));/* �ǔԍ���ۑ� */
            PCNO = (((uint16_t)RBUF0[(RXPTR0 + 3) & 0x0FF] << 8) | ((uint16_t)RBUF0[(RXPTR0 + 4) & 0x0FF]));/* PC�ԍ���ۑ� */
            
            if ((RBUF0[(RXPTR0 + 5) & 0x0FF] == 0x50) && (RBUF0[(RXPTR0 + 6) & 0x0FF] == 0x43)) {
                /* PC�R�}���h�ł���ꍇ */
                if (RBUF0[(RXPTR0 + 7) & 0x0FF] != 0x30) {
                    /* �`���E�F�C�g��0ms�ȊO�̏ꍇ */
                    SCRSTT = 0;/* �H���ر */
                    RXPTR0++;/* �ݸ���� */
                    RXLEN0--;/* ��M�����޸���� */
                    continue;/* ���[�v�̓��� */
                    
                }
                
                /* SUM�l�����[�h */
                REG_01 = (((uint16_t)RBUF0[(RXPTR0 + 8) & 0x0FF] << 8) | ((uint16_t)RBUF0[(RXPTR0 + 9) & 0x0FF]));/* SUM�l��ۑ� */
                
                /* ��M�����ް���SUM���Z�o */
                REG_00[0]  = (uint8_t)(KYNO & 0x00FF);
                REG_00[0] += (uint8_t)(KYNO >> 8);
                REG_00[0] += (uint8_t)(PCNO & 0x00FF);
                REG_00[0] += (uint8_t)(PCNO >> 8);
                REG_00[0] += 0x50;
                REG_00[0] += 0x43;
                REG_00[0] += 0x30;
                
                if (REG_01 != ASCICV(REG_00[0])) {
                    /* SUM�l����v���Ȃ��ꍇ */
                    SCRSTT = 0;/* �H���ر */
                    RXPTR0++;/* �ݸ���� */
                    RXLEN0--;/* ��M�����޸���� */
                    continue;/* ���[�v�̓��� */
                    
                }
                
                TBUF0[TXPTR0] = 0x02;/* STX���Z�b�g */
                TBUF0[(TXPTR0 + 1) & 0x0FF] = (uint8_t)(KYNO >> 8);/* ��ʂ��Z�b�g */
                TBUF0[(TXPTR0 + 2) & 0x0FF] = (uint8_t)(KYNO & 0x00FF);/* ���ʂ��Z�b�g */
                TBUF0[(TXPTR0 + 3) & 0x0FF] = (uint8_t)(PCNO >> 8);/* ��ʂ��Z�b�g */
                TBUF0[(TXPTR0 + 4) & 0x0FF] = (uint8_t)(PCNO & 0x00FF);/* ���ʂ��Z�b�g */
                TBUF0[(TXPTR0 + 5) & 0x0FF] = 0x38;/* PC�^��"0x3832"���Z�b�g */
                TBUF0[(TXPTR0 + 6) & 0x0FF] = 0x32;
                TBUF0[(TXPTR0 + 7) & 0x0FF] = 0x03;/* ETX���Z�b�g */
                
                REG_00[0] = 0;/* ��x�N���A���� */
                for (uint8_t i = 1;i < 8;i++) {
                    /* 7��J��Ԃ����s */
                    REG_00[0] += TBUF0[(TXPTR0 + i) & 0x0FF];/* �o�b�t�@�̒l�����Z���� */
                    
                }
                
                REG_01 = ASCICV(REG_00[0]);/* SUM�𱽷������� */
                TBUF0[(TXPTR0 + 8) & 0x0FF] = (uint8_t)(REG_01 & 0x00FF);/* SUM�Z�b�g1 */
                TBUF0[(TXPTR0 + 9) & 0x0FF] = (uint8_t)(REG_01 >> 8);/* SUM�Z�b�g2 */
                TBUF0[(TXPTR0 + 10) & 0x0FF] = 0x0D;/* CR�Z�b�g*/
                TBUF0[(TXPTR0 + 11) & 0x0FF] = 0x0A;/* LF�Z�b�g */
                
                TXLEN0 += 12;/* ���M���Z�b�g */
                TXSET0 += 12;/* ���M���Z�b�g */
                TXFLG0 |= 0x11;/* ���M�v���A�J�n�Z�b�g */
                TXFLG0 &= 0xDF;/* ���M��ݾق��N���A���� */
                
                TSB_SC0->BUF = TBUF0[TXPTR0];/* ���M���Z�b�g */
                TXPTR0++;/* �ݸ���� */
                TXPTR0 &= 0xFF;/* MAX FIX */
                TXLEN0--;/* �޸���� */
                TXSET0--;/* �޸���� */
                
                RXLEN0 = 0;/* ��M���N���A */
                RXPTR0 = 0;/* ��M�߲���N���A */
                
                SCRSTT = 0;/* �H���ر */
                
                OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[7]       = 0xA0;/* �x���t���O���z */
                
                return;/* �߂�܂� */
                
            } else {
                /* PC�R�}���h�ł͂Ȃ��ꍇ */
                /* �t���[���J�n�̔��� */
                if (RXLEN0 < 17) {
                    /* ��M����17�o�C�g�����ł���ꍇ */
                    OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* ����Ăяo������ */
                    OS_LoadTime->TSK_COND[7]       = 0xA0;/* �x���t���O���z */
                    
                    return;/* �߂�܂� */
                    
                }
                
                if ((RBUF0[(RXPTR0 + 5) & 0x0FF] == 'W') && (RBUF0[(RXPTR0 + 6) & 0x0FF] == 'R')) {
                    /* �R�}���h����"WR"�ł���ꍇ */
                    if ( ((RBUF0[(RXPTR0 + 7) & 0x0FF] >= 0x30) && (RBUF0[(RXPTR0 + 7) & 0x0FF] <= 0x39))
                    || ((RBUF0[(RXPTR0 + 7) & 0x0FF] >= 0x41) && (RBUF0[(RXPTR0 + 7) & 0x0FF] <= 0x46)) ) {
                        /* �`���E�F�C�g��ASCII��0�`F�͈̔͂ł���ꍇ */
                        /* ���������ʉ߂��� */
                    } else {
                        /* �`���E�F�C�g���ᔽ����ꍇ */
                        SCRSTT = 0;/* �H���ر */
                        RXPTR0++;/* �ݸ���� */
                        RXLEN0--;/* ��M�����޸���� */
                        continue;/* ���[�v�̓��� */
                        
                    }
                    
                    if (RBUF0[(RXPTR0 + 8) & 0x0FF] != 'W') {
                        /* W�̈�ȊO�ł���ꍇ */
                        SCRSTT = 0;/* �H���ر */
                        RXPTR0++;/* �ݸ���� */
                        RXLEN0--;/* ��M�����޸���� */
                        continue;/* ���[�v�̓��� */
                        
                    }
                    
                    for (REG_00[1] = 9;REG_00[1] < 15;REG_00[1]++) {
                        /* 4��J��Ԃ� */
                        if ( ((RBUF0[(RXPTR0 + REG_00[1]) & 0x0FF] >= 0x30) && (RBUF0[(RXPTR0 + REG_00[1]) & 0x0FF] <= 0x39))
                            /* ASCII��0�`9�͈̔͂ł���ꍇ */
                        || ((RBUF0[(RXPTR0 + REG_00[1]) & 0x0FF] >= 0x41) && (RBUF0[(RXPTR0 + REG_00[1]) & 0x0FF] <= 0x46)) ) {
                            /* ASCII��A�`F�͈̔͂ł���ꍇ */
                            /* �������Ȃ� */
                            
                        } else {
                            /* �͈͊O�̏ꍇ */
                            break;
                            
                        }
                        
                    }
                    
                    if (REG_00[1] <= 14) {
                        /* �ǂ����Ŕ͈͊O�ɂ���ău���[�N�����ꍇ */
                        SCRSTT = 0;/* �H���ر */
                        RXPTR0++;/* �ݸ���� */
                        RXLEN0--;/* ��M�����޸���� */
                        continue;/* ���[�v�̓��� */
                        
                    } else {
                        /* �Ō�܂Ŋm�F������̏ꍇ(REG_00[1]��15) */
                        REG_00[1]--;/* 14�ɂ��� */
                        
                    }
                    
                    FLMSTS = 17;/* �R�}���h���޲Đ��Z�b�g */
                    SCRSTT = 0x01;/* WR�R�}���h�Z�b�g */
                    
                    continue;/* ���[�v�̓��� */
                    
                } else {
                    /* �R�}���h�ُ�̏ꍇ */
                    SCRSTT = 0;/* �H���ر */
                    RXPTR0++;/* �ݸ���� */
                    RXLEN0--;/* ��M�����޸���� */
                    continue;/* ���[�v�̓��� */
                    
                }
                
            }
            
        } else {
            /* �t���[���������̏ꍇ */
            /* �R�}���h�̗p�̔��� */
            if (RXLEN0 < FLMSTS) {
                /* �t���[���I���܂Ńf�[�^����荞�߂Ă��Ȃ��ꍇ */
                OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[7]       = 0xA0;/* �x���t���O���z */
                
                return;
                
            }
            
            REG_00[0] = 0;/* �N���A */
            for (uint8_t i = 0;i < FLMSTS;i++) {
                /* �t���[���I���l - 2�܂ŌJ��Ԃ� */
                if ((i + 3) < FLMSTS) {
                    /* SUM�Z�o�G���A�̏ꍇ */
                    REG_00[0] += RBUF0[(RXPTR0 + 1 + i) & 0x0FF];/* SUM�l���Z�o���܂� */
                    
                } else if ((i + 3) == FLMSTS) {
                    /* SUM�Z�o�G���A��1��ځi��M�ް���SUM�l�j */
                    REG_00[1] = RBUF0[(RXPTR0 + 1 + i) & 0x0FF];/* SUM�l���Z�b�g���܂� */
                    
                } else if ((i + 2) == FLMSTS) {
                    /* SUM�Z�o�G���A��1��ځi��M�ް���SUM�l�j */
                    REG_00[2] = RBUF0[(RXPTR0 + 1 + i) & 0x0FF];/* SUM�l���Z�b�g���܂� */
                    
                }
                
            }
            
            REG_01 = ASCICV(REG_00[0]);/* �A�X�L�[�����܂� */
            if ((REG_00[1] != (uint8_t)(REG_01 & 0x00FF)) || (REG_00[2] != (uint8_t)(REG_01 >> 8))) {
                /* SUM�l����v���Ȃ��ꍇ */
                SCRSTT &= 0x00;/* �H���ر */
                RXLEN0 -= 17;/* ���Z���� */
                RXPTR0 += 17;
                
                continue;
                
            }
            
            break;/* SUM������Ȃ烋�[�v�𔲂��� */
            
        }
        
    }/* while�ɂ���M�f�[�^�m�F�͂����܂� */
    
    if ((RXLEN0 == 0) || (SCRSTT == 0)) {
        /* ��M�����O�A���͍H�����N���A����Ă���ꍇ */
        OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* ����Ăяo������ */
        OS_LoadTime->TSK_COND[7]       = 0xA0;/* �x���t���O���z */
        
        return;
        
    }
    
    /* �e�ʐM���̏��� */
    TBUF0[(TXPTR0 + 1) & 0x0FF] = (uint8_t)(KYNO >> 8);/* ���8�r�b�g���Z�b�g */
    TBUF0[(TXPTR0 + 2) & 0x0FF] = (uint8_t)(KYNO & 0x00FF);/* ����8�r�b�g���Z�b�g */
    TBUF0[(TXPTR0 + 3) & 0x0FF] = (uint8_t)(PCNO >> 8);/* ���8�r�b�g���Z�b�g */
    TBUF0[(TXPTR0 + 4) & 0x0FF] = (uint8_t)(PCNO & 0x00FF);/* ����8�r�b�g���Z�b�g */
    
    /* WR�R�}���h */
    REG_00[0] = RBUF0[(RXPTR0 + 9) & 0x0FF];/* ��M�o�b�t�@_�A�h���X��1000�̈� */
    REG_00[1] = RBUF0[(RXPTR0 + 10) & 0x0FF];/* ��M�o�b�t�@_�A�h���X��100�̈� */
    REG_00[2] = BNRCV(REG_00[0], REG_00[1]);/* ASCII -> Binary�ϊ� */
    REG_01 = ((uint16_t)REG_00[2] * 100);/* �~100�������̂��Z�b�g */
    
    REG_00[0] = RBUF0[(RXPTR0 + 11) & 0x0FF];/* ��M�o�b�t�@_�A�h���X��10�̈� */
    REG_00[1] = RBUF0[(RXPTR0 + 12) & 0x0FF];/* ��M�o�b�t�@_�A�h���X��1�̈� */
    REG_00[2] = BNRCV(REG_00[0], REG_00[1]);/* ASCII -> Binary�ϊ� */
    REG_01 += (uint16_t)REG_00[2];/* 10��1�̈ʂ����Z�Z�b�g */
    
    if (REG_01 > 0x0F) {
        /* �A�h���X���m�F���A�ǂݍ��ݔ͈͊O�ł���ꍇ */
        RXLEN0 = RXPTR0 = 0;/* ��M���A��M�߲���N���A���� */
        SCRSTT = 0;/* �H���ر */
        
        TBUF0[TXPTR0] = 0x15;/* "NAK"���Z�b�g */
        TBUF0[(TXPTR0 + 5) & 0x0FF] = 0x30;
        TBUF0[(TXPTR0 + 6) & 0x0FF] = 0x36;
        TBUF0[(TXPTR0 + 7) & 0x0FF] = 0x0D;
        TBUF0[(TXPTR0 + 8) & 0x0FF] = 0x0A;
        TXLEN0 += 9;/* ���M���Z�b�g */
        TXSET0 += 9;/* ���M���Z�b�g */
        TXFLG0 |= 0x11;/* ���M�v�����J�n���Z�b�g */
        TXFLG0 &= 0xDF;/* ���M��ݾى��� */
        
        TSB_SC0->BUF = TBUF0[TXPTR0];/* �o�b�t�@�ɃZ�b�g */
        
        TXPTR0++;/* �߲����ݸ���� */
        TXPTR0 &= 0xFF;/* MAX FIX */
        TXLEN0--;/* �޸���� */
        TXSET0--;/* �޸���� */
        
        OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* ����Ăяo������ */
        OS_LoadTime->TSK_COND[7]       = 0xA0;/* �x���t���O���z */
        
        return;
        
    }
    
    REG_02 = REG_01;/* �擪�A�h���X���L�[�v���Ƃ� */
    REG_01 = 0;/* �N���A */
    REG_00[0] = RBUF0[(RXPTR0 + 13) & 0x0FF];/* ��M�o�b�t�@_�ǂݍ��݃f�[�^����10�̈� */
    REG_00[1] = RBUF0[(RXPTR0 + 14) & 0x0FF];/* ��M�o�b�t�@_�ǂݍ��݃f�[�^����1�̈� */
    REG_00[2] = HEXCV(REG_00[0], REG_00[1]);/* ASCII -> HEX16�i(�ق�Binary)�ϊ� */
    REG_01 = (uint16_t)REG_00[2];/* 10��1�̈ʂ��Z�b�g */
    
    if ((REG_01 == 0) || (REG_01 > 16) || ((REG_01 + REG_02) > 0x10)) {
        /* �ǂݍ����ް������m�F���A�ǂݍ��ݔ͈͊O�̐��l�ł���ꍇ */
        /* �擪�A�h���X�@+�@�ǂݍ����ް�����15�ȏ�ɂȂ�ꍇ */
        RXLEN0 = RXPTR0 = 0;/* ��M���A��M�߲���N���A���� */
        SCRSTT = 0;/* �H���ر */
        
        TBUF0[TXPTR0] = 0x15;/* "NAK"���Z�b�g */
        TBUF0[(TXPTR0 + 5) & 0x0FF] = 0x30;
        TBUF0[(TXPTR0 + 6) & 0x0FF] = 0x36;
        TBUF0[(TXPTR0 + 7) & 0x0FF] = 0x0D;
        TBUF0[(TXPTR0 + 8) & 0x0FF] = 0x0A;
        TXLEN0 += 9;/* ���M���Z�b�g */
        TXSET0 += 9;/* ���M���Z�b�g */
        TXFLG0 |= 0x11;/* ���M�v�����J�n���Z�b�g */
        TXFLG0 &= 0xDF;/* ���M��ݾى��� */
        
        TSB_SC0->BUF = TBUF0[TXPTR0];/* �o�b�t�@�ɃZ�b�g */
        
        TXPTR0++;/* �߲����ݸ���� */
        TXPTR0 &= 0xFF;/* MAX FIX */
        TXLEN0--;/* �޸���� */
        TXSET0--;/* �޸���� */
        
        OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* ����Ăяo������ */
        OS_LoadTime->TSK_COND[7]       = 0xA0;/* �x���t���O���z */
        
        return;
        
    }
    
    /* �������[�}�b�v�X�V */
    for (uint8_t i = 0;i < 14;i++) {
        /* 14��J��Ԃ� */
        if (i == 5) {
            /* W0005(�ُ헚��1��ʂƗ�p�J�n�����u���v���ʁj�̃������ł������ꍇ */
            /* ���ʂ̂�BCD�ɒ����K�v�����郁�����̏ꍇ */
            REG_00[0] = *HENNP[(2 * i) + 3];/* ���ʃZ�b�g */
            REG_00[1] = REG_00[0];/* �R�s�[ */
            REG_00[0] = ((REG_00[0] % 100) / 10);/* 10�̈ʂ̂ݒ��o */
            REG_00[0] <<= 4;/* ��ʂ�4�r�b�g�V�t�g */
            REG_00[1] = (REG_00[1] % 10);/* 1�̈ʂ̂ݒ��o */
            REG_00[0] |= REG_00[1];/* 10��1�̈ʂ����킹��BCD���� */
            MEMRY01[i] = (uint16_t)REG_00[0];/* �܂����ʃZ�b�g */
            
            REG_03 = (uint16_t)*HENNP[(2 * i) + 2];
            MEMRY01[i] |= (REG_03 << 8);/* ��������񂱂ŏ�ʃZ�b�g */
            
        } else if (i == 7) {
            /* W0007(��p�I�������u���v��ʂƐώZ�ғ����ԁu���v����)�̃������ł������ꍇ */
            /* ��ʂ̂�BCD�ɒ����K�v�����郁�����̏ꍇ */
            MEMRY01[i] = (uint16_t)*HENNP[(2 * i) + 3];/* ���ʃZ�b�g */
            
            REG_00[0] = *HENNP[(2 * i) + 2];/* ��ʃZ�b�g */
            REG_00[1] = REG_00[0];/* �R�s�[ */
            REG_00[0] = ((REG_00[0] % 100) / 10);/* 10�̈ʂ̂ݒ��o */
            REG_00[0] <<= 4;/* ��ʂ�4�r�b�g�V�t�g */
            REG_00[1] = (REG_00[1] % 10);/* 1�̈ʂ̂ݒ��o */
            REG_00[0] |= REG_00[1];/* 10��1�̈ʂ����킹��BCD���� */
            REG_03 = (uint16_t)REG_00[0];/* ���ɏ�ʃZ�b�g */
            MEMRY01[i] |= (REG_03 << 8);/* ��������񂱂ŏ�ʃZ�b�g */
            
        } else if ( (i == 6) || (i == 9) ) {
            /* ���/���ʂƂ���BCD�ɒ����K�v�����郁�����̏ꍇ */
            REG_00[0] = *HENNP[(2 * i) + 3];/* ���ʃZ�b�g */
            REG_00[1] = REG_00[0];/* �R�s�[ */
            REG_00[0] = ((REG_00[0] % 100) / 10);/* 10�̈ʂ̂ݒ��o */
            REG_00[0] <<= 4;/* ��ʂ�4�r�b�g�V�t�g */
            REG_00[1] = (REG_00[1] % 10);/* 1�̈ʂ̂ݒ��o */
            REG_00[0] |= REG_00[1];/* 10��1�̈ʂ����킹��BCD���� */
            MEMRY01[i] = (uint16_t)REG_00[0];/* �܂����ʃZ�b�g */
            
            REG_00[0] = *HENNP[(2 * i) + 2];/* ��ʃZ�b�g */
            REG_00[1] = REG_00[0];/* �R�s�[ */
            REG_00[0] = ((REG_00[0] % 100) / 10);/* 10�̈ʂ̂ݒ��o */
            REG_00[0] <<= 4;/* ��ʂ�4�r�b�g�V�t�g */
            REG_00[1] = (REG_00[1] % 10);/* 1�̈ʂ̂ݒ��o */
            REG_00[0] |= REG_00[1];/* 10��1�̈ʂ����킹��BCD���� */
            REG_03 = (uint16_t)REG_00[0];/* ���ɏ�ʃZ�b�g */
            MEMRY01[i] |= (REG_03 << 8);/* ��������񂱂ŏ�ʃZ�b�g */
            
        } else if (i == 8) {
            /* W0008 �ώZ�ғ����ԁu���v�̃G���A�ł���ꍇ */
            /* ����������ʂƉ��ʂ��t�ɂȂ� */
            MEMRY01[i] = (uint16_t)*HENNP[(2 * i) + 2];/* ���ʃZ�b�g */
            REG_03 = (uint16_t)*HENNP[(2 * i) + 3];
            MEMRY01[i] |= (REG_03 << 8);/* ��ʃZ�b�g */
            
        } else {
            /* ��O�̉ӏ��͂��̂܂ܓ˂����� */
            MEMRY01[i] = (uint16_t)*HENNP[(2 * i) + 3];/* ���ʃZ�b�g */
            REG_03 = (uint16_t)*HENNP[(2 * i) + 2];
            MEMRY01[i] |= (REG_03 << 8);/* ��ʃZ�b�g */
            
        }
        
    }/* �J��Ԃ������܂� */
    
    MEMRY01[14] = (uint16_t)TMPDT0;/* ���ʃZ�b�g */
    REG_03 = (uint16_t)*HENNP[30];
    MEMRY01[14] |= (REG_03 << 8);/* ��ʃZ�b�g */
    MEMRY01[15] = (uint16_t)TMPDT2;/* ���ʃZ�b�g */
    REG_03 = (uint16_t)TMPDT1;
    MEMRY01[15] |= (REG_03 << 8);/* ��ʃZ�b�g */
    
    for (uint16_t i = 0;i < REG_01;i++) {
        /* �ǂݍ��݃��[�h���������J��Ԃ� */
        REG_00[0] = (uint8_t)(MEMRY01[REG_02 + i] >> 8);/* �擾�����f�[�^�̏�ʂ��Z�b�g���� */
        
        REG_00[1] = (REG_00[0] >> 4);/* ���4�r�b�g���Z�b�g */
        REG_00[1] += 0x30;
        if (REG_00[1] >= 0x3A) {
            /* 10�ȏ�̏ꍇ */
            REG_00[1] += 0x07;/* 7���Z */
            
        }
        TBUF0[(TXPTR0 + 5 + (i * 4)) & 0x0FF] = REG_00[1];
        
        REG_00[1] = (REG_00[0] & 0x0F);/* ����4�r�b�g���Z�b�g */
        REG_00[1] += 0x30;
        if (REG_00[1] >= 0x3A) {
            /* 10�ȏ�̏ꍇ */
            REG_00[1] += 0x07;/* 7���Z */
            
        }
        TBUF0[(TXPTR0 + 6 + (i * 4)) & 0x0FF] = REG_00[1];
        
        REG_00[0] = (uint8_t)(MEMRY01[REG_02 + i] & 0x00FF);/* �擾�����f�[�^�̏�ʂ��Z�b�g���� */
        
        REG_00[1] = (REG_00[0] >> 4);/* ���4�r�b�g���Z�b�g */
        REG_00[1] += 0x30;
        if (REG_00[1] >= 0x3A) {
            /* 10�ȏ�̏ꍇ */
            REG_00[1] += 0x07;/* 7���Z */
            
        }
        TBUF0[(TXPTR0 + 7 + (i * 4)) & 0x0FF] = REG_00[1];
        
        REG_00[1] = (REG_00[0] & 0x0F);/* ����4�r�b�g���Z�b�g */
        REG_00[1] += 0x30;
        if (REG_00[1] >= 0x3A) {
            /* 10�ȏ�̏ꍇ */
            REG_00[1] += 0x07;/* 7���Z */
            
        }
        TBUF0[(TXPTR0 + 8 + (i * 4)) & 0x0FF] = REG_00[1];
        
        if ((i + 1) == REG_01) {
            /* ���[�v�Ō�̏ꍇ */
            REG_03 = (9 + (i * 4));/* TBUF0�̃|�C���^�ꏊ��ۑ� */
        }
        
    }/* �J��Ԃ������܂� */
    
    TBUF0[(TXPTR0 + REG_03) & 0x0FF] = 0x03;/* ETX���Z�b�g���� */
    
    REG_00[1] = 0;/* �����ŃN���A */
    for (uint16_t i = 1;i <= REG_03;i++) {
        /* �J��Ԃ����Z */
        REG_00[1] += TBUF0[(TXPTR0 + i) & 0x0FF];/* ���Z���� */
        
    }
    
    REG_01 = ASCICV(REG_00[1]);/* �����ϊ� */
    TBUF0[(TXPTR0 + REG_03 + 1) & 0x0FF] = (uint8_t)(REG_01 & 0x00FF);/* SUM���Z�b�g���� */
    TBUF0[(TXPTR0 + REG_03 + 2) & 0x0FF] = (uint8_t)(REG_01 >> 8);/* SUM���Z�b�g���� */
    TBUF0[(TXPTR0 + REG_03 + 3) & 0x0FF] = 0x0D;/* CR���Z�b�g���� */
    TBUF0[(TXPTR0 + REG_03 + 4) & 0x0FF] = 0x0A;/* LF���Z�b�g���� */
    TBUF0[TXPTR0] = 0x02;
    TXLEN0 += (REG_03 + 5);/* ���M�������Z���� */
    TXSET0 += (REG_03 + 5);/* ���M�������Z���� */
    TXFLG0 |= 0x11;/* ���M�v���A�J�n�Z�b�g */
    TXFLG0 &= 0xDF;/* ���M��ݾكZ�b�g */
    
    TSB_SC0->BUF = TBUF0[TXPTR0];/* �o�b�t�@�ɃZ�b�g */
    
    TXPTR0++;/* �߲����ݸ���� */
    TXPTR0 &= 0xFF;/* MAX FIX */
    TXLEN0--;/* �޸���� */
    TXSET0--;/* �޸���� */
    
    RXLEN0 = 0;/* ��M���N���A */
    RXPTR0 = 0;/* ��M�|�C���^�N���A */
    SCRSTT = 0;/* �N���A */
    
    /* WR�R�}���h�����܂� */
    
    OS_LoadTime->NEXT_LOAD_Time[7] = 10;/* ����Ăяo������ */
    OS_LoadTime->TSK_COND[7] = 0xA0;/* �x���t���O���z */
    
    return;
    
}

/**
 *�薼�F�����ϊ�����
 *�쐬�ҁF����@����
 *�����F�����ϊ������l��߂�����
**/
uint16_t ASCICV(uint8_t A_REG)
{
    uint16_t REG_00 = 0;/* �ꎞ�o�b�t�@ */
    uint8_t  REG_01 = 0;/* �ꎞ�o�b�t�@ */
    
    REG_01 = (A_REG >> 4);/* ��ʃZ�b�g */
    REG_01 += 0x30;
    if (REG_01 >= 0x3A) {
        /* 0x3A�����̏ꍇ */
        REG_01 += 0x07;/* 7���Z */
        
    }
    
    REG_00 = (uint16_t)REG_01;/* ���ʂɃZ�b�g */
    
    REG_01 = (A_REG & 0x0F);/* ���ʃZ�b�g */
    REG_01 += 0x30;
    if (REG_01 >= 0x3A) {
        /* 0x3A�����̏ꍇ */
        REG_01 += 0x07;/* 7���Z */
        
    }
    
    REG_00 |= ((uint16_t)REG_01 << 8);/* ��ʂɃZ�b�g */
    
    return REG_00;
    
}


/**
 *�薼�F�������޲�ؕϊ�����
 *�쐬�ҁF����@����
 *�����F�������޲�ؕϊ������l��߂�����
**/
uint8_t BNRCV(uint8_t W_REG, uint8_t A_REG)
{
    uint8_t REG_00 = 0;
    
    W_REG -= 0x30;
    if (W_REG >= 0x0A) {
        /* 10�ȏ�̏ꍇ */
        W_REG -= 7;
    }
    A_REG -= 0x30;
    if (A_REG >= 0x0A) {
        /* 10�ȏ�̏ꍇ */
        A_REG -= 7;
    }
    
    A_REG <<= 4;/* 4bit���V�t�g */
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
 *�薼�F������HEX16�i(�޲��)�ϊ�����
 *�쐬�ҁF����@����
 *�����F������HEX16�i(�޲��)�ϊ������l��߂�����
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
    A_REG |= W_REG;/* OR���܂� */
    
    return A_REG;
}