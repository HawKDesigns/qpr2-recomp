/* Bounded routines recovered from the supplied retail disassembly. */

#define RECOMP_GENERATED_CODE

#include "recomp/gen/recomp_types.h"

#include "recomp/gen/recomp_funcs.h"

void qpr2_recovered_001A7D43(void);

void qpr2_recovered_0003FD60(void);

void qpr2_recovered_0004EC80(void);

void qpr2_recovered_0004FDD0(void);

void qpr2_recovered_00050010(void);

void qpr2_recovered_000A4070(void);

void qpr2_recovered_000A4DE0(void);

void qpr2_recovered_000EA920(void);

void qpr2_recovered_00013320(void);

void qpr2_recovered_000137B0(void);

void qpr2_recovered_00011AC0(void);

void qpr2_recovered_000112F0(void);

void qpr2_recovered_00012B50(void);

#include <stdio.h>
#include <stdlib.h>
static void qpr2_input_publish_probe(uint32_t payload)
{
    static int on = -1, seen;
    static uint8_t previous[10];
    uint8_t current[10];
    unsigned i;
    if (on < 0) on = getenv("RECOMP_INPUT_DIAG") != NULL;
    if (!on) return;
    for (i = 0; i < 10; ++i) current[i] = MEM8(payload + i);
    if (!seen || memcmp(previous, current, 10) != 0) {
        fprintf(stderr, "[INPUT-PUBLISHED] payload=%08X digital=%02X%02X A=%u B=%u X=%u Y=%u LT=%u RT=%u\n",
                payload, current[1], current[0], current[2], current[3], current[4], current[5], current[8], current[9]);
        memcpy(previous, current, 10); seen = 1;
    }
}

/**
 * qpr2_recovered_001A7D43
 * Original: 0x001A7D43 - 0x001A7D95 (82 bytes, 40 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_001A7D43(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_001A7D43: ;
    RECOMP_PC(0x001A7D43u);
    eax = ecx;
    ecx = MEM32(eax + 0x66);
    PUSH32(esp, ebx);
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(2) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, 2 (32-bit) */
    edx = eax + 0x14;
    if (CMP_B(_fa, _fb)) goto loc_001A7D93; /* jb: below (unsigned <) */

loc_001A7D53: ;
    RECOMP_PC(0x001A7D53u);
    PUSH32(esp, esi);
    _fb = (uint32_t)(0xFFFFFFFEu) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ecx = ecx + 0xFFFFFFFEu;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, edi);
    esi = eax + 0x34;
    eax = ecx;
    ecx = ecx >> 2;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    edi = edx;
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
    ecx = eax;
    ecx = ecx & 3;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    PUSH32(esp, 8);
    { uint64_t _n = (uint64_t)ecx * 1u;
    if (!g_df && esi < 0xFD000000u && edi < 0xFD000000u && (uint64_t)esi + _n <= 0xFD000000u && (uint64_t)edi + _n <= 0xFD000000u) {
      uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi);
      if ((uint64_t)edi + _n <= esi || (uint64_t)esi + _n <= edi) memcpy(_d, _s, (size_t)_n);
      else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM8(edi + _i*1u) = MEM8(esi + _i*1u); }
      esi += ecx * 1u; edi += ecx * 1u; }
    else { uint32_t _i; int32_t _st = RECOMP_DF_STEP(1);
      for (_i = 0; _i < ecx; _i++) MEM8(edi + _i*_st) = MEM8(esi + _i*_st);
      esi += ecx * _st; edi += ecx * _st; }
    ecx = 0; } /* rep movsb */
    POP32(esp, ecx);
    POP32(esp, edi);
    eax = edx + 2;
    POP32(esp, esi);

loc_001A7D73: ;
    RECOMP_PC(0x001A7D73u);
    _fa = (uint32_t)(MEM8(eax)) & 0xFFu; _fb = (uint32_t)(0x20) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp MEM8(eax), 0x20 (8-bit) */
    if (CMP_AE(_fa, _fb)) goto loc_001A7D7D; /* jae: above or equal (unsigned >=) */

loc_001A7D78: ;
    RECOMP_PC(0x001A7D78u);
    MEM8(eax) = MEM8(eax) & 0;
    _fa = (uint32_t)(MEM8(eax)) & 0xFFu; _fas = (int32_t)(int8_t)(_fa); /* and result */
    goto loc_001A7D80;

loc_001A7D7D: ;
    RECOMP_PC(0x001A7D7Du);
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ebx++;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */

loc_001A7D80: ;
    RECOMP_PC(0x001A7D80u);
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    ecx--;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    if ((_fa != 0)) goto loc_001A7D73; /* jne: not equal / not zero */

loc_001A7D84: ;
    RECOMP_PC(0x001A7D84u);
    qpr2_input_publish_probe(edx);
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 0 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_001A7D8D; /* jne: not equal / not zero */

loc_001A7D88: ;
    RECOMP_PC(0x001A7D88u);
    _fa = (uint32_t)(MEM8(edx)) & 0xFFu; _fb = (uint32_t)(0x3F) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test MEM8(edx), 0x3F (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_001A7D93; /* je: equal / zero */

loc_001A7D8D: ;
    RECOMP_PC(0x001A7D8Du);
    POP32(esp, ebx);
    g_seh_ebp = ebp; sub_000CDCC9(); return; /* tail jmp 0x000CDCC9 */

loc_001A7D93: ;
    RECOMP_PC(0x001A7D93u);
    POP32(esp, ebx);
    esp += 4; return; /* ret */

}


/**
 * qpr2_recovered_0003FD60
 * Original: 0x0003FD60 - 0x0003FE5F (255 bytes, 66 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_0003FD60(void)
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

loc_0003FD60: ;
    RECOMP_PC(0x0003FD60u);
    eax = MEM32(esp + 4);
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, esi);
    PUSH32(esp, eax);
    esi = ecx;
    PUSH32(esp, 0x0003FD70u); RECOMP_ABI_CALL(0x0010FEE0u, sub_0010FEE0); /* call 0x0010FEE0 */

loc_0003FD70: ;
    RECOMP_PC(0x0003FD70u);
    eax = MEM32(esi + 0xB0);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    POP32(esp, esi);
    if (CMP_EQ(_fa, _fb)) goto loc_0003FE59; /* je: equal / zero */

loc_0003FD7F: ;
    RECOMP_PC(0x0003FD7Fu);
    PUSH32(esp, 0x0003FD84u); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_0003FD84: ;
    RECOMP_PC(0x0003FD84u);
    PUSH32(esp, 0);
    PUSH32(esp, 0x0003FD8Bu); RECOMP_ABI_CALL(0x0001D270u, sub_0001D270); /* call 0x0001D270 */

loc_0003FD8B: ;
    RECOMP_PC(0x0003FD8Bu);
    eax = MEM32(0x1DE3E8);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0003FDA8; /* je: equal / zero */

loc_0003FD97: ;
    RECOMP_PC(0x0003FD97u);
    PUSH32(esp, 0);
    MEM32(0x1DE3E8) = 0;
    PUSH32(esp, 0x0003FDA8u); RECOMP_ABI_CALL(0x001931B0u, sub_001931B0); /* call 0x001931B0 */

loc_0003FDA8: ;
    RECOMP_PC(0x0003FDA8u);
    fp_push(MEMF(0x1B31E0)); /* fld float */
    ecx = MEM32(0x1DE3DC);
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(0x1DC16C)); /* fdiv dword ptr [0x1dc16c] */
    PUSH32(esp, ecx);
    PUSH32(esp, 0);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1B02B0)); /* fmul dword ptr [0x1b02b0] */
    MEMF(esp + 0xC) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x1B31E0)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(0x1DC168)); /* fdiv dword ptr [0x1dc168] */
    MEMF(esp + 8) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x0003FDDCu); RECOMP_ABI_CALL(0x000E6A00u, sub_000E6A00); /* call 0x000E6A00 */

loc_0003FDDC: ;
    RECOMP_PC(0x0003FDDCu);
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    ecx = esp + 0x1C;
    PUSH32(esp, 0x0003FDFCu); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_0003FDFC: ;
    RECOMP_PC(0x0003FDFCu);
    edx = MEM32(esp + 0xC);
    eax = MEM32(esp);
    ecx = MEM32(esp + 4);
    PUSH32(esp, 0xBF800000u);
    PUSH32(esp, edx);
    PUSH32(esp, 0);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    PUSH32(esp, 0x3F47AE14);
    PUSH32(esp, 0xBF6E147Bu);
    PUSH32(esp, 0x0003FE2Du); RECOMP_ABI_CALL(0x000E5710u, sub_000E5710); /* call 0x000E5710 */

loc_0003FE2D: ;
    RECOMP_PC(0x0003FE2Du);
    ecx = MEM32(0x1DE3E8);
    eax = 0x901;
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, eax (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0003FE4A; /* je: equal / zero */

loc_0003FE3F: ;
    RECOMP_PC(0x0003FE3Fu);
    PUSH32(esp, eax);
    MEM32(0x1DE3E8) = eax;
    PUSH32(esp, 0x0003FE4Au); RECOMP_ABI_CALL(0x001931B0u, sub_001931B0); /* call 0x001931B0 */

loc_0003FE4A: ;
    RECOMP_PC(0x0003FE4Au);
    PUSH32(esp, 0);
    PUSH32(esp, 0x0003FE51u); RECOMP_ABI_CALL(0x0001D370u, sub_0001D370); /* call 0x0001D370 */

loc_0003FE51: ;
    RECOMP_PC(0x0003FE51u);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x0003FE59u); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_0003FE59: ;
    RECOMP_PC(0x0003FE59u);
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; return; /* ret 4 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_0004EC80
 * Original: 0x0004EC80 - 0x0004EDA7 (295 bytes, 77 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_0004EC80(void)
{
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;

loc_0004EC80: ;
    RECOMP_PC(0x0004EC80u);
    PUSH32(esp, ebx);
    PUSH32(esp, edi);
    edi = ecx;
    eax = MEM32(edi + 0xB0);
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebx (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0004EDA2; /* je: equal / zero */

loc_0004EC94: ;
    RECOMP_PC(0x0004EC94u);
    _fa = (uint32_t)(MEM32(edi + 0x48)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(edi + 0x48), ebx (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0004EDA2; /* je: equal / zero */

loc_0004EC9D: ;
    RECOMP_PC(0x0004EC9Du);
    PUSH32(esp, esi);
    PUSH32(esp, 0x0004ECA3u); RECOMP_ABI_CALL(0x0009FEF0u, sub_0009FEF0); /* call 0x0009FEF0 */

loc_0004ECA3: ;
    RECOMP_PC(0x0004ECA3u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0004ECB1; /* je: equal / zero */

loc_0004ECA7: ;
    RECOMP_PC(0x0004ECA7u);
    PUSH32(esp, 0x0004ECACu); RECOMP_ABI_CALL(0x0009FEF0u, sub_0009FEF0); /* call 0x0009FEF0 */

loc_0004ECAC: ;
    RECOMP_PC(0x0004ECACu);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0xFFFFFFFFu) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0xFFFFFFFFu (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_0004ED17; /* jne: not equal / not zero */

loc_0004ECB1: ;
    RECOMP_PC(0x0004ECB1u);
    eax = MEM32(0x1DC444);
    esi = 0x207;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, esi (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0004ECD7; /* je: equal / zero */

loc_0004ECBF: ;
    RECOMP_PC(0x0004ECBFu);
    edx = esi;
    ecx = 0x40354;
    MEM32(0x1DC444) = esi;
    PUSH32(esp, 0x0004ECD1u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_0004ECD1: ;
    RECOMP_PC(0x0004ECD1u);
    MEM32(0x1A51DC) = esi;

loc_0004ECD7: ;
    RECOMP_PC(0x0004ECD7u);
    _fa = (uint32_t)(MEM32(0x1DC3CC)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC3CC), ebx (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0004ED17; /* je: equal / zero */

loc_0004ECDF: ;
    RECOMP_PC(0x0004ECDFu);
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = ebx;
    PUSH32(esp, 0x0004ECF1u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_0004ECF1: ;
    RECOMP_PC(0x0004ECF1u);
    _fa = (uint32_t)(MEM32(0x1DC3CC)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC3CC), ebx (32-bit) */
    MEM32(0x1A51F8) = ebx;
    if (CMP_EQ(_fa, _fb)) goto loc_0004ED17; /* je: equal / zero */

loc_0004ECFF: ;
    RECOMP_PC(0x0004ECFFu);
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = ebx;
    PUSH32(esp, 0x0004ED11u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_0004ED11: ;
    RECOMP_PC(0x0004ED11u);
    MEM32(0x1A51F8) = ebx;

loc_0004ED17: ;
    RECOMP_PC(0x0004ED17u);
    eax = MEM32(esp + 0x10);
    PUSH32(esp, eax);
    ecx = edi;
    PUSH32(esp, 0x0004ED23u); RECOMP_ABI_CALL(0x0010FEE0u, sub_0010FEE0); /* call 0x0010FEE0 */

loc_0004ED23: ;
    RECOMP_PC(0x0004ED23u);
    PUSH32(esp, 0x0004ED28u); RECOMP_ABI_CALL(0x0009FEF0u, sub_0009FEF0); /* call 0x0009FEF0 */

loc_0004ED28: ;
    RECOMP_PC(0x0004ED28u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0004ED36; /* je: equal / zero */

loc_0004ED2C: ;
    RECOMP_PC(0x0004ED2Cu);
    PUSH32(esp, 0x0004ED31u); RECOMP_ABI_CALL(0x0009FEF0u, sub_0009FEF0); /* call 0x0009FEF0 */

loc_0004ED31: ;
    RECOMP_PC(0x0004ED31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0xFFFFFFFFu) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0xFFFFFFFFu (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_0004EDA1; /* jne: not equal / not zero */

loc_0004ED36: ;
    RECOMP_PC(0x0004ED36u);
    eax = MEM32(0x1DC444);
    esi = 0x203;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, esi (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0004ED5C; /* je: equal / zero */

loc_0004ED44: ;
    RECOMP_PC(0x0004ED44u);
    edx = esi;
    ecx = 0x40354;
    MEM32(0x1DC444) = esi;
    PUSH32(esp, 0x0004ED56u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_0004ED56: ;
    RECOMP_PC(0x0004ED56u);
    MEM32(0x1A51DC) = esi;

loc_0004ED5C: ;
    RECOMP_PC(0x0004ED5Cu);
    _fa = (uint32_t)(MEM32(0x1DC3CC)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC3CC), ebx (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_0004EDA1; /* jne: not equal / not zero */

loc_0004ED64: ;
    RECOMP_PC(0x0004ED64u);
    esi = 1;
    edx = esi;
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = esi;
    PUSH32(esp, 0x0004ED7Bu); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_0004ED7B: ;
    RECOMP_PC(0x0004ED7Bu);
    _fa = (uint32_t)(MEM32(0x1DC3CC)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC3CC), ebx (32-bit) */
    MEM32(0x1A51F8) = esi;
    if (CMP_NE(_fa, _fb)) goto loc_0004EDA1; /* jne: not equal / not zero */

loc_0004ED89: ;
    RECOMP_PC(0x0004ED89u);
    edx = esi;
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = esi;
    PUSH32(esp, 0x0004ED9Bu); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_0004ED9B: ;
    RECOMP_PC(0x0004ED9Bu);
    MEM32(0x1A51F8) = esi;

loc_0004EDA1: ;
    RECOMP_PC(0x0004EDA1u);
    POP32(esp, esi);

loc_0004EDA2: ;
    RECOMP_PC(0x0004EDA2u);
    POP32(esp, edi);
    POP32(esp, ebx);
    esp += 8; return; /* ret 4 */

}


/**
 * qpr2_recovered_0004FDD0
 * Original: 0x0004FDD0 - 0x0004FE99 (201 bytes, 69 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_0004FDD0(void)
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

loc_0004FDD0: ;
    RECOMP_PC(0x0004FDD0u);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebx);
    ebx = ecx;
    eax = MEM32(ebx + 0x70);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0004FE92; /* je: equal / zero */

loc_0004FDE1: ;
    RECOMP_PC(0x0004FDE1u);
    eax = MEM32(esp + 0x28);
    PUSH32(esp, eax);
    PUSH32(esp, 0x0004FDEBu); RECOMP_ABI_CALL(0x001025F0u, sub_001025F0); /* call 0x001025F0 */

loc_0004FDEB: ;
    RECOMP_PC(0x0004FDEBu);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    MEM32(esp + 0x10) = eax;
    if (CMP_EQ(_fa, _fb)) goto loc_0004FE92; /* je: equal / zero */

loc_0004FDFA: ;
    RECOMP_PC(0x0004FDFAu);
    ecx = MEM32(esp + 0x18);
    edx = MEM32(esp + 0x1C);
    MEM32(esp + 4) = ecx;
    ecx = MEM32(0x1DE46C);
    MEM32(esp + 0xC) = edx;
    edx = MEM32(ebx + 0x40);
    MEM32(esp + 8) = 0;
    edx = MEM32(ecx + edx * 4 + 0x24);
    PUSH32(esp, ebp);
    MEM32(ecx) = edx;
    ecx = MEM32(0x1DE46C);
    ebp = MEM32(ecx);
    PUSH32(esp, esi);
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_LE(_fas, _fbs)) goto loc_0004FE90; /* jle: less or equal (signed <=) */

loc_0004FE31: ;
    RECOMP_PC(0x0004FE31u);
    PUSH32(esp, edi);

loc_0004FE32: ;
    RECOMP_PC(0x0004FE32u);
    edx = MEM32(esp + 0x34);
    eax = ZX8(MEM8(esi + edx));
    edi = esi + edx;
    eax = (uint32_t)((int32_t)eax * (int32_t)0x1C);
    edx = MEM32(ebx);
    fp_push(MEMF(eax + ebp + 0x14)); /* fld float */
    eax = esp + 0x14;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    ecx = esp + 0x1C;
    PUSH32(esp, ecx);
    eax = esp + 0x18;
    PUSH32(esp, eax);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x3C)); /* fmul dword ptr [esp + 0x3c] */
    PUSH32(esp, edi);
    ecx = ebx;
    MEMF(esp + 0x24) = (float)fp_top(); fp_pop(); /* fstp */
    { uint32_t _icall_target = MEM32(edx + 0x2C); PUSH32(esp, 0x0004FE63u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x0004FE60u); } /* indirect call */
    }

loc_0004FE63: ;
    RECOMP_PC(0x0004FE63u);
    ecx = MEM32(esp + 0x34);
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    SET_LO8(edx, MEM8(esi + ecx + 1));
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    SET_LO8(eax, MEM8(edi));
    ecx = ebp;
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    PUSH32(esp, 0x0004FE7Au); RECOMP_ABI_CALL(0x000EDB40u, sub_000EDB40); /* call 0x000EDB40 */

loc_0004FE7A: ;
    RECOMP_PC(0x0004FE7Au);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x2C)); /* fmul dword ptr [esp + 0x2c] */
    eax = MEM32(esp + 0x1C);
    esi++;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, eax (32-bit) */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esp + 0x10)); /* fadd dword ptr [esp + 0x10] */
    MEMF(esp + 0x10) = (float)fp_top(); fp_pop(); /* fstp */
    if (CMP_L(_fas, _fbs)) goto loc_0004FE32; /* jl: less (signed <) */

loc_0004FE8F: ;
    RECOMP_PC(0x0004FE8Fu);
    POP32(esp, edi);

loc_0004FE90: ;
    RECOMP_PC(0x0004FE90u);
    POP32(esp, esi);
    POP32(esp, ebp);

loc_0004FE92: ;
    RECOMP_PC(0x0004FE92u);
    POP32(esp, ebx);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 24; return; /* ret 20 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_00050010
 * Original: 0x00050010 - 0x000501D1 (449 bytes, 130 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00050010(void)
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

loc_00050010: ;
    RECOMP_PC(0x00050010u);
    PUSH32(esp, esi);
    esi = ecx;
    eax = MEM32(esi + 0x10);
    ecx = MEM32(eax + 0x54);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00050101; /* je: equal / zero */

loc_00050021: ;
    RECOMP_PC(0x00050021u);
    fp_push(MEMF(esi + 0x1C)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_0005003E; /* jne: not equal / not zero */

loc_00050031: ;
    RECOMP_PC(0x00050031u);
    fp_push(MEMF(esp + 0xC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esi + 0x1C)); /* fdiv dword ptr [esi + 0x1c] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x2C)); /* fadd dword ptr [esi + 0x2c] */
    MEMF(esi + 0x2C) = (float)fp_top(); fp_pop(); /* fstp */

loc_0005003E: ;
    RECOMP_PC(0x0005003Eu);
    fp_push(MEMF(esi + 0x2C)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91D0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91d0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 1 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_00050062; /* je: equal / zero */

loc_0005004E: ;
    RECOMP_PC(0x0005004Eu);
    fp_push(MEMF(esi + 0x1C)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0005015C; /* jp: parity */

loc_00050062: ;
    RECOMP_PC(0x00050062u);
    fp_push(MEMF(esp + 0xC)); /* fld float */
    ecx = 0x3F800000;
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esi + 0x14)); /* fdiv dword ptr [esi + 0x14] */
    MEM32(esi + 0x2C) = ecx;
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x28)); /* fadd dword ptr [esi + 0x28] */
    MEMF(esi + 0x28) = (float)fp_top(); /* fst */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91D0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91d0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 1 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_0005015C; /* jne: not equal / not zero */

loc_00050088: ;
    RECOMP_PC(0x00050088u);
    fp_push(MEMF(esi + 0x20)); /* fld float */
    MEM32(esi + 0x28) = ecx;
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_000500A8; /* jne: not equal / not zero */

loc_0005009B: ;
    RECOMP_PC(0x0005009Bu);
    fp_push(MEMF(esp + 0xC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esi + 0x20)); /* fdiv dword ptr [esi + 0x20] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x30)); /* fadd dword ptr [esi + 0x30] */
    MEMF(esi + 0x30) = (float)fp_top(); fp_pop(); /* fstp */

loc_000500A8: ;
    RECOMP_PC(0x000500A8u);
    fp_push(MEMF(esi + 0x30)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91D0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91d0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 1 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_000500CC; /* je: equal / zero */

loc_000500B8: ;
    RECOMP_PC(0x000500B8u);
    fp_push(MEMF(esi + 0x20)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0005015C; /* jp: parity */

loc_000500CC: ;
    RECOMP_PC(0x000500CCu);
    PUSH32(esp, 0x000500D1u); RECOMP_ABI_CALL(0x000474A0u, sub_000474A0); /* call 0x000474A0 */

loc_000500D1: ;
    RECOMP_PC(0x000500D1u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000501C1; /* je: equal / zero */

loc_000500D9: ;
    RECOMP_PC(0x000500D9u);
    PUSH32(esp, 0x000500DEu); RECOMP_ABI_CALL(0x000474A0u, sub_000474A0); /* call 0x000474A0 */

loc_000500DE: ;
    RECOMP_PC(0x000500DEu);
    fp_push(MEMF(eax + 0xC4)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1AF31C)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1af31c] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0005015C; /* jp: parity */

loc_000500F1: ;
    RECOMP_PC(0x000500F1u);
    ecx = MEM32(0x1D8264);
    eax = MEM32(esp + 8);
    MEM32(eax) = ecx;
    POP32(esp, esi);
    esp += 12; return; /* ret 8 */

loc_00050101: ;
    RECOMP_PC(0x00050101u);
    fp_push(MEMF(esi + 0x24)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_00050179; /* jne: not equal / not zero */

loc_00050111: ;
    RECOMP_PC(0x00050111u);
    fp_push(MEMF(esi + 0x34)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0005016C; /* jp: parity */

loc_00050121: ;
    RECOMP_PC(0x00050121u);
    PUSH32(esp, 0x00050126u); RECOMP_ABI_CALL(0x000474A0u, sub_000474A0); /* call 0x000474A0 */

loc_00050126: ;
    RECOMP_PC(0x00050126u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0005016C; /* je: equal / zero */

loc_0005012A: ;
    RECOMP_PC(0x0005012Au);
    fp_push(MEMF(esi + 0x38)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1B51C4)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1b51c4] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0005016C; /* jp: parity */

loc_0005013A: ;
    RECOMP_PC(0x0005013Au);
    PUSH32(esp, 0x0005013Fu); RECOMP_ABI_CALL(0x000474A0u, sub_000474A0); /* call 0x000474A0 */

loc_0005013F: ;
    RECOMP_PC(0x0005013Fu);
    fp_push(MEMF(eax + 0xC4)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0005016C; /* jp: parity */

loc_00050152: ;
    RECOMP_PC(0x00050152u);
    fp_push(MEMF(esp + 0xC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x38)); /* fadd dword ptr [esi + 0x38] */
    MEMF(esi + 0x38) = (float)fp_top(); fp_pop(); /* fstp */

loc_0005015C: ;
    RECOMP_PC(0x0005015Cu);
    ecx = MEM32(0x1CAD18);
    eax = MEM32(esp + 8);
    MEM32(eax) = ecx;
    POP32(esp, esi);
    esp += 12; return; /* ret 8 */

loc_0005016C: ;
    RECOMP_PC(0x0005016Cu);
    fp_push(MEMF(esp + 0xC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esi + 0x24)); /* fdiv dword ptr [esi + 0x24] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x34)); /* fadd dword ptr [esi + 0x34] */
    MEMF(esi + 0x34) = (float)fp_top(); fp_pop(); /* fstp */

loc_00050179: ;
    RECOMP_PC(0x00050179u);
    fp_push(MEMF(esi + 0x34)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91D0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91d0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 1 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_00050199; /* je: equal / zero */

loc_00050189: ;
    RECOMP_PC(0x00050189u);
    fp_push(MEMF(esi + 0x24)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0005015C; /* jp: parity */

loc_00050199: ;
    RECOMP_PC(0x00050199u);
    fp_push(MEMF(esp + 0xC)); /* fld float */
    MEM32(esi + 0x34) = 0x3F800000;
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esi + 0x18)); /* fdiv dword ptr [esi + 0x18] */
    fp_top() = RECOMP_FP_PC(MEMF(esi + 0x28) - fp_top()); /* fsubr dword ptr [esi + 0x28] */
    MEMF(esi + 0x28) = (float)fp_top(); /* fst */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91E0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1c91e0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0005015C; /* jp: parity */

loc_000501BA: ;
    RECOMP_PC(0x000501BAu);
    MEM32(esi + 0x28) = 0;

loc_000501C1: ;
    RECOMP_PC(0x000501C1u);
    eax = MEM32(esp + 8);
    edx = MEM32(0x1D8264);
    MEM32(eax) = edx;
    POP32(esp, esi);
    esp += 12; return; /* ret 8 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_000A4070
 * Original: 0x000A4070 - 0x000A415F (239 bytes, 82 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000A4070(void)
{
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;

loc_000A4070: ;
    RECOMP_PC(0x000A4070u);
    PUSH32(esp, ebx);
    ebx = MEM32(esp + 0xC);
    PUSH32(esp, esi);
    esi = MEM32(esp + 0xC);
    PUSH32(esp, edi);
    edi = ecx;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x1CE244);
    ecx = edi + 4;
    PUSH32(esp, ecx);
    MEM32(edi + 0x48) = ebx;
    eax = MEM32(esi);
    PUSH32(esp, 0x1BEE94);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0xAC); PUSH32(esp, 0x000A4098u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000A4092u); } /* indirect call */
    }

loc_000A4098: ;
    RECOMP_PC(0x000A4098u);
    edx = MEM32(esi);
    eax = edi + 8;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1BEE84);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x64); PUSH32(esp, 0x000A40A8u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000A40A5u); } /* indirect call */
    }

loc_000A40A8: ;
    RECOMP_PC(0x000A40A8u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 7);
    PUSH32(esp, 1);
    eax = edi + 0x18;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1BEE74);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x8C); PUSH32(esp, 0x000A40BFu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000A40B9u); } /* indirect call */
    }

loc_000A40BF: ;
    RECOMP_PC(0x000A40BFu);
    edx = MEM32(esi);
    eax = edi + 0x1C;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1BEE68);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x64); PUSH32(esp, 0x000A40CFu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000A40CCu); } /* indirect call */
    }

loc_000A40CF: ;
    RECOMP_PC(0x000A40CFu);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x7FFFFFFF);
    PUSH32(esp, 0x80000000u);
    eax = edi + 0x30;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1BEE60);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x8C); PUSH32(esp, 0x000A40ECu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000A40E6u); } /* indirect call */
    }

loc_000A40EC: ;
    RECOMP_PC(0x000A40ECu);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x7FFFFFFF);
    PUSH32(esp, 0x80000000u);
    eax = edi + 0x2C;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1BEE58);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x8C); PUSH32(esp, 0x000A4109u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000A4103u); } /* indirect call */
    }

loc_000A4109: ;
    RECOMP_PC(0x000A4109u);
    edx = MEM32(esi);
    eax = edi + 0x38;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1BEE4C);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x64); PUSH32(esp, 0x000A4119u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000A4116u); } /* indirect call */
    }

loc_000A4119: ;
    RECOMP_PC(0x000A4119u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x7FFFFFFF);
    PUSH32(esp, 0x80000000u);
    eax = edi + 0x34;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1BEE44);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x8C); PUSH32(esp, 0x000A4136u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000A4130u); } /* indirect call */
    }

loc_000A4136: ;
    RECOMP_PC(0x000A4136u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x1BEE34);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0xC); PUSH32(esp, 0x000A4142u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000A413Fu); } /* indirect call */
    }

loc_000A4142: ;
    RECOMP_PC(0x000A4142u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000A4159; /* je: equal / zero */

loc_000A4146: ;
    RECOMP_PC(0x000A4146u);
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    ecx = edi + 0x4C;
    PUSH32(esp, 0x000A4150u); RECOMP_ABI_CALL(0x000A3EC0u, sub_000A3EC0); /* call 0x000A3EC0 */

loc_000A4150: ;
    RECOMP_PC(0x000A4150u);
    eax = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0x10); PUSH32(esp, 0x000A4159u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000A4156u); } /* indirect call */
    }

loc_000A4159: ;
    RECOMP_PC(0x000A4159u);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp += 12; return; /* ret 8 */

}


/**
 * qpr2_recovered_000A4DE0
 * Original: 0x000A4DE0 - 0x000A51C4 (996 bytes, 273 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000A4DE0(void)
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

loc_000A4DE0: ;
    RECOMP_PC(0x000A4DE0u);
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x1C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edi);
    edi = MEM32(esp + 0x2C);
    eax = MEM32(edi);
    esi = ecx;
    PUSH32(esp, 0x1CE244);
    ecx = esi + 0xBC;
    PUSH32(esp, ecx);
    PUSH32(esp, 0x1BEE94);
    ecx = edi;
    { uint32_t _icall_target = MEM32(eax + 0xAC); PUSH32(esp, 0x000A4E07u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000A4E01u); } /* indirect call */
    }

loc_000A4E07: ;
    RECOMP_PC(0x000A4E07u);
    edx = MEM32(edi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x1BEE34);
    ecx = edi;
    { uint32_t _icall_target = MEM32(edx + 0xC); PUSH32(esp, 0x000A4E13u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000A4E10u); } /* indirect call */
    }

loc_000A4E13: ;
    RECOMP_PC(0x000A4E13u);
    ebp = 0; /* xor self */
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000A4E2D; /* je: equal / zero */

loc_000A4E19: ;
    RECOMP_PC(0x000A4E19u);
    PUSH32(esp, edi);
    ecx = esi + 0xCC;
    PUSH32(esp, 0x000A4E25u); RECOMP_ABI_CALL(0x000A48F0u, sub_000A48F0); /* call 0x000A48F0 */

loc_000A4E25: ;
    RECOMP_PC(0x000A4E25u);
    eax = MEM32(edi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebp);
    ecx = edi;
    { uint32_t _icall_target = MEM32(eax + 0x10); PUSH32(esp, 0x000A4E2Du); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000A4E2Au); } /* indirect call */
    }

loc_000A4E2D: ;
    RECOMP_PC(0x000A4E2Du);
    edx = MEM32(0x1CE4B8);
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edx, ebp (32-bit) */
    PUSH32(esp, ebx);
    if (CMP_BE(_fa, _fb)) goto loc_000A4E52; /* jbe: below or equal (unsigned <=) */

loc_000A4E3A: ;
    RECOMP_PC(0x000A4E3Au);
    ecx = MEM32(0x1CE4B4);

loc_000A4E40: ;
    RECOMP_PC(0x000A4E40u);
    edi = MEM32(esp + 0x34);
    _fa = (uint32_t)(MEM32(ecx + eax * 8)) & 0xFFFFFFFFu; _fb = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(ecx + eax * 8), edi (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000A51B4; /* je: equal / zero */

loc_000A4E4D: ;
    RECOMP_PC(0x000A4E4Du);
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edx (32-bit) */
    if (CMP_B(_fa, _fb)) goto loc_000A4E40; /* jb: below (unsigned <) */

loc_000A4E52: ;
    RECOMP_PC(0x000A4E52u);
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_000A4E54: ;
    RECOMP_PC(0x000A4E54u);
    ecx = MEM32(eax + 4);
    edi = esi + 0x18;
    PUSH32(esp, ecx);
    ecx = edi;
    PUSH32(esp, 0x000A4E62u); RECOMP_ABI_CALL(0x000E18A0u, sub_000E18A0); /* call 0x000E18A0 */

loc_000A4E62: ;
    RECOMP_PC(0x000A4E62u);
    ebx = esi + 8;
    PUSH32(esp, 0x1BF13C);
    ecx = ebx;
    PUSH32(esp, 0x000A4E71u); RECOMP_ABI_CALL(0x000E18A0u, sub_000E18A0); /* call 0x000E18A0 */

loc_000A4E71: ;
    RECOMP_PC(0x000A4E71u);
    edx = MEM32(edi);
    PUSH32(esp, edx);
    ecx = ebx;
    PUSH32(esp, 0x000A4E7Bu); RECOMP_ABI_CALL(0x000E1920u, sub_000E1920); /* call 0x000E1920 */

loc_000A4E7B: ;
    RECOMP_PC(0x000A4E7Bu);
    PUSH32(esp, 0x1BF11C);
    ecx = esi + 0x38;
    PUSH32(esp, 0x000A4E88u); RECOMP_ABI_CALL(0x000E18A0u, sub_000E18A0); /* call 0x000E18A0 */

loc_000A4E88: ;
    RECOMP_PC(0x000A4E88u);
    PUSH32(esp, 0x1BF13C);
    ecx = esp + 0x20;
    PUSH32(esp, 0x000A4E96u); RECOMP_ABI_CALL(0x000E1740u, sub_000E1740); /* call 0x000E1740 */

loc_000A4E96: ;
    RECOMP_PC(0x000A4E96u);
    eax = MEM32(edi);
    PUSH32(esp, eax);
    ecx = esp + 0x20;
    PUSH32(esp, 0x000A4EA2u); RECOMP_ABI_CALL(0x000E1920u, sub_000E1920); /* call 0x000E1920 */

loc_000A4EA2: ;
    RECOMP_PC(0x000A4EA2u);
    PUSH32(esp, 0x1BF118);
    ecx = esp + 0x20;
    PUSH32(esp, 0x000A4EB0u); RECOMP_ABI_CALL(0x000E1920u, sub_000E1920); /* call 0x000E1920 */

loc_000A4EB0: ;
    RECOMP_PC(0x000A4EB0u);
    ecx = MEM32(edi);
    PUSH32(esp, ecx);
    ecx = esp + 0x20;
    PUSH32(esp, 0x000A4EBCu); RECOMP_ABI_CALL(0x000E1920u, sub_000E1920); /* call 0x000E1920 */

loc_000A4EBC: ;
    RECOMP_PC(0x000A4EBCu);
    PUSH32(esp, 0x1BF110);
    ecx = esp + 0x20;
    PUSH32(esp, 0x000A4ECAu); RECOMP_ABI_CALL(0x000E1920u, sub_000E1920); /* call 0x000E1920 */

loc_000A4ECA: ;
    RECOMP_PC(0x000A4ECAu);
    edx = MEM32(esp + 0x1C);
    ecx = MEM32(esi + 4);
    PUSH32(esp, edx);
    PUSH32(esp, 0x000A4ED7u); RECOMP_ABI_CALL(0x00100C30u, sub_00100C30); /* call 0x00100C30 */

loc_000A4ED7: ;
    RECOMP_PC(0x000A4ED7u);
    ecx = MEM32(esi + 4);
    PUSH32(esp, 0x1BF100);
    PUSH32(esp, 0x000A4EE4u); RECOMP_ABI_CALL(0x00100BE0u, sub_00100BE0); /* call 0x00100BE0 */

loc_000A4EE4: ;
    RECOMP_PC(0x000A4EE4u);
    edi = eax;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, ebp (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000A4F4B; /* je: equal / zero */

loc_000A4EEA: ;
    RECOMP_PC(0x000A4EEAu);
    ecx = MEM32(esi + 4);
    PUSH32(esp, 0x1AF1C4);
    PUSH32(esp, 0x000A4EF7u); RECOMP_ABI_CALL(0x00100BE0u, sub_00100BE0); /* call 0x00100BE0 */

loc_000A4EF7: ;
    RECOMP_PC(0x000A4EF7u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebp (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000A4F4B; /* je: equal / zero */

loc_000A4EFB: ;
    RECOMP_PC(0x000A4EFBu);
    fp_push(MEMF(edi + 0x20)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(eax + 0x20)); /* fsub dword ptr [eax + 0x20] */
    fp_push(MEMF(edi + 0x24)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(eax + 0x24)); /* fsub dword ptr [eax + 0x24] */
    MEMF(esp + 0x14) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(edi + 0x28)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(eax + 0x28)); /* fsub dword ptr [eax + 0x28] */
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x1B6300)); /* fadd dword ptr [0x1b6300] */
    fp_push(MEMF(esp + 0x14)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x1AF3A0)); /* fadd dword ptr [0x1af3a0] */
    fp_push(MEMF(esp + 0x18)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1B6300)); /* fsub dword ptr [0x1b6300] */
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esp + 0x18);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(esi + 0xC0) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0xC8) = eax;
    MEMF(esi + 0xC4) = (float)fp_top(); fp_pop(); /* fstp */

loc_000A4F4B: ;
    RECOMP_PC(0x000A4F4Bu);
    MEM32(esi + 0x54) = ebp;
    MEM32(esi + 0x58) = 0xBED70A3Du;
    MEM32(esi + 0x5C) = ebp;
    ecx = MEM32(esi + 4);
    PUSH32(esp, 0x1BF0E8);
    PUSH32(esp, 0x000A4F65u); RECOMP_ABI_CALL(0x00100BE0u, sub_00100BE0); /* call 0x00100BE0 */

loc_000A4F65: ;
    RECOMP_PC(0x000A4F65u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebp (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000A4F8E; /* je: equal / zero */

loc_000A4F69: ;
    RECOMP_PC(0x000A4F69u);
    fp_push(MEMF(eax + 0x20)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x54)); /* fadd dword ptr [esi + 0x54] */
    fp_push(MEMF(esi + 0x58)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 0x24)); /* fadd dword ptr [eax + 0x24] */
    fp_push(MEMF(esi + 0x5C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 0x28)); /* fadd dword ptr [eax + 0x28] */
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = MEM32(esp + 0x18);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(esi + 0x60) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0x68) = ecx;
    MEMF(esi + 0x64) = (float)fp_top(); fp_pop(); /* fstp */

loc_000A4F8E: ;
    RECOMP_PC(0x000A4F8Eu);
    ecx = MEM32(esi + 4);
    PUSH32(esp, 0x1BF0D0);
    PUSH32(esp, 0x000A4F9Bu); RECOMP_ABI_CALL(0x00100BE0u, sub_00100BE0); /* call 0x00100BE0 */

loc_000A4F9B: ;
    RECOMP_PC(0x000A4F9Bu);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebp (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000A4FC4; /* je: equal / zero */

loc_000A4F9F: ;
    RECOMP_PC(0x000A4F9Fu);
    fp_push(MEMF(eax + 0x20)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x54)); /* fadd dword ptr [esi + 0x54] */
    fp_push(MEMF(eax + 0x24)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x58)); /* fadd dword ptr [esi + 0x58] */
    fp_push(MEMF(eax + 0x28)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x5C)); /* fadd dword ptr [esi + 0x5c] */
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(esp + 0x18);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(esi + 0x6C) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0x74) = edx;
    MEMF(esi + 0x70) = (float)fp_top(); fp_pop(); /* fstp */

loc_000A4FC4: ;
    RECOMP_PC(0x000A4FC4u);
    MEM32(esi + 0x78) = ebp;
    MEM32(esi + 0x7C) = ebp;
    MEM32(esi + 0x80) = ebp;
    ecx = MEM32(esi + 4);
    PUSH32(esp, 0x1BF0C0);
    PUSH32(esp, 0x000A4FDDu); RECOMP_ABI_CALL(0x00100BE0u, sub_00100BE0); /* call 0x00100BE0 */

loc_000A4FDD: ;
    RECOMP_PC(0x000A4FDDu);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebp (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000A5009; /* je: equal / zero */

loc_000A4FE1: ;
    RECOMP_PC(0x000A4FE1u);
    fp_push(MEMF(esi + 0x54)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 0x20)); /* fadd dword ptr [eax + 0x20] */
    fp_push(MEMF(esi + 0x58)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 0x24)); /* fadd dword ptr [eax + 0x24] */
    fp_push(MEMF(esi + 0x5C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 0x28)); /* fadd dword ptr [eax + 0x28] */
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esp + 0x18);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(esi + 0x78) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0x80) = eax;
    MEMF(esi + 0x7C) = (float)fp_top(); fp_pop(); /* fstp */

loc_000A5009: ;
    RECOMP_PC(0x000A5009u);
    MEM32(esi + 0x84) = ebp;
    MEM32(esi + 0x88) = ebp;
    MEM32(esi + 0x8C) = ebp;
    ecx = MEM32(esi + 4);
    PUSH32(esp, 0x1BF0B0);
    PUSH32(esp, 0x000A5028u); RECOMP_ABI_CALL(0x00100BE0u, sub_00100BE0); /* call 0x00100BE0 */

loc_000A5028: ;
    RECOMP_PC(0x000A5028u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebp (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000A505A; /* je: equal / zero */

loc_000A502C: ;
    RECOMP_PC(0x000A502Cu);
    fp_push(MEMF(esi + 0x54)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 0x20)); /* fadd dword ptr [eax + 0x20] */
    fp_push(MEMF(esi + 0x58)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 0x24)); /* fadd dword ptr [eax + 0x24] */
    fp_push(MEMF(esi + 0x5C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 0x28)); /* fadd dword ptr [eax + 0x28] */
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = MEM32(esp + 0x18);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(esi + 0x84) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0x8C) = ecx;
    MEMF(esi + 0x88) = (float)fp_top(); fp_pop(); /* fstp */

loc_000A505A: ;
    RECOMP_PC(0x000A505Au);
    MEM32(esi + 0x90) = ebp;
    MEM32(esi + 0x94) = ebp;
    MEM32(esi + 0x98) = ebp;
    ecx = MEM32(esi + 4);
    PUSH32(esp, 0x1BF0A0);
    PUSH32(esp, 0x000A5079u); RECOMP_ABI_CALL(0x00100BE0u, sub_00100BE0); /* call 0x00100BE0 */

loc_000A5079: ;
    RECOMP_PC(0x000A5079u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebp (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000A50AB; /* je: equal / zero */

loc_000A507D: ;
    RECOMP_PC(0x000A507Du);
    fp_push(MEMF(esi + 0x54)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 0x20)); /* fadd dword ptr [eax + 0x20] */
    fp_push(MEMF(esi + 0x58)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 0x24)); /* fadd dword ptr [eax + 0x24] */
    fp_push(MEMF(esi + 0x5C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 0x28)); /* fadd dword ptr [eax + 0x28] */
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(esp + 0x18);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(esi + 0x90) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0x98) = edx;
    MEMF(esi + 0x94) = (float)fp_top(); fp_pop(); /* fstp */

loc_000A50AB: ;
    RECOMP_PC(0x000A50ABu);
    MEM32(esi + 0x48) = ebp;
    MEM32(esi + 0x4C) = 0x3F851EB8;
    MEM32(esi + 0x50) = 0xBE947AE1u;
    fp_push(MEMF(esi + 0x54)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x48)); /* fadd dword ptr [esi + 0x48] */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    fp_push(MEMF(esi + 0x58)); /* fld float */
    ebx = esi + 0xE0;
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x4C)); /* fadd dword ptr [esi + 0x4c] */
    fp_push(MEMF(esi + 0x5C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x50)); /* fadd dword ptr [esi + 0x50] */
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esp + 0x18);
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    MEMF(esi + 0x48) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0x50) = eax;
    eax = 0x41A00000;
    MEMF(esi + 0x4C) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esi + 0x9C) = eax;
    MEM32(esi + 0xC0) = ebp;
    MEM32(esi + 0xC4) = ebp;
    MEM32(esi + 0xC8) = ebp;
    MEM32(esi + 0xA0) = 0x43020000;
    MEM32(esi + 0xA4) = eax;
    MEM32(esi + 0xA8) = 0x42F00000;
    MEM32(esi + 0xAC) = 0x41B00000;
    MEM32(esi + 0xB0) = 0xBF472B02u;
    MEM32(esi + 0xB4) = 0x3FAA7EFA;
    MEM32(esi + 0xB8) = 0x3F2978D5;

loc_000A5148: ;
    RECOMP_PC(0x000A5148u);
    ecx = MEM32(esi + 4);
    edx = MEM32(esp + 0x30);
    PUSH32(esp, ecx);
    PUSH32(esp, edi);
    PUSH32(esp, edx);
    ecx = ebx;
    PUSH32(esp, 0x000A5159u); RECOMP_ABI_CALL(0x000A4C00u, sub_000A4C00); /* call 0x000A4C00 */

loc_000A5159: ;
    RECOMP_PC(0x000A5159u);
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(0x98) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebx = ebx + 0x98;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(6) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 6 (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_000A5148; /* jl: less (signed <) */

loc_000A5165: ;
    RECOMP_PC(0x000A5165u);
    eax = MEM32(esp + 0x34);
    _fb = (uint32_t)(2) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    eax = eax - 2;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    ecx = 1;
    MEM32(esi + 0x470) = ebp;
    MEM32(esi + 0x474) = ecx;
    POP32(esp, ebx);
    if ((_fa == 0)) goto loc_000A518A; /* je: equal / zero */

loc_000A5180: ;
    RECOMP_PC(0x000A5180u);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    eax = eax - 4;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    if ((_fa == 0)) goto loc_000A51BC; /* je: equal / zero */

loc_000A5185: ;
    RECOMP_PC(0x000A5185u);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    eax = eax - 4;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    if ((_fa != 0)) goto loc_000A5196; /* jne: not equal / not zero */

loc_000A518A: ;
    RECOMP_PC(0x000A518Au);
    MEM32(esi + 0x178) = ebp;
    MEM32(esi + 0x210) = ebp;

loc_000A5196: ;
    RECOMP_PC(0x000A5196u);
    eax = MEM32(esp + 0x18);
    POP32(esp, edi);
    POP32(esp, esi);
    MEM32(esp + 0x1C) = ebp;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebp (32-bit) */
    POP32(esp, ebp);
    if (CMP_EQ(_fa, _fb)) goto loc_000A51AE; /* je: equal / zero */

loc_000A51A5: ;
    RECOMP_PC(0x000A51A5u);
    PUSH32(esp, eax);
    PUSH32(esp, 0x000A51ABu); RECOMP_ABI_CALL(0x0012B21Cu, sub_0012B21C); /* call 0x0012B21C */

loc_000A51AB: ;
    RECOMP_PC(0x000A51ABu);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */

loc_000A51AE: ;
    RECOMP_PC(0x000A51AEu);
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x1C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 12; return; /* ret 8 */

loc_000A51B4: ;
    RECOMP_PC(0x000A51B4u);
    eax = ecx + eax * 8;
    goto loc_000A4E54;

loc_000A51BC: ;
    RECOMP_PC(0x000A51BCu);
    MEM32(esi + 0x470) = ecx;
    goto loc_000A5196;

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_000EA920
 * Original: 0x000EA920 - 0x000EB26B (2379 bytes, 736 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000EA920(void)
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

loc_000EA920: ;
    RECOMP_PC(0x000EA920u);
    _fb = (uint32_t)(0x114) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x114;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    edx = MEM32(0x1DE80C);
    edx = MEM32(edx + 4);
    PUSH32(esp, ebx);
    ebx = ecx;
    ecx = MEM32(ebx + 0x10);
    ecx = MEM32(edx + ecx * 4);
    edx = ZX8(MEM8(ecx + 1));
    ecx = MEM32(ebx + 0xC);
    PUSH32(esp, ebp);
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    MEM32(esp + 0x90) = edx;
    _fa = (uint32_t)(MEM32(ecx + 8)) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(ecx + 8), eax (32-bit) */
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    MEM32(esp + 0x24) = ebx;
    MEM32(esp + 0x6C) = eax;
    MEM32(esp + 0x1C) = eax;
    if (CMP_BE(_fa, _fb)) goto loc_000EA96E; /* jbe: below or equal (unsigned <=) */

loc_000EA95C: ;
    RECOMP_PC(0x000EA95Cu);
    edx = ecx;
    ecx = MEM32(edx + 8);

loc_000EA961: ;
    RECOMP_PC(0x000EA961u);
    MEM8(esp + eax + 0xAC) = 0;
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ecx (32-bit) */
    if (CMP_B(_fa, _fb)) goto loc_000EA961; /* jb: below (unsigned <) */

loc_000EA96E: ;
    RECOMP_PC(0x000EA96Eu);
    ebp = MEM32(ebx + 4);
    MEM32(esp + 0x94) = 0xA;
    /* nop */

loc_000EA980: ;
    RECOMP_PC(0x000EA980u);
    SET_LO8(eax, MEM8(0x1C3E5C));
    _fa = (uint32_t)(MEM8(ebp)) & 0xFFu; _fb = (uint32_t)(LO8(eax)) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp MEM8(ebp), LO8(eax) (8-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000EB1C7; /* je: equal / zero */

loc_000EA98E: ;
    RECOMP_PC(0x000EA98Eu);
    _fa = (uint32_t)(MEM8(ebp + 6)) & 0xFFu; _fb = (uint32_t)(4) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test MEM8(ebp + 6), 4 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_000EA9A1; /* jne: not equal / not zero */

loc_000EA994: ;
    RECOMP_PC(0x000EA994u);
    fp_push(MEMF(esp + 0x128)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebp + 0x20)); /* fadd dword ptr [ebp + 0x20] */
    MEMF(ebp + 0x20) = (float)fp_top(); fp_pop(); /* fstp */

loc_000EA9A1: ;
    RECOMP_PC(0x000EA9A1u);
    SET_LO8(ecx, MEM8(ebp + 6));
    fp_push(MEMF(ebp + 0xC)); /* fld float */
    _fa = (uint32_t)(LO8(ecx)) & 0xFFu; _fb = (uint32_t)(8) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test LO8(ecx), 8 (8-bit) */
    fp_top() = RECOMP_FP_PC(MEMF(ebp + 0x20) / fp_top()); /* fdivr dword ptr [ebp + 0x20] */
    MEM8(esp + 0x13) = LO8(ecx);
    if (TEST_Z(_fa, _fb)) goto loc_000EAAA6; /* je: equal / zero */

loc_000EA9B7: ;
    RECOMP_PC(0x000EA9B7u);
    _fa = (uint32_t)(MEM32(ebx + 0x14)) & 0xFFFFFFFFu; _fb = (uint32_t)(2) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(ebx + 0x14), 2 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000EAA3B; /* jne: not equal / not zero */

loc_000EA9C1: ;
    RECOMP_PC(0x000EA9C1u);
    eax = MEM32(ebx + 0xC);
    fp_pop(); /* fstp st(0) */
    edx = MEM32(eax + 8);
    fp_push(MEMF(0x1C91E0)); /* fld float */
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edx, 0 (32-bit) */
    if (CMP_BE(_fa, _fb)) goto loc_000EAA2C; /* jbe: below or equal (unsigned <=) */

loc_000EA9D5: ;
    RECOMP_PC(0x000EA9D5u);
    edx = MEM32(eax + 0x14);
    ecx = eax;
    edi = MEM32(ecx + 8);
    ecx = MEM32(ebx + 0x18);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ecx = ecx + 0x10;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edx = edx + 0x10;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */

loc_000EA9E6: ;
    RECOMP_PC(0x000EA9E6u);
    fp_push(MEMF(edx + -4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ecx + -4)); /* fmul dword ptr [ecx - 4] */
    fp_push(MEMF(edx + 4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ecx + 4)); /* fmul dword ptr [ecx + 4] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    fp_push(MEMF(edx)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ecx)); /* fmul dword ptr [ecx] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    fp_push(MEMF(edx + 8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ecx + 8)); /* fmul dword ptr [ecx + 8] */
    fp_st1() = RECOMP_FP_PC(fp_st1() + fp_top()); fp_pop(); /* faddp st(1) */
    fp_top() = fabs(fp_top()); /* fabs */
    fp_top() = RECOMP_FP_PC(MEMF(0x1C91D0) - fp_top()); /* fsubr dword ptr [0x1c91d0] */
    MEMF(esp + 0x14) = (float)fp_top(); /* fst */
    g_fp_cmp = RECOMP_FCMP(fp_top(), fp_st1()); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp st(1) */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_000EAA1D; /* jne: not equal / not zero */

loc_000EAA17: ;
    RECOMP_PC(0x000EAA17u);
    fp_pop(); /* fstp st(0) */
    fp_push(MEMF(esp + 0x14)); /* fld float */

loc_000EAA1D: ;
    RECOMP_PC(0x000EAA1Du);
    esi++;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ecx = ecx + 0x1C;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edx = edx + 0x1C;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, edi (32-bit) */
    if (CMP_B(_fa, _fb)) goto loc_000EA9E6; /* jb: below (unsigned <) */

loc_000EAA28: ;
    RECOMP_PC(0x000EAA28u);
    SET_LO8(ecx, MEM8(esp + 0x13));

loc_000EAA2C: ;
    RECOMP_PC(0x000EAA2Cu);
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(ebp + 0x10)); /* fdiv dword ptr [ebp + 0x10] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1C3E54)); /* fmul dword ptr [0x1c3e54] */
    MEMF(ebp + 0xC) = (float)fp_top(); /* fst */
    fp_top() = RECOMP_FP_PC(MEMF(ebp + 0x20) / fp_top()); /* fdivr dword ptr [ebp + 0x20] */

loc_000EAA3B: ;
    RECOMP_PC(0x000EAA3Bu);
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1C91D0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [0x1c91d0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_000EAA9D; /* jne: not equal / not zero */

loc_000EAA48: ;
    RECOMP_PC(0x000EAA48u);
    MEM32(ebx + 0x14) = 0;
    fp_pop(); /* fstp st(0) */
    _fa = (uint32_t)(MEM8(ebp + 6)) & 0xFFu; _fb = (uint32_t)(4) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test MEM8(ebp + 6), 4 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_000EAA5A; /* jne: not equal / not zero */

loc_000EAA57: ;
    RECOMP_PC(0x000EAA57u);
    MEM32(ebx + 8) = MEM32(ebx + 8) - 1;
    _fa = (uint32_t)(MEM32(ebx + 8)) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */

loc_000EAA5A: ;
    RECOMP_PC(0x000EAA5Au);
    eax = MEM32(ebp + 0x14);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    SET_LO8(edx, MEM8(0x1C3E5C));
    if (CMP_EQ(_fa, _fb)) goto loc_000EAA91; /* je: equal / zero */

loc_000EAA67: ;
    RECOMP_PC(0x000EAA67u);
    ecx = 0xA;
    esi = ebp;
    edi = esp + 0xD4;
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
    ecx = esp + 0xD4;
    MEM8(ebp) = LO8(edx);
    MEM8(ebp + 1) = 0xFF;
    edx = MEM32(eax);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    ecx = eax;
    { uint32_t _icall_target = MEM32(edx); PUSH32(esp, 0x000EAA8Cu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000EAA8Au); } /* indirect call */
    }

loc_000EAA8C: ;
    RECOMP_PC(0x000EAA8Cu);
    goto loc_000EB1C7;

loc_000EAA91: ;
    RECOMP_PC(0x000EAA91u);
    MEM8(ebp) = LO8(edx);
    MEM8(ebp + 1) = 0xFF;
    goto loc_000EB1C7;

loc_000EAA9D: ;
    RECOMP_PC(0x000EAA9Du);
    MEMF(esp + 0x68) = (float)fp_top(); fp_pop(); /* fstp */
    goto loc_000EAB7E;

loc_000EAAA6: ;
    RECOMP_PC(0x000EAAA6u);
    _fa = (uint32_t)(LO8(ecx)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test LO8(ecx), 1 (8-bit) */
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    if (TEST_Z(_fa, _fb)) goto loc_000EAAEF; /* je: equal / zero */

loc_000EAAAD: ;
    RECOMP_PC(0x000EAAADu);
    SET_LO16(edi, MEM16(ebp + 4));
    esi = ZX16(LO16(edi));
    PUSH32(esp, 0x000EAAB9u); RECOMP_ABI_CALL(0x0012B278u, sub_0012B278); /* call 0x0012B278 */

loc_000EAAB9: ;
    RECOMP_PC(0x000EAAB9u);
    _fb = (uint32_t)(eax) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esi = esi - eax;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    eax = ZX16(MEM16(ebp + 2));
    ecx = esi + -1;
    MEM32(esp + 0x14) = eax;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, eax (32-bit) */
    MEM32(esp + 0x6C) = esi;
    MEM32(esp + 0x1C) = ecx;
    eax = esp + 0x14;
    if (CMP_L(_fas, _fbs)) goto loc_000EAADA; /* jl: less (signed <) */

loc_000EAAD6: ;
    RECOMP_PC(0x000EAAD6u);
    eax = esp + 0x1C;

loc_000EAADA: ;
    RECOMP_PC(0x000EAADAu);
    eax = MEM32(eax);
    ecx = ZX16(LO16(edi));
    _fb = (uint32_t)(esi) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    ecx = ecx - esi;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    MEM32(esp + 0x14) = ecx;
    MEM32(esp + 0x1C) = eax;
    fp_push((double)SMEM32(esp + 0x14)); /* fild */
    goto loc_000EAB31;

loc_000EAAEF: ;
    RECOMP_PC(0x000EAAEFu);
    SET_LO16(edi, MEM16(ebp + 2));
    esi = ZX16(LO16(edi));
    PUSH32(esp, 0x000EAAFBu); RECOMP_ABI_CALL(0x0012B278u, sub_0012B278); /* call 0x0012B278 */

loc_000EAAFB: ;
    RECOMP_PC(0x000EAAFBu);
    _fb = (uint32_t)(eax) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + eax;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    eax = ZX16(MEM16(ebp + 4));
    ecx = esi + 1;
    MEM32(esp + 0x14) = eax;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, eax (32-bit) */
    MEM32(esp + 0x6C) = esi;
    MEM32(esp + 0x1C) = ecx;
    eax = esp + 0x14;
    if (CMP_G(_fas, _fbs)) goto loc_000EAB1C; /* jg: greater (signed >) */

loc_000EAB18: ;
    RECOMP_PC(0x000EAB18u);
    eax = esp + 0x1C;

loc_000EAB1C: ;
    RECOMP_PC(0x000EAB1Cu);
    edx = MEM32(eax);
    eax = ZX16(LO16(edi));
    ecx = esi;
    _fb = (uint32_t)(eax) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    ecx = ecx - eax;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    MEM32(esp + 0x14) = ecx;
    MEM32(esp + 0x1C) = edx;
    fp_push((double)SMEM32(esp + 0x14)); /* fild */

loc_000EAB31: ;
    RECOMP_PC(0x000EAB31u);
    ecx = MEM32(ebp + 0x18);
    fp_top() = RECOMP_FP_PC(fp_st1() - fp_top()); /* fsubr st(1) */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, 0 (32-bit) */
    MEMF(esp + 0x68) = (float)fp_top(); fp_pop(); /* fstp */
    fp_pop(); /* fstp st(0) */
    if (CMP_EQ(_fa, _fb)) goto loc_000EAB53; /* je: equal / zero */

loc_000EAB40: ;
    RECOMP_PC(0x000EAB40u);
    edx = (uint32_t)(int32_t)SMEM16(ebp + 0x24);
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, edx (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000EAB53; /* je: equal / zero */

loc_000EAB48: ;
    RECOMP_PC(0x000EAB48u);
    MEM16(ebp + 0x24) = LO16(esi);
    eax = MEM32(ecx);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebp);
    { uint32_t _icall_target = MEM32(eax); PUSH32(esp, 0x000EAB51u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000EAB4Fu); } /* indirect call */
    }

loc_000EAB51: ;
    RECOMP_PC(0x000EAB51u);
    goto loc_000EAB57;

loc_000EAB53: ;
    RECOMP_PC(0x000EAB53u);
    MEM16(ebp + 0x24) = LO16(esi);

loc_000EAB57: ;
    RECOMP_PC(0x000EAB57u);
    SET_LO8(eax, MEM8(ebp + 6));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(4) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test LO8(eax), 4 (8-bit) */
    MEM8(esp + 0x13) = LO8(eax);
    if (TEST_NZ(_fa, _fb)) goto loc_000EAB7A; /* jne: not equal / not zero */

loc_000EAB62: ;
    RECOMP_PC(0x000EAB62u);
    ecx = ebp;
    PUSH32(esp, 0x000EAB69u); RECOMP_ABI_CALL(0x000286D0u, sub_000286D0); /* call 0x000286D0 */

loc_000EAB69: ;
    RECOMP_PC(0x000EAB69u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000EAB7A; /* je: equal / zero */

loc_000EAB6D: ;
    RECOMP_PC(0x000EAB6Du);
    PUSH32(esp, ebp);
    ecx = ebx;
    PUSH32(esp, 0x000EAB75u); RECOMP_ABI_CALL(0x000EA590u, sub_000EA590); /* call 0x000EA590 */

loc_000EAB75: ;
    RECOMP_PC(0x000EAB75u);
    goto loc_000EB1C7;

loc_000EAB7A: ;
    RECOMP_PC(0x000EAB7Au);
    SET_LO8(ecx, MEM8(esp + 0x13));

loc_000EAB7E: ;
    RECOMP_PC(0x000EAB7Eu);
    SET_LO8(eax, MEM8(ebp));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0xFE) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0xFE (8-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000EAB8D; /* jne: not equal / not zero */

loc_000EAB85: ;
    RECOMP_PC(0x000EAB85u);
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    MEM32(esp + 0x14) = esi;
    goto loc_000EABA9;

loc_000EAB8D: ;
    RECOMP_PC(0x000EAB8Du);
    edx = SX8(LO8(eax));
    eax = MEM32(0x1DE808);
    edx = (uint32_t)((int32_t)edx * (int32_t)0x2C);
    eax = MEM32(eax + 4);
    edx = ZX8(MEM8(edx + eax + 0x18));
    edx = edx & 1;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    MEM32(esp + 0x14) = edx;
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_000EABA9: ;
    RECOMP_PC(0x000EABA9u);
    _fa = (uint32_t)(LO8(ecx)) & 0xFFu; _fb = (uint32_t)(0x10) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test LO8(ecx), 0x10 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_000EABB6; /* je: equal / zero */

loc_000EABAE: ;
    RECOMP_PC(0x000EABAEu);
    MEM8(esp + 0xAC) = 4;

loc_000EABB6: ;
    RECOMP_PC(0x000EABB6u);
    ecx = MEM32(ebp + 8);
    eax = 1;
    MEM32(esp + 0x18) = eax;
    MEM32(esp + 0x70) = eax;
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    eax = eax | 0xFFFFFFFFu;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, eax (32-bit) */
    MEM32(esp + 0x20) = esi;
    MEM32(esp + 0x44) = edx;
    MEM32(esp + 0x48) = esi;
    MEM32(esp + 0x74) = esi;
    if (CMP_NE(_fa, _fb)) goto loc_000EABE9; /* jne: not equal / not zero */

loc_000EABDF: ;
    RECOMP_PC(0x000EABDFu);
    MEM32(esp + 0x44) = eax;
    MEM32(esp + 0x48) = eax;
    goto loc_000EAC31;

loc_000EABE9: ;
    RECOMP_PC(0x000EABE9u);
    edi = MEM32(esp + 0x98);
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, esi (32-bit) */
    if (CMP_BE(_fa, _fb)) goto loc_000EAC31; /* jbe: below or equal (unsigned <=) */

loc_000EABF6: ;
    RECOMP_PC(0x000EABF6u);
    _fa = (uint32_t)(MEM32(esp + 0x18)) & 0xFFFFFFFFu; _fb = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* test MEM32(esp + 0x18), ecx (32-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_000EAC1D; /* je: equal / zero */

loc_000EABFC: ;
    RECOMP_PC(0x000EABFCu);
    edi = MEM32(0x1DE80C);
    edi = MEM32(edi + 4);
    esi = MEM32(ebx + 0x10);
    esi = MEM32(edi + esi * 4);
    esi = MEM32(esi + 8);
    edi = MEM32(esi + eax * 8);
    esi = MEM32(esi + eax * 8 + 4);
    edx = edx | edi;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    MEM32(esp + 0x48) = MEM32(esp + 0x48) | esi;
    _fa = (uint32_t)(MEM32(esp + 0x48)) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_000EAC1D: ;
    RECOMP_PC(0x000EAC1Du);
    MEM32(esp + 0x18) = MEM32(esp + 0x18) << 1;
    _fa = (uint32_t)(MEM32(esp + 0x18)) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    edi = MEM32(esp + 0x98);
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edi (32-bit) */
    if (CMP_B(_fa, _fb)) goto loc_000EABF6; /* jb: below (unsigned <) */

loc_000EAC2D: ;
    RECOMP_PC(0x000EAC2Du);
    MEM32(esp + 0x44) = edx;

loc_000EAC31: ;
    RECOMP_PC(0x000EAC31u);
    ebx = MEM32(esp + 0x68);
    MEM32(esp + 0x18) = esi;
    /* nop */

loc_000EAC40: ;
    RECOMP_PC(0x000EAC40u);
    eax = MEM32(esp + 0x70);
    edi = MEM32(esp + 0x44);
    ecx = MEM32(esp + 0x74);
    esi = MEM32(esp + 0x48);
    eax = eax & edi;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    ecx = ecx & esi;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    eax = eax | ecx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    if ((_fa == 0)) goto loc_000EB134; /* je: equal / zero */

loc_000EAC5C: ;
    RECOMP_PC(0x000EAC5Cu);
    SET_LO8(edx, MEM8(ebp + 6));
    _fa = (uint32_t)(LO8(edx)) & 0xFFu; _fb = (uint32_t)(2) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test LO8(edx), 2 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_000EAC83; /* je: equal / zero */

loc_000EAC64: ;
    RECOMP_PC(0x000EAC64u);
    eax = MEM32(esp + 0x24);
    ecx = MEM32(eax + 0x10);
    eax = MEM32(0x1DE80C);
    eax = MEM32(eax + 4);
    ecx = MEM32(eax + ecx * 4);
    ecx = MEM32(ecx + 4);
    eax = MEM32(esp + 0x20);
    edi = ZX8(MEM8(eax + ecx));
    goto loc_000EAC89;

loc_000EAC83: ;
    RECOMP_PC(0x000EAC83u);
    eax = MEM32(esp + 0x20);
    edi = eax;

loc_000EAC89: ;
    RECOMP_PC(0x000EAC89u);
    SET_LO8(eax, MEM8(esp + eax + 0xAC));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(4) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 4 (8-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000EB134; /* je: equal / zero */

loc_000EAC98: ;
    RECOMP_PC(0x000EAC98u);
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(2) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 2 (8-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000EACA5; /* jne: not equal / not zero */

loc_000EAC9C: ;
    RECOMP_PC(0x000EAC9Cu);
    _fa = (uint32_t)(LO8(edx)) & 0xFFu; _fb = (uint32_t)(8) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test LO8(edx), 8 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_000EB134; /* je: equal / zero */

loc_000EACA5: ;
    RECOMP_PC(0x000EACA5u);
    eax = MEM32(esp + 0x24);
    ecx = MEM32(eax + 0xC);
    esi = MEM32(ecx + 0x14);
    ecx = MEM32(esp + 0x18);
    _fb = (uint32_t)(ecx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + ecx;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(MEM32(eax + 0x14)) & 0xFFFFFFFFu; _fb = (uint32_t)(1) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(eax + 0x14), 1 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000EAD0C; /* jne: not equal / not zero */

loc_000EACBB: ;
    RECOMP_PC(0x000EACBBu);
    edx = eax;
    eax = MEM32(edx + 0x18);
    _fb = (uint32_t)(ecx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + ecx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ecx = MEM32(eax);
    MEM32(esp + 0x4C) = ecx;
    edx = MEM32(eax + 4);
    MEM32(esp + 0x50) = edx;
    ecx = MEM32(eax + 8);
    MEM32(esp + 0x54) = ecx;
    edx = MEM32(eax + 0xC);
    MEM32(esp + 0x58) = edx;
    ecx = MEM32(eax + 0x10);
    MEM32(esp + 0x5C) = ecx;
    edx = MEM32(eax + 0x14);
    MEM32(esp + 0x60) = edx;
    eax = MEM32(eax + 0x18);
    MEM32(esp + 0x64) = eax;
    SET_LO16(eax, MEM16(ebp + 2));
    SET_LO8(ecx, MEM8(ebp));
    edx = esp + 0x28;
    PUSH32(esp, edx);
    eax = ZX16(LO16(eax));
    PUSH32(esp, edi);
    ecx = SX8(LO8(ecx));
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    goto loc_000EADA8;

loc_000EAD0C: ;
    RECOMP_PC(0x000EAD0Cu);
    _fa = (uint32_t)(LO8(edx)) & 0xFFu; _fb = (uint32_t)(8) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test LO8(edx), 8 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_000EAD79; /* je: equal / zero */

loc_000EAD11: ;
    RECOMP_PC(0x000EAD11u);
    eax = MEM32(eax + 0x18);
    edx = MEM32(eax + ecx);
    _fb = (uint32_t)(ecx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + ecx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x4C) = edx;
    ecx = MEM32(eax + 4);
    MEM32(esp + 0x50) = ecx;
    edx = MEM32(eax + 8);
    MEM32(esp + 0x54) = edx;
    ecx = MEM32(eax + 0xC);
    MEM32(esp + 0x58) = ecx;
    edx = MEM32(eax + 0x10);
    MEM32(esp + 0x5C) = edx;
    ecx = MEM32(eax + 0x14);
    MEM32(esp + 0x60) = ecx;
    edx = MEM32(eax + 0x18);
    MEM32(esp + 0x64) = edx;
    eax = MEM32(esi);
    MEM32(esp + 0x28) = eax;
    ecx = MEM32(esi + 4);
    MEM32(esp + 0x2C) = ecx;
    edx = MEM32(esi + 8);
    MEM32(esp + 0x30) = edx;
    eax = MEM32(esi + 0xC);
    MEM32(esp + 0x34) = eax;
    ecx = MEM32(esi + 0x10);
    MEM32(esp + 0x38) = ecx;
    edx = MEM32(esi + 0x14);
    MEM32(esp + 0x3C) = edx;
    eax = MEM32(esi + 0x18);
    MEM32(esp + 0x40) = eax;
    goto loc_000EADB3;

loc_000EAD79: ;
    RECOMP_PC(0x000EAD79u);
    SET_LO8(eax, MEM8(ebp));
    edx = MEM32(esp + 0x6C);
    ecx = esp + 0x4C;
    PUSH32(esp, ecx);
    ecx = MEM32(0x1DE808);
    PUSH32(esp, edi);
    eax = SX8(LO8(eax));
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    PUSH32(esp, 0x000EAD96u); RECOMP_ABI_CALL(0x00019C00u, sub_00019C00); /* call 0x00019C00 */

loc_000EAD96: ;
    RECOMP_PC(0x000EAD96u);
    SET_LO8(eax, MEM8(ebp));
    edx = MEM32(esp + 0x1C);
    ecx = esp + 0x28;
    PUSH32(esp, ecx);
    PUSH32(esp, edi);
    eax = SX8(LO8(eax));
    PUSH32(esp, edx);
    PUSH32(esp, eax);

loc_000EADA8: ;
    RECOMP_PC(0x000EADA8u);
    ecx = MEM32(0x1DE808);
    PUSH32(esp, 0x000EADB3u); RECOMP_ABI_CALL(0x00019C00u, sub_00019C00); /* call 0x00019C00 */

loc_000EADB3: ;
    RECOMP_PC(0x000EADB3u);
    SET_LO8(eax, MEM8(ebp + 6));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(2) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test LO8(eax), 2 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_000EAE04; /* je: equal / zero */

loc_000EADBA: ;
    RECOMP_PC(0x000EADBAu);
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(8) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test LO8(eax), 8 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_000EADE1; /* jne: not equal / not zero */

loc_000EADBE: ;
    RECOMP_PC(0x000EADBEu);
    fp_push(MEMF(esp + 0x4C)); /* fld float */
    ecx = esp + 0x58;
    fp_top() = -fp_top(); /* fchs */
    MEMF(esp + 0x4C) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x000EADD1u); RECOMP_ABI_CALL(0x00124AE0u, sub_00124AE0); /* call 0x00124AE0 */

loc_000EADD1: ;
    RECOMP_PC(0x000EADD1u);
    fp_push(MEMF(eax)); /* fld float */
    fp_top() = -fp_top(); /* fchs */
    MEMF(eax) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x64)); /* fld float */
    fp_top() = -fp_top(); /* fchs */
    MEMF(esp + 0x64) = (float)fp_top(); fp_pop(); /* fstp */

loc_000EADE1: ;
    RECOMP_PC(0x000EADE1u);
    fp_push(MEMF(esp + 0x28)); /* fld float */
    ecx = esp + 0x34;
    fp_top() = -fp_top(); /* fchs */
    MEMF(esp + 0x28) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x000EADF4u); RECOMP_ABI_CALL(0x00124AE0u, sub_00124AE0); /* call 0x00124AE0 */

loc_000EADF4: ;
    RECOMP_PC(0x000EADF4u);
    fp_push(MEMF(eax)); /* fld float */
    fp_top() = -fp_top(); /* fchs */
    MEMF(eax) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x40)); /* fld float */
    fp_top() = -fp_top(); /* fchs */
    MEMF(esp + 0x40) = (float)fp_top(); fp_pop(); /* fstp */

loc_000EAE04: ;
    RECOMP_PC(0x000EAE04u);
    _fa = (uint32_t)(MEM8(ebp + 6)) & 0xFFu; _fb = (uint32_t)(0x20) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test MEM8(ebp + 6), 0x20 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_000EAF7F; /* je: equal / zero */

loc_000EAE0E: ;
    RECOMP_PC(0x000EAE0Eu);
    ecx = MEM32(esp + 0x20);
    SET_LO8(eax, MEM8(esp + ecx + 0xAC));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000EAF1A; /* jne: not equal / not zero */

loc_000EAE21: ;
    RECOMP_PC(0x000EAE21u);
    SET_LO8(eax, MEM8(ebp));
    ecx = MEM32(0x1DE808);
    edx = esp + 0x78;
    PUSH32(esp, edx);
    eax = SX8(LO8(eax));
    PUSH32(esp, edi);
    PUSH32(esp, eax);
    PUSH32(esp, 0x000EAE39u); RECOMP_ABI_CALL(0x000EA330u, sub_000EA330); /* call 0x000EA330 */

loc_000EAE39: ;
    RECOMP_PC(0x000EAE39u);
    _fa = (uint32_t)(MEM8(ebp + 6)) & 0xFFu; _fb = (uint32_t)(2) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test MEM8(ebp + 6), 2 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_000EAE6B; /* je: equal / zero */

loc_000EAE3F: ;
    RECOMP_PC(0x000EAE3Fu);
    fp_push(MEMF(esp + 0x78)); /* fld float */
    ecx = esp + 0x84;
    fp_top() = -fp_top(); /* fchs */
    MEMF(esp + 0x78) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x000EAE55u); RECOMP_ABI_CALL(0x00124AE0u, sub_00124AE0); /* call 0x00124AE0 */

loc_000EAE55: ;
    RECOMP_PC(0x000EAE55u);
    fp_push(MEMF(eax)); /* fld float */
    fp_top() = -fp_top(); /* fchs */
    MEMF(eax) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x90)); /* fld float */
    fp_top() = -fp_top(); /* fchs */
    MEMF(esp + 0x90) = (float)fp_top(); fp_pop(); /* fstp */

loc_000EAE6B: ;
    RECOMP_PC(0x000EAE6Bu);
    ecx = MEM32(esp + 0x78);
    MEM32(esi) = ecx;
    edx = MEM32(esp + 0x7C);
    MEM32(esi + 4) = edx;
    eax = MEM32(esp + 0x80);
    MEM32(esi + 8) = eax;
    ecx = MEM32(esp + 0x84);
    edi = esi + 0xC;
    MEM32(edi) = ecx;
    edx = MEM32(esp + 0x88);
    MEM32(edi + 4) = edx;
    eax = MEM32(esp + 0x8C);
    PUSH32(esp, ebx);
    MEM32(edi + 8) = eax;
    ecx = MEM32(esp + 0x94);
    edx = esp + 0x2C;
    PUSH32(esp, edx);
    eax = esp + 0x54;
    PUSH32(esp, eax);
    PUSH32(esp, esi);
    MEM32(edi + 0xC) = ecx;
    PUSH32(esp, 0x000EAEBDu); RECOMP_ABI_CALL(0x0011C860u, sub_0011C860); /* call 0x0011C860 */

loc_000EAEBD: ;
    RECOMP_PC(0x000EAEBDu);
    eax = MEM32(ebp + 0x1C);
    ecx = MEM32(esp + 0x44);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, eax);
    PUSH32(esp, ebx);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    eax = esp;
    MEM32(eax) = ecx;
    edx = MEM32(esp + 0x50);
    MEM32(eax + 4) = edx;
    ecx = MEM32(esp + 0x54);
    MEM32(eax + 8) = ecx;
    edx = MEM32(esp + 0x58);
    MEM32(eax + 0xC) = edx;
    eax = esp + 0x70;
    PUSH32(esp, eax);
    ecx = esp + 0xB8;
    PUSH32(esp, ecx);
    PUSH32(esp, 0x000EAEF7u); RECOMP_ABI_CALL(0x0011D210u, sub_0011D210); /* call 0x0011D210 */

loc_000EAEF7: ;
    RECOMP_PC(0x000EAEF7u);
    PUSH32(esp, edi);
    edx = esp + 0xC0;
    PUSH32(esp, edx);
    PUSH32(esp, edi);
    PUSH32(esp, 0x000EAF06u); RECOMP_ABI_CALL(0x0011C6D0u, sub_0011C6D0); /* call 0x0011C6D0 */

loc_000EAF06: ;
    RECOMP_PC(0x000EAF06u);
    eax = MEM32(esp + 0x4C);
    _fb = (uint32_t)(0x2C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x2C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM8(esp + eax + 0xAC) = 1;
    goto loc_000EB134;

loc_000EAF1A: ;
    RECOMP_PC(0x000EAF1Au);
    PUSH32(esp, ebx);
    ecx = esp + 0x2C;
    PUSH32(esp, ecx);
    edx = esp + 0x54;
    PUSH32(esp, edx);
    PUSH32(esp, esi);
    PUSH32(esp, 0x000EAF2Bu); RECOMP_ABI_CALL(0x0011C860u, sub_0011C860); /* call 0x0011C860 */

loc_000EAF2B: ;
    RECOMP_PC(0x000EAF2Bu);
    eax = MEM32(ebp + 0x1C);
    ecx = MEM32(esp + 0x44);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, eax);
    PUSH32(esp, ebx);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    eax = esp;
    MEM32(eax) = ecx;
    edx = MEM32(esp + 0x50);
    MEM32(eax + 4) = edx;
    ecx = MEM32(esp + 0x54);
    MEM32(eax + 8) = ecx;
    edx = MEM32(esp + 0x58);
    MEM32(eax + 0xC) = edx;
    eax = esp + 0x70;
    PUSH32(esp, eax);
    ecx = esp + 0xB8;
    PUSH32(esp, ecx);
    PUSH32(esp, 0x000EAF65u); RECOMP_ABI_CALL(0x0011D210u, sub_0011D210); /* call 0x0011D210 */

loc_000EAF65: ;
    RECOMP_PC(0x000EAF65u);
    eax = esi + 0xC;
    PUSH32(esp, eax);
    edx = esp + 0xC0;
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    PUSH32(esp, 0x000EAF77u); RECOMP_ABI_CALL(0x0011C6D0u, sub_0011C6D0); /* call 0x0011C6D0 */

loc_000EAF77: ;
    RECOMP_PC(0x000EAF77u);
    _fb = (uint32_t)(0x2C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x2C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    goto loc_000EB134;

loc_000EAF7F: ;
    RECOMP_PC(0x000EAF7Fu);
    eax = MEM32(esp + 0x14);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000EB0DE; /* je: equal / zero */

loc_000EAF8B: ;
    RECOMP_PC(0x000EAF8Bu);
    SET_LO8(eax, MEM8(ebp));
    ecx = esp + 0x78;
    PUSH32(esp, ecx);
    ecx = MEM32(0x1DE808);
    edx = SX8(LO8(eax));
    PUSH32(esp, edi);
    PUSH32(esp, edx);
    PUSH32(esp, 0x000EAFA3u); RECOMP_ABI_CALL(0x000EA330u, sub_000EA330); /* call 0x000EA330 */

loc_000EAFA3: ;
    RECOMP_PC(0x000EAFA3u);
    _fa = (uint32_t)(MEM8(ebp + 6)) & 0xFFu; _fb = (uint32_t)(2) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test MEM8(ebp + 6), 2 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_000EAFD5; /* je: equal / zero */

loc_000EAFA9: ;
    RECOMP_PC(0x000EAFA9u);
    fp_push(MEMF(esp + 0x78)); /* fld float */
    ecx = esp + 0x84;
    fp_top() = -fp_top(); /* fchs */
    MEMF(esp + 0x78) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x000EAFBFu); RECOMP_ABI_CALL(0x00124AE0u, sub_00124AE0); /* call 0x00124AE0 */

loc_000EAFBF: ;
    RECOMP_PC(0x000EAFBFu);
    fp_push(MEMF(eax)); /* fld float */
    fp_top() = -fp_top(); /* fchs */
    MEMF(eax) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x90)); /* fld float */
    fp_top() = -fp_top(); /* fchs */
    MEMF(esp + 0x90) = (float)fp_top(); fp_pop(); /* fstp */

loc_000EAFD5: ;
    RECOMP_PC(0x000EAFD5u);
    eax = MEM32(esp + 0x78);
    MEM32(esi) = eax;
    ecx = MEM32(esp + 0x7C);
    MEM32(esi + 4) = ecx;
    edx = MEM32(esp + 0x80);
    MEM32(esi + 8) = edx;
    eax = MEM32(esp + 0x84);
    edi = esi + 0xC;
    MEM32(edi) = eax;
    ecx = MEM32(esp + 0x88);
    MEM32(edi + 4) = ecx;
    edx = MEM32(esp + 0x8C);
    MEM32(edi + 8) = edx;
    eax = MEM32(esp + 0x90);
    MEM32(edi + 0xC) = eax;
    _fa = (uint32_t)(MEM8(ebp + 6)) & 0xFFu; _fb = (uint32_t)(8) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test MEM8(ebp + 6), 8 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_000EB07F; /* je: equal / zero */

loc_000EB01C: ;
    RECOMP_PC(0x000EB01Cu);
    fp_push(MEMF(esp + 0x28)); /* fld float */
    PUSH32(esp, edi);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi)); /* fadd dword ptr [esi] */
    ecx = esp + 0x38;
    PUSH32(esp, ecx);
    PUSH32(esp, edi);
    MEMF(esi) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x38)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 4)); /* fadd dword ptr [esi + 4] */
    MEMF(esi + 4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x3C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 8)); /* fadd dword ptr [esi + 8] */
    MEMF(esi + 8) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x000EB044u); RECOMP_ABI_CALL(0x0011C6D0u, sub_0011C6D0); /* call 0x0011C6D0 */

loc_000EB044: ;
    RECOMP_PC(0x000EB044u);
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    edx = esp + 0x60;
    PUSH32(esp, edx);
    PUSH32(esp, esi);
    PUSH32(esp, 0x000EB051u); RECOMP_ABI_CALL(0x0011C7F0u, sub_0011C7F0); /* call 0x0011C7F0 */

loc_000EB051: ;
    RECOMP_PC(0x000EB051u);
    eax = MEM32(ebp + 0x1C);
    ecx = MEM32(edi);
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x1C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, eax);
    PUSH32(esp, ebx);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    eax = esp;
    MEM32(eax) = ecx;
    edx = MEM32(edi + 4);
    MEM32(eax + 4) = edx;
    ecx = MEM32(edi + 8);
    MEM32(eax + 8) = ecx;
    edx = MEM32(edi + 0xC);
    MEM32(eax + 0xC) = edx;
    eax = esp + 0x70;
    PUSH32(esp, eax);
    PUSH32(esp, edi);
    goto loc_000EB120;

loc_000EB07F: ;
    RECOMP_PC(0x000EB07Fu);
    PUSH32(esp, ebx);
    ecx = esp + 0x2C;
    PUSH32(esp, ecx);
    edx = esp + 0x54;
    PUSH32(esp, edx);
    PUSH32(esp, esi);
    PUSH32(esp, 0x000EB090u); RECOMP_ABI_CALL(0x0011C860u, sub_0011C860); /* call 0x0011C860 */

loc_000EB090: ;
    RECOMP_PC(0x000EB090u);
    eax = MEM32(ebp + 0x1C);
    ecx = MEM32(esp + 0x44);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, eax);
    PUSH32(esp, ebx);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    eax = esp;
    MEM32(eax) = ecx;
    edx = MEM32(esp + 0x50);
    MEM32(eax + 4) = edx;
    ecx = MEM32(esp + 0x54);
    MEM32(eax + 8) = ecx;
    edx = MEM32(esp + 0x58);
    MEM32(eax + 0xC) = edx;
    eax = esp + 0x70;
    PUSH32(esp, eax);
    ecx = esp + 0xB8;
    PUSH32(esp, ecx);
    PUSH32(esp, 0x000EB0CAu); RECOMP_ABI_CALL(0x0011D210u, sub_0011D210); /* call 0x0011D210 */

loc_000EB0CA: ;
    RECOMP_PC(0x000EB0CAu);
    PUSH32(esp, edi);
    edx = esp + 0xC0;
    PUSH32(esp, edx);
    PUSH32(esp, edi);
    PUSH32(esp, 0x000EB0D9u); RECOMP_ABI_CALL(0x0011C6D0u, sub_0011C6D0); /* call 0x0011C6D0 */

loc_000EB0D9: ;
    RECOMP_PC(0x000EB0D9u);
    _fb = (uint32_t)(0x2C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x2C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    goto loc_000EB128;

loc_000EB0DE: ;
    RECOMP_PC(0x000EB0DEu);
    PUSH32(esp, ebx);
    eax = esp + 0x2C;
    PUSH32(esp, eax);
    ecx = esp + 0x54;
    PUSH32(esp, ecx);
    PUSH32(esp, esi);
    PUSH32(esp, 0x000EB0EFu); RECOMP_ABI_CALL(0x0011C7F0u, sub_0011C7F0); /* call 0x0011C7F0 */

loc_000EB0EF: ;
    RECOMP_PC(0x000EB0EFu);
    eax = MEM32(ebp + 0x1C);
    edx = MEM32(esp + 0x44);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, eax);
    PUSH32(esp, ebx);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    eax = esp;
    MEM32(eax) = edx;
    ecx = MEM32(esp + 0x50);
    MEM32(eax + 4) = ecx;
    edx = MEM32(esp + 0x54);
    MEM32(eax + 8) = edx;
    ecx = MEM32(esp + 0x58);
    edx = esp + 0x70;
    PUSH32(esp, edx);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + 0xC;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(eax + 0xC) = ecx;
    PUSH32(esp, esi);

loc_000EB120: ;
    RECOMP_PC(0x000EB120u);
    PUSH32(esp, 0x000EB125u); RECOMP_ABI_CALL(0x0011D210u, sub_0011D210); /* call 0x0011D210 */

loc_000EB125: ;
    RECOMP_PC(0x000EB125u);
    _fb = (uint32_t)(0x20) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x20;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */

loc_000EB128: ;
    RECOMP_PC(0x000EB128u);
    eax = MEM32(esp + 0x20);
    MEM8(esp + eax + 0xAC) = 2;

loc_000EB134: ;
    RECOMP_PC(0x000EB134u);
    ecx = MEM32(esp + 0x70);
    edx = MEM32(esp + 0x74);
    eax = MEM32(esp + 0x20);
    { uint32_t _c = (uint32_t)(1) & 31u; if (_c && _c < 32u) { edx = (edx << _c) | (ecx >> (32 - _c)); _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shld result */ } }  /* shld */
    ecx = ecx << 1;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    MEM32(esp + 0x74) = edx;
    edx = MEM32(esp + 0x18);
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edx = edx + 0x1C;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x70) = ecx;
    ecx = MEM32(esp + 0x24);
    MEM32(esp + 0x18) = edx;
    edx = MEM32(ecx + 0xC);
    ecx = MEM32(edx + 8);
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ecx (32-bit) */
    MEM32(esp + 0x20) = eax;
    if (CMP_NE(_fa, _fb)) goto loc_000EAC40; /* jne: not equal / not zero */

loc_000EB170: ;
    RECOMP_PC(0x000EB170u);
    _fa = (uint32_t)(MEM8(ebp + 6)) & 0xFFu; _fb = (uint32_t)(4) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test MEM8(ebp + 6), 4 (8-bit) */
    if (TEST_Z(_fa, _fb)) goto loc_000EB1B7; /* je: equal / zero */

loc_000EB176: ;
    RECOMP_PC(0x000EB176u);
    eax = MEM32(ebp + 0x14);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000EB1AA; /* je: equal / zero */

loc_000EB17D: ;
    RECOMP_PC(0x000EB17Du);
    ecx = 0xA;
    esi = ebp;
    edi = esp + 0xFC;
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
    SET_LO8(ecx, MEM8(0x1C3E5C));
    MEM8(ebp) = LO8(ecx);
    ecx = esp + 0xFC;
    MEM8(ebp + 1) = 0xFF;
    edx = MEM32(eax);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    ecx = eax;
    { uint32_t _icall_target = MEM32(edx); PUSH32(esp, 0x000EB1A8u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000EB1A6u); } /* indirect call */
    }

loc_000EB1A8: ;
    RECOMP_PC(0x000EB1A8u);
    goto loc_000EB1B7;

loc_000EB1AA: ;
    RECOMP_PC(0x000EB1AAu);
    SET_LO8(edx, MEM8(0x1C3E5C));
    MEM8(ebp) = LO8(edx);
    MEM8(ebp + 1) = 0xFF;

loc_000EB1B7: ;
    RECOMP_PC(0x000EB1B7u);
    eax = MEM32(esp + 0x24);
    ecx = MEM32(eax + 0xC);
    MEM32(ecx + 0x20) = 1;
    ebx = eax;

loc_000EB1C7: ;
    RECOMP_PC(0x000EB1C7u);
    eax = MEM32(esp + 0x94);
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebp = ebp + 0x28;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    MEM32(esp + 0x94) = eax;
    if ((_fa != 0)) goto loc_000EA980; /* jne: not equal / not zero */

loc_000EB1DF: ;
    RECOMP_PC(0x000EB1DFu);
    eax = MEM32(ebx + 0x18);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000EB24B; /* je: equal / zero */

loc_000EB1E6: ;
    RECOMP_PC(0x000EB1E6u);
    _fa = (uint32_t)(MEM32(ebx + 0x14)) & 0xFFFFFFFFu; _fb = (uint32_t)(1) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(ebx + 0x14), 1 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000EB24B; /* je: equal / zero */

loc_000EB1EC: ;
    RECOMP_PC(0x000EB1ECu);
    edx = MEM32(ebx + 0xC);
    eax = MEM32(edx + 8);
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_BE(_fa, _fb)) goto loc_000EB24B; /* jbe: below or equal (unsigned <=) */

loc_000EB1F8: ;
    RECOMP_PC(0x000EB1F8u);
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    /* nop */

loc_000EB200: ;
    RECOMP_PC(0x000EB200u);
    eax = MEM32(ebx + 0xC);
    eax = MEM32(eax + 0x14);
    ecx = MEM32(ebx + 0x18);
    edi = MEM32(eax + edx);
    _fb = (uint32_t)(edx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + edx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(ecx + edx) = edi;
    edi = MEM32(eax + 4);
    _fb = (uint32_t)(edx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ecx = ecx + edx;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(ecx + 4) = edi;
    edi = MEM32(eax + 8);
    MEM32(ecx + 8) = edi;
    ecx = MEM32(ebx + 0x18);
    edi = MEM32(eax + 0xC);
    ecx = edx + ecx + 0xC;
    MEM32(ecx) = edi;
    edi = MEM32(eax + 0x10);
    MEM32(ecx + 4) = edi;
    edi = MEM32(eax + 0x14);
    MEM32(ecx + 8) = edi;
    eax = MEM32(eax + 0x18);
    MEM32(ecx + 0xC) = eax;
    ecx = MEM32(ebx + 0xC);
    eax = MEM32(ecx + 8);
    esi++;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edx = edx + 0x1C;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, eax (32-bit) */
    if (CMP_B(_fa, _fb)) goto loc_000EB200; /* jb: below (unsigned <) */

loc_000EB24B: ;
    RECOMP_PC(0x000EB24Bu);
    _fa = (uint32_t)(MEM32(ebx + 0x14)) & 0xFFFFFFFFu; _fb = (uint32_t)(2) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(ebx + 0x14), 2 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000EB25E; /* jne: not equal / not zero */

loc_000EB251: ;
    RECOMP_PC(0x000EB251u);
    edx = MEM32(ebx + 4);
    MEM32(edx + 0x188) = 0;

loc_000EB25E: ;
    RECOMP_PC(0x000EB25Eu);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    _fb = (uint32_t)(0x114) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x114;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; return; /* ret 4 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_00013320
 * Original: 0x00013320 - 0x00013594 (628 bytes, 210 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00013320(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00013320: ;
    RECOMP_PC(0x00013320u);
    _fb = (uint32_t)(0x20) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x20;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    esi = MEM32(esp + 0x30);
    ebp = ecx;
    PUSH32(esp, edi);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ebx = ebp + 4;

loc_00013332: ;
    RECOMP_PC(0x00013332u);
    PUSH32(esp, edi);
    eax = esp + 0x14;
    PUSH32(esp, 0x1AE4C4);
    PUSH32(esp, eax);
    PUSH32(esp, 0x00013342u); RECOMP_ABI_CALL(0x000E07A0u, sub_000E07A0); /* call 0x000E07A0 */

loc_00013342: ;
    RECOMP_PC(0x00013342u);
    edx = MEM32(esi);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    eax = esp + 0x10;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0xC); PUSH32(esp, 0x00013351u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x0001334Eu); } /* indirect call */
    }

loc_00013351: ;
    RECOMP_PC(0x00013351u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0001337B; /* je: equal / zero */

loc_00013355: ;
    RECOMP_PC(0x00013355u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x7F7FFFFF);
    PUSH32(esp, 0xFF7FFFFFu);
    PUSH32(esp, ebx);
    PUSH32(esp, 0x1AE4BC);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x88); PUSH32(esp, 0x0001336Fu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00013369u); } /* indirect call */
    }

loc_0001336F: ;
    RECOMP_PC(0x0001336Fu);
    eax = MEM32(esi);
    ecx = esp + 0x10;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0x10); PUSH32(esp, 0x0001337Bu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00013378u); } /* indirect call */
    }

loc_0001337B: ;
    RECOMP_PC(0x0001337Bu);
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebx = ebx + 4;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 4 (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_00013332; /* jl: less (signed <) */

loc_00013384: ;
    RECOMP_PC(0x00013384u);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ebx = ebp + 0x14;
    /* nop */

loc_00013390: ;
    RECOMP_PC(0x00013390u);
    PUSH32(esp, edi);
    edx = esp + 0x14;
    PUSH32(esp, 0x1AE4B4);
    PUSH32(esp, edx);
    PUSH32(esp, 0x000133A0u); RECOMP_ABI_CALL(0x000E07A0u, sub_000E07A0); /* call 0x000E07A0 */

loc_000133A0: ;
    RECOMP_PC(0x000133A0u);
    eax = MEM32(esi);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ecx = esp + 0x10;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0xC); PUSH32(esp, 0x000133AFu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000133ACu); } /* indirect call */
    }

loc_000133AF: ;
    RECOMP_PC(0x000133AFu);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000133D9; /* je: equal / zero */

loc_000133B3: ;
    RECOMP_PC(0x000133B3u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x7FFFFFFF);
    PUSH32(esp, 0x80000000u);
    PUSH32(esp, ebx);
    PUSH32(esp, 0x1AE4BC);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x8C); PUSH32(esp, 0x000133CDu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000133C7u); } /* indirect call */
    }

loc_000133CD: ;
    RECOMP_PC(0x000133CDu);
    eax = MEM32(esi);
    ecx = esp + 0x10;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0x10); PUSH32(esp, 0x000133D9u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000133D6u); } /* indirect call */
    }

loc_000133D9: ;
    RECOMP_PC(0x000133D9u);
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebx = ebx + 4;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(8) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 8 (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_00013390; /* jl: less (signed <) */

loc_000133E2: ;
    RECOMP_PC(0x000133E2u);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ebx = ebp + 0x34;
    goto loc_000133F0;

    RECOMP_PC(0x000133E9u);
    /* nop */

loc_000133F0: ;
    RECOMP_PC(0x000133F0u);
    PUSH32(esp, edi);
    edx = esp + 0x14;
    PUSH32(esp, 0x1AE4A0);
    PUSH32(esp, edx);
    PUSH32(esp, 0x00013400u); RECOMP_ABI_CALL(0x000E07A0u, sub_000E07A0); /* call 0x000E07A0 */

loc_00013400: ;
    RECOMP_PC(0x00013400u);
    eax = MEM32(esi);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ecx = esp + 0x10;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0xC); PUSH32(esp, 0x0001340Fu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x0001340Cu); } /* indirect call */
    }

loc_0001340F: ;
    RECOMP_PC(0x0001340Fu);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00013439; /* je: equal / zero */

loc_00013413: ;
    RECOMP_PC(0x00013413u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x7FFFFFFF);
    PUSH32(esp, 0x80000000u);
    PUSH32(esp, ebx);
    PUSH32(esp, 0x1AE4BC);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x8C); PUSH32(esp, 0x0001342Du); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00013427u); } /* indirect call */
    }

loc_0001342D: ;
    RECOMP_PC(0x0001342Du);
    eax = MEM32(esi);
    ecx = esp + 0x10;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0x10); PUSH32(esp, 0x00013439u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00013436u); } /* indirect call */
    }

loc_00013439: ;
    RECOMP_PC(0x00013439u);
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebx = ebx + 4;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 4 (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_000133F0; /* jl: less (signed <) */

loc_00013442: ;
    RECOMP_PC(0x00013442u);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ebx = ebp + 0x48;
    goto loc_00013450;

    RECOMP_PC(0x00013449u);
    /* nop */

loc_00013450: ;
    RECOMP_PC(0x00013450u);
    PUSH32(esp, edi);
    edx = esp + 0x14;
    PUSH32(esp, 0x1AE494);
    PUSH32(esp, edx);
    PUSH32(esp, 0x00013460u); RECOMP_ABI_CALL(0x000E07A0u, sub_000E07A0); /* call 0x000E07A0 */

loc_00013460: ;
    RECOMP_PC(0x00013460u);
    eax = MEM32(esi);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ecx = esp + 0x10;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0xC); PUSH32(esp, 0x0001346Fu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x0001346Cu); } /* indirect call */
    }

loc_0001346F: ;
    RECOMP_PC(0x0001346Fu);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00013499; /* je: equal / zero */

loc_00013473: ;
    RECOMP_PC(0x00013473u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x7FFFFFFF);
    PUSH32(esp, 0x80000000u);
    PUSH32(esp, ebx);
    PUSH32(esp, 0x1AE48C);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x8C); PUSH32(esp, 0x0001348Du); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00013487u); } /* indirect call */
    }

loc_0001348D: ;
    RECOMP_PC(0x0001348Du);
    eax = MEM32(esi);
    ecx = esp + 0x10;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0x10); PUSH32(esp, 0x00013499u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00013496u); } /* indirect call */
    }

loc_00013499: ;
    RECOMP_PC(0x00013499u);
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebx = ebx + 4;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0xF) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0xF (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_00013450; /* jl: less (signed <) */

loc_000134A2: ;
    RECOMP_PC(0x000134A2u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x7FFFFFFF);
    PUSH32(esp, 0x80000000u);
    eax = ebp + 0x44;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1AE480);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x8C); PUSH32(esp, 0x000134BFu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000134B9u); } /* indirect call */
    }

loc_000134BF: ;
    RECOMP_PC(0x000134BFu);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x7FFFFFFF);
    PUSH32(esp, 0x80000000u);
    eax = ebp + 0x84;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1AE474);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x8C); PUSH32(esp, 0x000134DFu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000134D9u); } /* indirect call */
    }

loc_000134DF: ;
    RECOMP_PC(0x000134DFu);
    ecx = MEM32(ebp + 0x88);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0);
    eax = esp + 0x3C;
    PUSH32(esp, eax);
    MEM32(esp + 0x40) = ecx;
    PUSH32(esp, 0x1AE464);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x90); PUSH32(esp, 0x00013501u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000134FBu); } /* indirect call */
    }

loc_00013501: ;
    RECOMP_PC(0x00013501u);
    ecx = MEM32(esp + 0x34);
    MEM32(ebp + 0x88) = ecx;
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    edi = ebp + 0x9C;

loc_00013513: ;
    RECOMP_PC(0x00013513u);
    PUSH32(esp, ebx);
    edx = esp + 0x14;
    PUSH32(esp, 0x1ADFA8);
    PUSH32(esp, edx);
    PUSH32(esp, 0x00013523u); RECOMP_ABI_CALL(0x000E07A0u, sub_000E07A0); /* call 0x000E07A0 */

loc_00013523: ;
    RECOMP_PC(0x00013523u);
    eax = MEM32(esi);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ecx = esp + 0x10;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0xC); PUSH32(esp, 0x00013532u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x0001352Fu); } /* indirect call */
    }

loc_00013532: ;
    RECOMP_PC(0x00013532u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00013576; /* je: equal / zero */

loc_00013536: ;
    RECOMP_PC(0x00013536u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x7FFFFFFF);
    PUSH32(esp, 0x80000000u);
    eax = edi + -16;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1AE45C);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x8C); PUSH32(esp, 0x00013553u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x0001354Du); } /* indirect call */
    }

loc_00013553: ;
    RECOMP_PC(0x00013553u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x7FFFFFFF);
    PUSH32(esp, 0x80000000u);
    PUSH32(esp, edi);
    PUSH32(esp, 0x1AE450);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x8C); PUSH32(esp, 0x0001356Du); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00013567u); } /* indirect call */
    }

loc_0001356D: ;
    RECOMP_PC(0x0001356Du);
    eax = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0x10); PUSH32(esp, 0x00013576u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00013573u); } /* indirect call */
    }

loc_00013576: ;
    RECOMP_PC(0x00013576u);
    ebx++;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edi = edi + 4;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 4 (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_00013513; /* jl: less (signed <) */

loc_0001357F: ;
    RECOMP_PC(0x0001357Fu);
    POP32(esp, edi);
    eax = 1;
    POP32(esp, esi);
    MEM32(ebp + 0xAC) = eax;
    POP32(esp, ebp);
    POP32(esp, ebx);
    _fb = (uint32_t)(0x20) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x20;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; return; /* ret 4 */

}


/**
 * qpr2_recovered_000137B0
 * Original: 0x000137B0 - 0x00013892 (226 bytes, 74 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000137B0(void)
{
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;

loc_000137B0: ;
    RECOMP_PC(0x000137B0u);
    PUSH32(esp, esi);
    esi = MEM32(esp + 8);
    eax = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edi);
    edi = ecx;
    PUSH32(esp, 0x1AE568);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0xC); PUSH32(esp, 0x000137C4u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000137C1u); } /* indirect call */
    }

loc_000137C4: ;
    RECOMP_PC(0x000137C4u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00013888; /* je: equal / zero */

loc_000137CC: ;
    RECOMP_PC(0x000137CCu);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x7FFFFFFF);
    PUSH32(esp, 0x80000000u);
    eax = edi + 4;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1AE554);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x8C); PUSH32(esp, 0x000137E9u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000137E3u); } /* indirect call */
    }

loc_000137E9: ;
    RECOMP_PC(0x000137E9u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x7FFFFFFF);
    PUSH32(esp, 0x80000000u);
    eax = edi + 8;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1AE53C);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x8C); PUSH32(esp, 0x00013806u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00013800u); } /* indirect call */
    }

loc_00013806: ;
    RECOMP_PC(0x00013806u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x7FFFFFFF);
    PUSH32(esp, 0x80000000u);
    eax = edi + 0xC;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1AE52C);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x8C); PUSH32(esp, 0x00013823u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x0001381Du); } /* indirect call */
    }

loc_00013823: ;
    RECOMP_PC(0x00013823u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0);
    eax = edi + 0x10;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1AE51C);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x90); PUSH32(esp, 0x0001383Au); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00013834u); } /* indirect call */
    }

loc_0001383A: ;
    RECOMP_PC(0x0001383Au);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0);
    eax = edi + 0x18;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1AE50C);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x90); PUSH32(esp, 0x00013851u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x0001384Bu); } /* indirect call */
    }

loc_00013851: ;
    RECOMP_PC(0x00013851u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0);
    eax = edi + 0x14;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1AE4F8);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x90); PUSH32(esp, 0x00013868u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00013862u); } /* indirect call */
    }

loc_00013868: ;
    RECOMP_PC(0x00013868u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0);
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edi = edi + 0x1C;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, edi);
    PUSH32(esp, 0x1AE4E0);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x90); PUSH32(esp, 0x0001387Fu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00013879u); } /* indirect call */
    }

loc_0001387F: ;
    RECOMP_PC(0x0001387Fu);
    eax = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0x10); PUSH32(esp, 0x00013888u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00013885u); } /* indirect call */
    }

loc_00013888: ;
    RECOMP_PC(0x00013888u);
    POP32(esp, edi);
    eax = 1;
    POP32(esp, esi);
    esp += 8; return; /* ret 4 */

}


/**
 * qpr2_recovered_00011AC0
 * Original: 0x00011AC0 - 0x00011BE0 (288 bytes, 100 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00011AC0(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00011AC0: ;
    RECOMP_PC(0x00011AC0u);
    _fb = (uint32_t)(0x14) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x14;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebp);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, esi);
    esi = MEM32(esp + 0x20);
    eax = MEM32(esi);
    ebp = ecx;
    PUSH32(esp, 0x1AE130);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0xC); PUSH32(esp, 0x00011AD7u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00011AD4u); } /* indirect call */
    }

loc_00011AD7: ;
    RECOMP_PC(0x00011AD7u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00011BD3; /* je: equal / zero */

loc_00011ADF: ;
    RECOMP_PC(0x00011ADFu);
    edx = MEM32(esi);
    PUSH32(esp, ebx);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edi);
    PUSH32(esp, 0x1CA1B8);
    eax = ebp + 4;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1AE11C);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0xAC); PUSH32(esp, 0x00011AF9u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00011AF3u); } /* indirect call */
    }

loc_00011AF9: ;
    RECOMP_PC(0x00011AF9u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0);
    eax = ebp + 0x308;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1ADFCC);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x90); PUSH32(esp, 0x00011B13u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00011B0Du); } /* indirect call */
    }

loc_00011B13: ;
    RECOMP_PC(0x00011B13u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0);
    eax = ebp + 0x5F4;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1AE110);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x90); PUSH32(esp, 0x00011B2Du); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00011B27u); } /* indirect call */
    }

loc_00011B2D: ;
    RECOMP_PC(0x00011B2Du);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ebx = ebp + 0x30C;

loc_00011B35: ;
    RECOMP_PC(0x00011B35u);
    PUSH32(esp, edi);
    ecx = esp + 0x14;
    PUSH32(esp, 0x1AE108);
    PUSH32(esp, ecx);
    PUSH32(esp, 0x00011B45u); RECOMP_ABI_CALL(0x000E07A0u, sub_000E07A0); /* call 0x000E07A0 */

loc_00011B45: ;
    RECOMP_PC(0x00011B45u);
    edx = MEM32(ebx);
    eax = MEM32(esi);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0);
    ecx = esp + 0x30;
    MEM32(esp + 0x30) = edx;
    PUSH32(esp, ecx);
    edx = esp + 0x1C;
    PUSH32(esp, edx);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0x90); PUSH32(esp, 0x00011B66u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00011B60u); } /* indirect call */
    }

loc_00011B66: ;
    RECOMP_PC(0x00011B66u);
    eax = MEM32(esp + 0x28);
    PUSH32(esp, edi);
    ecx = esp + 0x14;
    PUSH32(esp, 0x1AE0FC);
    PUSH32(esp, ecx);
    MEM32(ebx) = eax;
    PUSH32(esp, 0x00011B7Cu); RECOMP_ABI_CALL(0x000E07A0u, sub_000E07A0); /* call 0x000E07A0 */

loc_00011B7C: ;
    RECOMP_PC(0x00011B7Cu);
    edx = MEM32(esi);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x7FFFFFFF);
    PUSH32(esp, 0x80000000u);
    eax = ebx + 0x4C;
    PUSH32(esp, eax);
    ecx = esp + 0x1C;
    PUSH32(esp, ecx);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x8C); PUSH32(esp, 0x00011B9Cu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00011B96u); } /* indirect call */
    }

loc_00011B9C: ;
    RECOMP_PC(0x00011B9Cu);
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebx = ebx + 4;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0x13) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0x13 (32-bit) */
    if (CMP_B(_fa, _fb)) goto loc_00011B35; /* jb: below (unsigned <) */

loc_00011BA5: ;
    RECOMP_PC(0x00011BA5u);
    PUSH32(esp, esi);
    ecx = ebp + 0x3E4;
    PUSH32(esp, 0x00011BB1u); RECOMP_ABI_CALL(0x000118E0u, sub_000118E0); /* call 0x000118E0 */

loc_00011BB1: ;
    RECOMP_PC(0x00011BB1u);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ebx = ebp + 8;

loc_00011BB6: ;
    RECOMP_PC(0x00011BB6u);
    PUSH32(esp, edi);
    PUSH32(esp, esi);
    ecx = ebx;
    PUSH32(esp, 0x00011BBFu); RECOMP_ABI_CALL(0x00011760u, sub_00011760); /* call 0x00011760 */

loc_00011BBF: ;
    RECOMP_PC(0x00011BBFu);
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(0x60) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebx = ebx + 0x60;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(8) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 8 (32-bit) */
    if (CMP_B(_fa, _fb)) goto loc_00011BB6; /* jb: below (unsigned <) */

loc_00011BC8: ;
    RECOMP_PC(0x00011BC8u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x10); PUSH32(esp, 0x00011BD1u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00011BCEu); } /* indirect call */
    }

loc_00011BD1: ;
    RECOMP_PC(0x00011BD1u);
    POP32(esp, edi);
    POP32(esp, ebx);

loc_00011BD3: ;
    RECOMP_PC(0x00011BD3u);
    POP32(esp, esi);
    eax = 1;
    POP32(esp, ebp);
    _fb = (uint32_t)(0x14) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x14;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; return; /* ret 4 */

}


/**
 * qpr2_recovered_000112F0
 * Original: 0x000112F0 - 0x000113F4 (260 bytes, 95 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000112F0(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_000112F0: ;
    RECOMP_PC(0x000112F0u);
    _fb = (uint32_t)(0x14) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x14;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    esi = MEM32(esp + 0x20);
    eax = MEM32(esi);
    ebx = ecx;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x1ADFD4);
    ecx = esi;
    MEM32(esp + 0xC) = ebx;
    { uint32_t _icall_target = MEM32(eax + 0xC); PUSH32(esp, 0x0001130Bu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00011308u); } /* indirect call */
    }

loc_0001130B: ;
    RECOMP_PC(0x0001130Bu);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000113EA; /* je: equal / zero */

loc_00011313: ;
    RECOMP_PC(0x00011313u);
    edx = MEM32(esi);
    PUSH32(esp, ebp);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edi);
    PUSH32(esp, 0xF);
    PUSH32(esp, 0);
    eax = ebx + 4;
    PUSH32(esp, eax);
    PUSH32(esp, 0x1ADFCC);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x90); PUSH32(esp, 0x0001132Cu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00011326u); } /* indirect call */
    }

loc_0001132C: ;
    RECOMP_PC(0x0001132Cu);
    ebp = eax;
    ebp = ebp & 1;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    MEM32(esp + 0x28) = edi;

loc_00011337: ;
    RECOMP_PC(0x00011337u);
    ecx = esp + 0x28;
    PUSH32(esp, ecx);
    edx = edi + edi * 2;
    PUSH32(esp, esi);
    ecx = ebx + edx * 4 + 8;
    PUSH32(esp, 0x00011349u); RECOMP_ABI_CALL(0x00011150u, sub_00011150); /* call 0x00011150 */

loc_00011349: ;
    RECOMP_PC(0x00011349u);
    ebp = ebp & eax;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(8) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 8 (32-bit) */
    MEM32(esp + 0x28) = edi;
    if (CMP_L(_fas, _fbs)) goto loc_00011337; /* jl: less (signed <) */

loc_00011355: ;
    RECOMP_PC(0x00011355u);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fb = (uint32_t)(0x68) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebx = ebx + 0x68;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    /* nop */

loc_00011360: ;
    RECOMP_PC(0x00011360u);
    PUSH32(esp, edi);
    eax = esp + 0x18;
    PUSH32(esp, 0x1ADFC0);
    PUSH32(esp, eax);
    PUSH32(esp, 0x00011370u); RECOMP_ABI_CALL(0x000E07A0u, sub_000E07A0); /* call 0x000E07A0 */

loc_00011370: ;
    RECOMP_PC(0x00011370u);
    edx = MEM32(esi);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x7FFFFFFF);
    PUSH32(esp, 0x80000000u);
    PUSH32(esp, ebx);
    eax = esp + 0x20;
    PUSH32(esp, eax);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x8C); PUSH32(esp, 0x0001138Du); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00011387u); } /* indirect call */
    }

loc_0001138D: ;
    RECOMP_PC(0x0001138Du);
    ebp = ebp & eax;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebx = ebx + 4;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0x18) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0x18 (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_00011360; /* jl: less (signed <) */

loc_00011398: ;
    RECOMP_PC(0x00011398u);
    ebx = MEM32(esp + 0x10);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fb = (uint32_t)(0xC8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebx = ebx + 0xC8;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */

loc_000113A4: ;
    RECOMP_PC(0x000113A4u);
    PUSH32(esp, edi);
    ecx = esp + 0x18;
    PUSH32(esp, 0x1ADFB4);
    PUSH32(esp, ecx);
    PUSH32(esp, 0x000113B4u); RECOMP_ABI_CALL(0x000E07A0u, sub_000E07A0); /* call 0x000E07A0 */

loc_000113B4: ;
    RECOMP_PC(0x000113B4u);
    edx = MEM32(esi);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x1CE4B4);
    PUSH32(esp, ebx);
    eax = esp + 0x1C;
    PUSH32(esp, eax);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0xAC); PUSH32(esp, 0x000113CCu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000113C6u); } /* indirect call */
    }

loc_000113CC: ;
    RECOMP_PC(0x000113CCu);
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebx = ebx + 4;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0xF) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0xF (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_000113A4; /* jl: less (signed <) */

loc_000113D5: ;
    RECOMP_PC(0x000113D5u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x10); PUSH32(esp, 0x000113DEu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000113DBu); } /* indirect call */
    }

loc_000113DE: ;
    RECOMP_PC(0x000113DEu);
    POP32(esp, edi);
    eax = eax & ebp;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    POP32(esp, ebp);
    POP32(esp, esi);
    POP32(esp, ebx);
    _fb = (uint32_t)(0x14) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x14;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; return; /* ret 4 */

loc_000113EA: ;
    RECOMP_PC(0x000113EAu);
    POP32(esp, esi);
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    POP32(esp, ebx);
    _fb = (uint32_t)(0x14) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x14;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; return; /* ret 4 */

}


/**
 * qpr2_recovered_00012B50
 * Original: 0x00012B50 - 0x00012BFB (171 bytes, 73 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00012B50(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00012B50: ;
    RECOMP_PC(0x00012B50u);
    PUSH32(esp, ebp);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, esi);
    esi = MEM32(esp + 0xC);
    eax = MEM32(esi);
    ebp = ecx;
    PUSH32(esp, 0x1AE188);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0xC); PUSH32(esp, 0x00012B64u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00012B61u); } /* indirect call */
    }

loc_00012B64: ;
    RECOMP_PC(0x00012B64u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00012B7A; /* je: equal / zero */

loc_00012B68: ;
    RECOMP_PC(0x00012B68u);
    ecx = MEM32(ebp + 4);
    edx = MEM32(ecx);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, esi);
    { uint32_t _icall_target = MEM32(edx + 8); PUSH32(esp, 0x00012B71u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00012B6Eu); } /* indirect call */
    }

loc_00012B71: ;
    RECOMP_PC(0x00012B71u);
    eax = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0x10); PUSH32(esp, 0x00012B7Au); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00012B77u); } /* indirect call */
    }

loc_00012B7A: ;
    RECOMP_PC(0x00012B7Au);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x1AE17C);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0xC); PUSH32(esp, 0x00012B86u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00012B83u); } /* indirect call */
    }

loc_00012B86: ;
    RECOMP_PC(0x00012B86u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00012B9C; /* je: equal / zero */

loc_00012B8A: ;
    RECOMP_PC(0x00012B8Au);
    ecx = MEM32(ebp + 8);
    eax = MEM32(ecx);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, esi);
    { uint32_t _icall_target = MEM32(eax + 8); PUSH32(esp, 0x00012B93u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00012B90u); } /* indirect call */
    }

loc_00012B93: ;
    RECOMP_PC(0x00012B93u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x10); PUSH32(esp, 0x00012B9Cu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00012B99u); } /* indirect call */
    }

loc_00012B9C: ;
    RECOMP_PC(0x00012B9Cu);
    eax = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x1AE16C);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0xC); PUSH32(esp, 0x00012BA8u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00012BA5u); } /* indirect call */
    }

loc_00012BA8: ;
    RECOMP_PC(0x00012BA8u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00012BCF; /* je: equal / zero */

loc_00012BAC: ;
    RECOMP_PC(0x00012BACu);
    PUSH32(esp, ebx);
    PUSH32(esp, edi);
    edi = ebp + 0xC;
    ebx = 0x13;

loc_00012BB6: ;
    RECOMP_PC(0x00012BB6u);
    ecx = MEM32(edi);
    edx = MEM32(ecx);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, esi);
    { uint32_t _icall_target = MEM32(edx + 8); PUSH32(esp, 0x00012BBEu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00012BBBu); } /* indirect call */
    }

loc_00012BBE: ;
    RECOMP_PC(0x00012BBEu);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edi = edi + 4;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ebx--;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    if ((_fa != 0)) goto loc_00012BB6; /* jne: not equal / not zero */

loc_00012BC4: ;
    RECOMP_PC(0x00012BC4u);
    eax = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0x10); PUSH32(esp, 0x00012BCDu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00012BCAu); } /* indirect call */
    }

loc_00012BCD: ;
    RECOMP_PC(0x00012BCDu);
    POP32(esp, edi);
    POP32(esp, ebx);

loc_00012BCF: ;
    RECOMP_PC(0x00012BCFu);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x1AE160);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0xC); PUSH32(esp, 0x00012BDBu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00012BD8u); } /* indirect call */
    }

loc_00012BDB: ;
    RECOMP_PC(0x00012BDBu);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00012BF1; /* je: equal / zero */

loc_00012BDF: ;
    RECOMP_PC(0x00012BDFu);
    ecx = MEM32(ebp + 0x58);
    eax = MEM32(ecx);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, esi);
    { uint32_t _icall_target = MEM32(eax + 8); PUSH32(esp, 0x00012BE8u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00012BE5u); } /* indirect call */
    }

loc_00012BE8: ;
    RECOMP_PC(0x00012BE8u);
    edx = MEM32(esi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x10); PUSH32(esp, 0x00012BF1u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00012BEEu); } /* indirect call */
    }

loc_00012BF1: ;
    RECOMP_PC(0x00012BF1u);
    POP32(esp, esi);
    eax = 1;
    POP32(esp, ebp);
    esp += 8; return; /* ret 4 */

}

