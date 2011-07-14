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

#ifndef WP_IMAGE_H
#define WP_IMAGE_H

#include <string>
#include "WP_Def.h"
#include "WP_RGBA.h"

namespace WPCG
{
/**
 * this class is mainly used for image loading and texture creation. Currently, compressed/uncompressed 8 bit (indexed) bmp and uncompressed 24 bit (RGB) bmp and compressed 24 (indexed) bit pcx and compressed 24 bit (RGB) pcx files are supported\n. Note that the .bmp format is always in little endian format\n
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
class WP_Image
{
 public:
  WP_Image();

  WP_Image(int width, int height);

  WP_Image(const string &filename);

  //copy constructor
  WP_Image(const WP_Image &image);

  ~WP_Image();
  
  /**
   * assignment operator
   * @param image the to be copied WP_Image object
   * @return a reference to this new object containing the new copied image values
   */
  WP_Image& operator=(const WP_Image& image);
  
  /**
   * this function writes a pixel in the current image
   * @param x the column index of the to be placed pixel
   * @param y the row index of the to be placed pixel
   * @param p a WP_RGBA object representing the pixel and holding the pixels' RGBA values
   */
  inline void setPixel(int x, int y, const WP_RGBA& p)
    {
      if (x >= 0 && x < columns && y >= 0 && y < rows)
	pixels[y * columns + x] = p;
    }

  /**
   * this function writes a pixel in the current image without checking the x and y ranges
   * @param x the column index of the to be placed pixel
   * @param y the row index of the to be placed pixel
   * @param p a WP_RGBA object representing the pixel and holding the pixels' RGBA values
   */
  inline void setFastPixel(int x, int y, const WP_RGBA& p)
    {
      pixels[y * columns + x] = p;
    }

  /**
   * this functions gets a pixel from the image
   * @param x the column index of the wanted pixel
   * @param y the row index of the wanted pixel
   * @return a pointer to WP_RGBA object holding the pixels' RGBA values
   */
  inline WP_RGBA* getPixel(int x, int y)
    {
      if (x >= 0 && x < columns && y >= 0 && y < rows)
	return pixels + (y * columns + x);
  
      return (WP_RGBA*)0; //failure
    }

  /**
   * this functions gets a pixel from the image without checking the ranges
   * @param x the column index of the wanted pixel
   * @param y the row index of the wanted pixel
   * @return a pointer to WP_RGBA object holding the pixels' RGBA values
   */
  inline WP_RGBA* getFastPixel(int x, int y)
    {
      return pixels + (y * columns + x);
    }

  /**
   * this function draws the image directly to the framebuffer using OpenGL, placing the lower left corner of the pixmap at the current raster position (rasterpos_x and rasterpos_y)
   */
  void drawToFrameBuffer() const;

  /**
   * this function reads an rectangular image from the lower leftcorner of the framebuffer and stores it in this image
   * @param x the column index which indicates the left column of the image's rectangle
   * @param y the row index which indicates the lower row of the image's rectangle
   * @param width the width of the image's rectangle
   * @param height the height of the image's rectangle
   * @ return a boolean indicating the success or failure of this function
   */
  bool readFromFrameBuffer(int x, int y, int width, int height);

  /**
   * this function copies a rectangular portion of the backbuffer (the lowerleft corner, indicated by x, y, width and height) to the backbuffer at the current raster position
   * @param x the column index which indicates the left column of the image's rectangle
   * @param y the row index which indicates the lower row of the image's rectangle
   * @param width the width of the image's rectangle
   * @param height the height of the image's rectangle
   * @ return a boolean indicating the success or failure of this function
   */
  inline void copy(int x, int y, int width, int height)
    {
      glCopyPixels(x, y, width, height, GL_COLOR);
    }
  
  /**
   * this function creates a texture from this image and sets it for the use in OpenGL
   * @param texture_id a pointer to an unsigned integer in which the texture id is stored by OpenGL
   * @param wrap_s specifies OpenGL's behaviour for wrapping a texture in the s direction (see OpenGL red book), default is GL_REPEAT
   * @param wrap_t specifies OpenGL's behaviour for wrapping a texture in the t direction (see OpenGL red book), default is GL_REPEAT
   * @param mag_filter specifies OpenGL's behaviour for magnifying and filtering the texture (see OpenGL red book), default is GL_NEAREST 
   * @param min_filter specifies OpenGL's behaviour for minifying and filtering the texture (see OpenGL red book), default is GL_NEAREST
   * @param mipmapping a boolean indicating if mip maps of the textrure should be created ans used by OpenGL (see OpenGL red book), default is false
   */
  void setTextureGL(GLuint* texture_id, GLint wrap_s=GL_REPEAT, GLint wrap_t=GL_REPEAT, 
			    GLint mag_filter=GL_NEAREST, GLint min_filter=GL_NEAREST, bool mipmapping=false);

  /**
   * this function loads an image
   * @param file the name of the file containing the image
   * @return a boolean indicating the succes or failure of this function
   */
  bool loadImage(const string &file);

  /**
   * this function is able to set the alpha of all pixels matching the key pixel (chroma keying or blue screening)
   * @param key the to be matched pixel. Only the alpha value of pixels matching this key are set to <i>alpha</i>
   * @param alpha the new alpha value of the matching pixel
   */
  void chromaKey(const WP_RGBA& key, byte alpha);

  /**
   * the x raster position of the image in the framebuffer
   */
  int rasterpos_x; 
  
  /**
   * the y raster position of the image in the framebuffer
   */
  int rasterpos_y;

  /**
   * the number of pixel rows in the image
   */
  int rows;
  
  /**
   * the number of pixel columns in the image
   */
  int columns;


 protected:
  /**
   * the array of pixels
   */
  WP_RGBA* pixels;

  /**
   * this function checks the extension of a file
   * @param file the file name
   * @param extension the extension which is checked
   * @return a boolean indicating if the file has the extension
   */
  inline bool hasValidExtension(const string &file, const string &extension)
    {
      int pos = file.find('.' + extension);
      return pos != string::npos;
    }

  /**
   * this function loads a bitmap (.bmp). Currently compressed/uncompressed 8 bit (indexed) and uncompressed 24 bit RGB bmp\n
   * @param file the bitmap file
   * @return a boolean indicating the succes or failure of this function
   */
  bool loadBMP(const string &file);

  /**
   * this function loads ZSoft pcx file format (.pcx). Currently compressed 24 bit indexed and compressed 24 bit RGB pcs\n
   * @param file the pcx file
   * @return a boolean indicating the succes or failure of this function
   */
  bool loadPCX(const string &file);
};
}
#endif

