#include <windows.h>
#include <math.h>
#include "DrawUtil3D.h"
#include "Draw3D.h"
#include "Transform3D.h"
#include "Global3D.h"

LRESULT CALLBACK WindowF(HWND hwnd,UINT message,WPARAM wParam,
                         LPARAM lParam) 
{
  HDC hdc;
  PAINTSTRUCT ps;

  switch(message)
  {
    case WM_CREATE:
      hdc = GetDC(hwnd);
      initialize_XformData(hwnd, hdc);
      ReleaseDC(hwnd, hdc);
      break;
    
    case WM_COMMAND:
      processCommand(LOWORD(wParam), hwnd);
      break;

    case WM_SIZE:
      calculateSize(hwnd);
      reset_XformData(hwnd);
      setDrawMode(READY_MODE, hwnd);
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
  return DefWindowProc(hwnd, message, wParam, lParam);;
}
