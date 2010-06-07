#ifndef __CREDITS_STATE_H
#define __CREDITS_STATE_H

#include "State.h"
#include "TextWidget.h"
#include "Bitmap.h"

class CreditsState : public State
{
	public:   
		~CreditsState();
		static CreditsState* Instance(void) {return &I;}
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
		CreditsState();
		static CreditsState I;
	//	TextWidget mainMenu;
		Bitmap backdrop;
};

#define creditsState CreditsState::Instance()

#endif