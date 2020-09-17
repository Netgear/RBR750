PROGS     = utelnetd

INSTDIR   = $(TARGETDIR)/usr/sbin
INSTMODE  = 0755
INSTOWNER = root
INSTGROUP = root

OBJS      = utelnetd.o

CC        = $(CROSS_COMPILE)gcc
INSTALL   = install

all: $(PROGS)

CFLAGS   += -DSHELLPATH=\"$(run_prefix)/bin/sh\" -Wall -Os

ifneq ("","$(DEBUG)")
CFLAGS   += -DDEBUG -g
endif

$(PROGS): $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

.PHONY: install
install: $(PROGS)
	rm -f $(INSTDIR)/../../etc/rc.d/S50telnet
	$(INSTALL) -d $(INSTDIR)
	$(INSTALL) -m $(INSTMODE) $(PROGS) $(INSTDIR)
	$(STRIP) $(foreach prog,$(PROGS),$(INSTDIR)/$(prog))

.PHONY: clean
clean:
	rm -f $(PROGS) *.o core
