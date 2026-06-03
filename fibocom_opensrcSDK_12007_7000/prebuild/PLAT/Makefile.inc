CFLAGS_INC += -I $(LIBDIR)/PLAT/inc

LIBS_TYPE := $(TYPE)


$(info @@@@@@@@@@@@@@OPENCPU "$(OPENCPU)" !!!!!!) 

ifeq ($(TOOLCHAIN),ARMCC)

ifeq ($(OPENCPU), true)

ifeq ($(GCF_ENABLE), true)
PLAT_PREBUILD_LIBS_DIR   ?= $(LIBDIR)/PLAT/lib/keil/$(LIBS_TYPE)/gcf
else
PLAT_PREBUILD_LIBS_DIR   ?= $(LIBDIR)/PLAT/lib/keil/$(LIBS_TYPE)/oc
endif

else

PLAT_PREBUILD_LIBS_DIR   ?= $(LIBDIR)/PLAT/lib/keil/$(LIBS_TYPE)

endif

ifeq ($(BUILD_USE_PREBUILD_LIB),y)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libosa.lib \
                $(PLAT_PREBUILD_LIBS_DIR)/libmiddleware_ec_private.lib \
                $(PLAT_PREBUILD_LIBS_DIR)/libccio.lib

ifeq ($(MIDDLEWARE_FOTA_ENABLE), y)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libfota.lib\
                $(PLAT_PREBUILD_LIBS_DIR)/libdeltapatch.lib
endif

ifeq ($(BINNAME),ap_bootloader)	
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libdriver_private_bl.lib \
                $(PLAT_PREBUILD_LIBS_DIR)/libbootloader.lib
else
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libdriver_private.lib
endif
endif

ifeq ($(BUILD_USE_USB_LIB),y)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libusb_private.lib
endif

endif

ifeq ($(TOOLCHAIN),GCC)
ifeq ($(TYPE),ec718p)
ifeq ($(OPENCPU_MODE_ENABLE), y)
LIBSUBDIR := oc
ifeq ($(OPENCPU_MODE_ENABLE)_$(IMS_MODE_ENABLE), y_y)
LIBSUBDIR := ims
else ifeq ($(OPENCPU_MODE_ENABLE)_$(IMS_SMSONLY_MODE_ENABLE), y_y)
LIBSUBDIR := isms
endif
else
LIBSUBDIR := full
endif

else ifeq ($(TYPE),ec718u)
ifeq ($(OPENCPU_MODE_ENABLE), y)
LIBSUBDIR := oc
ifeq ($(OPENCPU_MODE_ENABLE)_$(IMS_MODE_ENABLE), y_y)
LIBSUBDIR := ims
else ifeq ($(OPENCPU_MODE_ENABLE)_$(IMS_SMSONLY_MODE_ENABLE), y_y)
LIBSUBDIR := isms
endif
else
LIBSUBDIR := full
endif

else# 716s 718s 716e
ifeq ($(MID_ENABLE), true)
LIBSUBDIR := mid
else ifeq ($(GCF_ENABLE), true)
LIBSUBDIR := gcf
else ifeq ($(IMS_SMSONLY_ENABLE), true)
LIBSUBDIR := isms
else ifeq ($(RAM_ENBALE), true)
LIBSUBDIR := ram
else ifeq ($(ROM_ENABLE)_$(LESS_LOG), true_true)
LIBSUBDIR := rom_lesslog
else ifeq ($(OPENCPU)_$(LESS_LOG), true_true)
LIBSUBDIR := oc_lesslog
else 
ifeq ($(ROM_ENABLE), true)
LIBSUBDIR := rom
else
LIBSUBDIR := oc
endif
endif
endif
PLAT_PREBUILD_LIBS_DIR   ?= $(LIBDIR)/PLAT/lib/gcc/$(LIBS_TYPE)/$(LIBSUBDIR)

$(info @@@@@@@@@@@@@@PLAT_PREBUILD_LIBS_DIR "$(PLAT_PREBUILD_LIBS_DIR)" !!!!!!) 
ifeq ($(BUILD_USE_PREBUILD_LIB),y)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libosa.a \
                $(PLAT_PREBUILD_LIBS_DIR)/libmiddleware_ec_private.a \
                $(PLAT_PREBUILD_LIBS_DIR)/libccio.a

ifeq ($(MIDDLEWARE_FOTA_ENABLE), y)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libfota.a
ifeq ($(MIDDLEWARE_FOTA_CORE2_ENABLE), y)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libdeltapatch2.a
else
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libdeltapatch.a
endif
endif


ifeq ($(BINNAME),ap_bootloader)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libdriver_private_bl.a \
                $(PLAT_PREBUILD_LIBS_DIR)/libbootloader.a

PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/bootloader/libmiddleware_ec.a \
                $(PLAT_PREBUILD_LIBS_DIR)/bootloader/liblfs.a \
                $(PLAT_PREBUILD_LIBS_DIR)/bootloader/libdriver.a

ifeq ($(THIRDPARTY_LZMA_ENABLE), y)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/bootloader/liblzma.a
endif

else ifeq ($(BINNAME),ap_driver_example)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libdriver_private_drv.a
else
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libdriver_private.a

PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libmiddleware_ec.a \
                $(PLAT_PREBUILD_LIBS_DIR)/liblfs.a \
                $(PLAT_PREBUILD_LIBS_DIR)/liblzma.a \
                $(PLAT_PREBUILD_LIBS_DIR)/libdriver.a \
                $(PLAT_PREBUILD_LIBS_DIR)/libpsnv.a \
                $(PLAT_PREBUILD_LIBS_DIR)/libyrcompress.a \
                $(PLAT_PREBUILD_LIBS_DIR)/libfibocom.a \
                $(PLAT_PREBUILD_LIBS_DIR)/libfreertos.a \
                $(PLAT_PREBUILD_LIBS_DIR)/liblwip.a \
                $(PLAT_PREBUILD_LIBS_DIR)/libmbedtls.a \
                $(PLAT_PREBUILD_LIBS_DIR)/libtcpipmgr.a \
                $(TOP)/fibocom/fibo-framework/PAL/src/ec718/lib/tts/libaisound50.a

ifeq ($(BUILD_IMS), y)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libimsnv.a
endif

ifeq ($(CONFIG_FFW_CUST_SDK_ENABLE), y)
ifeq ($(CONFIG_FFW_CUST_SDK_JIUAN), y)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libjiuancust.a
endif
ifeq ($(CONFIG_FFW_CUST_SDK_KDWL), y)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libsKDWLcust.a
endif
endif

ifeq ($(CONFIG_CUST_XIAONIU_FEATURE), y)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libNIUcust.a
endif

ifeq ($(CONFIG_FIBO_DEVCFG), y)
CFLAGS += -DCONFIG_FIBO_DEVCFG
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libdevcfg.a
endif

endif

endif

#qocm also use usb libs and bl/app use different libs
ifeq ($(BUILD_USE_USB_LIB),y)
ifeq ($(BINNAME),ap_bootloader)
ifeq ($(DRIVER_BLUSB_CUTDOWN_DIR_EXIST),y)
CFLAGS_INC += -I $(LIBDIR)/PLAT/inc/usb_bl
endif
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libusbbl_priv.a
else
ifeq ($(DRIVER_USB_ENABLE),y)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libusb_private.a
ifeq ($(DRIVER_BLUSB_CUTDOWN_DIR_EXIST),y)
CFLAGS_INC += -I $(LIBDIR)/PLAT/inc/usb
endif
endif
endif
endif

endif
