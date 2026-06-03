#include "app_image.h"
#include "fibo_opencpu.h"
#include "app.h"
#include "app_pdp.h"


/*****************************************************************************
*
* Note:
*   App receives fibocom events callback
*
*****************************************************************************/
static void sig_res_callback(GAPP_SIGNAL_ID_T sig, va_list arg)
{
    APP_LOG_INFO("sig_res_callback  sig = %d (200 is opencpu test.)", sig);
}

/*****************************************************************************
*
* Note:
*   fibo_send_at callback
*
*****************************************************************************/
static void at_res_callback(UINT8 *buf, UINT16 len)
{
    APP_LOG_INFO("FIBO <--%s", buf);
}

static FIBO_CALLBACK_T user_callback = {
    .fibo_signal = sig_res_callback,
    .at_resp = at_res_callback};

typedef enum
{
    DRV_SERIAL_EVENT_BROKEN = (1 << 3),      ///< usb port broken
    DRV_SERIAL_EVENT_READY = (1 << 4),       ///< usb port ready
}usbEvent;

int usbState = 0;
#define QUEUESIZE 8
UINT32 gQueueId = 0;
int msg=0;

/*
* Callback functions cannot have time-consuming operations, such as printing logs, otherwise a crash may occur.
* It is recommended to send an event for handling.
*/
int UsbDev_Event(int arg)
{
    usbState = arg;
    switch(usbState)
    {
        case DRV_SERIAL_EVENT_READY:
            if(gQueueId != 0){
                msg = DRV_SERIAL_EVENT_READY;
                fibo_queue_put_isr(gQueueId,&msg);
            }
        break;
        case DRV_SERIAL_EVENT_BROKEN:
            if(gQueueId != 0){
                msg = DRV_SERIAL_EVENT_BROKEN;
                fibo_queue_put_isr(gQueueId,&msg);
            }
        break;
        default:
        {
            usbState = 0;
            break;
        }
    }
    return 0;
}

static void usb_vbus_det_demo()
{
    APP_LOG_INFO("application thread enter");

    for (int n = 0; n < 10; n++)
    {
        APP_LOG_INFO("app image hello world %d", n);
        fibo_taskSleep(500);
    }
    int ret = fibo_queue_create(QUEUESIZE,(QUEUESIZE*sizeof(int)));
    if(ret != 0) {
        APP_LOG_INFO("fibo_queue_create OK %x",ret);
        gQueueId = ret;
    }

    FIBO_usbDevice_State_report((int)NULL, UsbDev_Event);

    while(1)
    {
        int msg = 0;
        if(usbState == DRV_SERIAL_EVENT_READY)
        {
            APP_LOG_INFO("usb State READY");
        }
        else if(usbState == DRV_SERIAL_EVENT_BROKEN)
        {
            APP_LOG_INFO("usb State BROKEN");
        }

        if(gQueueId != 0)
        {
            int ret = fibo_queue_get(gQueueId,&msg,0);
            if(ret == 0)
            {
                APP_LOG_INFO("fibo_queue_get true %d",msg);
            }
        }
    }
}


static void usb_input_callback(int usb_channel, void *data, size_t len, void *arg)
{
    APP_LOG_INFO("cust usb(%d) recevied(%d) %.*s",usb_channel,len,len,data);
}

static void usb_send_demo()
{

    int usb_channel;

    for (int n = 0; n < 10; n++)
    {
        APP_LOG_INFO("app image hello world %d", n);
        fibo_taskSleep(500);
    }

    usb_channel = fibo_usbDevice_init(NULL,usb_input_callback);

    while(1)
    {
        fibo_usbDevice_send(usb_channel,"hello world\n",12);
        fibo_taskSleep(1000);
    }
    fibo_usbDevice_deinit(usb_channel);
    
}

void *appimg_enter(void *param)
{
    APP_LOG_INFO("app image enter");
    fibo_thread_create(usb_vbus_det_demo, (INT8 *)"usb_vbus_det_demo", 4*1024, NULL, osPriorityNormal);
    fibo_thread_create(usb_send_demo, (INT8 *)"usb_send_demo", 4*1024, NULL, osPriorityNormal);
    return (void *)&user_callback;
}

void appimg_exit(void)
{
    APP_LOG_INFO("application image exit");
    return;
}

