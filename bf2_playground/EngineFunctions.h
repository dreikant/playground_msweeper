#ifndef _BF2_ENGINEFUNCS_H
#define _BF2_ENGINEFUNCS_H

//ttypedefs
typedef void (__stdcall* tStepSquare)(int,int);

// function to check if field is bomb.
bool isBomb(int x, int y);

// function to check a field.
void stepSquare(int x, int y);

#endif