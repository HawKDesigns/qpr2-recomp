#ifndef QPR2_INPUT_CONFIG_H
#define QPR2_INPUT_CONFIG_H
/*
 * Player input options applied in the host, to the input block the game has
 * just published (qpr2_recovered_001A7D43). The XboxRecomp Windows pad path hands
 * XInput to the guest unchanged, so remapping, deadzones, response curves and
 * stick inversion have to live here. Pure C: no game, Windows or runtime
 * dependency, so tools/test_qpr2_input_config.c checks every rule natively.
 *
 * The block is the 18 bytes the game keeps for a pad (the USB report payload):
 *   [0..1]  wButtons, little endian; only the low byte is used
 *           (bit0 up, 1 down, 2 left, 3 right, 4 start, 5 back, 6 L3, 7 R3)
 *   [2..9]  analog buttons: A B X Y Black White LT RT, 0..255
 *   [10..17] LX LY RX RY, int16 little endian, Y positive up
 *
 * Settings come from environment variables written by the QPR2 launcher, and a
 * setting left at its default is simply absent:
 *   QPR2_PAD_MAP_<CONTROL>=<source|none>   control takes its value from another
 *   QPR2_PAD_DZ_L / QPR2_PAD_DZ_R          stick deadzone, percent 0..60
 *   QPR2_PAD_TRIGGER                       trigger threshold, percent 0..60
 *   QPR2_PAD_CURVE_L / QPR2_PAD_CURVE_R    stick response exponent 0.5..3.0
 *   QPR2_PAD_INVERT_LY / QPR2_PAD_INVERT_RY / QPR2_PAD_SWAP_STICKS   1 = on
 * Order of operations: remap, trigger threshold, deadzone and curve per stick,
 * inversion, then stick swap.
 */
#include <stdint.h>

#define QPR2_PAD_CONTROLS   16
#define QPR2_PAD_BLOCK_SIZE 18

enum {
    QPR2_PAD_DPAD_UP, QPR2_PAD_DPAD_DOWN, QPR2_PAD_DPAD_LEFT, QPR2_PAD_DPAD_RIGHT,
    QPR2_PAD_START, QPR2_PAD_BACK, QPR2_PAD_L3, QPR2_PAD_R3,
    QPR2_PAD_A, QPR2_PAD_B, QPR2_PAD_X, QPR2_PAD_Y,
    QPR2_PAD_BLACK, QPR2_PAD_WHITE, QPR2_PAD_LT, QPR2_PAD_RT
};

typedef struct Qpr2InputConfig {
    int     active;                         /* 0: nothing to do, apply() is a no-op */
    int8_t  source[QPR2_PAD_CONTROLS];      /* control <- source control; -1 = none */
    uint8_t remapped;
    uint8_t dz_left, dz_right, trigger;     /* percent */
    float   curve_left, curve_right;        /* exponent, 1 = linear */
    uint8_t invert_ly, invert_ry, swap_sticks;
} Qpr2InputConfig;

typedef const char *(*Qpr2GetEnv)(const char *name);

extern Qpr2InputConfig g_qpr2_input_cfg;

/* Lower-case control name ("a", "black", "dpad_up"), or NULL. */
const char *qpr2_pad_control_name(int control);
/* Control index for a case-insensitive name, or -1. */
int qpr2_pad_control_from_name(const char *name);

void qpr2_input_config_defaults(Qpr2InputConfig *c);
/* Reads the variables above through `get`. Returns how many values were
 * rejected (out of range, unknown names); rejected values keep their default. */
int  qpr2_input_config_load(Qpr2InputConfig *c, Qpr2GetEnv get);
/* Transforms an 18-byte published block in place. */
void qpr2_input_config_apply(const Qpr2InputConfig *c, uint8_t block[QPR2_PAD_BLOCK_SIZE]);
/* Loads g_qpr2_input_cfg from the process environment and logs a summary. */
void qpr2_input_config_init(void);

#endif
