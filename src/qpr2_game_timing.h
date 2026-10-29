#ifndef QPR2_GAME_TIMING_H
#define QPR2_GAME_TIMING_H
#include <stdint.h>
typedef struct {
    double started;
    uint32_t function, object, argument, auxiliary;
    unsigned slot;
} Qpr2GameTiming;
Qpr2GameTiming qpr2_game_timing_begin(uint32_t function, uint32_t object,
                                     uint32_t argument, uint32_t auxiliary);
void qpr2_game_timing_end(const Qpr2GameTiming *timing);
void qpr2_camera_state(const Qpr2GameTiming *timing,
                       float previous_x,float previous_y,float previous_z,
                       float x,float y,float z);
#endif
