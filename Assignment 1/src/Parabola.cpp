#include <math.h>
#include <windows.h>
#include "Parabola.h"

DRAWING_DATA gDrawData; // global data
void drawParabola();

void reDraw(HWND hwnd)
{
  InvalidateRect(hwnd, NULL, 1);
}

void drawPoint(int x, int y)
{
    Ellipse(gDrawData.hdcMem,x-2,y-2,x+2,y+2);
}

void setupMenus(HWND hwnd)
{
  HMENU hMenu = GetMenu(hwnd);

  switch (gDrawData.drawMode)
  {
    case READY_MODE :
      // enable parabola menu
      EnableMenuItem(hMenu, ID_PARABOLA, MF_BYCOMMAND|MF_ENABLED);
      break;
    case DRAW_MODE :
    case DRAWN_MODE :
      // disable Parabola menu
      EnableMenuItem(hMenu, ID_PARABOLA, MF_BYCOMMAND|MF_GRAYED);
      break;
  }
}

void setDrawMode(MODE mode, HWND hwnd)
{
  gDrawData.drawMode = mode;
  setupMenus(hwnd);
}

void createMemoryBitmap(HDC hdc)
{
  gDrawData.hdcMem = CreateCompatibleDC(hdc);
  gDrawData.hbmp = CreateCompatibleBitmap(hdc, 
    gDrawData.maxBoundary.cx, gDrawData.maxBoundary.cy);
  SelectObject(gDrawData.hdcMem, gDrawData.hbmp);
  PatBlt(gDrawData.hdcMem, 0, 0, gDrawData.maxBoundary.cx, 
         gDrawData.maxBoundary.cy, PATCOPY);
}

void initialize(HWND hwnd, HDC hdc)
{
  gDrawData.hDrawPen=CreatePen(PS_SOLID, 1, RGB(255, 255, 0));

  gDrawData.maxBoundary.cx = GetSystemMetrics(SM_CXSCREEN);
  gDrawData.maxBoundary.cy = GetSystemMetrics(SM_CYSCREEN);

  createMemoryBitmap(hdc);
  setDrawMode(READY_MODE, hwnd);
}

void cleanup()
{
  DeleteDC(gDrawData.hdcMem);
}

void reset(HWND hwnd)
{
  gDrawData.origin.x = gDrawData.origin.y = 0;

  gDrawData.drawMode = READY_MODE;

  PatBlt(gDrawData.hdcMem, 0, 0, gDrawData.maxBoundary.cx, 
         gDrawData.maxBoundary.cy, PATCOPY);

  reDraw(hwnd);
  setupMenus(hwnd);
}

void plot_sympoint(int ex, int ey, COLORREF clr)
{
  int cx = gDrawData.origin.x;
  int cy = gDrawData.origin.y;
	
  SetPixel(gDrawData.hdcMem, ex+cx,cy+ey, clr);
  SetPixel(gDrawData.hdcMem, ex+cx,cy-ey, clr);
}

void addPoint(HWND hwnd, int x, int y)
{
  switch (gDrawData.drawMode)
  {
    case DRAW_MODE:
      /* the coordinates of the centre is stored 
         and the Parabola is drawn */
      SelectObject(gDrawData.hdcMem, gDrawData.hDrawPen);
      drawPoint(x,y);
      gDrawData.origin.x = x;
      gDrawData.origin.y = y;
      drawParabola();
      setDrawMode(DRAWN_MODE, hwnd);
      reDraw(hwnd);
      break;
    case DRAWN_MODE:
      MessageBox(hwnd,
       "Parabola already drawn, now you can clear the area", 
          "Warning",MB_OK);
      break;
    default:
      break;
  }
}

void drawImage(HDC hdc)
{
  BitBlt(hdc, 0, 0, gDrawData.maxBoundary.cx, 
    gDrawData.maxBoundary.cy, gDrawData.hdcMem, 
    0, 0, SRCCOPY);
}

void processLeftButtonDown(HWND hwnd, int x, int y)
{
  addPoint(hwnd,x,y);
}

void processCommand(int cmd, HWND hwnd)
{
  int response;
  switch(cmd)
  {
    case ID_CLEAR:
      reset(hwnd);
      setDrawMode(READY_MODE, hwnd);
      break;
    case ID_PARABOLA:
      setDrawMode(DRAW_MODE, hwnd);
      break;
    case ID_EXIT:
        response=MessageBox(hwnd,
          "Quit the program?", "EXIT", 
          MB_YESNO);
        if(response==IDYES) 
            PostQuitMessage(0);
        break;
    default:
      break;
  }
}

void drawParabola()
{
	int x,y;
	x=0;
	y=0;
//  	plot_sympoint(x,y, RGB(0,0,0));

//	Starting from Region 1
	double d = -0.5;
	while(y < 0.5)  
  	{
	    plot_sympoint(x,y, RGB(0,0,0));
    	if(d<0)
	    {
	    	d = d + (2*y) + 2;
	    }
	    else                              
	    {
	    	d = d + (2*y) + 2;
	    	x = x + 1;
	    }
	    y = y + 1;
	}
  	
//  Starting from Region 2	
	d = -0.25;  
  	while(y <= 10)
  	{
  		plot_sympoint(x,y, RGB(0,0,0));
	    if(d<0)
	    {
	      	d = d + (2*y) + 1; 
	      	y = y + 1;
	    }
	    else
	    {
	      	d = d - 1;
	    }
	    x = x + 1;
  	}
}

LRESULT CALLBACK WindowF(HWND hwnd,UINT message,WPARAM wParam,
                         LPARAM lParam) 
{
  HDC hdc;
  PAINTSTRUCT ps;
  int x,y;

  switch(message)
  {
    case WM_CREATE:
      hdc = GetDC(hwnd);
      initialize(hwnd, hdc);
      ReleaseDC(hwnd, hdc);
      break;
    
    case WM_COMMAND:
      processCommand(LOWORD(wParam), hwnd);
      break;

    case WM_LBUTTONDOWN:
      x = LOWORD(lParam);
      y = HIWORD(lParam);
      processLeftButtonDown(hwnd, x,y);
      break;

    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      drawImage(hdc);
      EndPaint(hwnd, &ps);
      break;

    case WM_DESTROY:
      cleanup();
      PostQuitMessage(0);
      break;
  }
  // Call the default window handler
  return DefWindowProc(hwnd, message, wParam, lParam);
}
