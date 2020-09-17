#
# Atheros image packet Makefile
#
#***************************************************************************
#***
#***    Copyright 2005  Hon Hai Precision Ind. Co. Ltd.
#***    All Rights Reserved.
#***    No portions of this material shall be reproduced in any form without the
#***    written permission of Hon Hai Precision Ind. Co. Ltd.
#***
#***    All information contained in this document is Hon Hai Precision Ind.  
#***    Co. Ltd. company private, proprietary, and trade secret property and 
#***    are protected by international intellectual property laws and treaties.
#***
#****************************************************************************
TARGET = packet

CC = gcc

OBJS = ap51_packet.c ap51_main.c
LIBS =
#INSTALL_PATH = ../../src/kernels/mips-linux-2.4.25/install
#CFLAGS += -I /usr/src/linux-2.4/include -I$(INSTALL_PATH) -D__MIPSEL__ -DPTRSIZE=4

#$(TARGET): $(OBJS)
$(TARGET):

install:

# include last so TARGET rule is default (else pickup rule from
# Rules.mak first).
include Rules.mak
