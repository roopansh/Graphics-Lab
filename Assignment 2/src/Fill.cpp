#include <windows.h>
#include <math.h>
#include "Fill.h"
#include <algorithm>
using namespace std;

void fill();

void setupMenus(HWND hwnd)
{
  HMENU hmenu;
  hmenu=GetMenu(hwnd);

  switch (gDrawData.drawMode)
  {
    case READY_MODE :
      // enable 'Draw Polygon', disable 'Filling' menu
      EnableMenuItem(hmenu,ID_DRAW_CIRCLE,
                     MF_BYCOMMAND|MF_ENABLED);
      EnableMenuItem(hmenu,ID_FILL,
                     MF_BYCOMMAND|MF_GRAYED);
      break;
    case DRAW_MODE :
    case FILL_MODE :
    case FILLED_MODE :
      // disable 'Draw Polygon', 'Filling' menu 
      EnableMenuItem(hmenu,ID_DRAW_CIRCLE,
                     MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hmenu,ID_FILL,
                     MF_BYCOMMAND|MF_GRAYED);
      break;
    case DRAWN_MODE :
      // enable 'Filling' menus, disable 'Draw Polygon' menu
      EnableMenuItem(hmenu,ID_DRAW_CIRCLE,
                     MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hmenu,ID_FILL,
                     MF_BYCOMMAND|MF_ENABLED);
      break;
  }
}

void performFilling(HWND hwnd)
{
  setDrawMode(FILL_MODE, hwnd);
  fill();
  reDraw(hwnd);
  setDrawMode(FILLED_MODE, hwnd);
}

void processCommand(int cmd, HWND hwnd)
{
  switch(cmd)
  {
    case ID_FILL:
      performFilling(hwnd);
      break;
    default:
      processCommonCommand(cmd, hwnd);
      break;
  }
}

float distance(int x, int y, int a, int b){
	return sqrt((y-b)*(y-b) + (x-a)*(x-a));
}

void fill()
{
	bool insideCircle[3];
	// initialise the gDrawData current inside/outside
	for (int circle = 0; circle < 3; circle++){
		if ( distance(0, 0, gDrawData.X[circle], gDrawData.Y[circle]) < gDrawData.radii[circle] ){	// inside
			insideCircle[circle] = true;
		} else {	// outside
			insideCircle[circle] = false;
		}
	}
	int X_MIN = min(gDrawData.X[0] - gDrawData.radii[0], min(gDrawData.X[1] - gDrawData.radii[1], gDrawData.X[2] - gDrawData.radii[2]));
	int X_MAX = max(gDrawData.X[0] + gDrawData.radii[0], max(gDrawData.X[1] + gDrawData.radii[1], gDrawData.X[2] + gDrawData.radii[2]));
	int Y_MIN = min(gDrawData.Y[0] - gDrawData.radii[0], min(gDrawData.Y[1] - gDrawData.radii[1], gDrawData.Y[2] - gDrawData.radii[2]));
	int Y_MAX = max(gDrawData.Y[0] + gDrawData.radii[0], max(gDrawData.Y[1] + gDrawData.radii[1], gDrawData.Y[2] + gDrawData.radii[2]));
	
	for(int y = Y_MIN; y <= Y_MAX; y++){	
		for (int x = X_MIN; x <= X_MAX; x++){
			COLORREF color = GetPixel(gDrawData.hdcMem, x, y);
			// check if it is boundary
			// update the gDrawData
			if (distance(x, y, gDrawData.X[0], gDrawData.Y[0]) < gDrawData.radii[0])		insideCircle[0] = true;
			else insideCircle[0] = false;
			if (distance(x, y, gDrawData.X[1], gDrawData.Y[1]) < gDrawData.radii[1])		insideCircle[1] = true;
			else insideCircle[1] = false;
			if (distance(x, y, gDrawData.X[2], gDrawData.Y[2]) < gDrawData.radii[2])		insideCircle[2] = true;
			else insideCircle[2] = false;

			if(color != RGB(0,0,0)){
				if (insideCircle[0] && insideCircle[1] && insideCircle[2])	SetPixel(gDrawData.hdcMem, x, y, RGB(255, 0, 0));
				else if (insideCircle[0] && insideCircle[1] )	SetPixel(gDrawData.hdcMem, x, y, RGB(0, 0, 255));
				else if (insideCircle[1] && insideCircle[2] )	SetPixel(gDrawData.hdcMem, x, y, RGB(0, 255, 0));
				else if (insideCircle[0] && insideCircle[2] )	SetPixel(gDrawData.hdcMem, x, y, RGB(255, 255, 0));
				else if (insideCircle[0] || insideCircle[1] || insideCircle[2])	SetPixel(gDrawData.hdcMem, x, y, RGB(0, 255, 255));								
			}
		}	
	}
}
