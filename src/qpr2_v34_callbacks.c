/* Bounded routines recovered from the supplied retail disassembly. */

#define RECOMP_GENERATED_CODE

#include "recomp/gen/recomp_types.h"

#include "recomp/gen/recomp_funcs.h"
#include "qpr2_ui_trace.h"

void qpr2_recovered_0001BA10(void);

void qpr2_recovered_000241D0(void);

void qpr2_recovered_00028F60(void);

void qpr2_recovered_00070C30(void);

void qpr2_recovered_00070D60(void);

void qpr2_recovered_00082E40(void);

void qpr2_recovered_000A9050(void);

void qpr2_recovered_0009E240(void);

/**
 * qpr2_recovered_0001BA10
 * Original: 0x0001BA10 - 0x0001BC06 (502 bytes, 142 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_0001BA10(void)
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

loc_0001BA10: ;
    RECOMP_PC(0x0001BA10u);
    SET_LO8(eax, MEM8(0x1D74F4));
    _fb = (uint32_t)(0x3C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x3C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test LO8(eax), 1 (8-bit) */
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    esi = ecx;
    if (TEST_NZ(_fa, _fb)) goto loc_0001BA49; /* jne: not equal / not zero */

loc_0001BA22: ;
    RECOMP_PC(0x0001BA22u);
    ebx = MEM32(0x1D74F4);
    PUSH32(esp, 0);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0);
    ebx = ebx | 1;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    PUSH32(esp, 0x3F000000);
    ecx = 0x1D74F0;
    MEM32(0x1D74F4) = ebx;
    PUSH32(esp, 0x0001BA49u); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_0001BA49: ;
    RECOMP_PC(0x0001BA49u);
    eax = MEM32(esi + 0x48);
    ecx = MEM32(eax + 0x48);
    eax = MEM32(ecx + 0x254);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_0001BAAA; /* jne: not equal / not zero */

loc_0001BA59: ;
    RECOMP_PC(0x0001BA59u);
    edx = MEM32(esi + 0x4C);
    edi = esi + 0x4C;
    ecx = edi;
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_target = MEM32(edx + 0x1C); PUSH32(esp, 0x0001BA64u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x0001BA61u); } /* indirect call */
    }

loc_0001BA64: ;
    RECOMP_PC(0x0001BA64u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 4 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_0001BAAA; /* jne: not equal / not zero */

loc_0001BA69: ;
    RECOMP_PC(0x0001BA69u);
    eax = esp + 0x10;
    PUSH32(esp, eax);
    ecx = esp + 0x20;
    PUSH32(esp, ecx);
    ecx = edi;
    MEM32(esp + 0x18) = 0;
    MEM32(esp + 0x1C) = 0;
    MEM32(esp + 0x20) = 0;
    MEM32(esp + 0x24) = 0;
    MEM32(esp + 0x28) = 0;
    MEM32(esp + 0x2C) = 0;
    PUSH32(esp, 0x0001BAAAu); RECOMP_ABI_CALL(0x000D0FC0u, sub_000D0FC0); /* call 0x000D0FC0 */

loc_0001BAAA: ;
    RECOMP_PC(0x0001BAAAu);
    ebp = MEM32(esp + 0x50);
    edx = MEM32(esi + 0x4C);
    ecx = esi + 0x4C;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebp);
    { uint32_t _icall_target = MEM32(edx + 0x24); PUSH32(esp, 0x0001BAB8u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x0001BAB5u); } /* indirect call */
    }

loc_0001BAB8: ;
    RECOMP_PC(0x0001BAB8u);
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    /* nop */

loc_0001BAC0: ;
    RECOMP_PC(0x0001BAC0u);
    eax = MEM32(esi + 0xAB0);
    ecx = edi + eax;
    eax = MEM32(ecx + 0x190);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0001BAEA; /* je: equal / zero */

loc_0001BAD3: ;
    RECOMP_PC(0x0001BAD3u);
    PUSH32(esp, ebp);
    PUSH32(esp, 0x0001BAD9u); RECOMP_ABI_CALL(0x00035020u, sub_00035020); /* call 0x00035020 */

loc_0001BAD9: ;
    RECOMP_PC(0x0001BAD9u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(1) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 1 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_0001BAEA; /* jne: not equal / not zero */

loc_0001BADE: ;
    RECOMP_PC(0x0001BADEu);
    ecx = MEM32(esi + 0xEE8);
    PUSH32(esp, ebx);
    PUSH32(esp, 0x0001BAEAu); RECOMP_ABI_CALL(0x0011E860u, sub_0011E860); /* call 0x0011E860 */

loc_0001BAEA: ;
    RECOMP_PC(0x0001BAEAu);
    _fb = (uint32_t)(0x4E8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edi = edi + 0x4E8;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ebx++;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0x1D70) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0x1D70 (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_0001BAC0; /* jl: less (signed <) */

loc_0001BAF9: ;
    RECOMP_PC(0x0001BAF9u);
    eax = MEM32(esi + 0xAA8);
    ecx = 0; /* xor self */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ecx (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0001BB34; /* je: equal / zero */

loc_0001BB05: ;
    RECOMP_PC(0x0001BB05u);
    fp_push(MEMF(esp + 0x50)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0xAA4)); /* fadd dword ptr [esi + 0xaa4] */
    MEMF(esi + 0xAA4) = (float)fp_top(); /* fst */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1AF02C)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1af02c] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 1 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_0001BB34; /* jne: not equal / not zero */

loc_0001BB22: ;
    RECOMP_PC(0x0001BB22u);
    MEM32(esi + 0xAA0) = ecx;
    MEM32(esi + 0xAA8) = ecx;
    MEM32(esi + 0xAA4) = ecx;

loc_0001BB34: ;
    RECOMP_PC(0x0001BB34u);
    PUSH32(esp, ebp);
    ecx = esi + 0x100C;
    PUSH32(esp, 0x0001BB40u); RECOMP_ABI_CALL(0x0001B3F0u, sub_0001B3F0); /* call 0x0001B3F0 */

loc_0001BB40: ;
    RECOMP_PC(0x0001BB40u);
    edx = MEM32(esi);
    ecx = esi;
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_target = MEM32(edx + 0x38); PUSH32(esp, 0x0001BB47u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x0001BB44u); } /* indirect call */
    }

loc_0001BB47: ;
    RECOMP_PC(0x0001BB47u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0x1DAEB4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0x1DAEB4 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0001BB9D; /* je: equal / zero */

loc_0001BB4E: ;
    RECOMP_PC(0x0001BB4Eu);
    ecx = MEM32(eax);
    MEM32(0x1DAEB4) = ecx;
    edx = MEM32(eax + 4);
    MEM32(0x1DAEB8) = edx;
    ecx = MEM32(eax + 8);
    MEM32(0x1DAEBC) = ecx;
    edx = MEM32(eax + 0xC);
    MEM32(0x1DAEC0) = edx;
    ecx = MEM32(eax + 0x10);
    MEM32(0x1DAEC4) = ecx;
    edx = MEM32(eax + 0x14);
    MEM32(0x1DAEC8) = edx;
    ecx = MEM32(eax + 0x18);
    MEM32(0x1DAECC) = ecx;
    edx = MEM32(eax + 0x1C);
    MEM32(0x1DAED0) = edx;
    eax = MEM32(eax + 0x20);
    MEM32(0x1DAED4) = eax;

loc_0001BB9D: ;
    RECOMP_PC(0x0001BB9Du);
    PUSH32(esp, 0x1DAEB4);
    ecx = esi + 0x1034;
    PUSH32(esp, ecx);
    edx = esp + 0x30;
    PUSH32(esp, edx);
    edi = esi + 0x1078;
    PUSH32(esp, 0x0001BBB9u); RECOMP_ABI_CALL(0x000CE660u, sub_000CE660); /* call 0x000CE660 */

loc_0001BBB9: ;
    RECOMP_PC(0x0001BBB9u);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, eax (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0001BBF4; /* je: equal / zero */

loc_0001BBC0: ;
    RECOMP_PC(0x0001BBC0u);
    ecx = MEM32(eax);
    MEM32(edi) = ecx;
    edx = MEM32(eax + 4);
    MEM32(edi + 4) = edx;
    ecx = MEM32(eax + 8);
    MEM32(edi + 8) = ecx;
    edx = MEM32(eax + 0xC);
    MEM32(edi + 0xC) = edx;
    ecx = MEM32(eax + 0x10);
    MEM32(edi + 0x10) = ecx;
    edx = MEM32(eax + 0x14);
    MEM32(edi + 0x14) = edx;
    ecx = MEM32(eax + 0x18);
    MEM32(edi + 0x18) = ecx;
    edx = MEM32(eax + 0x1C);
    MEM32(edi + 0x1C) = edx;
    eax = MEM32(eax + 0x20);
    MEM32(edi + 0x20) = eax;

loc_0001BBF4: ;
    RECOMP_PC(0x0001BBF4u);
    PUSH32(esp, ebp);
    ecx = esi;
    PUSH32(esp, 0x0001BBFCu); RECOMP_ABI_CALL(0x0001A9F0u, sub_0001A9F0); /* call 0x0001A9F0 */

loc_0001BBFC: ;
    RECOMP_PC(0x0001BBFCu);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    _fb = (uint32_t)(0x3C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x3C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; return; /* ret 4 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_000241D0
 * Original: 0x000241D0 - 0x0002430F (319 bytes, 99 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000241D0(void)
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

loc_000241D0: ;
    RECOMP_PC(0x000241D0u);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebx);
    ebx = ecx;
    eax = MEM32(ebx + 0x60);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    MEM32(esp + 8) = ebx;
    if (CMP_NE(_fa, _fb)) goto loc_00024303; /* jne: not equal / not zero */

loc_000241E5: ;
    RECOMP_PC(0x000241E5u);
    fp_push(MEMF(ebx + 0x5C)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_00024303; /* jnp: not parity */

loc_000241F9: ;
    RECOMP_PC(0x000241F9u);
    eax = MEM32(0x1D9FB8);
    ecx = MEM32(eax + 0x10);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, 0 (32-bit) */
    MEM32(esp + 0xC) = 0;
    if (CMP_LE(_fas, _fbs)) goto loc_00024303; /* jle: less or equal (signed <=) */

loc_00024211: ;
    RECOMP_PC(0x00024211u);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    MEM32(esp + 0xC) = 0x14;
    PUSH32(esp, edi);
    /* nop */

loc_00024220: ;
    RECOMP_PC(0x00024220u);
    ecx = MEM32(esp + 0x10);
    edx = MEM32(0x1D9FB8);
    eax = MEM32(ecx + edx);
    ebp = MEM32(eax + 0x3C);
    esi = MEM32(ebx + 0x1C);
    edi = MEM32(ebp + 0x1C);
    ecx = esi + 8;
    PUSH32(esp, ecx);
    edx = edi + 8;
    PUSH32(esp, edx);
    PUSH32(esp, 0x00024243u); RECOMP_ABI_CALL(0x00023810u, sub_00023810); /* call 0x00023810 */

loc_00024243: ;
    RECOMP_PC(0x00024243u);
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000242DB; /* je: equal / zero */

loc_0002424E: ;
    RECOMP_PC(0x0002424Eu);
    PUSH32(esp, edi);
    PUSH32(esp, esi);
    PUSH32(esp, 0x00024255u); RECOMP_ABI_CALL(0x00070720u, sub_00070720); /* call 0x00070720 */

loc_00024255: ;
    RECOMP_PC(0x00024255u);
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000242DB; /* je: equal / zero */

loc_0002425C: ;
    RECOMP_PC(0x0002425Cu);
    eax = MEM32(ebp + 0x48);
    fp_push(MEMF(eax + 0xB0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ebx + 0x5C)); /* fsub dword ptr [ebx + 0x5c] */
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    MEMF(eax + 0xB0) = (float)fp_top(); fp_pop(); /* fstp */
    edi = MEM32(0x1D9FB8);
    eax = MEM32(edi + 0x54);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_LE(_fas, _fbs)) goto loc_000242CE; /* jle: less or equal (signed <=) */

loc_0002427D: ;
    RECOMP_PC(0x0002427Du);
    /* nop */

loc_00024280: ;
    RECOMP_PC(0x00024280u);
    eax = MEM32(ebp + 0x48);
    SET_LO8(edx, MEM8(eax + 0x54));
    SET_LO8(ecx, LO8(ebx));
    SET_LO8(edx, LO8(edx) >> LO8(ecx));
    _fa = (uint32_t)(LO8(edx)) & 0xFFu; _fas = (int32_t)(int8_t)(_fa); /* shift result */
    _fa = (uint32_t)(LO8(edx)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test LO8(edx), 1 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_000242C6; /* je: equal / zero */

loc_0002428F: ;
    RECOMP_PC(0x0002428Fu);
    ecx = MEM32(edi + 0x58);
    eax = ZX8(LO8(ebx));
    edx = MEM32(ecx + eax * 4);
    esi = MEM32(edx + 0xD4);
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000242C6; /* je: equal / zero */

loc_000242A2: ;
    RECOMP_PC(0x000242A2u);
    ecx = MEM32(0x1DB454);
    PUSH32(esp, 7);
    PUSH32(esp, 0);
    PUSH32(esp, 1);
    PUSH32(esp, 0x9C);
    PUSH32(esp, 0x000242B8u); RECOMP_ABI_CALL(0x000E0830u, sub_000E0830); /* call 0x000E0830 */

loc_000242B8: ;
    RECOMP_PC(0x000242B8u);
    PUSH32(esp, eax);
    ecx = esi;
    PUSH32(esp, 0x000242C0u); RECOMP_ABI_CALL(0x000C6EC0u, sub_000C6EC0); /* call 0x000C6EC0 */

loc_000242C0: ;
    RECOMP_PC(0x000242C0u);
    edi = MEM32(0x1D9FB8);

loc_000242C6: ;
    RECOMP_PC(0x000242C6u);
    eax = MEM32(edi + 0x54);
    ebx++;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, eax (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_00024280; /* jl: less (signed <) */

loc_000242CE: ;
    RECOMP_PC(0x000242CEu);
    eax = MEM32(esp + 0x14);
    MEM32(eax + 0x60) = 1;
    ebx = eax;

loc_000242DB: ;
    RECOMP_PC(0x000242DBu);
    eax = MEM32(esp + 0x18);
    esi = MEM32(esp + 0x10);
    ecx = MEM32(0x1D9FB8);
    edx = MEM32(ecx + 0x10);
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + 4;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edx (32-bit) */
    MEM32(esp + 0x18) = eax;
    MEM32(esp + 0x10) = esi;
    if (CMP_L(_fas, _fbs)) goto loc_00024220; /* jl: less (signed <) */

loc_00024300: ;
    RECOMP_PC(0x00024300u);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);

loc_00024303: ;
    RECOMP_PC(0x00024303u);
    ecx = MEM32(ebx + 0x18);
    edx = MEM32(ecx);
    POP32(esp, ebx);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    g_seh_ebp = ebp; RECOMP_ITAIL(MEM32(edx + 0x34)); return; /* indirect tail jmp */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_00028F60
 * Original: 0x00028F60 - 0x00029097 (311 bytes, 65 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00028F60(void)
{
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;

loc_00028F60: ;
    RECOMP_PC(0x00028F60u);
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    esi = ecx;
    PUSH32(esp, edi);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    MEM32(esi + 0x4F0) = edi;
    MEM32(esi + 0x4F4) = edi;
    MEM32(esi + 0x4F8) = edi;
    MEM32(esi + 0x4FC) = edi;
    MEM32(esi + 0x51C) = 2;
    PUSH32(esp, 0x10);
    MEM32(esi + 0x4B4) = edi;
    MEM32(esi + 0x4B0) = edi;
    MEM32(esi + 0x4AC) = edi;
    eax = esi + 0x4DC;
    PUSH32(esp, edi);
    PUSH32(esp, eax);
    MEM32(esi + 0x4C0) = edi;
    MEM32(esi + 0x4BC) = edi;
    MEM32(esi + 0x4B8) = edi;
    PUSH32(esp, 0x00028FBCu); RECOMP_ABI_CALL(0x00100DA0u, sub_00100DA0); /* call 0x00100DA0 */

loc_00028FBC: ;
    RECOMP_PC(0x00028FBCu);
    MEM32(esi + 0x524) = edi;
    MEM32(esi + 0x520) = edi;
    PUSH32(esp, 0x18);
    ebx = 0xBF800000u;
    MEM32(esi + 0x4E8) = ebx;
    MEM32(esi + 0x528) = ebx;
    ecx = esi + 0x4C4;
    MEM32(esi + 0x488) = 0xFFFFFFFFu;
    MEM32(esi + 0x48C) = edi;
    MEM32(esi + 0x490) = edi;
    MEM32(esi + 0x494) = edi;
    MEM32(esi + 0x498) = edi;
    PUSH32(esp, edi);
    PUSH32(esp, ecx);
    MEM32(esi + 0x49C) = edi;
    MEM32(esi + 0x4A0) = edi;
    PUSH32(esp, 0x00029016u); RECOMP_ABI_CALL(0x00100DA0u, sub_00100DA0); /* call 0x00100DA0 */

loc_00029016: ;
    RECOMP_PC(0x00029016u);
    MEM32(esi + 0x538) = ebx;
    MEM32(esi + 0x534) = ebx;
    MEM32(esi + 0x4EC) = edi;
    MEM32(esi + 0x530) = edi;
    MEM32(esi + 0x504) = edi;
    MEM32(esi + 0x510) = edi;
    MEM32(esi + 0x508) = edi;
    MEM32(esi + 0x514) = edi;
    MEM32(esi + 0x50C) = edi;
    MEM32(esi + 0x518) = edi;
    ecx = MEM32(esi + 0x9E4);
    _fb = (uint32_t)(0x18) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x18;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x00029060u); RECOMP_ABI_CALL(0x000FC4C0u, sub_000FC4C0); /* call 0x000FC4C0 */

loc_00029060: ;
    RECOMP_PC(0x00029060u);
    edx = MEM32(esp + 0x14);
    eax = MEM32(esp + 0x10);
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    ecx = esi;
    MEM32(esi + 0x540) = edi;
    MEM32(esi + 0x544) = 0xFFFFFFFFu;
    MEM32(esi + 0x548) = 0x3E99999A;
    MEM32(esi + 0x53C) = ebx;
    PUSH32(esp, 0x00029091u); RECOMP_ABI_CALL(0x00020B60u, sub_00020B60); /* call 0x00020B60 */

loc_00029091: ;
    RECOMP_PC(0x00029091u);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp += 12; return; /* ret 8 */

}


/**
 * qpr2_recovered_00070C30
 * Original: 0x00070C30 - 0x00070D5D (301 bytes, 104 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00070C30(void)
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

loc_00070C30: ;
    RECOMP_PC(0x00070C30u);
    eax = MEM32(0x1BB27C);
    fp_push(MEMF(0x1BB27C)); /* fld float */
    _fb = (uint32_t)(0x18) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x18;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    fp_top() = -fp_top(); /* fchs */
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    esi = ecx;
    PUSH32(esp, edi);
    MEMF(esi + 0x14) = (float)fp_top(); /* fst */
    edi = esi + 8;
    ecx = eax;
    MEMF(edi + 0x10) = (float)fp_top(); /* fst */
    edx = eax;
    MEMF(edi + 0x14) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(edi + 4) = ecx;
    MEM32(edi) = eax;
    MEM32(edi + 8) = edx;
    eax = MEM32(esi + 0x20);
    edx = MEM32(eax + 0x654);
    ecx = 0; /* xor self */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edx, 0 (32-bit) */
    if (CMP_LE(_fas, _fbs)) goto loc_00070CBC; /* jle: less or equal (signed <=) */

loc_00070C6D: ;
    RECOMP_PC(0x00070C6Du);
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_00070C71: ;
    RECOMP_PC(0x00070C71u);
    eax = MEM32(eax + 0x650);
    fp_push(MEMF(eax + edx + 8)); /* fld float */
    _fb = (uint32_t)(edx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + edx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEMF(esp + 0x14) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(eax + 0x34)); /* fld float */
    _fb = (uint32_t)(0x114) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edx = edx + 0x114;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    fp_push(MEMF(esp + 0x14)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 0x38)); /* fadd dword ptr [eax + 0x38] */
    fp_push(MEMF(eax + 0x3C)); /* fld float */
    eax = MEM32(esi + 0x48);
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEMF(esp + 0x24) = (float)fp_top(); fp_pop(); /* fstp */
    ebp = MEM32(esp + 0x24);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(eax) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(eax + 8) = ebp;
    ecx++;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    MEMF(eax + 4) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esi + 0x20);
    ebp = MEM32(eax + 0x654);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebx = ebx + 0x10;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, ebp (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_00070C71; /* jl: less (signed <) */

loc_00070CBC: ;
    RECOMP_PC(0x00070CBCu);
    ebx = MEM32(esp + 0x30);
    ebp = MEM32(esp + 0x2C);
    edx = MEM32(esi + 0x24);
    ecx = esi + 0x24;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    { uint32_t _icall_target = MEM32(edx + 4); PUSH32(esp, 0x00070CCFu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00070CCCu); } /* indirect call */
    }

loc_00070CCF: ;
    RECOMP_PC(0x00070CCFu);
    eax = MEM32(esi + 0x8C);
    ecx = esi + 0x8C;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    { uint32_t _icall_target = MEM32(eax + 4); PUSH32(esp, 0x00070CE0u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00070CDDu); } /* indirect call */
    }

loc_00070CE0: ;
    RECOMP_PC(0x00070CE0u);
    ecx = esi + 0x2C;
    PUSH32(esp, ecx);
    ecx = edi;
    PUSH32(esp, 0x00070CEBu); RECOMP_ABI_CALL(0x00019480u, sub_00019480); /* call 0x00019480 */

loc_00070CEB: ;
    RECOMP_PC(0x00070CEBu);
    edx = esi + 0x94;
    PUSH32(esp, edx);
    ecx = edi;
    PUSH32(esp, 0x00070CF9u); RECOMP_ABI_CALL(0x00019480u, sub_00019480); /* call 0x00019480 */

loc_00070CF9: ;
    RECOMP_PC(0x00070CF9u);
    fp_push(MEMF(0x1BB27C)); /* fld float */
    eax = MEM32(0x1BB27C);
    fp_top() = -fp_top(); /* fchs */
    edi = esi + 0x1A4;
    ecx = eax;
    MEMF(edi + 0xC) = (float)fp_top(); /* fst */
    edx = eax;
    MEMF(edi + 0x10) = (float)fp_top(); /* fst */
    MEMF(edi + 0x14) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(edi) = eax;
    MEM32(edi + 4) = ecx;
    MEM32(edi + 8) = edx;
    eax = MEM32(esi + 0x164);
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_LE(_fas, _fbs)) goto loc_00070D53; /* jle: less or equal (signed <=) */

loc_00070D2D: ;
    RECOMP_PC(0x00070D2Du);
    ebp = 0; /* xor self */
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    /* nop */

loc_00070D30: ;
    RECOMP_PC(0x00070D30u);
    eax = MEM32(esi + 0x158);
    ecx = MEM32(eax + ebp + 0xC);
    _fb = (uint32_t)(ebp) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + ebp;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, ecx);
    PUSH32(esp, eax);
    ecx = edi;
    PUSH32(esp, 0x00070D45u); RECOMP_ABI_CALL(0x00019580u, sub_00019580); /* call 0x00019580 */

loc_00070D45: ;
    RECOMP_PC(0x00070D45u);
    eax = MEM32(esi + 0x164);
    ebx++;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebp = ebp + 0x10;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, eax (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_00070D30; /* jl: less (signed <) */

loc_00070D53: ;
    RECOMP_PC(0x00070D53u);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    _fb = (uint32_t)(0x18) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x18;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 12; return; /* ret 8 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_00070D60
 * Original: 0x00070D60 - 0x00070E6B (267 bytes, 84 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00070D60(void)
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

loc_00070D60: ;
    RECOMP_PC(0x00070D60u);
    eax = MEM32(0x1BB27C);
    fp_push(MEMF(0x1BB27C)); /* fld float */
    _fb = (uint32_t)(0x18) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x18;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    fp_top() = -fp_top(); /* fchs */
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    esi = ecx;
    MEMF(esi + 0x14) = (float)fp_top(); /* fst */
    MEM32(esi + 8) = eax;
    MEMF(esi + 0x18) = (float)fp_top(); /* fst */
    ecx = eax;
    MEMF(esi + 0x1C) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0xC) = ecx;
    edx = eax;
    MEM32(esi + 0x10) = edx;
    eax = MEM32(esi + 0x20);
    eax = MEM32(eax + 0x650);
    fp_push(MEMF(eax + 0x344)); /* fld float */
    PUSH32(esp, edi);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 0x230)); /* fadd dword ptr [eax + 0x230] */
    edi = esi + 8;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C91CC)); /* fmul dword ptr [0x1c91cc] */
    MEMF(eax + 0x230) = (float)fp_top(); /* fst */
    ecx = MEM32(esi + 0x20);
    edx = MEM32(ecx + 0x650);
    MEMF(edx + 0x344) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esi + 0x20);
    edx = MEM32(eax + 0x654);
    ecx = 0; /* xor self */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edx, 0 (32-bit) */
    if (CMP_LE(_fas, _fbs)) goto loc_00070E24; /* jle: less or equal (signed <=) */

loc_00070DCE: ;
    RECOMP_PC(0x00070DCEu);
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_00070DD2: ;
    RECOMP_PC(0x00070DD2u);
    eax = MEM32(eax + 0x650);
    fp_push(MEMF(ecx * 4 + 0x1CC198)); /* fld float */
    fp_push(MEMF(eax + edx + 8)); /* fld float */
    _fb = (uint32_t)(edx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + edx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEMF(esp + 0x14) = (float)fp_top(); fp_pop(); /* fstp */
    _fb = (uint32_t)(0x114) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edx = edx + 0x114;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 0x34)); /* fadd dword ptr [eax + 0x34] */
    fp_push(MEMF(esp + 0x14)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 0x38)); /* fadd dword ptr [eax + 0x38] */
    fp_push(MEMF(eax + 0x3C)); /* fld float */
    eax = MEM32(esi + 0x48);
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEMF(esp + 0x24) = (float)fp_top(); fp_pop(); /* fstp */
    ebp = MEM32(esp + 0x24);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(eax) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(eax + 8) = ebp;
    ecx++;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    MEMF(eax + 4) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esi + 0x20);
    ebp = MEM32(eax + 0x654);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebx = ebx + 0x10;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, ebp (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_00070DD2; /* jl: less (signed <) */

loc_00070E24: ;
    RECOMP_PC(0x00070E24u);
    ebx = MEM32(esp + 0x30);
    ebp = MEM32(esp + 0x2C);
    edx = MEM32(esi + 0x24);
    ecx = esi + 0x24;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    { uint32_t _icall_target = MEM32(edx + 8); PUSH32(esp, 0x00070E37u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00070E34u); } /* indirect call */
    }

loc_00070E37: ;
    RECOMP_PC(0x00070E37u);
    eax = MEM32(esi + 0x8C);
    ecx = esi + 0x8C;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    { uint32_t _icall_target = MEM32(eax + 8); PUSH32(esp, 0x00070E48u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00070E45u); } /* indirect call */
    }

loc_00070E48: ;
    RECOMP_PC(0x00070E48u);
    ecx = esi + 0x2C;
    PUSH32(esp, ecx);
    ecx = edi;
    PUSH32(esp, 0x00070E53u); RECOMP_ABI_CALL(0x00019480u, sub_00019480); /* call 0x00019480 */

loc_00070E53: ;
    RECOMP_PC(0x00070E53u);
    _fb = (uint32_t)(0x94) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + 0x94;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, esi);
    ecx = edi;
    PUSH32(esp, 0x00070E61u); RECOMP_ABI_CALL(0x00019480u, sub_00019480); /* call 0x00019480 */

loc_00070E61: ;
    RECOMP_PC(0x00070E61u);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    _fb = (uint32_t)(0x18) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x18;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 12; return; /* ret 8 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_00082E40
 * Original: 0x00082E40 - 0x00082FED (429 bytes, 134 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00082E40(void)
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

loc_00082E40: ;
    RECOMP_PC(0x00082E40u);
    _fb = (uint32_t)(0x3C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x3C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    esi = ecx;
    PUSH32(esp, 0x00082E4Cu); RECOMP_ABI_CALL(0x00013760u, sub_00013760); /* call 0x00013760 */

loc_00082E4C: ;
    RECOMP_PC(0x00082E4Cu);
    fp_push(MEMF(esp + 0x48)); /* fld float */
    ecx = MEM32(eax + 0x34);
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    MEM32(esp + 0x14) = ecx;
    edx = MEM32(eax + 0x38);
    MEM32(esp + 0x18) = edx;
    eax = MEM32(eax + 0x3C);
    MEM32(esp + 0x1C) = eax;
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_00082FB5; /* jnp: not parity */

loc_00082E76: ;
    RECOMP_PC(0x00082E76u);
    fp_push(MEMF(esp + 0x1C)); /* fld float */
    edi = esi + 0x40;
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 0x3C)); /* fsub dword ptr [esi + 0x3c] */
    fp_push(MEMF(esp + 0x18)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 0x38)); /* fsub dword ptr [esi + 0x38] */
    fp_push(MEMF(esp + 0x14)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 0x34)); /* fsub dword ptr [esi + 0x34] */
    MEMF(esp + 0x38) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esp + 0x48)); /* fdiv dword ptr [esp + 0x48] */
    MEMF(esp + 8) = (float)fp_top(); /* fst */
    fp_top() = RECOMP_FP_PC(fp_top() * g_fp_stack[(g_fp_top + 2) & 7]); /* fmul st(2) */
    MEMF(esp + 0xC) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 8)); /* fld float */
    ecx = MEM32(esp + 0xC);
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    PUSH32(esp, ecx);
    MEM32(esp + 0x2C) = ecx;
    MEMF(esp + 0x14) = (float)fp_top(); /* fst */
    g_fp_stack[(g_fp_top + 2) & 7] = fp_top(); fp_pop(); /* fstp st(2) */
    fp_pop(); /* fstp st(0) */
    fp_push(MEMF(esp + 0xC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x3C)); /* fmul dword ptr [esp + 0x3c] */
    MEMF(esp + 0x30) = (float)fp_top(); /* fst */
    MEMF(esp + 0x24) = (float)fp_top(); fp_pop(); /* fstp */
    MEMF(esp + 0x28) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(edi + 8)); /* fld float */
    fp_push(MEMF(edi + 4)); /* fld float */
    fp_push(MEMF(edi)); /* fld float */
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
    PUSH32(esp, 0x00082EF7u); RECOMP_ABI_CALL(0x000CDE60u, sub_000CDE60); /* call 0x000CDE60 */

loc_00082EF7: ;
    RECOMP_PC(0x00082EF7u);
    MEMF(esp + 0xC) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x30)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x30)); /* fmul dword ptr [esp + 0x30] */
    fp_push(MEMF(esp + 0x14)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x14)); /* fmul dword ptr [esp + 0x14] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    fp_push(MEMF(esp + 0x10)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x10)); /* fmul dword ptr [esp + 0x10] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x00082F1Fu); RECOMP_ABI_CALL(0x000CDE60u, sub_000CDE60); /* call 0x000CDE60 */

loc_00082F1F: ;
    RECOMP_PC(0x00082F1Fu);
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esp + 0xC)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esp + 0xc] */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    eax = esp + 0x20;
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_00082F33; /* jnp: not parity */

loc_00082F31: ;
    RECOMP_PC(0x00082F31u);
    eax = edi;

loc_00082F33: ;
    RECOMP_PC(0x00082F33u);
    edx = MEM32(eax);
    ecx = MEM32(eax + 4);
    MEM32(esp + 0x38) = edx;
    edx = MEM32(eax + 8);
    eax = MEM32(esi + 0x10);
    MEM32(esp + 0x40) = edx;
    edx = MEM32(esp + 0x48);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edx);
    edx = esp + 0x3C;
    PUSH32(esp, edx);
    MEM32(esp + 0x44) = ecx;
    ecx = esi + 0x10;
    edx = esp + 0x1C;
    PUSH32(esp, edx);
    { uint32_t _icall_target = MEM32(eax + 4); PUSH32(esp, 0x00082F5Fu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00082F5Cu); } /* indirect call */
    }

loc_00082F5F: ;
    RECOMP_PC(0x00082F5Fu);
    ecx = MEM32(eax);
    edx = MEM32(eax + 4);
    eax = MEM32(eax + 8);
    MEM32(esp + 0x2C) = ecx;
    ecx = esi;
    MEM32(esp + 0x30) = edx;
    MEM32(esp + 0x34) = eax;
    PUSH32(esp, 0x00082F7Au); RECOMP_ABI_CALL(0x00013760u, sub_00013760); /* call 0x00013760 */

loc_00082F7A: ;
    RECOMP_PC(0x00082F7Au);
    ecx = MEM32(esp + 0x2C);
    edx = MEM32(esp + 0x30);
    PUSH32(esp, 1);
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + 0x1C;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, eax);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    eax = esp;
    MEM32(eax) = ecx;
    ecx = MEM32(esp + 0x48);
    MEM32(eax + 8) = ecx;
    ecx = esi;
    MEM32(eax + 4) = edx;
    PUSH32(esp, 0x00082FA0u); RECOMP_ABI_CALL(0x00013760u, sub_00013760); /* call 0x00013760 */

loc_00082FA0: ;
    RECOMP_PC(0x00082FA0u);
    ecx = eax;
    PUSH32(esp, 0x00082FA7u); RECOMP_ABI_CALL(0x000806C0u, sub_000806C0); /* call 0x000806C0 */

loc_00082FA7: ;
    RECOMP_PC(0x00082FA7u);
    fp_push(MEMF(esp + 0x28)); /* fld float */
    fp_push(MEMF(esp + 0x24)); /* fld float */
    fp_push(MEMF(esp + 0x20)); /* fld float */
    goto loc_00082FC7;

loc_00082FB5: ;
    RECOMP_PC(0x00082FB5u);
    fp_push(MEMF(0x1C91E0)); /* fld float */
    fp_push(MEMF(0x1C91E0)); /* fld float */
    fp_push(MEMF(0x1C91E0)); /* fld float */

loc_00082FC7: ;
    RECOMP_PC(0x00082FC7u);
    edx = MEM32(esp + 0x14);
    eax = MEM32(esp + 0x18);
    ecx = MEM32(esp + 0x1C);
    MEM32(esi + 0x34) = edx;
    MEM32(esi + 0x38) = eax;
    MEM32(esi + 0x3C) = ecx;
    MEMF(esi + 0x40) = (float)fp_top(); fp_pop(); /* fstp */
    POP32(esp, edi);
    MEMF(esi + 0x44) = (float)fp_top(); fp_pop(); /* fstp */
    MEMF(esi + 0x48) = (float)fp_top(); fp_pop(); /* fstp */
    POP32(esp, esi);
    _fb = (uint32_t)(0x3C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x3C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; return; /* ret 4 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_000A9050
 * Original: 0x000A9050 - 0x000A9671 (1569 bytes, 398 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000A9050(void)
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

loc_000A9050: ;
    RECOMP_PC(0x000A9050u);
    _fb = (uint32_t)(0x38) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x38;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, esi);
    esi = ecx;
    eax = MEM32(esi + 0x18C);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000A966C; /* je: equal / zero */

loc_000A9064: ;
    RECOMP_PC(0x000A9064u);
    eax = MEM32(esi + 4);
    _fb = (uint32_t)(0x88) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + 0x88;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ecx = esi + 0x34;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, eax (32-bit) */
    MEM32(esi + 0x1E4) = 0;
    if (CMP_EQ(_fa, _fb)) goto loc_000A90B1; /* je: equal / zero */

loc_000A907D: ;
    RECOMP_PC(0x000A907Du);
    edx = MEM32(eax);
    MEM32(ecx) = edx;
    edx = MEM32(eax + 4);
    MEM32(ecx + 4) = edx;
    edx = MEM32(eax + 8);
    MEM32(ecx + 8) = edx;
    edx = MEM32(eax + 0xC);
    MEM32(ecx + 0xC) = edx;
    edx = MEM32(eax + 0x10);
    MEM32(ecx + 0x10) = edx;
    edx = MEM32(eax + 0x14);
    MEM32(ecx + 0x14) = edx;
    edx = MEM32(eax + 0x18);
    MEM32(ecx + 0x18) = edx;
    edx = MEM32(eax + 0x1C);
    MEM32(ecx + 0x1C) = edx;
    eax = MEM32(eax + 0x20);
    MEM32(ecx + 0x20) = eax;

loc_000A90B1: ;
    RECOMP_PC(0x000A90B1u);
    eax = MEM32(esi + 4);
    fp_push(MEMF(0x1CF1D8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(eax + 0x8C)); /* fmul dword ptr [eax + 0x8c] */
    edx = eax + 0x88;
    fp_push(MEMF(0x1CF1DC)); /* fld float */
    PUSH32(esp, edi);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(edx + 8)); /* fmul dword ptr [edx + 8] */
    edi = eax + 0x94;
    ecx = eax + 0xA0;
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    fp_push(MEMF(0x1CF1D4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(edx)); /* fmul dword ptr [edx] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    MEMF(esp + 8) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x1CF1D8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(edi + 4)); /* fmul dword ptr [edi + 4] */
    fp_push(MEMF(0x1CF1DC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(edi + 8)); /* fmul dword ptr [edi + 8] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    fp_push(MEMF(0x1CF1D4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(edi)); /* fmul dword ptr [edi] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    MEMF(esp + 0xC) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x1CF1D8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ecx + 4)); /* fmul dword ptr [ecx + 4] */
    fp_push(MEMF(0x1CF1DC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ecx + 8)); /* fmul dword ptr [ecx + 8] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    fp_push(MEMF(0x1CF1D4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ecx)); /* fmul dword ptr [ecx] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    fp_push(MEMF(esp + 8)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000A9147; /* jp: parity */

loc_000A913D: ;
    RECOMP_PC(0x000A913Du);
    fp_push(MEMF(esp + 8)); /* fld float */
    fp_top() = -fp_top(); /* fchs */
    MEMF(esp + 8) = (float)fp_top(); fp_pop(); /* fstp */

loc_000A9147: ;
    RECOMP_PC(0x000A9147u);
    fp_push(MEMF(esp + 0xC)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000A915A; /* jp: parity */

loc_000A9158: ;
    RECOMP_PC(0x000A9158u);
    fp_top() = -fp_top(); /* fchs */

loc_000A915A: ;
    RECOMP_PC(0x000A915Au);
    { double _t = fp_st1(); fp_push(_t); } /* fld st(1) */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000A916F; /* jp: parity */

loc_000A9169: ;
    RECOMP_PC(0x000A9169u);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    fp_top() = -fp_top(); /* fchs */
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */

loc_000A916F: ;
    RECOMP_PC(0x000A916Fu);
    fp_push(MEMF(esp + 8)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), fp_st1()); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp st(1) */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000A9195; /* jp: parity */

loc_000A917C: ;
    RECOMP_PC(0x000A917Cu);
    fp_pop(); /* fstp st(0) */
    fp_push(MEMF(esp + 8)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), fp_st1()); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp st(1) */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    fp_pop(); /* fstp st(0) */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000A91A8; /* jp: parity */

loc_000A918D: ;
    RECOMP_PC(0x000A918Du);
    MEM32(esi + 0x180) = edx;
    goto loc_000A91AE;

loc_000A9195: ;
    RECOMP_PC(0x000A9195u);
    g_fp_cmp = RECOMP_FCMP(fp_top(), fp_st1()); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp st(1) */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    fp_pop(); /* fstp st(0) */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000A91A8; /* jp: parity */

loc_000A91A0: ;
    RECOMP_PC(0x000A91A0u);
    MEM32(esi + 0x180) = edi;
    goto loc_000A91AE;

loc_000A91A8: ;
    RECOMP_PC(0x000A91A8u);
    MEM32(esi + 0x180) = ecx;

loc_000A91AE: ;
    RECOMP_PC(0x000A91AEu);
    ecx = MEM32(0x1CF1D4);
    MEM32(esi + 0x16C) = ecx;
    edx = MEM32(0x1CF1D8);
    MEM32(esi + 0x170) = edx;
    eax = MEM32(0x1CF1DC);
    MEM32(esi + 0x174) = eax;
    eax = MEM32(esi + 0x180);
    ecx = MEM32(eax);
    MEM32(esi + 0x160) = ecx;
    edx = MEM32(eax + 4);
    MEM32(esi + 0x164) = edx;
    eax = MEM32(eax + 8);
    MEM32(esi + 0x168) = eax;
    fp_push(MEMF(esi + 0x174)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x164)); /* fmul dword ptr [esi + 0x164] */
    edi = esi + 0x154;
    fp_push(MEMF(esi + 0x168)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x170)); /* fmul dword ptr [esi + 0x170] */
    fp_st1() = RECOMP_FP_PC(fp_st1() - fp_top()); fp_pop(); /* fsubp st(1) */
    fp_push(MEMF(esi + 0x168)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x16C)); /* fmul dword ptr [esi + 0x16c] */
    fp_push(MEMF(esi + 0x160)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x174)); /* fmul dword ptr [esi + 0x174] */
    fp_st1() = RECOMP_FP_PC(fp_st1() - fp_top()); fp_pop(); /* fsubp st(1) */
    fp_push(MEMF(esi + 0x160)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x170)); /* fmul dword ptr [esi + 0x170] */
    fp_push(MEMF(esi + 0x16C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x164)); /* fmul dword ptr [esi + 0x164] */
    fp_st1() = RECOMP_FP_PC(fp_st1() - fp_top()); fp_pop(); /* fsubp st(1) */
    MEMF(esp + 0x24) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = MEM32(esp + 0x24);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(edi) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(edi + 8) = ecx;
    ecx = edi;
    MEMF(edi + 4) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x000A925Eu); RECOMP_ABI_CALL(0x000190B0u, sub_000190B0); /* call 0x000190B0 */

loc_000A925E: ;
    RECOMP_PC(0x000A925Eu);
    fp_push(MEMF(edi + 8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x170)); /* fmul dword ptr [esi + 0x170] */
    fp_push(MEMF(edi + 4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x174)); /* fmul dword ptr [esi + 0x174] */
    fp_st1() = RECOMP_FP_PC(fp_st1() - fp_top()); fp_pop(); /* fsubp st(1) */
    fp_push(MEMF(edi)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x174)); /* fmul dword ptr [esi + 0x174] */
    fp_push(MEMF(edi + 8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x16C)); /* fmul dword ptr [esi + 0x16c] */
    fp_st1() = RECOMP_FP_PC(fp_st1() - fp_top()); fp_pop(); /* fsubp st(1) */
    fp_push(MEMF(edi + 4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x16C)); /* fmul dword ptr [esi + 0x16c] */
    fp_push(MEMF(edi)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x170)); /* fmul dword ptr [esi + 0x170] */
    fp_st1() = RECOMP_FP_PC(fp_st1() - fp_top()); fp_pop(); /* fsubp st(1) */
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(esp + 0x18);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(esi + 0x160) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0x168) = edx;
    MEMF(esi + 0x164) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esi + 4);
    fp_push(MEMF(esi + 0x74)); /* fld float */
    _fb = (uint32_t)(0x54) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + 0x54;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x5C)); /* fadd dword ptr [esi + 0x5c] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x1C9220)); /* fadd dword ptr [0x1c9220] */
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x16C)); /* fmul dword ptr [esi + 0x16c] */
    MEMF(esp + 0x10) = (float)fp_top(); fp_pop(); /* fstp */
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x170)); /* fmul dword ptr [esi + 0x170] */
    MEMF(esp + 0x14) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x174)); /* fmul dword ptr [esi + 0x174] */
    fp_push(MEMF(eax)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x10)); /* fsub dword ptr [esp + 0x10] */
    fp_push(MEMF(eax + 4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x14)); /* fsub dword ptr [esp + 0x14] */
    fp_push(MEMF(eax + 8)); /* fld float */
    PUSH32(esp, 0);
    fp_top() = RECOMP_FP_PC(fp_top() - g_fp_stack[(g_fp_top + 3) & 7]); /* fsub st(3) */
    PUSH32(esp, 0);
    MEMF(esp + 0x20) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esp + 0x20);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(esi + 0x148) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0x150) = eax;
    MEMF(esi + 0x14C) = (float)fp_top(); fp_pop(); /* fstp */
    fp_pop(); /* fstp st(0) */
    PUSH32(esp, 0x000A931Du); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000A931D: ;
    RECOMP_PC(0x000A931Du);
    ecx = MEM32(esi + 0x188);
    edx = MEM32(esi + 0x184);
    PUSH32(esp, 7);
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    eax = esi + 0xEC;
    PUSH32(esp, eax);
    PUSH32(esp, 0x000A9339u); RECOMP_ABI_CALL(0x000E9C80u, sub_000E9C80); /* call 0x000E9C80 */

loc_000A9339: ;
    RECOMP_PC(0x000A9339u);
    PUSH32(esp, 1);
    PUSH32(esp, 0);
    PUSH32(esp, 0x000A9342u); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000A9342: ;
    RECOMP_PC(0x000A9342u);
    fp_push(MEMF(0x1CF1D4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1AE140)); /* fmul dword ptr [0x1ae140] */
    ecx = MEM32(0x1D9FB8);
    _fb = (uint32_t)(0x20) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x20;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0);
    MEMF(esp + 0x20) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0);
    fp_push(MEMF(0x1CF1D8)); /* fld float */
    eax = esp + 0x24;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1AE140)); /* fmul dword ptr [0x1ae140] */
    PUSH32(esp, eax);
    MEMF(esp + 0x2C) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x1CF1DC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1AE140)); /* fmul dword ptr [0x1ae140] */
    MEMF(esp + 0x30) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(ecx + 0x38);
    ecx = MEM32(edx + 0x24);
    edx = MEM32(esi + 4);
    _fb = (uint32_t)(0x54) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edx = edx + 0x54;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, edx);
    PUSH32(esp, 0x000A9396u); RECOMP_ABI_CALL(0x000D37D0u, sub_000D37D0); /* call 0x000D37D0 */

loc_000A9396: ;
    RECOMP_PC(0x000A9396u);
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1BF1C4)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [0x1bf1c4] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_000A9416; /* jnp: not parity */

loc_000A93A3: ;
    RECOMP_PC(0x000A93A3u);
    fp_push(MEMF(esp + 0x1C)); /* fld float */
    eax = MEM32(esi + 4);
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    _fb = (uint32_t)(0x54) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + 0x54;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    fp_push(MEMF(esp + 0x20)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * g_fp_stack[(g_fp_top + 2) & 7]); /* fmul st(2) */
    MEMF(esp + 0x14) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x24)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * g_fp_stack[(g_fp_top + 2) & 7]); /* fmul st(2) */
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax)); /* fadd dword ptr [eax] */
    fp_st1() = fp_top(); fp_pop(); /* fstp st(1) */
    fp_push(MEMF(esp + 0x14)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 4)); /* fadd dword ptr [eax + 4] */
    fp_push(MEMF(esp + 0x18)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 8)); /* fadd dword ptr [eax + 8] */
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esp + 0x18);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(esi + 0x1C4) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0x1CC) = eax;
    MEMF(esi + 0x1C8) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = MEM32(0x1D9FB8);
    edx = MEM32(ecx + 0x38);
    eax = MEM32(edx + 0x24);
    eax = MEM32(eax + 0x34);
    ecx = MEM32(eax);
    edx = MEM32(eax + 4);
    eax = MEM32(eax + 8);
    MEM32(esp + 0x10) = ecx;
    MEM32(esp + 0x14) = edx;
    MEM32(esp + 0x18) = eax;
    goto loc_000A9430;

loc_000A9416: ;
    RECOMP_PC(0x000A9416u);
    fp_pop(); /* fstp st(0) */
    MEM32(esp + 0x10) = 0;
    MEM32(esp + 0x14) = 0x3F800000;
    MEM32(esp + 0x18) = 0;

loc_000A9430: ;
    RECOMP_PC(0x000A9430u);
    fp_push(MEMF(esp + 0x10)); /* fld float */
    PUSH32(esp, ecx);
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 0x194)); /* fsub dword ptr [esi + 0x194] */
    fp_push(MEMF(esp + 0x18)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 0x198)); /* fsub dword ptr [esi + 0x198] */
    fp_push(MEMF(esp + 0x1C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 0x19C)); /* fsub dword ptr [esi + 0x19c] */
    MEMF(esp + 0x34) = (float)fp_top(); fp_pop(); /* fstp */
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C9210)); /* fmul dword ptr [0x1c9210] */
    MEMF(esp + 0x38) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C9210)); /* fmul dword ptr [0x1c9210] */
    fp_push(MEMF(esp + 0x34)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C9210)); /* fmul dword ptr [0x1c9210] */
    fp_push(MEMF(esp + 0x38)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x194)); /* fadd dword ptr [esi + 0x194] */
    MEMF(esi + 0x194) = (float)fp_top(); fp_pop(); /* fstp */
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x198)); /* fadd dword ptr [esi + 0x198] */
    MEMF(esi + 0x198) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x19C)); /* fadd dword ptr [esi + 0x19c] */
    MEMF(esi + 0x19C) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esi + 0x19C)); /* fld float */
    fp_push(MEMF(esi + 0x198)); /* fld float */
    fp_push(MEMF(esi + 0x194)); /* fld float */
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
    PUSH32(esp, 0x000A94C9u); RECOMP_ABI_CALL(0x000CDE60u, sub_000CDE60); /* call 0x000CDE60 */

loc_000A94C9: ;
    RECOMP_PC(0x000A94C9u);
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [0x1c91e0] */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_000A9509; /* jnp: not parity */

loc_000A94D9: ;
    RECOMP_PC(0x000A94D9u);
    fp_top() = RECOMP_FP_PC(MEMF(0x1C91D0) / fp_top()); /* fdivr dword ptr [0x1c91d0] */
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x194)); /* fmul dword ptr [esi + 0x194] */
    MEMF(esi + 0x194) = (float)fp_top(); fp_pop(); /* fstp */
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x198)); /* fmul dword ptr [esi + 0x198] */
    MEMF(esi + 0x198) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x19C)); /* fmul dword ptr [esi + 0x19c] */
    MEMF(esi + 0x19C) = (float)fp_top(); fp_pop(); /* fstp */
    goto loc_000A9529;

loc_000A9509: ;
    RECOMP_PC(0x000A9509u);
    ecx = MEM32(esp + 0x10);
    fp_pop(); /* fstp st(0) */
    edx = MEM32(esp + 0x14);
    eax = MEM32(esp + 0x18);
    MEM32(esi + 0x194) = ecx;
    MEM32(esi + 0x198) = edx;
    MEM32(esi + 0x19C) = eax;

loc_000A9529: ;
    RECOMP_PC(0x000A9529u);
    ecx = MEM32(esi + 0x194);
    MEM32(esi + 0x1AC) = ecx;
    edx = MEM32(esi + 0x198);
    MEM32(esi + 0x1B0) = edx;
    eax = MEM32(esi + 0x19C);
    MEM32(esi + 0x1B4) = eax;
    fp_push(MEMF(0x1CF1DC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x198)); /* fmul dword ptr [esi + 0x198] */
    edi = esi + 0x1A0;
    fp_push(MEMF(0x1CF1D8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x19C)); /* fmul dword ptr [esi + 0x19c] */
    fp_st1() = RECOMP_FP_PC(fp_st1() - fp_top()); fp_pop(); /* fsubp st(1) */
    fp_push(MEMF(0x1CF1D4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x19C)); /* fmul dword ptr [esi + 0x19c] */
    fp_push(MEMF(0x1CF1DC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x194)); /* fmul dword ptr [esi + 0x194] */
    fp_st1() = RECOMP_FP_PC(fp_st1() - fp_top()); fp_pop(); /* fsubp st(1) */
    fp_push(MEMF(0x1CF1D8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x194)); /* fmul dword ptr [esi + 0x194] */
    fp_push(MEMF(0x1CF1D4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x198)); /* fmul dword ptr [esi + 0x198] */
    fp_st1() = RECOMP_FP_PC(fp_st1() - fp_top()); fp_pop(); /* fsubp st(1) */
    MEMF(esp + 0x3C) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = MEM32(esp + 0x3C);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(edi) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(edi + 8) = ecx;
    ecx = edi;
    MEMF(edi + 4) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x000A95BAu); RECOMP_ABI_CALL(0x0001FE00u, sub_0001FE00); /* call 0x0001FE00 */

loc_000A95BA: ;
    RECOMP_PC(0x000A95BAu);
    fp_push(MEMF(esi + 0x1B4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(edi + 4)); /* fmul dword ptr [edi + 4] */
    fp_push(MEMF(esi + 0x1B0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(edi + 8)); /* fmul dword ptr [edi + 8] */
    fp_st1() = RECOMP_FP_PC(fp_st1() - fp_top()); fp_pop(); /* fsubp st(1) */
    fp_push(MEMF(edi + 8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x1AC)); /* fmul dword ptr [esi + 0x1ac] */
    fp_push(MEMF(esi + 0x1B4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(edi)); /* fmul dword ptr [edi] */
    fp_st1() = RECOMP_FP_PC(fp_st1() - fp_top()); fp_pop(); /* fsubp st(1) */
    fp_push(MEMF(edi)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x1B0)); /* fmul dword ptr [esi + 0x1b0] */
    fp_push(MEMF(edi + 4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x1AC)); /* fmul dword ptr [esi + 0x1ac] */
    POP32(esp, edi);
    fp_st1() = RECOMP_FP_PC(fp_st1() - fp_top()); fp_pop(); /* fsubp st(1) */
    MEMF(esp + 0x38) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(esp + 0x38);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(esi + 0x1B8) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0x1C0) = edx;
    MEMF(esi + 0x1BC) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0x1D0) = 0x3FA00000;
    fp_push(MEMF(0x1CF1D8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x198)); /* fmul dword ptr [esi + 0x198] */
    fp_push(MEMF(0x1CF1DC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x19C)); /* fmul dword ptr [esi + 0x19c] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    fp_push(MEMF(0x1CF1D4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x194)); /* fmul dword ptr [esi + 0x194] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000A9652; /* jp: parity */

loc_000A9650: ;
    RECOMP_PC(0x000A9650u);
    fp_top() = -fp_top(); /* fchs */

loc_000A9652: ;
    RECOMP_PC(0x000A9652u);
    fp_push(MEMF(0x1BF3C4)); /* fld float */
    MEM32(esi + 0x1D8) = 1;
    fp_top() = RECOMP_FP_PC(fp_top() / fp_st1()); /* fdiv st(1) */
    MEMF(esi + 0x1D4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_pop(); /* fstp st(0) */

loc_000A966C: ;
    RECOMP_PC(0x000A966Cu);
    POP32(esp, esi);
    _fb = (uint32_t)(0x38) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x38;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_0009E240
 * Original: 0x0009E240 - 0x0009E54F (783 bytes, 217 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_0009E240(void)
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

loc_0009E240: ;
    RECOMP_PC(0x0009E240u);
    eax = MEM32(0x1D7424);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    edi = MEM32(esp + 0x14);
    ebx = ecx;
    if (CMP_NE(_fa, _fb)) goto loc_0009E278; /* jne: not equal / not zero */

loc_0009E253: ;
    RECOMP_PC(0x0009E253u);
    eax = MEM32(0x1D9FB8);
    ecx = MEM32(eax + 0x38);
    PUSH32(esp, edi);
    QPR2_LOADER_CALL(0x00030800u, sub_00030800, 0x0009E261u, 4u); /* call 0x00030800 */

loc_0009E261: ;
    RECOMP_PC(0x0009E261u);
    eax = MEM32(0x1D7424);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_0009E278; /* jne: not equal / not zero */

loc_0009E26A: ;
    RECOMP_PC(0x0009E26Au);
    ecx = MEM32(0x1D9FB8);
    ecx = MEM32(ecx + 0x38);
    edx = MEM32(ecx);
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_target = MEM32(edx + 0x28); PUSH32(esp, 0x0009E278u); QPR2_LOADER_ICALL(_icall_target, _icall_esp, 0x0009E275u); } /* indirect call */
    }

loc_0009E278: ;
    RECOMP_PC(0x0009E278u);
    ecx = MEM32(0x1D3EC0);
    QPR2_LOADER_CALL(0x000F36D0u, sub_000F36D0, 0x0009E283u, 0u); /* call 0x000F36D0 */

loc_0009E283: ;
    RECOMP_PC(0x0009E283u);
    eax = MEM32(0x1D7438);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0009E2BA; /* je: equal / zero */

loc_0009E28C: ;
    RECOMP_PC(0x0009E28Cu);
    PUSH32(esp, 0x1B01);
    QPR2_LOADER_CALL(0x00193A20u, sub_00193A20, 0x0009E296u, 4u); /* call 0x00193A20 */

loc_0009E296: ;
    RECOMP_PC(0x0009E296u);
    eax = MEM32(0x1D9FB8);
    ecx = MEM32(eax + 0x14);
    edx = MEM32(ecx + 0x3C);
    ecx = MEM32(edx + 0x1C);
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ecx = ecx + 8;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, ecx);
    ecx = MEM32(eax + 0x38);
    QPR2_LOADER_CALL(0x00030CA0u, sub_00030CA0, 0x0009E2B0u, 4u); /* call 0x00030CA0 */

loc_0009E2B0: ;
    RECOMP_PC(0x0009E2B0u);
    PUSH32(esp, 0x1B02);
    QPR2_LOADER_CALL(0x00193A20u, sub_00193A20, 0x0009E2BAu, 4u); /* call 0x00193A20 */

loc_0009E2BA: ;
    RECOMP_PC(0x0009E2BAu);
    ecx = MEM32(0x1D3EA4);
    ebp = 1;
    PUSH32(esp, ebp);
    PUSH32(esp, 0);
    QPR2_LOADER_CALL(0x000EECD0u, sub_000EECD0, 0x0009E2CDu, 8u); /* call 0x000EECD0 */

loc_0009E2CD: ;
    RECOMP_PC(0x0009E2CDu);
    edx = MEM32(ebx + 0xB0);
    ecx = MEM32(0x1D3EC8);
    PUSH32(esp, 0);
    PUSH32(esp, edx);
    PUSH32(esp, edi);
    QPR2_LOADER_CALL(0x000F1000u, sub_000F1000, 0x0009E2E2u, 12u); /* call 0x000F1000 */

loc_0009E2E2: ;
    RECOMP_PC(0x0009E2E2u);
    QPR2_LOADER_CALL(0x00033CA0u, sub_00033CA0, 0x0009E2E7u, 0u); /* call 0x00033CA0 */

loc_0009E2E7: ;
    RECOMP_PC(0x0009E2E7u);
    eax = MEM32(0x1D9FB8);
    ecx = MEM32(eax + 0x10);
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, 0 (32-bit) */
    if (CMP_LE(_fas, _fbs)) goto loc_0009E31E; /* jle: less or equal (signed <=) */

loc_0009E2F5: ;
    RECOMP_PC(0x0009E2F5u);
    edi = 0x14;
    /* nop */

loc_0009E300: ;
    RECOMP_PC(0x0009E300u);
    eax = MEM32(edi + eax);
    edx = MEM32(eax);
    ecx = eax;
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_target = MEM32(edx + 0x28); PUSH32(esp, 0x0009E30Au); QPR2_LOADER_ICALL(_icall_target, _icall_esp, 0x0009E307u); } /* indirect call */
    }

loc_0009E30A: ;
    RECOMP_PC(0x0009E30Au);
    eax = MEM32(0x1D9FB8);
    ecx = MEM32(eax + 0x10);
    esi++;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edi = edi + 4;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, ecx (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_0009E300; /* jl: less (signed <) */

loc_0009E31A: ;
    RECOMP_PC(0x0009E31Au);
    edi = MEM32(esp + 0x14);

loc_0009E31E: ;
    RECOMP_PC(0x0009E31Eu);
    eax = MEM32(eax + 0x38);
    ecx = MEM32(eax + 0x1C);
    PUSH32(esp, ecx);
    ecx = MEM32(0x1D3ECC);
    QPR2_LOADER_CALL(0x000F2F40u, sub_000F2F40, 0x0009E330u, 4u); /* call 0x000F2F40 */

loc_0009E330: ;
    RECOMP_PC(0x0009E330u);
    eax = MEM32(0x1D9FF0);
    ecx = MEM32(eax);
    PUSH32(esp, eax);
    QPR2_LOADER_CALL(0x00117F80u, sub_00117F80, 0x0009E33Du, 4u); /* call 0x00117F80 */

loc_0009E33D: ;
    RECOMP_PC(0x0009E33Du);
    edx = MEM32(0x1D9FB8);
    ecx = MEM32(edx + 0x38);
    eax = MEM32(ecx);
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_target = MEM32(eax + 0x3C); PUSH32(esp, 0x0009E34Bu); QPR2_LOADER_ICALL(_icall_target, _icall_esp, 0x0009E348u); } /* indirect call */
    }

loc_0009E34B: ;
    RECOMP_PC(0x0009E34Bu);
    ecx = MEM32(0x1D3EA4);
    QPR2_LOADER_CALL(0x000EE620u, sub_000EE620, 0x0009E356u, 0u); /* call 0x000EE620 */

loc_0009E356: ;
    RECOMP_PC(0x0009E356u);
    ecx = MEM32(0x1D9FB8);
    edx = MEM32(ecx + 0x38);
    eax = MEM32(edx + 0x1C);
    ecx = MEM32(eax + 0x60);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0009E374; /* je: equal / zero */

loc_0009E369: ;
    RECOMP_PC(0x0009E369u);
    ecx = MEM32(0x1D3EC0);
    QPR2_LOADER_CALL(0x000F3970u, sub_000F3970, 0x0009E374u, 0u); /* call 0x000F3970 */

loc_0009E374: ;
    RECOMP_PC(0x0009E374u);
    ecx = MEM32(0x1DA00C);
    ecx = MEM32(ecx + 0x18);
    QPR2_LOADER_CALL(0x00117D40u, sub_00117D40, 0x0009E382u, 0u); /* call 0x00117D40 */

loc_0009E382: ;
    RECOMP_PC(0x0009E382u);
    edx = MEM32(0x1DA00C);
    ecx = MEM32(edx + 0x1C);
    QPR2_LOADER_CALL(0x00106F50u, sub_00106F50, 0x0009E390u, 0u); /* call 0x00106F50 */

loc_0009E390: ;
    RECOMP_PC(0x0009E390u);
    ecx = ebx;
    QPR2_LOADER_CALL(0x0009DE50u, sub_0009DE50, 0x0009E397u, 0u); /* call 0x0009DE50 */

loc_0009E397: ;
    RECOMP_PC(0x0009E397u);
    PUSH32(esp, edi);
    ecx = ebx;
    QPR2_LOADER_CALL(0x0009DEB0u, sub_0009DEB0, 0x0009E39Fu, 4u); /* call 0x0009DEB0 */

loc_0009E39F: ;
    RECOMP_PC(0x0009E39Fu);
    eax = MEM32(0x1D9FB8);
    ecx = MEM32(eax + 0x38);
    ecx = MEM32(ecx + 0x1C);
    QPR2_LOADER_CALL(0x001035D0u, sub_001035D0, 0x0009E3AFu, 0u); /* call 0x001035D0 */

loc_0009E3AF: ;
    RECOMP_PC(0x0009E3AFu);
    edx = MEM32(0x1DA00C);
    ecx = MEM32(edx + 0x20);
    QPR2_LOADER_CALL(0x00105E20u, sub_00105E20, 0x0009E3BDu, 0u); /* call 0x00105E20 */

loc_0009E3BD: ;
    RECOMP_PC(0x0009E3BDu);
    eax = MEM32(ebx + 0xB0);
    ecx = MEM32(0x1D3EC8);
    PUSH32(esp, eax);
    PUSH32(esp, edi);
    QPR2_LOADER_CALL(0x000F10D0u, sub_000F10D0, 0x0009E3D0u, 8u); /* call 0x000F10D0 */

loc_0009E3D0: ;
    RECOMP_PC(0x0009E3D0u);
    ecx = MEM32(0x1DA00C);
    esi = MEM32(ecx + 0x24);
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0009E410; /* je: equal / zero */

loc_0009E3DD: ;
    RECOMP_PC(0x0009E3DDu);
    ecx = MEM32(ebx + 0xB4);
    QPR2_LOADER_CALL(0x0007E140u, sub_0007E140, 0x0009E3E8u, 0u); /* call 0x0007E140 */

loc_0009E3E8: ;
    RECOMP_PC(0x0009E3E8u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0009E410; /* je: equal / zero */

loc_0009E3EC: ;
    RECOMP_PC(0x0009E3ECu);
    edx = MEM32(0x1CCBF0);
    PUSH32(esp, edx);
    ecx = esi;
    MEM32(esi + 0x98) = ebp;
    QPR2_LOADER_CALL(0x00116EA0u, sub_00116EA0, 0x0009E400u, 4u); /* call 0x00116EA0 */

loc_0009E400: ;
    RECOMP_PC(0x0009E400u);
    ecx = esi;
    QPR2_LOADER_CALL(0x001173E0u, sub_001173E0, 0x0009E407u, 0u); /* call 0x001173E0 */

loc_0009E407: ;
    RECOMP_PC(0x0009E407u);
    PUSH32(esp, 0);
    ecx = esi;
    QPR2_LOADER_CALL(0x001175D0u, sub_001175D0, 0x0009E410u, 4u); /* call 0x001175D0 */

loc_0009E410: ;
    RECOMP_PC(0x0009E410u);
    ecx = MEM32(0x1D3EC0);
    PUSH32(esp, 7);
    QPR2_LOADER_CALL(0x000F3770u, sub_000F3770, 0x0009E41Du, 4u); /* call 0x000F3770 */

loc_0009E41D: ;
    RECOMP_PC(0x0009E41Du);
    ecx = MEM32(0x1D9FF8);
    QPR2_LOADER_CALL(0x00107DA0u, sub_00107DA0, 0x0009E428u, 0u); /* call 0x00107DA0 */

loc_0009E428: ;
    RECOMP_PC(0x0009E428u);
    eax = MEM32(0x1D7424);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0009E4EB; /* je: equal / zero */

loc_0009E435: ;
    RECOMP_PC(0x0009E435u);
    PUSH32(esp, 0x1B01);
    QPR2_LOADER_CALL(0x00193A20u, sub_00193A20, 0x0009E43Fu, 4u); /* call 0x00193A20 */

loc_0009E43F: ;
    RECOMP_PC(0x0009E43Fu);
    eax = MEM32(0x1D9FB8);
    ecx = MEM32(eax + 0x38);
    edx = MEM32(ecx);
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_target = MEM32(edx + 0x28); PUSH32(esp, 0x0009E44Cu); QPR2_LOADER_ICALL(_icall_target, _icall_esp, 0x0009E449u); } /* indirect call */
    }

loc_0009E44C: ;
    RECOMP_PC(0x0009E44Cu);
    eax = MEM32(0x1D9FB8);
    ecx = MEM32(eax + 0x38);
    edx = MEM32(ecx);
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_target = MEM32(edx + 0x3C); PUSH32(esp, 0x0009E459u); QPR2_LOADER_ICALL(_icall_target, _icall_esp, 0x0009E456u); } /* indirect call */
    }

loc_0009E459: ;
    RECOMP_PC(0x0009E459u);
    eax = MEM32(0x1DC3CC);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_0009E49B; /* jne: not equal / not zero */

loc_0009E462: ;
    RECOMP_PC(0x0009E462u);
    edx = ebp;
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = ebp;
    QPR2_LOADER_CALL(0x00193BC0u, sub_00193BC0, 0x0009E474u, 0u); /* call 0x00193BC0 */

loc_0009E474: ;
    RECOMP_PC(0x0009E474u);
    eax = MEM32(0x1DC3CC);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    MEM32(0x1A51F8) = ebp;
    if (CMP_NE(_fa, _fb)) goto loc_0009E49B; /* jne: not equal / not zero */

loc_0009E483: ;
    RECOMP_PC(0x0009E483u);
    edx = ebp;
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = ebp;
    QPR2_LOADER_CALL(0x00193BC0u, sub_00193BC0, 0x0009E495u, 0u); /* call 0x00193BC0 */

loc_0009E495: ;
    RECOMP_PC(0x0009E495u);
    MEM32(0x1A51F8) = ebp;

loc_0009E49B: ;
    RECOMP_PC(0x0009E49Bu);
    eax = MEM32(0x1DC444);
    esi = 0x203;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, esi (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0009E4E1; /* je: equal / zero */

loc_0009E4A9: ;
    RECOMP_PC(0x0009E4A9u);
    edx = esi;
    ecx = 0x40354;
    MEM32(0x1DC444) = esi;
    QPR2_LOADER_CALL(0x00193BC0u, sub_00193BC0, 0x0009E4BBu, 0u); /* call 0x00193BC0 */

loc_0009E4BB: ;
    RECOMP_PC(0x0009E4BBu);
    _fa = (uint32_t)(MEM32(0x1DC444)) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC444), esi (32-bit) */
    MEM32(0x1A51DC) = esi;
    if (CMP_EQ(_fa, _fb)) goto loc_0009E4E1; /* je: equal / zero */

loc_0009E4C9: ;
    RECOMP_PC(0x0009E4C9u);
    edx = esi;
    ecx = 0x40354;
    MEM32(0x1DC444) = esi;
    QPR2_LOADER_CALL(0x00193BC0u, sub_00193BC0, 0x0009E4DBu, 0u); /* call 0x00193BC0 */

loc_0009E4DB: ;
    RECOMP_PC(0x0009E4DBu);
    MEM32(0x1A51DC) = esi;

loc_0009E4E1: ;
    RECOMP_PC(0x0009E4E1u);
    PUSH32(esp, 0x1B02);
    QPR2_LOADER_CALL(0x00193A20u, sub_00193A20, 0x0009E4EBu, 4u); /* call 0x00193A20 */

loc_0009E4EB: ;
    RECOMP_PC(0x0009E4EBu);
    eax = MEM32(0x1D9FB8);
    ecx = MEM32(eax + 0x10);
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, 0 (32-bit) */
    if (CMP_LE(_fas, _fbs)) goto loc_0009E51F; /* jle: less or equal (signed <=) */

loc_0009E4F9: ;
    RECOMP_PC(0x0009E4F9u);
    edi = 0x14;
    edi = edi;

loc_0009E500: ;
    RECOMP_PC(0x0009E500u);
    eax = MEM32(edi + eax);
    ecx = MEM32(eax + 0x3C);
    QPR2_LOADER_CALL(0x0001ED70u, sub_0001ED70, 0x0009E50Bu, 0u); /* call 0x0001ED70 */

loc_0009E50B: ;
    RECOMP_PC(0x0009E50Bu);
    eax = MEM32(0x1D9FB8);
    ecx = MEM32(eax + 0x10);
    esi++;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edi = edi + 4;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, ecx (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_0009E500; /* jl: less (signed <) */

loc_0009E51B: ;
    RECOMP_PC(0x0009E51Bu);
    edi = MEM32(esp + 0x14);

loc_0009E51F: ;
    RECOMP_PC(0x0009E51Fu);
    ecx = MEM32(0x1D9FE4);
    edx = MEM32(0x1DC340);
    fp_push(MEMF(edx + 8)); /* fld float */
    eax = MEM32(0x1D9FB4);
    PUSH32(esp, ecx);
    PUSH32(esp, ecx);
    ecx = MEM32(0x1DA00C);
    ecx = MEM32(ecx + 0x14);
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, edi);
    PUSH32(esp, eax);
    QPR2_LOADER_CALL(0x00101FB0u, sub_00101FB0, 0x0009E548u, 16u); /* call 0x00101FB0 */

loc_0009E548: ;
    RECOMP_PC(0x0009E548u);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    esp += 8; return; /* ret 4 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

