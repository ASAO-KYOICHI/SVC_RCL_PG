/***
 * �薼�F���쐧��A�v��
 * �쐬�ҁF����@����
 * ���쐧����s���A�v���P�[�V����
***/
#include "tmpm380_OPRCNT.h"

const uint8_t PATMAX[3] = {
    /* �i���A�H�A���⎞��MAX */
    50, 0, 30
};
const uint8_t PATMIN[3] = {
    /* �i���A�H�A���⎞��MIN */
    10, 0, 2
};

const uint8_t STMTBL[16] = {
    /* �i���ݒ�̉����l�e�[�u�� */
    9, 9, 12, 12,/* 35R */
    7, 7, 12, 7,/* 50R */
    7, 7, 12, 7,/* 100R */
    9, 9, 12, 12
};

void OPRCNT(void)
{
    OS_TSKCon1 *OS_LoadTime;/* OS�ݒ�l��ǂނ��߂̃|�C���^�\���̂ɖ��O������ */
    OS_LoadTime = OSTSKCon1S();/* �֐��o�R�Ń|�C���^��OSparam�ɃA�N�Z�X */
    
    uint8_t REG_00 = 0;/* �ꎞ�ϐ� */
    
    if (TNO > 0) {
        /* �Z���t�e�X�g���ł���ꍇ */
        OS_LoadTime->NEXT_LOAD_Time[6] = 50;/* ����Ăяo������ */
        OS_LoadTime->TSK_COND[6] = 0xA0;/* �x���t���O���z */
        
        return;
        
    }
    
    if ((STSFL1 & 0x01) > 0) {
        /* �Z���t�e�X�g��t���ł���ꍇ */
        if ((MXSW & 0x90) < 0x90) {
            /* �`���[�\��Ɠd��SW��������Ă��Ȃ��ꍇ */
            STSAST = 0;
            STSFL1 &= 0xFE;/* �Z���t�e�X�g��t�����N���A */
            
        } else {
            /* �`���[�\��Ɠd��SW��������Ă���ꍇ */
            STSAST++;/* �^�C�}�C���N�������g */
            if (STSAST >= 60) {
                /* 3�b�o�߂����ꍇ */
                STSAST = 0;/* �^�C�}�\�N���A */
                TNO = 1;/* �Z���t�e�X�g1�� */
                
            }
            
            OS_LoadTime->NEXT_LOAD_Time[6] = 50;/* ����Ăяo������ */
            OS_LoadTime->TSK_COND[6] = 0xA0;/* �x���t���O���z */
            
            return;
            
        }
        
    }
    
    if ((STSFL1 & 0x02) > 0) {
        /* �������[�N���A��t���ł���ꍇ */
        if ((MXSW & 0x50) < 0x50) {
            /* �^�]�Ɠd���X�C�b�`��������Ă��Ȃ��ꍇ */
            MCLAST = 0;/* �^�C�}�N���A */
            STSFL1 &= 0xFD;/* ��t�����N���A */
            
        } else {
            /* �^�]�Ɠd���X�C�b�`��������Ă���ꍇ */
            MCLAST++;/* �C���N�������g */
            if (MCLAST < 60) {
                /* 3�b�����ł���ꍇ */
                /* �������Ȃ� */
                OS_LoadTime->NEXT_LOAD_Time[6] = 50;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[6] = 0xA0;/* �x���t���O���z */
                
                return;
                
            } else if (MCLAST == 60) {
                /* 3�b�W���X�g�ł���ꍇ */
                BLCLR();/* �������N���A */
                if ((SYKND0 & 0x20) > 0) {
                    /* 15���d�l�ł���ꍇ */
                    *HENNP[4] = 15;/* �ݒ艷�x��15���d�l�ɕύX���� */
                    *HENNP[5] = 15;
                    *HENNP[6] = 15;
                    *HENNP[7] = 15;
                    
                }
                
                MCLAST = 60;/* ����Z�b�g */
                
                OS_LoadTime->NEXT_LOAD_Time[6] = 50;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[6] = 0xA0;/* �x���t���O���z */
                
                return;
                
            } else {
                /* 3�b���̏ꍇ */
                MCLAST = 60;/* ����Z�b�g */
                
                OS_LoadTime->NEXT_LOAD_Time[6] = 50;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[6] = 0xA0;/* �x���t���O���z */
                
                return;
                
            }
            
        }
        
    } else {
        /* �������[�N���A��t���ł͂Ȃ��ꍇ */
        if (((SYSFLG & 0x40) == 0) && ((STSFL1 & 0x08) > 0) && ((MXSWO0 & 0x20) > 0)) {
            /* �d���I�t�ŁA�蓮�r����t���ŁA�I���X�C�b�`���I���ł���ꍇ */
            MDAST++;/* �^�C�}�C���N�������g */
            
            if (MDAST >= 60) {
                /* 3�b�o�߂����ꍇ */
                STSFL0 |= 0x40;/* �Z�b�g�N���b�N�� */
                STSFL1 |= 0x10;/* �蓮�r����t�t���O���Z�b�g */
                
                WLCTM5 = 0;/* �r���\�����Ԃ��N���A���� */
                RPAT |= 0xC0;/* �r����ԃt���O�Z�b�g */
                
                STSFL1 &= 0xF7;/* �蓮�r����t�����N���A */
                MDAST = 0;/* �^�C�}�N���A */
                
            }
            
        } else {
            /* �d���I�����͎蓮�r����t���ł͂Ȃ����͑I���X�C�b�`���I�t�ł���ꍇ */
            STSFL1 &= 0xF7;/* �蓮�r����t�����N���A */
            MDAST = 0;/* �^�C�}�N���A */
            
        }
        
        if ((STSFL0 & 0x02) == 0) {
            /* �C�j�V�����C�Y���������ł���ꍇ */
            if (MXSW == 0x90) {
                /* �`���[�\��Ɠd���X�C�b�`��������Ă���ꍇ */
                KFLG = 0x90;/* �L�[�t���O�쐬 */
                STSFL1 |= 0x01;/* �Z���t�e�X�g��t���Z�b�g */
                
            } else if (MXSW == 0x50) {
                /* �^�]�Ɠd���X�C�b�`��������Ă���ꍇ */
                KFLG = 0x50;/* �L�[�t���O�쐬 */
                STSFL1 |= 0x02;/* �������[�N���A��t���Z�b�g */
                
            }
            
            if ((STSFL1 & 0x03) > 0) {
                /* ��t���������Z�b�g����Ă���ꍇ */
                OS_LoadTime->NEXT_LOAD_Time[6] = 50;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[6] = 0xA0;/* �x���t���O���z */
                
                return;
                
            }
            
        }
        
    }
    
    if (((ABNF & 0xC0) == 0) || ((MXOTN2 & 0x80) == 0)) {
        /* �ُ�x��o�͂��Ă��Ȃ��Ƃ����A�O���x����o���Ă��Ȃ��ꍇ */
        if (((STSFL0 & 0x08) == 0) && ((STSFL1 & 0x04) == 0)) {
            /* �����A���[���o�͒��ł͖����A�}�j���A������G���[�ł��Ȃ��ꍇ */
            TSB_PC_DATA_PC2 = 0;/* �u�U�[�o�͂��N���A */
            
        }
        
    }
    
    REG_00 = KFLG;/* �L�[�t���O�̒l��ǂݍ��� */
    KFLG = MXSWO0;/* ���ݒl�ɍX�V */
    REG_00 ^= KFLG;/* �ω������m */
    REG_00 &= MXSWO0;/* ON�g���K�[�̂ݎc�� */
    
    /* �ݒ胂�[�h�̒ǉ����� */
    if (MMODE == 3) {
        /* �d��ON���Ƀ����e��ʂ���r�����삵���ہASW����ŕʉ�ʂɔ�ׂĂ��܂��A���̌�u�U�[����������ɂȂ�o�O�΍� */
        /* �}�j���A�����[�h����G���[�ɂȂ��Ă���ꍇ */
        REG_00 &= 0xF8;/* �������X�C�b�`�̃I���g���K�͏��� */
        
    }
    
    if (REG_00 > 0) {
        /* ��������ON�g���K�[�����m�����ꍇ */
        ONTRG = REG_00;/* �ۑ� */
        
        if (((ABNF & 0x0F) == 0) && ((ONTRG & 0x20) > 0) && ((SYSFLG & 0x1E) == 0)) {
            /* �ُ킪�����A�I���X�C�b�`�̃I���g���K�[�ł���A�^�]�H�����ł͂Ȃ��ꍇ */
            if (((STSFL0 & 0x80) > 0) || ((SYSFLG & 0x40) > 0)) {
                /* �ݒ胂�[�h�����́A�d�����I���ł���ꍇ */
                if ((RPAT & 0x20) == 0) {
                    /* ���H�����ł͂Ȃ��ꍇ */
                    STSFL0 |= 0x40;/* �N���b�N�����Z�b�g */
                    NPAT++;/* �p�^�[���I�����X�V */
                    NPAT &= 0x03;/* �K�v�ȕ������}�X�N���� */
                    
                }
                
            } else {
                /* �ݒ胂�[�h�ł͂Ȃ��d���I�t�ł���ꍇ */
                if ((MXOTN0 & 0xC0) > 0) {
                    /* �r���ق����Ă���ꍇ */
                    STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
                    STSFL1 |= 0x08;/* �蓮�r�������t���t���O���Z�b�g */
                    MDAST = 0;/* �蓮�r����t���^�C�}���N���A */
                    
                }
                
            }
            
        }
        
        if (((ABNF & 0x0F) > 0) || ((RPAT & 0x20) == 0)) {
            /* �ُ킠�肩�A���H�����łȂ��ꍇ */
            if ((ONTRG & 0x10) > 0) {
                /* �d���X�C�b�`�̃I���g���K�ł���ꍇ */
                RPAT &= 0xDF;/* ���H�������Z�b�g���� */
                
                SYSFLG ^= 0x40;/* RENEW */
                SYSFLG &= 0xE1;/* �K�v�ȕ������}�X�N */
                STSFL0 &= 0xDF;/* ���������Z�b�g */
                STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
                
                ABNF &= 0x20;/* �ُ�y�у`���[�ُ�����Z�b�g���� */
                AD1TM = AD2TM = 0;/* ���x�Z���T�ُ�^�C�}�N���A */
                
                TSB_PC_DATA_PC2 = 0;/* �u�U�[�o�͂��I�t�ɂ��� */
                MXOTN2 &= 0x7F;/* �O���x��o�͂��I�t */
                REG_00 = TSB_PC->DATA;/* �f�[�^���W�X�^�����[�h */
                REG_00 &= 0x07;/* �}�X�N�Z�b�g */
                REG_00 |= (MXOTN2 & 0xF8);/* �f�[�^�Z�b�g */
                TSB_PC->DATA = REG_00;/* �X�V */
                
                if ((SYSFLG & 0x40) > 0) {
                    /* �d���I���ł���ꍇ */
                    SYSFLG &= 0xDF;/* �`���[�\����� */
                    
                }
                
                if ((STSFL0 & 0x80) == 0) {
                    /* �ݒ蒆�ł͂Ȃ��ꍇ */
                    DSPSEQ = 0;/* �V�[�P���X�N���A */
                    
                }
                
            }
            
        }
        
        if (((SYSFLG & 0x40) > 0) && ((STSFL0 & 0x80) == 0) && ((ONTRG & 0x40) > 0)) {
            /* �d�����I���ŁA�ݒ胂�[�h���ł͂Ȃ��A�^�]�X�C�b�`�̃I���g���K�\�����m�����ꍇ */
            if ((ABNF & 0xC0) > 0) {
                /* �ُ�x��o�͒��ł���ꍇ */
                ABNF &= 0x3F;/* �ُ�x��o�͂����Z�b�g */
                
            } else if ((STSFL1 & 0x80) > 0) {
                /* �^��������̃A���[���v��������ꍇ */
                STSFL1 &= 0x7F;/* �^��������̃A���[���v���t���O���I�t�ɂ��� */
                STSFL0 &= 0xF7;/* �����A���[���o�͒��t���O���I�t�ɂ��� */
                
                TSB_PC_DATA_PC2 = 0;/* �u�U�[���I�t�ɂ��� */
                MXOTN2 &= 0x7F;/* �O���x��o�͂��I�t���� */
                STSFL0 |= 0x40;/* �N���b�N�t���O�Z�b�g */
                REG_00 = TSB_PC->DATA;/* �f�[�^���W�X�^�����[�h */
                REG_00 &= 0x07;/* �K�v�Ȃ����}�X�N */
                REG_00 |= (MXOTN2 & 0xF8);/* OR���� */
                TSB_PC->DATA = REG_00;/* �X�V */
                
            } else if (CCSEQ == 15) {
                /* ��p�����ł������ꍇ */
                CCSEQ = 0;/* ��p�V�[�P���X���N���A���� */
                STSFL0 &= 0xF7;/* �����A���[���o�͒��t���O���I�t�ɂ��� */
                                
                TSB_PC_DATA_PC2 = 0;/* �u�U�[���I�t�ɂ��� */
                MXOTN2 &= 0x7F;/* �O���x��o�͂��I�t���� */
                REG_00 = TSB_PC->DATA;/* �f�[�^���W�X�^�����[�h */
                REG_00 &= 0x07;/* �K�v�Ȃ����}�X�N */
                REG_00 |= (MXOTN2 & 0xF8);/* OR���� */
                TSB_PC->DATA = REG_00;/* �X�V */
                
                STSFL0 |= 0x40;/* �N���b�N�t���O�Z�b�g */
                SYSFLG &= 0xE1;/* �^�]�t���O��S�ăI�t�ɂ��� */
                STSFL0 &= 0xDF;/* �������t���O���N���A���� */
                
                if ((STSFL0 & 0x80) == 0) {
                    /* �ݒ胂�[�h���Ŗ����ꍇ */
                    DSPSEQ = 0;
                    
                }
                
            } else if ((STSFL0 & 0x20) == 0) {
                /* �������ł���ꍇ */
                /* �������Ȃ� */
                
            } else if (((ABNF & 0x0F) == 0) || ((ABNF & 0x0F) == 8) || ((ABNF & 0x0F) == 9) || ((ABNF & 0x0F) == 14)) {
                /* �ُ�Ȃ��A���͏퉷�����ʓd�ɖ_�ُ킩�A���͏퉷���x�Z���T�[�ُ�A�^��x�ُ�ł���ꍇ */
                if ((SYSFLG & 0x02) == 0) {
                    /* �^�]���I�t�̏ꍇ */
                    if ((ABNF & 0x0F) > 0) {
                        /* �ҋ@���ُ̈킪����ꍇ */
                        /* �������Ȃ� */
                        
                    } else {
                        /* �ُ킪�����ꍇ */
                        SYSFLG |= 0x02;/* �^�]�t���O���I������ */
                        STIME1 = RTCDT[1];/* �J�n�����u���v�̃o�C�i���f�[�^ */
                        STIME2 = RTCDT[2];/* �J�n�����u���v�̃o�C�i���f�[�^ */
                        ETIME1 = ETIME2 = 0;/* �I���������N���A���� */
                        STSFL0 |= 0x40;/* �Z�b�g�N���b�N�� */
                        
                    }
                    
                } else {
                    /* �^�]���I���ł���ꍇ */
                    if (CCSEQ >= 10) {
                        /* �^������H���ł���ꍇ */
                        /* �������Ȃ� */
                        
                    } else {
                        /* �^������H���ɂȂ��Ă��Ȃ��ꍇ */
                        if (((SYKND0 & 0x10) == 0) && (CCSEQ < 4) && ((short)SWTMP0 < TMPDT2)) {
                            /* �`���[���ł���A�퉷��p�H�����������ŁA�ݒ艷�x���퉷�������x�������ꍇ */
                            SYSFLG |= 0x01;/* �퉷���r�����Z�b�g���� */
                            
                        }
                        
                        SYSFLG &= 0xE3;/* �e�H���t���O�����Z�b�g���� */
                        CCSEQ = 10;/* �^������H�����Z�b�g���� */
                        DSPSEQ = 0;/* �f�B�X�v���[�V�[�P���X���N���A */
                        ETIME1 = RTCDT[1];/* �I�������u���v�Z�b�g */
                        ETIME2 = RTCDT[2];/* �I�������u���v�Z�b�g */
                        
                        STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
                        STSFL1 |= 0x40;/* �^������H�����̃A���[���֎~���Z�b�g */
                        ABNF &= 0xBF;/* �p���ُ펞�ُ̈�x������Z�b�g */
                        
                    }
                    
                }
                
            }
            
        }/* �d�����I�t���A�ݒ胂�[�h���ł��邩�A�^�]�X�C�b�`�̃I���g���K�\�����m�̏ꍇ�͂����ɔ����� */
        
        if (((RPAT & 0x20) == 0) && ((ONTRG & 0x80) > 0) && ((SYKND0 & 0x10) == 0) && ((ABNF & 0x0F) == 0)) {
            /* ���H�����ł͂Ȃ��A�`���[�\��X�C�b�`��������Ă���A�`���[���ł���A�ُ킪�Ȃ��ꍇ */
            SYSFLG ^= 0x20;/* �`���[�\��r�b�g�𔽓]���� */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            
        }
        
        if (((STSFL0 & 0x80) > 0) && (DSPSEQ > 0)) {
            /* �ݒ胂�[�h�ł���A�f�B�X�v���[�V�[�P���X��1�ȏ�ł���ꍇ */
            OPRSET();/* �ݒ胂�[�h�̑��쏈���� */
            
        }
        
        if (((ONTRG & 0x08) > 0) && ((ABNF & 0x0F) == 0)) {
            /* �ݒ�X�C�b�`�̃I���g���K�[�ŁA�ُ�Ȃ��ł���ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            STSFL0 ^= 0x80;/* �ݒ胂�[�h���t���O�𔽓] */
            DSPSEQ = 0;/* �\���V�[�P���X���N���A */
            DSPSSQ = 0;/* �\���T�u�V�[�P���X���N���A */
            STSFL0 |= 0x10;/* �\���؂�ւ����A�ŏ��̓u�U�[���֎~�t���O���Z�b�g */
            
        }
        
    }/* ON�g���K�[�����m�ł��Ă��Ȃ��ꍇ�͂����ɔ����� */
    
    if ((STSFL0 & 0x80) == 0) {
        /* �ݒ胂�[�h�ł͂łȂ��ꍇ */
        INDPTM = 0;/* �ʏ�\�����A�^�C�}���N���A */
        
    } else {
        /* �ݒ胂�[�h���ł���ꍇ */
        if (DSPSEQ != 4) {
            /* ��򃂁[�h�ł͖����ꍇ */
            if ((MXSWO0 & 0x2F) > 0) {
                /* �ݒ胂�[�h�Ɋ֌W����X�C�b�`���͂��������ꍇ */
                INDPTM = 0;/* �ʏ�\�����A�^�C�}���N���A */
                
            } else {
                /* �ݒ胂�[�h�Ɋ֌W����X�C�b�`���͂������ꍇ */
                if (INDPTM >= 12000) {
                    /* 10���o�߂����ꍇ */
                    INDPTM = 0;/* �ʏ�\�����A�^�C�}���N���A */
                    STSFL0 &= 0x7F;/* �ݒ胂�[�h���N���A */
                    DSPSEQ = 0;/* �f�B�X�v���[�V�[�P���X���N���A */
                    DSPSSQ = 0;/* �f�B�X�v���[�T�u�V�[�P���X���N���A */
                    STSFL0 |= 0x40;/* �ݒ胂�[�h�̉����N���b�N���Z�b�g */
                    MMODE = 0;/* �}�j���A�����[�h�i���o�[���N���A */
                    
                }
                
            }
            
        } else if (DSPSSQ == 3) {
            /* ��򒆒f����\���ł���ꍇ */
            CLNRTM++;/* �^�C�}�C���N�������g */
            if (CLNRTM >= 200) {
                /* 10�b�o�߂����ꍇ */
                CLNRTM = 0;/* �^�C�}�N���A */
                DSPSSQ = 2;/* ���H���\�����Z�b�g */
                
            }
            
        }
        
    }
    
    if (((STSFL0 & 0x40) > 0) && ((ABNF & 0x80) == 0) && ((STSFL0 & 0x08) == 0) && ((STSFL1 & 0x04) == 0)) {
        /* �N���b�N���Z�b�g����Ă���A�ُ�x��o�͂������A�����A���[���o�͒��ł͖����A�}�j���A�����[�h�̑���G���[�Ŗ����ꍇ */
        TSB_PC_DATA_PC2 = 1;/* �u�U�[�o�͂��I������ */
        
    }
    
    STSFL0 &= 0xBF;/* �N���b�N���t���O������ */
    OS_LoadTime->NEXT_LOAD_Time[6] = 50;/* ����Ăяo������ */
    OS_LoadTime->TSK_COND[6] = 0xA0;/* �x���t���O���z */
    
    return;
}

/***
 * �薼�F�ݒ胂�[�h���̑��쐧��
 * �쐬�ҁF����@����
 * �ݒ蒆�̑��쐧����s���T�u���[�`��
***/
void OPRSET(void)
{
    uint8_t REG_00 = 0;
    
    /* �g�b�v���j���[�R���g���[�� */
    if (DSPSSQ == 0) {
        /* �T�u�V�[�P���X���O�i�g�b�v���j���[�j�� */
        if (((ONTRG & 0x04) > 0) && ((DSPSEQ != 4) || ((DSPSEQ == 4) && ((SYSFLG & 0x40) == 0)))) {
            /* ���X�C�b�`�̃I���g���K�ŁA��򃂁[�h�̑I���ł͖������A��򃂁[�h�̑I���œd�����I�t�ł������ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            DSPSSQ = 1;/* �T�u�V�[�P���X1�ɂ��� */
            
            if (DSPSEQ == 1) {
                /* ��ʃV�[�P���X���u�P�v�p�^�[���ݒ�ł���ꍇ */
                /* �ݒ�p�f�[�^�ɃZ�b�g */
                if (NPAT == 0) {
                    /* 1�I���� */
                    EDDT[0] = PAT1;/* 1���Z�b�g */
                    
                } else if (NPAT == 1) {
                    /* 2�I���� */
                    EDDT[0] = PAT2;/* 2���Z�b�g */
                    
                } else if (NPAT == 2) {
                    /* 3�I���� */
                    EDDT[0] = PAT3;/* 3���Z�b�g */
                    
                } else {
                    /* 4�I���� */
                    EDDT[0] = PAT4;/* 4���Z�b�g */
                    
                }
                
            } else if (DSPSEQ == 2) {
                /* ��ʃV�[�P���X���u�Q�v�퉷�������x�ݒ�ł���ꍇ */
                EDDT[0] = SWTMP0;/* �ݒ�p�f�[�^�ɃZ�b�g */
                
            } else if (DSPSEQ == 3) {
                /* ��ʃV�[�P���X���u�R�v�����r�����[�h�ݒ�ł���ꍇ */
                EDDT[0] = (SYSFLG & 0x80);/* �ݒ�p�f�[�^�ɃZ�b�g */
                
            } else if (DSPSEQ == 4) {
                /* ��ʃV�[�P���X���u�S�v��򃂁[�h�ݒ�ł���ꍇ */
                EDDT[0] = 0;/* �ݒ�p�f�[�^�ɃZ�b�g */
                
            } else if (DSPSEQ == 5) {
                /* ��ʃV�[�P���X���u�T�v�`���[�\�񎞍��ݒ�ł���ꍇ */
                EDDT[0] = SETTM;/* �ݒ�p�f�[�^�ɃZ�b�g */
                EDDT[1] = SETTH;/* �ݒ�p�f�[�^�ɃZ�b�g */
                
            } else if (DSPSEQ == 6) {
                /* ��ʃV�[�P���X���u�U�v���ݎ����ݒ�ł���ꍇ */
                EDDT[0] = RTCDT[1];/* �ݒ�p�f�[�^�ɃZ�b�g */
                EDDT[1] = RTCDT[2];/* �ݒ�p�f�[�^�ɃZ�b�g */
                EDDT[2] = RTCDT[4];/* �ݒ�p�f�[�^�ɃZ�b�g */
                EDDT[3] = RTCDT[5];/* �ݒ�p�f�[�^�ɃZ�b�g */
                EDDT[4] = RTCDT[6];/* �ݒ�p�f�[�^�ɃZ�b�g */
                
            } else {
                /* ����ȊO�̏ꍇ */
                DABHP = 0;/* �ُ헚���ꏊ�N���A */
                
            }
            
        } else if ((ONTRG & 0x02) > 0) {
            /* DOWN�X�C�b�`�ł���ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            DSPSEQ++;/* �V�[�P���X���C���N�������g */
            
            if ((DSPSEQ == 5) && ((SYKND0 & 0x10) > 0)) {
                /* �`���[�\���ʂŏ��C���ł������ꍇ */
                DSPSEQ = 6;/* �`���[�\����X�L�b�v���� */
                
            }
            
            if (DSPSEQ >= 8) {
                /* ����ȏ�ɂȂ����ꍇ */
                DSPSEQ = 1;/* �����l��FIX */
                
            }
            
        } else if ((ONTRG & 0x01) > 0) {
            /* UP�X�C�b�`�ł���ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            DSPSEQ --;/* �V�[�P���X���C���N�������g */
            
            if ((DSPSEQ == 5) && ((SYKND0 & 0x10) > 0)) {
                /* �`���[�\���ʂŏ��C���ł������ꍇ */
                DSPSEQ = 4;/* �`���[�\����X�L�b�v���� */
                
            }
            
            if (DSPSEQ == 0) {
                /* ���������ɂȂ����ꍇ */
                DSPSEQ = 7;/* ����l��FIX */
                
            }
            
        }
        
    } else if ((DSPSEQ & 0x07) == 1) {
        /* �p�^�[���o�^�ݒ�ł���ꍇ */
        if (DSPSSQ == 2) {
            /* ���₠��/�����̐ݒ�ł���ꍇ */
            if ((ONTRG & 0x03) > 0) {
                /* UP/DOWN�X�C�b�`��������Ă����ꍇ */
                STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
                
                if (NPAT == 0) {
                    /* �p�^�[��1�̏ꍇ */
                    EDDT[0] = (0x01 ^ EDDT[0]);
                    
                } else if (NPAT == 1) {
                    /* �p�^�[��2�̏ꍇ */
                    EDDT[0] = (0x02 ^ EDDT[0]);
                    
                } else if (NPAT == 2) {
                    /* �p�^�[��3�̏ꍇ */
                    EDDT[0] = (0x04 ^ EDDT[0]);
                    
                } else {
                    /* �p�^�[��4�̏ꍇ */
                    EDDT[0] = (0x08 ^ EDDT[0]);
                    
                }
                
                return;/* �����Ŗ߂� */
                
            }
            
        } else if ((ONTRG & 0x01) > 0) {
            /* UP�X�C�b�`�̃I���g���K�ł������ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            
            EDDT[0]++;/* �l���C���N�������g */
            if (EDDT[0] > PATMAX[(DSPSSQ - 1) & 0x03]) {
                /* ����l�𒴂���ꍇ */
                EDDT[0] = PATMIN[(DSPSSQ - 1) & 0x03];/* �����l���Z�b�g���� */
                
                if (DSPSSQ == 1) {
                    /* ��p�i���ݒ�ł���ꍇ */
                    REG_00 = SYKND0;/* �f�B�b�v�X�C�b�`�����Z�b�g */
                    REG_00 >>= 4;/* 4�r�b�g���낷 */
                    EDDT[0] = STMTBL[REG_00];/* �i���ݒ�̉����l���Z�b�g���� */
                    
                }
                
            }
            
            return;/* �����Ŗ߂� */
            
        } else if ((ONTRG & 0x02) > 0) {
            /* �_�E���X�C�b�`�̃I���g���K�ł������ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            
            EDDT[0] --;/* �l���f�N�������g */
            if (DSPSSQ == 1) {
                /* ��p�i���ݒ�ł���ꍇ */
                REG_00 = SYKND0;/* �f�B�b�v�X�C�b�`�����Z�b�g */
                REG_00 >>= 4;/* 4�r�b�g���낷 */
                REG_00 = STMTBL[REG_00];/* ��p�i���̉����l */
                
            } else {
                /* ��p�i���ݒ�ȊO�ł���ꍇ */
                REG_00 = PATMIN[(DSPSSQ - 1) & 0x03];/* �m�[�}�������l */
                
            }
            
            if (EDDT[0] < REG_00) {
                /* �����l�������ꍇ */
                EDDT[0] = PATMAX[(DSPSSQ - 1) & 0x03];/* ����l���Z�b�g���� */
                
            }
            
            return;/* �����Ŗ߂� */
            
        }
        
        if ((ONTRG & 0x20) > 0) {
            /* �I���X�C�b�`��������Ă���ꍇ */
            if ((SYSFLG & 0x1E) > 0) {
                /* �^�]���ł���ꍇ */
                return;/* �߂� */
                
            }
            
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            DSPSSQ = 1;/* �ݒ�g�b�v�ɕύX */
            
        } else if ((ONTRG & 0x0C) == 0) {
            /* ���X�C�b�`�Ɛݒ�X�C�b�`��������Ă��Ȃ��ꍇ */
            return;/* �߂� */
            
        } else if (((ONTRG & 0x04) > 0) || ((ONTRG & 0x0C) == 0x08)) {
            /* ���X�C�b�`��������Ă���A���͐ݒ�X�C�b�`��������Ă���ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            if (DSPSSQ == 1) {
                /* �T�u�V�[�P���X���u�P�v�A�i���ݒ�ł���ꍇ */
                if (NPAT == 0) {
                    /* �p�^�[��1�̏ꍇ */
                    PAT1 = EDDT[0];/* �ҏW���̒l���Z�b�g���� */
                    
                } else if (NPAT == 1) {
                    /* �p�^�[��2�̏ꍇ */
                    PAT2 = EDDT[0];/* �ҏW���̒l���Z�b�g���� */
                    
                } else if (NPAT == 2) {
                    /* �p�^�[��3�̏ꍇ */
                    PAT3 = EDDT[0];/* �ҏW���̒l���Z�b�g���� */
                    
                } else {
                    /* �p�^�[��4�̏ꍇ */
                    PAT4 = EDDT[0];/* �ҏW���̒l���Z�b�g���� */
                    
                }
                
            } else if (DSPSSQ == 2) {
                /* �T�u�V�[�P���X���u�Q�v�A����L�薳���̐ݒ�ł���ꍇ */
                RPAT &= 0xF0;/* �}�X�N */
                RPAT |= (EDDT[0] & 0x0F);/* �ҏW���̒l���Z�b�g���� */
                
            } else {
                /* ���⎞�Ԑݒ�ł���ꍇ */
                if (NPAT == 0) {
                    /* �p�^�[��1�̏ꍇ */
                    SCTM1 = EDDT[0];/* �ҏW���̒l���Z�b�g���� */
                    
                } else if (NPAT == 1) {
                    /* �p�^�[��2�̏ꍇ */
                    SCTM2 = EDDT[0];/* �ҏW���̒l���Z�b�g���� */
                    
                } else if (NPAT == 2) {
                    /* �p�^�[��3�̏ꍇ */
                    SCTM3 = EDDT[0];/* �ҏW���̒l���Z�b�g���� */
                    
                } else {
                    /* �p�^�[��4�̏ꍇ */
                    SCTM4 = EDDT[0];/* �ҏW���̒l���Z�b�g���� */
                    
                }
                
            }
            
            if ((ONTRG & 0x08) > 0) {
                /* �ݒ�X�C�b�`��ON�ł������ꍇ */
                return;/* �����Ŗ߂� */
                
            }
            
            if (DSPSSQ == 2) {
                /* ����L�薳���̐ݒ�ł���ꍇ */
                REG_00 = (0x01 << NPAT);/* �r�b�g���Z�b�g */
                if ((RPAT & REG_00) == 0) {
                    /* ����Ȃ��ł���ꍇ */
                    DSPSSQ = 1;/* ���ڂ̍ŏ��Z�b�g */
                    
                } else {
                    /* ����L��̏ꍇ */
                    DSPSSQ++;/* �C���N�������g */
                    if (DSPSSQ >= 4) {
                        /* �ő�l�ȏ�ɂȂ����ꍇ */
                        DSPSSQ = 1;/* ���ڂ̍ŏ��Z�b�g */
                        
                    }
                    
                }
                
            } else {
                /* ����L�薳���ȊO�̐ݒ�ł���ꍇ */
                DSPSSQ++;/* �C���N�������g */
                if (DSPSSQ >= 4) {
                    /* �ő�l�ȏ�ɂȂ����ꍇ */
                    DSPSSQ = 1;/* ���ڂ̍ŏ��Z�b�g */
                    
                }
                
            }
            
        }
        
        /* ���̐ݒ�l���Z�b�g���鏈�� */
        if (DSPSSQ == 1) {
            /* ����A��p���x�ݒ�ł���ꍇ */
            if (NPAT == 0) {
                /* �p�^�[��1�ł���ꍇ */
                EDDT[0] = PAT1;
                
            } else if (NPAT == 1) {
                /* �p�^�[��2�ł���ꍇ */
                EDDT[0] = PAT2;
                
            } else if (NPAT == 2) {
                /* �p�^�[��3�ł���ꍇ */
                EDDT[0] = PAT3;
                
            } else {
                /* �p�^�[��4�ł���ꍇ */
                EDDT[0] = PAT4;
                
            }
            
        } else if (DSPSSQ == 2) {
            /* ���₠��/�Ȃ��̐ݒ�ł���ꍇ */
            EDDT[0] = (RPAT & 0x0F);/* LSB��4�r�b�g���Z�b�g���� */
            
        } else {
            /* ���⎞�Ԃ̐ݒ�ł���ꍇ */
            if (NPAT == 0) {
                /* �p�^�[��1�ł���ꍇ */
                EDDT[0] = SCTM1;
                
            } else if (NPAT == 1) {
                /* �p�^�[��2�ł���ꍇ */
                EDDT[0] = SCTM2;
                
            } else if (NPAT == 2) {
                /* �p�^�[��3�ł���ꍇ */
                EDDT[0] = SCTM3;
                
            } else {
                /* �p�^�[��4�ł���ꍇ */
                EDDT[0] = SCTM4;
                
            }
            
        }
        
        /* �p�^�[���o�^�ݒ�ł���ꍇ�����܂� */
        
    } else if ((DSPSEQ & 0x07) == 2) {
        /* �퉷�������x�ݒ�ł���ꍇ */
        if ((ONTRG & 0x01) > 0) {
            /* UP�X�C�b�`�ł������ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            EDDT[0]++;/* �C���N�������g */
            
            if ( (((SYKND0 & 0x10) == 0) && (EDDT[0] > 35))
            || (((SYKND0 & 0x10) > 0) && (EDDT[0] > 40)) ) {
                /* ����𒴂����ꍇ */
                EDDT[0] = 20;/* �����lFIX */
                
            }
            
        } else if ((ONTRG & 0x02) > 0) {
            /* DOWN�X�C�b�`�ł������ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            EDDT[0] --;/* �f�N�������g */
            
            if (EDDT[0] < 20) {
                /* �����l�����ɂȂ����ꍇ */
                if ((SYKND0 & 0x10) == 0) {
                    /* �`���[���ł���ꍇ */
                    EDDT[0] = 35;/* MAX FIX */
                    
                } else {
                    /* ���C���ł���ꍇ */
                    EDDT[0] = 40;/* MAX FIX */
                    
                }
                
            }
            
        } else if ((ONTRG & 0x08) > 0) {
            /* �ݒ�X�C�b�`�ł������ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            SWTMP0 = EDDT[0];/* �ݒ�l���f */
            
        }
        
        /* �퉷�������x�ݒ�ł���ꍇ�����܂� */
        
    } else if ((DSPSEQ & 0x07) == 3) {
        /* �����r�����[�h�ݒ�ł���ꍇ */
        if ((ONTRG & 0x03) > 0) {
            /* UP/DOWN�������ꂽ�ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            EDDT[0] ^= 0x80;/* �����r���ݒ���X�V */
            
        } else if ((ONTRG & 0x08) > 0) {
            /* �ݒ�X�C�b�`�ł������ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            SYSFLG &= 0x7F;/* �ŏ�ʃr�b�g���N���A */
            SYSFLG |= (EDDT[0] & 0x80);/* �����r���ݒ�X�V */
            
        }
        
        /* �����r�����[�h�ݒ�ł���ꍇ�����܂� */
        
    } else if ((DSPSEQ & 0x07) == 4) {
        /* ��򃂁[�h�ݒ�ł���ꍇ */
        INDPTM = 0;/* �ʏ�\�����A�^�C�}���N���A���� */
        
        if (DSPSSQ == 1) {
            /* ��򒆂ł͂Ȃ��ꍇ */
            if ((ONTRG & 0x08) > 0) {
                /* �ݒ�X�C�b�`�̃I���g���K�ł������ꍇ */
                ONTRG &= 0xF7;/* �ݒ�X�C�b�`�I���g���K���N���A */
                STSFL0 |= 0x40;/* �N���b�N�����Z�b�g */
                
                if (EDDT[0] == 0) {
                    /* ���N���ł͂Ȃ��ꍇ */
                    STSFL0 &= 0x7F;/* �ݒ胂�[�h���������� */
                    DSPSEQ = DSPSSQ = 0;/* �V�[�P���X���N���A */
                    
                } else if ((SYSFLG & 0x40) > 0) {
                    /* �d�����I���ł���ꍇ */
                    STSFL0 &= 0x7F;/* �ݒ胂�[�h���������� */
                    DSPSEQ = DSPSSQ = 0;/* �V�[�P���X���N���A */
                    
                } else {
                    /* �d�����I�t�ł������ꍇ */
                    SYSFLG |= 0x40;/* �d���t���O���I������ */
                    DSPSSQ = 2;/* ���H���\�����Z�b�g */
                    RPAT |= 0x20;/* ��򃂁[�h�t���O���Z�b�g���� */
                    CCSEQ = 20;/* ���V�[�P���X�̐擪���Z�b�g */
                    WCTM1 = WCTM2 = 0;/* ��򎞂̊e�ٓ���^�C�}���N���A */
                    
                }
                
            } else if ((ONTRG & 0x03) > 0) {
                /* UP/DOWN�̃I���g���K�����m�����ꍇ */
                STSFL0 |= 0x40;/* �N���b�N�����Z�b�g */
                EDDT[0] ^= 0x01;/* �I�����X�V */
                
            }
                
        } else if (DSPSSQ == 2) {
            /* ���H���\���ł���ꍇ */
            if (ONTRG > 0) {
                /* ��������̃X�C�b�`�̃I���g���K�����m�����ꍇ */
                STSFL0 |= 0x40;/* �N���b�N�����Z�b�g */
                ONTRG &= 0xF7;/* �ݒ�X�C�b�`�������I�t�ɂ��� */
                DSPSSQ = 3;/* ���H���������~�I��\���� */
                EDDT[0] = 0;/* �ҏW�G���A�ɐ��p�����Z�b�g */
                
            }
            
        } else {
            /* ����ȊO�̉�ʁi���H���������~�I��\���j�̏ꍇ */
            if ((ONTRG & 0x0B) > 0) {
                /* �ݒ�EUP�EDOWN�X�C�b�`�̂ǂꂩ��������Ă���ꍇ */
                CLNRTM = 0;/* ���H���\�����A�^�C�}���N���A���� */
                
                if ((ONTRG & 0x08) == 0) {
                    /* ���̃X�C�b�`�̓��A�ݒ�X�C�b�`���I�t�̏ꍇ */
                    EDDT[0] ^= 0x01;/* ���] */
                    STSFL0 |= 0x40;/* ����������Z�b�g */
                    
                } else {
                    /* �ݒ�X�C�b�`���I���ł������ꍇ */
                    ONTRG &= 0xF7;/* �ݒ�X�C�b�`���N���A */
                    STSFL0 |= 0x40;/* ����������Z�b�g */
                    
                    if (EDDT[0] == 0) {
                        /* ���p���ł���ꍇ */
                        DSPSSQ = 2;/* ���H���\�����Z�b�g */
                        CLNRTM = 0;/* ���H���\�����A�^�C�}���N���A */
                        
                    } else {
                        /* ���p���ł͂Ȃ��ꍇ */
                        STSFL0 &= 0x7F;/* �ݒ胂�[�h������ */
                        SYSFLG &= 0xBF;/* �d���t���O���I�t */
                        
                        DSPSEQ = DSPSSQ = 0;/* �f�B�X�v���[�V�[�P���X���N���A */
                        
                        RPAT |= 0xC0;/* �r���t���O���Z�b�g */
                        RPAT &= 0xDF;/* ��򃂁[�h������ */
                        
                        WCTM1 = WCTM2 = 0;/* ��򎞂̊e�ٓ���^�C�}���N���A */
                        TSB_PC_DATA_PC2 = 0;/* �u�U�[�I�t */
                        MXOTN2 &= 0x7F;/* �O���x��o�͂��N���A */
                        STSFL0 &= 0xF7;/* �����A���[���o�͒����N���A */
                        
                        REG_00 = TSB_PC->DATA;
                        REG_00 &= 0x07;
                        REG_00 |= (MXOTN2 & 0xF8);
                        TSB_PC->DATA = MXOTN2;/* �o�͍X�V */
                        
                    }
                    
                }
                
            }/* �ݒ�EUP�EDOWN�X�C�b�`�̂ǂꂩ��������Ă��Ȃ��ꍇ�͂����ɔ����� */
            
        }
        
        /* ��򃂁[�h�ݒ�ł���ꍇ�����܂� */
        
    } else if ((DSPSEQ & 0x07) == 5) {
        /* �`���[�\�񎞍��ݒ�ł���ꍇ */
        if ((ONTRG & 0x01) > 0) {
            /* UP�X�C�b�`�������ꂽ�ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            if (DSPSSQ == 1) {
                /* �u���v�̐ݒ�ł���ꍇ */
                EDDT[1]++;/* �C���N�������g */
                if (EDDT[1] > 23) {
                    /* ����𒴂����ꍇ */
                    EDDT[1] = 0;
                    
                }
                
            } else {
                /* �u���v�̐ݒ�ł���ꍇ */
                EDDT[0]++;/* �C���N�������g */
                if (EDDT[0] > 59) {
                    /* ����𒴂����ꍇ */
                    EDDT[0] = 0;
                    
                }
                
            }
            
        } else if ((ONTRG & 0x02) > 0) {
            /* DOWN�X�C�b�`�������ꂽ�ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            if (DSPSSQ == 1) {
                /* �u���v�̐ݒ�ł���ꍇ */
                if (EDDT[1] > 0) {
                    /* �������̏ꍇ */
                    EDDT[1]--;/* �f�N�������g */
                    
                } else {
                    /* �[���������ꍇ */
                    EDDT[1] = 23;/* MAX FIX */
                    
                }
                
            } else {
                /* �u���v�̐ݒ�ł���ꍇ */
                if (EDDT[0] > 0) {
                    /* �������̏ꍇ */
                    EDDT[0]--;/* �f�N�������g */
                    
                } else {
                    /* �[���������ꍇ */
                    EDDT[0] = 59;/* MAX FIX */
                    
                }
                
            }
            
        } else if ((ONTRG & 0x04) > 0) {
            /* ���X�C�b�`�������ꂽ�ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            DSPSSQ++;/* �T�u�V�[�P���X���C���N�������g */
            if (DSPSSQ >= 3) {
                /* ����ȏ�ɂȂ����ꍇ */
                DSPSSQ = 1;/* �����ɂ��� */
                
            }
            
        } else if ((ONTRG & 0x08) > 0) {
            /* �ݒ�X�C�b�`�������ꂽ�ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            SETTM = EDDT[0];/* ���ۑ� */
            SETTH = EDDT[1];/* ���ۑ� */
            
        }
        
        /* �`���[�\�񎞍��ݒ�ł���ꍇ�����܂� */
        
    } else if ((DSPSEQ & 0x07) == 6) {
        /* ���ݎ����ݒ�ł���ꍇ */
        if ((ONTRG & 0x01) > 0) {
            /* UP�X�C�b�`�������ꂽ�ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            if (DSPSSQ == 1) {
                /* �u�N�v�̐ݒ�ł���ꍇ */
                EDDT[4]++;/* �C���N�������g */
                if (EDDT[4] > 99) {
                    /* ����𒴂����ꍇ */
                    EDDT[4] = 0;
                    
                }
                
                if (EDDT[2] > DAYMAX(EDDT[4], EDDT[3])) {
                    /* ���̏���������Ⴄ�ꍇ */
                    EDDT[2] = DAYMAX(EDDT[4], EDDT[3]);/* MAX FIX */
                    
                }
                
            } else if (DSPSSQ == 2) {
                /* �u���v�̐ݒ�ł���ꍇ */
                EDDT[3]++;/* �C���N�������g */
                if (EDDT[3] > 12) {
                    /* ����𒴂����ꍇ */
                    EDDT[3] = 1;
                    
                }
                
                if (EDDT[2] > DAYMAX(EDDT[4], EDDT[3])) {
                    /* ���̏���������Ⴄ�ꍇ */
                    EDDT[2] = DAYMAX(EDDT[4], EDDT[3]);/* MAX FIX */
                    
                }
                
            } else if (DSPSSQ == 3) {
                /* �u���v�̐ݒ�ł���ꍇ */
                EDDT[2]++;/* �C���N�������g */
                if (EDDT[2] > DAYMAX(EDDT[4], EDDT[3])) {
                    /* ����𒴂����ꍇ */
                    EDDT[2] = 1;
                    
                }
                
            } else if (DSPSSQ == 4) {
                /* �u���v�̐ݒ�ł���ꍇ */
                EDDT[1]++;/* �C���N�������g */
                if (EDDT[1] > 23) {
                    /* ����𒴂����ꍇ */
                    EDDT[1] = 0;
                    
                }
                
            } else if (DSPSSQ == 5) {
                /* �u���v�̐ݒ�ł���ꍇ */
                EDDT[0]++;/* �C���N�������g */
                if (EDDT[0] > 59) {
                    /* ����𒴂����ꍇ */
                    EDDT[0] = 0;
                    
                }
                
            }
            
        } else if ((ONTRG & 0x02) > 0) {
            /* DOWN�X�C�b�`�������ꂽ�ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            if (DSPSSQ == 1) {
                /* �u�N�v�̐ݒ�ł���ꍇ */
                if (EDDT[4] > 0) {
                    /* �����ȏ�̏ꍇ */
                    EDDT[4]--;/* �f�N�������g */
                    
                } else {
                    /* �����̏ꍇ */
                    EDDT[4] = 99;/* MAX FIX */
                    
                }
                
                if (EDDT[2] > DAYMAX(EDDT[4], EDDT[3])) {
                    /* ���̏���������Ⴄ�ꍇ */
                    EDDT[2] = DAYMAX(EDDT[4], EDDT[3]);/* MAX FIX */
                    
                }
                
            } else if (DSPSSQ == 2) {
                /* �u���v�̐ݒ�ł���ꍇ */
                if (EDDT[3] > 1) {
                    /* �����ȏ�̏ꍇ */
                    EDDT[3]--;/* �f�N�������g */
                    
                } else {
                    /* �����̏ꍇ */
                    EDDT[3] = 12;/* MAX FIX */
                    
                }
                
                if (EDDT[2] > DAYMAX(EDDT[4], EDDT[3])) {
                    /* ���̏���������Ⴄ�ꍇ */
                    EDDT[2] = DAYMAX(EDDT[4], EDDT[3]);/* MAX FIX */
                    
                }
                
            } else if (DSPSSQ == 3) {
                /* �u���v�̐ݒ�ł���ꍇ */
                if (EDDT[2] > 1) {
                    /* �����ȏ�̏ꍇ */
                    EDDT[2]--;/* �f�N�������g */
                    
                } else {
                    EDDT[2] = DAYMAX(EDDT[4], EDDT[3]);/* MAX FIX */
                    
                }
                
            } else if (DSPSSQ == 4) {
                /* �u���v�̐ݒ�ł���ꍇ */
                if (EDDT[1] > 0) {
                    /* �����ȏ�̏ꍇ */
                    EDDT[1]--;/* �f�N�������g */
                    
                } else {
                    /* �����̏ꍇ */
                    EDDT[1] = 23;/* MAX FIX */
                    
                }
                
            } else if (DSPSSQ == 5) {
                /* �u���v�̐ݒ�ł���ꍇ */
                if (EDDT[0] > 0) {
                    /* �����ȏ�̏ꍇ */
                    EDDT[0]--;/* �f�N�������g */
                    
                } else {
                    /* �����̏ꍇ */
                    EDDT[0] = 59;/* MAX FIX */
                    
                }
                
            }
            
        } else if ((ONTRG & 0x04) > 0) {
            /* ���X�C�b�`�������ꂽ�ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            DSPSSQ++;/* �T�u�V�[�P���X���C���N�������g */
            if (DSPSSQ >= 6) {
                /* ����ȏ�ɂȂ����ꍇ */
                DSPSSQ = 1;/* �����ɂ��� */
                
            }
            
        } else if ((ONTRG & 0x08) > 0) {
            /* �ݒ�X�C�b�`�������ꂽ�ꍇ */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            SETST_BUF[1] = EDDT[0];/* ���ۑ� */
            SETST_BUF[2] = EDDT[1];/* ���ۑ� */
            SETST_BUF[4] = EDDT[2];/* ���ۑ� */
            SETST_BUF[5] = EDDT[3];/* ���ۑ� */
            SETST_BUF[6] = EDDT[4];/* �N�ۑ� */
            SETST_BUF[3] = GETWEEKDAY(EDDT[4], EDDT[3], EDDT[2]);/* �T�̒l�ۑ� */
            
            WR_RTC |= 0x01;/* �����������ݏ���������t���O�����Ă� */
            
        }
        
        /* ���ݎ����ݒ�ł���ꍇ�����܂� */
        
    } else {
        /* �����e�i���X���[�h�ł���ꍇ */
        if ((ONTRG & 0x02) > 0) {
            /* DOWN�X�C�b�`�������ꂽ�ꍇ */
            DABHP = 0;/* �ُ헚���̍ŏ����Z�b�g */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            
            DSPSSQ++;/* �T�u�V�[�P���X�ԍ����C���N�������g */
            
            if ((SYKND0 & 0x10) > 0) {
                /* ���C���ł���ꍇ */
                if ((DSPSSQ == 2) || (DSPSSQ == 5)) {
                    /* �\�m�x��\�����A�ቷ�����x�\���̏ꍇ */
                    DSPSSQ++;/* ��ʕ\�����X�L�b�v���� */
                    
                }
                
            }
            
            if (DSPSSQ >= 8) {
                /* ��ʐ��ȏ�ɂȂ����ꍇ */
                DSPSSQ = 1;/* �߂� */
                
            }
            
        } else if ((ONTRG & 0x01) > 0) {
            /* UP�X�C�b�`�������ꂽ�ꍇ */
            DABHP = 0;/* �ُ헚���̍ŏ����Z�b�g */
            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
            
            if (DSPSSQ > 1) {
                /* �������ł������ꍇ */
                DSPSSQ--;/* �T�u�V�[�P���X�ԍ����f�N�������g */
                
            } else {
                /* �����ł������ꍇ */
                DSPSSQ = 7;
                
            }
            
            if ((SYKND0 & 0x10) > 0) {
                /* ���C���ł���ꍇ */
                if ((DSPSSQ == 2) || (DSPSSQ == 5)) {
                    /* �\�m�x��\�����A�ቷ�����x�\���̏ꍇ */
                    DSPSSQ--;/* ��ʕ\�����X�L�b�v���� */
                    
                }
                
            }
            
            if (DSPSSQ == 0) {
                /* ��ʐ������ɂȂ����ꍇ */
                DSPSSQ = 7;/* �߂� */
                
            }
            
        } else if (DSPSSQ == 1) {
            /* �ُ헚�����[�h�ł���ꍇ */
            if ((ABNCUT & 0x0F) == 0) {
                /* �ُ헚����0���ł���ꍇ */
                DABHP = 0;/* �ُ헚���̍ŏ����Z�b�g���� */
                
            } else if ((ONTRG & 0x04) > 0) {
                /* ���X�C�b�`���I���ł���ꍇ */
                STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
                DABHP++;/* ���̗���ԍ����J�E���g�A�b�v���� */
                
                if (DABHP > (ABNCUT & 0x0F)) {
                    /* �ُ팏���𒴂����ꍇ */
                    DABHP = 1;/* �ŏ��̔ԍ��ɂ��� */
                    
                }
                
            }
            
        } else if (DSPSSQ == 2) {
            /* �\�m�x�񃂁[�h�ł���ꍇ */
            if (((ABNF & 0x20) > 0) && ((ONTRG & 0x04) > 0)) {
                /* �\�m�x��̓o�^������A���X�C�b�`��������Ă����ꍇ */
                STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
                YOTINO++;/* �C���N�������g */
                if (YOTINO >= 2) {
                    /* �\�m�\���̃J�E���^���I�[�o�[���Ă���ꍇ */
                    YOTINO = 0;/* �\�m�\���̃J�E���^���N���A */
                    
                }
                
            }
            
        } else {
            /* �����ȊO�̏ꍇ */
            YOTINO = 0;/* �N���A */
            
            if (DSPSSQ != 7) {
                /* �}�j���A�����[�h�̑���łȂ��ꍇ */
                /* �������Ȃ� */
                
            } else if ((ONTRG & 0x04) > 0) {
                /* ���X�C�b�`��������Ă��āA�`���[���ł���ꍇ */
                if ((SYKND0 & 0x10) == 0) {
                    /* �`���[���ł���ꍇ */
                    STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
                    MMODE++;/* �}�j���A�����[�h�i���o�[���C���N�������g */
                    if (MMODE >= 3) {
                        /* 3�ȏ�ɂȂ�ꍇ */
                        MMODE = 0;/* �������[�h�i���o�[���Z�b�g */
                        
                    }
                    
                }/* ���C���͂Ȃɂ����������ɔ����� */
                
            } else if ((ONTRG & 0x08) > 0) {
                /* �ݒ�X�C�b�`�������ꂽ�ꍇ */
                ONTRG &= 0xF7;/* �ݒ�X�C�b�`�������I�t�ɂ��� */
                
                if ((SYSFLG & 0x40) > 0) {
                    /* �d���I���ł���ꍇ */
                    MMODE = 3;/* �}�j���A�����[�h����G���[���Z�b�g */
                    MMDTM = 0;/* �}�j���A�����[�h�p�^�C�}���N���A */
                    
                } else if (MMODE == 0) {
                    /* �S�Ă̑��̔r���ł���ꍇ */
                    RPAT |= 0xC0;/* �S�Ă̔r���ق��J�ɂ��� */
                    MXOTN0 &= 0x0F;
                    TSB_PG->DATA = MXOTN0;/* �f�[�^���W�X�^�X�V */
                    
                    MMODE = 0;/* �}�j���A�����[�h�i���o�[���N���A */
                    STSFL0 |= 0x40;/* �N���b�N�����Z�b�g */
                    STSFL0 &= 0x7F;/* �ݒ胂�[�h�𔲂��� */
                    INDPTM = 0;/* �ʏ�\�����A�^�C�}���N���A */
                    DSPSEQ = DSPSSQ = 0;/* ��ʃV�[�P���X�𗼕��Ƃ��N���A */
                    
                } else if (MMODE == 1) {
                    /* �퉷�����̔r���ł���ꍇ */
                    RPAT |= 0x40;/* �퉷���r���ق��J�ɂ��� */
                    MXOTN0 &= 0x8F;
                    TSB_PG->DATA = MXOTN0;/* �f�[�^���W�X�^�X�V */
                    
                    MMODE = 0;/* �}�j���A�����[�h�i���o�[���N���A */
                    STSFL0 |= 0x40;/* �N���b�N�����Z�b�g */
                    STSFL0 &= 0x7F;/* �ݒ胂�[�h�𔲂��� */
                    INDPTM = 0;/* �ʏ�\�����A�^�C�}���N���A */
                    DSPSEQ = DSPSSQ = 0;/* ��ʃV�[�P���X�𗼕��Ƃ��N���A */
                    
                } else if (MMODE == 2) {
                    /* �ቷ�����̔r���ł���ꍇ */
                    RPAT |= 0x80;/* �ቷ���r���ق��J�ɂ��� */
                    MXOTN0 &= 0x4F;
                    TSB_PG->DATA = MXOTN0;/* �f�[�^���W�X�^�X�V */
                    
                    MMODE = 0;/* �}�j���A�����[�h�i���o�[���N���A */
                    STSFL0 |= 0x40;/* �N���b�N�����Z�b�g */
                    STSFL0 &= 0x7F;/* �ݒ胂�[�h�𔲂��� */
                    INDPTM = 0;/* �ʏ�\�����A�^�C�}���N���A */
                    DSPSEQ = DSPSSQ = 0;/* ��ʃV�[�P���X�𗼕��Ƃ��N���A */
                    
                }
                
            }
            
        }
        
        /* �����e�i���X���[�h�ł���ꍇ�����܂� */
        
    }
    
    return;/* �߂� */
    
}

/***
 * �薼�F�N�ƌ��������MAX�l��Ԃ�����
 * �쐬�ҁF����@����
 * �N�E������ɁA����MAX�l��ԋp����T�u���[�`��
***/
uint8_t DAYMAX(uint8_t YEAR, uint8_t MONTH)
{
    uint8_t  DAY = 0;
    uint16_t Y_16 = 0;
    
    if (MONTH == 2) {
        /* 2���ł���ꍇ */
        Y_16 = (uint16_t)YEAR;/* �N���� */
        DAY = (28 + (1 / (Y_16 % 4 + 1)) * (1 - 1 / (Y_16 % 100 + 1)) + (1 / (((Y_16 + 2000) % 400) + 1)));
        
    } else if ((MONTH == 4) || (MONTH == 6) || (MONTH == 9) || (MONTH == 11)) {
        /* 4���A6���A9���A11���ł���ꍇ */
        DAY = 30;/* 30���Z�b�g */
        
    } else {
        DAY = 31;
        
    }
    
    return DAY;
}