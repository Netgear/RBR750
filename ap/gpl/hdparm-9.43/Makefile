# Makefile for hdparm

# DESTDIR is for non root installs (eg packages, NFS) only!
DESTDIR = $(TARGETDIR)

binprefix = 
manprefix = /usr
exec_prefix = $(binprefix)/
sbindir = $(exec_prefix)sbin
mandir = $(manprefix)/share/man
oldmandir = $(manprefix)/man

CC ?= gcc
STRIP ?= strip

CFLAGS := -O2 -W -Wall -Wbad-function-cast -Wcast-align -Wpointer-arith -Wcast-qual -Wshadow -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -fkeep-inline-functions -Wwrite-strings -Waggregate-return -Wnested-externs -Wtrigraphs $(CFLAGS)

LDFLAGS = -s
#LDFLAGS = -s -static
INSTALL = install
INSTALL_DATA = $(INSTALL) -m 644
INSTALL_DIR = $(INSTALL) -m 755 -d
INSTALL_PROGRAM = $(INSTALL)

OBJS = hdparm.o identify.o sgio.o sysfs.o geom.o fallocate.o fibmap.o fwdownload.o dvdspeed.o wdidle3.o

all: hdparm

hdparm: hdparm.h sgio.h $(OBJS)
	$(CC) $(LDFLAGS) -o hdparm $(OBJS)
	$(STRIP) hdparm

hdparm.o:	hdparm.h sgio.h

identify.o:	hdparm.h

dvdspeed.o:     dvdspeed.c

sgio.o: sgio.c sgio.h hdparm.h

fwdownload.o: fwdownload.c sgio.h hdparm.h

install: all hdparm.8
	if [ ! -z $(DESTDIR) ]; then $(INSTALL_DIR) $(DESTDIR) ; fi
	if [ ! -z $(DESTDIR)$(sbindir) ]; then $(INSTALL_DIR) $(DESTDIR)$(sbindir) ; fi
#if [ ! -z $(DESTDIR)$(mandir) ]; then $(INSTALL_DIR) $(DESTDIR)$(mandir) ; fi
#	if [ ! -z $(DESTDIR)$(mandir)/man8/ ]; then $(INSTALL_DIR) $(DESTDIR)$(mandir)/man8/ ; fi
	if [ -f $(DESTDIR)$(sbindir)/hdparm ]; then rm -f $(DESTDIR)$(sbindir)/hdparm ; fi
#	if [ -f $(DESTDIR)$(mandir)/man8/hdparm.8 ]; then rm -f $(DESTDIR)$(mandir)/man8/hdparm.8 ;\
	elif [ -f $(DESTDIR)$(oldmandir)/man8/hdparm.8 ]; then rm -f $(DESTDIR)$(oldmandir)/man8/hdparm.8 ; fi
	$(INSTALL_PROGRAM) -D hdparm $(DESTDIR)$(sbindir)/hdparm
#	if [ -d $(DESTDIR)$(mandir) ]; then $(INSTALL_DATA) -D hdparm.8 $(DESTDIR)$(mandir)/man8/hdparm.8 ;\
	elif [ -d $(DESTDIR)$(oldmandir) ]; then $(INSTALL_DATA) -D hdparm.8 $(DESTDIR)$(oldmandir)/man8/hdparm.8 ; fi

clean:
	-rm -f hdparm $(OBJS) core 2>/dev/null

