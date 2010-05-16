#ifndef __FIREBALL_H
#define __FIREBALL_H

#include "Projectile.h"

class LightSource;

class Fireball : public Projectile
{
public:
	Fireball();
	~Fireball();
	void Render(long time);
	void Update(long time);
	void Start(int Team);
	int GetTeam(void) {return team;}
private:
	LightSource* lightSource;
	int team;
};

#endif
