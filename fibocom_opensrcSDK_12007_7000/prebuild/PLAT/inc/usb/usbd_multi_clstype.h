#ifndef __USBD_MULTI_CLSTYPE_H__
#define __USBD_MULTI_CLSTYPE_H__

#ifdef CONFIG_FFW_EC_USB_ACM_TO_USB_SERIAL //modify USB ACM to USB Serial by fibocom.liuben
#define SIMU_QUEC_CFG
#endif

typedef enum
{
    multidev_tp_none = 0,
    multidev_tp_rndis = 1,
    multidev_tp_ecm,
    multidev_tp_vcom_at,
    multidev_tp_vcom_log,    
    multidev_tp_vcom_ppp,        
    multidev_tp_vcom_com,            
    multidev_tp_ccid,            
}multidev_cls_type;


#endif

