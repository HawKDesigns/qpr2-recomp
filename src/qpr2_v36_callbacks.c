#include "qpr2_game_timing.h"
/* Bounded routines recovered from the supplied retail disassembly. */

#define RECOMP_GENERATED_CODE

#include "recomp/gen/recomp_types.h"

#include "recomp/gen/recomp_funcs.h"

void qpr2_recovered_000E0220(void);

void qpr2_recovered_000DA770(void);

void qpr2_recovered_00085680(void);

void qpr2_recovered_000C3310(void);

void qpr2_recovered_000C47A0(void);

void qpr2_recovered_000C3AF0(void);

void qpr2_recovered_000C2DD0(void);

void qpr2_recovered_000C6140(void);

void qpr2_recovered_000C0860(void);

/**
 * qpr2_recovered_000E0220
 * Original: 0x000E0220 - 0x000E0578 (856 bytes, 245 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000E0220(void)
{
    Qpr2GameTiming _game_time = qpr2_game_timing_begin(0x000E0220u, ecx, MEM32(esp + 4), MEM32(ecx + 0x108));
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

loc_000E0220: ;
    RECOMP_PC(0x000E0220u);
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x1C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    esi = ecx;
    ecx = MEM32(esi + 0x108);
    eax = MEM32(esi + 4);
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, ebx (32-bit) */
    ebp = eax;
    if (CMP_EQ(_fa, _fb)) goto loc_000E056F; /* je: equal / zero */

loc_000E023D: ;
    RECOMP_PC(0x000E023Du);
    _fb = (uint32_t)(0x3C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + 0x3C;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, eax);
    eax = esi + 0x40;
    PUSH32(esp, eax);
    ecx = esp + 0x24;
    PUSH32(esp, ecx);
    PUSH32(esp, 0x000E024Fu); RECOMP_ABI_CALL(0x000CE420u, sub_000CE420); /* call 0x000CE420 */

loc_000E024F: ;
    RECOMP_PC(0x000E024Fu);
    edx = MEM32(eax);
    MEM32(esi + 0x80) = edx;
    ecx = MEM32(eax + 4);
    MEM32(esi + 0x84) = ecx;
    edx = MEM32(eax + 8);
    MEM32(esi + 0x88) = edx;
    eax = MEM32(esi + 4);
    _fb = (uint32_t)(0x3C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + 0x3C;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, eax);
    ecx = esi + 0x34;
    PUSH32(esp, ecx);
    edx = esp + 0x30;
    PUSH32(esp, edx);
    PUSH32(esp, 0x000E027Eu); RECOMP_ABI_CALL(0x000CE420u, sub_000CE420); /* call 0x000CE420 */

loc_000E027E: ;
    RECOMP_PC(0x000E027Eu);
    ecx = MEM32(eax);
    MEM32(esi + 0x74) = ecx;
    edx = MEM32(eax + 4);
    MEM32(esi + 0x78) = edx;
    eax = MEM32(eax + 8);
    MEM32(esi + 0x7C) = eax;
    eax = MEM32(esi + 0xBC);
    _fb = (uint32_t)(0x18) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x18;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebx (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000E0318; /* jne: not equal / not zero */

loc_000E029C: ;
    RECOMP_PC(0x000E029Cu);
    fp_push(MEMF(esi + 0x58)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 8)); /* fmul dword ptr [esi + 8] */
    fp_push(MEMF(esi + 0x5C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0xC)); /* fmul dword ptr [esi + 0xc] */
    fp_push(MEMF(esi + 0xC)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000E02D8; /* jp: parity */

loc_000E02B8: ;
    RECOMP_PC(0x000E02B8u);
    fp_push(MEMF(esp + 0x2C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    fp_push(MEMF(esi + 0x4C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0xC)); /* fmul dword ptr [esi + 0xc] */
    g_fp_cmp = RECOMP_FCMP(fp_top(), fp_st1()); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); fp_pop(); /* fcompp  */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_000E02D8; /* jne: not equal / not zero */

loc_000E02CD: ;
    RECOMP_PC(0x000E02CDu);
    fp_pop(); /* fstp st(0) */
    MEM32(esi + 0xC) = ebx;
    fp_push(MEMF(0x1C91E0)); /* fld float */

loc_000E02D8: ;
    RECOMP_PC(0x000E02D8u);
    fp_push(MEMF(esi + 0xC)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_000E0308; /* jne: not equal / not zero */

loc_000E02E8: ;
    RECOMP_PC(0x000E02E8u);
    fp_push(MEMF(esp + 0x2C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    fp_push(MEMF(esi + 0x4C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0xC)); /* fmul dword ptr [esi + 0xc] */
    g_fp_cmp = RECOMP_FCMP(fp_top(), fp_st1()); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); fp_pop(); /* fcompp  */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000E0308; /* jp: parity */

loc_000E02FD: ;
    RECOMP_PC(0x000E02FDu);
    fp_pop(); /* fstp st(0) */
    MEM32(esi + 0xC) = ebx;
    fp_push(MEMF(0x1C91E0)); /* fld float */

loc_000E0308: ;
    RECOMP_PC(0x000E0308u);
    fp_top() = RECOMP_FP_PC(fp_top() + fp_st1()); /* fadd st(1) */
    fp_top() = RECOMP_FP_PC(MEMF(esi + 0x100) - fp_top()); /* fsubr dword ptr [esi + 0x100] */
    MEMF(esi + 0x100) = (float)fp_top(); fp_pop(); /* fstp */
    fp_pop(); /* fstp st(0) */

loc_000E0318: ;
    RECOMP_PC(0x000E0318u);
    fp_push(MEMF(esi + 0x100)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esi + 0x4C)); /* fdiv dword ptr [esi + 0x4c] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x2C)); /* fmul dword ptr [esp + 0x2c] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0xC)); /* fadd dword ptr [esi + 0xc] */
    MEMF(esi + 0xC) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esi + 0x68)); /* fld float */
    fp_top() = -fp_top(); /* fchs */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esi + 0xC)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [esi + 0xc] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_000E033F; /* jne: not equal / not zero */

loc_000E033A: ;
    RECOMP_PC(0x000E033Au);
    MEMF(esi + 0xC) = (float)fp_top(); fp_pop(); /* fstp */
    goto loc_000E0354;

loc_000E033F: ;
    RECOMP_PC(0x000E033Fu);
    fp_pop(); /* fstp st(0) */
    fp_push(MEMF(esi + 0xC)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esi + 0x68)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esi + 0x68] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_000E0354; /* jne: not equal / not zero */

loc_000E034E: ;
    RECOMP_PC(0x000E034Eu);
    ecx = MEM32(esi + 0x68);
    MEM32(esi + 0xC) = ecx;

loc_000E0354: ;
    RECOMP_PC(0x000E0354u);
    fp_push(MEMF(esp + 0x2C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0xC)); /* fmul dword ptr [esi + 0xc] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x104)); /* fadd dword ptr [esi + 0x104] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 8)); /* fadd dword ptr [esi + 8] */
    MEMF(esi + 8) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esi + 8)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esi + 0x60)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esi + 0x60] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000E037C; /* jp: parity */

loc_000E0374: ;
    RECOMP_PC(0x000E0374u);
    edx = MEM32(esi + 0x60);
    MEM32(esi + 8) = edx;
    goto loc_000E038F;

loc_000E037C: ;
    RECOMP_PC(0x000E037Cu);
    fp_push(MEMF(esi + 8)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esi + 0x64)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esi + 0x64] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_000E038F; /* jne: not equal / not zero */

loc_000E0389: ;
    RECOMP_PC(0x000E0389u);
    eax = MEM32(esi + 0x64);
    MEM32(esi + 8) = eax;

loc_000E038F: ;
    RECOMP_PC(0x000E038Fu);
    _fa = (uint32_t)(MEM32(esi + 0x28)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(esi + 0x28), ebx (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000E03AB; /* je: equal / zero */

loc_000E0394: ;
    RECOMP_PC(0x000E0394u);
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 0x2C)); /* fsub dword ptr [esi + 0x2c] */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esi + 0x50)); /* fdiv dword ptr [esi + 0x50] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ebp + 0x63C)); /* fmul dword ptr [ebp + 0x63c] */
    goto loc_000E048E;

loc_000E03AB: ;
    RECOMP_PC(0x000E03ABu);
    _fa = (uint32_t)(MEM32(esi + 0x70)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(esi + 0x70), ebx (32-bit) */
    PUSH32(esp, edi);
    if (CMP_EQ(_fa, _fb)) goto loc_000E046A; /* je: equal / zero */

loc_000E03B5: ;
    RECOMP_PC(0x000E03B5u);
    edi = MEM32(esi + 4);
    ecx = MEM32(edi + 0x25C);
    edx = MEM32(edi + 0x248);
    ecx = ecx << 4;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    fp_push(MEMF(ecx + edx)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_000E046A; /* jnp: not parity */

loc_000E03DB: ;
    RECOMP_PC(0x000E03DBu);
    _fa = (uint32_t)(MEM32(edi + 0x270)) & 0xFFFFFFFFu; _fb = (uint32_t)(2) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(edi + 0x270), 2 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000E046A; /* je: equal / zero */

loc_000E03E8: ;
    RECOMP_PC(0x000E03E8u);
    ebx = MEM32(esi + 0xF8);
    ecx = ebx;
    PUSH32(esp, 0x000E03F5u); RECOMP_ABI_CALL(0x0006DA60u, sub_0006DA60); /* call 0x0006DA60 */

loc_000E03F5: ;
    RECOMP_PC(0x000E03F5u);
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_000E045F; /* jne: not equal / not zero */

loc_000E0402: ;
    RECOMP_PC(0x000E0402u);
    ecx = ebx;
    PUSH32(esp, 0x000E0409u); RECOMP_ABI_CALL(0x0006DA60u, sub_0006DA60); /* call 0x0006DA60 */

loc_000E0409: ;
    RECOMP_PC(0x000E0409u);
    fp_top() = RECOMP_FP_PC(MEMF(0x1C91D0) - fp_top()); /* fsubr dword ptr [0x1c91d0] */
    fp_push(MEMF(ebp + 0x63C)); /* fld float */
    ecx = ebx;
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esi + 0x50)); /* fdiv dword ptr [esi + 0x50] */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    MEMF(esp + 0x10) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x000E0425u); RECOMP_ABI_CALL(0x0006DA60u, sub_0006DA60); /* call 0x0006DA60 */

loc_000E0425: ;
    RECOMP_PC(0x000E0425u);
    fp_push(MEMF(edi + 0x22C)); /* fld float */
    eax = MEM32(edi + 0x25C);
    ecx = MEM32(edi + 0x248);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1BB240)); /* fmul dword ptr [0x1bb240] */
    eax = eax << 4;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(edi + 0x264)); /* fmul dword ptr [edi + 0x264] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x10C)); /* fmul dword ptr [esi + 0x10c] */
    fp_push(MEMF(eax + ecx)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(edi + 0x250)); /* fmul dword ptr [edi + 0x250] */
    fp_st1() = RECOMP_FP_PC(fp_st1() / fp_top()); fp_pop(); /* fdivp st(1) */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esp + 0x10)); /* fadd dword ptr [esp + 0x10] */
    goto loc_000E048D;

loc_000E045F: ;
    RECOMP_PC(0x000E045Fu);
    fp_push(MEMF(ebp + 0x63C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esi + 0x50)); /* fdiv dword ptr [esi + 0x50] */
    goto loc_000E048D;

loc_000E046A: ;
    RECOMP_PC(0x000E046Au);
    _fa = (uint32_t)(MEM32(esi + 0xBC)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(esi + 0xBC), ebx (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000E045F; /* jne: not equal / not zero */

loc_000E0472: ;
    RECOMP_PC(0x000E0472u);
    edx = MEM32(esi + 0xF8);
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(edx + 0x28C)); /* fsub dword ptr [edx + 0x28c] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x14)); /* fmul dword ptr [esi + 0x14] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C91F0)); /* fmul dword ptr [0x1c91f0] */

loc_000E048D: ;
    RECOMP_PC(0x000E048Du);
    POP32(esp, edi);

loc_000E048E: ;
    RECOMP_PC(0x000E048Eu);
    MEMF(esi + 0x14) = (float)fp_top(); /* fst */
    eax = esi + 0x10;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x2C)); /* fmul dword ptr [esp + 0x2c] */
    ecx = esp + 0x2C;
    PUSH32(esp, ecx);
    edx = esp + 0x10;
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax)); /* fadd dword ptr [eax] */
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    MEM32(esp + 0x38) = 0x40C90FDB;
    MEMF(eax) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esp + 0x18) = 0;
    PUSH32(esp, 0x000E04BCu); RECOMP_ABI_CALL(0x0003FAA0u, sub_0003FAA0); /* call 0x0003FAA0 */

loc_000E04BC: ;
    RECOMP_PC(0x000E04BCu);
    fp_push(MEMF(esi + 0x8C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0xA4)); /* fadd dword ptr [esi + 0xa4] */
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    fp_push(MEMF(esi + 0x90)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0xA8)); /* fadd dword ptr [esi + 0xa8] */
    fp_push(MEMF(esi + 0x94)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0xAC)); /* fadd dword ptr [esi + 0xac] */
    MEMF(esp + 0x24) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esp + 0x24);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(esi + 0x98) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0xA0) = eax;
    MEMF(esi + 0x9C) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esi + 4);
    fp_push(MEMF(esi + 8)); /* fld float */
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + 8;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x80)); /* fmul dword ptr [esi + 0x80] */
    MEMF(esp + 0x10) = (float)fp_top(); fp_pop(); /* fstp */
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x84)); /* fmul dword ptr [esi + 0x84] */
    MEMF(esp + 0x14) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x88)); /* fmul dword ptr [esi + 0x88] */
    fp_push(MEMF(esi + 0x74)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax)); /* fadd dword ptr [eax] */
    fp_push(MEMF(eax + 4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x78)); /* fadd dword ptr [esi + 0x78] */
    MEMF(esp + 0x20) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(eax + 8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x7C)); /* fadd dword ptr [esi + 0x7c] */
    MEMF(esp + 0x24) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esp + 0x10)); /* fadd dword ptr [esp + 0x10] */
    fp_push(MEMF(esp + 0x20)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esp + 0x14)); /* fadd dword ptr [esp + 0x14] */
    fp_push(MEMF(esp + 0x24)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + g_fp_stack[(g_fp_top + 3) & 7]); /* fadd st(3) */
    MEMF(esp + 0x24) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = MEM32(esp + 0x24);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(esi + 0x8C) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0x94) = ecx;
    MEMF(esi + 0x90) = (float)fp_top(); fp_pop(); /* fstp */
    fp_pop(); /* fstp st(0) */

loc_000E056F: ;
    RECOMP_PC(0x000E056Fu);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
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
 * qpr2_recovered_000DA770
 * Original: 0x000DA770 - 0x000DA9D1 (609 bytes, 113 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000DA770(void)
{
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)

loc_000DA770: ;
    RECOMP_PC(0x000DA770u);
    fp_push(MEMF(ecx + 0x370)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x36C)); /* fsub dword ptr [ecx + 0x36c] */
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x650)); /* fsub dword ptr [ecx + 0x650] */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ecx + 0x36C)); /* fadd dword ptr [ecx + 0x36c] */
    MEMF(ecx + 0x368) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ecx + 0x37C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x378)); /* fsub dword ptr [ecx + 0x378] */
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x650)); /* fsub dword ptr [ecx + 0x650] */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ecx + 0x378)); /* fadd dword ptr [ecx + 0x378] */
    MEMF(ecx + 0x374) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ecx + 0x4AC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x4A8)); /* fsub dword ptr [ecx + 0x4a8] */
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x650)); /* fsub dword ptr [ecx + 0x650] */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ecx + 0x4A8)); /* fadd dword ptr [ecx + 0x4a8] */
    MEMF(ecx + 0x4A4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ecx + 0x4B8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x4B4)); /* fsub dword ptr [ecx + 0x4b4] */
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x650)); /* fsub dword ptr [ecx + 0x650] */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ecx + 0x4B4)); /* fadd dword ptr [ecx + 0x4b4] */
    MEMF(ecx + 0x4B0) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ecx + 0x4C4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x4C0)); /* fsub dword ptr [ecx + 0x4c0] */
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x650)); /* fsub dword ptr [ecx + 0x650] */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ecx + 0x4C0)); /* fadd dword ptr [ecx + 0x4c0] */
    MEMF(ecx + 0x4BC) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ecx + 0x4D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x4CC)); /* fsub dword ptr [ecx + 0x4cc] */
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x650)); /* fsub dword ptr [ecx + 0x650] */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ecx + 0x4CC)); /* fadd dword ptr [ecx + 0x4cc] */
    MEMF(ecx + 0x4C8) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ecx + 0x4DC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x4D8)); /* fsub dword ptr [ecx + 0x4d8] */
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x650)); /* fsub dword ptr [ecx + 0x650] */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ecx + 0x4D8)); /* fadd dword ptr [ecx + 0x4d8] */
    MEMF(ecx + 0x4D4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ecx + 0x4E8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x4E4)); /* fsub dword ptr [ecx + 0x4e4] */
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x650)); /* fsub dword ptr [ecx + 0x650] */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ecx + 0x4E4)); /* fadd dword ptr [ecx + 0x4e4] */
    MEMF(ecx + 0x4E0) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ecx + 0x4A0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x49C)); /* fsub dword ptr [ecx + 0x49c] */
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x650)); /* fsub dword ptr [ecx + 0x650] */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ecx + 0x49C)); /* fadd dword ptr [ecx + 0x49c] */
    MEMF(ecx + 0x498) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ecx + 0x41C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x418)); /* fsub dword ptr [ecx + 0x418] */
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x650)); /* fsub dword ptr [ecx + 0x650] */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ecx + 0x418)); /* fadd dword ptr [ecx + 0x418] */
    MEMF(ecx + 0x414) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ecx + 0x444)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x440)); /* fsub dword ptr [ecx + 0x440] */
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x650)); /* fsub dword ptr [ecx + 0x650] */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ecx + 0x440)); /* fadd dword ptr [ecx + 0x440] */
    MEMF(ecx + 0x43C) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ecx + 0x450)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x44C)); /* fsub dword ptr [ecx + 0x44c] */
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x650)); /* fsub dword ptr [ecx + 0x650] */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ecx + 0x44C)); /* fadd dword ptr [ecx + 0x44c] */
    MEMF(ecx + 0x448) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ecx + 0x45C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x458)); /* fsub dword ptr [ecx + 0x458] */
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x650)); /* fsub dword ptr [ecx + 0x650] */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ecx + 0x458)); /* fadd dword ptr [ecx + 0x458] */
    MEMF(ecx + 0x454) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ecx + 0x468)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x464)); /* fsub dword ptr [ecx + 0x464] */
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x650)); /* fsub dword ptr [ecx + 0x650] */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ecx + 0x464)); /* fadd dword ptr [ecx + 0x464] */
    MEMF(ecx + 0x460) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ecx + 0x3CC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x3C8)); /* fsub dword ptr [ecx + 0x3c8] */
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x650)); /* fsub dword ptr [ecx + 0x650] */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ecx + 0x3C8)); /* fadd dword ptr [ecx + 0x3c8] */
    MEMF(ecx + 0x3C4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ecx + 0x3F4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x3F0)); /* fsub dword ptr [ecx + 0x3f0] */
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 0x650)); /* fsub dword ptr [ecx + 0x650] */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ecx + 0x3F0)); /* fadd dword ptr [ecx + 0x3f0] */
    MEMF(ecx + 0x3EC) = (float)fp_top(); fp_pop(); /* fstp */
    esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_00085680
 * Original: 0x00085680 - 0x0008581D (413 bytes, 131 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00085680(void)
{
    Qpr2GameTiming _game_time = qpr2_game_timing_begin(0x00085680u, ecx, MEM32(esp + 4), 0);
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

loc_00085680: ;
    RECOMP_PC(0x00085680u);
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    esi = ecx;
    PUSH32(esp, 0x0008568Cu); RECOMP_ABI_CALL(0x00013760u, sub_00013760); /* call 0x00013760 */

loc_0008568C: ;
    RECOMP_PC(0x0008568Cu);
    ecx = esi;
    PUSH32(esp, 0x00085693u); RECOMP_ABI_CALL(0x00013760u, sub_00013760); /* call 0x00013760 */

loc_00085693: ;
    RECOMP_PC(0x00085693u);
    eax = MEM32(eax + 8);
    edi = MEM32(eax + 0x18);
    ecx = esi;
    PUSH32(esp, 0x000856A0u); RECOMP_ABI_CALL(0x00013760u, sub_00013760); /* call 0x00013760 */

loc_000856A0: ;
    RECOMP_PC(0x000856A0u);
    fp_push(MEMF(eax + 0x10)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(edi + 8)); /* fsub dword ptr [edi + 8] */
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + 0x10;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(eax + 4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(edi + 0xC)); /* fsub dword ptr [edi + 0xc] */
    fp_push(MEMF(eax + 8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(edi + 0x10)); /* fsub dword ptr [edi + 0x10] */
    MEMF(esp + 0x20) = (float)fp_top(); /* fst */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x20)); /* fmul dword ptr [esp + 0x20] */
    MEMF(esp + 0xC) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x18)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x18)); /* fmul dword ptr [esp + 0x18] */
    MEMF(esp + 8) = (float)fp_top(); fp_pop(); /* fstp */
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esp + 8)); /* fadd dword ptr [esp + 8] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esp + 0xC)); /* fadd dword ptr [esp + 0xc] */
    MEMF(esp + 0x10) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = MEM32(esp + 0x10);
    PUSH32(esp, ecx);
    fp_pop(); /* fstp st(0) */
    PUSH32(esp, 0x000856EDu); RECOMP_ABI_CALL(0x000CDE60u, sub_000CDE60); /* call 0x000CDE60 */

loc_000856ED: ;
    RECOMP_PC(0x000856EDu);
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esi + 0x10)); /* fld float */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esi + 0xC)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esi + 0xc] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_00085711; /* jne: not equal / not zero */

loc_00085701: ;
    RECOMP_PC(0x00085701u);
    fp_push(MEMF(esi + 0x10)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 0xC)); /* fsub dword ptr [esi + 0xc] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x34)); /* fmul dword ptr [esp + 0x34] */
    fp_top() = RECOMP_FP_PC(MEMF(esi + 0x10) - fp_top()); /* fsubr dword ptr [esi + 0x10] */
    MEMF(esi + 0x10) = (float)fp_top(); fp_pop(); /* fstp */

loc_00085711: ;
    RECOMP_PC(0x00085711u);
    edx = MEM32(0x1D9FB8);
    eax = MEM32(edx + 0x54);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_LE(_fas, _fbs)) goto loc_0008576F; /* jle: less or equal (signed <=) */

loc_00085720: ;
    RECOMP_PC(0x00085720u);
    ecx = esi;
    PUSH32(esp, 0x00085727u); RECOMP_ABI_CALL(0x00013760u, sub_00013760); /* call 0x00013760 */

loc_00085727: ;
    RECOMP_PC(0x00085727u);
    eax = MEM32(eax + 8);
    ecx = MEM32(eax + 0x478);
    edx = MEM32(ecx + 0x3C);
    eax = MEM32(edx + 0x1008);
    ecx = MEM32(eax + 0x74);
    _fa = (uint32_t)(MEM32(ecx + edi * 4)) & 0xFFFFFFFFu; _fb = (uint32_t)(1) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(ecx + edi * 4), 1 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_00085762; /* jne: not equal / not zero */

loc_00085742: ;
    RECOMP_PC(0x00085742u);
    fp_push(MEMF(esi + 0xC)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1AF318)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1af318] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0008575C; /* jp: parity */

loc_00085752: ;
    RECOMP_PC(0x00085752u);
    fp_push(MEMF(esi + 0xC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + fp_top()); /* fadd st(0), st(0) */
    MEMF(esi + 0x10) = (float)fp_top(); fp_pop(); /* fstp */
    goto loc_00085762;

loc_0008575C: ;
    RECOMP_PC(0x0008575Cu);
    edx = MEM32(esi + 0xC);
    MEM32(esi + 0x10) = edx;

loc_00085762: ;
    RECOMP_PC(0x00085762u);
    eax = MEM32(0x1D9FB8);
    ecx = MEM32(eax + 0x54);
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, ecx (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_00085720; /* jl: less (signed <) */

loc_0008576F: ;
    RECOMP_PC(0x0008576Fu);
    fp_push(MEMF(esi + 0x10)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1BC594)); /* fsub dword ptr [0x1bc594] */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esp + 0x14)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esp + 0x14] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_00085815; /* jne: not equal / not zero */

loc_00085787: ;
    RECOMP_PC(0x00085787u);
    fp_push(MEMF(esp + 8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esp + 0xC)); /* fadd dword ptr [esp + 0xc] */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1BC594)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [0x1bc594] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_00085813; /* jnp: not parity */

loc_0008579C: ;
    RECOMP_PC(0x0008579Cu);
    fp_push(MEMF(esi + 0x10)); /* fld float */
    PUSH32(esp, ecx);
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(MEMF(esp + 0x14) - fp_top()); /* fsubr dword ptr [esp + 0x14] */
    fp_top() = RECOMP_FP_PC(fp_top() / fp_st1()); /* fdiv st(1) */
    fp_top() = RECOMP_FP_PC(MEMF(0x1C91D0) - fp_top()); /* fsubr dword ptr [0x1c91d0] */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    fp_pop(); /* fstp st(0) */
    PUSH32(esp, 0x000857BAu); RECOMP_ABI_CALL(0x000CDE60u, sub_000CDE60); /* call 0x000CDE60 */

loc_000857BA: ;
    RECOMP_PC(0x000857BAu);
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1C91D0)); /* fsub dword ptr [0x1c91d0] */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    fp_push(MEMF(esp + 0x18)); /* fld float */
    ecx = esi;
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    MEMF(esp + 0x24) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x1C91E0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    MEMF(esp + 0x28) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x20)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    MEMF(esp + 0x2C) = (float)fp_top(); fp_pop(); /* fstp */
    fp_pop(); /* fstp st(0) */
    PUSH32(esp, 0x000857ECu); RECOMP_ABI_CALL(0x00013760u, sub_00013760); /* call 0x00013760 */

loc_000857EC: ;
    RECOMP_PC(0x000857ECu);
    fp_push(MEMF(esp + 0x24)); /* fld float */
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + 0x10;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax)); /* fadd dword ptr [eax] */
    POP32(esp, edi);
    POP32(esp, esi);
    MEMF(eax) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x20)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 4)); /* fadd dword ptr [eax + 4] */
    MEMF(eax + 4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x24)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 8)); /* fadd dword ptr [eax + 8] */
    MEMF(eax + 8) = (float)fp_top(); fp_pop(); /* fstp */
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; qpr2_game_timing_end(&_game_time); return; /* ret 4 */

loc_00085813: ;
    RECOMP_PC(0x00085813u);
    fp_pop(); /* fstp st(0) */

loc_00085815: ;
    RECOMP_PC(0x00085815u);
    POP32(esp, edi);
    POP32(esp, esi);
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; qpr2_game_timing_end(&_game_time); return; /* ret 4 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_000C3310
 * Original: 0x000C3310 - 0x000C35C3 (691 bytes, 199 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000C3310(void)
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

loc_000C3310: ;
    RECOMP_PC(0x000C3310u);
    _fb = (uint32_t)(0x44) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x44;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    esi = ecx;
    fp_push(MEMF(esi + 0x70)); /* fld float */
    PUSH32(esp, edi);
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C1488)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c1488] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_000C35B9; /* je: equal / zero */

loc_000C332D: ;
    RECOMP_PC(0x000C332Du);
    fp_push(MEMF(esi + 0x70)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000C3351; /* jp: parity */

loc_000C333D: ;
    RECOMP_PC(0x000C333Du);
    fp_push(MEMF(esi + 0x74)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C1484)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c1484] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_000C35B9; /* je: equal / zero */

loc_000C3351: ;
    RECOMP_PC(0x000C3351u);
    eax = MEM32(esi + 0x2C);
    ecx = MEM32(esi + 0x24);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    edx = esi + 0xC;
    PUSH32(esp, edx);
    ecx = esp + 0x48;
    PUSH32(esp, 0x000C3366u); RECOMP_ABI_CALL(0x00037EF0u, sub_00037EF0); /* call 0x00037EF0 */

loc_000C3366: ;
    RECOMP_PC(0x000C3366u);
    ecx = MEM32(esp + 0x58);
    eax = esp + 0x3C;
    PUSH32(esp, eax);
    PUSH32(esp, 0x000C3374u); RECOMP_ABI_CALL(0x00037C80u, sub_00037C80); /* call 0x00037C80 */

loc_000C3374: ;
    RECOMP_PC(0x000C3374u);
    edx = MEM32(esi);
    eax = esp + 0x3C;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x60); PUSH32(esp, 0x000C3380u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000C337Du); } /* indirect call */
    }

loc_000C3380: ;
    RECOMP_PC(0x000C3380u);
    PUSH32(esp, 0x000C3385u); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000C3385: ;
    RECOMP_PC(0x000C3385u);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    PUSH32(esp, edi);
    PUSH32(esp, 0x000C338Du); RECOMP_ABI_CALL(0x0001D270u, sub_0001D270); /* call 0x0001D270 */

loc_000C338D: ;
    RECOMP_PC(0x000C338Du);
    eax = MEM32(0x1DE3E8);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edi (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C33A5; /* je: equal / zero */

loc_000C3399: ;
    RECOMP_PC(0x000C3399u);
    PUSH32(esp, edi);
    MEM32(0x1DE3E8) = edi;
    PUSH32(esp, 0x000C33A5u); RECOMP_ABI_CALL(0x001931B0u, sub_001931B0); /* call 0x001931B0 */

loc_000C33A5: ;
    RECOMP_PC(0x000C33A5u);
    _fa = (uint32_t)(MEM32(0x1DC3CC)) & 0xFFFFFFFFu; _fb = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC3CC), edi (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C33C5; /* je: equal / zero */

loc_000C33AD: ;
    RECOMP_PC(0x000C33ADu);
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = edi;
    PUSH32(esp, 0x000C33BFu); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000C33BF: ;
    RECOMP_PC(0x000C33BFu);
    MEM32(0x1A51F8) = edi;

loc_000C33C5: ;
    RECOMP_PC(0x000C33C5u);
    ecx = MEM32(esi + 0x80);
    PUSH32(esp, ecx);
    PUSH32(esp, edi);
    PUSH32(esp, 0x000C33D2u); RECOMP_ABI_CALL(0x000E6A00u, sub_000E6A00); /* call 0x000E6A00 */

loc_000C33D2: ;
    RECOMP_PC(0x000C33D2u);
    fp_push(MEMF(esi + 0x70)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x1C91D0)); /* fadd dword ptr [0x1c91d0] */
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x000C33E3u); RECOMP_ABI_CALL(0x0012B278u, sub_0012B278); /* call 0x0012B278 */

loc_000C33E3: ;
    RECOMP_PC(0x000C33E3u);
    fp_push(MEMF(esi + 0x70)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    ebx = eax;
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000C33F7; /* jp: parity */

loc_000C33F5: ;
    RECOMP_PC(0x000C33F5u);
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_000C33F7: ;
    RECOMP_PC(0x000C33F7u);
    edx = MEM32(esi + ebx * 4 + 0x84);
    fp_push(MEMF(0x1AF32C)); /* fld float */
    eax = MEM32(edx + 8);
    ecx = MEM32(eax + 4);
    edx = MEM32(eax + 0x10);
    ebp = MEM32(eax);
    eax = MEM32(eax + 0x14);
    MEM32(esp + 0x28) = ecx;
    ecx = ebx;
    ecx = ecx << 4;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + esi + 0x9C)); /* fsub dword ptr [ecx + esi + 0x9c] */
    edi = ecx + esi;
    MEM32(esp + 0x20) = eax;
    MEM32(esp + 0x1C) = edx;
    MEMF(esp + 0x14) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esi + 0x70)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000C3453; /* jp: parity */

loc_000C3441: ;
    RECOMP_PC(0x000C3441u);
    edx = MEM32(esi + 0x74);
    eax = MEM32(0x1C1484);
    MEM32(esp + 0x10) = edx;
    MEM32(esp + 0x18) = eax;
    goto loc_000C3478;

loc_000C3453: ;
    RECOMP_PC(0x000C3453u);
    fp_push(MEMF(esi + 0x70)); /* fld float */
    PUSH32(esp, 0x000C345Bu); RECOMP_ABI_CALL(0x0012B278u, sub_0012B278); /* call 0x0012B278 */

loc_000C345B: ;
    RECOMP_PC(0x000C345Bu);
    MEM32(esp + 0x18) = eax;
    fp_push((double)SMEM32(esp + 0x18)); /* fild */
    MEM32(esp + 0x18) = 0x3F800000;
    fp_top() = RECOMP_FP_PC(MEMF(esi + 0x70) - fp_top()); /* fsubr dword ptr [esi + 0x70] */
    fp_top() = RECOMP_FP_PC(MEMF(0x1C91D0) - fp_top()); /* fsubr dword ptr [0x1c91d0] */
    MEMF(esp + 0x10) = (float)fp_top(); fp_pop(); /* fstp */

loc_000C3478: ;
    RECOMP_PC(0x000C3478u);
    fp_push(MEMF(edi + 0x94)); /* fld float */
    fp_push(MEMF(esp + 0x10)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1AEB20)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1aeb20] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000C34A5; /* jp: parity */

loc_000C348F: ;
    RECOMP_PC(0x000C348Fu);
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x14)); /* fsub dword ptr [esp + 0x14] */
    fp_push(MEMF(esp + 0x10)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1AE140)); /* fmul dword ptr [0x1ae140] */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esp + 0x14)); /* fadd dword ptr [esp + 0x14] */
    goto loc_000C34D6;

loc_000C34A5: ;
    RECOMP_PC(0x000C34A5u);
    fp_push(MEMF(esp + 0x18)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1AEB20)); /* fsub dword ptr [0x1aeb20] */
    fp_push(MEMF(esp + 0x10)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), fp_st1()); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp st(1) */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 1 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_000C34D4; /* jne: not equal / not zero */

loc_000C34BC: ;
    RECOMP_PC(0x000C34BCu);
    fp_push(MEMF(esp + 0x10)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - fp_st1()); /* fsub st(1) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1AE140)); /* fmul dword ptr [0x1ae140] */
    fp_push(MEMF(0x1C91D0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - g_fp_stack[(g_fp_top + 3) & 7]); /* fsub st(3) */
    fp_st1() = RECOMP_FP_PC(fp_st1() * fp_top()); fp_pop(); /* fmulp st(1) */
    g_fp_stack[(g_fp_top + 2) & 7] = RECOMP_FP_PC(g_fp_stack[(g_fp_top + 2) & 7] + fp_top()); fp_pop(); /* faddp st(2) */

loc_000C34D4: ;
    RECOMP_PC(0x000C34D4u);
    fp_pop(); /* fstp st(0) */

loc_000C34D6: ;
    RECOMP_PC(0x000C34D6u);
    ecx = MEM32(edi + 0x9C);
    MEMF(esp + 0x2C) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(edi + 0x98)); /* fld float */
    PUSH32(esp, 0x3F800000);
    _fb = (uint32_t)(0xA) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebx = ebx + 0xA;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x3F800000);
    MEMF(esp + 0x38) = (float)fp_top(); fp_pop(); /* fstp */
    ebx = ebx << 4;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    fp_push(MEMF(ebx + esi)); /* fld float */
    PUSH32(esp, 0x3F800000);
    MEM32(esp + 0x40) = ecx;
    MEMF(esp + 0x44) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x3F800000);
    ecx = esp + 0x28;
    PUSH32(esp, 0x000C3518u); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_000C3518: ;
    RECOMP_PC(0x000C3518u);
    edx = MEM32(esp + 0x28);
    eax = MEM32(esp + 0x1C);
    ecx = MEM32(esp + 0x20);
    PUSH32(esp, edx);
    edx = MEM32(esp + 0x1C);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = MEM32(esp + 0x38);
    PUSH32(esp, ebp);
    PUSH32(esp, edx);
    edx = MEM32(esp + 0x44);
    PUSH32(esp, 0x3F800000);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    eax = esp;
    MEM32(eax) = ecx;
    ecx = MEM32(esp + 0x5C);
    MEM32(eax + 4) = edx;
    edx = MEM32(esp + 0x60);
    MEM32(eax + 8) = ecx;
    ecx = MEM32(esp + 0x80);
    MEM32(eax + 0xC) = edx;
    PUSH32(esp, 0x000C355Eu); RECOMP_ABI_CALL(0x00112200u, sub_00112200); /* call 0x00112200 */

loc_000C355E: ;
    RECOMP_PC(0x000C355Eu);
    ecx = MEM32(0x1DE3E8);
    eax = 0x901;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, eax (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C3578; /* je: equal / zero */

loc_000C356D: ;
    RECOMP_PC(0x000C356Du);
    PUSH32(esp, eax);
    MEM32(0x1DE3E8) = eax;
    PUSH32(esp, 0x000C3578u); RECOMP_ABI_CALL(0x001931B0u, sub_001931B0); /* call 0x001931B0 */

loc_000C3578: ;
    RECOMP_PC(0x000C3578u);
    eax = MEM32(0x1DC3CC);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000C359E; /* jne: not equal / not zero */

loc_000C3581: ;
    RECOMP_PC(0x000C3581u);
    edi = 1;
    edx = edi;
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = edi;
    PUSH32(esp, 0x000C3598u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000C3598: ;
    RECOMP_PC(0x000C3598u);
    MEM32(0x1A51F8) = edi;

loc_000C359E: ;
    RECOMP_PC(0x000C359Eu);
    PUSH32(esp, 0);
    PUSH32(esp, 0x000C35A5u); RECOMP_ABI_CALL(0x0001D370u, sub_0001D370); /* call 0x0001D370 */

loc_000C35A5: ;
    RECOMP_PC(0x000C35A5u);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x000C35ADu); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000C35AD: ;
    RECOMP_PC(0x000C35ADu);
    eax = MEM32(esi);
    ecx = esp + 0x3C;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0x64); PUSH32(esp, 0x000C35B9u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000C35B6u); } /* indirect call */
    }

loc_000C35B9: ;
    RECOMP_PC(0x000C35B9u);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
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


/**
 * qpr2_recovered_000C47A0
 * Original: 0x000C47A0 - 0x000C4947 (423 bytes, 124 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000C47A0(void)
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

loc_000C47A0: ;
    RECOMP_PC(0x000C47A0u);
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    esi = ecx;
    SET_LO8(eax, MEM8(esi + 0x184));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    PUSH32(esp, edi);
    if (CMP_EQ(_fa, _fb)) goto loc_000C493E; /* je: equal / zero */

loc_000C47B6: ;
    RECOMP_PC(0x000C47B6u);
    SET_LO8(eax, MEM8(0x1DAD98));
    ebx = 1;
    _fa = (uint32_t)(LO8(ebx)) & 0xFFu; _fb = (uint32_t)(LO8(eax)) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test LO8(ebx), LO8(eax) (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_000C47EB; /* jne: not equal / not zero */

loc_000C47C4: ;
    RECOMP_PC(0x000C47C4u);
    eax = MEM32(0x1DAD98);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0);
    PUSH32(esp, 0x3F800000);
    eax = eax | ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    PUSH32(esp, 0x3F000000);
    ecx = 0x1DAD94;
    MEM32(0x1DAD98) = eax;
    PUSH32(esp, 0x000C47EBu); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_000C47EB: ;
    RECOMP_PC(0x000C47EBu);
    eax = MEM32(esi + 0x2C);
    ecx = MEM32(esi + 0x24);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    edx = esi + 0xC;
    PUSH32(esp, edx);
    ecx = esp + 0x28;
    PUSH32(esp, 0x000C4800u); RECOMP_ABI_CALL(0x00037EF0u, sub_00037EF0); /* call 0x00037EF0 */

loc_000C4800: ;
    RECOMP_PC(0x000C4800u);
    ecx = MEM32(esp + 0x38);
    eax = esp + 0x1C;
    PUSH32(esp, eax);
    PUSH32(esp, 0x000C480Eu); RECOMP_ABI_CALL(0x00037C80u, sub_00037C80); /* call 0x00037C80 */

loc_000C480E: ;
    RECOMP_PC(0x000C480Eu);
    edx = MEM32(esi);
    eax = esp + 0x1C;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x60); PUSH32(esp, 0x000C481Au); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000C4817u); } /* indirect call */
    }

loc_000C481A: ;
    RECOMP_PC(0x000C481Au);
    PUSH32(esp, 0x000C481Fu); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000C481F: ;
    RECOMP_PC(0x000C481Fu);
    PUSH32(esp, 0);
    PUSH32(esp, 0x000C4826u); RECOMP_ABI_CALL(0x0001D270u, sub_0001D270); /* call 0x0001D270 */

loc_000C4826: ;
    RECOMP_PC(0x000C4826u);
    eax = MEM32(0x1DE3E8);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C4843; /* je: equal / zero */

loc_000C4832: ;
    RECOMP_PC(0x000C4832u);
    PUSH32(esp, 0);
    MEM32(0x1DE3E8) = 0;
    PUSH32(esp, 0x000C4843u); RECOMP_ABI_CALL(0x001931B0u, sub_001931B0); /* call 0x001931B0 */

loc_000C4843: ;
    RECOMP_PC(0x000C4843u);
    ecx = MEM32(0x1DE46C);
    edi = esi + 0x80;
    PUSH32(esp, edi);
    MEM32(ecx + 0x64) = 0;
    PUSH32(esp, 0x000C485Cu); RECOMP_ABI_CALL(0x001025F0u, sub_001025F0); /* call 0x001025F0 */

loc_000C485C: ;
    RECOMP_PC(0x000C485Cu);
    fp_push(MEMF(0x1B2480)); /* fld float */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0x19) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0x19 (32-bit) */
    if (CMP_LE(_fas, _fbs)) goto loc_000C4872; /* jle: less or equal (signed <=) */

loc_000C486A: ;
    RECOMP_PC(0x000C486Au);
    fp_pop(); /* fstp st(0) */
    fp_push(MEMF(0x1BF3C4)); /* fld float */

loc_000C4872: ;
    RECOMP_PC(0x000C4872u);
    edx = MEM32(0x1D9FB8);
    ecx = MEM32(edx + 4);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(2) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, 2 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000C488A; /* jne: not equal / not zero */

loc_000C4880: ;
    RECOMP_PC(0x000C4880u);
    _fa = (uint32_t)(MEM32(0x1D74E4)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1D74E4), ebx (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000C488C; /* jne: not equal / not zero */

loc_000C4888: ;
    RECOMP_PC(0x000C4888u);
    goto loc_000C4892;

loc_000C488A: ;
    RECOMP_PC(0x000C488Au);
    if (CMP_BE(_fa, _fb)) goto loc_000C4892; /* jbe: below or equal (unsigned <=) */

loc_000C488C: ;
    RECOMP_PC(0x000C488Cu);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1AF328)); /* fmul dword ptr [0x1af328] */

loc_000C4892: ;
    RECOMP_PC(0x000C4892u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebx (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000C48A1; /* jne: not equal / not zero */

loc_000C4896: ;
    RECOMP_PC(0x000C4896u);
    _fa = (uint32_t)(MEM8(edi)) & 0xFFu; _fb = (uint32_t)(0x58) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp MEM8(edi), 0x58 (8-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000C48A1; /* jne: not equal / not zero */

loc_000C489B: ;
    RECOMP_PC(0x000C489Bu);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C91DC)); /* fmul dword ptr [0x1c91dc] */

loc_000C48A1: ;
    RECOMP_PC(0x000C48A1u);
    eax = MEM32(esi + 0x7C);
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = MEM32(esi + 0x78);
    edx = MEM32(esi + 0x74);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    eax = esp + 0x44;
    PUSH32(esp, eax);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    ecx = esp;
    MEM32(esp + 0x2C) = 0xBF666666u;
    edx = MEM32(esp + 0x2C);
    MEM32(ecx) = edx;
    MEM32(esp + 0x30) = 0xBF733333u;
    eax = MEM32(esp + 0x30);
    MEM32(ecx + 4) = eax;
    eax = MEM32(esp + 0x38);
    MEM32(esp + 0x34) = 0x3FE66666;
    edx = MEM32(esp + 0x34);
    MEM32(ecx + 8) = edx;
    MEM32(ecx + 0xC) = eax;
    ecx = esp + 0x3C;
    PUSH32(esp, edi);
    PUSH32(esp, ecx);
    MEM32(esp + 0x60) = ebx;
    PUSH32(esp, 0x000C48FDu); RECOMP_ABI_CALL(0x000C6F90u, sub_000C6F90); /* call 0x000C6F90 */

loc_000C48FD: ;
    RECOMP_PC(0x000C48FDu);
    edx = MEM32(0x1DE46C);
    MEM32(edx + 0x64) = ebx;
    ecx = MEM32(0x1DE3E8);
    eax = 0x901;
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, eax (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C4923; /* je: equal / zero */

loc_000C4918: ;
    RECOMP_PC(0x000C4918u);
    PUSH32(esp, eax);
    MEM32(0x1DE3E8) = eax;
    PUSH32(esp, 0x000C4923u); RECOMP_ABI_CALL(0x001931B0u, sub_001931B0); /* call 0x001931B0 */

loc_000C4923: ;
    RECOMP_PC(0x000C4923u);
    PUSH32(esp, 0);
    PUSH32(esp, 0x000C492Au); RECOMP_ABI_CALL(0x0001D370u, sub_0001D370); /* call 0x0001D370 */

loc_000C492A: ;
    RECOMP_PC(0x000C492Au);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x000C4932u); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000C4932: ;
    RECOMP_PC(0x000C4932u);
    eax = MEM32(esi);
    ecx = esp + 0x1C;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0x64); PUSH32(esp, 0x000C493Eu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000C493Bu); } /* indirect call */
    }

loc_000C493E: ;
    RECOMP_PC(0x000C493Eu);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; return; /* ret 4 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_000C3AF0
 * Original: 0x000C3AF0 - 0x000C41B6 (1734 bytes, 490 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000C3AF0(void)
{
    Qpr2GameTiming _game_time = qpr2_game_timing_begin(0x000C3AF0u, ecx, MEM32(esp + 4), 0);
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

loc_000C3AF0: ;
    RECOMP_PC(0x000C3AF0u);
    _fb = (uint32_t)(0x58) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x58;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    ebp = ecx;
    eax = MEM32(ebp + 0x2C);
    ecx = MEM32(ebp + 0x24);
    PUSH32(esp, edi);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    edx = ebp + 0xC;
    PUSH32(esp, edx);
    ecx = esp + 0x5C;
    PUSH32(esp, 0x000C3B0Eu); RECOMP_ABI_CALL(0x00037EF0u, sub_00037EF0); /* call 0x00037EF0 */

loc_000C3B0E: ;
    RECOMP_PC(0x000C3B0Eu);
    ecx = MEM32(esp + 0x6C);
    eax = esp + 0x50;
    PUSH32(esp, eax);
    PUSH32(esp, 0x000C3B1Cu); RECOMP_ABI_CALL(0x00037C80u, sub_00037C80); /* call 0x00037C80 */

loc_000C3B1C: ;
    RECOMP_PC(0x000C3B1Cu);
    edx = MEM32(ebp);
    ebx = ebp + 0x90;
    ecx = 6;
    esi = esp + 0x50;
    edi = ebx;
    eax = esp + 0x50;
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
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    ecx = ebp;
    { uint32_t _icall_target = MEM32(edx + 0x60); PUSH32(esp, 0x000C3B3Cu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000C3B39u); } /* indirect call */
    }

loc_000C3B3C: ;
    RECOMP_PC(0x000C3B3Cu);
    PUSH32(esp, 0x000C3B41u); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000C3B41: ;
    RECOMP_PC(0x000C3B41u);
    PUSH32(esp, 0);
    PUSH32(esp, 0x000C3B48u); RECOMP_ABI_CALL(0x0001D270u, sub_0001D270); /* call 0x0001D270 */

loc_000C3B48: ;
    RECOMP_PC(0x000C3B48u);
    eax = MEM32(0x1DE3E8);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C3B65; /* je: equal / zero */

loc_000C3B54: ;
    RECOMP_PC(0x000C3B54u);
    PUSH32(esp, 0);
    MEM32(0x1DE3E8) = 0;
    PUSH32(esp, 0x000C3B65u); RECOMP_ABI_CALL(0x001931B0u, sub_001931B0); /* call 0x001931B0 */

loc_000C3B65: ;
    RECOMP_PC(0x000C3B65u);
    ecx = MEM32(ebp + 0x70);
    PUSH32(esp, ecx);
    PUSH32(esp, 0);
    PUSH32(esp, 0x000C3B70u); RECOMP_ABI_CALL(0x000E6A00u, sub_000E6A00); /* call 0x000E6A00 */

loc_000C3B70: ;
    RECOMP_PC(0x000C3B70u);
    fp_push(MEMF(ebp + 0x9C)); /* fld float */
    edx = MEM32(ebp + 0x84);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C91CC)); /* fmul dword ptr [0x1c91cc] */
    eax = MEM32(edx + 8);
    esi = MEM32(eax + 0x14);
    edx = MEM32(eax + 0x10);
    MEMF(esp + 0x1C) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = MEM32(eax);
    fp_push(MEMF(ebx + 0xC)); /* fld float */
    edi = MEM32(eax);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C91CC)); /* fmul dword ptr [0x1c91cc] */
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x44) = esi;
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebx + 4)); /* fadd dword ptr [ebx + 4] */
    esi = edx;
    MEM32(esp + 0x40) = ecx;
    ecx = MEM32(eax + 4);
    PUSH32(esp, 0xBF800000u);
    MEM32(esp + 0x44) = esi;
    esi = MEM32(eax + 0x14);
    eax = ecx;
    MEM32(esp + 0x48) = eax;
    eax = MEM32(0x1D8214);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    PUSH32(esp, 0x3DA3D70A);
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1B6300)); /* fsub dword ptr [0x1b6300] */
    PUSH32(esp, 0x3DA3D70A);
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    MEMF(esp + 4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ebx + 8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C91CC)); /* fmul dword ptr [0x1c91cc] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebx)); /* fadd dword ptr [ebx] */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1C160C)); /* fsub dword ptr [0x1c160c] */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x000C3BFBu); RECOMP_ABI_CALL(0x000E5710u, sub_000E5710); /* call 0x000E5710 */

loc_000C3BFB: ;
    RECOMP_PC(0x000C3BFBu);
    edx = MEM32(0x1D9FB8);
    eax = MEM32(edx + 0x10);
    ecx = 0; /* xor self */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    MEM32(esp + 0x1C) = ecx;
    if (CMP_LE(_fas, _fbs)) goto loc_000C3D4C; /* jle: less or equal (signed <=) */

loc_000C3C16: ;
    RECOMP_PC(0x000C3C16u);
    goto loc_000C3C20;

    RECOMP_PC(0x000C3C18u);
    /* nop */
    /* nop */

loc_000C3C20: ;
    RECOMP_PC(0x000C3C20u);
    edx = MEM32(ebp + 0xA8);
    fp_push(MEMF(edx + ecx * 4)); /* fld float */
    edx = edx + ecx * 4;
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    MEM32(esp + 0x18) = edx;
    fp_push(MEMF(edx)); /* fld float */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000C3C41; /* jp: parity */

loc_000C3C3F: ;
    RECOMP_PC(0x000C3C3Fu);
    fp_top() = -fp_top(); /* fchs */

loc_000C3C41: ;
    RECOMP_PC(0x000C3C41u);
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91D0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91d0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000C3D35; /* jp: parity */

loc_000C3C52: ;
    RECOMP_PC(0x000C3C52u);
    fp_push(MEMF(edx)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    fp_push(MEMF(edx)); /* fld float */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000C3C65; /* jp: parity */

loc_000C3C63: ;
    RECOMP_PC(0x000C3C63u);
    fp_top() = -fp_top(); /* fchs */

loc_000C3C65: ;
    RECOMP_PC(0x000C3C65u);
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1ADFEC)); /* fsub dword ptr [0x1adfec] */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000C3C84; /* jp: parity */

loc_000C3C78: ;
    RECOMP_PC(0x000C3C78u);
    fp_pop(); /* fstp st(0) */
    MEM32(esp + 0x6C) = 0x3F800000;
    goto loc_000C3C94;

loc_000C3C84: ;
    RECOMP_PC(0x000C3C84u);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C1608)); /* fmul dword ptr [0x1c1608] */
    fp_top() = RECOMP_FP_PC(MEMF(0x1C91D0) - fp_top()); /* fsubr dword ptr [0x1c91d0] */
    MEMF(esp + 0x6C) = (float)fp_top(); fp_pop(); /* fstp */

loc_000C3C94: ;
    RECOMP_PC(0x000C3C94u);
    eax = MEM32(esp + 0x6C);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, eax);
    ecx = esp + 0x34;
    PUSH32(esp, 0x000C3CB1u); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_000C3CB1: ;
    RECOMP_PC(0x000C3CB1u);
    fp_push(MEMF(ebx + 0xC)); /* fld float */
    ecx = MEM32(ebp + 0x88);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C91CC)); /* fmul dword ptr [0x1c91cc] */
    eax = MEM32(ecx + 8);
    edx = MEM32(eax);
    esi = MEM32(eax + 0x14);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebx + 4)); /* fadd dword ptr [ebx + 4] */
    ecx = MEM32(eax + 4);
    fp_push(MEMF(esp + 0x14)); /* fld float */
    edi = MEM32(eax);
    MEM32(esp + 0x40) = edx;
    edx = MEM32(eax + 0x10);
    MEM32(esp + 0x44) = esi;
    esi = edx;
    PUSH32(esp, 0xBF800000u);
    MEM32(esp + 0x44) = esi;
    esi = MEM32(eax + 0x14);
    eax = ecx;
    MEM32(esp + 0x48) = eax;
    eax = MEM32(esp + 0x28);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = MEM32(esp + 0x24);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ecx)); /* fmul dword ptr [ecx] */
    PUSH32(esp, edx);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    fp_st1() = RECOMP_FP_PC(fp_st1() - fp_top()); fp_pop(); /* fsubp st(1) */
    PUSH32(esp, 0x3DA3D70A);
    PUSH32(esp, 0x3DA3D70A);
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1B6300)); /* fsub dword ptr [0x1b6300] */
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    MEMF(esp + 4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ebx + 8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C91CC)); /* fmul dword ptr [0x1c91cc] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebx)); /* fadd dword ptr [ebx] */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x000C3D2Eu); RECOMP_ABI_CALL(0x000E5710u, sub_000E5710); /* call 0x000E5710 */

loc_000C3D2E: ;
    RECOMP_PC(0x000C3D2Eu);
    ecx = MEM32(esp + 0x44);
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */

loc_000C3D35: ;
    RECOMP_PC(0x000C3D35u);
    edx = MEM32(0x1D9FB8);
    eax = MEM32(edx + 0x10);
    ecx++;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, eax (32-bit) */
    MEM32(esp + 0x1C) = ecx;
    if (CMP_L(_fas, _fbs)) goto loc_000C3C20; /* jl: less (signed <) */

loc_000C3D4C: ;
    RECOMP_PC(0x000C3D4Cu);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    ecx = esp + 0x34;
    PUSH32(esp, 0x000C3D69u); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_000C3D69: ;
    RECOMP_PC(0x000C3D69u);
    fp_push(MEMF(ebx + 8)); /* fld float */
    ecx = MEM32(ebp + 0x78);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C91CC)); /* fmul dword ptr [0x1c91cc] */
    eax = MEM32(ecx + 8);
    edx = MEM32(eax);
    esi = MEM32(eax + 0x14);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebx)); /* fadd dword ptr [ebx] */
    ecx = MEM32(eax + 4);
    edi = MEM32(eax);
    MEM32(esp + 0x40) = edx;
    edx = MEM32(eax + 0x10);
    MEM32(esp + 0x44) = esi;
    esi = edx;
    PUSH32(esp, 0xBF800000u);
    MEM32(esp + 0x44) = esi;
    esi = MEM32(eax + 0x14);
    eax = ecx;
    MEM32(esp + 0x48) = eax;
    eax = MEM32(esp + 0x28);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = MEM32(ebp + 0x9C);
    PUSH32(esp, edx);
    edx = MEM32(ebp + 0x94);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    PUSH32(esp, ecx);
    PUSH32(esp, 0x3CF5C28F);
    PUSH32(esp, edx);
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1BD240)); /* fsub dword ptr [0x1bd240] */
    PUSH32(esp, ecx);
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x000C3DCEu); RECOMP_ABI_CALL(0x000E5710u, sub_000E5710); /* call 0x000E5710 */

loc_000C3DCE: ;
    RECOMP_PC(0x000C3DCEu);
    eax = MEM32(ebp + 0xB8);
    ecx = MEM32(eax + 0x13C);
    edx = MEM32(ebp + 0xB4);
    eax = MEM32(edx + 0x5C);
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, esi (32-bit) */
    MEM32(esp + 0x28) = ecx;
    MEM32(esp + 0x24) = esi;
    if (CMP_LE(_fas, _fbs)) goto loc_000C4176; /* jle: less or equal (signed <=) */

loc_000C3DF8: ;
    RECOMP_PC(0x000C3DF8u);
    MEM32(esp + 0x1C) = esi;
    MEM32(esp + 0x18) = esi;

loc_000C3E00: ;
    RECOMP_PC(0x000C3E00u);
    eax = MEM32(ebp + 0xAC);
    fp_push(MEMF(esp + 0x28)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + eax)); /* fsub dword ptr [esi + eax] */
    edi = esi + eax;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C1550)); /* fmul dword ptr [0x1c1550] */
    MEMF(esp + 0x10) = (float)fp_top(); /* fst */
    fp_top() = fabs(fp_top()); /* fabs */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMD(0x1C1600)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom qword ptr [0x1c1600] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000C4076; /* jp: parity */

loc_000C3E2D: ;
    RECOMP_PC(0x000C3E2Du);
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1ADFEC)); /* fsub dword ptr [0x1adfec] */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000C3E4C; /* jp: parity */

loc_000C3E40: ;
    RECOMP_PC(0x000C3E40u);
    fp_pop(); /* fstp st(0) */
    MEM32(esp + 0x6C) = 0x3F800000;
    goto loc_000C3E5C;

loc_000C3E4C: ;
    RECOMP_PC(0x000C3E4Cu);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C1608)); /* fmul dword ptr [0x1c1608] */
    fp_top() = RECOMP_FP_PC(MEMF(0x1C91D0) - fp_top()); /* fsubr dword ptr [0x1c91d0] */
    MEMF(esp + 0x6C) = (float)fp_top(); fp_pop(); /* fstp */

loc_000C3E5C: ;
    RECOMP_PC(0x000C3E5Cu);
    fp_push(MEMF(esp + 0x6C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1AE830)); /* fmul dword ptr [0x1ae830] */
    PUSH32(esp, 0x000C3E6Bu); RECOMP_ABI_CALL(0x0012B278u, sub_0012B278); /* call 0x0012B278 */

loc_000C3E6B: ;
    RECOMP_PC(0x000C3E6Bu);
    MEM8(edi + 0xB) = LO8(eax);
    ecx = MEM32(ebp + 0xAC);
    eax = MEM32(ecx + esi + 0xC);
    _fb = (uint32_t)(esi) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ecx = ecx + esi;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C3F2D; /* je: equal / zero */

loc_000C3E82: ;
    RECOMP_PC(0x000C3E82u);
    edx = MEM32(esp + 0x6C);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, edx);
    ecx = esp + 0x3C;
    PUSH32(esp, 0x000C3E9Fu); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_000C3E9F: ;
    RECOMP_PC(0x000C3E9Fu);
    fp_push(MEMF(ebx + 0xC)); /* fld float */
    eax = MEM32(ebp + 0x7C);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C91CC)); /* fmul dword ptr [0x1c91cc] */
    eax = MEM32(eax + 8);
    edi = MEM32(eax + 0x14);
    edx = MEM32(eax + 0x10);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebx + 4)); /* fadd dword ptr [ebx + 4] */
    ecx = MEM32(eax);
    fp_push(MEMF(esp + 0x10)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x14)); /* fmul dword ptr [esp + 0x14] */
    MEM32(esp + 0x20) = edi;
    edi = edx;
    MEM32(esp + 0x38) = edi;
    edi = MEM32(eax + 0x14);
    MEM32(esp + 0x1C) = ecx;
    ecx = MEM32(eax + 4);
    PUSH32(esp, 0xBF800000u);
    MEM32(esp + 0x40) = edi;
    edi = MEM32(eax);
    eax = ecx;
    MEM32(esp + 0x24) = eax;
    eax = MEM32(esp + 0x30);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = MEM32(esp + 0x48);
    PUSH32(esp, edx);
    PUSH32(esp, ecx);
    PUSH32(esp, edi);
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    PUSH32(esp, 0x3DCCCCCD);
    PUSH32(esp, 0x3DCCCCCD);
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1AF3A0)); /* fsub dword ptr [0x1af3a0] */
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    MEMF(esp + 4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ebx + 8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C91CC)); /* fmul dword ptr [0x1c91cc] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebx)); /* fadd dword ptr [ebx] */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1AF3A0)); /* fsub dword ptr [0x1af3a0] */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x000C3F25u); RECOMP_ABI_CALL(0x000E5710u, sub_000E5710); /* call 0x000E5710 */

loc_000C3F25: ;
    RECOMP_PC(0x000C3F25u);
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    goto loc_000C4147;

loc_000C3F2D: ;
    RECOMP_PC(0x000C3F2Du);
    eax = MEM32(ecx + 0x10);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C3FE2; /* je: equal / zero */

loc_000C3F38: ;
    RECOMP_PC(0x000C3F38u);
    edx = MEM32(esp + 0x6C);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, edx);
    ecx = esp + 0x40;
    PUSH32(esp, 0x000C3F55u); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_000C3F55: ;
    RECOMP_PC(0x000C3F55u);
    fp_push(MEMF(ebx + 0xC)); /* fld float */
    eax = MEM32(ebp + 0x80);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C91CC)); /* fmul dword ptr [0x1c91cc] */
    eax = MEM32(eax + 8);
    esi = MEM32(eax + 0x14);
    edx = MEM32(eax + 0x10);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebx + 4)); /* fadd dword ptr [ebx + 4] */
    ecx = MEM32(eax);
    fp_push(MEMF(esp + 0x10)); /* fld float */
    edi = MEM32(eax);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x14)); /* fmul dword ptr [esp + 0x14] */
    MEM32(esp + 0x4C) = esi;
    esi = edx;
    MEM32(esp + 0x48) = ecx;
    ecx = MEM32(eax + 4);
    PUSH32(esp, 0xBF800000u);
    MEM32(esp + 0x4C) = esi;
    esi = MEM32(eax + 0x14);
    eax = ecx;
    MEM32(esp + 0x50) = eax;
    eax = MEM32(esp + 0x34);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    PUSH32(esp, 0x3DCCCCCD);
    PUSH32(esp, 0x3DCCCCCD);
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1AF3A0)); /* fsub dword ptr [0x1af3a0] */
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    MEMF(esp + 4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ebx + 8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C91CC)); /* fmul dword ptr [0x1c91cc] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebx)); /* fadd dword ptr [ebx] */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1AF3A0)); /* fsub dword ptr [0x1af3a0] */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x000C3FD6u); RECOMP_ABI_CALL(0x000E5710u, sub_000E5710); /* call 0x000E5710 */

loc_000C3FD6: ;
    RECOMP_PC(0x000C3FD6u);
    esi = MEM32(esp + 0x44);
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    goto loc_000C4147;

loc_000C3FE2: ;
    RECOMP_PC(0x000C3FE2u);
    edx = MEM32(ebp + 0x8C);
    fp_push(MEMF(ebx + 0xC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C91CC)); /* fmul dword ptr [0x1c91cc] */
    eax = MEM32(edx + 8);
    edi = MEM32(eax + 0x14);
    esi = MEM32(eax + 0x10);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebx + 4)); /* fadd dword ptr [ebx + 4] */
    edx = MEM32(eax);
    ecx = MEM32(ecx + 8);
    fp_push(MEMF(esp + 0x10)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x14)); /* fmul dword ptr [esp + 0x14] */
    PUSH32(esp, 0xBF800000u);
    MEM32(esp + 0x50) = edi;
    PUSH32(esp, ecx);
    edi = esi;
    MEM32(esp + 0x50) = edx;
    edx = MEM32(eax + 4);
    PUSH32(esp, edx);
    MEM32(esp + 0x4C) = edi;
    edi = MEM32(eax + 0x14);
    MEM32(esp + 0x50) = edi;
    edi = MEM32(eax);
    PUSH32(esp, esi);
    eax = edx;
    edx = MEM32(esp + 0x54);
    PUSH32(esp, edx);
    PUSH32(esp, edi);
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    PUSH32(esp, 0x3DCCCCCD);
    PUSH32(esp, 0x3DA3D70A);
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1AF3A0)); /* fsub dword ptr [0x1af3a0] */
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    MEM32(esp + 0x74) = eax;
    MEMF(esp + 4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ebx + 8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C91CC)); /* fmul dword ptr [0x1c91cc] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebx)); /* fadd dword ptr [ebx] */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1B6300)); /* fsub dword ptr [0x1b6300] */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x000C406Au); RECOMP_ABI_CALL(0x000E5710u, sub_000E5710); /* call 0x000E5710 */

loc_000C406A: ;
    RECOMP_PC(0x000C406Au);
    esi = MEM32(esp + 0x44);
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    goto loc_000C4147;

loc_000C4076: ;
    RECOMP_PC(0x000C4076u);
    fp_pop(); /* fstp st(0) */
    fp_push(MEMF(edi + 4)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_000C4147; /* jne: not equal / not zero */

loc_000C408C: ;
    RECOMP_PC(0x000C408Cu);
    eax = MEM32(edi + 0xC);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C409A; /* je: equal / zero */

loc_000C4093: ;
    RECOMP_PC(0x000C4093u);
    MEM32(edi + 0xC) = 0;

loc_000C409A: ;
    RECOMP_PC(0x000C409Au);
    eax = MEM32(ebp + 0xB4);
    ecx = MEM32(eax + 0x58);
    edx = MEM32(esp + 0x18);
    edi = MEM32(ecx + edx + 0xC4);
    ecx = MEM32(eax + 0x60);
    eax = MEM32(ebp + 0xAC);
    edx = MEM32(esi + eax + 0x10);
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edx, 0 (32-bit) */
    eax = esi + eax + 0x10;
    MEM32(esp + 0x1C) = ecx;
    if (CMP_NE(_fa, _fb)) goto loc_000C40E6; /* jne: not equal / not zero */

loc_000C40C7: ;
    RECOMP_PC(0x000C40C7u);
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, ecx (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000C40E6; /* jne: not equal / not zero */

loc_000C40CB: ;
    RECOMP_PC(0x000C40CBu);
    ecx = MEM32(ebp + 0xB8);
    edx = MEM32(ecx + 0x128);
    edx--;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fa = (uint32_t)(MEM32(ecx + 0x244)) & 0xFFFFFFFFu; _fb = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(ecx + 0x244), edx (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000C40E6; /* jne: not equal / not zero */

loc_000C40E0: ;
    RECOMP_PC(0x000C40E0u);
    MEM32(eax) = 1;

loc_000C40E6: ;
    RECOMP_PC(0x000C40E6u);
    eax = MEM32(ebp + 0xAC);
    edx = MEM32(ebp + 0xB4);
    fp_push(MEMF(edx + 0x38)); /* fld float */
    ecx = esi + eax;
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(ecx)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [ecx] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_000C4107; /* jnp: not parity */

loc_000C4101: ;
    RECOMP_PC(0x000C4101u);
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(MEM32(esp + 0x1C)) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, MEM32(esp + 0x1C) (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000C4147; /* jne: not equal / not zero */

loc_000C4107: ;
    RECOMP_PC(0x000C4107u);
    fp_push(MEMF(ecx)); /* fld float */
    PUSH32(esp, 0x3F800000);
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ecx + 4)); /* fsub dword ptr [ecx + 4] */
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    MEMF(ecx) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(ebp + 0xAC);
    ecx = esp + 0x44;
    MEM32(esi + eax + 4) = 0;
    PUSH32(esp, 0x000C4139u); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_000C4139: ;
    RECOMP_PC(0x000C4139u);
    ecx = MEM32(ebp + 0xAC);
    edx = MEM32(esp + 0x34);
    MEM32(esi + ecx + 8) = edx;

loc_000C4147: ;
    RECOMP_PC(0x000C4147u);
    ecx = MEM32(esp + 0x18);
    eax = MEM32(esp + 0x24);
    _fb = (uint32_t)(0x148) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ecx = ecx + 0x148;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x18) = ecx;
    ecx = MEM32(ebp + 0xB4);
    edx = MEM32(ecx + 0x5C);
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(0x14) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + 0x14;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edx (32-bit) */
    MEM32(esp + 0x24) = eax;
    MEM32(esp + 0x1C) = esi;
    if (CMP_L(_fas, _fbs)) goto loc_000C3E00; /* jl: less (signed <) */

loc_000C4176: ;
    RECOMP_PC(0x000C4176u);
    ecx = MEM32(0x1DE3E8);
    eax = 0x901;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, eax (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C4190; /* je: equal / zero */

loc_000C4185: ;
    RECOMP_PC(0x000C4185u);
    PUSH32(esp, eax);
    MEM32(0x1DE3E8) = eax;
    PUSH32(esp, 0x000C4190u); RECOMP_ABI_CALL(0x001931B0u, sub_001931B0); /* call 0x001931B0 */

loc_000C4190: ;
    RECOMP_PC(0x000C4190u);
    PUSH32(esp, 0);
    PUSH32(esp, 0x000C4197u); RECOMP_ABI_CALL(0x0001D370u, sub_0001D370); /* call 0x0001D370 */

loc_000C4197: ;
    RECOMP_PC(0x000C4197u);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x000C419Fu); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000C419F: ;
    RECOMP_PC(0x000C419Fu);
    edx = MEM32(ebp);
    eax = esp + 0x50;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    ecx = ebp;
    { uint32_t _icall_target = MEM32(edx + 0x64); PUSH32(esp, 0x000C41ACu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000C41A9u); } /* indirect call */
    }

loc_000C41AC: ;
    RECOMP_PC(0x000C41ACu);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    _fb = (uint32_t)(0x58) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x58;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; qpr2_game_timing_end(&_game_time); return; /* ret 4 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_000C2DD0
 * Original: 0x000C2DD0 - 0x000C309A (714 bytes, 203 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000C2DD0(void)
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

loc_000C2DD0: ;
    RECOMP_PC(0x000C2DD0u);
    SET_LO8(eax, MEM8(0x1DAC94));
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test LO8(eax), 1 (8-bit) */
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    esi = ecx;
    if (TEST_NZ(_fa, _fb)) goto loc_000C2E0F; /* jne: not equal / not zero */

loc_000C2DE2: ;
    RECOMP_PC(0x000C2DE2u);
    ebx = MEM32(0x1DAC94);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F000000);
    ebx = ebx | 1;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    PUSH32(esp, 0x3F800000);
    ecx = 0x1DAC90;
    MEM32(0x1DAC94) = ebx;
    PUSH32(esp, 0x000C2E0Fu); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_000C2E0F: ;
    RECOMP_PC(0x000C2E0Fu);
    eax = MEM32(esi + 0x11C);
    PUSH32(esp, eax);
    PUSH32(esp, 0x000C2E1Bu); RECOMP_ABI_CALL(0x000E3B00u, sub_000E3B00); /* call 0x000E3B00 */

loc_000C2E1B: ;
    RECOMP_PC(0x000C2E1Bu);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0x000C2E24u); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000C2E24: ;
    RECOMP_PC(0x000C2E24u);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0x000C2E35u); RECOMP_ABI_CALL(0x000E5430u, sub_000E5430); /* call 0x000E5430 */

loc_000C2E35: ;
    RECOMP_PC(0x000C2E35u);
    ecx = MEM32(esi + 0x118);
    edx = MEM32(esi + 0x78);
    _fb = (uint32_t)(0x24) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x24;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 7);
    PUSH32(esp, ecx);
    ecx = MEM32(0x1DE810);
    PUSH32(esp, edx);
    PUSH32(esp, 0x000C2E50u); RECOMP_ABI_CALL(0x000EBD80u, sub_000EBD80); /* call 0x000EBD80 */

loc_000C2E50: ;
    RECOMP_PC(0x000C2E50u);
    eax = MEM32(0x1DC444);
    edi = 0x203;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edi (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C2E76; /* je: equal / zero */

loc_000C2E5E: ;
    RECOMP_PC(0x000C2E5Eu);
    edx = edi;
    ecx = 0x40354;
    MEM32(0x1DC444) = edi;
    PUSH32(esp, 0x000C2E70u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000C2E70: ;
    RECOMP_PC(0x000C2E70u);
    MEM32(0x1A51DC) = edi;

loc_000C2E76: ;
    RECOMP_PC(0x000C2E76u);
    eax = MEM32(0x1DC3CC);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000C2E9C; /* jne: not equal / not zero */

loc_000C2E7F: ;
    RECOMP_PC(0x000C2E7Fu);
    edi = 1;
    edx = edi;
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = edi;
    PUSH32(esp, 0x000C2E96u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000C2E96: ;
    RECOMP_PC(0x000C2E96u);
    MEM32(0x1A51F8) = edi;

loc_000C2E9C: ;
    RECOMP_PC(0x000C2E9Cu);
    fp_push(MEMF(esi + 0x108)); /* fld float */
    ebx = 2;
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_000C2EB9; /* je: equal / zero */

loc_000C2EB4: ;
    RECOMP_PC(0x000C2EB4u);
    ebx = 1;

loc_000C2EB9: ;
    RECOMP_PC(0x000C2EB9u);
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 0 (32-bit) */
    if (CMP_BE(_fa, _fb)) goto loc_000C2F38; /* jbe: below or equal (unsigned <=) */

loc_000C2EBD: ;
    RECOMP_PC(0x000C2EBDu);
    ebp = esi + 0xB8;
    edi = esi + 0xD4;
    /* nop */

loc_000C2ED0: ;
    RECOMP_PC(0x000C2ED0u);
    eax = MEM32(esi + 0x94);
    ecx = MEM32(ebp);
    MEM32(eax + 0x10) = ecx;
    eax = MEM32(esi + 0x94);
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + 0x1C;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    edx = edi + -20;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edx (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C2F20; /* je: equal / zero */

loc_000C2EEC: ;
    RECOMP_PC(0x000C2EECu);
    ecx = MEM32(edi + -20);
    MEM32(eax) = ecx;
    edx = MEM32(edi + -16);
    MEM32(eax + 4) = edx;
    ecx = MEM32(edi + -12);
    MEM32(eax + 8) = ecx;
    edx = MEM32(edi + -8);
    MEM32(eax + 0xC) = edx;
    ecx = MEM32(edi + -4);
    MEM32(eax + 0x10) = ecx;
    edx = MEM32(edi);
    MEM32(eax + 0x14) = edx;
    ecx = MEM32(edi + 4);
    MEM32(eax + 0x18) = ecx;
    edx = MEM32(edi + 8);
    MEM32(eax + 0x1C) = edx;
    ecx = MEM32(edi + 0xC);
    MEM32(eax + 0x20) = ecx;

loc_000C2F20: ;
    RECOMP_PC(0x000C2F20u);
    ecx = MEM32(esi + 0x84);
    PUSH32(esp, 1);
    PUSH32(esp, 1);
    PUSH32(esp, 0x000C2F2Fu); RECOMP_ABI_CALL(0x000EECD0u, sub_000EECD0); /* call 0x000EECD0 */

loc_000C2F2F: ;
    RECOMP_PC(0x000C2F2Fu);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebp = ebp + 4;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fb = (uint32_t)(0x24) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edi = edi + 0x24;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ebx--;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    if ((_fa != 0)) goto loc_000C2ED0; /* jne: not equal / not zero */

loc_000C2F38: ;
    RECOMP_PC(0x000C2F38u);
    eax = MEM32(0x1DC444);
    edi = 0x207;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edi (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C2F5E; /* je: equal / zero */

loc_000C2F46: ;
    RECOMP_PC(0x000C2F46u);
    edx = edi;
    ecx = 0x40354;
    MEM32(0x1DC444) = edi;
    PUSH32(esp, 0x000C2F58u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000C2F58: ;
    RECOMP_PC(0x000C2F58u);
    MEM32(0x1A51DC) = edi;

loc_000C2F5E: ;
    RECOMP_PC(0x000C2F5Eu);
    eax = MEM32(0x1DC3CC);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edi (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C2F81; /* je: equal / zero */

loc_000C2F69: ;
    RECOMP_PC(0x000C2F69u);
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = edi;
    PUSH32(esp, 0x000C2F7Bu); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000C2F7B: ;
    RECOMP_PC(0x000C2F7Bu);
    MEM32(0x1A51F8) = edi;

loc_000C2F81: ;
    RECOMP_PC(0x000C2F81u);
    PUSH32(esp, 1);
    PUSH32(esp, edi);
    PUSH32(esp, 0x000C2F89u); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000C2F89: ;
    RECOMP_PC(0x000C2F89u);
    edx = MEM32(esi + 0x2C);
    eax = MEM32(esi + 0x24);
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    ecx = esi + 0xC;
    PUSH32(esp, ecx);
    ecx = esp + 0x2C;
    PUSH32(esp, 0x000C2FA1u); RECOMP_ABI_CALL(0x00037EF0u, sub_00037EF0); /* call 0x00037EF0 */

loc_000C2FA1: ;
    RECOMP_PC(0x000C2FA1u);
    ecx = MEM32(esp + 0x3C);
    edx = esp + 0x20;
    PUSH32(esp, edx);
    PUSH32(esp, 0x000C2FAFu); RECOMP_ABI_CALL(0x00037C80u, sub_00037C80); /* call 0x00037C80 */

loc_000C2FAF: ;
    RECOMP_PC(0x000C2FAFu);
    eax = MEM32(esi);
    ecx = esp + 0x20;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0x60); PUSH32(esp, 0x000C2FBBu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000C2FB8u); } /* indirect call */
    }

loc_000C2FBB: ;
    RECOMP_PC(0x000C2FBBu);
    PUSH32(esp, 0x1DE7B8);
    PUSH32(esp, 0x000C2FC5u); RECOMP_ABI_CALL(0x000E3B00u, sub_000E3B00); /* call 0x000E3B00 */

loc_000C2FC5: ;
    RECOMP_PC(0x000C2FC5u);
    ecx = MEM32(0x1DE810);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, edi);
    PUSH32(esp, 0x000C2FD4u); RECOMP_ABI_CALL(0x000EBD60u, sub_000EBD60); /* call 0x000EBD60 */

loc_000C2FD4: ;
    RECOMP_PC(0x000C2FD4u);
    PUSH32(esp, 0x000C2FD9u); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000C2FD9: ;
    RECOMP_PC(0x000C2FD9u);
    PUSH32(esp, edi);
    PUSH32(esp, 0x000C2FDFu); RECOMP_ABI_CALL(0x0001D270u, sub_0001D270); /* call 0x0001D270 */

loc_000C2FDF: ;
    RECOMP_PC(0x000C2FDFu);
    edx = MEM32(esi + 0x118);
    PUSH32(esp, edx);
    PUSH32(esp, edi);
    PUSH32(esp, 0x000C2FECu); RECOMP_ABI_CALL(0x000E6A00u, sub_000E6A00); /* call 0x000E6A00 */

loc_000C2FEC: ;
    RECOMP_PC(0x000C2FECu);
    fp_push(MEMF(0x1C13C8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x110)); /* fmul dword ptr [esi + 0x110] */
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, ecx);
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = esp + 0x4C;
    PUSH32(esp, 0x000C3017u); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_000C3017: ;
    RECOMP_PC(0x000C3017u);
    eax = MEM32(esp + 0x3C);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, edi);
    PUSH32(esp, edi);
    PUSH32(esp, eax);
    PUSH32(esp, 0x3F800000);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    MEM32(esp + 0x38) = 0xBF800000u;
    edx = MEM32(esp + 0x38);
    ecx = esp;
    MEM32(ecx) = edx;
    MEM32(esp + 0x3C) = 0x3F800000;
    eax = MEM32(esp + 0x3C);
    MEM32(ecx + 4) = eax;
    MEM32(esp + 0x40) = 0x40000000;
    edx = MEM32(esp + 0x40);
    MEM32(ecx + 8) = edx;
    MEM32(esp + 0x44) = 0xC0000000u;
    eax = MEM32(esp + 0x44);
    MEM32(ecx + 0xC) = eax;
    ecx = esp + 0x48;
    PUSH32(esp, 0x000C3076u); RECOMP_ABI_CALL(0x00112200u, sub_00112200); /* call 0x00112200 */

loc_000C3076: ;
    RECOMP_PC(0x000C3076u);
    PUSH32(esp, edi);
    PUSH32(esp, 0x000C307Cu); RECOMP_ABI_CALL(0x0001D370u, sub_0001D370); /* call 0x0001D370 */

loc_000C307C: ;
    RECOMP_PC(0x000C307Cu);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x000C3084u); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000C3084: ;
    RECOMP_PC(0x000C3084u);
    edx = MEM32(esi);
    eax = esp + 0x20;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x64); PUSH32(esp, 0x000C3090u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000C308Du); } /* indirect call */
    }

loc_000C3090: ;
    RECOMP_PC(0x000C3090u);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; return; /* ret 4 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_000C6140
 * Original: 0x000C6140 - 0x000C63F3 (691 bytes, 227 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000C6140(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_000C6140: ;
    RECOMP_PC(0x000C6140u);
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    esi = ecx;
    eax = MEM32(esi + 0xF8);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    PUSH32(esp, edi);
    if (CMP_EQ(_fa, _fb)) goto loc_000C63EA; /* je: equal / zero */

loc_000C6156: ;
    RECOMP_PC(0x000C6156u);
    eax = MEM32(0x1D9FB8);
    ecx = MEM32(eax + 0x5C);
    ebp = 1;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, ebp (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C63EA; /* je: equal / zero */

loc_000C616B: ;
    RECOMP_PC(0x000C616Bu);
    ecx = MEM32(esi + 0x2C);
    edx = MEM32(esi + 0x24);
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    eax = esi + 0xC;
    PUSH32(esp, eax);
    ecx = esp + 0x28;
    PUSH32(esp, 0x000C6180u); RECOMP_ABI_CALL(0x00037EF0u, sub_00037EF0); /* call 0x00037EF0 */

loc_000C6180: ;
    RECOMP_PC(0x000C6180u);
    ecx = esp + 0x1C;
    PUSH32(esp, ecx);
    ecx = MEM32(esp + 0x3C);
    PUSH32(esp, 0x000C618Eu); RECOMP_ABI_CALL(0x00037C80u, sub_00037C80); /* call 0x00037C80 */

loc_000C618E: ;
    RECOMP_PC(0x000C618Eu);
    edx = MEM32(esi);
    eax = esp + 0x1C;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x60); PUSH32(esp, 0x000C619Au); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000C6197u); } /* indirect call */
    }

loc_000C619A: ;
    RECOMP_PC(0x000C619Au);
    PUSH32(esp, 0x000C619Fu); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000C619F: ;
    RECOMP_PC(0x000C619Fu);
    PUSH32(esp, 0);
    PUSH32(esp, 0x000C61A6u); RECOMP_ABI_CALL(0x0001D270u, sub_0001D270); /* call 0x0001D270 */

loc_000C61A6: ;
    RECOMP_PC(0x000C61A6u);
    eax = MEM32(0x1DE3E8);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C61C3; /* je: equal / zero */

loc_000C61B2: ;
    RECOMP_PC(0x000C61B2u);
    PUSH32(esp, 0);
    MEM32(0x1DE3E8) = 0;
    PUSH32(esp, 0x000C61C3u); RECOMP_ABI_CALL(0x001931B0u, sub_001931B0); /* call 0x001931B0 */

loc_000C61C3: ;
    RECOMP_PC(0x000C61C3u);
    ecx = MEM32(0x1D9FB8);
    eax = MEM32(ecx + 0x5C);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0xA) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0xA (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C6360; /* je: equal / zero */

loc_000C61D5: ;
    RECOMP_PC(0x000C61D5u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0xB) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0xB (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C6360; /* je: equal / zero */

loc_000C61DE: ;
    RECOMP_PC(0x000C61DEu);
    eax = MEM32(esi + 0xE4);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C6236; /* je: equal / zero */

loc_000C61E8: ;
    RECOMP_PC(0x000C61E8u);
    edx = MEM32(esi + 0xDC);
    PUSH32(esp, ebp);
    PUSH32(esp, edx);
    eax = edx;
    PUSH32(esp, eax);
    ecx = esp + 0x44;
    PUSH32(esp, ecx);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    edx = esi + 0xE8;
    ecx = MEM32(edx);
    eax = esp;
    MEM32(eax) = ecx;
    ecx = MEM32(edx + 4);
    MEM32(eax + 4) = ecx;
    ecx = MEM32(edx + 8);
    edx = MEM32(edx + 0xC);
    MEM32(eax + 8) = ecx;
    MEM32(eax + 0xC) = edx;
    eax = MEM32(esi + 0xCC);
    PUSH32(esp, eax);
    ecx = esp + 0x40;
    PUSH32(esp, ecx);
    edx = esp + 0x34;
    PUSH32(esp, edx);
    MEM32(esp + 0x64) = ebp;
    PUSH32(esp, 0x000C6233u); RECOMP_ABI_CALL(0x000C6F10u, sub_000C6F10); /* call 0x000C6F10 */

loc_000C6233: ;
    RECOMP_PC(0x000C6233u);
    _fb = (uint32_t)(0x2C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x2C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */

loc_000C6236: ;
    RECOMP_PC(0x000C6236u);
    eax = MEM32(esi + 0x8C);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C628B; /* je: equal / zero */

loc_000C6240: ;
    RECOMP_PC(0x000C6240u);
    eax = MEM32(0x1DAE80);
    ecx = MEM32(esi + 0x84);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    edx = ecx;
    PUSH32(esp, edx);
    eax = esp + 0x44;
    PUSH32(esp, eax);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    ecx = esi + 0x90;
    eax = MEM32(ecx);
    edx = esp;
    MEM32(edx) = eax;
    eax = MEM32(ecx + 4);
    MEM32(edx + 4) = eax;
    eax = MEM32(ecx + 8);
    ecx = MEM32(ecx + 0xC);
    MEM32(edx + 8) = eax;
    MEM32(edx + 0xC) = ecx;
    edx = MEM32(esi + 0x74);
    PUSH32(esp, edx);
    eax = esp + 0x40;
    PUSH32(esp, eax);
    MEM32(esp + 0x60) = ebp;
    PUSH32(esp, 0x000C6288u); RECOMP_ABI_CALL(0x000C6F90u, sub_000C6F90); /* call 0x000C6F90 */

loc_000C6288: ;
    RECOMP_PC(0x000C6288u);
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */

loc_000C628B: ;
    RECOMP_PC(0x000C628Bu);
    ecx = MEM32(0x1D9FB8);
    _fa = (uint32_t)(MEM32(ecx + 4)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(ecx + 4), ebp (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000C630D; /* jne: not equal / not zero */

loc_000C6296: ;
    RECOMP_PC(0x000C6296u);
    eax = MEM32(esi + 0x104);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_BE(_fa, _fb)) goto loc_000C630D; /* jbe: below or equal (unsigned <=) */

loc_000C62A2: ;
    RECOMP_PC(0x000C62A2u);
    edx = MEM32(esi + 0x70);
    ecx = MEM32(edx + edi * 4);
    eax = edx + edi * 4;
    edx = MEM32(ecx + 0x18);
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edx, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C6302; /* je: equal / zero */

loc_000C62B2: ;
    RECOMP_PC(0x000C62B2u);
    edx = MEM32(0x1DAE80);
    PUSH32(esp, edx);
    eax = ecx;
    MEM32(esp + 0x3C) = 2;
    ecx = MEM32(eax + 0x10);
    PUSH32(esp, ecx);
    edx = ecx;
    PUSH32(esp, edx);
    ecx = esp + 0x44;
    PUSH32(esp, ecx);
    edx = eax + 0x1C;
    ebp = MEM32(edx);
    eax = MEM32(eax);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    ecx = esp;
    MEM32(ecx) = ebp;
    ebp = MEM32(edx + 4);
    MEM32(ecx + 4) = ebp;
    ebp = MEM32(edx + 8);
    edx = MEM32(edx + 0xC);
    MEM32(ecx + 8) = ebp;
    MEM32(ecx + 0xC) = edx;
    PUSH32(esp, eax);
    ecx = esp + 0x40;
    PUSH32(esp, ecx);
    PUSH32(esp, 0x000C62FAu); RECOMP_ABI_CALL(0x000C6F90u, sub_000C6F90); /* call 0x000C6F90 */

loc_000C62FA: ;
    RECOMP_PC(0x000C62FAu);
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ebp = 1;

loc_000C6302: ;
    RECOMP_PC(0x000C6302u);
    eax = MEM32(esi + 0x104);
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, eax (32-bit) */
    if (CMP_B(_fa, _fb)) goto loc_000C62A2; /* jb: below (unsigned <) */

loc_000C630D: ;
    RECOMP_PC(0x000C630Du);
    eax = MEM32(esi + 0xB8);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C63B5; /* je: equal / zero */

loc_000C631B: ;
    RECOMP_PC(0x000C631Bu);
    edx = MEM32(0x1DAE80);
    eax = MEM32(esi + 0xB0);
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    ecx = eax;
    PUSH32(esp, ecx);
    edx = esp + 0x44;
    PUSH32(esp, edx);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    eax = esi + 0xBC;
    edx = MEM32(eax);
    ecx = esp;
    MEM32(ecx) = edx;
    edx = MEM32(eax + 4);
    MEM32(ecx + 4) = edx;
    edx = MEM32(eax + 8);
    eax = MEM32(eax + 0xC);
    MEM32(ecx + 8) = edx;
    MEM32(ecx + 0xC) = eax;
    ecx = MEM32(esi + 0xA0);
    PUSH32(esp, ecx);
    edx = esp + 0x40;
    PUSH32(esp, edx);
    goto loc_000C63A9;

loc_000C6360: ;
    RECOMP_PC(0x000C6360u);
    eax = MEM32(esi + 0x8C);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C63B5; /* je: equal / zero */

loc_000C636A: ;
    RECOMP_PC(0x000C636Au);
    eax = MEM32(0x1DAE80);
    ecx = MEM32(esi + 0x84);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    edx = ecx;
    PUSH32(esp, edx);
    eax = esp + 0x44;
    PUSH32(esp, eax);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    ecx = esi + 0x90;
    eax = MEM32(ecx);
    edx = esp;
    MEM32(edx) = eax;
    eax = MEM32(ecx + 4);
    MEM32(edx + 4) = eax;
    eax = MEM32(ecx + 8);
    ecx = MEM32(ecx + 0xC);
    MEM32(edx + 8) = eax;
    MEM32(edx + 0xC) = ecx;
    edx = MEM32(esi + 0x74);
    PUSH32(esp, edx);
    eax = esp + 0x40;
    PUSH32(esp, eax);

loc_000C63A9: ;
    RECOMP_PC(0x000C63A9u);
    MEM32(esp + 0x60) = ebp;
    PUSH32(esp, 0x000C63B2u); RECOMP_ABI_CALL(0x000C6F90u, sub_000C6F90); /* call 0x000C6F90 */

loc_000C63B2: ;
    RECOMP_PC(0x000C63B2u);
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */

loc_000C63B5: ;
    RECOMP_PC(0x000C63B5u);
    ecx = MEM32(0x1DE3E8);
    eax = 0x901;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, eax (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C63CF; /* je: equal / zero */

loc_000C63C4: ;
    RECOMP_PC(0x000C63C4u);
    PUSH32(esp, eax);
    MEM32(0x1DE3E8) = eax;
    PUSH32(esp, 0x000C63CFu); RECOMP_ABI_CALL(0x001931B0u, sub_001931B0); /* call 0x001931B0 */

loc_000C63CF: ;
    RECOMP_PC(0x000C63CFu);
    PUSH32(esp, 0);
    PUSH32(esp, 0x000C63D6u); RECOMP_ABI_CALL(0x0001D370u, sub_0001D370); /* call 0x0001D370 */

loc_000C63D6: ;
    RECOMP_PC(0x000C63D6u);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x000C63DEu); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000C63DE: ;
    RECOMP_PC(0x000C63DEu);
    edx = MEM32(esi);
    eax = esp + 0x1C;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x64); PUSH32(esp, 0x000C63EAu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000C63E7u); } /* indirect call */
    }

loc_000C63EA: ;
    RECOMP_PC(0x000C63EAu);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; return; /* ret 4 */

}


/**
 * qpr2_recovered_000C0860
 * Original: 0x000C0860 - 0x000C0B7A (794 bytes, 227 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000C0860(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_000C0860: ;
    RECOMP_PC(0x000C0860u);
    _fb = (uint32_t)(0x3C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x3C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    ebx = ecx;
    eax = MEM32(ebx + 0x2C);
    ecx = MEM32(ebx + 0x24);
    PUSH32(esp, edi);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    edx = ebx + 0xC;
    PUSH32(esp, edx);
    ecx = esp + 0x40;
    PUSH32(esp, 0x000C087Eu); RECOMP_ABI_CALL(0x00037EF0u, sub_00037EF0); /* call 0x00037EF0 */

loc_000C087E: ;
    RECOMP_PC(0x000C087Eu);
    ecx = MEM32(esp + 0x50);
    eax = esp + 0x34;
    PUSH32(esp, eax);
    PUSH32(esp, 0x000C088Cu); RECOMP_ABI_CALL(0x00037C80u, sub_00037C80); /* call 0x00037C80 */

loc_000C088C: ;
    RECOMP_PC(0x000C088Cu);
    edx = MEM32(ebx);
    eax = esp + 0x34;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    ecx = ebx;
    { uint32_t _icall_target = MEM32(edx + 0x60); PUSH32(esp, 0x000C0898u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000C0895u); } /* indirect call */
    }

loc_000C0898: ;
    RECOMP_PC(0x000C0898u);
    PUSH32(esp, 0x000C089Du); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000C089D: ;
    RECOMP_PC(0x000C089Du);
    ebp = 0; /* xor self */
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    PUSH32(esp, ebp);
    PUSH32(esp, 0x000C08A5u); RECOMP_ABI_CALL(0x0001D270u, sub_0001D270); /* call 0x0001D270 */

loc_000C08A5: ;
    RECOMP_PC(0x000C08A5u);
    eax = MEM32(0x1DE3E8);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebp (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C08BD; /* je: equal / zero */

loc_000C08B1: ;
    RECOMP_PC(0x000C08B1u);
    PUSH32(esp, ebp);
    MEM32(0x1DE3E8) = ebp;
    PUSH32(esp, 0x000C08BDu); RECOMP_ABI_CALL(0x001931B0u, sub_001931B0); /* call 0x001931B0 */

loc_000C08BD: ;
    RECOMP_PC(0x000C08BDu);
    _fa = (uint32_t)(MEM32(0x1DC3CC)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC3CC), ebp (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C08DD; /* je: equal / zero */

loc_000C08C5: ;
    RECOMP_PC(0x000C08C5u);
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = ebp;
    PUSH32(esp, 0x000C08D7u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000C08D7: ;
    RECOMP_PC(0x000C08D7u);
    MEM32(0x1A51F8) = ebp;

loc_000C08DD: ;
    RECOMP_PC(0x000C08DDu);
    ecx = MEM32(0x1DE46C);
    MEM32(ecx + 0x64) = ebp;
    edx = MEM32(ebx + 0x70);
    eax = MEM32(edx + 0x48);
    esi = MEM32(eax + 0x128);
    eax = MEM32(eax + 0x244);
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, esi (32-bit) */
    if (CMP_LE(_fas, _fbs)) goto loc_000C08FF; /* jle: less or equal (signed <=) */

loc_000C08FD: ;
    RECOMP_PC(0x000C08FDu);
    eax = esi;

loc_000C08FF: ;
    RECOMP_PC(0x000C08FFu);
    PUSH32(esp, eax);
    eax = esp + 0x14;
    PUSH32(esp, 0x1C11AC);
    PUSH32(esp, eax);
    PUSH32(esp, 0x000C090Fu); RECOMP_ABI_CALL(0x000E07A0u, sub_000E07A0); /* call 0x000E07A0 */

loc_000C090F: ;
    RECOMP_PC(0x000C090Fu);
    ecx = MEM32(0x1DAE80);
    eax = MEM32(0x1D822C);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, ecx);
    PUSH32(esp, eax);
    PUSH32(esp, eax);
    edx = esp + 0x5C;
    PUSH32(esp, edx);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    eax = esp;
    MEM32(esp + 0x34) = 0xBF800000u;
    ecx = MEM32(esp + 0x34);
    MEM32(eax) = ecx;
    MEM32(esp + 0x38) = 0;
    edx = MEM32(esp + 0x38);
    MEM32(eax + 4) = edx;
    MEM32(esp + 0x3C) = 0x3F800000;
    ecx = MEM32(esp + 0x3C);
    MEM32(eax + 8) = ecx;
    MEM32(esp + 0x40) = 0x3F800000;
    edx = MEM32(esp + 0x40);
    MEM32(eax + 0xC) = edx;
    eax = esp + 0x30;
    PUSH32(esp, eax);
    ecx = esp + 0x58;
    edi = 1;
    PUSH32(esp, ecx);
    MEM32(esp + 0x78) = edi;
    PUSH32(esp, 0x000C097Du); RECOMP_ABI_CALL(0x000C6F90u, sub_000C6F90); /* call 0x000C6F90 */

loc_000C097D: ;
    RECOMP_PC(0x000C097Du);
    PUSH32(esp, esi);
    edx = esp + 0x3C;
    PUSH32(esp, 0x1C11AC);
    PUSH32(esp, edx);
    PUSH32(esp, 0x000C098Du); RECOMP_ABI_CALL(0x000E07A0u, sub_000E07A0); /* call 0x000E07A0 */

loc_000C098D: ;
    RECOMP_PC(0x000C098Du);
    eax = MEM32(0x1DAE80);
    _fb = (uint32_t)(0x34) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x34;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, eax);
    eax = MEM32(0x1D822C);
    PUSH32(esp, eax);
    PUSH32(esp, eax);
    ecx = esp + 0x5C;
    PUSH32(esp, ecx);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    edx = esp;
    MEM32(esp + 0x34) = 0;
    eax = MEM32(esp + 0x34);
    MEM32(edx) = eax;
    MEM32(esp + 0x38) = 0xBF800000u;
    ecx = MEM32(esp + 0x38);
    MEM32(edx + 4) = ecx;
    MEM32(esp + 0x3C) = 0x3F800000;
    eax = MEM32(esp + 0x3C);
    MEM32(edx + 8) = eax;
    MEM32(esp + 0x40) = 0x3FC00000;
    ecx = MEM32(esp + 0x40);
    MEM32(edx + 0xC) = ecx;
    edx = esp + 0x30;
    PUSH32(esp, edx);
    eax = esp + 0x58;
    PUSH32(esp, eax);
    MEM32(esp + 0x78) = edi;
    PUSH32(esp, 0x000C09F5u); RECOMP_ABI_CALL(0x000C6F90u, sub_000C6F90); /* call 0x000C6F90 */

loc_000C09F5: ;
    RECOMP_PC(0x000C09F5u);
    ecx = esp + 0x38;
    PUSH32(esp, 0x1C11A8);
    PUSH32(esp, ecx);
    PUSH32(esp, 0x000C0A04u); RECOMP_ABI_CALL(0x000E07A0u, sub_000E07A0); /* call 0x000E07A0 */

loc_000C0A04: ;
    RECOMP_PC(0x000C0A04u);
    edx = MEM32(0x1DAE80);
    eax = MEM32(0x1D822C);
    _fb = (uint32_t)(0x30) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x30;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, edx);
    MEM32(esp + 0x54) = 0x11;
    MEM32(esp + 0x18) = 0xBF000000u;
    MEM32(esp + 0x1C) = 0xBEB33333u;
    MEM32(esp + 0x20) = 0x3F800000;
    MEM32(esp + 0x24) = 0x3F800000;
    PUSH32(esp, eax);
    edx = MEM32(esp + 0x1C);
    PUSH32(esp, eax);
    eax = esp + 0x5C;
    PUSH32(esp, eax);
    eax = MEM32(esp + 0x28);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    ecx = esp;
    MEM32(ecx) = edx;
    edx = MEM32(esp + 0x3C);
    MEM32(ecx + 4) = eax;
    eax = MEM32(esp + 0x40);
    MEM32(ecx + 8) = edx;
    MEM32(ecx + 0xC) = eax;
    ecx = esp + 0x30;
    PUSH32(esp, ecx);
    edx = esp + 0x58;
    PUSH32(esp, edx);
    PUSH32(esp, 0x000C0A71u); RECOMP_ABI_CALL(0x000C6F90u, sub_000C6F90); /* call 0x000C6F90 */

loc_000C0A71: ;
    RECOMP_PC(0x000C0A71u);
    ecx = MEM32(0x1DB454);
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x12B);
    MEM32(esp + 0x54) = ebp;
    MEM32(esp + 0x18) = 0xBF800000u;
    MEM32(esp + 0x1C) = 0xBF8CCCCDu;
    MEM32(esp + 0x20) = 0;
    MEM32(esp + 0x24) = 0x3F000000;
    PUSH32(esp, 0x000C0AA8u); RECOMP_ABI_CALL(0x000E0830u, sub_000E0830); /* call 0x000E0830 */

loc_000C0AA8: ;
    RECOMP_PC(0x000C0AA8u);
    ecx = MEM32(0x1D822C);
    PUSH32(esp, edi);
    PUSH32(esp, ecx);
    PUSH32(esp, ecx);
    ecx = esp + 0x5C;
    PUSH32(esp, ecx);
    ecx = MEM32(esp + 0x24);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    edx = esp;
    MEM32(edx) = ecx;
    ecx = MEM32(esp + 0x38);
    MEM32(edx + 4) = ecx;
    ecx = MEM32(esp + 0x3C);
    MEM32(edx + 8) = ecx;
    ecx = MEM32(esp + 0x40);
    PUSH32(esp, eax);
    MEM32(edx + 0xC) = ecx;
    edx = esp + 0x58;
    PUSH32(esp, edx);
    eax = esp + 0x4C;
    PUSH32(esp, eax);
    PUSH32(esp, 0x000C0AE6u); RECOMP_ABI_CALL(0x000C6F10u, sub_000C6F10); /* call 0x000C6F10 */

loc_000C0AE6: ;
    RECOMP_PC(0x000C0AE6u);
    ecx = MEM32(0x1DE46C);
    MEM32(ecx + 0x64) = edi;
    ecx = MEM32(0x1DE3E8);
    eax = 0x901;
    _fb = (uint32_t)(0x2C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x2C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, eax (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000C0B0C; /* je: equal / zero */

loc_000C0B01: ;
    RECOMP_PC(0x000C0B01u);
    PUSH32(esp, eax);
    MEM32(0x1DE3E8) = eax;
    PUSH32(esp, 0x000C0B0Cu); RECOMP_ABI_CALL(0x001931B0u, sub_001931B0); /* call 0x001931B0 */

loc_000C0B0C: ;
    RECOMP_PC(0x000C0B0Cu);
    _fa = (uint32_t)(MEM32(0x1DC3CC)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC3CC), ebp (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000C0B2C; /* jne: not equal / not zero */

loc_000C0B14: ;
    RECOMP_PC(0x000C0B14u);
    edx = edi;
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = edi;
    PUSH32(esp, 0x000C0B26u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000C0B26: ;
    RECOMP_PC(0x000C0B26u);
    MEM32(0x1A51F8) = edi;

loc_000C0B2C: ;
    RECOMP_PC(0x000C0B2Cu);
    PUSH32(esp, ebp);
    PUSH32(esp, 0x000C0B32u); RECOMP_ABI_CALL(0x0001D370u, sub_0001D370); /* call 0x0001D370 */

loc_000C0B32: ;
    RECOMP_PC(0x000C0B32u);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x000C0B3Au); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000C0B3A: ;
    RECOMP_PC(0x000C0B3Au);
    edx = MEM32(ebx);
    eax = esp + 0x34;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    ecx = ebx;
    { uint32_t _icall_target = MEM32(edx + 0x64); PUSH32(esp, 0x000C0B46u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000C0B43u); } /* indirect call */
    }

loc_000C0B46: ;
    RECOMP_PC(0x000C0B46u);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    _fb = (uint32_t)(0x3C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x3C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; return; /* ret 4 */

loc_000C0B60: ;
    RECOMP_PC(0x000C0B60u);
    MEM32(eax + -12) = esi;
    MEM32(eax) = esi;
    MEM32(eax + 0xC) = esi;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + 4;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    edx--;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    if ((_fa != 0)) goto loc_000C0B60; /* jne: not equal / not zero */

loc_000C0B6E: ;
    RECOMP_PC(0x000C0B6Eu);
    MEM32(ecx + 0x94) = 1;
    POP32(esp, esi);
    esp += 4; return; /* ret */

}

