#include <windows.h>
#include "Cube.h"
#include <vector> 
#include <sstream>
using namespace std;
void setShapeData(char inp[])
{

	stringstream ss(inp);
	POINT3D origin;
	double x, y, z, side;
	while (ss >> x >> y >> z >> side){
		origin.x = x; origin.y = y; origin.z = z;
		setCubeData(origin, side);
	}

}

