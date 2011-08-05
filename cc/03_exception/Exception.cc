
#include "Exception.h"

Error::Error()
{
}

Error::Error(const char * err)
{
	cerr << endl << "\e[1;31m" << "Error: " << "\e[m"
		 << err << endl << endl;
}
