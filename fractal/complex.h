/** complex.h
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

#ifndef __COMPLEX_H__
#define __COMPLEX_H__

typedef struct _complex
{
	double x, y;
} complex;

complex complex_add(complex a, complex b);
complex complex_mul(complex a, complex b);
double complex_modsq(complex a);

#endif

