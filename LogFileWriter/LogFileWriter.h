/****************************** Module Header ******************************\
Module Name:  LogFileWriter.h
Project:      LogFileWriter

Module create or rewrite log file. Write in log metadata(creating date, size)
of files received in the constructor sorted in alphabetical order.

\***************************************************************************/
#pragma once

#include <iostream>
#include "semaphore.h"
#include <Shlwapi.h> 
#include "FileProperties.h"

class LogFileWriter
{
	struct TaskWorkerResults
	{
		boost::uintmax_t fileSize;
		SYSTEMTIME stLocal;
		unsigned __int32 checkSum;
	};
	struct TaskWorkerData
	{
		wchar_t ** filesName;
		LogFileWriterSynchronizationPrimitives::Semaphore * semArray;
		TaskWorkerResults * results;
	};
	static void FileInfoTaskWorker(TaskWorkerData &, unsigned int );
	wchar_t ** files_name_;
	unsigned int files_count_;
	void sort_files_name_array(void);
	void write_data_in_log(wchar_t *);
	wchar_t * identify_local_file_name(wchar_t * ); // file name without path
	void WriteFileInfo(std::wostream & ,  TaskWorkerResults & res);
public:
	void CreateAndWrite(wchar_t *);
	LogFileWriter(wchar_t ** files_name, unsigned int files_count);
	~LogFileWriter(void);
};

