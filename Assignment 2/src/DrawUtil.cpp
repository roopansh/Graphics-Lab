#include <windows.h>
#include "Fill.h"

DRAWING_DATA gDrawData; // global data
LRESULT CALLBACK DlgAxis(HWND hdlg,UINT mess,WPARAM more,LPARAM pos);

void reDraw(HWND hwnd)
{
	InvalidateRect(hwnd, NULL, 1);
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

void drawImage(HDC hdc)
{
	BitBlt(hdc, 0, 0, gDrawData.maxBoundary.cx,
	       gDrawData.maxBoundary.cy, gDrawData.hdcMem, 0, 0, SRCCOPY);
}

void initialize(HWND hwnd, HDC hdc)
{
	gDrawData.maxBoundary.cx = GetSystemMetrics(SM_CXSCREEN);
	gDrawData.maxBoundary.cy = GetSystemMetrics(SM_CYSCREEN);

	gDrawData.radii[0] = -1;
	gDrawData.radii[1] = -1;
	gDrawData.radii[2] = -1;

	gDrawData.X[0] = -1;
	gDrawData.X[1] = -1;
	gDrawData.X[2] = -1;
	gDrawData.Y[0] = -1;
	gDrawData.Y[1] = -1;
	gDrawData.Y[2] = -1;

	gDrawData.circleCount = -1;

	createMemoryBitmap(hdc);
	setDrawMode(READY_MODE, hwnd);
}

void cleanup()
{
	DeleteDC(gDrawData.hdcMem);
}

void reset(HWND hwnd)
{
	gDrawData.drawMode = READY_MODE;

	PatBlt(gDrawData.hdcMem, 0, 0, gDrawData.maxBoundary.cx,
	       gDrawData.maxBoundary.cy, PATCOPY);

	reDraw(hwnd);
	setupMenus(hwnd);
}

void plot_sympoint(int ex, int ey, COLORREF clr)
{
  int cx = gDrawData.X[gDrawData.circleCount];
  int cy = gDrawData.Y[gDrawData.circleCount];

  SetPixel(gDrawData.hdcMem, ex+cx,cy-ey, clr);
  SetPixel(gDrawData.hdcMem, -ex+cx,cy-ey, clr);
  SetPixel(gDrawData.hdcMem, -ex+cx,cy+ey, clr);
  SetPixel(gDrawData.hdcMem, ex+cx,cy+ey, clr);
}


void drawCircleAux(int r)
{
	int ma = r;
	int mb = r;
	int x,y;
  	x=0;
  	y=mb;
	plot_sympoint(0,0, RGB(0,0,0));
	plot_sympoint(x,y, RGB(0,0,0));

  	double d=(mb*mb)-(ma*ma*mb)+(ma*ma/4);
  	while((ma*ma*(y-(1/2)))>(mb*mb*(x+1)))  
  	{
    	/* in region 1*/
    	if(d<0)	
	   	{
      		/*choose E*/
      		d=d+(mb*mb*(2*x+3));
      		x=x+1;
	    }
    	else                              	
    	{
	      	/*choose SE*/
	      	d=d+(mb*mb*(2*x+3))+(ma*ma*(2-2*y));
	      	x=x+1;
	      	y=y-1;
    	}
    plot_sympoint(x,y, RGB(0,0,0));
  }
  d=(mb*mb*(x+(1/2))*(x+1/2))
    +ma*ma*(y-1)*(y-1)-(ma*ma*mb*mb);
  while(y>0)   
  {
    /*in region 2*/
    if(d<0)
    {
      /*choose SE*/
      d=d+mb*mb*(2*x+2)+ma*ma*(3-2*y);
      x=x+1;
      y=y-1;
    }
    else
    {
      /*choose S*/
      d=d+ma*ma*(3-2*y);
      y=y-1;
    }
    plot_sympoint(x,y, RGB(0,0,0));
  }
}


void drawCircle(HWND hwnd,int x,int y) {
	int circleCount = ++gDrawData.circleCount;
	if(circleCount < 3 ){
		gDrawData.X[circleCount] = x;
		gDrawData.Y[circleCount] = y;
		drawCircleAux(gDrawData.radii[gDrawData.circleCount]);
	}
	if(circleCount >= 2){
		setDrawMode(DRAWN_MODE, hwnd);
	}
}

void processLeftButtonDown(HWND hwnd, int x, int y)
{
	switch (gDrawData.drawMode)
	{
		case DRAW_MODE:
			drawCircle(hwnd, x, y);
			reDraw(hwnd);
			break;
		default:
			break;
	}
}

void processCommonCommand(int cmd, HWND hwnd)
{
	int response;
	switch(cmd)
	{
		case ID_CLEAR:
		reset(hwnd);
		setDrawMode(READY_MODE, hwnd);
		gDrawData.circleCount = -1;
		break;

		case ID_DRAW_CIRCLE:
		setDrawMode(DRAW_MODE, hwnd);
		DialogBox(NULL,"MyDB",hwnd,(DLGPROC)DlgAxis);
		break;

		case ID_EXIT:
		response=MessageBox(hwnd,"Quit the program?", "EXIT",
		                    MB_YESNO);
		if(response==IDYES)
			PostQuitMessage(0);
		break;
		default:
		break;
	}
}

LRESULT CALLBACK DlgAxis(HWND hdlg,UINT mess,WPARAM more,LPARAM pos)
{
	char str[20];
	switch(mess)
	{
		case WM_COMMAND:
		switch(more)
		{
			case ID_OK:
			GetDlgItemText(hdlg,ID_EB1,str,20);
			gDrawData.radii[0]=(int)(atof(str));
			GetDlgItemText(hdlg,ID_EB2,str,20);
			gDrawData.radii[1]=(int)(atof(str));
			GetDlgItemText(hdlg,ID_EB3,str,20);
			gDrawData.radii[2]=(int)(atof(str));
			
			if(gDrawData.radii[0] < 10 ||
			   gDrawData.radii[0] > 100 ||
			   gDrawData.radii[1] < 10 ||
			   gDrawData.radii[1] > 100 ||
			   gDrawData.radii[2] < 10 ||
			   gDrawData.radii[2] > 100 )
			{
				MessageBox(hdlg,
				           "All the Radii should be between 10 and 100.",
				           "Warning!",MB_ICONERROR);
			}
			EndDialog(hdlg,TRUE);
			return 1;
			break;
			case ID_CANCEL:
			EndDialog(hdlg,TRUE);
			break;
		}
		break;
		default:
		break;
	}
	return 0;
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

		default:
		break;
	}
	// Call the default window handler
	return DefWindowProc(hwnd, message, wParam, lParam);
}

