#include "FireballSpell.h"
#include "Fireball.h"
#include "ParticleSystemManager.h"
#include "ParticleSystem.h"
#include "Player.h"
//#include "MemoryManager.h"

static const float MIN_THROW_POWER=50.0f;
static const float MAX_THROW_POWER=250.0f;
static const float TIME_TO_REACH_MAX_THROW_POWER=2000.0f;

FireballSpell::FireballSpell()
{
	castingCost=1.5f;
}

FireballSpell::~FireballSpell()
{
}

void FireballSpell::Cast(D3DXVECTOR3* location, D3DXVECTOR3* direction, Fireball **fireballOut, ParticleSystem **particleSystemOut, int team)
{
	// Create a fireball
	*fireballOut = new Fireball;

	// Start the fireball at the 3d mouse cursor
	(*fireballOut)->SetLocation(location->x, location->y, location->z);

	// Create and set the fireball velocity vector (leave acceleration at default)
	(*fireballOut)->SetVelocity(*direction*throwPower);
	(*fireballOut)->SetAcceleration(D3DXVECTOR3(0.0f,GRAVITY,0.0f));
	(*fireballOut)->Start(team);

	*particleSystemOut = particleSystemManager->GetParticleSystem();
	(*particleSystemOut)->StartParticle(ParticleSystem::g_clrColor[COLOR_YELLOW], ParticleSystem::g_clrColorFade[COLOR_RED], *location, *direction*throwPower, 0.0f);

	// Everytime we cast throwPower is set back to 0 (our arm is tired) :)
	throwPower=0;
	increaseThrowPower=false;
}

void FireballSpell::Update(long time)
{
	// Throw power is constantly increasing if it is under MIN_THROW_POWER or if the caster wants to increase the power
	if (increaseThrowPower || throwPower < MIN_THROW_POWER)
	{
		float modifier;

		// It isn't perfect OOP to have a composite class know about its owner but
		// the only other way to handle this is to assign a team to the spell and use that
		// in a roundabout fashion to get the owner anyway.
		// Or we could make the function not virtual and use different parameters but that breaks OOD even worse
		// Or we could even maintain a variable set externally.  That doesn't break OOP but then the class relies on the external caller
		// to set it back, meaning the class doesn't take care of itself and there are two copies of the same data.  That is bad design.
		if (owner->HasPerk(STRONG_ARMS))
			modifier=1.2f; // Throw speed builds up 20% faster
		else
			modifier=1.0f;

		throwPower+= (float) time * ((MAX_THROW_POWER-MIN_THROW_POWER) / TIME_TO_REACH_MAX_THROW_POWER) * modifier;
		if (throwPower > MAX_THROW_POWER)
			throwPower = MAX_THROW_POWER;

		if (increaseThrowPower==false && throwPower > MIN_THROW_POWER)
			throwPower=MIN_THROW_POWER;
	}
}

void FireballSpell::Init(Player* Owner)
{
	Spell::Init(Owner);

	// Reset variables
	throwPower=MIN_THROW_POWER;
	increaseThrowPower=false;
}

bool FireballSpell::IsThrowPowerAtMinimum(void)
{
	return throwPower==MIN_THROW_POWER;
}

bool FireballSpell::ReadyToCast(void)
{
	return throwPower >= MIN_THROW_POWER;
}