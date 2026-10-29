/* Bounded routines recovered from the supplied retail disassembly. */

#define RECOMP_GENERATED_CODE

#include "recomp/gen/recomp_types.h"

#include "recomp/gen/recomp_funcs.h"

void qpr2_recovered_00037F90(void);

/**
 * qpr2_recovered_00037F90
 * Original: 0x00037F90 - 0x000380DF (335 bytes, 90 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00037F90(void)
{
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;

loc_00037F90: ;
    RECOMP_PC(0x00037F90u);
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, esi);
    esi = ecx;
    eax = MEM32(esi + 0xA4);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_000380D8; /* je: equal / zero */

loc_00037FA4: ;
    RECOMP_PC(0x00037FA4u);
    eax = MEM32(esi + 0x2C);
    ecx = MEM32(esi + 0x24);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    edx = esi + 0xC;
    PUSH32(esp, edx);
    ecx = esp + 0x20;
    PUSH32(esp, 0x00037FB9u); RECOMP_ABI_CALL(0x00037EF0u, sub_00037EF0); /* call 0x00037EF0 */

loc_00037FB9: ;
    RECOMP_PC(0x00037FB9u);
    ecx = MEM32(esp + 0x30);
    eax = esp + 0x14;
    PUSH32(esp, eax);
    PUSH32(esp, 0x00037FC7u); RECOMP_ABI_CALL(0x00037C80u, sub_00037C80); /* call 0x00037C80 */

loc_00037FC7: ;
    RECOMP_PC(0x00037FC7u);
    edx = MEM32(esi);
    eax = esp + 0x14;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    ecx = esi;
    { uint32_t _icall_target = MEM32(edx + 0x60); PUSH32(esp, 0x00037FD3u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00037FD0u); } /* indirect call */
    }

loc_00037FD3: ;
    RECOMP_PC(0x00037FD3u);
    PUSH32(esp, 0x00037FD8u); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_00037FD8: ;
    RECOMP_PC(0x00037FD8u);
    PUSH32(esp, 0);
    PUSH32(esp, 0x00037FDFu); RECOMP_ABI_CALL(0x0001D270u, sub_0001D270); /* call 0x0001D270 */

loc_00037FDF: ;
    RECOMP_PC(0x00037FDFu);
    edx = MEM32(0x1DC428);
    eax = 3;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edx, eax (32-bit) */
    ecx = 1;
    if (CMP_EQ(_fa, _fb)) goto loc_0003800E; /* je: equal / zero */

loc_00037FF6: ;
    RECOMP_PC(0x00037FF6u);
    edx = MEM32(0x1A50F0);
    edx = edx | ecx;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    MEM32(0x1DC428) = eax;
    MEM32(0x1A50F0) = edx;
    MEM32(0x1A2EA0) = eax;

loc_0003800E: ;
    RECOMP_PC(0x0003800Eu);
    _fa = (uint32_t)(MEM32(0x1DC368)) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC368), eax (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0003802E; /* je: equal / zero */

loc_00038016: ;
    RECOMP_PC(0x00038016u);
    edx = MEM32(0x1A50F0);
    edx = edx | ecx;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    MEM32(0x1DC368) = eax;
    MEM32(0x1A50F0) = edx;
    MEM32(0x1A2EA4) = eax;

loc_0003802E: ;
    RECOMP_PC(0x0003802Eu);
    ecx = MEM32(esi + 0x94);
    PUSH32(esp, ecx);
    PUSH32(esp, 0);
    PUSH32(esp, 0x0003803Cu); RECOMP_ABI_CALL(0x000E6A00u, sub_000E6A00); /* call 0x000E6A00 */

loc_0003803C: ;
    RECOMP_PC(0x0003803Cu);
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    ecx = esp + 0x40;
    PUSH32(esp, 0x0003805Cu); RECOMP_ABI_CALL(0x00015CD0u, sub_00015CD0); /* call 0x00015CD0 */

loc_0003805C: ;
    RECOMP_PC(0x0003805Cu);
    edx = MEM32(esp + 0x30);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, edx);
    PUSH32(esp, 0x3F800000);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    MEM32(esp + 0x2C) = 0xBF800000u;
    ecx = MEM32(esp + 0x2C);
    eax = esp;
    MEM32(eax) = ecx;
    MEM32(esp + 0x30) = 0x3F800000;
    edx = MEM32(esp + 0x30);
    MEM32(eax + 4) = edx;
    MEM32(esp + 0x34) = 0x40000000;
    ecx = MEM32(esp + 0x34);
    MEM32(eax + 8) = ecx;
    MEM32(esp + 0x38) = 0xC0000000u;
    edx = MEM32(esp + 0x38);
    ecx = esp + 0x3C;
    MEM32(eax + 0xC) = edx;
    PUSH32(esp, 0x000380BDu); RECOMP_ABI_CALL(0x00112200u, sub_00112200); /* call 0x00112200 */

loc_000380BD: ;
    RECOMP_PC(0x000380BDu);
    PUSH32(esp, 0);
    PUSH32(esp, 0x000380C4u); RECOMP_ABI_CALL(0x0001D370u, sub_0001D370); /* call 0x0001D370 */

loc_000380C4: ;
    RECOMP_PC(0x000380C4u);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x000380CCu); RECOMP_ABI_CALL(0x0012AF20u, sub_0012AF20); /* call 0x0012AF20 */

loc_000380CC: ;
    RECOMP_PC(0x000380CCu);
    eax = MEM32(esi);
    ecx = esp + 0x14;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    ecx = esi;
    { uint32_t _icall_target = MEM32(eax + 0x64); PUSH32(esp, 0x000380D8u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000380D5u); } /* indirect call */
    }

loc_000380D8: ;
    RECOMP_PC(0x000380D8u);
    POP32(esp, esi);
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; return; /* ret 4 */

}

