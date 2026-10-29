/* Bounded routines recovered from the supplied retail disassembly. */

#define RECOMP_GENERATED_CODE

#include "recomp/gen/recomp_types.h"

#include "recomp/gen/recomp_funcs.h"

void qpr2_recovered_0002EBA0(void);

/**
 * qpr2_recovered_0002EBA0
 * Original: 0x0002EBA0 - 0x0002EDCC (556 bytes, 159 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_0002EBA0(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    int _cf = 0; /* carry flag */
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_0002EBA0: ;
    RECOMP_PC(0x0002EBA0u);
    _fb = (uint32_t)(0x20) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esp) < (uint32_t)(0x20));
    esp = esp - 0x20;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, esi);
    esi = ecx;
    eax = MEM32(esi + 0x49C);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    PUSH32(esp, edi);
    edi = MEM32(esp + 0x2C);
    if (CMP_EQ(_fa, _fb)) goto loc_0002EBE2; /* je: equal / zero */

loc_0002EBB5: ;
    RECOMP_PC(0x0002EBB5u);
    _fa = (uint32_t)(MEM8(edi + 6)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test MEM8(edi + 6), 1 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (TEST_Z(_fa, _fb)) goto loc_0002EBCC; /* je: equal / zero */

loc_0002EBBB: ;
    RECOMP_PC(0x0002EBBBu);
    eax = ZX16(MEM16(edi + 2));
    ecx = (uint32_t)(int32_t)SMEM16(edi + 0x24);
    _fb = (uint32_t)(0xA) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0xA)) >> 32) & 1);
    eax = eax + 0xA;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, eax (32-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_GE(_fas, _fbs)) goto loc_0002EBE2; /* jge: greater or equal (signed >=) */

loc_0002EBCA: ;
    RECOMP_PC(0x0002EBCAu);
    goto loc_0002EBDB;

loc_0002EBCC: ;
    RECOMP_PC(0x0002EBCCu);
    edx = ZX16(MEM16(edi + 4));
    eax = (uint32_t)(int32_t)SMEM16(edi + 0x24);
    _fb = (uint32_t)(0xA) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(edx) < (uint32_t)(0xA));
    edx = edx - 0xA;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edx (32-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_LE(_fas, _fbs)) goto loc_0002EBE2; /* jle: less or equal (signed <=) */

loc_0002EBDB: ;
    RECOMP_PC(0x0002EBDBu);
    _cf = 0; /* logical op clears CF */
    MEM32(esi + 0x494) = MEM32(esi + 0x494) | 8;
    _fa = (uint32_t)(MEM32(esi + 0x494)) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */

loc_0002EBE2: ;
    RECOMP_PC(0x0002EBE2u);
    _fa = (uint32_t)(MEM32(esi + 0x488)) & 0xFFFFFFFFu; _fb = (uint32_t)(5) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(esi + 0x488), 5 (32-bit) */
    _cf = (int)(_fa < _fb);
    PUSH32(esp, ebx);
    if (CMP_NE(_fa, _fb)) goto loc_0002ECEA; /* jne: not equal / not zero */

loc_0002EBF0: ;
    RECOMP_PC(0x0002EBF0u);
    _fa = (uint32_t)(MEM16(edi + 0x24)) & 0xFFFFu; _fb = (uint32_t)(0xB) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(edi + 0x24), 0xB (16-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_NE(_fa, _fb)) goto loc_0002ECEA; /* jne: not equal / not zero */

loc_0002EBFB: ;
    RECOMP_PC(0x0002EBFBu);
    _fa = (uint32_t)(MEM8(esi + 0x490)) & 0xFFu; _fb = (uint32_t)(2) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test MEM8(esi + 0x490), 2 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    MEM32(esp + 0x10) = 0x3F99999A;
    MEM32(esp + 0x14) = 0;
    MEM32(esp + 0x18) = 0x3E99999A;
    if (TEST_Z(_fa, _fb)) goto loc_0002EC24; /* je: equal / zero */

loc_0002EC1C: ;
    RECOMP_PC(0x0002EC1Cu);
    MEM32(esp + 0x10) = 0xBF99999Au;

loc_0002EC24: ;
    RECOMP_PC(0x0002EC24u);
    eax = MEM32(esi + 0x18);
    PUSH32(esp, ebp);
    _fb = (uint32_t)(0x3C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0x3C)) >> 32) & 1);
    eax = eax + 0x3C;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, eax);
    ecx = esp + 0x18;
    PUSH32(esp, ecx);
    edx = esp + 0x28;
    PUSH32(esp, edx);
    PUSH32(esp, 0x0002EC3Bu); RECOMP_ABI_CALL(0x000CE420u, sub_000CE420); /* call 0x000CE420 */

loc_0002EC3B: ;
    RECOMP_PC(0x0002EC3Bu);
    ecx = MEM32(eax);
    MEM32(esp + 0x20) = ecx;
    edx = MEM32(eax + 4);
    MEM32(esp + 0x24) = edx;
    eax = MEM32(eax + 8);
    MEM32(esp + 0x28) = eax;
    eax = MEM32(0x1D9FB8);
    ecx = MEM32(eax + 0x10);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _cf = 0; /* xor clears CF */
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _cf = 0; /* xor clears CF */
    ebp = 0; /* xor self */
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_LE(_fas, _fbs)) goto loc_0002ECBF; /* jle: less or equal (signed <=) */

loc_0002EC62: ;
    RECOMP_PC(0x0002EC62u);
    MEM32(esp + 0x10) = 0x14;
    /* nop */

loc_0002EC70: ;
    RECOMP_PC(0x0002EC70u);
    ecx = MEM32(esp + 0x10);
    edi = MEM32(ecx + eax);
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(MEM32(esi + 0x478)) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, MEM32(esi + 0x478) (32-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_EQ(_fa, _fb)) goto loc_0002EC9F; /* je: equal / zero */

loc_0002EC7F: ;
    RECOMP_PC(0x0002EC7Fu);
    edx = MEM32(edi);
    ecx = edi;
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_target = MEM32(edx + 0x44); PUSH32(esp, 0x0002EC86u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x0002EC83u); } /* indirect call */
    }

loc_0002EC86: ;
    RECOMP_PC(0x0002EC86u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_NE(_fa, _fb)) goto loc_0002EC9F; /* jne: not equal / not zero */

loc_0002EC8A: ;
    RECOMP_PC(0x0002EC8Au);
    ecx = MEM32(edi + 0x44);
    PUSH32(esp, 0x3F800000);
    eax = esp + 0x18;
    PUSH32(esp, eax);
    PUSH32(esp, esi);
    PUSH32(esp, 0x0002EC9Du); RECOMP_ABI_CALL(0x0002AD00u, sub_0002AD00); /* call 0x0002AD00 */

loc_0002EC9D: ;
    RECOMP_PC(0x0002EC9Du);
    _cf = 0; /* logical op clears CF */
    ebx = ebx | eax;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */

loc_0002EC9F: ;
    RECOMP_PC(0x0002EC9Fu);
    edx = MEM32(esp + 0x10);
    eax = MEM32(0x1D9FB8);
    ecx = MEM32(eax + 0x10);
    ebp++;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edx) + (uint64_t)(4)) >> 32) & 1);
    edx = edx + 4;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fb = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebp, ecx (32-bit) */
    _cf = (int)(_fa < _fb);
    MEM32(esp + 0x10) = edx;
    if (CMP_L(_fas, _fbs)) goto loc_0002EC70; /* jl: less (signed <) */

loc_0002ECB7: ;
    RECOMP_PC(0x0002ECB7u);
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    edi = MEM32(esp + 0x34);
    if (CMP_NE(_fa, _fb)) goto loc_0002ECE9; /* jne: not equal / not zero */

loc_0002ECBF: ;
    RECOMP_PC(0x0002ECBFu);
    eax = MEM32(esi + 0x478);
    ebx = MEM32(eax + 0xC8);
    _fb = (uint32_t)(0xC0) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0xC0)) >> 32) & 1);
    eax = eax + 0xC0;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ebx++;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    MEM32(eax + 8) = ebx;
    eax = MEM32(esi + 0x478);
    ecx = MEM32(eax + 0xC8);
    _fb = (uint32_t)(0xC0) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0xC0)) >> 32) & 1);
    eax = eax + 0xC0;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ecx++;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    MEM32(eax + 8) = ecx;

loc_0002ECE9: ;
    RECOMP_PC(0x0002ECE9u);
    POP32(esp, ebp);

loc_0002ECEA: ;
    RECOMP_PC(0x0002ECEAu);
    _fa = (uint32_t)(MEM32(esi + 0x4DC)) & 0xFFFFFFFFu; _fb = (uint32_t)(7) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(esi + 0x4DC), 7 (32-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_NE(_fa, _fb)) goto loc_0002ED6D; /* jne: not equal / not zero */

loc_0002ECF3: ;
    RECOMP_PC(0x0002ECF3u);
    _fa = (uint32_t)(MEM32(esi + 0x48C)) & 0xFFFFFFFFu; _fb = (uint32_t)(3) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(esi + 0x48C), 3 (32-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_NE(_fa, _fb)) goto loc_0002ED6D; /* jne: not equal / not zero */

loc_0002ECFC: ;
    RECOMP_PC(0x0002ECFCu);
    ecx = MEM32(esi + 0x478);
    edx = MEM32(ecx);
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_target = MEM32(edx + 0x3C); PUSH32(esp, 0x0002ED07u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x0002ED04u); } /* indirect call */
    }

loc_0002ED07: ;
    RECOMP_PC(0x0002ED07u);
    ecx = (uint32_t)(int32_t)SMEM16(edi + 0x24);
    edx = ZX16(MEM16(edi + 4));
    _cf = (int)((eax) != 0);
    eax = (uint32_t)(-(int32_t)eax);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* neg result */
    eax = _cf ? 0xFFFFFFFF : 0; /* sbb self (CF extend) */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sbb result */
    _cf = 0; /* logical op clears CF */
    eax = eax & 0xF;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fb = (uint32_t)(5) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(5)) >> 32) & 1);
    eax = eax + 5;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fb = (uint32_t)(ecx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(edx) < (uint32_t)(ecx));
    edx = edx - ecx;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edx, eax (32-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_GE(_fas, _fbs)) goto loc_0002ED6D; /* jge: greater or equal (signed >=) */

loc_0002ED1F: ;
    RECOMP_PC(0x0002ED1Fu);
    ecx = MEM32(esi + 0x478);
    ebx = 1;
    PUSH32(esp, ebx);
    PUSH32(esp, 0x0002ED30u); RECOMP_ABI_CALL(0x000264C0u, sub_000264C0); /* call 0x000264C0 */

loc_0002ED30: ;
    RECOMP_PC(0x0002ED30u);
    eax = MEM32(esi + 0x478);
    SET_LO8(ecx, MEM8(eax + 0x55));
    _fa = (uint32_t)(LO8(ecx)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(ecx), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_BE(_fa, _fb)) goto loc_0002ED56; /* jbe: below or equal (unsigned <=) */

loc_0002ED3D: ;
    RECOMP_PC(0x0002ED3Du);
    ecx = MEM32(0x1D9FB8);
    eax = ZX8(MEM8(eax + 0x56));
    edx = MEM32(ecx + 0x58);
    ecx = MEM32(edx + eax * 4);
    PUSH32(esp, 0x0002ED52u); RECOMP_ABI_CALL(0x0009DDC0u, sub_0009DDC0); /* call 0x0009DDC0 */

loc_0002ED52: ;
    RECOMP_PC(0x0002ED52u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_NE(_fa, _fb)) goto loc_0002ED6D; /* jne: not equal / not zero */

loc_0002ED56: ;
    RECOMP_PC(0x0002ED56u);
    ecx = esi;
    MEM32(esi + 0x48C) = ebx;
    MEM32(esi + 0x4DC) = 8;
    PUSH32(esp, 0x0002ED6Du); RECOMP_ABI_CALL(0x0002CEC0u, sub_0002CEC0); /* call 0x0002CEC0 */

loc_0002ED6D: ;
    RECOMP_PC(0x0002ED6Du);
    _fa = (uint32_t)(MEM8(edi)) & 0xFFu; _fb = (uint32_t)(0x2B) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp MEM8(edi), 0x2B (8-bit) */
    _cf = (int)(_fa < _fb);
    POP32(esp, ebx);
    if (CMP_NE(_fa, _fb)) goto loc_0002EDC4; /* jne: not equal / not zero */

loc_0002ED73: ;
    RECOMP_PC(0x0002ED73u);
    _fa = (uint32_t)(MEM16(edi + 0x24)) & 0xFFFFu; _fb = (uint32_t)(0x24) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(edi + 0x24), 0x24 (16-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_NE(_fa, _fb)) goto loc_0002EDC4; /* jne: not equal / not zero */

loc_0002ED7A: ;
    RECOMP_PC(0x0002ED7Au);
    esi = MEM32(esi + 0x18);
    fp_push(MEMF(esi + 8)); /* fld float */
    eax = MEM32(esi + 0xC);
    fp_push(MEMF(esi + 0x10)); /* fld float */
    edx = MEM32(0x1DA00C);
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(8)) >> 32) & 1);
    esi = esi + 8;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x10) = eax;
    fp_push(MEMF(esp + 0x10)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x1C91CC)); /* fadd dword ptr [0x1c91cc] */
    PUSH32(esp, 1);
    { double _t = fp_top(); fp_top() = g_fp_stack[(g_fp_top + 2) & 7]; g_fp_stack[(g_fp_top + 2) & 7] = _t; } /* fxch st(2) */
    ecx = esp + 0x1C;
    MEMF(esp + 0x1C) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esp + 0x28) = 0x41200000;
    { double _t = fp_top(); fp_top() = fp_st1(); fp_st1() = _t; } /* fxch st(1) */
    PUSH32(esp, ecx);
    MEMF(esp + 0x24) = (float)fp_top(); fp_pop(); /* fstp */
    MEMF(esp + 0x28) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = MEM32(edx + 0x1C);
    PUSH32(esp, 0x0002EDC4u); RECOMP_ABI_CALL(0x00107440u, sub_00107440); /* call 0x00107440 */

loc_0002EDC4: ;
    RECOMP_PC(0x0002EDC4u);
    POP32(esp, edi);
    POP32(esp, esi);
    _fb = (uint32_t)(0x20) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x20)) >> 32) & 1);
    esp = esp + 0x20;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; return; /* ret 4 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

