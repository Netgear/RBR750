#!/bin/sh

usage() {
        cat <<EOF
Usage: $0 [rbr850|rbs850|cleanall]

Example:
 cbr750      : Build Orbi AX 64-bit OAK: CBR750
 cbr750 cable: Build Orbi AX 64-bit OAK: CBR750 CABLE WAN
 rbr750      : Build Orbi AX 64-bit HE V2: RBR750
 rbr750_32   : Build Orbi AX 32-bit: RBR750
 rbs750      : Build Orbi AX 64-bit HE V2: RBS750
 rbs750_32   : Build Orbi AX 32-bit: RBS750
 rbr850      : Build Orbi AX 64-bit HE V2: RBR850
 rbr850_32   : Build Orbi AX 32-bit: RBR850
 rbs850      : Build Orbi AX 64-bit HE V2: RBS850
 rbs850_32   : Build Orbi AX 32-bit: RBS850
 cleanall    : Removed any Build: first_build feeds ipq4019 ipq8074 etc.
EOF

}

BASEDIR="$( cd "$( dirname "$0" )" && pwd )"
PRODUCT=$(echo $1 | tr '[:lower:]' '[:upper:]' | cut -d "_" -f1)
PRODUCT_LOWER=$(echo $1 | cut -d "_" -f1)
NTGR_CMN_DIR=$BASEDIR/../netgear/common

FXCN_CMN_DIR=$BASEDIR/../fxcn/common
FXCN_PRODUCT_DTS_DIR=$BASEDIR/project/product/$PRODUCT/qca
FXCN_PROJECT_DIR=$BASEDIR/../fxcn/project
FXCN_PRODUCT_DIR=$BASEDIR/../fxcn/project/$PRODUCT
NTGR_SECURE_DIR=$BASEDIR/../netgear/secure-boot

copy_netgear_change(){
    if [ "$PRODUCT_LOWER" = "rbr850" ] || [ "$PRODUCT_LOWER" = "rbs850" ] ; then
        NTGR_CORE_DIR=$BASEDIR/../netgear/rbk850/core_dir
        NTGR_CMN_RBK_DIR=$BASEDIR/../netgear/rbk850/cmn_dir
    elif [ "$PRODUCT_LOWER" = "rbr750" ] || [ "$PRODUCT_LOWER" = "rbs750" ] || [ "$PRODUCT_LOWER" = "cbr750" ]; then
        NTGR_CORE_DIR=$BASEDIR/../netgear/rbk750/core_dir
        NTGR_CMN_RBK_DIR=$BASEDIR/../netgear/rbk750/cmn_dir
    fi
    #Copy the complete contents of cmn_dir of each platform
    cp -rf $NTGR_CMN_RBK_DIR/* $BASEDIR/
    cp -rf $NTGR_CMN_DIR/* $BASEDIR/
    cp -rf $NTGR_CORE_DIR/* $BASEDIR/
    #Add changes to update files for cbr750
    if [ "$PRODUCT_LOWER" = "cbr750" ]; then
        cp -rf $BASEDIR/../netgear/cbr750/core_dir/* $BASEDIR/
        cp -rf $BASEDIR/../netgear/cbr750/cmn_dir/* $BASEDIR/
    fi
}

copy_foxconn_change(){
    cp -rf $FXCN_CMN_DIR/* $BASEDIR/
    cp -rf $FXCN_PRODUCT_DTS_DIR/* $BASEDIR/qca/
}

copy_secure_boot_files()
{
	cp $NTGR_SECURE_DIR/cmd_source.c $BASEDIR/qca/src/u-boot-2016/common
        cp $NTGR_SECURE_DIR/scm.c $BASEDIR/qca/src/u-boot-2016/arch/arm/cpu/armv7/qca/common
        cp $NTGR_SECURE_DIR/mbn.h $BASEDIR/qca/src/u-boot-2016/include
        cp $NTGR_SECURE_DIR/Makefile $BASEDIR/qca/feeds/bootloader/uboot-qca
        cp $NTGR_SECURE_DIR/Config-kernel.in $BASEDIR/config/
}

check_feeds(){
if [ ! -e first_feeds ]; then
    
    echo "First feeds update&install...."
    ./scripts/feeds update -a
    ./scripts/feeds install -a -f
    touch first_feeds
fi
}

cbr750(){
echo "************************************************************"
echo "Cable OrbiAX 64-bit OAK"
echo "************************************************************"

if [ ! -e first_build ]; then
    echo "First build All..."
    fxcn_init
    copy_netgear_change
    check_feeds
    copy_foxconn_change
    cp project/product/$PRODUCT/configs/.config .config
    echo "CONFIG_PACKAGE_nvram_uci=y" >> .config
	ln -sf project/product/$PRODUCT/acos_config acos_config
	ln -sf project/product/$PRODUCT/acos_Makefile acos_Makefile
    make -j8
    if [ $? -ne 0 ] ; then echo "make failed!!!";exit 1 ; fi
    touch first_build
else 
    echo "acos build all ..."	
fi

if [ "$1" = "cable" ]; then
    sed -i 's/CONFIG_CABLE_WAN=n/CONFIG_CABLE_WAN=y/g' project/product/CBR750/acos.mk
    sed -i 's/CONFIG_CABLE_WAN=n/CONFIG_CABLE_WAN=y/g' project/product/CBR750/gpl.mk
else
    sed -i 's/CONFIG_CABLE_WAN=y/CONFIG_CABLE_WAN=n/g' project/product/CBR750/acos.mk
    sed -i 's/CONFIG_CABLE_WAN=y/CONFIG_CABLE_WAN=n/g' project/product/CBR750/gpl.mk
fi

source ./project/scripts/acos_env64.sh
make -f acos_Makefile PROFILE=CBR750 && make -f acos_Makefile PROFILE=CBR750 install

/bin/bash ./build_FIT_image.sh cbr750
}

rbr750_32(){
echo "************************************************************"
echo "orbiax 32-bit"
echo "************************************************************"

check_feeds

if [ ! -e first_build ]; then
    echo "First build All..."
    copy_netgear_change
    copy_foxconn_change
    copy_secure_boot_files
    cp project/product/$PRODUCT/configs/.config.x32 .config
    echo "CONFIG_SECUREBOOT=y" >> .config
    make -j8
    if [ $? -ne 0 ] ; then echo "make failed!!!";exit 1 ; fi
    touch first_build
fi

./build_FIT_image.sh rbr750_32
}

rbr750(){
echo "************************************************************"
echo "orbiax 64-bit"
echo "************************************************************"


if [ ! -e first_build ]; then
    echo "First build All..."
    fxcn_init
    copy_netgear_change
    check_feeds
    copy_foxconn_change
    cp project/product/$PRODUCT/configs/.config .config
    echo "CONFIG_PACKAGE_nvram_uci=y" >> .config
	ln -sf project/product/$PRODUCT/acos_config acos_config
	ln -sf project/product/$PRODUCT/acos_Makefile acos_Makefile
    make -j8
    if [ $? -ne 0 ] ; then echo "make failed!!!";exit 1 ; fi
    touch first_build
else 
    echo "acos build all ..."	
fi

source ./project/scripts/acos_env64.sh
make -f acos_Makefile PROFILE=RBR750 && make -f acos_Makefile PROFILE=RBR750 install
#Adding /bin/bash to avoid build errors
/bin/bash ./build_FIT_image.sh rbr750
}

rbs750_32(){
echo "************************************************************"
echo "orbiax 32-bit"
echo "************************************************************"

check_feeds

if [ ! -e first_build ]; then
    echo "First build All..."
    copy_netgear_change
    copy_foxconn_change
    copy_secure_boot_files
    cp project/product/$PRODUCT/configs/.config.x32 .config
    echo "CONFIG_SECUREBOOT=y" >> .config
    make -j8
    if [ $? -ne 0 ] ; then echo "make failed!!!";exit 1 ; fi
    touch first_build
fi

./build_FIT_image.sh rbs750_32
}

rbs750(){
echo "************************************************************"
echo "orbiax 64-bit"
echo "************************************************************"


if [ ! -e first_build ]; then
    echo "First build All..."
    fxcn_init
    copy_netgear_change
    check_feeds
    copy_foxconn_change
    cp project/product/$PRODUCT/configs/.config .config
    echo "CONFIG_PACKAGE_nvram_uci=y" >> .config
	ln -sf project/product/$PRODUCT/acos_config acos_config
	ln -sf project/product/$PRODUCT/acos_Makefile acos_Makefile
    make -j8
    if [ $? -ne 0 ] ; then echo "make failed!!!";exit 1 ; fi
    touch first_build
fi

source ./project/scripts/acos_env64.sh
make -f acos_Makefile PROFILE=RBS750 && make -f acos_Makefile PROFILE=RBS750 install
#Adding /bin/bash to avoid build errors
/bin/bash ./build_FIT_image.sh rbs750
}


rbr850_32(){
echo "************************************************************"
echo "orbiax 32-bit"
echo "************************************************************"

check_feeds

if [ ! -e first_build ]; then
    echo "First build All..."
    copy_netgear_change
    copy_foxconn_change
    copy_secure_boot_files
    cp project/product/$PRODUCT/configs/.config.x32 .config
    echo "CONFIG_SECUREBOOT=y" >> .config
    make -j8
    if [ $? -ne 0 ] ; then echo "make failed!!!";exit 1 ; fi
    touch first_build
fi

./build_FIT_image.sh rbr850_32
}

rbr850(){
echo "************************************************************"
echo "orbiax 64-bit"
echo "************************************************************"


if [ ! -e first_build ]; then
    echo "First build All..."
    fxcn_init
    copy_netgear_change
    check_feeds
    copy_foxconn_change
    cp project/product/$PRODUCT/configs/.config .config
    echo "CONFIG_PACKAGE_nvram_uci=y" >> .config
	ln -sf project/product/$PRODUCT/acos_config acos_config
	ln -sf project/product/$PRODUCT/acos_Makefile acos_Makefile
    exit 0
    make -j8
    if [ $? -ne 0 ] ; then echo "make failed!!!";exit 1 ; fi
    touch first_build
else 
    echo "acos build all ..."	
fi

source ./project/scripts/acos_env64.sh
make -f acos_Makefile PROFILE=RBR850 && make -f acos_Makefile PROFILE=RBR850 install
#Adding /bin/bash to avoid build errors
/bin/bash ./build_FIT_image.sh rbr850
}

rbs850_32(){
echo "************************************************************"
echo "orbiax 32-bit"
echo "************************************************************"

check_feeds

if [ ! -e first_build ]; then
    echo "First build All..."
    copy_netgear_change
    copy_foxconn_change
    copy_secure_boot_files
    cp project/product/$PRODUCT/configs/.config.x32 .config
    echo "CONFIG_SECUREBOOT=y" >> .config
    make -j8
    if [ $? -ne 0 ] ; then echo "make failed!!!";exit 1 ; fi
    touch first_build
fi

./build_FIT_image.sh rbs850_32
}

rbs850(){
echo "************************************************************"
echo "orbiax 64-bit"
echo "************************************************************"


if [ ! -e first_build ]; then
    echo "First build All..."
    fxcn_init
    copy_netgear_change
    check_feeds
    copy_foxconn_change
    cp project/product/$PRODUCT/configs/.config .config
    echo "CONFIG_PACKAGE_nvram_uci=y" >> .config
	ln -sf project/product/$PRODUCT/acos_config acos_config
	ln -sf project/product/$PRODUCT/acos_Makefile acos_Makefile
    make -j8
    if [ $? -ne 0 ] ; then echo "make failed!!!";exit 1 ; fi
    touch first_build
fi

source ./project/scripts/acos_env64.sh
make -f acos_Makefile PROFILE=RBS850 && make -f acos_Makefile PROFILE=RBS850 install
#Adding /bin/bash to avoid build errors
/bin/bash ./build_FIT_image.sh rbs850
}

cleanall(){
echo "************************************************************"
echo "cleanall "
echo "************************************************************"

echo "TBD"
}

fxcn_init(){
    ln -sf $FXCN_PROJECT_DIR project
    ln -sf project/product/$PRODUCT/ap ap
    ln -sf project/product/$PRODUCT/ap2 ap2
}

case "$1" in
        cbr750) cbr750 "$2";;
        rbr750) rbr750 "$2";;
        rbr750_32) rbr750_32;;
        rbs750) rbs750 "$2";;
        rbs750_32) rbs750_32;;
        rbr850) rbr850 "$2";;
        rbr850_32) rbr850_32;;
        rbs850) rbs850 "$2";;
        rbs850_32) rbs850_32;;
        cleanall) cleanall;;
        --help|help) usage;;
        *) usage;;
esac

