/****************************** Module Header ******************************\
Module Name:  FileProperties.cpp
Project:      LogFileWriter

File implements methods to get file metadata(creating date, size) and check sum
using WinAPI and boost.

Exceptons: FilePropertiesException
\***************************************************************************/

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "FileProperties.h"
#include "FilePropertiesException.h"

#pragma comment(lib, "shlwapi.lib")

FileProperties::FileProperties(const std::wstring & fileName)
{
	m_initialized = FALSE;
	if (fileName.empty())
	{
		//raise exception
		throw FilePropertiesException("File name is empty!");
		return;
	}
	// create string with path to file
	m_fileName = std::wstring(fileName);
}

bool FileProperties::init(void)
{
	if (m_initialized)
		return TRUE;
	// File size using boost
    boost::filesystem::path p(m_fileName) ;
	m_fileSize = boost::filesystem::file_size(p);
	
	WIN32_FILE_ATTRIBUTE_DATA fad;
	if (!GetFileAttributesEx(m_fileName.c_str(), GetFileExInfoStandard, &fad))
        return FALSE; // error condition, could call GetLastError to find out more
	// File creation time
	PFILETIME ftCreate;
	// SYSTEMTIME is another name for struct _SYSTEMTIME structure (a typedef)
	SYSTEMTIME stUTC;
	ftCreate = &fad.ftCreationTime;
	// Convert the file created time to local time.
	FileTimeToSystemTime(ftCreate, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &m_stLocal);
	return m_initialized = TRUE;

}

bool FileProperties::GetFileSize(boost::uintmax_t & fileSize)
{
	if(!init())
		return FALSE;
	fileSize = m_fileSize;
	return TRUE;
}
bool FileProperties::GetFileCreatingTime(SYSTEMTIME & stLocal)
{
	if(!init())
		return FALSE;
	stLocal = m_stLocal;
	return TRUE;
}
bool FileProperties::GetFileCheckSum(uint32_t & checkSum)
{
	std::ifstream selectedFile;
    selectedFile.open(m_fileName, std::ios::binary | std::ios::in);
    if(!selectedFile.is_open())
    {
        return FALSE;
    }
	// BSD checksum
    char ch;
	checkSum = 0;
	const uint32_t LastBitPosition = 8 * sizeof(uint32_t) - 1; // 31
    while(!selectedFile.eof())
    {
		selectedFile.read(&ch, 1);
     	checkSum = (checkSum >> 1) + ((checkSum & 1) << LastBitPosition);
		checkSum += static_cast<uint32_t>(ch);
		checkSum &= 0xffffffff;
    }
	selectedFile.close();
	return TRUE;
}
FileProperties::~FileProperties(void)
{
}
