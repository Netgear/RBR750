# Generated automatically from Makefile.in by configure.

# pling added 04/27/2009
include ../config.mk
include ../config.in

VERSION=1.6.6
CFLAGS += -O2 -DHAVE_CONFIG_H -Wall -I. -DVERSION=\"$(VERSION)\" -D_LARGEFILE_SOURCE -DQUICK_FIX_ISSUES -ULEAN
all: bftpd
LIBS= -lcrypt

HEADERS=bftpdutmp.h commands.h commands_admin.h cwd.h dirlist.h list.h login.h logging.h main.h mystring.h options.h targzip.h mypaths.h
OBJS=bftpdutmp.o commands.o commands_admin.o cwd.o dirlist.o list.o login.o logging.o main.o mystring.o options.o
SRCS=bftpdutmp.c commands.c commands_admin.c cwd.c dirlist.c list.c login.c logging.c main.c mystring.c options.c


LDFLAGS=-Wl,-allow-shlib-undefined
ifeq ($(CONFIG_XDSL_PRODUCT),y)
LDFLAGS	+= -L$(ACOSTOPDIR)/nvram -L$(INSTALL_DIR)/lib -lnvram 
LDFLAGS += -L$(ACOSTOPDIR)/shared -L$(INSTALL_DIR)/lib -lacos_shared
LDFLAGS	+= -L$(ACOSTOPDIR)/nvram -L$(INSTALL_DIR)/nvram/usr/lib -lnvram
CFLAGS	+= -I. -I$(ACOSTOPDIR)/include
CFLAGS += -D_XDSL_PRODUCT
else
CFLAGS  += -I. -I$(ACOSTOPDIR)/shared -I$(ACOSTOPDIR)/include -Wall
CFLAGS  += -I../../acos/include
LDFLAGS += -L$(ACOSTOPDIR)/nvram -L$(INSTALLDIR)/nvram/usr/lib -lnvram
endif

bftpd: $(OBJS)
	./mksources $(DIRPAX)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o bftpd

install:
	$(STRIP) bftpd
	#install -m 755 bftpd $(TARGETDIR)/bin
	install -m 755 bftpd $(TARGETDIR)/usr/sbin
	#$(STRIP) $(TARGETDIR)/bin/bftpd
	#cp -af bftpd.conf $(INSTALL_DIR)/etc

clean:
	-rm -f bftpd $(OBJS) mksources.finished config.cache
	rm -f $(TARGETDIR)/bin/bftpd
	#rm -f $(TARGETDIR)/etc/bftpd.conf

