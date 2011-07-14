/* Copyright (C) 2001 W.P. van Paassen - peter@paassen.tmfweb.nl

   This program is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your
   option) any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

#ifndef WP_MATH_H
#define WP_MATH_H

#include "WP_Def.h"

namespace WPCG
{
/**
 * a singleton class holding sin and cos values for a quick lookup of angles in degrees and for conversions from degrees to radians and vice versa\n
 * @author Copyright (C) 2001 W.P. van Paassen   peter@paassen.tmfweb.nl
 *
 *  This program is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  
 */
class WP_Math
{
public:
	~WP_Math(){};

	/**
	 * this function returns a pointer to the only instance of this class (singleton)
	 * @return a pointer to the only instance of this class
	 */
	static WP_Math* getInstance()
	{	if (!_instance)
			_instance = new WP_Math();
		return _instance;
	}

	/**
	 * this function converts an angle in degrees to an angle in radians
	 * @param angle the angle in degrees
	 * @return the angle in radians
	 */
	scalar degreeToRad(int angle) const
	  {
	    return ((scalar)angle) * DEGRAD;
	  }

	/**
	 * this function converts an angle in degrees to an angle in radians
	 * @param angle the angle in degrees
	 * @return the angle in radians
	 */
	scalar fDegreeToRad(scalar angle) const
	{
	  return angle * DEGRAD;
	}

	/**
	 * this functions converts an angle in radians to an angle in degrees
	 * @param rad the angle in radians
	 * @return the angle in degrees 
	 */
	int radToDegree(scalar rad) const
	{
	  int result = (int)(round(rad * RADDEG));
  
	  if (result < 0)
	    {
	      result %= 360;
	      return result + 360;
	    }
  
	  if (result >= 360)
	    {
	      result %= 360;
	      return result - 360;
	    }

	  return result % 360;
	}

	/**
	 * this functions converts an angle in radians to an angle in degrees
	 * @param rad the angle in radians
	 * @return the angle in degrees 
	 */
	scalar fRadToDegree(scalar rad) const
	{
	  return rad * RADDEG;
	}

	/**
	 * this function rounds of scalars to whole numbers. for example 1.49 is rounded to 1.0 and 1.50 is rounded to 2.0
	 * @param s the to be rounded scalar
	 * @return the rounded scalar
	 */
	scalar round(scalar s) const
	{
	  scalar f = (scalar)floor(s);
	  if ((s - 0.5) < f)
	    return f;
	  return (scalar)ceil(s);
	}

	/**
	 * this function returns the sin value of an angle in degrees
	 * @param i the angle in degrees
	 * @return the sin value of the angle
	 */
	scalar getSin(int i) const 
	  { 
	    if (i < 0)
	      {
		i %= 360;
		if (i < 0)
		  {
		    i = 360 + i ;
		  }
	      }
	    else
	      {
		i %= 360;
	      }
	    return aSin[i];
	  };

	/**
	 * this function returns the cos value of an angle in degrees
	 * @param i the angle in degrees
	 * @return the cos value of the angle
	 */
	scalar getCos(int i) const 
	  { 
	    if (i < 0)
	      {
		i %= 360;
		if (i < 0)
		  {
		    i = 360 + i;
		  }
	      }
	    else
	      {
		i %= 360;
	      }
	    return aCos[i];
	  };

private:
	WP_Math();

	/**
	 * an array of scalars holding 360 sin values mapped to their angle. So aSin[3] = the sin of an angle of 3 degrees
	 */
	scalar aSin[360];

	/**
	 * an array of scalars holding 360 cos values mapped to their angle. So aCos[3] = the cos of an angle of 3 degrees
	 */
	scalar aCos[360];

	/**
	 * a static pointer to the only instance of this class
	 */
	static WP_Math* _instance;
};	
}
#endif

