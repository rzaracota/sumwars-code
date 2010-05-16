#ifndef __JOIN_GAME_STATE_H
#define __JOIN_GAME_STATE_H

#include "State.h"
#include "FileLoadedVisibleEntity.h"
#include "TextWidget.h"
#include "Bitmap.h"

struct Packet;

class JoinGameState : public State
{
	public:   
		~JoinGameState();
		static JoinGameState* Instance(void) {return &I;}
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
		void ConnectClient(void);
		bool IsDigit(char c);
		bool VerifyIPString(const char* IPString);
		void IPStringToIPDigits(const char* IPString, unsigned char* IP);
		JoinGameState();
		static JoinGameState I;
		TextWidget mainMenu, statusBar;
		char tempConnectionIP[50];
		Bitmap backdrop;
		long connectionAttemptionTimer;
};

#define joinGameState JoinGameState::Instance()

#endif