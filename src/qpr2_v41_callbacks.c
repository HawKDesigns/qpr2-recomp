/* Bounded routines recovered from the supplied retail disassembly. */

#define RECOMP_GENERATED_CODE

#include "recomp/gen/recomp_types.h"

#include "recomp/gen/recomp_funcs.h"

void qpr2_recovered_00020600(void);

void qpr2_recovered_00028740(void);

void qpr2_recovered_00029340(void);

/**
 * qpr2_recovered_00020600
 * Original: 0x00020600 - 0x000207D7 (471 bytes, 110 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00020600(void)
{
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;

loc_00020600: ;
    RECOMP_PC(0x00020600u);
    eax = MEM32(0x1DC3CC);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebx (32-bit) */
    PUSH32(esp, edi);
    esi = ecx;
    if (CMP_EQ(_fa, _fb)) goto loc_0002064B; /* je: equal / zero */

loc_00020613: ;
    RECOMP_PC(0x00020613u);
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = ebx;
    PUSH32(esp, 0x00020625u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_00020625: ;
    RECOMP_PC(0x00020625u);
    _fa = (uint32_t)(MEM32(0x1DC3CC)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC3CC), ebx (32-bit) */
    MEM32(0x1A51F8) = ebx;
    if (CMP_EQ(_fa, _fb)) goto loc_0002064B; /* je: equal / zero */

loc_00020633: ;
    RECOMP_PC(0x00020633u);
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = ebx;
    PUSH32(esp, 0x00020645u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_00020645: ;
    RECOMP_PC(0x00020645u);
    MEM32(0x1A51F8) = ebx;

loc_0002064B: ;
    RECOMP_PC(0x0002064Bu);
    eax = MEM32(0x1DC444);
    edi = 0x207;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edi (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_00020691; /* je: equal / zero */

loc_00020659: ;
    RECOMP_PC(0x00020659u);
    edx = edi;
    ecx = 0x40354;
    MEM32(0x1DC444) = edi;
    PUSH32(esp, 0x0002066Bu); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_0002066B: ;
    RECOMP_PC(0x0002066Bu);
    _fa = (uint32_t)(MEM32(0x1DC444)) & 0xFFFFFFFFu; _fb = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC444), edi (32-bit) */
    MEM32(0x1A51DC) = edi;
    if (CMP_EQ(_fa, _fb)) goto loc_00020691; /* je: equal / zero */

loc_00020679: ;
    RECOMP_PC(0x00020679u);
    edx = edi;
    ecx = 0x40354;
    MEM32(0x1DC444) = edi;
    PUSH32(esp, 0x0002068Bu); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_0002068B: ;
    RECOMP_PC(0x0002068Bu);
    MEM32(0x1A51DC) = edi;

loc_00020691: ;
    RECOMP_PC(0x00020691u);
    PUSH32(esp, ebx);
    PUSH32(esp, ebx);
    PUSH32(esp, ebx);
    MEM32(esp + 0x18) = 0;
    eax = MEM32(esp + 0x18);
    PUSH32(esp, ebx);
    MEM32(0x1CF1E0) = eax;
    MEM32(esp + 0x28) = 0x3F800000;
    eax = MEM32(esp + 0x28);
    MEM32(esp + 0x24) = 0;
    edx = MEM32(esp + 0x24);
    MEM32(esp + 0x20) = 0;
    ecx = MEM32(esp + 0x20);
    PUSH32(esp, ebx);
    edi = 1;
    PUSH32(esp, edi);
    MEM32(0x1CF1E4) = ecx;
    MEM32(0x1CF1E8) = edx;
    MEM32(0x1CF1EC) = eax;
    PUSH32(esp, 0x000206E7u); RECOMP_ABI_CALL(0x000E5430u, sub_000E5430); /* call 0x000E5430 */

loc_000206E7: ;
    RECOMP_PC(0x000206E7u);
    _fb = (uint32_t)(0x18) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x18;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, 0x200);
    ecx = esi + 0x28C;
    PUSH32(esp, ecx);
    ecx = MEM32(esi + 0x2E0);
    PUSH32(esp, 0x00020701u); RECOMP_ABI_CALL(0x001001C0u, sub_001001C0); /* call 0x001001C0 */

loc_00020701: ;
    RECOMP_PC(0x00020701u);
    MEM32(esp + 0x10) = 0x3F800000;
    eax = MEM32(esp + 0x10);
    MEM32(esp + 0xC) = 0x3F800000;
    edx = MEM32(esp + 0xC);
    MEM32(0x1CF1E4) = eax;
    eax = MEM32(0x1DC444);
    MEM32(esp + 0x18) = 0x3F800000;
    MEM32(esp + 0x14) = 0x3F800000;
    ecx = MEM32(esp + 0x14);
    MEM32(0x1CF1E0) = edx;
    edx = MEM32(esp + 0x18);
    esi = 0x203;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, esi (32-bit) */
    MEM32(0x1CF1E8) = ecx;
    MEM32(0x1CF1EC) = edx;
    if (CMP_EQ(_fa, _fb)) goto loc_0002078E; /* je: equal / zero */

loc_00020756: ;
    RECOMP_PC(0x00020756u);
    edx = esi;
    ecx = 0x40354;
    MEM32(0x1DC444) = esi;
    PUSH32(esp, 0x00020768u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_00020768: ;
    RECOMP_PC(0x00020768u);
    _fa = (uint32_t)(MEM32(0x1DC444)) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC444), esi (32-bit) */
    MEM32(0x1A51DC) = esi;
    if (CMP_EQ(_fa, _fb)) goto loc_0002078E; /* je: equal / zero */

loc_00020776: ;
    RECOMP_PC(0x00020776u);
    edx = esi;
    ecx = 0x40354;
    MEM32(0x1DC444) = esi;
    PUSH32(esp, 0x00020788u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_00020788: ;
    RECOMP_PC(0x00020788u);
    MEM32(0x1A51DC) = esi;

loc_0002078E: ;
    RECOMP_PC(0x0002078Eu);
    _fa = (uint32_t)(MEM32(0x1DC3CC)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC3CC), ebx (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_000207CE; /* jne: not equal / not zero */

loc_00020796: ;
    RECOMP_PC(0x00020796u);
    edx = edi;
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = edi;
    PUSH32(esp, 0x000207A8u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000207A8: ;
    RECOMP_PC(0x000207A8u);
    _fa = (uint32_t)(MEM32(0x1DC3CC)) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1DC3CC), ebx (32-bit) */
    MEM32(0x1A51F8) = edi;
    if (CMP_NE(_fa, _fb)) goto loc_000207CE; /* jne: not equal / not zero */

loc_000207B6: ;
    RECOMP_PC(0x000207B6u);
    edx = edi;
    ecx = 0x4035C;
    MEM32(0x1DC3CC) = edi;
    PUSH32(esp, 0x000207C8u); RECOMP_ABI_CALL(0x00193BC0u, sub_00193BC0); /* call 0x00193BC0 */

loc_000207C8: ;
    RECOMP_PC(0x000207C8u);
    MEM32(0x1A51F8) = edi;

loc_000207CE: ;
    RECOMP_PC(0x000207CEu);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esp += 8; return; /* ret 4 */

}


/**
 * qpr2_recovered_00028740
 * Original: 0x00028740 - 0x0002894E (526 bytes, 131 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00028740(void)
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

loc_00028740: ;
    RECOMP_PC(0x00028740u);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    eax = ecx;
    ecx = MEM32(eax + 0x2E0);
    ecx = MEM32(ecx + 0x108);
    edx = MEM32(ecx + 0xC);
    _fb = (uint32_t)(0x48) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edx = edx + 0x48;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, esi);
    esi = MEM32(eax + 0x4A4);
    PUSH32(esp, edi);
    edi = MEM32(edx);
    MEM32(esi) = edi;
    edi = MEM32(edx + 4);
    MEM32(esi + 4) = edi;
    edx = MEM32(edx + 8);
    MEM32(esi + 8) = edx;
    edx = MEM32(ecx + 0xC);
    edi = MEM32(edx + 0x1EC);
    esi = MEM32(eax + 0x4A4);
    MEM32(esi + 0x10) = edi;
    edi = MEM32(edx + 0x1F0);
    MEM32(esi + 0x14) = edi;
    _fb = (uint32_t)(0x1EC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edx = edx + 0x1EC;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    edx = MEM32(edx + 8);
    MEM32(esi + 0x18) = edx;
    edx = MEM32(ecx + 0xC);
    edi = MEM32(edx + 0x588);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + 0x10;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esi = MEM32(eax + 0x4A4);
    MEM32(esi + 0x20) = edi;
    edi = MEM32(edx + 0x58C);
    MEM32(esi + 0x24) = edi;
    _fb = (uint32_t)(0x588) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edx = edx + 0x588;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    edx = MEM32(edx + 8);
    MEM32(esi + 0x28) = edx;
    edx = MEM32(ecx + 0xC);
    edi = MEM32(edx + 0x2E8);
    _fb = (uint32_t)(0x20) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + 0x20;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esi = MEM32(eax + 0x4A4);
    MEM32(esi + 0x30) = edi;
    edi = MEM32(edx + 0x2EC);
    MEM32(esi + 0x34) = edi;
    _fb = (uint32_t)(0x2E8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edx = edx + 0x2E8;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    edx = MEM32(edx + 8);
    MEM32(esi + 0x38) = edx;
    edx = MEM32(ecx + 0xC);
    edi = MEM32(edx + 0x630);
    _fb = (uint32_t)(0x30) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + 0x30;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esi = MEM32(eax + 0x4A4);
    MEM32(esi + 0x40) = edi;
    edi = MEM32(edx + 0x634);
    MEM32(esi + 0x44) = edi;
    _fb = (uint32_t)(0x630) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edx = edx + 0x630;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    edx = MEM32(edx + 8);
    MEM32(esi + 0x48) = edx;
    edx = MEM32(ecx + 0xC);
    edi = MEM32(edx + 0x390);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + 0x40;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esi = MEM32(eax + 0x4A4);
    MEM32(esi + 0x50) = edi;
    edi = MEM32(edx + 0x394);
    _fb = (uint32_t)(0x390) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edx = edx + 0x390;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esi + 0x54) = edi;
    edx = MEM32(edx + 8);
    MEM32(esi + 0x58) = edx;
    edx = MEM32(ecx + 0xC);
    edi = MEM32(edx + 0x828);
    _fb = (uint32_t)(0x50) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + 0x50;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esi = MEM32(eax + 0x4A4);
    _fb = (uint32_t)(0x828) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edx = edx + 0x828;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esi + 0x60) = edi;
    edi = MEM32(edx + 4);
    _fb = (uint32_t)(0x60) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + 0x60;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esi + 4) = edi;
    edx = MEM32(edx + 8);
    MEM32(esi + 8) = edx;
    edx = MEM32(ecx + 0xC);
    esi = MEM32(eax + 0x4A4);
    edi = MEM32(edx + 0x978);
    _fb = (uint32_t)(0x978) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edx = edx + 0x978;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fb = (uint32_t)(0x70) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + 0x70;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esi) = edi;
    edi = MEM32(edx + 4);
    MEM32(esi + 4) = edi;
    edx = MEM32(edx + 8);
    MEM32(esi + 8) = edx;
    edx = MEM32(ecx + 0xC);
    edi = MEM32(edx + 0x87C);
    esi = MEM32(eax + 0x4A4);
    MEM32(esi + 0x80) = edi;
    edi = MEM32(edx + 0x880);
    MEM32(esi + 0x84) = edi;
    _fb = (uint32_t)(0x87C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edx = edx + 0x87C;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    edx = MEM32(edx + 8);
    MEM32(esi + 0x88) = edx;
    ecx = MEM32(ecx + 0xC);
    edx = MEM32(eax + 0x4A4);
    _fb = (uint32_t)(0x80) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esi = esi + 0x80;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    esi = MEM32(ecx + 0x9CC);
    MEM32(edx + 0x90) = esi;
    esi = MEM32(ecx + 0x9D0);
    MEM32(edx + 0x94) = esi;
    _fb = (uint32_t)(0x90) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    edx = edx + 0x90;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fb = (uint32_t)(0x9CC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    ecx = ecx + 0x9CC;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ecx = MEM32(ecx + 8);
    MEM32(edx + 8) = ecx;
    ecx = MEM32(eax + 0x4A4);
    edx = MEM32(ecx);
    MEM32(ecx + 0xA0) = edx;
    edx = MEM32(ecx + 4);
    MEM32(ecx + 0xA4) = edx;
    edx = MEM32(ecx + 8);
    MEM32(ecx + 0xA8) = edx;
    ecx = MEM32(eax + 0x64);
    eax = MEM32(eax + 0x18);
    fp_push(MEMF(eax + 0x10)); /* fld float */
    edx = MEM32(eax + 8);
    fp_push(MEMF(eax + 0xC)); /* fld float */
    _fb = (uint32_t)(0x3C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    eax = eax + 0x3C;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x1AEB20)); /* fadd dword ptr [0x1aeb20] */
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    eax = esp + 0xC;
    MEM32(esp + 0xC) = edx;
    MEMF(esp + 0x10) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, eax);
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(ecx);
    { uint32_t _icall_target = MEM32(edx + 8); PUSH32(esp, 0x00028946u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00028943u); } /* indirect call */
    }

loc_00028946: ;
    RECOMP_PC(0x00028946u);
    POP32(esp, edi);
    POP32(esp, esi);
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
 * qpr2_recovered_00029340
 * Original: 0x00029340 - 0x00029454 (276 bytes, 72 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
void qpr2_recovered_00029340(void)
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

loc_00029340: ;
    RECOMP_PC(0x00029340u);
    eax = MEM32(esp + 4);
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    esp = esp - 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, esi);
    PUSH32(esp, eax);
    esi = ecx;
    PUSH32(esp, 0x00029350u); RECOMP_ABI_CALL(0x00020940u, sub_00020940); /* call 0x00020940 */

loc_00029350: ;
    RECOMP_PC(0x00029350u);
    eax = MEM32(esi + 0x278);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0002944D; /* je: equal / zero */

loc_0002935E: ;
    RECOMP_PC(0x0002935Eu);
    ecx = MEM32(esi + 0x18);
    PUSH32(esp, edi);
    MEM32(esi + 0x4EC) = 0;
    edi = MEM32(ecx + 0x1BC);
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    if (CMP_EQ(_fa, _fb)) goto loc_0002944C; /* je: equal / zero */

loc_0002937A: ;
    RECOMP_PC(0x0002937Au);
    /* nop */

loc_00029380: ;
    RECOMP_PC(0x00029380u);
    ecx = MEM32(esi + 0x64);
    PUSH32(esp, edi);
    PUSH32(esp, 0x00029389u); RECOMP_ABI_CALL(0x00071000u, sub_00071000); /* call 0x00071000 */

loc_00029389: ;
    RECOMP_PC(0x00029389u);
    fp_push(MEMF(edi + 0x78)); /* fld float */
    eax = MEM32(eax * 4 + 0x1CA6D8);
    edx = eax + eax * 2;
    eax = MEM32(esi + 0x4A8);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + edx * 4 + 8)); /* fadd dword ptr [eax + edx*4 + 8] */
    eax = eax + edx * 4 + 8;
    MEMF(eax) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esi + 0x4DC);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(3) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 3 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_00029429; /* jne: not equal / not zero */

loc_000293B1: ;
    RECOMP_PC(0x000293B1u);
    fp_push(MEMF(edi + 0x70)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1AF32C)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1af32c] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_00029429; /* jp: parity */

loc_000293C1: ;
    RECOMP_PC(0x000293C1u);
    edx = MEM32(esi + 0x480);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 4);
    PUSH32(esp, edx);
    PUSH32(esp, 0);
    PUSH32(esp, 0x3D23D70A);
    PUSH32(esp, 0);
    MEM32(esi + 0x4DC) = 4;
    MEM32(esi + 0x48C) = 1;
    SET_LO8(ecx, MEM8(0x1C3E5C));
    eax = esp + 0x24;
    PUSH32(esp, 0x1F);
    PUSH32(esp, eax);
    MEM8(esp + 0x2C) = LO8(ecx);
    MEM8(esp + 0x2D) = 0xFF;
    PUSH32(esp, 0x00029406u); RECOMP_ABI_CALL(0x000FA690u, sub_000FA690); /* call 0x000FA690 */

loc_00029406: ;
    RECOMP_PC(0x00029406u);
    _fb = (uint32_t)(0x24) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    esp = esp + 0x24;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ecx = esp + 8;
    PUSH32(esp, ecx);
    ecx = esi;
    MEM32(esi + 0x4A0) = 0x10;
    MEM32(esi + 0x53C) = 0;
    PUSH32(esp, 0x00029429u); RECOMP_ABI_CALL(0x00020880u, sub_00020880); /* call 0x00020880 */

loc_00029429: ;
    RECOMP_PC(0x00029429u);
    fp_push(MEMF(edi + 0x78)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esi + 0x4EC)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esi + 0x4ec] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    if (TEST_NZ(_fa, _fb)) goto loc_00029442; /* jne: not equal / not zero */

loc_00029439: ;
    RECOMP_PC(0x00029439u);
    edx = MEM32(edi + 0x78);
    MEM32(esi + 0x4EC) = edx;

loc_00029442: ;
    RECOMP_PC(0x00029442u);
    edi = MEM32(edi);
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    if (CMP_NE(_fa, _fb)) goto loc_00029380; /* jne: not equal / not zero */

loc_0002944C: ;
    RECOMP_PC(0x0002944Cu);
    POP32(esp, edi);

loc_0002944D: ;
    RECOMP_PC(0x0002944Du);
    POP32(esp, esi);
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

