#include "SummonDemonSpell.h"
#include "Demon.h"
#include "TerrainMesh.h"
#include "Player.h"

//#include "MemoryManager.h"

static const float CASTING_COST=4.0f;
static const long SUMMON_DELAY=400L; // How long to wait between summons


SummonDemonSpell::SummonDemonSpell()
{
	castingCost=CASTING_COST;
}

SummonDemonSpell::~SummonDemonSpell()
{
}

void SummonDemonSpell::Cast(D3DXVECTOR3& pointOnGround, int team, Demon **demon)
{
	*demon = new Demon;
	
	(*demon)->Load(team,CASTING_COST);

	// Why do we subtract yMin?  So the world matrix stores the opposite value and when it adds them they work out 0
	// The +5 is to fix an error with the model where the feet goes a bit into the ground
	(*demon)->SetLocation(pointOnGround.x, pointOnGround.y - (*demon)->GetYMin(), pointOnGround.z);

	float yMin = (*demon)->GetYMin();
	//currentAnimationIndex

	// Look straight ahead towards the enemy castle
	if (team==1)
		(*demon)->LookAt(D3DXVECTOR3(TEAM_2_CASTLE_POSITION, -(*demon)->GetYMin(), pointOnGround.z));
	else
		(*demon)->LookAt(D3DXVECTOR3(TEAM_1_CASTLE_POSITION, -(*demon)->GetYMin(), pointOnGround.z));

	if (owner->HasPerk(FAST_CASTER))  // Decrease summon time by 25%
		summonDelay=SUMMON_DELAY * 3L / 4L;
	else
		summonDelay=SUMMON_DELAY;
}

void SummonDemonSpell::Init(Player* Owner)
{
	Spell::Init(Owner);

	// Reset variables
	summonDelay=0L;
}

bool SummonDemonSpell::ReadyToCast(void)
{
	return summonDelay <= 0L;
}

void SummonDemonSpell::Update(long time)
{
	if (summonDelay > 0L)
		summonDelay -= time;
}