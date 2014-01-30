/****************************** Module Header ******************************\
Module Name:  LogFileWriter.cpp
Project:      LogFileWriter

Module create or rewrite log file. Write in log metadata(creating date, size)
of files received in the constructor sorted in alphabetical order.

\***************************************************************************/

#include <algorithm> // std::sort
#include <fstream> // std::wofstream
#include <codecvt> // locale
#include <locale> // locale
#include "FileProperties.h"
#include "LogFileWriter.h"

LogFileWriter::LogFileWriter(wchar_t ** files_name, unsigned int files_count)
{
	files_name_ = files_name;
	files_count_ = files_count;
}

void LogFileWriter::sort_files_name_array(void)
{
	std::sort(files_name_, files_name_ + files_count_, [](wchar_t * a, wchar_t * b) {
        return wcscmp(b, a) == 1;   // sorting in ascending order
    });
}
wchar_t * LogFileWriter::identify_local_file_name(wchar_t * file_path)
{
	wchar_t * next_to_last_slashPosition = file_path;
	wchar_t slash = L'/';
	wchar_t back_slash = L'\\';
	for(int i = 1; file_path[i] != L'\0'; i++)
	{
		if(file_path[i] == slash || file_path[i] == back_slash)
			next_to_last_slashPosition = &file_path[i+1];
	}
	return next_to_last_slashPosition; // pointer to the charecters after the last slash
									   // or inputed string if didn't find any.
}
void LogFileWriter::write_data_in_log(wchar_t * log_file_name)
{
	std::wofstream mylog;
	if(log_file_name == nullptr || wcslen(log_file_name) < 1)
		log_file_name = L"ShellExtLog.log";
	using std::endl;
	{ // in this block we create log file path string and free it
	size_t path_lenth = identify_local_file_name(files_name_[0]) - files_name_[0]; // number of characters to final directory
	size_t log_file_name_lenth = wcslen(log_file_name) + 1; // + 1 for ending zero;
	size_t log_path_lenth = path_lenth + log_file_name_lenth;
	wchar_t * log_path = new wchar_t[log_path_lenth];
	// copy directory location for log saving path
	wcsncpy_s(log_path, log_path_lenth, files_name_[0], path_lenth + 1); //+ 1 for the last character
	// copy file name for log
	wcsncpy_s(&log_path[path_lenth], log_file_name_lenth, log_file_name, log_file_name_lenth);
	// create log file
	mylog.open (log_path);
	delete[] log_path; // free memory for log file name;
	}
	if (mylog.fail())
    {
        // File failed to open
        // leave in silence
		return;
    }
	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	mylog.imbue(utf8_locale);
	mylog << L"Log file generated by shell extension." << endl;
	for(unsigned int i = 0; i < files_count_; i++)
	{
		FileProperties fileProperties = FileProperties(files_name_[i]);
		// write file name in log
		mylog << identify_local_file_name(files_name_[i]) << L" | ";
		// write file size in log
		fileProperties.WriteFileSize(mylog);
		mylog << L" | ";
		// write file creating time in log
		fileProperties.WriteFileCreatingTime(mylog);
		mylog << endl;
	}
	// close log file
	mylog.close();
}
void LogFileWriter::CreateAndWrite(wchar_t * log_file_name)
{
	sort_files_name_array();
	write_data_in_log(log_file_name);
}
LogFileWriter::~LogFileWriter(void)
{
}
