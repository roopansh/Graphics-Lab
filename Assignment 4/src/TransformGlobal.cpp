#include <windows.h>
#include "DrawUtil3D.h"
#include "Draw3D.h"
#include "Transform3D.h"

SHAPE_DATA gShpData;
const char *szShapeType[] = {"", "Cube", "Cone",  
                             "Cylinder", "Sphere"};
const char *szRotateType[] = {"X", "Y", "Z"};
