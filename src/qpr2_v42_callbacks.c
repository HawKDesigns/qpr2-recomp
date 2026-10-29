/* Bounded routines recovered from the supplied retail disassembly. */

#include <stdio.h>
#define RECOMP_GENERATED_CODE

#include "recomp/gen/recomp_types.h"

#include "recomp/gen/recomp_funcs.h"

/* Bounded success evidence only; no change to guest object state. */
static void hud_ctor_done(uint32_t address,uint32_t object,uint32_t result)
{
    static unsigned counts[6];
    const uint32_t ids[6]={0x44010,0x3d060,0x43be0,0x45060,0x6b950,0x45510};
    for(unsigned i=0;i<6;i++)if(ids[i]==address) {
        unsigned n=++counts[i];
        if(n<=2)fprintf(stderr,"[HUD-CONSTRUCT] fn=%08X this=%08X return=%08X vtable=%08X count=%u\n",
            address,object,result,MEM32(object),n);
    }
}

void qpr2_recovered_00044010(void);

void qpr2_recovered_0003D060(void);

void qpr2_recovered_00043BE0(void);

void qpr2_recovered_00045060(void);

void qpr2_recovered_0006B950(void);

void qpr2_recovered_00045510(void);

/**
 * qpr2_recovered_00044010
 * Original: 0x00044010 - 0x00044321 (785 bytes, 228 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00044010(void)
{
    uint32_t ctor_this=ecx;
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

loc_00044010: ;
    RECOMP_PC(0x00044010u);
    _fb = (uint32_t)(0x38) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x38;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    eax = esp + 0xC;
    PUSH32(esp, eax);
    ebx = ecx;
    MEM32(esp + 0x10) = 0x262;
    PUSH32(esp, 0x00044029u); RECOMP_ABI_CALL(0x0003CFD0u, sub_0003CFD0); /* call 0x0003CFD0 */

loc_00044029: ;
    RECOMP_PC(0x00044029u);
    PUSH32(esp, 0x19);
    MEM32(ebx) = 0x1B3A10;
    PUSH32(esp, 0x00044036u); RECOMP_ABI_CALL(0x000367C0u, sub_000367C0); /* call 0x000367C0 */

loc_00044036: ;
    RECOMP_PC(0x00044036u);
    MEMF(esp + 0x1C) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x1E0);
    PUSH32(esp, 0x00044044u); RECOMP_ABI_CALL(0x000367C0u, sub_000367C0); /* call 0x000367C0 */

loc_00044044: ;
    RECOMP_PC(0x00044044u);
    MEMF(esp + 0x24) = (float)fp_top(); fp_pop(); /* fstp */
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    ecx = esp + 0x1C;
    MEM32(ebx + 0x2C) = 0xBF800000u;
    PUSH32(esp, 0x0004406Fu); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_0004406F: ;
    RECOMP_PC(0x0004406Fu);
    ecx = MEM32(esp + 0xC);
    PUSH32(esp, ecx);
    PUSH32(esp, 0x1AE9EC);
    ecx = ebx;
    PUSH32(esp, 0x00044080u); RECOMP_ABI_CALL(0x000427F0u, sub_000427F0); /* call 0x000427F0 */

loc_00044080: ;
    RECOMP_PC(0x00044080u);
    edx = MEM32(0x1D8090);
    PUSH32(esp, 0x84);
    MEM32(esp + 0xC) = edx;
    PUSH32(esp, 0x00044094u); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_00044094: ;
    RECOMP_PC(0x00044094u);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000440C7; /* je: equal / zero */

loc_0004409B: ;
    RECOMP_PC(0x0004409Bu);
    ecx = MEM32(0x1D820C);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    PUSH32(esp, 0);
    PUSH32(esp, ecx);
    PUSH32(esp, 0x1B36B8);
    PUSH32(esp, 0x1B3770);
    edx = esp + 0x20;
    PUSH32(esp, edx);
    PUSH32(esp, 0x1D8088);
    ecx = eax;
    PUSH32(esp, 0x000440C3u); RECOMP_ABI_CALL(0x00035F10u, sub_00035F10); /* call 0x00035F10 */

loc_000440C3: ;
    RECOMP_PC(0x000440C3u);
    esi = eax;
    goto loc_000440C9;

loc_000440C7: ;
    RECOMP_PC(0x000440C7u);
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_000440C9: ;
    RECOMP_PC(0x000440C9u);
    ecx = MEM32(0x1DB454);
    PUSH32(esp, 0x9B);
    PUSH32(esp, 0x000440D9u); RECOMP_ABI_CALL(0x000E0830u, sub_000E0830); /* call 0x000E0830 */

loc_000440D9: ;
    RECOMP_PC(0x000440D9u);
    MEM32(esp + 0xC) = eax;
    eax = esp + 0xC;
    PUSH32(esp, eax);
    ecx = esi;
    PUSH32(esp, 0x000440E9u); RECOMP_ABI_CALL(0x0004D080u, sub_0004D080); /* call 0x0004D080 */

loc_000440E9: ;
    RECOMP_PC(0x000440E9u);
    ecx = MEM32(0x1D8228);
    edx = MEM32(0x1D8218);
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    ecx = esi;
    PUSH32(esp, 0x000440FEu); RECOMP_ABI_CALL(0x0004C170u, sub_0004C170); /* call 0x0004C170 */

loc_000440FE: ;
    RECOMP_PC(0x000440FEu);
    PUSH32(esp, 0);
    PUSH32(esp, esi);
    ecx = ebx;
    PUSH32(esp, 0x00044108u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_00044108: ;
    RECOMP_PC(0x00044108u);
    PUSH32(esp, 0x84);
    PUSH32(esp, 0x00044112u); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_00044112: ;
    RECOMP_PC(0x00044112u);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00044145; /* je: equal / zero */

loc_00044119: ;
    RECOMP_PC(0x00044119u);
    ecx = MEM32(0x1D820C);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    PUSH32(esp, 1);
    PUSH32(esp, ecx);
    PUSH32(esp, 0x1B36B8);
    PUSH32(esp, 0x1B3770);
    edx = esp + 0x20;
    PUSH32(esp, edx);
    ecx = esp + 0x38;
    PUSH32(esp, ecx);
    ecx = eax;
    PUSH32(esp, 0x00044141u); RECOMP_ABI_CALL(0x00035F10u, sub_00035F10); /* call 0x00035F10 */

loc_00044141: ;
    RECOMP_PC(0x00044141u);
    esi = eax;
    goto loc_00044147;

loc_00044145: ;
    RECOMP_PC(0x00044145u);
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_00044147: ;
    RECOMP_PC(0x00044147u);
    ecx = MEM32(0x1DB454);
    PUSH32(esp, 0x2CA);
    PUSH32(esp, 0x00044157u); RECOMP_ABI_CALL(0x000E0830u, sub_000E0830); /* call 0x000E0830 */

loc_00044157: ;
    RECOMP_PC(0x00044157u);
    edx = esp + 0xC;
    PUSH32(esp, edx);
    ecx = esi;
    MEM32(esp + 0x10) = eax;
    PUSH32(esp, 0x00044167u); RECOMP_ABI_CALL(0x0004D080u, sub_0004D080); /* call 0x0004D080 */

loc_00044167: ;
    RECOMP_PC(0x00044167u);
    eax = MEM32(0x1D8228);
    ecx = MEM32(0x1D8218);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = esi;
    PUSH32(esp, 0x0004417Bu); RECOMP_ABI_CALL(0x0004C170u, sub_0004C170); /* call 0x0004C170 */

loc_0004417B: ;
    RECOMP_PC(0x0004417Bu);
    PUSH32(esp, 0);
    PUSH32(esp, esi);
    ecx = ebx;
    PUSH32(esp, 0x00044185u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_00044185: ;
    RECOMP_PC(0x00044185u);
    ecx = ebx;
    PUSH32(esp, 0x0004418Cu); RECOMP_ABI_CALL(0x00042BC0u, sub_00042BC0); /* call 0x00042BC0 */

loc_0004418C: ;
    RECOMP_PC(0x0004418Cu);
    MEMF(esp + 8) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(0x1D9FB8);
    eax = MEM32(edx + 0x10);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    MEM32(esp + 0xC) = 0;
    if (CMP_BE(_fa, _fb)) goto loc_00044312; /* jbe: below or equal (unsigned <=) */

loc_000441A9: ;
    RECOMP_PC(0x000441A9u);
    PUSH32(esp, ebp);
    PUSH32(esp, edi);
    ebp = ebx + 0x25C;

loc_000441B1: ;
    RECOMP_PC(0x000441B1u);
    fp_push((double)SMEM32(esp + 0x14)); /* fild */
    PUSH32(esp, 0x3D4CCCCD);
    ecx = esp + 0x20;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1AF3A0)); /* fmul dword ptr [0x1af3a0] */
    fp_top() = RECOMP_FP_PC(MEMF(0x1B22F0) - fp_top()); /* fsubr dword ptr [0x1b22f0] */
    MEMF(esp + 0x1C) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esp + 0x1C);
    PUSH32(esp, eax);
    PUSH32(esp, eax);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x000441DEu); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_000441DE: ;
    RECOMP_PC(0x000441DEu);
    PUSH32(esp, 0x84);
    PUSH32(esp, 0x000441E8u); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_000441E8: ;
    RECOMP_PC(0x000441E8u);
    edi = MEM32(esp + 0x20);
    esi = eax;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00044244; /* je: equal / zero */

loc_000441F5: ;
    RECOMP_PC(0x000441F5u);
    ecx = MEM32(esp + 0x10);
    eax = MEM32(esp + 0x20);
    PUSH32(esp, 0x1B36A8);
    edx = esp + 0x2C;
    MEM32(esp + 0x30) = ecx;
    PUSH32(esp, edx);
    ecx = esi;
    MEM32(esp + 0x30) = eax;
    MEM32(esp + 0x38) = 0;
    MEM32(esp + 0x3C) = 0;
    PUSH32(esp, 0x00044226u); RECOMP_ABI_CALL(0x00110B60u, sub_00110B60); /* call 0x00110B60 */

loc_00044226: ;
    RECOMP_PC(0x00044226u);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    MEM32(esi) = 0x1B0D80;
    eax = MEM32(0x1B3774);
    PUSH32(esp, 0);
    PUSH32(esp, eax);
    PUSH32(esp, edi);
    ecx = esi;
    PUSH32(esp, 0x00044240u); RECOMP_ABI_CALL(0x0004CF80u, sub_0004CF80); /* call 0x0004CF80 */

loc_00044240: ;
    RECOMP_PC(0x00044240u);
    ecx = esi;
    goto loc_00044246;

loc_00044244: ;
    RECOMP_PC(0x00044244u);
    ecx = 0; /* xor self */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_00044246: ;
    RECOMP_PC(0x00044246u);
    MEM32(ebp + -32) = ecx;
    edx = MEM32(0x1D8240);
    PUSH32(esp, edx);
    PUSH32(esp, edi);
    PUSH32(esp, 0x00044256u); RECOMP_ABI_CALL(0x0004C170u, sub_0004C170); /* call 0x0004C170 */

loc_00044256: ;
    RECOMP_PC(0x00044256u);
    eax = MEM32(ebp + -32);
    PUSH32(esp, 0);
    PUSH32(esp, eax);
    ecx = ebx;
    PUSH32(esp, 0x00044263u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_00044263: ;
    RECOMP_PC(0x00044263u);
    PUSH32(esp, 0x84);
    PUSH32(esp, 0x0004426Du); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_0004426D: ;
    RECOMP_PC(0x0004426Du);
    esi = eax;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000442C6; /* je: equal / zero */

loc_00044276: ;
    RECOMP_PC(0x00044276u);
    ecx = MEM32(esp + 0x24);
    edx = MEM32(esp + 0x10);
    PUSH32(esp, 0x1B3698);
    eax = esp + 0x3C;
    MEM32(esp + 0x3C) = ecx;
    PUSH32(esp, eax);
    ecx = esi;
    MEM32(esp + 0x44) = edx;
    MEM32(esp + 0x48) = 0;
    MEM32(esp + 0x4C) = 0;
    PUSH32(esp, 0x000442A7u); RECOMP_ABI_CALL(0x00110B60u, sub_00110B60); /* call 0x00110B60 */

loc_000442A7: ;
    RECOMP_PC(0x000442A7u);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    MEM32(esi) = 0x1B0D80;
    ecx = MEM32(0x1B3774);
    PUSH32(esp, 1);
    PUSH32(esp, ecx);
    PUSH32(esp, edi);
    ecx = esi;
    PUSH32(esp, 0x000442C2u); RECOMP_ABI_CALL(0x0004CF80u, sub_0004CF80); /* call 0x0004CF80 */

loc_000442C2: ;
    RECOMP_PC(0x000442C2u);
    ecx = esi;
    goto loc_000442C8;

loc_000442C6: ;
    RECOMP_PC(0x000442C6u);
    ecx = 0; /* xor self */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_000442C8: ;
    RECOMP_PC(0x000442C8u);
    MEM32(ebp) = ecx;
    edx = MEM32(0x1D8240);
    PUSH32(esp, edx);
    PUSH32(esp, edi);
    PUSH32(esp, 0x000442D8u); RECOMP_ABI_CALL(0x0004C170u, sub_0004C170); /* call 0x0004C170 */

loc_000442D8: ;
    RECOMP_PC(0x000442D8u);
    eax = MEM32(ebp);
    PUSH32(esp, 0);
    PUSH32(esp, eax);
    ecx = ebx;
    PUSH32(esp, 0x000442E5u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_000442E5: ;
    RECOMP_PC(0x000442E5u);
    fp_push(MEMF(0x1D8084)); /* fld float */
    eax = MEM32(esp + 0x14);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esp + 0x10)); /* fadd dword ptr [esp + 0x10] */
    ecx = MEM32(0x1D9FB8);
    edx = MEM32(ecx + 0x10);
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    MEMF(esp + 0x10) = (float)fp_top(); fp_pop(); /* fstp */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebp = ebp + 4;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edx (32-bit) */
    MEM32(esp + 0x14) = eax;
    if (CMP_B(_fa, _fb)) goto loc_000441B1; /* jb: below (unsigned <) */

loc_00044310: ;
    RECOMP_PC(0x00044310u);
    POP32(esp, edi);
    POP32(esp, ebp);

loc_00044312: ;
    RECOMP_PC(0x00044312u);
    ecx = ebx;
    PUSH32(esp, 0x00044319u); RECOMP_ABI_CALL(0x000430F0u, sub_000430F0); /* call 0x000430F0 */

loc_00044319: ;
    RECOMP_PC(0x00044319u);
    POP32(esp, esi);
    eax = ebx;
    POP32(esp, ebx);
    _fb = (uint32_t)(0x38) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x38;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    hud_ctor_done(0x00044010u,ctor_this,eax);
    esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_0003D060
 * Original: 0x0003D060 - 0x0003D18A (298 bytes, 91 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_0003D060(void)
{
    uint32_t ctor_this=ecx;
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;

loc_0003D060: ;
    RECOMP_PC(0x0003D060u);
    PUSH32(esp, ecx);
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    eax = esp + 0xC;
    PUSH32(esp, eax);
    esi = ecx;
    MEM32(esp + 0x10) = 0x262;
    PUSH32(esp, 0x0003D078u); RECOMP_ABI_CALL(0x0003CFD0u, sub_0003CFD0); /* call 0x0003CFD0 */

loc_0003D078: ;
    RECOMP_PC(0x0003D078u);
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ecx = esi;
    MEM32(esi) = 0x1B2A58;
    MEM32(esi + 0x1580) = ebx;
    PUSH32(esp, 0x0003D08Du); RECOMP_ABI_CALL(0x0004E580u, sub_0004E580); /* call 0x0004E580 */

loc_0003D08D: ;
    RECOMP_PC(0x0003D08Du);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    ecx = esp + 0x1C;
    MEM32(esi + 0x2C) = 0xBF800000u;
    PUSH32(esp, 0x0003D0B1u); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_0003D0B1: ;
    RECOMP_PC(0x0003D0B1u);
    ecx = MEM32(esp + 0xC);
    PUSH32(esp, ecx);
    PUSH32(esp, 0x1AE9EC);
    ecx = esi;
    PUSH32(esp, 0x0003D0C2u); RECOMP_ABI_CALL(0x000427F0u, sub_000427F0); /* call 0x000427F0 */

loc_0003D0C2: ;
    RECOMP_PC(0x0003D0C2u);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_0003D0C4: ;
    RECOMP_PC(0x0003D0C4u);
    PUSH32(esp, edi);
    ecx = esi;
    PUSH32(esp, 0x0003D0CCu); RECOMP_ABI_CALL(0x0003BEE0u, sub_0003BEE0); /* call 0x0003BEE0 */

loc_0003D0CC: ;
    RECOMP_PC(0x0003D0CCu);
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(9) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 9 (32-bit) */
    if (CMP_B(_fa, _fb)) goto loc_0003D0C4; /* jb: below (unsigned <) */

loc_0003D0D2: ;
    RECOMP_PC(0x0003D0D2u);
    edx = MEM32(0x1D9FB8);
    _fa = (uint32_t)(MEM32(edx + 4)) & 0xFFFFFFFFu; _fb = (uint32_t)(1) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(edx + 4), 1 (32-bit) */
    if (CMP_A(_fa, _fb)) goto loc_0003D101; /* ja: above (unsigned >) */

loc_0003D0DE: ;
    RECOMP_PC(0x0003D0DEu);
    _fa = (uint32_t)(MEM32(0x1D743C)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1D743C), ebx (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_0003D101; /* jne: not equal / not zero */

loc_0003D0E6: ;
    RECOMP_PC(0x0003D0E6u);
    ecx = MEM32(0x1DB454);
    PUSH32(esp, 0x270);
    PUSH32(esp, 0x0003D0F6u); RECOMP_ABI_CALL(0x000E0830u, sub_000E0830); /* call 0x000E0830 */

loc_0003D0F6: ;
    RECOMP_PC(0x0003D0F6u);
    MEM32(esp + 0xC) = eax;
    eax = esp + 0xC;
    PUSH32(esp, eax);
    goto loc_0003D10A;

loc_0003D101: ;
    RECOMP_PC(0x0003D101u);
    ecx = esp + 0xC;
    MEM32(esp + 0xC) = ebx;
    PUSH32(esp, ecx);

loc_0003D10A: ;
    RECOMP_PC(0x0003D10Au);
    PUSH32(esp, ebx);
    PUSH32(esp, 0x0003D110u); RECOMP_ABI_CALL(0x0004E570u, sub_0004E570); /* call 0x0004E570 */

loc_0003D110: ;
    RECOMP_PC(0x0003D110u);
    ecx = eax;
    PUSH32(esp, 0x0003D117u); RECOMP_ABI_CALL(0x00049040u, sub_00049040); /* call 0x00049040 */

loc_0003D117: ;
    RECOMP_PC(0x0003D117u);
    edx = esp + 0xC;
    PUSH32(esp, edx);
    PUSH32(esp, 1);
    MEM32(esp + 0x14) = ebx;
    PUSH32(esp, 0x0003D127u); RECOMP_ABI_CALL(0x0004E570u, sub_0004E570); /* call 0x0004E570 */

loc_0003D127: ;
    RECOMP_PC(0x0003D127u);
    ecx = eax;
    PUSH32(esp, 0x0003D12Eu); RECOMP_ABI_CALL(0x00049040u, sub_00049040); /* call 0x00049040 */

loc_0003D12E: ;
    RECOMP_PC(0x0003D12Eu);
    eax = esp + 0xC;
    PUSH32(esp, eax);
    PUSH32(esp, 3);
    MEM32(esp + 0x14) = ebx;
    PUSH32(esp, 0x0003D13Eu); RECOMP_ABI_CALL(0x0004E570u, sub_0004E570); /* call 0x0004E570 */

loc_0003D13E: ;
    RECOMP_PC(0x0003D13Eu);
    ecx = eax;
    PUSH32(esp, 0x0003D145u); RECOMP_ABI_CALL(0x00049040u, sub_00049040); /* call 0x00049040 */

loc_0003D145: ;
    RECOMP_PC(0x0003D145u);
    ecx = MEM32(0x1DB454);
    PUSH32(esp, 0x9F);
    PUSH32(esp, 0x0003D155u); RECOMP_ABI_CALL(0x000E0830u, sub_000E0830); /* call 0x000E0830 */

loc_0003D155: ;
    RECOMP_PC(0x0003D155u);
    ecx = esp + 0xC;
    PUSH32(esp, ecx);
    PUSH32(esp, 2);
    MEM32(esp + 0x14) = eax;
    PUSH32(esp, 0x0003D165u); RECOMP_ABI_CALL(0x0004E570u, sub_0004E570); /* call 0x0004E570 */

loc_0003D165: ;
    RECOMP_PC(0x0003D165u);
    ecx = eax;
    PUSH32(esp, 0x0003D16Cu); RECOMP_ABI_CALL(0x00049040u, sub_00049040); /* call 0x00049040 */

loc_0003D16C: ;
    RECOMP_PC(0x0003D16Cu);
    PUSH32(esp, ebx);
    PUSH32(esp, ebx);
    PUSH32(esp, ebx);
    PUSH32(esp, ebx);
    PUSH32(esp, 0x0003D175u); RECOMP_ABI_CALL(0x0004E570u, sub_0004E570); /* call 0x0004E570 */

loc_0003D175: ;
    RECOMP_PC(0x0003D175u);
    ecx = eax;
    PUSH32(esp, 0x0003D17Cu); RECOMP_ABI_CALL(0x00049000u, sub_00049000); /* call 0x00049000 */

loc_0003D17C: ;
    RECOMP_PC(0x0003D17Cu);
    ecx = esi;
    PUSH32(esp, 0x0003D183u); RECOMP_ABI_CALL(0x000430F0u, sub_000430F0); /* call 0x000430F0 */

loc_0003D183: ;
    RECOMP_PC(0x0003D183u);
    POP32(esp, edi);
    eax = esi;
    POP32(esp, esi);
    POP32(esp, ebx);
    POP32(esp, ecx);
    hud_ctor_done(0x0003D060u,ctor_this,eax);
    esp += 4; return; /* ret */

}


/**
 * qpr2_recovered_00043BE0
 * Original: 0x00043BE0 - 0x00044001 (1057 bytes, 310 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00043BE0(void)
{
    uint32_t ctor_this=ecx;
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

loc_00043BE0: ;
    RECOMP_PC(0x00043BE0u);
    _fb = (uint32_t)(0x4C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x4C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    eax = esp + 0xC;
    PUSH32(esp, eax);
    ebp = ecx;
    MEM32(esp + 0x10) = 0x262;
    PUSH32(esp, 0x00043BF9u); RECOMP_ABI_CALL(0x0003CFD0u, sub_0003CFD0); /* call 0x0003CFD0 */

loc_00043BF9: ;
    RECOMP_PC(0x00043BF9u);
    PUSH32(esp, 0x19);
    MEM32(ebp) = 0x1B3970;
    PUSH32(esp, 0x00043C07u); RECOMP_ABI_CALL(0x000367C0u, sub_000367C0); /* call 0x000367C0 */

loc_00043C07: ;
    RECOMP_PC(0x00043C07u);
    MEMF(esp + 0x1C) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x193);
    PUSH32(esp, 0x00043C15u); RECOMP_ABI_CALL(0x000367C0u, sub_000367C0); /* call 0x000367C0 */

loc_00043C15: ;
    RECOMP_PC(0x00043C15u);
    MEMF(esp + 0x24) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x226);
    PUSH32(esp, 0x00043C23u); RECOMP_ABI_CALL(0x000367C0u, sub_000367C0); /* call 0x000367C0 */

loc_00043C23: ;
    RECOMP_PC(0x00043C23u);
    MEMF(esp + 0x2C) = (float)fp_top(); fp_pop(); /* fstp */
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    ecx = esp + 0x1C;
    MEM32(ebp + 0x2C) = 0xBF800000u;
    PUSH32(esp, 0x00043C4Eu); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_00043C4E: ;
    RECOMP_PC(0x00043C4Eu);
    ecx = MEM32(esp + 0xC);
    PUSH32(esp, ecx);
    PUSH32(esp, 0x1AE9EC);
    ecx = ebp;
    PUSH32(esp, 0x00043C5Fu); RECOMP_ABI_CALL(0x000427F0u, sub_000427F0); /* call 0x000427F0 */

loc_00043C5F: ;
    RECOMP_PC(0x00043C5Fu);
    edx = MEM32(0x1D8090);
    PUSH32(esp, 0x84);
    MEM32(esp + 0xC) = edx;
    PUSH32(esp, 0x00043C73u); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_00043C73: ;
    RECOMP_PC(0x00043C73u);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00043CA6; /* je: equal / zero */

loc_00043C7A: ;
    RECOMP_PC(0x00043C7Au);
    ecx = MEM32(0x1D820C);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    PUSH32(esp, 0);
    PUSH32(esp, ecx);
    PUSH32(esp, 0x1B36B8);
    PUSH32(esp, 0x1B3770);
    edx = esp + 0x20;
    PUSH32(esp, edx);
    PUSH32(esp, 0x1D8088);
    ecx = eax;
    PUSH32(esp, 0x00043CA2u); RECOMP_ABI_CALL(0x00035F10u, sub_00035F10); /* call 0x00035F10 */

loc_00043CA2: ;
    RECOMP_PC(0x00043CA2u);
    esi = eax;
    goto loc_00043CA8;

loc_00043CA6: ;
    RECOMP_PC(0x00043CA6u);
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_00043CA8: ;
    RECOMP_PC(0x00043CA8u);
    ecx = MEM32(0x1DB454);
    PUSH32(esp, 0x24B);
    PUSH32(esp, 0x00043CB8u); RECOMP_ABI_CALL(0x000E0830u, sub_000E0830); /* call 0x000E0830 */

loc_00043CB8: ;
    RECOMP_PC(0x00043CB8u);
    MEM32(esp + 0xC) = eax;
    eax = esp + 0xC;
    PUSH32(esp, eax);
    ecx = esi;
    PUSH32(esp, 0x00043CC8u); RECOMP_ABI_CALL(0x0004D080u, sub_0004D080); /* call 0x0004D080 */

loc_00043CC8: ;
    RECOMP_PC(0x00043CC8u);
    ecx = MEM32(0x1D8228);
    edx = MEM32(0x1D8218);
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    ecx = esi;
    PUSH32(esp, 0x00043CDDu); RECOMP_ABI_CALL(0x0004C170u, sub_0004C170); /* call 0x0004C170 */

loc_00043CDD: ;
    RECOMP_PC(0x00043CDDu);
    PUSH32(esp, 0);
    PUSH32(esp, esi);
    ecx = ebp;
    PUSH32(esp, 0x00043CE7u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_00043CE7: ;
    RECOMP_PC(0x00043CE7u);
    PUSH32(esp, 0x84);
    PUSH32(esp, 0x00043CF1u); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_00043CF1: ;
    RECOMP_PC(0x00043CF1u);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00043D24; /* je: equal / zero */

loc_00043CF8: ;
    RECOMP_PC(0x00043CF8u);
    ecx = MEM32(0x1D820C);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    PUSH32(esp, 1);
    PUSH32(esp, ecx);
    PUSH32(esp, 0x1B36B8);
    PUSH32(esp, 0x1B3770);
    edx = esp + 0x20;
    PUSH32(esp, edx);
    ecx = esp + 0x38;
    PUSH32(esp, ecx);
    ecx = eax;
    PUSH32(esp, 0x00043D20u); RECOMP_ABI_CALL(0x00035F10u, sub_00035F10); /* call 0x00035F10 */

loc_00043D20: ;
    RECOMP_PC(0x00043D20u);
    esi = eax;
    goto loc_00043D26;

loc_00043D24: ;
    RECOMP_PC(0x00043D24u);
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_00043D26: ;
    RECOMP_PC(0x00043D26u);
    ecx = MEM32(0x1DB454);
    PUSH32(esp, 0x2CA);
    PUSH32(esp, 0x00043D36u); RECOMP_ABI_CALL(0x000E0830u, sub_000E0830); /* call 0x000E0830 */

loc_00043D36: ;
    RECOMP_PC(0x00043D36u);
    edx = esp + 0xC;
    PUSH32(esp, edx);
    ecx = esi;
    MEM32(esp + 0x10) = eax;
    PUSH32(esp, 0x00043D46u); RECOMP_ABI_CALL(0x0004D080u, sub_0004D080); /* call 0x0004D080 */

loc_00043D46: ;
    RECOMP_PC(0x00043D46u);
    eax = MEM32(0x1D8228);
    ecx = MEM32(0x1D8218);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = esi;
    PUSH32(esp, 0x00043D5Au); RECOMP_ABI_CALL(0x0004C170u, sub_0004C170); /* call 0x0004C170 */

loc_00043D5A: ;
    RECOMP_PC(0x00043D5Au);
    PUSH32(esp, 0);
    PUSH32(esp, esi);
    ecx = ebp;
    PUSH32(esp, 0x00043D64u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_00043D64: ;
    RECOMP_PC(0x00043D64u);
    PUSH32(esp, 0x84);
    PUSH32(esp, 0x00043D6Eu); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_00043D6E: ;
    RECOMP_PC(0x00043D6Eu);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00043DA1; /* je: equal / zero */

loc_00043D75: ;
    RECOMP_PC(0x00043D75u);
    edx = MEM32(0x1D820C);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    PUSH32(esp, 1);
    PUSH32(esp, edx);
    PUSH32(esp, 0x1B36B8);
    PUSH32(esp, 0x1B3770);
    ecx = esp + 0x20;
    PUSH32(esp, ecx);
    edx = esp + 0x3C;
    PUSH32(esp, edx);
    ecx = eax;
    PUSH32(esp, 0x00043D9Du); RECOMP_ABI_CALL(0x00035F10u, sub_00035F10); /* call 0x00035F10 */

loc_00043D9D: ;
    RECOMP_PC(0x00043D9Du);
    esi = eax;
    goto loc_00043DA3;

loc_00043DA1: ;
    RECOMP_PC(0x00043DA1u);
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_00043DA3: ;
    RECOMP_PC(0x00043DA3u);
    ecx = MEM32(0x1DB454);
    PUSH32(esp, 0x248);
    PUSH32(esp, 0x00043DB3u); RECOMP_ABI_CALL(0x000E0830u, sub_000E0830); /* call 0x000E0830 */

loc_00043DB3: ;
    RECOMP_PC(0x00043DB3u);
    MEM32(esp + 0xC) = eax;
    eax = esp + 0xC;
    PUSH32(esp, eax);
    ecx = esi;
    PUSH32(esp, 0x00043DC3u); RECOMP_ABI_CALL(0x0004D080u, sub_0004D080); /* call 0x0004D080 */

loc_00043DC3: ;
    RECOMP_PC(0x00043DC3u);
    ecx = MEM32(0x1D8228);
    edx = MEM32(0x1D8218);
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    ecx = esi;
    PUSH32(esp, 0x00043DD8u); RECOMP_ABI_CALL(0x0004C170u, sub_0004C170); /* call 0x0004C170 */

loc_00043DD8: ;
    RECOMP_PC(0x00043DD8u);
    PUSH32(esp, 0);
    PUSH32(esp, esi);
    ecx = ebp;
    PUSH32(esp, 0x00043DE2u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_00043DE2: ;
    RECOMP_PC(0x00043DE2u);
    ecx = ebp;
    PUSH32(esp, 0x00043DE9u); RECOMP_ABI_CALL(0x00042BC0u, sub_00042BC0); /* call 0x00042BC0 */

loc_00043DE9: ;
    RECOMP_PC(0x00043DE9u);
    MEMF(esp + 8) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(0x1D9FB8);
    ecx = MEM32(eax + 0x10);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, 0 (32-bit) */
    MEM32(esp + 0xC) = 0;
    if (CMP_BE(_fa, _fb)) goto loc_00043FF2; /* jbe: below or equal (unsigned <=) */

loc_00043E05: ;
    RECOMP_PC(0x00043E05u);
    PUSH32(esp, ebx);
    PUSH32(esp, edi);
    ebx = ebp + 0x25C;
    /* nop */

loc_00043E10: ;
    RECOMP_PC(0x00043E10u);
    fp_push((double)SMEM32(esp + 0x14)); /* fild */
    PUSH32(esp, 0x3D4CCCCD);
    ecx = esp + 0x20;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1AF3A0)); /* fmul dword ptr [0x1af3a0] */
    fp_top() = RECOMP_FP_PC(MEMF(0x1B22F0) - fp_top()); /* fsubr dword ptr [0x1b22f0] */
    MEMF(esp + 0x1C) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esp + 0x1C);
    PUSH32(esp, eax);
    PUSH32(esp, eax);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x00043E3Du); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_00043E3D: ;
    RECOMP_PC(0x00043E3Du);
    PUSH32(esp, 0x84);
    PUSH32(esp, 0x00043E47u); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_00043E47: ;
    RECOMP_PC(0x00043E47u);
    edi = MEM32(esp + 0x20);
    esi = eax;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00043EA2; /* je: equal / zero */

loc_00043E54: ;
    RECOMP_PC(0x00043E54u);
    ecx = MEM32(esp + 0x20);
    edx = MEM32(esp + 0x10);
    PUSH32(esp, 0x1B36A8);
    eax = esp + 0x30;
    MEM32(esp + 0x30) = ecx;
    PUSH32(esp, eax);
    ecx = esi;
    MEM32(esp + 0x38) = edx;
    MEM32(esp + 0x3C) = 0;
    MEM32(esp + 0x40) = 0;
    PUSH32(esp, 0x00043E85u); RECOMP_ABI_CALL(0x00110B60u, sub_00110B60); /* call 0x00110B60 */

loc_00043E85: ;
    RECOMP_PC(0x00043E85u);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    MEM32(esi) = 0x1B0D80;
    ecx = MEM32(0x1B3774);
    PUSH32(esp, 0);
    PUSH32(esp, ecx);
    PUSH32(esp, edi);
    ecx = esi;
    PUSH32(esp, 0x00043EA0u); RECOMP_ABI_CALL(0x0004CF80u, sub_0004CF80); /* call 0x0004CF80 */

loc_00043EA0: ;
    RECOMP_PC(0x00043EA0u);
    goto loc_00043EA4;

loc_00043EA2: ;
    RECOMP_PC(0x00043EA2u);
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_00043EA4: ;
    RECOMP_PC(0x00043EA4u);
    MEM32(ebx + -32) = esi;
    edx = MEM32(0x1D8240);
    PUSH32(esp, edx);
    PUSH32(esp, edi);
    ecx = esi;
    PUSH32(esp, 0x00043EB6u); RECOMP_ABI_CALL(0x0004C170u, sub_0004C170); /* call 0x0004C170 */

loc_00043EB6: ;
    RECOMP_PC(0x00043EB6u);
    eax = MEM32(ebx + -32);
    PUSH32(esp, 0);
    PUSH32(esp, eax);
    ecx = ebp;
    PUSH32(esp, 0x00043EC3u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_00043EC3: ;
    RECOMP_PC(0x00043EC3u);
    PUSH32(esp, 0x84);
    PUSH32(esp, 0x00043ECDu); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_00043ECD: ;
    RECOMP_PC(0x00043ECDu);
    esi = eax;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00043F26; /* je: equal / zero */

loc_00043ED6: ;
    RECOMP_PC(0x00043ED6u);
    ecx = MEM32(esp + 0x24);
    edx = MEM32(esp + 0x10);
    PUSH32(esp, 0x1B3698);
    eax = esp + 0x40;
    MEM32(esp + 0x40) = ecx;
    PUSH32(esp, eax);
    ecx = esi;
    MEM32(esp + 0x48) = edx;
    MEM32(esp + 0x4C) = 0;
    MEM32(esp + 0x50) = 0;
    PUSH32(esp, 0x00043F07u); RECOMP_ABI_CALL(0x00110B60u, sub_00110B60); /* call 0x00110B60 */

loc_00043F07: ;
    RECOMP_PC(0x00043F07u);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    MEM32(esi) = 0x1B0D80;
    ecx = MEM32(0x1B3774);
    PUSH32(esp, 1);
    PUSH32(esp, ecx);
    PUSH32(esp, edi);
    ecx = esi;
    PUSH32(esp, 0x00043F22u); RECOMP_ABI_CALL(0x0004CF80u, sub_0004CF80); /* call 0x0004CF80 */

loc_00043F22: ;
    RECOMP_PC(0x00043F22u);
    ecx = esi;
    goto loc_00043F28;

loc_00043F26: ;
    RECOMP_PC(0x00043F26u);
    ecx = 0; /* xor self */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_00043F28: ;
    RECOMP_PC(0x00043F28u);
    MEM32(ebx) = ecx;
    edx = MEM32(0x1D8240);
    PUSH32(esp, edx);
    PUSH32(esp, edi);
    PUSH32(esp, 0x00043F37u); RECOMP_ABI_CALL(0x0004C170u, sub_0004C170); /* call 0x0004C170 */

loc_00043F37: ;
    RECOMP_PC(0x00043F37u);
    eax = MEM32(ebx);
    PUSH32(esp, 0);
    PUSH32(esp, eax);
    ecx = ebp;
    PUSH32(esp, 0x00043F43u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_00043F43: ;
    RECOMP_PC(0x00043F43u);
    PUSH32(esp, 0x84);
    PUSH32(esp, 0x00043F4Du); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_00043F4D: ;
    RECOMP_PC(0x00043F4Du);
    esi = eax;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00043FA6; /* je: equal / zero */

loc_00043F56: ;
    RECOMP_PC(0x00043F56u);
    ecx = MEM32(esp + 0x28);
    edx = MEM32(esp + 0x10);
    PUSH32(esp, 0x1B3958);
    eax = esp + 0x50;
    MEM32(esp + 0x50) = ecx;
    PUSH32(esp, eax);
    ecx = esi;
    MEM32(esp + 0x58) = edx;
    MEM32(esp + 0x5C) = 0;
    MEM32(esp + 0x60) = 0;
    PUSH32(esp, 0x00043F87u); RECOMP_ABI_CALL(0x00110B60u, sub_00110B60); /* call 0x00110B60 */

loc_00043F87: ;
    RECOMP_PC(0x00043F87u);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    MEM32(esi) = 0x1B0D80;
    ecx = MEM32(0x1B3774);
    PUSH32(esp, 1);
    PUSH32(esp, ecx);
    PUSH32(esp, edi);
    ecx = esi;
    PUSH32(esp, 0x00043FA2u); RECOMP_ABI_CALL(0x0004CF80u, sub_0004CF80); /* call 0x0004CF80 */

loc_00043FA2: ;
    RECOMP_PC(0x00043FA2u);
    ecx = esi;
    goto loc_00043FA8;

loc_00043FA6: ;
    RECOMP_PC(0x00043FA6u);
    ecx = 0; /* xor self */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_00043FA8: ;
    RECOMP_PC(0x00043FA8u);
    MEM32(ebx + 0x20) = ecx;
    edx = MEM32(0x1D8240);
    PUSH32(esp, edx);
    PUSH32(esp, edi);
    PUSH32(esp, 0x00043FB8u); RECOMP_ABI_CALL(0x0004C170u, sub_0004C170); /* call 0x0004C170 */

loc_00043FB8: ;
    RECOMP_PC(0x00043FB8u);
    eax = MEM32(ebx + 0x20);
    PUSH32(esp, 0);
    PUSH32(esp, eax);
    ecx = ebp;
    PUSH32(esp, 0x00043FC5u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_00043FC5: ;
    RECOMP_PC(0x00043FC5u);
    fp_push(MEMF(0x1D8084)); /* fld float */
    eax = MEM32(esp + 0x14);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esp + 0x10)); /* fadd dword ptr [esp + 0x10] */
    ecx = MEM32(0x1D9FB8);
    edx = MEM32(ecx + 0x10);
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    MEMF(esp + 0x10) = (float)fp_top(); fp_pop(); /* fstp */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ebx = ebx + 4;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edx (32-bit) */
    MEM32(esp + 0x14) = eax;
    if (CMP_B(_fa, _fb)) goto loc_00043E10; /* jb: below (unsigned <) */

loc_00043FF0: ;
    RECOMP_PC(0x00043FF0u);
    POP32(esp, edi);
    POP32(esp, ebx);

loc_00043FF2: ;
    RECOMP_PC(0x00043FF2u);
    ecx = ebp;
    PUSH32(esp, 0x00043FF9u); RECOMP_ABI_CALL(0x000430F0u, sub_000430F0); /* call 0x000430F0 */

loc_00043FF9: ;
    RECOMP_PC(0x00043FF9u);
    POP32(esp, esi);
    eax = ebp;
    POP32(esp, ebp);
    _fb = (uint32_t)(0x4C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x4C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    hud_ctor_done(0x00043BE0u,ctor_this,eax);
    esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_00045060
 * Original: 0x00045060 - 0x0004550E (1198 bytes, 343 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00045060(void)
{
    uint32_t ctor_this=ecx;
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

loc_00045060: ;
    RECOMP_PC(0x00045060u);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x40;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    eax = esp + 0x14;
    PUSH32(esp, eax);
    esi = ecx;
    MEM32(esp + 0x18) = 0x2CB;
    PUSH32(esp, 0x0004507Bu); RECOMP_ABI_CALL(0x0003CFD0u, sub_0003CFD0); /* call 0x0003CFD0 */

loc_0004507B: ;
    RECOMP_PC(0x0004507Bu);
    PUSH32(esp, 0x118);
    MEM32(esi) = 0x1B3AD8;
    PUSH32(esp, 0x0004508Bu); RECOMP_ABI_CALL(0x000367E0u, sub_000367E0); /* call 0x000367E0 */

loc_0004508B: ;
    RECOMP_PC(0x0004508Bu);
    MEMF(esp + 0x14) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x13E);
    PUSH32(esp, 0x00045099u); RECOMP_ABI_CALL(0x000367E0u, sub_000367E0); /* call 0x000367E0 */

loc_00045099: ;
    RECOMP_PC(0x00045099u);
    MEMF(esp + 0x20) = (float)fp_top(); fp_pop(); /* fstp */
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    ecx = esp + 0x24;
    MEM32(esi + 0x2C) = 0xBF800000u;
    PUSH32(esp, 0x000450C4u); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_000450C4: ;
    RECOMP_PC(0x000450C4u);
    ecx = MEM32(esp + 0x14);
    PUSH32(esp, ecx);
    PUSH32(esp, 0x1AE9B4);
    ecx = esi;
    PUSH32(esp, 0x000450D5u); RECOMP_ABI_CALL(0x000427F0u, sub_000427F0); /* call 0x000427F0 */

loc_000450D5: ;
    RECOMP_PC(0x000450D5u);
    PUSH32(esp, 0x84);
    PUSH32(esp, 0x000450DFu); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_000450DF: ;
    RECOMP_PC(0x000450DFu);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00045112; /* je: equal / zero */

loc_000450E6: ;
    RECOMP_PC(0x000450E6u);
    edx = MEM32(0x1D820C);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    PUSH32(esp, 0);
    PUSH32(esp, edx);
    PUSH32(esp, 0x1B36B8);
    PUSH32(esp, 0x1B3770);
    PUSH32(esp, 0x1D8090);
    PUSH32(esp, 0x1D8088);
    ecx = eax;
    PUSH32(esp, 0x0004510Eu); RECOMP_ABI_CALL(0x00035F10u, sub_00035F10); /* call 0x00035F10 */

loc_0004510E: ;
    RECOMP_PC(0x0004510Eu);
    edi = eax;
    goto loc_00045114;

loc_00045112: ;
    RECOMP_PC(0x00045112u);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_00045114: ;
    RECOMP_PC(0x00045114u);
    ecx = MEM32(0x1DB454);
    PUSH32(esp, 0x321);
    PUSH32(esp, 0x00045124u); RECOMP_ABI_CALL(0x000E0830u, sub_000E0830); /* call 0x000E0830 */

loc_00045124: ;
    RECOMP_PC(0x00045124u);
    MEM32(esp + 0x14) = eax;
    eax = esp + 0x14;
    PUSH32(esp, eax);
    ecx = edi;
    PUSH32(esp, 0x00045134u); RECOMP_ABI_CALL(0x0004D080u, sub_0004D080); /* call 0x0004D080 */

loc_00045134: ;
    RECOMP_PC(0x00045134u);
    ecx = MEM32(0x1D8228);
    edx = MEM32(0x1D8218);
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    ecx = edi;
    PUSH32(esp, 0x00045149u); RECOMP_ABI_CALL(0x0004C170u, sub_0004C170); /* call 0x0004C170 */

loc_00045149: ;
    RECOMP_PC(0x00045149u);
    PUSH32(esp, 0);
    PUSH32(esp, edi);
    ecx = esi;
    PUSH32(esp, 0x00045153u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_00045153: ;
    RECOMP_PC(0x00045153u);
    PUSH32(esp, 0x84);
    PUSH32(esp, 0x0004515Du); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_0004515D: ;
    RECOMP_PC(0x0004515Du);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00045190; /* je: equal / zero */

loc_00045164: ;
    RECOMP_PC(0x00045164u);
    ecx = MEM32(0x1D820C);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    PUSH32(esp, 2);
    PUSH32(esp, ecx);
    PUSH32(esp, 0x1B36B8);
    PUSH32(esp, 0x1B3770);
    PUSH32(esp, 0x1D8090);
    PUSH32(esp, 0x1D80B0);
    ecx = eax;
    PUSH32(esp, 0x0004518Cu); RECOMP_ABI_CALL(0x00035F10u, sub_00035F10); /* call 0x00035F10 */

loc_0004518C: ;
    RECOMP_PC(0x0004518Cu);
    edi = eax;
    goto loc_00045192;

loc_00045190: ;
    RECOMP_PC(0x00045190u);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_00045192: ;
    RECOMP_PC(0x00045192u);
    edx = MEM32(0x1D8228);
    eax = MEM32(0x1D8218);
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    ecx = edi;
    PUSH32(esp, 0x000451A6u); RECOMP_ABI_CALL(0x0004C170u, sub_0004C170); /* call 0x0004C170 */

loc_000451A6: ;
    RECOMP_PC(0x000451A6u);
    ecx = MEM32(0x1DB454);
    PUSH32(esp, 0x348);
    PUSH32(esp, 0x000451B6u); RECOMP_ABI_CALL(0x000E0830u, sub_000E0830); /* call 0x000E0830 */

loc_000451B6: ;
    RECOMP_PC(0x000451B6u);
    ecx = esp + 0x14;
    PUSH32(esp, ecx);
    ecx = edi;
    MEM32(esp + 0x18) = eax;
    PUSH32(esp, 0x000451C6u); RECOMP_ABI_CALL(0x0004D080u, sub_0004D080); /* call 0x0004D080 */

loc_000451C6: ;
    RECOMP_PC(0x000451C6u);
    PUSH32(esp, 0);
    PUSH32(esp, edi);
    ecx = esi;
    PUSH32(esp, 0x000451D0u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_000451D0: ;
    RECOMP_PC(0x000451D0u);
    eax = MEM32(0x1D74D8);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 4 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00045269; /* je: equal / zero */

loc_000451DE: ;
    RECOMP_PC(0x000451DEu);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(2) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 2 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00045269; /* je: equal / zero */

loc_000451E7: ;
    RECOMP_PC(0x000451E7u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(3) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 3 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00045269; /* je: equal / zero */

loc_000451EC: ;
    RECOMP_PC(0x000451ECu);
    PUSH32(esp, 0x84);
    PUSH32(esp, 0x000451F6u); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_000451F6: ;
    RECOMP_PC(0x000451F6u);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00045229; /* je: equal / zero */

loc_000451FD: ;
    RECOMP_PC(0x000451FDu);
    edx = MEM32(0x1D820C);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    PUSH32(esp, 1);
    PUSH32(esp, edx);
    PUSH32(esp, 0x1B36B8);
    PUSH32(esp, 0x1B3770);
    PUSH32(esp, 0x1D8090);
    PUSH32(esp, 0x1D80AC);
    ecx = eax;
    PUSH32(esp, 0x00045225u); RECOMP_ABI_CALL(0x00035F10u, sub_00035F10); /* call 0x00035F10 */

loc_00045225: ;
    RECOMP_PC(0x00045225u);
    edi = eax;
    goto loc_0004522B;

loc_00045229: ;
    RECOMP_PC(0x00045229u);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_0004522B: ;
    RECOMP_PC(0x0004522Bu);
    eax = MEM32(0x1D8228);
    ecx = MEM32(0x1D8218);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = edi;
    PUSH32(esp, 0x0004523Fu); RECOMP_ABI_CALL(0x0004C170u, sub_0004C170); /* call 0x0004C170 */

loc_0004523F: ;
    RECOMP_PC(0x0004523Fu);
    ecx = MEM32(0x1DB454);
    PUSH32(esp, 0x2CA);
    PUSH32(esp, 0x0004524Fu); RECOMP_ABI_CALL(0x000E0830u, sub_000E0830); /* call 0x000E0830 */

loc_0004524F: ;
    RECOMP_PC(0x0004524Fu);
    edx = esp + 0x14;
    PUSH32(esp, edx);
    ecx = edi;
    MEM32(esp + 0x18) = eax;
    PUSH32(esp, 0x0004525Fu); RECOMP_ABI_CALL(0x0004D080u, sub_0004D080); /* call 0x0004D080 */

loc_0004525F: ;
    RECOMP_PC(0x0004525Fu);
    PUSH32(esp, 0);
    PUSH32(esp, edi);
    ecx = esi;
    PUSH32(esp, 0x00045269u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_00045269: ;
    RECOMP_PC(0x00045269u);
    PUSH32(esp, 0x84);
    PUSH32(esp, 0x00045273u); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_00045273: ;
    RECOMP_PC(0x00045273u);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000452A6; /* je: equal / zero */

loc_0004527A: ;
    RECOMP_PC(0x0004527Au);
    ecx = MEM32(0x1D820C);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    PUSH32(esp, 0);
    PUSH32(esp, ecx);
    PUSH32(esp, 0x1B36B8);
    PUSH32(esp, 0x1B3770);
    edx = esp + 0x28;
    PUSH32(esp, edx);
    PUSH32(esp, 0x1D8088);
    ecx = eax;
    PUSH32(esp, 0x000452A2u); RECOMP_ABI_CALL(0x00035F10u, sub_00035F10); /* call 0x00035F10 */

loc_000452A2: ;
    RECOMP_PC(0x000452A2u);
    edi = eax;
    goto loc_000452A8;

loc_000452A6: ;
    RECOMP_PC(0x000452A6u);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_000452A8: ;
    RECOMP_PC(0x000452A8u);
    ecx = MEM32(0x1DB454);
    PUSH32(esp, 0x28);
    PUSH32(esp, 0x000452B5u); RECOMP_ABI_CALL(0x000E0830u, sub_000E0830); /* call 0x000E0830 */

loc_000452B5: ;
    RECOMP_PC(0x000452B5u);
    MEM32(esp + 0x14) = eax;
    eax = esp + 0x14;
    PUSH32(esp, eax);
    ecx = edi;
    PUSH32(esp, 0x000452C5u); RECOMP_ABI_CALL(0x0004D080u, sub_0004D080); /* call 0x0004D080 */

loc_000452C5: ;
    RECOMP_PC(0x000452C5u);
    ecx = MEM32(0x1D8228);
    edx = MEM32(0x1D8218);
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    ecx = edi;
    PUSH32(esp, 0x000452DAu); RECOMP_ABI_CALL(0x0004C170u, sub_0004C170); /* call 0x0004C170 */

loc_000452DA: ;
    RECOMP_PC(0x000452DAu);
    PUSH32(esp, 0);
    PUSH32(esp, edi);
    ecx = esi;
    PUSH32(esp, 0x000452E4u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_000452E4: ;
    RECOMP_PC(0x000452E4u);
    eax = MEM32(0x1D8074);
    ecx = esi;
    MEM32(esp + 0x10) = eax;
    PUSH32(esp, 0x000452F4u); RECOMP_ABI_CALL(0x000426E0u, sub_000426E0); /* call 0x000426E0 */

loc_000452F4: ;
    RECOMP_PC(0x000452F4u);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1B3AD4)); /* fmul dword ptr [0x1b3ad4] */
    ebp = 0; /* xor self */
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    MEM32(esp + 0x14) = ebp;
    fp_top() = RECOMP_FP_PC(MEMF(esp + 0x10) - fp_top()); /* fsubr dword ptr [esp + 0x10] */
    MEMF(esp + 0x10) = (float)fp_top(); fp_pop(); /* fstp */

loc_00045308: ;
    RECOMP_PC(0x00045308u);
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fb = (uint32_t)(5) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebp, 5 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_00045315; /* jne: not equal / not zero */

loc_0004530D: ;
    RECOMP_PC(0x0004530Du);
    ecx = MEM32(esp + 0x18);
    MEM32(esp + 0x10) = ecx;

loc_00045315: ;
    RECOMP_PC(0x00045315u);
    fp_push((double)SMEM32(esp + 0x14)); /* fild */
    PUSH32(esp, 0x3D4CCCCD);
    ecx = esp + 0x20;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1AF3A0)); /* fmul dword ptr [0x1af3a0] */
    fp_top() = RECOMP_FP_PC(MEMF(0x1B22F0) - fp_top()); /* fsubr dword ptr [0x1b22f0] */
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esp + 0x18);
    PUSH32(esp, eax);
    PUSH32(esp, eax);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x00045342u); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_00045342: ;
    RECOMP_PC(0x00045342u);
    PUSH32(esp, 0x84);
    PUSH32(esp, 0x0004534Cu); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_0004534C: ;
    RECOMP_PC(0x0004534Cu);
    ebx = MEM32(esp + 0x20);
    edi = eax;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000453A9; /* je: equal / zero */

loc_00045359: ;
    RECOMP_PC(0x00045359u);
    edx = MEM32(0x1D80A8);
    eax = MEM32(esp + 0x10);
    PUSH32(esp, 0x1B36A8);
    ecx = esp + 0x24;
    PUSH32(esp, ecx);
    ecx = edi;
    MEM32(esp + 0x28) = edx;
    MEM32(esp + 0x2C) = eax;
    MEM32(esp + 0x30) = 0;
    MEM32(esp + 0x34) = 0;
    PUSH32(esp, 0x0004538Cu); RECOMP_ABI_CALL(0x00110B60u, sub_00110B60); /* call 0x00110B60 */

loc_0004538C: ;
    RECOMP_PC(0x0004538Cu);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    MEM32(edi) = 0x1B0D80;
    edx = MEM32(0x1B3774);
    PUSH32(esp, 0);
    PUSH32(esp, edx);
    PUSH32(esp, ebx);
    ecx = edi;
    PUSH32(esp, 0x000453A7u); RECOMP_ABI_CALL(0x0004CF80u, sub_0004CF80); /* call 0x0004CF80 */

loc_000453A7: ;
    RECOMP_PC(0x000453A7u);
    goto loc_000453AB;

loc_000453A9: ;
    RECOMP_PC(0x000453A9u);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_000453AB: ;
    RECOMP_PC(0x000453ABu);
    MEM32(esi + ebp * 4 + 0x25C) = edi;
    eax = MEM32(0x1D8240);
    PUSH32(esp, eax);
    PUSH32(esp, ebx);
    ecx = edi;
    PUSH32(esp, 0x000453C0u); RECOMP_ABI_CALL(0x0004C170u, sub_0004C170); /* call 0x0004C170 */

loc_000453C0: ;
    RECOMP_PC(0x000453C0u);
    ecx = MEM32(esi + ebp * 4 + 0x25C);
    PUSH32(esp, 0);
    PUSH32(esp, ecx);
    ecx = esi;
    PUSH32(esp, 0x000453D1u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_000453D1: ;
    RECOMP_PC(0x000453D1u);
    PUSH32(esp, 0x84);
    PUSH32(esp, 0x000453DBu); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_000453DB: ;
    RECOMP_PC(0x000453DBu);
    edi = eax;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00045436; /* je: equal / zero */

loc_000453E4: ;
    RECOMP_PC(0x000453E4u);
    edx = MEM32(0x1D80B4);
    eax = MEM32(esp + 0x10);
    PUSH32(esp, 0x1B36A8);
    ecx = esp + 0x34;
    PUSH32(esp, ecx);
    ecx = edi;
    MEM32(esp + 0x38) = edx;
    MEM32(esp + 0x3C) = eax;
    MEM32(esp + 0x40) = 0;
    MEM32(esp + 0x44) = 0;
    PUSH32(esp, 0x00045417u); RECOMP_ABI_CALL(0x00110B60u, sub_00110B60); /* call 0x00110B60 */

loc_00045417: ;
    RECOMP_PC(0x00045417u);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    MEM32(edi) = 0x1B0D80;
    edx = MEM32(0x1B3774);
    PUSH32(esp, 2);
    PUSH32(esp, edx);
    PUSH32(esp, ebx);
    ecx = edi;
    PUSH32(esp, 0x00045432u); RECOMP_ABI_CALL(0x0004CF80u, sub_0004CF80); /* call 0x0004CF80 */

loc_00045432: ;
    RECOMP_PC(0x00045432u);
    ecx = edi;
    goto loc_00045438;

loc_00045436: ;
    RECOMP_PC(0x00045436u);
    ecx = 0; /* xor self */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_00045438: ;
    RECOMP_PC(0x00045438u);
    MEM32(esi + ebp * 4 + 0x27C) = ecx;
    eax = MEM32(0x1D8240);
    PUSH32(esp, eax);
    PUSH32(esp, ebx);
    PUSH32(esp, 0x0004544Bu); RECOMP_ABI_CALL(0x0004C170u, sub_0004C170); /* call 0x0004C170 */

loc_0004544B: ;
    RECOMP_PC(0x0004544Bu);
    ecx = MEM32(esi + ebp * 4 + 0x27C);
    PUSH32(esp, 0);
    PUSH32(esp, ecx);
    ecx = esi;
    PUSH32(esp, 0x0004545Cu); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_0004545C: ;
    RECOMP_PC(0x0004545Cu);
    PUSH32(esp, 0x84);
    PUSH32(esp, 0x00045466u); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_00045466: ;
    RECOMP_PC(0x00045466u);
    edi = eax;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000454C1; /* je: equal / zero */

loc_0004546F: ;
    RECOMP_PC(0x0004546Fu);
    edx = MEM32(0x1D80AC);
    eax = MEM32(esp + 0x10);
    PUSH32(esp, 0x1B3698);
    ecx = esp + 0x44;
    PUSH32(esp, ecx);
    ecx = edi;
    MEM32(esp + 0x48) = edx;
    MEM32(esp + 0x4C) = eax;
    MEM32(esp + 0x50) = 0;
    MEM32(esp + 0x54) = 0;
    PUSH32(esp, 0x000454A2u); RECOMP_ABI_CALL(0x00110B60u, sub_00110B60); /* call 0x00110B60 */

loc_000454A2: ;
    RECOMP_PC(0x000454A2u);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    MEM32(edi) = 0x1B0D80;
    edx = MEM32(0x1B3774);
    PUSH32(esp, 1);
    PUSH32(esp, edx);
    PUSH32(esp, ebx);
    ecx = edi;
    PUSH32(esp, 0x000454BDu); RECOMP_ABI_CALL(0x0004CF80u, sub_0004CF80); /* call 0x0004CF80 */

loc_000454BD: ;
    RECOMP_PC(0x000454BDu);
    ecx = edi;
    goto loc_000454C3;

loc_000454C1: ;
    RECOMP_PC(0x000454C1u);
    ecx = 0; /* xor self */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_000454C3: ;
    RECOMP_PC(0x000454C3u);
    MEM32(esi + ebp * 4 + 0x23C) = ecx;
    eax = MEM32(0x1D8240);
    PUSH32(esp, eax);
    PUSH32(esp, ebx);
    PUSH32(esp, 0x000454D6u); RECOMP_ABI_CALL(0x0004C170u, sub_0004C170); /* call 0x0004C170 */

loc_000454D6: ;
    RECOMP_PC(0x000454D6u);
    ecx = MEM32(esi + ebp * 4 + 0x23C);
    PUSH32(esp, 0);
    PUSH32(esp, ecx);
    ecx = esi;
    PUSH32(esp, 0x000454E7u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_000454E7: ;
    RECOMP_PC(0x000454E7u);
    ecx = esi;
    PUSH32(esp, 0x000454EEu); RECOMP_ABI_CALL(0x000426E0u, sub_000426E0); /* call 0x000426E0 */

loc_000454EE: ;
    RECOMP_PC(0x000454EEu);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esp + 0x10)); /* fadd dword ptr [esp + 0x10] */
    ebp++;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fb = (uint32_t)(8) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebp, 8 (32-bit) */
    MEMF(esp + 0x10) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esp + 0x14) = ebp;
    if (CMP_B(_fa, _fb)) goto loc_00045308; /* jb: below (unsigned <) */

loc_00045504: ;
    RECOMP_PC(0x00045504u);
    POP32(esp, edi);
    eax = esi;
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x40;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    hud_ctor_done(0x00045060u,ctor_this,eax);
    esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}


/**
 * qpr2_recovered_0006B950
 * Original: 0x0006B950 - 0x0006BA7C (300 bytes, 88 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_0006B950(void)
{
    uint32_t ctor_this=ecx;
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;

loc_0006B950: ;
    RECOMP_PC(0x0006B950u);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    eax = esp + 8;
    PUSH32(esp, eax);
    esi = ecx;
    MEM32(esp + 0xC) = 0x230;
    PUSH32(esp, 0x0006B969u); RECOMP_ABI_CALL(0x0003CFD0u, sub_0003CFD0); /* call 0x0003CFD0 */

loc_0006B969: ;
    RECOMP_PC(0x0006B969u);
    PUSH32(esp, 0x84);
    MEM32(esi) = 0x1BAD68;
    PUSH32(esp, 0x0006B979u); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_0006B979: ;
    RECOMP_PC(0x0006B979u);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0006B9C4; /* je: equal / zero */

loc_0006B980: ;
    RECOMP_PC(0x0006B980u);
    ecx = MEM32(0x1D8228);
    PUSH32(esp, 1);
    PUSH32(esp, 0x20);
    PUSH32(esp, 1);
    PUSH32(esp, ecx);
    PUSH32(esp, 0x1B3F3C);
    edx = esp + 0x1C;
    PUSH32(esp, edx);
    ecx = esp + 0x24;
    PUSH32(esp, ecx);
    edx = esp + 0x2C;
    PUSH32(esp, edx);
    ecx = eax;
    MEM32(esp + 0x28) = 0x3F266666;
    MEM32(esp + 0x2C) = 0x3E4CCCCD;
    MEM32(esp + 0x30) = 0;
    PUSH32(esp, 0x0006B9C0u); RECOMP_ABI_CALL(0x00035F10u, sub_00035F10); /* call 0x00035F10 */

loc_0006B9C0: ;
    RECOMP_PC(0x0006B9C0u);
    edi = eax;
    goto loc_0006B9C6;

loc_0006B9C4: ;
    RECOMP_PC(0x0006B9C4u);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_0006B9C6: ;
    RECOMP_PC(0x0006B9C6u);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0xBF19999Au);
    MEM32(esi + 0x23C) = edi;
    eax = MEM32(edi);
    PUSH32(esp, 0xBF333333u);
    ecx = edi;
    { uint32_t _icall_target = MEM32(eax + 0x30); PUSH32(esp, 0x0006B9DDu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x0006B9DAu); } /* indirect call */
    }

loc_0006B9DD: ;
    RECOMP_PC(0x0006B9DDu);
    edx = MEM32(edi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x3F4CCCCD);
    PUSH32(esp, 0x3FB33333);
    ecx = edi;
    { uint32_t _icall_target = MEM32(edx + 0x34); PUSH32(esp, 0x0006B9EEu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x0006B9EBu); } /* indirect call */
    }

loc_0006B9EE: ;
    RECOMP_PC(0x0006B9EEu);
    eax = MEM32(0x1D8228);
    ecx = MEM32(0x1D8218);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = MEM32(esi + 0x23C);
    PUSH32(esp, 0x0006BA06u); RECOMP_ABI_CALL(0x0004C170u, sub_0004C170); /* call 0x0004C170 */

loc_0006BA06: ;
    RECOMP_PC(0x0006BA06u);
    edx = MEM32(esi + 0x23C);
    PUSH32(esp, 0);
    PUSH32(esp, edx);
    ecx = esi;
    PUSH32(esp, 0x0006BA16u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_0006BA16: ;
    RECOMP_PC(0x0006BA16u);
    PUSH32(esp, 0x7C);
    PUSH32(esp, 0x0006BA1Du); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_0006BA1D: ;
    RECOMP_PC(0x0006BA1Du);
    edi = eax;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0006BA61; /* je: equal / zero */

loc_0006BA26: ;
    RECOMP_PC(0x0006BA26u);
    ecx = MEM32(0x1DB454);
    PUSH32(esp, 1);
    eax = esp + 0x14;
    PUSH32(esp, eax);
    PUSH32(esp, 0x24E);
    MEM32(esp + 0x1C) = 0;
    MEM32(esp + 0x18) = 0xBE99999Au;
    PUSH32(esp, 0x0006BA4Du); RECOMP_ABI_CALL(0x000E0830u, sub_000E0830); /* call 0x000E0830 */

loc_0006BA4D: ;
    RECOMP_PC(0x0006BA4Du);
    PUSH32(esp, eax);
    PUSH32(esp, 0x1B3904);
    ecx = esp + 0x1C;
    PUSH32(esp, ecx);
    ecx = edi;
    PUSH32(esp, 0x0006BA5Fu); RECOMP_ABI_CALL(0x00035E90u, sub_00035E90); /* call 0x00035E90 */

loc_0006BA5F: ;
    RECOMP_PC(0x0006BA5Fu);
    goto loc_0006BA63;

loc_0006BA61: ;
    RECOMP_PC(0x0006BA61u);
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_0006BA63: ;
    RECOMP_PC(0x0006BA63u);
    PUSH32(esp, 0);
    PUSH32(esp, eax);
    ecx = esi;
    MEM32(eax + 0x24) = 0;
    PUSH32(esp, 0x0006BA74u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_0006BA74: ;
    RECOMP_PC(0x0006BA74u);
    POP32(esp, edi);
    eax = esi;
    POP32(esp, esi);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    hud_ctor_done(0x0006B950u,ctor_this,eax);
    esp += 4; return; /* ret */

}


/**
 * qpr2_recovered_00045510
 * Original: 0x00045510 - 0x00045A31 (1313 bytes, 339 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00045510(void)
{
    uint32_t ctor_this=ecx;
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

loc_00045510: ;
    RECOMP_PC(0x00045510u);
    _fb = (uint32_t)(0x6C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x6C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    eax = esp + 0x10;
    MEM32(esp + 0x20) = ebx;
    PUSH32(esp, eax);
    esi = ecx;
    MEM32(esp + 0x14) = 0xF3;
    PUSH32(esp, 0x00045530u); RECOMP_ABI_CALL(0x0003CFD0u, sub_0003CFD0); /* call 0x0003CFD0 */

loc_00045530: ;
    RECOMP_PC(0x00045530u);
    MEM32(esi) = 0x1B3B80;
    eax = esi + 0x5C8;
    ecx = 6;

loc_00045541: ;
    RECOMP_PC(0x00045541u);
    MEM32(eax) = 0xFFFFFFFFu;
    MEM32(eax + 4) = ebx;
    MEM32(eax + 8) = ebx;
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + 0xC;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ecx--;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    if ((_fa != 0)) goto loc_00045541; /* jne: not equal / not zero */

loc_00045553: ;
    RECOMP_PC(0x00045553u);
    PUSH32(esp, 0x14E);
    PUSH32(esp, 0x0004555Du); RECOMP_ABI_CALL(0x000367C0u, sub_000367C0); /* call 0x000367C0 */

loc_0004555D: ;
    RECOMP_PC(0x0004555Du);
    MEMF(esp + 0x28) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x15C);
    PUSH32(esp, 0x0004556Bu); RECOMP_ABI_CALL(0x000367C0u, sub_000367C0); /* call 0x000367C0 */

loc_0004556B: ;
    RECOMP_PC(0x0004556Bu);
    MEMF(esp + 0x30) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x1B3);
    PUSH32(esp, 0x00045579u); RECOMP_ABI_CALL(0x000367C0u, sub_000367C0); /* call 0x000367C0 */

loc_00045579: ;
    RECOMP_PC(0x00045579u);
    MEMF(esp + 0x38) = (float)fp_top(); fp_pop(); /* fstp */
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    ecx = esp + 0x20;
    MEM32(esp + 0x70) = 0x327;
    MEM32(esp + 0x74) = 0x167;
    MEM32(esp + 0x78) = 0x40;
    MEM32(esp + 0x7C) = 0x122;
    MEM32(esp + 0x80) = 0x2A;
    MEM32(esp + 0x84) = 0x29;
    MEM32(esi + 0x2C) = 0xBF800000u;
    PUSH32(esp, 0x000455DAu); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_000455DA: ;
    RECOMP_PC(0x000455DAu);
    ecx = MEM32(esp + 0x10);
    PUSH32(esp, ecx);
    PUSH32(esp, 0x1AE9EC);
    ecx = esi;
    PUSH32(esp, 0x000455EBu); RECOMP_ABI_CALL(0x000427F0u, sub_000427F0); /* call 0x000427F0 */

loc_000455EB: ;
    RECOMP_PC(0x000455EBu);
    edx = MEM32(0x1D9FB8);
    eax = MEM32(edx + 0x4C);
    MEM32(esi + 0x23C) = eax;
    ecx = MEM32(0x1D9FB8);
    edx = MEM32(ecx + 0x5C);
    MEM32(esi + 0x240) = edx;
    eax = MEM32(0x1D8090);
    ecx = esi;
    MEM32(esp + 0xC) = eax;
    PUSH32(esp, 0x00045619u); RECOMP_ABI_CALL(0x000426E0u, sub_000426E0); /* call 0x000426E0 */

loc_00045619: ;
    RECOMP_PC(0x00045619u);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esp + 0xC)); /* fadd dword ptr [esp + 0xc] */
    ecx = esi;
    MEMF(esp + 0xC) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x00045628u); RECOMP_ABI_CALL(0x000426E0u, sub_000426E0); /* call 0x000426E0 */

loc_00045628: ;
    RECOMP_PC(0x00045628u);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esp + 0xC)); /* fadd dword ptr [esp + 0xc] */
    eax = MEM32(esi + 0x240);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(8) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 8 (32-bit) */
    MEMF(esp + 0xC) = (float)fp_top(); fp_pop(); /* fstp */
    if (CMP_NE(_fa, _fb)) goto loc_00045692; /* jne: not equal / not zero */

loc_0004563B: ;
    RECOMP_PC(0x0004563Bu);
    ecx = MEM32(0x1DB454);
    PUSH32(esp, 0xF4);
    PUSH32(esp, 0x0004564Bu); RECOMP_ABI_CALL(0x000E0830u, sub_000E0830); /* call 0x000E0830 */

loc_0004564B: ;
    RECOMP_PC(0x0004564Bu);
    ecx = esp + 0x10;
    PUSH32(esp, ecx);
    ecx = esi;
    MEM32(esp + 0x14) = eax;
    PUSH32(esp, 0x0004565Bu); RECOMP_ABI_CALL(0x0004E810u, sub_0004E810); /* call 0x0004E810 */

loc_0004565B: ;
    RECOMP_PC(0x0004565Bu);
    PUSH32(esp, 0x12C);
    PUSH32(esp, 0x00045665u); RECOMP_ABI_CALL(0x000367C0u, sub_000367C0); /* call 0x000367C0 */

loc_00045665: ;
    RECOMP_PC(0x00045665u);
    MEMF(esp + 0x28) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x13A);
    PUSH32(esp, 0x00045673u); RECOMP_ABI_CALL(0x000367C0u, sub_000367C0); /* call 0x000367C0 */

loc_00045673: ;
    RECOMP_PC(0x00045673u);
    MEMF(esp + 0x30) = (float)fp_top(); fp_pop(); /* fstp */
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x60) = 0x205;
    MEM32(esp + 0x68) = 0x202;
    MEM32(esp + 0x6C) = 0x204;

loc_00045692: ;
    RECOMP_PC(0x00045692u);
    MEM32(esp + 0x10) = ebx;
    PUSH32(esp, ebp);

loc_00045697: ;
    RECOMP_PC(0x00045697u);
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(1) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 1 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000456AB; /* jne: not equal / not zero */

loc_0004569C: ;
    RECOMP_PC(0x0004569Cu);
    ecx = esi;
    PUSH32(esp, 0x000456A3u); RECOMP_ABI_CALL(0x000426E0u, sub_000426E0); /* call 0x000426E0 */

loc_000456A3: ;
    RECOMP_PC(0x000456A3u);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esp + 0x10)); /* fadd dword ptr [esp + 0x10] */
    MEMF(esp + 0x10) = (float)fp_top(); fp_pop(); /* fstp */

loc_000456AB: ;
    RECOMP_PC(0x000456ABu);
    eax = MEM32(esp + ebx * 4 + 0x64);
    ecx = MEM32(0x1DB454);
    edx = ebx;
    edx = edx << 6;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    PUSH32(esp, eax);
    ebp = edx + esi + 0x448;
    PUSH32(esp, 0x000456C7u); RECOMP_ABI_CALL(0x000E0830u, sub_000E0830); /* call 0x000E0830 */

loc_000456C7: ;
    RECOMP_PC(0x000456C7u);
    PUSH32(esp, eax);
    PUSH32(esp, ebp);
    PUSH32(esp, 0x000456CEu); RECOMP_ABI_CALL(0x000E1310u, sub_000E1310); /* call 0x000E1310 */

loc_000456CE: ;
    RECOMP_PC(0x000456CEu);
    PUSH32(esp, 0x113);
    PUSH32(esp, 0x000456D8u); RECOMP_ABI_CALL(0x000367A0u, sub_000367A0); /* call 0x000367A0 */

loc_000456D8: ;
    RECOMP_PC(0x000456D8u);
    MEMF(esp + 0x24) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x1B3774);
    ecx = esp + 0x28;
    PUSH32(esp, ecx);
    PUSH32(esp, ebp);
    PUSH32(esp, 0x000456ECu); RECOMP_ABI_CALL(0x0006B280u, sub_0006B280); /* call 0x0006B280 */

loc_000456EC: ;
    RECOMP_PC(0x000456ECu);
    PUSH32(esp, 0x84);
    PUSH32(esp, 0x000456F6u); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_000456F6: ;
    RECOMP_PC(0x000456F6u);
    edi = eax;
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x1C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0004575A; /* je: equal / zero */

loc_000456FF: ;
    RECOMP_PC(0x000456FFu);
    edx = MEM32(0x1D8214);
    ecx = MEM32(esp + 0x10);
    eax = MEM32(esp + 0x28);
    MEM32(esp + 0x1C) = edx;
    PUSH32(esp, 0x1B36B8);
    edx = esp + 0x38;
    MEM32(esp + 0x3C) = ecx;
    PUSH32(esp, edx);
    ecx = edi;
    MEM32(esp + 0x3C) = eax;
    MEM32(esp + 0x44) = 0;
    MEM32(esp + 0x48) = 0;
    PUSH32(esp, 0x0004573Au); RECOMP_ABI_CALL(0x00110B60u, sub_00110B60); /* call 0x00110B60 */

loc_0004573A: ;
    RECOMP_PC(0x0004573Au);
    ecx = MEM32(esp + 0x1C);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    MEM32(edi) = 0x1B0D80;
    eax = MEM32(0x1B3774);
    PUSH32(esp, 2);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = edi;
    PUSH32(esp, 0x00045758u); RECOMP_ABI_CALL(0x0004CF80u, sub_0004CF80); /* call 0x0004CF80 */

loc_00045758: ;
    RECOMP_PC(0x00045758u);
    goto loc_0004575C;

loc_0004575A: ;
    RECOMP_PC(0x0004575Au);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_0004575C: ;
    RECOMP_PC(0x0004575Cu);
    edx = esp + 0x1C;
    PUSH32(esp, edx);
    ecx = edi;
    MEM32(esp + 0x20) = ebp;
    PUSH32(esp, 0x0004576Cu); RECOMP_ABI_CALL(0x0004D080u, sub_0004D080); /* call 0x0004D080 */

loc_0004576C: ;
    RECOMP_PC(0x0004576Cu);
    PUSH32(esp, 0);
    PUSH32(esp, edi);
    ecx = esi;
    PUSH32(esp, 0x00045776u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_00045776: ;
    RECOMP_PC(0x00045776u);
    PUSH32(esp, 0x84);
    PUSH32(esp, 0x00045780u); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_00045780: ;
    RECOMP_PC(0x00045780u);
    edi = eax;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00045807; /* je: equal / zero */

loc_00045789: ;
    RECOMP_PC(0x00045789u);
    fp_push((double)SMEM32(esp + 0x14)); /* fild */
    PUSH32(esp, 0x3D4CCCCD);
    ecx = esp + 0x24;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1AF3A0)); /* fmul dword ptr [0x1af3a0] */
    fp_top() = RECOMP_FP_PC(MEMF(0x1B22F0) - fp_top()); /* fsubr dword ptr [0x1b22f0] */
    MEMF(esp + 0x20) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esp + 0x20);
    PUSH32(esp, eax);
    PUSH32(esp, eax);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x000457B6u); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_000457B6: ;
    RECOMP_PC(0x000457B6u);
    ecx = MEM32(esp + 0x10);
    eax = MEM32(esp + 0x2C);
    PUSH32(esp, 0x1B36B8);
    edx = esp + 0x48;
    MEM32(esp + 0x4C) = ecx;
    PUSH32(esp, edx);
    ecx = edi;
    MEM32(esp + 0x4C) = eax;
    MEM32(esp + 0x54) = 0;
    MEM32(esp + 0x58) = 0;
    PUSH32(esp, 0x000457E7u); RECOMP_ABI_CALL(0x00110B60u, sub_00110B60); /* call 0x00110B60 */

loc_000457E7: ;
    RECOMP_PC(0x000457E7u);
    ecx = MEM32(esp + 0x20);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    MEM32(edi) = 0x1B0D80;
    eax = MEM32(0x1B3774);
    PUSH32(esp, 0);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = edi;
    PUSH32(esp, 0x00045805u); RECOMP_ABI_CALL(0x0004CF80u, sub_0004CF80); /* call 0x0004CF80 */

loc_00045805: ;
    RECOMP_PC(0x00045805u);
    goto loc_00045809;

loc_00045807: ;
    RECOMP_PC(0x00045807u);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_00045809: ;
    RECOMP_PC(0x00045809u);
    PUSH32(esp, 0);
    PUSH32(esp, edi);
    ecx = esi;
    MEM32(esi + ebx * 4 + 0x790) = edi;
    PUSH32(esp, 0x0004581Au); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_0004581A: ;
    RECOMP_PC(0x0004581Au);
    PUSH32(esp, 0x84);
    PUSH32(esp, 0x00045824u); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_00045824: ;
    RECOMP_PC(0x00045824u);
    edi = eax;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0004587F; /* je: equal / zero */

loc_0004582D: ;
    RECOMP_PC(0x0004582Du);
    edx = MEM32(esp + 0x30);
    eax = MEM32(esp + 0x10);
    ebp = MEM32(0x1D8218);
    PUSH32(esp, 0x1B36B8);
    ecx = esp + 0x58;
    PUSH32(esp, ecx);
    ecx = edi;
    MEM32(esp + 0x5C) = edx;
    MEM32(esp + 0x60) = eax;
    MEM32(esp + 0x64) = 0;
    MEM32(esp + 0x68) = 0;
    PUSH32(esp, 0x00045864u); RECOMP_ABI_CALL(0x00110B60u, sub_00110B60); /* call 0x00110B60 */

loc_00045864: ;
    RECOMP_PC(0x00045864u);
    PUSH32(esp, 0);
    PUSH32(esp, 0x10);
    PUSH32(esp, 0);
    PUSH32(esp, 0x3F000000);
    PUSH32(esp, ebp);
    ecx = edi;
    MEM32(edi) = 0x1B0D80;
    PUSH32(esp, 0x0004587Du); RECOMP_ABI_CALL(0x0004CF80u, sub_0004CF80); /* call 0x0004CF80 */

loc_0004587D: ;
    RECOMP_PC(0x0004587Du);
    goto loc_00045881;

loc_0004587F: ;
    RECOMP_PC(0x0004587Fu);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_00045881: ;
    RECOMP_PC(0x00045881u);
    MEM32(esi + ebx * 4 + 0x7A8) = edi;
    ecx = MEM32(0x1DB454);
    PUSH32(esp, 0x230);
    PUSH32(esp, 0x00045898u); RECOMP_ABI_CALL(0x000E0830u, sub_000E0830); /* call 0x000E0830 */

loc_00045898: ;
    RECOMP_PC(0x00045898u);
    ecx = MEM32(esi + ebx * 4 + 0x7A8);
    edx = esp + 0x24;
    PUSH32(esp, edx);
    MEM32(esp + 0x28) = eax;
    PUSH32(esp, 0x000458ADu); RECOMP_ABI_CALL(0x0004D080u, sub_0004D080); /* call 0x0004D080 */

loc_000458AD: ;
    RECOMP_PC(0x000458ADu);
    eax = MEM32(esi + ebx * 4 + 0x7A8);
    PUSH32(esp, 0);
    PUSH32(esp, eax);
    ecx = esi;
    PUSH32(esp, 0x000458BEu); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_000458BE: ;
    RECOMP_PC(0x000458BEu);
    ecx = esi;
    PUSH32(esp, 0x000458C5u); RECOMP_ABI_CALL(0x000426E0u, sub_000426E0); /* call 0x000426E0 */

loc_000458C5: ;
    RECOMP_PC(0x000458C5u);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esp + 0x10)); /* fadd dword ptr [esp + 0x10] */
    ebx++;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(6) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 6 (32-bit) */
    MEMF(esp + 0x10) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esp + 0x14) = ebx;
    if (CMP_B(_fa, _fb)) goto loc_00045697; /* jb: below (unsigned <) */

loc_000458DB: ;
    RECOMP_PC(0x000458DBu);
    eax = MEM32(esi + 0x240);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 4 (32-bit) */
    POP32(esp, ebp);
    if (CMP_EQ(_fa, _fb)) goto loc_000458F0; /* je: equal / zero */

loc_000458E7: ;
    RECOMP_PC(0x000458E7u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(8) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 8 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_00045A21; /* jne: not equal / not zero */

loc_000458F0: ;
    RECOMP_PC(0x000458F0u);
    PUSH32(esp, 0x84);
    PUSH32(esp, 0x000458FAu); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_000458FA: ;
    RECOMP_PC(0x000458FAu);
    edi = eax;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0004595C; /* je: equal / zero */

loc_00045903: ;
    RECOMP_PC(0x00045903u);
    PUSH32(esp, 2);
    MEM32(esp + 0x24) = 0x3F000000;
    PUSH32(esp, 0x00045912u); RECOMP_ABI_CALL(0x000367B0u, sub_000367B0); /* call 0x000367B0 */

loc_00045912: ;
    RECOMP_PC(0x00045912u);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x1D8090)); /* fadd dword ptr [0x1d8090] */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x1B36B8);
    ecx = esp + 0x54;
    MEMF(esp + 0x58) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, ecx);
    ecx = edi;
    MEM32(esp + 0x58) = 0xBF800000u;
    MEM32(esp + 0x60) = 0x40000000;
    MEM32(esp + 0x64) = 0;
    PUSH32(esp, 0x00045948u); RECOMP_ABI_CALL(0x00110B60u, sub_00110B60); /* call 0x00110B60 */

loc_00045948: ;
    RECOMP_PC(0x00045948u);
    edx = esp + 0x20;
    PUSH32(esp, edx);
    ecx = edi;
    MEM32(edi) = 0x1B0D80;
    PUSH32(esp, 0x0004595Au); RECOMP_ABI_CALL(0x0004D850u, sub_0004D850); /* call 0x0004D850 */

loc_0004595A: ;
    RECOMP_PC(0x0004595Au);
    goto loc_0004595E;

loc_0004595C: ;
    RECOMP_PC(0x0004595Cu);
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */

loc_0004595E: ;
    RECOMP_PC(0x0004595Eu);
    PUSH32(esp, 0x3DCCCCCD);
    PUSH32(esp, 0x3DCCCCCD);
    PUSH32(esp, 0x3DCCCCCD);
    PUSH32(esp, 0x3F800000);
    ecx = esp + 0x30;
    MEM32(esi + 0x344) = edi;
    PUSH32(esp, 0x00045981u); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_00045981: ;
    RECOMP_PC(0x00045981u);
    eax = MEM32(esp + 0x20);
    ecx = MEM32(0x1D8218);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = edi;
    PUSH32(esp, 0x00045994u); RECOMP_ABI_CALL(0x0004C170u, sub_0004C170); /* call 0x0004C170 */

loc_00045994: ;
    RECOMP_PC(0x00045994u);
    _fa = (uint32_t)(MEM32(0x1D74D8)) & 0xFFFFFFFFu; _fb = (uint32_t)(1) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1D74D8), 1 (32-bit) */
    ecx = MEM32(0x1DB454);
    PUSH32(esp, 0xF3);
    if (CMP_NE(_fa, _fb)) goto loc_000459D3; /* jne: not equal / not zero */

loc_000459A8: ;
    RECOMP_PC(0x000459A8u);
    PUSH32(esp, 0x000459ADu); RECOMP_ABI_CALL(0x000E0830u, sub_000E0830); /* call 0x000E0830 */

loc_000459AD: ;
    RECOMP_PC(0x000459ADu);
    edx = MEM32(esi + 0x23C);
    PUSH32(esp, edx);
    ebx = eax;
    edi = esi + 0x348;
    PUSH32(esp, 0x000459C1u); RECOMP_ABI_CALL(0x000B6820u, sub_000B6820); /* call 0x000B6820 */

loc_000459C1: ;
    RECOMP_PC(0x000459C1u);
    PUSH32(esp, eax);
    PUSH32(esp, ebx);
    PUSH32(esp, 0x1B3B74);
    PUSH32(esp, edi);
    PUSH32(esp, 0x000459CEu); RECOMP_ABI_CALL(0x000E07A0u, sub_000E07A0); /* call 0x000E07A0 */

loc_000459CE: ;
    RECOMP_PC(0x000459CEu);
    _fb = (uint32_t)(0x14) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x14;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    goto loc_000459FD;

loc_000459D3: ;
    RECOMP_PC(0x000459D3u);
    edi = esi + 0x348;
    PUSH32(esp, 0x000459DEu); RECOMP_ABI_CALL(0x000E0830u, sub_000E0830); /* call 0x000E0830 */

loc_000459DE: ;
    RECOMP_PC(0x000459DEu);
    PUSH32(esp, eax);
    eax = MEM32(esi + 0x23C);
    PUSH32(esp, eax);
    PUSH32(esp, 0x000459EBu); RECOMP_ABI_CALL(0x000B6820u, sub_000B6820); /* call 0x000B6820 */

loc_000459EB: ;
    RECOMP_PC(0x000459EBu);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, eax);
    PUSH32(esp, 0x1B3B74);
    PUSH32(esp, edi);
    PUSH32(esp, 0x000459FAu); RECOMP_ABI_CALL(0x000E07A0u, sub_000E07A0); /* call 0x000E07A0 */

loc_000459FA: ;
    RECOMP_PC(0x000459FAu);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */

loc_000459FD: ;
    RECOMP_PC(0x000459FDu);
    ecx = esp + 0x20;
    PUSH32(esp, ecx);
    ecx = MEM32(esi + 0x344);
    MEM32(esp + 0x24) = edi;
    PUSH32(esp, 0x00045A11u); RECOMP_ABI_CALL(0x0004D080u, sub_0004D080); /* call 0x0004D080 */

loc_00045A11: ;
    RECOMP_PC(0x00045A11u);
    edx = MEM32(esi + 0x344);
    PUSH32(esp, 0);
    PUSH32(esp, edx);
    ecx = esi;
    PUSH32(esp, 0x00045A21u); RECOMP_ABI_CALL(0x0010FAA0u, sub_0010FAA0); /* call 0x0010FAA0 */

loc_00045A21: ;
    RECOMP_PC(0x00045A21u);
    ecx = esi;
    PUSH32(esp, 0x00045A28u); RECOMP_ABI_CALL(0x000430F0u, sub_000430F0); /* call 0x000430F0 */

loc_00045A28: ;
    RECOMP_PC(0x00045A28u);
    POP32(esp, edi);
    eax = esi;
    POP32(esp, esi);
    POP32(esp, ebx);
    _fb = (uint32_t)(0x6C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x6C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    hud_ctor_done(0x00045510u,ctor_this,eax);
    esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

