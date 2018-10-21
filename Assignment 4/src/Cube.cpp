#include <windows.h>
#include "DrawUtil3D.h"
#include "Global3D.h"

void setCubeData(double cubeSide)
{
  // h is half the length of each cube side
  double h = cubeSide / 2;
  const POINT3D cubeCornersWorld[] = 
  {
    { h, -h, -h}, // A
    { h,  h, -h}, // B
    {-h,  h, -h}, // C
    {-h, -h, -h}, // D
    { h, -h,  h}, // E
    { h,  h,  h}, // F
    {-h,  h,  h}, // G
    {-h, -h,  h}  // H   
  };

  // ABCD
  addSurface(cubeCornersWorld[0], 
             cubeCornersWorld[1],               
             cubeCornersWorld[2], 
             cubeCornersWorld[3]); 
  // BCGF 
  addSurface(cubeCornersWorld[1], 
             cubeCornersWorld[2], 
             cubeCornersWorld[6], 
             cubeCornersWorld[5]);
  // CGHD  
  addSurface(cubeCornersWorld[2], 
             cubeCornersWorld[6], 
             cubeCornersWorld[7], 
             cubeCornersWorld[3]); 
  // GHEF  
  
  addSurface(cubeCornersWorld[6], 
             cubeCornersWorld[7], 
             cubeCornersWorld[4],               
             cubeCornersWorld[5]);
  // HEAD 
  addSurface(cubeCornersWorld[7], 
             cubeCornersWorld[4], 
             cubeCornersWorld[0], 
             cubeCornersWorld[3]);
  // EFBA
  addSurface(cubeCornersWorld[4], 
             cubeCornersWorld[5], 
             cubeCornersWorld[1], 
             cubeCornersWorld[0]); 
}
