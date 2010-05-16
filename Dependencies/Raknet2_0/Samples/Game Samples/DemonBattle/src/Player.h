#ifndef __PLAYER_H
#define __PLAYER_H

#include "NetworkObject.h"
#include "ArrayList.h"
#include "Perks.h"

enum SpellEnum {FIREBALL_SPELL_INDEX, SUMMON_DEMON_SPELL_INDEX, NUMBER_OF_SPELL_INDICES};

#define NUMBER_OF_PERKS_TO_DRAW 5

class Spell;

// Represents each user's avatar such as current mana and health.
class Player
{
public:
	Player();
	~Player();
	void SetMana(float Mana) {mana=Mana; if (mana>maxMana) mana=maxMana;}
	float GetMana(void) const {return mana;}
	void SetMaxMana(float MaxMana) {maxMana=MaxMana;}
	float GetMaxMana(void) const;

	// Give the player this much experience
	void IncreaseExperience(int value);

	// Did the player gain a level?  This will be true if there are any levels unhandled (see next function)
	bool IsLevelGainPending(void) const;

	// Increase the player's level, but does not affect experience total
	void RaiseLevel(void);

	// Sets your experience such that you will gain a level next time Update is called
	void RaiseExperienceToRequirementForNextLevel(void);

	// Return what level we are
	int GetLevel(void);

	// Fill the output array with up to NUMBER_OF_PERKS_TO_DRAW perks you can still select and return how much of the array was filled.
	int GetUnusedPerks(int output[NUMBER_OF_PERKS_TO_DRAW]);

	// How many perks do we have unhandled?
	int GetRemainingPerks(void) const {return remainingPerks;}

	void SetRemainingPerks(int RemainingPerks) {remainingPerks=RemainingPerks;}

	// Lose some number of perks
	void LosePerks(int numberOfPerksToLose, unsigned char perksLost[]);

	// Give the player a perk
	// Note this is an internal function that DOES NOT handle instantaneous perk events.
	// You should always call GameState::GivePerkToPlayer instead of this
	void GiveUnhandledEventPerk(int perk);

	// Do we have the specified perk?
	bool HasPerk(int perk) const;
	void SetPerk(int perk,bool b);

	void SubtractSpellManaCost(SpellEnum spellEnum);

	// Can we cast?  (Checks a variety of things)
	bool CanCastSpell(SpellEnum spellEnum);
	// Is there enough mana to cast?  (Checks just that)
	bool HasEnoughManaToCastSpell(SpellEnum spellEnum);
	bool SpellAllowsCasting(SpellEnum spellEnum);
	float GetCastingCost(SpellEnum spellEnum);

	Spell* GetSpell(int index);
	void Init(void);
	void Update(long time);
protected:
	int GetNumberOfUnusedPerks(void);
	int GetNumberOfLosablePerks(void);

	int experience,level, nextLevelGain,remainingPerks;
	bool hasPerk[NUMBER_OF_PERKS];
	float mana,maxMana;
	Spell* spellList[NUMBER_OF_SPELL_INDICES];
	int team;
};

#endif