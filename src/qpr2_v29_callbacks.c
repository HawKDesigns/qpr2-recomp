/* Bounded routines recovered from the supplied retail disassembly. */

#define RECOMP_GENERATED_CODE

#include "recomp/gen/recomp_types.h"

#include "recomp/gen/recomp_funcs.h"

void qpr2_recovered_000BAC20(void);

/**
 * qpr2_recovered_000BAC20
 * Original: 0x000BAC20 - 0x000BAD92 (370 bytes, 122 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_000BAC20(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_000BAC20: ;
    RECOMP_PC(0x000BAC20u);
    PUSH32(esp, esi);
    esi = ecx;
    ecx = MEM32(esp + 8);
    eax = MEM32(ecx + 4);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000BAD08; /* jne: not equal / not zero */

loc_000BAC32: ;
    RECOMP_PC(0x000BAC32u);
    eax = MEM32(ecx);
    PUSH32(esp, eax);
    PUSH32(esp, 0x1B62EC);
    PUSH32(esp, 0x000BAC3Fu); RECOMP_ABI_CALL(0x000E12E0u, sub_000E12E0); /* call 0x000E12E0 */

loc_000BAC3F: ;
    RECOMP_PC(0x000BAC3Fu);
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000BAD8C; /* jne: not equal / not zero */

loc_000BAC4A: ;
    RECOMP_PC(0x000BAC4Au);
    eax = MEM32(esi + 0x14);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000BAD8C; /* jne: not equal / not zero */

loc_000BAC55: ;
    RECOMP_PC(0x000BAC55u);
    ecx = MEM32(esi + 0x10);
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, 0x000BAC5Fu); RECOMP_ABI_CALL(0x0005A050u, sub_0005A050); /* call 0x0005A050 */

loc_000BAC5F: ;
    RECOMP_PC(0x000BAC5Fu);
    ebx = eax;
    ebp = esi + 0x20;
    PUSH32(esp, ebx);
    ecx = ebp;
    PUSH32(esp, 0x000BAC6Cu); RECOMP_ABI_CALL(0x00012E40u, sub_00012E40); /* call 0x00012E40 */

loc_000BAC6C: ;
    RECOMP_PC(0x000BAC6Cu);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000BACF4; /* jne: not equal / not zero */

loc_000BAC74: ;
    RECOMP_PC(0x000BAC74u);
    ecx = MEM32(esi + 0x18);
    PUSH32(esp, edi);
    MEM32(esi + 0x14) = 1;
    PUSH32(esp, ecx);
    ecx = MEM32(0x1D3EF0);
    PUSH32(esp, 0x000BAC8Bu); RECOMP_ABI_CALL(0x000BAB20u, sub_000BAB20); /* call 0x000BAB20 */

loc_000BAC8B: ;
    RECOMP_PC(0x000BAC8Bu);
    edi = eax;
    edx = MEM32(edi);
    ecx = edi;
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_target = MEM32(edx + 4); PUSH32(esp, 0x000BAC94u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000BAC91u); } /* indirect call */
    }

loc_000BAC94: ;
    RECOMP_PC(0x000BAC94u);
    ecx = edi;
    PUSH32(esp, 0x000BAC9Bu); RECOMP_ABI_CALL(0x000144B0u, sub_000144B0); /* call 0x000144B0 */

loc_000BAC9B: ;
    RECOMP_PC(0x000BAC9Bu);
    PUSH32(esp, ebx);
    ecx = edi + 8;
    PUSH32(esp, 0x000BACA4u); RECOMP_ABI_CALL(0x000E18A0u, sub_000E18A0); /* call 0x000E18A0 */

loc_000BACA4: ;
    RECOMP_PC(0x000BACA4u);
    ecx = MEM32(esi + 0x1C);
    eax = MEM32(edi + 0x20);
    MEM32(eax + 4) = ecx;
    MEM32(edi + 0x3C) = 1;
    ecx = MEM32(0x1D3F0C);
    PUSH32(esp, edi);
    PUSH32(esp, 0x000BACC0u); RECOMP_ABI_CALL(0x00012C00u, sub_00012C00); /* call 0x00012C00 */

loc_000BACC0: ;
    RECOMP_PC(0x000BACC0u);
    PUSH32(esp, edi);
    ecx = ebp;
    PUSH32(esp, 0x000BACC8u); RECOMP_ABI_CALL(0x00013060u, sub_00013060); /* call 0x00013060 */

loc_000BACC8: ;
    RECOMP_PC(0x000BACC8u);
    ecx = MEM32(0x1DE810);
    PUSH32(esp, 0);
    PUSH32(esp, 0x1C0884);
    PUSH32(esp, 0x000BACDAu); RECOMP_ABI_CALL(0x000EBD00u, sub_000EBD00); /* call 0x000EBD00 */

loc_000BACDA: ;
    RECOMP_PC(0x000BACDAu);
    edx = MEM32(esi);
    ecx = esi;
    MEM32(esi + 0x28) = 2;
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_target = MEM32(edx + 0xC); PUSH32(esp, 0x000BACE8u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000BACE5u); } /* indirect call */
    }

loc_000BACE8: ;
    RECOMP_PC(0x000BACE8u);
    POP32(esp, edi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    eax = 1;
    POP32(esp, esi);
    esp += 8; return; /* ret 4 */

loc_000BACF4: ;
    RECOMP_PC(0x000BACF4u);
    ecx = MEM32(esi + 0x10);
    PUSH32(esp, eax);
    PUSH32(esp, 0x000BACFDu); RECOMP_ABI_CALL(0x0005A070u, sub_0005A070); /* call 0x0005A070 */

loc_000BACFD: ;
    RECOMP_PC(0x000BACFDu);
    POP32(esp, ebp);
    POP32(esp, ebx);
    eax = 1;
    POP32(esp, esi);
    esp += 8; return; /* ret 4 */

loc_000BAD08: ;
    RECOMP_PC(0x000BAD08u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(1) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 1 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000BAD5A; /* jne: not equal / not zero */

loc_000BAD0D: ;
    RECOMP_PC(0x000BAD0Du);
    edx = MEM32(esi + 0x14);
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edx, 0 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000BAD5A; /* jne: not equal / not zero */

loc_000BAD14: ;
    RECOMP_PC(0x000BAD14u);
    PUSH32(esp, 0x20);
    MEM32(esi + 0x14) = eax;
    PUSH32(esp, 0x000BAD1Eu); RECOMP_ABI_CALL(0x0012B267u, sub_0012B267); /* call 0x0012B267 */

loc_000BAD1E: ;
    RECOMP_PC(0x000BAD1Eu);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000BAD46; /* je: equal / zero */

loc_000BAD25: ;
    RECOMP_PC(0x000BAD25u);
    ecx = MEM32(esi + 0x1C);
    edx = MEM32(esi + 0x18);
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    ecx = eax;
    PUSH32(esp, 0x000BAD34u); RECOMP_ABI_CALL(0x000BAAC0u, sub_000BAAC0); /* call 0x000BAAC0 */

loc_000BAD34: ;
    RECOMP_PC(0x000BAD34u);
    ecx = MEM32(esi + 4);
    PUSH32(esp, eax);
    PUSH32(esp, 0x000BAD3Du); RECOMP_ABI_CALL(0x000BCA10u, sub_000BCA10); /* call 0x000BCA10 */

loc_000BAD3D: ;
    RECOMP_PC(0x000BAD3Du);
    eax = 1;
    POP32(esp, esi);
    esp += 8; return; /* ret 4 */

loc_000BAD46: ;
    RECOMP_PC(0x000BAD46u);
    ecx = MEM32(esi + 4);
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    PUSH32(esp, eax);
    PUSH32(esp, 0x000BAD51u); RECOMP_ABI_CALL(0x000BCA10u, sub_000BCA10); /* call 0x000BCA10 */

loc_000BAD51: ;
    RECOMP_PC(0x000BAD51u);
    eax = 1;
    POP32(esp, esi);
    esp += 8; return; /* ret 4 */

loc_000BAD5A: ;
    RECOMP_PC(0x000BAD5Au);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(6) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 6 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000BAD8C; /* jne: not equal / not zero */

loc_000BAD5F: ;
    RECOMP_PC(0x000BAD5Fu);
    eax = MEM32(ecx + 8);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(2) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 2 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000BAD7C; /* je: equal / zero */

loc_000BAD67: ;
    RECOMP_PC(0x000BAD67u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(8) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 8 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000BAD7C; /* je: equal / zero */

loc_000BAD6C: ;
    RECOMP_PC(0x000BAD6Cu);
    MEM32(esi + 0x28) = 4;
    eax = 1;
    POP32(esp, esi);
    esp += 8; return; /* ret 4 */

loc_000BAD7C: ;
    RECOMP_PC(0x000BAD7Cu);
    MEM32(esi + 0x28) = 2;
    eax = 1;
    POP32(esp, esi);
    esp += 8; return; /* ret 4 */

loc_000BAD8C: ;
    RECOMP_PC(0x000BAD8Cu);
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    POP32(esp, esi);
    esp += 8; return; /* ret 4 */

}

