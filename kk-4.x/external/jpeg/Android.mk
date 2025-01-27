LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
	jcapimin.c jcapistd.c jccoefct.c jccolor.c jcdctmgr.c jchuff.c \
	jcinit.c jcmainct.c jcmarker.c jcmaster.c jcomapi.c jcparam.c \
	jcphuff.c jcprepct.c jcsample.c jctrans.c jdapimin.c jdapistd.c \
	jdatadst.c jdatasrc.c jdcoefct.c jdcolor.c jddctmgr.c jdhuff.c \
	jdinput.c jdmainct.c jdmarker.c jdmaster.c jdmerge.c jdphuff.c \
	jdpostct.c jdsample.c jdtrans.c jerror.c jfdctflt.c jfdctfst.c \
	jfdctint.c jidctflt.c jidctfst.c jidctint.c jidctred.c jquant1.c \
	jquant2.c jutils.c jmemmgr.c armv6_idct.S \
	jpgdec.c pngdec.c

# include mtimage header files
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../vm_linux/chiling/app_if/mtal/mtal_inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../vm_linux/chiling/app_if/mtal/include

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../chiling/app_if/mtal/mtal_inc \
                    $(LOCAL_PATH)/../../../../chiling/app_if/mtal/include

ifeq "$(SUPPORT_MTK_ANDROID_RM)" "true"
LOCAL_C_INCLUDES += \
    $(ANDROID_BUILD_TOP)/../mediatek/frameworks/MtkResourceManager/client \
    $(ANDROID_BUILD_TOP)/../mediatek/frameworks/MtkResourceManager        \
    $(ANDROID_BUILD_TOP)/../mediatek/frameworks/MtkResourceManager/include
endif

ifeq "$(DRAM768M)" "true"
LOCAL_CFLAGS += -DPIC_HW_SUPPORT
endif


ifeq (,$(TARGET_BUILD_APPS))
# building against master
# use ashmem as libjpeg decoder's backing store
LOCAL_CFLAGS += -DUSE_ANDROID_ASHMEM
LOCAL_SRC_FILES += \
    jmem-ashmem.c
else
# unbundled branch, built against NDK.
LOCAL_SDK_VERSION := 17
# the original android memory manager.
# use sdcard as libjpeg decoder's backing store
LOCAL_SRC_FILES += \
    jmem-android.c
endif

LOCAL_CFLAGS += -DAVOID_TABLES
LOCAL_CFLAGS += -DANDROID_TOOLCHAIN
LOCAL_CFLAGS += -O3 -fstrict-aliasing -fprefetch-loop-arrays
#LOCAL_CFLAGS += -march=armv6j

# enable tile based decode
LOCAL_CFLAGS += -DANDROID_TILE_BASED_DECODE

ifeq ($(TARGET_ARCH_VARIANT),x86-atom)
  LOCAL_CFLAGS += -DANDROID_INTELSSE2_IDCT
  LOCAL_SRC_FILES += jidctintelsse.c
endif

ifeq ($(strip $(TARGET_ARCH)),arm)
  ifeq ($(ARCH_ARM_HAVE_NEON),true)
    #use NEON accelerations
    LOCAL_CFLAGS += -DNV_ARM_NEON
    LOCAL_SRC_FILES += \
        jsimd_arm_neon.S \
        jsimd_neon.c
  else
    # enable armv6 idct assembly
    LOCAL_CFLAGS += -DANDROID_ARMV6_IDCT
  endif
endif

# use mips assembler IDCT implementation if MIPS DSP-ASE is present
ifeq ($(strip $(TARGET_ARCH)),mips)
  ifeq ($(strip $(ARCH_MIPS_HAS_DSP)),true)
  LOCAL_CFLAGS += -DANDROID_MIPS_IDCT
  LOCAL_SRC_FILES += \
      mips_jidctfst.c \
      mips_idct_le.S
  endif
endif

LOCAL_MODULE := libjpeg_static

include $(BUILD_STATIC_LIBRARY)



# Build shared library
include $(CLEAR_VARS)

LOCAL_MODULE := libjpeg

LOCAL_MODULE_TAGS := optional

LOCAL_WHOLE_STATIC_LIBRARIES = libjpeg_static

ifeq (,$(TARGET_BUILD_APPS))
LOCAL_SHARED_LIBRARIES := \
    libcutils
else
# unbundled branch, built against NDK.
LOCAL_SDK_VERSION := 17
endif

ifeq "$(SUPPORT_MTK_ANDROID_RM)" "true"
LOCAL_C_INCLUDES += \
    $(ANDROID_BUILD_TOP)/../mediatek/frameworks/MtkResourceManager/client \
    $(ANDROID_BUILD_TOP)/../mediatek/frameworks/MtkResourceManager        \
    $(ANDROID_BUILD_TOP)/../mediatek/frameworks/MtkResourceManager/include 
ifneq "$(RLS_CUSTOM_BUILD)" "true"
LOCAL_SHARED_LIBRARIES += libMtkRmClient
else
LOCAL_LDFLAGS  += -L$(ANDROID_BUILD_TOP)/../mediatek/device/prebuilt/ -lMtkRmClient
endif
endif

DEVICE_STRING = device/$(BRAND)/$(ODM)/$(IC_SETTING)
LOCAL_LDFLAGS += -L$(ANDROID_BUILD_TOP)/$(DEVICE_STRING)/DTV_OUT/shares_libraries \
	-lmtal


include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_ARM_MODE := arm
LOCAL_SRC_FILES := \
	cjpeg.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h jversion.h rdswitch.c cdjpeg.c rdtarga.c rdppm.c rdgif.c rdbmp.c
LOCAL_MODULE:= cjpeg
LOCAL_MODULE_TAGS := eng
LOCAL_SHARED_LIBRARIES := libc libcutils libjpeg
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_ARM_MODE := arm
LOCAL_SRC_FILES := \
	djpeg.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h jversion.h cdjpeg.c wrppm.c wrgif.c wrbmp.c rdcolmap.c wrtarga.c
LOCAL_MODULE:= djpeg
LOCAL_MODULE_TAGS := eng
LOCAL_SHARED_LIBRARIES := libc libcutils libjpeg
include $(BUILD_EXECUTABLE)
