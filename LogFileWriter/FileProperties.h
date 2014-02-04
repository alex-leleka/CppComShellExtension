/****************************** Module Header ******************************\
Module Name:  FileProperties.h
Project:      LogFileWriter

Module using to write file metadata(creating date, size) in std::wostream 
without opening file and calculate check sum ( in this case file'll be open)

\***************************************************************************/
#pragma once
#include <Shlwapi.h> // GetFileAttributesEx
#include <boost/filesystem/operations.hpp> // file size
#include <string>
class FileProperties
{
	std::wstring m_file_name_; 
	bool m_initialized;
	boost::uintmax_t m_file_size;
	SYSTEMTIME m_stLocal;
	bool init(void);
public:
	FileProperties(const wchar_t *);
	FileProperties(const std::wstring &);
	~FileProperties(void);

	bool GetFileSize(boost::uintmax_t &);
	bool GetFileCreatingTime(SYSTEMTIME &);
	bool GetFileCheckSum(unsigned __int32 &);
	bool WriteFileCreatingTime(std::wostream &);
	bool WriteFileSize(std::wostream &);
};

