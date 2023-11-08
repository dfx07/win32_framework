////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2023 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xsyslogger.h
* @create   Nov 15, 2023
* @brief    Defines logger
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XSYSLOGGER_H
#define XSYSLOGGER_H

#include "xsysdef.h"
#include <Windows.h>
#include <memory>
#include <mutex>

___BEGIN_NAMESPACE___

#define MSG_BUFF_LENGTH		2048
#define TIME_BUFF_LENGTH	256

class Dllexport Logger
{
public:
	enum LoggerState
	{
		LoggerError  = 0x0001,
		LoggerWaring = 0x0002,
		LoggerOut	 = 0x0004
	};

	enum LoggerInfo
	{
		LoggerIfTime = 0x0001,
	};

private:

	/*******************************************************************************
	*! @brief  : init file name and max log file
	*! @param    [in] file_name : file name
	*! @param    [in] ztmax_size : size of file
	*! @author : thuong.nv          - [Date] : 08/11/2023
	*******************************************************************************/
	void get_time_local(char* buff, const int length)
	{
		struct tm newtime;
		time_t now = time(0);
		localtime_s(&newtime, &now);

		strftime(buff, length, "<%Y-%m-%d %H:%M:%S>", &newtime);
	}

private:

	/*******************************************************************************
	*! @brief  : init file name and max log file
	*! @param    [in] file_name : file name
	*! @param    [in] ztmax_size : size of file
	*! @author : thuong.nv          - [Date] : 08/11/2023
	*******************************************************************************/
	FILE* open_log()
	{
		FILE* file = NULL;
		file = _wfsopen(m_pathLog, L"ab", _SH_DENYWR);

		if (!file)
		{
			assert(0);
		}

		return file;
	}

	/*******************************************************************************
	*! @brief  : Create backup file
	*! @param    [in] file_name : file name
	*! @param    [in] ztmax_size : size of file
	*! @author : thuong.nv          - [Date] : 08/11/2023
	*******************************************************************************/
	bool create_backup_file()
	{
		wchar_t bakupLog[MAX_PATH];

		memcpy_s(bakupLog, MAX_PATH, m_pathLog, MAX_PATH);

		wchar_t* dot = wcschr(bakupLog, L'.');
		if (!dot || dot == bakupLog)
		{
			assert(0);
			return false;
		}

		const int nExtension = 12;
		wchar_t strext[nExtension];
		memset(strext, 0, nExtension);
		memcpy_s(strext, nExtension, dot + 1, wcslen(dot + 1) * sizeof(wchar_t));

		memset(dot, 0, wcslen(dot) * sizeof(wchar_t));

		const int nDataLength = 64;
		wchar_t strNewDate[nDataLength];
		memset(strNewDate, 0, 12);

		struct tm newtime;
		time_t now = time(0);
		localtime_s(&newtime, &now);
		wcsftime(strNewDate, nDataLength, L"%Y%m%d_%H%M%S", &newtime);

		swprintf(bakupLog, MAX_PATH, L"%s_%s.%s", bakupLog, strNewDate, strext);

		if (_wrename(m_pathLog, bakupLog) != 0){
			assert(0);
			return false;
		}
	
		return true;
	}

	/*******************************************************************************
	*! @brief  : Save log
	*! @param    [in] file_name : file name
	*! @param    [in] ztmax_size : size of file
	*! @author : thuong.nv          - [Date] : 08/11/2023
	*******************************************************************************/
	void save_log(const char* msg, bool badd_endline = true)
	{
		std::unique_lock<std::mutex> lck(m_mutexlog);

log_begin:
		FILE* logfile = open_log();
		if (!logfile)
			return;

		if (m_nMaxLengthLogFile > 0)
		{
			fseek(logfile, 0, SEEK_END);	// seek to end of file
			size_t szfile = ftell(logfile); // get current file pointer
			fseek(logfile, 0, SEEK_SET);	// seek back to beginning of file

			if (szfile > m_nMaxLengthLogFile)
			{
				fclose(logfile);

				if (!create_backup_file())
					return;

				goto log_begin;
			}
		}

		if (badd_endline)
		{
			fprintf_s(logfile, "%s\n", msg);
		}
		else
		{
			fprintf_s(logfile, "%s", msg);
		}

		fclose(logfile);
	}

	/*******************************************************************************
	*! @brief  : init file name and max log file
	*! @param    [in] file_name : file name
	*! @param    [in] ztmax_size : size of file
	*! @author : thuong.nv          - [Date] : 08/11/2023
	*******************************************************************************/
	void write_log(const char* _file, const char* _func, const int _line, const char* msg)
	{
		// meta data segment
		char _metadata[MSG_BUFF_LENGTH];
		memset(_metadata, 0, MSG_BUFF_LENGTH);

		if (m_unitLogInfo & LoggerInfo::LoggerIfTime)
		{
			char tmBuff[TIME_BUFF_LENGTH];
			memset(tmBuff, 0, TIME_BUFF_LENGTH);
			get_time_local(tmBuff, TIME_BUFF_LENGTH);
			snprintf(_metadata, MSG_BUFF_LENGTH, "%s ", tmBuff);
		}

		if (_file)
		{
			const char* pTempFile = NULL;
			pTempFile = strrchr(_file, '\\');
			const char* pStartFile = NULL;

			if (pTempFile)
			{
				pStartFile = pTempFile + 1;
			}

			snprintf(_metadata, MSG_BUFF_LENGTH, "%s[%s]", _metadata, (pStartFile ? pStartFile : _file));
		}

		if (_func && _line >=0)
			snprintf(_metadata, MSG_BUFF_LENGTH, "%s[%s:%d]", _metadata, _func, _line);

		// concate data
		snprintf(_metadata, MSG_BUFF_LENGTH, "%s %s", _metadata, msg);

		save_log(_metadata);
	}

public:
	/*******************************************************************************
	*! @brief  : init file name and max log file
	*! @param    [in] file_name : file name
	*! @param    [in] ztmax_size : size of file
	*! @author : thuong.nv          - [Date] : 08/11/2023
	*******************************************************************************/
	void Init(const wchar_t* file_name, size_t ztmax_size)
	{
		memset(m_pathLog, 0, _MAX_PATH);
		memcpy_s(m_pathLog, _MAX_PATH, file_name, wcslen(file_name)* sizeof(wchar_t));

		m_nMaxLengthLogFile = ztmax_size;
	}

	/*******************************************************************************
	*! @brief  : init file name and max log file
	*! @param    [in] file_name : file name
	*! @param    [in] ztmax_size : size of file
	*! @author : thuong.nv          - [Date] : 08/11/2023
	*! @note   : when format was mulitple byte ... is same
	*******************************************************************************/
	void Write(LoggerState state, const char* _file, const char* _func, const int _line, const wchar_t* format,...)
	{
		if (m_unitLogFlag & state)
			return;

		wchar_t _wmsgout[MSG_BUFF_LENGTH];
		memset(_wmsgout, 0, MSG_BUFF_LENGTH * sizeof(wchar_t));

		va_list argptr;
		va_start(argptr, format);
		vswprintf_s(_wmsgout, MSG_BUFF_LENGTH, format, argptr);
		va_end(argptr);

		char _msgout[MSG_BUFF_LENGTH];
		memset(_msgout, 0, MSG_BUFF_LENGTH * sizeof(char));

		int nbytes = WideCharToMultiByte(CP_UTF8, 0, _wmsgout, static_cast<int>(wcslen(_wmsgout)), _msgout,
			MSG_BUFF_LENGTH, NULL, NULL);

		write_log(_file, _func, _line, _msgout);
	}

	/*******************************************************************************
	*! @brief  : Write log use charector byte
	*! @param    [in] file_name : file name
	*! @param    [in] ztmax_size : size of file
	*! @author : thuong.nv          - [Date] : 08/11/2023
	*! @note   : when format was charactor ... is same
	*******************************************************************************/
	void Write(LoggerState state, const char* _file, const char* _func, const int _line, const char* format, ...)
	{
		if (!(m_unitLogFlag & state))
			return;

		char _msgout[MSG_BUFF_LENGTH];
		memset(_msgout, 0, MSG_BUFF_LENGTH * sizeof(char));

		va_list argptr;
		va_start(argptr, format);
		vsprintf_s(_msgout, MSG_BUFF_LENGTH, format, argptr);
		va_end(argptr);

		write_log(_file, _func, _line, _msgout);
	}

private:
	std::mutex		m_mutexlog;
	wchar_t			m_pathLog[_MAX_PATH];
	size_t			m_nMaxLengthLogFile;

	unsigned int	m_unitLogFlag = 0xffff;
	unsigned int	m_unitLogInfo = 0xffff;
};

#define Logger_Write(log_state, format, ...) \
	Write(log_state, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)


____END_NAMESPACE____

#endif // !XSYSLOGGER_H