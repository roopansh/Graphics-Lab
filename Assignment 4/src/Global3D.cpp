#include <windows.h>
#include "DrawUtil3D.h"

// global data 

DRAWING_DATA gDrawData;
SHAPE3D shape;
double V[4][3];
int MAX_SURFACES=6000; // maximum number of surfaces
int MAX_SURFACE_EDGES=100; // maximum no of edges per surface
COLOR clrEdge ={255,0,0}; 
