#include "qpr2_game_timing.h"
/* Bounded routines recovered from the supplied retail disassembly. */

#define RECOMP_GENERATED_CODE

#include "recomp/gen/recomp_types.h"

#include "recomp/gen/recomp_funcs.h"

void qpr2_recovered_000FD970(void);

void qpr2_recovered_00083560(void);

void qpr2_recovered_000D0600(void);

void qpr2_recovered_000D0B80(void);

void qpr2_recovered_0006F5F0(void);

void qpr2_recovered_0010D020(void);

void qpr2_recovered_00115F30(void);

void qpr2_recovered_0010E930(void);

void qpr2_recovered_0010DF60(void);

void qpr2_recovered_001284F0(void);

void qpr2_recovered_0007F390(void);

void qpr2_recovered_000A6B00(void);

void qpr2_recovered_000C50F0(void);

void qpr2_recovered_000C96B0(void);

/**
 * qpr2_recovered_000FD970
 * Original: 0x000FD970 - 0x000FDB77 (519 bytes, 124 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000FD970(void)
{
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)

loc_000FD970: ;
    RECOMP_PC(0x000FD970u);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    PUSH32(esp, 0x000FD97Au); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000FD97A: ;
    RECOMP_PC(0x000FD97Au);
    PUSH32(esp, 0x1DE580);
    PUSH32(esp, 0);
    PUSH32(esp, 0x000FD986u); RECOMP_ABI_CALL(0x000E6A00u, sub_000E6A00); /* call 0x000E6A00 */

loc_000FD986: ;
    RECOMP_PC(0x000FD986u);
    ecx = MEM32(0x1DC428);
    eax = 3;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, eax (32-bit) */
    edi = 1;
    if (CMP_EQ(_fa, _fb)) goto loc_000FD9B5; /* je: equal / zero */

loc_000FD99D: ;
    RECOMP_PC(0x000FD99Du);
    ecx = MEM32(0x1A50F0);
    ecx = ecx | edi;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    MEM32(0x1DC428) = eax;
    MEM32(0x1A50F0) = ecx;
    MEM32(0x1A2EA0) = eax;

loc_000FD9B5: ;
    RECOMP_PC(0x000FD9B5u);
    _fa = (uint32_t)(MEM32(0x1DC368)) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC368), eax (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000FD9D5; /* je: equal / zero */

loc_000FD9BD: ;
    RECOMP_PC(0x000FD9BDu);
    ecx = MEM32(0x1A50F0);
    ecx = ecx | edi;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    MEM32(0x1DC368) = eax;
    MEM32(0x1A50F0) = ecx;
    MEM32(0x1A2EA4) = eax;

loc_000FD9D5: ;
    RECOMP_PC(0x000FD9D5u);
    eax = MEM32(0x1DC444);
    esi = 0x207;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, esi (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000FD9FB; /* je: equal / zero */

loc_000FD9E3: ;
    RECOMP_PC(0x000FD9E3u);
    edx = esi;
    ecx = 0x40354;
    MEM32(0x1DC444) = esi;
    PUSH32(esp, 0x000FD9F5u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000FD9F5: ;
    RECOMP_PC(0x000FD9F5u);
    MEM32(0x1A51DC) = esi;

loc_000FD9FB: ;
    RECOMP_PC(0x000FD9FBu);
    eax = MEM32(0x1DC3B4);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000FDA24; /* je: equal / zero */

loc_000FDA04: ;
    RECOMP_PC(0x000FDA04u);
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ecx = 0x40300;
    MEM32(0x1DC3B4) = 0;
    PUSH32(esp, 0x000FDA1Au); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000FDA1A: ;
    RECOMP_PC(0x000FDA1Au);
    MEM32(0x1A51E8) = 0;

loc_000FDA24: ;
    RECOMP_PC(0x000FDA24u);
    PUSH32(esp, 0);
    PUSH32(esp, 0x000FDA2Bu); RECOMP_ABI_CALL(0x000E4CA0u, sub_000E4CA0); /* call 0x000E4CA0 */

loc_000FDA2B: ;
    RECOMP_PC(0x000FDA2Bu);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x3F8CCCCD);
    PUSH32(esp, 0x3F8CCCCD);
    PUSH32(esp, 0x3F8CCCCD);
    PUSH32(esp, 0x3F800000);
    ecx = esp + 0x20;
    PUSH32(esp, 0x000FDA4Bu); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_000FDA4B: ;
    RECOMP_PC(0x000FDA4Bu);
    eax = MEM32(0x1DC16C);
    MEM32(esp + 8) = eax;
    PUSH32(esp, 0x000FDA59u); RECOMP_ABI_CALL(0x000E3360u, sub_000E3360); /* call 0x000E3360 */

loc_000FDA59: ;
    RECOMP_PC(0x000FDA59u);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 8)); /* fmul dword ptr [esp + 8] */
    ecx = MEM32(0x1DC168);
    MEM32(esp + 0xC) = ecx;
    MEMF(esp + 8) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x000FDA70u); RECOMP_ABI_CALL(0x000E3380u, sub_000E3380); /* call 0x000E3380 */

loc_000FDA70: ;
    RECOMP_PC(0x000FDA70u);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0xC)); /* fmul dword ptr [esp + 0xc] */
    edx = MEM32(esp + 0x10);
    fp_push(MEMF(0x1CF374)); /* fld float */
    PUSH32(esp, 0xBF800000u);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x1CF36C)); /* fadd dword ptr [0x1cf36c] */
    PUSH32(esp, edx);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1C91CC)); /* fsub dword ptr [0x1c91cc] */
    MEMF(esp + 0xC) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x1CF370)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x1CF368)); /* fadd dword ptr [0x1cf368] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x20)); /* fmul dword ptr [esp + 0x20] */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1C91CC)); /* fsub dword ptr [0x1c91cc] */
    MEMF(esp + 8) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x1CF36C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x1C91CC)); /* fadd dword ptr [0x1c91cc] */
    MEMF(esp + 4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_pop(); /* fstp st(0) */
    fp_push(MEMF(0x1CF368)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x20)); /* fmul dword ptr [esp + 0x20] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x1C91CC)); /* fadd dword ptr [0x1c91cc] */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0xC0000000u);
    PUSH32(esp, 0x40000000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0xBF800000u);
    PUSH32(esp, 0x000FDAF3u); RECOMP_ABI_CALL(0x000E5710u, sub_000E5710); /* call 0x000E5710 */

loc_000FDAF3: ;
    RECOMP_PC(0x000FDAF3u);
    PUSH32(esp, 0);
    PUSH32(esp, 0x000FDAFAu); RECOMP_ABI_CALL(0x0001D370u, sub_0001D370); /* call 0x0001D370 */

loc_000FDAFA: ;
    RECOMP_PC(0x000FDAFAu);
    eax = MEM32(0x1DC3B4);
    _fb = (uint32_t)(0x2C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x2C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000FDB1E; /* jne: not equal / not zero */

loc_000FDB06: ;
    RECOMP_PC(0x000FDB06u);
    edx = edi;
    ecx = 0x40300;
    MEM32(0x1DC3B4) = edi;
    PUSH32(esp, 0x000FDB18u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000FDB18: ;
    RECOMP_PC(0x000FDB18u);
    MEM32(0x1A51E8) = edi;

loc_000FDB1E: ;
    RECOMP_PC(0x000FDB1Eu);
    eax = MEM32(0x1DC444);
    esi = 0x203;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, esi (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000FDB44; /* je: equal / zero */

loc_000FDB2C: ;
    RECOMP_PC(0x000FDB2Cu);
    edx = esi;
    ecx = 0x40354;
    MEM32(0x1DC444) = esi;
    PUSH32(esp, 0x000FDB3Eu); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000FDB3E: ;
    RECOMP_PC(0x000FDB3Eu);
    MEM32(0x1A51DC) = esi;

loc_000FDB44: ;
    RECOMP_PC(0x000FDB44u);
    eax = MEM32(0x1DC3E8);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    POP32(esp, edi);
    POP32(esp, esi);
    PUSH32(esp, 0);
    if (CMP_EQ(_fa, _fb)) goto loc_000FDB64; /* je: equal / zero */

loc_000FDB51: ;
    RECOMP_PC(0x000FDB51u);
    PUSH32(esp, 0x000FDB56u); RECOMP_ABI_CALL(0x000E4C60u, sub_000E4C60); /* call 0x000E4C60 */

loc_000FDB56: ;
    RECOMP_PC(0x000FDB56u);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x000FDB5Eu); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000FDB5E: ;
    RECOMP_PC(0x000FDB5Eu);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; return; /* ret 4 */

loc_000FDB64: ;
    RECOMP_PC(0x000FDB64u);
    PUSH32(esp, 0x000FDB69u); RECOMP_ABI_CALL(0x000E4CA0u, sub_000E4CA0); /* call 0x000E4CA0 */

loc_000FDB69: ;
    RECOMP_PC(0x000FDB69u);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x000FDB71u); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000FDB71: ;
    RECOMP_PC(0x000FDB71u);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; return; /* ret 4 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_00083560
 * Original: 0x00083560 - 0x0008365E (254 bytes, 77 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00083560(void)
{
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)

loc_00083560: ;
    RECOMP_PC(0x00083560u);
    _fb = (uint32_t)(0x24) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x24;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    edx = MEM32(esp + 0x28);
    fp_push(MEMF(edx)); /* fld float */
    eax = ecx + 4;
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(eax)); /* fsub dword ptr [eax] */
    fp_push(MEMF(edx + 4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(eax + 4)); /* fsub dword ptr [eax + 4] */
    fp_push(MEMF(edx + 8)); /* fld float */
    edx = MEM32(ecx + 0x1C);
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(eax + 8)); /* fsub dword ptr [eax + 8] */
    MEM32(esp + 0x28) = edx;
    edx = MEM32(esp + 0x2C);
    MEMF(esp + 0x14) = (float)fp_top(); fp_pop(); /* fstp */
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x28)); /* fmul dword ptr [esp + 0x28] */
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x28)); /* fmul dword ptr [esp + 0x28] */
    fp_push(MEMF(esp + 0x14)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x28)); /* fmul dword ptr [esp + 0x28] */
    fp_push(MEMF(edx)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x10)); /* fsub dword ptr [ecx + 0x10] */
    fp_push(MEMF(edx + 4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x14)); /* fsub dword ptr [ecx + 0x14] */
    fp_push(MEMF(edx + 8)); /* fld float */
    edx = MEM32(ecx + 0x20);
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x18)); /* fsub dword ptr [ecx + 0x18] */
    MEM32(esp + 0x28) = edx;
    MEMF(esp + 0x14) = (float)fp_top(); fp_pop(); /* fstp */
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x28)); /* fmul dword ptr [esp + 0x28] */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x28)); /* fmul dword ptr [esp + 0x28] */
    fp_push(MEMF(esp + 0x14)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x28)); /* fmul dword ptr [esp + 0x28] */
    MEMF(esp + 8) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esp + 0x18)); /* fadd dword ptr [esp + 0x18] */
    MEMF(esp + 0xC) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() + g_fp_stack[(g_fp_top + 2) & 7]); /* fadd st(2) */
    MEMF(esp + 0x10) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + fp_st1()); /* fadd st(1) */
    g_fp_stack[(g_fp_top + 2) & 7] = fp_top(); fp_pop(); /* fstp st(2) */
    fp_pop(); /* fstp st(0) */
    fp_push(MEMF(esp + 0xC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x30)); /* fmul dword ptr [esp + 0x30] */
    fp_push(MEMF(esp + 0x10)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x30)); /* fmul dword ptr [esp + 0x30] */
    MEMF(esp + 0x10) = (float)fp_top(); fp_pop(); /* fstp */
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x30)); /* fmul dword ptr [esp + 0x30] */
    MEMF(esp + 0x14) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ecx + 0x10)); /* fadd dword ptr [ecx + 0x10] */
    MEMF(ecx + 0x10) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x10)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ecx + 0x14)); /* fadd dword ptr [ecx + 0x14] */
    MEMF(ecx + 0x14) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x14)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ecx + 0x18)); /* fadd dword ptr [ecx + 0x18] */
    MEMF(ecx + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x30)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ecx + 0x10)); /* fmul dword ptr [ecx + 0x10] */
    fp_push(MEMF(esp + 0x30)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ecx + 0x14)); /* fmul dword ptr [ecx + 0x14] */
    fp_push(MEMF(esp + 0x30)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ecx + 0x18)); /* fmul dword ptr [ecx + 0x18] */
    MEMF(esp + 0x20) = (float)fp_top(); fp_pop(); /* fstp */
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax)); /* fadd dword ptr [eax] */
    MEMF(eax) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 4)); /* fadd dword ptr [eax + 4] */
    MEMF(eax + 4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x20)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 8)); /* fadd dword ptr [eax + 8] */
    MEMF(eax + 8) = (float)fp_top(); fp_pop(); /* fstp */
    _fb = (uint32_t)(0x24) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x24;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 16; return; /* ret 12 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_000D0600
 * Original: 0x000D0600 - 0x000D0723 (291 bytes, 100 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000D0600(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_000D0600: ;
    RECOMP_PC(0x000D0600u);
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x1C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    fp_push(MEMF(0x1C1F94)); /* fld float */
    PUSH32(esp, ebx);
    ebx = ecx;
    fp_top() = -fp_top(); /* fchs */
    ecx = MEM32(0x1C1F94);
    MEMF(ebx + 0x14) = (float)fp_top(); /* fst */
    MEM32(ebx + 8) = ecx;
    MEMF(ebx + 0x18) = (float)fp_top(); /* fst */
    MEM32(ebx + 0x10) = ecx;
    MEMF(ebx + 0x1C) = (float)fp_top(); /* fst */
    edx = ecx;
    MEM32(ebx + 0xC) = edx;
    edx = MEM32(ebx + 0x2C);
    ecx = MEM32(ebx + 0x28);
    eax = ebx + 8;
    MEM32(ebx + 0x28) = edx;
    MEM32(ebx + 0x2C) = ecx;
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    ebp = ebx + 0x38;
    PUSH32(esp, edi);
    edi = ebx + 0x50;
    ecx = 6;
    esi = ebp;
    { uint64_t _n = (uint64_t)ecx * 4u;
    if (!g_df && esi < 0xFD000000u && edi < 0xFD000000u && (uint64_t)esi + _n <= 0xFD000000u && (uint64_t)edi + _n <= 0xFD000000u) {
      uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi);
      if ((uint64_t)edi + _n <= esi || (uint64_t)esi + _n <= edi) memcpy(_d, _s, (size_t)_n);
      else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4u) = MEM32(esi + _i*4u); }
      esi += ecx * 4u; edi += ecx * 4u; }
    else { uint32_t _i; int32_t _st = RECOMP_DF_STEP(4);
      for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*_st) = MEM32(esi + _i*_st);
      esi += ecx * _st; edi += ecx * _st; }
    ecx = 0; } /* rep movsd */
    edi = eax;
    MEM32(esp + 0x10) = eax;
    eax = MEM32(0x1C1F94);
    ecx = 6;
    esi = ebp;
    { uint64_t _n = (uint64_t)ecx * 4u;
    if (!g_df && esi < 0xFD000000u && edi < 0xFD000000u && (uint64_t)esi + _n <= 0xFD000000u && (uint64_t)edi + _n <= 0xFD000000u) {
      uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi);
      if ((uint64_t)edi + _n <= esi || (uint64_t)esi + _n <= edi) memcpy(_d, _s, (size_t)_n);
      else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4u) = MEM32(esi + _i*4u); }
      esi += ecx * 4u; edi += ecx * 4u; }
    else { uint32_t _i; int32_t _st = RECOMP_DF_STEP(4);
      for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*_st) = MEM32(esi + _i*_st);
      esi += ecx * _st; edi += ecx * _st; }
    ecx = 0; } /* rep movsd */
    MEMF(ebp + 0xC) = (float)fp_top(); /* fst */
    MEMF(ebp + 0x10) = (float)fp_top(); /* fst */
    MEMF(ebp + 0x14) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = eax;
    edx = eax;
    MEM32(ebp) = eax;
    eax = MEM32(esp + 0x30);
    MEM32(ebp + 4) = ecx;
    ecx = MEM32(esp + 0x34);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    MEM32(ebp + 8) = edx;
    PUSH32(esp, 0x000D0682u); RECOMP_ABI_CALL(0x00019680u, sub_00019680); /* call 0x00019680 */

loc_000D0682: ;
    RECOMP_PC(0x000D0682u);
    eax = MEM32(ebx + 0x30);
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, esi (32-bit) */
    MEM32(esp + 0x30) = esi;
    if (CMP_LE(_fas, _fbs)) goto loc_000D070F; /* jle: less or equal (signed <=) */

loc_000D0692: ;
    RECOMP_PC(0x000D0692u);
    eax = MEM32(ebx + 0x24);
    edi = MEM32(ebx + 0x28);
    _fb = (uint32_t)(esi) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + esi;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x1DAEB4);
    PUSH32(esp, eax);
    edx = esp + 0x28;
    PUSH32(esp, edx);
    _fb = (uint32_t)(esi) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edi = edi + esi;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x000D06ACu); RECOMP_ABI_CALL(0x000CE420u, sub_000CE420); /* call 0x000CE420 */

loc_000D06AC: ;
    RECOMP_PC(0x000D06ACu);
    fp_push(MEMF(0x1DAEA8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax)); /* fadd dword ptr [eax] */
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    fp_push(MEMF(0x1DAEAC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 4)); /* fadd dword ptr [eax + 4] */
    fp_push(MEMF(0x1DAEB0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 8)); /* fadd dword ptr [eax + 8] */
    MEMF(esp + 0x1C) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esp + 0x1C);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(edi) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(edi + 8) = eax;
    MEMF(edi + 4) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = MEM32(ebx + 0x24);
    eax = MEM32(esi + ecx + 0xC);
    edx = MEM32(ebx + 0x28);
    MEM32(edx + esi + 0xC) = eax;
    ecx = MEM32(ebx + 0x28);
    edx = MEM32(ecx + esi + 0xC);
    eax = ecx + esi;
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    ecx = ebp;
    PUSH32(esp, 0x000D06FCu); RECOMP_ABI_CALL(0x00019580u, sub_00019580); /* call 0x00019580 */

loc_000D06FC: ;
    RECOMP_PC(0x000D06FCu);
    eax = MEM32(esp + 0x30);
    ecx = MEM32(ebx + 0x30);
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + 0x10;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ecx (32-bit) */
    MEM32(esp + 0x30) = eax;
    if (CMP_L(_fas, _fbs)) goto loc_000D0692; /* jl: less (signed <) */

loc_000D070F: ;
    RECOMP_PC(0x000D070Fu);
    ecx = MEM32(esp + 0x10);
    PUSH32(esp, ebp);
    PUSH32(esp, 0x000D0719u); RECOMP_ABI_CALL(0x00019480u, sub_00019480); /* call 0x00019480 */

loc_000D0719: ;
    RECOMP_PC(0x000D0719u);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x1C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 12; return; /* ret 8 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_000D0B80
 * Original: 0x000D0B80 - 0x000D0C95 (277 bytes, 100 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000D0B80(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_000D0B80: ;
    RECOMP_PC(0x000D0B80u);
    _fb = (uint32_t)(0x14) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x14;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    edx = MEM32(esp + 0x1C);
    PUSH32(esp, ebx);
    ebx = ecx;
    PUSH32(esp, ebp);
    ebp = MEM32(esp + 0x20);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    eax = ebx + 0x50;
    esi = eax;
    edi = ebx + 0x68;
    ecx = 6;
    { uint64_t _n = (uint64_t)ecx * 4u;
    if (!g_df && esi < 0xFD000000u && edi < 0xFD000000u && (uint64_t)esi + _n <= 0xFD000000u && (uint64_t)edi + _n <= 0xFD000000u) {
      uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi);
      if ((uint64_t)edi + _n <= esi || (uint64_t)esi + _n <= edi) memcpy(_d, _s, (size_t)_n);
      else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4u) = MEM32(esi + _i*4u); }
      esi += ecx * 4u; edi += ecx * 4u; }
    else { uint32_t _i; int32_t _st = RECOMP_DF_STEP(4);
      for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*_st) = MEM32(esi + _i*_st);
      esi += ecx * _st; edi += ecx * _st; }
    ecx = 0; } /* rep movsd */
    edi = ebx + 8;
    esi = eax;
    ecx = 6;
    MEM32(esp + 0x14) = edi;
    { uint64_t _n = (uint64_t)ecx * 4u;
    if (!g_df && esi < 0xFD000000u && edi < 0xFD000000u && (uint64_t)esi + _n <= 0xFD000000u && (uint64_t)edi + _n <= 0xFD000000u) {
      uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi);
      if ((uint64_t)edi + _n <= esi || (uint64_t)esi + _n <= edi) memcpy(_d, _s, (size_t)_n);
      else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4u) = MEM32(esi + _i*4u); }
      esi += ecx * 4u; edi += ecx * 4u; }
    else { uint32_t _i; int32_t _st = RECOMP_DF_STEP(4);
      for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*_st) = MEM32(esi + _i*_st);
      esi += ecx * _st; edi += ecx * _st; }
    ecx = 0; } /* rep movsd */
    ecx = MEM32(ebx + 0x2C);
    MEM32(esp + 0x10) = eax;
    eax = MEM32(ebx + 0x28);
    PUSH32(esp, ebp);
    MEM32(ebx + 0x28) = ecx;
    PUSH32(esp, edx);
    MEM32(ebx + 0x2C) = eax;
    PUSH32(esp, 0x000D0BC7u); RECOMP_ABI_CALL(0x00019680u, sub_00019680); /* call 0x00019680 */

loc_000D0BC7: ;
    RECOMP_PC(0x000D0BC7u);
    eax = MEM32(ebx + 0x30);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edi (32-bit) */
    MEM32(esp + 0x28) = edi;
    if (CMP_LE(_fas, _fbs)) goto loc_000D0C3D; /* jle: less or equal (signed <=) */

loc_000D0BD7: ;
    RECOMP_PC(0x000D0BD7u);
    eax = MEM32(ebx + 0x24);
    esi = MEM32(ebx + 0x28);
    _fb = (uint32_t)(edi) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + edi;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x1DAEB4);
    PUSH32(esp, eax);
    eax = esp + 0x20;
    PUSH32(esp, eax);
    _fb = (uint32_t)(edi) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + edi;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x000D0BF1u); RECOMP_ABI_CALL(0x000CE420u, sub_000CE420); /* call 0x000CE420 */

loc_000D0BF1: ;
    RECOMP_PC(0x000D0BF1u);
    ecx = MEM32(eax);
    MEM32(esi) = ecx;
    edx = MEM32(eax + 4);
    MEM32(esi + 4) = edx;
    eax = MEM32(eax + 8);
    MEM32(esi + 8) = eax;
    ecx = MEM32(ebx + 0x28);
    fp_push(MEMF(edi + ecx + 8)); /* fld float */
    edx = MEM32(ebx + 0x24);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ebp + 8)); /* fmul dword ptr [ebp + 8] */
    eax = edi + ecx;
    fp_push(MEMF(eax + 4)); /* fld float */
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ebp + 4)); /* fmul dword ptr [ebp + 4] */
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edi = edi + 0x10;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    fp_push(MEMF(ebp)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(eax)); /* fmul dword ptr [eax] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    fp_top() = RECOMP_FP_PC(MEMF(edx + edi + -4) - fp_top()); /* fsubr dword ptr [edx + edi - 4] */
    MEMF(eax + 0xC) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esp + 0x28);
    ecx = MEM32(ebx + 0x30);
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ecx (32-bit) */
    MEM32(esp + 0x28) = eax;
    if (CMP_L(_fas, _fbs)) goto loc_000D0BD7; /* jl: less (signed <) */

loc_000D0C3D: ;
    RECOMP_PC(0x000D0C3Du);
    eax = MEM32(esp + 0x10);
    edi = eax;
    esi = ebx + 0x38;
    ecx = 6;
    { uint64_t _n = (uint64_t)ecx * 4u;
    if (!g_df && esi < 0xFD000000u && edi < 0xFD000000u && (uint64_t)esi + _n <= 0xFD000000u && (uint64_t)edi + _n <= 0xFD000000u) {
      uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi);
      if ((uint64_t)edi + _n <= esi || (uint64_t)esi + _n <= edi) memcpy(_d, _s, (size_t)_n);
      else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4u) = MEM32(esi + _i*4u); }
      esi += ecx * 4u; edi += ecx * 4u; }
    else { uint32_t _i; int32_t _st = RECOMP_DF_STEP(4);
      for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*_st) = MEM32(esi + _i*_st);
      esi += ecx * _st; edi += ecx * _st; }
    ecx = 0; } /* rep movsd */
    fp_push(MEMF(eax)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebp)); /* fadd dword ptr [ebp] */
    MEMF(eax) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ebp)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 0xC)); /* fadd dword ptr [eax + 0xc] */
    MEMF(eax + 0xC) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(eax + 4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebp + 4)); /* fadd dword ptr [ebp + 4] */
    ecx = MEM32(esp + 0x14);
    PUSH32(esp, eax);
    MEMF(eax + 4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(eax + 0x10)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebp + 4)); /* fadd dword ptr [ebp + 4] */
    MEMF(eax + 0x10) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(eax + 8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebp + 8)); /* fadd dword ptr [ebp + 8] */
    MEMF(eax + 8) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(eax + 0x14)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebp + 8)); /* fadd dword ptr [ebp + 8] */
    MEMF(eax + 0x14) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x000D0C8Bu); RECOMP_ABI_CALL(0x00019480u, sub_00019480); /* call 0x00019480 */

loc_000D0C8B: ;
    RECOMP_PC(0x000D0C8Bu);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    _fb = (uint32_t)(0x14) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x14;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 12; return; /* ret 8 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_0006F5F0
 * Original: 0x0006F5F0 - 0x0006F74B (347 bytes, 105 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_0006F5F0(void)
{
    Qpr2GameTiming _game_time = qpr2_game_timing_begin(0x0006F5F0u, ecx, MEM32(esp + 4), MEM32(ecx + 0x608));
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_0006F5F0: ;
    RECOMP_PC(0x0006F5F0u);
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x1C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebp);
    ebp = MEM32(esp + 0x24);
    PUSH32(esp, esi);
    PUSH32(esp, ebp);
    esi = ecx;
    { Qpr2GameTiming _child_time = qpr2_game_timing_begin(0x0006D490u, ecx, MEM32(esp), 0);
    PUSH32(esp, 0x0006F601u); RECOMP_ABI_CALL(0x0006D490u, sub_0006D490); /* call 0x0006D490 */
    qpr2_game_timing_end(&_child_time); }

loc_0006F601: ;
    RECOMP_PC(0x0006F601u);
    eax = MEM32(esi + 0x65C);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0006F612; /* je: equal / zero */

loc_0006F60B: ;
    RECOMP_PC(0x0006F60Bu);
    ecx = esi;
    PUSH32(esp, 0x0006F612u); RECOMP_ABI_CALL(0x000D9190u, sub_000D9190); /* call 0x000D9190 */

loc_0006F612: ;
    RECOMP_PC(0x0006F612u);
    PUSH32(esp, edi);
    PUSH32(esp, ebp);
    ecx = esi;
    { Qpr2GameTiming _physics_time = qpr2_game_timing_begin(0x000D1970u, ecx, MEM32(esp), MEM32(ecx + 0x1E8));
    PUSH32(esp, 0x0006F61Bu); RECOMP_ABI_CALL(0x000D1970u, sub_000D1970);
    qpr2_game_timing_end(&_physics_time); } /* call 0x000D1970 */

loc_0006F61B: ;
    RECOMP_PC(0x0006F61Bu);
    fp_push(MEMF(esi + 0x60)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 0x94)); /* fsub dword ptr [esi + 0x94] */
    eax = esp + 0x10;
    PUSH32(esp, eax);
    edi = esi + 0x3C;
    MEMF(esp + 0x14) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = esp + 0x20;
    fp_push(MEMF(esi + 0x64)); /* fld float */
    PUSH32(esp, edi);
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 0x98)); /* fsub dword ptr [esi + 0x98] */
    PUSH32(esp, ecx);
    MEMF(esp + 0x20) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esi + 0x68)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 0x9C)); /* fsub dword ptr [esi + 0x9c] */
    MEMF(esp + 0x24) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x0006F655u); RECOMP_ABI_CALL(0x000CE480u, sub_000CE480); /* call 0x000CE480 */

loc_0006F655: ;
    RECOMP_PC(0x0006F655u);
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esp + 0x38)); /* fdiv dword ptr [esp + 0x38] */
    ecx = esp + 0x28;
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(eax)); /* fmul dword ptr [eax] */
    { double _t = fp_st1(); fp_push(_t); } /* fld st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(eax + 4)); /* fmul dword ptr [eax + 4] */
    { double _t = fp_top(); fp_top() = g_fp_stack[(g_fp_top + 2) & 7]; g_fp_stack[(g_fp_top + 2) & 7] = _t; } /* fxch st(2) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(eax + 8)); /* fmul dword ptr [eax + 8] */
    eax = esi + 0x6C;
    PUSH32(esp, eax);
    PUSH32(esp, edi);
    MEMF(esp + 0x2C) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(esp + 0x2C);
    MEMF(esi + 0x7A4) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, ecx);
    MEMF(esi + 0x7A8) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0x7AC) = edx;
    PUSH32(esp, 0x0006F696u); RECOMP_ABI_CALL(0x000CE480u, sub_000CE480); /* call 0x000CE480 */

loc_0006F696: ;
    RECOMP_PC(0x0006F696u);
    edx = MEM32(eax);
    MEM32(esi + 0x7B0) = edx;
    ecx = MEM32(eax + 4);
    MEM32(esi + 0x7B4) = ecx;
    edx = MEM32(eax + 8);
    MEM32(esi + 0x7B8) = edx;
    eax = MEM32(esi + 0x65C);
    _fb = (uint32_t)(0x18) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x18;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0006F6C4; /* je: equal / zero */

loc_0006F6BD: ;
    RECOMP_PC(0x0006F6BDu);
    ecx = esi;
    PUSH32(esp, 0x0006F6C4u); RECOMP_ABI_CALL(0x000D9460u, sub_000D9460); /* call 0x000D9460 */

loc_0006F6C4: ;
    RECOMP_PC(0x0006F6C4u);
    eax = MEM32(esi + 0x608);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_LE(_fas, _fbs)) goto loc_0006F6F5; /* jle: less or equal (signed <=) */

loc_0006F6D0: ;
    RECOMP_PC(0x0006F6D0u);
    PUSH32(esp, ebx);
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_0006F6D3: ;
    RECOMP_PC(0x0006F6D3u);
    eax = MEM32(esi + 0x604);
    edx = MEM32(eax + ebx);
    ecx = eax + ebx;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebp);
    { uint32_t _icall_target = MEM32(edx + 8); PUSH32(esp, 0x0006F6E3u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x0006F6E0u); } /* indirect call */
    }

loc_0006F6E3: ;
    RECOMP_PC(0x0006F6E3u);
    eax = MEM32(esi + 0x608);
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(0x114) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebx = ebx + 0x114;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, eax (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_0006F6D3; /* jl: less (signed <) */

loc_0006F6F4: ;
    RECOMP_PC(0x0006F6F4u);
    POP32(esp, ebx);

loc_0006F6F5: ;
    RECOMP_PC(0x0006F6F5u);
    fp_push(MEMF(esi + 0x630)); /* fld float */
    POP32(esp, edi);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x68)); /* fmul dword ptr [esi + 0x68] */
    fp_push(MEMF(esi + 0x628)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x60)); /* fmul dword ptr [esi + 0x60] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    fp_push(MEMF(esi + 0x600)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1AF338)); /* fmul dword ptr [0x1af338] */
    MEMF(esp + 8) = (float)fp_top(); fp_pop(); /* fstp */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esp + 8)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [esp + 8] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_0006F72B; /* jne: not equal / not zero */

loc_0006F725: ;
    RECOMP_PC(0x0006F725u);
    fp_pop(); /* fstp st(0) */
    fp_push(MEMF(esp + 8)); /* fld float */

loc_0006F72B: ;
    RECOMP_PC(0x0006F72Bu);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x28)); /* fmul dword ptr [esp + 0x28] */
    PUSH32(esp, ebp);
    ecx = esi;
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x5FC)); /* fadd dword ptr [esi + 0x5fc] */
    MEMF(esi + 0x5FC) = (float)fp_top(); fp_pop(); /* fstp */
    { Qpr2GameTiming _child_time = qpr2_game_timing_begin(0x0006E2E0u, ecx, MEM32(esp), 0);
    PUSH32(esp, 0x0006F743u); RECOMP_ABI_CALL(0x0006E2E0u, sub_0006E2E0); /* call 0x0006E2E0 */
    qpr2_game_timing_end(&_child_time); }

loc_0006F743: ;
    RECOMP_PC(0x0006F743u);
    POP32(esp, esi);
    POP32(esp, ebp);
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x1C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; qpr2_game_timing_end(&_game_time); return; /* ret 4 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_0010D020
 * Original: 0x0010D020 - 0x0010D2DA (698 bytes, 188 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_0010D020(void)
{
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)

loc_0010D020: ;
    RECOMP_PC(0x0010D020u);
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x1C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, esi);
    esi = ecx;
    PUSH32(esp, edi);
    MEM32(esi + 0x78) = 1;
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    MEM32(esi + 0x54) = edi;
    fp_push(MEMF(0x1DE75C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 8)); /* fsub dword ptr [esi + 8] */
    PUSH32(esp, 0x1DAEB4);
    fp_push(MEMF(0x1DE760)); /* fld float */
    ecx = esp + 0x10;
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 0xC)); /* fsub dword ptr [esi + 0xc] */
    PUSH32(esp, ecx);
    fp_push(MEMF(0x1DE764)); /* fld float */
    edx = esp + 0x20;
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 0x10)); /* fsub dword ptr [esi + 0x10] */
    PUSH32(esp, edx);
    MEMF(esp + 0x2C) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esp + 0x2C);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esp + 0x20) = eax;
    MEMF(esp + 0x1C) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x0010D078u); RECOMP_ABI_CALL(0x000CE420u, sub_000CE420); /* call 0x000CE420 */

loc_0010D078: ;
    RECOMP_PC(0x0010D078u);
    ecx = MEM32(eax);
    MEM32(esi + 0x7C) = ecx;
    edx = MEM32(eax + 4);
    MEM32(esi + 0x80) = edx;
    eax = MEM32(eax + 8);
    MEM32(esi + 0x84) = eax;
    fp_push(MEMF(esi + 0x84)); /* fld float */
    fp_top() = -fp_top(); /* fchs */
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEMF(esi + 0x38) = (float)fp_top(); /* fst */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1DE6B4)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1de6b4] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_0010D2D1; /* jnp: not parity */

loc_0010D0AE: ;
    RECOMP_PC(0x0010D0AEu);
    fp_push(MEMF(esi + 0x84)); /* fld float */
    PUSH32(esp, ecx);
    fp_push(MEMF(esi + 0x80)); /* fld float */
    fp_push(MEMF(esi + 0x7C)); /* fld float */
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    { double _t = g_fp_stack[(g_fp_top + 2) & 7]; fp_push(_t); } /* fld st(2) */
    fp_top() = RECOMP_FP_PC(fp_top() * g_fp_stack[(g_fp_top + 3) & 7]); /* fmul st(3) */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    { double _t = g_fp_stack[(g_fp_top + 3) & 7]; fp_push(_t); } /* fld st(3) */
    fp_top() = RECOMP_FP_PC(fp_top() * g_fp_stack[(g_fp_top + 4) & 7]); /* fmul st(4) */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    fp_pop(); /* fstp st(0) */
    fp_pop(); /* fstp st(0) */
    fp_pop(); /* fstp st(0) */
    PUSH32(esp, 0x0010D0DCu); RECOMP_ABI_CALL(0x000CDE60u, sub_000CDE60); /* call 0x000CDE60 */

loc_0010D0DC: ;
    RECOMP_PC(0x0010D0DCu);
    fp_push(MEMF(0x1C91D0)); /* fld float */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    fp_top() = RECOMP_FP_PC(fp_top() / fp_st1()); /* fdiv st(1) */
    MEMF(esp + 8) = (float)fp_top(); fp_pop(); /* fstp */
    fp_pop(); /* fstp st(0) */
    fp_push(MEMF(esp + 8)); /* fld float */
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x7C)); /* fmul dword ptr [esi + 0x7c] */
    MEMF(esi + 0x7C) = (float)fp_top(); fp_pop(); /* fstp */
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x80)); /* fmul dword ptr [esi + 0x80] */
    MEMF(esi + 0x80) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x84)); /* fmul dword ptr [esi + 0x84] */
    MEMF(esi + 0x84) = (float)fp_top(); fp_pop(); /* fstp */
    SET_LO8(eax, MEM8(esi + 0x44));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(8) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test LO8(eax), 8 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_0010D14C; /* jne: not equal / not zero */

loc_0010D11A: ;
    RECOMP_PC(0x0010D11Au);
    PUSH32(esp, 0x1DAEB4);
    ecx = esi + 0x14;
    PUSH32(esp, ecx);
    edx = esp + 0x20;
    PUSH32(esp, edx);
    PUSH32(esp, 0x0010D12Du); RECOMP_ABI_CALL(0x000CE420u, sub_000CE420); /* call 0x000CE420 */

loc_0010D12D: ;
    RECOMP_PC(0x0010D12Du);
    ecx = MEM32(eax);
    MEM32(esi + 0x94) = ecx;
    edx = MEM32(eax + 4);
    MEM32(esi + 0x98) = edx;
    eax = MEM32(eax + 8);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esi + 0x9C) = eax;
    goto loc_0010D162;

loc_0010D14C: ;
    RECOMP_PC(0x0010D14Cu);
    MEM32(esi + 0x94) = edi;
    MEM32(esi + 0x98) = edi;
    MEM32(esi + 0x9C) = 0xBF800000u;

loc_0010D162: ;
    RECOMP_PC(0x0010D162u);
    fp_push(MEMF(esi + 0x9C)); /* fld float */
    MEMF(esi + 0x3C) = (float)fp_top(); /* fst */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 1 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_0010D2D1; /* je: equal / zero */

loc_0010D17C: ;
    RECOMP_PC(0x0010D17Cu);
    fp_push(MEMF(esi + 0x84)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x9C)); /* fmul dword ptr [esi + 0x9c] */
    fp_push(MEMF(esi + 0x80)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x98)); /* fmul dword ptr [esi + 0x98] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    fp_push(MEMF(esi + 0x7C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x94)); /* fmul dword ptr [esi + 0x94] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_0010D2D1; /* jnp: not parity */

loc_0010D1B2: ;
    RECOMP_PC(0x0010D1B2u);
    fp_push(MEMF(esi + 0x38)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C4C04)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c4c04] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0010D1CA; /* jp: parity */

loc_0010D1C2: ;
    RECOMP_PC(0x0010D1C2u);
    fp_push(MEMF(0x1C4C08)); /* fld float */
    goto loc_0010D1D0;

loc_0010D1CA: ;
    RECOMP_PC(0x0010D1CAu);
    fp_push(MEMF(0x1C4C0C)); /* fld float */

loc_0010D1D0: ;
    RECOMP_PC(0x0010D1D0u);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 8)); /* fmul dword ptr [esp + 8] */
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    fp_push(MEMF(esp + 0x18)); /* fld float */
    eax = esp;
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    MEMF(esp + 0x24) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x1C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    MEMF(esp + 0x28) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x20)); /* fmul dword ptr [esp + 0x20] */
    fp_push(MEMF(esp + 0x24)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 8)); /* fadd dword ptr [esi + 8] */
    fp_push(MEMF(esp + 0x28)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0xC)); /* fadd dword ptr [esi + 0xc] */
    { double _t = fp_top(); fp_top() = g_fp_stack[(g_fp_top + 2) & 7]; g_fp_stack[(g_fp_top + 2) & 7] = _t; } /* fxch st(2) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x10)); /* fadd dword ptr [esi + 0x10] */
    MEMF(esp + 0x2C) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = MEM32(esp + 0x2C);
    MEMF(esi + 0x88) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0x90) = ecx;
    MEMF(esi + 0x8C) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(esi + 0x88);
    MEM32(eax) = edx;
    ecx = MEM32(esi + 0x8C);
    MEM32(eax + 4) = ecx;
    edx = MEM32(esi + 0x90);
    MEM32(eax + 8) = edx;
    eax = esp + 0x24;
    PUSH32(esp, eax);
    PUSH32(esp, 0x0010D242u); RECOMP_ABI_CALL(0x000E4190u, sub_000E4190); /* call 0x000E4190 */

loc_0010D242: ;
    RECOMP_PC(0x0010D242u);
    ecx = MEM32(eax);
    MEM32(esi + 0xA0) = ecx;
    edx = MEM32(eax + 4);
    MEM32(esi + 0xA4) = edx;
    eax = MEM32(eax + 8);
    MEM32(esi + 0xA8) = eax;
    fp_push(MEMF(esi + 0xA0)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1AF32C)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1af32c] */
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_0010D2D1; /* jnp: not parity */

loc_0010D272: ;
    RECOMP_PC(0x0010D272u);
    fp_push(MEMF(esi + 0xA0)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91D0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91d0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_0010D2D1; /* je: equal / zero */

loc_0010D285: ;
    RECOMP_PC(0x0010D285u);
    fp_push(MEMF(esi + 0xA4)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1AF32C)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1af32c] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_0010D2D1; /* jnp: not parity */

loc_0010D298: ;
    RECOMP_PC(0x0010D298u);
    fp_push(MEMF(esi + 0xA4)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91D0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91d0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_0010D2D1; /* je: equal / zero */

loc_0010D2AB: ;
    RECOMP_PC(0x0010D2ABu);
    fp_push(MEMF(esi + 0xA8)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1AF32C)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1af32c] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_0010D2D1; /* jnp: not parity */

loc_0010D2BE: ;
    RECOMP_PC(0x0010D2BEu);
    fp_push(MEMF(esi + 0xA8)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91D0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91d0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_0010D2D4; /* jne: not equal / not zero */

loc_0010D2D1: ;
    RECOMP_PC(0x0010D2D1u);
    MEM32(esi + 0x78) = edi;

loc_0010D2D4: ;
    RECOMP_PC(0x0010D2D4u);
    POP32(esp, edi);
    POP32(esp, esi);
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x1C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_00115F30
 * Original: 0x00115F30 - 0x001160D6 (422 bytes, 131 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00115F30(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00115F30: ;
    RECOMP_PC(0x00115F30u);
    _fb = (uint32_t)(0x5C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x5C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebx);
    ebx = ecx;
    eax = MEM32(ebx + 0x58);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_LE(_fas, _fbs)) goto loc_001160CE; /* jle: less or equal (signed <=) */

loc_00115F41: ;
    RECOMP_PC(0x00115F41u);
    PUSH32(esp, ebp);
    PUSH32(esp, 0x00115F47u); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_00115F47: ;
    RECOMP_PC(0x00115F47u);
    eax = esp + 0x1C;
    PUSH32(esp, eax);
    ecx = 0x1DE768;
    PUSH32(esp, 0x00115F56u); RECOMP_ABI_CALL(0x000192E0u, sub_000192E0); /* call 0x000192E0 */

loc_00115F56: ;
    RECOMP_PC(0x00115F56u);
    edx = esp + 0x1C;
    ecx = 0x60;
    PUSH32(esp, 0x00115F64u); RECOMP_ABI_CALL(0x00195820u, sub_00195820); /* call 0x00195820 */

loc_00115F64: ;
    RECOMP_PC(0x00115F64u);
    ecx = MEM32(ebx + 0x38);
    edx = MEM32(ebx + 0x34);
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    ecx = ebx;
    PUSH32(esp, 0x00115F73u); RECOMP_ABI_CALL(0x00128A60u, sub_00128A60); /* call 0x00128A60 */

loc_00115F73: ;
    RECOMP_PC(0x00115F73u);
    ebp = MEM32(ebx + 0x50);
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fb = (uint32_t)(MEM32(ebx + 0x54)) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebp, MEM32(ebx + 0x54) (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_001160C1; /* je: equal / zero */

loc_00115F7F: ;
    RECOMP_PC(0x00115F7Fu);
    PUSH32(esp, esi);
    MEM32(esp + 0xC) = 0x3F800000;
    MEM32(esp + 0x10) = 0;
    PUSH32(esp, edi);

loc_00115F91: ;
    RECOMP_PC(0x00115F91u);
    edx = MEM32(ebx + 0x4C);
    esi = ebp;
    esi = (uint32_t)((int32_t)esi * (int32_t)0x34);
    eax = esp + 0x10;
    PUSH32(esp, eax);
    _fb = (uint32_t)(edx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + edx;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ecx = esp + 0x18;
    PUSH32(esp, ecx);
    edi = esi + 0x2C;
    PUSH32(esp, edi);
    PUSH32(esp, 0x00115FAEu); RECOMP_ABI_CALL(0x0003FAA0u, sub_0003FAA0); /* call 0x0003FAA0 */

loc_00115FAE: ;
    RECOMP_PC(0x00115FAEu);
    edx = MEM32(ebx + 0x44);
    eax = MEM32(esi + 0x28);
    ecx = MEM32(edx + eax * 8);
    eax = edx + eax * 8;
    eax = MEM32(eax + 4);
    _fb = (uint32_t)(ecx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    eax = eax - ecx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    MEM32(esp + 0x18) = eax;
    MEM32(esp + 0x1C) = ecx;
    fp_push((double)SMEM32(esp + 0x18)); /* fild */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(edi)); /* fmul dword ptr [edi] */
    PUSH32(esp, 0x00115FD6u); RECOMP_ABI_CALL(0x0012B278u, sub_0012B278); /* call 0x0012B278 */

loc_00115FD6: ;
    RECOMP_PC(0x00115FD6u);
    ecx = MEM32(ebx + 0x3C);
    edi = eax;
    _fb = (uint32_t)(MEM32(esp + 0x1C)) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edi = edi + MEM32(esp + 0x1C);
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x58) = 0;
    edi = edi << 4;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(ecx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edi = edi + ecx;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ecx = MEM32(esi + 0x24);
    MEM32(esi + 0x20) = ecx;
    fp_push(MEMF(esi + 0x18)); /* fld float */
    edx = MEM32(esi + 0x14);
    PUSH32(esp, edx);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    MEMF(esp + 8) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esi + 0x1C)); /* fld float */
    ecx = esp + 0x30;
    MEMF(esp + 4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esi + 0x20)); /* fld float */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x00116016u); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_00116016: ;
    RECOMP_PC(0x00116016u);
    fp_push(MEMF(ebx + 0x2C)); /* fld float */
    eax = MEM32(esp + 0x20);
    MEM32(esp + 0x34) = eax;
    MEM32(esp + 0x2C) = 0;
    MEM32(esp + 0x30) = 0;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x10)); /* fmul dword ptr [esi + 0x10] */
    MEMF(esp + 0x24) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ebx + 0x30)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x10)); /* fmul dword ptr [esi + 0x10] */
    MEMF(esp + 0x28) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = MEM32(edi);
    MEM32(esp + 0x38) = ecx;
    edx = MEM32(edi + 4);
    MEM32(esp + 0x3C) = edx;
    eax = MEM32(edi + 8);
    MEM32(esp + 0x40) = eax;
    ecx = MEM32(edi + 4);
    MEM32(esp + 0x44) = ecx;
    edx = MEM32(edi);
    MEM32(esp + 0x48) = edx;
    eax = MEM32(edi + 0xC);
    MEM32(esp + 0x4C) = eax;
    ecx = MEM32(edi + 8);
    MEM32(esp + 0x50) = ecx;
    edx = MEM32(edi + 0xC);
    MEM32(esp + 0x54) = edx;
    MEM32(esp + 0x64) = 0x40000000;
    MEM32(esp + 0x58) = 4;
    eax = MEM32(esi + 0xC);
    PUSH32(esp, eax);
    ecx = esp + 0x28;
    PUSH32(esp, ecx);
    PUSH32(esp, esi);
    PUSH32(esp, 0x00116097u); RECOMP_ABI_CALL(0x000E80B0u, sub_000E80B0); /* call 0x000E80B0 */

loc_00116097: ;
    RECOMP_PC(0x00116097u);
    eax = MEM32(ebx + 4);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ebp++;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    if ((_fas >= 0)) goto loc_001160A4; /* jns: not sign (positive) */

loc_001160A0: ;
    RECOMP_PC(0x001160A0u);
    _fb = (uint32_t)(eax) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebp = ebp + eax;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    if ((_fas < 0)) goto loc_001160A0; /* js: sign (negative) */

loc_001160A4: ;
    RECOMP_PC(0x001160A4u);
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebp, eax (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_001160B6; /* jl: less (signed <) */

loc_001160A8: ;
    RECOMP_PC(0x001160A8u);
    ecx = eax;
    ecx = (uint32_t)(-(int32_t)ecx);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* neg result */
    /* nop */

loc_001160B0: ;
    RECOMP_PC(0x001160B0u);
    _fb = (uint32_t)(ecx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebp = ebp + ecx;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebp, eax (32-bit) */
    if (CMP_GE(_fas, _fbs)) goto loc_001160B0; /* jge: greater or equal (signed >=) */

loc_001160B6: ;
    RECOMP_PC(0x001160B6u);
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fb = (uint32_t)(MEM32(ebx + 0x54)) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebp, MEM32(ebx + 0x54) (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_00115F91; /* jne: not equal / not zero */

loc_001160BF: ;
    RECOMP_PC(0x001160BFu);
    POP32(esp, edi);
    POP32(esp, esi);

loc_001160C1: ;
    RECOMP_PC(0x001160C1u);
    ecx = ebx;
    PUSH32(esp, 0x001160C8u); RECOMP_ABI_CALL(0x00128AF0u, sub_00128AF0); /* call 0x00128AF0 */

loc_001160C8: ;
    RECOMP_PC(0x001160C8u);
    PUSH32(esp, 0x001160CDu); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_001160CD: ;
    RECOMP_PC(0x001160CDu);
    POP32(esp, ebp);

loc_001160CE: ;
    RECOMP_PC(0x001160CEu);
    eax = MEM32(ebx + 0x58);
    POP32(esp, ebx);
    _fb = (uint32_t)(0x5C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x5C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_0010E930
 * Original: 0x0010E930 - 0x0010EAD6 (422 bytes, 128 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_0010E930(void)
{
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)

loc_0010E930: ;
    RECOMP_PC(0x0010E930u);
    _fb = (uint32_t)(0x20) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x20;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebx);
    ebx = MEM32(0x1DE448);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    PUSH32(esp, 0);
    esi = ecx;
    PUSH32(esp, 0x0010E945u); RECOMP_ABI_CALL(0x000E4CA0u, sub_000E4CA0); /* call 0x000E4CA0 */

loc_0010E945: ;
    RECOMP_PC(0x0010E945u);
    fp_push(MEMF(esi + 0x10)); /* fld float */
    eax = MEM32(esi + 8);
    MEMF(esp + 0x1C) = (float)fp_top(); /* fst */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x1C)); /* fmul dword ptr [esp + 0x1c] */
    MEM32(esp + 0x14) = eax;
    fp_push(MEMF(esp + 0x14)); /* fld float */
    MEM32(esp + 0x18) = 0;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x14)); /* fmul dword ptr [esp + 0x14] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x0010E971u); RECOMP_ABI_CALL(0x000CDE60u, sub_000CDE60); /* call 0x000CDE60 */

loc_0010E971: ;
    RECOMP_PC(0x0010E971u);
    fp_top() = RECOMP_FP_PC(MEMF(0x1AE140) / fp_top()); /* fdivr dword ptr [0x1ae140] */
    ecx = MEM32(esi + 0xB4);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    fp_push(MEMF(esp + 0x1C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    fp_push(MEMF(esp + 0x20)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * g_fp_stack[(g_fp_top + 2) & 7]); /* fmul st(2) */
    MEMF(esp + 0x20) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x24)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * g_fp_stack[(g_fp_top + 2) & 7]); /* fmul st(2) */
    MEMF(esp + 0x24) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x1DE75C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + fp_st1()); /* fadd st(1) */
    MEMF(esp + 0x1C) = (float)fp_top(); fp_pop(); /* fstp */
    fp_pop(); /* fstp st(0) */
    fp_pop(); /* fstp st(0) */
    fp_push(MEMF(esp + 0x20)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x1DE760)); /* fadd dword ptr [0x1de760] */
    MEMF(esp + 0x20) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x24)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x1DE764)); /* fadd dword ptr [0x1de764] */
    MEMF(esp + 0x24) = (float)fp_top(); fp_pop(); /* fstp */
    edi = MEM32(ecx + 0x120);
    fp_push(MEMF(esi + 0x6C)); /* fld float */
    ecx = esp + 0x18;
    MEMF(edi) = (float)fp_top(); /* fst */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x70)); /* fadd dword ptr [esi + 0x70] */
    MEMF(edi + 4) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(esi + 0x68);
    eax = edx;
    PUSH32(esp, eax);
    MEM32(esp + 0x38) = edx;
    PUSH32(esp, 0x0010E9F9u); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_0010E9F9: ;
    RECOMP_PC(0x0010E9F9u);
    ecx = MEM32(esp + 0xC);
    MEM32(edi + 0x10) = ecx;
    fp_push(MEMF(esp + 0x10)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1DE75C)); /* fsub dword ptr [0x1de75c] */
    fp_push(MEMF(esp + 0x14)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1DE760)); /* fsub dword ptr [0x1de760] */
    fp_push(MEMF(esp + 0x18)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1DE764)); /* fsub dword ptr [0x1de764] */
    { double _t = g_fp_stack[(g_fp_top + 2) & 7]; fp_push(_t); } /* fld st(2) */
    fp_top() = -fp_top(); /* fchs */
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    fp_top() = RECOMP_FP_PC(fp_top() * g_fp_stack[(g_fp_top + 3) & 7]); /* fmul st(3) */
    { double _t = g_fp_stack[(g_fp_top + 2) & 7]; fp_push(_t); } /* fld st(2) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C91E0)); /* fmul dword ptr [0x1c91e0] */
    fp_st1() = RECOMP_FP_PC(fp_st1() - fp_top()); fp_pop(); /* fsubp st(1) */
    MEMF(esp + 0x1C) = (float)fp_top(); fp_pop(); /* fstp */
    { double _t = fp_st1(); fp_push(_t); } /* fld st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() * g_fp_stack[(g_fp_top + 2) & 7]); /* fmul st(2) */
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() * g_fp_stack[(g_fp_top + 4) & 7]); /* fmul st(4) */
    fp_st1() = RECOMP_FP_PC(fp_st1() - fp_top()); fp_pop(); /* fsubp st(1) */
    MEMF(esp + 0x20) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, ecx);
    { double _t = fp_top(); fp_top() = g_fp_stack[(g_fp_top + 2) & 7]; g_fp_stack[(g_fp_top + 2) & 7] = _t; } /* fxch st(2) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C91E0)); /* fmul dword ptr [0x1c91e0] */
    { double _t = fp_top(); fp_top() = g_fp_stack[(g_fp_top + 2) & 7]; g_fp_stack[(g_fp_top + 2) & 7] = _t; } /* fxch st(2) */
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    g_fp_stack[(g_fp_top + 2) & 7] = RECOMP_FP_PC(g_fp_stack[(g_fp_top + 2) & 7] - fp_top()); fp_pop(); /* fsubp st(2) */
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(esp + 0x28) = (float)fp_top(); fp_pop(); /* fstp */
    fp_pop(); /* fstp st(0) */
    fp_push(MEMF(esp + 0x28)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x28)); /* fmul dword ptr [esp + 0x28] */
    fp_push(MEMF(esp + 0x24)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x24)); /* fmul dword ptr [esp + 0x24] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    fp_push(MEMF(esp + 0x20)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x20)); /* fmul dword ptr [esp + 0x20] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x0010EA7Du); RECOMP_ABI_CALL(0x000CDE60u, sub_000CDE60); /* call 0x000CDE60 */

loc_0010EA7D: ;
    RECOMP_PC(0x0010EA7Du);
    fp_top() = RECOMP_FP_PC(MEMF(0x1C91D0) / fp_top()); /* fdivr dword ptr [0x1c91d0] */
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    eax = esp;
    PUSH32(esp, edi);
    fp_push(MEMF(esp + 0x2C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    fp_push(MEMF(esp + 0x30)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * g_fp_stack[(g_fp_top + 2) & 7]); /* fmul st(2) */
    fp_push(MEMF(esp + 0x34)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * g_fp_stack[(g_fp_top + 3) & 7]); /* fmul st(3) */
    MEMF(esp + 0x34) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(esp + 0x34);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEM32(eax + 8) = edx;
    MEMF(eax) = (float)fp_top(); fp_pop(); /* fstp */
    MEMF(eax + 4) = (float)fp_top(); fp_pop(); /* fstp */
    eax = esp + 0x20;
    PUSH32(esp, eax);
    fp_pop(); /* fstp st(0) */
    PUSH32(esp, 0x0010EAB9u); RECOMP_ABI_CALL(0x000E63F0u, sub_000E63F0); /* call 0x000E63F0 */

loc_0010EAB9: ;
    RECOMP_PC(0x0010EAB9u);
    _fb = (uint32_t)(0x14) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x14;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0010EACA; /* je: equal / zero */

loc_0010EAC0: ;
    RECOMP_PC(0x0010EAC0u);
    PUSH32(esp, 0);
    PUSH32(esp, 0x0010EAC7u); RECOMP_ABI_CALL(0x000E4C60u, sub_000E4C60); /* call 0x000E4C60 */

loc_0010EAC7: ;
    RECOMP_PC(0x0010EAC7u);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */

loc_0010EACA: ;
    RECOMP_PC(0x0010EACAu);
    POP32(esp, edi);
    POP32(esp, esi);
    eax = 1;
    POP32(esp, ebx);
    _fb = (uint32_t)(0x20) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x20;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_0010DF60
 * Original: 0x0010DF60 - 0x0010E29A (826 bytes, 215 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_0010DF60(void)
{
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)

loc_0010DF60: ;
    RECOMP_PC(0x0010DF60u);
    _fb = (uint32_t)(0x74) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x74;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    PUSH32(esp, 0x3F800000);
    esi = ecx;
    ecx = MEM32(0x1C4C00);
    eax = MEM32(esi + 0x34);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    MEM32(esp + 0x14) = ecx;
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    PUSH32(esp, 0x3F800000);
    ecx = esp + 0x1C;
    MEM32(esp + 0x78) = edi;
    MEM32(esp + 0x24) = eax;
    PUSH32(esp, 0x0010DF9Bu); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_0010DF9B: ;
    RECOMP_PC(0x0010DF9Bu);
    fp_push(MEMF(0x1DE6B8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C1F48)); /* fmul dword ptr [0x1c1f48] */
    ecx = MEM32(esi + 0x10);
    eax = MEM32(esi + 0xC);
    edx = MEM32(esi + 8);
    MEMF(esp + 0x10) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esp + 0x24) = ecx;
    fp_push(MEMF(esp + 0x24)); /* fld float */
    MEM32(esp + 0x20) = eax;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x24)); /* fmul dword ptr [esp + 0x24] */
    MEM32(esp + 0x1C) = edx;
    fp_push(MEMF(esp + 0x20)); /* fld float */
    PUSH32(esp, ecx);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x24)); /* fmul dword ptr [esp + 0x24] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    fp_push(MEMF(esp + 0x20)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x20)); /* fmul dword ptr [esp + 0x20] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x0010DFE5u); RECOMP_ABI_CALL(0x000CDE60u, sub_000CDE60); /* call 0x000CDE60 */

loc_0010DFE5: ;
    RECOMP_PC(0x0010DFE5u);
    fp_top() = RECOMP_FP_PC(MEMF(esp + 0x14) / fp_top()); /* fdivr dword ptr [esp + 0x14] */
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    eax = esp;
    fp_push(MEMF(esp + 0x28)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    fp_push(MEMF(esp + 0x2C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * g_fp_stack[(g_fp_top + 2) & 7]); /* fmul st(2) */
    MEMF(esp + 0x2C) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x30)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * g_fp_stack[(g_fp_top + 2) & 7]); /* fmul st(2) */
    MEMF(esp + 0x30) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x1DE75C)); /* fadd dword ptr [0x1de75c] */
    MEMF(esp + 0x28) = (float)fp_top(); /* fst */
    fp_st1() = fp_top(); fp_pop(); /* fstp st(1) */
    fp_push(MEMF(esp + 0x2C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x1DE760)); /* fadd dword ptr [0x1de760] */
    MEMF(esp + 0x2C) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x30)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x1DE764)); /* fadd dword ptr [0x1de764] */
    MEMF(esp + 0x30) = (float)fp_top(); fp_pop(); /* fstp */
    MEMF(eax) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(esp + 0x2C);
    MEM32(eax + 4) = edx;
    ecx = MEM32(esp + 0x30);
    edx = esp + 0x34;
    PUSH32(esp, edx);
    MEM32(eax + 8) = ecx;
    PUSH32(esp, 0x0010E04Au); RECOMP_ABI_CALL(0x000E4190u, sub_000E4190); /* call 0x000E4190 */

loc_0010E04A: ;
    RECOMP_PC(0x0010E04Au);
    fp_push(MEMF(esp + 0x18)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C91CC)); /* fmul dword ptr [0x1c91cc] */
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    fp_push(MEMF(esp + 0x30)); /* fld float */
    MEM32(esi + 0x54) = edi;
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1AF32C)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1af32c] */
    ecx = 1;
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_0010E081; /* jnp: not parity */

loc_0010E070: ;
    RECOMP_PC(0x0010E070u);
    fp_push(MEMF(esp + 0x30)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91D0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91d0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_0010E083; /* jne: not equal / not zero */

loc_0010E081: ;
    RECOMP_PC(0x0010E081u);
    ecx = 0; /* xor self */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_0010E083: ;
    RECOMP_PC(0x0010E083u);
    fp_push(MEMF(esp + 0x28)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + fp_st1()); /* fadd st(1) */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1AF32C)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1af32c] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_0010E0A9; /* jnp: not parity */

loc_0010E096: ;
    RECOMP_PC(0x0010E096u);
    fp_push(MEMF(esp + 0x28)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - fp_st1()); /* fsub st(1) */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91D0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91d0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_0010E0AB; /* jne: not equal / not zero */

loc_0010E0A9: ;
    RECOMP_PC(0x0010E0A9u);
    ecx = 0; /* xor self */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_0010E0AB: ;
    RECOMP_PC(0x0010E0ABu);
    fp_push(MEMF(esp + 0x2C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + fp_st1()); /* fadd st(1) */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1AF32C)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1af32c] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_0010E270; /* jnp: not parity */

loc_0010E0C2: ;
    RECOMP_PC(0x0010E0C2u);
    fp_push(MEMF(esp + 0x2C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - fp_st1()); /* fsub st(1) */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91D0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91d0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    fp_pop(); /* fstp st(0) */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_0010E272; /* je: equal / zero */

loc_0010E0DB: ;
    RECOMP_PC(0x0010E0DBu);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, edi (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0010E272; /* je: equal / zero */

loc_0010E0E3: ;
    RECOMP_PC(0x0010E0E3u);
    _fa = (uint32_t)(MEM32(esi + 0xEC)) & 0xFFFFFFFFu; _fb = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(esi + 0xEC), edi (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0010E14A; /* je: equal / zero */

loc_0010E0EB: ;
    RECOMP_PC(0x0010E0EBu);
    eax = MEM32(esi + 4);
    ecx = MEM32(esp + 0x28);
    PUSH32(esp, eax);
    PUSH32(esp, 0x3D4CCCCD);
    PUSH32(esp, 0x3D4CCCCD);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    eax = esp;
    MEM32(eax) = ecx;
    edx = MEM32(esp + 0x44);
    MEM32(eax + 4) = edx;
    ecx = MEM32(esp + 0x48);
    MEM32(eax + 8) = ecx;
    PUSH32(esp, 0x0010E117u); RECOMP_ABI_CALL(0x000E6AA0u, sub_000E6AA0); /* call 0x000E6AA0 */

loc_0010E117: ;
    RECOMP_PC(0x0010E117u);
    MEMF(esi + 0x54) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(0x1DC3CC);
    _fb = (uint32_t)(0x18) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x18;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edi (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0010E13E; /* je: equal / zero */

loc_0010E126: ;
    RECOMP_PC(0x0010E126u);
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = edi;
    PUSH32(esp, 0x0010E138u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_0010E138: ;
    RECOMP_PC(0x0010E138u);
    MEM32(0x1A51F8) = edi;

loc_0010E13E: ;
    RECOMP_PC(0x0010E13Eu);
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 0x54)); /* fsub dword ptr [esi + 0x54] */
    MEMF(esi + 0x54) = (float)fp_top(); fp_pop(); /* fstp */

loc_0010E14A: ;
    RECOMP_PC(0x0010E14Au);
    fp_push(MEMF(esi + 0x54)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C4BF0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c4bf0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_0010E27A; /* jne: not equal / not zero */

loc_0010E15E: ;
    RECOMP_PC(0x0010E15Eu);
    edx = MEM32(esi + 0xB4);
    eax = MEM32(edx + 0x108);
    PUSH32(esp, eax);
    PUSH32(esp, edi);
    PUSH32(esp, 0x0010E171u); RECOMP_ABI_CALL(0x000E6A00u, sub_000E6A00); /* call 0x000E6A00 */

loc_0010E171: ;
    RECOMP_PC(0x0010E171u);
    fp_push(MEMF(esi + 0x2C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x54)); /* fmul dword ptr [esi + 0x54] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1AE830)); /* fmul dword ptr [0x1ae830] */
    PUSH32(esp, 0x0010E182u); RECOMP_ABI_CALL(0x0012B278u, sub_0012B278); /* call 0x0012B278 */

loc_0010E182: ;
    RECOMP_PC(0x0010E182u);
    fp_push(MEMF(esp + 0x1C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x54)); /* fmul dword ptr [esi + 0x54] */
    MEM8(esp + 0x17) = LO8(eax);
    eax = MEM32(esp + 0x70);
    ecx = MEM32(esp + 0x14);
    eax = eax | 3;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    MEM32(esp + 0x70) = eax;
    MEM32(esp + 0x4C) = ecx;
    MEM32(esp + 0x1C) = 0;
    edx = MEM32(esp + 0x1C);
    MEM32(esp + 0x20) = 0;
    eax = MEM32(esp + 0x20);
    MEM32(esp + 0x50) = edx;
    MEM32(esp + 0x1C) = 0x3F800000;
    ecx = MEM32(esp + 0x1C);
    MEM32(esp + 0x20) = 0;
    edx = MEM32(esp + 0x20);
    MEM32(esp + 0x54) = eax;
    MEM32(esp + 0x58) = ecx;
    MEM32(esp + 0x1C) = 0;
    eax = MEM32(esp + 0x1C);
    MEM32(esp + 0x20) = 0x3F800000;
    ecx = MEM32(esp + 0x20);
    MEM32(esp + 0x5C) = edx;
    MEM32(esp + 0x64) = ecx;
    MEM32(esp + 0x1C) = 0x3F800000;
    edx = MEM32(esp + 0x1C);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x10)); /* fmul dword ptr [esp + 0x10] */
    PUSH32(esp, edi);
    ecx = esp + 0x40;
    MEM32(esp + 0x6C) = edx;
    MEMF(esp + 0x40) = (float)fp_top(); /* fst */
    MEM32(esp + 0x64) = eax;
    MEMF(esp + 0x44) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, ecx);
    edx = esp + 0x2C;
    MEM32(esp + 0x28) = 0x3F800000;
    eax = MEM32(esp + 0x28);
    PUSH32(esp, edx);
    MEM32(esp + 0x50) = 0;
    MEM32(esp + 0x54) = 0;
    MEM32(esp + 0x80) = 0xBF800000u;
    MEM32(esp + 0x84) = 0xBF800000u;
    MEM32(esp + 0x78) = eax;
    PUSH32(esp, 0x0010E262u); RECOMP_ABI_CALL(0x000E80B0u, sub_000E80B0); /* call 0x000E80B0 */

loc_0010E262: ;
    RECOMP_PC(0x0010E262u);
    _fb = (uint32_t)(0x14) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x14;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    eax = 1;
    POP32(esp, edi);
    POP32(esp, esi);
    _fb = (uint32_t)(0x74) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x74;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 4; return; /* ret */

loc_0010E270: ;
    RECOMP_PC(0x0010E270u);
    fp_pop(); /* fstp st(0) */

loc_0010E272: ;
    RECOMP_PC(0x0010E272u);
    MEM32(esi + 0x54) = edi;
    goto loc_0010E14A;

loc_0010E27A: ;
    RECOMP_PC(0x0010E27Au);
    eax = MEM32(esi + 0xB4);
    eax = MEM32(eax + 0x1C);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 4 (32-bit) */
    if (CMP_GE(_fas, _fbs)) goto loc_0010E292; /* jge: greater or equal (signed >=) */

loc_0010E288: ;
    RECOMP_PC(0x0010E288u);
    fp_push(MEMF(0x1C4BF8)); /* fld float */
    MEMF(esi + eax * 4 + 0x58) = (float)fp_top(); fp_pop(); /* fstp */

loc_0010E292: ;
    RECOMP_PC(0x0010E292u);
    POP32(esp, edi);
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    POP32(esp, esi);
    _fb = (uint32_t)(0x74) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x74;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_001284F0
 * Original: 0x001284F0 - 0x00128798 (680 bytes, 201 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_001284F0(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_001284F0: ;
    RECOMP_PC(0x001284F0u);
    _fb = (uint32_t)(0xC8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0xC8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebp);
    PUSH32(esp, edi);
    edi = ecx;
    eax = MEM32(edi + 0x4C);
    ebp = 0; /* xor self */
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebp (32-bit) */
    MEM32(esp + 8) = 0x1AF33C;
    MEM32(esp + 0xC) = ebp;
    if (CMP_LE(_fas, _fbs)) goto loc_0012878C; /* jle: less or equal (signed <=) */

loc_00128513: ;
    RECOMP_PC(0x00128513u);
    eax = esp + 0x90;
    PUSH32(esp, eax);
    ecx = 0x1DE768;
    PUSH32(esp, 0x00128525u); RECOMP_ABI_CALL(0x000192E0u, sub_000192E0); /* call 0x000192E0 */

loc_00128525: ;
    RECOMP_PC(0x00128525u);
    edx = esp + 0x90;
    ecx = 0x60;
    PUSH32(esp, 0x00128536u); RECOMP_ABI_CALL(0x00195820u, sub_00195820); /* call 0x00195820 */

loc_00128536: ;
    RECOMP_PC(0x00128536u);
    PUSH32(esp, 0x0012853Bu); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_0012853B: ;
    RECOMP_PC(0x0012853Bu);
    ecx = MEM32(edi + 0x28);
    PUSH32(esp, ecx);
    PUSH32(esp, ebp);
    ecx = edi;
    PUSH32(esp, 0x00128547u); RECOMP_ABI_CALL(0x00128A60u, sub_00128A60); /* call 0x00128A60 */

loc_00128547: ;
    RECOMP_PC(0x00128547u);
    _fa = (uint32_t)(MEM32(edi + 0x4C)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(edi + 0x4C), ebp (32-bit) */
    if (CMP_LE(_fas, _fbs)) goto loc_00128780; /* jle: less or equal (signed <=) */

loc_00128550: ;
    RECOMP_PC(0x00128550u);
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    ebx = 4;

loc_00128557: ;
    RECOMP_PC(0x00128557u);
    eax = MEM32(edi + 0x48);
    _fb = (uint32_t)(ebp) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + ebp;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    edx = ((int32_t)eax < 0) ? 0xFFFFFFFF : 0; /* cdq */
    { int64_t _dividend = ((int64_t)(int32_t)edx << 32) | eax;
      eax = (uint32_t)((int32_t)(_dividend / (int32_t)MEM32(edi + 0x44)));
      edx = (uint32_t)((int32_t)(_dividend % (int32_t)MEM32(edi + 0x44))); }
    eax = MEM32(edi + 0x40);
    edx = edx + edx * 8;
    fp_push(MEMF(eax + edx * 4 + 0x18)); /* fld float */
    esi = eax + edx * 4;
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_00128772; /* jp: parity */

loc_0012857E: ;
    RECOMP_PC(0x0012857Eu);
    fp_push(MEMF(esi + 0x10)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esi + 0x14)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esi + 0x14] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 1 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_00128772; /* je: equal / zero */

loc_0012858F: ;
    RECOMP_PC(0x0012858Fu);
    fp_push(MEMF(esi + 0x10)); /* fld float */
    edx = ZX8(MEM8(esi + 0xF));
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esi + 0x14)); /* fdiv dword ptr [esi + 0x14] */
    ecx = MEM32(esi + 0xC);
    MEM32(esp + 0x34) = edx;
    MEM32(esp + 0x20) = ecx;
    fp_top() = RECOMP_FP_PC(MEMF(0x1C91D0) - fp_top()); /* fsubr dword ptr [0x1c91d0] */
    fp_top() = RECOMP_FP_PC(fp_top() * (double)SMEM32(esp + 0x34)); /* fimul dword ptr [esp + 0x34] */
    PUSH32(esp, 0x001285B3u); RECOMP_ABI_CALL(0x0012B278u, sub_0012B278); /* call 0x0012B278 */

loc_001285B3: ;
    RECOMP_PC(0x001285B3u);
    PUSH32(esp, 1);
    PUSH32(esp, 0);
    MEM8(esp + 0x2B) = LO8(eax);
    PUSH32(esp, 0);
    eax = esp + 0x28;
    PUSH32(esp, eax);
    PUSH32(esp, ebx);
    MEM32(esp + 0x2C) = ebx;
    MEM32(esp + 0x28) = ebx;
    PUSH32(esp, 0x001285D0u); RECOMP_ABI_CALL(0x000FAF60u, sub_000FAF60); /* call 0x000FAF60 */

loc_001285D0: ;
    RECOMP_PC(0x001285D0u);
    fp_push(MEMF(esi + 0x1C)); /* fld float */
    eax = MEM32(esp + 0x28);
    fp_top() = -fp_top(); /* fchs */
    MEMF(esp + 0x88) = (float)fp_top(); /* fst */
    _fb = (uint32_t)(0x14) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x14;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    fp_push(MEMF(esp + 0x74)); /* fld float */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi)); /* fadd dword ptr [esi] */
    fp_push(MEMF(esi + 4)); /* fld float */
    { double _t = fp_top(); fp_top() = g_fp_stack[(g_fp_top + 2) & 7]; g_fp_stack[(g_fp_top + 2) & 7] = _t; } /* fxch st(2) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 8)); /* fadd dword ptr [esi + 8] */
    MEM32(esp + 0x14) = eax;
    eax = MEM32(esp + 0x1C);
    MEMF(esp + 0x88) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = MEM32(esp + 0x88);
    MEMF(eax) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(eax + 8) = ecx;
    MEMF(eax + 4) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esp + 0x1C);
    edx = MEM32(edi + 0x2C);
    MEM32(eax + 0x1C) = edx;
    edx = MEM32(esp + 0x1C);
    ecx = MEM32(edi + 0x30);
    eax = MEM32(esp + 0x20);
    MEM32(edx + 0x20) = ecx;
    ecx = MEM32(esp + 0x1C);
    MEM32(ecx + 0x18) = eax;
    fp_push(MEMF(edi + 0x30)); /* fld float */
    ecx = MEM32(esp + 0x1C);
    edx = MEM32(edi + 0x34);
    _fb = (uint32_t)(0x34) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ecx = ecx + 0x34;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x1C) = ecx;
    MEMF(esp + 0x28) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esi + 0x1C)); /* fld float */
    MEM32(esp + 0x24) = edx;
    fp_push(MEMF(esi + 0x1C)); /* fld float */
    edx = MEM32(esp + 0x14);
    fp_top() = -fp_top(); /* fchs */
    edx--;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    MEMF(esp + 0x40) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi)); /* fadd dword ptr [esi] */
    fp_push(MEMF(esi + 4)); /* fld float */
    fp_push(MEMF(esp + 0x40)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 8)); /* fadd dword ptr [esi + 8] */
    MEM32(esp + 0x14) = edx;
    MEMF(esp + 0x64) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(esp + 0x64);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(ecx) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(ecx + 8) = edx;
    MEMF(ecx + 4) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = MEM32(esp + 0x24);
    edx = MEM32(esp + 0x1C);
    MEM32(edx + 0x1C) = ecx;
    ecx = MEM32(esp + 0x28);
    edx = MEM32(esp + 0x1C);
    MEM32(edx + 0x20) = ecx;
    ecx = MEM32(esp + 0x1C);
    MEM32(ecx + 0x18) = eax;
    fp_push(MEMF(edi + 0x38)); /* fld float */
    ecx = MEM32(esp + 0x1C);
    edx = MEM32(edi + 0x2C);
    _fb = (uint32_t)(0x34) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ecx = ecx + 0x34;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x1C) = ecx;
    MEM32(esp + 0x2C) = edx;
    edx = MEM32(esi + 0x1C);
    MEM32(esp + 0x94) = edx;
    edx = MEM32(esp + 0x14);
    MEMF(esp + 0x30) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esi + 0x1C)); /* fld float */
    edx--;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    fp_top() = -fp_top(); /* fchs */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi)); /* fadd dword ptr [esi] */
    fp_push(MEMF(esi + 4)); /* fld float */
    fp_push(MEMF(esp + 0x94)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 8)); /* fadd dword ptr [esi + 8] */
    MEM32(esp + 0x14) = edx;
    MEMF(esp + 0x4C) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(esp + 0x4C);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(ecx) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(ecx + 8) = edx;
    MEMF(ecx + 4) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(esp + 0x1C);
    ecx = MEM32(esp + 0x2C);
    MEM32(edx + 0x1C) = ecx;
    edx = MEM32(esp + 0x1C);
    ecx = MEM32(esp + 0x30);
    MEM32(edx + 0x20) = ecx;
    ecx = MEM32(esp + 0x1C);
    MEM32(ecx + 0x18) = eax;
    ecx = MEM32(esp + 0x1C);
    _fb = (uint32_t)(0x34) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ecx = ecx + 0x34;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x1C) = ecx;
    fp_push(MEMF(esi + 0x1C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi)); /* fadd dword ptr [esi] */
    edx = MEM32(esi + 0x1C);
    fp_push(MEMF(esi + 4)); /* fld float */
    MEM32(esp + 0x58) = edx;
    edx = MEM32(esp + 0x14);
    fp_push(MEMF(esp + 0x58)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 8)); /* fadd dword ptr [esi + 8] */
    edx--;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    MEM32(esp + 0x14) = edx;
    MEMF(esp + 0x70) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(esp + 0x70);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(ecx) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(ecx + 8) = edx;
    MEMF(ecx + 4) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(esp + 0x1C);
    ecx = MEM32(edi + 0x34);
    MEM32(edx + 0x1C) = ecx;
    ecx = MEM32(edi + 0x38);
    edx = MEM32(esp + 0x1C);
    MEM32(edx + 0x20) = ecx;
    ecx = MEM32(esp + 0x1C);
    MEM32(ecx + 0x18) = eax;
    eax = MEM32(esp + 0x1C);
    _fb = (uint32_t)(0x34) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + 0x34;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ecx = esp + 0x10;
    MEM32(esp + 0x1C) = eax;
    PUSH32(esp, 0x00128772u); RECOMP_ABI_CALL(0x00019D40u, sub_00019D40); /* call 0x00019D40 */

loc_00128772: ;
    RECOMP_PC(0x00128772u);
    eax = MEM32(edi + 0x4C);
    ebp++;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebp, eax (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_00128557; /* jl: less (signed <) */

loc_0012877E: ;
    RECOMP_PC(0x0012877Eu);
    POP32(esp, esi);
    POP32(esp, ebx);

loc_00128780: ;
    RECOMP_PC(0x00128780u);
    ecx = edi;
    PUSH32(esp, 0x00128787u); RECOMP_ABI_CALL(0x00128AF0u, sub_00128AF0); /* call 0x00128AF0 */

loc_00128787: ;
    RECOMP_PC(0x00128787u);
    PUSH32(esp, 0x0012878Cu); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_0012878C: ;
    RECOMP_PC(0x0012878Cu);
    eax = MEM32(edi + 0x4C);
    POP32(esp, edi);
    POP32(esp, ebp);
    _fb = (uint32_t)(0xC8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_0007F390
 * Original: 0x0007F390 - 0x0007F676 (742 bytes, 194 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_0007F390(void)
{
    Qpr2GameTiming _game_time = qpr2_game_timing_begin(0x0007F390u, ecx, MEM32(esp + 4), 0);
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)

loc_0007F390: ;
    RECOMP_PC(0x0007F390u);
    _fb = (uint32_t)(0x2C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x2C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, esi);
    esi = ecx;
    eax = MEM32(esi + 0x10);
    MEM32(esi + 0xA8) = eax;
    ecx = MEM32(esi + 0x14);
    eax = MEM32(esp + 0x34);
    MEM32(esi + 0xAC) = ecx;
    edx = MEM32(esi + 0x18);
    PUSH32(esp, eax);
    ecx = esi;
    MEM32(esi + 0xB0) = edx;
    PUSH32(esp, 0x0007F3BDu); RECOMP_ABI_CALL(0x00080980u, sub_00080980); /* call 0x00080980 */

loc_0007F3BD: ;
    RECOMP_PC(0x0007F3BDu);
    fp_push(MEMF(esi + 0x60)); /* fld float */
    ecx = MEM32(esi + 0x54);
    MEM32(esi + 0x64) = ecx;
    edx = MEM32(esi + 0x58);
    MEM32(esi + 0x68) = edx;
    eax = MEM32(esi + 0x5C);
    MEMF(esi + 0x70) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0x6C) = eax;
    fp_push(MEMF(esi + 0x48)); /* fld float */
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    PUSH32(esp, ecx);
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + fp_top()); /* fadd st(0), st(0) */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    fp_pop(); /* fstp st(0) */
    PUSH32(esp, 0x0007F3E9u); RECOMP_ABI_CALL(0x000CDE60u, sub_000CDE60); /* call 0x000CDE60 */

loc_0007F3E9: ;
    RECOMP_PC(0x0007F3E9u);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0xA4)); /* fadd dword ptr [esi + 0xa4] */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEMF(esp + 4) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = MEM32(esp + 4);
    fp_push(MEMF(esp + 4)); /* fld float */
    MEM32(esi + 0x60) = ecx;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x34)); /* fmul dword ptr [esi + 0x34] */
    ecx = esi;
    fp_push(MEMF(esp + 4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x38)); /* fmul dword ptr [esi + 0x38] */
    MEMF(esp + 0x1C) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x3C)); /* fmul dword ptr [esi + 0x3c] */
    MEMF(esp + 0x20) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x10)); /* fadd dword ptr [esi + 0x10] */
    fp_push(MEMF(esp + 0x1C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x14)); /* fadd dword ptr [esi + 0x14] */
    fp_push(MEMF(esp + 0x20)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x18)); /* fadd dword ptr [esi + 0x18] */
    MEMF(esp + 0x20) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(esp + 0x20);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(esi + 0x54) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0x5C) = edx;
    MEMF(esi + 0x58) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x18)); /* fadd dword ptr [esi + 0x18] */
    fp_push(MEMF(esi + 0x18)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 4)); /* fsub dword ptr [esp + 4] */
    fp_push(MEMF(esp + 4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x14)); /* fadd dword ptr [esi + 0x14] */
    fp_push(MEMF(esi + 0x14)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 4)); /* fsub dword ptr [esp + 4] */
    fp_push(MEMF(esp + 4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x10)); /* fadd dword ptr [esi + 0x10] */
    fp_push(MEMF(esi + 0x10)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 4)); /* fsub dword ptr [esp + 4] */
    MEMF(esi + 0x74) = (float)fp_top(); fp_pop(); /* fstp */
    MEMF(esi + 0x80) = (float)fp_top(); fp_pop(); /* fstp */
    MEMF(esi + 0x78) = (float)fp_top(); fp_pop(); /* fstp */
    MEMF(esi + 0x84) = (float)fp_top(); fp_pop(); /* fstp */
    MEMF(esi + 0x7C) = (float)fp_top(); fp_pop(); /* fstp */
    MEMF(esi + 0x88) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x0007F48Au); RECOMP_ABI_CALL(0x0007F1C0u, sub_0007F1C0); /* call 0x0007F1C0 */

loc_0007F48A: ;
    RECOMP_PC(0x0007F48Au);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0007F4B7; /* je: equal / zero */

loc_0007F48E: ;
    RECOMP_PC(0x0007F48Eu);
    eax = MEM32(esi + 0xB4);
    MEM32(esi + 0xC0) = eax;
    ecx = MEM32(esi + 0xB8);
    MEM32(esi + 0xC4) = ecx;
    edx = MEM32(esi + 0xBC);
    MEM32(esi + 0xC8) = edx;
    goto loc_0007F53C;

loc_0007F4B7: ;
    RECOMP_PC(0x0007F4B7u);
    eax = MEM32(esi + 0xCC);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0007F53C; /* je: equal / zero */

loc_0007F4C1: ;
    RECOMP_PC(0x0007F4C1u);
    fp_push(MEMF(esi + 0xC0)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1BBB0C)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1bbb0c] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0007F4EA; /* jp: parity */

loc_0007F4D4: ;
    RECOMP_PC(0x0007F4D4u);
    fp_push(MEMF(esp + 0x34)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1AF318)); /* fmul dword ptr [0x1af318] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0xC0)); /* fadd dword ptr [esi + 0xc0] */
    MEMF(esi + 0xC0) = (float)fp_top(); fp_pop(); /* fstp */

loc_0007F4EA: ;
    RECOMP_PC(0x0007F4EAu);
    fp_push(MEMF(esi + 0xC4)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1BBB0C)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1bbb0c] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0007F513; /* jp: parity */

loc_0007F4FD: ;
    RECOMP_PC(0x0007F4FDu);
    fp_push(MEMF(esp + 0x34)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1AF318)); /* fmul dword ptr [0x1af318] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0xC4)); /* fadd dword ptr [esi + 0xc4] */
    MEMF(esi + 0xC4) = (float)fp_top(); fp_pop(); /* fstp */

loc_0007F513: ;
    RECOMP_PC(0x0007F513u);
    fp_push(MEMF(esi + 0xC8)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1BBB0C)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1bbb0c] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0007F53C; /* jp: parity */

loc_0007F526: ;
    RECOMP_PC(0x0007F526u);
    fp_push(MEMF(esp + 0x34)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1AF318)); /* fmul dword ptr [0x1af318] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0xC8)); /* fadd dword ptr [esi + 0xc8] */
    MEMF(esi + 0xC8) = (float)fp_top(); fp_pop(); /* fstp */

loc_0007F53C: ;
    RECOMP_PC(0x0007F53Cu);
    fp_push(MEMF(esi + 0x60)); /* fld float */
    MEM32(esp + 0xC) = 0x3F800000;
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 0xA4)); /* fsub dword ptr [esi + 0xa4] */
    MEM32(esp + 0x10) = 0x3F800000;
    MEM32(esp + 0x14) = 0x3F800000;
    edx = esp + 4;
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x34)); /* fmul dword ptr [esi + 0x34] */
    { double _t = fp_st1(); fp_push(_t); } /* fld st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x38)); /* fmul dword ptr [esi + 0x38] */
    MEMF(esp + 0x1C) = (float)fp_top(); fp_pop(); /* fstp */
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x3C)); /* fmul dword ptr [esi + 0x3c] */
    MEMF(esp + 0x20) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(MEMF(esi + 0x54) - fp_top()); /* fsubr dword ptr [esi + 0x54] */
    fp_push(MEMF(esi + 0x58)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x1C)); /* fsub dword ptr [esp + 0x1c] */
    fp_push(MEMF(esi + 0x5C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x20)); /* fsub dword ptr [esp + 0x20] */
    MEMF(esp + 0x2C) = (float)fp_top(); fp_pop(); /* fstp */
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 0xA8)); /* fsub dword ptr [esi + 0xa8] */
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 0xAC)); /* fsub dword ptr [esi + 0xac] */
    fp_push(MEMF(esp + 0x2C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 0xB0)); /* fsub dword ptr [esi + 0xb0] */
    MEMF(esp + 0x20) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x34)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0xC8)); /* fmul dword ptr [esi + 0xc8] */
    MEMF(esp + 4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x34)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0xC4)); /* fmul dword ptr [esi + 0xc4] */
    MEMF(esp + 8) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x34)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0xC0)); /* fmul dword ptr [esi + 0xc0] */
    MEMF(esp + 0x34) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x1C91D0)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esp + 4)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esp + 4] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_0007F5EC; /* je: equal / zero */

loc_0007F5E8: ;
    RECOMP_PC(0x0007F5E8u);
    edx = esp + 0xC;

loc_0007F5EC: ;
    RECOMP_PC(0x0007F5ECu);
    fp_push(MEMF(0x1C91D0)); /* fld float */
    ecx = esp + 8;
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esp + 8)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esp + 8] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_0007F605; /* je: equal / zero */

loc_0007F601: ;
    RECOMP_PC(0x0007F601u);
    ecx = esp + 0x10;

loc_0007F605: ;
    RECOMP_PC(0x0007F605u);
    fp_push(MEMF(0x1C91D0)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esp + 0x34)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esp + 0x34] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    eax = esp + 0x34;
    if (TEST_Z(_fa, _fb)) goto loc_0007F61E; /* je: equal / zero */

loc_0007F61A: ;
    RECOMP_PC(0x0007F61Au);
    eax = esp + 0x14;

loc_0007F61E: ;
    RECOMP_PC(0x0007F61Eu);
    fp_push(MEMF(eax)); /* fld float */
    eax = MEM32(edx);
    fp_push(MEMF(ecx)); /* fld float */
    MEM32(esp + 0x2C) = eax;
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x18)); /* fmul dword ptr [esp + 0x18] */
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    MEMF(esp + 0x1C) = (float)fp_top(); fp_pop(); /* fstp */
    fp_pop(); /* fstp st(0) */
    fp_push(MEMF(esp + 0x2C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x20)); /* fmul dword ptr [esp + 0x20] */
    fp_push(MEMF(esp + 0x18)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0xA8)); /* fadd dword ptr [esi + 0xa8] */
    fp_push(MEMF(esp + 0x1C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0xAC)); /* fadd dword ptr [esi + 0xac] */
    { double _t = fp_top(); fp_top() = g_fp_stack[(g_fp_top + 2) & 7]; g_fp_stack[(g_fp_top + 2) & 7] = _t; } /* fxch st(2) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0xB0)); /* fadd dword ptr [esi + 0xb0] */
    MEMF(esp + 0x2C) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = MEM32(esp + 0x2C);
    MEMF(esi + 0x10) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0x18) = ecx;
    MEMF(esi + 0x14) = (float)fp_top(); fp_pop(); /* fstp */
    if(_game_time.function) qpr2_camera_state(&_game_time,
        MEMF(esi+0xA8),MEMF(esi+0xAC),MEMF(esi+0xB0),
        MEMF(esi+0x10),MEMF(esi+0x14),MEMF(esi+0x18));
    POP32(esp, esi);
    _fb = (uint32_t)(0x2C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x2C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; qpr2_game_timing_end(&_game_time); return; /* ret 4 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_000A6B00
 * Original: 0x000A6B00 - 0x000A6CE9 (489 bytes, 129 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000A6B00(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_000A6B00: ;
    RECOMP_PC(0x000A6B00u);
    eax = MEM32(0x1DC3CC);
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    ebp = 0; /* xor self */
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebp (32-bit) */
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    ebx = ecx;
    if (CMP_EQ(_fa, _fb)) goto loc_000A6B49; /* je: equal / zero */

loc_000A6B11: ;
    RECOMP_PC(0x000A6B11u);
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = ebp;
    PUSH32(esp, 0x000A6B23u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000A6B23: ;
    RECOMP_PC(0x000A6B23u);
    _fa = (uint32_t)(MEM32(0x1DC3CC)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC3CC), ebp (32-bit) */
    MEM32(0x1A51F8) = ebp;
    if (CMP_EQ(_fa, _fb)) goto loc_000A6B49; /* je: equal / zero */

loc_000A6B31: ;
    RECOMP_PC(0x000A6B31u);
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = ebp;
    PUSH32(esp, 0x000A6B43u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000A6B43: ;
    RECOMP_PC(0x000A6B43u);
    MEM32(0x1A51F8) = ebp;

loc_000A6B49: ;
    RECOMP_PC(0x000A6B49u);
    eax = MEM32(0x1DC444);
    esi = 0x207;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, esi (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000A6B8F; /* je: equal / zero */

loc_000A6B57: ;
    RECOMP_PC(0x000A6B57u);
    edx = esi;
    ecx = 0x40354;
    MEM32(0x1DC444) = esi;
    PUSH32(esp, 0x000A6B69u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000A6B69: ;
    RECOMP_PC(0x000A6B69u);
    _fa = (uint32_t)(MEM32(0x1DC444)) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC444), esi (32-bit) */
    MEM32(0x1A51DC) = esi;
    if (CMP_EQ(_fa, _fb)) goto loc_000A6B8F; /* je: equal / zero */

loc_000A6B77: ;
    RECOMP_PC(0x000A6B77u);
    edx = esi;
    ecx = 0x40354;
    MEM32(0x1DC444) = esi;
    PUSH32(esp, 0x000A6B89u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000A6B89: ;
    RECOMP_PC(0x000A6B89u);
    MEM32(0x1A51DC) = esi;

loc_000A6B8F: ;
    RECOMP_PC(0x000A6B8Fu);
    eax = MEM32(ebx + 4);
    ecx = MEM32(eax + 0x654);
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, ebp (32-bit) */
    if (CMP_LE(_fas, _fbs)) goto loc_000A6BD3; /* jle: less or equal (signed <=) */

loc_000A6B9E: ;
    RECOMP_PC(0x000A6B9Eu);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_000A6BA0: ;
    RECOMP_PC(0x000A6BA0u);
    eax = MEM32(eax + 0xEC4);
    ecx = MEM32(eax + edi + 0x458);
    _fb = (uint32_t)(edi) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + edi;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, ebp (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000A6BBF; /* je: equal / zero */

loc_000A6BB3: ;
    RECOMP_PC(0x000A6BB3u);
    PUSH32(esp, 0x100);
    ecx = eax;
    PUSH32(esp, 0x000A6BBFu); RECOMP_ABI_CALL(0x00035A90u, sub_00035A90); /* call 0x00035A90 */

loc_000A6BBF: ;
    RECOMP_PC(0x000A6BBFu);
    eax = MEM32(ebx + 4);
    ecx = MEM32(eax + 0x654);
    esi++;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(0x45C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edi = edi + 0x45C;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, ecx (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_000A6BA0; /* jl: less (signed <) */

loc_000A6BD3: ;
    RECOMP_PC(0x000A6BD3u);
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    goto loc_000A6BE0;

    RECOMP_PC(0x000A6BD7u);
    /* nop */
    edi = edi;

loc_000A6BE0: ;
    RECOMP_PC(0x000A6BE0u);
    eax = MEM32(ebx + 4);
    ecx = MEM32(eax + 0xAB0);
    eax = MEM32(ecx + esi + 0x190);
    _fb = (uint32_t)(esi) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ecx = ecx + esi;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebp (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000A6BFB; /* je: equal / zero */

loc_000A6BF6: ;
    RECOMP_PC(0x000A6BF6u);
    edx = MEM32(ecx);
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_target = MEM32(edx + 0x68); PUSH32(esp, 0x000A6BFBu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000A6BF8u); } /* indirect call */
    }

loc_000A6BFB: ;
    RECOMP_PC(0x000A6BFBu);
    _fb = (uint32_t)(0x4E8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + 0x4E8;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(0x1D70) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 0x1D70 (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_000A6BE0; /* jl: less (signed <) */

loc_000A6C09: ;
    RECOMP_PC(0x000A6C09u);
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    edi = 1;

loc_000A6C10: ;
    RECOMP_PC(0x000A6C10u);
    eax = MEM32(ebx + 4);
    ecx = esi + eax + 0xAB4;
    _fa = (uint32_t)(MEM32(ecx + 0xCC)) & 0xFFFFFFFFu; _fb = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(ecx + 0xCC), edi (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000A6C2C; /* jne: not equal / not zero */

loc_000A6C22: ;
    RECOMP_PC(0x000A6C22u);
    PUSH32(esp, 0x100);
    PUSH32(esp, 0x000A6C2Cu); RECOMP_ABI_CALL(0x000F9EA0u, sub_000F9EA0); /* call 0x000F9EA0 */

loc_000A6C2C: ;
    RECOMP_PC(0x000A6C2Cu);
    _fb = (uint32_t)(0xD0) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + 0xD0;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(0x410) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 0x410 (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_000A6C10; /* jl: less (signed <) */

loc_000A6C3A: ;
    RECOMP_PC(0x000A6C3Au);
    ecx = MEM32(ebx + 4);
    edx = MEM32(ecx);
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_target = MEM32(edx + 0x68); PUSH32(esp, 0x000A6C42u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000A6C3Fu); } /* indirect call */
    }

loc_000A6C42: ;
    RECOMP_PC(0x000A6C42u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000A6C5C; /* jne: not equal / not zero */

loc_000A6C46: ;
    RECOMP_PC(0x000A6C46u);
    eax = MEM32(ebx + 4);
    ecx = MEM32(eax + 0x48);
    ecx = MEM32(ecx + 0x44);
    _fa = (uint32_t)(MEM32(ecx + 0x278)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(ecx + 0x278), ebp (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000A6C5C; /* jne: not equal / not zero */

loc_000A6C57: ;
    RECOMP_PC(0x000A6C57u);
    PUSH32(esp, 0x000A6C5Cu); RECOMP_ABI_CALL(0x000204F0u, sub_000204F0); /* call 0x000204F0 */

loc_000A6C5C: ;
    RECOMP_PC(0x000A6C5Cu);
    eax = MEM32(0x1DC444);
    esi = 0x203;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, esi (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000A6CA2; /* je: equal / zero */

loc_000A6C6A: ;
    RECOMP_PC(0x000A6C6Au);
    edx = esi;
    ecx = 0x40354;
    MEM32(0x1DC444) = esi;
    PUSH32(esp, 0x000A6C7Cu); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000A6C7C: ;
    RECOMP_PC(0x000A6C7Cu);
    _fa = (uint32_t)(MEM32(0x1DC444)) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC444), esi (32-bit) */
    MEM32(0x1A51DC) = esi;
    if (CMP_EQ(_fa, _fb)) goto loc_000A6CA2; /* je: equal / zero */

loc_000A6C8A: ;
    RECOMP_PC(0x000A6C8Au);
    edx = esi;
    ecx = 0x40354;
    MEM32(0x1DC444) = esi;
    PUSH32(esp, 0x000A6C9Cu); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000A6C9C: ;
    RECOMP_PC(0x000A6C9Cu);
    MEM32(0x1A51DC) = esi;

loc_000A6CA2: ;
    RECOMP_PC(0x000A6CA2u);
    _fa = (uint32_t)(MEM32(0x1DC3CC)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC3CC), ebp (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000A6CE2; /* jne: not equal / not zero */

loc_000A6CAA: ;
    RECOMP_PC(0x000A6CAAu);
    edx = edi;
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = edi;
    PUSH32(esp, 0x000A6CBCu); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000A6CBC: ;
    RECOMP_PC(0x000A6CBCu);
    _fa = (uint32_t)(MEM32(0x1DC3CC)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC3CC), ebp (32-bit) */
    MEM32(0x1A51F8) = edi;
    if (CMP_NE(_fa, _fb)) goto loc_000A6CE2; /* jne: not equal / not zero */

loc_000A6CCA: ;
    RECOMP_PC(0x000A6CCAu);
    edx = edi;
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = edi;
    PUSH32(esp, 0x000A6CDCu); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000A6CDC: ;
    RECOMP_PC(0x000A6CDCu);
    MEM32(0x1A51F8) = edi;

loc_000A6CE2: ;
    RECOMP_PC(0x000A6CE2u);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    esp += 8; return; /* ret 4 */

}


/**
 * qpr2_recovered_000C50F0
 * Original: 0x000C50F0 - 0x000C54B9 (969 bytes, 248 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000C50F0(void)
{
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)

loc_000C50F0: ;
    RECOMP_PC(0x000C50F0u);
    _fb = (uint32_t)(0x50) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x50;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, esi);
    esi = ecx;
    _fa = (uint32_t)(MEM32(esi + 0x48)) & 0xFFFFFFFFu; _fb = (uint32_t)(3) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(esi + 0x48), 3 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C54A6; /* je: equal / zero */

loc_000C5100: ;
    RECOMP_PC(0x000C5100u);
    ecx = MEM32(esi + 0x8C);
    eax = MEM32(ecx + 8);
    edx = MEM32(eax);
    MEM32(esp + 0xC) = edx;
    edx = MEM32(eax + 4);
    MEM32(esp + 0x10) = edx;
    fp_push(MEMF(esp + 0x10)); /* fld float */
    edx = MEM32(eax + 0x10);
    MEM32(esp + 4) = edx;
    fp_push(MEMF(esp + 4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0xC)); /* fsub dword ptr [esp + 0xc] */
    eax = MEM32(eax + 0x14);
    MEM32(esp + 8) = eax;
    eax = MEM32(esi + 0x98);
    fp_top() = RECOMP_FP_PC(MEMF(esi + 0x7C) / fp_top()); /* fdivr dword ptr [esi + 0x7c] */
    PUSH32(esp, edi);
    MEMF(esp + 0x10) = (float)fp_top(); /* fst */
    MEMF(esi + 0x23C) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0xC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - fp_st1()); /* fsub st(1) */
    fp_top() = RECOMP_FP_PC(MEMF(esi + 0x80) / fp_top()); /* fdivr dword ptr [esi + 0x80] */
    MEMF(esp + 8) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(esp + 8);
    MEM32(esi + 0x240) = edx;
    eax = MEM32(eax + 8);
    fp_pop(); /* fstp st(0) */
    edx = MEM32(eax + 0x10);
    ecx = MEM32(ecx + 8);
    MEM32(esp + 0x18) = edx;
    edx = MEM32(eax + 0x14);
    MEM32(esp + 0x1C) = edx;
    fp_push(MEMF(esp + 0x1C)); /* fld float */
    edx = MEM32(eax);
    MEM32(esp + 0x20) = edx;
    edx = MEM32(eax + 4);
    MEM32(esp + 0x24) = edx;
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x24)); /* fsub dword ptr [esp + 0x24] */
    edx = MEM32(eax + 0x10);
    MEM32(esp + 0x28) = edx;
    edx = MEM32(eax + 0x14);
    MEM32(esp + 0x2C) = edx;
    edx = MEM32(eax);
    MEM32(esp + 0x30) = edx;
    edx = MEM32(eax + 4);
    MEM32(esp + 0x34) = edx;
    edx = MEM32(ecx + 0x10);
    MEM32(esp + 0x38) = edx;
    edx = MEM32(ecx + 0x14);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 8)); /* fmul dword ptr [esp + 8] */
    fp_push(MEMF(esp + 0x28)); /* fld float */
    MEM32(esp + 0x3C) = edx;
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x30)); /* fsub dword ptr [esp + 0x30] */
    edx = MEM32(eax + 0x10);
    MEM32(esp + 0x40) = edx;
    edx = MEM32(eax + 0x14);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x10)); /* fmul dword ptr [esp + 0x10] */
    fp_push(MEMF(esp + 0x3C)); /* fld float */
    MEM32(esp + 0x44) = edx;
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x44)); /* fsub dword ptr [esp + 0x44] */
    edx = MEM32(eax);
    eax = MEM32(eax + 4);
    MEM32(esp + 0x48) = edx;
    edx = MEM32(ecx);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 8)); /* fmul dword ptr [esp + 8] */
    MEM32(esp + 0x50) = edx;
    MEM32(esp + 0x4C) = eax;
    eax = MEM32(ecx + 4);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x78)); /* fadd dword ptr [esi + 0x78] */
    fp_push(MEMF(esp + 0x48)); /* fld float */
    MEM32(esp + 0x54) = eax;
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x50)); /* fsub dword ptr [esp + 0x50] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x10)); /* fmul dword ptr [esp + 0x10] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x74)); /* fadd dword ptr [esi + 0x74] */
    MEMF(esi + 0x258) = (float)fp_top(); fp_pop(); /* fstp */
    MEMF(esi + 0x25C) = (float)fp_top(); fp_pop(); /* fstp */
    MEMF(esi + 0x260) = (float)fp_top(); fp_pop(); /* fstp */
    MEMF(esi + 0x264) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = MEM32(esi + 0x90);
    eax = MEM32(ecx + 8);
    edx = MEM32(eax + 0x10);
    ecx = MEM32(eax + 0x14);
    MEM32(esp + 0x50) = edx;
    edx = MEM32(eax);
    MEM32(esp + 0x48) = edx;
    edx = MEM32(eax + 0x10);
    MEM32(esp + 0x54) = ecx;
    fp_push(MEMF(esp + 0x54)); /* fld float */
    ecx = MEM32(eax + 4);
    MEM32(esp + 0x40) = edx;
    edx = MEM32(eax);
    MEM32(esp + 0x4C) = ecx;
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x4C)); /* fsub dword ptr [esp + 0x4c] */
    ecx = MEM32(eax + 0x14);
    MEM32(esp + 0x38) = edx;
    edx = MEM32(esi + 0x8C);
    MEM32(esp + 0x44) = ecx;
    ecx = MEM32(eax + 4);
    MEM32(esp + 0x3C) = ecx;
    ecx = MEM32(edx + 8);
    edx = MEM32(ecx + 0x10);
    MEM32(esp + 0x30) = edx;
    edx = MEM32(ecx + 0x14);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x240)); /* fmul dword ptr [esi + 0x240] */
    fp_push(MEMF(esp + 0x40)); /* fld float */
    MEM32(esp + 0x34) = edx;
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x38)); /* fsub dword ptr [esp + 0x38] */
    edx = MEM32(eax + 0x10);
    MEM32(esp + 0x28) = edx;
    edx = MEM32(eax + 0x14);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x23C)); /* fmul dword ptr [esi + 0x23c] */
    fp_push(MEMF(esp + 0x34)); /* fld float */
    MEM32(esp + 0x2C) = edx;
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x2C)); /* fsub dword ptr [esp + 0x2c] */
    edx = MEM32(eax);
    eax = MEM32(eax + 4);
    MEM32(esp + 0x20) = edx;
    edx = MEM32(ecx);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x240)); /* fmul dword ptr [esi + 0x240] */
    MEM32(esp + 0x18) = edx;
    MEM32(esp + 0x24) = eax;
    eax = MEM32(ecx + 4);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x78)); /* fadd dword ptr [esi + 0x78] */
    fp_push(MEMF(esp + 0x20)); /* fld float */
    MEM32(esp + 0x1C) = eax;
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x18)); /* fsub dword ptr [esp + 0x18] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x23C)); /* fmul dword ptr [esi + 0x23c] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x74)); /* fadd dword ptr [esi + 0x74] */
    MEMF(esi + 0x248) = (float)fp_top(); fp_pop(); /* fstp */
    MEMF(esi + 0x24C) = (float)fp_top(); fp_pop(); /* fstp */
    MEMF(esi + 0x250) = (float)fp_top(); fp_pop(); /* fstp */
    MEMF(esi + 0x254) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esi + 0x8C);
    fp_push(MEMF(eax + 0x1C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(eax + 0x14)); /* fdiv dword ptr [eax + 0x14] */
    fp_top() = RECOMP_FP_PC(MEMF(0x1C91D0) - fp_top()); /* fsubr dword ptr [0x1c91d0] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x80)); /* fmul dword ptr [esi + 0x80] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x78)); /* fadd dword ptr [esi + 0x78] */
    fp_push(MEMF(eax + 0x18)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(eax + 0x10)); /* fdiv dword ptr [eax + 0x10] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x7C)); /* fmul dword ptr [esi + 0x7c] */
    ecx = esi;
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x74)); /* fadd dword ptr [esi + 0x74] */
    MEM32(esi + 0x280) = 0xBF800000u;
    MEMF(esi + 0x278) = (float)fp_top(); fp_pop(); /* fstp */
    MEMF(esi + 0x27C) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x000C5338u); RECOMP_ABI_CALL(0x00110470u, sub_00110470); /* call 0x00110470 */

loc_000C5338: ;
    RECOMP_PC(0x000C5338u);
    ecx = eax;
    PUSH32(esp, 0x000C533Fu); RECOMP_ABI_CALL(0x000C6EB0u, sub_000C6EB0); /* call 0x000C6EB0 */

loc_000C533F: ;
    RECOMP_PC(0x000C533Fu);
    edi = MEM32(eax + 0x3C);
    fp_push(MEMF(edi + 0x688)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1AF358)); /* fmul dword ptr [0x1af358] */
    PUSH32(esp, 0x000C5353u); RECOMP_ABI_CALL(0x0012B278u, sub_0012B278); /* call 0x0012B278 */

loc_000C5353: ;
    RECOMP_PC(0x000C5353u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_GE(_fas, _fbs)) goto loc_000C5359; /* jge: greater or equal (signed >=) */

loc_000C5357: ;
    RECOMP_PC(0x000C5357u);
    eax = (uint32_t)(-(int32_t)eax);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* neg result */

loc_000C5359: ;
    RECOMP_PC(0x000C5359u);
    fp_push(MEMF(esi + 0x238)); /* fld float */
    ecx = esi + 0x238;
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    MEM32(esi + 0x224) = eax;
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_000C53A1; /* jnp: not parity */

loc_000C5378: ;
    RECOMP_PC(0x000C5378u);
    fp_push(MEMF(ecx)); /* fld float */
    MEM32(esp + 0x10) = 0;
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x60)); /* fsub dword ptr [esp + 0x60] */
    MEMF(ecx) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ecx)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    eax = esp + 0x10;
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_000C539D; /* jnp: not parity */

loc_000C539B: ;
    RECOMP_PC(0x000C539Bu);
    eax = ecx;

loc_000C539D: ;
    RECOMP_PC(0x000C539Du);
    edx = MEM32(eax);
    MEM32(ecx) = edx;

loc_000C53A1: ;
    RECOMP_PC(0x000C53A1u);
    fp_push(MEMF(esi + 0x228)); /* fld float */
    fp_push(MEMF(edi + 0x808)); /* fld float */
    MEMF(esi + 0x228) = (float)fp_top(); /* fst */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91D0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91d0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_000C53CA; /* jne: not equal / not zero */

loc_000C53C0: ;
    RECOMP_PC(0x000C53C0u);
    MEM32(esi + 0x228) = 0x3F800000;

loc_000C53CA: ;
    RECOMP_PC(0x000C53CAu);
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_000C53F9; /* jnp: not parity */

loc_000C53D7: ;
    RECOMP_PC(0x000C53D7u);
    fp_push(MEMF(esi + 0x228)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000C53F9; /* jp: parity */

loc_000C53EA: ;
    RECOMP_PC(0x000C53EAu);
    eax = MEM32(0x1C1744);
    MEMF(esi + 0x234) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(ecx) = eax;
    goto loc_000C53FB;

loc_000C53F9: ;
    RECOMP_PC(0x000C53F9u);
    fp_pop(); /* fstp st(0) */

loc_000C53FB: ;
    RECOMP_PC(0x000C53FBu);
    fp_push(MEMF(esi + 0x228)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_000C5414; /* jnp: not parity */

loc_000C540E: ;
    RECOMP_PC(0x000C540Eu);
    MEM32(ecx) = 0;

loc_000C5414: ;
    RECOMP_PC(0x000C5414u);
    ecx = esi;
    PUSH32(esp, 0x000C541Bu); RECOMP_ABI_CALL(0x00110470u, sub_00110470); /* call 0x00110470 */

loc_000C541B: ;
    RECOMP_PC(0x000C541Bu);
    ecx = eax;
    PUSH32(esp, 0x000C5422u); RECOMP_ABI_CALL(0x000C6EB0u, sub_000C6EB0); /* call 0x000C6EB0 */

loc_000C5422: ;
    RECOMP_PC(0x000C5422u);
    ecx = MEM32(eax + 0x3C);
    fp_push(MEMF(ecx + 0x1114)); /* fld float */
    MEMF(esi + 0x22C) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x1C174C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x60)); /* fmul dword ptr [esp + 0x60] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x230)); /* fadd dword ptr [esi + 0x230] */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1AF36C)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [0x1af36c] */
    MEMF(esi + 0x230) = (float)fp_top(); /* fst */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_000C5462; /* jne: not equal / not zero */

loc_000C5454: ;
    RECOMP_PC(0x000C5454u);
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1AF36C)); /* fsub dword ptr [0x1af36c] */
    MEMF(esi + 0x230) = (float)fp_top(); fp_pop(); /* fstp */
    goto loc_000C5464;

loc_000C5462: ;
    RECOMP_PC(0x000C5462u);
    fp_pop(); /* fstp st(0) */

loc_000C5464: ;
    RECOMP_PC(0x000C5464u);
    edx = MEM32(esi + 0x224);
    PUSH32(esp, edx);
    eax = esi + 0x2A8;
    PUSH32(esp, 0x1C1238);
    PUSH32(esp, eax);
    PUSH32(esp, 0x000C547Cu); RECOMP_ABI_CALL(0x000E07A0u, sub_000E07A0); /* call 0x000E07A0 */

loc_000C547C: ;
    RECOMP_PC(0x000C547Cu);
    fp_push(MEMF(edi + 0x278)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(edi + 0x26C)); /* fdiv dword ptr [edi + 0x26c] */
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    POP32(esp, edi);
    fp_push(MEMF(0x1C1740)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x1C173C)); /* fadd dword ptr [0x1c173c] */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(MEMF(0x1C173C) - fp_top()); /* fsubr dword ptr [0x1c173c] */
    MEMF(esi + 0x244) = (float)fp_top(); fp_pop(); /* fstp */

loc_000C54A6: ;
    RECOMP_PC(0x000C54A6u);
    ecx = MEM32(0x1DADF0);
    eax = MEM32(esp + 0x58);
    MEM32(eax) = ecx;
    POP32(esp, esi);
    _fb = (uint32_t)(0x50) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x50;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 12; return; /* ret 8 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_000C96B0
 * Original: 0x000C96B0 - 0x000C98CF (543 bytes, 151 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000C96B0(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_000C96B0: ;
    RECOMP_PC(0x000C96B0u);
    _fb = (uint32_t)(0x2C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x2C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    esi = ecx;
    eax = MEM32(esi + 0x2C);
    ecx = MEM32(esi + 0x24);
    PUSH32(esp, edi);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    edx = esi + 0xC;
    PUSH32(esp, edx);
    ecx = esp + 0x30;
    PUSH32(esp, 0x000C96CEu); RECOMP_ABI_CALL(0x00037EF0u, sub_00037EF0); /* call 0x00037EF0 */

loc_000C96CE: ;
    RECOMP_PC(0x000C96CEu);
    edi = MEM32(esp + 0x40);
    eax = esp + 0x24;
    PUSH32(esp, eax);
    ecx = edi;
    PUSH32(esp, 0x000C96DEu); RECOMP_ABI_CALL(0x00037C80u, sub_00037C80); /* call 0x00037C80 */

loc_000C96DE: ;
    RECOMP_PC(0x000C96DEu);
    eax = MEM32(0x1DC444);
    ebx = 0x207;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebx (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C9704; /* je: equal / zero */

loc_000C96EC: ;
    RECOMP_PC(0x000C96ECu);
    edx = ebx;
    ecx = 0x40354;
    MEM32(0x1DC444) = ebx;
    PUSH32(esp, 0x000C96FEu); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000C96FE: ;
    RECOMP_PC(0x000C96FEu);
    MEM32(0x1A51DC) = ebx;

loc_000C9704: ;
    RECOMP_PC(0x000C9704u);
    eax = MEM32(0x1DC3CC);
    ebp = 0; /* xor self */
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebp (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C9747; /* je: equal / zero */

loc_000C970F: ;
    RECOMP_PC(0x000C970Fu);
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = ebp;
    PUSH32(esp, 0x000C9721u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000C9721: ;
    RECOMP_PC(0x000C9721u);
    _fa = (uint32_t)(MEM32(0x1DC3CC)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC3CC), ebp (32-bit) */
    MEM32(0x1A51F8) = ebp;
    if (CMP_EQ(_fa, _fb)) goto loc_000C9747; /* je: equal / zero */

loc_000C972F: ;
    RECOMP_PC(0x000C972Fu);
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = ebp;
    PUSH32(esp, 0x000C9741u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000C9741: ;
    RECOMP_PC(0x000C9741u);
    MEM32(0x1A51F8) = ebp;

loc_000C9747: ;
    RECOMP_PC(0x000C9747u);
    PUSH32(esp, edi);
    ecx = esi;
    PUSH32(esp, 0x000C974Fu); RECOMP_ABI_CALL(0x0010FEE0u, sub_0010FEE0); /* call 0x0010FEE0 */

loc_000C974F: ;
    RECOMP_PC(0x000C974Fu);
    _fa = (uint32_t)(MEM32(esi + 0xB4)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(esi + 0xB4), ebp (32-bit) */
    ebx = 1;
    if (CMP_EQ(_fa, _fb)) goto loc_000C985F; /* je: equal / zero */

loc_000C9760: ;
    RECOMP_PC(0x000C9760u);
    PUSH32(esp, 0x000C9765u); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000C9765: ;
    RECOMP_PC(0x000C9765u);
    fp_push(MEMF(esi + 0xB0)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91D0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91d0] */
    MEM32(esp + 0x40) = 0x3F800000;
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000C978A; /* jp: parity */

loc_000C9780: ;
    RECOMP_PC(0x000C9780u);
    ecx = MEM32(esi + 0xB0);
    MEM32(esp + 0x40) = ecx;

loc_000C978A: ;
    RECOMP_PC(0x000C978Au);
    fp_push(MEMF(esi + 0xB0)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91DC)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91dc] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_000C97AF; /* jne: not equal / not zero */

loc_000C979D: ;
    RECOMP_PC(0x000C979Du);
    fp_push(MEMF(esi + 0xB0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1C921C)); /* fsub dword ptr [0x1c921c] */
    fp_top() = fabs(fp_top()); /* fabs */
    MEMF(esp + 0x40) = (float)fp_top(); fp_pop(); /* fstp */

loc_000C97AF: ;
    RECOMP_PC(0x000C97AFu);
    edx = MEM32(esp + 0x40);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, edx);
    ecx = esp + 0x50;
    PUSH32(esp, 0x000C97CCu); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_000C97CC: ;
    RECOMP_PC(0x000C97CCu);
    eax = MEM32(esi + 0xF4);
    ecx = MEM32(0x1D9FB8);
    PUSH32(esp, eax);
    PUSH32(esp, 0x000C97DEu); RECOMP_ABI_CALL(0x000A1780u, sub_000A1780); /* call 0x000A1780 */

loc_000C97DE: ;
    RECOMP_PC(0x000C97DEu);
    edi = MEM32(eax + 0xCC);
    eax = MEM32(0x1D8A88);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebp (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000C980E; /* jne: not equal / not zero */

loc_000C97ED: ;
    RECOMP_PC(0x000C97EDu);
    PUSH32(esp, 0x34C);
    PUSH32(esp, 0x000C97F7u); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_000C97F7: ;
    RECOMP_PC(0x000C97F7u);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebp (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C9807; /* je: equal / zero */

loc_000C97FE: ;
    RECOMP_PC(0x000C97FEu);
    ecx = eax;
    PUSH32(esp, 0x000C9805u); RECOMP_ABI_CALL(0x00082440u, sub_00082440); /* call 0x00082440 */

loc_000C9805: ;
    RECOMP_PC(0x000C9805u);
    goto loc_000C9809;

loc_000C9807: ;
    RECOMP_PC(0x000C9807u);
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_000C9809: ;
    RECOMP_PC(0x000C9809u);
    MEM32(0x1D8A88) = eax;

loc_000C980E: ;
    RECOMP_PC(0x000C980Eu);
    PUSH32(esp, edi);
    ecx = eax;
    PUSH32(esp, 0x000C9816u); RECOMP_ABI_CALL(0x00082210u, sub_00082210); /* call 0x00082210 */

loc_000C9816: ;
    RECOMP_PC(0x000C9816u);
    ecx = MEM32(esp + 0x40);
    PUSH32(esp, ebx);
    PUSH32(esp, ecx);
    PUSH32(esp, ecx);
    ecx = esp + 0x1C;
    PUSH32(esp, ecx);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fb = (uint32_t)(0x1C8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + 0x1C8;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ecx = MEM32(esi);
    edx = esp;
    MEM32(edx) = ecx;
    ecx = MEM32(esi + 4);
    MEM32(edx + 4) = ecx;
    ecx = MEM32(esi + 8);
    MEM32(edx + 8) = ecx;
    ecx = MEM32(esi + 0xC);
    PUSH32(esp, eax);
    MEM32(edx + 0xC) = ecx;
    edx = esp + 0x48;
    PUSH32(esp, edx);
    eax = esp + 0x3C;
    PUSH32(esp, eax);
    MEM32(esp + 0x3C) = ebx;
    PUSH32(esp, 0x000C9857u); RECOMP_ABI_CALL(0x000C6F10u, sub_000C6F10); /* call 0x000C6F10 */

loc_000C9857: ;
    RECOMP_PC(0x000C9857u);
    _fb = (uint32_t)(0x2C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x2C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x000C985Fu); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000C985F: ;
    RECOMP_PC(0x000C985Fu);
    eax = MEM32(0x1DC444);
    esi = 0x203;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, esi (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C9885; /* je: equal / zero */

loc_000C986D: ;
    RECOMP_PC(0x000C986Du);
    edx = esi;
    ecx = 0x40354;
    MEM32(0x1DC444) = esi;
    PUSH32(esp, 0x000C987Fu); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000C987F: ;
    RECOMP_PC(0x000C987Fu);
    MEM32(0x1A51DC) = esi;

loc_000C9885: ;
    RECOMP_PC(0x000C9885u);
    _fa = (uint32_t)(MEM32(0x1DC3CC)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC3CC), ebp (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000C98C5; /* jne: not equal / not zero */

loc_000C988D: ;
    RECOMP_PC(0x000C988Du);
    edx = ebx;
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = ebx;
    PUSH32(esp, 0x000C989Fu); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000C989F: ;
    RECOMP_PC(0x000C989Fu);
    _fa = (uint32_t)(MEM32(0x1DC3CC)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC3CC), ebp (32-bit) */
    MEM32(0x1A51F8) = ebx;
    if (CMP_NE(_fa, _fb)) goto loc_000C98C5; /* jne: not equal / not zero */

loc_000C98AD: ;
    RECOMP_PC(0x000C98ADu);
    edx = ebx;
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = ebx;
    PUSH32(esp, 0x000C98BFu); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000C98BF: ;
    RECOMP_PC(0x000C98BFu);
    MEM32(0x1A51F8) = ebx;

loc_000C98C5: ;
    RECOMP_PC(0x000C98C5u);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    _fb = (uint32_t)(0x2C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x2C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; return; /* ret 4 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

