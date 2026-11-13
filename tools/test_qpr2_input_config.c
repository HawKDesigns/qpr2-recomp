/* Native check of the QPR2 input options. Build and run:
 *   gcc -std=c11 -Wall -Wextra -Werror -o /tmp/qpr2_input_config tools/test_qpr2_input_config.c -lm && /tmp/qpr2_input_config
 * The block layout matches tools/test_qpr2_input_callback.c: Start is wButtons bit 4,
 * A pressure is byte 2, left stick X is the int16 at byte 10. */
#include "../src/qpr2_input_config.c"
#include <assert.h>

static const char *s_env[64][2];
static int s_envn;
static const char *fake_getenv(const char *n)
{
    int i;
    for (i = 0; i < s_envn; ++i) if (!strcmp(s_env[i][0], n)) return s_env[i][1];
    return NULL;
}
static void env_reset(void) { s_envn = 0; }
static void env_set(const char *n, const char *v) { s_env[s_envn][0] = n; s_env[s_envn][1] = v; ++s_envn; }
static int load(Qpr2InputConfig *c) { return qpr2_input_config_load(c, fake_getenv); }

static void put16(uint8_t *b, int off, int v) { b[off] = (uint8_t)(v & 0xFF); b[off + 1] = (uint8_t)((v >> 8) & 0xFF); }
static int get16(const uint8_t *b, int off) { return (int16_t)(uint16_t)(b[off] | (b[off + 1] << 8)); }
#define LX 10
#define LY 12
#define RX 14
#define RY 16

int main(void)
{
    Qpr2InputConfig c;
    uint8_t b[QPR2_PAD_BLOCK_SIZE], ref[QPR2_PAD_BLOCK_SIZE];
    int i;

    /* names */
    assert(qpr2_pad_control_from_name("A") == QPR2_PAD_A && qpr2_pad_control_from_name("black") == QPR2_PAD_BLACK);
    assert(qpr2_pad_control_from_name("DPAD_UP") == QPR2_PAD_DPAD_UP && qpr2_pad_control_from_name("nope") == -1);
    for (i = 0; i < QPR2_PAD_CONTROLS; ++i) assert(qpr2_pad_control_from_name(qpr2_pad_control_name(i)) == i);

    /* nothing set: inactive and the block is untouched */
    env_reset(); assert(load(&c) == 0 && !c.active);
    for (i = 0; i < QPR2_PAD_BLOCK_SIZE; ++i) b[i] = ref[i] = (uint8_t)(i * 13 + 7);
    qpr2_input_config_apply(&c, b); assert(!memcmp(b, ref, sizeof b));
    qpr2_input_config_apply(NULL, b); assert(!memcmp(b, ref, sizeof b));

    /* swap A and B, one gets pressure, the other digital semantics preserved */
    env_reset(); env_set("QPR2_PAD_MAP_A", "b"); env_set("QPR2_PAD_MAP_B", "A");
    assert(load(&c) == 0 && c.active && c.remapped);
    memset(b, 0, sizeof b); b[2] = 255; b[3] = 90;
    qpr2_input_config_apply(&c, b); assert(b[2] == 90 && b[3] == 255);

    /* identity mappings do not activate anything */
    env_reset(); env_set("QPR2_PAD_MAP_A", "a"); assert(load(&c) == 0 && !c.active);

    /* digital <- analog and analog <- digital */
    env_reset(); env_set("QPR2_PAD_MAP_START", "rt"); env_set("QPR2_PAD_MAP_LT", "dpad_left");
    assert(load(&c) == 0);
    memset(b, 0, sizeof b); b[9] = 200; b[0] = 0x04;                 /* RT pressed, d-pad left held */
    qpr2_input_config_apply(&c, b);
    assert((b[0] & 0x10) && b[8] == 255 && (b[0] & 0x04) && b[9] == 200); /* Start takes RT, LT takes d-pad left; the sources keep their own jobs */
    memset(b, 0, sizeof b); b[9] = 30;                               /* a light RT touch is below the 25% mark */
    qpr2_input_config_apply(&c, b); assert(!(b[0] & 0x10));

    /* none disables a control; the high byte of wButtons is left alone */
    env_reset(); env_set("QPR2_PAD_MAP_X", "none"); assert(load(&c) == 0);
    memset(b, 0, sizeof b); b[4] = 255; b[1] = 0xA5;
    qpr2_input_config_apply(&c, b); assert(b[4] == 0 && b[1] == 0xA5);

    /* the exact bytes used by test_qpr2_input_callback.c survive a remap that does not touch them */
    env_reset(); env_set("QPR2_PAD_MAP_B", "y"); assert(load(&c) == 0);
    memset(b, 0, sizeof b); b[0] = 0x10; b[2] = 255; put16(b, LX, 12345);
    qpr2_input_config_apply(&c, b); assert(b[0] == 0x10 && b[2] == 255 && get16(b, LX) == 12345);

    /* stick deadzone: inside reads zero, beyond keeps direction and still reaches full throw */
    env_reset(); env_set("QPR2_PAD_DZ_L", "20"); assert(load(&c) == 0 && c.dz_left == 20);
    memset(b, 0, sizeof b); put16(b, LX, 5000); put16(b, LY, 3000);  /* about 15% */
    qpr2_input_config_apply(&c, b); assert(get16(b, LX) == 0 && get16(b, LY) == 0);
    memset(b, 0, sizeof b); put16(b, LX, 32767);
    qpr2_input_config_apply(&c, b); assert(get16(b, LX) >= 32760);
    memset(b, 0, sizeof b); put16(b, LX, -32767); put16(b, LY, 0);
    qpr2_input_config_apply(&c, b); assert(get16(b, LX) <= -32760);
    memset(b, 0, sizeof b); put16(b, LX, 19660);                     /* 60% -> (0.6-0.2)/0.8 = 50% */
    qpr2_input_config_apply(&c, b); assert(abs(get16(b, LX) - 16384) < 40);
    memset(b, 0, sizeof b); put16(b, LX, 19660); put16(b, RX, 19660); /* right stick has no deadzone set */
    qpr2_input_config_apply(&c, b); assert(get16(b, RX) == 19660);
    {   /* direction preserved on a diagonal */
        int x, y;
        memset(b, 0, sizeof b); put16(b, LX, 20000); put16(b, LY, 20000);
        qpr2_input_config_apply(&c, b); x = get16(b, LX); y = get16(b, LY);
        assert(abs(x - y) <= 1 && x > 0 && x < 20000);
    }

    /* response curve: exponent above 1 softens the middle, below 1 sharpens it */
    env_reset(); env_set("QPR2_PAD_CURVE_L", "2"); assert(load(&c) == 0 && c.active);
    memset(b, 0, sizeof b); put16(b, LX, 16384);
    qpr2_input_config_apply(&c, b); assert(abs(get16(b, LX) - 8192) < 40);
    env_reset(); env_set("QPR2_PAD_CURVE_L", "0.5"); assert(load(&c) == 0);
    memset(b, 0, sizeof b); put16(b, LX, 8192);
    qpr2_input_config_apply(&c, b); assert(get16(b, LX) > 14000);

    /* trigger threshold rescales so full pull is still 255 */
    env_reset(); env_set("QPR2_PAD_TRIGGER", "20"); assert(load(&c) == 0);
    memset(b, 0, sizeof b); b[8] = 40; b[9] = 255;
    qpr2_input_config_apply(&c, b); assert(b[8] == 0 && b[9] == 255);
    memset(b, 0, sizeof b); b[8] = 153;                               /* (153-51)*255/204 = 127 */
    qpr2_input_config_apply(&c, b); assert(abs(b[8] - 127) <= 1);

    /* inversion, including the int16 edge, and stick swap */
    env_reset(); env_set("QPR2_PAD_INVERT_LY", "1"); env_set("QPR2_PAD_INVERT_RY", "1"); assert(load(&c) == 0);
    memset(b, 0, sizeof b); put16(b, LY, 1000); put16(b, RY, -32768); put16(b, LX, 77);
    qpr2_input_config_apply(&c, b); assert(get16(b, LY) == -1000 && get16(b, RY) == 32767 && get16(b, LX) == 77);
    env_reset(); env_set("QPR2_PAD_SWAP_STICKS", "1"); assert(load(&c) == 0);
    memset(b, 0, sizeof b); put16(b, LX, 100); put16(b, LY, 200); put16(b, RX, -300); put16(b, RY, -400);
    qpr2_input_config_apply(&c, b);
    assert(get16(b, LX) == -300 && get16(b, LY) == -400 && get16(b, RX) == 100 && get16(b, RY) == 200);
    env_reset(); env_set("QPR2_PAD_SWAP_STICKS", "0"); assert(load(&c) == 0 && !c.active);

    /* rejected values keep their defaults and are counted */
    env_reset(); env_set("QPR2_PAD_DZ_L", "999"); env_set("QPR2_PAD_DZ_R", "abc"); env_set("QPR2_PAD_CURVE_L", "9");
    env_set("QPR2_PAD_MAP_A", "banana"); env_set("QPR2_PAD_TRIGGER", "-3");
    assert(load(&c) == 5 && !c.active && c.dz_left == 0 && c.source[QPR2_PAD_A] == QPR2_PAD_A);

    /* everything together stays inside the int16 range and keeps unrelated bytes */
    env_reset(); env_set("QPR2_PAD_MAP_A", "b"); env_set("QPR2_PAD_DZ_L", "10"); env_set("QPR2_PAD_DZ_R", "10");
    env_set("QPR2_PAD_CURVE_L", "1.5"); env_set("QPR2_PAD_INVERT_RY", "1"); env_set("QPR2_PAD_SWAP_STICKS", "1"); env_set("QPR2_PAD_TRIGGER", "10");
    assert(load(&c) == 0);
    for (i = -32768; i <= 32767; i += 257) {
        memset(b, 0, sizeof b); put16(b, LX, i); put16(b, LY, -i); put16(b, RX, i / 2); put16(b, RY, i);
        qpr2_input_config_apply(&c, b);
        assert(get16(b, LX) >= -32768 && get16(b, RY) <= 32767);
    }
    puts("qpr2 input config: ok");
    return 0;
}
