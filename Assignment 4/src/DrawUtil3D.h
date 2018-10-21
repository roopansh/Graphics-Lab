const double PI = 3.142857;

typedef struct 
{
  double x;
  double y;
  double z;
} POINT3D;

typedef struct 
{
  double x;
  double y;
} POINT2D;

typedef struct 
{
  int r, g, b;
} COLOR;

typedef struct
{
  HDC hdcMem; 
  HBITMAP hbmp;
  SIZE windowSize;
  POINT origin;
  SIZE maxBoundary;
  double r, theta, phi, screen_dist;
} DRAWING_DATA;

typedef struct
{
  int nPoints;
  POINT3D *vertex;
  POINT3D *eye3D;
  POINT2D *proj2D;
  POINT2D min;
  POINT2D max;
  double A, B, C, D; // for plane eqn
  COLOR clr;
} SURFACE3D;

typedef struct
{
  int nSurfaces;
  SURFACE3D *surface;
  POINT2D min;
  POINT2D max;  
} SHAPE3D;

const int WINDOW_WIDTH=350;
const int WINDOW_HEIGHT=250;

void initialize_shapeData();
void initialize(HWND hwnd, HDC hdc);
void compute3DCoefficients(double r, double theta, double phi);
void move3D(POINT3D ptWorld);
void line3D(POINT3D ptWorld);
void setShapeData();
void cleanup();
void drawImage(HDC hdc);
void get3DEyeFrom3DWorld(const POINT3D & pt3DWorld, 
                         POINT3D& pt3DEye);
void get2DPerspectiveFrom3DWorld(const POINT3D & pt3DWorld, 
                                 double &x, double &y);
void crossProduct(const POINT3D pt[], int nPts, double& A,
                  double& B,double& C);
void drawWireframe(COLOR);
void genImage(HWND hwnd);
void addSurface(const POINT3D& a, 
                const POINT3D& b, 
                const POINT3D& c,
                const POINT3D& d);
void addSurface(POINT3D pts[], 
                const int &noOfPts 
               );
void resetDataForShape(); 
void prepareShapeDataForDrawing();             
