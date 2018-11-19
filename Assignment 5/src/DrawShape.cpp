#include <windows.h>
#include <math.h>
#include "DrawUtil3D.h"
#include "Global3D.h"
#include <sstream>
#include <vector>
#include "InputBox.h"
using namespace std;
int clickCount = 0;
LRESULT CALLBACK WindowF(HWND hwnd,UINT message,WPARAM wParam,
                         LPARAM lParam) 
{
  HDC hdc;
  PAINTSTRUCT ps;

  switch (message)
  {
  case WM_CREATE:
  {   
	  CInputBox ibox(hwnd);
	  if (ibox.DoModal("Caption", "Enter Points in this format ' x y z side x1 y1 z1 side1 "))
		  MessageBox(NULL, ibox.Text, "Title", MB_OK);
	  
	  hdc = GetDC(hwnd);
      initialize(hwnd, hdc);
      initialize_shapeData();
      setShapeData(ibox.Text);
      drawWireframe(clrEdge);
      ReleaseDC(hwnd, hdc);

	  break;
  }
  case WM_PAINT:

	  hdc = BeginPaint(hwnd, &ps);
	  drawImage(hdc);
	  EndPaint(hwnd, &ps);
	  break;

  case WM_LBUTTONDOWN:
  {
	  genImage(hwnd);
	  break;
  }
    case WM_DESTROY:
      cleanup();
      PostQuitMessage(0);
      break;

    default:
      break;
  } 
  return DefWindowProc(hwnd, message, wParam, lParam);
}
