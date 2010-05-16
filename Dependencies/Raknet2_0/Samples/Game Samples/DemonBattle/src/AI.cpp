#include "AI.h"
#include "rand.h"
#include "GameState.h"
#include "Player.h"
#include "Math.h"
#include "SummonDemonSpell.h"
#include "FireballSpell.h"
#include "TextManager.h"
#include "ArrayList.h"
#include "Demon.h"
#include "ParticleSystem.h"
#include "User.h"
#include <assert.h>

AI AI::I;

//#include "MemoryManager.h"

static const float IDLE_RANGE=1.0f;

void AI::Update(long time)
{
	if (gameState->GetNonUserPlayer()->IsLevelGainPending())
		HandleGainLevel();

	if (currentState==AI_IDLE) // Idle so get a new action
	{
		Player* player = gameState->GetNonUserPlayer();
		D3DXVECTOR3 rayDir, rayOrg;
		AIDecision decision = GetDecision();

		switch (decision)
		{
		case DECISION_NO_ACTION:
			#ifdef _DEBUG
			//textManager->ShowMessage(-1, "DECISION_NO_ACTION");
			#endif
			break;
		case DECISION_RECHARGE_MANA_FOR_SHOOT:
			#ifdef _DEBUG
			//textManager->ShowMessage(-1, "DECISION_RECHARGE_MANA_FOR_SHOOT");
			#endif
			break;
		case DECISION_RECHARGE_MANA_FOR_SUMMON:
			#ifdef _DEBUG
			//textManager->ShowMessage(-1, "DECISION_RECHARGE_MANA_FOR_SUMMON");
			#endif
			break;
		case DECISION_WAIT_TO_SHOOT:
			#ifdef _DEBUG
			//textManager->ShowMessage(-1, "DECISION_WAIT_TO_SHOOT");
			#endif
			break;
		case DECISION_WAIT_TO_SUMMON:
			#ifdef _DEBUG
			//textManager->ShowMessage(-1, "DECISION_WAIT_TO_SUMMON");
			#endif
			break;
		case DECISION_NO_TARGETS_TO_SHOOT:
			#ifdef _DEBUG
			//textManager->ShowMessage(-1, "DECISION_NO_TARGETS_TO_SHOOT");
			#endif
			break;
		case DECISION_SHOOT:
			#ifdef _DEBUG
			//textManager->ShowMessage(-1, "DECISION_SHOOT");
			#endif

			currentState = AI_SHOOTING;
			break;
		case DECISION_SUMMON:
			#ifdef _DEBUG
			//textManager->ShowMessage(-1, "DECISION_SUMMON");
			#endif

			//currentState = AI_SUMMONING;

			// Lets calculate the spawn point as if there were a user who clicked on the screen
			D3DXVECTOR3 pointOnGround;
			POINT ptCursor;
			BasicDataStructures::List<Demon*> *demonList;
			demonList = gameState->GetDemonList();
			float distance, nearestEnemyDemonDistance;
			int bestTargetIndex=-1;
			nearestEnemyDemonDistance = 100000.0f;

			// The chance that we countersummon increases linearly according to distance to the nearest unhandled demon

			// Find the best target among the demons
			for (int i=0; i < (int)demonList->size(); i++)
			{
				if ((*demonList)[i]->GetTeam()==1 && (*demonList)[i]->IsIgnoredByAI()==false)
				{
					distance = (*demonList)[i]->GetX() - TEAM_2_CASTLE_POSITION;
					if (distance<0.0f)
						distance=0.0f;

					if (distance < nearestEnemyDemonDistance)
					{
						nearestEnemyDemonDistance=distance;
						bestTargetIndex = i;
					}
				}
			}

			if ((randomMT() % (unsigned long)(TEAM_1_CASTLE_POSITION - TEAM_2_CASTLE_POSITION)) > (unsigned long) nearestEnemyDemonDistance)
			{
				// Countersummon
				pointOnGround.x = TEAM_2_CASTLE_POSITION;
				pointOnGround.y = 0.0f;
				pointOnGround.z = (*demonList)[bestTargetIndex]->GetZ();

				Demon *newDemon = gameState->CreateDemon(pointOnGround, 2, false);

				// Time to run that far plus time to kill that demon at max health + 100 ms
				(*demonList)[bestTargetIndex]->SetCounterSummonedByAI((long)(distance / newDemon->GetRunSpeed() + (*demonList)[bestTargetIndex]->GetMaxHealth() / newDemon->GetAttackDamage(1) + 100L));
			}
			else
			{
				// Summon at random
				ptCursor.x=randomMT() % manager->GetScreenWidth();
				ptCursor.y=manager->GetScreenHeight(); 
				GetClickPos (rayDir, rayOrg, ptCursor);
				
				// Flip the point and direction around the yz plane
				rayDir.x *= -1.0f;
				rayOrg.x *= -1.0f;

				// Normalize rayDir
				D3DXVec3Normalize(&rayDir, &rayDir);

				// For now the terrain is a plane, so find where on the plane the AI 'clicked'
				if (RayPlaneIntersection(&D3DXPLANE(0,1,0,0), &pointOnGround, &rayOrg, &rayDir))
					gameState->CreateDemon(pointOnGround, user->GetOppositeTeam(), false);
			}

			break;
			
		}
	}

	if (currentState == AI_SHOOTING)
	{
		// Get a pointer to your fireball spell
		FireballSpell *fireballSpell = (FireballSpell *) gameState->GetNonUserPlayer()->GetSpell(FIREBALL_SPELL_INDEX);

		// Get a pointer to the list of demons
		BasicDataStructures::List<Demon*> *demonList;
		demonList = gameState->GetDemonList();
		D3DXVECTOR3 rayDir, rayOrg;
		
		// The shot angle is up at 45 degrees
		rayDir.x = .70710678f; // 1 / sqrt(2)
		rayDir.y = .70710678f;
		rayDir.z = 0.0f;

		#ifdef _DEBUG
		//textManager->ShowMessage(-1, "THROWING FIREBALL");
		#endif

		if (demonList->size()>0L) // Are there any demons?
		{
			float distance, nearestEnemyDemonDistance;
			int bestTargetIndex=-1;
			nearestEnemyDemonDistance = 100000.0f;

			// Find the best target among the demons
			for (int i=0; i < (int)demonList->size(); i++)
			{
				if ((*demonList)[i]->GetTeam()==1 && (*demonList)[i]->IsIgnoredByAI()==false)
				{
					distance = (*demonList)[i]->GetX() - TEAM_2_CASTLE_POSITION;

					if (distance < nearestEnemyDemonDistance)
					{
						nearestEnemyDemonDistance=distance;
						bestTargetIndex = i;
					}
				}
			}

			if (bestTargetIndex==-1)
			{
				// Can't find a target now
				AbortShot(&rayOrg);

				// Fire!
				gameState->CreateFireball(rayOrg, rayDir, user->GetOppositeTeam());
				currentState = AI_IDLE;
			}
			else // Found a demon to shoot at
			{
				// Set the firing origin
				rayOrg.x = TEAM_2_CASTLE_POSITION;
				rayOrg.y = TEAM_2_CASTLE_HEIGHT; // Fire from the ground
				rayOrg.z = (*demonList)[bestTargetIndex]->GetZ(); // Fire along the same line as the demon's position

				float firingPower, timeToTarget;

				/*
				// Adjust the firing line by some margin of diminishing error
				float error = (float)sqrt(randomMT() % 500);
				if (randomMT() % 2 ==0)
					rayOrg.z += error;
				else
					rayOrg.z -= error;
	

				// Amount to miss by (+- 20 percent)
				error = (float) ((40.0f/2.0f) - sqrt(randomMT() % (40*40))) / 100.0f;
				*/

				// Calculate the firing power.
				// The negative on the GetRunSpeedPerSecond is because that function always returns a positive speed, and the demon is travelling towards negative x
				firingPower=CalculateFiringPower((*demonList)[bestTargetIndex]->GetX() + (*demonList)[bestTargetIndex]->GetXMin(), - (*demonList)[bestTargetIndex]->GetRunSpeedPerSecond(), (*demonList)[bestTargetIndex]->GetY(), rayOrg.x, TEAM_2_CASTLE_HEIGHT, &timeToTarget);

			//	textManager->ShowMessage(-1, "Calculated firingPower = %f", firingPower);

				if (fireballSpell->GetThrowPower() >= firingPower) // Do we have enough power to shoot?
				{	
					// Ignore this demon from then on.  The .1 is to ignore it 1/10th of a second longer than the time it takes to normally hit it
					(*demonList)[bestTargetIndex]->SetTargetedByAI((long)((timeToTarget+.1) * 1000.0f));

					// Fire!
					gameState->CreateFireball(rayOrg, rayDir, user->GetOppositeTeam());
					currentState = AI_IDLE;

				}
				else
					fireballSpell->SetIncreaseThrowPower(true);
			}
		}
		else // There are no demons but we are in firing mode
		{
			// Abort the shot
			AbortShot(&rayOrg);

			// Fire!
			gameState->CreateFireball(rayOrg, rayDir, user->GetOppositeTeam());
			currentState = AI_IDLE;
		}
	}
}

void AI::AbortShot(D3DXVECTOR3 *rayOrg)
{
	rayOrg->x = TEAM_2_CASTLE_POSITION;
	rayOrg->y = TEAM_2_CASTLE_HEIGHT; // Fire from the ground
	rayOrg->z=0.0f;
}

// The closer the average enemy demon is to you, the more you want to shoot
// The closer the nearest enemy demon is to you, the more you want to shoot
// The closer the average friendly demon is to your enemy, the more you want to summon
// The closer the nearest friendly demon is to your enemy, the more you want to summon
// The fewer the enemy demons compared to you, the more you want to shoot
// The greater the enemy demons compared to you, the more you want to summon a demon
// The more mana you have, the more you want to summon a demon
// The less mana you have, the more you want to shoot
// If you don't have enough mana to do the desired action, do nothing.
// If you don't have a strong enough desire to either shoot or summmon, just recharge mana.  However, if you are at full mana, always do something.
// If you can't do the desired action but have enough mana to do another action, do the other action
AIDecision AI::GetDecision(void)
{
	float desireToShoot=0.0f, desireToSummon=0.0f, averageEnemyDemonDistance=0.0f, nearestEnemyDemonDistance, averageFriendlyDemonDistance=0.0f, nearestFriendlyDemonDistance, distance, mana, maxMana;
	int demonsOnTeam1=0, demonsOnTeam2=0;
	Player* player = gameState->GetNonUserPlayer();

	// Get a pointer to the list of demons
	BasicDataStructures::List<Demon*> *demonList;
	demonList = gameState->GetDemonList();

	mana = gameState->GetNonUserPlayer()->GetMana();
	maxMana = gameState->GetNonUserPlayer()->GetMaxMana();

    // First of all lets calculate the average and nearest enemy demon distance
	if (demonList->size()>0L)
	{
		nearestEnemyDemonDistance = 100000.0f;
		nearestFriendlyDemonDistance = 100000.0f;

		for (int i=0; i < (int)demonList->size(); i++)
		{
			// Distance from the AI's castle
			distance = (*demonList)[i]->GetX() - TEAM_2_CASTLE_POSITION;

			if ((*demonList)[i]->GetTeam()==1 && (*demonList)[i]->IsIgnoredByAI()==false) // Enemy
			{
				if (distance < nearestEnemyDemonDistance)
				{
					nearestEnemyDemonDistance=distance;

					// If an enemy demon is very close to you, you want to shoot him
			//		if (distance < (TEAM_1_CASTLE_POSITION - TEAM_2_CASTLE_POSITION)/10.0f)
			//			desireToShoot+=3.0f;

				}

				averageEnemyDemonDistance+=distance;
				demonsOnTeam1++;
			}
			else // Friendly
			{
				if (distance < nearestFriendlyDemonDistance)
				{
					nearestFriendlyDemonDistance=distance;
				}

				averageFriendlyDemonDistance+=distance;
				demonsOnTeam2++;
			}
		}

		if (demonsOnTeam1)
		{
			averageEnemyDemonDistance/=demonsOnTeam1;

			// The closer the average enemy demon is to you, the more you want to shoot
			desireToShoot += 1.0f - averageEnemyDemonDistance / (TEAM_1_CASTLE_POSITION - TEAM_2_CASTLE_POSITION);

			// The closer the nearest enemy demon is to you, the more you want to shoot
			desireToShoot += 1.0f - nearestEnemyDemonDistance / (TEAM_1_CASTLE_POSITION - TEAM_2_CASTLE_POSITION);


			// The closer the nearest enemy demon is to you, the more you want to summon
			//desireToSummon += 1.0f - nearestEnemyDemonDistance / (TEAM_1_CASTLE_POSITION - TEAM_2_CASTLE_POSITION);

		}
		
		if (demonsOnTeam2)
		{
			averageFriendlyDemonDistance/=demonsOnTeam2;

			// The closer the average friendly demon is to you, the more you want to summon
			desireToSummon += 1.0f - averageFriendlyDemonDistance / (TEAM_1_CASTLE_POSITION - TEAM_2_CASTLE_POSITION);

			// The closer the nearest friendly demon is to you, the more you want to summon
			desireToSummon += 1.0f - nearestFriendlyDemonDistance / (TEAM_1_CASTLE_POSITION - TEAM_2_CASTLE_POSITION);

			// The closer the average friendly demon is to you, the less you want to shoot
		//	desireToShoot -= 1.0f - averageFriendlyDemonDistance / (TEAM_1_CASTLE_POSITION - TEAM_2_CASTLE_POSITION);

			// The closer the nearest friendly demon is to the enemy castle, the more you want to summon
			//desireToSummon += 1.0f - nearestFriendlyDemonDistance / (TEAM_1_CASTLE_POSITION - TEAM_2_CASTLE_POSITION);

			// The closer the average enemy demon is to the enemy castle, the more you want to summon
			//desireToSummon += 1.0f - nearestFriendlyDemonDistance / (TEAM_1_CASTLE_POSITION - TEAM_2_CASTLE_POSITION);

			// The closer the nearest friendly demon is to the enemy castle, the more you want to shoot
			//desireToSummon += 1.0f - nearestFriendlyDemonDistance / (TEAM_1_CASTLE_POSITION - TEAM_2_CASTLE_POSITION);
		}

		
		if (demonsOnTeam2 > demonsOnTeam1) // AI team is always team 2
		{
            // If you have more demons you want to shoot
			if (demonsOnTeam1)
				desireToShoot += (float)demonsOnTeam2 / (float)demonsOnTeam1;
		}
		else if (demonsOnTeam2)
		{
			// If you have fewer demons you want to summon
			desireToSummon += (float)demonsOnTeam1 / (float)demonsOnTeam2;
		}
	}

	// The more mana you have, the more you want to summon a demon
	// The less mana you have, the more you want to shoot
	if (mana > maxMana / 2)
		desireToSummon += ((mana - maxMana/2) / (maxMana/2)) * 2.0f;
	else
		desireToShoot += ((maxMana/2 - mana) / (maxMana/2)) * 2.0f;

	// If you don't have a strong enough desire to either shoot or summmon, just recharge mana.  However, if you are at full mana, always do something.
	if (desireToShoot < IDLE_RANGE && desireToSummon < IDLE_RANGE) // No strong desire
	{
		if (mana<maxMana) // lets just recharge
			return DECISION_NO_ACTION;
	}

	// If you don't have enough mana to do the desired action, do the other action if it is close enough
	// Bug - this code assumes fireballs are cheaper than summoning.  They are, and I plan to keep it that way,
	// but if I ever change it this code will need to be changed too
	assert(player->GetCastingCost(FIREBALL_SPELL_INDEX) < player->GetCastingCost(SUMMON_DEMON_SPELL_INDEX));

	if (desireToShoot > desireToSummon ||
		(player->HasEnoughManaToCastSpell(SUMMON_DEMON_SPELL_INDEX) == false &&
		player->HasEnoughManaToCastSpell(FIREBALL_SPELL_INDEX) == true &&
		desireToSummon / 2 < desireToShoot)
		)
	{
		// enough mana to shoot
		if (player->HasEnoughManaToCastSpell(FIREBALL_SPELL_INDEX) == false)
			return DECISION_RECHARGE_MANA_FOR_SHOOT;
		else if (player->CanCastSpell(FIREBALL_SPELL_INDEX) == true)
		{
			if (demonsOnTeam1 > 0)
				return DECISION_SHOOT;
			else
				return DECISION_NO_TARGETS_TO_SHOOT;
		}
		else
			return DECISION_WAIT_TO_SHOOT;
	}
	else
	{
		// enough mana to cast
		if (player->HasEnoughManaToCastSpell(SUMMON_DEMON_SPELL_INDEX) == false)
			return DECISION_RECHARGE_MANA_FOR_SUMMON;
		else if (player->CanCastSpell(SUMMON_DEMON_SPELL_INDEX) == true)
			return DECISION_SUMMON;
		else
			return DECISION_WAIT_TO_SUMMON;
	}
}

// How much power to use if firing up at a 45 degree angle at a moving target
// Also returns time to target in seconds
float AI::CalculateFiringPower(float x0target, float velocityTarget, float heightTarget, float x0Fireball, float heightCastle, float *timeToTarget)
{
	assert(x0target >= x0Fireball);

	// Here's the problem:
	// Solve for p:
	//
	// VARIABLES
	// a = gravitational acceleration (negative)
	// x0target = target position
	// velocityTarget = target velocity
	// t = time
	// heightTarget = target height
	// x0Fireball = fireball origin
	// v = sin(45) * p = velocity
	// heightCastle = fireball origin height
	//
	// TARGET EQUATIONS
	// x = x0target + velocityTarget * t
	// y = heightTarget
	//
	// FIREBALL EQUATIONS
	// x = x0Fireball + vt = x0Fireball + sin(45)pt
	// y = heightCastle + vt + .5 * a * t * t = heightCastle + sin(45)pt + .5 * a * t * t
	//
	// Note: We use the same v for both x and y since it's 45 degrees and the cos(45) == sin(45)
	// To solve, plug in x and y from the target into the fireball equation.
	// Solve for p, substitute, solve for t, calculate t, then use that to get p.
	// Note that the inverse won't work (i.e. solve for t, substitute, then the resulting equation is too complex.
	// We cannot solve for p so cannot calculate it)
	//
	// Substitute to remove x and y and get 2 equations from 4
	// x0target + velocityTarget * t = x0Fireball + sin(45)pt
	// heightTarget = heightCastle + sin(45)pt + .5 * a * t * t
	//
	// Solve for p
	// p = (x0target - x0Fireball + velocityTarget * t) / (sin(45) * t)
	// p = (heightTarget - heightCastle - .5 * a * t * t) / (sin(45) * t)
	//
	// Substitute to remove p and get 1 equation from 2.
	// (x0target - x0Fireball + velocityTarget * t) / sin(45) * t = (heightTarget - heightCastle - .5 * a * t * t) / sin(45) * t
	//
	// Solve for t
	// x0target - x0Fireball + velocityTarget * t = heightTarget - heightCastle - .5 * a * t * t
	// .5 * a * t * t + velocityTarget * t + x0target - x0Fireball - heightTarget + heightCastle = 0
	// Use the Quadratic equation with
	// A = .5 * a
	// B = velocityTarget
	// C = x0target - x0Fireball - heightTarget + heightCastle
	// t = (- B +- (B*B - 4 * A * C)^.5) / (2 * A)
	// t = (- velocityTarget +- (velocityTarget*velocityTarget - 4 * (.5 * a) * (x0target - x0Fireball - heightTarget + heightCastle))^.5) / (2 * (.5 * a))
	//
	// Now that we have a value for t, we can go back and plug that into a formula that will give us p.
	// Either one of the two above will do.  Done!
	//*timeToTarget = (- velocityTarget + sqrt(velocityTarget*velocityTarget - 4 * (.5f * GRAVITY) * (x0target - x0Fireball - heightTarget + heightCastle))) / (2 * (.5f * GRAVITY));
	*timeToTarget = (float) (- velocityTarget - (float) sqrt(velocityTarget*velocityTarget - 4.0f * (.5f * GRAVITY) * (x0target - x0Fireball - heightTarget + heightCastle))) / (2.0f * (.5f * GRAVITY));


	assert(*timeToTarget > 0.0f);
	//textManager->ShowMessage(-1, "Time to target = %f", *timeToTarget);

    return (x0target - x0Fireball + velocityTarget * *timeToTarget) / (.70710678f * *timeToTarget);
}

void AI::HandleGainLevel(void)
{
	assert(gameState->GetNonUserPlayer()->IsLevelGainPending());

	// We can gain a level.  Take the level immediately
	gameState->GetNonUserPlayer()->RaiseLevel();

	// Pick a perk randomly
	int perks[NUMBER_OF_PERKS_TO_DRAW], numberOfPerksToChooseFrom;
	numberOfPerksToChooseFrom=gameState->GetNonUserPlayer()->GetUnusedPerks(perks);
	if (numberOfPerksToChooseFrom)
		gameState->GivePerkToPlayer(perks[randomMT() % numberOfPerksToChooseFrom], 2, false);
}