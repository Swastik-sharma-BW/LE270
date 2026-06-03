#ifndef APIS_UART_H_
#define APIS_UART_H_
NK_CPP_EXTERN_BEGIN

typedef enum 
{
    NK_UART_PORT_NONE = -1,
    NK_UART_PORT_SOC,          //跟soc通信的串口
    NK_UART_PORT_BLE,          //跟蓝牙通信的串口

    NK_UART_PORT_MAX
}NK_UART_PORT_NUM_E;

typedef enum
{
    NK_UART_PARITY_NONE,
    NK_UART_PARITY_ODD,
    NK_UART_PARITY_EVEN,
}NK_UART_PARITYBIT_E;

typedef enum
{
    NK_FC_NONE = 0,
    NK_FC_HW,
}NK_UART_FLOWCTL_E;

typedef enum{
    NK_UART_EVENT_UNKNOW,
    NK_UART_EVENT_RX_RECV_DATA,
    NK_UART_EVENT_RX_OVERFLOW,
}NK_UART_EVENT_E;

#define COMMPLAT_BLE_UART_BOARDRATE             (115200)
#define COMMPLAT_BLE_UART_DATABIT               (8)
#define COMMPLAT_BLE_UART_STOPBIT               (1)
#define COMMPLAT_BLE_UART_PARITY                (NK_UART_PARITY_NONE)
#define COMMPLAT_BLE_UART_FLOWCTL               (NK_FC_NONE)

#define COMMPLAT_SOC_UART_BOARDRATE             (115200)
#define COMMPLAT_SOC_UART_DATABIT               (8)
#define COMMPLAT_SOC_UART_STOPBIT               (1)
#define COMMPLAT_SOC_UART_PARITY                (NK_UART_PARITY_NONE)
#define COMMPLAT_SOC_UART_FLOWCTL               (NK_FC_NONE)

typedef NK_Void (*NK_UART_CALLBACK)(NK_UART_EVENT_E ind_type, NK_UART_PORT_NUM_E port, NK_UInt32 size);

typedef struct
{
    NK_Int              baudrate;
    NK_Int              data_bit;
    NK_Int              stop_bit;
    NK_UART_PARITYBIT_E parity_bit;
    NK_UART_FLOWCTL_E   flow_ctrl;
    NK_UART_CALLBACK    uart_callback;
}NK_UART_CONFIG_S;

struct APIS_UART_S{
    /**
    * @brief 打开并初始化串口
    * @param  ps32Fd [OUT] 创建的文件句柄
    * @param  port   [IN]  要打开的目标串口
    * @param  pConf  [IN]  串口初始化配置
    * @return NK_TRUE：成功
    * @return NK_FALSE：失败
    */
    NK_Int32 (*open)(NK_Int32 *ps32Fd, NK_UART_PORT_NUM_E port, NK_UART_CONFIG_S *pConf);

    /**
    * @brief 从串口读取指定大小的数据
    * @param  s32Fd  [IN]  文件描述符
    * @param  pu8Buf [OUT] 保存读取数据的缓冲区
    * @param  u32Len [IN]  需要读取的长度
    * @return >=0：成功, 返回成功读取的字节数
    * @return -1：失败
    * @note 缓冲区应由用户申请内存，并注意读取的长度不超过缓冲区大小
    */
    NK_Int32 (*read)(NK_Int32 s32Fd, NK_UInt8 *pu8Buf, NK_UInt32 u32Len);

    /**
    * @brief 向串口写入指定大小的数据
    * @param  s32Fd  [IN] 文件描述符
    * @param  pu8Buf [IN] 存放需要写入的数据
    * @param  u32Len [IN] 需要写入的长度
    * @return >=0：成功, 返回成功写入的字节数
    * @return -1：失败
    */
    NK_Int32 (*write)(NK_Int32 s32Fd, const NK_UInt8 *pu8Buf, NK_UInt32 u32Len);

    /**
    * @brief 关闭一个已经打开的串口
    * @param  ps32Fd [IN] 文件描述符
    * @return NK_TRUE：成功
    * @return NK_FALSE:失败
    */
    NK_Int32 (*close)(NK_Int32 s32Fd);
};


NK_API struct APIS_UART_S *APIS_Uart(void);

NK_CPP_EXTERN_END
#endif