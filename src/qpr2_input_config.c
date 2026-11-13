#include "qpr2_input_config.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Qpr2InputConfig g_qpr2_input_cfg;

static const char *const s_names[QPR2_PAD_CONTROLS] = {
    "dpad_up", "dpad_down", "dpad_left", "dpad_right", "start", "back", "l3", "r3",
    "a", "b", "x", "y", "black", "white", "lt", "rt"
};

const char *qpr2_pad_control_name(int control)
{
    return (control >= 0 && control < QPR2_PAD_CONTROLS) ? s_names[control] : NULL;
}

static int ieq(const char *a, const char *b)
{
    for (; *a && *b; ++a, ++b) {
        char x = *a, y = *b;
        if (x >= 'A' && x <= 'Z') x = (char)(x + 32);
        if (y >= 'A' && y <= 'Z') y = (char)(y + 32);
        if (x != y) return 0;
    }
    return *a == *b;
}

int qpr2_pad_control_from_name(const char *name)
{
    int i;
    if (!name) return -1;
    for (i = 0; i < QPR2_PAD_CONTROLS; ++i)
        if (ieq(name, s_names[i])) return i;
    return -1;
}

void qpr2_input_config_defaults(Qpr2InputConfig *c)
{
    int i;
    memset(c, 0, sizeof *c);
    for (i = 0; i < QPR2_PAD_CONTROLS; ++i) c->source[i] = (int8_t)i;
    c->curve_left = c->curve_right = 1.0f;
}

/* Whole-string integer within [0, max], else -1. */
static int parse_pct(const char *s, int max)
{
    char *end;
    long v;
    if (!s || !*s) return -1;
    v = strtol(s, &end, 10);
    if (*end || v < 0 || v > max) return -1;
    return (int)v;
}

static int parse_curve(const char *s, float *out)
{
    char *end;
    double v;
    if (!s || !*s) return 0;
    v = strtod(s, &end);
    if (*end || !(v >= 0.5 && v <= 3.0)) return 0;
    *out = (float)v;
    return 1;
}

static int parse_flag(const char *s)
{
    return s && *s && *s != '0';
}

int qpr2_input_config_load(Qpr2InputConfig *c, Qpr2GetEnv get)
{
    int i, rejected = 0;
    const char *v;
    char name[40];

    qpr2_input_config_defaults(c);

    for (i = 0; i < QPR2_PAD_CONTROLS; ++i) {
        size_t n;
        snprintf(name, sizeof name, "QPR2_PAD_MAP_%s", s_names[i]);
        for (n = 0; name[n]; ++n) if (name[n] >= 'a' && name[n] <= 'z') name[n] = (char)(name[n] - 32);
        v = get(name);
        if (!v || !*v) continue;
        if (ieq(v, "none")) { c->source[i] = -1; c->remapped = 1; continue; }
        {
            int s = qpr2_pad_control_from_name(v);
            if (s < 0) { ++rejected; continue; }
            c->source[i] = (int8_t)s;
            if (s != i) c->remapped = 1;
        }
    }
    {
        struct { const char *env; uint8_t *dst; int max; } pct[] = {
            { "QPR2_PAD_DZ_L", &c->dz_left, 60 }, { "QPR2_PAD_DZ_R", &c->dz_right, 60 }, { "QPR2_PAD_TRIGGER", &c->trigger, 60 }
        };
        for (i = 0; i < 3; ++i) {
            v = get(pct[i].env);
            if (!v || !*v) continue;
            { int p = parse_pct(v, pct[i].max); if (p < 0) ++rejected; else *pct[i].dst = (uint8_t)p; }
        }
    }
    v = get("QPR2_PAD_CURVE_L"); if (v && *v && !parse_curve(v, &c->curve_left)) ++rejected;
    v = get("QPR2_PAD_CURVE_R"); if (v && *v && !parse_curve(v, &c->curve_right)) ++rejected;
    c->invert_ly = (uint8_t)parse_flag(get("QPR2_PAD_INVERT_LY"));
    c->invert_ry = (uint8_t)parse_flag(get("QPR2_PAD_INVERT_RY"));
    c->swap_sticks = (uint8_t)parse_flag(get("QPR2_PAD_SWAP_STICKS"));

    c->active = c->remapped || c->dz_left || c->dz_right || c->trigger ||
                c->curve_left != 1.0f || c->curve_right != 1.0f ||
                c->invert_ly || c->invert_ry || c->swap_sticks;
    return rejected;
}

static uint8_t trigger(uint8_t v, int pct)
{
    int t = (255 * pct) / 100;
    if (v <= t) return 0;
    return (uint8_t)(((int)(v - t) * 255) / (255 - t));
}

/* Radial deadzone that still reaches full deflection, then a response curve. */
static void stick(int16_t *x, int16_t *y, int dz_pct, float curve)
{
    float fx, fy, mag, dz, m;
    if (!dz_pct && curve == 1.0f) return;
    fx = (float)*x / 32767.0f; fy = (float)*y / 32767.0f;
    mag = sqrtf(fx * fx + fy * fy);
    dz = (float)dz_pct / 100.0f;
    if (mag <= dz || mag <= 0.0f) { *x = 0; *y = 0; return; }
    m = (mag - dz) / (1.0f - dz);
    if (m > 1.0f) m = 1.0f;
    if (curve != 1.0f) m = powf(m, curve);
    *x = (int16_t)lrintf(fx / mag * m * 32767.0f);
    *y = (int16_t)lrintf(fy / mag * m * 32767.0f);
}

static int16_t flip(int16_t v) { return v == -32768 ? 32767 : (int16_t)-v; }

void qpr2_input_config_apply(const Qpr2InputConfig *c, uint8_t blk[QPR2_PAD_BLOCK_SIZE])
{
    uint8_t val[QPR2_PAD_CONTROLS];
    int16_t sx[4];
    unsigned i, bits;
    if (!c || !c->active) return;

    for (i = 0; i < 8; ++i) val[i] = (blk[0] >> i) & 1 ? 255 : 0;
    for (i = 0; i < 8; ++i) val[8 + i] = blk[2 + i];
    for (i = 0; i < 4; ++i) sx[i] = (int16_t)(uint16_t)(blk[10 + 2 * i] | (blk[11 + 2 * i] << 8));

    if (c->remapped) {
        uint8_t nv[QPR2_PAD_CONTROLS];
        for (i = 0; i < QPR2_PAD_CONTROLS; ++i) nv[i] = c->source[i] < 0 ? 0 : val[c->source[i]];
        memcpy(val, nv, sizeof val);
    }
    if (c->trigger) {
        val[QPR2_PAD_LT] = trigger(val[QPR2_PAD_LT], c->trigger);
        val[QPR2_PAD_RT] = trigger(val[QPR2_PAD_RT], c->trigger);
    }
    bits = 0;
    for (i = 0; i < 8; ++i) if (val[i] >= 0x40) bits |= 1u << i;   /* an analog source reads as pressed from 25% */
    blk[0] = (uint8_t)bits;                                         /* high byte stays as the game left it */
    for (i = 0; i < 8; ++i) blk[2 + i] = val[8 + i];

    stick(&sx[0], &sx[1], c->dz_left, c->curve_left);
    stick(&sx[2], &sx[3], c->dz_right, c->curve_right);
    if (c->invert_ly) sx[1] = flip(sx[1]);
    if (c->invert_ry) sx[3] = flip(sx[3]);
    if (c->swap_sticks) {
        int16_t t;
        t = sx[0]; sx[0] = sx[2]; sx[2] = t;
        t = sx[1]; sx[1] = sx[3]; sx[3] = t;
    }
    for (i = 0; i < 4; ++i) { blk[10 + 2 * i] = (uint8_t)((uint16_t)sx[i] & 0xFF); blk[11 + 2 * i] = (uint8_t)((uint16_t)sx[i] >> 8); }
}

static const char *process_getenv(const char *name) { return getenv(name); }

void qpr2_input_config_init(void)
{
    int rejected = qpr2_input_config_load(&g_qpr2_input_cfg, process_getenv);
    if (g_qpr2_input_cfg.active || rejected)
        fprintf(stderr, "[INPUT-CONFIG] remap=%u deadzone=%u/%u trigger=%u curve=%.2f/%.2f invert_ly=%u invert_ry=%u swap=%u rejected=%d\n",
                g_qpr2_input_cfg.remapped, g_qpr2_input_cfg.dz_left, g_qpr2_input_cfg.dz_right, g_qpr2_input_cfg.trigger,
                g_qpr2_input_cfg.curve_left, g_qpr2_input_cfg.curve_right,
                g_qpr2_input_cfg.invert_ly, g_qpr2_input_cfg.invert_ry, g_qpr2_input_cfg.swap_sticks, rejected);
}
