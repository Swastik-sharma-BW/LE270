typedef signed int int32_t;
typedef unsigned int uint32_t;
/**
* @brief DEFAULT_USB_MODE, 配置默认USBMODE
*  0: 开启USB,但是不开启RNDIS或ECM网卡      （相当于AT+GTUSBMODE=30)
*  1: 开启USB,网卡是RNDIS                   （相当于AT+GTUSBMODE=31)
*  2: 开启USB,网卡ECM网卡                   （相当于AT+GTUSBMODE=32)
*/
#define  DEFAULT_USB_MODE                    2


/**
*  ECM_AUTOCONNECT: 配置默认ECM/RNDIS自动拨号
*  0:  非自动拨号
*  1： 自动拨号
*/
#define  DEFAULT_ECM_RNDIS_AUTOCONNECT        0


/**
*  ECM_NAT_ENABLE: 配置默认NAT开
*  0:  IP直连模式，NAT关闭
*  1： NAT开启
*/
#define  DEFAULT_NAT_ENABLE                   1


/**
*  NAT DCHP 相关配置, DEFAULT_NAT_ENABLE 设置为1的情况下有效
*  NAT_DHCP_GW_IP:    NAT 模式下模组网关IP地址
*  NAT_DHCP_START_IP: NAT 模式下DHCP起始地址 (主机IP地址)
*  NAT_DHCP_END_IP:   NAT 模式下DHCP结束地址 (LE270系列只支持单地址，和其实地址一致)
*  NAT_DHCP_DNS1:     NAT 模式DNS地址1           (DNS地址1，可以和网关一致)
*  NAT_DHCP_DNS2:     NAT 模式DNS地址2           (DNS地址2，可以和网关一致)
*/
/* C0A80A01 == 192.168.10.1 , 设置范围192.168.X.X*/
#define  DEFAULT_NAT_DHCP_GW_IP    0xC0A80A01
/* C0A80A02 == 192.168.10.2 */
#define  DEFAULT_NAT_DHCP_START_IP 0xC0A80A02
#define  DEFAULT_NAT_DHCP_END_IP   DEFAULT_NAT_DHCP_START_IP
#define  DEFAULT_NAT_DHCP_DNS1     DEFAULT_NAT_DHCP_GW_IP
#define  DEFAULT_NAT_DHCP_DNS2     DEFAULT_NAT_DHCP_GW_IP

#define  DEFAULT_CLAT_NETIF_IP    0xC0A80A05


/**
* DEFAULT_ECM_RST_DISABLE  0: SEND TCP RST when ECM disconnected (Default)
* DEFAULT_ECM_RST_DISABLE  1: Don't send TCP RST when ECM disconnected
*/
#define  DEFAULT_ECM_RST_DISABLE           0


int32_t fibo_get_default_usbmode()
{
    #ifdef CONFIG_ECM_CUST_SET_DISABLE
    return -1;
    #else
    return DEFAULT_USB_MODE;
    #endif
}

int32_t fibo_get_ecm_autoconnect()
{
    #ifdef CONFIG_ECM_CUST_SET_DISABLE
    return -1;
    #else
    return DEFAULT_ECM_RNDIS_AUTOCONNECT;
    #endif
}

int32_t fibo_get_ecm_nat_enable()
{
    #ifdef CONFIG_ECM_CUST_SET_DISABLE
    return -1;
    #else
    return DEFAULT_NAT_ENABLE;
    #endif
}

int32_t fibo_get_ecm_nat_dhcp_cfg(uint32_t *gw_ip, uint32_t *dhcp_start, uint32_t *dhcp_end, uint32_t *dns1, uint32_t *dns2)
{
    #ifdef CONFIG_ECM_CUST_SET_DISABLE
    return -1;
    #else
    if(DEFAULT_NAT_ENABLE != 1)
        return -1;
    if(!gw_ip || !dhcp_start || !dhcp_end || !dns1 || !dns2)
        return -1;

    *gw_ip      = DEFAULT_NAT_DHCP_GW_IP;
    *dhcp_start = DEFAULT_NAT_DHCP_START_IP;
    *dhcp_end   = DEFAULT_NAT_DHCP_END_IP;
    *dns1       = DEFAULT_NAT_DHCP_DNS1;
    *dns2       = DEFAULT_NAT_DHCP_DNS2;

    return 0;
    #endif
}

int32_t fibo_get_clat_cfg(uint32_t *ip)
{

    if(DEFAULT_NAT_ENABLE != 1)
        return -1;
    if(!ip)
        return -1;

    *ip      = DEFAULT_CLAT_NETIF_IP;

    return 0;
}

int32_t fibo_get_ecm_rst_status(uint32_t *status)
{
    *status = DEFAULT_ECM_RST_DISABLE;
    return 0;
}

int32_t fibo_get_xlat_enable()
{
#ifdef FIBO_FEATURE_XLAT
    return 1;
#else
    return 0;
#endif
}

