/****************************** Module Header ******************************\
Module Name:  FilePropertiesException.cpp
Project:      LogFileWriter

File implements exception which rasing in FileProperties methods.

\***************************************************************************/

#include "FilePropertiesException.h"

FilePropertiesException::FilePropertiesException(const char * m) : std::exception(m)
{
}

