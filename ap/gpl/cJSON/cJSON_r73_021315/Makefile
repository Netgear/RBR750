OBJ = cJSON.o
LIBNAME = libcjson

#PREFIX ?= /usr/local
#INCLUDE_PATH ?= include/cjson
#LIBRARY_PATH ?= lib
#CROSS_COMPILE=arm-uclibc-linux-2.6.36-

#INSTALL_INCLUDE_PATH = $(DESTDIR)$(PREFIX)/$(INCLUDE_PATH)
#INSTALL_LIBRARY_PATH = $(DESTDIR)$(PREFIX)/$(LIBRARY_PATH)

#INSTALL ?= cp -a

R_CFLAGS = -fpic $(CFLAGS) -Wall -Werror -Wstrict-prototypes -Wwrite-strings

## shared lib
DYLIBNAME = $(LIBNAME).so 
#DYLIBCMD = $(CROSS_COMPILE)$(CC) -shared -o $(DYLIBNAME)
DYLIBCMD = $(LD) -shared -o $(DYLIBNAME)

STLIBNAME = $(LIBNAME).a

.PHONY: all clean

all: $(DYLIBNAME)

$(DYLIBNAME): $(OBJ)
		$(DYLIBCMD) $< $(LDFLAGS) -lm

$(STLIBNAME): $(OBJ)
		ar rcs $@ $<

$(OBJ): cJSON.c cJSON.h 

.c.o:
#		$(CROSS_COMPILE)$(CC) -ansi -pedantic -c $(R_CFLAGS) $<
		$(CC) -ansi -pedantic -c $(R_CFLAGS) $<

install: all
		install -d $(TARGETDIR)/usr/lib
		install -m 755 $(DYLIBNAME) $(TARGETDIR)/usr/lib
		$(STRIP) $(TARGETDIR)/usr/lib/$(DYLIBNAME)
#		mkdir -p $(INSTALL_LIBRARY_PATH) $(INSTALL_INCLUDE_PATH)
#		$(INSTALL) cJSON.h $(INSTALL_INCLUDE_PATH)
#		$(INSTALL) $(DYLIBNAME) $(INSTALL_LIBRARY_PATH)

clean: 
		rm -rf $(DYLIBNAME) $(STLIBNAME) *.o
