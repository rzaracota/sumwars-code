#ifndef __MAIN_MENU_STATE_H
#define __MAIN_MENU_STATE_H

#include "State.h"
#include "BitmapWidget.h"
#include "Bitmap.h"
#include "Text.h"

class MainMenuState : public State
{
	public:   
		~MainMenuState();
		static MainMenuState* Instance(void) {return &I;}
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

	private:
		MainMenuState();
		static MainMenuState I;

		BitmapWidget singlePlayer, hostGame, joinGame, credits, quit, instructions;
		Bitmap backdrop;
		Text version;
};

#define mainMenuState MainMenuState::Instance()

#endif