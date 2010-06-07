#ifndef __INSTRUCTIONS_STATE_H
#define __INSTRUCTIONS_STATE_H

#include "State.h"
//#include "TextWidget.h"
#include "Bitmap.h"

class InstructionsState : public State
{
	public:   
		~InstructionsState();
		static InstructionsState* Instance(void) {return &I;}
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
		InstructionsState();
		static InstructionsState I;
//		TextWidget mainMenu;
		Bitmap backdrop;
};

#define instructionsState InstructionsState::Instance()

#endif