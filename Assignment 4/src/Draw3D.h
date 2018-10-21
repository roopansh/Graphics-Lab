#define ID_LABEL     40001
#define ID_CLEAR     40002
#define ID_EXIT      40003
#define ID_THETA     40004
#define ID_PHI       40005
#define ID_R         40006
#define ID_DIST      40007
#define ID_DRAW      40008 
#define ID_CANCEL    40009 

#define ID_VARLBL0   40010
#define ID_VARFLD0   40011
#define ID_VARLBL1   40012
#define ID_VARFLD1   40013
#define ID_VARLBL2   40014
#define ID_VARFLD2   40015

#define ID_CUBE      40016
#define ID_CONE      40017
#define ID_CYLINDER  40018
#define ID_SPHERE    40019

#define ID_TRANSLATE 40020
#define ID_ROTATE    40021
#define ID_SCALE     40022

typedef enum
{
  READY_MODE,
  DRAW_MODE,
  DRAWN_MODE,
  TRANSLATE_MODE,
  ROTATE_MODE,
  SCALE_MODE,
  TRANSFORMED_MODE,
} MODE;
 
const COLOR clrBG = {255,255,255};
const COLOR clrTransform = {0,0,100};

typedef enum
{
  ROTATE_X,
  ROTATE_Y,
  ROTATE_Z
} ROTATE_TYPE;

LRESULT CALLBACK DlgTrnsltScale(HWND hdlg,UINT mess,
                                WPARAM more,LPARAM pos);
LRESULT CALLBACK DlgRotate(HWND hdlg,UINT mess,WPARAM more,LPARAM pos);
void translate3D(POINT3D & cartesianPt, double delx, 
                 double dely, double delz);
void translate3D(POINT3D & cartesianPt, const POINT3D& origin);
void rotate3D(POINT3D & cartesianPt, ROTATE_TYPE rotType, 
              double rotAngle);
void rotateNew(POINT3D & cartesianPt, double angle);              
void scale3D(POINT3D & cartesianPt, double sx, double sy, double sz);
void transformAllSurfaces();
