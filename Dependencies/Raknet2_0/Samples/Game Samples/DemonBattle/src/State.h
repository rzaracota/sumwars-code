#ifndef __STATE_H
#define __STATE_H

#include <windows.h>
#include <D3d9types.h>
#include "D3DHeaders.h"

#define MOUSE_NOT_OVER_BUTTON_COLOR D3DCOLOR_ARGB(255,200,200,255)
#define MOUSE_OVER_BUTTON_COLOR D3DCOLOR_ARGB(255,255,255,0)
#define MOUSE_PRESSED_BUTTON_COLOR D3DCOLOR_ARGB(255,0,255,0)

struct Packet;

class State
{
public:
	State() {}
	virtual ~State() {}
	virtual void Render(long time)=0;
	virtual void Update(long time)=0;
	virtual void Init(void)=0;
	virtual void DeInit(void)=0;
	virtual void Load(void)=0;
	virtual void KeyJustPressed(int key)=0;
	virtual void KeyJustReleased(int key)=0;
	virtual void MouseButtonJustPressed(int button)=0;
	virtual void MouseButtonJustReleased(int button)=0;
	virtual bool ProcessPacket(Packet* p, unsigned char packetIdentifier);
};

#endif