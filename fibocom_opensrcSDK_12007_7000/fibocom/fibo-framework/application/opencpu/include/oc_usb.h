#ifndef __OC_USB_H__
#define __OC_USB_H__

typedef void (*usb_input_callback_t)(int usb_channel, void *buf, size_t size, void *arg);
typedef int (*Report_UsbDev_Event) (int msg);

INT8 fibo_usbswitch(uint8_t en);
INT32 fibo_set_usbmode(uint8_t usbmode);
INT32 fibo_get_usbmode(void);
UINT8 fibo_usbDevice_init(char *name, usb_input_callback_t recv_cb);
UINT8 fibo_usbDevice_deinit(int usb_channel);
UINT32 fibo_usbDevice_send(int usb_channel, const void *data, size_t length);
INT8 fibo_get_usbisinsert(void);
int FIBO_usbDevice_State_report(int usb_channel, Report_UsbDev_Event  usedev_Event);
bool fibo_get_log_status(uint8_t *Apsetport, uint8_t *Apport, uint8_t *Apenable, uint8_t *Cpport, uint8_t *Cpenable);
INT32 fibo_log_to_usb(bool on);

#endif

