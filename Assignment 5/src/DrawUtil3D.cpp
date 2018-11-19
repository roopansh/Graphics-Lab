#include <windows.h>
#include <math.h>
#include "DrawUtil3D.h"
#include "Global3D.h"
#include "Line.h"

void createMemoryBitmap(HDC hdc)
{
  gDrawData.hdcMem = CreateCompatibleDC(hdc); 
  gDrawData.hbmp = CreateCompatibleBitmap(hdc, 
       gDrawData.maxBoundary.cx, gDrawData.maxBoundary.cy);
  SelectObject(gDrawData.hdcMem, gDrawData.hbmp);
  PatBlt(gDrawData.hdcMem, 0, 0, gDrawData.maxBoundary.cx, 
           gDrawData.maxBoundary.cy, PATCOPY);
}

void initialize_shapeData()
{
  shape.nSurfaces = 0;
  shape.surface = 
    (SURFACE3D *) malloc(MAX_SURFACES * sizeof(SURFACE3D));
  for (int i=0; i < MAX_SURFACES; i++)
  {
    shape.surface[i].nPoints = 0;
    shape.surface[i].vertex 
      = (POINT3D *) malloc(MAX_SURFACE_EDGES * sizeof(POINT3D)); 
    shape.surface[i].eye3D 
      = (POINT3D *) malloc(MAX_SURFACE_EDGES * sizeof(POINT3D));
    shape.surface[i].proj2D 
      = (POINT2D *) malloc(MAX_SURFACE_EDGES * sizeof(POINT3D));
  }
}

void compute3DCoefficients(double r, double theta, double phi)
{
  double th, ph, costh, sinth, cosph, sinph, factor;

  factor = atan(1.0)/45.0;
  th = theta * factor; // convert to radian
  ph = phi * factor; // convert to radian

  costh = cos(th);
  sinth = sin(th);
  cosph = cos(ph);
  sinph = sin(ph);

  /* Elements of matrix V */
  V[0][0] = - sinth;
  V[0][1] = - cosph * costh;
  V[0][2] = - sinph * costh;
  
  V[1][0] = costh;
  V[1][1] = - cosph * sinth;
  V[1][2] = - sinph * sinth;

  V[2][0] = 0;
  V[2][1] = sinph;
  V[2][2] = - cosph;

  V[3][0] = V[3][1] = 0;
  V[3][2] = r;
}

void initialize(HWND hwnd, HDC hdc)
{
  gDrawData.windowSize.cx = WINDOW_WIDTH;  
  gDrawData.windowSize.cy = WINDOW_HEIGHT; 
  gDrawData.origin.x = gDrawData.windowSize.cx/2;
  gDrawData.origin.y = gDrawData.windowSize.cy/2;

  gDrawData.maxBoundary.cx = GetSystemMetrics(SM_CXSCREEN);
  gDrawData.maxBoundary.cy = GetSystemMetrics(SM_CYSCREEN);
  createMemoryBitmap(hdc);

  gDrawData.r = 400; // distance of object from eye 
  gDrawData.screen_dist = 200; // distance of screen from eye
  gDrawData.phi = 45;
  gDrawData.theta = 135;
  compute3DCoefficients(gDrawData.r, gDrawData.theta, gDrawData.phi);
}

void get3DEyeFrom3DWorld(const POINT3D & pt3DWorld, 
                         POINT3D& pt3DEye)
{
  // conversion of world co-ordinates to eye co-ordinates
  pt3DEye.x = V[0][0] * pt3DWorld.x + V[1][0] * pt3DWorld.y;
  pt3DEye.y = V[0][1] * pt3DWorld.x + V[1][1] * pt3DWorld.y 
              + V[2][1]* pt3DWorld.z;
  pt3DEye.z = V[0][2] * pt3DWorld.x + V[1][2] * pt3DWorld.y + 
            V[2][2]* pt3DWorld.z + V[3][2];
}

void get2DPerspectiveFrom3DWorld(const POINT3D & pt3DWorld,
                                  double &x, double &y)
{
  POINT3D eyeCoords;

  // conversion of world co-ordinates to eye coordinates computation
  get3DEyeFrom3DWorld(pt3DWorld, eyeCoords);

  // perspective screen co-ordinates computation

  x = (long)(gDrawData.screen_dist * eyeCoords.x / eyeCoords.z );
  y = (long)(gDrawData.screen_dist * eyeCoords.y / eyeCoords.z );
}

void crossProduct(const POINT3D pt[], int nPts, double& A,
                  double& B,double& C)
{
  /* find plane equation using crossproduct of 2 non-collinear vectors*/
  // we presume, pt[] has at least three points

  A = B = C = 0;
  int i, j;

  if (nPts < 3)
    return;

  for (j=0; j < 3; j++)
  {
    i = (j + 1) % 3;
    A = A + (pt[i].y - pt[j].y)*(pt[i].z + pt[j].z);
    B = B + (pt[i].z - pt[j].z)*(pt[i].x + pt[j].x);
    C = C + (pt[i].x - pt[j].x)*(pt[i].y + pt[j].y);
  }
}

void drawImage(HDC hdc)
{
  BitBlt(hdc, 0, 0, gDrawData.maxBoundary.cx, 
       gDrawData.maxBoundary.cy, gDrawData.hdcMem, 
       0, 0, SRCCOPY);
}

void cleanup()
{
  DeleteDC(gDrawData.hdcMem);
}

void move3D(POINT3D ptWorld)
{
  double x, y;
  get2DPerspectiveFrom3DWorld(ptWorld, x, y);
  MoveToEx(gDrawData.hdcMem,(int)x+gDrawData.origin.x,
           (int)y+gDrawData.origin.y,0);
}

void line3D(POINT3D ptWorld)
{
  double x, y;

  get2DPerspectiveFrom3DWorld(ptWorld, x, y);
  LineTo(gDrawData.hdcMem,(int)x+gDrawData.origin.x,
         (int)y+gDrawData.origin.y);
}

void drawEdge(const POINT2D& start, 
              const POINT2D& end,
              const COLOR &clr)
{
  COLORREF clrLine;
  POINT startPt, endPt;

  startPt.x = (int) start.x + gDrawData.origin.x;
  startPt.y = (int) start.y + gDrawData.origin.y;
  endPt.x = (int) end.x + gDrawData.origin.x;
  endPt.y = (int) end.y + gDrawData.origin.y;
  clrLine = RGB(clr.r, clr.g, clr.b);

  drawLineSegment(gDrawData.hdcMem, startPt, endPt, clrLine);
}

void drawWireframe(COLOR clr)
{
  int i, j, k;
  // prepare stage for drawing by computing the screen projections
  prepareShapeDataForDrawing(); 

  for (i = 0; i < shape.nSurfaces; i++)
  {
    // draw edge
    k = shape.surface[i].nPoints - 1;
    for (j = 0; j < shape.surface[i].nPoints; j++)
    {
      drawEdge(shape.surface[i].proj2D[k], 
               shape.surface[i].proj2D[j], clr);
      k = j;
    }
  }
}

void addSurfacePts(SURFACE3D& dest, 
                   const POINT3D& a
                  ) 
{
  if (dest.nPoints == MAX_SURFACE_EDGES)
    return;

  dest.vertex[dest.nPoints].x = a.x;
  dest.vertex[dest.nPoints].y = a.y;
  dest.vertex[dest.nPoints].z = a.z;  
  dest.nPoints++;
}

void addSurface(const POINT3D& a, 
                const POINT3D& b, 
                const POINT3D& c,
                const POINT3D& d)
{
  if (shape.nSurfaces == MAX_SURFACES)
    return;

  addSurfacePts(shape.surface[shape.nSurfaces], a);
  addSurfacePts(shape.surface[shape.nSurfaces], b);
  addSurfacePts(shape.surface[shape.nSurfaces], c);
  addSurfacePts(shape.surface[shape.nSurfaces], d);
  shape.nSurfaces++; 
}

void addSurface(POINT3D pts[], 
                const int &noOfPts 
              )
{
  if (shape.nSurfaces == MAX_SURFACES)
    return;

  for (int i = 0; i < noOfPts; i++)
    addSurfacePts(shape.surface[shape.nSurfaces], pts[i]);
  shape.nSurfaces++;
}

void resetDataForShape()
{
  int i;
  for (i = 0; i < shape.nSurfaces; i++)
  {
    shape.surface[i].nPoints = 0;
  }
  shape.nSurfaces = 0;
}

void prepareShapeDataForDrawing()
{
  int i, j;
  double x, y;

  get2DPerspectiveFrom3DWorld(shape.surface[0].vertex[0], x, y);
  shape.surface[0].proj2D[0].x = x;
  shape.surface[0].proj2D[0].y = y;

  shape.min.x = shape.max.x = x;
  shape.min.y = shape.max.y = y;

  for (i = 0; i < shape.nSurfaces; i++)
  {
    get3DEyeFrom3DWorld(shape.surface[i].vertex[0],
                        shape.surface[i].eye3D[0]);
    get2DPerspectiveFrom3DWorld(shape.surface[i].vertex[0], 
                                x, y);
    shape.surface[i].proj2D[0].x = x;
    shape.surface[i].proj2D[0].y = y;

    shape.surface[i].min.x  =
      shape.surface[i].max.x = x;
    shape.surface[i].min.y  =
      shape.surface[i].max.y = y;

    for (j = 1; j < shape.surface[i].nPoints; j++)
    {
      get3DEyeFrom3DWorld(shape.surface[i].vertex[j],
                          shape.surface[i].eye3D[j]);
      get2DPerspectiveFrom3DWorld(shape.surface[i].vertex[j],
                                  x, y);
      shape.surface[i].proj2D[j].x = x;
      shape.surface[i].proj2D[j].y = y;

      if (x < shape.surface[i].min.x)
        shape.surface[i].min.x = x;
      if (y < shape.surface[i].min.y)
        shape.surface[i].min.y = y;
      if (x > shape.surface[i].max.x)
        shape.surface[i].max.x = x;
      if (y > shape.surface[i].max.y)
        shape.surface[i].max.y = y;
    }
    if (shape.min.x > shape.surface[i].min.x)
      shape.min.x = shape.surface[i].min.x;
    if (shape.min.y > shape.surface[i].min.y)
      shape.min.y = shape.surface[i].min.y;
    if (shape.max.x < shape.surface[i].max.x)
      shape.max.x = shape.surface[i].max.x;
    if (shape.max.y < shape.surface[i].max.y)
      shape.max.y = shape.surface[i].max.y;

    crossProduct(shape.surface[i].eye3D,
                 shape.surface[i].nPoints,
                 shape.surface[i].A,
                 shape.surface[i].B,
                 shape.surface[i].C);
    shape.surface[i].D =  
      - shape.surface[i].A * shape.surface[i].eye3D[0].x
      - shape.surface[i].B * shape.surface[i].eye3D[0].y
      - shape.surface[i].C * shape.surface[i].eye3D[0].z;

    shape.surface[i].clr.r
      = shape.surface[i].clr.g
      = shape.surface[i].clr.b
      = 255;
  }
}
