#ifndef _ATA_LOGFILE_
#define _ATA_LOGFILE_

#include <windows.h>
#include <stdlib.h>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <stdexcept>
#include <tchar.h>

using namespace std;

/////////////////////////////////////////////////////////////////
//features:	not depends on MFC ore ATL.
//			file name could use absolute path or just the name, in which case the 
//			file will be created at the same place with the binary module, no concern 
//			with curret directory, which always bring me truble.
//			every log line has a time stamp attached, with million seconds.
//			uses printf like format to write log lines
//			uses a preprocessor definition _DEBUG_LOG to switch if write log file
//			multi thread safe, finally added:)

////////////////////////////////////////////////////
//	CLogFile, a debug log file wrapper
class CLogFile
{
public:
	//////////////////////////////
	//	Constructor, open the logfile
	CLogFile(LPCTSTR strFile, bool bAppend = FALSE, long lTruncate = 4096)
	{
		m_lTruncate = lTruncate;
		TCHAR	szFile[MAX_PATH+1];
		if (_tcslen(strFile)>3 && strFile[1]!=':')	//no absolute path designated
		{
			::GetModuleFileName(NULL, szFile, MAX_PATH);
			long llength = _tcslen(szFile);
			TCHAR*	pcat = szFile+(llength - 1);	//point to the last char
			while (llength--)
			{
				pcat--;
				if (*pcat == '\\')
					break;
			}
			if (*pcat == '\\')
			{
				pcat++;
				_tcscpy(pcat, strFile);
			}
			else	//something wrong, use the original filename, ignore path problem
				_tcscpy(szFile, strFile);
		}
		else
			_tcscpy(szFile, strFile);

		m_pLogFile = fopen(szFile, bAppend ? "a" : "w");
		InitializeCriticalSection(&m_cs);
	}

	/////////////////////////////////
	//	Destructor, close if logfile if opened
	~CLogFile()
	{
		if (m_pLogFile)
		{
			fputs("\n===============Finish Loging================\n\n", m_pLogFile);
			fclose(m_pLogFile);
		}
		DeleteCriticalSection(&m_cs);
	}
	/////////////////////////////////
	//	Write log info into the logfile, with printf like parameters support
	void Write(LPCTSTR pszFormat, ...)
	{
		if (!m_pLogFile)
			return;

		EnterCriticalSection(&m_cs);
		//write the formated log string to szLog
		TCHAR	szLog[256];
		va_list argList;
		va_start( argList, pszFormat );
		vsprintf( szLog, pszFormat, argList );
		va_end( argList );

		//Trancate if the file grow too large
		long	lLength = ftell(m_pLogFile);
		if (lLength > m_lTruncate)
			rewind(m_pLogFile);

		//Get current time
		SYSTEMTIME	time;
		::GetLocalTime(&time);
		TCHAR	szLine[256];
		
		sprintf(szLine, "%02d:%02d:%02d:%03d \t%s\n", 
			time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
			szLog);
		
		fputs(szLine, m_pLogFile);

		LeaveCriticalSection(&m_cs);
	}

private:
	FILE*	m_pLogFile;
	long	m_lTruncate;
	CRITICAL_SECTION	m_cs;
};

#endif //_ATA_LOGFILE_