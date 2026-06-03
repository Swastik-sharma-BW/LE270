
ifeq ($(TYPE), ec716e)
CFLAGS_INC += -I$(TOP)/driver/board/$(TARGET)/inc       \
              -I$(TOP)/driver/chip/$(CHIP)/$(CORE)/inc \
			  -I$(TOP)/driver/chip/$(CHIP)/$(CORE)/inc_cmsis \
              -I$(TOP)/driver/chip/$(CHIP)/$(CORE)/inc/ec716 \
			  -I$(TOP)/driver/hal/common/inc \
              -I$(TOP)/driver/hal/$(CHIP)/$(CORE)/inc \
			  -I$(TOP)/driver/hal/$(CHIP)/$(CORE)/inc/ec716 \
              -I$(TOP)/driver/hal/$(CHIP)/common/inc
else ifeq ($(TYPE), ec716s)
CFLAGS_INC += -I$(TOP)/driver/board/$(TARGET)/inc       \
              -I$(TOP)/driver/chip/$(CHIP)/$(CORE)/inc \
			  -I$(TOP)/driver/chip/$(CHIP)/$(CORE)/inc_cmsis \
              -I$(TOP)/driver/chip/$(CHIP)/$(CORE)/inc/ec716 \
			  -I$(TOP)/driver/hal/common/inc \
              -I$(TOP)/driver/hal/$(CHIP)/$(CORE)/inc \
			  -I$(TOP)/driver/hal/$(CHIP)/$(CORE)/inc/ec716 \
              -I$(TOP)/driver/hal/$(CHIP)/common/inc
else
$(info CFLAGS_INC: ec718)
CFLAGS_INC += -I$(TOP)/driver/board/$(TARGET)/inc       \
              -I$(TOP)/driver/chip/$(CHIP)/$(CORE)/inc \
			  -I$(TOP)/driver/chip/$(CHIP)/$(CORE)/inc_cmsis \
              -I$(TOP)/driver/chip/$(CHIP)/$(CORE)/inc/ec718 \
			  -I$(TOP)/driver/hal/common/inc \
              -I$(TOP)/driver/hal/$(CHIP)/$(CORE)/inc \
			  -I$(TOP)/driver/hal/$(CHIP)/$(CORE)/inc/ec718 \
              -I$(TOP)/driver/hal/$(CHIP)/common/inc
endif

			  
ifeq ($(CORE),ap)
include $(TOP)/driver/board/$(TARGET)/src/Makefile.inc

ifeq ($(DRIVER_PSRAM_ENABLE),y)
    CFLAGS      += -DPSRAM_FEATURE_ENABLE
endif

endif
