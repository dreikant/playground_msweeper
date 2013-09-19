#include <windows.h>
#include <stdlib.h>

#include "Detours.h"

#define _DEBUG_LOG    TRUE
#include "LogFile.h"

#include "bf2Defines.h"

#include "EngineFunctions.h"

#include "ConsoleOutput.h"


DWORD CreateDetours()
{
	// init hooks.
	//CLogFile logFile = CLogFile("log.txt",true);
	//logFile.Write("Find...");
	//			logFile.Write("Find...");

	CConsole console = CConsole();
	printf("Hello from Minesweeper - Press F5 to solve.\n\n");

	while(1)
	{
			CGame* pGame = (CGame*)CGAME_OFFSET;

			if( GetAsyncKeyState(VK_F5) )
			{
				for(int i = 1; i <= pGame->xBoxMac; i++ )
				{
					for(int j = 1; j <= pGame->yBoxMac; j++ )
					{
						if( isBomb(i,j) )
						{
							stepSquare(i,j);
						}
					}
				}
				printf("Solving...\n");
			}
			Sleep(10);
	}


	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpvReserved)
{
    if(dwReason == DLL_PROCESS_ATTACH) { 
		CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CreateDetours,0,0,0);
    } 
    return TRUE;
}