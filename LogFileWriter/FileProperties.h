/****************************** Module Header ******************************\
Module Name:  FileProperties.h
Project:      LogFileWriter

Module using to write file metadata(creating date, size) in std::wostream 
without opening file.

\***************************************************************************/
#pragma once
#include <Shlwapi.h> // GetFileAttributesEx
#include <string>
class FileProperties
{
	std::wstring m_file_name_; 
	bool m_initialized;
	LARGE_INTEGER m_file_size;
	SYSTEMTIME m_stLocal;
	bool init(void);
public:
	FileProperties(const wchar_t *);
	FileProperties(const std::wstring &);
	~FileProperties(void);
	
	bool WriteFileCreatingTime(std::wostream &);
	bool WriteFileSize(std::wostream &);
};

