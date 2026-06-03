/*
 * @Description: 
 * @Author: Chen.xd
 * @Date: 2020-12-24 13:56:47
 */
/************************************************************************
**函数名称:  starcart_http_get
**入口参数:  data: 响应数据buf
            dataLen: 响应数据长度
            url: 数据地址
**出口参数:  int  ： 0 成功,-1失败
**函数功能:  http get请求
*************************************************************************/
int starcart_http_get(char *data, int *dataLen, const char *url); //回调获取cors服务差分改正数据