#include "EngineFunctions.h"
#include "bf2Defines.h"

bool isBomb(int x, int y)
{
	// See Code below.
	return (*(BYTE*)((y<<5)+x+CGAME_GAMEFIELD) & 0x80) == 0;
}


void stepSquare(int x, int y)
{
	tStepSquare oStepSquare = (tStepSquare)GAME_STEPSQUARE;
	oStepSquare(x,y);
}

/*
             push    _xBoxMac
.text:010036C7                 push    _xBoxMac
.text:010036CD                 call    _Rnd@4          ; Rnd(x)
.text:010036D2                 push    _yBoxMac
.text:010036D8                 mov     esi, eax
.text:010036DA                 inc     esi
.text:010036DB                 call    _Rnd@4          ; Rnd(x)
.text:010036E0                 inc     eax
.text:010036E1                 mov     ecx, eax

####
2 Random Zahlen erstellen(+1)

1. in esi
2. in ecx(noch in eax)
###

.text:010036E3                 shl     ecx, 5

2. yRandom << 5

.text:010036E6                 test    byte ptr _rgBlk[ecx+esi], 80h
OLLY:
010036E6   F68431 40530001 >TEST BYTE PTR DS:[ECX+ESI+1005340],80


3. Test (yRandom<<5)+xRandom+1005340 == 0x80 (BOMBE?)


.text:010036EE                 jnz     short loc_10036C7

.text:010036F0                 shl     eax, 5
.text:010036F3                 lea     eax, _rgBlk[eax+esi]
.text:010036FA                 or      byte ptr [eax], 80h

4. Bombe setzen etc.
.text:010036FD                 dec     _cBombStart
.text:01003703                 jnz     short loc_10036C7
.text:01003705                 mov     ecx, _yBoxMac
.text:0100370B                 imul    ecx, _xBoxMac
.text:01003712                 mov     eax, dword_10056A4
.text:01003717                 sub     ecx, eax        ; int
.text:01003719                 push    edi
.text:0100371A                 mov     _cSec, edi
.text:01003720                 mov     _cBombStart, eax
.text:01003725                 mov     _cBombLeft, eax
.text:0100372A                 mov     _cBoxVisit, edi
.text:01003730                 mov     _cBoxVisitMac, ecx
.text:01003736                 mov     _fStatus, 1
.text:01003740                 call    _UpdateBombCount@4 ; UpdateBombCount(x)
.text:01003745                 push    ebx             ; int
.text:01003746                 call    _AdjustWindow@4 ; AdjustWindow(x)
.text:0100374B                 pop     edi
.text:0100374C                 pop     esi
.text:0100374D                 pop     ebx
.text:0100374E                 retn


*/