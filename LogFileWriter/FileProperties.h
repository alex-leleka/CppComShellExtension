/****************************** Module Header ******************************\
Module Name:  FileProperties.h
Project:      LogFileWriter

File implements methods to get file metadata(creating date, size) and check sum
using WinAPI and boost.

Exceptons: FilePropertiesException
\***************************************************************************/
#pragma once
#include <string>
#include <cstdint>
#include <Shlwapi.h> // GetFileAttributesEx
#include <boost/filesystem/operations.hpp> // file size

class FileProperties
{
	std::wstring m_fileName; 
	bool m_initialized;
	boost::uintmax_t m_fileSize;
	SYSTEMTIME m_stLocal;
	bool init(void);
public:
	explicit FileProperties(const std::wstring &);
	~FileProperties(void);

	bool GetFileSize(boost::uintmax_t &);
	bool GetFileCreatingTime(SYSTEMTIME &);
	bool GetFileCheckSum(uint32_t &);
};

