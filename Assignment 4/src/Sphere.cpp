#include <windows.h>
#include <math.h>
#include <windows.h>
#include "DrawUtil3D.h"
#include "Global3D.h"

void setSphereData(double sphereRadius)
{
  POINT3D *pts;
  
  pts = (POINT3D *) malloc(MAX_SURFACE_EDGES * sizeof(POINT3D));
  int nPts = 0;

  const int nMaxLatSurfaces = 30; // horizontal
  const int nMaxLongSurfaces = 60; // vertical

  double r;

  r = sphereRadius;

  double theta1, phi1, theta2, phi2;
  double thetaMin, thetaMax, phiMin, phiMax; 
  double thetaDelta, phiDelta;
  double cosph1, sinph1, cosph2, sinph2;
  double costh1, sinth1, costh2, sinth2;  

  thetaMin = 0;
  thetaMax =  2.0 * PI;
  phiMin =  0;
  phiMax = PI;

  thetaDelta = 
    (thetaMax - thetaMin)/((double)nMaxLongSurfaces);
  phiDelta = 
    (phiMax - phiMin)/((double)nMaxLatSurfaces);  

  for (phi1 = 0; phi1 < phiMax; phi1 += phiDelta)
  {
    for (theta1 = 0; theta1 < thetaMax; theta1 += thetaDelta)
    {
      costh1 = cos(theta1);
      sinth1 = sin(theta1);
      cosph1 = cos(phi1);
      sinph1 = sin(phi1);
      
      theta2 = theta1 + thetaDelta;
      phi2 = phi1 + phiDelta;
      
      costh2 = cos(theta2);
      sinth2 = sin(theta2);
      cosph2 = cos(phi2);
      sinph2 = sin(phi2);

      nPts = 0;

      pts[nPts].x = r * costh1 * sinph1;    
      pts[nPts].y = r * sinth1 * sinph1;    
      pts[nPts].z = r * cosph1;    
      nPts++;

      pts[nPts].x = r * costh2 * sinph1;    
      pts[nPts].y = r * sinth2 * sinph1;    
      pts[nPts].z = r * cosph1;    
      nPts++;

      pts[nPts].x = r * costh2 * sinph2;    
      pts[nPts].y = r * sinth2 * sinph2;    
      pts[nPts].z = r * cosph2;    
      nPts++;
      
      pts[nPts].x = r * costh1 * sinph2;    
      pts[nPts].y = r * sinth1 * sinph2;    
      pts[nPts].z = r * cosph2;    
      nPts++;

      addSurface(pts, nPts);
    }
  }
  if (pts != NULL)
    free(pts);
}
