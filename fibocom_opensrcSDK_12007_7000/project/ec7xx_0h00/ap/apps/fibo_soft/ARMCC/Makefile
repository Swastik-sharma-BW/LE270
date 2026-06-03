
AVAILABLE_TARGETS = ec718_0h00
TOOLCHAIN         = ARMCC
BINNAME           = ap_$(PROJECT)
TOP               := ../../../../../../..

FPGA_DEBUG                  = n
DRIVER_EEPROM_ENABLE        = n
DRIVER_SPI_ENABLE           = n
DRIVER_I2C_ENABLE           = n

THIRDPARTY_MQTT_ENABLE     = n
THIRDPARTY_HTTPC_ENABLE    = y
THIRDPARTY_MBEDTLS_ENABLE  = y
MBEDTLS_WITH_CA = y

THIRDPARTY_CJSON_ENABLE = n
THIRDPARTY_YRCOMPRESS_ENABLE = n
THIRDPARTY_CTCC_DM_ENABLE = y
THIRDPARTY_CUCC_DM_ENABLE = n
THIRDPARTY_CMCC_DM_ENABLE = y
BUILD_AT                    = y
BUILD_AT_DEBUG              = y

THIRDPARTY_FILE_AT_ENABLE   = y
BUILD_EC_MW                 = n
AT_EXAMPLE_ENABLE = n
AT_ADC_ENABLE = y
BUILD_AT_REF   = n
MBEDTLS_WITH_MQTT_TLS = n
BUILD_PS_TCPIP_API_ENABLE  = y


ifeq ($(EUTRAN_MODE), cat_mode)
EUTRAN_CAT_MODE = y
endif

CFLAGS_INC        +=  -I ../inc                   \
                      -I $(TOP)/FIRMWARE/SRC/CAT1/Common/Inc \
                      -I $(TOP)/driver/chip/ec718/ap/src/usb
obj-y             += project/$(TARGET)/$(CORE)/apps/at_command/src/app.o \
                     project/$(TARGET)/$(CORE)/apps/at_command/src/bsp_custom.o

#CONFIG_PROJ_APP_SECURITY_BOOT  = y

include $(TOP)/project/$(TARGET)/$(CORE)/apps/$(PROJECT)/armcc/MakeProj.rules
include $(TOP)/tools/scripts/Makefile.rules

#enable wdt
CFLAGS += -DWDT_FEATURE_ENABLE=1
CFLAGS += --diag_error=warning

CFLAGS += -DFEATURE_AT_COMMAND_PROJECT_ENABLE

ifneq ($(BUILD_AT),y)
$(error This example needs to modify "BUILD_AT" to "y" in device\eigencomm\board\$(TARGET)\ec718_0h00.mk)
endif

