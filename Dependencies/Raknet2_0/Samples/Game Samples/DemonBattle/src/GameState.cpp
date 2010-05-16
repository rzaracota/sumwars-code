#include "GameState.h"
#include "ManagerState.h"
#include "InputSystem.h"
#include "ModelContainer.h"
#include "MainMenuState.h"
#include "User.h"
#include "RakClientInterface.h"
#include "RakServerInterface.h"
#include "Fireball.h"
#include "AI.h"
#include "..\Common\Include\d3dutil.h"
#include "LightSourceManager.h"
#include "TextManager.h"
#include "LightSource.h"
#include <assert.h>
#include "ModelEnum.h"
#include "Math.h"
#include "ParticleSystemManager.h"
#include "ParticleSystem.h"
#include "Collidable.h"
#include "Player.h"
#include "SummonDemonSpell.h"
#include "FireballSpell.h"
#include "rand.h"
#include "GetTime.h"
#include "SoundContainer.h"
#include "PacketEnumerations.h"
#include "NetworkStructures.h"
#include "Bitstream.h"
#include "DemonBattleTypes.h"

static const float MAX_SUMMON_RANGE=500.0f;

static const int NUM_PARTICLES=1;
static const int PARTICLE_EMIT_COLOR=1;
static const int PARTICLE_FADE_COLOR=2;

static const float CASTLE_HEALTH=500.0f;
static const long TIME_BETWEEN_CASTLE_HEALTH_TRANSIMSSIONS=2000L; // Send out your castle's health every 1 second
static const float FIREBALL_EXPLOSION_RANGE=175.0f;
static const float FIREBALL_EXPLOSION_DAMAGE=200.0f;  // Decreases linearly with distance

static const int NUMBER_OF_PERKS_TO_LOSE_FROM_VICIOUS_HEX=2;

static const char *winMessage = {"You won! Press space to play again or ESC to quit."};
static const char *loseMessage = {"You lost! Press space to play again or ESC to quit."};

extern RakServerInterface* rakServer;
extern RakClientInterface* rakClient;

GameState GameState::I;

GameState::GameState()
{
	castleDoorLight=0;
}

GameState::~GameState()
{
}

void GameState::Load(void)
{
	enemyCastle.SetXFile(&(modelContainer->xModels[CASTLE_MODEL]));

	// Load the backdrop image
	backdrop.Load("Graphic\\JPG\\Backdrops\\GameBackdrop.jpg");

	// Load the level up bitmap widget
	levelUp.Load(0, 0, "Graphic\\JPG\\Buttons\\LevelUpMouseOver.jpg", "Graphic\\JPG\\Buttons\\LevelUpMouseNotOver.jpg");

	// Load the perk text widgets with mostly dummy data
	for (int i=0; i < NUMBER_OF_PERKS_TO_DRAW; i++)
		perkWidgets[i].Load(0,0,12 * user->GetResolutionMultiplier(), "Arial", m_pd3dDevice, MOUSE_NOT_OVER_BUTTON_COLOR, MOUSE_OVER_BUTTON_COLOR, MOUSE_PRESSED_BUTTON_COLOR, "dummytext");

	lightedSphere.Load();

	/*
	// The light in front of the enemy castle door
	castleDoorLight = lightSourceManager->GetLightSource();
	castleDoorLight->lightProperties.Type=D3DLIGHT_SPOT;
	
	castleDoorLight->lightProperties.Diffuse=D3DXCOLOR(.5f, .5f, .5f, 1.0f); // RGBA
	castleDoorLight->lightProperties.Ambient=D3DXCOLOR(.4f, .4f, .4f, .4f); // RGBA
	castleDoorLight->lightProperties.Specular=D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // RGBA

	if (user->GetTeam()==2)
	{
		castleDoorLight->lightProperties.Position=D3DXVECTOR3(TEAM_1_CASTLE_POSITION-200.0f, 200.0f, 0.0f);
		castleDoorLight->lightProperties.Direction=D3DXVECTOR3(1.0f, 0, 0);
	}
	else
	{
		castleDoorLight->lightProperties.Position=D3DXVECTOR3(TEAM_2_CASTLE_POSITION+200.0f, 200.0f, 0.0f);
		castleDoorLight->lightProperties.Direction=D3DXVECTOR3(-1.0f, 0, 0);
	}


	castleDoorLight->lightProperties.Falloff=1.0f;
	castleDoorLight->lightProperties.Theta =D3DX_PI/16.0f;
	castleDoorLight->lightProperties.Phi =D3DX_PI/8.0f;
	castleDoorLight->lightProperties.Range=300.0f;
	castleDoorLight->lightProperties.Attenuation0=1.0f;
	castleDoorLight->lightProperties.Attenuation1=0.0000f;
	castleDoorLight->lightProperties.Attenuation2=.00000f;
	castleDoorLight->Update();
	*/

	HRESULT hr = terrainMesh.Generate();

	centeredMessage.Create(24*user->GetResolutionMultiplier(), "Arial", m_pd3dDevice);
	bottomMessage.Create(12*user->GetResolutionMultiplier(), "Arial", m_pd3dDevice);

	int fontHeight, fontWidth;
	bottomMessage.GetTextDimensions("A", 0,0, &fontWidth, &fontHeight);

	userChatMessageInput.Load(0, 0, 20 * user->GetResolutionMultiplier(), "Arial", m_pd3dDevice, MOUSE_NOT_OVER_BUTTON_COLOR, MOUSE_OVER_BUTTON_COLOR, MOUSE_PRESSED_BUTTON_COLOR, 0);
	userChatMessageInput.SetMaximumTextLength(80);
	chatMessageContainer.Load(5, manager->GetScreenHeight()-fontHeight*2-5, 4, 2000, 12, m_pd3dDevice);

	assert(hr==S_OK);
}

void GameState::Unload(void)
{
	// THis needs to go first
	DeInit();

	backdrop.Free();
	terrainMesh.Destroy();
	levelUp.Unload();
	/*
	if (castleDoorLight)
		lightSourceManager->ReleaseLightSource(castleDoorLight);
	castleDoorLight=0;
	*/
	centeredMessage.Unload();
	bottomMessage.Unload();
	for (int i=0; i < NUMBER_OF_PERKS_TO_DRAW; i++)
		perkWidgets[i].Unload();

	userChatMessageInput.Unload();
	chatMessageContainer.Unload();
	lightedSphere.Unload();

	// In case they resize the screen in a multiplayer game don't let them keep playing with new stats
	rakClient->Disconnect();
	rakServer->Disconnect();
}

void GameState::Init(void)
{
	if (user->GetTeam()==2)
		enemyCastle.SetLocation(TEAM_1_CASTLE_POSITION, 0.0f, 0.0f);
	else
		enemyCastle.SetLocation(TEAM_2_CASTLE_POSITION, 0.0f, 0.0f);

	enemyCastle.LookAt(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// Set the camera position depending on your team
	if (user->GetTeam()==2)
	{
		user->SetCamera(D3DXVECTOR3(-(GROUND_WIDTH/2),175.0f,0), D3DXVECTOR3(0, 100.0f, 0.0f));
	}
	else
	{
		user->SetCamera(D3DXVECTOR3(GROUND_WIDTH/2,175.0f,0), D3DXVECTOR3(0, 100.0f, 0.0f));
	}

	if (rakServer->IsActive() || rakClient->IsConnected())
	{
		char str[300];
		assert(user->userName.GetUserInput());
		strcpy(str, user->userName.GetUserInput());
		strcat(str, ">");
		
		userChatMessageInput.Init();
		userChatMessageInput.SetPrefixText(str);
		userChatMessageInput.StartTextInput();
	}
	lightedSphere.Init();
	DoRestart();
}

void GameState::DoRestart(void)
{
	// Reset local variables
	ResetPlayers();
	ResetCastleHealth();

	AI::Instance()->Init();

	// Prepare our array of random perks
	FillPerksArray();

	/*
	castleDoorLight->Deactivate();
	*/
	currentState = GAME_IN_PROGRESS;

	// Same as NULL since castle health > 0
	lastTransmitSetCastleHealth=-1.0f;
	timeToNextCastleHealthTransmission=TIME_BETWEEN_CASTLE_HEALTH_TRANSIMSSIONS;
	gameMessage[0]=0;
}

void GameState::RestartGame(void)
{
	// Delete all the fireballs and demons
	ClearNetworkObjects();

	// Reset the variables that change when the game restarts
	DoRestart();
}

void GameState::DeInit(void)
{
	// Delete all the fireballs and demons
	ClearNetworkObjects();

	// Delete the chat messages
	if (rakServer->IsActive() || rakClient->IsConnected())
		userChatMessageInput.Deinit();

	// Clear the chat messages
	userChatMessageInput.SetText(0);
	chatMessageContainer.Clear();

	lightedSphere.Deinit();
}

void GameState::ClearNetworkObjects(void)
{
	// Get rid of the demons
	for (unsigned int i=0; i < demonList.size(); i++)
		delete demonList[i];
	demonList.clear();

//	DeactivateFog();

	// Get rid of the fireballs
	for (unsigned int i=0; i < fireballList.size(); i++)
		delete fireballList[i];
	fireballList.clear();

	// Get rid of the fireball particles
	for (unsigned int i=0; i < fireballParticleList.size(); i++)
	{
		particleSystemManager->ReleaseParticleSystem(fireballParticleList[i]);
	}
	fireballParticleList.clear();
}

void GameState::ResetPlayers(void)
{
	players[0].Init(); players[1].Init();
}

void GameState::ResetCastleHealth(void)
{
	castleHealth[0]=CASTLE_HEALTH; castleHealth[1]=CASTLE_HEALTH;
}

void GameState::Render(long time)
{
	unsigned int i;

	// Pause in SP for picking perks
	if (rakClient->IsConnected()==false && rakServer->IsActive()==false && CanChoosePerk())
		time=0L;

	// NOTE: Since we are zbuffering render front to back for the best efficiency

//	textManager->ShowMessage(-1, "IsSingleAnimationRunning = %i", user->mouseCursor.remoteAnimation->IsSingleAnimationRunning());
	
	backdrop.RenderTiled();

	// Render all the demons
	Demon::UpdateSharedAnimations(time);
	for (i=0; i < demonList.size(); i++)
		demonList[i]->Render(time);
	
	lightedSphere.Render(time);

	terrainMesh.Render();

	enemyCastle.Render(time);
	
	// Render all the fireballs
	//for (i=0; i < fireballList.size(); i++)
	//	fireballList[i]->Render(time);

	// Render all the fireball particles
	for (i=0; i < fireballParticleList.size(); i++)
		fireballParticleList[i]->DrawParticles();



	/*
	textManager->ShowMessage(-1, "ThrowPower (AI) = %f", ((FireballSpell*)players[1].GetSpell(FIREBALL_SPELL_INDEX))->GetThrowPower());

	textManager->ShowMessage(-1, "ThrowPower = %f", ((FireballSpell*)players[user->GetTeam()-1].GetSpell(FIREBALL_SPELL_INDEX))->GetThrowPower());
			*/
		/*
	textManager->ShowMessage(-1, "Mana = %f / %f", players[user->GetTeam()-1].GetMana(), players[user->GetTeam()-1].GetMaxMana());
	//textManager->ShowMessage(-1, "Mana (AI) = %f / %f", players[1].GetMana(), players[1].GetMaxMana());	
	textManager->ShowMessage(-1, "Castle 0 health = %f. Castle 1 health = %f", castleHealth[0], castleHealth[1]);
	textManager->ShowMessage(-1, "Player 0 level = %i. Player 1 level = %i", players[0].GetLevel(), players[1].GetLevel());
	*/
/*
	if (demonList.size()>0)
		textManager->ShowMessage(-1, "Demon 0 health = %f", demonList[0]->GetHealth());
	if (demonList.size()>1)
	textManager->ShowMessage(-1, "Demon 1 health = %f", demonList[1]->GetHealth());

	if (demonList.size()>2)
	textManager->ShowMessage(-1, "Demon 2 y = %f", demonList[1]->GetY());
*/

	if (CanLevelUp())
	{
		// Render level up
		levelUp.SetLocation(manager->GetScreenWidth() - levelUp.GetWidth()-1, manager->GetScreenHeight() - levelUp.GetHeight()-1);
		levelUp.Render(time);
	}

	if (CanChoosePerk())
	{
		int y=5;
		// Render perk list
		for (int i=0; i < numberOfPerksToChooseFrom; i++)
		{
			perkWidgets[i].SetLocation(5, y);
			perkWidgets[i].Render(time);
			y+=perkWidgets[i].GetHeight();
		}
	}

	// Render status message on the bottom of the screen
	D3DCOLOR color;
	char message[300];	
	int width, height, x, y;
	if (CanChoosePerk())
	{
		// Render perk list
		for (i=0; i < (unsigned int)numberOfPerksToChooseFrom; i++)
		{
			if (perkWidgets[i].GetMouseStateIndex()==MOUSE_OVER)
			{
				strcpy(message, perkDescriptions[perks[i]]); // Use the text description of the perk
				break;
			}
		}

		if (i==numberOfPerksToChooseFrom)
			strcpy(message, "Pick a perk"); // If the mouse is not over a perk name just use "Pick a perk"

		// Blue
		color = D3DCOLOR_ARGB(255,0,0,255);
	}
	else
	{
		// Write out the game status
		sprintf(message, "Throw Power %.0f. Mana %.0f / %.0f. Castle Health %.0f. Level %i. Enemy Castle Health %.0f.", 
			((FireballSpell*)players[user->GetTeam()-1].GetSpell(FIREBALL_SPELL_INDEX))->GetThrowPower(),
			players[user->GetTeam()-1].GetMana(), players[user->GetTeam()-1].GetMaxMana(),
			castleHealth[user->GetTeam()-1],
			players[user->GetTeam()-1].GetLevel(),
			castleHealth[!(user->GetTeam()-1)]);

		// White
		color = D3DCOLOR_ARGB(255,255,255,255);
	}

	// We have to calculate the text dimensions every frame because the screen may be resized at any time
	bottomMessage.GetTextDimensions((char*)message, 0,0, &width, &height);
	x = (manager->GetScreenWidth() - width) / 2;
	y = manager->GetScreenHeight() - height - 5;
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;

	bottomMessage.Render((char*)message, x, y, color, true);

	// If the game is in a state that requires a message then show it
	if (currentState!=GAME_IN_PROGRESS)
	{
		if (currentState==GAME_WON)
		{
			strcpy(message, winMessage);
			color = D3DCOLOR_ARGB(255,255,0,255);
		}
		else if (currentState==GAME_LOST)
		{
			strcpy(message, loseMessage);
			color = D3DCOLOR_ARGB(255,255,0,0);
		}
		else
		{
			strcpy(message, gameMessage);
			color = D3DCOLOR_ARGB(255,255,255,255);
		}

		// We have to calculate the text dimensions every frame because the screen may be resized at any time
		centeredMessage.GetTextDimensions((char*)message, 0,0, &width, &height);
		x = (manager->GetScreenWidth() - width) / 2;
		y = (manager->GetScreenHeight() - height) / 2;
		if (x < 0)
			x = 0;
		if (y < 0)
			y = 0;

		centeredMessage.Render((char*)message, x, y, color, true);
	}

	if (rakServer->IsActive() || rakClient->IsConnected())
	{
		chatMessageContainer.Render(time);
		if (userChatMessageInput.GetUserInput())
			userChatMessageInput.Render(time);
	}
}

void GameState::Update(long time)
{
	unsigned int i;
	D3DXVECTOR3 rayDir, rayOrg;

	// Pause in SP for picking perks
	if (rakClient->IsConnected()==false && rakServer->IsActive()==false && CanChoosePerk())
		time=0L;

	if ((rakClient->IsConnected() || rakServer->IsActive()) && timeToNextCastleHealthTransmission>=0)
	{
		timeToNextCastleHealthTransmission-=time;
		if (timeToNextCastleHealthTransmission<=0)
			TransmitSetCastleHealth();
	}

	
	if (rakServer->IsActive() || rakClient->IsConnected())
	{
		chatMessageContainer.Update(time);
		userChatMessageInput.Update(time);
	}

	enemyCastle.Update(time);
	UpdateCamera(time);

	float tempX, tempZ;
	PositionLightedSphere(&tempX, &tempZ);
	lightedSphere.SetLocation(tempX, LIGHTED_SPHERE_RADIUS, tempZ);
	lightedSphere.Update(time);

	// Update the players
	players[0].Update(time); players[1].Update(time);

	// Update the widgets
	if (CanLevelUp())
	{
		levelUp.Update(time);
		/*
		if (levelUp.isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
			levelUp.SetMouseOver();
			*/
	}
	if (CanChoosePerk())
	{
		// Render perk list
		for (int i=0; i < numberOfPerksToChooseFrom; i++)
		{
			perkWidgets[i].Update(time);
			/*
			if (perkWidgets[i].isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
				perkWidgets[i].SetMouseOver();
				*/

			if (perkWidgets[i].GetLastMouseStateIndex()==MOUSE_NOT_OVER && perkWidgets[i].GetMouseStateIndex()==MOUSE_OVER)
				directSound->PlaySound(soundContainer->sounds[MOUSE_OVER_WIDGET_SOUND], false);
		}

	}

	// Update all the demons
	for (i=0; i < demonList.size(); i++)
		demonList[i]->Update(time);

	// Remove dead demons
	for (i=0; i < demonList.size();)
	{
		if (demonList[i]->GetState() == DEAD)
		{
			// If this demon was killed by an enemy fireball then give that enemy the demon's death mana
			if (demonList[i]->GetKillerType()==TEAM1_FIREBALL_DAMAGE)
				players[0].SetMana(players[0].GetMana() + demonList[i]->GetDeathMana());
			else if (demonList[i]->GetKillerType()==TEAM2_FIREBALL_DAMAGE)
				players[1].SetMana(players[1].GetMana() + demonList[i]->GetDeathMana());

			delete demonList[i];
			demonList.del(i);
		}
		else
			i++;
	}

	// Get input from the AI if in single player and the game is running
	if (rakClient->IsConnected()==false && rakServer->IsActive()==false && currentState==GAME_IN_PROGRESS)
		AI::Instance()->Update(time);

	// Have all demons do damage to the castle if the game is running
	if (currentState==GAME_IN_PROGRESS)
		for (i=0; i < demonList.size(); i++)
		{
			if (demonList[i]->IsAttackingEnemyCastle()) // If this demon is hitting an enemy castle
			{
				DamageCastle(!(demonList[i]->GetTeam()-1), demonList[i]->GetAttackDamage(time));
			}
		}

	FireballSpell *fireballSpell = (FireballSpell *) players[user->GetTeam()-1].GetSpell(FIREBALL_SPELL_INDEX);

	if (inputSystem->MouseButtonPressed(0))
	{
		/*
		if (fireballSpell->IsThrowPowerAtMinimum() && CanLevelUp() && levelUp.isMouseOver(inputSystem->MouseX(), inputSystem->MouseY())) // If over a widget then let the widget handle it
		{
			levelUp.SetMousePressed();
		}
		else 
		*/if (fireballSpell->IsThrowPowerAtMinimum() && CanChoosePerk())
		{/*
			// Check to see if we are over a perk name
			for (i=0; i < (unsigned int)numberOfPerksToChooseFrom; i++)
				if (perkWidgets[i].isMouseOver(inputSystem->MouseX(), inputSystem->MouseY()))
				{
					perkWidgets[i].SetMousePressed();
					break;
				}
				*/

			// Check to see if we are over a perk name
			for (i=0; i < (unsigned int)numberOfPerksToChooseFrom; i++)
				if (perkWidgets[i].GetMouseStateIndex()==MOUSE_OVER)
					break;

			// We weren't over any of the perks, so increase throw power the same as if we couldn't choose perks
			if (i==numberOfPerksToChooseFrom)
				fireballSpell->SetIncreaseThrowPower(true);
		}
		else
		{
			// Tell the fireball spell to update throw power
			fireballSpell->SetIncreaseThrowPower(true);
		}
	}
	else
		fireballSpell->SetIncreaseThrowPower(false);

	// Update fireballs
	for (i=0; i < fireballList.size(); i++)
	{
	//	textManager->ShowMessage(-1, "Fireball moving at %f, %f, %f", fireballList[i]->oMatrix._41, fireballList[i]->oMatrix._42, fireballList[i]->oMatrix._43);
		fireballList[i]->Update(time);
	}

	// Update fireball particles
	for (i=0; i < fireballParticleList.size();)
	{
		if (fireballParticleList[i]->IsParticlesUpdating())
		{
			fireballParticleList[i]->Update(time / 1000.0f, NUM_PARTICLES, ParticleSystem::g_clrColor[COLOR_RED], ParticleSystem::g_clrColorFade[COLOR_BLUE]);
			i++;
		}
		else
		{
			particleSystemManager->ReleaseParticleSystem(fireballParticleList[i]);
			fireballParticleList.del(i);
		}
	}

	i=0;
	while (i < fireballList.size())
	{
		if ((fireballList[i]->GetY() < terrainMesh.GetHeight(fireballList[i]->GetX(), fireballList[i]->GetZ())) ||
			(fabs(fireballList[i]->GetX()) > GROUND_WIDTH) ||
			(fabs(fireballList[i]->GetZ()) > GROUND_HEIGHT)
			)
		{
			// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			// Do explosion
			// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			// Play explosion sound
			// Too annoying
		// 	directSound->PlaySound(soundContainer->sounds[FIREBALL_EXPLOSION_SOUND], false);

			int j;
			float distance2, damage;
			D3DXVECTOR3 entityLocation, fireballLocation;
			j = 0;
			while (j<(int)Collidable::collidablesList.size())
			{
				entityLocation.x = Collidable::collidablesList[j]->GetOwner()->GetX();
				entityLocation.y = Collidable::collidablesList[j]->GetOwner()->GetY();
				entityLocation.z = Collidable::collidablesList[j]->GetOwner()->GetZ();
				
				fireballLocation.x = fireballList[i]->GetX();
				fireballLocation.y = fireballList[i]->GetY();
				fireballLocation.z = fireballList[i]->GetZ();

				distance2 = (fireballLocation.x - entityLocation.x) * (fireballLocation.x - entityLocation.x) +
					(fireballLocation.y - entityLocation.y) * (fireballLocation.y - entityLocation.y) +
					(fireballLocation.z - entityLocation.z) * (fireballLocation.z - entityLocation.z);

				float fireballExplosionRange=FIREBALL_EXPLOSION_RANGE;
				// Overachiever perk increases explosion range by 10%
				if (players[fireballList[i]->GetTeam()-1].HasPerk(OVERACHIEVER))
					fireballExplosionRange*=1.10f;

				assert(fireballList[i]->GetTeam()-1 < 2);

				if (distance2 < fireballExplosionRange * fireballExplosionRange)
				{
					// Each shot has a 10% chance of causing a team change to each hit target
					if (players[fireballList[i]->GetTeam()-1].HasPerk(INSANE_BABBLE) && randomMT()%10==0)
					{
						// Team change
						// This is qualified as a direct trigger if the fireball belongs to you
						Collidable::collidablesList[j]->GetOwner()->SwitchTeams(fireballList[i]->GetTeam()==user->GetTeam());
					}
					else
					{
						// Damage is the percentage of the distance from the center
						damage = FIREBALL_EXPLOSION_DAMAGE * (1.0f - (float) sqrt(distance2) / fireballExplosionRange);

						// Indigestion perk increases damage by 10%
						if (players[fireballList[i]->GetTeam()-1].HasPerk(INDIGESTION))
							damage*=1.1f;

						// Overacheiver perk increases damage by 10%
						if (players[fireballList[i]->GetTeam()-1].HasPerk(OVERACHIEVER))
							damage*=1.1f;

						DamageType type;
						if (fireballList[i]->GetTeam()==1)
							type=TEAM1_FIREBALL_DAMAGE;
						else
							type=TEAM2_FIREBALL_DAMAGE;
						Collidable::collidablesList[j]->GetOwner()->Damage(damage,type);
					}
				}

				j++;
			}

			delete fireballList[i];
			fireballList.del(i);
		}
		else
			i++;
	}
	
	// If one castle or the other is dead then end the game
	if (currentState==GAME_IN_PROGRESS)
	{
		// Are any castles dead?
		if (castleHealth[0] <= 0 || castleHealth[1] <= 0)
		{
			// NETWORKING NOTE:  AT THIS POINT YOU SHOULD SEND A PACKET TO SYNCH CASTLE HEALTH

			// If your castle is dead then you lose (in case of ties you both lose)
			if (castleHealth[user->GetTeam()-1] <= 0)
			{
				currentState=GAME_LOST;

				// Start the demons crying
				for (i=0; i < demonList.size();i++)
					demonList[i]->EnterEndGameState(user->GetTeam());
			}
			else
			{
				currentState=GAME_WON;
				int loser;
				if (user->GetTeam()==1)
					loser=2;
				else
					loser=1;

				// Start the demons celebrating
				for (i=0; i < demonList.size();i++)
				{
					demonList[i]->EnterEndGameState(loser);
				}
			}
		}
	}
}

void GameState::KeyJustPressed(int key)
{
	if (key==DIK_ESCAPE)
		manager->SetState(mainMenuState);
	else if (userChatMessageInput.GetUserInput()==false && key==DIK_SPACE && (currentState==GAME_LOST || currentState==GAME_WON || currentState==RESTART_WAITING_ON_LOCAL_SYSTEM))
	{
		if (rakClient->IsConnected()==false && rakServer->IsActive()==false)
			RestartGame(); // Single player
		else
		{
			char c=ID_RESTART_GAME;
			if (rakClient->IsConnected())
				rakClient->Send(&c, 1, HIGH_PRIORITY, RELIABLE, 0);
			else if (rakServer->IsActive())
				rakServer->Send(&c, 1, HIGH_PRIORITY, RELIABLE, 0, 255, true, false);
			
			if (currentState==RESTART_WAITING_ON_LOCAL_SYSTEM) // They already indicated they wanted to start
				RestartGame();
			else
			{
				currentState=RESTART_WAITING_ON_REMOTE_SYSTEM; // We are now waiting for them (game restarts from network code)
				strcpy(gameMessage, "Waiting for opponent to restart game.");
			}
		}
	}
	else if (CanChoosePerk())
	{
		for (int i=0; i < numberOfPerksToChooseFrom; i++)
			if (key==DIK_1 + i)
			{
				GivePerkToPlayer(perks[i], user->GetTeam(), false);
				break;
			}
	}
	else if (rakServer->IsActive() || rakClient->IsConnected())
	{
		userChatMessageInput.KeyJustPressed(key);
		if (key==DIK_RETURN)
		{
			TransmitChatMessage((char*)userChatMessageInput.GetUserInput());
			AddChatMessage((char*)user->userName.GetUserInput(), (char*)userChatMessageInput.GetUserInput());
			
			userChatMessageInput.SetText(0);
			userChatMessageInput.StartTextInput();
		}
	}

	/*
	#ifdef _DEBUG
	if (key==DIK_L)
	{
		players[0].IncreaseExperience(10); players[1].IncreaseExperience(10);
	}
	#endif
	*/
}

void GameState::KeyJustReleased(int key)
{
}

void GameState::MouseButtonJustPressed(int button)
{
	// User hit button 1 and mouse is at the bottom of the screen we are allowed to cast a summon demon spell
	if (button==1 && currentState==GAME_IN_PROGRESS && /*inputSystem->MouseY() > manager->GetScreenHeight()-100 &&*/ players[user->GetTeam()-1].CanCastSpell(SUMMON_DEMON_SPELL_INDEX))
	{
		// Get an origin and ray as if they clicked on the bottom of the screen
		D3DXVECTOR3 rayDir, rayOrg, pointOnGround;
		POINT ptCursor;
		ptCursor.x=inputSystem->MouseX(); ptCursor.y=manager->GetScreenHeight(); 
		GetClickPos (rayDir, rayOrg, ptCursor);

		// Normalize rayDir
		D3DXVec3Normalize(&rayDir, &rayDir);

		// For now the terrain is a plane, so find where on the plane you clicked
		if (RayPlaneIntersection(&D3DXPLANE(0,1,0,0), &pointOnGround, &rayOrg, &rayDir))
		{
			/*
			// If they clicked too far out into the battlefield, move the point to the maximum allowable
			// If they clicked inside the castle move the point to the base of the castle
			if (user->GetTeam()==1)
			{
				if (pointOnGround.x < TEAM_1_CASTLE_POSITION - MAX_SUMMON_RANGE)
					pointOnGround.x = TEAM_1_CASTLE_POSITION - MAX_SUMMON_RANGE;
				else if (pointOnGround.x > TEAM_1_CASTLE_POSITION)
					pointOnGround.x = TEAM_1_CASTLE_POSITION;
			}
			else if (user->GetTeam()==2)
			{
				if (pointOnGround.x > TEAM_2_CASTLE_POSITION + MAX_SUMMON_RANGE)
					pointOnGround.x = TEAM_2_CASTLE_POSITION + MAX_SUMMON_RANGE;
				else if (pointOnGround.x < TEAM_2_CASTLE_POSITION)
					pointOnGround.x = TEAM_2_CASTLE_POSITION;
			}
			*/

			CreateDemon(pointOnGround, user->GetTeam(), false);
		}
	}
	
	if (button==2 && CanLevelUp())
	{
		HandleLevelUp();
	}
}

void GameState::MouseButtonJustReleased(int button)
{
	// The user released the mouse button
	if (button==0 && currentState==GAME_IN_PROGRESS)
	{
		bool clickHandled=false;
		// If over a widget then handle that
		if (CanLevelUp() && levelUp.GetMouseStateIndex()==MOUSE_PRESSED)
		{
			HandleLevelUp();
			clickHandled=true;
		}
		else if (CanChoosePerk())
		{
			// Check to see if we are over a perk name
			for (int i=0; i < numberOfPerksToChooseFrom; i++)
				if (perkWidgets[i].GetMouseStateIndex()==MOUSE_PRESSED)
				{
					GivePerkToPlayer(perks[i], user->GetTeam(), false);
					clickHandled=true;
					break;
				}
		}

		// we can cast the fireball spell for the user's team
		if (clickHandled==false && players[user->GetTeam()-1].CanCastSpell(FIREBALL_SPELL_INDEX))
		{
			D3DXVECTOR3 fireballStartPoint, rayDir;
			GetFireballStartPoint(&fireballStartPoint, &rayDir);

			CreateFireball(fireballStartPoint, rayDir, user->GetTeam());
		
			// Play the mouse cursor attack animation
			user->mouseCursor.PlayAttackAnimation();
		}
	}
}

Demon * GameState::CreateDemon(D3DXVECTOR3 pointOnGround, int team, bool calledFromNetwork)
{
	assert(team == 1 || team==2);

	// Clients don't do this unless it was called from the network
	if (rakClient->IsConnected())
	{
		if (calledFromNetwork==false)
		{
			// Just send a request to summon a creature.  The network code will actually create it
			TransmitSummonCreature(pointOnGround, 0, user->GetTeam());
			return 0; // Don't do anything else
		}
		// else this is a reply from the server, so go ahead and make the demon
	}
		
	// Get the spell corresponding to the summon demon spell
	SummonDemonSpell* summonDemonSpell = (SummonDemonSpell *) players[team-1].GetSpell(SUMMON_DEMON_SPELL_INDEX);

	players[team-1].SubtractSpellManaCost(SUMMON_DEMON_SPELL_INDEX);

	// Cast it and get the demon created
	Demon *demon;
	summonDemonSpell->Cast(pointOnGround, team, &demon);
	
	// Apply perks
	if (players[team-1].HasPerk(WELL_FED))
	{
		demon->SetScale(1.5f);
		demon->SetAttackReach(demon->GetAttackReach()*2.0f);
		demon->SetAttackDamage(demon->GetAttackDamage(1)*1.05f);
	}
	if (players[team-1].HasPerk(SHARPENED_CLAWS))
		demon->SetAttackDamage(demon->GetAttackDamage(1)*1.15f);
	if (players[team-1].HasPerk(HASTE))
	{
		 // Run 15% faster, give 15% more mana to enemy on death
		demon->SetRunSpeed(demon->GetRunSpeed()*1.15f);
		demon->SetDeathMana(demon->GetDeathMana()*1.15f);
	}
	if (players[team-1].HasPerk(THICK_HIDES))
	{
		demon->SetArmor(demon->GetArmor()*.8f); // 20% less damage
	}
	if (players[team-1].HasPerk(HIGH_STANDARDS))
	{
		// Demons do 15% more damage, have 15% better armor, cost 15% more
		// Increased cost is handled at the player level
		demon->SetArmor(demon->GetArmor()*.85f);
		demon->SetAttackDamage(demon->GetAttackDamage(1)*1.15f);
	}

	// Add the demon to the list of active demons
	demonList.insert(demon);

	if (team==user->GetTeam())
	{
		directSound->PlaySound(soundContainer->sounds[DEMON_SUMMON_SOUND], false);
	}
			
	// The server always sends a packet when this is called.
	// If it was called by the server, then we send a packet to broadcast
	// If it was called by the network code, then the client requested that one be created, so this is the reply to the request
	if (rakServer->IsActive())
		TransmitSummonCreature(pointOnGround, demon->GetID(), team);

	return demon;

//	textManager->ShowMessage(2000, "Demon y = %f", demon->GetY());
}

Fireball* GameState::CreateFireball(D3DXVECTOR3 rayOrg, D3DXVECTOR3 rayDir, int team)
{
	assert(team == 1 || team==2);

	float throwPower;

	// Get the spell corresponding to the fireball spell
	FireballSpell* fireballSpell = (FireballSpell*) players[team-1].GetSpell(FIREBALL_SPELL_INDEX);

	players[team-1].SubtractSpellManaCost(FIREBALL_SPELL_INDEX);
	throwPower = fireballSpell->GetThrowPower();

	// Pointers to the output
	ParticleSystem *remoteParticleSystem;
	Fireball *fireball;

	// Cast the spell
	fireballSpell->Cast(&rayOrg, &rayDir, &fireball, &remoteParticleSystem, team);

	// Add to the list of fireballs and particle systems
	fireballList.insert(fireball);
	fireballParticleList.insert(remoteParticleSystem);

	if (team==user->GetTeam())
	{
		directSound->PlaySound(soundContainer->sounds[SHOOT_FIREBALL_SOUND], false);

		// Local system is the authority on when fireballs are created. Timestamped
		if (rakServer->IsActive() || rakClient->IsConnected())
			TransmitCreateFireball(rayOrg, rayDir, throwPower);
	}

	return fireball;
}

void GameState::UpdateCamera(long time)
{
	if (inputSystem->KeyPressed(DIK_LEFT)==false && 
		inputSystem->KeyPressed(DIK_RIGHT)==false && 
		inputSystem->KeyPressed(DIK_DOWN)==false && 
		inputSystem->KeyPressed(DIK_UP)==false)
		return;

	float height = user->GetCameraEye().y;
	bool angleModified=false;

	if (inputSystem->KeyPressed(DIK_LEFT))
	{
		// camera._42 is the height of the camera
		height-=time/3.0f;
		if (height<175.0f)
			height=175.0f;
	}
	else if (inputSystem->KeyPressed(DIK_RIGHT))
	{
		height+=time/3.0f;
		if (height>400.0f)
			height=400.0f;
	}

	// Constrain the camera angle
	D3DXVECTOR3 direction;
	user->GetCameraDirection(&direction);
	// Direction is not normalized.  We need both the length and to normalize it
	float length = D3DXVec3Length(&direction);
	direction.y=direction.y/length;
	// Get the angle so we can modify it direction
	double angle = asin(direction.y);

	if (inputSystem->KeyPressed(DIK_DOWN))  // Rotate the y axis
	{
		angle-=time * .0001f;
		angleModified=true;
	}
	else if (inputSystem->KeyPressed(DIK_UP))
	{
		angle+=time * .0001f;
		angleModified=true;
	}

	// Fix the angle if necessary
	if (angle < -D3DX_PI/20.0f)
	{
		angle=-D3DX_PI/20.0f;
		angleModified=true;
	}
	else if (angle > 0)
	{
		angle=0;
		angleModified=true;
	}

	// We directly modify the look at value, then call the function to set the position
	D3DXVECTOR3 vec = user->camera.GetLookatPt();

	if (angleModified)
	{
		direction.y=(float)sin(angle);
		// Put the direction vector back to the same length
		vec.y=user->GetCameraEye().y + direction.y * length;
	}

	user->SetCamera(D3DXVECTOR3(user->GetCameraEye().x, height,user->GetCameraEye().z), vec);
}

// Give both players 1 experience point
void GameState::IncreasePlayersExperience(void)
{
	bool alreadyCanLevelUp = CanLevelUp();
	players[0].IncreaseExperience(1); players[1].IncreaseExperience(1);

	if (alreadyCanLevelUp==false && CanLevelUp()) // You can level up where before you couldn't
		directSound->PlaySound(soundContainer->sounds[GAIN_LEVEL_SOUND], false);
}

bool GameState::CanLevelUp(void)
{
	return currentState==GAME_IN_PROGRESS && players[user->GetTeam()-1].IsLevelGainPending() && players[user->GetTeam()-1].GetRemainingPerks()==0;
}

bool GameState::CanChoosePerk(void)
{
	return currentState==GAME_IN_PROGRESS && players[user->GetTeam()-1].GetRemainingPerks()>0;
}

void GameState::HandleLevelUp(void)
{
	assert(CanLevelUp());
	
	// Raise the level, which allocates a perk point
	gameState->players[user->GetTeam()-1].RaiseLevel();
}

void GameState::FillPerksArray(void)
{
	char str[200];
	int i;

	// Fill up the perks array with some random perks
	numberOfPerksToChooseFrom=players[user->GetTeam()-1].GetUnusedPerks(perks);

	// Set the perk text names
	for (i=0; i < numberOfPerksToChooseFrom; i++)
	{
		sprintf(str, "%i. ", i+1);
		strcat(str, perkNames[perks[i]]);
		perkWidgets[i].SetText(str);
	}
}

void GameState::GivePerkToPlayer(int perk, int team, bool calledFromNetwork)
{
	int i;
	unsigned char perksLost[NUMBER_OF_PERKS_TO_LOSE_FROM_VICIOUS_HEX];
	assert(team==1 || team==2);

	if (team==user->GetTeam())
	{
		// Once we choose a perk, choose our next group of random perks
		FillPerksArray();
		directSound->PlaySound(soundContainer->sounds[SELECTED_PERK_SOUND], false);
	}

	players[team-1].GiveUnhandledEventPerk(perk);

	// Handle instaneous effect perks
	if (perk==EARTH_ELEMENTAL)
	{
		// 10% of max castle health to damage for the opposite team
		DamageCastle(!(team-1), CASTLE_HEALTH/10.0f);
	}
	else if (perk==FIRE_ELEMENTAL)
	{
		// Immediately kill all enemy demons
		for (i=0; i < (int)demonList.size(); i++)
			if (demonList[i]->GetTeam() != team)
				demonList[i]->Die(UNKNOWN_DAMAGE, false, false); // Not necessary to send a packet for each demon dying
	}
	else if (perk==WATER_ELEMENTAL)
	{
		// Immediate full mana
		players[team-1].SetMana(players[team-1].GetMaxMana());
	}
	else if (perk==AIR_ELEMENTAL && calledFromNetwork==false) // Switch teams sends the packet and switches the team manually so don't do it again here
	{
		// Half the enemy demons join your team
		for (i=0; i < (int)demonList.size(); i++)
			if (demonList[i]->GetTeam() != team && (randomMT() % 2 == 0))
				demonList[i]->SwitchTeams(true); // This function is called only for the local player and from the network code, so is always a direct trigger
	}
	else if (perk==VICIOUS_HEX && calledFromNetwork==false) // Network code takes away the perks manually
	{
		// Enemy loses two perks
		players[!(team-1)].LosePerks(NUMBER_OF_PERKS_TO_LOSE_FROM_VICIOUS_HEX, perksLost);
	}
	else if (perk==UNHOLY_PACT)
	{
		// +3 perks, castle has 1 hp
		castleHealth[team-1]=1;
        players[team-1].SetRemainingPerks(players[team-1].GetRemainingPerks()+3);
	}

	if (team==user->GetTeam() && (rakServer->IsActive() || rakClient->IsConnected()) && calledFromNetwork==false)
	{
		// This is a trigger so send it out over the network
		TransmitChoosePerk(perk,perksLost[0],perksLost[1]);
	}
}

void GameState::DamageCastle(int index, float damage)
{	
	assert(index==0 || index==1);

	if (castleHealth[index]==0)
		return;

	float modifier;
	// Sloppy imps affect both castles
	if (players[0].HasPerk(SLOPPY_IMPS))
		modifier=2.0f;
	else
		modifier=1.0f;

	// Sloppy imps can stack!
	if (players[1].HasPerk(SLOPPY_IMPS))
		modifier*=2.0f;

	castleHealth[index]-=damage * modifier;

	if (castleHealth[index] < 0)
	{
		castleHealth[index]=0;
		if (index==user->GetTeam()-1 && (rakServer->IsActive() || rakClient->IsConnected())) // Your castle was destroyed so transmit it
			timeToNextCastleHealthTransmission=0L;
	}

	// Local systems ignore death to the enemy castle
	if ((rakServer->IsActive() || rakClient->IsConnected()) && 
		castleHealth[!(user->GetTeam()-1)]<=0)
		castleHealth[!(user->GetTeam()-1)]=1; // We think they should be dead. Set to 1 health

	if (castleHealth[!(user->GetTeam()-1)] == 0)
		directSound->PlaySound(soundContainer->sounds[ENEMY_LOSES_SOUND], false);
}

void GameState::SetCastleHealth(int index, float health)
{
	assert(index >=0 && index <=1);
	castleHealth[index]=health;

	if (castleHealth[!(user->GetTeam()-1)] <= 0)
		directSound->PlaySound(soundContainer->sounds[ENEMY_LOSES_SOUND], false);
}

bool GameState::ProcessPacket(Packet* p, unsigned char packetIdentifier)
{
	switch (packetIdentifier)
	{
		case ID_DISCONNECTION_NOTIFICATION:
			strcpy(gameMessage, "Opponent has quit.");
			currentState=CONNECTION_LOST;
			break;
		case ID_KICKED_BY_SERVER:
			strcpy(gameMessage, "You have been kicked out of the server.");
			currentState=CONNECTION_LOST;
			break;
		case ID_MODIFIED_PACKET:
			strcpy(gameMessage, "Opponent is cheating.  Disconnecting");
			currentState=CONNECTION_LOST;
			break;
		case ID_CONNECTION_LOST:
			strcpy(gameMessage, "Connection lost to opponent.");
			currentState=CONNECTION_LOST;
			break;
		case ID_SUMMON_CREATURE:
			#ifdef _DEBUG
			manager->LogWrite("ID_SUMMON_CREATURE.");
			#endif
			ReceiveSummonCreature(p);
			break;
		case ID_KILL_CREATURE:
			#ifdef _DEBUG
			manager->LogWrite("ID_KILL_CREATURE.");
			#endif
			ReceiveKillCreature(p);
			break;
		case ID_SET_CREATURE_TEAM:
			#ifdef _DEBUG
			manager->LogWrite("ID_SET_CREATURE_TEAM.");
			#endif
			ReceiveSetCreatureTeam(p);
			break;
		case ID_CHOOSE_PERK:
			#ifdef _DEBUG
			manager->LogWrite("ID_CHOOSE_PERK.");
			#endif
			ReceiveChoosePerk(p);
			break;
		case ID_CREATE_FIREBALL:
			#ifdef _DEBUG
			manager->LogWrite("ID_CREATE_FIREBALL.");
			#endif
			ReceiveCreateFireball(p);
			break;
		case ID_SET_CASTLE_HEALTH:
			#ifdef _DEBUG
			manager->LogWrite("ID_SET_CASTLE_HEALTH.");
			#endif
			ReceiveSetCastleHealth(p);
			break;
		case ID_CHAT_MESSAGE:
			#ifdef _DEBUG
			manager->LogWrite("ID_CHAT_MESSAGE.");
			#endif
			ReceiveChatMessage(p);
			break;
		case ID_RESTART_GAME:
			#ifdef _DEBUG
			manager->LogWrite("ID_RESTART_GAME.");
			#endif
			if (currentState==RESTART_WAITING_ON_REMOTE_SYSTEM)
				RestartGame();
			else
			{
				currentState=RESTART_WAITING_ON_LOCAL_SYSTEM;
				strcpy(gameMessage, "Opponent is waiting for new game.  Hit space to restart.");
			}
			break;
		default:
			manager->LogWrite("Unknown packet type in GameState::ProcessPacket.  Type %i with length %i", packetIdentifier, p->length);
			assert(1);
			break;
	}

	return true;
}

void GameState::TransmitSetCastleHealth(void)
{
	assert(rakServer->IsActive() || rakClient->IsConnected());

	if (lastTransmitSetCastleHealth!=castleHealth[user->GetTeam()-1])
	{
		// The next transmission time is less because the castle health is probably continually changing so we
		// want more frequent updates
		timeToNextCastleHealthTransmission=TIME_BETWEEN_CASTLE_HEALTH_TRANSIMSSIONS/4;
	}
	else
		timeToNextCastleHealthTransmission=TIME_BETWEEN_CASTLE_HEALTH_TRANSIMSSIONS;

	// The castle health is different from what we sent last time, so send it now
	SetCastleHealthStruct s;
	s.typeId=ID_SET_CASTLE_HEALTH;
	s.health=castleHealth[user->GetTeam()-1];

	lastTransmitSetCastleHealth=castleHealth[user->GetTeam()-1];
	if (castleHealth[user->GetTeam()-1] <=0)
		timeToNextCastleHealthTransmission=-1; // Stop transmitting
	
	if (rakServer->IsActive())
		rakServer->Send((char*)&s,sizeof(SetCastleHealthStruct), HIGH_PRIORITY, RELIABLE_ORDERED, 0, 255, true, false);
	else
		rakClient->Send((char*)&s,sizeof(SetCastleHealthStruct), HIGH_PRIORITY, RELIABLE_ORDERED, 0);

}

void GameState::TransmitSummonCreature(D3DXVECTOR3 pointOnGround, ObjectID objectId, unsigned char team)
{
	assert(rakServer->IsActive() || rakClient->IsConnected());

	SummonCreatureStruct s;
	s.useTimestamp=ID_TIMESTAMP;
	s.timeStamp=getTime();
	s.typeId=ID_SUMMON_CREATURE;
	s.pointOnGround=pointOnGround;
	s.objectId=objectId;
	s.team=team;
	
	if (rakServer->IsActive())
		rakServer->Send((char*)&s,sizeof(SummonCreatureStruct), HIGH_PRIORITY, RELIABLE_ORDERED, 0, 255, true, false);
	else
		rakClient->Send((char*)&s,sizeof(SummonCreatureStruct), HIGH_PRIORITY, RELIABLE_ORDERED,0);
}

void GameState::TransmitCreateFireball(D3DXVECTOR3 rayOrg, D3DXVECTOR3 rayDir, float throwPower)
{
	assert(rakServer->IsActive() || rakClient->IsConnected());

	CreateFireballStruct s;
	s.useTimestamp=ID_TIMESTAMP;
	s.timeStamp=getTime();
	s.typeId=ID_CREATE_FIREBALL;
	s.rayOrg=rayOrg;
	s.rayDir=rayDir;
	s.throwPower=throwPower;
	
	if (rakServer->IsActive())
		rakServer->Send((char*)&s,sizeof(CreateFireballStruct), HIGH_PRIORITY, RELIABLE_ORDERED, 0, 255, true, false);
	else
		rakClient->Send((char*)&s,sizeof(CreateFireballStruct), HIGH_PRIORITY, RELIABLE_ORDERED,0);
}

void GameState::TransmitChoosePerk(unsigned char perk, unsigned char additionalData1, unsigned char additionalData2)
{
	assert(rakServer->IsActive() || rakClient->IsConnected());

	ChoosePerkStruct s;
	s.typeId=ID_CHOOSE_PERK;
	s.perk=perk;
	s.additionalData1=additionalData1;
	s.additionalData2=additionalData2;

	if (rakServer->IsActive())
		rakServer->Send((char*)&s,sizeof(ChoosePerkStruct), HIGH_PRIORITY, RELIABLE_ORDERED, 0, 255, true, false);
	else
		rakClient->Send((char*)&s,sizeof(ChoosePerkStruct), HIGH_PRIORITY, RELIABLE_ORDERED,0);
}

void GameState::TransmitChatMessage(const char* msg)
{
	assert(rakServer->IsActive() || rakClient->IsConnected());

	if (msg==0)
		return;

	ChatMessageStruct s;
	s.typeId=ID_CHAT_MESSAGE;

	char* packet;
	packet = new char[strlen(msg) + 1 + sizeof(ChatMessageStruct)]; // 1 for the null terminator
	memcpy(packet, &s, sizeof(ChatMessageStruct));
	memcpy(packet + sizeof(ChatMessageStruct), msg, strlen(msg) + 1);
	
	if (rakClient->IsConnected())
		rakClient->Send(packet, strlen(msg) + 1 + sizeof(ChatMessageStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1);
	else
		rakServer->Send(packet, strlen(msg) + 1 + sizeof(ChatMessageStruct), LOW_PRIORITY, RELIABLE_ORDERED, 1, 255, true, false);
	delete [] packet;
}

void GameState::ReceiveSummonCreature(Packet *p)
{
	SummonCreatureStruct *s = ((SummonCreatureStruct *) p->data);
	assert(p->length == sizeof(SummonCreatureStruct));
	Demon *demon = CreateDemon(s->pointOnGround, s->team, true); // Create the demon
	demon->Update(getTime() - s->timeStamp);  // Advance it in time to right now to compensate for lag
	if (rakServer->IsActive())
	{
		// If the server gets this then this is actually a client request to summon a creature.
		// So send the packet back with the objectID field filled in
		s->objectId = demon->GetID();
	//	rakServer->SendPacket((char*)s, sizeof(SummonCreatureStruct), p->playerID, HIGH_PRIORITY, RELIABLE_ORDERED,0);
	}
	else
	{
		demon->SetID(s->objectId);  // This is the client, just record the ID of the demon created
	}
}

void GameState::ReceiveKillCreature(Packet *p)
{
	KillCreatureStruct *s = ((KillCreatureStruct *) p->data);
	assert(p->length == sizeof(KillCreatureStruct));
	Creature *creature = (Creature *) GET_OBJECT_FROM_ID(s->objectId);
	if (creature==0)
		return;
	creature->Die((DamageType)s->dt, false, true);
}

void GameState::ReceiveSetCreatureTeam(Packet *p)
{
	SetCreatureTeamStruct *s = ((SetCreatureTeamStruct *) p->data);
	assert(p->length == sizeof(SetCreatureTeamStruct));
	Creature *creature = (Creature *) GET_OBJECT_FROM_ID(s->objectId);
	if (creature==0)
		return;
	creature->SetTeam(false, s->team);
}

void GameState::ReceiveChoosePerk(Packet *p)
{
	ChoosePerkStruct *s = ((ChoosePerkStruct *) p->data);
	assert(p->length == sizeof(ChoosePerkStruct));

	int team = user->GetOppositeTeam();
	assert(team==1 || team==2);
	assert(s->perk >=0 && s->perk < NUMBER_OF_PERKS);
	GivePerkToPlayer(s->perk, team, true);

	// Vicious hex requires a bit more coding
	if (s->perk==VICIOUS_HEX)
	{
		players[team-1].SetPerk(s->additionalData1, false);
		players[team-1].SetPerk(s->additionalData2, false);
	}
}

void GameState::ReceiveCreateFireball(Packet *p)
{
	CreateFireballStruct *s = ((CreateFireballStruct *) p->data);
	assert(p->length == sizeof(CreateFireballStruct));

	// Get the spell corresponding to the fireball spell
	FireballSpell* fireballSpell = (FireballSpell*) players[user->GetOppositeTeam()-1].GetSpell(FIREBALL_SPELL_INDEX);

	// Set the throw power according to the power
	fireballSpell->SetThrowPower(s->throwPower);

	// Create the fireball (which uses the throw power)
	Fireball * fireball = CreateFireball(s->rayOrg, s->rayDir, user->GetOppositeTeam());

	// Update the fireball for lag
	fireball->Update(getTime() - s->timeStamp);
}

void GameState::ReceiveSetCastleHealth(Packet *p)
{
	SetCastleHealthStruct *s = ((SetCastleHealthStruct *) p->data);
	assert(p->length == sizeof(SetCastleHealthStruct));
	assert(castleHealth[user->GetOppositeTeam()-1] >= 0 && castleHealth[user->GetOppositeTeam()-1] <= CASTLE_HEALTH);

	castleHealth[user->GetOppositeTeam()-1]=s->health;
}

void GameState::ReceiveChatMessage(Packet *p)
{
	ChatMessageStruct s = *((ChatMessageStruct *) p->data);
	assert(p->length >= sizeof(ChatMessageStruct));

	directSound->PlaySound(soundContainer->sounds[RECEIVE_CHAT_MESSAGE_SOUND], false);

	if (rakClient->IsConnected())
		AddChatMessage(((EnumerationDataStruct*)(rakClient->GetStaticServerData()->GetData()))->serverName, p->data+sizeof(ChatMessageStruct));
	else
		AddChatMessage(((StaticClientDataStruct*)(rakServer->GetStaticClientData(p->playerId)->GetData()))->name, p->data+sizeof(ChatMessageStruct));
}

void GameState::AddChatMessage(char *name, char *message)
{
	if (name==0 && message==0)
		return;

	char str[200];
	if (name)
		strcpy(str, name);
	strcat(str, "> ");
	if (message)
		strcat(str, message);
	chatMessageContainer.AddText(str);
}


Player* GameState::GetNonUserPlayer(void)
{
	return &players[user->GetOppositeTeam()-1];
}

void GameState::GetFireballStartPoint(D3DXVECTOR3 *fireballStartPoint, D3DXVECTOR3 *rayDir)
{
	*rayDir = D3DXVECTOR3 (user->mouseCursor.GetX() - user->camera.GetEyePt().x, 
				user->mouseCursor.GetY() - user->camera.GetEyePt().y, 
				user->mouseCursor.GetZ() - user->camera.GetEyePt().z);

	D3DXVec3Normalize(rayDir, rayDir);

	D3DXVECTOR3 temp;
	temp = D3DXVECTOR3(user->mouseCursor.GetX(), user->mouseCursor.GetY(),user->mouseCursor.GetZ()) + *rayDir * (GROUND_WIDTH/2 - TEAM_1_CASTLE_POSITION);

	*fireballStartPoint = temp;
}

void GameState::PositionLightedSphere(float *x, float *z)
{

	float timeToImpact, fireballVelocity;
//	static const float groundHeight=0.0f; // change this if I stop using flat ground later
	D3DXVECTOR3 fireballStartPoint, rayDir;
	GetFireballStartPoint(&fireballStartPoint, &rayDir);
	fireballVelocity = ((FireballSpell*)players[user->GetTeam()-1].GetSpell(FIREBALL_SPELL_INDEX))->GetThrowPower();

	// Time to impact
	// y = y0 + v0t + 1/2 * a * t * t
	// To get the time to impact 
	// Use the Quadratic equation with
	// A = .5 * a
	// B = initial velocity of the fireball = rayDir.y * fireballVelocity
	// C = initial height of the fireball
	// t = (- B +- (B*B - 4 * A * C)^.5) / (2 * A)

	float A = .5 * GRAVITY;
	float B = rayDir.y * fireballVelocity;
	float C = fireballStartPoint.y;
    
	timeToImpact = (- B - (float)sqrt(B*B - 4 * A * C)) / (2 * A);

	// Distance it will travel
	if (user->GetTeam()==1)
		*x = fireballStartPoint.x + rayDir.x * fireballVelocity * timeToImpact;
	else
		*x = fireballStartPoint.x + rayDir.x * fireballVelocity * timeToImpact;

	*z = fireballStartPoint.z + rayDir.z * fireballVelocity * timeToImpact;


	/*
	float timeToImpact, fireballVelocity;
//	static const float groundHeight=0.0f; // change this if I stop using flat ground later
	D3DXVECTOR3 fireballStartPoint, rayDir;
	GetFireballStartPoint(&fireballStartPoint, &rayDir);
	fireballVelocity = ((FireballSpell*)players[user->GetTeam()-1].GetSpell(FIREBALL_SPELL_INDEX))->GetThrowPower();

	// Time to impact
	// y = y0 + v0t + 1/2 * a * t * t

	timeToImpact = sqrt(-2.0f * fireballStartPoint.y / GRAVITY);

	// Distance it will travel
	if (user->GetTeam()==1)
		*x = fireballStartPoint.x - fireballVelocity * timeToImpact;
	else
		*x = fireballStartPoint.x + fireballVelocity * timeToImpact;

	*z = fireballStartPoint.z;
	*/
}