#include "Demon.h"
#include "ModelContainer.h"
#include "ModelEnum.h"
#include "TerrainMesh.h"
#include <assert.h>
#include "TextManager.h"
#include "Collidable.h"
#include "rand.h"
#include "GameState.h"
#include "RakServerInterface.h"
#include "RakClientInterface.h"
#include "User.h"

//#include "MemoryManager.h"

static const float DEMON_MAX_HEALTH=100.0f;
static const float DEMON_ATTACK_DAMAGE=.025f; // multiply this by 1000 to get damage per second
static const float DEMON_ATTACK_REACH=15.0f;
static const float RUN_SPEED=.15f; // units per 1 ms

// The shared walking animations for all demons
AnimationGenerator Demon::sharedWalkTeam1;
AnimationGenerator Demon::sharedWalkTeam2;

Demon::Demon()
{
	health=maxHealth=DEMON_MAX_HEALTH;
	attackDamage=DEMON_ATTACK_DAMAGE;
	attackReach=DEMON_ATTACK_REACH;
	collidable.SetOwner(this);
	targetedByAITimer=0L;
	armor=1.0f;
	counterSummonedByAITimer=0L;
	creatureState = IDLE;
	killerType=UNKNOWN_DAMAGE;
	SetScale(.75f);
	health = DEMON_MAX_HEALTH;
	runSpeed = RUN_SPEED;
}

Demon::~Demon()
{
	Unload();
}

void Demon::LoadSharedAnimations(void)
{
	modelContainer->animations[DEMON_MODEL].RestartAnimation();
	// Copy the base animation data from the 'global' demon
	sharedWalkTeam2=modelContainer->animations[DEMON_MODEL];
	sharedWalkTeam1=sharedWalkTeam2;
	//sharedWalkTeam1.SetAnimation("death0");
	sharedWalkTeam1.SetAnimation("run0");
	sharedWalkTeam2.SetAnimation("run0");
	UpdateSharedAnimations(0L);
}

void Demon::Load(int Team, float DeathMana)
{
	assert(Team==1 || Team==2);
	team = Team;
	deathMana = DeathMana;
	assert(modelContainer->models[DEMON_MODEL].GetTriangleCount());

	// Copy the base animation data from the 'global' demon
	localAnimation=modelContainer->animations[DEMON_MODEL];
	localAnimation.SetMD2Model(&modelContainer->models[DEMON_MODEL]);
	localAnimation.RestartAnimation();

	UseRunningAnimation();
	AssignRemoteTexture();
}

bool Demon::SwitchTeams(bool transmitPacket)
{
	// Returns true if the network allows switching teams
	if (Creature::SwitchTeams(transmitPacket)==false)
		return false;

	AssignRemoteTexture();
	return true;
}

void Demon::AssignRemoteTexture(void)
{
	if (team==1)
		SetRemoteTexture(modelContainer->textures[DEMON_TEXTURE_1]);
	else
		SetRemoteTexture(modelContainer->textures[DEMON_TEXTURE_2]);
}

void Demon::UseIdleAnimation(void)
{
	// This animation is not in synch with shared data so we use our local generator
	remoteAnimation=&localAnimation;
	remoteAnimation->SetAnimation("stand0");
	remoteAnimation->ComputeMeshData(0L);
}

void Demon::UseAttackAnimation(void)
{
	// This animation is not in synch with shared data so we use our local generator
	remoteAnimation=&localAnimation;
	remoteAnimation->SetAnimation("attack0");
	remoteAnimation->ComputeMeshData(0L);
}

void Demon::UseRunningAnimation(void)
{
	assert(team==1 || team==2);

	// Set the pointer to the shared animation data, either walking for team 1 or walking for team 2
	if (team==1)
		remoteAnimation = &sharedWalkTeam1;
	else
		remoteAnimation = &sharedWalkTeam2;

	remoteModel = &modelContainer->models[DEMON_MODEL];

	creatureState = RUNNING;
}

void Demon::UsePainAnimation(void)
{
	// This animation is not in synch with shared data so we use our local generator
	remoteAnimation=&localAnimation;
	remoteAnimation->SetSingleAnimation("pain0", false);
	remoteAnimation->ComputeMeshData(0L);
}

void Demon::UseDeathAnimation(void)
{
	// This animation is not in synch with shared data so we use our local generator
	remoteAnimation=&localAnimation;
	remoteAnimation->SetSingleAnimation("death0", true);
	remoteAnimation->ComputeMeshData(0L);
}

void Demon::Update(long time)
{
	assert(remoteAnimation);
	static int runningAnimationIndex = remoteAnimation->GetIndexOfAnimation("run0");

	assert(creatureState != IDLE);
	assert(creatureState != DEAD); // Shouldn't update dead demon
	FileLoadedVisibleEntity::Update(time);

	targetedByAITimer -= time;
	counterSummonedByAITimer -= time;

	if (remoteAnimation==&localAnimation)
		remoteAnimation->ComputeMeshData(time);

	// Process states that end with the end of the animation for it
	if (creatureState==IN_PAIN || creatureState==IN_PAIN_MOVEMENT_NOT_HALTED)
	{
		if (remoteAnimation->IsSingleAnimationRunning()==false)
			UseRunningAnimation(); // Pain state has ended
		else if (creatureState==IN_PAIN)
			return; // Still in pain, we don't move
		// else still in pain, we do move
	}
	else if (creatureState==DYING)
	{
		if (remoteAnimation->IsSingleAnimationRunning()==false)
			creatureState=DEAD;
		return;
	}
	else if (creatureState==CELEBRATING_VICTORY)
	{
		// Randomly flip between animations.
		if (remoteAnimation->IsSingleAnimationRunning()==false)
		{
			PlayVictoryAnimation();
		}

		return;
	}
	else if (creatureState==CELEBRATING_DEFEAT)
	{
		// Randomly flip between animations.
		if (remoteAnimation->IsSingleAnimationRunning()==false)
		{
			PlayDefeatAnimation();
		}

		return;
	}

	assert(health>0);

	if (creatureState == FIGHTING || creatureState == BLOCKED_BY_FRIENDLIES || creatureState == RUNNING || creatureState==IN_PAIN_MOVEMENT_NOT_HALTED)
	{
		// These are all states in which we look ahead of us for other demons
		BasicDataStructures::List<Demon*> *demonList = gameState->GetDemonList();
		Demon* demon=0, *demonInFront=0;

		for (int i=0; i < (int)demonList->size(); i++)
		{
			demon = (*demonList)[i];
			if (demon==this)
			{
				continue; // Don't count ourselves of course
			}

			float myXMax, myXMin, demonXMax, demonXMin;

			// We use the x and z values for a specific animation keyframe so we have a consistent value that 
			// doesn't change as the demon changes animation.  If we didn't do that the demons would slip past each other and scoot into each other
			// when they changed to a smaller animation
			myXMax = GetX() + GetXMax(runningAnimationIndex);
			myXMin = GetX() + GetXMin(runningAnimationIndex);
			demonXMax = demon->GetX() + demon->GetXMax(runningAnimationIndex);
            demonXMin = demon->GetX() + demon->GetXMin(runningAnimationIndex);

			// If different teams use the longer attack reach values.  Otherwise stick with the body reach values
			if (team != demon->GetTeam())
			{
				if (team==1) // reach is negative
					myXMin-=attackReach;
				else // reach is positive
					myXMax+=attackReach;
				
				if (demon->GetTeam()==1)
					demonXMin-=demon->GetAttackReach();
				else // reach is positive
					demonXMax+=demon->GetAttackReach();
			}

			if (myXMax > demonXMin && myXMin < demonXMax &&
				GetZ()+GetZMax(runningAnimationIndex)*.8f > demon->GetZ() + demon->GetZMin(runningAnimationIndex)*.8f && GetZ()+GetZMin(runningAnimationIndex)*.8f < demon->GetZ() + demon->GetZMax(runningAnimationIndex)*.8f &&
				// To count as a blocker the guy must be in front of you.  This fixes a problem where 2 demons intersect and neither will move
				((team==1 && demon->GetX() < GetX()) || (team==2 && demon->GetX() > GetX()))
				)
			{
				demonInFront = demon;
				if (demonInFront->GetTeam() != team)
					break; // Found a demon on the opposite team.  Stop looking

				// Otherwise this demon is on the same team.  Although we are blocked there might be an enemy demon in range we can hit so keep looking.
			}
		}

		if (demonInFront) // If there is a demon ahead of us
		{
			// If the demon is on the other team then fight
			if (demonInFront->GetTeam() != team)
			{
				// We are not already fighting
				if (creatureState!=FIGHTING)
				{
					// We weren't fighting last frame.  Start the fighting animation and enter the fighting state
					UseAttackAnimation();
					creatureState = FIGHTING;
				}
				
				// Damage the enemy demon ahead of us.  We're done.
				if (team==1)
					demonInFront->Damage(attackDamage * time, TEAM1_DEMON_DAMAGE, false);
				else
					demonInFront->Damage(attackDamage * time, TEAM2_DEMON_DAMAGE, false);

			}
			else // Same team demon
			{
				if (creatureState!=BLOCKED_BY_FRIENDLIES)
				{
					// We are not already blocked
					// We weren't blocked last frame.  Start the idle animation and enter the blocked state
					UseIdleAnimation();
					creatureState = BLOCKED_BY_FRIENDLIES;
				}
			}

			return;
		}
		else if (creatureState!=RUNNING) // Nothing ahead of us.  Start running
			UseRunningAnimation();
	}

	// Run towards the enemy castle.  If in range after the update change to the attack animation
	if (team==1)
	{
		if (oMatrix._41 - GetXMin() > TEAM_2_CASTLE_POSITION+200)
		{
			oMatrix._41 -= runSpeed * (float)time;
			if (oMatrix._41 - GetXMin() < TEAM_2_CASTLE_POSITION+200)
			{
				StartAttackingEnemyCastle();
			}
		} else if (creatureState!=ATTACKING_ENEMY_CASTLE) // Within range, ensure they are attacking the enemy castle
			StartAttackingEnemyCastle();
	}
	else
	{
		if (oMatrix._41 - GetXMax() < TEAM_1_CASTLE_POSITION-200)
		{
			oMatrix._41 += runSpeed * (float)time;
			if (oMatrix._41 - GetXMax() > TEAM_1_CASTLE_POSITION-200)
			{
				StartAttackingEnemyCastle();
			}
		} else if (creatureState!=ATTACKING_ENEMY_CASTLE) // Within range, ensure they are attacking the enemy castle
			StartAttackingEnemyCastle();
	}
}

void Demon::EnterEndGameState(int losingTeam)
{
	if (creatureState != DYING && creatureState != DEAD)
	{
		if (team == losingTeam)
		{
			/*
			state = CELEBRATING_DEFEAT;
			PlayDefeatAnimation();
			*/
			// Just kill the demon
			Die(UNKNOWN_DAMAGE, false, false);
		}
		else
		{
            creatureState = CELEBRATING_VICTORY;
			PlayVictoryAnimation();			
		}
	}
}

void Demon::PlayVictoryAnimation(void)
{
	// This animation is not in synch with shared data so we use our local generator
	remoteAnimation=&localAnimation;
	if (randomMT() % 2 == 0)
		remoteAnimation->SetSingleAnimation("flip0", true);
	else
		remoteAnimation->SetSingleAnimation("taunt0", true);
	remoteAnimation->ComputeMeshData(0L);
}

void Demon::PlayDefeatAnimation(void)
{
	// This animation is not in synch with shared data so we use our local generator
	remoteAnimation=&localAnimation;
	if (randomMT() % 2 == 0)
		remoteAnimation->SetSingleAnimation("salute0", true);
	else
		remoteAnimation->SetSingleAnimation("wave0", true);
	remoteAnimation->ComputeMeshData(0L);
}

#ifdef _DEBUG
void Demon::Render(long time)
{
	assert(creatureState != IDLE);
	assert(creatureState != DEAD); // Shouldn't update dead demon

	FileLoadedVisibleEntity::Render(time);
}
#endif

void Demon::StartAttackingEnemyCastle(void)
{
	if (creatureState==DYING || creatureState==DEAD)
		return;

	UseAttackAnimation();
	creatureState = ATTACKING_ENEMY_CASTLE;
}

bool Demon::Die(DamageType dt, bool transmitPacket, bool calledFromNetwork)
{
	if (creatureState==DYING || creatureState==DEAD)
		return false;

	// Are we allowed to kill this creature by the network code?
	if (Creature::Die(dt, transmitPacket, calledFromNetwork)==false)
	{
		// Nope.  Lets put the creature alive again but with 1 health and ignore the death command
		health=1;
		return false;
	}

	UseDeathAnimation();

	// Everytime a demon dies both players get some experience
	if (dt != UNKNOWN_DAMAGE)
		gameState->IncreasePlayersExperience();

	return true;
}

void Demon::Damage(float damage, DamageType damageType)
{
	// Demons have an expanded version of the virtual damage function so call that one
	Damage(damage, damageType, true);
}

void Demon::Damage(float damage, DamageType damageType, bool doPainAnim)
{
	// Can't damage someone already dead
	if (health <=0)
		return;

	bool haltFromPain=true;

	damage *= armor;

	if ((damageType==TEAM1_FIREBALL_DAMAGE && team==1) ||
		damageType==TEAM2_FIREBALL_DAMAGE && team==2)
	{
		haltFromPain=false;
		damage/=4;
	}

	health -= damage;
	if (health <= 0)
	{
		// Record what kind of damage it was that killed us
		Die(damageType, true, false);
	}
	else if (doPainAnim)
	{
		if (haltFromPain)
			creatureState = IN_PAIN;
		else
			creatureState = IN_PAIN_MOVEMENT_NOT_HALTED;
		UsePainAnimation();
	}
}

float Demon::GetRunSpeedPerSecond(void)
{
	return GetRunSpeed() * 1000.0f;
}

float Demon::GetRunSpeed(void)
{
	if (creatureState==RUNNING || creatureState==IN_PAIN) return runSpeed;
	else return 0.0f;
}
