#include <windows.h>
#include <math.h>
#include "DrawUtil3D.h"
#include "Global3D.h"

void setCylinderData(double cylBaseRadius, double cylHeight)
{
  double h, h2, R, H, halfH;
  POINT3D pt[100];
  int nPts, i, k, m, n;
  double a1, a2;
  double cosa1, cosa2, sina1, sina2;
  double aDelta;
  double hDelta;

  H = cylHeight;
  halfH = H / 2.0;
  R = cylBaseRadius;
  n = 30; // no of sampled points on circumference
  hDelta = 20;
  m = (int)(H / hDelta); // number of equi-spaced circles
  if (((int)H) > (int)(m*hDelta)) 
    m = m + 1;// adjustment for last leap
  aDelta = 2.0 * PI / n;

  for (i = 0; i < m; i++)
  {
    h = i * hDelta; // height from base
    h2 = h + hDelta;
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
      pt[nPts].x = R * cosa1; 
      pt[nPts].y = R * sina1;       pt[nPts].z = halfH - h;
      nPts++;

      // another point on the lower circumference
      pt[nPts].x = R * cosa2; 
      pt[nPts].y = R * sina2;
      pt[nPts].z = halfH - h;
      nPts++;

      // sampled point on the upper circumference         
      pt[nPts].x = R * cosa2;
      pt[nPts].y = R * sina2;
      pt[nPts].z = halfH - h2;
      nPts++;

      // another point on the upper circumference
      pt[nPts].x = R * cosa1;
      pt[nPts].y = R * sina1;
      pt[nPts].z = halfH - h2;
      nPts++;

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

  // add the top surface
  nPts = 0;
  for (k = 0; k < n; k++)
  {
    // point on the circumference of base
    pt[nPts].x = R * cos(k * aDelta);
    pt[nPts].y = R * sin(k * aDelta);
    pt[nPts].z = -halfH;
    nPts++;
  }
  addSurface(pt, nPts);
}
