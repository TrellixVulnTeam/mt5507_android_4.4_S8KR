/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "PowerManagerService-JNI"

//#define LOG_NDEBUG 0

#include "JNIHelp.h"
#include "jni.h"

#include <ScopedUtfChars.h>

#include <limits.h>

#include <android_runtime/AndroidRuntime.h>
#include <android_runtime/Log.h>
#include <utils/Timers.h>
#include <utils/misc.h>
#include <utils/String8.h>
#include <utils/Log.h>
#include <hardware/power.h>
#include <hardware_legacy/power.h>
#include <cutils/android_reboot.h>
#include <suspend/autosuspend.h>

#include "mtcommon.h"
#include "mtperipheral.h"

#include "com_android_server_power_PowerManagerService.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <cutils/properties.h>
#include <unistd.h>


namespace android {

// ----------------------------------------------------------------------------

static struct {
    jmethodID wakeUpFromNative;
    jmethodID goToSleepFromNative;
    jmethodID userActivityFromNative;
} gPowerManagerServiceClassInfo;

// ----------------------------------------------------------------------------

static jobject gPowerManagerServiceObj;
static struct power_module* gPowerModule;

static Mutex gPowerManagerLock;
static bool gScreenOn;
static bool gScreenBright;

static nsecs_t gLastEventTime[USER_ACTIVITY_EVENT_LAST + 1];

// Throttling interval for user activity calls.
static const nsecs_t MIN_TIME_BETWEEN_USERACTIVITIES = 500 * 1000000L; // 500ms

// ----------------------------------------------------------------------------

static bool checkAndClearExceptionFromCallback(JNIEnv* env, const char* methodName) {
    if (env->ExceptionCheck()) {
        ALOGE("An exception was thrown by callback '%s'.", methodName);
        LOGE_EX(env);
        env->ExceptionClear();
        return true;
    }
    return false;
}

bool android_server_PowerManagerService_isScreenOn() {
    AutoMutex _l(gPowerManagerLock);
    return gScreenOn;
}

bool android_server_PowerManagerService_isScreenBright() {
    AutoMutex _l(gPowerManagerLock);
    return gScreenBright;
}

void android_server_PowerManagerService_userActivity(nsecs_t eventTime, int32_t eventType) {
    // Tell the power HAL when user activity occurs.
    if (gPowerModule && gPowerModule->powerHint) {
        gPowerModule->powerHint(gPowerModule, POWER_HINT_INTERACTION, NULL);
    }

    if (gPowerManagerServiceObj) {
        // Throttle calls into user activity by event type.
        // We're a little conservative about argument checking here in case the caller
        // passes in bad data which could corrupt system state.
        if (eventType >= 0 && eventType <= USER_ACTIVITY_EVENT_LAST) {
            nsecs_t now = systemTime(SYSTEM_TIME_MONOTONIC);
            if (eventTime > now) {
                eventTime = now;
            }

            if (gLastEventTime[eventType] + MIN_TIME_BETWEEN_USERACTIVITIES > eventTime) {
                return;
            }
            gLastEventTime[eventType] = eventTime;
        }

        JNIEnv* env = AndroidRuntime::getJNIEnv();

        env->CallVoidMethod(gPowerManagerServiceObj,
                gPowerManagerServiceClassInfo.userActivityFromNative,
                nanoseconds_to_milliseconds(eventTime), eventType, 0);
        checkAndClearExceptionFromCallback(env, "userActivityFromNative");
    }
}

void android_server_PowerManagerService_wakeUp(nsecs_t eventTime) {
    if (gPowerManagerServiceObj) {
        JNIEnv* env = AndroidRuntime::getJNIEnv();

        env->CallVoidMethod(gPowerManagerServiceObj,
                gPowerManagerServiceClassInfo.wakeUpFromNative,
                nanoseconds_to_milliseconds(eventTime));
        checkAndClearExceptionFromCallback(env, "wakeUpFromNative");
    }
}

void android_server_PowerManagerService_goToSleep(nsecs_t eventTime) {
    if (gPowerManagerServiceObj) {
        JNIEnv* env = AndroidRuntime::getJNIEnv();

        env->CallVoidMethod(gPowerManagerServiceObj,
                gPowerManagerServiceClassInfo.goToSleepFromNative,
                nanoseconds_to_milliseconds(eventTime), 0);
        checkAndClearExceptionFromCallback(env, "goToSleepFromNative");
    }
}

// ----------------------------------------------------------------------------

static void nativeInit(JNIEnv* env, jobject obj) {
    gPowerManagerServiceObj = env->NewGlobalRef(obj);

    status_t err = hw_get_module(POWER_HARDWARE_MODULE_ID,
            (hw_module_t const**)&gPowerModule);
    if (!err) {
        gPowerModule->init(gPowerModule);
    } else {
        ALOGE("Couldn't load %s module (%s)", POWER_HARDWARE_MODULE_ID, strerror(-err));
    }
}

static void nativeSetPowerState(JNIEnv* env,
        jclass clazz, jboolean screenOn, jboolean screenBright) {
    AutoMutex _l(gPowerManagerLock);
    gScreenOn = screenOn;
    gScreenBright = screenBright;
}

static void nativeAcquireSuspendBlocker(JNIEnv *env, jclass clazz, jstring nameStr) {
    ScopedUtfChars name(env, nameStr);
    acquire_wake_lock(PARTIAL_WAKE_LOCK, name.c_str());
}

static void nativeReleaseSuspendBlocker(JNIEnv *env, jclass clazz, jstring nameStr) {
    ScopedUtfChars name(env, nameStr);
    release_wake_lock(name.c_str());
}

static void nativeSetInteractive(JNIEnv *env, jclass clazz, jboolean enable) {
    if (gPowerModule) {
        if (enable) {
            ALOGD_IF_SLOW(20, "Excessive delay in setInteractive(true) while turning screen on");
            gPowerModule->setInteractive(gPowerModule, true);
        } else {
            ALOGD_IF_SLOW(20, "Excessive delay in setInteractive(false) while turning screen off");
            gPowerModule->setInteractive(gPowerModule, false);
        }
    }
}

static void nativeSetAutoSuspend(JNIEnv *env, jclass clazz, jboolean enable) {
    if (enable) {
        ALOGD_IF_SLOW(100, "Excessive delay in autosuspend_enable() while turning screen off");
        autosuspend_enable();
    } else {
        ALOGD_IF_SLOW(100, "Excessive delay in autosuspend_disable() while turning screen on");
        autosuspend_disable();
    }
}
#if 1	//liufeng_110717
extern "C" int c_pcl_set_power_down(void);

#endif

#if 1	//liufeng_110615
//#include "mtk_dtv_svc.h"
#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*_app_tv_svc_status_nfy) (const char *msg,
                                       int         status,
                                       int         data1,
                                       int         data2);


extern int _tv_svc_init_client (_app_tv_svc_status_nfy pf_nfy);

#ifdef __cplusplus
}
#endif

extern "C" void _pf_jni_tv_svc_status (const char    *msg,
                       int           status,
                       int           data1,
                       int           data2)
{
    return;
}
#endif

static void nativeShutdown(JNIEnv *env, jclass clazz) {
		int ret;
		sync();
		
		
#if 1
		EEPDTVCFG_T rDtvCfg;

		ret = MTAL_Init();
		if(ret != MTR_OK)
		{
			LOGE("ERROR: init_vss : MTAL_Init.... , t_ret = %d \n", ret);

		}
    LOGD("call MTPDWNC_EnterStandby directly to shutdown\n");

        if(MTR_OK != MTEEPDTV_GetCfg(&rDtvCfg))
        {
            LOGE("error:MTEEPDTV_GetCfg failed \n");
        }

        rDtvCfg.u1Flags &= ~((UINT8)(1U << 5));
            
	    if(MTR_OK != MTEEPDTV_SetCfg(&rDtvCfg))
		{
			LOGE("error:MTEEPDTV_SetCfg failed \n");
		}

		MTPDWNC_EnterStandby(4);
#else
#if 1	//liufeng_110717
			_tv_svc_init_client(_pf_jni_tv_svc_status);
			/*register */
			//LOG(("Register JNI Class\r\n"));
			LOGD("------rpc init--ok-------");
#endif
	
#if 1	//liufeng_110717
		LOGD("----android_os_Power_shutdown---call mw------haha------ok----");
	
		c_pcl_set_power_down();
#else
		
#ifdef HAVE_ANDROID_OS
		android_reboot(ANDROID_RB_POWEROFF, 0, 0);
#endif
#endif
#endif

}

static void nativeReboot(JNIEnv *env, jclass clazz, jstring reason) {
	int ret;
    if (reason == NULL) {
        //android_reboot(ANDROID_RB_RESTART, 0, 0);
        ret = MTAL_Init();
	 if(ret != MTR_OK)
	 {
		LOGE("ERROR: init_vss : MTAL_Init.... , t_ret = %d \n", ret);
	 }
        LOGD("call MTPDWNC_Reboot directly to reboot\n");
	 MTPDWNC_Reboot ();
	 
    } else {
        const char *chars = env->GetStringUTFChars(reason, NULL);
		ret = property_set(ANDROID_RB_PROPERTY, "reboot,recovery");
		if(ret < 0) {
			perror("reboot");
			exit(EXIT_FAILURE);
		}
		// Don't return early. Give the reboot command time to take effect
		// to avoid messing up scripts which do "adb shell reboot && adb wait-for-device"
		while(1) { pause(); }
        env->ReleaseStringUTFChars(reason, chars);  // In case it fails.
    }
    jniThrowIOException(env, errno);

}


// ----------------------------------------------------------------------------

static JNINativeMethod gPowerManagerServiceMethods[] = {
    /* name, signature, funcPtr */
    { "nativeInit", "()V",
            (void*) nativeInit },
    { "nativeSetPowerState", "(ZZ)V",
            (void*) nativeSetPowerState },
    { "nativeAcquireSuspendBlocker", "(Ljava/lang/String;)V",
            (void*) nativeAcquireSuspendBlocker },
    { "nativeReleaseSuspendBlocker", "(Ljava/lang/String;)V",
            (void*) nativeReleaseSuspendBlocker },
    { "nativeSetInteractive", "(Z)V",
            (void*) nativeSetInteractive },
    { "nativeSetAutoSuspend", "(Z)V",
            (void*) nativeSetAutoSuspend },
    { "nativeShutdown", "()V",
            (void*) nativeShutdown },
    { "nativeReboot", "(Ljava/lang/String;)V",
            (void*) nativeReboot },
};

#define FIND_CLASS(var, className) \
        var = env->FindClass(className); \
        LOG_FATAL_IF(! var, "Unable to find class " className);

#define GET_METHOD_ID(var, clazz, methodName, methodDescriptor) \
        var = env->GetMethodID(clazz, methodName, methodDescriptor); \
        LOG_FATAL_IF(! var, "Unable to find method " methodName);

#define GET_FIELD_ID(var, clazz, fieldName, fieldDescriptor) \
        var = env->GetFieldID(clazz, fieldName, fieldDescriptor); \
        LOG_FATAL_IF(! var, "Unable to find field " fieldName);

int register_android_server_PowerManagerService(JNIEnv* env) {
    int res = jniRegisterNativeMethods(env, "com/android/server/power/PowerManagerService",
            gPowerManagerServiceMethods, NELEM(gPowerManagerServiceMethods));
    LOG_FATAL_IF(res < 0, "Unable to register native methods.");

    // Callbacks

    jclass clazz;
    FIND_CLASS(clazz, "com/android/server/power/PowerManagerService");

    GET_METHOD_ID(gPowerManagerServiceClassInfo.wakeUpFromNative, clazz,
            "wakeUpFromNative", "(J)V");

    GET_METHOD_ID(gPowerManagerServiceClassInfo.goToSleepFromNative, clazz,
            "goToSleepFromNative", "(JI)V");

    GET_METHOD_ID(gPowerManagerServiceClassInfo.userActivityFromNative, clazz,
            "userActivityFromNative", "(JII)V");

    // Initialize
    for (int i = 0; i <= USER_ACTIVITY_EVENT_LAST; i++) {
        gLastEventTime[i] = LLONG_MIN;
    }
    gScreenOn = true;
    gScreenBright = true;
    gPowerManagerServiceObj = NULL;
    gPowerModule = NULL;
    return 0;
}

} /* namespace android */
