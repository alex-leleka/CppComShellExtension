/****************************** Module Header ******************************\
Module Name:  FileProperties.cpp
Project:      LogFileWriter

Module using to write file metadata(creating date, size) in std::wostream 
without opening file.

\***************************************************************************/

#include "FileProperties.h"
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

#pragma comment(lib, "shlwapi.lib")


FileProperties::FileProperties(const wchar_t * file_name)
{
	m_initialized = false;
	if (file_name ==  nullptr)
	{
		//raise exception
		throw std::string("wrong arguments!");
		return;
	}
	// create string with path to file
	m_file_name_ = std::wstring(file_name);
}
FileProperties::FileProperties(const std::wstring & file_name)
{
	m_initialized = FALSE;
	if (file_name.empty())
	{
		//raise exception
		throw std::string("wrong arguments!");
		return;
	}
	// create string with path to file
	m_file_name_ = std::wstring(file_name);
}

bool FileProperties::init(void)
{
	if (m_initialized)
		return TRUE;
	// File size using boost
    boost::filesystem::path p(m_file_name_) ;
	m_file_size = boost::filesystem::file_size(p);
	
	WIN32_FILE_ATTRIBUTE_DATA fad;
	if (!GetFileAttributesEx(m_file_name_.c_str(), GetFileExInfoStandard, &fad))
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
bool FileProperties::WriteFileSize(std::wostream & input_file)
{
	if(!init())
		return FALSE;
	input_file << "Size: " << m_file_size;
    return TRUE;
}
bool FileProperties::WriteFileCreatingTime(std::wostream & input_file)
{
	if(!init())
		return FALSE;
	auto old_char_fill = input_file.fill(L'0');
	auto old_field_width = input_file.width(2);
	input_file << L"Created on: " << m_stLocal.wDay << L"/" << m_stLocal.wMonth << L"/" <<
		m_stLocal.wYear <<  L" : "<< L" " << m_stLocal.wHour << L":" << m_stLocal.wMinute;
	input_file.fill(old_char_fill);
	input_file.width(old_field_width);
	return TRUE;
}
bool FileProperties::GetFileSize(boost::uintmax_t & file_size)
{
	if(!init())
		return FALSE;
	file_size = m_file_size;
	return TRUE;
}
bool FileProperties::GetFileCreatingTime(SYSTEMTIME & stLocal)
{
	if(!init())
		return FALSE;
	stLocal = m_stLocal;
	return TRUE;
}
bool FileProperties::GetFileCheckSum(unsigned __int32 & checkSum)
{
	std::ifstream selectedFile;
    selectedFile.open(m_file_name_, std::ios::binary | std::ios::in);
    if(!selectedFile.is_open())
    {
        return FALSE;
    }
	// BSD checksum
    char ch;
	checkSum = 0;
	const unsigned int LastBitPosition = 8 * sizeof(__int32) - 1; // 31
    while(!selectedFile.eof())
    {
		selectedFile.read(&ch, 1);
     	checkSum = (checkSum >> 1) + ((checkSum & 1) << LastBitPosition);
		checkSum += static_cast<unsigned int>(ch);
		checkSum &= 0xffffffff;
    }
	selectedFile.close();
	return TRUE;
}
FileProperties::~FileProperties(void)
{
}
