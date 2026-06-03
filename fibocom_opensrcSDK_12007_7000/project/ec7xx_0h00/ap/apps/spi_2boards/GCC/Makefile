
AVAILABLE_TARGETS = ec7xx_0h00
TOOLCHAIN         = GCC
BINNAME           = ap_$(PROJECT)
TOP              := ../../../../../..

ifeq ($(EUTRAN_MODE), cat_mode)
EUTRAN_CAT_MODE     = y
endif

#enable below setting if OPENCPU_MODE is needed
ifeq ($(OPENCPU), true)
OPENCPU_MODE_ENABLE = y
endif

ifeq ($(GCF_ENABLE), true)
GCF_FEATURE_ENABLE = y
endif

ifeq ($(MID_ENABLE), true)
MID_FEATURE_ENABLE = y
endif

ifeq ($(IMS_ENABLE), true)
IMS_MODE_ENABLE = y
endif

ifeq ($(IMS_SMSONLY_ENABLE), true)
IMS_SMSONLY_MODE_ENABLE = y
endif

ifeq ($(AUDIO_ENABLE), true)
AUDIO_MODE_ENABLE = y
endif

ifeq ($(AUDIO_FIX_ENABLE), true)
AUDIO_FIX_SRC_ENABLE = y
endif

ifeq ($(TYPE), ec716s)
ifeq ($(RAM_ENBALE), true)
MORE_RAM_ENABLE = y
endif
endif

ifeq ($(ROM_ENABLE), true)
MORE_ROM_ENABLE = y
ifeq ($(LESS_LOG), true)
LESS_LOG_ENABLE = y
endif
endif




#features will be customized to whatever customers want!
include $(TOP)/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/GCC/FeatCustom_$(TYPE).inc

ifeq ($(EXCEPTION_FLASH_DUMP_ENABLE), y)
CFLAGS_DEFS += -DFEATURE_EXCEPTION_FLASH_DUMP_ENABLE
endif

ifeq ($(MIDDLEWARE_AMR_ENABLE), y)
CFLAGS_DEFS += -DFEATURE_AMR_CP_ENABLE
endif

ifeq ($(MIDDLEWARE_VEM_ENABLE), y)
CFLAGS_DEFS += -DFEATURE_VEM_CP_ENABLE
endif

ifeq ($(THIRDPARTY_JPEGCOMP_ENABLE),y)
CFLAGS_DEFS += -DFEATURE_JPEGCOMP_ENABLE
endif

ifeq ($(IMS_ENABLE), true)
CFLAGS_DEFS += -DFEATURE_IMS_ENABLE
CFLAGS_DEFS += -DFEATURE_IMS_CC_ENABLE
CFLAGS_DEFS += -DFEATURE_IMS_SMS_ENABLE
CFLAGS_DEFS += -DFEATURE_IMS_USE_PSRAM_ENABLE
CFLAGS_DEFS += -fno-strict-aliasing
endif

ifeq ($(IMS_UT_ENABLE), true)
CFLAGS_DEFS += -DFEATURE_IMS_UT_ENABLE
endif

ifeq ($(IMS_SMSONLY_ENABLE), true)
CFLAGS_DEFS += -DFEATURE_IMS_ENABLE
CFLAGS_DEFS += -DFEATURE_IMS_SMS_ENABLE
CFLAGS_DEFS += -fno-strict-aliasing
#        NO FEATURE_IMS_CC_ENABLE
endif

ifeq ($(AUDIO_ENABLE), true)
ifeq ($(BUILD_SUPPORT_APP_PCM_MEM_POOL), y)
CFLAGS_DEFS += -DFEATURE_SUPPORT_APP_PCM_MEM_POOL
endif
CFLAGS_DEFS += -DFEATURE_AUDIO_ENABLE
endif

ifneq ($(findstring $(TYPE), ec716s ec716e),)
ifeq ($(RAM_ENBALE), true)
CFLAGS_DEFS += -DFEATURE_MORERAM_ENABLE
endif
endif

ifeq ($(ROM_ENABLE), true)
CFLAGS_DEFS += -DFEATURE_MOREROM_ENABLE
ifeq ($(LESS_LOG), true)
CFLAGS_DEFS += -DFEATURE_LESSLOG_ENABLE
endif
endif




ifeq ($(THIRDPARTY_CMCC_DM_ENABLE), y)
THIRDPARTY_CISONENET_ENABLE    = y
THIRDPARTY_ERCOAP_ENABLE       = y
endif

#ensure AT command to be consistent with protocol
#such as mqtt/...
ifeq ($(THIRDPARTY_MQTT_ENABLE), n)
BUILD_PLAT_MQTT_AT_ENABLE      = n
endif

ifeq ($(THIRDPARTY_HTTPC_ENABLE), n)
BUILD_PLAT_HTTP_AT_ENABLE      = n
endif

ifeq ($(THIRDPARTY_MBEDTLS_ENABLE), n)
BUILD_PLAT_SSL_AT_ENABLE      = n
endif

ifeq ($(MIDDLEWARE_FOTAPAR_ENABLE), n)
BUILD_PLAT_FOTA_AT_ENABLE     = n
BUILD_PLAT_ECOTA_AT_ENABLE    = n
endif

SFDT_FUNC_USE_FLOWCTRL = n

CFLAGS_INC        += -I ../inc                   \
                     -I $(TOP)/FIRMWARE/SRC/CAT1/Common/Inc \
                     -I $(TOP)/driver/chip/$(CHIP)/ap/src/usb
obj-y             += project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/app.o \
                     project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/rawData.o \
                     project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/bsp_custom.o \
                     project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/sfdt_master.o \
                     project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/cmux.o \
                     project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/cmux_chat.o \
                     project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/cmux_utils.o \
                     project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/cmux_host_example.o \
                     project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/loop_back_test.o
ifeq ($(SFDT_FUNC_USE_FLOWCTRL), n)
obj-y             += project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/sfdt_slave.o
else
obj-y             += project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/sfdt_slave_fc.o
endif
#CONFIG_PROJ_APP_SECURITY_BOOT  = y

include $(TOP)/tools/scripts/Makefile.rules

#enable wdt
CFLAGS += -DWDT_FEATURE_ENABLE=1
CFLAGS += -DFEATURE_UART_HELP_DUMP_ENABLE

#Enable SIM hotswap feature with pad configration and jitter handle by required----don't remove it
#CFLAGS += -DSIM_HOT_SWAP_FEATURE


#Make all warnings into errors
CFLAGS += -Werror
CFLAGS += -DSFDT_MASTER_BOARD

ifneq ($(OPENCPU_MODE_ENABLE),y)
ifneq ($(BUILD_AT),y)
$(error This example needs to modify "BUILD_AT" to "y" in device\target\board\$(TARGET)\$(CORE)\$(TARGET)_$(CORE).mk)
endif
endif