/* Exact bounded routines recovered from the supplied QPR2 disassembly. */
#include <stdio.h>
#define RECOMP_GENERATED_CODE
#include "recomp/gen/recomp_types.h"
#include "recomp/gen/recomp_funcs.h"

/**
 * qpr2_recovered_00128880
 * Original: 0x00128880 - 0x0012888E (14 bytes, 6 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00128880(void)
{
    fprintf(stderr, "[WORKER-RECOVERED] 00128880 object=%08X\n", MEM32(g_esp + 4u));

loc_00128880: ;
    RECOMP_PC(0x00128880u);
    PUSH32(esp, esi);
    esi = MEM32(esp + 8);

loc_00128885: ;
    RECOMP_PC(0x00128885u);
    eax = MEM32(esi);
    ecx = esi;
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_target = MEM32(eax + 0xC); PUSH32(esp, 0x0012888Cu); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00128889u); } /* indirect call */
    }

loc_0012888C: ;
    RECOMP_PC(0x0012888Cu);
    goto loc_00128885;

}

/**
 * qpr2_recovered_00087CC0
 * Original: 0x00087CC0 - 0x00087D42 (130 bytes, 43 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00087CC0(void)
{
    fprintf(stderr, "[WORKER-RECOVERED] 00087CC0 service loop entered\n");
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;

loc_00087CC0: ;
    RECOMP_PC(0x00087CC0u);
    PUSH32(esp, esi);
    PUSH32(esp, edi);

loc_00087CC2: ;
    RECOMP_PC(0x00087CC2u);
    ecx = MEM32(0x1D9348);
    goto loc_00087CD0;

    RECOMP_PC(0x00087CCAu);
    /* nop */

loc_00087CD0: ;
    RECOMP_PC(0x00087CD0u);
    eax = MEM32(0x1D9358);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00087D36; /* je: equal / zero */

loc_00087CD9: ;
    RECOMP_PC(0x00087CD9u);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, 0 (32-bit) */
    if (CMP_LE(_fas, _fbs)) goto loc_00087CE9; /* jle: less or equal (signed <=) */

loc_00087CDD: ;
    RECOMP_PC(0x00087CDDu);
    eax = 0x10;
    edx = ((int32_t)eax < 0) ? 0xFFFFFFFF : 0; /* cdq */
    { int64_t _dividend = ((int64_t)(int32_t)edx << 32) | eax;
      eax = (uint32_t)((int32_t)(_dividend / (int32_t)ecx));
      edx = (uint32_t)((int32_t)(_dividend % (int32_t)ecx)); }
    edi = eax;
    goto loc_00087CEE;

loc_00087CE9: ;
    RECOMP_PC(0x00087CE9u);
    edi = 0x64;

loc_00087CEE: ;
    RECOMP_PC(0x00087CEEu);
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, 0 (32-bit) */
    if (CMP_LE(_fas, _fbs)) goto loc_00087CD0; /* jle: less or equal (signed <=) */

loc_00087CF4: ;
    RECOMP_PC(0x00087CF4u);
    eax = MEM32(0x1D934C);
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, eax);
    PUSH32(esp, 0x00087D01u); RECOMP_ABI_CALL(0x000C9B9Fu, sub_000C9B9F); /* call 0x000C9B9F */

loc_00087D01: ;
    RECOMP_PC(0x00087D01u);
    ecx = MEM32(0x1D9358);
    ecx = MEM32(ecx + esi * 4);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00087D13; /* je: equal / zero */

loc_00087D0E: ;
    RECOMP_PC(0x00087D0Eu);
    edx = MEM32(ecx);
    { uint32_t _icall_esp = g_esp;
    { uint32_t _icall_target = MEM32(edx + 0x20); PUSH32(esp, 0x00087D13u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00087D10u); } /* indirect call */
    }

loc_00087D13: ;
    RECOMP_PC(0x00087D13u);
    eax = MEM32(0x1D934C);
    PUSH32(esp, eax);
    PUSH32(esp, 0x00087D1Eu); RECOMP_ABI_CALL(0x000C9AE9u, sub_000C9AE9); /* call 0x000C9AE9 */

loc_00087D1E: ;
    RECOMP_PC(0x00087D1Eu);
    PUSH32(esp, 0x00087D23u); RECOMP_ABI_CALL(0x000C9CEBu, sub_000C9CEB); /* call 0x000C9CEB */

loc_00087D23: ;
    RECOMP_PC(0x00087D23u);
    PUSH32(esp, edi);
    PUSH32(esp, 0x00087D29u); RECOMP_ABI_CALL(0x000C9BB1u, sub_000C9BB1); /* call 0x000C9BB1 */

loc_00087D29: ;
    RECOMP_PC(0x00087D29u);
    ecx = MEM32(0x1D9348);
    esi++;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, ecx (32-bit) */
    if (CMP_L(_fas, _fbs)) goto loc_00087CF4; /* jl: less (signed <) */

loc_00087D34: ;
    RECOMP_PC(0x00087D34u);
    goto loc_00087CD0;

loc_00087D36: ;
    RECOMP_PC(0x00087D36u);
    PUSH32(esp, 0x3E8);
    PUSH32(esp, 0x00087D40u); RECOMP_ABI_CALL(0x000C9BB1u, sub_000C9BB1); /* call 0x000C9BB1 */

loc_00087D40: ;
    RECOMP_PC(0x00087D40u);
    goto loc_00087CC2;

}
