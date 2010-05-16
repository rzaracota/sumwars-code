#include "ManagerState.h"
#include "InputSystem.h"

#include <Time.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys\stat.h>
#include "..\Common\Include\d3dutil.h"

//#include "MemoryManager.h"

ManagerState ManagerState::I;

ManagerState::ManagerState()
{
	log = 0;
	resX=resY=0;
	currentState=0;
	programQuitByUser=false;
}

ManagerState::~ManagerState()
{
	if (log)
		fclose(log);
}

void ManagerState::LogWrite(const char *format, ...)
{
	char LogString[1000];
	va_list arg_ptr;
	int len;

	if (log==0)
	{
		log = fopen("ErrorLog.txt", "wt");
		_strdate(LogString);
		len = strlen(LogString);
		LogString[len++]=' ';
		_strtime( LogString+len );
		len = strlen(LogString);
		LogString[len++]=' ';
		LogString[len]=0;
	}
		
	if(strlen(format) > sizeof(LogString)-256) return;
	va_start(arg_ptr, format);
	vsprintf(LogString, format, arg_ptr);
	va_end(arg_ptr);
	if (LogString[0]==0)
		return;

	strcat(LogString, "\n");	

	fwrite(LogString, 1, strlen(LogString), log);
}

void ManagerState::SaveResolution(int x, int y)
{
	resX=x;
	resY=y;
	inputSystem->m_posX = x/2;
	inputSystem->m_posY = y/2;
}

void ManagerState::SetState(State *newState)
{
	if (currentState)
		currentState->DeInit();
	currentState=newState;
	currentState->Init();
}

unsigned ManagerState::GetFileSize(const char* file)
{
	struct _stat fileInfo;

	FILE* stream = fopen(file, "rb");
	if (stream != NULL)
	{
		if (_stat(file, &fileInfo)==-1)
		{
			fclose(stream);
			return 0;
		}

		fclose(stream);
		return fileInfo.st_size;
	}

	return 0;
}

int ManagerState::GetScreenWidth(void) const
{
	return m_d3dsdBackBuffer.Width;
}

int ManagerState::GetScreenHeight(void) const
{
	return m_d3dsdBackBuffer.Height;
}