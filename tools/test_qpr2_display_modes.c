#include <assert.h>
#include <stdio.h>

#include "kernel/kernel.h"
#include "video/present_layout.h"
#include "video/pvideo_scanout.h"

void xbox_log(int level, const char *subsystem, const char *format, ...)
{
    (void)level;
    (void)subsystem;
    (void)format;
}

static void check_guest_flags(ULONG expected)
{
    ULONG flags = 0;
    uint8_t settings[0x60];

    assert(xbox_ExQueryNonVolatileSetting(XC_VIDEO, NULL, &flags, sizeof flags, NULL)
           == STATUS_SUCCESS);
    assert(flags == expected);
    assert(xbox_ExQueryNonVolatileSetting(XC_MAX_OS, NULL, settings, sizeof settings, NULL)
           == STATUS_SUCCESS);
    memcpy(&flags, settings + 0x34, sizeof flags);
    assert(flags == expected);
}

static void check_rect(int width, int height, XboxPresentFilter filter,
                       int x, int y, int picture_width, int picture_height)
{
    XboxPresentRect rect = xbox_present_rect(width, height, 640, 480,
                                             xbox_video_widescreen(), filter);
    assert(rect.x == x && rect.y == y);
    assert(rect.width == picture_width && rect.height == picture_height);
}

static void check_scaled_video_overlay(void)
{
    uint32_t regs[0x1000/4]={0};
    const uint8_t yuy2[4]={235,128,16,128};
    uint32_t pixels[12*9];
    const uint32_t untouched=0x11223344;
    for(unsigned i=0;i<12*9;i++)pixels[i]=untouched;
    regs[NV_PVIDEO_BUFFER/4]=1;
    regs[NV_PVIDEO_FORMAT/4]=4u|(1u<<16)|NV_PVIDEO_FORMAT_DISPLAY;
    regs[NV_PVIDEO_LIMIT/4]=4;
    regs[NV_PVIDEO_SIZE_IN/4]=2|(1u<<16);
    regs[NV_PVIDEO_POINT_OUT/4]=1|(1u<<16);
    regs[NV_PVIDEO_SIZE_OUT/4]=2|(1u<<16);
    regs[NV_PVIDEO_DS_DX/4]=1u<<20;
    regs[NV_PVIDEO_DT_DY/4]=1u<<20;
    regs[NV_PVIDEO_COLOR_KEY/4]=untouched;
    pixels[4*12+7]=0xAABBCCDD; /* Native center of second output pixel. */
    assert(pvideo_scanout_scaled(regs,yuy2,sizeof yuy2,pixels,4,3,0,3));
    for(unsigned y=0;y<9;y++)for(unsigned x=0;x<12;x++) {
        uint32_t expected=untouched;
        if(y>=3 && y<6 && x>=3 && x<6)expected=0xFFFFFFFF;
        if(y==4 && x==7)expected=0xAABBCCDD;
        assert(pixels[y*12+x]==expected);
    }
}

int main(void)
{
    unsetenv("RECOMP_VIDEO_ASPECT");
    unsetenv("RECOMP_GUEST_WIDESCREEN");
    unsetenv("RECOMP_PRESENT_FILTER");
    check_guest_flags(XC_VIDEO_FLAGS_60Hz);
    assert(xbox_present_filter(XBOX_PRESENT_LINEAR) == XBOX_PRESENT_LINEAR);
    assert(xbox_present_filter(XBOX_PRESENT_POINT) == XBOX_PRESENT_POINT);
    check_rect(1920, 1080, XBOX_PRESENT_LINEAR, 240, 0, 1440, 1080);
    check_rect(1920, 1080, XBOX_PRESENT_UNSCALED, 640, 300, 640, 480);
    check_rect(320, 480, XBOX_PRESENT_UNSCALED, 0, 120, 320, 240);

    setenv("RECOMP_VIDEO_ASPECT", "16:9", 1);
    check_guest_flags(XC_VIDEO_FLAGS_60Hz);
    assert(xbox_present_display_width(640, 480, xbox_video_widescreen()) == 853);
    check_rect(1920, 1080, XBOX_PRESENT_LINEAR, 0, 0, 1920, 1080);
    check_rect(640, 480, XBOX_PRESENT_POINT, 0, 60, 640, 360);
    check_rect(1920, 1080, XBOX_PRESENT_UNSCALED, 533, 300, 853, 480);
    check_rect(1, 1, XBOX_PRESENT_UNSCALED, 0, 0, 1, 1);

    setenv("RECOMP_GUEST_WIDESCREEN", "1", 1);
    check_guest_flags(XC_VIDEO_FLAGS_60Hz | XC_VIDEO_FLAGS_WIDESCREEN);
    unsetenv("RECOMP_GUEST_WIDESCREEN");
    setenv("RECOMP_VIDEO_ASPECT", "4:3", 1);
    check_guest_flags(XC_VIDEO_FLAGS_60Hz);
    setenv("RECOMP_PRESENT_FILTER", "off", 1);
    assert(xbox_present_filter(XBOX_PRESENT_LINEAR) == XBOX_PRESENT_UNSCALED);
    setenv("RECOMP_PRESENT_FILTER", "point", 1);
    assert(xbox_present_filter(XBOX_PRESENT_LINEAR) == XBOX_PRESENT_POINT);
    setenv("RECOMP_PRESENT_FILTER", "linear", 1);
    assert(xbox_present_filter(XBOX_PRESENT_POINT) == XBOX_PRESENT_LINEAR);
    check_scaled_video_overlay();
    puts("PASS: Xbox video flags, aspect correction and presentation filters");
    return 0;
}
