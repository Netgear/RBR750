# Set up paths and environment for cross compiling for openwrt
# Shell script location
BASEDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/../../" && pwd )"
echo "Shell script location: ${BASEDIR}"

# reference qsdk/rules.mk
TARGET_DIR_NAME=target-arm_cortex-a7_musl-1.1.16_eabi
TOOLCHAIN_DIR_NAME=toolchain-arm_cortex-a7_gcc-5.2.0_musl-1.1.16_eabi

# Set the TOOLCHAIN_DIR environment variable to the above toolchain dir and export it:
STAGING_DIR=${BASEDIR}/staging_dir/${TARGET_DIR_NAME}
TOOLCHAIN_DIR=${BASEDIR}/staging_dir/${TOOLCHAIN_DIR_NAME}
export STAGING_DIR
export TARGET_DIR_NAME
export PATH=${TOOLCHAIN_DIR}/bin:$PATH
