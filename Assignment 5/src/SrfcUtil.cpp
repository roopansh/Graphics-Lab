#include <windows.h>
#include "DrawUtil3D.h"
#include "Line.h"
#include "Global3D.h"

void drawPixelAt(double a, double b, COLOR clr)
{
  SetPixel(gDrawData.hdcMem, 
           (int) (a) + gDrawData.origin.x, 
           (int) (b) + gDrawData.origin.y, 
           RGB(clr.r, clr.g, clr.b));
}

bool isPointWithinPolygon(const POINT2D &pt, POINT2D vert[],
                          int count)
{
  int i, j;
  double x, m;
  bool odd_intersecs;

  if (count < 3)
    return false;
    
  odd_intersecs = false;

  for (i = 0; i < count; i++) 
  {
    j = (i + 1) % count;
    if ( ((vert[i].y < pt.y) && (vert[j].y >= pt.y)) ||
         ((vert[j].y < pt.y) && (vert[i].y >= pt.y)) 
       )
    {
      if ( vert[i].y != vert[j].y)
      {
        m = (vert[j].y - vert[i].y)/(vert[j].x - vert[i].x);
        x = vert[i].x + (pt.y - vert[i].y) / m;
        if (x <= pt.x ) 
          odd_intersecs = !odd_intersecs; 
      }
    }
  }
  return odd_intersecs;
}

bool isPtOnLineSegment(const POINT2D& pt, 
                          const POINT2D& start, 
                          const POINT2D& end)
{
  POINT ptNext;
  POINT startPt, endPt, ptToCheck;

  ptToCheck.x = (int) (pt.x + gDrawData.origin.x);
  ptToCheck.y = (int) (pt.y + gDrawData.origin.y);
  startPt.x = (int) (start.x + gDrawData.origin.x); 
  startPt.y = (int) (start.y + gDrawData.origin.y);
  endPt.x = (int) (end.x + gDrawData.origin.x);
  endPt.y = (int) (end.y + gDrawData.origin.y);

  setupLineSegmentDrawing(NULL, 
    startPt, endPt); // ignore device context
  while (findNextPtInLineSegment(endPt, ptNext))
  {
    if ((ptNext.x == ptToCheck.x) && 
        (ptNext.y == ptToCheck.y))
      return true;
  }
  return false;
}

bool isPtOnPolygonEdge(const POINT2D &pt, POINT2D vert[],
                       int count)
{
  int j = count-1;
  for (int i = 0; i < count; i++)
  {
    if (isPtOnLineSegment(pt, vert[j], vert[i]))
      return true;
    j = i;
  }
  return false;
}
