/* Bounded routines recovered from the supplied retail disassembly. */

#define RECOMP_GENERATED_CODE

#include "recomp/gen/recomp_types.h"

#include "recomp/gen/recomp_funcs.h"

void qpr2_recovered_00027E00(void);

void qpr2_recovered_00084440(void);

/**
 * qpr2_recovered_00027E00
 * Original: 0x00027E00 - 0x00027F77 (375 bytes, 107 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00027E00(void)
{
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;

loc_00027E00: ;
    RECOMP_PC(0x00027E00u);
    edx = MEM32(esp + 4);
    PUSH32(esp, esi);
    esi = ecx;
    ecx = MEM32(esp + 0xC);
    eax = MEM32(esi);
    PUSH32(esp, edi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0x54); PUSH32(esp, 0x00027E15u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00027E12u); } /* indirect call */
    }

loc_00027E15: ;
    RECOMP_PC(0x00027E15u);
    eax = MEM32(esi);
    ecx = esi;
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_target = MEM32(eax + 4); PUSH32(esp, 0x00027E1Cu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00027E19u); } /* indirect call */
    }

loc_00027E1C: ;
    RECOMP_PC(0x00027E1Cu);
    ecx = MEM32(esi + 0x3C);
    edx = MEM32(ecx);
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_target = MEM32(edx + 0x6C); PUSH32(esp, 0x00027E24u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00027E21u); } /* indirect call */
    }

loc_00027E24: ;
    RECOMP_PC(0x00027E24u);
    eax = MEM32(esi + 0x44);
    ecx = MEM32(eax + 0x278);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, edi (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00027E43; /* je: equal / zero */

loc_00027E33: ;
    RECOMP_PC(0x00027E33u);
    ecx = MEM32(esi + 0x3C);
    PUSH32(esp, 0x00027E3Bu); RECOMP_ABI_CALL(0x0001A720u, sub_0001A720); /* call 0x0001A720 */

loc_00027E3B: ;
    RECOMP_PC(0x00027E3Bu);
    ecx = MEM32(esi + 0x44);
    PUSH32(esp, 0x00027E43u); RECOMP_ABI_CALL(0x00028950u, sub_00028950); /* call 0x00028950 */

loc_00027E43: ;
    RECOMP_PC(0x00027E43u);
    eax = MEM32(esi + 0x3C);
    ecx = eax + 0x88;
    PUSH32(esp, ecx);
    ecx = MEM32(esi + 0x44);
    _fb = (uint32_t)(0x54) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + 0x54;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, eax);
    PUSH32(esp, 0x00027E59u); RECOMP_ABI_CALL(0x00029460u, sub_00029460); /* call 0x00029460 */

loc_00027E59: ;
    RECOMP_PC(0x00027E59u);
    edx = MEM32(0x1D9FB8);
    PUSH32(esp, edx);
    ecx = esi;
    PUSH32(esp, 0x00027E67u); RECOMP_ABI_CALL(0x00026B30u, sub_00026B30); /* call 0x00026B30 */

loc_00027E67: ;
    RECOMP_PC(0x00027E67u);
    ecx = MEM32(esi + 0x48);
    PUSH32(esp, 0x00027E6Fu); RECOMP_ABI_CALL(0x000AC0D0u, sub_000AC0D0); /* call 0x000AC0D0 */

loc_00027E6F: ;
    RECOMP_PC(0x00027E6Fu);
    ecx = MEM32(esi + 0x48);
    PUSH32(esp, 0x00027E77u); RECOMP_ABI_CALL(0x000AD350u, sub_000AD350); /* call 0x000AD350 */

loc_00027E77: ;
    RECOMP_PC(0x00027E77u);
    ecx = MEM32(esi + 0x5C);
    eax = MEM32(ecx);
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_target = MEM32(eax + 0x10); PUSH32(esp, 0x00027E7Fu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00027E7Cu); } /* indirect call */
    }

loc_00027E7F: ;
    RECOMP_PC(0x00027E7Fu);
    MEM32(esi + 0x94) = edi;
    MEM32(esi + 0x98) = edi;
    MEM32(esi + 0x9C) = edi;
    MEM32(esi + 0xA8) = edi;
    MEM32(esi + 0xA4) = edi;
    MEM32(esi + 0xA0) = edi;
    eax = MEM32(esi + 0x20);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edi (32-bit) */
    MEM32(esi + 0xB4) = edi;
    MEM32(esi + 0x2C) = edi;
    MEM32(esi + 0xB8) = edi;
    MEM32(esi + 0xBC) = edi;
    MEM32(esi + 0xB0) = edi;
    if (CMP_EQ(_fa, _fb)) goto loc_00027ED5; /* je: equal / zero */

loc_00027EC5: ;
    RECOMP_PC(0x00027EC5u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(2) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 2 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_00027ED2; /* jne: not equal / not zero */

loc_00027ECA: ;
    RECOMP_PC(0x00027ECAu);
    ecx = MEM32(esi + 0x18);
    PUSH32(esp, 0x00027ED2u); RECOMP_ABI_CALL(0x0007B960u, sub_0007B960); /* call 0x0007B960 */

loc_00027ED2: ;
    RECOMP_PC(0x00027ED2u);
    MEM32(esi + 0x20) = edi;

loc_00027ED5: ;
    RECOMP_PC(0x00027ED5u);
    MEM32(esi + 0x34) = 0x3F800000;
    MEM32(esi + 0xC0) = edi;
    MEM32(esi + 0xC4) = edi;
    MEM32(esi + 0xC8) = edi;
    MEM32(esi + 0xCC) = edi;
    MEM32(esi + 0xD0) = edi;
    MEM32(esi + 0xD4) = edi;
    MEM32(esi + 0xD8) = edi;
    eax = esi + 0xE8;
    ecx = 8;

loc_00027F11: ;
    RECOMP_PC(0x00027F11u);
    MEM32(eax + -8) = edi;
    MEM32(eax + -4) = edi;
    MEM32(eax) = edi;
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + 0xC;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ecx--;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    if ((_fa != 0)) goto loc_00027F11; /* jne: not equal / not zero */

loc_00027F1F: ;
    RECOMP_PC(0x00027F1Fu);
    edx = MEM32(esi + 0x84);
    ecx = MEM32(esi + 0x80);
    eax = edx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edx (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00027F41; /* je: equal / zero */

loc_00027F31: ;
    RECOMP_PC(0x00027F31u);
    PUSH32(esp, ebx);

loc_00027F32: ;
    RECOMP_PC(0x00027F32u);
    ebx = MEM32(eax);
    MEM32(ecx) = ebx;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + 4;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ecx = ecx + 4;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edx (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_00027F32; /* jne: not equal / not zero */

loc_00027F40: ;
    RECOMP_PC(0x00027F40u);
    POP32(esp, ebx);

loc_00027F41: ;
    RECOMP_PC(0x00027F41u);
    MEM32(esi + 0x84) = ecx;
    MEM32(esi + 0x90) = edi;
    ecx = MEM32(esi + 0x44);
    PUSH32(esp, 1);
    PUSH32(esp, 0x00027F57u); RECOMP_ABI_CALL(0x0002EDD0u, sub_0002EDD0); /* call 0x0002EDD0 */

loc_00027F57: ;
    RECOMP_PC(0x00027F57u);
    ecx = MEM32(esi + 0x3C);
    PUSH32(esp, 1);
    _fb = (uint32_t)(0x4C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ecx = ecx + 0x4C;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x00027F64u); RECOMP_ABI_CALL(0x0006E470u, sub_0006E470); /* call 0x0006E470 */

loc_00027F64: ;
    RECOMP_PC(0x00027F64u);
    PUSH32(esp, 1);
    ecx = esi;
    PUSH32(esp, 0x00027F6Du); RECOMP_ABI_CALL(0x00026F20u, sub_00026F20); /* call 0x00026F20 */

loc_00027F6D: ;
    RECOMP_PC(0x00027F6Du);
    POP32(esp, edi);
    eax = 1;
    POP32(esp, esi);
    esp += 12; return; /* ret 8 */

}


/**
 * qpr2_recovered_00084440
 * Original: 0x00084440 - 0x00084516 (214 bytes, 70 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00084440(void)
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

loc_00084440: ;
    RECOMP_PC(0x00084440u);
    _fb = (uint32_t)(0x44) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x44;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    esi = ecx;
    PUSH32(esp, 0x0008444Du); RECOMP_ABI_CALL(0x00013760u, sub_00013760); /* call 0x00013760 */

loc_0008444D: ;
    RECOMP_PC(0x0008444Du);
    fp_push(MEMF(eax + 0x34)); /* fld float */
    _fb = (uint32_t)(0x34) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + 0x34;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEMF(esp + 0x14) = (float)fp_top(); /* fst */
    fp_push(MEMF(eax + 4)); /* fld float */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    MEMF(esp + 0x18) = (float)fp_top(); /* fst */
    eax = MEM32(eax + 8);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    ecx = eax;
    MEMF(esi + 0x34) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0x3C) = ecx;
    ecx = 0x1CEFD8;
    MEMF(esi + 0x38) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esp + 0x1C) = eax;
    MEM32(esi + 0x48) = edi;
    MEM32(esi + 0x44) = edi;
    MEM32(esi + 0x40) = edi;
    PUSH32(esp, 0x00084487u); RECOMP_ABI_CALL(0x000CE350u, sub_000CE350); /* call 0x000CE350 */

loc_00084487: ;
    RECOMP_PC(0x00084487u);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0xC)); /* fmul dword ptr [esi + 0xc] */
    MEM32(esp + 0x10) = 0x40490FDB;
    ebx = esp + 0xC;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1AF36C)); /* fmul dword ptr [0x1af36c] */
    MEMF(esp + 0xC) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x1AF36C)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esp + 0xC)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esp + 0xc] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_000844B5; /* je: equal / zero */

loc_000844B1: ;
    RECOMP_PC(0x000844B1u);
    ebx = esp + 0x10;

loc_000844B5: ;
    RECOMP_PC(0x000844B5u);
    ecx = esi;
    PUSH32(esp, 0x000844BCu); RECOMP_ABI_CALL(0x00013760u, sub_00013760); /* call 0x00013760 */

loc_000844BC: ;
    RECOMP_PC(0x000844BCu);
    fp_push(MEMF(ebx)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(eax + 0x40)); /* fdiv dword ptr [eax + 0x40] */
    eax = esp + 0x2C;
    MEMF(esp + 0x10) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(esp + 0x10);
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    PUSH32(esp, 0x000844D4u); RECOMP_ABI_CALL(0x000CF300u, sub_000CF300); /* call 0x000CF300 */

loc_000844D4: ;
    RECOMP_PC(0x000844D4u);
    PUSH32(esp, eax);
    ecx = esp + 0x20;
    PUSH32(esp, ecx);
    edx = esp + 0x30;
    PUSH32(esp, edx);
    PUSH32(esp, 0x000844E4u); RECOMP_ABI_CALL(0x000CE420u, sub_000CE420); /* call 0x000CE420 */

loc_000844E4: ;
    RECOMP_PC(0x000844E4u);
    eax = MEM32(esp + 0x34);
    ecx = MEM32(esp + 0x38);
    edx = MEM32(esp + 0x3C);
    MEM32(esi + 0x18) = ecx;
    MEM32(esi + 0x14) = eax;
    MEM32(esi + 0x1C) = edx;
    _fb = (uint32_t)(0x14) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x14;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esi + 0x20) = edi;
    MEM32(esi + 0x24) = edi;
    MEM32(esi + 0x28) = edi;
    eax = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edi);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0xC); PUSH32(esp, 0x0008450Du); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x0008450Au); } /* indirect call */
    }

loc_0008450D: ;
    RECOMP_PC(0x0008450Du);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    _fb = (uint32_t)(0x44) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x44;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; return; /* ret 4 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

