/* Copyright (C) 2001-2002 W.P. van Paassen - peter@paassen.tmfweb.nl

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

#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <fstream>
#include "WP_Endian.h"
#include "WP_Image.h"

namespace WPCG
{
WP_Image::WP_Image(): rasterpos_x(0), rasterpos_y(0), rows(0), columns(0), pixels(0)
{
  glPixelZoom(1, -1); //reverse y
}

WP_Image::WP_Image(int width, int height): rasterpos_x(0), rasterpos_y(0), rows(height), columns(width)
{
  pixels = new WP_RGBA[rows * columns];
  glPixelZoom(1, -1); //reverse y
}

WP_Image::WP_Image(const string &filename): rasterpos_x(0), rasterpos_y(0), rows(0), columns(0), pixels(0)
{
  loadImage(filename);
  glPixelZoom(1, -1); //reverse y
}

//COPY CONSTRUCTOR
WP_Image::WP_Image(const WP_Image &image)
{
  rows = image.rows;
  columns = image.columns;
  rasterpos_x = image.rasterpos_x;
  rasterpos_y = image.rasterpos_y;
  int size = rows * columns;
  
  pixels = new WP_RGBA[size];

  int i = 0;
  for (; i < size; i++)
    pixels[i] = image.pixels[i];
}

// ASSIGMENT OPERATOR 
WP_Image& WP_Image::operator=(const WP_Image& image)
{
  if (this == &image)
    return *this;

  rows = image.rows;
  columns = image.columns;
  rasterpos_x = image.rasterpos_x;
  rasterpos_y = image.rasterpos_y;
  int size = rows * columns;
  
  delete[] pixels;
  
  pixels = new WP_RGBA[size];
  
  int i = 0;
  for (; i < size; i++)
    pixels[i] = image.pixels[i];

  return *this;
}

WP_Image::~WP_Image()
{
  delete[] pixels;
}

void WP_Image::drawToFrameBuffer() const
{
  if (rows == 0 || columns == 0)
    return;
  
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glRasterPos2i(rasterpos_x, rasterpos_y);
  glDrawPixels(columns, rows, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

bool WP_Image::readFromFrameBuffer(int x, int y, int width, int height)
{
  delete[] pixels;
  pixels = 0;

  rows = height;
  columns = width;
  pixels = new WP_RGBA[rows * columns];
  if (!pixels)
    {
      return false;
    }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glReadPixels(x, y, columns, rows, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  
  return true;
}

bool WP_Image::loadImage(const string &file)
{
  //check file extensions

  if (hasValidExtension(file, "bmp"))
    {
      //load .bmp
      return loadBMP(file);
    }
  else if (hasValidExtension(file, "pcx"))
    {
      //load .pcx
      return loadPCX(file);
    }
  
  //file format not supported 
  return false;
}

bool WP_Image::loadBMP(const string &file)
{
  //.BMP is always stored little endian, pixels are stored bottom up, left to right
  
  WP_Endian* endian = WP_Endian::getInstance();

  try
    {
      fstream input(file.c_str(), ios::binary | ios::in);
      if(input.fail())
	{ 
	  throw("");
	}

      //read file to memory

      int size = 0;
      char c;
      while(input.get(c))
	{
	  size++;
	}
      
      byte* buffer = new byte[size];
      if (!buffer)
	{
	  input.close();
	  throw("WP_Image::loadBMP: Error while allocating memory");
	}

      input.clear();//!!!
      input.seekg(ios_base::beg);

      byte* p = buffer;

      while(input.get(c))
	{
	  *p = (unsigned char)c;
	  p++;
	}

      input.close();
      p = buffer;
      
      //read header from memory buffer
  
      p += 2; //skip 'B' and 'B'
      
      unsigned long filesize; 
      endian->getTypedData(&filesize, p, sizeof(unsigned long));
      unsigned short reserved1;			
      endian->getTypedData(&reserved1, p, sizeof(unsigned short)); //	0
      unsigned short reserved2;			
      endian->getTypedData(&reserved2, p, sizeof(unsigned short)); //	0
      unsigned long offBits;					
      endian->getTypedData(&offBits, p, sizeof(unsigned long));
      unsigned long headerSize;			
      endian->getTypedData(&headerSize, p, sizeof(unsigned long));	//	40
      unsigned long _columns;				
      endian->getTypedData(&_columns, p, sizeof(unsigned long));
      unsigned long _rows;						
      endian->getTypedData(&_rows, p, sizeof(unsigned long));
      unsigned short planes;				       
      endian->getTypedData(&planes, p, sizeof(unsigned short));	//	1
      unsigned short bitsPerPixel;			
      endian->getTypedData(&bitsPerPixel, p, sizeof(unsigned short));	
      unsigned long compression;			
      endian->getTypedData(&compression, p, sizeof(unsigned long));	//	0 for uncompressed
      unsigned long imageSize;				
      endian->getTypedData(&imageSize, p, sizeof(unsigned long));	
      unsigned long xPels;						
      endian->getTypedData(&xPels, p, sizeof(unsigned long));	
      unsigned long yPels;						
      endian->getTypedData(&yPels, p, sizeof(unsigned long));	
      unsigned long numLUTentries;		
      endian->getTypedData(&numLUTentries, p, sizeof(unsigned long));  //  0 in case of 24 bitsPerPixel
      unsigned long impColors;				
      endian->getTypedData(&impColors, p, sizeof(unsigned long));	//	0
      
      if ((bitsPerPixel == 24 && compression != 0) || (bitsPerPixel != 8 && bitsPerPixel != 24))
	{
	  delete[] buffer;
	  throw("");
	}
  
      int numberBytesInRow = ((_columns * (bitsPerPixel / 8)) + 3) & (~3);
      int numberPadBytes = numberBytesInRow - (_columns * ( bitsPerPixel / 8 ));
  
      rows = _rows;
      columns = _columns;
      
      delete[] pixels;
      pixels = 0;
      
      pixels = new WP_RGBA[rows * columns];
      
      if (!pixels)
	{
	  delete[] buffer;
	  throw("WP_Image::loadBMP: Unable to allocate memory for storage of image");
	}
  
      long count = 0;
      
      if (bitsPerPixel == 24)
	{
	  //24 bit uncompressed RGB bmp
	  
	  for (int row = rows - 1; row >= 0; row--)
	    {
	      for (int col = 0; col < columns; col++)
		{
		  count = row * columns + col;
		  pixels[count].b = *p;
		  p++;
		  pixels[count].g = *p;
		  p++;
		  pixels[count].r = *p;
		  p++;
		}
	      for (int k = 0; k < numberPadBytes; k++)
		p++;
	    }
	}
      else if (!compression)
	{
	  //8 bit uncompressed indexed bmp

	  //load palette

	  byte palette[1024];

	  p = buffer + 54 * sizeof(byte);

	  for (int i = 0; i < 1024; i++)
	    {
	      palette[i] = *p;
	      p++;
	    }

	  for (int row = rows - 1; row >= 0; row--)
	    {
	      for (int col = 0; col < columns; col++)
		{
		  int index = *p * 4;
		  p++;
		  
		  count = row * columns + col;
		  pixels[count].b = palette[index++];
		  pixels[count].g = palette[index++];
		  pixels[count].r = palette[index];
		}
	      for (int k = 0; k < numberPadBytes; k++)
		p++;
	    }
	}
      else
	{
	  //8 bit compressed (using RLE8) indexed bmp file

	  //load palette
	  
	  byte palette[1024];
	  
	  p = buffer + 54 * sizeof(byte);
	  
	  for (int i = 0; i < 1024; i++)
	    {
	      palette[i] = *p;
	      p++;
	    }

	  byte n;

	  for (int row = rows - 1; row >= 0; row--)
	    {
	      int x = 0;
	      while(true)
		{
		  n = *p;
		  p++;
		  c = *p;
		  p++;

		  if (n == 0x0)
		    {
		      if (c == 0x0)
			{
			  //end of line;
			  break;
			}
		      else if (c == 0x1)
			{
			  //end of bitmap
			  row = -1;
			  break;
			}
		      else if (c == 0x2)
			{
			  //delta
			  n = *p;
			  p++;
			  c = *p;
			  p++;
			  x += n;
			  row -= c;
			}
		      else
			{
			  //normal pixels
			  int pos = row * columns;
			  byte pad = c;
			  for (int k = 0; k < pad; k++)
			    {
			      c = *p;
			      p++;
			      int position = pos + x;
			      int index = c * 4;
			      pixels[position].b =  palette[index++];
			      pixels[position].g =  palette[index++];
			      pixels[position].r =  palette[index];
			      x++;
			    }
			  if (pad % 2)
			    {
			      p++; //read extra byte for maintaining 16 bit boundary
			    }
			}
		    }
		  else
		    {
		      int pos = row * columns;
		      int index = c * 4;
		      for (byte k = 0; k < n; k++)
			{
			  int position = pos + x;
			  pixels[position].b =  palette[index];
			  pixels[position].g =  palette[index + 1];
			  pixels[position].r =  palette[index + 2];
			  x++;
			}
		    }
		}      
	    }	  
	}
      
      delete[] buffer;
      return true;
    }
  catch(char* s)
    {
      cerr << s << endl;
      return false;
    }
  catch(...)
    {
      return false;
    }
}

bool WP_Image::loadPCX(const string &file)
{
  WP_Endian* endian = WP_Endian::getInstance();

  try
    {
      fstream input(file.c_str(), ios::binary | ios::in);
      if(input.fail())
	{ 
	  throw("");
	}

      //read file to memory

      int size = 0;
      char c;
      while(input.get(c))
	{
	  size++;
	}
      
      byte* buffer = new byte[size];
      if (!buffer)
	{
	  input.close();
	  throw("WP_Image::loadPCX: Error while allocating memory");
	}

      input.clear(); //!!!
      input.seekg(ios_base::beg);

      byte* p = buffer;

      while(input.get(c))
	{
	  *p = (byte)c;
	  p++;
	}

      input.close();
      p = buffer;
      
      //read header from memory buffer

      byte manufacturer = *p; //must be 10
      p++;

      byte version = *p; //version 5 for 24 bit version
      p++;
      byte encoding = *p; //1 if encoded
      p++;
      byte bitsPerPixel = *p; //must be 8
      p++;

      unsigned short xMin;
      endian->getTypedData(&xMin, p, sizeof(unsigned short)); 

      unsigned short yMin;
      endian->getTypedData(&yMin, p, sizeof(unsigned short)); 

      unsigned short xMax;
      endian->getTypedData(&xMax, p, sizeof(unsigned short)); 

      unsigned short yMax;
      endian->getTypedData(&yMax, p, sizeof(unsigned short)); 

      unsigned short hdPi; //horizontal resolution of image in dpi
      endian->getTypedData(&hdPi, p, sizeof(unsigned short)); 

      unsigned short vdPi; //vertical resolution of image in dpi
      endian->getTypedData(&vdPi, p, sizeof(unsigned short)); 
  
      //read beyond colormap
      p += 48 * sizeof(byte);

      byte reserved = *p; //should be 0
      p++;
      byte nPlanes = *p; //number of color planes
      p++;

      unsigned short bytesPerLine; //number of bytes per scanline, always even
      endian->getTypedData(&bytesPerLine, p, sizeof(unsigned short)); 

      unsigned short paletteInfo;
      endian->getTypedData(&paletteInfo, p, sizeof(unsigned short)); 

      unsigned short hScreenSize; //horizontal screen size in pixels
      endian->getTypedData(&hScreenSize, p, sizeof(unsigned short)); 

      unsigned short vScreenSize; //vertical screen size in pixels
      endian->getTypedData(&vScreenSize, p, sizeof(unsigned short)); 

      //read beyond filler
      p += 54 * sizeof(byte); //header filler

      //check header for compressed 24 bit pcx file

      if (encoding != 1 || bitsPerPixel != 8 || version < 5 || manufacturer != 10)
	{
	  delete[] buffer;
	  throw("");
	}

      //uncompress pcx file and fill scanlines
      
      delete[] pixels;
      pixels = 0;
      
      rows = yMax - yMin + 1;
      columns = xMax - xMin + 1;

      pixels = new WP_RGBA[rows * columns];
         
      if (!pixels)
	{
	  delete[] buffer;
	  throw("WP_Image::loadPCX: Unable to allocate memory for storage of image");
	}

      if (nPlanes == 3)
	{
	  //compressed 24 bit RGB pcx file (no 256 color palette)

	  for (int i = 0; i < rows; i++)
	    {
	      int pos = i * columns;
	      unsigned short count = 0; 
	      
	      while(count < bytesPerLine && count < columns)
		{
		  byte b = *p;
		  p++;
		  if ((b & 0xC0) == 0xc0)
		    {
		      b &= 0x3F;
		      for (byte k = 0; k < b; k++)
			{
			  pixels[pos + count++].r = *p;
			}
		      p++;
		    }
		  else
		    {
		      pixels[pos + count++].r = b;
		    }
		}
	      
	      count = 0;
	      
	      while(count < bytesPerLine && count < columns)
		{
		  byte b = *p;
		  p++;
		  if ((b & 0xC0) == 0xC0)
		    {
		      b &= 0x3F;
		      for (byte k = 0; k < b; k++)
			{
			  pixels[pos + count++].g = *p;
			}
		      p++;
		    }
		  else
		    {
		      pixels[pos + count++].g = b;
		    } 
		}
	      
	      count = 0;
	      
	      while(count < bytesPerLine && count < columns)
		{
		  byte b = *p;
		  p++;
		  if ((b & 0xC0) == 0xC0)
		    {
		      b &= 0x3F;
		      for (byte k = 0; k < b; k++)
			{
			  pixels[pos + count++].b = *p;
			}
		      p++;
		    }
		  else
		    {
		      pixels[pos + count++].b = b;
		    }
		} 
	    }
	}
      else
	{
	  //compressed 24 bit indexed pcx file

	  //find 256 color palette in buffer

	  p = (buffer + size) - 768; //768 = 3 * 256, 3 bytes representing RGB 

	  //fill palette

	  byte palette[768];

	  for (int i = 0; i < 768; i++)
	    {
	      palette[i] = *p;
	      p++;
	    }

	  //point p to begin of image data

	  p = buffer + (128 * sizeof(byte)); //read beyond header

	  for (int i = 0; i < rows; i++)
	    {
	      int pos = i * columns;
	      unsigned short count = 0; 
	      
	      while(count < bytesPerLine && count < columns)
		{
		  byte b = *p;
		  p++;
		  if ((b & 0xC0) == 0xc0)
		    {
		      b &= 0x3F;
		      for (byte k = 0; k < b; k++)
			{
			  int index = *p * 3;
			  int position = pos + count++;
			  pixels[position].r = palette[index++];
			  pixels[position].g = palette[index++];
			  pixels[position].b = palette[index];
			}
		      p++;
		    }
		  else
		    {
		      int index = b * 3;
		      int position = pos + count++;
		      pixels[position].r = palette[index++];
		      pixels[position].g = palette[index++];
		      pixels[position].b = palette[index];
		    }
		}
	    }
	}
      
      delete[] buffer;
      return true;
    }
  catch(char* s)
    {
      cerr << s << endl;
      return false;
    }
  catch(...)
    {
      return false;
    }
}

void WP_Image::chromaKey(const WP_RGBA& key, byte alpha)
{
  long count = 0;
  for (int row = 0; row < rows; row++)
    {
      for (int col = 0; col < columns; col++)
	{
	  if (pixels[count].r == key.r && pixels[count].g == key.g &&
	      pixels[count].b == key.b)
	    {		
	      pixels[count].a = alpha;
	    }
	  count++;
	}
    }
}

void WP_Image::setTextureGL(GLuint* texture_id, GLint wrap_s, GLint wrap_t, 
			    GLint mag_filter, GLint min_filter, bool mipmapping)
{
  glGenTextures(1, texture_id);
  glBindTexture(GL_TEXTURE_2D, *texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
  
  if (mipmapping)
    {
      //using mipmaps
      gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, columns, rows, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    }
  else
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,columns, rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    }
}
}
