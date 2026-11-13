#ifndef QPR2_DISPLAY_H
#define QPR2_DISPLAY_H
/*
 * Monitor choice for the XboxRecomp framebuffer window.
 *
 * The runtime creates the window on the primary monitor and reads
 * RECOMP_WINDOW_SCALE / RECOMP_FULLSCREEN itself. It has no monitor option, so
 * when the launcher sets QPR2_MONITOR (e.g. DISPLAY2) this module finds the
 * window, centres it on that monitor, and, if QPR2_FULLSCREEN is set, asks the
 * runtime's own F11 handler to go borderless fullscreen there. With neither
 * variable set nothing starts. Windows only; a no-op elsewhere.
 */
void qpr2_display_start(void);
#endif
