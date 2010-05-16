#ifndef __HOST_GAME_STATE_H
#define __HOST_GAME_STATE_H

#include "State.h"
#include "TextWidget.h"
#include "TextInputWidget.h"
#include "Bitmap.h"
#include "Demon.h"

struct Packet;

class HostGameState : public State
{
	public:   
		~HostGameState();
		static HostGameState* Instance(void) {return &I;}
		void Init(void);
		void DeInit(void);
		void Render(long time);
		void Update(long time);
		void Load(void);
		void Unload(void);
		void KeyJustPressed(int key);
		void KeyJustReleased(int key);
		void MouseButtonJustPressed(int button);
		void MouseButtonJustReleased(int button);
		bool ProcessPacket(Packet* p, unsigned char packetIdentifier);

	private:
		void StartServer(void);
		HostGameState();
		static HostGameState I;
		TextWidget mainMenu, statusBar;
		Bitmap backdrop;
};

#define hostGameState HostGameState::Instance()

#endif