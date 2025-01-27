/*
 * Copyright (C) 2007 The Android Open Source Project
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

#define LOG_TAG "GraphicBufferMapper"
#define ATRACE_TAG ATRACE_TAG_GRAPHICS

#include <stdint.h>
#include <errno.h>

#include <utils/Errors.h>
#include <utils/Log.h>
#include <utils/Trace.h>

#include <ui/GraphicBufferMapper.h>
#include <ui/Rect.h>

#include <hardware/gralloc.h>


namespace android {
// ---------------------------------------------------------------------------

ANDROID_SINGLETON_STATIC_INSTANCE( GraphicBufferMapper )

GraphicBufferMapper::GraphicBufferMapper()
    : mAllocMod(0)
{
    hw_module_t const* module;
    int err = hw_get_module(GRALLOC_HARDWARE_MODULE_ID, &module);
    ALOGE_IF(err, "FATAL: can't find the %s module", GRALLOC_HARDWARE_MODULE_ID);
    if (err == 0) {
        mAllocMod = (gralloc_module_t const *)module;
    }
}

status_t GraphicBufferMapper::registerBuffer(buffer_handle_t handle)
{
    ATRACE_CALL();
    status_t err;

    err = mAllocMod->registerBuffer(mAllocMod, handle);

    ALOGW_IF(err, "registerBuffer(%p) failed %d (%s)",
            handle, err, strerror(-err));
    return err;
}

status_t GraphicBufferMapper::unregisterBuffer(buffer_handle_t handle)
{
    ATRACE_CALL();
    status_t err;

    err = mAllocMod->unregisterBuffer(mAllocMod, handle);

    ALOGW_IF(err, "unregisterBuffer(%p) failed %d (%s)",
            handle, err, strerror(-err));
    return err;
}

status_t GraphicBufferMapper::lock(buffer_handle_t handle, 
        int usage, const Rect& bounds, void** vaddr)
{
    ATRACE_CALL();
    status_t err;

    err = mAllocMod->lock(mAllocMod, handle, usage,
            bounds.left, bounds.top, bounds.width(), bounds.height(),
            vaddr);

    ALOGW_IF(err, "lock(...) failed %d (%s)", err, strerror(-err));
    return err;
}

status_t GraphicBufferMapper::lockYCbCr(buffer_handle_t handle,
        int usage, const Rect& bounds, android_ycbcr *ycbcr)
{
    ATRACE_CALL();
    status_t err;

    err = mAllocMod->lock_ycbcr(mAllocMod, handle, usage,
            bounds.left, bounds.top, bounds.width(), bounds.height(),
            ycbcr);

    ALOGW_IF(err, "lock(...) failed %d (%s)", err, strerror(-err));
    return err;
}

status_t GraphicBufferMapper::unlock(buffer_handle_t handle)
{
    ATRACE_CALL();
    status_t err;

    err = mAllocMod->unlock(mAllocMod, handle);

    ALOGW_IF(err, "unlock(...) failed %d (%s)", err, strerror(-err));
    return err;
}

#if 0 //#ifdef CC_SUPPORT_TV_STAGEFRIGHT  
//Added by mtk40134 for Locking VDP Buffer Mechanism
#define OPRATION_ID 1
void GraphicBufferMapper::isBufferKeepByVDP(buffer_handle_t handle, bool* isKeepByVDP)
{
	if(mAllocMod != NULL){
		mAllocMod->perform(mAllocMod, OPRATION_ID,  handle, isKeepByVDP);
	}
}
#endif
// ---------------------------------------------------------------------------
}; // namespace android
