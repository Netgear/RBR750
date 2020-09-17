
#***************************************************************************
#***
#***    Copyright 2005-2019  Hon Hai Precision Ind. Co. Ltd.
#***    All Rights Reserved.
#***    No portions of this material shall be reproduced in any form without the
#***    written permission of Hon Hai Precision Ind. Co. Ltd.
#***
#***    All information contained in this document is Hon Hai Precision Ind.  
#***    Co. Ltd. company private, proprietary, and trade secret property and 
#***    are protected by international intellectual property laws and treaties.
#***
#****************************************************************************
#       2019/06/03 Harry created. This file is used for replacing the
#       AMBIT_UI_VERSION in ambitCfg.h. The AMBIT_UI_VERSION will read
#       this files as teh ODM version.
#
#


include ../config.mk
include ../config.in

all: install

install:
	mkdir -p $(TARGETDIR)/etc/
	install -m 644 odm_version $(TARGETDIR)/etc/

clean:
