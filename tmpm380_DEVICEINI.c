/**
 *�薼�F�������A�v��
 *�쐬�ҁF����@����
 *�������ɕK�v�ȏ��������s����A�v���P�[�V����
**/
#include "tmpm380_DEVICEINI.h"

void DEVICE_INIT(void)
{
    OS_TSKCon1 *OS_LoadTime;/* OS�ݒ�l��ǂނ��߂̃|�C���^�\���̂ɖ��O������ */
    OS_LoadTime = OSTSKCon1S();/* �֐��o�R�Ń|�C���^��OSparam�ɃA�N�Z�X */
    
    uint8_t REG_00, REG_01 = 0;/* �ꎞ�����ϐ� */
    
    /* �ȉ��A�������A�v�������s���鏈�� */
    WDTCLR();/* �O�������ޯ����ς̸ر���� */
    
    if (D_INISQ == 0) {
        /* ����N�������� */
        /* �ި��߽����̓ǂݏo������ */
        while (1) {
            /* ���[�v�������� */
            NVIC_EnableIRQ(INTTB00_IRQn);/* �^�C�}���荞�݋��� */
            
            while (count01 < 5);/* 5ms�҂��܂� */
            TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
            count01 = 0;/* �J�E���g��0�N���A */
            while (count01 < 5);/* 5ms�҂��܂� */
            TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
            count01 = 0;
            WDTCLR();/* �O�t���E�H�b�`�h�b�O�^�C�}�[�̃N���A */
            
            DIPBUF = (MXIN1 & 0xF0);/* ���݃f�B�b�v�X�C�b�`�̒l�������� */
            
            while (count01 < 5);/* 5ms�҂��܂� */
            TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
            count01 = 0;/* �J�E���g��0�N���A */
            while (count01 < 5);/* 5ms�҂��܂� */
            TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
            count01 = 0;
            WDTCLR();/* �O�t���E�H�b�`�h�b�O�^�C�}�[�̃N���A */
            while (count01 < 5);/* 5ms�҂��܂� */
            TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
            count01 = 0;/* �J�E���g��0�N���A */
            while (count01 < 5);/* 5ms�҂��܂� */
            TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
            count01 = 0;
            WDTCLR();/* �O�t���E�H�b�`�h�b�O�^�C�}�[�̃N���A */
            
            if ((MXIN1 & 0xF0) == DIPBUF) {
                /* �ި��߽����̎�荞�݂�����ł���ꍇ */
                SYKND0 = DIPBUF;/* �@����ɕۑ����� */
                break;/* �����܂� */
                
            }
            
        }/* ���[�v�����܂� */
        
        TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
        count01 = 0;
        WDTCLR();/* �O�t���E�H�b�`�h�b�O�^�C�}�[�̃N���A */
        
        /* NV-RAM����̃f�[�^�ǂݏo������ */
        SETUP();/* �ǂݏo������ */
        
        ABNFO = (ABNF & 0x0F);/* �I�[���h�ɕۑ����� */
        if ((SYSFLG & 0x40) > 0) {
            /* �d��ON�t���O������ꍇ */
            DSPNO = 1;/* �g�b�v��ʂɃZ�b�g */
            
        }
        
        if ((SYSFLG & 0x02) > 0) {
            /* �^�]���t���O���������ꍇ */
            if (((ABNF & 0x0F) == 0) || ((ABNF & 0x0F) == 11)) {
                /* ����œd���������Ă����A�܂��͒�d�ُ�ł������ꍇ */
                ABNF |= 0x0B;/* 11�i0x0B�j��d�ُ���Z�b�g */
                ABNF |= 0x80;/* �u�U�[�t���O��ON */
                STSFL0 |= 0x20;/* �������̃t���O���Z�b�g */
                
            }/* ��d�ُ�ȊO�ُ̈�ł������ꍇ�͂����ɔ����� */
            
        }/* �^�]���t���O���������ꍇ�͂����ɔ����� */
        
        if ((RPAT & 0x20) > 0) {
            /* ���H�����ł������ꍇ */
            DSPSEQ = 4;/* ��򃂁[�h�Z�b�g */
            DSPSSQ = 2;/* ���H���\�����Z�b�g */
            STSFL0 |= 0x80;/* �ݒ胂�[�h���Z�b�g */
            
        }
        
        if (NPAT == 11) {
            /* NPAT�̒l��11�ł������ꍇ */
            TNO = 11;/* �Z���t�e�X�g�i���o�[��11�ɂ��� */
            
        }
        
        WDTCLR();/* �O�t���E�H�b�`�h�b�O�^�C�}�[�̃N���A */
        TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
        count01 = 0;/* �J�E���g�N���A */
        
        /* I2C�o�X��RTC��DAC�̊m�F���s������ */
        I2C_WCNT = 0;/* I2C�����݉񐔂�ر */
        I2C_RCNT = 0;/* I2C�ǂݍ��ݏ����񐔂��N���A */
        I2C_STATE = 0;/* I2C���荞�ݽðĂ�����p�ɂ��� */
        
        SBI_I2CState i2c_state;/* I2C�o�X��Ԃ�����\���̂ɖ��O��t���� */
        
        NVIC_EnableIRQ(INTSBI0_IRQn);/* I2C�ʐM���荞�݂̋��� */
        SBI_GenerateI2CStop(TSB_SBI0);/* I2C�ʐM�̽į�߂�� */
        
        i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
        while (i2c_state.Bit.BusState == 1) {
            /* �o�X�r�W�[�ł���ꍇ */
            SBI_GenerateI2CStop(TSB_SBI0);/* �ŏ��ɽį�ߺ��ި��݂�� */
            while (count01 < 5);/* 5ms�҂� */
            WDTCLR();/* �O�t���E�H�b�`�h�b�O�^�C�}�[�̃N���A */
            TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
            count01 = 0;/* �J�E���g�N���A */
            
            I2C_RSTE();/* I2C�޽ײ݂�ر */
            
            i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
            
        }
        
        /* RTC�̏����ݒ�Ȃǂ̊m�F���� */
        /* �_�~�[���[�h���� */
        T0BF1 = 0;/* �ǂݏo�������p�ɃN���A */
        I2C_RCNT = 1;/* ��M�񐔾��(�����ɃX�g�b�v���鏈��) */
        
        i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
        for (uint8_t j = 0;j < 10;j++) {
            /* �_�~�[���[�h��10��g���C�A�� */
            SBI_SetSendData(TSB_SBI0, ((RTC_ADDR) | 0x01));/* RTC�Ǐo�� */
            SBI_GenerateI2CStart(TSB_SBI0);/* I2C�X�^�[�g������B���M�I����A���肱�ݔ��� */
            
            count01 = 0;/* �J�E���g�N���A */
            while (count01 < 5);/* 5ms�҂� */
            count01 = 0;/* �J�E���g�N���A */
            while (count01 < 5);/* 5ms�҂� */
            TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
            WDTCLR();/* �O�t���E�H�b�`�h�b�O�^�C�}�[�̃N���A */
            count01 = 0;/* �J�E���g�N���A */
            
            if ((I2C_RCNT > 2) && (T0BF1 == 1)) {
                /* �_�~�[�Ǐo���ł����ꍇ */
                SBI_SetI2CACK(TSB_SBI0, ENABLE);/* ACK�𖳌������Ă��邽�߁A�����ŗL���ɂ��� */
                break;
                
            } else {
                /* �_�~�[�Ǐo���o���Ă��Ȃ��ꍇ */
                i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
                while (i2c_state.Bit.BusState == 1) {
                    /* �o�X�r�W�[�ł���ꍇ */
                    SBI_GenerateI2CStop(TSB_SBI0);/* �ŏ��ɽį�ߺ��ި��݂�� */
                    count01 = 0;/* �J�E���g�N���A */
                    while (count01 < 5);/* 10ms�҂� */ 
                    TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
                    WDTCLR();/* �O�t���E�H�b�`�h�b�O�^�C�}�[�̃N���A */
                    
                    I2C_RSTE();/* I2C�޽ײ݂�ر */
                    
                    i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
                    
                }
                
            }
            
            SBI_SetI2CACK(TSB_SBI0, ENABLE);/* ACK�𖳌������Ă��邽�߁A�����ŗL���ɂ��� */
            
        }
        
        T0BF1 = 0;/* �ر */
        SBI_GenerateI2CStop(TSB_SBI0);/* �į�ߺ��ި��݂�� */
        
        /* VLF�ޯĂ̊m�F */
        I2C_STATE = 0;/* ���荞�݃X�e�[�g�ύX */
        I2C_WCNT = 0;/* ���񏑍��݃J�E���g��0���Z�b�g */
        I2C_RCNT = 0;/* ����Ǎ��݃J�E���g��0���Z�b�g */
        T0BF0 |= 0x02;/* ����VLF�׸ނ��(RTC�����������̈�) */
        
        count01 = 0;/* �J�E���g�N���A */
        for (uint8_t i = 0;i < 10;i++) {
            i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
            if (i2c_state.Bit.BusState == 1) {
                /* �o�X�r�W�[�ł���ꍇ */
                SBI_GenerateI2CStop(TSB_SBI0);/* �į�ߺ��ި��݂�� */
                count01 = 0;/* �J�E���g�N���A */
                while (count01 < 5);/* 5ms�҂� */
                TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
                WDTCLR();/* �O�t���E�H�b�`�h�b�O�^�C�}�[�̃N���A */
                count01 = 0;/* �J�E���g�N���A */
                
                I2C_RSTE();/* I2C�޽ײ݂�ر */
                
            } else {
                /* �o�X�t���[�m�F���o�����ꍇ */
                break;
                
            }
            
            if (i == 9) {
                /* �Ō�̏ꍇ */
                RTC_ERR |= 0x01;/* RTC�ُ킪�������t���O���Z�b�g */
            }
            
        }
        
        i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
        if (i2c_state.Bit.BusState == 0) {
            /* �o�X�t���[�ł���ꍇ */
            T0BF1 = 0;/* �g�p�����ޯ̧�N���A */
            SBI_SetSendData(TSB_SBI0, ((RTC_ADDR) | 0x00));/* �_�~�[������ */
            SBI_GenerateI2CStart(TSB_SBI0);/* I2C�X�^�[�g������B���M�I����A���肱�ݔ��� */
            
            count01 = 0;/* �J�E���g�N���A */
            while (count01 < 5);/* 5ms�҂��܂� */
            TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
            count01 = 0;/* �J�E���g�N���A */
            
            i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
            if ((I2C_WCNT >= 2) && (i2c_state.Bit.BusState == 0)) {
                /* RTC�ƒʐM�\�ł���ꍇ */
                SBI_SetSendData(TSB_SBI0, (RTC_ADDR | 0x01));/* �X�^�[�g��A�X���[�u�A�h���X�𑗂�B�����ēǍ��� */
                SBI_GenerateI2CStart(TSB_SBI0);/* I2C�X�^�[�g��Ԃɂ�����B���M�I����A���肱�ݔ��� */
                count01 = 0;/* �J�E���g�N���A */
                while (count01 < 5);/* 5ms�҂��܂� */
                TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
                WDTCLR();/* �N���A�[ */
                count01 = 0;/* �J�E���g�N���A */
                
                if ((I2C_RCNT >= 3) && (T0BF1 == 1)) {
                    /* ��M���������ꍇ */
                    RTC_ERR = 0;/* RTC�ُ���N���A */
                } else {
                    RTC_ERR |= 0x01;/* RTC�ُ킪�������t���O���Z�b�g */
                }
            } else {
                /* RTC�ƒʐM�o���Ȃ��ꍇ */
                RTC_ERR |= 0x01;/* RTC�ُ킪�������t���O���Z�b�g */
            }
        } else {
            /* �o�X�r�W�[�ł���ꍇ */
            RTC_ERR |= 0x01;/* RTC�ُ킪�������t���O���Z�b�g */
        }
        
        SBI_SetI2CACK(TSB_SBI0, ENABLE);/* ACK�𖳌������Ă��邽�߁A�����ŗL���ɂ��� */
        
        while ((T0BF0 & 0x02) == 0x02) {
            /* �Ǐo����VLF�r�b�g��1�������ꍇ */
            I2C_STATE = 1;/* ����ǂݏo��1�I�� ���荞�݃X�e�[�g�̕ύX */
            T0BF2 = 0;/* �ر */
            
            /* �������R�}���h����сAVLF���W�X�^��0�������� */
            for (;T0BF2 <= 8;T0BF2++) {
                I2C_WCNT = 0;/* �����݃J�E���g��0���Z�b�g */
                I2C_RCNT = 0;/* �Ǎ��݃J�E���g��0���Z�b�g */
                /* I2C�o�X��Ԃ̊m�F���s�� */
                count01 = 0;/* �J�E���g�N���A */
                while (count01 < 5);/* 5ms�҂� */
                TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
                WDTCLR();/* WDT clear */
                count01 = 0;/* �J�E���g�N���A */
                
                i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
                if (i2c_state.Bit.BusState == 1) {
                    /* �o�X�r�W�[�ł���ꍇ */
                    SBI_GenerateI2CStop(TSB_SBI0);/* �ŏ��ɽį�ߺ��ި��݂�� */
                    count01 = 0;/* �J�E���g�N���A */
                    while (count01 < 5);/* 5ms�҂� */
                    TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
                    WDTCLR();/* WDT clear */
                    count01 = 0;/* �J�E���g�N���A */
                    
                }
                
                i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
                if (i2c_state.Bit.BusState == 0) {
                    /* �o�X�t���[�ł���ꍇ */
                    SBI_SetSendData(TSB_SBI0, ((RTC_ADDR) | 0x00));/* RTC�֏����� */
                    SBI_GenerateI2CStart(TSB_SBI0);/* I2C�X�^�[�g������B���M�I����A���肱�ݔ��� */
                    
                    count01 = 0;/* �J�E���g�N���A */
                    for (uint8_t i = 0;i < 4;i++) {
                        /* 20ms�҂��� */
                        while (count01 < 5);/* 5ms�҂� */
                        TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
                        WDTCLR();/* WDT clear */
                        count01 = 0;/* �J�E���g�N���A */
                        
                    }
                    
                    if (I2C_WCNT >= 2) {
                        /* �������݊����ł���ꍇ */
                        RTC_ERR = 0;/* �ر� */
                    } else {
                        RTC_ERR |= 0x01;/* RTC�ʐM�ُ���Z�b�g */
                    }
                } else {
                    /* �o�X�t���[�ɂȂ�Ȃ��ꍇ */
                    RTC_ERR |= 0x01;/* RTC�ʐM�ُ���Z�b�g */
                }
            }
            
            I2C_STATE = 0;/* ���荞�݃X�e�[�g�̕ύX */
            I2C_WCNT = 0;/* ���񏑍��݃J�E���g��0���Z�b�g */
            I2C_RCNT = 0;/* ����Ǎ��݃J�E���g��0���Z�b�g */
            T0BF0 |= 0x02;/* ����VLF�׸޾�� */
            
            i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
            while (i2c_state.Bit.BusState == 1) {
                /* �o�X�r�W�[�ł���ꍇ�͌J��Ԃ� */
                SBI_GenerateI2CStop(TSB_SBI0);/* �į�ߺ��ި��݂�� */
                count01 = 0;/* �J�E���g�N���A */
                while (count01 < 5);/* 5ms�҂� */
                TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
                WDTCLR();/* WD clear */
                count01 = 0;/* �J�E���g�N���A */
                i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
                
            }
            
            i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
            if (i2c_state.Bit.BusState == 0) {
                /* �o�X�t���[�ł���ꍇ */
                T0BF1 = 0;/* �g�p�����ޯ̧�N���A */
                SBI_SetSendData(TSB_SBI0, ((RTC_ADDR) | 0x00));/* �_�~�[������ */
                SBI_GenerateI2CStart(TSB_SBI0);/* I2C�X�^�[�g������B���M�I����A���肱�ݔ��� */
                
                count01 = 0;/* �J�E���g�N���A */
                while (count01 < 5);/* 5ms�҂� */
                count01 = 0;/* �J�E���g�N���A */
                while (count01 < 5);/* 5ms�҂� */
                
                TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
                WDTCLR();/* WDT clear */
                count01 = 0;/* �J�E���g�N���A */
                i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
                
                if ((I2C_WCNT >= 2) && (i2c_state.Bit.BusState == 0)) {
                    /* RTC�ƒʐM�\�ł���ꍇ */
                    SBI_SetSendData(TSB_SBI0, (RTC_ADDR | 0x01));/* �X�^�[�g��A�X���[�u�A�h���X�𑗂�B(�Ǎ���) */
                    SBI_GenerateI2CStart(TSB_SBI0);/* I2C�X�^�[�g��Ԃɂ�����B���M�I����A���肱�ݔ��� */
                    count01 = 0;/* �J�E���g�N���A */
                    while (count01 < 5);/* 5ms�҂� */
                    count01 = 0;/* �J�E���g�N���A */
                    while (count01 < 5);/* 5ms�҂� */
                    
                    TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
                    WDTCLR();/* WDT clear */
                    count01 = 0;/* �J�E���g�N���A */
                    
                    if ((I2C_RCNT >= 3) && (T0BF1 == 1)) {
                        /* ��M���������ꍇ */
                        RTC_ERR = 0;
                    } else {
                        RTC_ERR |= 0x01;
                    }
                } else {
                    /* RTC�ƒʐM�o���Ȃ��ꍇ */
                    RTC_ERR |= 0x01;
                }
            } else {
                /* �o�X�r�W�[�ł���ꍇ */
                RTC_ERR |= 0x01;
            }
            
            SBI_SetI2CACK(TSB_SBI0, ENABLE);/* ACK�𖳌������Ă��邽�߁A�����ŗL���ɂ��� */
            
            if ((RTC_ERR & 0x01) == 0x01) {
                /* �ʐM�s�ǂ��������ꍇ�͔����܂� */
                break;
            }
            
        }/* ���[�v�����܂� */
        
        I2C_STATE = 2;/* ����RTC��Ԋm�F�I�� ���荞�݃X�e�[�g�̕ύX */
        i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
        /* ��RTC�̃��W�X�^�ɐݒ�l����͂��Ă����� */
        if (i2c_state.Bit.BusState == 1) {
            /* �o�X�r�W�[�ł���ꍇ */
            SBI_GenerateI2CStop(TSB_SBI0);/* I2C�ʐM�I����� */
            count01 = 0;/* �J�E���g�N���A */
            while (count01 < 5);/* 5ms�҂� */
            TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
            WDTCLR();/* WDT clear */
            count01 = 0;/* �J�E���g�N���A */
            
        }
        T0BF0 = 0;/* �������p��Ɨ̈�̃N���A */
        
        if ((RTC_ERR & 0x01) == 0x00) {
            /* �o�X�t���[�ł���ꍇ */
            for (;T0BF0 <= 6; T0BF0++) {
                I2C_WCNT = 0;/* ���񏑍��݃J�E���g��0���Z�b�g */
                I2C_RCNT = 0;/* ����Ǎ��݃J�E��0���Z�b�g */
                /* �҂��܂� */
                count01 = 0;/* �J�E���g�N���A */
                while (count01 < 5);/* 5ms�҂� */
                TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
                WDTCLR();/* WDT clear */
                count01 = 0;/* �J�E���g�N���A */
                
                /* I2C�o�X��Ԃ̊m�F���s�� */
                i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
                if (i2c_state.Bit.BusState == 0) {
                    /* I2C�o�X�̏�Ԋm�F(���p�ɂȂ�܂ŌJ��Ԃ�) */
                    SBI_SetSendData(TSB_SBI0, ((RTC_ADDR) | 0x00));/* RTC�֏����� */
                    SBI_GenerateI2CStart(TSB_SBI0);/* I2C�X�^�[�g������B���M�I����A���肱�ݔ��� */
                    count01 = 0;/* �J�E���g�N���A */
                    while (count01 < 5);/* 5ms�҂� */
                    count01 = 0;/* �J�E���g�N���A */
                    while (count01 < 5);/* 5ms�҂� */
                    
                    TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
                    WDTCLR();/* WDT clear */
                    count01 = 0;/* �J�E���g�N���A */
                    
                    if (I2C_WCNT < 2) {
                        /* �������ݏI�����Ă��Ȃ��ꍇ */
                        RTC_ERR |= 0x01;
                    } else {
                        /* �������ݏI�������ꍇ */
                        RTC_ERR = 0;
                    }
                } else {
                    /* ���p�s�ł���ꍇ */
                    RTC_ERR |= 0x01;
                }
                
            }
            
            SBI_GenerateI2CStop(TSB_SBI0);/* �X�g�b�v�R���f�B�V������� */
        }
        
        I2C_STATE = 3;/* ����RTC���W�X�^�����ݏI�� ���荞�݃X�e�[�g�̕ύX */
        
        /* ��RTC�̎����ǂݍ��ݏ����� */
        T0BF0 = 0;/* �������A�v����Ɨ̈�̃N���A */
        T0BF1 = 0;
        
        I2C_WCNT = 0;/* ���񏑍��݃J�E���g��0���Z�b�g */
        I2C_RCNT = 0;/* ����Ǎ��݃J�E���g��0���Z�b�g */
        
        /* I2C�o�X��Ԃ̊m�F���s�� */
        if (RTC_ERR == 0) {
            /* ���p�\�ł���ꍇ */
            i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
            if (i2c_state.Bit.BusState == 0) {
                /* I2C�o�X�̏�Ԋm�F(���p�ɂȂ�܂ŌJ��Ԃ�) */
                SBI_SetSendData(TSB_SBI0, ((RTC_ADDR) | 0x00));/* �_�~�[������ */
                SBI_GenerateI2CStart(TSB_SBI0);/* I2C�X�^�[�g������B���M�I����A���肱�ݔ��� */
                
                count01 = 0;/* �J�E���g�N���A */
                while (count01 < 5);/* 5ms�҂� */
                count01 = 0;/* �J�E���g�N���A */
                while (count01 < 5);/* 5ms�҂� */
                
                TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
                WDTCLR();/* WDT clear */
                count01 = 0;/* �J�E���g�N���A */
                
                i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
                if ((I2C_WCNT >= 2) && (i2c_state.Bit.BusState == 0)) {
                    /* �_�~�[���C�g�ł��Ă���A�o�X�t���[�ł���ꍇ */
                    SBI_SetSendData(TSB_SBI0, (RTC_ADDR | 0x01));/* �X�^�[�g��A�X���[�u�A�h���X�𑗂�B�����ēǍ��� */
                    SBI_GenerateI2CStart(TSB_SBI0);/* I2C�X�^�[�g��Ԃɂ�����B���M�I����A���肱�ݔ��� */
                    
                    count01 = 0;/* �J�E���g�N���A */
                    while (count01 < 5);/* 5ms�҂� */
                    count01 = 0;/* �J�E���g�N���A */
                    while (count01 < 5);/* 5ms�҂� */
                    
                    TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
                    WDTCLR();
                    count01 = 0;/* �J�E���g�N���A */
                    
                    if (T0BF1 == 1) {
                        /* �Ǐo�����������ꍇ */
                        RTC_ERR = 0;/* �N���A */
                    } else {
                        /* �Ǐo���������̏ꍇ */
                        RTC_ERR |= 0x01;/* �Z�b�g */
                    }
                } else {
                    /* �ʐM�ł��Ȃ��ꍇ */
                    RTC_ERR |= 0x01;/* �Z�b�g */
                }
            } else {
                /* �o�X�t���[�ɂȂ�Ȃ��ꍇ */
                RTC_ERR |= 0x01;/* �Z�b�g */
            }
            
        }
        
        SBI_SetI2CACK(TSB_SBI0, ENABLE);/* �Ǎ��݂̍Ō��ACK�𖳌������Ă��邽�߁A�����ŗL���ɂ��� */
        T0BF1 = 0;
        
        REG_00 = (RTCDT[0] & 0x0F);
        REG_01 = ((RTCDT[0] & 0xF0) >> 4) * 10;
        RTCDT[0] = REG_00 + REG_01;/* �b�@BCD���o�C�i���ϊ� */
        REG_00 = (RTCDT[1] & 0x0F);
        REG_01 = ((RTCDT[1] & 0xF0) >> 4) * 10;
        RTCDT[1] = REG_00 + REG_01;/* ���@BCD���o�C�i���ϊ� */
        REG_00 = (RTCDT[2] & 0x0F);
        REG_01 = ((RTCDT[2] & 0xF0) >> 4) * 10;
        RTCDT[2] = REG_00 + REG_01;/* ���@BCD���o�C�i���ϊ� */
        REG_00 = (RTCDT[4] & 0x0F);
        REG_01 = ((RTCDT[4] & 0xF0) >> 4) * 10;
        RTCDT[4] = REG_00 + REG_01;/* ���@BCD���o�C�i���ϊ� */
        REG_00 = (RTCDT[5] & 0x0F);
        REG_01 = ((RTCDT[5] & 0xF0) >> 4) * 10;
        RTCDT[5] = REG_00 + REG_01;/* ���@BCD���o�C�i���ϊ� */
        REG_00 = (RTCDT[6] & 0x0F);
        REG_01 = ((RTCDT[6] & 0xF0) >> 4) * 10;
        RTCDT[6] = REG_00 + REG_01;/* �N�@BCD���o�C�i���ϊ� */
        
        RTC_VAL_CHK();/* ���o�������������������Ȃ����`�F�b�N */
        
        if (RTC_ERR > 0) {
            /* ��x�ł��o�X�t���[�ɂȂ�Ȃ������A�������͓��t�Ɉُ킪����ꂽ�ꍇ */
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
            
            if ((RTC_ERR & 0x02) > 0) {
                /* �ǂݍ��ݓ��t�����������ꍇ */
                SETST_BUF[6] = RTCDT[6] = 21;/* �N */
                SETST_BUF[5] = RTCDT[5] = 4;/* �� */
                SETST_BUF[4] = RTCDT[4] = 1;/* �� */
                SETST_BUF[2] = RTCDT[2] = 12;/* �� */
                SETST_BUF[1] = RTCDT[1] = 0;/* �� */
                WR_RTC  |= 0x01;/* �����݂̈ӂ��Z�b�g */
                
            }
            
            RTC_ERR  = 0;/* �N���A */
            
        }
        
        /* �N��������j�����Z�o���� */
        SETST_BUF[3] = RTCDT[3] = GETWEEKDAY(RTCDT[6], RTCDT[5], RTCDT[4]);/* �j���Z�b�g */
        
        /* D/A�R���o�[�^�[�̏����ݒ�i�������t�@�����X�����j */
        I2C_WCNT = 0;/* I2C�����݉񐔂�ر */
        I2C_STATE = 4;/* I2C���荞�ݽðĂ�����p�ɂ��� */
        SBI_GenerateI2CStop(TSB_SBI0);/* I2C�ʐM�̽į�߂�� */
        
        /* I2C�ʐM�ɂ��DA���ް��̏��������� */
        I2CBUF[0] = (DACN_ADDR | 0x00);/* DAC���ڽ(write)��� */
        I2CBUF[1] = 0x6F;/* DAC_A/B���ɓ����̧�ݽӰ�ނɐݒ肷�� */
        I2CBUF[2] = 0xFF;/* Don't Care 1�޲ľ�� */
        I2CBUF[3] = 0xFF;/* Don't Care 1�޲ľ�� */
        
        i2c_state = SBI_GetI2CState(TSB_SBI0);
        while (i2c_state.Bit.BusState) {
            /*I2C�o�X�̏�Ԋm�F(���p�ɂȂ�܂ŌJ��Ԃ�)*/
            count01 = 0;/* �N���A */
            while(count01 < 5);/* 5ms�҂� */
            
            TSB_WD->CR = 0x0000004E;/* ����WDT�̃N���A�R�[�h���������� */
            WDTCLR();
            SBI_GenerateI2CStop(TSB_SBI0);/* I2C�ʐM�̽į�߂�� */
            i2c_state = SBI_GetI2CState(TSB_SBI0);
            
        }
        
        count01 = 0;/* �^�C�}�[�J�E���g�N���A */
        
        for (uint8_t i = 0;i < 10;i++) {
            /* 10��J��Ԃ�ٰ�ߍ\�� */
            I2C_WCNT = 1;
            SBI_SetSendData(TSB_SBI0, I2CBUF[0]);/* �X�^�[�g��ADAC�A�h���X�𑗂�B������ */
            SBI_GenerateI2CStart(TSB_SBI0);/* I2C�X�^�[�g��Ԃɂ�����B���M�I����A���肱�ݔ��� */
            
            for (uint8_t j = 0;j < 5;j++) {
                /* 10��J��Ԃ�ٰ�ߍ\��(��100ms�҂���) */
                while (count01 < 5);/* 5ms�҂��܂� */
                TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
                count01 = 0;/* �J�E���g��0�N���A */
                while (count01 < 5);/* 5ms�҂��܂� */
                TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
                count01 = 0;
                while (count01 < 5);/* 5ms�҂��܂� */
                TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
                count01 = 0;/* �J�E���g��0�N���A */
                while (count01 < 5);/* 5ms�҂��܂� */
                TSB_WD->CR = 0x0000004E;/*WDT�̃N���A�R�[�h����������*/
                count01 = 0;
                WDTCLR();/* �O�t���E�H�b�`�h�b�O�^�C�}�[�̃N���A */
                
            }
            
            if (I2C_WCNT > 3) {
                /* �S�Ă��ް������M���������ꍇ */
                /* DA���ް��̓����̧�ݽ�������� */
                SBI_GenerateI2CStop(TSB_SBI0);/* I2C�ʐM�̽į�߂�� */
                I2C_WCNT  = 0;
                I2C_STATE = 5;/* I2C���荞�ݽðĂ̕ύX */
                break;
                
            } else {
                /* �f�[�^�����M�o���Ă��Ȃ��ꍇ */
                SBI_GenerateI2CStop(TSB_SBI0);/* I2C�ʐM�̽į�߂�� */
                I2C_WCNT = 0;
                i2c_state = SBI_GetI2CState(TSB_SBI0);
                
                while (i2c_state.Bit.BusState) {
                    /*I2C�o�X�̏�Ԋm�F(���p�ɂȂ�܂ŌJ��Ԃ�)*/
                    count01 = 0;/* �N���A */
                    while(count01 < 5);/* 5ms�҂� */
                    
                    TSB_WD->CR = 0x0000004E;/* ����WDT�̃N���A�R�[�h���������� */
                    WDTCLR();
                    SBI_GenerateI2CStop(TSB_SBI0);/* I2C�ʐM�̽į�߂�� */
                    i2c_state = SBI_GetI2CState(TSB_SBI0);
                    
                }
                
            }
            
        }
        
        NVIC_DisableIRQ(INTTB00_IRQn);/* �^�C�}���荞�ݕs���� */
        NVIC_DisableIRQ(INTSBI0_IRQn);/* I2C�ʐM���荞�ݕs���� */
        
        count01 = 0;/* �J�E���g�N���A */
        kidou = 1;/* �N��������� */
        D_INISQ = 1;/* �����ި��߽�����荞�݁ANV-RAM��荞�݁ARTC�ݒ�E�����ADA���ް��ݒ菑���݊������ */
        INI_STT = 0;/* �T�u�V�[�P���X�N���A */
        
    } else {
        /* �`�`�`�`�`������������̏����`�`�`�`�` */
        /* ���ʏ��� */
        if ((SYSFLG & 0x5E) == 0x40) {
            /* �^�]�҂���Ԃł���ꍇ */
            if (RWTMS >= 240) {
                /* 60�b�o�߂��Ă���ꍇ */
                RWTMS -= 240;/* 60�b�����Z */
                RWAITT++;/* �����C���N�������g */
                
                if (RWAITT >= 240) {
                    /* 4���Ԍo�߂����ꍇ */
                    RWAITT = 0;/* �����N���A */
                    SYSFLG &= 0xA1;/* �p���[�I�t�ɂ��� */
                    
                }
                
            }
            
        } else {
            /* �^�]�҂���Ԃł͂Ȃ��ꍇ */
            RWTMS = 0;
            RWAITT = 0;
            
        }
        
        SBI_I2CState i2c_state;/* I2C�o�X��Ԃ�����\���̂ɖ��O��t���� */
        
        STSFL0 |= 0x02;/* �����������Z�b�g */
        
        if (D_INISQ == 1) {
            /* �`�`�`�`�`RTC�ǂݏo�������`�`�`�`�` */
            /* �������番�� */
            if (INI_STT == 0) {
                /* ���� */
                I2C_STATE = 3;/* I2C���荞�݃X�e�[�g�̕ύX */
                
                /* RTC�̎����ǂݍ��ݏ��� */
                T0BF0 = 0;/* �������A�v����Ɨ̈�̃N���A */
                I2C_WCNT = 0;/* ���񏑍��݃J�E���g��0���Z�b�g */
                I2C_RCNT = 0;/* ����Ǎ��݃J�E���g��0���Z�b�g */
                
                /* I2C�o�X��Ԃ̊m�F���s�� */
                for (uint8_t i = 0;i < 200;i++) {
                    i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
                    __NOP();
                    RTC_ERR |= 0x80;
                    
                    if (!i2c_state.Bit.BusState) {
                        /* �ʐM���Ă��Ȃ��ꍇ */
                        RTC_ERR &= 0x7F;/* RTC�ُ�N���A */
                        SBI_SetSendData(TSB_SBI0, ((RTC_ADDR) | 0x00));/* �_�~�[������ */
                        SBI_GenerateI2CStart(TSB_SBI0);/* I2C�X�^�[�g������B���M�I����A���肱�ݔ��� */
                        
                        break;
                    } else if (i == 199) {
                        /* �Ō�܂Ńo�X�t���[���m�F�ł��Ȃ������ꍇ */
                        I2C_RSTE();/* �o�X�`�F�b�N�E�N���A */
                    }
                }
                
                INI_STT++;/* ����̏����ʒu�C���N�������g */
                
                OS_LoadTime->NEXT_LOAD_Time[0] = 5;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[0] = 0xA0;/* �x���t���O���z */
                
            } else if (INI_STT == 1) {
                /* �_�~�[���C�g�I���m�F */
                if (I2C_WCNT > 1) {
                    /* �_�~�[���C�g�҂��I������� */
                    for (uint8_t i = 0;i < 200;i++) {
                        i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԃ̎擾 */
                        __NOP();
                        RTC_ERR |= 0x80;
                        if (!i2c_state.Bit.BusState) {
                            /* I2C�o�X�̏�Ԋm�F(�o�X���󂫂ł����) */
                            RTC_ERR &= 0x7F;/* RTC�ُ�N���A */
                            SBI_SetSendData(TSB_SBI0, (RTC_ADDR | 0x01));/* �X�^�[�g��A�X���[�u�A�h���X�𑗂�B�����ēǍ��� */
                            SBI_GenerateI2CStart(TSB_SBI0);/* I2C�X�^�[�g��Ԃɂ�����B���M�I����A���肱�ݔ��� */
                            break;
                            
                        }
                        
                    }
                    
                } else if (NACK5 > 5) {
                    /* NACK��5��ɒB�����ꍇ */
                    SBI_GenerateI2CStop(TSB_SBI0);/* I2C���~������ */
                    RTC_ERR |= 0x80;/* RTC�ُ�� */
                    
                } else {
                    /* ���������҂� */
                    SBI_GenerateI2CStop(TSB_SBI0);/* I2C���~������ */
                    RTC_ERR |= 0x80;/* RTC�ُ�� */
                    
                }
                
                INI_STT++;/* ���񏈗��ʒu�C���N�������g */
                
                OS_LoadTime->NEXT_LOAD_Time[0] = 10;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[0] = 0xA0;/* �x���t���O���z */
                
            } else if (INI_STT == 2) {
                /* �ǂݍ��ݑ҂���� */
                if (I2C_RCNT >= 9) {
                    /* �Ǎ��݉񐔂��Ǎ��ݗ\��f�[�^���𒴂����ꍇ */
                    RTC_ERR &= 0x7F;/* ��M�ł����̂ň�URTC�ُ���N���A���� */
                    
                    SBI_SetI2CACK(TSB_SBI0, ENABLE);/* �Ǎ��݂̍Ō��ACK�𖳌������Ă��邽�߁A�����ŗL���ɂ��� */
                    REG_00 = (RTCDT[0] & 0x0F);
                    REG_01 = (RTCDT[0] >> 4) * 10;
                    RTCDT[0] = REG_00 + REG_01;/* �b�@BCD���o�C�i���ϊ� */
                    REG_00 = (RTCDT[1] & 0x0F);
                    REG_01 = ((RTCDT[1] & 0xF0) >> 4) * 10;
                    RTCDT[1] = REG_00 + REG_01;/* ���@BCD���o�C�i���ϊ� */
                    REG_00 = (RTCDT[2] & 0x0F);
                    REG_01 = ((RTCDT[2] & 0xF0) >> 4) * 10;
                    RTCDT[2] = REG_00 + REG_01;/* ���@BCD���o�C�i���ϊ� */
                    REG_00 = (RTCDT[4] & 0x0F);
                    REG_01 = ((RTCDT[4] & 0xF0) >> 4) * 10;
                    RTCDT[4] = REG_00 + REG_01;/* ���@BCD���o�C�i���ϊ� */
                    REG_00 = (RTCDT[5] & 0x0F);
                    REG_01 = ((RTCDT[5] & 0xF0) >> 4) * 10;
                    RTCDT[5] = REG_00 + REG_01;/* ���@BCD���o�C�i���ϊ� */
                    REG_00 = (RTCDT[6] & 0x0F);
                    REG_01 = ((RTCDT[6] & 0xF0) >> 4) * 10;
                    RTCDT[6] = REG_00 + REG_01;/* �N�@BCD���o�C�i���ϊ� */
                    
                    RTC_VAL_CHK();/* RTC�̒l�`�F�b�N���{ */
                    
                    if ((RTC_ERR & 0x02) > 0) {
                        /* �f�[�^�Ɉُ킪�������ꍇ */
                        RTC_ECT++;/* �J�E���g���C���N�������g */
                        if (RTC_ECT >= 5) {
                            /* �J�E���g��5�ȏ�ɂȂ����ꍇ */
                            RTC_ERR |= 0x80;/* ���vIC���̂��߃t���O */
                            RTC_ECT = 0;/* �N���A */
                            
                        }
                        
                    } else {
                        /* �f�[�^�Ɉُ킪�Ȃ��ꍇ */
                        RTC_ECT = 0;/* �N���A */
                        RTC_ERR &= 0x7F;/* ���vIC���Ȃ� */
                    }
                    
                    /* �N��������j�����Z�o���� */
                    RTCDT[3] = GETWEEKDAY(RTCDT[6], RTCDT[5], RTCDT[4]);/* �j���Z�b�g */
                    
                    
                    
                } else if (NACK5 >= 5) {
                    /* NACK�񐔂���萔�𒴂����ꍇ */
                    SBI_SetI2CACK(TSB_SBI0, ENABLE);/* �Ǎ��݂̍Ō��ACK�𖳌������Ă��邽�߁A�����ŗL���ɂ��� */
                    RTC_ERR |= 0x80;
                    
                } else {
                    /* ����ȊO�̏ꍇ */
                    SBI_SetI2CACK(TSB_SBI0, ENABLE);/* �Ǎ��݂̍Ō��ACK�𖳌������Ă��邽�߁A�����ŗL���ɂ��� */
                    RTC_ECT++;/* �G���[�J�E���g���C���N�������g */
                    if (RTC_ECT >= 5) {
                        /* 5��ȏ㎸�s�����ꍇ */
                        RTC_ECT = 0;/* ���s�񐔃N���A */
                        RTC_ERR |= 0x80;
                        
                    }
                    
                }
                
                /* �I������ */
                SBI_GenerateI2CStop(TSB_SBI0);/* I2C���~������ */
                INI_STT = 0;/* ���񏈗��ʒu��ύX */
                I2C_RCNT = I2C_WCNT = 0;/* �l���N���A���� */
                T0BF0 = T0BF1 = 0;/* �\�萔�ȏ�̎�M�������������m�����t���O���N���A���� */
                
                if ((WR_RTC & 0x01) == 0x01) {
                    /* RTC�̏������������������ꍇ */
                    D_INISQ = 4;/* RTC�������݂ɐ؂�ւ��� */
                    
                } else {
                    /* RTC�̏����������������Ă��Ȃ��ꍇ */
                    D_INISQ = 2;/* NV-RAM�������݂ɐ؂�ւ��� */
                    
                }
                
                OS_LoadTime->NEXT_LOAD_Time[0] = 15;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[0] = 0xA0;/* �x���t���O���z */
                
            }
            
            if (RTC_ERR > 0) {
                /* ��x�ł��o�X�t���[�ɂȂ�Ȃ������A�������͓��t�Ɉُ킪����ꂽ�ꍇ */
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
                
                RTC_ERR = 0;
            }
            
        } else if (D_INISQ == 2) {
            /* �`�`�`�`�`NV-RAM�������ݏ����`�`�`�`�` */
            STSFL0 ^= 0x01;/* XOR�@0�r�b�g�� */
            if ((STSFL0 & 0x01) == 0) {
                /* NV-RAM�������݃^�C�~���O�ł���ꍇ */
                MOMCHK();/* �������[���� */
                KEEP();/* NV-RAM�փo�b�N�A�b�v���� */
                
            }
            
            D_INISQ = 3;/* DAC�������݂ɐ؂�ւ��� */
            INI_STT = 0;/* �����l�ɂ��� */
            
            OS_LoadTime->NEXT_LOAD_Time[0] = 30;/* ����Ăяo������ */
            OS_LoadTime->TSK_COND[0] = 0xA0;/* �x���t���O���z */
            
        } else if (D_INISQ == 3) {
            /* �`�`�`�`�`DAC�������ݏ����`�`�`�`�` */
            if (INI_STT == 0) {
                /* ���M���ݸނł���ꍇ */
                I2C_STATE = 5;/* I2C���荞�݃X�e�[�g�̕ύX */
                
                T0BF0 = 0;/* �������A�v����Ɨ̈�̃N���A */
                I2C_WCNT = 0;/* ���񏑍��݃J�E���g��0���Z�b�g */
                I2C_RCNT = 0;/* ����Ǎ��݃J�E���g��0���Z�b�g */
                
                i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
                
                for (uint8_t i = 0;i < 200;i++) {
                    /* 200�񃋁[�v */
                    if (!i2c_state.Bit.BusState) {
                        /* I2C�o�X���o�X�t���[�ł���ꍇ */
                        REG_00 = DACOUT;/* DAC�o�͗p�ϐ��l���Z�b�g���� */
                        
                        I2CBUF[0] = (DACN_ADDR | 0x00);/* DAC���ڽ(write)��� */
                        I2CBUF[1] = 0x30;/* DAC_A�̃��W�X�^�ɏ������ޖ��� */
                        I2CBUF[2] = (uint8_t)((0x0FF0 & REG_00) >> 4);/* DAC�ɑ�����8�r�b�g��� */
                        I2CBUF[3] = (uint8_t)((0x000F & REG_00) << 4);/* DAC�ɑ��鉺��4�r�b�g��� */
                        
                        I2C_WCNT = 1;/* ���M�O�ɃJ�E���g�����Z���� */
                        SBI_SetSendData(TSB_SBI0, I2CBUF[0]);/* �X�^�[�g��ADAC�A�h���X�𑗂�B������ */
                        SBI_GenerateI2CStart(TSB_SBI0);/* I2C�X�^�[�g��Ԃɂ�����B���M�I����A���肱�ݔ��� */
                        
                        break;/* ���[�v�𔲂��܂� */
                        
                    } else {
                        /* I2C�o�X���o�X�r�W�[�ł���ꍇ */
                        SBI_GenerateI2CStop(TSB_SBI0);/* I2C�ʐM�̽į�߂�� */
                        __NOP();
                        __NOP();
                        i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
                        
                    }
                    
                }
                
                INI_STT = 1;/* �����V�[�P���X�����̃V�[�P���X�ɂ��� */
                
                OS_LoadTime->NEXT_LOAD_Time[0] = 15;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[0] = 0xA0;/* �x���t���O���z */
                
            } else {
                /* �S���M�����m�F�^�C�~���O�ł���ꍇ */
                if (I2C_WCNT > 3) {
                    /* DA�R���o�[�^�����݂̑S�Ă��ް������M���������ꍇ */
                    I2CCNT = 0;/* �N���A */
                    
                } else {
                    /* �f�[�^�����M�o���Ă��Ȃ��ꍇ */
                    I2CCNT++;/* �҂����Բݸ���� */
                    if (I2CCNT >= 5) {
                        /* 5��ȏ�҂��ڂ����̏ꍇ */
                        I2CCNT = 5;/* 5��FIX */
                        I2C_RSTE();/* F-RAM���Z�b�g���� */
                        
                    }
                    
                }
                
                SBI_GenerateI2CStop(TSB_SBI0);/* I2C�ʐM�̽į�߂�� */
                I2C_WCNT = 0;/* �����݉񐔃N���A */
                I2C_STATE = 3;/* ���񊄂荞�݃X�e�[�g�ύX */
                INI_STT = 0;/* �����l */
                D_INISQ = 1;/* ����RTC�����ǂݍ��� */
                
                OS_LoadTime->NEXT_LOAD_Time[0] = 25;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[0] = 0xA0;/* �x���t���O���z */
                
            }
            
        } else if (D_INISQ == 4) {
            /* RTC�������ݏ��� */
            if (INI_STT == 0) {
                /* ����͏������ݏ��� */
                I2C_STATE = 6;/* ���荞�݃X�e�[�g�̕ύX */
                
                /* RTC�̎����ǂݍ��ݏ��� */
                T0BF0 = 0;/* �������A�v����Ɨ̈�̃N���A */
                T0BF2 = 0;
                I2C_WCNT = 0;/* �����݃J�E���g��0���Z�b�g */
                I2C_RCNT = 0;/* �Ǎ��݃J�E���g��0���Z�b�g */
                
                /* I2C�o�X��Ԃ̊m�F���s�� */
                for (uint8_t i = 0;i < 200;i++) {
                    i2c_state = SBI_GetI2CState(TSB_SBI0);/* I2C�o�X��Ԏ擾 */
                    __NOP();/* NOP */
                    SBI_GenerateI2CStop(TSB_SBI0);/* I2C���~������ */
                    if (!i2c_state.Bit.BusState) {
                        /* I2C�o�X���󂫂ł���ꍇ */
                        SBI_SetSendData(TSB_SBI0, ((RTC_ADDR) | 0x00));/* �����ݾ�� */
                        SBI_GenerateI2CStart(TSB_SBI0);/* I2C�X�^�[�g������B���M�I����A���肱�ݔ��� */
                        INI_STT++;/* ����̏����ʒu�C���N�������g */
                        break;
                        
                    } else if (i == 199) {
                        /* �Ō�܂Ńo�X�t���[�����o�ł��Ȃ������ꍇ */
                        D_INISQ = 1;/* �����ǂݍ��ݏ����� */
                        I2C_RSTE();/* �o�X�`�F�b�N�E�N���A */
                    }
                    
                }
                
                OS_LoadTime->NEXT_LOAD_Time[0] = 15;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[0] = 0xA0;/* �x���t���O���z */
                
            } else if (INI_STT == 1) {
                /* �������ݑ҂� */
                if (I2C_WCNT > 6) {
                    /* �������ݑ҂��I������� */
                    RTC_ERR &= 0x7F;/* �G���[�t���O���N���A���� */
                    
                } else if (NACK5 >= 5) {
                    /* NACK��5��ɒB�����ꍇ */
                    RTC_ERR |= 0x80;/* ���vIC���̂��߃t���O */
                    SBI_GenerateI2CStop(TSB_SBI0);/* I2C���~������ */
                    
                } else {
                    /* ���������҂� */
                    RTC_ERR |= 0x80;/* ���vIC���̂��߃t���O */
                    
                }
                
                /* �I������ */
                SBI_GenerateI2CStop(TSB_SBI0);/* I2C���~������ */
                INI_STT  = 0;/* ���񏈗��ʒu��ύX */
                I2C_WCNT = 0;/* �������݉񐔃N���A */
                D_INISQ  = 2;/* NV-RAM�������݂ɐ؂�ւ��� */
                WR_RTC  &= 0xFE;/* ���v�����ݏI�� */
                
                for (uint8_t i = 0;i < 7;i++) {
                    SETST_BUF[i] = 0;
                }
                
                OS_LoadTime->NEXT_LOAD_Time[0] = 15;/* ����Ăяo������ */
                OS_LoadTime->TSK_COND[0] = 0xA0;/* �x���t���O���z */
                
            }
            
        }
        
        /* ������������̏��������܂� */
    }
    
    if (OS_LoadTime->NEXT_LOAD_Time[0] == 0) {
        /* ���͂���Ă��Ȃ��ꍇ */
        OS_LoadTime->NEXT_LOAD_Time[0] = 20;/* ����Ăяo������ */
        OS_LoadTime->TSK_COND[0] = 0xA0;/* �x���t���O���z */
        
    }
    
    return;
}

/**
 *�薼�FI2C�o�X�̃��Z�b�g���[�`��
 *�쐬�ҁF����@����
 *�����FI2C�o�X��SDA���C����Low����t�������ꍇ�A�������Z�b�g���s���B
**/
void I2C_RSTE(void)
{
    
    if (TSB_PC_DATA_PC0 == 0) {
        /* PC0��Low����t���ł������ꍇ */
        /* SCL���C�����J�`�J�`���� */
        for (uint8_t i=0;i<=9;i++) {
            TSB_PC_DATA_PC1 = 1;
            TSB_PC->CR &= 0xFE;/* PC0��HI-Z�ɂ��� */
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
            TSB_PC->CR |= 0x01;
            TSB_PC_DATA_PC0 = 0;
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
            TSB_PC_DATA_PC1 = 0;
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
            TSB_PC->CR &= 0xFE;/* PG0��HI-Z�ɂ��� */
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
            TSB_PC_DATA_PC1 = 1;
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
        }
        TSB_PC->CR |= 0x01;
    }
    
    return;
}

/**
 *�薼�FNV-RAM�̃f�[�^�擾���[�`��
 *�쐬�ҁF����@����
 *�����F����ANV-RAM����f�[�^���擾����
**/
void SETUP(void)
{
    for (uint8_t i = 0;i < 2;i++) {
        /* 2��m�F���s���܂� */
        ECMD(0x85);/* ���R�[���R�}���h(NV-RAM�̓���E2PROM�������RAM�ɓǂݏo������) */
        
        TSB_PE_DATA_PE4 = 0;/* NV-RAM �`�b�v�m�b�g�Z���N�g */
        TSB_PD->DATA &= 0xCF;/* DI��SK���N���A */
        
        count01 = 0;/* �^�C�}�N���A */
        while(count01 < 1);/* 1ms�҂��܂� */
        
        ERD();/* 1�x�ǂݎ̂� */
        ERD();/* 2��ڂ��̗p */
        
        if ((PASS1 == 0x01) && (PASS2 == 0xAA)) {
            /* �p�X���[�hOK�ł���ꍇ */
            break;/* �����܂� */
            
        } else {
            /* �p�X���[�h��������Ȃ��ꍇ */
            if (i == 1) {
                /* 2��ڂł������ꍇ */
                BLCLR();/* �����l�Z�b�g���� */
                ABNF = 0x0D;/* NV-RAM�ُ� */
                ABNF |= 0x80;/* �u�U�[�t���O�Z�b�g */
                SYSFLG |= 0x40;/* �d��ON�t���O�Z�b�g */
                
            }
            
        }
    }
    
    return;/* �߂�܂� */
    
}

/**
 *�薼�FNV-RAM�ۑ����郁�����[��Ԃ̊m�F
 *�쐬�ҁF����@����
 *�����F�ۑ��������[�̃`�F�b�N�����Ă���������Ώ����l�ɂ���
**/
void MOMCHK(void)
{
    if ((PASS1 == 0x01) && (PASS2 == 0xAA)) {
        /* �p�X���[�h���ݒ肳��Ă���ꍇ */
        /* �������Ȃ� */
        
    } else {
        /* �p�X���[�h���ݒ肳��Ă��Ȃ��ꍇ */
        BLCLR();/* �����l�ɂ��� */
        ABNF = 0x0D;
        
    }
    
    return;/* �߂� */
    
}

/**
 *�薼�FNV-RAM�փf�[�^�ۑ����郋�[�`��
 *�쐬�ҁF����@����
 *�����FNV-RAM�փf�[�^���������݁E�ۑ����鏈��
**/
void KEEP(void)
{
    uint16_t REG_00 = 0;/* �ꎞ�o�b�t�@�G���A */
    
    ECMD(0x84);/* �������݋��R�}���h�𑗐M */
    
    TSB_PE_DATA_PE4 = 0;/* NV-RAM �`�b�v�m�b�g�Z���N�g */
    TSB_PD->DATA &= 0xCF;/* DI��SK���N���A */
    
    for (uint8_t i = 0;i < 16;i++) {
        /* 16��J��Ԃ��i16���[�h���j */
        ECMD((0x83 | (i << 3)));/* NV-RAM�������݃R�}���h */
        TSB_PE_DATA_PE4 = 1;/* NV-RAM �`�b�v�Z���N�g */
        
        REG_00 = 0;/* ��U�N���A */
        REG_00 = (uint16_t)*HENNP[i * 2];/* �Z�b�g */
        REG_00 |= ((uint16_t)*HENNP[(i * 2) + 1] << 8);/* �Z�b�g */
        
        for (uint8_t j = 0;j < 16;j++) {
            /* 16bit���J��Ԃ��m�F */
            if ((REG_00 & 0x0001) > 0) {
                /* NV-RAM�֏o�͂���f�[�^��High�ł���ꍇ */
                TSB_PD_DATA_PD5 = 1;/* �Z�b�g */
                
            } else {
                /* NV-RAM�֏o�͂���f�[�^��Low�ł���ꍇ */
                TSB_PD_DATA_PD5 = 0;/* �Z�b�g */
                
            }
            REG_00 >>= 1;/* 1�r�b�g�E�V�t�g */
            
            /* �N���b�N�f�[�^���o�� */
            TSB_PD_DATA_PD4 = 1;/* SK��High */
            for (uint8_t k = 0;k < 4;k++) {
                /* ���������҂��܂� */
                __NOP();/* �҂� */
            }
            TSB_PD_DATA_PD4 = 0;/* SK��Low */
            __NOP();/* �҂� */
            
        }
        
        TSB_PE_DATA_PE4 = 0;/* NV-RAM �`�b�v�m�b�g�Z���N�g */
        TSB_PD->DATA &= 0xCF;/* DI��SK���N���A */
        __NOP();/* �҂� */
        
    }
    
    return;
}

/**
 *�薼�FNV-RAM�̃R�}���h�������݃��[�`��
 *�쐬�ҁF����@����
 *�����F����ANV-RAM�փR�}���h�𑗐M����
**/
void ECMD(uint8_t CoMD)
{
    TSB_PE_DATA_PE4 = 0;/* NV-RAM �`�b�v�m�b�g�Z���N�g */
    TSB_PD->DATA &= 0xCF;/* DI��SK���N���A */
    
    TSB_PE_DATA_PE4 = 1;/* NV-RAM �`�b�v�Z���N�g */
    
    for (uint8_t i = 0;i < 8;i++) {
        /* 8�񃋁[�v���� */
        /* �܂��̓R�}���h�r�b�g���o�͂��� */
        if ((CoMD & 0x80) == 0) {
            /* �R�}���h�̍ŏ�ʃr�b�g��0�̏ꍇ */
            TSB_PD_DATA_PD5 = 0;/* Low */
            
        } else {
            /* �R�}���h�̍ŏ�ʃr�b�g��1�̏ꍇ */
            TSB_PD_DATA_PD5 = 1;/* High */
            
        }
        CoMD <<= 1;/* 1�ޯč��V�t�g */
        
        /* �N���b�N�f�[�^���o�� */
        TSB_PD_DATA_PD4 = 1;/* SK��High */
        for (uint8_t j = 0;j < 4;j++) {
            /* ���������҂��܂� */
            __NOP();/* �҂� */
        }
        TSB_PD_DATA_PD4 = 0;/* SK��Low */
        
    }
    
    return;/* �߂� */
}

/**
 *�薼�FNV-RAM����f�[�^�ǂݏo�����[�`��
 *�쐬�ҁF����@����
 *�����F����ANV-RAM����f�[�^����荞�ޏ���
**/
void ERD(void)
{
    uint16_t REG_00 = 0;/* �����ł����g��Ȃ����̒�` */
    
    TSB_PE_DATA_PE4 = 0;/* NV-RAM �`�b�v�m�b�g�Z���N�g */
    TSB_PD->DATA &= 0xCF;/* DI��SK���N���A */
    
    for (uint8_t i = 0;i < 16;i++) {
        /* 16��J��Ԃ��i16���[�h���j */
        ECMD((0x86 | (i << 3)));/* NV-RAM�ǂݏo���R�}���h */
        TSB_PE_DATA_PE4 = 1;/* NV-RAM �`�b�v�Z���N�g */
        
        for (uint8_t j = 0;j < 16;j++) {
            /* 16bit���J��Ԃ��m�F */
            REG_00 >>= 1;/* 1�r�b�g�E�V�t�g */
            if (TSB_PD_DATA_PD6 == 1) {
                /* NV-RAM�̏o�̓f�[�^��High�ł���ꍇ */
                REG_00 |= 0x8000;/* �Z�b�g */
                
            }
            
            /* �N���b�N�f�[�^���o�� */
            TSB_PD_DATA_PD4 = 1;/* SK��High */
            for (uint8_t k = 0;k < 4;k++) {
                /* ���������҂��܂� */
                __NOP();/* �҂� */
            }
            TSB_PD_DATA_PD4 = 0;/* SK��Low */
            __NOP();/* �҂� */
            
        }
        
        TSB_PE_DATA_PE4 = 0;/* NV-RAM �`�b�v�m�b�g�Z���N�g */
        /* �f�[�^�𔽉f���� */
        *HENNP[i * 2] = (uint8_t)REG_00;/* ���ʃZ�b�g */
        *HENNP[(i * 2) + 1] = (uint8_t)(REG_00 >> 8);/* ��ʃZ�b�g */
        
    }
    
    return;/* �߂� */
}

/**
 *�薼�FNV-RAM�̃f�[�^���������[�`��
 *�쐬�ҁF����@����
 *�����F����ANV-RAM�̃p�X���[�h�f�[�^���������������ꍇ�Ƀf�[�^���N���A���鏈��
**/
void BLCLR(void)
{
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
    *HENNP[29] = 0;/* ���݂̃p�^�[���I��l�@0 */
    
    return;
    
}

/**
 *�薼�FRTC�f�[�^�̐��퐫�m�F
 *�쐬�ҁF����@����
 *�����FRTC����ǂݏo�����l������Ȓl�ł��邩���肷��
**/
void RTC_VAL_CHK(void)
{
    if (RTCDT[0] > 59) {
        /* �b���G���[ */
        RTC_ERR |= 0x02;/* �G���[�Z�b�g */
        RTCDT[0] = 0;
    }
    
    if (RTCDT[1] > 59) {
        /* �����G���[ */
        RTC_ERR |= 0x02;/* �G���[�Z�b�g */
        RTCDT[1] = 0;
    }
    
    if (RTCDT[2] > 23) {
        /* �����G���[ */
        RTC_ERR |= 0x02;/* �G���[�Z�b�g */
        RTCDT[2] = 12;
    }
    
    if ((RTCDT[5] > 12) || (RTCDT[5] < 1)) {
        /* �����G���[ */
        RTC_ERR |= 0x02;/* �G���[�Z�b�g */
        RTCDT[5] = 4;
    }
    
    if (RTCDT[6] > 99) {
        /* �N���G���[ */
        RTC_ERR |= 0x02;/* �G���[�Z�b�g */
        RTCDT[6] = 22;
    }
    
    if (
    (RTCDT[4] > 31) || (RTCDT[4] < 1)
    || ((RTCDT[4] > 30) && ((RTCDT[5] == 4) || (RTCDT[5] == 6) || (RTCDT[5] == 9) || (RTCDT[5] == 11)))
    || ((RTCDT[4] > (28 + (1 / (RTCDT[6] % 4 + 1)) * (1 - 1 / (RTCDT[6] % 100 + 1)) + (1 / (RTCDT[6] % 400 + 1))))
        && (RTCDT[5] == 2))
    ) {
        /* �����G���[ */
        RTC_ERR |= 0x02;/* �G���[�Z�b�g */
        RTCDT[4] = 1;
    }
    
    return;
}

/*
 �N�A���A�����w�肷��ƁA�j����Ԃ��B(0:���`6:�y)
*/
uint8_t GETWEEKDAY(uint8_t nYear, uint8_t nMonth, uint8_t nDay)
{
    int nWeekday, nTmp, Year2;
    
    Year2 = (2000 + nYear);/* 2000�N���Z */
    if ((nMonth == 1) || (nMonth == 2)) {
        /* 1���A2���̏ꍇ */
        Year2--;/* �N���޸���� */
        nMonth += 12;/* 12�������Z */
    }
    
    nTmp     = (Year2 / 100);/* �N�l��100�Ŋ������l���Z�b�g */
    nWeekday = ((Year2 + (Year2 >> 2) - nTmp + (nTmp >> 2) + (13 * (int)nMonth + 8) / 5 + (int)nDay) % 7);/* �j���Z�o */
    nWeekday &= 0x00000007;/* �K�v�Ȓl�݂̂��}�X�N */
    
    return (uint8_t)nWeekday;/* 1�o�C�g�Ń��^�[�� */
}