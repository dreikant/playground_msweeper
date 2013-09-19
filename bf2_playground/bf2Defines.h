#ifndef _BF2_DEFINES_H
#define _BF2_DEFINES_H

#include <windows.h>

#define CGAME_OFFSET	0x01005330
#define GAME_STEPSQUARE 0x01003512
#define CGAME_GAMEFIELD 0x01005340


class CGame
{
public:
	DWORD cBombStart;
	DWORD xBoxMac;
	DWORD yBoxMac;
	DWORD rgBlk;
};


#endif