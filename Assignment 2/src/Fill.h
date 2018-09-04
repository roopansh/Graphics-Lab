#define ID_DRAW_CIRCLE      40001
#define ID_FILL				40002
#define ID_CLEAR            40003
#define ID_EXIT             40004
#define ID_OK               40005
#define ID_CANCEL           40006
#define ID_EB1              40007
#define ID_EB2              40008
#define ID_EB3              40009
#define ID_LABEL1           40010
#define ID_LABEL2           40011
#define ID_LABEL3           40012

const COLORREF CLR_FILL_RED = RGB(255, 0, 0);
const COLORREF CLR_FILL_BLUE = RGB(0, 0, 255);
const COLORREF CLR_FILL_GREEN = RGB(0, 255, 0);
const COLORREF CLR_FILL_YELLOW = RGB(255, 255, 0);
const COLORREF CLR_BOUNDARY = RGB(0, 0, 0);
const COLORREF CLR_BG = RGB(255, 255, 255);

typedef enum
{
	READY_MODE,
	DRAW_MODE,
	DRAWN_MODE,
	FILL_MODE,
	FILLED_MODE
} MODE;


typedef struct
{
	HDC hdcMem;
	HBITMAP hbmp;
	MODE drawMode;
	SIZE maxBoundary;
	int radii[3];
	int X[3];
	int Y[3];
	int circleCount;
} DRAWING_DATA;

extern DRAWING_DATA gDrawData; // global data being shared across files

void setupMenus(HWND hwnd);
void setDrawMode(MODE mode, HWND hwnd);
void reDraw(HWND hwnd);
void processCommonCommand(int cmd, HWND hwnd);
void processCommand(int cmd, HWND hwnd);

