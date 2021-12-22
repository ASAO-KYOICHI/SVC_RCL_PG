/**
 *******************************************************************************
 * ̧�ٖ��Fmain.c
 * �쐬�ҁF����@����
 * �p�r�@�FOperating System
 *******************************************************************************
 */
#include "main.h"

SBI_InitI2CTypeDef myI2C;
UART_InitTypeDef   myUART;

int main(void)
{
    /*****�����E�H�b�`�h�b�O�̐ݒ�*********************************************/
    WDT_SetDetectTime(WDT_DETECT_TIME_EXP_19);/* ��52ms�Ŕ������� */
    WDT_SetOverflowOutput(WDT_WDOUT);/* ��ϵ��ް�۰����ϲ��ؾ�� */
    WDT_Enable();/* �����ޯ�ނ̋��� */
    /*****�����E�H�b�`�h�b�O�̐ݒ肱���܂�*************************************/
    
    /* �|�[�g�̏�������������J�n-------------------------------------------- */
    /* ���̓|�[�g�ɐݒ�****************************************************** */
    TSB_PA->IE  = 0x01;/* PA0������߰Ăɐݒ� */
    TSB_PD->IE  = 0x4F;/* PD0~PD3,PD6������߰Ăɐݒ肷�� */
    TSB_PE->IE  = 0xC2;/* PE1������߰Ăɐݒ肷�� */
    
    /* �o�̓|�[�g�ɐݒ�****************************************************** */
    TSB_PA->DATA |= 0xC0;/* LCD/VFD�̐�����iE�����j��HIGH�ɂ��� */
    TSB_PA->DATA &= 0xC7;/* LED�o�͂��}�X�N���� */
    TSB_PA->CR |= 0xFE;/* PA1~PA7���o���߰Ăɂ��� */
    
    TSB_PC->CR |= 0xFC;/* PC2~PC7���o���߰Ăɂ��� */
    TSB_PC->DATA &= 0x03;/* �o�̓N���A */
    
    TSB_PE->CR |= 0x1C;/* PE2~PE4���o���߰Ăɂ��� */
    TSB_PE_DATA_PE4 = 0;/* NV-RAM�`�b�v�Z���N�g�o�͏����l(Disable)�Z�b�g */
    
    TSB_PD->CR |= 0x30;/* PD4,5���o�̓|�[�g�ɂ��� */
    TSB_PD->DATA &= 0xCF;/* PD4,5(NV-RAM�ւ�DOUT���C����SK��LOW�ɂ���) */
    
    TSB_PF->CR  = 0x10;/* PF4���o���߰Ăɂ��� */
    TSB_PF_DATA_PF4 = 0;/* LCD�o�b�N���C�g�o�͂�OFF�ɂ��� */
    
    TSB_PG->CR |= 0xFF;/* PG0~PG7���o�̓|�[�g�ɐݒ� */
    TSB_PG->DATA = 0;/* �o�͂��N���A */
    
    TSB_PN->CR |= 0xFF;/* PN0~PN7���o�̓|�[�g�ɐݒ� */
    TSB_PN->DATA = 0;/* �o�͂��N���A */
    
    /*************AD�R���o�[�^�[�ݒ�*******************************************/
    /* ADC�N���b�N�̃Z�b�g */
    ADC_SetClk(ADC_HOLD_FIX, ADC_FC_DIVIDE_LEVEL_NONE);
    /* �\�t�g�E�F�A�g���K�p�v���O�����ɂ��AD�ϊ����ʃ��W�X�^��I�� */
    /* TRG_ENABLE()�őI��AD�`���l���������� */
    /* ���W�X�^0 */
    ADC_SetSWTrg(ADC_REG0, TRG_ENABLE(ADC_AIN0));
    /* ���W�X�^1 */
    ADC_SetSWTrg(ADC_REG1, TRG_ENABLE(ADC_AIN1));
    /* ���W�X�^2 */
    ADC_SetSWTrg(ADC_REG2, TRG_ENABLE(ADC_AIN2));
    
    ADC_SetResolution(ADC_12BITS);/* AD�ϊ��̕���\��12�r�b�g�ɂ��� */
    /* AVREFH�`AVREFL�Ԃ̃��t�@�����X�d���̐���BDISABLE�Ń��Z�b�g���ȊO�͏펞�ʓd�ƂȂ� */
    ADC_SetLowPowerMode(DISABLE);
    /* AD�ϊ����W���[���̗L���� */
    ADC_Enable();
    /*********AD�R���o�[�^�[�ݒ肱���܂�***************************************/
    
    /* SBI0��I2C���[�h�ɂ��鏉���ݒ� */
    /****************** PG�s��0�`1��I2C���[�h�ɐݒ肷�� ***********************/
    /*PC0�`1�|�[�g�̏o�͋���*/
    GPIO_SetOutputEnableReg(GPIO_PC, GPIO_BIT_0, ENABLE);/* PC0�ASDA0���C�� */
    GPIO_SetOutputEnableReg(GPIO_PC, GPIO_BIT_1, ENABLE);/* PC1�ASCL0���C�� */
    
    if (TSB_PC_DATA_PC0 == 0) {
        /* SDA�ł���PC0��Low����t���ł������ꍇ */
        /* SDA,SCL���C�����J�`�J�`���� */
        TSB_PC_DATA_PC1 = 1;/* PC1��High�ɂ��� */
        for (uint8_t j=0;j<=5;j++) {
            __NOP();
        }
        for (uint8_t i=0;i<=9;i++) {
            TSB_PC->CR &= 0xFE;/* PC0��HI-Z�ɂ��� */
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
            TSB_PC->CR |= 0x01;/* PC0���o�͋����� */
            TSB_PC_DATA_PC0 = 0;/* PC0��Low�ɂ��� */
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
            TSB_PC_DATA_PC1 = 0;/* PC1��Low�ɂ��� */
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
            TSB_PC->CR &= 0xFE;/* PC0��HI-Z�ɂ��� */
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
            TSB_PC_DATA_PC1 = 1;/* PC1��High�ɂ��� */
            for (uint8_t j=0;j<=5;j++) {
                __NOP();
            }
        }
        TSB_PC->CR |= 0x01;/* PC0�̏o�͋��� */
    }
    
    /*PC0�`1�|�[�g�̓��͋���*/
    GPIO_SetInputEnableReg(GPIO_PC, GPIO_BIT_0, ENABLE);/* PC0�ASDA0���C�� */
    GPIO_SetInputEnableReg(GPIO_PC, GPIO_BIT_1, ENABLE);/* PC1�ASCL0���C�� */
    
    /*�t�@���N�V�������W�X�^�̐ݒ�(SDA��SCL�ɂ��Ă���)*/
    GPIO_EnableFuncReg(GPIO_PC, GPIO_FUNC_REG_3, GPIO_BIT_0);/*SDA�ɐݒ�*/
    GPIO_EnableFuncReg(GPIO_PC, GPIO_FUNC_REG_3, GPIO_BIT_1);/*SCL�ɐݒ�*/
    /*PG0�`1���I�[�v���h���C���ݒ�*/
    GPIO_SetOpenDrain(GPIO_PC, GPIO_BIT_0, ENABLE);/*PG0�I�[�v���h���C������*/
    GPIO_SetOpenDrain(GPIO_PC, GPIO_BIT_1, ENABLE);/*PG1�I�[�v���h���C������*/
    /*�ǂ����Ă���Ȑݒ肵�Ă��邩�̓f�[�^�V�[�g���Q��*/
    /*******************I2C�ݒ�************************************************/
    /* SBI0�����ݒ� */
    myI2C.I2CSelfAddr = SELF_ADDR;/*�����̃A�h���X�ݒ�*/
    myI2C.I2CDataLen  = SBI_I2C_DATA_LEN_8; /* I2C���[�h�̓]���r�b�g���B8�r�b�g�ɂ��� */
    myI2C.I2CACKState = ENABLE;/* ACK��L���ɐݒ肷�� */
    myI2C.I2CClkDiv   = SBI_I2C_CLK_DIV_200;/* I2C�̓]���ۯ� = fsys/200 = ��200kHz */
    SBI_Enable(TSB_SBI0);/*�V���A���o�X�C���^�[�t�F�[�X�̋���*/
    SBI_SWReset(TSB_SBI0);/*SBI��H������������*/
    SBI_InitI2C(TSB_SBI0, &myI2C);/*I2C�ʐM�̏�����*/
    /***************I2C�ݒ肱���܂�********************************************/
    
    /************�^�C�}�[���荞�݂̐ݒ�****************************************/
    TMRB_InitTypeDef m_tmrb;/*TMRB�ݒ�p�\���̂�main���ň������߂ɖ��O��t����*/
    
    /* �` TMRB�ݒ�p�\���̂̒�` �` */
    m_tmrb.Mode = TMRB_INTERVAL_TIMER;/*���[�h�̓C���^�[�o���^�C�}���[�h*/
    m_tmrb.ClkDiv = TMRB_CLK_DIV_8;/*�C���^�[�o���^�C�}�̃\�[�X�N���b�N��f/8*/
    m_tmrb.TrailingTiming = TMRB_1MS;/*�^�C�}RG1�ɏ������ޒl�B�A�b�v�J�E���^�Ɣ�r����*/
    /*���ꁪ�͗v�v�Z�ӏ� 1ms=0x1388�@tmpm380_CMN.h�̒��Œ�`���Ă���*/
    m_tmrb.UpCntCtrl = TMRB_AUTO_CLEAR;/*�A�b�v�J�E���^�̓���ݒ�.*/
    /*Trailing Timing�Őݒ肵���l�ƃA�b�v�J�E���^����v�����ۂ�0�N���A���čăX�^�[�g����*/
    m_tmrb.LeadingTiming = TMRB_1MS;/*�^�C�}RG0�ɏ������ޒl�B�A�b�v�J�E���^�Ɣ�r����*/
    /*���ꁪ�͗v�v�Z�ӏ� 1ms=0x1388�@main.h�̒��Œ�`���Ă���*/
    
    /*�����Ń^�C�}�[���荞�݂ɂ��1ms�u���Ƀf�R�[�_�[�o�͂��J�E���g�A�b�v����*/
    TMRB_Enable(TSB_TB0);/*TSB_TB0�`���l����TMRB�^�C�}�̓��싖��*/
    TMRB_Init(TSB_TB0, &m_tmrb);/*TSB_TB0�`���l���̏�����*/
    /********�^�C�}�[���荞�݂̐ݒ肱���܂�************************************/
    
    /********�O��°ٗpUART�̗L��������*****************************************/
    TSB_PE->FR1 |= 0x03;/* UART�@�\��ON */
    TSB_PE->CR  |= 0x01;/* �o�͂ɃZ�b�g */
    TSB_PE->IE  |= 0x02;/* ���͂ɃZ�b�g */
    
    myUART.BaudRate = (uint32_t)9600;/* �{�[���[�g�ݒ� */
    myUART.DataBits = UART_DATA_BITS_7;/* �r�b�g���ݒ� */
    myUART.StopBits = UART_STOP_BITS_1;/* �X�g�b�v�r�b�g�ݒ� */
    myUART.Parity   = UART_EVEN_PARITY;/* �p���e�B�r�b�g�ݒ� */
    myUART.Mode     = UART_ENABLE_TX|UART_ENABLE_RX;/* ����M���[�h�ɐݒ� */
    myUART.FlowCtrl = UART_NONE_FLOW_CTRL;/* �t���[����Ȃ� */
    
    UART_Enable(UART0);/* UART0�̗L���� */
    UART_Init(UART0, &myUART);/* ��L�Őݒ肵��UART���ڂ̗L���� */

    UART_TRxAutoDisable(UART0, UART_RXTXCNT_AUTODISABLE);/* ����M�̎����֎~�ɐݒ�
    �@�@�@ �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ (�f�[�^�V�[�g�Q��) */
    UART_FIFOConfig(UART0, DISABLE);/* UART0��FIFO��s���Ƃ��� */
    /********�O��°ٗpUART�̗L�������������܂�*********************************/
    
    /******* ���荞�ݗD�揇�ʐݒ� *********************************************/
    NVIC_SetPriority(INT3_IRQn, 0x00);/* INT3���荞�ݗD�揇�ʕύX */
    NVIC_SetPriority(INTADSFT_IRQn, 0x02);/* AD�ϊ����荞�ݗD�揇�ʕύX */
    NVIC_SetPriority(INTTB00_IRQn, 0x02);/* �^�C�}���荞�ݗD�揇�ʕύX */
    NVIC_SetPriority(INTSBI0_IRQn, 0x02);/* I2C���荞�ݗD�揇�ʕύX */
    NVIC_SetPriority(INTTX0_IRQn, 0x02);/* UART0 ���M���荞�ݗD�揇�ʕύX */
    NVIC_SetPriority(INTRX0_IRQn, 0x02);/* UART0 ��M���荞�ݗD�揇�ʕύX */
    
    /* �V�X�e���n���h�����荞�ݗD�揇�ʕύX(�g�p���Ȃ��񂾂��ǂ�) */
    NVIC_SetPriority(MemoryManagement_IRQn, 0x01);
    NVIC_SetPriority(BusFault_IRQn, 0x01);
    NVIC_SetPriority(UsageFault_IRQn, 0x01);
    NVIC_SetPriority(SVCall_IRQn, 0x01);
    NVIC_SetPriority(DebugMonitor_IRQn, 0x01);
    NVIC_SetPriority(PendSV_IRQn, 0x01);
    NVIC_SetPriority(SysTick_IRQn, 0x01);
    
    /******* INT3���荞�݋��ݒ� *********************************************/
    CG_SetSTBYReleaseINTSrc(CG_INT_SRC_3, CG_INT_ACTIVE_STATE_FALLING, DISABLE);/*INT3�͗�����G�b�W�Ŋ��荞�ݔ����ɐݒ�*/
    CG_SetSTBYReleaseINTSrc(CG_INT_SRC_3, CG_INT_ACTIVE_STATE_FALLING, ENABLE);/*INT3�������͋���*/
    CG_ClearINTReq(CG_INT_SRC_3);/*���荞�ݗv��INT3�̉���*/
    NVIC_ClearPendingIRQ(INT3_IRQn);/*INT3���荞�ݕۗ����b�`�̃N���A*/
    NVIC_EnableIRQ(INT3_IRQn);/*INT3���荞�݋���*/
    /***** INT3���荞�݋��ݒ肱���܂� ***************************************/
    
    /** �^�C�}�[���荞�݋��� **/
    NVIC_EnableIRQ(INTTB00_IRQn);/* INTTB00�̊��荞�݋��� */
    TMRB_SetRunState(TSB_TB0, TMRB_RUN);/*�^�C�}�[�̃A�b�v�J�E���^�̋N��*/
    
    /* Main Roop */
    while (1) {
        /*-- OS�������� --*/
        if (ERRFG == 0xA1) {
            /* NMI�������Ă����ꍇ */
            break;/* �߂� */
            
        }
        
        /** AD���荞��(�\�t�g�E�F�AAD�ϊ�������) **/
        NVIC_EnableIRQ(INTADSFT_IRQn);/* INTADSFT�̊��荞�݋��� */
        /** �^�C�}�[���荞�݋��� **/
        NVIC_EnableIRQ(INTTB00_IRQn);/*  INTTB00�̊��荞�݋���  */
        /** I2C���荞�݋��� **/
        NVIC_EnableIRQ(INTSBI0_IRQn);/*  INTSBI0�̊��荞�݋���  */
        /** �O���c�[���ʐM�̊��荞�݋��� **/
        NVIC_EnableIRQ(INTTX0_IRQn);/*   UART0 ���M���荞�݋��� */
        NVIC_EnableIRQ(INTRX0_IRQn);/*   UART0 ��M���荞�݋��� */
        
        TMRB_SetRunState(TSB_TB0, TMRB_RUN);/*�^�C�}�[�̃A�b�v�J�E���^�̋N��*/
        
        OS_TSKCon1 *OS1;/* OS�ݒ�l��ǂނ��߂̃|�C���^�\���̂ɖ��O������ */
        OS1 = OSTSKCon1S();/* �֐��o�R�Ń|�C���^��OSparam�ɃA�N�Z�X */

        static uint8_t NextRunTask = (TSKNUM - 1);/* �^�X�N�ԍ��̏����l */
        
        NextRunTask++;/* ����N���^�X�N�ԍ����C���N�������g */
        if (NextRunTask <= (TSKNUM - 1)) {
            /* �͈͓��ł���Ή������Ȃ� */
        } else {
            /* �͈͊O�ł����0�N���A���� */
            NextRunTask = 0;
        }
        
        if (((OS1->TSK_COND[NextRunTask]) & 0x80) == 0x80) {
            /* TSK_COND[NextRunTask]��7�r�b�g�ڂ�1�̂Ƃ� */
            NVIC_DisableIRQ(INTADSFT_IRQn);/* AD�ϊ����荞�݋֎~ */
            NVIC_DisableIRQ(INTTB00_IRQn);/* �^�C�}�[���荞�݋֎~ */
            NVIC_DisableIRQ(INTSBI0_IRQn);/* I2C���荞�݋��� */
            NVIC_DisableIRQ(INTTX0_IRQn);/* UART0 ���M���荞�݋֎~ */
            NVIC_DisableIRQ(INTRX0_IRQn);/* UART0 ��M���荞�݋֎~ */
            
            if (((OS1->TSK_COND[NextRunTask]) & 0x20) == 0x00) {
                /* TSK_COND[NextRunTask]��5�r�b�g�ڂ�0�̂Ƃ� */
                /* OS���������^�X�N���Z���N�g���鏈�� */
                switch (NextRunTask) {
                case 0:
                    DEVICE_INIT();/*�������A�v��*/
                    break;
                case 1:
                    INPUT_CONT();/*���͏����A�v��*/
                    break;
                case 2:
                    AD_CONVERTER();/*AD�ϊ������A�v��*/
                    break;
                case 3:
                    CCONT();/* �^���p�A�v�� */
                    break;
                case 4:
                    WCONT();/* ���ʐ���A�v�� */
                    break;
                case 5:
                    DCONT();/* �\������A�v�� */
                    break;
                case 6:
                    OPRCNT();/* ���쐧����� */
                    break;
                case 7:
                    COMCNT();/* �ʐM������� */
                    break;
                default:
                    break;/*�͈͊O�̒l�������ꍇ�̓u���[�N����*/
                }
            } else {
                /* �������Ȃ� */
            }
        } else {
            /* �������Ȃ� */
        }
        /*-- OS�������� �����܂� --*/
    }
    /* Main Loop End */
    
    if (D_INISQ > 0) {
        /* �f�[�^�ǂݏo���������Ă����ꍇ */
        KEEP();/* ��������A�f�[�^�ۑ����� */
        
    }
    CPUERR();/* "Good bye!" */
    
}

/* --�O�t��WDT�̃N���A------------------------------------------------------- */
void WDTCLR(void)
{
    TSB_PN_DATA_PN0 = (TSB_PN_DATA_PN0 ^ 1);/* �r�b�g�̔��]���� */
    return;
}

/**
 *���́F�O���c�[���p�̒ʐM�|�[�g0���M���荞�ݏ���
 *�쐬�ҁF����@����
 *�����F�ʐM�|�[�g0�̑��M����
**/
void INTTX0_IRQHandler(void)
{
    if (((TXFLG0 & 0x02) == 0x02) || (TXLEN0 == 0) || (TXSET0 == 0)) {
        /* ���M�L�����Z���̏ꍇ or ���M�I�� or ���M�o�b�t�@����̏ꍇ */
        TXFLG0 &= 0xEE;/* ���M���Ƒ��M�t���O���폜 */
        TXFLG0 |= 0x20;/* ���M�o�b�t�@�G���v�e�B�t���O�Z�b�g */
        
    } else {
        /* �����ł͂Ȃ��ꍇ */
        TSB_SC0->BUF = TBUF0[(TXPTR0 & 0x0FF)];/* ���M�����Z�b�g */
        TXPTR0++;/* ���M�|�C���^���C���N�������g */
        TXPTR0 &= 0x0FF;/* MAX => 255(���M�|�[�g0�̃o�b�t�@����)�ɂȂ�悤�Ƀ}�X�N���� */
        TXSET0--;/* ���M�o�b�t�@�Z�b�g�� - 1 */
        TXLEN0--;/* ���M�� - 1 */
        
    }
    
    return;/* �߂� */
}

/**
 *���́F�O���c�[���p�̒ʐM�|�[�g0��M���荞�ݏ���
 *�쐬�ҁF����@����
 *�����F�ʐM�|�[�g0�̎�M����
**/
void INTRX0_IRQHandler(void)
{
    uint8_t REG_00 = 0;
    if ((TSB_SC0->CR & 0x1C) > 0x00) {
        /* ��M����������̃G���[������Ή������Ȃ����� */
        REG_00 = TSB_SC0->BUF;/* �_�~�[�ǂݏo�� */
        REG_00 = 0x04;
        USTS0 |= REG_00;/* ������M���G���[�Z�b�g */
        
    } else {
        /* �G���[�������ꍇ */
        if (RXLEN0 >= 255) {
            /* ��M������M�o�b�t�@���傫���Ȃ��Ă��܂����ꍇ */
            REG_00 = TSB_SC0->BUF;/* �_�~�[�ǂݏo�� */
            
            REG_00 = 0x08;
            USTS0 |= REG_00;/* �o�b�t�@�I�[�o�[�����G���[�Z�b�g */
            
        } else {
            /* ��M������M�o�b�t�@�ȓ��̏ꍇ */
            RBUF0[(RXPTR0 + RXLEN0) & 0x0FF] = (TSB_SC0->BUF & 0x7F);/* ��M�o�b�t�@�Ɏ�M�f�[�^���i�[���� */
            RXLEN0++;/* ��M�f�[�^���C���N�������g */
            
        }
    }
    
    return;/* �߂� */
}