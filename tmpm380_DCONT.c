/***
 * �薼�FLCD�\������A�v��
 * �쐬�ҁF����@����
 * LCD/VFD�\���̐�����s���A�v���P�[�V����
***/
#include "tmpm380_DCONT.h"

const uint8_t NUMDEG[]  = "0123456789";
const uint8_t NUMDEG2[] = " 123456789";

/* �Z���t�e�X�g�pNV-RAM�`�F�b�N�� */
const uint8_t NVRMITB[] = {
    0x01, 0xAA,/* �p�X���[�h */
    0x00,/* SYSFLG */
    0x00,/* ABNF */
    0x0A,/* �p�^�[��1�ݒ艷�x */
    0x0A,/* �p�^�[��2�ݒ艷�x */
    0x0A,/* �p�^�[��3�ݒ艷�x */
    0x0A,/* �p�^�[��4�ݒ艷�x */
    0x00,/* ��p�p�^�[�� */
    20,/* �퉷���ݒ萅��20�� */
    0x00,/* �ُ헚���ƈُ팏�� */
    0x00,/* �ُ헚�� */
    0x00,/* �ُ헚�� */
    0x00,/* ��p�J�n�����i���j */
    0x00,/* ��p�J�n�����i���j */
    0x00,/* ��p�I�������i���j */
    0x00,/* ��p�I�������i���j */
    0x00,/* �ώZ�ғ����ԁi���j */
    0x00,/* �ώZ�ғ����ԁi���j���� */
    0x00,/* �ώZ�ғ����ԁi���j��� */
    0x00,/* �`���[�\�񎞊ԁi���j */
    0x08,/* �`���[�\�񎞊ԁi���j */
    0x00,/* ��p�H�����ԁi�b�j */
    0x00,/* ��p�H�����ԁi���j */
    0x00,/* ��p�H���ԍ� */
    0x05,/* �p�^�[��1���⎞�� */
    0x05,/* �p�^�[��2���⎞�� */
    0x05,/* �p�^�[��3���⎞�� */
    0x05,/* �p�^�[��4���⎞�� */
    0x00,/* ���݂̃p�^�[���I��l */
    0x00/*  �^�]�ҋ@���ԁi���j */
};

void DCONT(void)
{
    OS_TSKCon1 *OS_LoadTime;/* OS�ݒ�l��ǂނ��߂̃|�C���^�\���̂ɖ��O������ */
    OS_LoadTime = OSTSKCon1S();/* �֐��o�R�Ń|�C���^��OSparam�ɃA�N�Z�X */
    
    uint8_t  REG_00, REG_01 = 0;/* �ꎞ�ϐ��Z�b�g */
    uint32_t REG_02 = 0;
    
    for (uint8_t i = 0;i < 2;i++) {
        /* �킴�ƌJ��Ԃ��\����ݒu����icontinue;�Ŗ߂�l�ɂ���ׁj */
        i = 1;/* ����A�K�����[�v���甲������悤�ɒl���Z�b�g */
        
        switch (DLWSQ) {
        case 0:
            /* ����ł���ꍇ */
            DLWSQ = 1;/* ����X�^�[�g�ʒu��1�ɂ��� */
            OS_LoadTime->NEXT_LOAD_Time[5] = 100;/* ����Ăяo������ */
            OS_LoadTime->TSK_COND[5] = 0xA0;/* �x���t���O���z */
                
            return;/* �߂� */
            break;
            
        case 1:
            /* 2��ڂł���ꍇ */
            TSB_PA->DATA &= 0x1F;/* LCD,VFD��E/RW/RS���I�t���� */
            TSB_PF->IE &= 0xF0;/* LCD�f�[�^�o�X�����͂ɂ��� */
            TSB_PF->CR |= 0x0F;/* LCD�f�[�^�o�X���o�͂ɂ��� */
            TSB_PF->DATA &= 0xF0;/* ��U�}�X�N */
            TSB_PF->DATA |= 0x03;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A�������i4�r�b�g�o�X�ݒ�j�R�}���h */
            
            EPLS();/* E���p���X�o�͂��� */
            DLWSQ++;/* ����X�^�[�g�ʒu���C���N�������g */
            
            OS_LoadTime->NEXT_LOAD_Time[5] = 5;/* ����Ăяo������ */
            OS_LoadTime->TSK_COND[5] = 0xA0;/* �x���t���O���z */
                
            return;/* �߂� */
            break;
            
        case 2:
        case 3:
            /* 3,4��ڂł���ꍇ */
            EPLS();/* E���p���X�o�͂��� */
            DLWSQ++;/* ����X�^�[�g�ʒu���C���N�������g */
            
            OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* ����Ăяo������ */
            OS_LoadTime->TSK_COND[5] = 0xA0;/* �x���t���O���z */
                
            return;/* �߂� */
            break;
            
        case 4:
            /* 5��ڂł���ꍇ */
            TSB_PF->CR &= 0xF0;/* �o�͋@�\���I�t */
            TSB_PF->IE |= 0x0F;/* ���͋@�\���I�� */
            TSB_PA_DATA_PA6 = 1;/* RW���I������ */
            
            if ((EPBSY() & 0x08) > 0) {
                /* �r�W�[�t���O���I���ł���ꍇ */
                OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[5] = 0xA0;/* �x���t���O���z */
                
                FLZFG |= 0x01;/* �t���O�Z�b�g */
                if (FLZTM >= 60000) {
                    /* 1���ȏ�o�߂����ꍇ */
                    FLZFG = DLWSQ = 0;/* ��蒼�� */
                    FLZTM = 0;/* �^�C�}�N���A */
                    
                }
                
                return;/* �߂� */
                
            }
            
            FLZFG = 0;/* �N���A */
            DLWSQ++;/* ����X�^�[�g�ʒu���C���N�������g */
            
            TSB_PA_DATA_PA6 = 0;/* RW���I�t���� */
            TSB_PF->IE &= 0xF0;/* LCD�f�[�^�o�X�����͂ɂ��� */
            TSB_PF->CR |= 0x0F;/* LCD�f�[�^�o�X���o�͂ɂ��� */
            TSB_PF->DATA &= 0xF0;/* ��U�}�X�N */
            TSB_PF->DATA |= 0x02;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A2�s�R�}���h */
            
            EPLS();/* E���p���X�o�͂��� */
            
            OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* ����Ăяo������ */
            OS_LoadTime->TSK_COND[5] = 0xA0;/* �x���t���O���z */
                
            return;/* �߂� */
            break;
            
        case 5:
            /* 6��ڂł���ꍇ */
            TSB_PF->CR &= 0xF0;/* �o�͋@�\���I�t */
            TSB_PF->IE |= 0x0F;/* ���͋@�\���I�� */
            TSB_PA_DATA_PA6 = 1;/* RW���I������ */
            
            if ((EPBSY() & 0x08) > 0) {
                /* �r�W�[�t���O���I���ł���ꍇ */
                OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[5] = 0xA0;/* �x���t���O���z */
                
                FLZFG |= 0x01;/* �t���O�Z�b�g */
                if (FLZTM >= 60000) {
                    /* 1���ȏ�o�߂����ꍇ */
                    FLZFG = DLWSQ = 0;/* ��蒼�� */
                    FLZTM = 0;/* �^�C�}�N���A */
                    
                }
                
                return;/* �߂� */
                
            }
            
            FLZFG = 0;/* �N���A */
            DLWSQ++;/* ����X�^�[�g�ʒu���C���N�������g */
            
            TSB_PA_DATA_PA6 = 0;/* RW���I�t���� */
            TSB_PF->IE &= 0xF0;/* LCD�f�[�^�o�X�����͂ɂ��� */
            TSB_PF->CR |= 0x0F;/* LCD�f�[�^�o�X���o�͂ɂ��� */
            TSB_PF->DATA &= 0xF0;/* ��U�}�X�N */
            TSB_PF->DATA |= 0x02;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A�H�R�}���h */
            
            EPLS();/* E���p���X�o�͂��� */
            
            TSB_PF->DATA &= 0xF0;/* ��U�}�X�N */
            TSB_PF->DATA |= 0x08;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A2�s�R�}���h */
            
            EPLS();/* E���p���X�o�͂��� */
            
            OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* ����Ăяo������ */
            OS_LoadTime->TSK_COND[5] = 0xA0;/* �x���t���O���z */
                
            return;/* �߂� */
            break;
            
        case 6:
            /* 7��ڂł���ꍇ */
            TSB_PF->CR &= 0xF0;/* �o�͋@�\���I�t */
            TSB_PF->IE |= 0x0F;/* ���͋@�\���I�� */
            TSB_PA_DATA_PA6 = 1;/* RW���I������ */
            
            if ((EPBSY() & 0x08) > 0) {
                /* �r�W�[�t���O���I���ł���ꍇ */
                OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[5] = 0xA0;/* �x���t���O���z */
                
                FLZFG |= 0x01;/* �t���O�Z�b�g */
                if (FLZTM >= 60000) {
                    /* 1���ȏ�o�߂����ꍇ */
                    FLZFG = DLWSQ = 0;/* ��蒼�� */
                    FLZTM = 0;/* �^�C�}�N���A */
                    
                }
                
                return;/* �߂� */
                
            }
            
            FLZFG = 0;/* �N���A */
            DLWSQ++;/* ����X�^�[�g�ʒu���C���N�������g */
            
            TSB_PA_DATA_PA6 = 0;/* RW���I�t���� */
            TSB_PF->IE &= 0xF0;/* LCD�f�[�^�o�X�����͂ɂ��� */
            TSB_PF->CR |= 0x0F;/* LCD�f�[�^�o�X���o�͂ɂ��� */
            TSB_PF->DATA &= 0xF0;/* ��U�}�X�N */
            TSB_PF->DATA |= 0x00;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A�H�R�}���h */
            
            EPLS();/* E���p���X�o�͂��� */
            
            TSB_PF->DATA |= 0x08;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A�H�R�}���h */
            
            EPLS();/* E���p���X�o�͂��� */
            
            OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* ����Ăяo������ */
            OS_LoadTime->TSK_COND[5] = 0xA0;/* �x���t���O���z */
                
            return;/* �߂� */
            break;
            
        case 7:
            /* 8��ڂł���ꍇ */
            TSB_PF->CR &= 0xF0;/* �o�͋@�\���I�t */
            TSB_PF->IE |= 0x0F;/* ���͋@�\���I�� */
            TSB_PA_DATA_PA6 = 1;/* RW���I������ */
            
            if ((EPBSY() & 0x08) > 0) {
                /* �r�W�[�t���O���I���ł���ꍇ */
                OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[5] = 0xA0;/* �x���t���O���z */
                
                FLZFG |= 0x01;/* �t���O�Z�b�g */
                if (FLZTM >= 60000) {
                    /* 1���ȏ�o�߂����ꍇ */
                    FLZFG = DLWSQ = 0;/* ��蒼�� */
                    FLZTM = 0;/* �^�C�}�N���A */
                    
                }
                
                return;/* �߂� */
                
            }
            
            FLZFG = 0;/* �N���A */
            DLWSQ++;/* ����X�^�[�g�ʒu���C���N�������g */
            
            TSB_PA_DATA_PA6 = 0;/* RW���I�t���� */
            TSB_PF->IE &= 0xF0;/* LCD�f�[�^�o�X�����͂ɂ��� */
            TSB_PF->CR |= 0x0F;/* LCD�f�[�^�o�X���o�͂ɂ��� */
            TSB_PF->DATA &= 0xF0;/* ��U�}�X�N */
            TSB_PF->DATA |= 0x00;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A�H�R�}���h */
            
            EPLS();/* E���p���X�o�͂��� */
            
            TSB_PF->DATA |= 0x01;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A�H�R�}���h */
            
            EPLS();/* E���p���X�o�͂��� */
            
            OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* ����Ăяo������ */
            OS_LoadTime->TSK_COND[5] = 0xA0;/* �x���t���O���z */
                
            return;/* �߂� */
            break;
            
        case 8:
            /* 9��ڂł���ꍇ */
            TSB_PF->CR &= 0xF0;/* �o�͋@�\���I�t */
            TSB_PF->IE |= 0x0F;/* ���͋@�\���I�� */
            TSB_PA_DATA_PA6 = 1;/* RW���I������ */
            
            if ((EPBSY() & 0x08) > 0) {
                /* �r�W�[�t���O���I���ł���ꍇ */
                OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[5] = 0xA0;/* �x���t���O���z */
                
                FLZFG |= 0x01;/* �t���O�Z�b�g */
                if (FLZTM >= 60000) {
                    /* 1���ȏ�o�߂����ꍇ */
                    FLZFG = DLWSQ = 0;/* ��蒼�� */
                    FLZTM = 0;/* �^�C�}�N���A */
                    
                }
                
                return;/* �߂� */
                
            }
            
            FLZFG = 0;/* �N���A */
            DLWSQ++;/* ����X�^�[�g�ʒu���C���N�������g */
            
            TSB_PA_DATA_PA6 = 0;/* RW���I�t���� */
            TSB_PF->IE &= 0xF0;/* LCD�f�[�^�o�X�����͂ɂ��� */
            TSB_PF->CR |= 0x0F;/* LCD�f�[�^�o�X���o�͂ɂ��� */
            TSB_PF->DATA &= 0xF0;/* ��U�}�X�N */
            TSB_PF->DATA |= 0x00;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A�H�R�}���h */
            
            EPLS();/* E���p���X�o�͂��� */
            
            TSB_PF->DATA |= 0x06;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A�H�R�}���h */
            
            EPLS();/* E���p���X�o�͂��� */
            
            OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* ����Ăяo������ */
            OS_LoadTime->TSK_COND[5] = 0xA0;/* �x���t���O���z */
                
            return;/* �߂� */
            break;
            
        case 9:
            /* 10��ڂł���ꍇ */
            TSB_PF->CR &= 0xF0;/* �o�͋@�\���I�t */
            TSB_PF->IE |= 0x0F;/* ���͋@�\���I�� */
            TSB_PA_DATA_PA6 = 1;/* RW���I������ */
            
            if ((EPBSY() & 0x08) > 0) {
                /* �r�W�[�t���O���I���ł���ꍇ */
                OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[5] = 0xA0;/* �x���t���O���z */
                
                FLZFG |= 0x01;/* �t���O�Z�b�g */
                if (FLZTM >= 60000) {
                    /* 1���ȏ�o�߂����ꍇ */
                    FLZFG = DLWSQ = 0;/* ��蒼�� */
                    FLZTM = 0;/* �^�C�}�N���A */
                    
                }
                
                return;/* �߂� */
                
            }
            
            FLZFG = 0;/* �N���A */
            DLWSQ++;/* ����X�^�[�g�ʒu���C���N�������g */
            
            TSB_PA_DATA_PA6 = 0;/* RW���I�t���� */
            TSB_PF->IE &= 0xF0;/* LCD�f�[�^�o�X�����͂ɂ��� */
            TSB_PF->CR |= 0x0F;/* LCD�f�[�^�o�X���o�͂ɂ��� */
            TSB_PF->DATA &= 0xF0;/* ��U�}�X�N */
            TSB_PF->DATA |= 0x00;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A�H�R�}���h */
            
            EPLS();/* E���p���X�o�͂��� */
            
            TSB_PF->DATA |= 0x0C;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A�H�R�}���h */
            
            EPLS();/* E���p���X�o�͂��� */
            
            OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* ����Ăяo������ */
            OS_LoadTime->TSK_COND[5] = 0xA0;/* �x���t���O���z */
                
            return;/* �߂� */
            break;
            
        case 10:
            /* 11��ڂł���ꍇ */
            if ((STSFL0 & 0x02) == 0) {
                /* �������������Ă��Ȃ��ꍇ */
                OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[5] = 0xA0;/* �x���t���O���z */
                
                return;/* �߂� */
                
            }
            
            if ((STSFL1 & 0x02) > 0) {
                /* �������[�N���A�t���O���I�����Ă���ꍇ */
                if (MCLAST >= 60) {
                    /* �������[�N���A��t�����ł���ꍇ */
                    MCLRMS();/* ��т܂� */
                    DLWSQ = 29;/* LCD�\��������u29�v�I�� */
                    
                    i = 0;/* �J��Ԃ��̕ϐ�����U�N���A */
                    continue;/* ���[�v�̐擪�� */
                    
                } else {
                    /* �������[�N���A��t�������ł���ꍇ */
                    OS_LoadTime->NEXT_LOAD_Time[5] = 1;/* ����Ăяo������ */
                    OS_LoadTime->TSK_COND[5] = 0xA0;/* �x���t���O���z */
                    
                    return;/* �߂� */
                    
                }
                
            }/* �������[�N���A�t���O���I�����Ă��Ȃ��ꍇ�͂����ɔ����� */
            
            if (TNO > 0) {
                /* �Z���t�e�X�g���[�h�ł���ꍇ */
                DLWSQ = 19;/* LCD�\��������u19�v�I���A���ýĂ̏����� */
                i = 0;/* �J��Ԃ��̕ϐ�����U�N���A */
                continue;/* ���[�v�̐擪�� */
                
            }
            
            if (((SYSFLG & 0x40) > 0) && ((RPAT & 0x20) == 0) && ((STSFL0 & 0x20) == 0)) {
                /* �d��ON�ŁA���H�����ł͂Ȃ��A�������ł���ꍇ */
                MXOTN6 |= 0x20;/* ����LED���I������ */
                MX6BLK |= 0x20;/* ����LED�̓_�ł��Z�b�g */
                
            } else {
                /* �d��OFF���A���H�����ł��邩�A�������łȂ��ꍇ */
                MXOTN6 &= 0xDF;/* ����LED���I�t���� */
                MX6BLK &= 0xDF;/* ����LED�̓_�ł��N���A */
                
            }
            
            if ((SYSFLG & 0x40) > 0) {
                /* �d���I���ł���ꍇ */
                MXOTN6 |= 0x10;/* �d��LED���Z�b�g���� */
                
            } else {
                /* �d���I�t�ł���ꍇ */
                MXOTN6 &= 0xEF;/* �d��LED���N���A���� */
                
            }
            
            if ((SYKND0 & 0x10) > 0) {
                /* ���C���ł���ꍇ */
                SYSFLG &= 0xDF;/* �`���[�\��t���O�������I�t�ɂ��� */
                
            }
            
            if ((SYSFLG & 0x20) > 0) {
                /* �`���[�\�񂳂�Ă���ꍇ */
                MXOTN6 |= 0x80;/* �`���[�\��LED���I�� */
                
            } else {
                /* �`���[�\�񂳂�Ă��Ȃ��ꍇ */
                MXOTN6 &= 0x7F;/* �`���[�\��LED���N���A���� */
                
            }
            
            if ((SYSFLG & 0x02) == 0) {
                /* �^�]���I�t�ł���ꍇ */
                MXOTN6 &= 0xBF;/* �^�]LED���I�t */
                MX6BLK &= 0xBF;/* �_�Ńt���O���I�t */
                
            } else {
                /* �^�]���I���ł���ꍇ */
                MXOTN6 |= 0x40;/* �^�]LED���I�� */
                if (CCSEQ >= 10) {
                    /* �^������H���ł���ꍇ */
                    MX6BLK |= 0x40;/* �_�Ńt���O���I�� */
                    
                }
                
            }
            
            if (((RPAT & 0x20) > 0)
            || ((RPAT & 0x20) == 0) && ((SYSFLG & 0x40) == 0) && ((STSFL0 & 0x80) == 0)) {
                /* ���H�������A���H�����ł͂Ȃ��d�����I�t�ŁA�ݒ�Ӱ�ނł͂Ȃ��ꍇ */
                MXOTN6 &= 0xF0;/* �p�^�[��LED���N���A */
                
            } else {
                /* ���H�����ł͂Ȃ��Ƃ��A�d���I�����A�ݒ胂�[�h�̂Ƃ� */
                MXOTN6 &= 0xF0;/* �}�X�N */
                if (NPAT == 0) {
                    /* �p�^�[����1�̏ꍇ */
                    MXOTN6 |= 0x01;/* 1�Z�b�g */
                    
                } else if (NPAT == 1) {
                    /* �p�^�[����2�̏ꍇ */
                    MXOTN6 |= 0x02;/* 2�Z�b�g */
                    
                } else if (NPAT == 2) {
                    /* �p�^�[����3�̏ꍇ */
                    MXOTN6 |= 0x04;/* 3�Z�b�g */
                    
                } else {
                    /* �p�^�[����4�̏ꍇ */
                    MXOTN6 |= 0x08;/* 4�Z�b�g */
                    
                }
                
            }
            
            if ((SYSFLG & 0x40) == 0) {
                /* �d���I�t�ł���ꍇ */
                ABNFO = 0;/* ���ُ�t���O�����Z�b�g */
                
                if ((STSFL0 & 0x80) == 0) {
                    /* �ݒ蒆�ł͂Ȃ��ꍇ */
                    DSPNO = 0;/* ���v�\�����Z�b�g */
                    
                } else if ((DSPSEQ != 4) || ((DSPSEQ == 4) && (DSPSSQ == 0))) {
                    /* ��򃂁[�h�łȂ��A���͐�򃂁[�h�ł���A���C�����j���[�ł���ꍇ */
                    if ((DSPNO != 7) && (DSPSEQ > 0)) {
                        /* �ݒ��ʂ֕ύX���ł���A��ʃV�[�P���X���i��ł����ꍇ */
                        DSPSEQ = 0;/* ��x�N���A */
                        
                    }
                    DSPNO = 7;/* �ݒ胂�[�h�\���Z�b�g */
                    
                } else {
                    /* ����ȊO�̏ꍇ */
                    DSPNO = 5;/* ��򃂁[�h�\�����Z�b�g */
                    
                }
                
            } else {
                /* �d���I���ł���ꍇ */
                if ((ABNF & 0x0F) > 0) {
                    /* �ُ킪�Z�b�g����Ă���ꍇ */
                    STSFL0 &= 0x7F;/* �ݒ胂�[�h�t���O���������Z�b�g */
                    STSFL0 |= 0x20;/* �������t���O�����Z�b�g */
                    
                    if ((ABNF & 0x0F) != ABNFO) {
                        /* �ُ킪�ω����Ă����ꍇ */
                        DSPSEQ = 0;/* ��ʃV�[�P���X���N���A���� */
                        
                        /* �ُ헚���̍X�V */
                        ABNFO = (ABNF & 0x0F);/* �I�[���h���X�V���� */
                        
                        REG_00 = ABNF;/* �ŐV�ُ�f�[�^ */
                        REG_00 &= 0x0F;/* �}�X�N */
                        REG_01 = ABNCUT;/* ��O�ُ̈�f�[�^ */
                        REG_01 &= 0xF0;/* �}�X�N */
                        
                        ABNCUT &= 0x0F;/* 4�`7bit�ڂ��N���A */
                        ABNCUT |= (REG_00 << 4);/* �ŐV�f�[�^���Z�b�g���܂� */
                        
                        ABNH1 <<= 4;/* ��ԌÂ��ُ�f�[�^�������o���A4�Ԗڂ̃f�[�^���Z�b�g */
                        REG_00 = ABNH0;/* �f�[�^���[�h */
                        REG_00 >>= 4;/* �V�t�g */
                        ABNH1 |= REG_00;/* 3�Ԗڂُ̈�����[�h */
                        ABNH0 <<= 4;/* 4bit�V�t�g */
                        ABNH0 |= (REG_01 >> 4);/* 2�Ԗڂ̃f�[�^�Z�b�g */
                        
                        REG_00 = ABNCUT;/* �f�[�^���[�h */
                        REG_00 &= 0x0F;/* �ُ팏���̂݃}�X�N */
                        REG_00 ++;/* �ُ팏�����C���N�������g */
                        
                        if (REG_00 >= 5) {
                            /* 5���ȏ�ɂȂ�ꍇ */
                            REG_00 = 5;/* MAX FIX */
                            
                        }
                        
                        ABNCUT &= 0xF0;
                        ABNCUT |= REG_00;/* �ُ팏���Z�b�g */
                        
                    }/* �ُ킪�ω����Ă����ꍇ�����܂� */
                    
                    if (((SYSFLG & 0x02) == 0)
                    || (((SYSFLG & 0x02) > 0) && ((ABNF & 0x0F) != 8) && ((ABNF & 0x0F) != 9) && ((ABNF & 0x0F) != 14))) {
                        /* �^�]���ł͂Ȃ��A���͉^�]���ŏ퉷�����ʓd�ɖ_�ُ�ł͂Ȃ��A�퉷���x�Z���T�[�ُ�ł͂Ȃ��A�^��x�ُ�ł͂Ȃ��ꍇ */
                        DSPNO = 6;/* �ُ�\�����Z�b�g���� */
                        
                    } else {
                        /* �^�]���ɁA�퉷�����ʓd�ɖ_�ُ킩�A�퉷���x�Z���T�[�ُ킩�A�^��x�ُ�ł���ꍇ */
                        if ((STSFL0 & 0x20) == 0) {
                            /* �������̏ꍇ */
                            DSPNO = 1;/* �������̃��b�Z�[�W�Z�b�g */
                            
                        } else if ((SYSFLG & 0x1E) == 0) {
                            /* �^�]�H���ł͂Ȃ��ꍇ */
                            DSPNO = 2;/* �ҋ@�\���Z�b�g */
                            
                        } else if ((SYSFLG & 0x0C) > 0) {
                            /* ��p�H���ł������ꍇ */
                            DSPNO = 3;/* ��p�H���Z�b�g */
                            
                        } else {
                            /* ��p�H���ł͂Ȃ��ꍇ */
                            DSPNO = 4;/* ��p�H�������Z�b�g */
                            
                        }
                        
                    }
                    
                } else {
                    /* �ُ킪�Z�b�g����Ă��Ȃ��ꍇ */
                    if ((STSFL0 & 0x80) == 0) {
                        /* �ݒ胂�[�h�łȂ��ꍇ */
                        if ((STSFL0 & 0x20) == 0) {
                            /* �������̏ꍇ */
                            if ((DSPNO == 0) && (DSPSEQ > 0)) {
                                /* OFF -> ON���ł���A��ʃV�[�P���X���i��ł����ꍇ */
                                DSPSEQ = 0;/* ��x�N���A */
                                
                            }
                            DSPNO = 1;/* �������̃��b�Z�[�W�Z�b�g */
                            
                        } else if ((SYSFLG & 0x1E) == 0) {
                            /* �^�]�H���ł͂Ȃ��ꍇ */
                            DSPNO = 2;/* �ҋ@�\���Z�b�g */
                            
                        } else if ((SYSFLG & 0x0C) > 0) {
                            /* ��p�H���ł������ꍇ */
                            DSPNO = 3;/* ��p�H���Z�b�g */
                            
                        } else {
                            /* ��p�H���ł͂Ȃ��ꍇ */
                            DSPNO = 4;/* ��p�H�������Z�b�g */
                            
                        }
                        
                    } else {
                        /* �ݒ胂�[�h�̏ꍇ */
                        if ((DSPSEQ != 4) || ((DSPSEQ == 4) && (DSPSSQ == 0))) {
                            /* ��򃂁[�h�ł͂Ȃ��ꍇ�A���͐�򃂁[�h�Ń��C�����j���[�̏ꍇ */
                            if ((DSPNO != 7) && (DSPSEQ > 0)) {
                                /* �ݒ��ʂ֕ύX���ł���A��ʃV�[�P���X���i��ł����ꍇ */
                                DSPSEQ = 0;/* ��x�N���A */
                                
                            }
                            DSPNO = 7;/* �ݒ胂�[�h�\�����Z�b�g */
                            
                        } else {
                            /* ��򃂁[�h�Ń��C�����j���[�łȂ��ꍇ */
                            DSPNO = 5;/* ��򃂁[�h�\���Z�b�g */
                            
                        }
                        
                    }
                    
                }
                /* �d���I���ł���ꍇ�����܂� */
                
            }
            
            DLWSQ++;/* �C���N�������g */
            DLWSQ += DSPNO;/* ��ʔԍ������Z */
            i = 0;/* �ēx���[�v�ɓ���Ȃ������߁A���[�v�l���N���A */
            
            break;
            
        case 11:
            /* ���v�\�� */
            STSFL0 &= 0xEF;/* �u�U�[���֎~�t���O */
            TSB_PF_DATA_PF4 = 0;/* �o�b�N���C�g�I�t */
            
            if (DSPSEQ == 0) {
                /* ����̏ꍇ */
                DSPSEQ = 1;/* 1�ɂ��� */
                DSPTM2 = 0;/* �^�C�}�N���A */
                
                memcpy(DSPLB1, CLKMS1 + 1, 16);/* 16�o�C�g�R�s�[ */
                memcpy(DSPLB2, CLKMS2 + 1, 16);/* 16�o�C�g�R�s�[ */
                DSPLB2[2] = NUMDEG[((RTCDT[6] % 100) / 10)];/* �N10�̈� */
                DSPLB2[3] = NUMDEG[(RTCDT[6] % 10)];/* �N�P�̈� */
                DSPLB2[5] = NUMDEG[((RTCDT[5] % 100) / 10)];/* ��10�̈� */
                DSPLB2[6] = NUMDEG[(RTCDT[5] % 10)];/* ���P�̈� */
                DSPLB2[8] = NUMDEG[((RTCDT[4] % 100) / 10)];/* ��10�̈� */
                DSPLB2[9] = NUMDEG[(RTCDT[4] % 10)];/* ���P�̈� */
                DSPLB2[11] = NUMDEG[((RTCDT[2] % 100) / 10)];/* ��10�̈� */
                DSPLB2[12] = NUMDEG[(RTCDT[2] % 10)];/* ��1�̈� */
                DSPLB2[14] = NUMDEG[((RTCDT[1] % 100) / 10)];/* ��10�̈� */
                DSPLB2[15] = NUMDEG[(RTCDT[1] % 10)];/* ��1�̈� */
                
                DSPLN1 = 16;/* �f�[�^���Z�b�g */
                MHEAD1 = DSPLB1;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                MHEAD1 --;/* �A�h���X���f�N�������g */
                DSPLN2 = 16;/* �f�[�^��16���Z�b�g */
                MHEAD2 = DSPLB2;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                MHEAD2 --;/* �A�h���X���f�N�������g */
                
                DLWSQ = 25;/* �\���N���A����J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                continue;/* �擪�ɖ߂� */
                
            } else {
                /* ����ȍ~�ł���ꍇ */
                if ((STSFL1 & 0x08) > 0) {
                    /* �蓮�r����t���ł���ꍇ */
                    TSB_PF_DATA_PF4 = 1;/* �o�b�N���C�g�I�� */
                    memcpy(DSPLB1, DRAIN1 + 1, 16);/* 16�o�C�g�R�s�[ */
                    
                } else if ((STSFL1 & 0x10) > 0) {
                    /* �r�����ł���ꍇ */
                    TSB_PF_DATA_PF4 = 1;/* �o�b�N���C�g�I�� */
                    memcpy(DSPLB1, DRAIN2 + 1, 16);/* 16�o�C�g�R�s�[ */
                    
                } else {
                    /* �r���֌W�������ꍇ */
                    TSB_PF_DATA_PF4 = 0;/* �o�b�N���C�g�I�t */
                    memcpy(DSPLB1, CLKMS1+ 1, 16);/* 16�o�C�g�R�s�[ */
                    
                }
                
                DSPTM2++;/* �^�C�}�C���N�������g */
                if ((DSPTM2 < 10) && ((STSFL1 & 0x18) == 0)) {
                    /* �^�C�}��0.5�b�����ŁA�r���֌W�̃t���O�������ꍇ */
                    /* �������Ȃ� */
                    
                } else {
                    /* �^�C�}��0.5�b�ȏォ�A�r���֌W�̃t���O������ꍇ */
                    if (DSPTM2 >= 10) {
                        /* �^�C�}��1�b�o�߂����ꍇ */
                        DSPTM2 = 0;/* �N���A */
                        DCLNF ^= 0x01;/* �u�F�v���]�t���O�\�z */
                        
                    }
                    
                    memcpy(DSPLB2, CLKMS2 + 1, 16);/* 16�o�C�g�R�s�[ */
                    DSPLB2[2] = NUMDEG[((RTCDT[6] % 100) / 10)];/* �N10�̈� */
                    DSPLB2[3] = NUMDEG[(RTCDT[6] % 10)];/* �N�P�̈� */
                    DSPLB2[5] = NUMDEG[((RTCDT[5] % 100) / 10)];/* ��10�̈� */
                    DSPLB2[6] = NUMDEG[(RTCDT[5] % 10)];/* ���P�̈� */
                    DSPLB2[8] = NUMDEG[((RTCDT[4] % 100) / 10)];/* ��10�̈� */
                    DSPLB2[9] = NUMDEG[(RTCDT[4] % 10)];/* ���P�̈� */
                    DSPLB2[11] = NUMDEG[((RTCDT[2] % 100) / 10)];/* ��10�̈� */
                    DSPLB2[12] = NUMDEG[(RTCDT[2] % 10)];/* ��1�̈� */
                    DSPLB2[14] = NUMDEG[((RTCDT[1] % 100) / 10)];/* ��10�̈� */
                    DSPLB2[15] = NUMDEG[(RTCDT[1] % 10)];/* ��1�̈� */
                    
                    if ((DCLNF & 0x01) > 0) {
                        /* �R�����������^�C�~���O�ł���ꍇ */
                        DSPLB2[13] = 0x20;/* �R�����@���@�X�y�[�X */
                        
                    }
                    
                    DSPLN1 = 16;/* �f�[�^���Z�b�g */
                    MHEAD1 = DSPLB1;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                    MHEAD1 --;/* �A�h���X���f�N�������g */
                    DSPLN2 = 16;/* �f�[�^��16���Z�b�g */
                    MHEAD2 = DSPLB2;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                    MHEAD2 --;/* �A�h���X���f�N�������g */
                    
                    DLWSQ = 29;/* 1�`�Q�s�ڕ\����������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                }
                
            }/* ����ȍ~�̏��������܂� */
            
            break;
            
        case 12:
            /* TOP���b�Z�[�W�\�� */
            TSB_PF_DATA_PF4 = 1;/* �o�b�N���C�g�I�� */
            
            if (DSPSEQ == 0) {
                /* �\���V�[�P���X��0�ł���ꍇ */
                DSPSEQ = 1;/* 1�ɂ��� */
                
                memcpy(DSPLB1, TOPMS1 + 1, 16);/* 16�o�C�g�R�s�[ */
                
                if ((SYKND0 & 0xC0) == 0) {
                    /* 35RCL�ł���ꍇ */
                    /* �������Ȃ� */
                } else if ((SYKND0 & 0xC0) == 0xC0) {
                    /* 40RCL�ł���ꍇ */
                    DSPLB1[12] = '4';/* shift-jis */
                    DSPLB1[13] = '0';/* shift-jis */
                    
                } else if ((SYKND0 & 0xC0) == 0x40) {
                    /* 50RCL�ł���ꍇ */
                    DSPLB1[12] = '5';/* shift-jis */
                    DSPLB1[13] = '0';/* shift-jis */
                    
                } else {
                    /* 100RCL�ł���ꍇ */
                    DSPLB1[11] = '1';/* shift-jis */
                    DSPLB1[12] = '0';/* shift-jis */
                    DSPLB1[13] = '0';/* shift-jis */
                    
                }
                
                DSPLN1 = 16;/* �f�[�^���Z�b�g */
                MHEAD1 = DSPLB1;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                MHEAD1 --;/* �A�h���X���f�N�������g */
                
                memset(DSPLB2, 0x20, 16);
                DSPLN2 = 16;/* �f�[�^��16���Z�b�g */
                MHEAD2 = DSPLB2;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                MHEAD2 --;/* �A�h���X���f�N�������g */
                
                DSPTM1 = 0;/* �^�C�}���N���A���� */
                
                DLWSQ = 25;/* �\���N���A����J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                continue;/* �擪�ɖ߂� */
                
            } else {
                /* ����ȍ~�ł���ꍇ */
                DSPTM1++;/* �^�C�}�C���N�������g */
                if (DSPTM1 >= 4) {
                    /* �^�C�}��200ms�o�߂����ꍇ */
                    DSPTM1 = 0;/* �^�C�}�[�N���A */
                    
                    memcpy(DSPLB2, DSPLB2 + 1, 15);/* 15�o�C�g�V�t�g */
                    
                    if (DSPSEQ < 20) {
                        /* 2�ڂ̃��b�Z�[�W�\���^�C�~���O�ł͂Ȃ��ꍇ */
                        DSPLB2[15] = TOPMS2[DSPSEQ];
                        
                    } else if (DSPSEQ < 21) {
                        /* �X�y�[�X�\���^�C�~���O�ł���ꍇ */
                        DSPLB2[15] = 0x20;/* �X�y�[�X */
                        
                    } else if (DSPSEQ < 37) {
                        /* �X�y�[�X�G���A�ł���ꍇ */
                        if ((ABNF & 0x30) == 0) {
                            /* �\�m�x��E�`���[�ُ�Ȃ��ł���ꍇ */
                            DSPLB2[15] = SPACCE[(DSPSEQ - 20)];
                            
                        } else if ((ABNF & 0x30) == 0x20) {
                            /* �\�m�x�񂠂�A�`���[�ُ�Ȃ��ł���ꍇ */
                            DSPLB2[15] = ALMSG[(DSPSEQ - 20)];
                            
                        } else {
                            /* �`���[�ُ킪�������Ă���ꍇ */
                            DSPLB2[15] = ABN02[(DSPSEQ - 20)];
                            if (((ABNF & 0x40) > 0) && (DSPSEQ < 23)) {
                                /* �u�U�[�t���O������A0.4�b�ȏ�o�߂��Ă��Ȃ��ꍇ */
                                TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                                TSB_PC_DATA_PC7 = 1;/* �O���x��o�͂��I������ */
                                MXOTN2 |= 0x80;/* �r�b�g���Z�b�g */
                                
                            } else {
                                /* �u�U�[�t���O�������A����0.4�ȏ�o�߂����ꍇ */
                                TSB_PC_DATA_PC2 = 0;/* �u�U�[�I�t */
                                TSB_PC_DATA_PC7 = 0;/* �O���x��o�͂��I�t���� */
                                MXOTN2 &= 0x7F;/* �r�b�g�N���A */
                                
                            }
                            
                        }
                        
                    } else if (DSPSEQ < 39) {
                        /* ���b�Z�[�W�G���h�ł���ꍇ */
                        DSPLB2[15] = 0x20;/* �X�y�[�X */
                        
                    } else {
                        /* ����ȊO(�ŏI) */
                        DSPLB2[15] = 0x20;/* �X�y�[�X */
                        DSPSEQ = 0;/* �N���A */
                        
                    }
                    
                    DSPSEQ++;/* �C���N�������g */
                    DSPLN2 = 16;/* �f�[�^�����Z�b�g */
                    MHEAD2 = DSPLB2;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                    MHEAD2 --;/* �A�h���X���f�N�������g */
                    
                    DLWSQ = 33;/* 2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                }/* �^�C�}��200ms�o�߂��Ă��Ȃ��ꍇ�͂����ɔ����� */
                
            }
            
            break;
            
        case 13:
            /* �^�]�ҋ@�\�� */
            TSB_PF_DATA_PF4 = 1;/* �o�b�N���C�g�I�� */
            
            if (DSPSEQ == 0) {
                /* 1��ڂł���ꍇ */
                BZTM1 = 0;/* �u�U�[�^�C�}���N���A���� */
                if ((STSFL0 & 0x10) == 0) {
                    /* �u�U�[���֎~�ł͖����ꍇ */
                    STSFL0 |= 0x40;/* �Z�b�g�@�N���b�N */
                    
                }
                
                DSPSEQ = 1;/* ����2��ڂɕύX */
                DSPTM1 = 0;/* �^�C�}�N���A */
                
                DSPLN1 = RDYMS1[0];/* 1�s�ڂ̕������Z�b�g */
                MHEAD1 = RDYMS1;/* 1�s�ڂ̐擪�A�h���X���Z�b�g */
                
                memcpy(DSPLB2, RDYMS2 + 1, 16);/* 16�o�C�g�������[�R�s�[���� */
                
                if (NPAT == 0) {
                    /* �p�^�[��1�ł���ꍇ */
                    REG_00 = PAT1;/* �p�^�[��1�̐ݒ艷�x */
                    REG_01 = (RPAT & 0x01);/* ����̗L�����Z�b�g�i0�Ȃ疳���j */
                    
                } else if (NPAT == 1) {
                    /* �p�^�[��2�ł���ꍇ */
                    REG_00 = PAT2;/* �p�^�[��2�̐ݒ艷�x */
                    REG_01 = (RPAT & 0x02);/* ����̗L�����Z�b�g�i0�Ȃ疳���j */
                    
                } else if (NPAT == 2) {
                    /* �p�^�[��3�ł���ꍇ */
                    REG_00 = PAT3;/* �p�^�[��3�̐ݒ艷�x */
                    REG_01 = (RPAT & 0x04);/* ����̗L�����Z�b�g�i0�Ȃ疳���j */
                    
                } else {
                    /* �p�^�[��4�ł���ꍇ */
                    REG_00 = PAT4;/* �p�^�[��4�̐ݒ艷�x */
                    REG_01 = (RPAT & 0x08);/* ����̗L�����Z�b�g�i0�Ȃ疳���j */
                    
                }
                
                DSPLB2[4] = NUMDEG2[((REG_00 % 100) / 10)];/* ���x10�̈�(�[���͕\�����Ȃ�) */
                DSPLB2[5] =  NUMDEG[(REG_00 % 10)];/* ���x1�̈� */
                
                if (REG_01 == 0) {
                    /* ���₪�����ꍇ */
                    DSPLB2[14] = '�';/* shift-jis */
                    DSPLB2[15] = '�';/* shift-jis */
                    
                }
                
                DSPLN2 = 16;/* �f�[�^�����Z�b�g */
                MHEAD2 = DSPLB2;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                MHEAD2 --;/* �A�h���X���f�N�������g */
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                continue;/* �擪�ɖ߂� */
                
            } else if (DSPSEQ == 1) {
                /* 2��ڂł���ꍇ */
                BZTM1++;/* �u�U�[�^�C�}���C���N�������g */
                
                if (BZTM1 >= 4) {
                    /* 200ms�ȏ�o�߂��Ă����ꍇ */
                    if (BZTM1 == 4) {
                        /* �W���X�g200ms�ł������ꍇ */
                        if ((STSFL0 & 0x10) == 0) {
                            /* �u�U�[���֎~�ł͖����ꍇ */
                            STSFL0 |= 0x40;/* �N���b�N���Z�b�g */
                            
                        }
                        
                    }
                    
                    STSFL0 &= 0xEF;/* �u�U�[�֎~�t���O�����Z�b�g���� */
                    BZTM1 = 4;/* FIX */
                    
                }/* 200ms�����̏ꍇ�͂����ɔ����� */
                
                DSPTM1++;/* �^�C�}���C���N�������g */
                
                if (DSPTM1 < 40) {
                    /* 2�b�����̏ꍇ */
                    DSPLN1 = RDYMS1[0];/* 1�s�ڂ̕������Z�b�g */
                    MHEAD1 = RDYMS1;/* 1�s�ڂ̐擪�A�h���X���Z�b�g */
                    
                    memcpy(DSPLB2, RDYMS2 + 1, 16);/* 16�o�C�g�������[�R�s�[���� */
                    
                    if (NPAT == 0) {
                        /* �p�^�[��1�ł���ꍇ */
                        REG_00 = PAT1;/* �p�^�[��1�̐ݒ艷�x */
                        REG_01 = (RPAT & 0x01);/* ����̗L�����Z�b�g�i0�Ȃ疳���j */
                        
                    } else if (NPAT == 1) {
                        /* �p�^�[��2�ł���ꍇ */
                        REG_00 = PAT2;/* �p�^�[��2�̐ݒ艷�x */
                        REG_01 = (RPAT & 0x02);/* ����̗L�����Z�b�g�i0�Ȃ疳���j */
                        
                    } else if (NPAT == 2) {
                        /* �p�^�[��3�ł���ꍇ */
                        REG_00 = PAT3;/* �p�^�[��3�̐ݒ艷�x */
                        REG_01 = (RPAT & 0x04);/* ����̗L�����Z�b�g�i0�Ȃ疳���j */
                        
                    } else {
                        /* �p�^�[��4�ł���ꍇ */
                        REG_00 = PAT4;/* �p�^�[��4�̐ݒ艷�x */
                        REG_01 = (RPAT & 0x08);/* ����̗L�����Z�b�g�i0�Ȃ疳���j */
                        
                    }
                    
                    DSPLB2[4] = NUMDEG2[((REG_00 % 100) / 10)];/* ���x10�̈�(�[���͕\�����Ȃ�) */
                    DSPLB2[5] =  NUMDEG[(REG_00 % 10)];/* ���x1�̈� */
                    
                    if (REG_01 == 0) {
                        /* ���₪�����ꍇ */
                        DSPLB2[14] = '�';/* shift-jis */
                        DSPLB2[15] = '�';/* shift-jis */
                        
                    }
                    
                    DSPLN2 = 16;/* �f�[�^�����Z�b�g */
                    MHEAD2 = DSPLB2;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                    MHEAD2 --;/* �A�h���X���f�N�������g */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                } else {
                    /* 2�b�o�߂����ꍇ */
                    DSPSEQ = 2;/* �V�[�P���X��3��ڂɂ��� */
                    DSPTM1 = 0;/* �^�C�}���N���A */
                    /* �ēxDLWSQ��13�ɂď�����蒼���ƂȂ� */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* ���[�v�擪�ɖ߂� */
                    
                }
                
            } else if (DSPSEQ == 2) {
                /* 3��ڂł���ꍇ */
                if ( ((ABNF & 0x30) == 0)
                || (DSPTM1++ >= 40) ) {
                    /* �`���[�ُ�Ɨ\�m�x�񂪖����A����2�b�ȏ�o�߂����ꍇ */
                    DSPSEQ = 1;/* ����2��ڂɕύX */
                    DSPTM1 = 0;/* �^�C�}�N���A */
                    
                    DSPLN1 = RDYMS1[0];/* 1�s�ڂ̕������Z�b�g */
                    MHEAD1 = RDYMS1;/* 1�s�ڂ̐擪�A�h���X���Z�b�g */
                    
                    memcpy(DSPLB2, RDYMS2 + 1, 16);/* 16�o�C�g�������[�R�s�[���� */
                    
                    if (NPAT == 0) {
                        /* �p�^�[��1�ł���ꍇ */
                        REG_00 = PAT1;/* �p�^�[��1�̐ݒ艷�x */
                        REG_01 = (RPAT & 0x01);/* ����̗L�����Z�b�g�i0�Ȃ疳���j */
                        
                    } else if (NPAT == 1) {
                        /* �p�^�[��2�ł���ꍇ */
                        REG_00 = PAT2;/* �p�^�[��2�̐ݒ艷�x */
                        REG_01 = (RPAT & 0x02);/* ����̗L�����Z�b�g�i0�Ȃ疳���j */
                        
                    } else if (NPAT == 2) {
                        /* �p�^�[��3�ł���ꍇ */
                        REG_00 = PAT3;/* �p�^�[��3�̐ݒ艷�x */
                        REG_01 = (RPAT & 0x04);/* ����̗L�����Z�b�g�i0�Ȃ疳���j */
                        
                    } else {
                        /* �p�^�[��4�ł���ꍇ */
                        REG_00 = PAT4;/* �p�^�[��4�̐ݒ艷�x */
                        REG_01 = (RPAT & 0x08);/* ����̗L�����Z�b�g�i0�Ȃ疳���j */
                        
                    }
                    
                    DSPLB2[4] = NUMDEG2[((REG_00 % 100) / 10)];/* ���x10�̈�(�[���͕\�����Ȃ�) */
                    DSPLB2[5] =  NUMDEG[(REG_00 % 10)];/* ���x1�̈� */
                    
                    if (REG_01 == 0) {
                        /* ���₪�����ꍇ */
                        DSPLB2[14] = '�';/* shift-jis */
                        DSPLB2[15] = '�';/* shift-jis */
                        
                    }
                    
                    DSPLN2 = 16;/* �f�[�^�����Z�b�g */
                    MHEAD2 = DSPLB2;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                    MHEAD2 --;/* �A�h���X���f�N�������g */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                }/* �`���[�ُ킩�\�m�x�񂪂���A���Ԃ�2�b�����ł���ꍇ�͂����ɔ����� */
                
                if ((ABNF & 0x10) == 0) {
                    /* �`���[�ُ�ł͂Ȃ��\�m�x�񂪂���ꍇ */
                    DSPLN2 = ALMSG[0];/* �f�[�^�����Z�b�g */
                    MHEAD2 = ALMSG;/* ������̐擪�A�h���X���Z�b�g */
                    
                    DLWSQ = 33;/* 2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                } else {
                    /* �`���[�ُ킪����ꍇ */
                    if (((ABNF & 0x40) > 0) && (DSPTM1 < 10)) {
                        /* �u�U�[�v��������A0.5�b�����ł���ꍇ */
                        TSB_PC_DATA_PC2 = 1;/* �u�U�[�o�͂��I�� */
                        TSB_PC_DATA_PC7 = 1;/* �O���x��o�͂��I�� */
                        MXOTN2 |= 0x80;/* �r�b�g�Z�b�g */
                        
                    } else {
                        /* �u�U�[�v�����������A0.5�b�ȏ�o�߂����ꍇ */
                        TSB_PC_DATA_PC2 = 0;/* �u�U�[�o�͂��I�t */
                        TSB_PC_DATA_PC7 = 0;/* �O���x��o�͂��I�t */
                        MXOTN2 &= 0x7F;/* �r�b�g�N���A */
                        
                    }
                    
                    DSPLN2 = ABN02[0];/* �f�[�^�����Z�b�g */
                    MHEAD2 = ABN02;/* ������̐擪�A�h���X���Z�b�g */
                    
                    DLWSQ = 33;/* 2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                }
                /* 3��ڂ����܂� */
                
            }
            
            break;
            
        case 14:
            /* ����E��p�H���\�� */
            TSB_PF_DATA_PF4 = 1;/* �o�b�N���C�g�I�� */
            
            if ((DSPSEQ == 0) || ((DSPSEQ == 1) && (DSPTM1++ < 40))) {
                /* 1��ڂł���A����2��ڈȍ~��2�b�����ł���ꍇ */
                if (DSPSEQ == 0) {
                    /* 1��ڂɌ��� */
                    DSPSEQ = 1;/* �Z�b�g�A����2��ڂɂ��� */
                    DSPTM1 = 0;/* �^�C�}���N���A */
                    
                }
                
                if ((SYSFLG & 0x08) == 0) {
                    /* ��p�H���ł͂Ȃ��ꍇ */
                    memcpy(DSPLB1, RNMS1 + 1, 16);/* 16�o�C�g������[�R�s�[ */
                    
                } else {
                    /* ��p�H�����ł���ꍇ */
                    memcpy(DSPLB1, RNMS2 + 1, 16);/* 16�o�C�g������[���ҁ[ */
                    
                }
                
                memcpy(DSPLB2, RNSTM1 + 1, 16);/* 16�o�C�g�������[�R�s�[ */
                
                if (NPAT == 0) {
                    /* �p�^�[��1�ł���ꍇ */
                    REG_00 = PAT1;/* �p�^�[��1�̐ݒ艷�x */
                    
                } else if (NPAT == 1) {
                    /* �p�^�[��2�ł���ꍇ */
                    REG_00 = PAT2;/* �p�^�[��2�̐ݒ艷�x */
                    
                } else if (NPAT == 2) {
                    /* �p�^�[��3�ł���ꍇ */
                    REG_00 = PAT3;/* �p�^�[��3�̐ݒ艷�x */
                    
                } else {
                    /* �p�^�[��4�ł���ꍇ */
                    REG_00 = PAT4;/* �p�^�[��4�̐ݒ艷�x */
                    
                }
                
                DSPLB1[12] = NUMDEG2[((REG_00 % 100) / 10)];/* ���x10�̈�(�[���͕\�����Ȃ�) */
                DSPLB1[13] =  NUMDEG[(REG_00 % 10)];/* ���x1�̈� */
                
                DSPLB2[11] = NUMDEG2[((TMPDT0 % 100) / 10)];/* ���x10�̈�(�[���͕\�����Ȃ�) */
                DSPLB2[12] =  NUMDEG[(TMPDT0 % 10)];/* ���x1�̈� */
                
                DSPLN1 = 16;/* �f�[�^�����Z�b�g */
                MHEAD1 = DSPLB1;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                MHEAD1 --;/* �A�h���X���f�N�������g */
                
                DSPLN2 = 16;/* �f�[�^�����Z�b�g */
                MHEAD2 = DSPLB2;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                MHEAD2 --;/* �A�h���X���f�N�������g */
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                continue;/* �擪�ɖ߂� */
                
            } else if ((DSPSEQ == 1) && (DSPTM1 >= 40)) {
                /* 2��ڈȍ~��2�b�ȏ�o�߂����ꍇ */
                DSPSEQ = 2;/* 3��ڂɃZ�b�g */
                DSPTM1 = 0;/* �^�C�}�N���A */
                
            }
            
            if (DSPSEQ == 2) {
                /* 3��ڂł���ꍇ */
                DSPTM1++;/* �^�C�}���C���N�������g */
                if (DSPTM1 < 40) {
                    /* 2�b�o�߂��Ă��Ȃ��ꍇ */
                    memcpy(DSPLB2, RNSTM2 + 1, 16);/* �������[�R�s�[ */
                    
                    DSPLB2[10] = NUMDEG2[((STIME2 % 100) / 10)];/* �J�n�����u���v10�̈�(�[���͕\�����Ȃ�) */
                    DSPLB2[11] =  NUMDEG[(STIME2 % 10)];/* �J�n�����u���v1�̈� */
                    DSPLB2[13] = NUMDEG2[((STIME1 % 100) / 10)];/* �J�n�����u���v10�̈�(�[���͕\�����Ȃ�) */
                    DSPLB2[14] =  NUMDEG[(STIME1 % 10)];/* �J�n�����u���v1�̈� */
                    
                    DSPLN2 = 16;/* �f�[�^�����Z�b�g */
                    MHEAD2 = DSPLB2;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                    MHEAD2 --;/* �A�h���X���f�N�������g */
                    
                    DLWSQ = 33;/* 2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                } else {
                    /* 2�b�o�߂����ꍇ */
                    DSPSEQ = 3;/* ����4��ڕ\���� */
                    DSPTM1 = 0;/* �^�C�}�[�N���A */
                    
                }
                
            }
            
            if (DSPSEQ == 3) {
                /* 4��ڂł���ꍇ */
                DSPTM1++;/* �^�C�}���C���N�������g */
                if (DSPTM1 < 40) {
                    /* 2�b�o�߂��Ă��Ȃ��ꍇ */
                    memcpy(DSPLB2, RNSTM3 + 1, 16);/* �������[�R�s�[ */
                    
                    DSPLB2[10] = NUMDEG2[((CSEQTM % 1000) / 100)];/* �V�[�P���X���Ԃ�100�̈�(�[���͕\�����Ȃ�) */
                    if (CSEQTM >= 10) {
                        /* 10�̈ʂ���̏ꍇ */
                        DSPLB2[11] = NUMDEG[((CSEQTM % 100) / 10)];/* �V�[�P���X���Ԃ�10�̈� */
                        
                    }/* 10�̈ʖ����̏ꍇ�͉������Ȃ� */
                    DSPLB2[12] =  NUMDEG[(CSEQTM % 10)];/* �V�[�P���X���Ԃ�1�̈� */
                    
                    DSPLN2 = 16;/* �f�[�^�����Z�b�g */
                    MHEAD2 = DSPLB2;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                    MHEAD2 --;/* �A�h���X���f�N�������g */
                    
                    DLWSQ = 33;/* 2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                } else {
                    /* 2�b�o�߂����ꍇ */
                    DSPSEQ = 4;/* ����5��ڂ̕\���� */
                    DSPTM1 = 0;/* �^�C�}�[�N���A */
                    
                }
                
            }
            
            if (DSPSEQ == 4) {
                /* 5��ڂł���ꍇ */
                if (((ABNF & 0x3F) == 0) || (DSPTM1++ >= 40)) {
                    /* �ُ킩�\�m�x�񂪖����ꍇ�A����2�b�ȏ�o�߂����ꍇ */
                    i = 0;/* �ēx���[�v����Ȃ����� */
                    DSPTM1 = 0;/* �^�C�}�N���A */
                    DSPSEQ = 1;/* �V�[�P���X�ԍ���߂� */
                    continue;/* ���[�v�̐擪�֖߂� */
                    
                }/* �ُ킪�����āA�^�C�}��2�b�����̏ꍇ�͂����ɔ����� */
                
                if ((ABNF & 0x1F) == 0) {
                    /* �\�m�x��ł������ꍇ */
                    DSPLN2 = ALMSG[0];/* �f�[�^�����Z�b�g */
                    MHEAD2 = ALMSG;/* �擪�A�h���X���Z�b�g */
                    
                    DLWSQ = 33;/* 2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                }
                
                if (((ABNF & 0x40) > 0) && (DSPTM1 < 10)) {
                    /* �u�U�[�v��������A0.5�b�����ł���ꍇ */
                    TSB_PC_DATA_PC2 = 1;/* �u�U�[�o�͂��I�� */
                    TSB_PC_DATA_PC7 = 1;/* �O���x��o�͂��I�� */
                    MXOTN2 |= 0x80;/* �r�b�g�Z�b�g */
                    
                } else {
                    /* �u�U�[�v�����������A0.5�b�ȏ�o�߂����ꍇ */
                    TSB_PC_DATA_PC2 = 0;/* �u�U�[�o�͂��I�t */
                    TSB_PC_DATA_PC7 = 0;/* �O���x��o�͂��I�t */
                    MXOTN2 &= 0x7F;/* �r�b�g�N���A */
                    
                }
                
                if ((ABNF & 0x0F) == 0) {
                    /* �`���[�ُ�(�p��)�ł������ꍇ */
                    DSPLN2 = ABN02[0];/* �f�[�^�����Z�b�g */
                    MHEAD2 = ABN02;/* �擪�A�h���X���Z�b�g */
                    
                    DLWSQ = 33;/* 2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                } else {
                    /* ����ȊO�ُ̈�ł������ꍇ */
                    if (((SYKND0 & 0x10) > 0) && ((ABNF & 0x0F) == 8)) {
                        /* ���C���ł���A�퉷�����ʓd�ɖ_�ُ�ł���ꍇ */
                        DSPLN2 = ABNFSTBL[17][0];/* �f�[�^�� */
                        MHEAD2 = ABNFSTBL[17];/* �擪�A�h���X */
                        
                    } else if (((SYKND0 & 0x10) > 0) && ((ABNF & 0x0F) == 9)) {
                        /* ���C���ł���A�퉷���������x�Z���T�[�ُ�ł���ꍇ */
                        DSPLN2 = ABNFSTBL[18][0];/* �f�[�^�� */
                        MHEAD2 = ABNFSTBL[18];/* �擪�A�h���X */
                        
                    } else {
                        /* ����ȊO�̏ꍇ */
                        DSPLN2 = ABNFSTBL[(ABNF & 0x0F)][0];/* �f�[�^�� */
                        MHEAD2 = ABNFSTBL[(ABNF & 0x0F)];/* �擪�A�h���X */
                        
                    }
                    
                    DLWSQ = 33;/* 2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                }
                /* 5��ڂ����܂� */
                
            }
            
            break;
            
        case 15:
            /* ��p�H�������\�� */
            TSB_PF_DATA_PF4 = 1;/* �o�b�N���C�g�o�̓I�� */
            
            if ((DSPSEQ == 0) || ((DSPSEQ == 1) && (DSPTM1++ < 40))) {
                /* 1��ڂł���A����2��ڈȍ~��2�b�����ł���ꍇ */
                if (DSPSEQ == 0) {
                    /* 1��ڂɌ��� */
                    DSPSEQ = 1;/* �Z�b�g�A����2��ڂɂ��� */
                    DSPTM1 = 0;/* �^�C�}���N���A */
                    
                }
                
                if (CCSEQ == 15) {
                    /* �����\���^�C�~���O�ł���ꍇ */
                    memcpy(DSPLB1, ENDMS1 + 1, 16);/* �������[�R�s�[ */
                    
                } else {
                    /* �����ȊO�̃^�C�~���O�ł���ꍇ */
                    memcpy(DSPLB1, DESVC1 + 1, 16);/* �������[�R�s�[ */
                    
                }
                
                memcpy(DSPLB2, RNSTM1 + 1, 16);/* �i���\���������[�R�s�[ */
                
                if (CCSEQ == 15) {
                    /* �����\���^�C�~���O�ł���ꍇ */
                    if (NPAT == 0) {
                        /* �p�^�[��1�̏ꍇ */
                        REG_00 = PAT1;/* �p�^�[��1�̉��x���Z�b�g */
                        
                    } else if (NPAT == 1) {
                        /* �p�^�[��2�̏ꍇ */
                        REG_00 = PAT2;/* �p�^�[��2�̉��x���Z�b�g */
                        
                    } else if (NPAT == 2) {
                        /* �p�^�[��3�̏ꍇ */
                        REG_00 = PAT3;/* �p�^�[��3�̉��x���Z�b�g */
                        
                    } else {
                        /* �p�^�[��4�̏ꍇ */
                        REG_00 = PAT4;/* �p�^�[��4�̉��x���Z�b�g */
                        
                    }
                    
                    DSPLB1[12] = NUMDEG2[((REG_00 % 100) / 10)];/* ���x10�̈�(�[���͕\�����Ȃ�) */
                    DSPLB1[13] =  NUMDEG[(REG_00 % 10)];/* ���x1�̈� */
                    
                }
                
                DSPLB2[11] = NUMDEG2[((TMPDT0 % 100) / 10)];/* ���x10�̈�(�[���͕\�����Ȃ�) */
                DSPLB2[12] =  NUMDEG[(TMPDT0 % 10)];/* ���x1�̈� */
                
                DSPLN1 = 16;/* �f�[�^�����Z�b�g */
                MHEAD1 = DSPLB1;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                MHEAD1 --;/* �A�h���X���f�N�������g */
                
                DSPLN2 = 16;/* �f�[�^�����Z�b�g */
                MHEAD2 = DSPLB2;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                MHEAD2 --;/* �A�h���X���f�N�������g */
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                continue;/* �擪�ɖ߂� */
                
            } else if ((DSPSEQ == 1) && (DSPTM1 >= 40)) {
                /* 2��ڈȍ~��2�b�ȏ�o�߂����ꍇ */
                DSPSEQ = 2;/* 3��ڂ̏����� */
                DSPTM1 = 0;/* �^�C�}�N���A */
                
            }
            
            if (DSPSEQ == 2) {
                /* 3��ڂ̏����ł���ꍇ */
                if (DSPTM1++ < 40) {
                    /* �^�C���A�b�v���Ă��Ȃ��ꍇ */
                    memcpy(DSPLB2, RNSTM2 + 1, 16);/* �J�n�����\�����������[�R�s�[ */
                    
                    DSPLB2[10] = NUMDEG2[((STIME2 % 100) / 10)];/* �J�n�����u���v10�̈�(�[���͕\�����Ȃ�) */
                    DSPLB2[11] =  NUMDEG[(STIME2 % 10)];/* �J�n�����u���v1�̈� */
                    DSPLB2[13] = NUMDEG2[((STIME1 % 100) / 10)];/* �J�n�����u���v10�̈�(�[���͕\�����Ȃ�) */
                    DSPLB2[14] =  NUMDEG[(STIME1 % 10)];/* �J�n�����u���v1�̈� */
                    
                    DSPLN2 = 16;/* �f�[�^�����Z�b�g */
                    MHEAD2 = DSPLB2;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                    MHEAD2 --;/* �A�h���X���f�N�������g */
                    
                    DLWSQ = 33;/* 2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                } else {
                    /* �^�C���A�b�v�����ꍇ */
                    DSPSEQ = 3;/* 4��ڂ̏����� */
                    DSPTM1 = 0;/* �^�C�}�N���A */
                    
                }
                
            }
            
            if (DSPSEQ == 3) {
                /* 4��ڂ̏����ł���ꍇ */
                if (DSPTM1++ < 40) {
                    /* �^�C���A�b�v���Ă��Ȃ��ꍇ */
                    memcpy(DSPLB2, ENDMS2 + 1, 16);/* �I�������\�����������[�R�s�[ */
                    
                    DSPLB2[10] = NUMDEG2[((ETIME2 % 100) / 10)];/* �I�������u���v10�̈�(�[���͕\�����Ȃ�) */
                    DSPLB2[11] =  NUMDEG[(ETIME2 % 10)];/* �I�������u���v1�̈� */
                    DSPLB2[13] = NUMDEG2[((ETIME1 % 100) / 10)];/* �I�������u���v10�̈�(�[���͕\�����Ȃ�) */
                    DSPLB2[14] =  NUMDEG[(ETIME1 % 10)];/* �I�������u���v1�̈� */
                    
                    DSPLN2 = 16;/* �f�[�^�����Z�b�g */
                    MHEAD2 = DSPLB2;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                    MHEAD2 --;/* �A�h���X���f�N�������g */
                    
                    DLWSQ = 33;/* 2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                } else {
                    /* �^�C���A�b�v�����ꍇ */
                    DSPSEQ = 4;/* 5��ڂ̏����� */
                    DSPTM1 = 0;/* �^�C�}�N���A */
                    
                }
                
            }
            
            if (DSPSEQ == 4) {
                /* 5��ڂ̏����ł������ꍇ */
                if (((ABNF & 0x3F) == 0) || (DSPTM1++ >= 40)) {
                    /* �ُ킩�\�m�x�񂪖����ꍇ�A����2�b�ȏ�o�߂����ꍇ */
                    i = 0;/* �ēx���[�v����Ȃ����� */
                    DSPTM1 = 0;/* �^�C�}�N���A */
                    DSPSEQ = 1;/* �V�[�P���X�ԍ���߂� */
                    continue;/* ���[�v�̐擪�֖߂� */
                    
                }/* �ُ킪�����āA�^�C�}��2�b�����̏ꍇ�͂����ɔ����� */
                
                if ((ABNF & 0x1F) == 0) {
                    /* �\�m�x��ł������ꍇ */
                    DSPLN2 = ALMSG[0];/* �f�[�^�����Z�b�g */
                    MHEAD2 = ALMSG;/* �擪�A�h���X���Z�b�g */
                    
                    DLWSQ = 33;/* 2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                }
                
                ABNF &= 0xBF;/* �p���ُ�x��(�u�U�[)�t���O�����Z�b�g���� */
                
                if ((ABNF & 0x0F) == 0) {
                    /* �`���[�ُ�(�p��)�ł������ꍇ */
                    DSPLN2 = ABN02[0];/* �f�[�^�����Z�b�g */
                    MHEAD2 = ABN02;/* �擪�A�h���X���Z�b�g */
                    
                    DLWSQ = 33;/* 2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                } else {
                    /* ����ȊO�ُ̈�ł������ꍇ */
                    if (((SYKND0 & 0x10) > 0) && ((ABNF & 0x0F) == 8)) {
                        /* ���C���ł���A�퉷�����ʓd�ɖ_�ُ�ł���ꍇ */
                        DSPLN2 = ABNFSTBL[17][0];/* �f�[�^�� */
                        MHEAD2 = ABNFSTBL[17];/* �擪�A�h���X */
                        
                    } else if (((SYKND0 & 0x10) > 0) && ((ABNF & 0x0F) == 9)) {
                        /* ���C���ł���A�퉷���������x�Z���T�[�ُ�ł���ꍇ */
                        DSPLN2 = ABNFSTBL[18][0];/* �f�[�^�� */
                        MHEAD2 = ABNFSTBL[18];/* �擪�A�h���X */
                        
                    } else {
                        /* ����ȊO�̏ꍇ */
                        DSPLN2 = ABNFSTBL[(ABNF & 0x0F)][0];/* �f�[�^�� */
                        MHEAD2 = ABNFSTBL[(ABNF & 0x0F)];/* �擪�A�h���X */
                        
                    }
                    
                    DLWSQ = 33;/* 2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                }
                /* 5��ڂ����܂� */
                
            }
            
            break;
            
        case 16:
            /* ��򃂁[�h�\�� */
            TSB_PF_DATA_PF4 = 1;/* �o�b�N���C�g�I������ */
            
            if (DSPSSQ == 1) {
                /* ��򃂁[�h�I��\�� */
                DSPLN1 = CLEAN1[0];/* �f�[�^�����Z�b�g */
                MHEAD1 = CLEAN1;/* 1�s�ڐ擪�A�h���X���Z�b�g */
                
                memcpy(DSPLB2, CLEAN2 + 1, 16);/* �������[�R�s�[ */
                DSPLN2 = 16;/* �f�[�^���Z�b�g */
                if (EDDT[0] > 0) {
                    /* �u�n�C�v�I�𒆂ł���ꍇ */
                    DSPLB2[2] = '(';
                    DSPLB2[5] = ')';
                    DSPLB2[9] = ' ';
                    DSPLB2[13] = ' ';/* shift-jis */
                    
                }
                
                MHEAD2 = DSPLB2;/* ���ڽ�Z�b�g */
                MHEAD2--;/* �A�W���X�g */
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                continue;/* �擪�ɖ߂� */
                
            } else if (DSPSSQ == 2) {
                /* ���H���\���ł���ꍇ */
                DSPLN1 = CLEAN4[0];/* �f�[�^�����Z�b�g */
                MHEAD1 = CLEAN4;/* 1�s�ڐ擪�A�h���X���Z�b�g */
                
                if (CCSEQ < 23) {
                    /* ���H�����ł���ꍇ */
                    MHEAD2 = CLEAN5;/* 2�s�ڂ̐擪�A�h���X���Z�b�g */
                    
                } else if (CCSEQ < 25) {
                    /* 1��ڂ̂������H���ł���ꍇ */
                    MHEAD2 = CLEAN6;/* 2�s�ڂ̐擪�A�h���X���Z�b�g */
                    
                } else if (CCSEQ < 27) {
                    /* 2��ڂ̂������H���ł���ꍇ */
                    MHEAD2 = CLEAN7;/* 2�s�ڂ̐擪�A�h���X���Z�b�g */
                    
                } else {
                    /* ���I���ł���ꍇ */
                    MHEAD2 = CLEAN8;/* 2�s�ڂ̐擪�A�h���X���Z�b�g */
                    
                }
                DSPLN2 = 16;/* �f�[�^�����Z�b�g���� */
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                continue;/* �擪�ɖ߂� */
                
            } else {
                /* �p��/���~�I����ʂł���ꍇ */
                DSPLN1 = CLEAN4[0];/* �f�[�^�����Z�b�g */
                MHEAD1 = CLEAN4;/* 1�s�ڐ擪�A�h���X���Z�b�g */
                
                memcpy(DSPLB2, CLEAN3 + 1, 16);/* �������[�R�s�[ */
                DSPLN2 = 16;/* �f�[�^���Z�b�g */
                if (EDDT[0] > 0) {
                    /* �u�n�C�v�I�𒆂ł���ꍇ */
                    DSPLB2[1] = '(';
                    DSPLB2[6] = ')';
                    DSPLB2[8] = ' ';
                    DSPLB2[14] = ' ';/* shift-jis */
                    
                }
                
                MHEAD2 = DSPLB2;/* ���ڽ�Z�b�g */
                MHEAD2--;/* �A�W���X�g */
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                continue;/* �擪�ɖ߂� */
                
            }
            
            break;
            
        case 17:
            /* �ُ�\�� */
            if (DSPSEQ == 0) {
                /* �ŏ��ł���ꍇ */
                DSPSEQ = 1;/* ���̃V�[�P���X�ɂ��� */
                BZTM1 = BZTM2 = 0;/* �u�U�[����p�^�C�}���N���A */
                DSPTM1 = 0;/* �^�C�}�\�N���A */
                STSFL0 &= 0x7F;/* �ݒ胂�[�h�̋������� */
                TSB_PC_DATA_PC7 = 1;/* �O���x��o�͂��I������ */
                MXOTN2 |= 0x80;/* �r�b�g�Z�b�g */
                
            }
            
            TSB_PF_DATA_PF4 = 0;/* �o�b�N���C�g�I�t */
            DSPTM1++;/* �^�C�}�C���N�������g */
            if (DSPTM1 >= 20) {
                /* �^�C�}��1�b�o�߂����ꍇ */
                DSPTM1 = 0;/* �^�C�}�N���A */
                
            }
            
            if (DSPTM1 < 10) {
                /* �^�C�}��0.5�b�����ł���ꍇ */
                TSB_PF_DATA_PF4 = 1;/* �o�b�N���C�g�I�� */
                
            }
            
            DSPLN2 = ABNTOP[0];/* �ُ�\��2�s�ڂ̃f�[�^�����Z�b�g */
            MHEAD2 = ABNTOP;/* �擪�A�h���X���Z�b�g */
            
            if (((SYKND0 & 0x10) > 0) && ((ABNFO & 0x0F) == 2)) {
                /* ���C���ł���A�`���[�ꊇ�ُ�ł���ꍇ */
                DSPLN1 = ABNFSTBL[16][0];/* �N�[�����O�^���[�ُ�̃f�[�^�� */
                memcpy(DSPLB1, ABNFSTBL[16] + 2, 15);/* �I�t�Z�b�g�擪�A�h���X */
                
            } else if (((SYKND0 & 0x10) > 0) && ((ABNFO & 0x0F) == 8)) {
                /* ���C���ł���A�퉷�����ʓd�ɖ_�ُ�ł���ꍇ */
                DSPLN1 = ABNFSTBL[17][0];/* ���ʓd�ɖ_�ُ�f�[�^�� */
                memcpy(DSPLB1, ABNFSTBL[17] + 2, 15);/* �I�t�Z�b�g�擪�A�h���X */
                
            } else if (((SYKND0 & 0x10) > 0) && ((ABNFO & 0x0F) == 9)) {
                /* ���C���ł���A�퉷���������x�Z���T�[�ُ�ł���ꍇ */
                DSPLN1 = ABNFSTBL[18][0];/* ���x�Z���T�[�ُ�f�[�^�� */
                memcpy(DSPLB1, ABNFSTBL[18] + 2, 15);/* �I�t�Z�b�g�擪�A�h���X */
                
            } else {
                /* ����ȊO�̏ꍇ */
                DSPLN1 = ABNFSTBL[(ABNFO & 0x0F)][0];/* �f�[�^�� */
                memcpy(DSPLB1, ABNFSTBL[(ABNFO & 0x0F)] + 2, 15);/* �I�t�Z�b�g�擪�A�h���X */
                
            }
            
            DSPLB1[10] = DSPLB1[11] = ' ';/* �X�y�[�X���� */
            DSPLB1[12] = DSPLB1[13] = ' ';/* �X�y�[�X���� */
            DSPLB1[14] = DSPLB1[15] = ' ';/* �X�y�[�X���� */
            
            MHEAD1 = DSPLB1;/* �擪�A�h���X���Z�b�g���� */
            MHEAD1 --;/* �f�N�������g */
            
            if ((ABNF & 0x80) == 0) {
                /* �u�U�[�o�͂��I�t�ł���ꍇ */
                TSB_PC_DATA_PC7 = 0;/* �x��O���o�͂��I�t�ɂ��� */
                MXOTN2 &= 0x7F;/* �r�b�g�N���A */
                
            } else {
                /* �u�U�[�o�͂��I���ɂȂ��Ă���ꍇ */
                if (BZSEQ == 0) {
                    /* �u�U�[�V�[�P���X��0�ł���ꍇ */
                    BZTM1 = BZTM2 = 0;/* �^�C�}�\�N���A */
                    BZSEQ++;/* ���̃V�[�P���X�� */
                    
                }
                
                if (BZSEQ == 1) {
                    /* �u�U�[�V�[�P���X��1�̏ꍇ */
                    TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                    BZTM1++;/* �^�C�}���C���N�������g */
                    if (BZTM1 >= 20) {
                        /* 1�b�o�߂����ꍇ */
                        BZTM1 = 0;/* �N���A */
                        BZSEQ++;/* ���� */
                        
                    }
                    
                } else {
                    /* �u�U�[�V�[�P���X��2�̏ꍇ */
                    TSB_PC_DATA_PC2 = 0;/* �u�U�[�I�t */
                    BZTM1++;/* �^�C�}���C���N�������g */
                    if (BZTM1 >= 20) {
                        /* 1�b�o�߂����ꍇ */
                        BZTM1 = 0;/* �N���A */
                        BZSEQ = 1;/* ���� */
                        
                    }
                    
                }
                
            }
            
            DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
            i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
            continue;/* �擪�ɖ߂� */
            
            break;
            
        case 18:
            /* �ݒ�\�� */
            TSB_PF_DATA_PF4 = 1;/* �o�b�N���C�g�I������ */
            
            if (DSPSEQ == 0) {
                /* 1��ڂł������ꍇ */
                DSPTM1 = 0;/* �^�C�}�N���A */
                DSPSEQ = 1;/* 2��ڂ̏����� */
                
                DSPLN1 = DSMDHD[0];/* �f�[�^�����Z�b�g */
                MHEAD1 = DSMDHD;/* �擪�A�h���X���Z�b�g */
                
                DSPLN2 = DSMD01[0];/* �f�[�^�����Z�b�g */
                MHEAD2 = DSMD01;/* �擪�A�h���X���Z�b�g */
                
                DLWSQ = 25;/* ��ʃN���A��1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                continue;/* �擪�ɖ߂� */
                
            }/* 2��ڈȍ~�͂����ɔ����� */
            
            if (DSPSSQ == 0) {
                /* �T�u�V�[�P���X�ԍ���0�ł���ꍇ */
                DSPLN2 = 16;/* �f�[�^�����Z�b�g */
                MHEAD2 = DSMDSTBL_1[(DSPSEQ - 1)];/* �擪�A�h���X���Z�b�g */
                
                DLWSQ = 33;/* 2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                continue;/* �擪�ɖ߂� */
                
            }/* �T�u�V�[�P���X�ԍ���1�ȏ�̏ꍇ�͂����ɔ����� */
            
            if (DSPSEQ == 1) {
                /* �p�^�[���ݒ�ł���ꍇ */
                DSPLN1 = DSMD06[0];/* �f�[�^�����Z�b�g */
                memcpy(DSPLB1, DSMD06 + 1, 16);/* �������[�R�s�[ */
                
                DSPLN2 = 16;/* �f�[�^�����Z�b�g */
                memcpy(DSPLB2, DSMDSTBL_1[(DSPSSQ - 1) + 8] + 1, 16);/* �������[�R�s�[ */
                
                DSPLB1[7] = NUMDEG[(NPAT + 1) % 10];/* 1�s�ڃf�[�^�i�p�^�[���ԍ��j���Z�b�g */
                
                if (DSPSSQ == 2) {
                    /* ���₠��/�Ȃ��̐ݒ�ł���ꍇ */
                    if ((EDDT[0] & (0x01 << NPAT)) == 0) {
                        /* ���ݑI�𒆂̃p�^�[�������△���ł���ꍇ */
                        DSPLB2[13] = '�';/* shift-jis */
                        DSPLB2[14] = '�';/* shift-jis */
                        
                    }
                    
                } else {
                    /* ����ȊO�̐ݒ�ł���ꍇ */
                    DSPLB2[12] = NUMDEG2[((EDDT[0] % 100) / 10)];/* �ݒ�l10�̈�(�[���͕\�����Ȃ�) */
                    DSPLB2[13] =  NUMDEG[(EDDT[0] % 10)];/* �ݒ�l1�̈� */
                    
                }
                
                MHEAD1 = DSPLB1;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                MHEAD1 --;/* �A�h���X���f�N�������g */
                MHEAD2 = DSPLB2;/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g */
                MHEAD2 --;/* �A�h���X���f�N�������g */
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                continue;/* �擪�ɖ߂� */
                
            } else if (DSPSEQ == 2) {
                /* �퉷���������x�ݒ�ł���ꍇ */
                DSPLN1 = DSMD10[0];/* �f�[�^�� */
                MHEAD1 = DSMD10;/* �擪�A�h���X�Z�b�g */
                
                DSPLN2 = DSMD11[0];/* �f�[�^�� */
                memcpy(DSPLB2, DSMD11 + 1, 16);/* �퉷���ݒ萅���Z�b�g */
                DSPLB2[12] = NUMDEG2[((EDDT[0] % 100) / 10)];/* �ݒ�l10�̈�(�[���͕\�����Ȃ�) */
                DSPLB2[13] =  NUMDEG[(EDDT[0] % 10)];/* �ݒ�l1�̈� */
                
                MHEAD2 = DSPLB2;/* �擪�A�h���X�Z�b�g */
                MHEAD2 --;/* �A�h���X���f�N�������g */
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                continue;/* �擪�ɖ߂� */
                
            } else if (DSPSEQ == 3) {
                /* �����r���ݒ�ł���ꍇ */
                DSPLN1 = DSMD11A[0];/* �f�[�^�����Z�b�g���� */
                MHEAD1 = DSMD11A;/* �擪�A�h���X���Z�b�g */
                
                if ((EDDT[0] & 0x80) == 0) {
                    /* �����r������̏ꍇ */
                    DSPLN2 = DSMD11B[0];/* �f�[�^���Z�b�g */
                    MHEAD2 = DSMD11B;/* �擪�A�h���X�Z�b�g */
                    
                } else {
                    /* �����r�������̏ꍇ */
                    DSPLN2 = DSMD11C[0];/* �f�[�^���Z�b�g */
                    MHEAD2 = DSMD11C;/* �擪�A�h���X�Z�b�g */
                    
                }
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                continue;/* �擪�ɖ߂� */
                
            } else if (DSPSEQ == 5) {
                /* �`���[�\��ݒ�ł���ꍇ */
                DSPTM2++;/* �^�C�}�C���N�������g */
                
                if (DSPTM2 >= 10) {
                    /* 0.5�b�o�߂����Ƃ� */
                    DSPTM2 = 0;/* �^�C�}�N���A */
                    DCLNF ^= 0x01;/* XOR 0x01 */
                    
                }
                
                DSPLN1 = DSMD12[0];/* �f�[�^�� */
                memcpy(DSPLB1, DSMD12 + 1, 16);/* ���ݎ����ݒ�Z�b�g */
                
                DSPLN2 = DSMD13[0];/* �f�[�^�� */
                memcpy(DSPLB2, DSMD13 + 1, 16);/* �`���[�\�񎞍��ݒ�Z�b�g */
                
                /* ���ݎ��� */
                DSPLB1[11] = NUMDEG2[((RTCDT[2] % 100) / 10)];/* 10�̈�(�[���͕\�����Ȃ�) */
                DSPLB1[12] =  NUMDEG[(RTCDT[2] % 10)];/* 1�̈� */
                DSPLB1[14] = NUMDEG2[((RTCDT[1] % 100) / 10)];/* 10�̈�(�[���͕\�����Ȃ�) */
                DSPLB1[15] =  NUMDEG[(RTCDT[1] % 10)];/* 1�̈� */
                
                /* �`���[�\��ݒ莞�� */
                DSPLB2[11] = NUMDEG2[((EDDT[1] % 100) / 10)];/* 10�̈�(�[���͕\�����Ȃ�) */
                DSPLB2[12] =  NUMDEG[(EDDT[1] % 10)];/* 1�̈� */
                DSPLB2[14] =  NUMDEG[((EDDT[0] % 100) / 10)];/* 10�̈� */
                DSPLB2[15] =  NUMDEG[(EDDT[0] % 10)];/* 1�̈� */
                
                if ((DCLNF & 0x01) == 0) {
                    /* �R�����_���^�C�~���O�ł���ꍇ */
                    if (DSPSSQ == 1) {
                        /* ���̕ҏW���ł���ꍇ */
                        DSPLB2[11] = DSPLB2[12] = ' ';/* �� */
                        
                    } else {
                        /* ���̕ҏW���ł���ꍇ */
                        DSPLB2[14] = DSPLB2[15] = ' ';/* �� */
                        
                    }
                    
                } else {
                    /* �R���������^�C�~���O�ł���ꍇ */
                    DSPLB1[13] = ' ';/* �� */
                    
                }
                
                MHEAD1 = DSPLB1;/* �擪�A�h���X�Z�b�g */
                MHEAD2 = DSPLB2;
                MHEAD1 --;/* �A�h���X�f�N�������g */
                MHEAD2 --;/* �A�h���X�f�N�������g */
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                continue;/* �擪�ɖ߂� */
                
            } else if (DSPSEQ == 6) {
                /* �����ݒ�ł���ꍇ */
                DSPTM2++;/* �^�C�}�C���N�������g */
                
                if (DSPTM2 >= 10) {
                    /* 0.5�b�o�߂����Ƃ� */
                    DSPTM2 = 0;/* �^�C�}�N���A */
                    DCLNF ^= 0x01;/* XOR 0x01 */
                    
                }
                
                DSPLN1 = DSMD14[0];/* �f�[�^�����Z�b�g */
                MHEAD1 = DSMD14;/* 1�s�ڂ̃��b�Z�[�W�Z�b�g */
                
                DSPLN2 = DSMD15[0];/* �f�[�^���Z�b�g */
                memcpy(DSPLB2, DSMD15 + 1, 16);/* �������[�R�s�[ */
                
                /* �N */
                DSPLB2[2] =  NUMDEG[((EDDT[4] % 100) / 10)];/* 10�̈� */
                DSPLB2[3] =  NUMDEG[(EDDT[4] % 10)];/* 1�̈� */
                /* �� */
                DSPLB2[5] =  NUMDEG[((EDDT[3] % 100) / 10)];/* 10�̈� */
                DSPLB2[6] =  NUMDEG[(EDDT[3] % 10)];/* 1�̈� */
                /* �� */
                DSPLB2[8] =  NUMDEG[((EDDT[2] % 100) / 10)];/* 10�̈� */
                DSPLB2[9] =  NUMDEG[(EDDT[2] % 10)];/* 1�̈� */
                /* �� */
                DSPLB2[11] = NUMDEG2[((EDDT[1] % 100) / 10)];/* 10�̈� */
                DSPLB2[12] =  NUMDEG[(EDDT[1] % 10)];/* 1�̈� */
                /* �� */
                DSPLB2[14] =  NUMDEG[((EDDT[0] % 100) / 10)];/* 10�̈� */
                DSPLB2[15] =  NUMDEG[(EDDT[0] % 10)];/* 1�̈� */
                
                if ((DCLNF & 0x01) > 0) {
                    /* �����^�C�~���O�ł���ꍇ */
                    DSPLB2[((DSPSSQ - 1) * 3) + 2] = ' ';/* �X�y�[�X */
                    DSPLB2[((DSPSSQ - 1) * 3) + 3] = ' ';/* �X�y�[�X */
                    
                }
                
                MHEAD2 = DSPLB2;/* �擪�A�h���X�Z�b�g */
                MHEAD2 --;/* �A�h���X�f�N�������g */
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                continue;/* �擪�ɖ߂� */
                
            } else {
                /* �����e�i���X���[�h�ł̐ݒ� */
                DSPLN1 = DSMD16[0];/* �f�[�^�����Z�b�g���� */
                MHEAD1 = DSMD16;/* �擪�A�h���X���Z�b�g */
                
                if (DSPSSQ == 1) {
                    /* �ُ헚��\�����[�h�ł���ꍇ */
                    if (DABHP == 0) {
                        /* �P���\���̏ꍇ */
                        DSPLN2 = DSMD17[0];/* �f�[�^�����Z�b�g */
                        memcpy(DSPLB2, DSMD17 + 1, 16);/* �������[�R�s�[ */
                        
                        DSPLB2[13] = NUMDEG[(ABNCUT & 0x0F) % 10];/* �ُ헚�������̐������Z�b�g */
                        
                        MHEAD2 = DSPLB2;/* �擪�A�h���X���Z�b�g */
                        MHEAD2 --;/* �A�h���X���f�N�������g */
                        
                        DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                        i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                        continue;/* �擪�ɖ߂� */
                        
                    } else {
                        /* �ڍו\���ł���ꍇ */
                        REG_00 = 0;/* ��U�N���A */
                        if (DABHP == 1) {
                            /* �ŐV�\���̏ꍇ */
                            REG_00 = ABNCUT;/* �ŐV�ُ���Z�b�g */
                            REG_00 >>= 4;/* 4bit�E�V�t�g */
                            
                        } else if (DABHP == 2) {
                            /* 2�Ԗڂ̕\���̏ꍇ */
                            REG_00 = ABNH0;/* 2�Ԗڂُ̈���Z�b�g */
                            REG_00 &= 0x0F;/* �}�X�N */
                            
                        } else if (DABHP == 3) {
                            /* 3�Ԗڂ̕\���̏ꍇ */
                            REG_00 = ABNH0;/* 3�Ԗڂُ̈���Z�b�g */
                            REG_00 >>= 4;/* 4bit�E�V�t�g */
                            
                        } else if (DABHP == 4) {
                            /* 4�Ԗڂ̕\���̏ꍇ */
                            REG_00 = ABNH1;/* 4�Ԗڂُ̈���Z�b�g */
                            REG_00 &= 0x0F;/* �}�X�N */
                            
                        } else {
                            /* 5�Ԗڂ̕\���̏ꍇ */
                            REG_00 = ABNH1;/* 5�Ԗڂُ̈���Z�b�g */
                            REG_00 >>= 4;/* 4bit�E�V�t�g */
                            
                        }
                        
                        if (((SYKND0 & 0x10) > 0) && (REG_00 == 2)) {
                            /* ���C���ŁA�`���[�ꊇ�ُ�ł���ꍇ */
                            DSPLN2 = ABN16[0];
                            memcpy(DSPLB2, ABN16 + 1, 16);/* �N�[�����O�^���[�ꊇ�ُ�����[�h */
                            
                        } else if (((SYKND0 & 0x10) > 0) && (REG_00 == 8)) {
                            /* ���C���ŁA�퉷�����ʓd�ɖ_�ُ�ł���ꍇ */
                            DSPLN2 = ABN17[0];
                            memcpy(DSPLB2, ABN17 + 1, 16);/* ���ʓd�ɖ_�ُ�����[�h */
                            
                        } else if (((SYKND0 & 0x10) > 0) && (REG_00 == 9)) {
                            /* ���C���ŁA�퉷���������x�Z���T�[�ُ�ł���ꍇ */
                            DSPLN2 = ABN18[0];
                            memcpy(DSPLB2, ABN18 + 1, 16);/* �����Z���T�ُ�����[�h */
                            
                        } else {
                            /* ����ȊO�̏ꍇ */
                            DSPLN2 = ABNFSTBL[REG_00][0];/* �f�[�^���Z�b�g */
                            memcpy(DSPLB2, ABNFSTBL[REG_00] + 1, 16);/* �ُ�̕����񃍁[�h */
                            
                        }
                        
                        DSPLB2[0] = NUMDEG[(DABHP % 10)];/* ����ԍ���shift-jis�ŃZ�b�g���� */
                        
                        MHEAD2 = DSPLB2;/* �擪�A�h���X���Z�b�g */
                        MHEAD2 --;/* �A�h���X���f�N�������g */
                        
                        DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                        i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                        continue;/* �擪�ɖ߂� */
                        
                    }
                    
                } else if (DSPSSQ == 2) {
                    /* �\�m�x���ʂł���ꍇ */
                    if ((ABNF & 0x20) == 0) {
                        /* �\�m�x��r�b�g���N���A����Ă���ꍇ */
                        YOTINO = 0;/* �\�m�\���i���o�[���N���A */
                        DSPLN2 = DSMD18[0];/* �f�[�^���Z�b�g */
                        memcpy(DSPLB2, DSMD18 + 1, 16);/* �f�[�^�����[�h */
                        
                        DSPLB2[13] = '�';
                        DSPLB2[14] = '�';/* shift-jis */
                        
                    } else if (((ABNF & 0x20) > 0) && (YOTINO == 0)) {
                        /* �\�m�x�񂠂�ŁA�ڍו\���ł͂Ȃ��ꍇ */
                        YOTINO = 0;/* �\�m�\���i���o�[���N���A */
                        DSPLN2 = DSMD18[0];/* �f�[�^���Z�b�g */
                        memcpy(DSPLB2, DSMD18 + 1, 16);/* �f�[�^�����[�h */
                        
                    } else {
                        /* ����ȊO�̏ꍇ */
                        DSPLN2 = DALM1[0];/* �f�[�^���Z�b�g */
                        memcpy(DSPLB2, DALM1 + 1, 16);/* �`���[�\�͒ቺ��\�� */
                        
                    }
                    
                    MHEAD2 = DSPLB2;/* �擪�A�h���X���Z�b�g */
                    MHEAD2 --;/* �A�h���X���f�N�������g */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                } else if (DSPSSQ == 3) {
                    /* �ғ����ԕ\���ł���ꍇ */
                    DSPLN2 = DSMD19[0];/* �f�[�^�������[�h */
                    memcpy(DSPLB2, DSMD19 + 1, 16);/* �ݔ��ғ����ԕ�������������[�R�s�[ */
                    
                    REG_02 = (uint32_t)RTIMH2;/* �ݔ��ғ����Ԃ̏�ʃ��[�h */
                    REG_02 <<= 8;/* 8bit ���V�t�g */
                    REG_02 |= (uint32_t)RTIMH1;/* �ݔ��ғ����Ԃ̉��ʃ��[�h */
                    
                    DSPLB2[11] = NUMDEG[(REG_02 % 100000) / 10000];/* 1���̈ʃZ�b�g */
                    DSPLB2[12] = NUMDEG[(REG_02 % 10000) / 1000];/* 1000�̈ʃZ�b�g */
                    DSPLB2[13] = NUMDEG[(REG_02 % 1000) / 100];/* 100�̈ʃZ�b�g */
                    DSPLB2[14] = NUMDEG[(REG_02 % 100) / 10];/* 10�̈ʃZ�b�g */
                    DSPLB2[15] = NUMDEG[REG_02 % 10];/* 1�̈ʃZ�b�g */
                    
                    MHEAD2 = DSPLB2;/* �擪�A�h���X���Z�b�g */
                    MHEAD2 --;/* �A�h���X���f�N�������g */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                } else if (DSPSSQ == 4) {
                    /* �퉷�����x�\���ł���ꍇ */
                    DSPLN2 = DSMD20[0];/* �f�[�^���Z�b�g */
                    memcpy(DSPLB2, DSMD20 + 1, 16);/* �������[�R�s�[ */
                    
                    DSPLB2[12] = NUMDEG2[(TMPDT2 % 100) / 10];/* 10�̈� */
                    DSPLB2[13] = NUMDEG[TMPDT2 % 10];/* 1�̈� */
                    
                    MHEAD2 = DSPLB2;/* �擪�A�h���X���Z�b�g */
                    MHEAD2 --;/* �A�h���X���f�N�������g */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                } else if (DSPSSQ == 5) {
                    /* �ቷ�����x�\���ł���ꍇ */
                    DSPLN2 = DSMD21[0];/* �f�[�^���Z�b�g */
                    memcpy(DSPLB2, DSMD21 + 1, 16);/* �������[�R�s�[ */
                    
                    DSPLB2[12] = NUMDEG2[(TMPDT1 % 100) / 10];/* 10�̈� */
                    DSPLB2[13] = NUMDEG[TMPDT1 % 10];/* 1�̈� */
                    
                    MHEAD2 = DSPLB2;/* �擪�A�h���X���Z�b�g */
                    MHEAD2 --;/* �A�h���X���f�N�������g */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                } else if (DSPSSQ == 6) {
                    /* �i���\���ł���ꍇ */
                    DSPLN2 = DSMD22[0];/* �f�[�^���Z�b�g */
                    memcpy(DSPLB2, DSMD22 + 1, 16);/* �������[�R�s�[ */
                    
                    DSPLB2[12] = NUMDEG2[(TMPDT0 % 100) / 10];/* 10�̈� */
                    DSPLB2[13] = NUMDEG[TMPDT0 % 10];/* 1�̈� */
                    
                    MHEAD2 = DSPLB2;/* �擪�A�h���X���Z�b�g */
                    MHEAD2 --;/* �A�h���X���f�N�������g */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                } else if (MMODE == 0) {
                    /* ���r�����[�h�ł���ꍇ */
                    DSPLN2 = DSMD23[0];/* �f�[�^���Z�b�g */
                    MHEAD2 = DSMD23;/* �擪�A�h���X���Z�b�g���� */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                } else if (MMODE == 1) {
                    /* �퉷���r�����[�h�ł���ꍇ */
                    DSPLN2 = DSMD231[0];/* �f�[�^���Z�b�g */
                    MHEAD2 = DSMD231;/* �擪�A�h���X���Z�b�g���� */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                } else if (MMODE == 2) {
                    /* �ቷ���r�����[�h�ł���ꍇ */
                    DSPLN2 = DSMD232[0];/* �f�[�^���Z�b�g */
                    MHEAD2 = DSMD232;/* �擪�A�h���X���Z�b�g���� */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    continue;/* �擪�ɖ߂� */
                    
                } else {
                    /* ����ȊO�̕\���̏ꍇ */
                    MMDTM++;/* �^�C�}�C���N�������g */
                    if (MMDTM < 10) {
                        /* 0.5�b�o�߂��Ă��Ȃ��ꍇ */
                        TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                        STSFL1 |= 0x04;/* �}�j���A�����[�h����G���[�t���O���Z�b�g */
                        
                    } else {
                        /* 0.5�b�ȏ�o�߂����ꍇ */
                        TSB_PC_DATA_PC2 = 0;/* �u�U�[�I�t */
                        STSFL1 &= 0xFB;/* �}�j���A�����[�h����G���[�t���O���N���A */
                        
                        if (MMDTM >= 20) {
                            /* 2�b�ȏ�o�߂����ꍇ */
                            MMODE = 0;/* �}�j���A�����[�h���N���A */
                            MMDTM = 0;/* �^�C�}�N���A */
                            INDPTM = 0;/* �ʏ�\�����A�^�C�}���N���A */
                            
                            STSFL0 &= 0x7F;/* �ݒ胂�[�h���N���A */
                            DSPSEQ = 0;/* �f�B�X�v���[�V�[�P���X���N���A���� */
                            DSPSSQ = 0;/* �T�u�V�[�P���X���N���A */
                            
                        }
                        
                    }
                    
                    DSPLN2 = DSMD23E[0];/* �f�[�^���Z�b�g */
                    MHEAD2 = DSMD23E;/* �擪�A�h���X���Z�b�g���� */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    
                }
                
            }
            
            break;
            
        case 19:
            /* �Z���t�e�X�g���[�h�̕\�� */
            TSB_PF_DATA_PF4 = 1;/* �o�b�N���C�g�I�� */
            
            if (TNO == 1) {
                /* �Z���t�e�X�g�X�^�[�g�� */
                MXOTN6 = 0;/* LED�o�͂�S�ăI�t�ɂ��� */
                MX6BLK = 0;/* LED�̓_�ŗv�������Z�b�g */
                MXOTN0 = 0;/* �o�͂��N���A���� */
                MXOTN1 = 0;/* �o�͂��N���A���� */
                MXOTN2 = 0;/* �o�͂��N���A���� */
                
                TSB_PG->DATA = MXOTN0;/* �o�͍X�V */
                
                REG_00 = TSB_PN->DATA;/* �f�[�^���W�X�^�����[�h */
                REG_00 &= 0x01;/* 0�r�b�g�ڂ̂ݎc�� */
                MXOTN1 |= REG_00;/* OR���� */
                TSB_PN->DATA = MXOTN1;/* �o�͍X�V */
                
                REG_00 = TSB_PC->DATA;/* �f�[�^���W�X�^�����[�h */
                REG_00 &= 0x07;/* 0�`2�r�b�g�ڂ̂ݎc�� */
                MXOTN2 |= REG_00;/* OR���� */
                TSB_PC->DATA = MXOTN2;/* �o�͍X�V */
                
                DSPLN1 = TSTTOP[0];/* �f�[�^�����Z�b�g */
                MHEAD1 = TSTTOP;/* �擪�A�h���X�Z�b�g */
                
                DSPLN2 = TSTMS0[0];/* �f�[�^�����Z�b�g */
                MHEAD2 = TSTMS0;/* �擪�A�h���X�Z�b�g */
                
                TSB_PC_DATA_PC2 = 1;/* �u�U�[�o�͂��I������ */
                
                STSTM1++;/* �^�C�}�C���N�������g */
                
                if (STSTM1 >= 10) {
                    /* 0.5�b�o�߂����ꍇ */
                    TSB_PC_DATA_PC2 = 0;/* �u�U�[�o�͂��I�t���� */
                    TNO = 2;/* LCD�iVFD�j�m�F�̍��ڔԍ����Z�b�g */
                    SUBTNO = 0;/* �T�u�e�X�g�i���o�[���N���A */
                    
                }
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                
            } else if (TNO == 2) {
                /* LCD�iVFD�j�̊m�F�ł���ꍇ */
                REG_00 = SUBTNO;/* �T�u�e�X�g�ԍ����Z�b�g */
                REG_00 &= 0x0F;/* �K�v�ȕ������}�X�N */
                DSPLB1[0] = NUMDEG[REG_00];/* ���l���cshift-jis�Z�b�g */
                memcpy(DSPLB1 + 1, DSPLB1, 15);/* �������[�R�s�[�őS�������ɂ��� */
                
                STSTM1++;/* �^�C�}���C���N�������g */
                if (STSTM1 >= 10) {
                    /* 0.5�b�o�߂����ꍇ */
                    STSTM1 = 0;/* �^�C�}�N���A */
                    SUBTNO++;/* �C���N�������g */
                    if ((SUBTNO & 0x0F) >= 10) {
                        /* 10�ȏ�ɂȂ�ꍇ */
                        SUBTNO &= 0xF0;/* �����N���A */
                        SUBTNO += 0x10;/* ���Z���� */
                        
                        if (SUBTNO >= 0x20) {
                            /* 2��I�����ł������ꍇ */
                            SUBTNO = 0;/* �T�u�ԍ����N���A */
                            TNO = 3;/* ���̊m�F�� */
                            
                        }
                        
                    }
                    
                }
                
                DSPLN1 = DSPLN2 = 16;/* �f�[�^�����Z�b�g���� */
                MHEAD1 = DSPLB1;/* �擪�A�h���X�Z�b�g */
                MHEAD2 = DSPLB1;/* �擪�A�h���X�Z�b�g�i��Ɠ����j */
                MHEAD1 --;/* �A�h���X�f�N�������g */
                MHEAD2 --;/* �A�h���X�f�N�������g */
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                
            } else if (TNO == 3) {
                /* LED�̊m�F�ł���ꍇ */
                if (SUBTNO > 0) {
                    /* �ŏ��̃V�[�P���X�ł͂Ȃ��ꍇ */
                    TSB_PC_DATA_PC2 = 0;/* �u�U�[�I�t */
                    
                } else {
                    /* �ŏ��̃V�[�P���X�ł���ꍇ */
                    TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                    MXOTN6 |= 0x80;/* �`���[�\��LED���I�� */
                    SUBTNO = 1;/* �T�u�ԍ������� */
                    
                }
                
                STSTM1++;/* �^�C�}�C���N�������g */
                if (STSTM1 >= 10) {
                    /* 0.5�b�o�߂����ꍇ */
                    STSTM1 = 0;
                    MXOTN6 >>= 1;/* 1bit�V�t�g */
                    
                    if (MXOTN6 == 0) {
                        /* LED�o�͏I���̏ꍇ */
                        SUBTNO = 0;/* �T�u�ԍ����N���A */
                        TNO = 4;/* ���̊m�F���ڂ� */
                        
                    }
                    
                }
                
                DSPLN1 = TSTTOP[0];/* �f�[�^�����Z�b�g */
                MHEAD1 = TSTTOP;/* �擪�A�h���X���Z�b�g */
                
                DSPLN2 = TSTMS1[0];/* �f�[�^�����Z�b�g */
                MHEAD2 = TSTMS1;/* �擪�A�h���X���Z�b�g */
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                
            } else if (TNO == 4) {
                /* �X�C�b�`�̊m�F�ł���ꍇ */
                if (SUBTNO > 0) {
                    /* �ŏ��̃V�[�P���X�ł͂Ȃ��ꍇ */
                    TSB_PC_DATA_PC2 = 0;/* �u�U�[�I�t */
                    
                } else {
                    /* �ŏ��̃V�[�P���X�ł���ꍇ */
                    TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                    BITPOS = 0x01;/* �X�C�b�`���͂̃x���t�@�C�p */
                    SUBTNO = 1;/* �T�u�ԍ������� */
                    
                }
                
                if (MXSWO0 == BITPOS) {
                    /* �X�C�b�`���͂ƃx���t�@�C�p�̒l�������ł���ꍇ */
                    TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                    SUBTNO++;/* �C���N�������g */
                    BITPOS <<= 1;/* 1�ޯăV�t�g */
                    
                    if (BITPOS == 0) {
                        /* �V�t�g��̒l���[���̏ꍇ */
                        SUBTNO = 0;/* �N���A */
                        TNO = 5;/* ���̊m�F�� */
                        
                    }
                    
                }
                
                memcpy(DSPLB2, TSTMS2 + 1, 16);/* �������[�R�s�[ */
                DSPLB2[2] = NUMDEG[SUBTNO];/* �����𕶎��񉻂��ăC�� */
                DSPLN2 = 16;/* �f�[�^���Z�b�g */
                
                DSPLN1 = TSTTOP[0];/* �f�[�^�����Z�b�g */
                MHEAD1 = TSTTOP;/* �擪�A�h���X�Z�b�g */
                MHEAD2 = DSPLB2;/* �擪�A�h���X���Z�b�g */
                MHEAD2 --;/* �f�N�������g */
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                
            } else if (TNO == 5) {
                /* �o�͂̊m�F�ł���ꍇ */
                TSB_PC_DATA_PC2 = 0;/* �u�U�[�o�͂��N���A���� */
                
                if (SUBTNO == 0) {
                    /* �ŏ��̃V�[�P���X�ł������ꍇ */
                    SUBTNO = 1;/* 1���Z�b�g */
                    MXOTN0 = 0x01;/* �퉷�^��ُo�͂��Z�b�g���� */
                    
                }
                
                STSTM1++;/* �^�C�}���C���N�������g */
                if (STSTM1 >= 10) {
                    /* 0.5�b�o�߂����ꍇ */
                    TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                    STSTM1 = 0;/* �^�C�}�N���A */
                    if (MXOTN0 > 0) {
                        /* �����r�b�g�������Ă���ꍇ */
                        MXOTN0 <<= 1;/* 1�ޯč��V�t�g */
                        if (MXOTN0 == 0) {
                            /* �[���ɂȂ�ꍇ */
                            MXOTN1 = 0x02;/* ��p���r���ُo�̓Z�b�g */
                            
                        }
                        
                    } else if ((MXOTN1 & 0xFE) > 0) {
                        /* �����r�b�g�������Ă���ꍇ */
                        MXOTN1 <<= 1;/* 1�ޯăV�t�g */
                        
                        if (MXOTN1 == 0) {
                            /* �[���ɂȂ�ꍇ */
                            MXOTN2 = 0x08;/* �`���[�z��P���Z�b�g */
                            
                        }
                        
                    } else if ((MXOTN2 & 0xF8) > 0) {
                        /* �����r�b�g�������Ă���ꍇ */
                        MXOTN2 <<= 1;/* 1�r�b�g�V�t�g */
                        
                        if (MXOTN2 == 0) {
                            /* �[���ɂȂ�ꍇ */
                            SUBTNO = 0;/* �e�X�g�i���o�[�N���A */
                            TNO = 6;/* ���̊m�F�� */
                            
                        }
                        
                    }
                    
                }/* 0.5�b�o�߂��Ă��Ȃ��ꍇ�͂����ɔ����� */
                
                /* �o�͂̍X�V */
                TSB_PG->DATA = MXOTN0;/* �o�͍X�V */
                
                REG_00 = TSB_PN->DATA;/* ���݂̏o�͂�ǂݏo�� */
                REG_00 &= 0x01;/* 0�r�b�g�ڂ͎c���悤�Ƀ}�X�N */
                REG_00 |= (MXOTN1 & 0xFE);/* OR���� */
                TSB_PN->DATA = REG_00;/* �o�͍X�V */
                
                REG_00 = TSB_PC->DATA;/* ���݂̏o�͂�ǂݏo�� */
                REG_00 &= 0x07;/* 0�r�b�g�ڂ͎c���悤�Ƀ}�X�N */
                REG_00 |= (MXOTN2 & 0xF8);/* OR���� */
                TSB_PC->DATA = REG_00;/* �o�͍X�V */
                
                /* ��ʃf�[�^���Z�b�g���� */
                DSPLN1 = TSTTOP[0];/* �f�[�^�����Z�b�g */
                MHEAD1 = TSTTOP;/* �擪�A�h���X���Z�b�g */
                DSPLN2 = TSTMS3[0];/* �f�[�^�����Z�b�g */
                MHEAD2 = TSTMS3;/* �擪�A�h���X���Z�b�g */
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                
            } else if (TNO == 6) {
                /* �O�����͂̊m�F�ł���ꍇ */
                TSB_PC_DATA_PC2 = 0;/* �u�U�[�o�͂��N���A���� */
                
                if (SUBTNO == 0) {
                    /* �ŏ��̃V�[�P���X�ł������ꍇ */
                    SUBTNO = 1;/* 1���Z�b�g */
                    BITPOS = 0x01;/* ��r�p�ϐ��Z�b�g */
                    
                }
                
                if (SUBTNO < 9) {
                    /* MXINO0�̊m�F�̏ꍇ */
                    if (((MXINO0 ^ 0x0F) == BITPOS) && ((MXINO1 ^ 0xF0) == 0)) {
                        /* ��v����ꍇ(���]���K�v�ȉӏ���XOR�Ŕ��]�����Ă���) */
                        TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                        
                        SUBTNO++;/* �e�X�g�i���o�[�C���N�������g */
                        if (SUBTNO == 6) {
                            /* �i���o�[��6�ɂȂ�ꍇ */
                            SUBTNO++;/* �X�ɃC���N�������g */
                            BITPOS <<= 1;/* 1�ޯč��V�t�g */
                            
                        }
                        BITPOS <<= 1;/* 1�r�b�g���V�t�g */
                        
                        if (BITPOS == 0) {
                            /* �[���ɂȂ����ꍇ */
                            BITPOS = 1;/* �ēx1�Z�b�g */
                            
                        }
                        
                    }/* ��v���Ȃ���΂����ɔ����� */
                    
                } else {
                    /* MXINO1�̊m�F�̏ꍇ */
                    if (((MXINO1 ^ 0xF0) == BITPOS) && ((MXINO0 ^ 0x0F) == 0)) {
                        /* ��v����ꍇ */
                        TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                        
                        SUBTNO++;/* �e�X�g�i���o�[�C���N�������g */
                        if (SUBTNO == 10) {
                            /* �i���o�[��10�ɂȂ�ꍇ */
                            SUBTNO += 3;/* +3���� */
                            BITPOS = 0x08;/* �f�B�b�v�X�C�b�`�m�F�ʒu-1�̃|�W�V�����ɂ��� */
                            
                        }
                        BITPOS <<= 1;/* 1�ޯč��V�t�g */
                        
                        if (BITPOS == 0) {
                            /* �[���ɂȂ����ꍇ */
                            SUBTNO = 0;/* �T�u�ԍ����N���A */
                            TNO = 7;/* ���̊m�F�� */
                            
                        }
                        
                    }/* ��v���Ȃ��ꍇ�͂����ɔ����� */
                    
                }
                
                DSPLN1 = TSTTOP[0];/* �f�[�^���Z�b�g */
                MHEAD1 = TSTTOP;/* �擪�A�h���X�Z�b�g */
                
                if (SUBTNO < 13) {
                    /* �f�B�b�v�X�C�b�`�̊m�F�ȑO�ł���ꍇ */
                    DSPLN2 = TSTMS4[0];/* �f�[�^�����Z�b�g */
                    MHEAD2 = TSTMS4;/* �擪�A�h���X�Z�b�g */
                    
                } else {
                    /* �f�B�b�v�X�C�b�`�̊m�F�ł���ꍇ */
                    DSPLN2 = TSTMSB[0];/* �f�[�^���Z�b�g */
                    memcpy(DSPLB2, TSTMSB + 1, 16);/* �������[�R�s�[ */
                    DSPLB2[14] = NUMDEG[ (SUBTNO - 12)/*4 - (SUBTNO - 13)*/ ];/* �����𕶎��ɃR���o�[�g */
                    
                    MHEAD2 = DSPLB2;/* �擪�A�h���X�Z�b�g */
                    MHEAD2 --;/* �A�h���X�f�N�������g */
                    
                }
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                
            } else if (TNO == 7) {
                /* A/D595 ALARM�̊m�F�ł���ꍇ */
                TSB_PC_DATA_PC2 = 0;/* �u�U�[�o�͂��N���A���� */
                
                if (((MXINO1 ^ 0xF0) == 0x02) && ((MXINO0 ^ 0x0F) == 0)) {
                    /* �f���M�����������ꍇ */
                    TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                    SUBTNO = 0;/* �N���A */
                    TNO = 8;/* ���̊m�F�� */
                    
                }
                
                DSPLN1 = TSTTOP[0];/* �f�[�^���Z�b�g */
                MHEAD1 = TSTTOP;/* �擪�A�h���X�Z�b�g */
                
                DSPLN2 = TSTMS5[0];/* �f�[�^���Z�b�g */
                MHEAD2 = TSTMS5;/* �擪�A�h���X�Z�b�g */
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                
            } else if (TNO == 8) {
                /* �ʐM�|�[�g�̊m�F�ł���ꍇ */
                TSB_PC_DATA_PC2 = 0;/* �u�U�[�o�͂��N���A���� */
                
                if (SUBTNO == 0) {
                    /* �ŏ��̏ꍇ */
                    TBUF0[0] = 0x0F;/* ���M�f�[�^�Z�b�g */
                    TBUF0[1] = 0x70;/* ���M�f�[�^�Z�b�g */
                    
                    /* ���M�O���� */
                    TXPTR0 = 0;/* �|�C���^�[�� */
                    TXLEN0 = TXSET0 = 2;/* 2�o�C�g�Z�b�g */
                    TXFLG0 |= 0x11;/* ���M���A���M�t���O���Z�b�g */
                    TXFLG0 &= 0xDF;/* ���M�o�b�t�@�G���v�e�B���N���A */
                    
                    /* �f�[�^���i�[�����M���� */
                    TSB_SC0->BUF = TBUF0[TXPTR0];/* �o�b�t�@�ɃZ�b�g */
                    
                    TXPTR0++;/* �߲����ݸ���� */
                    TXLEN0--;/* �޸���� */
                    TXSET0--;/* �޸���� */
                    
                    SUBTNO++;/* �C���N�������g */
                    
                } else if (SUBTNO == 1) {
                    /* ��M�҂� */
                    STSTM1++;/* �^�C�}�[���C���N�������g */
                    if (STSTM1 >= 10) {
                        /* 0.5�b�o�߂����ꍇ */
                        STSTM1 = 0;/* �^�C�}�N���A */
                        SUBTNO++;/* �C���N�������g */
                        
                    }
                    
                } else {
                    /* ���ʊm�F */
                    if (((TXFLG0 & 0x20) > 0) && ((TXFLG0 & 0x11) == 0) && (RXLEN0 == 2)
                    && (RBUF0[(RXPTR0 + 0) & 0x0FF] == 0x0F) && (RBUF0[(RXPTR0 + 1) & 0x0FF] == 0x70)) {
                        /* ���M�o�b�t�@�G���v�e�B�ŁA���M�����M�t���O���I�t�ł���A��M����2��
                        ���Ғʂ�̎�M�f�[�^�ł������ꍇ */
                        TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                        SUBTNO = 0;/* �N���A */
                        TNO = 9;/* ���̊m�F������ */
                        
                    } else {
                        /* �������������ꍇ */
                        SUBTNO = 0;/* �N���A */
                        
                    }
                    
                    RXPTR0 = 0;/* ��M�|�C���^�N���A */
                    RXLEN0 = 0;/* ��M���N���A */
                    
                    TXFLG0 &= 0xEE;/* ���M�t���O�N���A */
                    TXFLG0 |= 0x20;/* ���M�o�b�t�@�G���v�e�B���Z�b�g */
                    TXPTR0 = 0;/* ���M�|�C���^�N���A */
                    TXLEN0 = TXSET0 = 0;/* ���M���A���M�Z�b�g���N���A */
                    
                }
                
                DSPLN1 = TSTTOP[0];/* �f�[�^���Z�b�g */
                MHEAD1 = TSTTOP;/* �擪�A�h���X�Z�b�g */
                DSPLN2 = TSTMS6[0];/* �f�[�^���Z�b�g */
                MHEAD2 = TSTMS6;/* �擪�A�h���X�Z�b�g */
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                
            } else if (TNO == 9) {
                /* RTC�̊m�F�ł���ꍇ */
                TSB_PC_DATA_PC2 = 0;/* �u�U�[�o�͂��N���A���� */
                
                if (SUBTNO == 0) {
                    /* 1��ڊm�F�ł���ꍇ */
                    CRTCDT = RTCDT[0];/* �b�����[�h */
                    SUBTNO = 1;/* 1���Z�b�g */
                    
                } else {
                    /* 2��ڂ̊m�F�ł���ꍇ */
                    STSTM1++;/* �^�C�}���C���N�������g */
                    if (STSTM1 >= 30) {
                        /* 1.5�b�ȏ�o�߂����ꍇ */
                        STSTM1 = 0;/* �^�C�}�[�N���A */
                        SUBTNO = 0;/* �T�u�e�X�g�i���o�[�N���A */
                        
                        if (RTCDT[0] >= CRTCDT) {
                            /* ���ݕb�̕����ߋ��b�ȏ�ł���ꍇ */
                            REG_00 = (RTCDT[0] - CRTCDT);/* �����܂� */
                            
                        } else {
                            /* �t�̏ꍇ */
                            REG_00 = (RTCDT[0] + 60);/* 60�b���Z�Z�b�g */
                            REG_00 -= CRTCDT;/* �ߋ��b������ */
                            
                        }
                        
                        if ((REG_00 >= 1) && (REG_00 < 3)) {
                            /* �b�̍���1�ȏ�A3�����ł���ꍇ */
                            TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                            TNO = 10;/* ���̊m�F������ */
                            
                        }
                        
                    }
                    
                }
                
                DSPLN1 = TSTTOP[0];/* �f�[�^���Z�b�g */
                MHEAD1 = TSTTOP;/* �擪�A�h���X�Z�b�g */
                DSPLN2 = TSTMS7[0];/* �f�[�^���Z�b�g */
                MHEAD2 = TSTMS7;/* �擪�A�h���X�Z�b�g */
                
                DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                
            } else if (TNO == 10) {
                /* NV-RAM�̊m�F1�ł���ꍇ */
                TSB_PC_DATA_PC2 = 0;/* �u�U�[�o�͂��N���A���� */
                
                if (SUBTNO == 0) {
                    /* �T�u�e�X�g�i���o�[���O�̏ꍇ */
                    STSTM1++;/* �^�C�}�C���N�������g */
                    if (STSTM1 >= 20) {
                        /* 1�b�o�߂����ꍇ */
                        TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                        STSTM1 = 0;/* �^�C�}�N���A */
                        SUBTNO = 1;/* ���̏����� */
                        
                    }
                    
                    for (uint8_t i = 0;i < 32;i++) {
                        /* 32��J��Ԃ����� */
                        *HENNP[i] = 0;/* �f�[�^���N���A���� */
                        
                    }
                    
                    *HENNP[0] = 0x01;/* PASS1 */
                    *HENNP[1] = 0xAA;/* PASS2 */
                    *HENNP[4] = 0x0A;/* �p�^�[��1�ݒ艷�x�@10�� */
                    *HENNP[5] = 0x0A;/* �p�^�[��2�ݒ艷�x�@10�� */
                    *HENNP[6] = 0x0A;/* �p�^�[��3�ݒ艷�x�@10�� */
                    *HENNP[7] = 0x0A;/* �p�^�[��4�ݒ艷�x�@10�� */
                    *HENNP[9] = 20;/* �퉷���ݒ萅���@20�� */
                    *HENNP[20] = 0;/* �`���[�\�񎞊ԁi���j�@0�� */
                    *HENNP[21] = 8;/* �`���[�\�񎞊ԁi���j�@8�� */
                    *HENNP[25] = 5;/* �p�^�[��1���≷�x�@5�� */
                    *HENNP[26] = 5;/* �p�^�[��2���≷�x�@5�� */
                    *HENNP[27] = 5;/* �p�^�[��3���≷�x�@5�� */
                    *HENNP[28] = 5;/* �p�^�[��4���≷�x�@5�� */
                    *HENNP[29] = 11;/* �����݂̃p�^�[���I��l��"11"�ɂ���i�e�X�g�p�j�� */
                    
                    DSPLN1 = TSTTOP[0];/* �f�[�^���Z�b�g */
                    MHEAD1 = TSTTOP;/* �擪�A�h���X�Z�b�g */
                    DSPLN2 = TSTMS8[0];/* �f�[�^���Z�b�g */
                    MHEAD2 = TSTMS8;/* �擪�A�h���X�Z�b�g */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    
                } else {
                    /* �T�u�e�X�g�i���o�[��1�̏ꍇ */
                    DSPLN2 = TSTMSC[0];/* �f�[�^���Z�b�g */
                    MHEAD2 = TSTMSC;/* �擪�A�h���X�Z�b�g */
                    
                    DLWSQ = 33;/* 2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    
                }
                
            } else if (TNO == 11) {
                /* NV-RAM�̊m�F2�ł���ꍇ */
                TSB_PC_DATA_PC2 = 0;/* �u�U�[�o�͂��N���A���� */
                
                if (SUBTNO == 0) {
                    /* �ŏ��̃V�[�P���X�ł���ꍇ */
                    STSTM1++;/* �^�C�}�C���N�������g */
                    if (STSTM1 >= 10) {
                        /* 0.5�b�o�߂����ꍇ */
                        TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                        STSTM1 = 0;/* �^�C�}�N���A */
                        SUBTNO = 1;/* ���� */
                        
                    }
                    
                    DSPLN1 = TSTTOP[0];/* �f�[�^�����Z�b�g */
                    MHEAD1 = TSTTOP;/* �擪�A�h���X���Z�b�g */
                    DSPLN2 = TSTMS8[0];/* �f�[�^�����Z�b�g */
                    MHEAD2 = TSTMS8;/* �擪�A�h���X���Z�b�g */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v����Ȃ����� */
                    
                } else {
                    /* �f�[�^�ǂݏo���`�F�b�N���ł���ꍇ */
                    NPAT = 0;/* "0"�ɂ��� */
                    
                    REG_00 = 0;/* ��U�N���A */
                    for (uint8_t j = 0;j < 31;j++) {
                        /* 31��J��Ԃ� */
                        if (NVRMITB[j] == *HENNP[j]) {
                            /* �l����v����ꍇ */
                            /* �������Ȃ� */
                            
                        } else {
                            /* �l����v���Ȃ��ꍇ */
                            REG_00 ++;/* �C���N�������g */
                            
                        }
                        
                    }/* �J��Ԃ������܂� */
                    
                    if (REG_00 == 0) {
                        /* �����ُ킪���������ꍇ */
                        STSTM1++;/* �^�C�}�C���N�������g */
                        if (STSTM1 >= 10) {
                            /* 0.5�b�o�߂����ꍇ */
                            TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                            STSTM1 = 0;/* �^�C�}�N���A */
                            SUBTNO = 0;/* �T�u�V�[�P���X�N���A */
                            TNO = 12;/* ���̊m�F�� */
                            
                        }
                        
                        
                        DSPLN2 = TSTMS9[0];/* �f�[�^���Z�b�g */
                        MHEAD2 = TSTMS9;/* �擪�A�h���X�Z�b�g */
                        
                    } else {
                        /* �����ُ킪�������ꍇ */
                        DSPLN2 = TSTMSA[0];/* �f�[�^���Z�b�g */
                        MHEAD2 = TSTMSA;/* �擪�A�h���X�Z�b�g */
                        
                    }
                    
                    DLWSQ = 33;/* 2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v�ɓ���Ȃ����� */
                    
                }
                
            } else if (TNO == 12) {
                /* HACCP�o�͐ړ_�̊m�F�ł���ꍇ */
                TSB_PC_DATA_PC2 = 0;/* �u�U�[�o�͂��N���A���� */
                
                if (SUBTNO == 0) {
                    /* �ŏ��̃V�[�P���X�ł���ꍇ */
                    STSTM1++;/* �^�C�}�C���N�������g */
                    if (STSTM1 >= 10) {
                        /* 0.5�b�o�߂����ꍇ */
                        TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                        STSTM1 = 0;/* �^�C�}�N���A */
                        SUBTNO = 1;/* ���� */
                        
                    }
                    
                    TSB_PE->DATA &= 0xF3;/* ��p�^�]�E�ُ�o�͂��I�t�ɂ��� */
                    
                    DSPLN1 = TSTTOP[0];/* �f�[�^�����Z�b�g */
                    MHEAD1 = TSTTOP;/* �擪�A�h���X���Z�b�g */
                    DSPLN2 = TSTMSD[0];/* �f�[�^�����Z�b�g */
                    MHEAD2 = TSTMSD;/* �擪�A�h���X���Z�b�g */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v����Ȃ����� */
                    
                } else if (SUBTNO == 1) {
                    /* 2�Ԗڂ̃V�[�P���X�ł���ꍇ */
                    STSTM1++;/* �^�C�}�C���N�������g */
                    if (STSTM1 >= 40) {
                        /* 2�b�o�߂����ꍇ */
                        TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                        STSTM1 = 0;/* �^�C�}�N���A */
                        SUBTNO = 2;/* ���� */
                        
                    }
                    
                    TSB_PE->DATA |= 0x04;/* ��p�^�]�o�͂��I���ɂ��� */
                    
                    DSPLN1 = TSTTOP[0];/* �f�[�^�����Z�b�g */
                    MHEAD1 = TSTTOP;/* �擪�A�h���X���Z�b�g */
                    DSPLN2 = TSTMSD[0];/* �f�[�^�����Z�b�g */
                    
                    memcpy(DSPLB2, TSTMSD + 1, 16);/* �������[�R�s�[ */
                    DSPLB2[11] = NUMDEG[1];/* �����f�[�^�u�P�v�Z�b�g */
                    MHEAD2 = DSPLB2;/* �擪�A�h���X���Z�b�g */
                    MHEAD2 --;/* �A�h���X�f�N�������g */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v����Ȃ����� */
                    
                } else if (SUBTNO == 2) {
                    /* 3�Ԗڂ̃V�[�P���X�ł���ꍇ */
                    STSTM1++;/* �^�C�}�C���N�������g */
                    if (STSTM1 >= 40) {
                        /* 2�b�o�߂����ꍇ */
                        TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                        STSTM1 = 0;/* �^�C�}�N���A */
                        SUBTNO = 3;/* ���̊m�F�� */
                        
                    }
                    
                    TSB_PE->DATA |= 0x08;/* �ُ�o�͂��I���ɂ��� */
                    
                    DSPLN1 = TSTTOP[0];/* �f�[�^�����Z�b�g */
                    MHEAD1 = TSTTOP;/* �擪�A�h���X���Z�b�g */
                    DSPLN2 = TSTMSD[0];/* �f�[�^�����Z�b�g */
                    
                    memcpy(DSPLB2, TSTMSD + 1, 16);/* �������[�R�s�[ */
                    DSPLB2[11] = NUMDEG[2];/* �����f�[�^�u�Q�v�Z�b�g */
                    MHEAD2 = DSPLB2;/* �擪�A�h���X���Z�b�g */
                    MHEAD2 --;/* �A�h���X�f�N�������g */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v����Ȃ����� */
                    
                } else if (SUBTNO == 3) {
                    /* 4�Ԗڂ̃V�[�P���X�ł���ꍇ */
                    STSTM1++;/* �^�C�}�C���N�������g */
                    if (STSTM1 >= 40) {
                        /* 2�b�o�߂����ꍇ */
                        TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                        STSTM1 = 0;/* �^�C�}�N���A */
                        SUBTNO = 4;/* ���̊m�F�� */
                        
                    }
                    
                    TSB_PE->DATA &= 0xFB;/* ��p�o�͂��I�t�ɂ��� */
                    
                    DSPLN1 = TSTTOP[0];/* �f�[�^�����Z�b�g */
                    MHEAD1 = TSTTOP;/* �擪�A�h���X���Z�b�g */
                    DSPLN2 = TSTMSD[0];/* �f�[�^�����Z�b�g */
                    
                    memcpy(DSPLB2, TSTMSD + 1, 16);/* �������[�R�s�[ */
                    DSPLB2[11] = NUMDEG[3];/* �����f�[�^�u�R�v�Z�b�g */
                    MHEAD2 = DSPLB2;/* �擪�A�h���X���Z�b�g */
                    MHEAD2 --;/* �A�h���X�f�N�������g */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v����Ȃ����� */
                    
                } else if (SUBTNO == 4) {
                    /* 5�Ԗڂ̃V�[�P���X�ł���ꍇ */
                    STSTM1++;/* �^�C�}�C���N�������g */
                    if (STSTM1 >= 50) {
                        /* 2.5�b�o�߂����ꍇ */
                        TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                        STSTM1 = 0;/* �^�C�}�N���A */
                        SUBTNO = 0;/* �T�u�V�[�P���X�N���A */
                        TNO = 13;/* ���̊m�F�� */
                        
                    }
                    
                    TSB_PE->DATA &= 0xF3;/* ��p�E�ُ�o�͂��I�t�ɂ��� */
                    
                    DSPLN1 = TSTTOP[0];/* �f�[�^�����Z�b�g */
                    MHEAD1 = TSTTOP;/* �擪�A�h���X���Z�b�g */
                    DSPLN2 = TSTMSD[0];/* �f�[�^�����Z�b�g */
                    
                    memcpy(DSPLB2, TSTMSD + 1, 16);/* �������[�R�s�[ */
                    DSPLB2[11] = NUMDEG[4];/* �����f�[�^�u�S�v�Z�b�g */
                    MHEAD2 = DSPLB2;/* �擪�A�h���X���Z�b�g */
                    MHEAD2 --;/* �A�h���X�f�N�������g */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v����Ȃ����� */
                    
                }
                
            } else if (TNO == 13) {
                /* DAC�o�͒l / ��d���͂̊m�F�ł���ꍇ */
                TSB_PC_DATA_PC2 = 0;/* �u�U�[�o�͂��N���A���� */
                
                if (SUBTNO == 0) {
                    /* �ŏ��̃V�[�P���X�ł���ꍇ */
                    STSTM1++;/* �^�C�}�C���N�������g */
                    if (STSTM1 >= 10) {
                        /* 0.5�b�o�߂����ꍇ */
                        TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
                        STSTM1 = 0;/* �^�C�}�N���A */
                        SUBTNO = 1;/* ���� */
                        
                    }
                    
                    DSPLN1 = TSTTOP[0];/* �f�[�^�����Z�b�g */
                    MHEAD1 = TSTTOP;/* �擪�A�h���X���Z�b�g */
                    DSPLN2 = TSTMSE[0];/* �f�[�^�����Z�b�g */
                    MHEAD2 = TSTMSE;/* �擪�A�h���X���Z�b�g */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v����Ȃ����� */
                    
                } else {
                    /* 2�Ԗڈȍ~�̃V�[�P���X�ł���ꍇ */
                    DSPLN1 = TSTTOP[0];/* �f�[�^�����Z�b�g */
                    MHEAD1 = TSTTOP;/* �擪�A�h���X���Z�b�g */
                    DSPLN2 = TSTMSE[0];/* �f�[�^�����Z�b�g */
                    
                    memcpy(DSPLB2, TSTMSE + 1, 16);/* �������[�R�s�[ */
                    DSPLB2[4] = NUMDEG[((DACOUT % 10000) / 1000)];/* �����̕����f�[�^�Z�b�g1 */
                    DSPLB2[5] = NUMDEG[((DACOUT % 1000) / 100)];/* �����̕����f�[�^�Z�b�g2 */
                    DSPLB2[6] = NUMDEG[((DACOUT % 100) / 10)];/* �����̕����f�[�^�Z�b�g3 */
                    DSPLB2[7] = NUMDEG[(DACOUT % 10)];/* �����̕����f�[�^�Z�b�g4 */
                    
                    if ((MXINO1 & 0x08) > 0) {
                        /* ��d���͂�����ꍇ */
                        DSPLB2[13] = 'N';/* ON��N */
                        DSPLB2[14] = ' ';/* OFF��F������ */
                        
                    }
                    
                    MHEAD2 = DSPLB2;/* �擪�A�h���X���Z�b�g */
                    MHEAD2 --;/* �A�h���X�f�N�������g */
                    
                    DLWSQ = 29;/* 1,2�s�ڕ\������J�n */
                    i = 0;/* �ēx���[�v����Ȃ����� */
                    
                }
                
            }
            
            break;/* �����܂� */
            
        case 25:
            /* 1�s��2�s�ڂ̕\�����N���A���鏈�� */
            TSB_PF->CR &= 0xF0;/* �o�͋@�\���I�t */
            TSB_PF->IE |= 0x0F;/* ���͋@�\���I�� */
            TSB_PA_DATA_PA6 = 1;/* RW���I������ */
            
            DLWSQ++;/* �ԍ��C���N�������g */
            /* �u���[�N�͏����Ȃ� */
            
        case 26:
            /* �r�W�[�t���O�`�F�b�N */
            if ((EPBSY() & 0x08) > 0) {
                /* �r�W�[�t���O���I���ł���ꍇ */
                OS_LoadTime->NEXT_LOAD_Time[5] = 0;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[5] = 0x80;/* �x���t���O���z */
                
                FLZFG |= 0x01;/* �t���O�Z�b�g */
                if (FLZTM >= 60000) {
                    /* 1���ȏ�o�߂����ꍇ */
                    FLZFG = DLWSQ = 0;/* ��蒼�� */
                    FLZTM = 0;/* �^�C�}�N���A */
                    
                }
                
                return;/* �߂� */
                
            }
            
            FLZFG = 0;/* �N���A */
            DLWSQ++;/* �ԍ��C���N�������g */
            /* �u���[�N�͏����Ȃ� */
            
        case 27:
            /* ��ʃN���A�R�}���h�̑��M */
            TSB_PA_DATA_PA6 = 0;/* RW�I�t�ɂ��� */
            TSB_PF->IE &= 0xF0;/* LCD�f�[�^�o�X�����͂ɂ��� */
            TSB_PF->CR |= 0x0F;/* LCD�f�[�^�o�X���o�͂ɂ��� */
            TSB_PF->DATA &= 0xF0;/* ��U�}�X�N */
            TSB_PF->DATA |= 0x00;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A�N���A�R�}���h */
            
            EPLS();/* E���p���X�o�͂��� */
            
            TSB_PF->DATA |= 0x01;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A�N���A�R�}���h */
            
            EPLS();/* E���p���X�o�͂��� */
            
            TSB_PF->CR &= 0xF0;/* �o�͋@�\���I�t */
            TSB_PF->IE |= 0x0F;/* ���͋@�\���I�� */
            TSB_PA_DATA_PA6 = 1;/* RW���I������ */
            
            DLWSQ++;/* �ԍ��C���N�������g */
            /* �u���[�N�͏����Ȃ� */
            
        case 28:
            /* �r�W�[�t���O�`�F�b�N */
            if ((EPBSY() & 0x08) > 0) {
                /* �r�W�[�t���O���I���ł���ꍇ */
                OS_LoadTime->NEXT_LOAD_Time[5] = 0;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[5] = 0x80;/* �x���t���O���z */
                
                FLZFG |= 0x01;/* �t���O�Z�b�g */
                if (FLZTM >= 60000) {
                    /* 1���ȏ�o�߂����ꍇ */
                    FLZFG = DLWSQ = 0;/* ��蒼�� */
                    FLZTM = 0;/* �^�C�}�N���A */
                    
                }
                
                return;/* �߂� */
                
            }
            
            FLZFG = 0;/* �N���A */
            DLWSQ = 31;/* �ԍ���31�ɂ��� */
            i = 0;/* �N���A */
            continue;/* ���[�v�擪�ɖ߂� */
            /* �u���[�N�͏����Ȃ� */
            
        case 29:
            /* 1,2�s�ڂ�\�����鏈�� */
            /* �܂���1�s�ڂ̕\���J�[�\�������킹�� */
            TSB_PA_DATA_PA6 = 0;/* RW�I�t�ɂ��� */
            TSB_PF->IE &= 0xF0;/* LCD�f�[�^�o�X�����͂ɂ��� */
            TSB_PF->CR |= 0x0F;/* LCD�f�[�^�o�X���o�͂ɂ��� */
            TSB_PF->DATA &= 0xF0;/* ��U�}�X�N */
            TSB_PF->DATA |= 0x08;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A1�s�ڃR�}���h */
            
            EPLS();/* E���p���X�o�͂��� */
            
            TSB_PF->DATA &= 0xF0;/* ��U�}�X�N */
            TSB_PF->DATA |= 0x00;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A1�s�ڃR�}���h */
            
            EPLS();/* E���p���X�o�͂��� */
            
            TSB_PF->CR &= 0xF0;/* �o�͋@�\���I�t */
            TSB_PF->IE |= 0x0F;/* ���͋@�\���I�� */
            TSB_PA_DATA_PA6 = 1;/* RW���I������ */
            
            DLWSQ++;/* �ԍ��C���N�������g */
            /* �u���[�N�͏����Ȃ� */
            
        case 30:
            /* �r�W�[�t���O�̊m�F */
            if ((EPBSY() & 0x08) > 0) {
                /* �r�W�[�t���O���I���ł���ꍇ */
                OS_LoadTime->NEXT_LOAD_Time[5] = 0;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[5] = 0x80;/* �x���t���O���z */
                
                FLZFG |= 0x01;/* �t���O�Z�b�g */
                if (FLZTM >= 60000) {
                    /* 1���ȏ�o�߂����ꍇ */
                    FLZFG = DLWSQ = 0;/* ��蒼�� */
                    FLZTM = 0;/* �^�C�}�N���A */
                    
                }
                
                return;/* �߂� */
                
            }
            
            FLZFG = 0;/* �N���A */
            DLWSQ++;/* �ԍ��C���N�������g */
            /* �u���[�N�͏����Ȃ� */
            
        case 31:
            /* 1�s�ڂɕ�����\�����鏈�� */
            TSB_PA_DATA_PA6 = 0;/* R/W���I�t */
            TSB_PA_DATA_PA7 = 1;/* RS���I�� */
            
            MHEAD1++;/* �A�h���X�C���N�������g */
            
            REG_00 = *MHEAD1;/* ���̂̐������Z�b�g */
            REG_01 = REG_00;/* �����ЂƂɂ��R�s�[ */
            REG_00 >>= 4;/* 4�r�b�g�E�V�t�g */
            REG_00 &= 0x0F;/* �K�v�ȕ������}�X�N */
            REG_01 &= 0x0F;/* �K�v�ȕ������}�X�N */
            
            TSB_PF->IE &= 0xF0;/* LCD�f�[�^�o�X�����͂ɂ��� */
            TSB_PF->CR |= 0x0F;/* LCD�f�[�^�o�X���o�͂ɂ��� */
            TSB_PF->DATA &= 0xF0;/* ��U�}�X�N */
            TSB_PF->DATA |= REG_00;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A�f�[�^��� */
            
            EPLS();/* E���p���X�o�͂��� */
            
            TSB_PF->DATA &= 0xF0;/* ��U�}�X�N */
            TSB_PF->DATA |= REG_01;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A�f�[�^���� */
            
            EPLS();/* E���p���X�o�͂��� */
            
            TSB_PF->CR &= 0xF0;/* �o�͋@�\���I�t */
            TSB_PF->IE |= 0x0F;/* ���͋@�\���I�� */
            TSB_PA_DATA_PA6 = 1;/* RW���I������ */
            TSB_PA_DATA_PA7 = 0;/* RS���I�t���� */
            
            DLWSQ++;/* �ԍ��C���N�������g */
            /* �u���[�N�͏����Ȃ� */
            
        case 32:
            /* �r�W�[�t���O�̊m�F�Ƃ܂������\���c�����邩�m�F */
            if ((EPBSY() & 0x08) > 0) {
                /* �r�W�[�t���O���I���ł���ꍇ */
                OS_LoadTime->NEXT_LOAD_Time[5] = 0;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[5] = 0x80;/* �x���t���O���z */
                
                FLZFG |= 0x01;/* �t���O�Z�b�g */
                if (FLZTM >= 60000) {
                    /* 1���ȏ�o�߂����ꍇ */
                    FLZFG = DLWSQ = 0;/* ��蒼�� */
                    FLZTM = 0;/* �^�C�}�N���A */
                    
                }
                
                return;/* �߂� */
                
            }
            
            FLZFG = 0;/* �N���A */
            if (DSPLN1 > 0) {
                /* 1�ȏ�̏ꍇ */
                DSPLN1--;/* �f�N�������g */
                
            }
            
            if (DSPLN1 == 0) {
                /* ���M������̂�0�ł���ꍇ */
                DLWSQ++;/* �ԍ��C���N�������g */
                
            } else {
                /* ���M������̂��܂�����ꍇ */
                DLWSQ = 31;/* �ēx31�ɃZ�b�g */
                i = 0;/* �J��Ԃ��ł���l�ɒl���N���A */
                continue;/* ���[�v�̐擪�� */
                
            }
            /* �u���[�N�͏����Ȃ� */
            
        case 33:
            /* 2�s�ڂ�\�����鏈�� */
            /* 2�s�ڂ̕\���J�[�\�������킹�� */
            TSB_PA_DATA_PA6 = 0;/* RW�I�t�ɂ��� */
            TSB_PA_DATA_PA7 = 0;/* RS�I�t�ɂ��� */
            TSB_PF->IE &= 0xF0;/* LCD�f�[�^�o�X�����͂ɂ��� */
            TSB_PF->CR |= 0x0F;/* LCD�f�[�^�o�X���o�͂ɂ��� */
            TSB_PF->DATA &= 0xF0;/* ��U�}�X�N */
            TSB_PF->DATA |= 0x0C;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A2�s�ڃR�}���h */
            
            EPLS();/* E���p���X�o�͂��� */
            
            TSB_PF->DATA &= 0xF0;/* ��U�}�X�N */
            TSB_PF->DATA |= 0x00;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A2�s�ڃR�}���h */
            
            EPLS();/* E���p���X�o�͂��� */
            
            TSB_PF->CR &= 0xF0;/* �o�͋@�\���I�t */
            TSB_PF->IE |= 0x0F;/* ���͋@�\���I�� */
            TSB_PA_DATA_PA6 = 1;/* RW���I������ */
            
            DLWSQ++;/* �ԍ��C���N�������g */
            /* �u���[�N�͏����Ȃ� */
            
        case 34:
            /* �r�W�[�t���O�̊m�F */
            if ((EPBSY() & 0x08) > 0) {
                /* �r�W�[�t���O���I���ł���ꍇ */
                OS_LoadTime->NEXT_LOAD_Time[5] = 0;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[5] = 0x80;/* �x���t���O���z */
                
                FLZFG |= 0x01;/* �t���O�Z�b�g */
                if (FLZTM >= 60000) {
                    /* 1���ȏ�o�߂����ꍇ */
                    FLZFG = DLWSQ = 0;/* ��蒼�� */
                    FLZTM = 0;/* �^�C�}�N���A */
                    
                }
                
                return;/* �߂� */
                
            }
            
            FLZFG = 0;/* �N���A */
            DLWSQ++;/* �ԍ��C���N�������g */
            /* �u���[�N�͏����Ȃ� */
            
        case 35:
            /* 2�s�ڂɕ�����\�����鏈�� */
            TSB_PA_DATA_PA6 = 0;/* R/W���I�t */
            TSB_PA_DATA_PA7 = 1;/* RS���I�� */
            
            MHEAD2++;/* �A�h���X�C���N�������g */
            
            REG_00 = *MHEAD2;/* ���̂̐������Z�b�g */
            REG_01 = REG_00;/* �����ЂƂɂ��R�s�[ */
            REG_00 >>= 4;/* 4�r�b�g�E�V�t�g */
            REG_00 &= 0x0F;/* �K�v�ȕ������}�X�N */
            REG_01 &= 0x0F;/* �K�v�ȕ������}�X�N */
            
            TSB_PF->IE &= 0xF0;/* LCD�f�[�^�o�X�����͂ɂ��� */
            TSB_PF->CR |= 0x0F;/* LCD�f�[�^�o�X���o�͂ɂ��� */
            TSB_PF->DATA &= 0xF0;/* ��U�}�X�N */
            TSB_PF->DATA |= REG_00;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A�f�[�^��� */
            
            EPLS();/* E���p���X�o�͂��� */
            
            TSB_PF->DATA &= 0xF0;/* ��U�}�X�N */
            TSB_PF->DATA |= REG_01;/* LCD�f�[�^�o�X�o�͂��Z�b�g�A�f�[�^���� */
            
            EPLS();/* E���p���X�o�͂��� */
            
            TSB_PF->CR &= 0xF0;/* �o�͋@�\���I�t */
            TSB_PF->IE |= 0x0F;/* ���͋@�\���I�� */
            TSB_PA_DATA_PA6 = 1;/* RW���I������ */
            TSB_PA_DATA_PA7 = 0;/* RS���I�t���� */
            
            DLWSQ++;/* �ԍ��C���N�������g */
            /* �u���[�N�͏����Ȃ� */
            
        case 36:
            /* �r�W�[�t���O�̊m�F�Ƃ܂������\���c�����邩�m�F */
            if ((EPBSY() & 0x08) > 0) {
                /* �r�W�[�t���O���I���ł���ꍇ */
                OS_LoadTime->NEXT_LOAD_Time[5] = 0;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[5] = 0x80;/* �x���t���O���z */
                
                FLZFG |= 0x01;/* �t���O�Z�b�g */
                if (FLZTM >= 60000) {
                    /* 1���ȏ�o�߂����ꍇ */
                    FLZFG = DLWSQ = 0;/* ��蒼�� */
                    FLZTM = 0;/* �^�C�}�N���A */
                    
                }
                
                return;/* �߂� */
                
            }
            
            FLZFG = 0;/* �N���A */
            if (DSPLN2 > 0) {
                /* 1�ȏ�̏ꍇ */
                DSPLN2--;/* �f�N�������g */
                
            }
            
            if (DSPLN2 == 0) {
                /* ���M������̂�0�ł���ꍇ */
                DLWSQ = 10;/* ����̊J�n�ʒu���Z�b�g���� */
                i = 1;/* �����I�ɌJ��Ԃ����I��点�� */
                
            } else {
                /* ���M������̂��܂�����ꍇ */
                DLWSQ = 35;/* �ēx35�ɃZ�b�g */
                i = 0;/* �J��Ԃ��ł���l�ɒl���N���A */
                continue;/* ���[�v�̐擪�� */
                
            }
            
            break;/* �����܂� */
            
        default:
            /* ���Ă͂܂�Ȃ��ꍇ�͂����ɔ����� */
            DLWSQ = 0;/* ���Z�b�g���邺 */
            break;/* ������ */
            
        }/* �X�C�b�`���͂����܂� */
        
    }/* ���[�v�͂����܂� */
    
    OS_LoadTime->NEXT_LOAD_Time[5] = 50;/* ����Ăяo������ */
    OS_LoadTime->TSK_COND[5] = 0xA0;/* �x���t���O���z */
    
    return;/* �߂� */
    
}

/***
 * �薼�FE�p���X�o�̓��[�`��
 * �쐬�ҁF����@����
 * LCD/VFD�\�����ɕK�v��E�p���X����
***/
void EPLS(void)
{
    TSB_PA_DATA_PA5 = 1;/* E�M����HIGH�ɂ��� */
    for (uint8_t i = 0;i < 5;i++) {
        __NOP();/* �^�C�~���O���炵 */
        
    }
    TSB_PA_DATA_PA5 = 0;/* E�M����LOW�ɂ��� */
    
    return;/* �߂� */
    
}

/***
 * �薼�FP�r�W�[�t���O�̃`�F�b�N���[�`��
 * �쐬�ҁF����@����
 * LCD/VFD�������ɕK�v��P�r�W�[�t���O�`�F�b�N���[�`��
***/
uint8_t EPBSY(void)
{
    uint8_t REG_00 = 0;/* �ꎞ�ϐ����Z�b�g */
    
    TSB_PA_DATA_PA5 = 1;/* E�M����HIGH�ɂ��� */
    for (uint8_t i = 0;i < 5;i++) {
        __NOP();/* �^�C�~���O���炵 */
        
    }
    REG_00 = TSB_PF->DATA;/* PF�|�[�g�̃f�[�^�����[�h */
    REG_00 &= 0x0F;/* 4bit�Ƀ}�X�N */
    TSB_PA_DATA_PA5 = 0;/* E�M����LOW�ɂ��� */
    
    for (uint8_t i = 0;i < 5;i++) {
        __NOP();/* �^�C�~���O���炵 */
        
    }
    TSB_PA_DATA_PA5 = 1;/* E�M����HIGH�ɂ��� */
    for (uint8_t i = 0;i < 5;i++) {
        __NOP();/* �^�C�~���O���炵 */
        
    }
    TSB_PA_DATA_PA5 = 0;/* E�M����LOW�ɂ��� */
    
    return REG_00;/* �l���Z�b�g���Ė߂� */
    
}

/***
 * �薼�F�������[�N���A��t���̕\��
 * �쐬�ҁF����@����
 * �������[�N���A��t���\�����o��
***/
void MCLRMS(void)
{
    TSB_PF_DATA_PF4 = 1;/* �o�b�N���C�g�o�͂��I������ */
    DSPLN1 = MCLMS0[0];/* 1�s�ڂ̃f�[�^�����Z�b�g���� */
    MHEAD1 = &(MCLMS0[0]);/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g���� */
    
    DSPLN2 = MCLMS1[0];/* 1�s�ڂ̃f�[�^�����Z�b�g���� */
    MHEAD2 = &(MCLMS1[0]);/* ���b�Z�[�W�̐擪�A�h���X���Z�b�g���� */
    
    TSB_PC_DATA_PC2 = 1;/* �u�U�[�I�� */
    
    STSTM1++;/* �^�C�}�C���N�������g */
    if (STSTM1 >= 10) {
        /* 0.5�b�o�߂����ꍇ */
        STSTM1 = 0;/* �^�C�}�N���A */
        TSB_PC_DATA_PC2 = 0;/* �u�U�[�I�t */
        STSFL1 &= 0xFD;/* �������[�N���A��t�����N���A */
        
        MCLAST = 0;/* �������[�N���A��t�^�C�}���N���A���� */
        
    }
    
    return;/* �߂� */
    
}