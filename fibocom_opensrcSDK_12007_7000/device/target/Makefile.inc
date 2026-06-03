ifeq ($(TOOLCHAIN),GCC)
PRECINIT_FILE_PATH ?= device/target/board/$(TARGET)/$(CORE)/gcc
SYSCALLS_FILE_PATH ?= device/target/board/$(TARGET)/$(CORE)/gcc
SYSTEM_FILE_PATH   ?= device/target/board/$(TARGET)/$(CORE)/src
STARTUP_FILE_PATH  ?= device/target/board/$(TARGET)/$(CORE)/gcc
LINK_FILE_PATH     ?= device/target/board/$(TARGET)/$(CORE)/gcc
endif

ifeq ($(TOOLCHAIN),ARMCC)
PRECINIT_FILE_PATH ?= device/target/board/$(TARGET)/$(CORE)/keil
SYSTEM_FILE_PATH   ?= device/target/board/$(TARGET)/$(CORE)/src
STARTUP_FILE_PATH  ?= device/target/board/$(TARGET)/$(CORE)/keil
LINK_FILE_PATH     ?= device/target/board/$(TARGET)/$(CORE)/keil
endif

##$(error "error  $(MAIN_FILE_PATH)")
CFLAGS_INC += -I $(TOP)/device/target/board/common/ARMCM3/inc  \
              -I $(TOP)/device/target/board/$(TARGET)/common/inc \
              -I $(TOP)/device/target/board/$(TARGET)/common/pkginc
  
#FIBOCOM_BASE modify
STARTUP_DIRS            := $(TOP)/device/target
STARTUP_CFILES          += $(TOP)/device/target/board/$(TARGET)/$(CORE)/src/mem_zone.c
STARTUP_CFILES          += $(TOP)/device/target/board/$(TARGET)/$(CORE)/src/ecm_config.c
STARTUP_CFILES          += $(TOP)/device/target/board/$(TARGET)/$(CORE)/src/fibo_config.c
STARTUP_COBJSTEMP       := $(patsubst %.c, %.o, $(STARTUP_CFILES))
DEVICE_COBJS           := $(subst $(STARTUP_DIRS), device/target,$(STARTUP_COBJSTEMP))
DEVICE_COBJS           := $(addprefix $(BUILDDIR)/, $(DEVICE_COBJS))


ifeq ($(TOOLCHAIN),GCC)

ifeq ($(EC718XM_LD_AND_PREC), y)
libstartup-y := $(PRECINIT_FILE_PATH)/ec718xm/prec_init.o        \
                $(SYSCALLS_FILE_PATH)/syscalls.o         \
                $(SYSTEM_FILE_PATH)/system_$(CHIP).o       \
                $(STARTUP_FILE_PATH)/startup_$(CHIP)_gcc.o
else
libstartup-y := $(PRECINIT_FILE_PATH)/prec_init.o        \
                $(SYSCALLS_FILE_PATH)/syscalls.o         \
                $(SYSTEM_FILE_PATH)/system_$(CHIP).o       \
                $(STARTUP_FILE_PATH)/startup_$(CHIP)_gcc.o
endif

STARTUP_OBJS := $(addprefix $(BUILDDIR)/,$(libstartup-y))


ifneq ($(MAKECMDGOALS), unilog)
-include $(STARTUP_OBJS:.o=.d)
-include $(DEVICE_COBJS:.o=.d)
endif

CFLAGS_INC += -I $(TOP)/device/target/board/$(TARGET)/$(CORE)/gcc \
              -I $(TOP)/device/target/board/$(TARGET)/$(CORE)/inc
lib-y += libstartup.a
#FIBOCOM_BASE modify
$(BUILDDIR)/lib/libstartup.a: $(STARTUP_OBJS) $(DEVICE_COBJS)
	@mkdir -p $(dir $@)
	$(ECHO) AR $@
	$(Q)$(AR) -cr $@ $^


ifeq ($(SOC_DEBUG), y)
LD_FILE ?= $(TOP)/device/target/board/$(TARGET)/$(CORE)/gcc/ec718xm/$(TARGET)_soc.ld
else

ifeq ($(EC718XM_LD_AND_PREC), y)
LD_FILE ?= $(TOP)/device/target/board/$(TARGET)/$(CORE)/gcc/ec718xm/$(TARGET)_flash.ld
else
LD_FILE ?= $(TOP)/device/target/board/$(TARGET)/$(CORE)/gcc/$(TARGET)_flash.ld
endif

endif

endif

ifeq ($(TOOLCHAIN),ARMCC)
libstartup-y += $(PRECINIT_FILE_PATH)/prec_init.o  \
                $(SYSTEM_FILE_PATH)/system_$(CHIP).o \
				$(STARTUP_FILE_PATH)/startup_$(CHIP).o

STARTUP_OBJS := $(addprefix $(BUILDDIR)/,$(libstartup-y))

ifneq ($(MAKECMDGOALS), unilog)
-include $(STARTUP_OBJS:.o=.d)
-include $(DEVICE_COBJS:.o=.d)
endif


##obj-y        += $(STARTUP_FILE_PATH)/startup_$(CHIP).o

CFLAGS_INC += -I $(TOP)/device/target/board/$(TARGET)/$(CORE)/keil \
              -I $(TOP)/device/target/board/$(TARGET)/$(CORE)/inc
lib-y += libstartup.lib

ifeq ($(FPGA_DEBUG),y)
linker-script-y := $(TOP)/$(LINK_FILE_PATH)/$(TARGET)_flash_fpga.sct
else
linker-script-y := $(TOP)/$(LINK_FILE_PATH)/$(TARGET)_flash.sct
endif


$(BUILDDIR)/lib/libstartup.lib: $(STARTUP_OBJS) $(DEVICE_COBJS)
	@mkdir -p $(dir $@)
	$(ECHO) AR $@
	$(Q)$(AR) $(ARFLAGS) $@ $^

endif


