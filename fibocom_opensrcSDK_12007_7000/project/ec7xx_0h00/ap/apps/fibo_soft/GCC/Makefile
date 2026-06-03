
AVAILABLE_TARGETS = ec7xx_0h00
TOOLCHAIN         = GCC
BINNAME           = ap_$(PROJECT)
TOP              := ../../../../../..

include $(TOP)/project/$(AVAILABLE_TARGETS)/ap/apps/fibo_soft/GCC/target.inc
include $(TOP)/project/$(AVAILABLE_TARGETS)/ap/apps/fibo_soft/GCC/fibo_soft.mak

ifeq ($(EUTRAN_MODE), cat_mode)
EUTRAN_CAT_MODE     = y
endif

#if need usb auto adapt support, set to y
#USBNET_AUTO_ADAPT_ENABLE=y

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

ifeq ($(TYPE), ec718p)
#AT+ECDIEXY
BUILD_PLAT_PROD_ECDIEXY_ENABLE     = y
endif

ifeq ($(ROM_ENABLE), true)
MORE_ROM_ENABLE = y
endif

ifeq ($(LESS_LOG), true)
LESS_LOG_ENABLE = y
endif

ifeq ($(TYPE), ec718pm)
EC718XM_LD_AND_PREC = y
EC718XM_VPU_ENHANCE = y
endif

ifeq ($(TYPE), ec718um)
EC718XM_LD_AND_PREC = y
EC718XM_VPU_ENHANCE = y
endif

ifeq ($(TYPE), ec718sm)
EC718XM_LD_AND_PREC = y
EC718XM_VPU_ENHANCE = y
endif

ifeq ($(TYPE), ec718hm)
EC718XM_LD_AND_PREC = y
EC718XM_VPU_ENHANCE = y
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

ifeq ($(CONFIG_FIBO_FEATURE_IMS_ENABLE),y)
CFLAGS_DEFS += -DCONFIG_FIBO_FEATURE_IMS_ENABLE
endif

ifeq ($(CONFIG_FIBO_FEATURE_ISMS_ENABLE),y)
CFLAGS_DEFS += -DCONFIG_FIBO_FEATURE_ISMS_ENABLE
endif

ifeq ($(IMS_ENABLE), true)
CFLAGS_DEFS += -DFEATURE_IMS_ENABLE
CFLAGS_DEFS += -DFEATURE_IMS_CC_ENABLE
CFLAGS_DEFS += -DFEATURE_IMS_SMS_ENABLE
ifneq ($(findstring $(TYPE), ec718p ec718u),)
CFLAGS_DEFS += -DFEATURE_IMS_USE_PSRAM_ENABLE
endif
ifeq ($(TYPE), ec718p)
CFLAGS_DEFS += -DFEATURE_IMS_EMC_ENABLE
endif
CFLAGS_DEFS += -DFEATURE_IMS_UT_ENABLE
CFLAGS_DEFS += -fno-strict-aliasing
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
ifeq ($(CONFIG_FIBO_FEATURE_PLAT_TASKINFO_ENABLE), y)
BUILD_PLAT_MISC_ECTASKINFO_ENABLE = y
endif


ifeq ($(CONFIG_FFW_AUDIO_ENABLE), y)

ifeq ($(CONFIG_FFW_AUDIO_MP3_ENABLE), n)
CONFIG_FFW_AUDIO_MP3_ENABLE = n
else
CONFIG_FFW_AUDIO_MP3_ENABLE = y
endif

ifeq ($(CONFIG_FFW_AUDIO_WAV_ENABLE), n)
CONFIG_FFW_AUDIO_WAV_ENABLE = n
else
CONFIG_FFW_AUDIO_WAV_ENABLE = y
endif

ifeq ($(CONFIG_FFW_AUDIO_PCM_ENABLE), n)
CONFIG_FFW_AUDIO_PCM_ENABLE = n
else
CONFIG_FFW_AUDIO_PCM_ENABLE = y
endif

ifeq ($(CONFIG_FFW_AUDIO_MP3_ENABLE),y)
CFLAGS_INC += -I $(TOP)/fibocom/fibo-framework/3rd/mp3/libmad-0.15.1b \
              -I $(TOP)/fibocom/fibo-framework/3rd/mp3/libmad-0.15.1b/msvc++
endif
endif

ifeq ($(ROM_ENABLE), true)
CFLAGS_DEFS += -DFEATURE_MOREROM_ENABLE
endif

ifeq ($(LESS_LOG), true)
CFLAGS_DEFS += -DFEATURE_LESSLOG_ENABLE
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


CFLAGS_INC        += -I ../inc                   \
                     -I $(TOP)/FIRMWARE/SRC/CAT1/Common/Inc \
                     -I $(TOP)/driver/chip/$(CHIP)/ap/src/usb
obj-y             += project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/app.o \
					 project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/rawData.o \
                     project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/bsp_custom.o
ifeq ($(CONFIG_FFW_MIFI_ENABLE),y)
obj-y             += project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/sfdt_slave.o
endif

ifeq ($(CONFIG_FFW_CUST_ETHER_ENABLE), y)
obj-y             += project/$(TARGET)/$(CORE)/apps/$(PROJECT)/src/cecs_master.o
endif

#CONFIG_PROJ_APP_SECURITY_BOOT  = y

include $(TOP)/tools/scripts/Makefile.rules

#configure USBNET_AUTO_ADAPT_ENABLE in ".\device\target\board\%BOARD_NAME%\ap\%BOARD_NAME%_ap.mk"'
ifeq ($(USBNET_AUTO_ADAPT_ENABLE), y)
CFLAGS_DEFS += -DFEATURE_USBNET_ATA_FOR_AP
CFLAGS_DEFS += -DFEATURE_FIX_REMOTE_WKUP_UNPAIRED_CASE
endif


#enable wdt
CFLAGS += -DWDT_FEATURE_ENABLE=1
CFLAGS += -DFEATURE_UART_HELP_DUMP_ENABLE

#Enable SIM hotswap feature with pad configration and jitter handle by required----don't remove it
#CFLAGS += -DSIM_HOT_SWAP_FEATURE


#Make all warnings into errors
CFLAGS += -Werror


ifneq ($(OPENCPU_MODE_ENABLE),y)
ifneq ($(BUILD_AT),y)
$(error This example needs to modify "BUILD_AT" to "y" in device\target\board\$(TARGET)\$(CORE)\$(TARGET)_$(CORE).mk)
endif
endif
