#include <windows.h>
#include "DrawUtil3D.h"
#include "Global3D.h"

void setCubeData(POINT3D origin,double cubeSide)
{
  // h is half the length of each cube side
  double h = cubeSide / 2;
  const POINT3D cubeCornersWorld[] = 
  {
    {origin.x + ( h),origin.y + (-h),origin.z + (-h)}, // A
    {origin.x + ( h),origin.y + ( h),origin.z + (-h)}, // B
    {origin.x + (-h),origin.y + ( h),origin.z + (-h)}, // C
    {origin.x + (-h),origin.y + (-h),origin.z + (-h)}, // D
    {origin.x + ( h),origin.y + (-h),origin.z + ( h)}, // E
    {origin.x + ( h),origin.y + ( h),origin.z + ( h)}, // F
    {origin.x + (-h),origin.y + ( h),origin.z + ( h)}, // G
    {origin.x + (-h),origin.y + (-h),origin.z + ( h)}  // H   
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
