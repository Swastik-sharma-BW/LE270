/******************************************************
*
* Please copy the contents of aw88262.c to the main.c *
*
* External API are as follows:
* 1.aw_init();
* 2.aw_pa_start();
* 3.aw_pa_stop();
*
******************************************************/

#include "aw88266.h"
#include "codecDrv.h"


/****************************************************************************
*
* Please modify the following information(#1~#5) according to the platform **
*
* Taking the STM32HAL library as an example *********************************
*
****************************************************************************/
#define	AW_FAIL		(-1)
#define	AW_OK		(0)


/* #1.Device I2C address, taking 0x30 as an example*/
#define	AW88266_I2C_ADDR	(0x34)

#define AW88266_DEFAULT_CONFIG()                         \
{                                                       \
        .adcInput  = CODEC_ADC_INPUT_LINE1,             \
        .dacOutput = CODEC_DAC_OUTPUT_ALL,              \
        .codecMode = CODEC_MODE_BOTH,                   \
        .codecIface = {                                 \
            .mode    = CODEC_MODE_SLAVE,                \
            .fmt     = CODEC_I2S_MODE,                  \
            .samples = CODEC_16K_SAMPLES,               \
            .bits    = CODEC_BIT_LENGTH_16BITS,         \
            .channel = CODEC_MONO,                      \
            .polarity = 1,                              \
        },                                              \
};


/* #2.Print Function*/
#define aw_printf(format, ...) \
	do { \
		printf("[Awinic]%s: " format "\r\n", __func__, ##__VA_ARGS__); \
	} while (0)


static int32_t AW88266WriteReg(uint8_t regAddr, uint16_t data)
{
    int32_t ret = 0;
    ret = codecI2cWrite2(AW88266_I2C_ADDR, regAddr, data);

    if (ret == 1)
    {
        // if fail , write again
        ret = codecI2cWrite2(AW88266_I2C_ADDR, regAddr, data);
    }
#ifdef FEATURE_OS_ENABLE
    DEBUG_PRINT(UNILOG_PLA_DRIVER, AW88266WriteReg_1, P_DEBUG, "zzg reg write: reg=%x, data=%x,%d", regAddr, data,ret);
#endif

    return ret;
}

static uint16_t AW88266ReadReg(uint8_t regAddr, uint16_t regVal)
{
    regVal = codecI2cRead2(AW88266_I2C_ADDR, regAddr);
#ifdef FEATURE_OS_ENABLE
    DEBUG_PRINT(UNILOG_PLA_DRIVER, AW88266ReadReg_1, P_DEBUG, "zzg reg read: reg=%x, data=%x", regAddr, regVal);
#endif
    return regVal;
}


/************************************************************
*
****************** Function definition **********************
*
************************************************************/


/******* General I2C Read/Write API *******/

int i2c_read(unsigned char reg_addr, unsigned int *reg_data)
{
    int ret = AW_OK;
	uint16_t data = 0;
    *reg_data = AW88266ReadReg(reg_addr, data);
#ifdef FEATURE_OS_ENABLE
    DEBUG_PRINT(UNILOG_PLA_DRIVER, i2c_read0_1, P_DEBUG, "zzg i2c_read: reg=%x, data=%x", reg_addr, *reg_data);
#endif
    return ret;
}

int i2c_write(unsigned char reg_addr, unsigned int reg_data)
{
    int ret = AW_OK;
    AW88266WriteReg(reg_addr, reg_data);
    return ret;
}

/*General I2C Write bits API*/
int i2c_write_bits(unsigned char reg_addr, unsigned int mask, unsigned int reg_data)
{
	int ret = -1;
	unsigned int reg_val = 0;
	ret = i2c_read(reg_addr, &reg_val);
#ifdef FEATURE_OS_ENABLE
    DEBUG_PRINT(UNILOG_PLA_DRIVER, i2c11_write_1, P_DEBUG, "zzg i2c_write_bits11 reg=%x, data=%x, ret=%d", reg_addr, reg_val,ret);
#endif
	if (ret < 0) {
		return ret;
	}
	reg_val &= mask;
	reg_val |= reg_data;
	ret = i2c_write(reg_addr, reg_val);
#ifdef FEATURE_OS_ENABLE
    DEBUG_PRINT(UNILOG_PLA_DRIVER, i2c22_write_2, P_DEBUG, "zzg i2c_write_bits22: reg=%x, data=%x, ret=%d", reg_addr, reg_val,ret);
#endif
	if (ret < 0) {
		return ret;
	}
	return AW_OK;
}
/*NOTE: i2c read/write concurrency is not allowed at this time, please use LOCK protection*/



/******* Relevant internal API of aw_init() *******/


/*step2 of aw_init():register configuration sequence*/
const uint16_t config_register[] = {
0x03,0xffff,	/*Sequence：reg_addr,reg_data*/
0x04,0xb240,
0x05,0x6000,
0x06,0xdc08,	/*0x06:configure I2S format,taking mono/32bits/48kHz as an example*/
0x07,0x0010,
0x08,0x98a9,
0x09,0x0030,
0x0a,0x01e0,
0x0b,0x1c64,
0x0c,0x001b,
0x0d,0x4abb,
0x0e,0x4500,
0x10,0x5700,
0x11,0x2101,
0x12,0x6003,
0x38,0x002c,
0x51,0x00d8,
0x52,0x2ce8,
0x53,0x3f08,
0x54,0x0202,
0x55,0x3024,
0x56,0x9491,
0x57,0x1105,
0x58,0x9884,
0x60,0x9b19,
0x61,0x6b3b,
0x62,0xfd40,
0x63,0x3067,
0x64,0xa008,
0x65,0x2abf,
0x66,0x460c,
0x67,0xc989,
0x68,0x3541,
0x69,0x4db8,
0x6a,0xd6f9,
0x6b,0x7ace,
0x6c,0xec7c,
0x6d,0x000c,
};

#define CODEC_CTRL_PAD             42
#define CODEC_CTRL_GPIO            36
#define CODEC_CTRL_PORT            ((CODEC_CTRL_GPIO) / 16)
#define CODEC_CTRL_PIN             ((CODEC_CTRL_GPIO) % 16)


static void aw_codec_poweron(bool enable)
{
    PadConfig_t     padConfig = {0};
    GpioPinConfig_t pinConfig = {0};


    slpManAONIOPowerOn();
    PAD_getDefaultConfig(&padConfig);
    padConfig.mux = PAD_MUX_ALT0;
    PAD_setPinConfig(CODEC_CTRL_PAD, &padConfig);

    pinConfig.pinDirection = GPIO_DIRECTION_OUTPUT;
    pinConfig.misc.initOutput = 0;
    GPIO_pinConfig(CODEC_CTRL_PORT, CODEC_CTRL_PIN, &pinConfig);

    GPIO_pinWrite(CODEC_CTRL_PORT, 1 << CODEC_CTRL_PIN, enable ? (1 << CODEC_CTRL_PIN) : 0);
}

/*step1 of aw_init():hardware reset*/
static void aw_hw_reset()
{
	aw_codec_poweron(0);		/*Reset pin pulled down*/
	osDelay(1);			/*delay 1ms*/
	aw_codec_poweron(1);		/*Reset pin pulled up*/
	osDelay(2);			/*delay 2ms*/
}

/*step2 of aw_init(): update vcalb*/
void aw_set_vcalb()
{
	unsigned int reg_val;
	int vcalb, icalk, vcalk;
	int16_t icalk_val, vcalk_val, reg_icalk, reg_vcalk;

	i2c_read(0x78, &reg_val);
	reg_icalk = (uint16_t)reg_val & (0x03ff);
	if (reg_icalk & (0x0200)) {
		reg_icalk = reg_icalk | (0xFC00);
	}
	icalk_val = (int16_t)reg_icalk;		/*get icalk_val*/

	i2c_read(0x79, &reg_val);
	reg_vcalk = (uint16_t)reg_val & (0x03ff);
	if (reg_vcalk & (0x0200)) {
		reg_vcalk = reg_vcalk | (0xFC00);
	}
	vcalk_val = (int16_t)reg_vcalk;		/*get vcalk_val*/

	icalk = 1000 + 1 * icalk_val;
	vcalk = 1000 + 1 * vcalk_val;
	vcalb = (1<<13) * icalk / vcalk;		/*calculation vcalb */
	reg_val = (unsigned int)vcalb;
	i2c_write(0x50, reg_val);			/*update vcalb register*/
}


/*step2 of aw_init():update register sequence*/
void aw_reg_update()
{
	int i = 0;
	uint16_t *data = NULL;
	int data_len;
	unsigned int reg_addr = 0;
	unsigned int reg_val = 0;
	unsigned int read_val;

	data = (uint16_t *)config_register;
	data_len = sizeof(config_register) / sizeof(uint16_t);

	for (i = 0; i < data_len; i += 2) {
		reg_addr = data[i];
		reg_val = data[i + 1];

		/*Operate the specified register according to the timing requirements*/
		if (reg_addr == 0x04) {
			i2c_read((unsigned char)reg_addr, (unsigned int *)&read_val);
			read_val &= (1<<1);
			reg_val &= (~(1<<1));
			reg_val |= read_val;
		}
		if (reg_addr == 0x04) {
			i2c_read((unsigned char)reg_addr, (unsigned int *)&read_val);
			read_val &= (1<<0);
			reg_val &= (~(1<<0));
			reg_val |= read_val;
		}
		if (reg_addr == 0x04) {
			i2c_read((unsigned char)reg_addr, (unsigned int *)&read_val);
			read_val &= (1<<8);
			reg_val &= (~(1<<8));
			reg_val |= read_val;
		}
		if (reg_addr == 0x50) {
			aw_set_vcalb();		/*update vcalb*/
			continue;
		}

		i2c_write(reg_addr, reg_val);	/*update register sequence*/
        osDelay(2);
	}
}


/******* Relevant internal API of aw_pa_start() *******/

/*step3 of aw_pa_start():mode1 check in syspll check*/
int aw_mode1_pll_check()
{
	int ret = AW_FAIL;
	unsigned char i;
	unsigned int reg_val = 0;

	for (i = 0; i < 10; i++) {		/*check 10 times*/
		i2c_read(0x01, &reg_val);
		if (reg_val & (1<<0)) {
			ret = AW_OK;
			break;
		} else {
			osDelay(2);
		}
	}

	return ret;
}


/*step3 of aw_pa_start():mode2 check in syspll check*/
int aw_mode2_pll_check()
{
	int ret = AW_FAIL;
	unsigned int reg_val = 0;

	i2c_read(0x55, &reg_val);
	reg_val &= (1<<2);
	if (reg_val == (0<<2)) {	/*If DIVIDED,check failed*/
		return ret;
	}

	i2c_write_bits(0x55, (~(1<<2)), 0<<2);		/*Change to DIVIDED*/
	ret = aw_mode1_pll_check();
	i2c_write_bits(0x55, (~(1<<2)), 1<<2);		/*Change to BYPASS*/
	if (ret == 0) {
		osDelay(2);
		ret = aw_mode1_pll_check();
	}
	return ret;
}


/*step3 of aw_pa_start():syspll check*/
int aw_syspll_check()
{
	int ret = AW_FAIL;

	ret = aw_mode1_pll_check();
	if (ret < 0) {						/*if mode1 check failed,check mode2*/
		aw_printf("mode1_pll_check failed");
		ret= aw_mode2_pll_check();
		if (ret < 0) {					/*if mode2 check failed,syspll check failed*/
			aw_printf("mode2_pll_check failed");
			return ret;
		}
	}
	return AW_OK;
}


/*step5 of aw_pa_start():sys check*/
int aw_sysst_check()
{
	int ret = AW_FAIL;
	unsigned char i = 0;
	unsigned int reg_val = 0;

	for (i = 0; i < 10; i++) {		/*check 10 times*/
		i2c_read(0x01, &reg_val);
		if (((reg_val & 0x4b3b) & 0x311) == 0x311) {
			ret = AW_OK;		/*check pass*/
			break;
		} else {
			osDelay(2);		/*if check failed,delay 2ms and check again*/
		}
	}
	return ret;
}

/******* Relevant internal API of aw_pa_stop() *******/

void aw_pa_stop()
{
	unsigned int reg_val = 0;

	/*step1: clear inturrupt*/
	i2c_read(0x02, &reg_val);
	i2c_read(0x02, &reg_val);

	/*step2:close inturrupt*/
	i2c_write(0x03, 0xFFFF);

	/*step3:set mute*/
	i2c_write_bits(0x04, (~(1 << 8)), 1<<8);

	osDelay(5);		/*delay 5ms*/

	/*step4:close tx feedback*/
	//i2c_write_bits(0x11, (~(1<<14)), 0<<14);
	i2c_write_bits(0x06, (~(1<<14)), 0<<14);

	osDelay(1);		/*delay 1ms*/

	/*step5:amppd down*/
	i2c_write_bits(0x04, (~(1<<1)), 1<<1);

	/*step6:power down*/
	i2c_write_bits(0x04, (~(1<<0)), 1<<0);

	aw_printf("done");
}


/*PA start API*/
int aw_pa_start()
{
	int ret = AW_FAIL;
	unsigned int reg_val = 0;

	/*step1:disable DSM data dither*/
	i2c_write_bits(0x12, (~(1<<14)), 0<<14);

	/*step2:power on*/
	i2c_write_bits(0x04, (~(1<<0)), 0<<0);

	osDelay(2);		/*delay 2ms*/

	/*step3:syspll check*/
	ret = aw_syspll_check();
	if (ret < 0) {
		/*if syspll check failed,PA power down*/
#ifdef FEATURE_OS_ENABLE
    DEBUG_PRINT(UNILOG_PLA_DRIVER, aw_pa_start1_1, P_DEBUG, "zzg aw_pa_start syspll check failed");
#endif
		/*power down*/
		i2c_write_bits(0x04, (~(1<<0)), 1<<0);
		return ret;
	}

	/*step4:amppd on*/
	i2c_write_bits(0x04, (~(1<<1)), 0<<1);

	osDelay(1);		/*delay 1ms*/

	/*step5:sys check*/
	ret = aw_sysst_check();
	if (ret < 0) {
		/*if sys check failed,PA stop*/
#ifdef FEATURE_OS_ENABLE
    DEBUG_PRINT(UNILOG_PLA_DRIVER, aw_pa_start1_2, P_DEBUG, "zzg aw_pa_start sysst check failed");
#endif
		aw_pa_stop();
		return ret;
	}

	/*step6:enable tx feedback*/
	i2c_write_bits(0x06, (~(1<<14)), 1<<14);

	/*step7:enable DSM data dither*/
	i2c_write_bits(0x12, (~(1<<14)), 1<<14);

	/*step8:disable mute*/
	i2c_write_bits(0x04, (~(1<<8)), 0<<8);

	/*step9: clear inturrupt*/
	i2c_read(0x02, &reg_val);
	i2c_read(0x02, &reg_val);

	/*step10:set inturrupt*/
	i2c_write(0x03, 0xFFFF);

#ifdef FEATURE_OS_ENABLE
        DEBUG_PRINT(UNILOG_PLA_DRIVER, aw_pa_start1_3, P_DEBUG, "zzg aw_pa_start ok");
#endif
	return AW_OK;
}




/*PA initialization API*/
//void aw_init()
HalCodecSts_e aw_init(HalCodecCfg_t *codecCfg)
{
	aw_hw_reset();

	//i2c_write(0x00, 0x55aa);	/*software reset*/
    codecgetchipidaw();

	/*delay 1ms*/
	osDelay(1);

	/*step2:configuration register*/
	aw_reg_update();

	aw_printf("done");
	return CODEC_EOK;
}

HalCodecSts_e aw88266StartStop(HalCodecMode_e mode, HalCodecCtrlState_e ctrlState)
{
    HalCodecSts_e ret = CODEC_EOK;

    if(ctrlState == CODEC_CTRL_START)
    {
        ret = aw_pa_start();
    }
    if(ctrlState == CODEC_CTRL_STOP)
    {
        aw_pa_stop();
    }

    return ret;
}
HalCodecSts_e aw88266Config(HalCodecMode_e mode, HalCodecIface_t *iface)
{
    int ret = CODEC_EOK;
    return ret;
}

HalCodecSts_e aw88266SetMute(HalCodecCfg_t* codecHalCfg, bool enable)
{
    int ret = CODEC_EOK;
    return ret;
}

HalCodecSts_e aw88266SetVolume(HalCodecCfg_t* codecHalCfg, int volume)
{
    int ret = CODEC_EOK;
    return ret;
}

HalCodecSts_e aw88266GetVolume(HalCodecCfg_t* codecHalCfg, int *volume)
{
    int ret = CODEC_EOK;
    return ret;
}

HalCodecSts_e aw88266ConfigFmt(HalCodecIfaceFormat_e fmt)
{
    int ret = CODEC_EOK;
    return ret;
}

HalCodecSts_e aw88266SetMicVolume(HalCodecCfg_t* codecHalCfg, uint8_t micGain, int micVolume)
{
    int ret = CODEC_EOK;
    return ret;
}

// 8311 func list
HalCodecFuncList_t aw88266DefaultHandle =
{
    .codecType                  = AW88266,
    .halCodecInitFunc           = aw_init,
    .halCodecDeinitFunc         = NULL,
    .halCodecCtrlStateFunc      = aw88266StartStop,
    .halCodecCfgIfaceFunc       = aw88266Config,
    .halCodecSetMuteFunc        = aw88266SetMute,
    .halCodecSetVolumeFunc      = aw88266SetVolume,
    .halCodecGetVolumeFunc      = aw88266GetVolume,
    .halCodecSetMicVolumeFunc   = aw88266SetMicVolume,
    .halCodecLock               = NULL,
    .handle                     = NULL,
    .halCodecGetDefaultCfg      = AW88266GetDefaultCfg,
};


HalCodecCfg_t AW88266GetDefaultCfg()
{
    HalCodecCfg_t codecCfg = AW88266_DEFAULT_CONFIG();
    return codecCfg;
}



