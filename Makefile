#
# (C) Copyright 2000-2015
# yexf:yexf_fyf@163.com 2015/9/8
#
####################### wxf sd loader for TQ210 #########################
# 为TQ210开发板设计的从sd卡启动loader 
# s5pv210的bl1启动程序
# 
################################ bl2 过程   #################################	
# 从sd卡的跟路径下找wboot.ini文件
# 如果没有找到,打印出"Can't find wboot.ini form SD!"
# 如果找到,则根据wboot.ini文件的配置信息把bl2加载到指定位置
#
# wboot.ini文件格式:
# mount=/
# boot=/wxfbl2.bin
# default=0x20000000
# isimg=false
#
#########################################################################

VERSION = 1
PATCHLEVEL = 0
SUBLEVEL = 1
EXTRAVERSION =
WXF_SDLOADER_VERSION = $(VERSION).$(PATCHLEVEL).$(SUBLEVEL)$(EXTRAVERSION)
VERSION_FILE = $(obj)include/version_autogenerated.h

HOSTARCH := $(shell uname -m | \
	sed -e s/i.86/i386/ \
	    -e s/sun4u/sparc64/ \
	    -e s/arm.*/arm/ \
	    -e s/sa110/arm/ \
	    -e s/powerpc/ppc/ \
	    -e s/ppc64/ppc/ \
	    -e s/macppc/ppc/)

HOSTOS := $(shell uname -s | tr '[:upper:]' '[:lower:]' | \
	    sed -e 's/\(cygwin\).*/cygwin/')

export	HOSTARCH HOSTOS

# Deal with colliding definitions from tcsh etc.
VENDOR=

#########################################################################
# Allow for silent builds
ifeq (,$(findstring s,$(MAKEFLAGS)))
XECHO = echo
else
XECHO = :
endif

#########################################################################
#
# wxfld build supports producing a object files to the separate external
# directory. Two use cases are supported:
#
# 1) Add O= to the make command line
# 'make O=/tmp/build all'
#
# 2) Set environement variable BUILD_DIR to point to the desired location
# 'export BUILD_DIR=/tmp/build'
# 'make'
#
# Command line 'O=' setting overrides BUILD_DIR environent variable.
#
# When none of the above methods is used the local build is performed and
# the object files are placed in the source directory.
#

ifdef O
ifeq ("$(origin O)", "command line")
BUILD_DIR := $(O)
endif
endif

ifneq ($(BUILD_DIR),)
saved-output := $(BUILD_DIR)

# Attempt to create a output directory.
$(shell [ -d ${BUILD_DIR} ] || mkdir -p ${BUILD_DIR})

# Verify if it was successful.
BUILD_DIR := $(shell cd $(BUILD_DIR) && /bin/pwd)
$(if $(BUILD_DIR),,$(error output directory "$(saved-output)" does not exist))
endif # ifneq ($(BUILD_DIR),)

OBJTREE		:= $(if $(BUILD_DIR),$(BUILD_DIR),$(CURDIR))
SRCTREE		:= $(CURDIR)
TOPDIR		:= $(SRCTREE)
LNDIR		:= $(OBJTREE)
export	TOPDIR SRCTREE OBJTREE

ifneq ($(OBJTREE),$(SRCTREE))
REMOTE_BUILD	:= 1
export REMOTE_BUILD
endif

# $(obj) and (src) are defined in config.mk but here in main Makefile
# we also need them before config.mk is included which is the case for
# some targets like unconfig, clean, clobber, distclean, etc.
ifneq ($(OBJTREE),$(SRCTREE))
obj := $(OBJTREE)/
src := $(SRCTREE)/
else
obj :=
src :=
endif
export obj src

# Make sure CDPATH settings don't interfere
unexport CDPATH

ARCH   = arm
CPU    = s5pv210
BOARD  = TQ210
VENDOR = WXF
SOC    = s5pv210

ifndef CROSS_COMPILE
CROSS_COMPILE = arm-linux-
endif	# CROSS_COMPILE

export	ARCH CPU BOARD VENDOR SOC CROSS_COMPILE

# load other configuration
include $(TOPDIR)/config.mk


#############################################################################
#############################################################################

OBJS  = board/$(BOARDDIR)/start.o
OBJS := $(addprefix $(obj),$(OBJS))

LIBS = lib_$(ARCH)/lib$(ARCH).a
LIBS += common/libcommon.a
LIBS += lib_generic/libgeneric.a

LIBS := $(addprefix $(obj),$(LIBS))
.PHONY : $(LIBS) $(VERSION_FILE)

LIBBOARD = board/$(BOARDDIR)/lib$(BOARD).a
LIBBOARD := $(addprefix $(obj),$(LIBBOARD))

# Add GCC lib
PLATFORM_LIBS += -L $(shell dirname `$(CC) $(CFLAGS) -print-libgcc-file-name`) -lgcc

__OBJS := $(subst $(obj),,$(OBJS))
__LIBS := $(subst $(obj),,$(LIBS)) $(subst $(obj),,$(LIBBOARD))

#########################################################################
#########################################################################

ALL += $(obj)wxfld.srec $(obj)wxfld.bin $(obj)System.map $(obj)wxfld.dis

all:		$(ALL)

$(obj)wxfld.dis:	$(obj)wxfld
		$(OBJDUMP) -d $< > $@

$(obj)wxfld.srec:	$(obj)wxfld
		$(OBJCOPY) ${OBJCFLAGS} -O srec $< $@

$(obj)wxfld.bin:	$(obj)wxfld
		$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@
		
$(obj)System.map:	$(obj)wxfld
		@$(NM) $< | \
		grep -v '\(compiled\)\|\(\.o$$\)\|\( [aUw] \)\|\(\.\.ng$$\)\|\(LASH[RL]DI\)' | \
		sort > $(obj)System.map
	
$(obj)wxfld:		depend $(OBJS) $(LIBBOARD) $(LIBS) $(LDSCRIPT)
		UNDEF_SYM=`$(OBJDUMP) -x $(LIBBOARD) $(LIBS) | \
		sed  -n -e 's/.*\($(SYM_PREFIX)__u_boot_cmd_.*\)/-u\1/p'|sort|uniq`;\
		cd $(LNDIR) && $(LD) $(LDFLAGS) $$UNDEF_SYM $(__OBJS) \
			--start-group $(__LIBS) --end-group $(PLATFORM_LIBS) \
			-Map wxfld.map -o $@

$(OBJS):	depend $(obj)include/autoconf.mk
		$(MAKE) -C $(if $(REMOTE_BUILD),$@,$(dir $@)) $(if $(REMOTE_BUILD),$@,$(notdir $@))

$(LIBS):	depend $(obj)include/autoconf.mk
		$(MAKE) -C $(dir $(subst $(obj),,$@))

$(LIBBOARD):	depend $(LIBS) $(obj)include/autoconf.mk
		$(MAKE) -C $(dir $(subst $(obj),,$@))

$(LDSCRIPT):	depend $(obj)include/autoconf.mk
		$(MAKE) -C $(dir $@) $(notdir $@)
		
#
# Auto-generate the autoconf.mk file (which is included by all makefiles)
#
# This target actually generates 2 files; autoconf.mk and autoconf.mk.dep.
# the dep file is only include in this top level makefile to determine when
# to regenerate the autoconf.mk file.
$(obj)include/autoconf.mk.dep: $(obj)include/config.h include/common.h
	$(XECHO) Generating $@ ; \
	set -e ; \
	: Generate the dependancies ; \
	$(CC) -x c -DDO_DEPS_ONLY -M $(HOST_CFLAGS) $(CPPFLAGS) \
		-MQ $(obj)include/autoconf.mk include/common.h > $@

$(obj)include/autoconf.mk: $(obj)include/config.h
	$(XECHO) Generating $@ ; \
	set -e ; \
	: Extract the config macros ; \
	$(CPP) $(CFLAGS) -DDO_DEPS_ONLY -dM include/common.h | \
		sed -n -f tools/scripts/define2mk.sed > $@

$(VERSION_FILE):
		@( printf '#define U_BOOT_VERSION "U-Boot %s%s"\n' "$(U_BOOT_VERSION)" \
		 '$(shell $(CONFIG_SHELL) $(TOPDIR)/tools/setlocalversion $(TOPDIR))' \
		 ) > $@.tmp
		@cmp -s $@ $@.tmp && rm -f $@.tmp || mv -f $@.tmp $@
		
sinclude $(obj)include/autoconf.mk.dep		

depend dep:	$(VERSION_FILE)

#########################################################################
#########################################################################
#########################################################################

clean:
	@find $(OBJTREE) -type f \
		\( -name 'core' -o -name '*.bak' -o -name '*~' \
		-o -name '*.o'	-o -name '*.a'	\) -print \
		| xargs rm -f
	@rm -f $(ALL) wxfld wxfld.map
	@find -name '.depend' | xargs rm -f

#########################################################################