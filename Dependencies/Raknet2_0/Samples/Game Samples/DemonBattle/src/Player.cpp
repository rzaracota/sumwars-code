#include "Player.h"
#include <assert.h>
#include "FireballSpell.h"
#include "SummonDemonSpell.h"
#include <assert.h>
#include "rand.h"

//#include "MemoryManager.h"

static const float MAX_MANA=30.0f;
static const float MANA_REGENERATION_RATE=.0015625f; // how much mana per ms
static const int BASE_EXP_FOR_LEVEL_GAIN = 21;

Player::Player()
{
	// Do this in the order specified by the enum in the header file
	spellList[FIREBALL_SPELL_INDEX] = new FireballSpell;
	spellList[SUMMON_DEMON_SPELL_INDEX] = new SummonDemonSpell;
}

Player::~Player()
{
	for (int i=0; i < NUMBER_OF_SPELL_INDICES; i++)
		delete spellList[i];
}

bool Player::CanCastSpell(SpellEnum spellEnum)
{
	// Add other conditions as I think of them
	return HasEnoughManaToCastSpell(spellEnum) && SpellAllowsCasting(spellEnum);
}

bool Player::HasEnoughManaToCastSpell(SpellEnum spellEnum)
{
	return mana >= GetCastingCost(spellEnum);
}

bool Player::SpellAllowsCasting(SpellEnum spellEnum)
{
	assert(spellEnum < NUMBER_OF_SPELL_INDICES);
	return spellList[(int)spellEnum]->ReadyToCast();
}

float Player::GetCastingCost(SpellEnum spellEnum)
{
	assert(spellEnum < NUMBER_OF_SPELL_INDICES);

	float modifier=1.0f;
	// High Standards cause demons to do 15% more damage, have 15% better armor, and cost 15% more
	// Increased damage, armor is handled at GameState::CreateDemon
	if (HasPerk(HIGH_STANDARDS) && spellEnum==SUMMON_DEMON_SPELL_INDEX)
		modifier= 1.15f;
	else if (HasPerk(OVERACHIEVER) && spellEnum==FIREBALL_SPELL_INDEX)
		modifier= 1.1f; // Overachiever increases fireball casting cost by 10%
	
	return spellList[(int)spellEnum]->GetCastingCost() * modifier;
}

void Player::SubtractSpellManaCost(SpellEnum spellEnum)
{
	mana -= GetCastingCost(spellEnum);
}

void Player::Init(void)
{
	mana=maxMana=MAX_MANA;
	for (int i=0; i < NUMBER_OF_SPELL_INDICES; i++)
		spellList[i]->Init(this);
	experience=0;
	level=1;
	nextLevelGain = BASE_EXP_FOR_LEVEL_GAIN;
	remainingPerks=0;
	memset(hasPerk, false, sizeof(bool) * NUMBER_OF_PERKS);
}

void Player::Update(long time)
{
//	assert(NUMBER_OF_PERKS - GetNumberOfUnusedPerks() < level);

	for (int i=0; i < NUMBER_OF_SPELL_INDICES; i++)
		spellList[i]->Update(time);

	float modifier;
		
	if (HasPerk(OVERACHIEVER)) // Overachiever perk increases mana recharge by 5%
		modifier=1.05f;
	else
		modifier=1.0f;

	if (HasPerk(MORE_SACRIFICES)) // More sacrifices perk increases mana recharge by 5%
		modifier*=1.05f;

	if (HasPerk(MANA_BURN)) // Mana burn increases recharge rate by 10%
		modifier*=1.1f;

	mana += MANA_REGENERATION_RATE * time * modifier;

	if (mana > GetMaxMana())
		mana=GetMaxMana();
}

Spell* Player::GetSpell(int index)
{
	assert(index < NUMBER_OF_SPELL_INDICES);
	return spellList[index];
}

// Give the player this much experience
void Player::IncreaseExperience(int value)
{
	experience+=value;
}

// Did the player gain a level?
bool Player::IsLevelGainPending(void) const
{
	return experience >= nextLevelGain;
}

void Player::RaiseLevel(void)
{
	level++;
	nextLevelGain += (BASE_EXP_FOR_LEVEL_GAIN / 3 * level); // exp points at 21 35 56 84 119 .. 
	maxMana += MAX_MANA * .1f;
	if (GetNumberOfUnusedPerks() > 0)
		remainingPerks++;
}

void Player::RaiseExperienceToRequirementForNextLevel(void)
{
	experience=nextLevelGain;
}

void Player::GiveUnhandledEventPerk(int perk)
{
	assert(perk < NUMBER_OF_PERKS);
	assert(perk>=0);
	remainingPerks--;
	hasPerk[perk]=true;
}

// Do we have the specified perk?
bool Player::HasPerk(int perk) const
{
	assert(perk < NUMBER_OF_PERKS);
	assert(perk>=0);
	return hasPerk[perk];
}

// Return what level we are
int Player::GetLevel(void)
{
	return level;
}

int Player::GetNumberOfUnusedPerks(void)
{
	int count=0;

	for (int i=0; i < NUMBER_OF_PERKS; i++)
	{
		if (hasPerk[i]==false)
		{
			count++;
		}
	}

	return count;
}

// Fill the output array with up to NUMBER_OF_PERKS_TO_DRAW perks you can still select and return how much of the array was filled.
int Player::GetUnusedPerks(int output[NUMBER_OF_PERKS_TO_DRAW])
{
	int outputIndex=0, numberOfUnusedPerks;

	numberOfUnusedPerks = GetNumberOfUnusedPerks();

	if (numberOfUnusedPerks <= NUMBER_OF_PERKS_TO_DRAW)
	{
		// Just return all the remaining perks
		for (int i=0; i < NUMBER_OF_PERKS; i++)
		{
			if (hasPerk[i]==false)
			{
				output[outputIndex++]=i;
				if (outputIndex==NUMBER_OF_PERKS_TO_DRAW)
					break;
			}
		}
	}
	else // We have more perks available than we want to pick
	{
		// Randomly pick NUMBER_OF_PERKS_TO_DRAW.  This is not the most efficient way to do it but it doesn't matter in this situation
		while (outputIndex < NUMBER_OF_PERKS_TO_DRAW)
		{
			do
			{
				// Pick one
				output[outputIndex] = randomMT() % NUMBER_OF_PERKS;
			} while (hasPerk[output[outputIndex]]==true); // If it is already used pick again

			hasPerk[output[outputIndex]]=true; // Temporarily set to true so we don't pick it twice

			outputIndex++;
		}

		// Set the ones we temporarily set to true to false again
		for (int i=0; i < outputIndex; i++)
			hasPerk[output[i]]=false;

	}

	return outputIndex;
}

float Player::GetMaxMana(void) const
{
	if (HasPerk(MANA_BURN)) // Mana burn makes you only hold half the mana
		return maxMana/2.0f;
	else
		return maxMana;
}

// Lose some number of perks
void Player::LosePerks(int numberOfPerksToLose, unsigned char perksLost[])
{
	assert(numberOfPerksToLose>0);
	int index,perksLostIndex=0;

	int numberOfLosablePerks = GetNumberOfLosablePerks();
	if (numberOfLosablePerks <= numberOfPerksToLose)
	{
		// Just go down the array and set all perks to false
		// Just return all the remaining perks
		for (int i=0; i < NUMBER_OF_PERKS; i++)
		{
			if (perkLosable[i])
			{
				if (hasPerk[i]==true)
				{
					hasPerk[i]=false;
					assert(perksLostIndex < numberOfPerksToLose);
					perksLost[perksLostIndex++]=i;
				}
			}
		}
	}
	else
	{
		// Keep picking among losable perks until we hit the desired amount
		// Randomly pick NUMBER_OF_PERKS_TO_DRAW.  This is not the most efficient way to do it but it doesn't matter in this situation
		while (numberOfPerksToLose>0)
		{
			do
			{
				// Pick one
				index = randomMT() % NUMBER_OF_PERKS;
			} while (hasPerk[index]==false || perkLosable[index]==false); // If we cannot lose this perk because it is not losable or we don't have it pick again

			hasPerk[index]=false;
			assert(perksLostIndex < numberOfPerksToLose);
			perksLost[perksLostIndex++]=index;

			numberOfPerksToLose--;
		}
	}
}

int Player::GetNumberOfLosablePerks(void)
{
	int count=0;

	for (int i=0; i < NUMBER_OF_PERKS; i++)
	{
		if (hasPerk[i]==true && perkLosable[i]==true)
		{
			count++;
		}
	}

	return count;
}

void Player::SetPerk(int perk,bool b)
{
	assert(perk >=0 && perk < NUMBER_OF_PERKS);
	hasPerk[perk]=b;
}