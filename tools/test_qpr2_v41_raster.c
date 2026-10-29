#define main baseline_raster_main
#include "test_qpr2_raster_capture.c"
#undef main
int main(void) {
    baseline_raster_main();
    setup();
    Nv2aVshOutput a=point(-1e8f,-1e8f),b=point(1e8f,-1e8f),c=point(0,1e8f);
    a.pos[2]=b.pos[2]=c.pos[2]=100;
    raster_xf_clipped(&a,&b,&c);
    for(int i=0;i<64;i++)assert(((uint32_t*)(ram+0x1000))[i]==0xFFFF0000);
    /* Homogeneous triangle: x_h+y_h=8; its positive-w visible portion
     * projects to x+y >= 8, with the diagonal owned by this winding. */
    setup();a=point(-4,-4);a.pos[3]=-1;b=point(8,0);c=point(0,8);
    raster_xf_clipped(&a,&b,&c);
    for(int y=0;y<8;y++)for(int x=0;x<8;x++)
        assert((((uint32_t*)(ram+0x1000))[y*8+x]!=0)==(x+y>=7));
    /* Offscreen and wholly behind triangles write nothing. */
    setup();a=point(-100,-100);b=point(-50,-100);c=point(-50,-50);
    raster_xf_clipped(&a,&b,&c);assert(s_gpu.xf_pixels==0);
    a.pos[3]=b.pos[3]=c.pos[3]=-1;raster_xf_clipped(&a,&b,&c);assert(s_gpu.xf_pixels==0);
    /* Repeated passes must compare in Z24/F24 precision, rather than in
     * all the extra bits of the software float interpolation. */
    for(int floating=0;floating<2;floating++) {
        setup();s_gpu.depth_test=1;s_gpu.depth_mask=1;s_gpu.depth_func=0x207;
        s_gpu.control0=floating?1u<<12:0;
        float z=100.125f,z2=100.25f;
        if(floating){uint32_t bits=0x7000000A;memcpy(&z,&bits,4);bits+=80;memcpy(&z2,&bits,4);}
        a=point(0,0);b=point(8,0);c=point(8,8);Nv2aVshOutput d=point(0,8);
        a.pos[2]=b.pos[2]=c.pos[2]=d.pos[2]=z;
        raster_xf_triangle(&a,&b,&c);raster_xf_triangle(&a,&c,&d);
        uint64_t old=s_gpu.xf_pixels;
        s_gpu.depth_func=0x202;s_gpu.depth_mask=0;
        a.pos[2]=b.pos[2]=c.pos[2]=d.pos[2]=z2;
        raster_xf_triangle(&a,&b,&c);raster_xf_triangle(&a,&c,&d);
        assert(s_gpu.xf_pixels==old+64 && s_gpu.xf_depth_fail==0);
        assert(nv2a_depth_store(s_gpu.format,s_gpu.control0,z)==nv2a_depth_store(s_gpu.format,s_gpu.control0,z2));
    }
    puts("PASS: huge projected triangles, mixed eye-plane homogeneous coverage, offscreen/behind clipping, coplanar Z24/F24 EQUAL passes and prior raster/capture regressions");
}
