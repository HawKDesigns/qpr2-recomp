#ifndef QPR2_UI_TRACE_H
#define QPR2_UI_TRACE_H
#include <stdint.h>
void qpr2_ui_trace(uint32_t phase, uint32_t site, uint32_t target,
                   uint32_t object, uint32_t caller, uint32_t argument,
                   uint32_t expected_esp, uint32_t old_ebx,
                   uint32_t old_esi, uint32_t old_edi);
void qpr2_ui_trace_dump(void);
void qpr2_academy_register(uint32_t kind, uint32_t object);
void qpr2_loader_trace(uint32_t phase, uint32_t site, uint32_t target,
                      uint32_t expected_esp, uint32_t old_ebx,
                      uint32_t old_esi, uint32_t old_edi);

/* pop_bytes comes from the retail callee's RET, not the generated ABI
 * metadata. Arguments have already been pushed; RET consumes our return
 * address plus pop_bytes. Observation only: never adjust guest state. */
#define QPR2_LOADER_CALL(target, fn, return_pc, pop_bytes) do { \
    uint32_t _ld_esp = g_esp + (pop_bytes); \
    uint32_t _ld_b = g_ebx, _ld_s = g_esi, _ld_d = g_edi; \
    qpr2_loader_trace(4, (return_pc) - 5, (target), _ld_esp, \
                      _ld_b, _ld_s, _ld_d); \
    PUSH32(esp, (return_pc)); RECOMP_ABI_CALL((target), fn); \
    qpr2_loader_trace(5, (return_pc) - 5, (target), _ld_esp, \
                      _ld_b, _ld_s, _ld_d); \
} while (0)

#define QPR2_LOADER_ICALL(target, saved_esp, site) do { \
    uint32_t _ld_target = (target), _ld_esp = (saved_esp); \
    uint32_t _ld_b = g_ebx, _ld_s = g_esi, _ld_d = g_edi; \
    qpr2_loader_trace(6, (site), _ld_target, _ld_esp, _ld_b, _ld_s, _ld_d); \
    RECOMP_ICALL_SAFE_AT(_ld_target, _ld_esp, (site)); \
    qpr2_loader_trace(7, (site), _ld_target, _ld_esp, _ld_b, _ld_s, _ld_d); \
} while (0)

/* Observe UI virtual-call boundaries. saved_esp is the expected caller
 * stack after callee cleanup. Do not repair or suppress an invalid call. */
#define QPR2_UI_ICALL(target, saved_esp, site) do { \
    uint32_t _ui_target = (target), _ui_esp = (saved_esp); \
    uint32_t _ui_b = g_ebx, _ui_s = g_esi, _ui_d = g_edi; \
    uint32_t _ui_object = g_ecx, _ui_arg = MEM32(g_esp + 4); \
    qpr2_ui_trace(1, (site), _ui_target, _ui_object, 0, _ui_arg, \
                  _ui_esp, _ui_b, _ui_s, _ui_d); \
    RECOMP_ICALL_SAFE_AT(_ui_target, _ui_esp, (site)); \
    qpr2_ui_trace(2, (site), _ui_target, _ui_object, 0, _ui_arg, \
                  _ui_esp, _ui_b, _ui_s, _ui_d); \
} while (0)
#endif
