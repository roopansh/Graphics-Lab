typedef struct
{
  int coneHeight, coneBaseRadius;
} CONE_DATA;

typedef struct
{
  int cubeSide;
} CUBE_DATA;

typedef struct
{
  int cylHeight, cylBaseRadius;
} CYLINDER_DATA;

typedef struct
{
  int sphereRadius;
} SPHERE_DATA;

typedef enum
{
  NONE,
  CUBE,
  CONE,
  CYLINDER,
  SPHERE
} SHAPE;

typedef struct
{
  HPEN hBGPen, hDrawPen;
  HPEN hTransformPen;
  MODE drawMode;
  SHAPE shape;
  void *pShapeData;
  int statusHt;
  HWND hwndStatus;
  int tx, ty, tz; // used for translate 
  double sx, sy, sz; // used for scale
  ROTATE_TYPE rotType; // used for rotate
  double angOfRotation; // used for rotate
} SHAPE_DATA;

const double MIN_R=100.0;
const double MAX_R=800.0;
const double MIN_THETA=0.0;
const double MAX_THETA=360.0; 
const double MIN_PHI=0.0;
const double MAX_PHI=180.0; 

LRESULT CALLBACK StatusDlgShape(HWND hdlg,UINT mess,WPARAM more,
                                LPARAM pos);
void setDrawMode(MODE mode, HWND hwnd);
void reDraw(HWND hwnd);
void calculateSize(HWND hwnd);
void setupAndDrawShape(HWND hwndParent, SHAPE shape);
void eraseShape();
void drawShape();
void showStatusOfDrawing();
void drawOutline();
void setupMenus(HWND hwnd);
void resetDataForShape();
LRESULT CALLBACK DlgShape(HWND hdlg,UINT mess,WPARAM more,
                          LPARAM pos);
void reset_XformData(HWND hwnd);
void initialize_XformData(HWND hwnd, HDC hdc);
void processCommand(int cmd, HWND hwnd);
