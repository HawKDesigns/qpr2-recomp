#ifndef QPR2_LOG_BUFFER_H
#define QPR2_LOG_BUFFER_H
#include <stdio.h>
#include <stdlib.h>

/* stderr was explicitly unbuffered. On the Windows CRT a formatted record
 * can then issue tiny writes and block the guest/render threads in logging.
 * One process-lifetime buffer coalesces those writes. Existing periodic GPU
 * reports, fault handlers and normal CRT shutdown explicitly flush stderr.
 * Opt out only when investigating a crash before the first periodic report. */
static int qpr2_log_buffer_init(FILE *stream)
{
    static char buffer[64u * 1024u];
    const char *raw = getenv("RECOMP_LOG_UNBUFFERED");
    if (raw && *raw && *raw != '0') return setvbuf(stream, NULL, _IONBF, 0) == 0 ? 0 : -1;
    return setvbuf(stream, buffer, _IOFBF, sizeof buffer) == 0 ? 1 : -1;
}
#endif
