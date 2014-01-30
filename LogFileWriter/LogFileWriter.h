/****************************** Module Header ******************************\
Module Name:  LogFileWriter.h
Project:      LogFileWriter

Module create or rewrite log file. Write in log metadata(creating date, size)
of files received in the constructor sorted in alphabetical order.

\***************************************************************************/
#pragma once

#include <iostream>
class LogFileWriter
{
	wchar_t ** files_name_;
	unsigned int files_count_;
	void sort_files_name_array(void);
	void write_data_in_log(wchar_t *);
	wchar_t * identify_local_file_name(wchar_t * ); // file name without path
public:
	void CreateAndWrite(wchar_t *);
	LogFileWriter(wchar_t ** files_name, unsigned int files_count);
	~LogFileWriter(void);
};

