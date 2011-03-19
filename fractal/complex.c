/** complex.c
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

#include "complex.h"

complex complex_add(complex a, complex b)
{
	return (complex){ a.x+b.x, a.y+b.y };
}

complex complex_mul(complex a, complex b)
{
	/* (x1+y1i)(x2+y2i)
	 * = (x1x2 - y1y2) + (x1y2 + y1x2)i 
	 */
	return (complex){ a.x*b.x-a.y*b.y, a.x*b.y+a.y*b.x };
}

double complex_modsq(complex a)
{
	return a.x*a.x + a.y*a.y;
}

