#ifndef __SPELL_H
#define __SPELL_H

class Player;

class Spell
{
public:
	Spell() {owner=0;}
	virtual ~Spell() {}
	// Note this doesn't account for dynamic perks.  Call Player::GetCastingCost pass this spell to do that
	virtual float GetCastingCost(void) const {return castingCost;}
	virtual void Update(long time) {}
	virtual void Init(Player* Owner);
	// Does the spell allow itself to be casted in its current state?
	virtual bool ReadyToCast(void) {return true;}
protected:
	float castingCost;
	Player* owner;
};

#endif