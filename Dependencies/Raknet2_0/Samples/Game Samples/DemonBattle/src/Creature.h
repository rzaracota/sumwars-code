#ifndef __CREATURE_H
#define __CREATURE_H

#include <D3dx9math.h>
#include "FileLoadedVisibleEntity.h"
#include "NetworkObject.h"
#include "Collidable.h"
#include "NetworkTypes.h"

enum CreatureState {IDLE, ATTACKING_ENEMY_CASTLE, RUNNING, FIGHTING, IN_PAIN, IN_PAIN_MOVEMENT_NOT_HALTED, DYING, DEAD, CELEBRATING_VICTORY, CELEBRATING_DEFEAT, BLOCKED_BY_FRIENDLIES};

class Creature : public FileLoadedVisibleEntity, public NetworkObject
{
public:
	virtual ~Creature();

	virtual void Update(long time);
	virtual void Render(long time);

	// Accessors
	virtual float GetHealth(void) const {return health;}
	virtual float GetMaxHealth(void) const {return maxHealth;}
	virtual float GetAttackDamage(long time) const {return attackDamage * time;}
	virtual float GetAttackReach(void) const {return attackReach;}
	virtual int GetTeam(void) const {return team;}
	virtual void SetHealth(float Health) {health = Health;}
	virtual void SetMaxHealth(float MaxHealth) {maxHealth = MaxHealth;}
	virtual void SetAttackDamage(float AttackDamage) {attackDamage = AttackDamage;}
	virtual void SetAttackReach(float AttackReach) {attackReach = AttackReach;}
	virtual void SetTeam(bool transmitPacket, int Team);
	virtual float GetDeathMana(void) const {return deathMana;}
	virtual void SetDeathMana(float DeathMana) {deathMana=DeathMana;}
	virtual float GetArmor(void) const {return armor;} // Direct muliple to modify damage by
	virtual void SetArmor(float Armor) {armor=Armor;} // Direct muliple to modify damage by
	virtual void SetTargetedByAI(long time);
	virtual bool IsTargetedByAI(void) const;
	virtual void SetCounterSummonedByAI(long time);
	virtual bool IsCounterSummonedByAI(void) const;
	virtual bool IsIgnoredByAI(void) const;
	virtual void SetScale(float Scale);
	// Local system calls this
	virtual bool Die(DamageType dt, bool transmitPacket, bool calledFromNetwork);
	 // transmitPacket means was this caused by your own actions?  True then yes, false then another player is the cause.  This only has meaning in multiplayer
	virtual bool SwitchTeams(bool transmitPacket);
protected:
	void TransmitKillCreature(ObjectID objectId, DamageType dt);
	void TransmitSetCreatureTeam(ObjectID objectId, unsigned char team);

	Creature();
	Collidable collidable;
	int team;
	float health, maxHealth, attackDamage, attackReach, deathMana, armor;
	long targetedByAITimer, counterSummonedByAITimer;
	DamageType killerType;
	CreatureState creatureState;
};

#endif