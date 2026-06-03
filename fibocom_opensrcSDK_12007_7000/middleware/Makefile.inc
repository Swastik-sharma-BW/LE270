#Makefile.inc
#used for opencpu


MW_HOST_DIRS := $(TOP)/middleware
CFLAGS += -DARM_MATH_CM3
CFLAGS += -DFEATURE_LZMA_ENABLE -DFEATURE_CCIO_ENABLE -DFOTA_NVM_FS_ENABLE -DFEATURE_OS_ENABLE
CFLAGS_INC += -I $(MW_HOST_DIRS)/developed/debug/inc \
              -I $(MW_HOST_DIRS)/developed/common/inc        \
              -I $(MW_HOST_DIRS)/developed/cms/psdial/inc \
              -I $(MW_HOST_DIRS)/developed/cms/cms/inc \
              -I $(MW_HOST_DIRS)/developed/cms/psil/inc \
              -I $(MW_HOST_DIRS)/developed/cms/psstk/inc \
              -I $(MW_HOST_DIRS)/developed/cms/sockmgr/inc \
              -I $(MW_HOST_DIRS)/developed/cms/cmsnetlight/inc \
              -I $(MW_HOST_DIRS)/developed/ecapi/aal/inc        \
              -I $(MW_HOST_DIRS)/developed/ecapi/appmwapi/inc \
              -I $(MW_HOST_DIRS)/developed/ecapi/psapi/inc \
              -I $(MW_HOST_DIRS)/developed/simbip/inc        \
              -I $(MW_HOST_DIRS)/developed/ccio/device/inc        \
              -I $(MW_HOST_DIRS)/developed/ccio/custom/inc        \
              -I $(MW_HOST_DIRS)/developed/ccio/pub        \
              -I $(MW_HOST_DIRS)/developed/ccio/service/inc \
              -I $(MW_HOST_DIRS)/developed/fota/upm/inc \
              -I $(MW_HOST_DIRS)/developed/fota/delta/inc \
              -I $(MW_HOST_DIRS)/developed/imsnv/inc \
              -I $(MW_HOST_DIRS)/thirdparty/lzma2201/C  \
              -I $(MW_HOST_DIRS)/thirdparty/lzma2201/wrapper  \
              -I $(MW_HOST_DIRS)/thirdparty/lzma2201/C/Util/7z  \
              -I $(MW_HOST_DIRS)/thirdparty/lzma2201/C/Util/SfxSetup  \
              -I $(MW_HOST_DIRS)/thirdparty/lwip/src/include  \
              -I $(MW_HOST_DIRS)/thirdparty/lwip/src/include/lwip
			  
LWIPCONFIG_CFLAGS ?= -DLWIP_CONFIG_FILE=\"lwip_config_cat.h\"
CFLAGS += $(LWIPCONFIG_CFLAGS)
			  
ifeq ($(MIDDLEWARE_FOTA_ENABLE), y)
CFLAGS              += -DFEATURE_FOTA_ENABLE -DMIDDLEWARE_FOTA_ENABLE
	
ifeq ($(MIDDLEWARE_FOTA_USBURC_ENABLE), y)
CFLAGS += -DFEATURE_FOTA_USBURC_ENABLE
endif
endif

ifeq ($(MIDDLEWARE_FOTAPAR_ENABLE), y)
CFLAGS_DEFS += -DFEATURE_FOTAPAR_ENABLE
endif

ifeq ($(BUILD_FIXED_AUDIO_SOURCE_ENABLE),y)
CFLAGS += -DFEATURE_FIXED_AUDIO_SOURCE_ENABLE
endif

ifeq ($(FOTA_BUILD), y)
ifeq ($(TOOLCHAIN),GCC)
CFLAGS_INC += -I $(TOP)/PLAT/os/freertos/portable/gcc
endif

endif