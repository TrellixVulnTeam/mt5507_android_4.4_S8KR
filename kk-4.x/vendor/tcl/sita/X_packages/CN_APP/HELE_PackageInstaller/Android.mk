ifeq "$(Hele_tMedia_project)" "true"
$(info	"build hele PackageInstaller")
LOCAL_PATH :=$(call my-dir)
include $(CLEAR_VARS)

# Module name should match apk name to be installed
LOCAL_MODULE := HELE_PackageInstaller
LOCAL_SRC_FILES := PackageInstaller.apk
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_SUFFIX:=$(COMMON_ANDROID_PACKAGE_SUFFIX)

#choose apk's location 
LOCAL_MODULE_PATH := $(TARGET_OUT_APPS)
LOCAL_CERTIFICATE:=PRESIGNED
LOCAL_MODULE_TAGS := optional
include $(BUILD_PREBUILT)

else
$(warning  "do not need to build hele PackageInstaller")
endif

