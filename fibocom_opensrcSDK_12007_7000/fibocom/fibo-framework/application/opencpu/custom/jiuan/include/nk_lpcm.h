#pragma once

#include "nk_types.h"

typedef enum
{
    WORK_STA_FIRST_POWERON,
    WORK_STA_SOC_POWER_ON,
    WORK_STA_WAITSYNC,
    WORK_STA_RUN,
    WORK_STA_UPGRADE,
    WORK_STA_SLEEP,
    WORK_STA_LOWPOWER,
    WORK_STA_POWEROFF,
    WORK_STA_BUTT,
}NK_WORK_STA_E;

/* GPIO 枚举，每个枚举对应一个实际的IO */
//typedef enum __IO
typedef enum 
{
    /* LED指示灯对应的IO枚举 */
    NK_LPCM_IO_LED1,
    NK_LPCM_IO_LED2,
    NK_LPCM_IO_LED3,
    NK_LPCM_IO_LED4,
    
    /* 按键检测对应的IO枚举 */
    NK_LPCM_IO_KEY1,
    NK_LPCM_IO_KEY2,

    /* PIR驱动管脚枚举 */
    NK_LPCM_IO_PIR_IN, 
    NK_LPCM_IO_PIR_OUT, 

    /* ADC电量计电压采集管脚枚举 */
    NK_LPCM_IO_BAT_ADC, // 模拟信号输入

    /* 数字电量计驱动管脚枚举 */
    NK_LPCM_IO_IIC_SCL, // 输出
    NK_LPCM_IO_IIC_SDA, // 输入或输出

    /* 充电状态检测管脚枚举 */
    NK_LPCM_IO_USB_CH_STA_TAMPER_DET, // 输入，usb充电状态或破坏报警检测
    NK_LPCM_IO_SOLAR_CH_STA, // 输入
    NK_LPCM_IO_USB_DET, // 输入

    /* SOC电源控制管脚枚举 */
    NK_LPCM_IO_SOC_POWER, // 输出
    NK_LPCM_IO_PIR1_IN,  //第二路pir
    NK_LPCM_IO_PIR1_OUT,

    NK_LPCM_IO_BT_PIR_IN,
    NK_LPCM_IO_BT_PIR_OUT,
    
    NK_LPCM_IO_BLE_CTRL_OUT,
    NK_LPCM_IO_VBUS_CTRL_OUT,
    NK_LPCM_IO_BUTT
}NK_LPCM_IO_E;

/* GPIO 上下拉模式 */
typedef enum __IO_PUPD
{
    NK_LPCM_IO_PULL_DOWN, // 下拉
    NK_LPCM_IO_PULL_UP, // 上拉
    NK_LPCM_IO_PULL_NO, // 无上下拉
    NK_LPCM_IO_PUPD_BUTT
}NK_LPCM_IO_PUPD_E;

/* GPIO 模式 */
typedef enum __IO_MODE
{
    NK_LPCM_IO_OUTPUT_MODE, // 输出模式
    NK_LPCM_IO_INPUT_MODE, // 输入模式
    NK_LPCM_IO_HZ, // 高阻态
    NK_LPCM_IO_ALTERNATE_MODE, // 复用功能模式(该模式下可以配置IO为ADC/DAC等等功能)
    NK_LPCM_IO_MODE_BUTT
}NK_LPCM_IO_MODE_E;

/* GPIO 复用功能 */
typedef enum __IO_ALTERNATE_FUN
{
    NK_LPCM_IO_AF_ADC,
    NK_LPCM_IO_AF_BUTT
}NK_LPCM_IO_AF_E;

/* GPIO 输出模式 */
typedef enum __IO_OUTPUT_TYPE
{
    NK_LPCM_IO_OUTPUT_PUSHPULL, // 推挽输出(输出能力强，CMOS)
    NK_LPCM_IO_OUTPUT_OPENDRAIN,// 开漏输出(MOS管的漏极开路，没有高电平输出能力，需要外部接上拉)
    NK_LPCM_IO_OUTPUT_BUTT
}NK_LPCM_IO_OUTPUT_T_E;

/**
 * @brief
 *  GPIO 初始化属性。
 */
typedef struct __IO_Init_Attr
{
    NK_LPCM_IO_E io;                 /* GPIO 枚举，每个枚举对应一个实际的IO */
    NK_LPCM_IO_PUPD_E pull;			 /* GPIO 上下拉模式 */
    NK_LPCM_IO_MODE_E mode;			 /* GPIO 模式 */
    NK_LPCM_IO_AF_E altFun;			 /* GPIO 复用功能 */
    NK_LPCM_IO_OUTPUT_T_E outputType;/* GPIO 输出模式 */
}NK_LPCM_IO_INIT_ATTR_S,*pNK_LPCM_IO_INIT_ATTR_S;

/**
 * @brief
 *  IO初始化。
 * @param initAttr [in]
 *  GPIO 初始化属性。
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_IO_INIT_F)(pNK_LPCM_IO_INIT_ATTR_S initAttr);

/**
 * @brief
 *  设置IO电平。
 * @param io [in]
 *  GPIO 枚举，每个枚举对应一个实际的IO。
 * @param stat [in]
 *  IO电平状态。
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_GPIO_WRITE_F)(NK_LPCM_IO_E io, NK_Boolean stat);

/**
 * @brief
 *  读取IO电平
 * @param io [in]
 *  GPIO 枚举，每个枚举对应一个实际的IO。
 * @return
 *  发送成功返回IO电平。
 */
typedef NK_Boolean (*NK_LPCM_GPIO_READ_F)(NK_LPCM_IO_E io);



/* 外部中断 */
typedef enum __EXTI_TRIGGER_MODE
{
    NK_LPCM_TRI_RISING, // 上升沿触发
    NK_LPCM_TRI_FALLING, // 下降沿触发
    NK_LPCM_TRI_RISING_FALLING, // 上升和下降沿都触发
    NK_LPCM_TRI_BUTT
}NK_LPCM_EXTI_TRI_MODE_E;

/* 外部中断初始化 */
typedef struct __EXTI_INIT_ATTR
{
    NK_LPCM_IO_E io; //GPIO 枚举，每个枚举对应一个实际的IO
    NK_LPCM_EXTI_TRI_MODE_E triMode; // 中断触发方式
    NK_Boolean enabled; // 是否开启中断
}NK_LPCM_EXTI_INIT_ATTR_S,*pNK_LPCM_EXTI_INIT_ATTR_S;

/**
 * @brief
 *  外部中断初始化
 * @param initAttr [in]
 *  外部中断初始化属性。
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_EXTI_INIT_F)(pNK_LPCM_EXTI_INIT_ATTR_S initAttr);


/**
 * @brief
 *  当产生外部中断时调用该函数
 * @param io [in]
 *  IO。
 * @return
 *  发送成功返回 True。
 */
NK_Boolean NK_LPCM_EXTI_IrqHdl(NK_LPCM_IO_E io);



/* 计时器 */

/**
 * @brief
 *  当产生RTC中断时调用该函数，RTC中断为20S一次
 * @return
 *  发送成功返回 True。
 */
NK_Boolean NK_LPCM_RTC_IrqHdl(NK_Void);

/**
 * @brief
 *  获取系统时间搓，单位为Ms
 * @return
 *  发送成功返回 系统时间搓。
 */
typedef NK_UInt64 (*NK_LPCM_GET_TICK_MS_F)(NK_Void);

/**
 * @brief
 *  获取RTC时间戳，单位为s。
 * @param pRtcTime[out]
 *  RTC时间戳。
 * @return
 *  发送成功返回 True，失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_GET_RTC_TIME_F)(NK_UInt64 *pRtcTime);

/**
 * @brief
 *  延时函数，该函数必须是阻塞的不可被打断的。
 * @param us [in]
 *  延时时间，单位为us。
 */
typedef NK_Void (*NK_LPCM_DELAY_US_F)(NK_UInt32 us);

/**
 * @brief
 *  延时函数，该函数需让出cpu的使用权，相当于sleep函数的功能。
 * @param ms [in]
 *  延时时间，单位为ms。
 */
typedef NK_Void (*NK_LPCM_DELAY_MS_F)(NK_UInt32 ms);


/* 模数转换 */

/**
 * @brief
 *  读取指定IO的模拟电压值，单位为mv（如果该IO被配置为ADC模式）
 * @param io [in]
 *  GPIO 枚举，每个枚举对应一个实际的IO。
 * @param volt [out]
 *  IO的模拟电压值。
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_ADC_READ_F)(NK_LPCM_IO_E io, NK_UInt16 *volt);

/**
 * @brief
 *  adc使能控制
 * @param en [in]
 *  使能。
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_ADC_EN_F)(NK_Boolean en);


/**
 * @brief
 *  获取当前LPCM库的版本号
 * @return
 *  发送成功返回当前LPCM库的版本号。
 */
NK_Char *NK_LPCM_GetLibVer(NK_Void);

/**
 * @brief
 *  获取当前处理器的型号
 * @param pModelStr [out]
 *  当前处理器的型号。
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_GET_PRCSR_MDL_F)(NK_Char **pModelStr);


/*系统参数,获取和保存配置参数的接口 */
/**
 * @brief
 *  保存配置参数
 * @param pData [in]
 *  配置参数
 * @param len [in]
 *  配置参数长度
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_PARAM_WRITE_F)(NK_Void *pData, NK_UInt32 len);

/**
 * @brief
 *  获取配置参数
 * @param pData [out]
 *  配置参数
 * @param len [in]
 *  配置参数长度
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_PARAM_READ_F)(NK_Void *pData, NK_UInt32 len);

/**
 * @brief
 *  保存配置参数
 * @param pData [in]
 *  配置参数
 * @param len [in]
 *  配置参数长度
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_PARAM_WRITE_V2_F)(NK_Void *pData, NK_UInt32 len);

/**
 * @brief
 *  获取配置参数
 * @param pData [out]
 *  配置参数
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_PARAM_READ_V2_F)(NK_Void *pData);

/**
 * @brief
 *  获取配置参数的长度
 * @return
 *  配置参数的长度
 */
typedef NK_Int32 (*NK_LPCM_PARAM_GET_LEN_F)(NK_Void);


/*重置原因*/
typedef enum __RESET_REASON
{
    NK_LPCM_RESET_REASON_KEY_FORCE = 0,  //按键强制
    NK_LPCM_RESET_REASON_RECV_SOC_CMD,   //收到SOC命令
    NK_LPCM_RESET_REASON_WAITSYNC_TIMEOUT,//等待系统超时
    NK_LPCM_RESET_REASON_EXCEPTION_TIMEOUT, //
    NK_LPCM_RESET_REASON_UPGRADE_TIMEOUT,  //升级超时
    NK_LPCM_RESET_REASON_HEARTBEAT_TIMEOUT = 5,//心跳超时
    NK_LPCM_RESET_REASON_CLR_PIR_INT_FAILED,//清除PIR初始化失败
    NK_LPCM_RESET_REASON_BUTT
}NK_LPCM_RESET_REASON_E;

/* 软件重启 */

/**
 * @brief
 *  重启处理器
 * @param reason[in]
 *  重置原因
 */
typedef NK_Void (*NK_LPCM_SFTRST_F)(NK_LPCM_RESET_REASON_E reason);


/**
 * @brief
 *  往SOC发送业务逻辑通信数据
 * @param pData[in]
 *  通信数据
 * @param len[in]
 *  数据长度
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_DEVMSG_SND_F)(NK_Void *pData, NK_UInt32 len);

/**
 * @brief
 *  
 * @param pData[]
 *  通信数据
 * @param len[]
 *  数据长度
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
NK_Boolean NK_LPCM_DEVMSG_Recv(NK_Void *pData, NK_UInt32 len);

/**
 * @brief
 *  休眠后会调用该接口去初始化通信接口再给SOC下电
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_COM_INIT_F)(NK_Void);

/**
 * @brief
 *  给SOC上电后会调用该接口初始化通信接口
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_COM_DEINIT_F)(NK_Void);

/**
 * @brief
 *  通信模块重启接口
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_COM_RESET_F)(NK_Void *arg);


/* 工作模式 */
typedef enum __PM_MODE
{
    NK_LPCM_PM_MODE_RUN,// 正常运行模式
    NK_LPCM_PM_MODE_SLEEP,// 休眠低功耗模式
    NK_LPCM_PM_MODE_POWER_OFF,// 关机模式
    NK_LPCM_PM_MODE_BUTT
}NK_LPCM_PM_MODE_E;

/* 电源管理 */

/**
 * @brief
 *  设置处理器的工作模式
 * @param mode[in]
 *  工作模式
 * @param arg[]
 *  
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_PM_SET_MODE_F)(NK_LPCM_PM_MODE_E mode, NK_Void *arg);

/**
 * @brief
 *  恢复出厂设置
 * @param arg[]
 *  
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_FACTORY_RESET_F)(NK_Void *arg);


/* 休眠唤醒原因 */
typedef enum __WAKE_UP_REASON
{
    NK_LPCM_WAKE_UP_REASON_LAN,            // 局域网UDP唤醒
    NK_LPCM_WAKE_UP_REASON_REMOTE,         // 远程TCP唤醒
    NK_LPCM_WAKE_UP_REASON_KEEPALIVE_FAIL, // P2P保活失败
    NK_LPCM_WAKE_UP_REASON_NO_P2P_PARAM,   // 休眠时没有P2P保活参数
    NK_LPCM_WAKE_UP_REASON_WIFI_CONNECTED, // wifi已连接
    NK_LPCM_WAKE_UP_REASON_WIFI_DISCONNECTED, // wifi已断开
    NK_LPCM_WAKE_UP_REASON_RESET_FACTORY,  // 唤醒soc复位
    NK_LPCM_WAKE_UP_REASON_BUTT
}NK_LPCM_WAKE_UP_REASON_E;

/**
 * @brief
 *  休眠后可通过该接口唤醒LPCM库
 * @param reason[in]
 *  休眠唤醒原因
 */
NK_Void NK_LPCM_WakeUp(NK_LPCM_WAKE_UP_REASON_E reason);

/**
 * @brief
 *  线程挂起接口，设置SLEEP或者POWER_OFF工作模式后会调用该接口挂起LPCM库的运行线程。
 * @param arg[]
 *  
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_THREAD_SUSPEND_F)(NK_Void *arg);

/**
 * @brief
 *  线程恢复接口，产生中断或者调用NK_LPCM_WakeUp接口都会调用该接口来恢复LPCM库线程的运行。
 * @param arg[]
 *  
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_THREAD_RESUME_F)(NK_Void *arg);

/**
 * @brief
 *  LPCM库主任务调用接口，创建一个线程一直调用即刻，不需要对该任务进行休眠操作，LPCM库内部会自行调用fnDelayMs接口进行休眠。
 * @param arg[]
 *  
 */
NK_Void *NK_LPCM_CircalCall(NK_Void *arg);


/**
 * @brief
 *  动态分配内存空间
 * @param sz[in]
 *  开辟空间的大小
 */
typedef NK_Void *(*NK_LPCM_MALLOC_F)(NK_UInt32 sz);

/**
 * @brief
 *  动态释放内存空间
 * @param pMem[in]
 *  释放空间的大小
 */
typedef NK_Void (*NK_LPCM_FREE_F)(NK_Void *pMem);

/**
 * @brief
 *  申请互斥锁
 */
typedef void *NK_LPCM_MUTEX_T;

/**
 * @brief
 * 互斥锁初始化 
 * @param pMutex[out]
 *  互斥锁
 * @return
 *  
 */
typedef NK_Int32 (*NK_LPCM_MUTEX_INIT_F)(NK_LPCM_MUTEX_T *pMutex);

/**
 * @brief
 *  互斥锁去初始化
 * @param mutex[in]
 *  互斥锁
 * @return
 * 
 */
typedef NK_Int32 (*NK_LPCM_MUTEX_DEINIT_F)(NK_LPCM_MUTEX_T mutex);

/**
 * @brief
 *  互斥锁上锁
 * @param mutex[in]
 *  互斥锁
 * @return
 * 
 */
typedef NK_Int32 (*NK_LPCM_MUTEX_LOCK_F)(NK_LPCM_MUTEX_T mutex);

/**
 * @brief
 *  互斥锁解锁
 * @param mutex[in]
 *  互斥锁
 * @return
 * 
 */
typedef NK_Int32 (*NK_LPCM_MUTEX_UNLOCK_F)(NK_LPCM_MUTEX_T mutex);

/**
 * @brief
 *  全局中断关闭接口
 * @return
 * 
 */
typedef NK_UInt32 (*NK_LPCM_IRQ_DISABLE_F)(NK_Void);

/**
 * @brief
 *  全局中断开启接口
 * @param flag[in]
 *  
 */
typedef NK_Void (*NK_LPCM_IRQ_RESTORE_F)(NK_UInt32 flag);

/*电池电压和按键状态*/

typedef enum __KEY_E
{
    NK_LPCM_KEY_1,  // 按键枚举
    NK_LPCM_KEY_2,
    NK_LPCM_KEY_3,
    NK_LPCM_KEY_4GWKP, // 旧版本4G分体板使用按键触发模拟远程唤醒的信号通知LPCM库
    NK_LPCM_KEY_BUTT,
}NK_LPCM_KEY_E;

/**
 * @brief
 *  获取电池电压，单位mv
 * @param pVolt[in]
 *  电池电压
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_GET_BAT_VOLT_F)(NK_UInt16 *pVolt);

/**
 * @brief
 *  按键初始化
 * @param availableKeyNb[in]
 *  可供应按键号
 * @return
 *  发送成功返回 True，发送失败返回 False。
 */
typedef NK_Boolean (*NK_LPCM_KEY_INIT_F)(NK_Int32 availableKeyNb);

/**
 * @brief
 *  获取按键状态
 * @param key[in]
 *  按键
 * @return
 *  返回值为true：按键被按下，false：按键被释放
 */
typedef NK_Boolean (*NK_LPCM_GET_KEY_STA_F)(NK_LPCM_KEY_E key); 
/**
 * @brief
 *  
 * @param arg1[]
 * 
 * @param arg2[]
 * 
 */
typedef NK_Void (*NK_LPCM_DRV_PIR_F)(NK_Void *arg1, NK_Void *arg2);
typedef NK_Boolean (*NK_LPCM_SET_BLE_ENABLE_F)(NK_UInt8 bEn);


/* 上电类型 */
typedef enum __POWER_UP_TYPE
{
    NK_LPCM_POWER_UP_TYPE_UNKNOWN,  // 未知类型
    NK_LPCM_POWER_UP_TYPE_FIRST,    // 首次上电
    NK_LPCM_POWER_UP_TYPE_UPGRADE,  // 升级后上电
    NK_LPCM_POWER_UP_TYPE_BUTT
}NK_LPCM_POWER_UP_TYPE_E;

/* 网络状态 */
typedef enum __NET_STATUS
{
    NK_LPCM_NET_STATUS_CONNNECTED,    // 网络已连接
    NK_LPCM_NET_STATUS_DISCONNECTED,  // 网络已断开
    NK_LPCM_NET_STATUS_WAIT_WIFIOFF_LINKED, //网络关闭过程中，网络仍连接
    NK_LPCM_NET_STATUS_WAIT_WIFIOFF_NOLINK, //网络关闭过程中，网络已断开
    NK_LPCM_NET_STATUS_WAIT_WIFIOFF_FINISHED,   //离线模式，网络已关闭
    NK_LPCM_NET_STATUS_BUTT
}NK_LPCM_NET_STATUS_E;

/**
 * @brief
 *  获取当前的网络状态
 * @param NK_Void
 *  
 * @return
 *  网络状态
 */
typedef NK_LPCM_NET_STATUS_E (*NK_LPCM_GET_NET_STATUS_F)(NK_Void);

/* 初始化低功耗管理器 */

/* LPCM库初始化属性结构体 */
typedef struct __LPCM_INIT_ATTR
{
    NK_LPCM_IO_INIT_F fnIOInit; // 初始化IO的回调
    NK_LPCM_GPIO_WRITE_F fnIOWrite; // 驱动IO的回调
    NK_LPCM_GPIO_READ_F fnIORead; // 读取IO状态的回调

    NK_LPCM_EXTI_INIT_F fnExtiInit; // 外部中断初始化函数

    NK_LPCM_ADC_READ_F fnADCRead; // 获取ADC值回调
    NK_LPCM_ADC_EN_F fnADCEn; // ADC使能控制回调

    NK_LPCM_GET_PRCSR_MDL_F fnGetPrcsrMdlName; // 获取处理器型号名

    NK_LPCM_PARAM_WRITE_F fnWriteParam; // 写数据到flash
    NK_LPCM_PARAM_READ_F fnReadParam; // 从flash读数据

    NK_LPCM_PARAM_WRITE_V2_F fnWriteParamV2;
    NK_LPCM_PARAM_READ_V2_F fnReadParamV2;
    NK_LPCM_PARAM_GET_LEN_F fnGetParamLen;
    

    NK_LPCM_SFTRST_F fnReboot; // 重启

    NK_LPCM_DEVMSG_SND_F fnSndDevMsg; // 往SOC发送业务逻辑通信数据

    NK_LPCM_PM_SET_MODE_F fnSetMode;  //设置处理器的工作模式
    NK_LPCM_FACTORY_RESET_F fnFactoryReset;  //恢复出厂设置

    NK_LPCM_DELAY_MS_F fnDelayMs;        //延时函数，该函数需让出cpu的使用权，相当于sleep函数的功能。
    NK_LPCM_DELAY_US_F fnDelayUs;       //延时函数，该函数必须是阻塞的不可被打断的。
    NK_LPCM_GET_TICK_MS_F fnGetTickMs; // 获取系统运行时间
    NK_LPCM_GET_RTC_TIME_F fnGetRtcTime; // 获取RTC实时时间

    NK_LPCM_THREAD_SUSPEND_F fnThreadSuspend; //线程挂起接口，设置SLEEP或者POWER_OFF工作模式后会调用该接口挂起LPCM库的运行线程。
    NK_LPCM_THREAD_RESUME_F fnThreadResume;  //线程恢复接口，产生中断或者调用NK_LPCM_WakeUp接口都会调用该接口来恢复LPCM库线程的运行。

    NK_LPCM_MALLOC_F fnMalloc;//动态分配内存空间
    NK_LPCM_FREE_F fnFree;    //动态释放内存空间

    NK_LPCM_MUTEX_INIT_F fnMutexInit;         //互斥锁初始化
    NK_LPCM_MUTEX_DEINIT_F fnMutexDeInit;    //互斥锁去初始化
    NK_LPCM_MUTEX_LOCK_F fnMutexLock;       //互斥锁上锁
    NK_LPCM_MUTEX_UNLOCK_F fnMutexUnlock;  //互斥锁解锁

    NK_LPCM_IRQ_DISABLE_F fnIrqDisable;   //全局中断关闭接口
    NK_LPCM_IRQ_RESTORE_F fnIrqRestore;  //全局中断开启接口

    NK_LPCM_GET_NET_STATUS_F fnGetNetStatus;//获取当前的网络状态

    NK_LPCM_COM_INIT_F fnComInit;      //休眠后会调用该接口去初始化通信接口再给SOC下电
    NK_LPCM_COM_DEINIT_F fnComDeinit; //给SOC上电后会调用该接口初始化通信接口
    NK_LPCM_COM_RESET_F fnComReset;  //通信模块重启接口
    
    NK_LPCM_GET_BAT_VOLT_F fnGetBatVolt;  //获取电池电压
    NK_LPCM_KEY_INIT_F fnKeyInit; //按键初始化
    NK_LPCM_GET_KEY_STA_F fnGetKeySta;//获取按键状态

    NK_LPCM_DRV_PIR_F fnDrvPir;
    NK_LPCM_SET_BLE_ENABLE_F fnSetBleEnable;

    NK_LPCM_POWER_UP_TYPE_E enPowerUpType; //上电类型
    NK_UInt8 batteryLevel; // 异常重启前保存的电量
}NK_LPCM_INIT_ATTR_S,*pNK_LPCM_INIT_ATTR_S;


/**
 * @brief
 *  初始化低功耗管理库
 * @param initAttr[in]
 *  LPCM库初始化属性
 * @return
 *  返回值为true成功，false失败
 */
NK_Boolean NK_LPCM_Init(pNK_LPCM_INIT_ATTR_S initAttr); 

/**
 * @brief
 *  去初始化
 * @param NK_Void
 *  
 * @return
 *  返回值为true成功，false失败
 */
NK_Boolean NK_LPCM_Deinit(NK_Void); 

/**
 * @brief
 *  获取库参数区保存的调试串口是否打开
 * @param NK_Void
 *  
 * @return
 *  返回值：0:close 1: tx 2: rx 3: tx + rx
 */
NK_UInt8 NK_LPCM_HAL_GetDbgUartEnSta(NK_Void);

/**
 * @brief
 *  获取当前的电池电量值0 - 100，用于在重启前写入Flash，重启后再把该值传入到初始化接口中，用于防止重启后电量跳变
 * @param NK_Void
 *  
 * @return
 *  电池电量
 */
NK_UInt8 NK_LPCM_GetBatteryLevel(NK_Void);
/**
 * @brief
 *  开关LPCM库的打印信息
 * @param en[in]
 *  使能
 */
NK_Void NK_LPCM_ToggleDbgPrint(NK_Boolean en);

NK_WORK_STA_E NK_LPCM_GetWkSta(NK_Void);