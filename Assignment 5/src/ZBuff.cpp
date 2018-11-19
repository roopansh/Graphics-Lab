#include<windows.h>
#include "DrawUtil3D.h"
#include "Global3D.h"
#include "SrfcUtil.h"

typedef struct
{
  double **z;
  COLOR **clr;
  int nRows;
  int nCols;
} ZBUFF;

ZBUFF zbuff;

void init_zBuff()
{
  int x, y;
  
  zbuff.nRows = (int)(shape.max.x - shape.min.x + 1);
  zbuff.nCols = (int)(shape.max.y - shape.min.y + 1);
  zbuff.z = new double *[zbuff.nRows];
  zbuff.clr = new COLOR *[zbuff.nRows];
  for (x=0; x < zbuff.nRows; x++)
  {
    zbuff.z[x] = new double[zbuff.nCols]; 

    zbuff.clr[x] = new COLOR[zbuff.nCols];
  }
  for (y = 0; y < zbuff.nCols; y++)
  {
    for(x = 0; x < zbuff.nRows; x++)
    {
      zbuff.z[x][y]=99999999;
      zbuff.clr[x][y].r 
        = zbuff.clr[x][y].g 
        = zbuff.clr[x][y].b = 255;
    }
  }
}

void calculateZvaluesOfSurface(int i)
{
  int j;
  POINT2D pt;
  double z, zStartEye, xStart, yStart;
  double xDiff, yDiff;
  int x, y;
  bool isPtOnEdge, isPtWithinPolygon;

  for (j=0; j < shape.surface[i].nPoints; j++)
  {
    if (shape.surface[i].proj2D[j].y == shape.surface[i].min.y)
      break;
  }

  zStartEye = shape.surface[i].eye3D[j].z;
  yStart = shape.surface[i].proj2D[j].y;
  xStart = shape.surface[i].proj2D[j].x;

  for (pt.y = shape.surface[i].min.y; 
       pt.y <= shape.surface[i].max.y; pt.y++)
  {
    for (pt.x = shape.surface[i].min.x; 
         pt.x <= shape.surface[i].max.x; pt.x++)
    {
      isPtOnEdge = 
        isPtOnPolygonEdge(pt, shape.surface[i].proj2D, 
                        shape.surface[i].nPoints);
      isPtWithinPolygon = 
        isPointWithinPolygon(pt, shape.surface[i].proj2D, 
                             shape.surface[i].nPoints);
      if ((isPtWithinPolygon==false) && (isPtOnEdge==false))
        continue;
 

      yDiff = (pt.y - yStart);
      xDiff = (pt.x - xStart);

      if (shape.surface[i].C == 0)
          return;

      z = zStartEye 
          - xDiff * (shape.surface[i].A / shape.surface[i].C)
          - yDiff * (shape.surface[i].B / shape.surface[i].C);

      x = (int)(pt.x-shape.min.x);
      y = (int)(pt.y-shape.min.y);
      if (z < zbuff.z[x][y])
      {
        zbuff.z[x][y] = z;
        if (isPtOnEdge)
        {
          zbuff.clr[x][y].r = clrEdge.r;
          zbuff.clr[x][y].g = clrEdge.g;
          zbuff.clr[x][y].b = clrEdge.b;
        }
        else
        {
          zbuff.clr[x][y].r = shape.surface[i].clr.r;
          zbuff.clr[x][y].g = shape.surface[i].clr.g;
          zbuff.clr[x][y].b = shape.surface[i].clr.b;
        }
      } 
    } 
  }
}

void calculateZvaluesOfSurfaces()
{
  for (int i = 0; i < shape.nSurfaces; i++)
    calculateZvaluesOfSurface(i);
}

void drawFigureWithZBuff()
{  
  int y, x;
  for (y = 0; y < zbuff.nCols; y++)
  {
    for(x = 0; x < zbuff.nRows; x++)
    {
      drawPixelAt(shape.min.x+x, 
                  shape.min.y+y, zbuff.clr[x][y]);          
    }
  }
}

void genImage(HWND hwnd)
{
  init_zBuff();
  calculateZvaluesOfSurfaces();
  drawFigureWithZBuff();
  InvalidateRect(hwnd, NULL, 1);
}
