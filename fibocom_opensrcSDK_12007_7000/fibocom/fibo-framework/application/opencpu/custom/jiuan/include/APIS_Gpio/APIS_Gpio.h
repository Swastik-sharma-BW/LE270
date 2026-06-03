/**
 * @Description  : gpio外部接口
 * @Version      : 1.0
 * @Author       : ZhongXiaoHui
 * @Date         : 2021-10-23 17:59:56
 * @copyright    : Copyright (C) JUAN. All rights reserved.
 **/

#if !defined(APIS_GPIO_H_)
#define APIS_GPIO_H_
NK_CPP_EXTERN_BEGIN

#include "nk_lpcm.h"

#define NK_INVALID_GPIO_PIN (-1)

typedef enum{
    NK_GPIO_LEVEL_LOW,
    NK_GPIO_LEVEL_HIGH
}NK_GPIO_LV_E;

typedef enum{
    NK_GPIO_NAME_UART_TO_SOC_RX,        //4G和soc通信串口, 4G rx(部分平台是中断唤醒脚,即使uart未open也会触发)
    NK_GPIO_NAME_UART_TO_BLE_RX,        //4G和蓝牙通信串口, 4G rx(中断唤醒脚)
    NK_GPIO_NAME_WAKE_IN,               //常用于soc唤醒4G(中断唤醒脚)
    NK_GPIO_NAME_USB_VBUS_CTL,          //usb vbus电源使能脚,部分平台要求断开后才能休眠
    NK_GPIO_NAME_SIM0_DET,              //SIM卡槽0的det脚(插拔支持中断唤醒,以便执行自动切卡的逻辑)(LE平台该引脚不具备gpio功能，仅支持读取电平值不支持设置)

    NK_GPIO_NAME_END
}NK_GPIO_NAME_E;

typedef enum{
    NK_GPIO_FUNC_GPIO,
    NK_GPIO_FUNC_UART_RX,

    NK_GPIO_FUNC_END
}NK_GPIO_FUNC_E;

typedef enum{
    NK_GPIO_DIR_OUTPUT,
    NK_GPIO_DIR_INPUT
}NK_GPIO_DIR_E;


struct APIS_GPIO_S{
    /**
     * @brief 初始化指定的gpio,主要提供给mcu库调用
     * @param pIoAttr [in] 指定要初始化的io属性 @ref pNK_LPCM_IO_INIT_ATTR_S
     * @return NK_FALSE 失败
     * @return NK_TRUE 成功
    */
    NK_Int (*init)(pNK_LPCM_IO_INIT_ATTR_S pIoAttr);

    /**
     * @brief 反初始化某个gpio,主要提供给mcu库调用
     * @return NK_FALSE 失败
     * @return NK_TRUE 成功
    */
    NK_Int (*deinit)(NK_Void);

    /**
     * @brief 根据板型对4G wakeupSoc、soc wakeup4G 等gpio初始化
     * @param pIoAttr [in] 指定要初始化的io属性 @ref pNK_LPCM_IO_INIT_ATTR_S
     * @return NK_FALSE 失败
     * @return NK_TRUE 成功
    */
    NK_Int (*initV2)();

    /**
     * @brief 初始化指定的gpio为中断方式,主要提供给mcu库调用
     * @param pIoAttr [in] 指定要初始化的io属性 @ref pNK_LPCM_EXTI_INIT_ATTR_S
     * @return NK_FALSE 失败
     * @return NK_TRUE 成功
    */
    NK_Boolean (*setExtIrq)(pNK_LPCM_EXTI_INIT_ATTR_S pExtAttr);

    /**
     * @brief 读取指定的gpio值,主要提供给mcu库调用
     * @param enIoType [in] 指定要读取的io @ref NK_LPCM_IO_E
     * @param nValue [out] 读取的值
     * @return NK_FALSE 失败
     * @return NK_TRUE 成功
    */
    NK_Int (*read)(NK_LPCM_IO_E enIoType, NK_Int *nValue);

    /**
     * @brief 往指定的gpio写值,主要提供给mcu库调用
     * @param enIoType [in] 指定要写入的io @ref NK_LPCM_IO_E
     * @param nValue [out] 待写入的值
     * @return NK_FALSE 失败
     * @return NK_TRUE 成功
    */
    NK_Int (*write)(NK_LPCM_IO_E enIoType, NK_Int nValue);

    /**
     * @brief linux平台才需对接此接口,主要用于pir驱动时序、以及读参数的初始化,电平拉高拉低还在用户态操作
     */
    NK_Void (*pirDrv)(NK_Void *arg1, NK_Void *arg2);

    /**
     * @brief 设置vbus ctl脚电平,休眠时断开,唤醒时打开,降低功耗
     * @param nValue 0代表拉低, 非0代表拉高
     * @return 成功返回NK_TRUE, 失败返回NK_FALSE
     */
    NK_Int (*setUsbVbusCtl)(NK_Int nValue);
    /**
     * @brief 复位蓝牙,一般在初始化时 和 蓝牙通信异常时调用
     * @return 成功返回NK_TRUE, 失败返回NK_FALSE
     */
    NK_Int (*resetBlePower)();
    /**
     * @brief 设置管脚复用
     * @param pin_name[in] 指定要复用的管脚名 @ref NK_GPIO_NAME_E
     * @param func_sel[in] 指定要复用的功能 @ref NK_GPIO_FUNC_E
     * @return 成功返回NK_TRUE, 失败返回NK_FALSE
     */
    NK_Int (*setPinFunc)(NK_GPIO_NAME_E pin_name, NK_GPIO_FUNC_E func_sel);
    /**
     * @brief 设置管脚方向(仅当管脚复用为gpio后才有效)
     * @param pin_name[in] 指定要设置的管脚名 @ref NK_GPIO_NAME_E
     * @param dir[in] 指定要设置的方向 @ref NK_GPIO_DIR_E
     * @return 成功返回NK_TRUE, 失败返回NK_FALSE
     */
    NK_Int (*setPinDir)(NK_GPIO_NAME_E pin_name, NK_GPIO_DIR_E dir);

    /**
     * @brief 设置管脚电平(设置之前确保改管脚一定已被复用为gpio)
     * @param pin_name  [in] 指定要设置的管脚名 @ref NK_GPIO_NAME_E
     * @param lv        [in] 指定要设置的电平 @ref NK_GPIO_LV_E
     * @return 成功返回NK_TRUE, 失败返回NK_FALSE
     */
    NK_Int (*setLevel)(NK_GPIO_NAME_E pin_name, NK_GPIO_LV_E lv);

    /**
     * @brief 获取管脚电平
     * @param pin_name  [in] 指定要读取的管脚名 @ref NK_GPIO_NAME_E
     * @param pLv       [out] 返回管脚的电平 @ref NK_GPIO_LV_E
     * @return 成功返回NK_TRUE, 失败返回NK_FALSE
     */
    NK_Int (*getLevel)(NK_GPIO_NAME_E pin_name, NK_GPIO_LV_E *pLv);
};


NK_API struct APIS_GPIO_S *APIS_Gpio(void);


NK_CPP_EXTERN_END
#endif
