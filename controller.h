#ifndef CONTOLLER_H
#define CONTOLLER_H
#include <tamtypes.h>


void InitPad();
int WaitPadReady(int port, int slot);
u32 GetPad();
#endif // CONTOLLER_H
