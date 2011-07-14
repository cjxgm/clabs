#include "WP_Endian.h"

namespace WPCG
{
WP_Endian* WP_Endian::_instance = 0;

WP_Endian::WP_Endian()
{
  //determine endiannes

  int x = 1;
  (*(char*)&x) ? little_endian = true : little_endian = false;
}

void WP_Endian::getTypedData(void* destination, byte*& buffer, int size)
{
  byte* d = (byte*)destination;

  if (little_endian)
    {
      for (int i = 0; i < size; i++)
	{
	  *(d + i) = *buffer;
	  buffer++;
	}
    }
  else
    {
      for (int i = 0; i < size; i++)
	{
	  *(d + size - i - 1) = *buffer;
	  buffer++;
	}
    }
}
}
