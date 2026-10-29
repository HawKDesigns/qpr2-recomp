/* Independent triangle/expanded-pixel-box SAT model for host API tests.
 * This does not emulate a particular GPU or compile HLSL. */
static int coverage_candidate(const NvGpuTri *t,int x,int y)
{
    double lo_x=x-1.0/256.0,hi_x=x+1.0+1.0/256.0,lo_y=y-1.0/256.0,hi_y=y+1.0+1.0/256.0;
    double area=((double)t->v[1].pos[0]-t->v[0].pos[0])*((double)t->v[2].pos[1]-t->v[0].pos[1])-((double)t->v[1].pos[1]-t->v[0].pos[1])*((double)t->v[2].pos[0]-t->v[0].pos[0]);
    double minx=t->v[0].pos[0],maxx=minx,miny=t->v[0].pos[1],maxy=miny;
    for(unsigned i=1;i<3;i++){double px=t->v[i].pos[0],py=t->v[i].pos[1];if(px<minx)minx=px;if(px>maxx)maxx=px;if(py<miny)miny=py;if(py>maxy)maxy=py;}
    if(maxx<lo_x || minx>hi_x || maxy<lo_y || miny>hi_y)return 0;
    for(unsigned i=0;i<3;i++){
        const float *a=t->v[i].pos,*b=t->v[(i+1)%3].pos;double dx=(double)b[0]-a[0],dy=(double)b[1]-a[1],min_e=1e100,max_e=-1e100;
        for(unsigned corner=0;corner<4;corner++){double px=(corner&1)?hi_x:lo_x,py=(corner&2)?hi_y:lo_y,e=dx*(py-a[1])-dy*(px-a[0]);if(e<min_e)min_e=e;if(e>max_e)max_e=e;}
        if((area>0 && max_e<0) || (area<0 && min_e>0))return 0;
    }
    return 1;
}
