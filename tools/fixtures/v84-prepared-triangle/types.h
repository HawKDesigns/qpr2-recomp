#ifndef NV2A_GPU_TYPES_H
#define NV2A_GPU_TYPES_H
#ifdef NV_GPU_HLSL
#define uint32_t uint
struct Nv2aVshOutput { float pos[4]; float d0[4]; float d1[4]; float fog[4]; float tex[4][4]; };
struct Nv2aCombiner {
    uint color_icw[8], alpha_icw[8], color_ocw[8], alpha_ocw[8];
    uint factor0[8], factor1[8];
    uint final0, final1, final_c0, final_c1, control, stage_program;
};
#else
#include <stdint.h>
#include "nv2a_vsh_interp.h"
#include "nv2a_combiner.h"
typedef struct NvGpuTexture NvGpuTexture;
typedef struct NvGpuState NvGpuState;
typedef struct NvGpuTri NvGpuTri;
typedef struct NvGpuTile NvGpuTile;
typedef struct NvGpuPixel NvGpuPixel;
#endif

/* Structured buffers use scalar layout, including tightly packed arrays.
 * Do not put these records in a HLSL constant buffer (array layout differs). */
struct NvGpuTexture {
    uint32_t width, height, format, normalized, addr_u, addr_v, filter, valid, cube, cache_slot, texel_base;
};
struct NvGpuState {
    Nv2aCombiner rc;
    NvGpuTexture tex[4];
    uint32_t width, height, pitch, bpp, format, control0;
    uint32_t depth_available, depth_test, depth_mask, depth_func;
    uint32_t alpha_test, alpha_func, alpha_ref, zpass_enable;
    uint32_t color_mask, blend_enable, blend_sfactor, blend_dfactor, blend_equation, blend_color;
    uint32_t clip_plane_mode;
};
struct NvGpuTri {
    Nv2aVshOutput v[3];
    float iw[3], uv[3][2], stc[3][4][4], vfog[3], fogc[4], inv_area, area;
    int minx, maxx, miny, maxy, inclusive[3];
    uint32_t use_rc, textured, state_index;
};
struct NvGpuTile { uint32_t x, y, first, count; };
struct NvGpuPixel {
    uint32_t color;
    float depth;
    uint32_t depth_fail, pixels, zpass, pixel_max;
};
#endif
