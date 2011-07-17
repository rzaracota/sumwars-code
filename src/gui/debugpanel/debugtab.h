#ifndef DEBUGTAB_H
#define DEBUGTAB_H

#include <CEGUI/CEGUIString.h>

namespace OIS
{
	class Keyboard;
	class Mouse;
}

class DebugTab
{
public:
	virtual void update(OIS::Keyboard *keyboard, OIS::Mouse *mouse) = 0;
};

#endif // DEBUGTAB_H
