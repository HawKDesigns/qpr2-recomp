#include "qpr2_display.h"

#if defined(_WIN32)
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { char device[40]; RECT work; int found; } MonitorSearch;

static BOOL CALLBACK monitor_proc(HMONITOR monitor, HDC dc, LPRECT rect, LPARAM param)
{
    MonitorSearch *s = (MonitorSearch *)param;
    MONITORINFOEXA info;
    (void)dc; (void)rect;
    memset(&info, 0, sizeof info);
    info.cbSize = sizeof info;
    if (GetMonitorInfoA(monitor, (MONITORINFO *)&info) && _stricmp(info.szDevice, s->device) == 0) {
        s->work = info.rcWork;
        s->found = 1;
        return FALSE;
    }
    return TRUE;
}

/* "DISPLAY2" only: a bare device name, never a path or anything else. */
static int valid_monitor_name(const char *s)
{
    size_t n = strlen(s), i;
    if (n < 8 || n > 9 || _strnicmp(s, "DISPLAY", 7) != 0) return 0;
    for (i = 7; i < n; ++i) if (s[i] < '0' || s[i] > '9') return 0;
    return 1;
}

static BOOL CALLBACK find_framebuffer(HWND window, LPARAM param)
{
    DWORD pid = 0;
    char class_name[32];
    GetWindowThreadProcessId(window, &pid);
    if (pid != GetCurrentProcessId() || !IsWindowVisible(window) ||
        !GetClassNameA(window, class_name, sizeof class_name) ||
        strcmp(class_name, "XboxRecompFramebuffer") != 0)
        return TRUE;
    *(HWND *)param = window;
    return FALSE;
}

static DWORD WINAPI display_thread(LPVOID unused)
{
    const char *want = getenv("QPR2_MONITOR");
    const char *fs = getenv("QPR2_FULLSCREEN");
    int fullscreen = fs && *fs && *fs != '0';
    HWND hwnd = NULL;
    int i;
    (void)unused;

    /* The window appears once the guest sets up its framebuffer; wait up to two minutes. */
    for (i = 0; i < 1200 && !hwnd; ++i) {
        EnumWindows(find_framebuffer, (LPARAM)&hwnd);
        if (!hwnd) Sleep(100);
    }
    if (!hwnd) {
        fprintf(stderr, "[DISPLAY] framebuffer window never appeared; monitor options not applied\n");
        return 0;
    }

    if (want && *want) {
        MonitorSearch s;
        memset(&s, 0, sizeof s);
        if (!valid_monitor_name(want)) {
            fprintf(stderr, "[DISPLAY] ignoring invalid QPR2_MONITOR\n");
        } else {
            _snprintf(s.device, sizeof s.device, "\\\\.\\%s", want);
            EnumDisplayMonitors(NULL, NULL, monitor_proc, (LPARAM)&s);
            if (!s.found) {
                fprintf(stderr, "[DISPLAY] monitor %s not found; staying on the current one\n", want);
            } else {
                RECT wr;
                int ww, wh, aw = s.work.right - s.work.left, ah = s.work.bottom - s.work.top;
                if (!GetWindowRect(hwnd, &wr)) return 0;
                ww = wr.right - wr.left; wh = wr.bottom - wr.top;
                if (ww > aw) ww = aw;
                if (wh > ah) wh = ah;
                if (SetWindowPos(hwnd, NULL, s.work.left + (aw - ww) / 2, s.work.top + (ah - wh) / 2,
                                 ww, wh, SWP_NOZORDER | SWP_NOACTIVATE))
                    fprintf(stderr, "[DISPLAY] window moved to %s (%dx%d)\n", want, ww, wh);
                else
                    fprintf(stderr, "[DISPLAY] could not move window to %s\n", want);
            }
        }
    }
    if (fullscreen) {
        /* The runtime's own handler (fb_present.c) goes borderless on the window's current monitor. */
        if (PostMessageA(hwnd, WM_KEYDOWN, VK_F11, 1))
            fprintf(stderr, "[DISPLAY] fullscreen requested\n");
    }
    fflush(stderr);
    return 0;
}

void qpr2_display_start(void)
{
    const char *mon = getenv("QPR2_MONITOR");
    const char *fs = getenv("QPR2_FULLSCREEN");
    HANDLE t;
    if (!(mon && *mon) && !(fs && *fs && *fs != '0')) return;
    t = CreateThread(NULL, 0, display_thread, NULL, 0, NULL);
    if (t) CloseHandle(t);
}
#else
void qpr2_display_start(void) {}
#endif
