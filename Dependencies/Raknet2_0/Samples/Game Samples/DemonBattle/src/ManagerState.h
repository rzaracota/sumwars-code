#ifndef __MANAGER_STATE_H
#define __MANAGER_STATE_H

#include <stdio.h>

#include "State.h"

class ManagerState
{
public:
	~ManagerState();
	static inline ManagerState* Instance(void) {return &I;}
	void SaveResolution(int x, int y);
	void LogWrite(const char *format, ...);
	void SetState(State *newState);
	State* GetState(void) {return currentState;}
	void Quit(void) {programQuitByUser=true;}
	bool isQuit(void) {return programQuitByUser;}
	unsigned GetFileSize(const char* file);
	int GetScreenWidth(void) const;
	int GetScreenHeight(void) const;
private:
	ManagerState();
	static ManagerState I;
	FILE *log;
	int resX, resY;
	State *currentState;
	bool programQuitByUser;
};

#define manager ManagerState::Instance()

#endif