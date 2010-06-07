#include "Creature.h"
#include <assert.h>
#include "User.h"
#include "RakClientInterface.h"
#include "RakServerInterface.h"
#include "NetworkStructures.h"
#include "DemonBattleTypes.h"

extern RakServerInterface* rakServer;
extern RakClientInterface* rakClient;

//#include "MemoryManager.h"

Creature::Creature()
{
	collidable.SetOwner(this);
}

Creature::~Creature()
{
}

void Creature::Update(long time)
{
	FileLoadedVisibleEntity::Update(time);
}

void Creature::Render(long time)
{
	FileLoadedVisibleEntity::Render(time);
}

void Creature::SetScale(float Scale)
{
	FileLoadedVisibleEntity::SetScale(Scale);
	attackReach*=Scale;
}

bool Creature::SwitchTeams(bool transmitPacket)
{
	if (team==1)
		team=2;
	else
		team=1;

	if ((rakServer->IsActive() || rakClient->IsConnected()) && transmitPacket)
	{ 
		TransmitSetCreatureTeam(GetID(), team);
	}

	// Rotate 180 degrees
	D3DXMATRIX ypr;

	D3DXMatrixRotationY(&ypr, D3DX_PI);
	D3DXMatrixMultiply(&oMatrix, &oMatrix, &ypr); // Rotate the result vector

	return true;
}

void Creature::SetTeam(bool transmitPacket, int Team)
{
	if (Team!=team)
	{
		SwitchTeams(transmitPacket);
	}
}

bool Creature::Die(DamageType dt, bool transmitPacket, bool calledFromNetwork)
{
	// Local system is the authority on creatures dying from your own fireballs.  Local system ignores death from remote fireballs
	// Local system is the authority on creatures dying from your own demons.  Local system ignores death from remote demons
	if (calledFromNetwork==false && (rakServer->IsActive() || rakClient->IsConnected()))
	{ 
		if ((user->GetTeam()==1 && (dt==TEAM2_FIREBALL_DAMAGE || dt==TEAM2_DEMON_DAMAGE)) ||
			(user->GetTeam()==2 && (dt==TEAM1_FIREBALL_DAMAGE || dt==TEAM1_DEMON_DAMAGE)))
			return false;

		if (transmitPacket)
			TransmitKillCreature(GetID(),dt);
	}

	killerType=dt;
	health = 0;
	creatureState = DYING;

	// Remove this demon from the collidables list so we don't do collision checking on him anymore
	collidable.collidablesList.del(collidable.collidablesList.getIndexOf(&collidable));

	return true;
}

void Creature::TransmitKillCreature(ObjectID objectId, DamageType dt)
{
	assert(rakServer->IsActive() || rakClient->IsConnected());

	KillCreatureStruct s;
	s.typeId=ID_KILL_CREATURE;
	s.objectId=objectId;
	s.dt=dt;
	
	if (rakServer->IsActive())
		rakServer->Send((char*)&s,sizeof(KillCreatureStruct), HIGH_PRIORITY, RELIABLE_ORDERED, 0, 255, true, false);
	else
		rakClient->Send((char*)&s,sizeof(KillCreatureStruct), HIGH_PRIORITY, RELIABLE_ORDERED, 0);
}

void Creature::TransmitSetCreatureTeam(ObjectID objectId, unsigned char team)
{
	assert(rakServer->IsActive() || rakClient->IsConnected());

	SetCreatureTeamStruct s;
	s.typeId=ID_SET_CREATURE_TEAM;
	s.objectId=objectId;
	s.team=team;
	
	if (rakServer->IsActive())
		rakServer->Send((char*)&s,sizeof(SetCreatureTeamStruct), HIGH_PRIORITY, RELIABLE_ORDERED, 0, 255, true, false);
	else
		rakClient->Send((char*)&s,sizeof(SetCreatureTeamStruct), HIGH_PRIORITY, RELIABLE_ORDERED,0);
}

void Creature::SetTargetedByAI(long time) {assert(time > 0); targetedByAITimer=time;}
bool Creature::IsTargetedByAI(void) const {return targetedByAITimer>0 || creatureState==DYING || creatureState==DEAD;}
void Creature::SetCounterSummonedByAI(long time) {assert(time > 0); counterSummonedByAITimer=time;}
bool Creature::IsCounterSummonedByAI(void) const {return counterSummonedByAITimer>0 || creatureState==DYING || creatureState==DEAD;}
bool Creature::IsIgnoredByAI(void) const {return IsCounterSummonedByAI() || IsTargetedByAI();}

