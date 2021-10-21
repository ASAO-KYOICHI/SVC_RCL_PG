/**
 *�薼�F�^�C�}�[���荞�ݏ���
 *�쐬�ҁF����@����
 *1ms���ƂɌĂяo����鏈��
**/
#include "TIMER00.h"

void INTTB00_IRQHandler(void)
{
    OS_TSKCon1 *OS_LoadTime;/* OS�ݒ�l��ǂނ��߂̃|�C���^�\���̂ɖ��O������ */
    OS_LoadTime = OSTSKCon1S();/* �֐��o�R�Ń|�C���^��OSparam�ɃA�N�Z�X */
    
    uint8_t  REG_00 = 0;
    uint16_t REG_01 = 0;
    
    TSB_WD->CR = 0x0000004E;/* ����WDT�̃N���A�R�[�h���������� */
    
    /* �}�g���N�X�̓��͐��f�[�^���z���グ�� */
    if ((TSB_PA->DATA & 0x06) == 0) {
        /* �f�R�[�_0�o�͂̏ꍇ */
        /* �}�g���N�X�X�C�b�`�̏����擾 */
        MXSW &= 0xFC;
        MXSW |= (uint8_t)((TSB_PE->DATA & 0xC0) >> 6);/* ����SW�̏����X�V */
        
        /* �f�[�^�o�X�̏����擾 */
        MXIN0 &= 0xF0;/* ����4�r�b�g�N���A */
        MXIN0 |= (uint8_t)(TSB_PD->DATA & 0x0F);/* �f�[�^���Z�b�g */
        
    } else if ((TSB_PA->DATA & 0x06) == 2) {
        /* �f�R�[�_1�o�͂̏ꍇ */
        /* �}�g���N�X�X�C�b�`�̏����擾 */
        MXSW &= 0xF3;
        MXSW |= (uint8_t)((TSB_PE->DATA & 0xC0) >> 4);/* ���ݒ�SW�̏����X�V */
        
        /* �f�[�^�o�X�̏����擾 */
        MXIN0 &= 0x0F;/* ���4�r�b�g�N���A */
        MXIN0 |= (uint8_t)((TSB_PD->DATA & 0x0F) << 4);/* �f�[�^���Z�b�g */
        
    } else if ((TSB_PA->DATA & 0x06) == 4) {
        /* �f�R�[�_2�o�͂̏ꍇ */
        /* �}�g���N�X�X�C�b�`�̏����擾 */
        MXSW &= 0xCF;
        MXSW |= (uint8_t)((TSB_PE->DATA & 0xC0) >> 2);/* �d���I��SW�̏����X�V */
        
        /* �f�[�^�o�X�̏����擾 */
        MXIN1 &= 0xF0;/* ����4�r�b�g�N���A */
        MXIN1 |= (uint8_t)(TSB_PD->DATA & 0x0F);/* �f�[�^���Z�b�g */
        
    } else if ((TSB_PA->DATA & 0x06) == 6) {
        /* �f�R�[�_3�o�͂̏ꍇ */
        /* �}�g���N�X�X�C�b�`�̏����擾 */
        MXSW &= 0x3F;
        MXSW |= (uint8_t)(TSB_PE->DATA & 0xC0);/* �^�]/��~�`���[�\��SW�̏����X�V */
        
        /* �f�[�^�o�X�̏����擾 */
        MXIN1 &= 0x0F;/* ���4�r�b�g�N���A */
        MXIN1 |= (uint8_t)((TSB_PD->DATA & 0x0F) << 4);/* �f�[�^���Z�b�g */
        
    }
    
    REG_00 = (uint8_t)TSB_PA->DATA;/* �f�[�^���W�X�^��ǂݏo�� */
    REG_00 &= 0xF9;/* �}�X�N */
    REG_00 |= (uint8_t)(MXTM << 1);/* �Z�b�g */
    TSB_PA->DATA = (uint32_t)REG_00;/* �f�R�[�_�\�o�͂��X�V */
    
    MXOTN5 = (uint8_t)TSB_PA->DATA;/* �f�[�^�������Ă��� */
    MXOTN5 &= 0xE7;/* MASK���� */
    
    /* �f�R�[�_�\��؂�ւ������LED�o�͂��Z�b�g���� */
    if (MXTM == 0) {
        /* �f�R�[�_�\0�̏ꍇ */
        MXOTN5 |= ((MXOTN6 & 0x03) << 3);/* �Z�b�g */
        if (((MX6BLK & 0x01) > 0) && (MSECD >= 600)) {
            /* �_�Ńt���O������A�����^�C�~���O�ł���ꍇ */
            MXOTN5 &= 0xF7;/* �N���A */
            
        }
        if (((MX6BLK & 0x02) > 0) && (MSECD >= 600)) {
            /* �_�Ńt���O������A�����^�C�~���O�ł���ꍇ */
            MXOTN5 &= 0xEF;/* �N���A */
            
        }
        
    } else if (MXTM == 1) {
        /* �f�R�[�_�\1�̏ꍇ */
        MXOTN5 |= ((MXOTN6 & 0x0C) << 1);/* �Z�b�g */
        if (((MX6BLK & 0x04) > 0) && (MSECD >= 600)) {
            /* �_�Ńt���O������A�����^�C�~���O�ł���ꍇ */
            MXOTN5 &= 0xF7;/* �N���A */
            
        }
        if (((MX6BLK & 0x08) > 0) && (MSECD >= 600)) {
            /* �_�Ńt���O������A�����^�C�~���O�ł���ꍇ */
            MXOTN5 &= 0xEF;/* �N���A */
            
        }
        
    } else if (MXTM == 2) {
        /* �f�R�[�_�\2�̏ꍇ */
        MXOTN5 |= ((MXOTN6 & 0x30) >> 1);/* �Z�b�g */
        if (((MX6BLK & 0x10) > 0) && (MSECD >= 600)) {
            /* �_�Ńt���O������A�����^�C�~���O�ł���ꍇ */
            MXOTN5 &= 0xF7;/* �N���A */
            
        }
        if (((MX6BLK & 0x20) > 0) && (MSECD >= 600)) {
            /* �_�Ńt���O������A�����^�C�~���O�ł���ꍇ */
            MXOTN5 &= 0xEF;/* �N���A */
            
        }
        
    } else {
        /* �f�R�[�_�\3�̏ꍇ */
        MXOTN5 |= ((MXOTN6 & 0xC0) >> 3);/* �Z�b�g */
        if (((MX6BLK & 0x40) > 0) && (MSECD >= 600)) {
            /* �_�Ńt���O������A�����^�C�~���O�ł���ꍇ */
            MXOTN5 &= 0xF7;/* �N���A */
            
        }
        if (((MX6BLK & 0x80) > 0) && (MSECD >= 600)) {
            /* �_�Ńt���O������A�����^�C�~���O�ł���ꍇ */
            MXOTN5 &= 0xEF;/* �N���A */
            
        }
        
    }
    
    TSB_PA->DATA = (uint32_t)MXOTN5;/* LED�o�͍X�V�Z�b�g */
    
    MXTM++;/* ����p�Ƀ}�g���b�N�X�J�E���g���C���N�������g */
    MXTM &= 0x03;/* 0�`3�ɂȂ�l�Ƀ}�X�N */
    
    /* HACCP�p�ړ_�o�� */
    if (TNO > 0) {
        /* �Z���t�e�X�g���ł���ꍇ */
        /* �o�͍X�V���Ȃ� */
        
    } else if ((CCSEQ >= 1) && (CCSEQ < 10)) {
        /* ��p�V�[�P���X��1�ȏ�A10�����ł���ꍇ */
        TSB_PE_DATA_PE2 = 1;/* HACCP�p��p�H���o�͂��I������ */
        
    } else {
        /* ��p�V�[�P���X�������l�łȂ��ꍇ */
        TSB_PE_DATA_PE2 = 0;/* HACCP�p��p�H���o�͂��N���A���� */
        
    }
    
    if (TNO > 0) {
        /* �Z���t�e�X�g���ł���ꍇ */
        /* �o�͍X�V���Ȃ� */
        
    } else if ((ABNF & 0x1F) > 0) {
        /* �ُ킪�Z�b�g����Ă���ꍇ */
        TSB_PE_DATA_PE3 = 1;/* HACCP�p�ُ�o�͂��I������ */
        
    } else {
        /* ��������Ă���ꍇ */
        TSB_PE_DATA_PE3 = 0;/* HACCP�p�ُ�o�͂��N���A���� */
        
    }
    
    /* �e�^�X�N�̎���Ăяo�����Ԃ��f�N�������g���Ă������� */
    for (uint8_t i = 0; i < TSKNUM; i++) {
        if ( ((OS_LoadTime->TSK_COND[i] & 0x20) == 0x20)
        && (OS_LoadTime->NEXT_LOAD_Time[i] > 0) ) {
            /* �x���׸ނ�����A��ςɐ��l�������Ă���ꍇ */
            OS_LoadTime->NEXT_LOAD_Time[i]--;/* ����Ăяo�����Ԃ��f�N�������g */
            if (OS_LoadTime->NEXT_LOAD_Time[i] == 0) {
                /* ���Z�����ۂ�0�������ꍇ */
                OS_LoadTime->NEXT_LOAD_Time[i] = 0;/* 0�ɂȂ�����ȍ~0�Œ� */
                OS_LoadTime->TSK_COND[i] &= 0xDF;/* �x���t���O������ */
            }
        } else if ( ((OS_LoadTime->TSK_COND[i] & 0x20) == 0x20)
        && (OS_LoadTime->NEXT_LOAD_Time[i] == 0) ) {
            /* �x���׸ނ�����A��ς�0�ɂȂ��Ă���ꍇ */
            OS_LoadTime->NEXT_LOAD_Time[i] = 0;/* 0�ɂȂ�����ȍ~0�Œ� */
            OS_LoadTime->TSK_COND[i] &= 0xDF;/* �x���t���O������ */
        }
    }
    
    /* ����f�B�b�v�X�C�b�`�m�F����p */
    if (kidou == 0) {
        /* �N���t���O�����ċ��Ȃ��ꍇ */
        if (count01 < 5) {
            count01++;/*5�~���ȓ��ł����5�~���J�E���g*/
        }
    } else {
        /* �N���t���O�������Ă���ꍇ */
        /* �������Ȃ� */
    }
    
    /* �Q�Ɨp�̃^�C�}�[ */
    MSECD++;/*�~���Z�J���h�J�E���g�A�b�v*/
    if (MSECD == 1000) {
        /* == 1000ms */
        MSECD = 0;
        SECND++;/*�Z�J���h�J�E���g�A�b�v*/
        BOOT_SEC++;/* ��N�����Ă���̎��Զ��ı��� */
        
        if (BOOT_SEC >= 5) {
            /* �N������5�b�o�߂���ꍇ */
            BOOT_SEC = 5;/* MAX FIX */
        }
        
        if (SECND == 60) {
            /* == 60s */
            SECND = 0;
            
        }
    }
    
    if (BOOT_SEC >= 5) {
        /* �N������5�b�o�߂����ꍇ */
        SEC_CNT++;/* �C���N�������g */
        if (SEC_CNT >= 3000) {
            /* 3�b�ȏ�ɂȂ�ꍇ */
            SEC_CNT = 3000;/* MAX FIX */
            
            if (((ABNF & 0x0F) == 0)
            || (((SYSFLG & 0x02) > 0) && ((ABNF & 0x0F) == 8))
            || (((SYSFLG & 0x02) > 0) && ((ABNF & 0x0F) == 9))
            || (((SYSFLG & 0x02) > 0) && ((ABNF & 0x0F) == 14))) {
                /* �ُ�ɂȂ��ĂȂ��ꍇ�A���͉^�]���ŏ퉷���ʓd�ɖ_�ُ�ł���
                ���͉^�]���ŏ퉷���x�Z���T�[�ُ�ł���A���͐^��x�ُ�ł���ꍇ */
                ABNF &= 0x60;/* MASK */
                ABNF |= 0x8C;/* RTC�G���[�Z�b�g�A�u�U�[�I�� */
                SYSFLG |= 0x40;/* �p���[�I���t���O�Z�b�g */
                
            }
            
        }
        
        RTCKBF[2] = RTCKBF[1];/* KEEP */
        RTCKBF[1] = RTCKBF[0];/* KEEP */
        RTCKBF[0] = RTCDT[0];/* RENEW */
        
        if ((RTCKBF[0] != RTCKBF[1]) || (RTCKBF[0] != RTCKBF[2])) {
            /* �b�̕ω����������ꍇ */
            SEC_CNT = 0;/* �N���A */
            
        }
        
    }
    
    /* �`�`AD�ϊ��̕⏕�����`�` */
    if ((AD_FLG01 & 0x04) == 0x00) {
        /* AD�ϊ��I�����Ă���ꍇ */
        if (AD_RESLT >= 6) {
            /* 6�񕪁AAD�ϊ����������ꍇ */
            for (uint8_t i = 0;i < 3;i++) {
                /* 3��J��Ԃ����� */
                if (AD_NOW_CNT[i] >= 10) {
                    /* AD�ϊ��񐔂�10��ɂȂ����ꍇ */
                    REG_01 = (uint16_t)(uint32_t)((AD_NOW_SUM[i] - ((uint32_t)AD_NOW_MIN[i] + (uint32_t)AD_NOW_MAX[i])) / 8);/* ���ώZ�o */
                    AD_NOW_CNT[i] = 0;/* �e��l���N���A���� */
                    AD_NOW_MAX[i] = 0;
                    AD_NOW_SUM[i] = 0;
                    AD_NOW_MIN[i] = 4095;
                    
                    if ((AD_FLG02 & (0x01 << i)) == 0) {
                        /* 1�b�Ԃ�AD�ϊ��������Ă��Ȃ��ꍇ */
                        AD_ONE_CNT[i]++;/* 1�b��AD�ϊ��񐔂��C���N�������g���� */
                        AD_ONE_SUM[i] += REG_01;/* 1�b�Ԃ̕��ϗp�ɉ��Z���� */
                        if (AD_ONE_MIN[i] > REG_01) {
                            /* �ŏ��l�����ɂȂ�ꍇ */
                            AD_ONE_MIN[i] = REG_01;/* �ŏ��l���X�V */
                        }
                        
                        if (AD_ONE_MAX[i] < REG_01) {
                            /* �ő�l���ɂȂ�ꍇ */
                            AD_ONE_MAX[i] = REG_01;/* �ő�l���X�V */
                        }
                    }
                }
                
                if ((AD_ONE_CNT[i] >= 10) && ((AD_FLG02 & (0x01 << i)) == 0)) {
                    /* 1�b�Ԃ�AD�ϊ��񐔂�10��ɂȂ����ꍇ */
                    AD_FLG02 |= (0x01 << i);/* 1�b�Ԃ�AD�ϊ������t���O���Z�b�g */
                    REG_01 = (uint16_t)(uint32_t)((AD_ONE_SUM[i] - ((uint32_t)AD_ONE_MIN[i] + (uint32_t)AD_ONE_MAX[i])) / 8);/* ���ώZ�o */
                    AD_ONE_AVR[i] = REG_01;/* ���ϒl�ɒl���Z�b�g���� */
                    AD_ONE_MAX[i] = 0;/* �e��l���N���A���� */
                    AD_ONE_SUM[i] = 0;
                    AD_ONE_MIN[i] = 4095;
                    
                }
                
            }
            
            AD_MX    = 0;/* �N���A */
            AD_RESLT = 0;/* �N���A */
            
            /* �܂��͏��C���͂��� */
            TSB_AD->SSET03 = 0x00000080;/* AD�ϊ����郌�W�X�^�Z�b�g */
            TSB_AD->SSET47 = 0x00000000;
            
            AD_FLG01 |= 0x04;/* AD�ϊ����Z�b�g */
            
            ADC_Start(ADC_TRG_SW);/* �\�t�g�E�F�AAD�ϊ��J�n */
            AD_CTM0 = 0;/* �Œ��^�C�}�N���A */
            
        } else if (AD_RESLT == 0) {
            /* AD�ϊ����J�n���Ă��Ȃ��ꍇ */
            AD_MX    = 0;/* �N���A */
            AD_RESLT = 0;/* �N���A */
            
            for (uint8_t i = 0;i < 3;i++) {
                /* 6��J��Ԃ����� */
                AD_NOW_CNT[i] = 0;/* �e��l���N���A���� */
                AD_NOW_MAX[i] = 0;
                AD_NOW_SUM[i] = 0;
                AD_NOW_MIN[i] = 4095;
                
            }
            
            /* �܂��͏��C���͂��� */
            TSB_AD->SSET03 = 0x00000080;/* AD�ϊ����郌�W�X�^�Z�b�g */
            TSB_AD->SSET47 = 0x00000000;
            
            AD_FLG01 |= 0x04;/* AD�ϊ����Z�b�g */
            
            ADC_Start(ADC_TRG_SW);/* �\�t�g�E�F�AAD�ϊ��J�n */
            AD_CTM0 = 0;/* �Œ��^�C�}�N���A */
            
        } else {
            /* ���r���[�ɏI���Ă���ꍇ */
            AD_CTM0++;/* �Œ��^�C�}�ݸ���� */
            if (AD_CTM0 >= 10) {
                /* 10�~���Z�b�N�ȏ�o�߂����ꍇ */
                AD_CTM0 = 0;/* �Œ��^�C�}���N���A���� */
                
                AD_MX    = 0;/* �N���A */
                AD_RESLT = 0;/* �N���A */
                
                for (uint8_t i = 0;i < 6;i++) {
                    /* 6��J��Ԃ����� */
                    AD_NOW_CNT[i] = 0;/* �e��l���N���A���� */
                    AD_NOW_MAX[i] = 0;
                    AD_NOW_SUM[i] = 0;
                    AD_NOW_MIN[i] = 4095;
                    
                }
                
                /* �܂��͏��C���͂��� */
                TSB_AD->SSET03 = 0x00000080;/* AD�ϊ����郌�W�X�^�Z�b�g */
                TSB_AD->SSET47 = 0x00000000;
                
                AD_FLG01 |= 0x04;/* AD�ϊ����Z�b�g */
                
                ADC_Start(ADC_TRG_SW);/* �\�t�g�E�F�AAD�ϊ��J�n */
                
            }
        }
        
    } else {
        /* AD�ϊ����̏ꍇ */
        AD_CTM0++;/* �Œ��^�C�}�ݸ���� */
        if (AD_CTM0 >= 10) {
            /* 10�~���Z�b�N�ȏ�o�߂����ꍇ */
            AD_CTM0 = 0;/* �Œ��^�C�}���N���A���� */
            
            AD_MX    = 0;/* �N���A */
            AD_RESLT = 0;/* �N���A */
            
            for (uint8_t i = 0;i < 6;i++) {
                /* 6��J��Ԃ����� */
                AD_NOW_CNT[i] = 0;/* �e��l���N���A���� */
                AD_NOW_MAX[i] = 0;
                AD_NOW_SUM[i] = 0;
                AD_NOW_MIN[i] = 4095;
                
            }
            
            /* �܂��͏��C���͂��� */
            TSB_AD->SSET03 = 0x00000080;/* AD�ϊ����郌�W�X�^�Z�b�g */
            TSB_AD->SSET47 = 0x00000000;
            
            AD_FLG01 |= 0x04;/* AD�ϊ����Z�b�g */
            
            ADC_Start(ADC_TRG_SW);/* �\�t�g�E�F�AAD�ϊ��J�n */
            
        }
        
    }
    /* �`�`AD�ϊ��̕⏕���������܂Ł`�` */
    
    return;
}
