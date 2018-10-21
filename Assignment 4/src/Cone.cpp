#include <windows.h>
#include <math.h>
#include "DrawUtil3D.h"
#include "Global3D.h"

void setConeData(double coneBaseRadius, double coneHeight)
{
  double h, h2, R, rh, rh2, H, halfH;
  POINT3D pt[100];
  int nPts, i, k, m, n;
  double a1, a2;
  double cosa1, cosa2, sina1, sina2;
  double aDelta;
  double hDelta;

  H = coneHeight;
  halfH = H / 2.0;
  R = coneBaseRadius;
  n = 30; // no of sampled points on circumference
  hDelta = 20;
  m = (int)(H / hDelta); // number of equi-spaced circles
  if (((int)H) > (int)(m*hDelta)) 
    m = m + 1;// adjustment for last leap
  aDelta = 2.0 * PI / n;

  for (i = 0; i < m; i++)
  {
    h = i * hDelta; // height from base
    rh = R * (1.0 - h/H);
    h2 = h + hDelta;
    rh2 = R * (1.0 - h2/H);
    for (k = 0; k < n; k++)
    {
      nPts = 0;
      a1 = k*aDelta;
      a2 = a1 + aDelta;
      cosa1 = cos(a1);
      cosa2 = cos(a2);
      sina1 = sin(a1);
      sina2 = sin(a2);
      // sampled point on the lower circumference 
      pt[nPts].x = rh * cosa1; 
      pt[nPts].y = rh * sina1;
      pt[nPts].z = halfH - h;
      nPts++;

      // another point on the lower circumference
      pt[nPts].x = rh * cosa2; 
      pt[nPts].y = rh * sina2;
      pt[nPts].z = halfH - h;
      nPts++;

      if (i == (m-1))
      {
        // add cone top for triangular planar patch
        pt[nPts].x = 0;
        pt[nPts].y = 0;
        pt[nPts].z = -halfH;
        nPts++;        
      }
      else
      {
        // 4-cornered planar patch, add 2 more points
        // sampled point on the upper circumference         
        pt[nPts].x = rh2 * cosa2;
        pt[nPts].y = rh2 * sina2;
        pt[nPts].z = halfH - h2;
        nPts++;

        // another point on the upper circumference
        pt[nPts].x = rh2 * cosa1;
        pt[nPts].y = rh2 * sina1;
        pt[nPts].z = halfH - h2;
        nPts++;
      }
      addSurface(pt, nPts);
    }
  }
  // add the base surface
  nPts = 0;
  for (k = 0; k < n; k++)
  {
    // point on the circumference of base
    pt[nPts].x = R * cos(k * aDelta);
    pt[nPts].y = R * sin(k * aDelta);
    pt[nPts].z = halfH;
    nPts++;
  }
  addSurface(pt, nPts);
}
