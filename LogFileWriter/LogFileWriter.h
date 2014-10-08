/****************************** Module Header ******************************\
Module Name:  LogFileWriter.h
Project:      LogFileWriter

This file implements classes for writing selected files meta-data in log file.
It writes in log creating date, size of files received in the constructor
sorted in alphabetical order.

\***************************************************************************/
#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cstdint>
#include "semaphore.h"
#include <Shlwapi.h> 
#include "FileProperties.h"

class LogFileWriter
{
	struct TaskWorkerResults
	{
		uintmax_t fileSize;
		SYSTEMTIME stLocal;
		uint32_t checkSum;
	};
	struct TaskWorkerData
	{
		TaskWorkerData(std::vector<std::wstring> & _filesName);
		std::vector<std::wstring> & filesName;
		std::vector<std::shared_ptr<LogFileWriterSynchronizationPrimitives::Semaphore> > semArray;
		std::vector<TaskWorkerResults> results;

	};
	static void FileInfoTaskWorker(TaskWorkerData &, unsigned int );
	std::vector<std::wstring> m_fileNames;
	void SortFilesNameArray();
	void WriteDataInLog(const std::wstring & logFileName);
	void WriteFileInfo(std::wostream & ,  TaskWorkerResults & res);
	std::wstring getFileName(const std::wstring & file_name);
	std::wstring getLogPath(const std::wstring &file_name);
public:
	void CreateAndWrite(const std::wstring & logFileName);
	explicit LogFileWriter(std::vector<std::wstring> & files_name);
	~LogFileWriter();
};

