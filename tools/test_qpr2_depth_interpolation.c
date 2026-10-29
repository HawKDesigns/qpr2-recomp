#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../third_party/xboxrecomp/src/kernel/nv2a_interpolation.h"
int main(void)
{
    unsigned old_holes = 0, checked = 0;
    const float depth = 8388607.0f;
    /* Two equal-depth layers should pass LEQUAL at every covered pixel.
     * Independently rounded barycentric weights reproduce the old defect. */
    for (int y = 0; y < 311; ++y) for (int x = 0; x < 601; ++x) {
        float b = (float)x / 601.0f, c = (float)y / 311.0f;
        if (b + c > 1.0f) continue;
        float a = 1.0f - b - c;
        volatile float previous = a * depth + b * depth + c * depth;
        float current = nv2a_interpolate_depth(depth, depth, depth, b, c);
        if (previous > depth) ++old_holes;
        assert(current == depth); ++checked;
    }
    assert(old_holes > 0);
    assert(nv2a_interpolate_depth(10, 20, 30, 0.25f, 0.25f) == 17.5f);
    assert(nv2a_interpolate_depth(0, 0, 0, 0.2f, 0.3f) == 0); /* Video plane. */
    assert(nv2a_interpolate_depth(10, 20, 30, 1, 0) == 20);
    assert(nv2a_interpolate_depth(10, 20, 30, 0, 1) == 30);
    printf("PASS: %u coplanar pixels; old interpolation falsely rejected %u, fixed rejects zero; slopes/vertices/video depth retained\n", checked, old_holes);
}
