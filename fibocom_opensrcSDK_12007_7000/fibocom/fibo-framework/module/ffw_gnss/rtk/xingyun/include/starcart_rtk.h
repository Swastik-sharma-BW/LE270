#ifndef _STARCART_RTK_H_
#define _STARCART_RTK_H_
#ifdef __cplusplus
extern "C"
{
#endif
    //SDK状态码
    typedef enum
    {
        STARCART_SDK_STATUS_UNKNOWN = 0, //未知状态

        STARCART_SDK_STATUS_AUTH_SUCCESS = 1000,         //SDK鉴权成功
        STARCART_SDK_STATUS_AUTH_FAILED = 1001,          //SDK鉴权过程遇到失败
        STARCART_SDK_STATUS_AUTH_MISS_PARAM = 1002,      //SDK鉴权参数缺失
        STARCART_SDK_STATUS_AUTH_INVALID_PARAM = 1003,   //SDK鉴权存在无效的参数
        STARCART_SDK_STATUS_AUTH_INVALID_REQUEST = 1004, //SDK鉴权启用了无效的服务请求
        STARCART_SDK_STATUS_AUTH_ERROR_UNKONWN = 1005,   //SDK鉴权遇到未知错误

        STARCART_SDK_STATUS_AUTH_APP_NOT_EXIST = 2000,   //SDK鉴权（appkey）应用不存在
        STARCART_SDK_STATUS_AUTH_APP_LIMIT_COUNT = 2001, //SDK鉴权（appkey）应用用量已到达限额

        STARCART_SDK_STATUS_AUTH_USER_NOT_EXIST = 3000,       //用户不存在
        STARCART_SDK_STATUS_AUTH_USER_IS_USED = 3001,         //用户账号已被占用
        STARCART_SDK_STATUS_AUTH_USER_IS_DISABLED = 3002,     //用户账号被禁用
        STARCART_SDK_STATUS_AUTH_USER_ACTIVE_EXCEEDED = 3003, //用户账号激活时间已超过有效期
        STARCART_SDK_STATUS_AUTH_USER_EXPIRED = 3004,         //用户账号的使用时间已到期
        STARCART_SDK_STATUS_AUTH_USER_NOT_ACTIVE = 3005,      //用户账号不存在激活时间
        STARCART_SDK_STATUS_AUTH_USER_ATV_WAY_ERROR = 3006,   //用户账号激活方式不符合

        STARCART_SDK_STATUS_AUTH_DEVICE_NOT_EXIST = 4000, //SDK鉴权设备不存在
        STARCART_SDK_STATUS_AUTH_DEVICE_CODE_USED = 4001, //SDK鉴权设备编码已被占用

        STARCART_SDK_STATUS_AUTH_SERVICE_NOT_EXIST = 5000,  //服务不存在
        STARCART_SDK_STATUS_AUTH_SERVICE_IS_EXPIRED = 5001, //服务已过期

        STARCART_SDK_STATUS_CORS_INVALID_GGA = 6001,    //无效的GGA，找不到VRS点
        STARCART_SDK_STATUS_CORS_NO_BOS_MESSAGE = 6002, //没有观测电文
        STARCART_SDK_STATUS_CORS_NO_NAV_MESSAGE = 6003, //没有导航电文
        STARCART_SDK_STATUS_CORS_NO_MESSAGE = 6004,     //没有导航电文和观测电文
        STARCART_SDK_STATUS_CORS_NO_VRS_INFO = 6005,    //不存在的VRS信息

        STARCART_SDK_STATUS_CORS_TOKEN_EXPIRED = 7000,   //TOKEN过期
        STARCART_SDK_STATUS_CORS_TOKEN_INVALID = 7001,   //无效的TOKEN
        STARCART_SDK_STATUS_CORS_TOKEN_NOT_EXIST = 7002, //TOKEN不存在
    } starcart_status;

    typedef enum
    {
        MANUAL = 0,   //外部控制接入服务
        INTERNAL = 1, //SDK内部控制接入服务
    } starcart_exec_mode;

    typedef enum
    {
        HTTP = 0,  //HTTP   协议
        HTTPS = 1, //HTTPS  协议
    } starcart_auth_mode;

    /*输出高精度定位数据的结构体*/
    typedef struct
    {
        char gga[128]; //高精度的gga数据
        char rmc[128]; //高精度的rmc数据
    } nmea_data_t;

    /*初始化配置结构体*/
    typedef struct
    {
        char appKey[64];                //app密钥
        char deviceId[64];              //设备ID：设备序列号，cpu id，网卡id IMEI等设备唯一标识符
        int log_level;                  //日志级别 0~5,0日志不可用;
        starcart_exec_mode exec_mode;   //SDK CORS接入模式，内部控制或外部控制
        starcart_auth_mode auth_mode;   //SDK 鉴权协议，HTTP or HTTPS
    } starcart_config_t;

    /*用户需实现的回调函数结构体*/
    typedef struct
    {
        void (*outputSdkNmeaData)(nmea_data_t *nmeaData); //回调输出高精度定位结果
        void (*outputUartNmeaData)(const char *nmeaData); //回调输出原始精度定位结果
        void (*outputSdkStatus)(starcart_status status);  //回调输出SDK状态
        void (*logPrintf)(const char *fmt, ...);                //回调用户平台日志函数   
    } starcart_callbacks_t;
    /*SDK接口结构体*/
    typedef struct
    {
        int (*init)(starcart_config_t *config, starcart_callbacks_t *callbacks);  //初始化SDK
        int (*start)(void);                                                       //启动高精度定位服务
        int (*updateGnssRawData)(const char *gnssRawData, int gnssRawDataLen);    //更新gnss 原始数据
        int (*updateCorsRtcmData)(const char *CorsRtcmData, int CorsRtcmDataLen); //更新Cors服务的差分改正数,外部控制模式下调用，其他模式不需调用
        int (*stop)(void);                                                        //停止或退出高精度定位服务
        int (*destroy)(void);                                                     //回收SDK资源
    } starcart_sdk_interface_t;
    /*获取SDK接口函数*/
    extern starcart_sdk_interface_t *getStarcartLocationSdkInterface(void);
#ifdef __cplusplus
}
#endif
#endif
