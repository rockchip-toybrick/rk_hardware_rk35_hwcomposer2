#ifndef RGAUTILS_H
#define RGAUTILS_H
namespace hwc_rga_utils{

int HwcGetRgaFormat(int format);
int UnifyAndroidFormatForRK3588(int format);
bool isRK3588RGA3SupportFormat(int format);
int UnifyAndroidFormatForRK3576(int format);
bool isRK3576RGA2SupportFormat(int format);

};
#endif //RGAUTILS_H