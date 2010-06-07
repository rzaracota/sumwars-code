#ifndef __MOUSE_CURSOR_H
#define __MOUSE_CURSOR_H

#include "FileLoadedVisibleEntity.h"
#include "ModelContainer.h"

class LightSource;

class MouseCursor : public FileLoadedVisibleEntity
{
public:
	MouseCursor();
	~MouseCursor();
	void Render(long time);
	void Update(long time);
	void Unload(void);
	void Load(int texture); // Pass one of the texture enums defined in ModelEnum.h
	void PlayAttackAnimation(void);

	const float PROJECTION_DISTANCE;
private:
	LightSource* lightSource;
};

#endif
