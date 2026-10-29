#include "../third_party/xboxrecomp/src/kernel/nv2a_pb_exec.c"
#include <assert.h>
#include <sys/mman.h>
extern uint8_t *g_apu_ram_ptr;
extern uint8_t *mcpx_apu_phys(uint64_t address);
extern uintptr_t xbox_MmGetPhysicalAddress(void *address);
extern uint32_t qpr2_test_usb_bus_resolve(uint32_t address);
uint32_t g_xbox_image_lo=0x10000, g_xbox_image_hi=0x1C0000;
static uint8_t *memory;
ptrdiff_t xbox_GetMemoryOffset(void) { return (ptrdiff_t)memory; }
uint32_t xbox_ContiguousAllocatedBytes(void) { return 0x2000000; }
int main(void)
{
    size_t size=0x84000000ull;
    memory=mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS|MAP_NORESERVE,-1,0);
    assert(memory!=MAP_FAILED); g_apu_ram_ptr=memory;
    const uint32_t at=0x010B5CA0;
    memcpy(memory+at,"sound",6); memcpy(memory+0x80000000ull+at,"model",6);
    /* Reproduce the ambiguity with precisely the logged sample offset. */
    assert(!strcmp((char*)mcpx_apu_phys(at),"model"));
    uint32_t low=(uint32_t)xbox_MmGetPhysicalAddress((void*)(uintptr_t)at);
    uint32_t contig=(uint32_t)xbox_MmGetPhysicalAddress((void*)(uintptr_t)(0x80000000u+at));
    assert(low==0x050B5CA0 && contig==at && low!=contig);
    assert(!strcmp((char*)mcpx_apu_phys(low),"sound"));
    assert(!strcmp((char*)mcpx_apu_phys(contig),"model"));
    assert(dma_resolve(low)==at && dma_resolve(contig)==0x80000000u+at);
    assert(qpr2_test_usb_bus_resolve(low)==at);
    assert(qpr2_test_usb_bus_resolve(contig)==0x80000000u+at);
    assert(xbox_bus_encode(0)==0 && xbox_bus_encode(0x80000000)==0);
    assert(xbox_bus_encode(0x83FFFFFF)==0x03FFFFFF);
    assert(xbox_bus_encode(0x03FFFFFF)==0x07FFFFFF);
    assert(xbox_bus_encode(0x84000000)==0x84000000);
    munmap(memory,size);
    puts("PASS: actual kernel physical-address API, APU/GPU/USB bus resolution, overlapping sample/model offsets and address boundaries");
}
