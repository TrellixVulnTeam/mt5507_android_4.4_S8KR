/*
 * Copyright (C) 2009 The Android Open Source Project
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


#include <stdint.h>
#include <stdbool.h>

#include <media/AudioSystem.h>
#include <binder/IServiceManager.h>
#include <utils/RefBase.h>
#include <IUploadService.h>

#include <hardware_legacy/AudioPolicyManagerBase.h>

namespace android_audio_legacy {

class AudioPolicyManagerDefault: public AudioPolicyManagerBase
{

public:
                AudioPolicyManagerDefault(AudioPolicyClientInterface *clientInterface)
                : AudioPolicyManagerBase(clientInterface) {}

#if 1 //def WITH_A2DP
        status_t setDeviceConnectionState(audio_devices_t device,
                                          AudioSystem::device_connection_state state,
                                          const char *device_address);
#endif
        virtual ~AudioPolicyManagerDefault() {}

};
};
