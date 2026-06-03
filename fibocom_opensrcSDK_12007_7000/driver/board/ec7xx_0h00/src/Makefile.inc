
DRV_CUST_HOST_REL_DIR  := driver/board/$(TARGET)
DRV_CUST_HOST_DIRS  := $(TOP)/$(DRV_CUST_HOST_REL_DIR)

CFLAGS_INC += -I$(TOP)/driver/board/$(TARGET)/inc  \
		-I$(TOP)/driver/board/$(TARGET)/inc/camera \
		-I$(TOP)/driver/board/$(TARGET)/inc/camera/sp0A39 \
		-I$(TOP)/driver/board/$(TARGET)/inc/camera/sp0821 \
		-I$(TOP)/driver/board/$(TARGET)/inc/camera/gc6123 \
		-I$(TOP)/driver/board/$(TARGET)/inc/camera/gc6153 \
		-I$(TOP)/driver/board/$(TARGET)/inc/camera/gc032A \
		-I$(TOP)/driver/board/$(TARGET)/inc/camera/bf30a2 \
		-I$(TOP)/driver/board/$(TARGET)/inc/codec \
		-I$(TOP)/driver/board/$(TARGET)/inc/lcd/lcdDev \
		-I$(TOP)/driver/board/$(TARGET)/inc/lcd \
		-I$(TOP)/driver/board/$(TARGET)/inc/exstorage \
		-I$(TOP)/driver/board/$(TARGET)/inc/fibo_gpio_resource

ifeq ($(CONFIG_FFW_CUST_ETHER_ENABLE), y)
CFLAGS_INC += -I$(TOP)/driver/board/$(TARGET)/inc/ethernet
endif

ifeq ($(TOOLCHAIN),GCC)
#FREERTOS_SRC_DIRS += $(FREERTOS_DIR)/portable/gcc  \
#                     $(FREERTOS_DIR)/portable/mem/tlsf
#CFLAGS += -DconfigUSE_NEWLIB_REENTRANT=1
#LDFLAGS += -Wl,--wrap=_malloc_r -Wl,--wrap=_free_r -Wl,--wrap=_realloc_r
endif

ifeq ($(DRIVER_CAMERA_ENABLE), y)
DRV_CUST_SRC             +=  $(wildcard $(TOP)/driver/board/$(TARGET)/src/camera/*.c)
DRV_CUST_SRC             +=  $(TOP)/driver/board/$(TARGET)/src/camera/gc032A/gc032A.c
DRV_CUST_SRC             +=  $(TOP)/driver/board/$(TARGET)/src/camera/gc6123/gc6123.c
DRV_CUST_SRC             +=  $(TOP)/driver/board/$(TARGET)/src/camera/gc6153/gc6153.c
DRV_CUST_SRC             +=  $(TOP)/driver/board/$(TARGET)/src/camera/sp0A39/sp0A39.c
DRV_CUST_SRC             +=  $(TOP)/driver/board/$(TARGET)/src/camera/bf30a2/bf30a2.c
DRV_CUST_SRC             +=  $(TOP)/driver/board/$(TARGET)/src/camera/sp0821/sp0821.c
endif

#fibocom
ifeq ($(DRIVER_I2S_ENABLE), y)
CFLAGS_INC +=            -I$(TOP)/driver/board/$(TARGET)/inc/codec/codecDev
DRV_CUST_SRC             +=  $(TOP)/driver/board/$(TARGET)/src/codec/fibo_es8311.c
DRV_CUST_SRC             +=  $(TOP)/driver/board/$(TARGET)/src/codec/codecDrv.c
DRV_CUST_SRC             +=  $(TOP)/driver/board/$(TARGET)/src/codec/fibo_aw88266.c
endif

#fibocom
ifeq ($(DRIVER_LCD_ENABLE), y)
#DRV_CUST_SRC             +=  $(wildcard $(TOP)/driver/board/$(TARGET)/src/lcd/lcdDev/lcdDev_7789.c)
#DRV_CUST_SRC             +=  $(wildcard $(TOP)/driver/board/$(TARGET)/src/lcd/lcdDev/lcdDev_7567.c)
ifeq ($(CONFIG_CUST_ONE_LCD), y)
DRV_CUST_SRC             +=  $(wildcard $(TOP)/driver/board/$(TARGET)/src/lcd/lcdDev/lcdDev_7789_cust_one.c)
endif
DRV_CUST_SRC             +=  $(wildcard $(TOP)/driver/board/$(TARGET)/src/lcd/*.c)
endif

ifeq ($(DRIVER_EXSTORAGE_ENABLE), y)
##DRV_CUST_SRC             +=  $(wildcard $(TOP)/driver/board/$(TARGET)/src/exstorage/*.c)
DRV_CUST_SRC             +=  $(TOP)/driver/board/$(TARGET)/src/exstorage/ex_flash.c
#DRV_CUST_SRC             +=  $(TOP)/driver/board/$(TARGET)/src/exstorage/ex_storage.c
endif

ifeq ($(CONFIG_LE230_PROJECT), y)
DRV_CUST_SRC             +=  $(TOP)/driver/board/$(TARGET)/src/fibo_gpio_resource/LE230/fibo_gpio_resource.c
else ifeq ($(CONFIG_LE370_PROJECT), y)
DRV_CUST_SRC             +=  $(TOP)/driver/board/$(TARGET)/src/fibo_gpio_resource/LE370/fibo_gpio_resource.c
else ifeq ($(CONFIG_LE371_PROJECT), y)
DRV_CUST_SRC             +=  $(TOP)/driver/board/$(TARGET)/src/fibo_gpio_resource/LE371/fibo_gpio_resource.c
else ifeq ($(CONFIG_LE250_PROJECT), y)
DRV_CUST_SRC             +=  $(TOP)/driver/board/$(TARGET)/src/fibo_gpio_resource/LE250/fibo_gpio_resource.c
else ifeq ($(CONFIG_LE270_1S3W8_PROJECT), y)
DRV_CUST_SRC             +=  $(TOP)/driver/board/$(TARGET)/src/fibo_gpio_resource/LE270_1S3W8/fibo_gpio_resource.c
else
DRV_CUST_SRC             +=  $(TOP)/driver/board/$(TARGET)/src/fibo_gpio_resource/LE270/fibo_gpio_resource.c
endif

ifeq ($(CONFIG_FFW_CUST_ETHER_ENABLE), y)
DRV_CUST_SRC             +=  $(wildcard $(TOP)/driver/board/$(TARGET)/src/ethernet/*.c)
endif

##FIBOCOM_BASE
ifeq ($(BINNAME),ap_bootloader)
## booterloader no need build softsim code
else
DRV_CUST_SRC             +=  $(TOP)/driver/board/$(TARGET)/src/softsim/ecuiccapi.c
endif
##END

DRV_CUST_COBJSTEMP       := $(patsubst %.c, %.o, $(DRV_CUST_SRC))
DRV_CUST_COBJS           := $(subst $(DRV_CUST_HOST_DIRS),$(DRV_CUST_HOST_REL_DIR),$(DRV_CUST_COBJSTEMP)) 
DRV_CUST_COBJS           := $(addprefix $(BUILDDIR)/, $(DRV_CUST_COBJS))
DRV_CUST_PPFILES         := $(patsubst %.o, %.pp, $(DRV_CUST_COBJS))
PPFILES                  += $(DRV_CUST_PPFILES)

ifneq ($(MAKECMDGOALS), unilog)
-include $(DRV_CUST_COBJS:.o=.d)
endif
-include $(DRV_CUST_COBJS:.o=.dd)
##END	

ifeq ($(TOOLCHAIN),GCC)
CFLAGS_INC += -I $(TOP)/os/freertos/portable/gcc

lib-y += libdrvcust.a

$(BUILDDIR)/lib/libdrvcust.a: $(DRV_CUST_COBJS)
	@mkdir -p $(dir $@)
	$(ECHO) AR $@
	$(Q)$(AR) -cr $@ $^

endif

