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

#ifndef WP_ENDIAN_H
#define WP_ENDIAN_H

#include <iostream>
#include <fstream>
using namespace std;

#include "WP_Def.h"
#include "WP_RGBA.h"

namespace WPCG
{
/**
 * this singleton class contains utility functions for loading different types on platforms which differ in endiannes (little endian (intel, etc.) or big endian (sparc, etc.)) \n
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
class WP_Endian
{
 public:
  ~WP_Endian(){};
  
  /**
   * this function returns a pointer to the only instance of this class (singleton)
   * @return a pointer to the only instance of this class
   */
  static WP_Endian* getInstance()
    {	
      if (!_instance)
	_instance = new WP_Endian();
      return _instance;
    }

  /**
   * this function gets a specific type of size <i>size</i> from memory pointed to by the buffer pointer, for example to get an int from memory
   * @param destination a void pointer to a variable in which the read memory data will be stored
   * @param buffer a reference to a pointer pointing to the memory buffer holding the image file
   * @param size the number of bytes to be read from memory
   */
   void getTypedData(void* destination, byte*& buffer, int size);

  /**
   * this boolean indicates if the functions presume a little endian architecture
   */
  bool little_endian;

private:
	WP_Endian();

	/**
	 * a static pointer to the only instance of this class
	 */
	static WP_Endian* _instance;
};
}
#endif

