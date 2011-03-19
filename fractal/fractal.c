/** fractal.c
 * 
 **
 * This file is part of PoshX project.
 * Under GPLv3. See COPYING for more details.
 * Obey the KING Principle.
 * NO WARRANTY!
 **
 * AUTHORS:
 *		eXerigumo Clanjor
 * COPYLEFT(C):
 *		Clan Open Studio, 2010-2011.
 */

#include <stdio.h>
#include "complex.h"

void fractal();
int julia(complex z, complex c, int lim);
void plot(double x, double y, int deep);

int am=0;

int main()
{
	fractal();
	return 0;
}

#define BY -0.40
#define EY 1.4
#define BX -0.90
#define EX 0.70
#define SS 0.01

void fractal()
{
	printf("fdata = new Array();\n");
	double x, y;
	for (y=BY; y<=EY; y+=SS)
		for (x=BX; x<=EX; x+=SS)
			plot(x, y, julia((complex){x, y},
						(complex){0.27334, 0.00742}, 50));
	int i;
}

void plot(double x, double y, int deep)	
{
	printf("fdata[%d]={ x:%f, y:%f, deep:%f };\n",
			am,
			(x-BX)/(EX-BX)*200-100, 
			(y-BY)/(EY-BY)*200-100, 
			deep/50.0*3.14);
	am++;
}

int julia(complex z, complex c, int lim)
{
	int ct=0;
	while(complex_modsq(z) < 4.0 && ct<lim){
		++ct;
		z = complex_mul(z, z);
		z = complex_add(z, c);
	}
	return ct;
}
