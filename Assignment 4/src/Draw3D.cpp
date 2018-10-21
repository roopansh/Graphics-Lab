#include <math.h>
#include <stdio.h>
#include <windows.h>
#include "DrawUtil3D.h"
#include "Draw3D.h"
#include "Global3D.h"
#include "Transform3D.h"
#include "TransformGlobal.h"

void setupMenus(HWND hwnd)
{
  HMENU hMenu;

  hMenu=GetMenu(hwnd);

  switch (gShpData.drawMode)
  {
    case READY_MODE :
      // enable all 'draw' menus, disable 'transform' menus
      EnableMenuItem(hMenu,ID_CUBE,MF_BYCOMMAND|MF_ENABLED);
      EnableMenuItem(hMenu,ID_CONE,MF_BYCOMMAND|MF_ENABLED);
      EnableMenuItem(hMenu,ID_CYLINDER,MF_BYCOMMAND|MF_ENABLED);
      EnableMenuItem(hMenu,ID_SPHERE,MF_BYCOMMAND|MF_ENABLED);
      EnableMenuItem(hMenu,ID_TRANSLATE,MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hMenu,ID_ROTATE,MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hMenu,ID_SCALE,MF_BYCOMMAND|MF_GRAYED);
      break;
    case DRAW_MODE :
    case DRAWN_MODE :
      // disable relevant 'draw' menus
      if (gShpData.shape != CUBE)
        EnableMenuItem(hMenu,ID_CUBE,MF_BYCOMMAND|MF_GRAYED);
      if (gShpData.shape != CONE)
        EnableMenuItem(hMenu,ID_CONE,MF_BYCOMMAND|MF_GRAYED);
      if (gShpData.shape != CYLINDER)
        EnableMenuItem(hMenu,ID_CYLINDER,MF_BYCOMMAND|MF_GRAYED);
      if (gShpData.shape != SPHERE)
        EnableMenuItem(hMenu,ID_SPHERE,MF_BYCOMMAND|MF_GRAYED);
      if (gShpData.drawMode == DRAWN_MODE)
      {
        // enable 'transform' menus + 'fill' menu
        EnableMenuItem(hMenu,ID_TRANSLATE,MF_BYCOMMAND|MF_ENABLED);
        EnableMenuItem(hMenu,ID_ROTATE,MF_BYCOMMAND|MF_ENABLED);
        EnableMenuItem(hMenu,ID_SCALE,MF_BYCOMMAND|MF_ENABLED);
      }
      break;

    case TRANSLATE_MODE :
    case ROTATE_MODE :
    case SCALE_MODE :
    case TRANSFORMED_MODE :
      // disable all 'draw' menus as well as all 'transform' menus
      EnableMenuItem(hMenu,ID_CUBE,MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hMenu,ID_CONE,MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hMenu,ID_CYLINDER,MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hMenu,ID_SPHERE,MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hMenu,ID_TRANSLATE,MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hMenu,ID_ROTATE,MF_BYCOMMAND|MF_GRAYED);
      EnableMenuItem(hMenu,ID_SCALE,MF_BYCOMMAND|MF_GRAYED);
      break;
  }
}

void reset_XformData(HWND hwnd)
{
  int i;
  RECT rect;

  PatBlt(gDrawData.hdcMem, 0, 0, gDrawData.maxBoundary.cx, 
         gDrawData.maxBoundary.cy, PATCOPY);

  gDrawData.theta = gDrawData.phi = 0;

  // default values

  if (gShpData.pShapeData == NULL)
  {
    free(gShpData.pShapeData);
    gShpData.pShapeData = NULL;
  }
  gDrawData.r = 220; // distance of object from eye 
  gDrawData.screen_dist = 100; // distance of screen from eye
  gDrawData.phi = 45;
  gDrawData.theta = 135;
  compute3DCoefficients(gDrawData.r, gDrawData.theta, gDrawData.phi);

  if (gShpData.hwndStatus == NULL)
  {
    gShpData.hwndStatus = 
     CreateDialog(NULL, "SHAPESB",hwnd,(DLGPROC)StatusDlgShape);
    SetParent(gShpData.hwndStatus, hwnd);
    GetWindowRect(gShpData.hwndStatus, &rect);
    gShpData.statusHt = rect.bottom - rect.top;
    SetWindowPos(gShpData.hwndStatus, HWND_TOP, 0, 0, 
      gDrawData.windowSize.cx, gShpData.statusHt, 
      0);

    gDrawData.origin.x = rect.right >> 1; // half
    gDrawData.origin.y = gShpData.statusHt + 
      (gDrawData.windowSize.cy - gShpData.statusHt) /2;
  }

  for (i=0; i < shape.nSurfaces; i++)
  {
    shape.surface[i].nPoints = 0;
  }
  shape.nSurfaces = 0;

  setDrawMode(READY_MODE, hwnd);
  ShowWindow(gShpData.hwndStatus, FALSE);
  reDraw(hwnd);
  setupMenus(hwnd);
}

void initialize_XformData(HWND hwnd, HDC hdc)
{
  initialize(hwnd, hdc);
  gShpData.hBGPen = CreatePen(PS_SOLID, 1, 
                       RGB(clrBG.r, clrBG.g, clrBG.b));
  gShpData.hDrawPen = CreatePen(PS_SOLID, 1, 
          RGB(clrEdge.r, clrEdge.g, clrEdge.b));
  gShpData.hTransformPen = CreatePen(PS_SOLID, 1, 
          RGB(clrTransform.r, clrTransform.g, clrTransform.b));

  gShpData.shape = NONE;
  gShpData.pShapeData = NULL;
  gShpData.hwndStatus = NULL;

  calculateSize(hwnd);

  reset_XformData(hwnd);
  
  initialize_shapeData();
}

void processCommand(int cmd, HWND hwnd)
{
  int response; 
  switch(cmd)
  {
    case ID_CLEAR:
      reset_XformData(hwnd);
      setDrawMode(READY_MODE, hwnd);
      break;
    case ID_CUBE:
      setupAndDrawShape(hwnd, CUBE);
      break;
    case ID_CONE:
      setupAndDrawShape(hwnd, CONE);      
      break;
    case ID_CYLINDER:
      setupAndDrawShape(hwnd, CYLINDER);      
      break;
    case ID_SPHERE:
      setupAndDrawShape(hwnd, SPHERE);
      break;
    case ID_TRANSLATE:
    case ID_SCALE:
      if (cmd == ID_SCALE)
        setDrawMode(SCALE_MODE, hwnd);
      else
        setDrawMode(TRANSLATE_MODE, hwnd);
      if (DialogBox(NULL, "TRNSLT_SCALEDB", hwnd, 
              (DLGPROC)DlgTrnsltScale)==TRUE)
      {
        eraseShape();
        drawShape();
        showStatusOfDrawing();
        setDrawMode(TRANSFORMED_MODE, hwnd);
      }
      else
      {
        setDrawMode(DRAWN_MODE, hwnd);
      }
      reDraw(hwnd);
      break;
    case ID_ROTATE:
      setDrawMode(ROTATE_MODE, hwnd);
      if (DialogBox(NULL,"ROTATEDB", hwnd, 
                (DLGPROC)DlgRotate) == TRUE)
      {
        eraseShape();
        drawShape();
        showStatusOfDrawing();
        setDrawMode(TRANSFORMED_MODE, hwnd);
      }
      else
      {
        setDrawMode(DRAWN_MODE, hwnd);
      }
      reDraw(hwnd);
      break;
    case ID_EXIT:
      response=MessageBox(HWND_DESKTOP,"Quit the program?", 
                   "EXIT", MB_YESNO  );
      if(response==IDYES) 
        PostQuitMessage(0);
      break;
    default:
      break;
  }
}

void showStatusOfDrawing()
{
  char szText[100];

  switch(gShpData.drawMode)
  {
    case READY_MODE:
    case DRAW_MODE:
      SetDlgItemText(gShpData.hwndStatus,ID_VARLBL1,"");
      break;
    case DRAWN_MODE:
      break;
    case TRANSLATE_MODE:
      sprintf(szText,"%s Translated by: X = %d, Y = %d, Z = %d",
             szShapeType[gShpData.shape],
             gShpData.tx, gShpData.ty, gShpData.tz);
      SetDlgItemText(gShpData.hwndStatus,ID_VARLBL0, szText);
      break;
    case ROTATE_MODE:
      sprintf(szText, 
        "%s Rotated counter-clockwise wrt body diagonal from (0,0,0), by %5.2f degrees",
        szShapeType[gShpData.shape],
        gShpData.angOfRotation);
      SetDlgItemText(gShpData.hwndStatus,ID_VARLBL0, szText);
      break;
    case SCALE_MODE:
      sprintf(szText, 
        "%s Scaled by: X = %5.2f, Y = %5.2f, Z = %5.2f",
        szShapeType[gShpData.shape],
        gShpData.sx, gShpData.sy, gShpData.sz);
      SetDlgItemText(gShpData.hwndStatus,ID_VARLBL0, szText);
      break;

    default:
      break;
  }
}

LRESULT CALLBACK StatusDlgShape(HWND hdlg,UINT mess,
                                WPARAM more,LPARAM pos)
{
  return 0;
}

LRESULT CALLBACK DlgTrnsltScale(HWND hdlg,UINT mess,
                                WPARAM more,LPARAM pos)
{
  char str[20];
  switch(mess)
  {
    case WM_INITDIALOG:
      if (gShpData.drawMode == TRANSLATE_MODE)
      {
        SetWindowText(hdlg, "Input: Data for translation");
        SetDlgItemInt(hdlg,ID_VARFLD0, 0, 1);
        SetDlgItemInt(hdlg,ID_VARFLD1, 0, 1);
        SetDlgItemInt(hdlg,ID_VARFLD2, 0, 1);
      }
      else
      {
        SetWindowText(hdlg, "Input: Data for scaling");
        SetDlgItemInt(hdlg,ID_VARFLD0, 1, 1);
        SetDlgItemInt(hdlg,ID_VARFLD1, 1, 1);
        SetDlgItemInt(hdlg,ID_VARFLD2, 1, 1);
      }
      break;
    case WM_COMMAND:
      switch(more)
      {  
        case ID_DRAW:
          GetDlgItemText(hdlg,ID_VARFLD0,str,20);
          gShpData.sx = atof(str); /*convert the text into number*/
          GetDlgItemText(hdlg,ID_VARFLD1,str,20);
          gShpData.sy = atof(str); /*convert the text into number*/
          GetDlgItemText(hdlg,ID_VARFLD2,str,20);
          gShpData.sz = atof(str); /*convert the text into number*/

          if((gShpData.drawMode == SCALE_MODE) &&
             ((gShpData.sx<=0)||(gShpData.sy<=0)||(gShpData.sz<=0)))
          {
            MessageBox(hdlg, "YOU MUST GIVE A NON-ZERO NUMBER", 
            "REMEMBER",MB_ICONERROR);
            break;
          }
          if (gShpData.drawMode == TRANSLATE_MODE)
          {
            gShpData.tx = (int) gShpData.sx;
            gShpData.ty = (int) gShpData.sy;
            gShpData.tz = (int) gShpData.sz;
          }
          EndDialog(hdlg,TRUE);
          break;
        case ID_CANCEL:
          if (gShpData.drawMode == TRANSLATE_MODE)
          {
            gShpData.tx = gShpData.ty = gShpData.tz = 0;
          }
          else
          {
            gShpData.tx = gShpData.ty = gShpData.tz = 1;
          }
          EndDialog(hdlg,FALSE);
          break;
      }
      break;
  }
  return 0;
}

LRESULT CALLBACK DlgRotate(HWND hdlg,UINT mess,
                           WPARAM more,LPARAM pos)
{
  char szText[10];

  switch(mess)
  {
    case WM_INITDIALOG:
      SendDlgItemMessage(hdlg, ID_VARFLD0, CB_ADDSTRING, 
                         0, (LPARAM) ("X"));
      SendDlgItemMessage(hdlg, ID_VARFLD0, CB_ADDSTRING, 
                         0, (LPARAM) ("Y"));
      SendDlgItemMessage(hdlg, ID_VARFLD0, CB_ADDSTRING, 
                         0, (LPARAM) ("Z"));
      SetDlgItemInt(hdlg,ID_VARFLD1, 0, 1);
      break;
    case WM_COMMAND:
      switch(more)
      { 
        case ID_DRAW:
          GetDlgItemText(hdlg, ID_VARFLD0, szText, sizeof(szText));
          if (strcmp(szText, "X")==0)
            gShpData.rotType = ROTATE_X;
          else if (strcmp(szText, "Y")==0)
            gShpData.rotType = ROTATE_Y;
          else 
            gShpData.rotType = ROTATE_Z;

          GetDlgItemText(hdlg,ID_VARFLD1,szText,10);
          gShpData.angOfRotation = atof(szText); 

          if(gShpData.angOfRotation<=0)
          {
            MessageBox(hdlg,
                "The rotation angle value should be greater than zero",
                "REMEMBER",MB_ICONERROR);
            break;
          }
          else
            EndDialog(hdlg,TRUE);           
          break;
        case ID_CANCEL:
          EndDialog(hdlg,FALSE);
          break;
      }
      break;
  }
  return 0;
}
         
void drawShape(bool drawOrErase)
{
  // drawOrErase == true -> fresh draw, false => erase
  COLOR clr;
  if (drawOrErase)
  {
    switch(gShpData.drawMode)
    {
      case TRANSLATE_MODE:
      case ROTATE_MODE:
      case SCALE_MODE:
        clr = clrTransform;
        SelectObject(gDrawData.hdcMem, gShpData.hTransformPen);
        transformAllSurfaces();
        break;
      default:
        clr = clrEdge;
        SelectObject(gDrawData.hdcMem, gShpData.hDrawPen);
        break;
    }
  }
  else
  {
    clr = clrBG;
    SelectObject(gDrawData.hdcMem, gShpData.hBGPen);
  }

  switch(gShpData.drawMode)
  {
    case DRAW_MODE:
    case DRAWN_MODE:
    case TRANSLATE_MODE:
    case ROTATE_MODE:
    case SCALE_MODE:
    case TRANSFORMED_MODE:
      drawWireframe(clr); // or drawOutine();
      break;
  }
}

void eraseShape()
{
  drawShape(false);
}

void drawShape()
{
  drawShape(true);
}

void setShapeData()
{
}
