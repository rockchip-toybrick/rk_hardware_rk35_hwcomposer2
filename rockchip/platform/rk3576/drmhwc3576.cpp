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
#define LOG_TAG "hwc-drm-two"

#include "platform.h"
#include "rockchip/platform/drmhwc3576.h"
#include "drmdevice.h"

#include <log/log.h>

namespace android {

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

void Hwc3576::Init(){
}

bool Hwc3576::SupportPlatform(uint32_t soc_id){
  switch(soc_id){
    case 0x3576:
      return true;
    default:
      break;
  }
  return false;
}

struct assign_plane_group_3576{
  int assigned_crtc_id;
  uint64_t drm_type_mask;
  bool have_assigin;
};
struct assign_plane_group_3576 assign_mask_default_3576[] = {
  { -1, PLANE_RK3576_ALL_CLUSTER0_MASK | PLANE_RK3576_ALL_ESMART0_MASK, false},
  { -1, PLANE_RK3576_ALL_CLUSTER1_MASK | PLANE_RK3576_ALL_ESMART1_MASK, false},
  { -1, PLANE_RK3576_ALL_ESMART2_MASK  | PLANE_RK3576_ALL_ESMART3_MASK, false},
};

int Hwc3576::assignPlaneByHWC(DrmDevice* drm){
  std::vector<PlaneGroup*> all_plane_group = drm->GetPlaneGroups();
  for (auto &conn : drm->connectors()) {
    int display_id = conn->display();
    if(conn->state() != DRM_MODE_CONNECTED){
      HWC2_ALOGE("display=%d conn state() is disconnect.", display_id);
      continue;
    }
    DrmCrtc *crtc = drm->GetCrtcForDisplay(display_id);
    if(!crtc){
        HWC2_ALOGE("display=%d crtc is NULL.", display_id);
        continue;
    }
    
    uint32_t crtc_mask = 1<<crtc->pipe();

    uint64_t plane_mask=0;
    //热插拔等二次分配流程
    for(int i = 0; i < ARRAY_SIZE(assign_mask_default_3576);i++){
      if(crtc->id() == assign_mask_default_3576[i].assigned_crtc_id){
        plane_mask = assign_mask_default_3576[i].drm_type_mask;
        break;
      }
    }

    // 1. 将所有存在 uboot_bind_crtc_id 的 PlaneGroup 进行分配
    if(plane_mask == 0){
      for(auto &plane_group : all_plane_group){
        // 获取 uboot 阶段绑定的 crtc id
        if(plane_group->uboot_bind_crtc_id == crtc->id()){
          for(int i = 0; i < ARRAY_SIZE(assign_mask_default_3576);i++){
            // 将绑定的crtc id 对应的一组DrmPlane分配给对应的crtc
            if(assign_mask_default_3576[i].have_assigin == false && 
              (plane_group->win_type & assign_mask_default_3576[i].drm_type_mask)>0){
              plane_mask = assign_mask_default_3576[i].drm_type_mask;
              assign_mask_default_3576[i].assigned_crtc_id = crtc->id();
              assign_mask_default_3576[i].have_assigin = true;
              break;
            }
          }
        }
      }
    }

    // 2. 上一步未分配的 DrmPlane 组进行最终分配
    if(plane_mask == 0){
      for(int i = 0; i < ARRAY_SIZE(assign_mask_default_3576);i++){

        // 找到还未分配的 DrmPlane 组
        if(assign_mask_default_3576[i].have_assigin == false){
          uint64_t allow_assigin_mask = 0;
          // 遍历所有可分配到 crtc 的 drmplane, 获得 allow_assigin_mask
          for(auto &plane_group : all_plane_group){
            if((crtc_mask & plane_group->possible_crtcs) > 0){
              for(auto& drm_plane : plane_group->planes){
                allow_assigin_mask |= drm_plane->win_type();
              }
            }
          }
          uint64_t will_assigin_mask = assign_mask_default_3576[i].drm_type_mask;
          if((allow_assigin_mask & will_assigin_mask) == will_assigin_mask){
              plane_mask = assign_mask_default_3576[i].drm_type_mask;
              assign_mask_default_3576[i].assigned_crtc_id = crtc->id();
              assign_mask_default_3576[i].have_assigin = true;
              break;
          }
        }
      }
    }

    ALOGI_IF(DBG_INFO,"%s,line=%d, crtc-id=%d mask=0x%x ,plane_mask=0x%" PRIx64 ,__FUNCTION__,__LINE__,
             crtc->id(),crtc_mask,plane_mask);
    for(auto &plane_group : all_plane_group){
      uint64_t plane_group_win_type = plane_group->win_type;
      if((plane_mask & plane_group_win_type) == plane_group_win_type){
        plane_group->set_current_crtc(crtc_mask, display_id);
      }else{
        plane_group->current_crtc_ &= ~crtc_mask;
      }
    }
  }

  for(auto &plane_group : all_plane_group){
    ALOGI_IF(DBG_INFO,"%s,line=%d, name=%s cur_crtcs_mask=0x%x",__FUNCTION__,__LINE__,
             plane_group->planes[0]->name(),plane_group->current_crtc_);
  }
  return 0;
}

int Hwc3576::TryAssignPlane(DrmDevice* drm){
  int ret = -1;
  bool exist_plane_mask = false;

  for (auto &conn : drm->connectors()) {
    int display_id = conn->display();
    if(conn->state() != DRM_MODE_CONNECTED)
      continue;
    DrmCrtc *crtc = drm->GetCrtcForDisplay(display_id);
    if(!crtc){
      HWC2_ALOGE("display %d crtc is NULL.", display_id);
      continue;
    }

    ret = assignPlaneByHWC(drm);
  }

  return ret;
}
}

