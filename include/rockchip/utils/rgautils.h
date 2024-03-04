#ifndef RGAUTILS_H
#define RGAUTILS_H
namespace hwc_rga_utils{

int HwcGetRgaFormat(int format);
int UnifyAndroidFormatForRK3588(int format);
bool isRK3588RGA3SupportFormat(int format);
int UnifyAndroidFormatForRK3576(int format);
bool isRK3576RGA2SupportFormat(int format);

#ifndef IM_SCHEDULER_RGA2_CORE0
#define IM_SCHEDULER_RGA2_CORE0 (1 << 2)
#define IM_SCHEDULER_RGA2_CORE1 (1 << 3)
#endif
};
#endif //RGAUTILS_H