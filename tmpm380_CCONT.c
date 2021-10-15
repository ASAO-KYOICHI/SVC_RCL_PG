/**
 *�薼�FSVC�̗�p�H���A�v��
 *�쐬�ҁF����@����
 *SVC�̗�p��Ƃ��s���A�v���P�[�V����
**/

#include "tmpm380_CCONT.h"

void CCONT(void)
{
    OS_TSKCon1 *OS_LoadTime;/* OS�ݒ�l��ǂނ��߂̃|�C���^�\���̂ɖ��O������ */
    OS_LoadTime = OSTSKCon1S();/* �֐��o�R�Ń|�C���^��OSparam�ɃA�N�Z�X */
    
    uint32_t REG_00, REG_01 = 0;/* �v�Z�p */
    uint16_t REG_02 = 0;
    
    if ( ((STSFL0 & 0x02) == 0) || (((STSFL0 & 0x02) > 0) && (TNO > 0)) ) {
        /* ���������������A��������������ýĒ��ł���ꍇ */
        OS_LoadTime->NEXT_LOAD_Time[3] = 100;/* ����Ăяo������ */
        OS_LoadTime->TSK_COND[3] = 0xA0;/* �x���t���O���z */
        
        return;
        
    }
    
    if (((MXOTN0 & 0x01) > 0) && ((MXINO0 & 0x10) > 0)) {
        /* �퉷�^��ق�ON�ŁA�퉷�^��ٕ��~�b�g��ON�ł���ꍇ */
        CCTM3++;/* �^��ق̊Ď��^�C�}���C���N�������g */
        if (CCTM3 >= 250) {
            /* 25�b�o�߂����ꍇ */
            CCTM3 = 0;/* �^�C�}�N���A */
            if (((ABNF & 0x0F) == 0)
            || (((SYSFLG & 0x02) > 0) && ((ABNF & 0x0F) == 8))
            || (((SYSFLG & 0x02) > 0) && ((ABNF & 0x0F) == 9))
            || (((SYSFLG & 0x02) > 0) && ((ABNF & 0x0F) == 14))) {
                /* �ُ픭���Ȃ����A�^�]���ŏ퉷���������ʓd�ɖ_�ُ�ł���A
                �܂��͉^�]���ŏ퉷�����x�Z���T�[�ُ킩�A�^�]���Ő^��x�ُ�ł���ꍇ */
                ABNF &= 0x30;/* MASK */
                ABNF |= 0x86;/* �u�U�[�t���O�Z�b�g�A�d���وُ�Z�b�g */
                
            }
            
        }
        
    } else {
        /* �퉷�^��ق�OFF���A�퉷�^��ٕ��~�b�g��OFF�ł���ꍇ */
        CCTM3 = 0;/* �Ď��^�C�}�N���A */
        
    }
    
    if ( ((SYSFLG & 0x02) == 0)
    || (((SYSFLG & 0x02) > 0) && ((ABNF & 0x0F) > 0) && ((ABNF & 0x0F) != 8) && ((ABNF & 0x0F) != 9)
        && ((ABNF & 0x0F) != 14)) ) {
        /* �^�]��~���ł���A���͉^�]���ŏ퉷�����ʓd�ɖ_�ُ�ł͂��A�퉷�����x�Z���T�[�ُ�ł͂Ȃ��A�^��x�ُ�ł͂Ȃ��ꍇ */
        STSFL1 &= 0x7F;/* �^��������̃A���[���v��FLAG�����Z�b�g */
        STSFL0 &= 0xF7;/* �����A���[���o�͒��t���O�����Z�b�g */
        
        if ((RPAT & 0x20) > 0) {
            /* ��򃂁[�h�ł���ꍇ */
            CCLEAN();
            
        } else {
            /* ��򃂁[�h�łȂ��ꍇ */
            RPAT &= 0xDF;/* ��򃂁[�h�t���O������OFF */
            CCSEQ = 0;/* ��p�V�[�P���X���N���A */
            CCTM0 = CCTM1 = CCTM2 = CCTM4 = 0;/* ��p�V�[�P���X�^�C�}���N���A */
            CSEQTS = 0;/* ��p���Ԃ��N���A */
            CSEQTM = 0;/* ��p���Ԃ��N���A */
            
            if ((SYSFLG & 0x40) == 0) {
                /* �d��OFF�ł���ꍇ */
                CCTM3 = 0;/* �퉷�^��يĎ��p�^�C�}���N���A */
                MXOTN0 &= 0xF0;/* �ቷ�퉷�G�W�F�N�^�\P�I�t�A�ቷ�퉷�^��ق��I�t */
                MXOTN1 &= 0x80;/* �^��j��فA����فA�ቷ�퉷�G�W�F�N�^�\�ق��I�t */
                MXOTN2 &= 0xAF;/* �M���z��P�ƔM��r���ق��I�t */
                
            } else {
                /* �d��ON�ł���ꍇ */
                MXOTN0 &= 0xF1;/* �ቷ�퉷�G�W�F�N�^�\P�I�t�A�ቷ�^��ق��I�t */
                MXOTN0 |= 0x01;/* �퉷�^��ق��I���ɂ��� */
                MXOTN1 &= 0x82;/* �^��j��فA����فA�ቷ�퉷�G�W�F�N�^�\�ق��I�t */
                MXOTN1 |= 0x02;/* ��p���r���فi�\���j���I�� */
                MXOTN2 &= 0xAF;/* �M���z��P�ƔM��r���ق��I�t */
                
            }
            
        }
        
    } else {
        /* �^�]ON�ł���ꍇ */
        /* �^�]���쒆 */
        if ((STSFL0 & 0x20) == 0) {
            /* �u�₩��̏������ł���ꍇ */
            OS_LoadTime->NEXT_LOAD_Time[3] = 0;/* ����Ăяo������0ms */
            OS_LoadTime->TSK_COND[3] = 0x80;/* �x���t���O���z�͂��̂܂� */
            
            return;
            
        }
        
        if (CCTM0 >= 10) {
            /* 1�b�o�߂����ꍇ */
            CCTM0 -= 10;/* 1�b�����Z���� */
            CSEQTS++;/* �V�[�P���X�^�C�}�[���C���N�������g */
            
            if (CSEQTS >= 60) {
                /* 60�b�o�߂����ꍇ */
                CSEQTS = 0;/* �b���N���A���� */
                CSEQTM++;/* +1������ */
                
                if (CCSEQ != 15) {
                    /* ��p�����ҋ@�łȂ��ꍇ */
                    RTIMM++;/* �ݔ��ғ�����+1�� */
                    if (RTIMM >= 60) {
                        /* 60���o�߂����ꍇ */
                        RTIMM = 0;/* �N���A */
                        REG_00 = (uint32_t)RTIMH1;
                        REG_00 |= ((uint32_t)RTIMH2 << 16);
                        REG_00++;/* �C���N�������g */
                        
                        if (REG_00 >= 60000) {
                            /* 60000���Ԉȏ�ɂȂ�ꍇ */
                            REG_00 = 60000;/* MAX FIX */
                            
                        }
                        RTIMH1 = (uint16_t)REG_00;/* ���ʃZ�b�g */
                        RTIMH2 = (uint16_t)(REG_00 >> 16);/* ��ʃZ�b�g */
                        
                    }
                    
                }/* ��p�����ҋ@�ł���ꍇ�͂����ɔ����� */
                
                if (CSEQTM >= 200) {
                    /* 200���ȏ�ɂȂ�ꍇ */
                    CSEQTM = 200;/* MAX FIX */
                    
                }
                
            }/* 60�b�o�߂��Ă��Ȃ��ꍇ�͂����ɔ����� */
            
        }/* 1�b�o�߂��Ă��Ȃ��ꍇ�͂����ɔ����� */
        
        if (CCSEQ == 0) {
            /* �^�]�J�n����ł���ꍇ */
            MXOTN0 &= 0xF5;/* �ቷ�G�W�F�N�^�\P�A�ቷ�^��ق��I�t�ɂ��� */
            MXOTN0 |= 0x05;/* �퉷�G�W�F�N�^�\P�A�퉷�^��ق��I���ɂ��� */
            MXOTN1 &= 0xB6;/* ���C�فA�ቷ�G�W�F�N�^�ق��I�t�ɂ��� */
            MXOTN1 |= 0x36;/* �^��j��فA����فA�퉷�G�W�F�N�^�فA��p���r���ق��I���ɂ��� */
            
            DSPNO = 3;/* ��ʔԍ�3�ɂ��� */
            DSPSEQ = 0;/* �N���A�f�B�X�v���[�V�[�P���X */
            
            SYSFLG &= 0xE3;/* �H���t���O�N���A */
            SYSFLG |= 0x04;/* ����H�����Z�b�g */
            CCSEQ = 1;/* ����V�[�P���X�ɂ��� */
            
            REG_00 = 1;/* �����l�N���A */
            REG_00 <<= NPAT;/* �p�^�[���������V�t�g */
            if (((uint8_t)REG_00 & RPAT) == 0) {
                /* ����Ȃ��ł���ꍇ */
                MXOTN1 &= 0xAF;/* ���C�فA����ق��I�t */
                SYSFLG &= 0xFB;/* ����s�������Z�b�g */
                SYSFLG |= 0x08;/* ��p�H�����Z�b�g */
                CCSEQ = 2;/* ��p�V�[�P���X���Z�b�g */
                
            }
            
        } else if (CCSEQ == 1) {
            /* ����V�[�P���X�ł���ꍇ */
            MXOTN0 &= 0xF0;/* ��U�N���A */
            MXOTN0 |= 0x05;/* �퉷�G�W�F�N�^P�Ə퉷�^��ق�ON�ɂ��� */
            MXOTN1 &= 0x80;/* ��U�N���A */
            MXOTN1 |= 0x36;/* �^��j��فA����فA�퉷�G�W�F�N�^�فA��p���r���ق�ON�ɂ��� */
            
            if (NPAT == 0) {
                /* �p�^�[��1�ł���ꍇ */
                REG_00 = (uint32_t)SCTM1;
                
            } else if (NPAT == 1) {
                /* �p�^�[��2�ł���ꍇ */
                REG_00 = (uint32_t)SCTM2;
                
            } else if (NPAT == 2) {
                /* �p�^�[��3�ł���ꍇ */
                REG_00 = (uint32_t)SCTM3;
                
            } else {
                /* �p�^�[��4�ł���ꍇ */
                REG_00 = (uint32_t)SCTM4;
                
            }
            
            if (CSEQTM >= (uint8_t)REG_00) {
                /* ���⎞�Ԍo�߂����ꍇ */
                STSFL0 |= 0x40;/* �N���b�N�t���O���Z�b�g */
                MXOTN1 &= 0xAF;/* ���C�فA����ق��I�t�ɂ��� */
                SYSFLG &= 0xFB;/* ����s�������Z�b�g���� */
                SYSFLG |= 0x08;/* ��p�H���ɂ��� */
                
                if ((STSFL0 & 0x80) == 0) {
                    /* �ݒ蒆�łȂ��ꍇ */
                    DSPSEQ = 0;/* ��ʃV�[�P���X���N���A���� */
                    
                }
                
            }
            
        } else if (CCSEQ < 10) {
            /* �^������H�������ł���ꍇ */
            REG_00 = 1;/* �����l�N���A */
            REG_00 <<= NPAT;/* �p�^�[���������V�t�g */
            if (((uint8_t)REG_00 & RPAT) == 0) {
                /* ����Ȃ��ł���ꍇ */
                REG_00 = 0;/* �N���A */
                
            } else {
                /* ����L��ł���ꍇ */
                if (NPAT == 0) {
                    /* �p�^�[��1�ł���ꍇ */
                    REG_00 = (uint32_t)SCTM1;/* ���⎞��1�Z�b�g */
                    
                } else if (NPAT == 1) {
                    /* �p�^�[��2�ł���ꍇ */
                    REG_00 = (uint32_t)SCTM2;/* ���⎞��2�Z�b�g */
                    
                } else if (NPAT == 2) {
                    /* �p�^�[��3�ł���ꍇ */
                    REG_00 = (uint32_t)SCTM3;/* ���⎞��3�Z�b�g */
                    
                } else {
                    /* �p�^�[��4�ł���ꍇ */
                    REG_00 = (uint32_t)SCTM4;/* ���⎞��4�Z�b�g */
                    
                }
                
            }
            
            if ((CSEQTM >= (uint8_t)(REG_00 + 5)) && (CSEQTS == 0)
            && ((MXINO0 & 0x40) == 0) && ((ABNF & 0x0F) == 0)) {
                /* �H���ɓ����ăW���X�g5���ł���A�^�󈳗͂̓��͂������A�ُ킪�������Ă��Ȃ��ꍇ */
                ABNF &= 0x30;/* �}�X�N�Z�b�g */
                ABNF |= 14;/* �^��x�ُ���Z�b�g */
                ABNF |= 0x40;/* �^�]�p���ُ̈�Z�b�g */
                
            }
            
            if (CSEQTM >= (uint8_t)(REG_00 + 2)) {
                /* 2���o�߂����ꍇ�i2�������͕i���Z���T�s�����ԁj */
                if (NPAT == 0) {
                    /* �p�^�[��1�ł���ꍇ */
                    REG_01 = (uint32_t)PAT1;/* �ݒ艷�x1�Z�b�g */
                    
                } else if (NPAT == 1) {
                    /* �p�^�[��2�ł���ꍇ */
                    REG_01 = (uint32_t)PAT2;/* �ݒ艷�x2�Z�b�g */
                    
                } else if (NPAT == 2) {
                    /* �p�^�[��3�ł���ꍇ */
                    REG_01 = (uint32_t)PAT3;/* �ݒ艷�x3�Z�b�g */
                    
                } else {
                    /* �p�^�[��4�ł���ꍇ */
                    REG_01 = (uint32_t)PAT4;/* �ݒ艷�x4�Z�b�g */
                    
                }
                
                if (TMPDT0 <= (short)REG_01) {
                    /* �i�����ݒ艷�x�ȉ��ɂȂ����ꍇ */
                    if ((CCSEQ == 3) || (CCSEQ >= 7)) {
                        /* �H�����퉷����p���ł���ꍇ�A���͒ቷ����p�ɐ؂�ւ���ł���ꍇ */
                        if (((SYKND0 & 0x10) == 0) && (CCSEQ < 4)) {
                            /* �`���[���ŁA�퉷����p�H�����������Ă��Ȃ��ꍇ */
                            if (TMPDT2 <= (short)SWTMP0) {
                                /* �ݒ艷�x�ȉ��ɂȂ��Ă���ꍇ */
                                SYSFLG |= 0x01;/* �퉷�r�����Z�b�g */
                                
                            }
                            
                        }
                        
                        DSPNO = 4;/* ��p�����\�����Z�b�g���� */
                        CCSEQ = 10;/* �^������H�����Z�b�g */
                        SYSFLG &= 0xE3;/* �}�X�N */
                        SYSFLG |= 0x10;/* ��p�������Z�b�g */
                        
                        if ((STSFL0 & 0x80) == 0) {
                            /* �ݒ蒆�ł͖����ꍇ */
                            DSPSEQ = 0;/* �\���V�[�P���X���N���A */
                            
                        }
                        
                        REG_01 = (uint32_t)RTCDT[1];/* �������Z�b�g */
                        REG_02 = (uint16_t)REG_01;/* �R�s�[ */
                        REG_01 /= 10;/* 4�r�b�g�V�t�g */
                        REG_01 <<= 4;/* 10�̈ʃZ�b�g */
                        REG_02 %= 10;/* 1�̈ʃZ�b�g */
                        REG_01 |= REG_02;/* �g�ݍ��킹��BCD��� */
                        ETIME1 = REG_01;/* ��p���������i���j�Z�b�g */
                        
                        REG_01 = (uint32_t)RTCDT[2];/* �������Z�b�g */
                        REG_02 = (uint16_t)REG_01;/* �R�s�[ */
                        REG_01 /= 10;/* 4�r�b�g�V�t�g */
                        REG_01 <<= 4;/* 10�̈ʃZ�b�g */
                        REG_02 %= 10;/* 1�̈ʃZ�b�g */
                        REG_01 |= REG_02;/* �g�ݍ��킹��BCD��� */
                        ETIME2 = REG_01;/* ��p���������i���j�Z�b�g */
                        
                    }
                    
                }/* �i�����ݒ艷�x���ł���ꍇ�͂����ɔ����� */
                
            }/* �i���Z���T�̕s�����Ԃ��o�߂��Ă��Ȃ��ꍇ�͂����ɔ����� */
            
            if (CCSEQ == 2) {
                /* �퉷��p�V�[�P���X�ł���ꍇ */
                /* �퉷���ɂ���p�H�� */
                MXOTN0 &= 0xF0;/* ��������MASK */
                MXOTN0 |= 0x05;/* �퉷�G�W�F�N�^P�A�퉷�^��ق��I�� */
                MXOTN1 &= 0x80;/* ��UMASK */
                MXOTN1 |= 0x26;/* �^��j��فA�퉷�G�W�F�N�^�فA��p���r���ق��I�� */
                
                if (CSEQTM >= (uint8_t)(REG_00 + 1)) {
                    /* 1���o�߂����ꍇ */
                    CCSEQ = 3;/* �M�����틋���N��������Z�b�g */
                    
                }
                
            } else if (CCSEQ == 3) {
                /* �M�����틋���N������̏ꍇ */
                MXOTN0 &= 0xF0;/* MASK */
                MXOTN0 |= 0x05;/* �퉷�G�W�F�N�^P�A�퉷�^��ق��I�� */
                MXOTN1 &= 0x80;/* ��UMASK */
                MXOTN1 |= 0x26;/* �^��j��فA�퉷�G�W�F�N�^�فA��p���r���ق��I�� */
                MXOTN2 &= 0xAF;/* MASK */
                MXOTN2 |= 0x10;/* �M��r���ق�ON */
                
                REG_02 = 0;/* ��U�N���A */
                
                if ((SYKND0 & 0x10) > 0) {
                    /* ���C���ł���ꍇ */
                    REG_02 = 5;/* ���C�G�W�F�N�^�[�̋N���x�����ԕ���5�����Z�b�g */
                    
                } else if ((SYKND0 & 0xC0) == 0x40) {
                    /* 50R�ł���ꍇ */
                    REG_02 = 4;/* 2�����؂�ւ��x�����Ԃ�4�����Z�b�g */
                    
                } else if ((SYKND0 & 0xC0) == 0x80) {
                    /* 100R�ł���ꍇ */
                    if ((SYKND0 & 0x20) > 0) {
                        /* 15���d�l�̏ꍇ */
                        REG_02 = 5;/* 15���d�l�p��5���Z�b�g */
                        
                    } else {
                        /* 10���d�l�̏ꍇ */
                        REG_02 = 3;/* 10����2�����؂�ւ��x�����Ԃ�3�����Z�b�g */
                        
                    }
                    
                } else {
                    /* 35R�ł���ꍇ */
                    if ((SYKND0 & 0x20) > 0) {
                        /* 15���d�l�̏ꍇ */
                        REG_02 = 4;/* 15���d�l�p��4���Z�b�g */
                        
                    } else {
                        /* 10���d�l�̏ꍇ */
                        REG_02 = 9;/* 10����2�����؂�ւ��x�����Ԃ�9�����Z�b�g */
                        
                    }
                    
                }
                
                if (CSEQTM >= (uint8_t)(REG_00 + (uint32_t)REG_02)) {
                    /* 2�����؂�ւ��x�����Ԃ��I�������ꍇ */
                    MXOTN0 &= 0xFE;/* �퉷�^��ق��I�t�ɂ��� */
                    CCTM1 = 0;/* �퉷�^��ٕ��~�b�g���m�^�C�}���N���A���� */
                    CCSEQ = 4;/* �퉷�^��ٕ��~�b�g���m�H�����Z�b�g���� */
                    
                }
                
            } else if (CCSEQ == 4) {
                /* �퉷�^��ٕ��~�b�g���m�H�� */
                MXOTN0 &= 0xF0;/* MASK */
                MXOTN0 |= 0x04;/* �퉷�G�W�F�N�^P���I�� */
                MXOTN1 &= 0x80;/* ��UMASK */
                MXOTN1 |= 0x26;/* �^��j��فA�퉷�G�W�F�N�^�فA��p���r���ق��I�� */
                MXOTN2 &= 0xAF;/* MASK */
                MXOTN2 |= 0x10;/* �M��r���ق�ON */
                
                if ((MXINO0 & 0x10) == 0) {
                    /* �퉷�^��ٕ��~�b�g�����m���Ă��Ȃ��ꍇ */
                    CCTM1++;/* �^�C�}���C���N�������g */
                    if (CCTM1 >= 250) {
                        /* 25�b�o�߂����ꍇ */
                        ABNF &= 0x30;/* MASK */
                        ABNF |= 0x86;/* �u�U�[�t���O�Ɠd���وُ���Z�b�g���� */
                        
                    }
                    
                } else {
                    /* �퉷�^��ق̕��~�b�g�����m�����ꍇ */
                    if ((SYKND0 & 0x10) == 0) {
                        /* �`���[���ł���ꍇ */
                        MXOTN0 &= 0xF0;/* MASK */
                        MXOTN1 &= 0x80;/* ��UMASK */
                        MXOTN1 |= 0x2A;/* �^��j��فA�ቷ�G�W�F�N�^�فA��p���r���ق��I�� */
                        MXOTN2 &= 0xAF;/* MASK */
                        MXOTN2 |= 0x10;/* �M��r���ق�ON */
                        
                        CCTM1 = 0;/* 2�b�x���^�C�}���N���A */
                        CCSEQ = 5;/* 2�b�x���H�����Z�b�g���� */
                        
                        if (TMPDT2 > (short)SWTMP0) {
                            /* �퉷���������ݒ艷�x���ł���ꍇ */
                            SYSFLG |= 0x01;/* �퉷�r���H���v���t���O�Z�b�g */
                            
                        }
                        
                    } else {
                        /* ���C���ł���ꍇ */
                        MXOTN0 &= 0xF0;/* MASK */
                        MXOTN0 |= 0x04;/* �퉷�G�W�F�N�^P���I�� */
                        MXOTN1 &= 0xC0;/* ��UMASK */
                        MXOTN1 |= 0x6E;/* ���C�فA�^��j��فA���C��2�i�ቷ�G�W�F�N�^�فj�A�퉷�G�W�F�N�^�فA��p���r���ق��I�� */
                        MXOTN2 &= 0xAF;/* MASK */
                        MXOTN2 |= 0x10;/* �M��r���ق�ON */
                        
                        CCSEQ = 8;/* ��p�ŏI�H�� */
                    }
                    
                }
                
            } else if (CCSEQ == 5) {
                /* 2�b�x���H���ł���ꍇ */
                MXOTN0 &= 0xF0;/* MASK */
                MXOTN1 &= 0x80;/* ��UMASK */
                MXOTN1 |= 0x2A;/* �^��j��فA�ቷ�G�W�F�N�^�فA��p���r���ق��I�� */
                MXOTN2 &= 0xAF;/* MASK */
                MXOTN2 |= 0x10;/* �M��r���ق�ON */
                
                CCTM1++;/* �^�C�}�[�C���N�������g */
                if (CCTM1 >= 20) {
                    /* 2�b�o�߂����ꍇ */
                    MXOTN0 &= 0xF0;/* MASK */
                    MXOTN0 |= 0x08;/* �ቷ�G�W�F�N�^�\P��ON */
                    MXOTN1 &= 0x80;/* ��UMASK */
                    MXOTN1 |= 0x2A;/* �^��j��فA�ቷ�G�W�F�N�^�فA��p���r���ق��I�� */
                    MXOTN2 &= 0xAF;/* MASK */
                    MXOTN2 |= 0x10;/* �M��r���ق�ON */
                    
                    CCTM1 = 0;/* 2�b�x���^�C�}���N���A */
                    CCSEQ = 6;/* �^��يJ�܂ł̒x���H�����Z�b�g */
                    
                }
                
            } else if (CCSEQ == 6) {
                /* �^��يJ�܂ł̒x���H���ł���ꍇ */
                MXOTN0 &= 0xF0;/* MASK */
                MXOTN0 |= 0x08;/* �ቷ�G�W�F�N�^�\P��ON */
                MXOTN1 &= 0x80;/* ��UMASK */
                MXOTN1 |= 0x2A;/* �^��j��فA�ቷ�G�W�F�N�^�فA��p���r���ق��I�� */
                MXOTN2 &= 0xAF;/* MASK */
                MXOTN2 |= 0x10;/* �M��r���ق�ON */
                
                CCTM1++;/* �^�C�}�[���C���N�������g */
                if (CCTM1 >= 40) {
                    /* 4�b�o�߂����ꍇ */
                    MXOTN0 &= 0xF0;/* MASK */
                    MXOTN0 |= 0x0A;/* �ቷ�G�W�F�N�^�\P�A�ቷ�^��ق�ON */
                    MXOTN1 &= 0x80;/* ��UMASK */
                    MXOTN1 |= 0x2A;/* �^��j��فA�ቷ�G�W�F�N�^�فA��p���r���ق��I�� */
                    MXOTN2 &= 0xAF;/* MASK */
                    MXOTN2 |= 0x10;/* �M��r���ق�ON */
                    
                    CCTM2 = 0;/* �M���z�A��̐���^�C�}���N���A */
                    CCSEQ = 7;/* �M���z�A��̐���H���Z�b�g */
                    
                }
                
            } else if ((CCSEQ == 7) && ((SYSFLG & 0x10) == 0)) {
                /* �M���z�A��̐���H���i��p�������j�ł���ꍇ */
                MXOTN0 &= 0xF0;/* MASK */
                MXOTN0 |= 0x0A;/* �ቷ�G�W�F�N�^�\P�A�ቷ�^��ق�ON */
                MXOTN1 &= 0x80;/* ��UMASK */
                MXOTN1 |= 0x2A;/* �^��j��فA�ቷ�G�W�F�N�^�فA��p���r���ق��I�� */
                MXOTN2 &= 0xAF;/* MASK */
                MXOTN2 |= 0x10;/* �M��r���ق�ON */
                
                CCTM2++;/* �^�C�}�C���N�������g */
                if (CCTM2 >= 600) {
                    /* 60�b�o�߂����ꍇ */
                    MXOTN0 &= 0xF0;/* MASK */
                    MXOTN0 |= 0x0A;/* �ቷ�G�W�F�N�^�\P�A�ቷ�^��ق�ON */
                    MXOTN1 &= 0x80;/* ��UMASK */
                    MXOTN1 |= 0x2A;/* �^��j��فA�ቷ�G�W�F�N�^�فA��p���r���ق��I�� */
                    MXOTN2 &= 0xAF;/* MASK */
                    MXOTN2 |= 0x40;/* �M���z�A���ON */
                    
                    CCSEQ = 8;/* ��p�ŏI�H�����Z�b�g */
                    CCTM1 = 0;/* ��p�H���^�C�}1�N���A */
                    CCTM2 = 0;/* ��p�H���^�C�}2�N���A */
                    
                }
                
            } else if (((CCSEQ == 7) && ((SYSFLG & 0x10) > 0)) || (CCSEQ == 8)) {
                /* �M���z�A��̍H��(��p�����j���͗�p�ŏI�H���ł���ꍇ */
                if (CCSEQ == 7) {
                    /* 7�̂܂܂ł������ꍇ */
                    CCSEQ = 8;/* 8�ɍX�V */
                    
                }
                
                if ((SYKND0 & 0x40) > 0) {
                    /* ���C���ł���ꍇ */
                    MXOTN0 &= 0xF0;/* MASK */
                    MXOTN0 |= 0x04;/* �퉷�G�W�F�N�^�\P��ON */
                    MXOTN1 &= 0x80;/* ��UMASK */
                    MXOTN1 |= 0x6E;/* ���C�فA�^��j��فA�ቷ�G�W�F�N�^��(���C��2)�A�퉷�G�W�F�N�^�فA��p���r���ق��I�� */
                    MXOTN2 &= 0xAF;/* MASK */
                    MXOTN2 |= 0x10;/* �M��r���ق�ON */
                    
                } else {
                    /* �`���[���ł���ꍇ */
                    MXOTN0 &= 0xF0;/* MASK */
                    MXOTN0 |= 0x0A;/* �ቷ�G�W�F�N�^�\P�A�ቷ�^��ق�ON */
                    MXOTN1 &= 0x80;/* ��UMASK */
                    MXOTN1 |= 0x2A;/* �^��j��فA�ቷ�G�W�F�N�^�فA��p���r���ق��I�� */
                    MXOTN2 &= 0xAF;/* MASK */
                    MXOTN2 |= 0x40;/* �M���z�A���ON */
                    
                }
                
                REG_02 = 0;/* ��U�N���A */
                REG_02 = (uint16_t)(CSEQTM - (uint8_t)REG_00);/* ���݂̍H�����ԁi���j - ���⎞�Ԃ����߂� */
                /* ����������A����܂ł̍H�����Ԃ�����ŋ��߂� */
                
                if ((SYKND0 & 0xC0) == 0xC0) {
                    /* 40R�ł���ꍇ */
                    if (REG_02 >= 50) {
                        /* ��p���Ԃ�50���o�߂��Ă���ꍇ */
                        ABNF &= 0x30;
                        ABNF |= 0x84;/* �u�U�[�t���OON�A��p�ُ�ɂ��� */
                        
                    }
                    
                } else {
                    /* 50�`100R�A35R�̏ꍇ */
                    if (REG_02 >= 30) {
                        /* ��p���Ԃ�30���o�߂��Ă���ꍇ */
                        ABNF &= 0x30;
                        ABNF |= 0x84;/* �u�U�[�t���OON�A��p�ُ�ɂ��� */
                        
                    }
                    
                }
                
            }
            /* �^������H�������ł���ꍇ�@�����܂� */
            
        } else if (CCSEQ == 10) {
            /* �^������H������̍H���ł���ꍇ */
            if ((STSFL1 & 0x40) == 0) {
                /* �^������H�����̃A���[���o�͂��֎~�ł���ꍇ */
                STSFL1 |= 0x80;/* �^��������̃A���[���v���t���O�Z�b�g */
                
            }
            
            STSFL1 &= 0xBF;/* �A���[���o�͋֎~�t���O���N���A */
            CCTM5 = 0;/* �^��������̃A���[���o�͗p�^�C�}���N���A���� */
            
            if ((SYKND0 & 0x10) > 0) {
                /* ���C���ł���ꍇ */
                MXOTN0 &= 0xF4;/* �퉷�E�ቷ�^��ق��N���A */
                MXOTN1 &= 0x82;/* �N�[�����O�^���[�^�]�M���E��p���r���وȊO�N���A */
                MXOTN2 &= 0xAF;/* �M��r���A�M���z���N���A */
                
            } else {
                /* �`���[���ł���ꍇ */
                MXOTN0 &= 0xFC;/* �퉷�E�ቷ�^��ق��N���A */
                MXOTN1 &= 0xAE;/* ���C�فA����ق��N���A */
                MXOTN2 &= 0xAF;/* �M��r���A�M���z���N���A */
                
            }
            
            CCSEQ = 11;/* �^��j��ِ���J�n�܂ł̒x���H�����Z�b�g */
            CCTM1 = 0;/* �^�C�}�N���A */
            CCTM2 = 0;/* �^�C�}�N���A */
            
            if ((STSFL0 & 0x80) == 0) {
                /* �ݒ蒆�ł͂Ȃ��ꍇ */
                DSPSEQ = 0;/* ��ʕ\���V�[�P���X���N���A */
                
            }
            
            ABNF &= 0xBF;/* �p���ُ�̌x��A���[���o�͂����Z�b�g */
            if ((STSFL1 & 0x80) > 0) {
                /* �^��������̃A���[���o�͗v��������ꍇ */
                CCTM5++;/* �C���N�������g */
                if ((CCTM5 >= 6) || ((CCTM5 & 0x01) == 0)) {
                    /* 0.6�b�ȏォ�A�����ł���ꍇ */
                    STSFL0 &= 0xF7;/* �A���[���o�͒��t���O���I�t�ɂ��� */
                    TSB_PC_DATA_PC2 = 0;/* �u�U�[�N���A */
                    MXOTN2 &= 0x7F;/* �O���x��o�͂��N���A */
                    
                } else {
                    /* 0.6�b��������ł���ꍇ */
                    STSFL0 |= 0x08;/* �A���[���o�͒��t���O���I���ɂ��� */
                    TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                    MXOTN2 |= 0x80;/* �O���x��o�͂��I�� */
                    
                }
                
                if (CCTM5 >= 30) {
                    /* 3�b�ȏ�ɂȂ����ꍇ */
                    CCTM5 = 0;/* �N���A���� */
                    
                }
                
            }
            
        } else if (CCSEQ == 11) {
            /* �^��j��H���܂ł̒x���H���ł���ꍇ */
            CCTM2++;/* �^�C�}�C���N�������g */
            CCTM1++;/* �^�C�}�C���N�������g */
            
            if ((SYKND0 & 0x10) > 0) {
                /* ���C���ł���ꍇ */
                if (CCTM1 >= 100) {
                    /* 10�b�ȏ�ɂȂ����ꍇ */
                    MXOTN0 &= 0xF0;/* �퉷�G�W�F�N�^�|���v��OFF�ɂ��� */
                    CCSEQ = 13;/* �퉷�^��يJ�x���H�����Z�b�g */
                    CCTM1 = 0;/* �^�C�}�N���A */
                    
                }
                
            } else {
                /* �`���[���ł���ꍇ */
                if (CCTM1 >= 150) {
                    /* 15�b�ȏ�ɂȂ����ꍇ */
                    MXOTN1 &= 0x82;/* ���C�فA�^��j��فA����فA�퉷�E�ቷ�G�W�F�N�^�ق�OFF�ɂ��� */
                    CCSEQ = 12;/* �|���v��~�x���H�����Z�b�g */
                    
                }
                
            }
            
            ABNF &= 0xBF;/* �p���ُ�̌x��A���[���o�͂����Z�b�g */
            if ((STSFL1 & 0x80) > 0) {
                /* �^��������̃A���[���o�͗v��������ꍇ */
                CCTM5++;/* �C���N�������g */
                if ((CCTM5 >= 6) || ((CCTM5 & 0x01) == 0)) {
                    /* 0.6�b�ȏォ�A�����ł���ꍇ */
                    STSFL0 &= 0xF7;/* �A���[���o�͒��t���O���I�t�ɂ��� */
                    TSB_PC_DATA_PC2 = 0;/* �u�U�[�N���A */
                    MXOTN2 &= 0x7F;/* �O���x��o�͂��N���A */
                    
                } else {
                    /* 0.6�b��������ł���ꍇ */
                    STSFL0 |= 0x08;/* �A���[���o�͒��t���O���I���ɂ��� */
                    TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                    MXOTN2 |= 0x80;/* �O���x��o�͂��I�� */
                    
                }
                
                if (CCTM5 >= 30) {
                    /* 3�b�ȏ�ɂȂ����ꍇ */
                    CCTM5 = 0;/* �N���A���� */
                    
                }
                
            }
            
        } else if (CCSEQ == 12) {
            /* �|���v��~�x���H���ł���ꍇ */
            CCTM2++;/* �^�C�}�C���N�������g */
            CCTM1++;/* �^�C�}�C���N�������g */
            
            if (CCTM1 >= 200) {
                /* 20�b�o�߂����ꍇ */
                MXOTN0 &= 0xF0;/* �퉷�E�ቷ�G�W�F�N�^P�A�퉷�E�ቷ�^��ق��I�t�ɂ��� */
                CCSEQ = 13;/* �^��ِ؂�ւ��x���H�����Z�b�g */
                CCTM1 = 0;/* �^�C�}�N���A */
                
            }
            
            ABNF &= 0xBF;/* �p���ُ�̌x��A���[���o�͂����Z�b�g */
            if ((STSFL1 & 0x80) > 0) {
                /* �^��������̃A���[���o�͗v��������ꍇ */
                CCTM5++;/* �C���N�������g */
                if ((CCTM5 >= 6) || ((CCTM5 & 0x01) == 0)) {
                    /* 0.6�b�ȏォ�A�����ł���ꍇ */
                    STSFL0 &= 0xF7;/* �A���[���o�͒��t���O���I�t�ɂ��� */
                    TSB_PC_DATA_PC2 = 0;/* �u�U�[�N���A */
                    MXOTN2 &= 0x7F;/* �O���x��o�͂��N���A */
                    
                } else {
                    /* 0.6�b��������ł���ꍇ */
                    STSFL0 |= 0x08;/* �A���[���o�͒��t���O���I���ɂ��� */
                    TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                    MXOTN2 |= 0x80;/* �O���x��o�͂��I�� */
                    
                }
                
                if (CCTM5 >= 30) {
                    /* 3�b�ȏ�ɂȂ����ꍇ */
                    CCTM5 = 0;/* �N���A���� */
                    
                }
                
            }
            
        } else if (CCSEQ == 13) {
            /* �^��ِ؂�ւ��x���H���ł���ꍇ */
            CCTM2++;/* �^�C�}�C���N�������g */
            CCTM1++;/* �^�C�}�C���N�������g */
            
            if (CCTM1 >= 200) {
                /* 20�b�o�߂����ꍇ */
                CCTM1 = 0;/* �^�C�}�[���N���A */
                CCSEQ = 14;/* �^������x���H�����Z�b�g���� */
                MXOTN0 |= 0x01;/* �퉷�^��ق�ON���� */
                
            }
            
            ABNF &= 0xBF;/* �p���ُ�̌x��A���[���o�͂����Z�b�g */
            if ((STSFL1 & 0x80) > 0) {
                /* �^��������̃A���[���o�͗v��������ꍇ */
                CCTM5++;/* �C���N�������g */
                if ((CCTM5 >= 6) || ((CCTM5 & 0x01) == 0)) {
                    /* 0.6�b�ȏォ�A�����ł���ꍇ */
                    STSFL0 &= 0xF7;/* �A���[���o�͒��t���O���I�t�ɂ��� */
                    TSB_PC_DATA_PC2 = 0;/* �u�U�[�N���A */
                    MXOTN2 &= 0x7F;/* �O���x��o�͂��N���A */
                    
                } else {
                    /* 0.6�b��������ł���ꍇ */
                    STSFL0 |= 0x08;/* �A���[���o�͒��t���O���I���ɂ��� */
                    TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                    MXOTN2 |= 0x80;/* �O���x��o�͂��I�� */
                    
                }
                
                if (CCTM5 >= 30) {
                    /* 3�b�ȏ�ɂȂ����ꍇ */
                    CCTM5 = 0;/* �N���A���� */
                    
                }
                
            }
            
        } else if (CCSEQ == 14) {
            /* �^������x���H���ł���ꍇ */
            CCTM2++;/* �C���N�������g */
            
            if (CCTM2 >= 700) {
                /* 70�b�o�߂����ꍇ */
                CCTM2 = 0;/* �^�C�}�[�N���A */
                STSFL1 &= 0x7F;/* �^��������̃A���[���o�͗v���t���O���N���A */
                CCSEQ = 15;/* �����ҋ@�H�����Z�b�g���� */
                CCTM1 = 0;/* �^�C�}�N���A */
                
                if ((STSFL0 & 0x80) == 0) {
                    /* �ݒ胂�[�h�ł͂Ȃ��ꍇ */
                    DSPSEQ = 0;/* �f�B�X�v���[�V�[�P���X���N���A */
                    
                }
                
                ABNF &= 0xBF;/* �p���ُ�̌x��A���[���o�͂����Z�b�g */
                CCTM1++;/* �^�C�}�C���N�������g */
                if (CCTM1 < 2) {
                    /* �^�C�}��0.2�b�����ł���ꍇ */
                    STSFL0 |= 0x08;/* �A���[���o�͒��t���O���I���ɂ��� */
                    TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                    MXOTN2 |= 0x80;/* �O���x��o�͂��I�� */
                    
                } else {
                    /* �^�C�}��0.2�b�ȏ�ł���ꍇ */
                    STSFL0 &= 0xF7;/* �A���[���o�͒��t���O���I�t�ɂ��� */
                    TSB_PC_DATA_PC2 = 0;/* �u�U�[�N���A */
                    MXOTN2 &= 0x7F;/* �O���x��o�͂��N���A */
                    
                    if (CCTM1 >= 3) {
                        /* 0.3�b�ȏ�ł���ꍇ */
                        CCTM1 = 0;/* �N���A���� */
                        
                    }
                    
                }
                
            } else {
                /* 70�b�o�߂��Ă��Ȃ��ꍇ */
                ABNF &= 0xBF;/* �p���ُ�̌x��A���[���o�͂����Z�b�g */
                if ((STSFL1 & 0x80) > 0) {
                    /* �^��������̃A���[���o�͗v��������ꍇ */
                    CCTM5++;/* �C���N�������g */
                    if ((CCTM5 >= 6) || ((CCTM5 & 0x01) == 0)) {
                        /* 0.6�b�ȏォ�A�����ł���ꍇ */
                        STSFL0 &= 0xF7;/* �A���[���o�͒��t���O���I�t�ɂ��� */
                        TSB_PC_DATA_PC2 = 0;/* �u�U�[�N���A */
                        MXOTN2 &= 0x7F;/* �O���x��o�͂��N���A */
                        
                    } else {
                        /* 0.6�b��������ł���ꍇ */
                        STSFL0 |= 0x08;/* �A���[���o�͒��t���O���I���ɂ��� */
                        TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                        MXOTN2 |= 0x80;/* �O���x��o�͂��I�� */
                        
                    }
                    
                    if (CCTM5 >= 30) {
                        /* 3�b�ȏ�ɂȂ����ꍇ */
                        CCTM5 = 0;/* �N���A���� */
                        
                    }
                    
                }
                
            }
            
        } else {
            /* �p���ُ�̌x��A���[���o�� */
            ABNF &= 0xBF;/* �p���ُ�̌x��A���[���o�͂����Z�b�g */
            CCTM1++;/* �^�C�}�C���N�������g */
            if (CCTM1 < 2) {
                /* �^�C�}��0.2�b�����ł���ꍇ */
                STSFL0 |= 0x08;/* �A���[���o�͒��t���O���I���ɂ��� */
                TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                MXOTN2 |= 0x80;/* �O���x��o�͂��I�� */
                
            } else {
                /* �^�C�}��0.2�b�ȏ�ł���ꍇ */
                STSFL0 &= 0xF7;/* �A���[���o�͒��t���O���I�t�ɂ��� */
                TSB_PC_DATA_PC2 = 0;/* �u�U�[�N���A */
                MXOTN2 &= 0x7F;/* �O���x��o�͂��N���A */
                
                if (CCTM1 >= 3) {
                    /* 0.3�b�ȏ�ł���ꍇ */
                    CCTM1 = 0;/* �N���A���� */
                    
                }
                
            }
            
        }
        
    }
    
    /* �A�v��������O�̏��� */
    if ((STSFL0 & 0x04) > 0) {
        /* �u�╜�A����v���t���O���I���ł���ꍇ */
        CCTM4++;/* �^�C�}�C���N�������g */
        if (CCTM4 >= 30) {
            /* 3�b�o�߂����ꍇ */
            CCTM4 = 0;/* �^�C�}�N���A */
            STSFL0 &= 0xFB;/* �u�╜�A����v���t���O���I�t�ɂ��� */
            
        } else {
            /* 3�b�����ł���ꍇ */
            MXOTN1 &= 0xDF;/* �^��j��ق��N���A */
            
        }
        
    } else {
        /* �u�╜�A����v���t���O���I�t�ł���ꍇ */
        CCTM4 = 0;/* �^�C�}�N���A */
        STSFL0 &= 0xFB;/* �u�╜�A����v���t���O���I�t�ɂ��� */
        
    }
    
    TSB_PG->DATA = MXOTN0;/* �o�̓Z�b�g */
    
    REG_00 = TSB_PN->DATA;/* �f�[�^�����[�h */
    REG_00 &= 0x01;/* 0�r�b�g�ڂ̂݃}�X�N���� */
    REG_00 |= (MXOTN1 & 0xFE);/* �K�v�ȃf�[�^���Z�b�g */
    TSB_PN->DATA = REG_00;/* �o�̓Z�b�g */
    
    REG_00 = TSB_PC->DATA;/* �f�[�^�����[�h */
    REG_00 &= 0x07;/* 0�`3�r�b�g�ڂ̂݃}�X�N���� */
    REG_00 |= (MXOTN2 & 0xF8);/* �K�v�ȃf�[�^���Z�b�g */
    TSB_PC->DATA = REG_00;/* �o�̓Z�b�g */
    
    OS_LoadTime->NEXT_LOAD_Time[3] = 100;/* ����Ăяo������ */
    OS_LoadTime->TSK_COND[3] = 0xA0;/* �x���t���O���z */
    
    return;/* �߂� */
}

/**
 *�薼�FSVC�̐��H���A�v��
 *�쐬�ҁF����@����
 *SVC�̐���Ƃ��s���A�v���P�[�V����
**/
void CCLEAN(void)
{
    uint8_t REG_00 = 0;/* �v�Z�p�ꎞ�ϐ� */
    
    if (CCSEQ >= 20) {
        /* ���H���ԍ�������ł���ꍇ */
        if (CCTM0 >= 10) {
            /* 1�b�o�߂��Ă���ꍇ */
            CCTM0 -= 10;/* 1�b���Z */
            CSEQTS++;/* �b���C���N�������g */
            
            if (CSEQTS >= 60) {
                /* 60�b�o�߂����ꍇ */
                CSEQTS = 0;/* �b���N���A */
                CSEQTM++;/* �����C���N�������g */
                
                if (CSEQTM >= 200) {
                    /* 200���o�߂����ꍇ */
                    RPAT &= 0xDF;/* ��򃂁[�h�t���O������OFF */
                    CCSEQ = 0;/* ��p�V�[�P���X���N���A */
                    CCTM0 = CCTM1 = CCTM2 = CCTM4 = 0;/* ��p�V�[�P���X�^�C�}���N���A */
                    CSEQTS = 0;/* ��p���Ԃ��N���A */
                    CSEQTM = 0;/* ��p���Ԃ��N���A */
                    
                    if ((SYSFLG & 0x40) == 0) {
                        /* �d��OFF�ł���ꍇ */
                        CCTM3 = 0;/* �퉷�^��يĎ��p�^�C�}���N���A */
                        MXOTN0 &= 0xF0;/* �ቷ�퉷�G�W�F�N�^�\P�I�t�A�ቷ�퉷�^��ق��I�t */
                        MXOTN1 &= 0x80;/* �^��j��فA����فA�ቷ�퉷�G�W�F�N�^�\�ق��I�t */
                        MXOTN2 &= 0xAF;/* �M���z��P�ƔM��r���ق��I�t */
                        
                    } else {
                        /* �d��ON�ł���ꍇ */
                        MXOTN0 &= 0xF1;/* �ቷ�퉷�G�W�F�N�^�\P�I�t�A�ቷ�^��ق��I�t */
                        MXOTN0 |= 0x01;/* �퉷�^��ق��I���ɂ��� */
                        MXOTN1 &= 0x82;/* �^��j��فA����فA�ቷ�퉷�G�W�F�N�^�\�ق��I�t */
                        MXOTN1 |= 0x02;/* ��p���r���فi�\���j���I�� */
                        MXOTN2 &= 0xAF;/* �M���z��P�ƔM��r���ق��I�t */
                        
                    }
                    
                    return;/* �����Ŗ߂� */
                    
                }/* 200�������ł���ꍇ�͂����ɔ����� */
                
            }/* 1���o�߂��Ă��Ȃ��ꍇ�͂����ɔ����� */
            
        }/* 1�b�o�߂��Ă��Ȃ��ꍇ�͂����ɔ����� */
        
        MXOTN0 &= 0xF0;/* �ቷ�E�퉷�G�W�F�N�^P�A�ቷ�^��ق��I�t */
        MXOTN1 &= 0x80;/* �^��j��فA����فA�ቷ�E�퉷�G�W�F�N�^�ق��I�t */
        MXOTN2 &= 0xAF;/* �M���z�A�M��r���ق��I�t */
        
        if ((ABNF & 0x0F) > 0) {
            /* �ُ킪�������Ă���ꍇ */
            return;/* �߂� */
            
        }
        
        REG_00 = CCSEQ;/* �V�[�P���X�ԍ����Z�b�g */
        REG_00 -= 20;/* 20�����Z */
        REG_00 &= 0x07;/* �}�X�N */
        
        if ((SYKND0 & 0x10) > 0) {
            /* ���C���ł���ꍇ */
            REG_00 += 8;
            
        }
        
    } else {
        /* ���H���ԍ����ُ�ȏꍇ */
        RPAT &= 0xDF;/* ��򃂁[�h�t���O������OFF */
        CCSEQ = 0;/* ��p�V�[�P���X���N���A */
        CCTM0 = CCTM1 = CCTM2 = CCTM4 = 0;/* ��p�V�[�P���X�^�C�}���N���A */
        CSEQTS = 0;/* ��p���Ԃ��N���A */
        CSEQTM = 0;/* ��p���Ԃ��N���A */
        
        if ((SYSFLG & 0x40) == 0) {
            /* �d��OFF�ł���ꍇ */
            CCTM3 = 0;/* �퉷�^��يĎ��p�^�C�}���N���A */
            MXOTN0 &= 0xF0;/* �ቷ�퉷�G�W�F�N�^�\P�I�t�A�ቷ�퉷�^��ق��I�t */
            MXOTN1 &= 0x80;/* �^��j��فA����فA�ቷ�퉷�G�W�F�N�^�\�ق��I�t */
            MXOTN2 &= 0xAF;/* �M���z��P�ƔM��r���ق��I�t */
            
        } else {
            /* �d��ON�ł���ꍇ */
            MXOTN0 &= 0xF1;/* �ቷ�퉷�G�W�F�N�^�\P�I�t�A�ቷ�^��ق��I�t */
            MXOTN0 |= 0x01;/* �퉷�^��ق��I���ɂ��� */
            MXOTN1 &= 0x82;/* �^��j��فA����فA�ቷ�퉷�G�W�F�N�^�\�ق��I�t */
            MXOTN1 |= 0x02;/* ��p���r���فi�\���j���I�� */
            MXOTN2 &= 0xAF;/* �M���z��P�ƔM��r���ق��I�t */
            
        }
        
        return;/* �����Ŗ߂� */
        
    }
    
    /* REG_00 0�`7�c�`���[���̐��H�� */
    /* REG_00 8�`15�c���C���̐��H�� */
    if (REG_00 == 0) {
        /* ���J�n�H�� */
        CCTM0 = 0;/* ���H���^�C�}�N���A */
        CSEQTS = 0;/* �N���A */
        CSEQTM = 0;/* �N���A */
        
        if ((WLOLD & 0x03) == 0x03) {
            /* �퉷�����ʂ������ʂ܂ł���ꍇ */
            CCSEQ = 21;/* �퉷�����H���Z�b�g���� */
            WCTM1 = WCTM2 = 0;/* ��򎞂̃^�C�}�N���A */
            
        }
        
    } else if (REG_00 == 1) {
        /* �퉷�����H�� */
        MXOTN0 |= 0x04;/* �퉷�G�W�F�N�^�\P���I�� */
        MXOTN1 |= 0x04;/* �퉷�G�W�F�N�^�ق��I�� */
        
        if (CSEQTM >= 9) {
            /* �M��r���x�����Ԃ�9���o�߂����ꍇ */
            MXOTN2 |= 0x10;/* �M��r���ق��I�� */
            
            if (CSEQTM >= 10) {
                /* �M��r���x���H�����Ԃ�10���o�߂����ꍇ */
                MXOTN0 &= 0xF0;/* �}�X�N */
                MXOTN1 &= 0x80;/* �}�X�N */
                MXOTN2 &= 0xAF;/* �}�X�N */
                
                if ((WLOLD & 0x0C) == 0x0C) {
                    /* �ቷ�����ʂ������ʂ܂ł���ꍇ */
                    CCTM0 = 0;/* �^�C�}�N���A */
                    CSEQTS = 0;
                    CSEQTM = 0;
                    
                    CCSEQ = 22;/* �ቷ���̐��H���Z�b�g */
                    WCTM1 = 0;/* ��򎞂̓d���ٓ���^�C�}���N���A */
                    
                }
                
            }
            
        }
        
    } else if (REG_00 == 2) {
        /* �ቷ�����H�� */
        MXOTN0 |= 0x08;/* �ቷ�G�W�F�N�^�[P�I�� */
        MXOTN1 |= 0x08;/* �ቷ�G�W�F�N�^�ق��I�� */
        MXOTN2 |= 0x40;/* �M���z���I�� */
        
        if ((CSEQTM == 0) && (CSEQTS < 2)) {
            /* �J�n2�b�����̏ꍇ */
            MXOTN0 &= 0xF0;/* �ቷ�G�W�F�N�^P���I�t */
            return;/* �����Ŗ߂� */
            
        } else if (CSEQTM < 9) {
            /* 9���o�߂��Ă��Ȃ��ꍇ */
            return;/* �߂� */
            
        }
        
        MXOTN2 &= 0xBF;/* �M���z���I�t�ɂ��� */
        MXOTN2 |= 0x10;/* �M��r���ق��I���ɂ��� */
        
        if (CSEQTM < 10) {
            /* 10���o�߂��Ă��Ȃ��ꍇ */
            return;/* �߂� */
            
        }
        
        MXOTN0 &= 0xF0;/* �o�͂��}�X�N */
        MXOTN1 &= 0x80;/* �o�͂��}�X�N */
        
        if ((WLOLD & 0x03) == 0x03) {
            /* �퉷�������̐��ʂ������ʂ܂ł���ꍇ */
            CCTM0 = 0;/* �^�C�}�N���A */
            CSEQTS = 0;
            CSEQTM = 0;
            
            CCSEQ = 23;/* �퉷���̂������H��1�Z�b�g */
            WCTM1 = WCTM2 = 0;/* ��򎞂̓d���ٓ���^�C�}�ƁA��򎞂̃G�A�����ٓ���^�C�}���N���A */
            
        }
        
    } else if (REG_00 == 3) {
        /* �퉷��������1�H�� */
        MXOTN0 |= 0x04;/* �퉷�G�W�F�N�^P���Z�b�g */
        MXOTN1 |= 0x04;/* �퉷�G�W�F�N�^�ق��Z�b�g */
        MXOTN2 |= 0x10;/* �M��r���ق��Z�b�g */
        
        if ((CSEQTM == 0) && (CSEQTS < 2)) {
            /* �J�n2�b�����̏ꍇ */
            MXOTN0 &= 0xF0;/* �ቷ�G�W�F�N�^P���I�t */
            return;/* �����Ŗ߂� */
            
        } else if (CSEQTM < 10) {
            /* 10���o�߂��Ă��Ȃ��ꍇ */
            return;/* �߂� */
            
        }
        
        MXOTN0 &= 0xF0;/* �o�͂��}�X�N */
        MXOTN1 &= 0x80;/* �o�͂��}�X�N */
        
        if ((WLOLD & 0x0C) == 0x0C) {
            /* �ቷ�������̐��ʂ������ʂ܂ł���ꍇ */
            CCTM0 = 0;/* �^�C�}�N���A */
            CSEQTS = 0;
            CSEQTM = 0;
            
            CCSEQ = 24;/* �ቷ���̂������H��1�Z�b�g */
            WCTM1 = 0;/* ��򎞂̓d���ٓ���^�C�}���N���A */
            
        }
        
    } else if (REG_00 == 4) {
        /* �ቷ��������1�H�� */
        MXOTN0 |= 0x08;/* �ቷ�G�W�F�N�^P���Z�b�g */
        MXOTN1 |= 0x08;/* �ቷ�G�W�F�N�^�ق��Z�b�g */
        MXOTN2 |= 0x50;/* �M���z�A�M��r���ق��Z�b�g */
        
        if ((CSEQTM == 0) && (CSEQTS < 2)) {
            /* �J�n2�b�����̏ꍇ */
            MXOTN0 &= 0xF0;/* �ቷ�G�W�F�N�^P���I�t */
            return;/* �����Ŗ߂� */
            
        } else if (CSEQTM < 10) {
            /* 10���o�߂��Ă��Ȃ��ꍇ */
            return;/* �߂� */
            
        }
        
        MXOTN0 &= 0xF0;/* �o�͂��}�X�N */
        MXOTN1 &= 0x80;/* �o�͂��}�X�N */
        
        if ((WLOLD & 0x03) == 0x03) {
            /* �퉷�������̐��ʂ������ʂ܂ł���ꍇ */
            CCTM0 = 0;/* �^�C�}�N���A */
            CSEQTS = 0;
            CSEQTM = 0;
            
            CCSEQ = 25;/* �퉷���̂������H��2�Z�b�g */
            WCTM1 = WCTM2 = 0;/* ��򎞂̓d���ٓ���^�C�}�ƁA��򎞂̃G�A�����ٓ���^�C�}���N���A */
            
        }
        
    } else if (REG_00 == 5) {
        /* �퉷��������2�H�� */
        MXOTN0 |= 0x04;/* �퉷�G�W�F�N�^P���Z�b�g */
        MXOTN1 |= 0x04;/* �퉷�G�W�F�N�^�ق��Z�b�g */
        MXOTN2 |= 0x10;/* �M��r���ق��Z�b�g */
        
        if ((CSEQTM == 0) && (CSEQTS < 2)) {
            /* �J�n2�b�����̏ꍇ */
            MXOTN0 &= 0xF0;/* �ቷ�G�W�F�N�^P���I�t */
            return;/* �����Ŗ߂� */
            
        } else if (CSEQTM < 10) {
            /* 10���o�߂��Ă��Ȃ��ꍇ */
            return;/* �߂� */
            
        }
        
        MXOTN0 &= 0xF0;/* �o�͂��}�X�N */
        MXOTN1 &= 0x80;/* �o�͂��}�X�N */
        
        if ((WLOLD & 0x0C) == 0x0C) {
            /* �ቷ�������̐��ʂ������ʂ܂ł���ꍇ */
            CCTM0 = 0;/* �^�C�}�N���A */
            CSEQTS = 0;
            CSEQTM = 0;
            
            CCSEQ = 26;/* �ቷ���̂������H��2�Z�b�g */
            WCTM1 = 0;/* ��򎞂̓d���ٓ���^�C�}���N���A */
            
        }
        
    } else if (REG_00 == 6) {
        /* �ቷ��������2�H�� */
        MXOTN0 |= 0x08;/* �ቷ�G�W�F�N�^P���Z�b�g */
        MXOTN1 |= 0x08;/* �ቷ�G�W�F�N�^�ق��Z�b�g */
        MXOTN2 |= 0x50;/* �M���z�A�M��r���ق��Z�b�g */
        
        if ((CSEQTM == 0) && (CSEQTS < 2)) {
            /* �J�n2�b�����̏ꍇ */
            MXOTN0 &= 0xF0;/* �ቷ�G�W�F�N�^P���I�t */
            return;/* �����Ŗ߂� */
            
        } else if (CSEQTM < 10) {
            /* 10���o�߂��Ă��Ȃ��ꍇ */
            return;/* �߂� */
            
        }
        
        MXOTN0 &= 0xF0;/* �o�͂��}�X�N */
        MXOTN1 &= 0x80;/* �o�͂��}�X�N */
        
        CCTM0 = 0;/* �^�C�}�N���A */
        CCTM5 = 0;
        CSEQTS = 0;
        CSEQTM = 0;
        
        CCSEQ = 27;/* ���H���I���Z�b�g */
        WCTM1 = WCTM2 = 0;/* ��򎞂̓d���ٓ���^�C�}�ƁA��򎞂̃G�A�����ٓ���^�C�}���N���A */
        
    } else if (REG_00 == 7) {
        /* ���H�������ł���ꍇ */
        if (CSEQTM < 5) {
            /* �r�����삪5���o�߂��Ă��Ȃ��ꍇ */
            return;/* �߂� */
            
        }
        
        CCTM5++;/* �C���N�������g */
        if (CCTM5 >= 12) {
            /* �����A���[���o�͏I���ł���ꍇ */
            if (CCTM5 < 30) {
                /* �����\����3�b�����ł���ꍇ */
                TSB_PC_DATA_PC2 = 0;/* �u�U�[�I�t */
                MXOTN2 &= 0x7F;/* �u�U�[�O���o�͂��N���A */
                STSFL0 &= 0xF7;/* �����A���[���o�͒����N���A */
                
            } else {
                /* �����\����3�b�ȏ�ł���ꍇ */
                SYSFLG &= 0xBF;/* �d���t���O���I�t */
                STSFL0 &= 0x7F;/* �ݒ胂�[�h������ */
                DSPSEQ = 0;/* �\���V�[�P���X�N���A */
                DSPSSQ = 0;/* �\���T�u�V�[�P���X�N���A */
                RPAT &= 0xDF;/* ��򃂁[�h�t���O�����Z�b�g */
                RPAT |= 0xC0;/* �r���t���O���Z�b�g */
                CCSEQ = 0;/* ���H���N���A */
                CSEQTS = 0;/* �b�J�E���^�N���A */
                CSEQTM = 0;/* ���J�E���^�N���A */
                WCTM1 = WCTM2 = 0;/* ��򎞂̓d���ٓ���^�C�}�ƁA��򎞂̃G�A�����ٓ���^�C�}���N���A */
                
            }
            
        } else {
            /* �����A���[���o�͏I���ł͂Ȃ��ꍇ */
            if ((CCTM5 & 0x01) == 0) {
                /* �u�U�[�I���̃^�C�~���O�ł���ꍇ */
                TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                MXOTN2 &= 0x80;/* �u�U�[�O���o�̓I�� */
                STSFL0 &= 0x08;/* �����A���[���o�͒����I�� */
                
            } else {
                /* �u�U�[�I�t�̃^�C�~���O�ł���ꍇ */
                TSB_PC_DATA_PC2 = 0;/* �u�U�[�I�t */
                MXOTN2 &= 0x7F;/* �u�U�[�O���o�͂��N���A */
                STSFL0 &= 0xF7;/* �����A���[���o�͒����N���A */
                
            }
            
        }
        
    } else if (REG_00 == 8) {
        /* ���J�n�H��(���C��) */
        CCTM0 = 0;/* ���H���^�C�}�N���A */
        CSEQTS = 0;/* �N���A */
        CSEQTM = 0;/* �N���A */
        
        if ((WLOLD & 0x03) == 0x03) {
            /* �퉷�����ʂ������ʂ܂ł���ꍇ */
            CCSEQ = 21;/* �퉷�����H���Z�b�g���� */
            WCTM1 = WCTM2 = 0;/* ��򎞂̃^�C�}�N���A */
            
        }
        
    } else if (REG_00 == 9) {
        /* �퉷�����H��(���C��) */
        MXOTN0 |= 0x04;/* �퉷�G�W�F�N�^�\P���I�� */
        MXOTN1 |= 0x04;/* �퉷�G�W�F�N�^�ق��I�� */
        
        if (CSEQTM >= 8) {
            /* �M��r���x�����Ԃ�8���o�߂����ꍇ */
            if ((SYKND0 & 0xC0) != 0x80) {
                /* 100R�ȊO�ł���ꍇ */
                MXOTN2 |= 0x10;/* �M��r���ق��I�� */
                
            }
            
            if (CSEQTM >= 10) {
                /* �M��r���x���H�����Ԃ�10���o�߂����ꍇ */
                MXOTN0 &= 0xF0;/* �}�X�N */
                MXOTN1 &= 0x80;/* �}�X�N */
                
                CCTM0 = 0;/* �^�C�}�N���A */
                CSEQTS = 0;
                CSEQTM = 0;
                
                CCSEQ = 22;/* �퉷���̔r���H���Z�b�g */
                WCTM1 = 0;/* ��򎞂̓d���ٓ���^�C�}���N���A */
                
            }
            
        }
        
    } else if (REG_00 == 10) {
        /* �퉷���r���H��(���C��) */
        if ((SYKND0 & 0xC0) != 0x80) {
            /* 100R�ȊO�ł���ꍇ */
            MXOTN2 |= 0x10;/* �M��r���ق��I�� */
            
        }
        
        if (CSEQTM < 5) {
            /* 5���o�߂��Ă��Ȃ��ꍇ */
            return;/* �߂� */
            
        }
        
        if ((WLOLD & 0x03) == 0x03) {
            /* �퉷�������̐��ʂ������ʂ܂ł���ꍇ */
            CCTM0 = 0;/* �^�C�}�N���A */
            CSEQTS = 0;
            CSEQTM = 0;
            
            CCSEQ = 23;/* �퉷���̂������H��1�Z�b�g */
            WCTM1 = WCTM2 = 0;/* ��򎞂̓d���ٓ���^�C�}�ƁA��򎞂̃G�A�����ٓ���^�C�}���N���A */
            
        }
        
    } else if (REG_00 == 11) {
        /* �퉷��������1�H��(���C��) */
        MXOTN0 |= 0x04;/* �퉷�G�W�F�N�^P���Z�b�g */
        MXOTN1 |= 0x04;/* �퉷�G�W�F�N�^�ق��Z�b�g */
        if ((SYKND0 & 0xC0) != 0x80) {
            /* 100R�ȊO�ł���ꍇ */
            MXOTN2 |= 0x10;/* �M��r���ق��Z�b�g */
            
        }
        
        if (CSEQTM < 10) {
            /* 10���o�߂��Ă��Ȃ��ꍇ */
            return;/* �߂� */
            
        }
        
        MXOTN0 &= 0xF0;/* �o�͂��}�X�N */
        MXOTN1 &= 0x80;/* �o�͂��}�X�N */
        
        CCTM0 = 0;/* �^�C�}�N���A */
        CSEQTS = 0;
        CSEQTM = 0;
        
        CCSEQ = 24;/* �퉷���̔r���H���Z�b�g */
        WCTM1 = 0;/* ��򎞂̓d���ٓ���^�C�}���N���A */
        
    } else if (REG_00 == 12) {
        /* �퉷���r���H��2(���C��) */
        if ((SYKND0 & 0xC0) != 0x80) {
            /* 100R�ȊO�ł���ꍇ */
            MXOTN2 |= 0x10;/* �M��r���ق��I�� */
            
        }
        
        if (CSEQTM < 5) {
            /* 5���o�߂��Ă��Ȃ��ꍇ */
            return;/* �߂� */
            
        }
        
        if ((WLOLD & 0x03) == 0x03) {
            /* �퉷�������̐��ʂ������ʂ܂ł���ꍇ */
            CCTM0 = 0;/* �^�C�}�N���A */
            CSEQTS = 0;
            CSEQTM = 0;
            
            CCSEQ = 25;/* �퉷���̂������H��2�Z�b�g */
            WCTM1 = WCTM2 = 0;/* ��򎞂̓d���ٓ���^�C�}�ƁA��򎞂̃G�A�����ٓ���^�C�}���N���A */
            
        }
        
    } else if (REG_00 == 13) {
        /* �퉷��������2�H��(���C��) */
        MXOTN0 |= 0x04;/* �퉷�G�W�F�N�^P���Z�b�g */
        MXOTN1 |= 0x04;/* �퉷�G�W�F�N�^�ق��Z�b�g */
        if ((SYKND0 & 0xC0) != 0x80) {
            /* 100R�ȊO�ł���ꍇ */
            MXOTN2 |= 0x10;/* �M��r���ق��Z�b�g */
            
        }
        
        if (CSEQTM < 10) {
            /* 10���o�߂��Ă��Ȃ��ꍇ */
            return;/* �߂� */
            
        }
        
        MXOTN0 &= 0xF0;/* �o�͂��}�X�N */
        MXOTN1 &= 0x80;/* �o�͂��}�X�N */
        
        CCTM0 = 0;/* �^�C�}�N���A */
        CSEQTS = 0;
        CSEQTM = 0;
        
        CCSEQ = 26;/* �r���H���Z�b�g */
        WCTM1 = 0;/* ��򎞂̓d���ٓ���^�C�}���N���A */
        
    } else if (REG_00 == 14) {
        /* �r���H�� */
        CCTM0 = 0;/* �^�C�}�N���A */
        CCTM5 = 0;
        CSEQTS = 0;
        CSEQTM = 0;
        
        CCSEQ = 27;/* ���H���I�����Z�b�g */
        WCTM1 = WCTM2 = 0;/* ��򎞂̓d���ٓ���^�C�}�ƁA��򎞂̃G�A�����ٓ���^�C�}���N���A */
        
    } else if (REG_00 == 15) {
        /* ���H�������ł���ꍇ */
        if (CSEQTM < 5) {
            /* �r�����삪5���o�߂��Ă��Ȃ��ꍇ */
            return;/* �߂� */
            
        }
        
        CCTM5++;/* �C���N�������g */
        if (CCTM5 >= 12) {
            /* �����A���[���o�͏I���ł���ꍇ */
            if (CCTM5 < 30) {
                /* �����\����3�b�����ł���ꍇ */
                TSB_PC_DATA_PC2 = 0;/* �u�U�[�I�t */
                MXOTN2 &= 0x7F;/* �u�U�[�O���o�͂��N���A */
                STSFL0 &= 0xF7;/* �����A���[���o�͒����N���A */
                
            } else {
                /* �����\����3�b�ȏ�ł���ꍇ */
                SYSFLG &= 0xBF;/* �d���t���O���I�t */
                STSFL0 &= 0x7F;/* �ݒ胂�[�h������ */
                DSPSEQ = 0;/* �\���V�[�P���X�N���A */
                DSPSSQ = 0;/* �\���T�u�V�[�P���X�N���A */
                RPAT &= 0xDF;/* ��򃂁[�h�t���O�����Z�b�g */
                RPAT |= 0xC0;/* �r���t���O���Z�b�g */
                CCSEQ = 0;/* ���H���N���A */
                CSEQTS = 0;/* �b�J�E���^�N���A */
                CSEQTM = 0;/* ���J�E���^�N���A */
                WCTM1 = WCTM2 = 0;/* ��򎞂̓d���ٓ���^�C�}�ƁA��򎞂̃G�A�����ٓ���^�C�}���N���A */
                
            }
            
        } else {
            /* �����A���[���o�͏I���ł͂Ȃ��ꍇ */
            if ((CCTM5 & 0x01) == 0) {
                /* �u�U�[�I���̃^�C�~���O�ł���ꍇ */
                TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                MXOTN2 &= 0x80;/* �u�U�[�O���o�̓I�� */
                STSFL0 &= 0x08;/* �����A���[���o�͒����I�� */
                
            } else {
                /* �u�U�[�I�t�̃^�C�~���O�ł���ꍇ */
                TSB_PC_DATA_PC2 = 0;/* �u�U�[�I�t */
                MXOTN2 &= 0x7F;/* �u�U�[�O���o�͂��N���A */
                STSFL0 &= 0xF7;/* �����A���[���o�͒����N���A */
                
            }
            
        }
        
    }
    
    return;
    
}
