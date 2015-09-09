include $(TOPDIR)/config.mk

LIB	= lib$(BOARD).a

START	= start.o

OBJS	= lowlevel_init.o  cpu_init.o pmic.o interrupts.o cpu.o speed.o

all:	.depend $(START) $(LIB)

$(LIB):	$(OBJS)
	$(AR) crv $@ $(OBJS)

clean:
	rm -f $(SOBJS) $(OBJS)

distclean:	clean
	rm -f $(LIB) core *.bak .depend

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
