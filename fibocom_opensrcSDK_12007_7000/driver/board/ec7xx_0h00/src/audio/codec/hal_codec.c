/****************************************************************************
 *
 * Copy right:   2019-, Copyrigths of EigenComm Ltd.
 * File name:    hal_codec.c
 * Description:  EC7XX codec hal 
 * History:      Rev1.0   2021-9-18
 *
 ****************************************************************************/

#include "hal_codec.h"

/*----------------------------------------------------------------------------*
 *                    MACROS                                                  *
 *----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*
 *                    DATA TYPE DEFINITION                                    *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 *                      PRIVATE FUNCTION DECLEARATION                         *
 *----------------------------------------------------------------------------*/
#ifdef FIBOCOM_BASE
#undef RTE_I2C1
#undef RTE_I2C1_SCL_BIT
#undef RTE_I2C1_SCL_FUNC
#undef RTE_I2C1_SDA_BIT
#undef RTE_I2C1_SDA_FUNC
#define RTE_I2C1                        1
#define RTE_I2C1_SCL_BIT                24
#define RTE_I2C1_SCL_FUNC               PAD_MUX_ALT2
#define RTE_I2C1_SDA_BIT                23
#define RTE_I2C1_SDA_FUNC               PAD_MUX_ALT2
extern ARM_DRIVER_I2C Driver_I2C1;
static ARM_DRIVER_I2C   *i2cDrvInstance = &CREATE_SYMBOL(Driver_I2C, 1);
static bool i2c1_openstate = false;
#else
extern ARM_DRIVER_I2C Driver_I2C0;
static ARM_DRIVER_I2C   *i2cDrvInstance = &CREATE_SYMBOL(Driver_I2C, 0);
#endif

/*----------------------------------------------------------------------------*
 *                      GLOBAL VARIABLES                                      *
 *----------------------------------------------------------------------------*/
const AudioParaCfgCodec_t codecTlvDefaultVal = 
{
    .isDmic         = false,                
    .isExPa         = true,              
    .exPaGain       = 0, 
    .txDigGain      = 0xbf,     
    .txAnaGain      = 0x8, 

    .rxDigGain0     = 0,
    .rxAnaGain0     = 0,
    .rxDigGain50    = 0,
    .rxAnaGain50    = 0,
    .rxDigGain100   = 0xff,
    .rxAnaGain100   = 0xff,
};

/*----------------------------------------------------------------------------*
 *                      PRIVATE FUNCTIONS                                     *
 *----------------------------------------------------------------------------*/

#ifdef FEATURE_OS_ENABLE

HalCodecSts_e halCodecLock(void* arg)
{
    osSemaphoreId_t codecLock = (osSemaphoreId_t)arg;

    if (osSemaphoreAcquire(codecLock, 1000) != osOK)
    {
        return CODEC_TIMEOUT;
    }
    return  CODEC_EOK;
}

HalCodecSts_e halCodecUnlock(void* arg)
{
    osSemaphoreId_t codecLock = (osSemaphoreId_t)arg;

    if (osSemaphoreRelease(codecLock) != osOK)
    {
        return CODEC_TIMEOUT;
    }
    return  CODEC_EOK;
}

HalCodecSts_e halCodecLockDestroy(void *arg)
{
    osSemaphoreId_t codecLock = (osSemaphoreId_t)arg;

    if (osSemaphoreDelete(codecLock) != osOK)
    {
        return CODEC_TIMEOUT;
    }
    return  CODEC_EOK;
}

#else

HalCodecSts_e halCodecLock(void* arg)
{
    return  CODEC_EOK;
}

HalCodecSts_e halCodecUnlock(void* arg)
{
    return  CODEC_EOK;
}

HalCodecSts_e halCodecLockDestroy(void *arg)
{
    return  CODEC_EOK;
}

#endif


void codecI2cInit()
{
#ifdef FIBOCOM_BASE
    PadConfig_t padConfig;
    PAD_getDefaultConfig(&padConfig);
    padConfig.mux = RTE_I2C1_SCL_FUNC;
    PAD_setPinConfig(RTE_I2C1_SCL_BIT, &padConfig);
    padConfig.mux = RTE_I2C1_SDA_FUNC;
    PAD_setPinConfig(RTE_I2C1_SDA_BIT, &padConfig);
    if(i2c1_openstate)
    {
        return;
    }
    else
    {
        i2c1_openstate = true;
    }
    i2cDrvInstance->SetMode(POLLING_MODE);
#endif

    i2cDrvInstance->Initialize(NULL);
    i2cDrvInstance->PowerControl(ARM_POWER_FULL);
    i2cDrvInstance->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD);
    i2cDrvInstance->Control(ARM_I2C_BUS_CLEAR, 0);
}

#ifdef FIBOCOM_BASE
//Indicates whether codec is occupying i2c
bool codecI2cOpenState(void)
{
    return i2c1_openstate;
}
#endif

void codecI2cDeInit()
{
    i2cDrvInstance->Uninitialize();
#ifdef FIBOCOM_BASE
    //2024-07-04, wangzhen, MTC0729-395.At the same time two groups GPIO cannot be multiplex as i2c.
    PadConfig_t padConfig;
    PAD_getDefaultConfig(&padConfig);
    padConfig.mux = PAD_MUX_ALT0;
    PAD_setPinConfig(RTE_I2C1_SCL_BIT, &padConfig);
    padConfig.mux = PAD_MUX_ALT0;
    PAD_setPinConfig(RTE_I2C1_SDA_BIT, &padConfig);
    //2024-07-04, wangzhen, MTC0729-395.End.
    i2c1_openstate = false;
#endif
}

int32_t codecI2cWrite(uint8_t slaveAddr, uint8_t regAddr, uint16_t regVal)
{
    ARM_I2C_STATUS status = {0};
    
    uint8_t  cmd[2] = {0, 0};
    cmd[0]  = regAddr;
    cmd[1]  = regVal & 0xff;

    i2cDrvInstance->MasterTransmit(slaveAddr, cmd, 2, false);
    status = i2cDrvInstance->GetStatus();
    return status.rx_nack;
}

uint8_t codecI2cRead(uint8_t slaveAddr, uint8_t regAddr)
{
    uint8_t a = regAddr, readData = 0;
    i2cDrvInstance->MasterTransmit(slaveAddr, (uint8_t *)&a, 1, true);   
    i2cDrvInstance->MasterReceive(slaveAddr, &readData, 1, false);
    return readData;
}

ARM_I2C_STATUS codecI2cGetStats()
{
    return i2cDrvInstance->GetStatus();

}

#ifdef FIBOCOM_BASE
int32_t codecgetchipid(void)
{
    int32_t chipid = 0;
    uint8_t  cmd[2] = {0, 0};
    codecI2cInit();
    osDelay(100);
    cmd[0] = codecI2cRead(ES8311_IICADDR, 0XFD);
    osDelay(50);
    cmd[1] = codecI2cRead(ES8311_IICADDR, 0XFE);
    osDelay(50);

    chipid = (cmd[0]<<8) | cmd[1];
    if(chipid == 0)
    {
        codecI2cDeInit();
        osDelay(10);
    }
    return chipid;
}
#endif


/*----------------------------------------------------------------------------*
 *                      GLOBAL FUNCTIONS                                      *
 *----------------------------------------------------------------------------*/
 
HalCodecCfg_t halCodecGetDefaultCfg(HalCodecFuncList_t* codecHalFunc)
{
    return codecHalFunc->halCodecGetDefaultCfg();
}

HalCodecFuncList_t* halCodecInit(HalCodecCfg_t* codecHalCfg, HalCodecFuncList_t* codecHalFunc, bool needLock)
{
    HalCodecSts_e ret = CODEC_EOK;
    HalCodecFuncList_t* codecHal = codecHalFunc;
    
#ifdef FEATURE_OS_ENABLE
    if (codecHal->halCodecLock == NULL)
    {
        codecHal->halCodecLock = osSemaphoreNew(1, 1, NULL);
        if (codecHal->halCodecLock == PNULL)
        {
            DEBUG_PRINT(UNILOG_PLA_DRIVER, halCodecInit_0, P_WARNING, "codec hal can't create semaphore");
            return NULL;
        }
    }
#endif

    if (needLock) halCodecLock(codecHal->halCodecLock);

    ret |= codecHal->halCodecInitFunc(codecHalCfg);
    if (ret != CODEC_EOK)
    {        
        halCodecUnlock(codecHal->halCodecLock);
        DEBUG_PRINT(UNILOG_PLA_DRIVER, halCodecInit_2, P_DEBUG, "codec init fail. ret=%d", ret);
        EC_ASSERT(false, 0, 0, 0);
        return NULL;
    }

    codecHal->halCodecCfgIfaceFunc(codecHalCfg->codecMode, &codecHalCfg->codecIface);
    
    codecHal->handle = codecHal;
    codecHalFunc->handle = codecHal;
    if (needLock) halCodecUnlock(codecHal->halCodecLock);
    return codecHal;
}

HalCodecSts_e halCodecDeinit(HalCodecFuncList_t* codecHal)
{
    int32_t ret = CODEC_EOK;
    
    HAL_CODEC_CHECK_NULL(codecHal, "audio_hal handle is null", -1);
    halCodecLockDestroy(codecHal->halCodecLock);
    codecHal->halCodecDeinitFunc();
    codecHal->halCodecLock = NULL;
    codecHal->handle = NULL;
    codecHal = NULL;
    return ret;
}

HalCodecSts_e halCodecCtrlState(HalCodecFuncList_t* codecHal, HalCodecMode_e mode, HalCodecCtrlState_e codecStartStop, bool needLock)
{
    int32_t ret;
    HAL_CODEC_CHECK_NULL(codecHal, "audio_hal handle is null", -1);
    if (needLock) halCodecLock(codecHal->halCodecLock);
    DEBUG_PRINT(UNILOG_PLA_DRIVER, halCodecCtrl_1, P_DEBUG, "codec mode=%d, startStop=%d", mode, codecStartStop);
    ret = codecHal->halCodecCtrlStateFunc(mode, codecStartStop);
    if (needLock) halCodecUnlock(codecHal->halCodecLock);
    return ret;
}

HalCodecSts_e halCodecIfaceCfg(HalCodecFuncList_t* codecHal, HalCodecMode_e mode, HalCodecIface_t *iface, bool needLock)
{
    int32_t ret = CODEC_EOK;
    HAL_CODEC_CHECK_NULL(codecHal, "audio_hal handle is null", -1);
    HAL_CODEC_CHECK_NULL(iface, "Get volume para is null", -1); 
    if (needLock) halCodecLock(codecHal->halCodecLock);
    ret = codecHal->halCodecCfgIfaceFunc(mode, iface);
    if (needLock) halCodecUnlock(codecHal->halCodecLock);

    return ret;
}

HalCodecSts_e halCodecSetMute(HalCodecFuncList_t* codecHal, HalCodecCfg_t* codecHalCfg, bool mute, bool needLock)
{
    int32_t ret;
    HAL_CODEC_CHECK_NULL(codecHal, "audio_hal handle is null", -1);
    if (needLock) halCodecLock(codecHal->halCodecLock);
    ret = codecHal->halCodecSetMuteFunc(codecHalCfg, mute);
    if (needLock) halCodecUnlock(codecHal->halCodecLock);
    return ret;
}

#if 0
HalCodecSts_e halCodecEnablePA(HalCodecFuncList_t* codecHal, bool enable, bool needLock)
{
    int32_t ret = CODEC_EOK;
    HAL_CODEC_CHECK_NULL(codecHal, "audio_hal handle is null", -1);
    if (needLock) halCodecLock(codecHal->halCodecLock);
        codecHal->halCodecEnablePAFunc(enable);
    if (needLock) halCodecUnlock(codecHal->halCodecLock);
    return ret;
}
#endif

HalCodecSts_e halCodecSetVolume(HalCodecFuncList_t* codecHal, HalCodecCfg_t* codecHalCfg, int volume, bool needLock)
{
    int32_t ret;
    HAL_CODEC_CHECK_NULL(codecHal, "audio_hal handle is null", -1);
    if (needLock) halCodecLock(codecHal->halCodecLock);
    ret = codecHal->halCodecSetVolumeFunc(codecHalCfg, volume);
    if (needLock) halCodecUnlock(codecHal->halCodecLock);
    return ret;
}

HalCodecSts_e halCodecGetVolume(HalCodecFuncList_t* codecHal, HalCodecCfg_t* codecHalCfg, int *volume, bool needLock)
{
    int32_t ret;
    HAL_CODEC_CHECK_NULL(codecHal, "audio_hal handle is null", -1);
    HAL_CODEC_CHECK_NULL(volume, "Get volume para is null", -1);
    if (needLock) halCodecLock(codecHal->halCodecLock);
    ret = codecHal->halCodecGetVolumeFunc(codecHalCfg, volume);
    if (needLock) halCodecUnlock(codecHal->halCodecLock);
    return ret;
}

HalCodecSts_e halCodecSetMicVolume(HalCodecFuncList_t* codecHal, HalCodecCfg_t* codecHalCfg, uint8_t micGain, int volume, bool needLock)
{
    int32_t ret;
    HAL_CODEC_CHECK_NULL(codecHal, "audio_hal handle is null", -1);
    if (needLock) halCodecLock(codecHal->halCodecLock);
    ret = codecHal->halCodecSetMicVolumeFunc(codecHalCfg, micGain, volume);
    if (needLock) halCodecUnlock(codecHal->halCodecLock);
    return ret;
}


