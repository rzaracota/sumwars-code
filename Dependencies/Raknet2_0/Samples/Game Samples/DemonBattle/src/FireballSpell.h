#ifndef __FIREBALL_SPELL_H
#define __FIREBALL_SPELL_H

#include <D3dx9math.h>
#include "Spell.h"
class Fireball;
class ParticleSystem;

class Player;

class FireballSpell : public Spell
{
public:
	FireballSpell();
	~FireballSpell();
	// Create a fireball at the specified location and direction with the specified power.  Returns the fireball and the particle system generated
	void Cast(D3DXVECTOR3* location, D3DXVECTOR3* direction, Fireball **fireballOut, ParticleSystem **particleSystemOut, int team);
	void Update(long time);
	// Do we want to increase the throw power during the next call to Update?
	// You should update this to the initial value and everytime it changes, or just every frame to be easy
	void SetIncreaseThrowPower(bool b) {increaseThrowPower=b;}
	// Directly modify throw power
	void SetThrowPower(float Power) {throwPower = Power;}
	float GetThrowPower(void) const {return throwPower;}
	bool IsThrowPowerAtMinimum(void);

	// Overrides the base class and checks the condition that we have enough power to cast
	bool ReadyToCast(void);

	// Reset variables
	void Init(Player* Owner);
private:
	float throwPower;
	bool increaseThrowPower;
};

#endif
