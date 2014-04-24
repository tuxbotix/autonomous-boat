#include "control.h"
#include <math.h>

int getPerpendicularDistance(float *x,float *y){//x[0] & x[1] are line, x[3]the current position

float dx=x[1]-x[2];
float dy=y[1]-y[2];

int d=abs(dy*x[0] - dx*y[0] + x[2]*y[2]-x[2]*y[1]);
d= d/sqrt(dx*dx + dy*dy);

	return d;
}
