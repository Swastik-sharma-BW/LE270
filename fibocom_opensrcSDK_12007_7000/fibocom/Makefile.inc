#*****************************************************************
# Copyright (C), 2023, Xian Fibocom Wireless Inc
#     All rights reserved.                  
#     FileName    :Makefile.inc
#     Author      : taric.wang
#     Created     :2023-02-22
#     Description :Makefile.inc
#****************************************************************/


CUST_HOST_DIRS := $(TOP)/fibocom
$(info @@@@@@@@@@@@@@CFLAGS "$(CFLAGS)" !!!!!!)      
#CFLAGS_INC    += -I  $(TOP)/fibocom/fibo-framework/module/ffw_verno     

	
ifeq ($(TOOLCHAIN),GCC)
FREERTOS_SRC_DIRS += $(FREERTOS_DIR)/portable/gcc  \
                     $(FREERTOS_DIR)/portable/mem/tlsf
CFLAGS += -DconfigUSE_NEWLIB_REENTRANT=1
LDFLAGS += -Wl,--wrap=_malloc_r -Wl,--wrap=_free_r -Wl,--wrap=_realloc_r
endif

ifeq ($(CONFIG_ECM_DATACALL_ENABLE), y)
CFLAGS += -DCONFIG_ECM_DATACALL_ENABLE
endif

ifeq ($(CONFIG_ECM_AUTOCONNECT_DEFAULT), y)
CFLAGS += -DCONFIG_ECM_AUTOCONNECT_DEFAULT
endif

ifeq ($(CONFIG_ECM_NAT_DEFAULT), y)
CFLAGS += -DCONFIG_ECM_NAT_DEFAULT
endif

ifeq ($(CONFIG_FFW_CUST_SDK_KDWL), y)
CFLAGS += -DCONFIG_FFW_CUST_SDK_KDWL
endif

CFLAGS += -DCONFIG_FIBOCOM_LOG_LEVEL_FATAL
CFLAGS += -DCONFIG_FIBOCOM_LOG_LEVEL_ERROR
CFLAGS += -DCONFIG_FIBOCOM_LOG_LEVEL_WARN
CFLAGS += -DCONFIG_FIBOCOM_LOG_LEVEL_INFO
#CFLAGS += -DCONFIG_FIBOCOM_LOG_LEVEL_DEBUG
#CFLAGS += -DCONFIG_FIBOCOM_LOG_LEVEL_TRACE

ifneq ($(MAKECMDGOALS), unilog)
-include $(CUST_COBJS:.o=.d)
endif
-include $(CUST_COBJS:.o=.dd)



CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/PAL/include
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/common/include
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/PAL/src/ec718
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/PAL/at_engine/include
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/PAL/at_engine/ec718/include
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/module/ffw_dns
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/module/ffw_ml/include
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/module/ffw_network/include
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/module/ffw_lpg
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/module/ffw_misc
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/application/atcmd/gtset/include
CFLAGS_INC += -I $(TOP)/fibocom/gtset/include
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/3rd/uthash
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/3rd/cjson
CFLAGS_INC += -I $(TOP)/driver/board/$(TARGET)/inc/audio
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/module/ffw_audio/include
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/module/ffw_audio/src/media/inc
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/application/atcmd/tcpip/include
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/application/atcmd/http/include
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/application/opencpu/include
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/module/ffw_opencpu


ifeq ($(CONFIG_FFW_CUST_SDK_ENABLE), y)
ifeq ($(CONFIG_FFW_CUST_SDK_JIUAN), y)
SDK_CUST_DIRS  := $(TOP)/fibocom/fibo-framework/application/opencpu/custom/jiuan
CFLAGS_INC += -I$(SDK_CUST_DIRS)/include \
		-I$(SDK_CUST_DIRS)/include/NK_Apis \
		-I$(SDK_CUST_DIRS)/include/APIS_AtCmd \
		-I$(SDK_CUST_DIRS)/include/APIS_Fs \
		-I$(SDK_CUST_DIRS)/include/APIS_Gpio \
		-I$(SDK_CUST_DIRS)/include/APIS_Gsm \
		-I$(SDK_CUST_DIRS)/include/APIS_Log \
		-I$(SDK_CUST_DIRS)/include/APIS_Socket \
		-I$(SDK_CUST_DIRS)/include/APIS_Stdlib \
		-I$(SDK_CUST_DIRS)/include/APIS_System \
		-I$(SDK_CUST_DIRS)/include/APIS_Thread \
		-I$(SDK_CUST_DIRS)/include/APIS_Uart \
		-I$(SDK_CUST_DIRS)/include/APIS_Upgrade \
		-I$(SDK_CUST_DIRS)/include/APIS_Wdt
endif
endif

ifeq ($(CONFIG_CUST_XIAONIU_FEATURE), y)
    CFLAGS_INC += -I  $(TOP)/fibocom/fibo-framework/application/opencpu/custom/NIU/cwpack/
    CFLAGS_INC += -I  $(TOP)/fibocom/fibo-framework/application/opencpu/custom/NIU/aes/
    CFLAGS_INC += -I  $(TOP)/fibocom/fibo-framework/application/opencpu/custom/NIU/fota/
    CFLAGS_INC += -I  $(TOP)/fibocom/fibo-framework/application/opencpu/custom/NIU/mqtt/MQTTClient/
    CFLAGS_INC += -I  $(TOP)/fibocom/fibo-framework/3rd/MQTTPacket/src/
endif

ifeq ($(CONFIG_CUST_NBSANXING_FEATURE), y)
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/application/opencpu/custom/NBSANXING
endif

ifeq ($(CONFIG_FFW_LPM_SUPPORT), y)
    CFLAGS_INC += -I  $(TOP)/fibocom/fibo-framework/module/ffw_lpm
    CFLAGS += -DCONFIG_FFW_LPM_SUPPORT
endif
ifeq ($(CONFIG_FFW_NETWORK_ENABLE), y)
		CFLAGS += -DFFW_NETWORK_CONFIG
endif

ifeq ($(CONFIG_FFW_LCD_ENABLE), y)
       CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/module/ffw_lcd/include
endif

ifeq ($(CONFIG_FFW_CUST_SDK_JIUAN), y)
CFLAGS += -DCONFIG_FFW_CUST_SDK_JIUAN
endif

CFLAGS_DEFS += -DFEATURE_OS_ENABLE
CFLAGS_DEFS += -DFEATURE_FREERTOS_ENABLE

CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/module/ffw_key
ifeq ($(CONFIG_FFW_KEY_ENABLE), y)
CFLAGS += -DCONFIG_FFW_KEY_ENABLE
endif
