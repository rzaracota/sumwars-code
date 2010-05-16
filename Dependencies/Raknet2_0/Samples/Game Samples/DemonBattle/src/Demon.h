#ifndef __DEMON_H
#define __DEMON_H

#include "Creature.h"
#include "AnimationGenerator.h"
#include "Collidable.h"

class Demon : public Creature
{
public:
	Demon();
	~Demon();

	// Call this ONCE after loading the demon models in model container
	static void LoadSharedAnimations(void);

	// Call this for each demon after calling LoadSharedAnimations
	void Load(int Team, float DeathMana);
	void Unload(void) {}

	void Update(long time);
	void EnterEndGameState(int losingTeam);
	void PlayVictoryAnimation(void);
	void PlayDefeatAnimation(void);
	void StartAttackingEnemyCastle(void);
	bool IsAttackingEnemyCastle(void) const {return creatureState==ATTACKING_ENEMY_CASTLE;}
	void UseIdleAnimation(void);
	void UseRunningAnimation(void);
	void UseAttackAnimation(void);
	void UseDeathAnimation(void);
	void UsePainAnimation(void);
	void Damage(float damage, DamageType damageType);
	void Damage(float damage, DamageType damageType, bool doPainAnim);
	DamageType GetKillerType(void) const {return killerType;}

	// Local system calls this
	bool Die(DamageType dt, bool transmitPacket, bool calledFromNetwork);

	#ifdef _DEBUG
	void Render(long time); // In debug we run asserts in Render
	#endif
	CreatureState GetState(void) const {return creatureState;}
	float GetRunSpeedPerSecond(void); // Return the positive run speed of the demon
	float GetRunSpeed(void); // Return the positive run speed of the demon
	void SetRunSpeed(float RunSpeed) {runSpeed=RunSpeed;}
	 // transmitPacket means was this caused by your own actions?  True then yes, false then another player is the cause.  This only has meaning in multiplayer
	bool SwitchTeams(bool transmitPacket);
	
	static void UpdateSharedAnimations(long time) {sharedWalkTeam1.ComputeMeshData(time); sharedWalkTeam2.ComputeMeshData(time);}

protected:
	void AssignRemoteTexture(void);

	static AnimationGenerator sharedWalkTeam1;
	static AnimationGenerator sharedWalkTeam2;
	AnimationGenerator localAnimation;
	float  runSpeed;
};

#endif
