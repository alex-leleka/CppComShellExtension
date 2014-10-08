/****************************** Module Header ******************************\
Module Name:  FilePropertiesException.h
Project:      LogFileWriter

File implements exception which rasing in FileProperties methods.

\***************************************************************************/
#pragma once
#include <exception>

class FilePropertiesException :
	public std::exception
{
public:
	FilePropertiesException(const char * m);
};
