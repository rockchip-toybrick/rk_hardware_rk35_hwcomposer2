#ifndef RGAUTILS_H
#define RGAUTILS_H

#include "im2d.hpp"
#include <drm_fourcc.h>
#include <set>

int HwcGetRgaCompatibleFormat(int format);
int HwcGetRgaFormatFromAndroid(int format);
int HwcGetRgaFormat(int format);
int UnifyAndroidFormatForRK3588(int format);
bool isRK3588RGA3SupportFormat(int format);

#endif //RGAUTILS_H