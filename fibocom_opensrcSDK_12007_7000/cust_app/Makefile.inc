#*****************************************************************
# Copyright (C), 2023, Xian Fibocom Wireless Inc
#     All rights reserved.                  
#     FileName    :Makefile.inc
#     Author      : taric.wang
#     Created     :2023-02-22
#     Description :Makefile.inc
#****************************************************************/


CUST_APP_DIRS := $(TOP)/cust_app
$(info @@@@@@@@@@@@@@CFLAGS "$(CFLAGS)" !!!!!!)      
CFLAGS_INC    += -I  $(TOP)/cust_app/include     
CFLAGS_INC    += -I  $(TOP)/cust_app/app_demo 
CFLAGS_INC    += -I  $(TOP)/middleware/thirdparty
CFLAGS_INC    += -I  $(TOP)/middleware/thirdparty/mbedtls/include
CFLAGS_INC    += -I  $(TOP)/middleware/developed/fota/pub

	
ifeq ($(TOOLCHAIN),GCC)
CFLAGS += -DconfigUSE_NEWLIB_REENTRANT=1
LDFLAGS += -Wl,--wrap=_malloc_r -Wl,--wrap=_free_r -Wl,--wrap=_realloc_r
endif

CUSTAPP_SRC             +=  $(wildcard $(TOP)/cust_app/src/*.c)
CUSTAPP_SRC             +=  $(wildcard $(TOP)/cust_app/app_demo/*.c)
CUSTAPP_SRC             +=  $(wildcard $(TOP)/gccout/oc_export_api.c)
CUSTAPP_COBJSTEMP       := $(patsubst %.c, %.o, $(CUSTAPP_SRC))
CUSTAPP_COBJS           := $(subst $(CUST_APP_DIRS),cust_app,$(CUSTAPP_COBJSTEMP)) 
CUSTAPP_COBJS           := $(subst $(TOP)/gccout,gccout,$(CUSTAPP_COBJS))
CUSTAPP_COBJS           := $(addprefix $(BUILDDIR)/, $(CUSTAPP_COBJS))
CUSTAPP_PPFILES         := $(patsubst %.o, %.pp, $(CUSTAPP_COBJS))
PPFILES                 += $(CUSTAPP_PPFILES)


ifneq ($(MAKECMDGOALS), unilog)
-include $(CUST_COBJS:.o=.d)
endif
-include $(CUST_COBJS:.o=.dd)
	
ifeq ($(TOOLCHAIN),GCC)
CFLAGS_INC += -I $(TOP)/os/freertos/portable/gcc
endif

lib-y += libcustapp.a

$(BUILDDIR)/lib/libcustapp.a: $(CUSTAPP_COBJS) 
	@mkdir -p $(dir $@)
	$(ECHO) AR $@
	$(Q)$(AR) -cr $@ $^

