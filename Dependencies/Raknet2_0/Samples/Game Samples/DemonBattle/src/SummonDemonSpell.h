#ifndef __SUMMON_DEMON_SPELL_H
#define __SUMMON_DEMON_SPELL_H

#include "Spell.h"
#include <D3dx9math.h>
class Demon;
class Player;
class SummonDemonSpell : public Spell
{
public:
	SummonDemonSpell();
	~SummonDemonSpell();
	// Create a demon at the specified location and direction.  Returns the demon generated
	void Cast(D3DXVECTOR3& pointOnGround, int team, Demon **demon);
	// Reset variables
	void Init(Player* Owner);
	void Update(long time);
	// Overrides the base class and checks the condition that enough time has passed to cast
	bool ReadyToCast(void);
private:
	long summonDelay;
};

#endif
