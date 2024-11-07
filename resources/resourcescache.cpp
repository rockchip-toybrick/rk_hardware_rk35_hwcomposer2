/*
 * Copyright (C) 2016 The Android Open Source Project
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
#define ATRACE_TAG ATRACE_TAG_GRAPHICS
#define LOG_TAG "hwc-resource-cache"

#include <inttypes.h>
#include "resources/resourcescache.h"
#include "rockchip/utils/drmdebug.h"

namespace android {
GemHandle::GemHandle() : drmGralloc_(DrmGralloc::getInstance()), name_(NULL){};
GemHandle::~GemHandle(){
  if(drmGralloc_ == NULL || name_ == NULL)
    return;

  if(uBufferId_ == 0 || uGemHandle_ == 0)
    return;

  int ret = drmGralloc_->hwc_free_gemhandle(uBufferId_);
  if(ret){
    HWC2_ALOGE("%s hwc_free_gemhandle fail, buffer_id =%" PRIx64, name_, uBufferId_);
  }
}

int GemHandle::InitGemHandle(const char *name,
                  uint64_t buffer_fd,
                  uint64_t buffer_id){
    name_ = name;
    uBufferId_ = buffer_id;
    int ret = drmGralloc_->hwc_get_gemhandle_from_fd(buffer_fd, uBufferId_, &uGemHandle_);
    if(ret){
      HWC2_ALOGE("%s hwc_get_gemhandle_from_fd fail, buffer_id =%" PRIx64, name_, uBufferId_);
    }
    return ret;
}

uint32_t GemHandle::GetGemHandle(){ return uGemHandle_;}
bool GemHandle::isValid(){ return uGemHandle_ != 0;}

RgaHandle::RgaHandle() : uRgaHandle_(0), uBufferId_(0), iSize_(0), iFd_(-1), name_(NULL){};
RgaHandle::~RgaHandle(){
  std::unique_lock<std::recursive_mutex> lock(mRecursiveMutex);
  if (uRgaHandle_ > 0){
    int ret = releasebuffer_handle(uRgaHandle_);
    if(ret < 0){
        HWC2_ALOGE("RgaHandle %s releasebuffer fail, buffer_id=0x%" PRIx64, name_, uBufferId_);
    }
    uRgaHandle_ = 0;
  }
}

uint32_t RgaHandle::GetRgaHandle(const char* name, int fd, int size, uint64_t buffer_id){
  std::unique_lock<std::recursive_mutex> lock(mRecursiveMutex);
  if(uRgaHandle_ > 0){
    if(buffer_id == uBufferId_ && size == iSize_){
      return uRgaHandle_;
    }else{
      HWC2_ALOGW("%s fd=%d buffer-id=0x%" PRIx64 " size=%d is change, old %s fd=%d buffer-id=0x%" PRIx64 " size=%d",
                 name, fd, buffer_id,  size,
                 name_, iFd_, uBufferId_, iSize_);
      int ret = releasebuffer_handle(uRgaHandle_);
      if(ret < 0){
          HWC2_ALOGE("RgaHandle %s releasebuffer fail, buffer_id=0x%" PRIx64, name_, uBufferId_);
      }
      uRgaHandle_ = 0;
    }
  }

  uBufferId_ = buffer_id;
  iSize_ = size;
  name_ = name;
  iFd_ = fd;
  uRgaHandle_ = importbuffer_fd(fd, size);
  if (uRgaHandle_ == 0)
  {
      HWC2_ALOGE("RgaHandle %s import fail, fd=%d, size=%d buffer_id=0x%" PRIx64, name, fd, size, buffer_id);
      return 0;
  }
  return uRgaHandle_;
}

bool RgaHandle::isValid(){
  std::unique_lock<std::recursive_mutex> lock(mRecursiveMutex);
  return uRgaHandle_ != 0;
}

LayerInfoCache::LayerInfoCache(){};
LayerInfoCache::~LayerInfoCache(){
  if(bFbIdCached_){
    bFbIdCached_ = false;
    DrmGralloc::getInstance()->hwc_fbid_dec_layer_ref_count(uBufferId_);
  }
  if(buffer_){
    DrmGralloc::getInstance()->freeBuffer(buffer_);
    buffer_ = NULL;
  }

}

};