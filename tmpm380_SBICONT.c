/***
 * �薼�FI2C�ʐM�A�v��
 * �쐬�ҁF����@����
 * I2C�ʐM������s���A�v���P�[�V����
***/
#include "tmpm380_SBICONT.h"

const uint8_t WRITE01[9] = {
    /* RTCڼ޽����ڽ */
    0x17, 0x31, 0x1F, 0x1F, 0x60, 0x66, 0x6B, 0x6B, 0x1E
};
const uint8_t WRITE02[9] = {
    /* RTCڼ޽����ڽ�ɏ������ޒl */
    0xA8, 0x00, 0x00, 0x80, 0xD3, 0x03, 0x02, 0x01, 0x00
};
const uint8_t WRITE03[7] = {
    /* RTCڼ޽����ڽ2 */
    0x17, 0x1D, 0x1E, 0x1F, 0x18, 0x19, 0x1A
};
const uint8_t WRITE04[7] = {
    /* RTCڼ޽����ڽ�ɏ������ޒl2 */
    0xA8, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
};

/***
  * �薼�FSBI0(I2C)���荞��
  * �쐬�ҁF����@����
  * I2C�ʐM ���M�A��M���Ɋ��荞��
 ***/
void INTSBI0_IRQHandler(void)
{
    TSB_SBI_TypeDef *SBIx;
    SBI_I2CState sbi_sr;

    SBIx = TSB_SBI0;
    sbi_sr = SBI_GetI2CState(SBIx);
    
    uint8_t REG_00, REG_01, REG_02 = 0;
    
    if (I2C_STATE == 0) {
        /** �N������RTC����f�[�^�ǂݏo�����s���ӏ� ---------------------------------**/
        if (sbi_sr.Bit.TRx) {
            /* ���M���[�h�̍�(1�͑��M) */
            if (sbi_sr.Bit.LastRxBit) {
                /* LRB=1:�ŏI�u��M�v�r�b�g��1(NACK�̂Ƃ�) */
                NACK5++;
                if (NACK5 < 5) {
                    /* NACK��5�񖢖��ł���ꍇ */
                    I2C_WCNT = 0;
                    SBI_GenerateI2CStop(SBIx);/* I2C���~ */
                    __NOP();
                    SBI_SetSendData(SBIx, (RTC_ADDR | 0x00));/* �ēxRTC�A�h���X�𑗂� */
                    __NOP();
                    SBI_GenerateI2CStart(SBIx); /* I2C���J�n */
                    
                } else {
                    NACK5 = 5;/* MAX FIX */
                    SBI_GenerateI2CStop(SBIx); /* I2C���~ */
                }
            } else {
                /* LRB = 0:�ŏI�u��M�v�r�b�g��0(ACK�̂Ƃ�) */
                NACK5 = 0;/* NACK�񐔂����Z�b�g���� */
                if (I2C_WCNT == 0) {
                    SBI_SetSendData(SBIx, 0x1E);/* RTC�A�h���X�f�[�^�𑗐M���� */
                } else {
                    SBI_GenerateI2CStop(SBIx);/* I2C���~ */
                }
                I2C_WCNT++;/* ���M�����瑗�M�J�E���g�����Z */
            }
        } else {
            /* ��M���[�h�̍�(0�͎�M) */
            if (I2C_RCNT >= 2) {
                /* ��M�񐔂���萔���z������ */
                T0BF1 = 1;
                REG_00 = SBI_GetReceiveData(SBIx);/* RTC����ǂݏo�����f�[�^�����o�� */
                SBI_GenerateI2CStop(SBIx);
                
            } else {
                /* ��M�񐔂���萔���z���Ă��Ȃ��ꍇ */
                switch (I2C_RCNT) {/*�Ǎ��݉�*/
                case 0:
                    /* ��x�A�l�q�����s���B�_�~�[�œǂݏo������ */
                    REG_00 = SBI_GetReceiveData(SBIx);/* RTC����ǂݏo�����f�[�^ */
                    
                    break;
                    
                case 1:
                    /*��M�����f�[�^ �ǂݍ��ރf�[�^�́A�t���O���W�X�^*/
                    T0BF0 = SBI_GetReceiveData(SBIx);/* RTC����ǂݏo�����f�[�^ */
                    T0BF1 = 1;
                    SBI_SetI2CACK(TSB_SBI0, DISABLE);/* ���̃f�[�^��M����ACK���Ȃ� */
                    
                    break;
                    
                }
            }
            I2C_RCNT++;/* �C���N�������g */
            
        }
        
    } else if (I2C_STATE == 1) {
        /** �N�����ARTC�փf�[�^�����ݏ��� -------------------------------------------**/
        if (sbi_sr.Bit.TRx) {
            /* ���M���[�h�̍�(1�͑��M) */
            if (sbi_sr.Bit.LastRxBit) {
                /* LRB=1:�ŏI�u��M�v�r�b�g��1(NACK�̂Ƃ�) */
                NACK5++;
                if (NACK5 < 5) {
                    I2C_WCNT = 0;
                    SBI_GenerateI2CStop(SBIx); /* I2C���~ */
                    __NOP();
                    SBI_SetSendData(SBIx, (RTC_ADDR | 0x00));/* �ēxRTC�A�h���X�𑗂� */
                    __NOP();
                    SBI_GenerateI2CStart(SBIx); /* I2C���J�n */
                    
                } else {
                    SBI_GenerateI2CStop(SBIx); /* I2C���~ */
                }
            } else {
                /* LRB=0:�ŏI�u��M�v�r�b�g��0(ACK�̂Ƃ�) */
                NACK5 = 0;/* NACK�񐔂����Z�b�g���� */
                if (I2C_WCNT <= 1) {
                    /* �J�E���g���܂��f�[�^���t�\��񐔈ȉ��̍� */
                    if (I2C_WCNT == 0) {
                        /* ���ڽ���M�� */
                        if (T0BF2 <= 8) {
                            /* 8�ȉ��̏ꍇ */
                            SBI_SetSendData(SBIx, WRITE01[T0BF2]);/* ���ڽ�ް� */
                        } else {
                            /* 8���߂̏ꍇ */
                            SBI_SetSendData(SBIx, WRITE01[8]);/* ���ڽ�ް� */
                        }
                        
                    } else if (I2C_WCNT == 1) {
                        if (T0BF2 <= 8) {
                            /* 8�ȉ��̏ꍇ */
                            SBI_SetSendData(SBIx, WRITE02[T0BF2]);/* ���ڽ�ް� */
                        } else {
                            /* 8���߂̏ꍇ */
                            SBI_SetSendData(SBIx, WRITE02[8]);/* ���ڽ�ް� */
                        }
                        
                    } else {
                        /* �������Ȃ� */
                    }
                    I2C_WCNT++;/* ���M�����瑗�M�J�E���g�����Z */
                } else {
                    /* ���M�J�E���g�����M�\��񐔂����傫���Ȃ����� */
                    SBI_GenerateI2CStop(SBIx);  /* I2C���~ */
                    
                }
            }
        }
        
    } else if (I2C_STATE == 2) {
        /** �N�����ARTC�փf�[�^�����ݏ�������2 --------------------------------------**/
        if (sbi_sr.Bit.TRx) {
            /* ���M���[�h�̍�(1�͑��M) */
            if (sbi_sr.Bit.LastRxBit) {
                /* LRB=1:�ŏI�u��M�v�r�b�g��1(NACK�̂Ƃ�) */
                NACK5++;
                if (NACK5 < 5) {
                    I2C_WCNT = 0;
                    SBI_GenerateI2CStop(SBIx); /* I2C���~ */
                    __NOP();
                    SBI_SetSendData(SBIx, (RTC_ADDR | 0x00));/* �ēxRTC�A�h���X�𑗂� */
                    __NOP();
                    SBI_GenerateI2CStart(SBIx);/* I2C���J�n */
                } else {
                    SBI_GenerateI2CStop(SBIx); /* I2C���~ */
                }
            } else {
                /* LRB=0:�ŏI�u��M�v�r�b�g��0(ACK�̂Ƃ�) */
                NACK5 = 0;/* NACK�񐔂����Z�b�g���� */
                if (I2C_WCNT < 2) {
                    /* �J�E���g���܂��f�[�^���t�\��񐔈ȉ��̍� */
                    if (I2C_WCNT == 0) {
                        /* RTC�A�h���X�f�[�^�𑗐M���� */
                        if (T0BF0 <= 6) {
                            /* 6�ȉ��̏ꍇ */
                            SBI_SetSendData(SBIx, WRITE03[T0BF0]);/* �A�h���X�f�[�^���M */
                        } else {
                            /* 6���̏ꍇ */
                            SBI_SetSendData(SBIx, WRITE03[6]);/* �A�h���X�f�[�^���M */
                        }
                    } else if (I2C_WCNT == 1) {
                        /* RTC�ݒ�f�[�^�𑗐M���� */
                        if (T0BF0 <= 6) {
                            /* 6�ȉ��̏ꍇ */
                            SBI_SetSendData(SBIx, WRITE04[T0BF0]);/* �A�h���X�f�[�^���M */
                        } else {
                            /* 6���̏ꍇ */
                            SBI_SetSendData(SBIx, WRITE04[6]);/* �A�h���X�f�[�^���M */
                        }
                    }
                    I2C_WCNT++;/* ���M�����瑗�M�J�E���g�����Z */
                } else {
                    /* ���M�J�E���g�����M�\��񐔂����傫���Ȃ����� */
                    SBI_GenerateI2CStop(SBIx);  /* I2C���~ */
                }
            }
        } else {
            /* ��M���[�h�̍�(0�͎�M) */
            /* �������Ȃ� */
        }
        
    } else if (I2C_STATE == 3) {
        /** �N������RTC���玞���f�[�^�ǂݏo�����s���ӏ� -----------------------------**/
        if (sbi_sr.Bit.TRx) {
            /* ���M���[�h�̍�(1�͑��M) */
            if (sbi_sr.Bit.LastRxBit) {
                /* LRB=1:�ŏI�u��M�v�r�b�g��1(NACK�̂Ƃ�) */
                NACK5++;
                if (NACK5 < 5) {
                    SBI_GenerateI2CStop(SBIx); /* I2C���~ */
                    __NOP();
                    SBI_SetSendData(SBIx, (RTC_ADDR | 0x00));/* �ēxRTC�A�h���X�𑗂� */
                    __NOP();
                    SBI_GenerateI2CStart(SBIx); /* I2C���J�n */
                    I2C_WCNT = 0;
                } else {
                    SBI_GenerateI2CStop(SBIx); /* I2C���~ */
                }
                
            } else {
                /* LRB=0:�ŏI�u��M�v�r�b�g��0(ACK�̂Ƃ�) */
                NACK5 = 0;/* NACK�񐔂����Z�b�g���� */
                if (I2C_WCNT == 0) {
                    /* �J�E���g���܂��f�[�^���t�\��񐔈ȉ��̍� */
                    SBI_SetSendData(SBIx, 0x10);/* RTC�A�h���X�f�[�^�𑗐M���� */
                    I2C_WCNT++;   /* ���M�����瑗�M�J�E���g�����Z */
                } else { /* ���M�J�E���g�����M�\��񐔂����傫���Ȃ����� */
                    SBI_GenerateI2CStop(SBIx);  /* I2C���~ */
                    I2C_WCNT++;   /* ���M�����瑗�M�J�E���g�����Z */
                }
            }
            
        } else {
            /* ��M���[�h�̍�(0�͎�M) */
            /* �u�b�����T�����N�v�f�[�^�Ǎ��� */
            if (I2C_RCNT == 0) {
                RTCD_R[I2C_RCNT] = SBI_GetReceiveData(SBIx);/* RTC����ǂݏo�����f�[�^ */
            } else if (I2C_RCNT >= 1) {
                RTCD_R[I2C_RCNT - 1] = SBI_GetReceiveData(SBIx);/* RTC����ǂݏo�����f�[�^ */
            }
            
            if (I2C_RCNT == 7) {
                SBI_SetI2CACK(SBIx, DISABLE);/* ���̃f�[�^��M����ACK���Ȃ� */
            } else if (I2C_RCNT >= 8) {/* ��M�񐔂���萔���z������ */
                SBI_GenerateI2CStop(SBIx);
                T0BF1 = 1;
            }
            
            I2C_RCNT++;
        }
        
    } else if (I2C_STATE == 4) {
        /* DA���ް�������̧�ݽӰ�ނɂ���(����̂�)���� */
        if (sbi_sr.Bit.MasterSlave) {
            /* �}�X�^�[���[�h�̍�(1�̓}�X�^�[���[�h) */
            if (sbi_sr.Bit.TRx) {
                /* ���M���[�h�̍�(1�͑��M) */
                if (sbi_sr.Bit.LastRxBit) {
                    /* LRB=1:�ŏI�u��M�v�r�b�g��1(NACK�̂Ƃ�) */
                    NACK5 = 1;
                    SBI_GenerateI2CStop(SBIx);/* I2C���~ */
                    I2C_WCNT = 0;/* ���M�񐔸ر */
                } else {
                    /* LRB=0:�ŏI�u��M�v�r�b�g��0(ACK�̂Ƃ�) */
                    NACK5 = 0;
                    if (I2C_WCNT <= 3) {
                        /* �J�E���g���܂��f�[�^���ȉ��̍� */
                        SBI_SetSendData(SBIx, I2CBUF[I2C_WCNT]);/* ���M����� */
                        I2C_WCNT++;/* ���M�����瑗�M�J�E���g�����Z */
                    } else {
                        /* ���M�J�E���g�����M�񐔂����傫���Ȃ�����(�f�[�^���M���I��) */
                        SBI_GenerateI2CStop(SBIx);/* I2C���~ */
                    }
                }
            } else {
                /* ��M���[�h�̍� */
                /* Do nothing */
            }
        } else {
            /* �X���[�u���[�h�̍� */
            /* Do nothing */
        }
        
    } else if (I2C_STATE == 5) {
        /* DA���ް����޼��ْl���������ޏ��� */
        if (sbi_sr.Bit.MasterSlave) {
            /* �}�X�^�[���[�h�̍�(1�̓}�X�^�[���[�h) */
            if (sbi_sr.Bit.TRx) {
                /* ���M���[�h�̍�(1�͑��M) */
                if (sbi_sr.Bit.LastRxBit) {
                    /* LRB=1:�ŏI�u��M�v�r�b�g��1(NACK�̂Ƃ�) */
                    NACK5++;
                    SBI_GenerateI2CStop(SBIx);/* I2C���~ */
                    I2C_WCNT = 0;/* ���M�񐔸ر */
                } else {
                    /* LRB=0:�ŏI�u��M�v�r�b�g��0(ACK�̂Ƃ�) */
                    NACK5 = 0;
                    /* DAC�������ݏ������ł���ꍇ */
                    if (I2C_WCNT <= 3) {
                        /* �J�E���g���܂��f�[�^���ȉ��̍� */
                        SBI_SetSendData(SBIx, I2CBUF[I2C_WCNT]);/* ���M����� */
                        I2C_WCNT++;/* ���M�����瑗�M�J�E���g�����Z */
                    } else {
                        /* ���M�J�E���g�����M�񐔂����傫���Ȃ�����(�f�[�^���M���I��) */
                        SBI_GenerateI2CStop(SBIx);/* I2C���~ */
                    }
                    
                }
            } else {
                /* ��M���[�h�̍� */
                /* Do nothing */
            }
        } else {
            /* �X���[�u���[�h�̍� */
            /* Do nothing */
        }
        
    } else {
        /** RTC�փf�[�^�����ݏ������s���ӏ� -----------------------------------------**/
        if (sbi_sr.Bit.TRx) {
            /* ���M���[�h�̍�(1�͑��M) */
            if (sbi_sr.Bit.LastRxBit) {
                /* LRB=1:�ŏI�u��M�v�r�b�g��1(NACK�̂Ƃ�) */
                NACK5++;/* NACK�񐔂����Z���� */
                if (NACK5 < 5) {
                    /* NACK�񐔂�5�񖢖��ł���ꍇ */
                    I2C_WCNT = 0;
                    SBI_GenerateI2CStop(SBIx); /* I2C���~ */
                    __NOP();
                    SBI_SetSendData(SBIx, (RTC_ADDR | 0x00));/* �ēxRTC�A�h���X�𑗂� */
                    __NOP();
                    SBI_GenerateI2CStart(SBIx); /* I2C���J�n */
                    
                } else {
                    SBI_GenerateI2CStop(SBIx); /* I2C���~ */
                }
                
            } else {
                /* LRB=0:�ŏI�u��M�v�r�b�g��0(ACK�̂Ƃ�) */
                NACK5 = 0;/* NACK�񐔂����Z�b�g���� */
                
                if (I2C_WCNT <= 6) {
                    /* �J�E���g���܂��f�[�^���t�\��񐔈ȉ��̍� */
                    if (I2C_WCNT == 0) {
                        /* ����̏ꍇ */
                        SBI_SetSendData(SBIx, 0x11);/* RTC�A�h���X�f�[�^�𑗐M����("��"�A�h���X����) */
                        
                    } else if (I2C_WCNT == 1) {
                        /* ���ް��̏ꍇ */
                        REG_01 = (SETST_BUF[1] / 10);/* 10�̈ʒ��o */
                        REG_02 = (SETST_BUF[1] % 10);/* 1�̈ʒ��o */
                        REG_00 = ((REG_01 << 4) | REG_02);/* RTC�����̓��t�ް��쐬 */
                        
                        SBI_SetSendData(SBIx, REG_00);/* ���M����� */
                    } else if (I2C_WCNT == 2) {
                        /* ���ް��̏ꍇ */
                        REG_01 = (SETST_BUF[2] / 10);/* 10�̈ʒ��o */
                        REG_02 = (SETST_BUF[2] % 10);/* 1�̈ʒ��o */
                        REG_00 = ((REG_01 << 4) | REG_02);/* RTC�����̓��t�ް��쐬 */
                        
                        SBI_SetSendData(SBIx, REG_00);/* ���M����� */
                    } else if (I2C_WCNT == 3) {
                        /* �T�ް��̏ꍇ */
                        REG_01 = (SETST_BUF[3] / 10);/* 10�̈ʒ��o */
                        REG_02 = (SETST_BUF[3] % 10);/* 1�̈ʒ��o */
                        REG_00 = ((REG_01 << 4) | REG_02);/* RTC�����̓��t�ް��쐬 */
                        
                        SBI_SetSendData(SBIx, REG_00);/* ���M����� */
                    } else if (I2C_WCNT == 4) {
                        /* ���ް��̏ꍇ */
                        REG_01 = (SETST_BUF[4] / 10);/* 10�̈ʒ��o */
                        REG_02 = (SETST_BUF[4] % 10);/* 1�̈ʒ��o */
                        REG_00 = ((REG_01 << 4) | REG_02);/* RTC�����̓��t�ް��쐬 */
                        
                        SBI_SetSendData(SBIx, REG_00);/* ���M����� */
                    } else if (I2C_WCNT == 5) {
                        /* ���ް��̏ꍇ */
                        REG_01 = (SETST_BUF[5] / 10);/* 10�̈ʒ��o */
                        REG_02 = (SETST_BUF[5] % 10);/* 1�̈ʒ��o */
                        REG_00 = ((REG_01 << 4) | REG_02);/* RTC�����̓��t�ް��쐬 */
                        
                        SBI_SetSendData(SBIx, REG_00);/* ���M����� */
                    } else if (I2C_WCNT == 6) {
                        /* �N�ް��̏ꍇ */
                        REG_01 = (SETST_BUF[6] / 10);/* 10�̈ʒ��o */
                        REG_02 = (SETST_BUF[6] % 10);/* 1�̈ʒ��o */
                        REG_00 = ((REG_01 << 4) | REG_02);/* RTC�����̓��t�ް��쐬 */
                        
                        SBI_SetSendData(SBIx, REG_00);/* ���M����� */
                    }
                    
                    I2C_WCNT++;/* ���M�����瑗�M�J�E���g�����Z */
                } else { /* ���M�J�E���g�����M�\��񐔂����傫���Ȃ����� */
                    /* ���M�񐔂�7�ɂȂ�Ƃ����ɂȂ�܂� */
                    SBI_GenerateI2CStop(SBIx);  /* I2C���~ */
                }
            }
        } else {
            /* ��M���[�h�̍�(0�͎�M) */
            /* �������Ȃ� */
        }
        
    }/* �����܂� */
    
    return;/* �߂� */
}