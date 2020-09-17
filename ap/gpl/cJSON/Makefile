include ../config.mk

SRCDIR := cJSON_r73_021315

LIBPNG_DIR_PATH=$(shell pwd)

#CROSS_COMPILE=arm-uclibc-linux-2.6.36-

all: lib_src

lib_src:	
	for i in ${SRCDIR}; do \
		(cd $$i; make install CROSS=$(CROSS_COMPILE)) || exit 1; \
	done
		 
clean:
	cd $(SRCDIR); \
	for i in ${SRCDIR}; do \
		(cd $$i; make clean); \
	done
	rm -rf $(TARGETDIR)/usr/lib/libcjson.*

install:
	install -m 755 $(LIBPNG_DIR_PATH)/$(SRCDIR)/libcjson.so  $(TARGETDIR)/usr/lib

