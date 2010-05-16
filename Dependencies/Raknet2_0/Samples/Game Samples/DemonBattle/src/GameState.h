#ifndef __GAME_STATE_H
#define __GAME_STATE_H

#include "State.h"
#include "ArrayList.h"
#include "Bitmap.h"
#include "Text.h"
#include "TerrainMesh.h"
#include "Demon.h"
#include "FileLoadedVisibleEntity.h"
#include "Player.h"
#include "BitmapWidget.h"
#include "TextWidget.h"
#include "TextInputWidget.h"
#include "ChatMessageContainer.h"
#include "LightedSphere.h"
#include <D3d.h>

struct D3DXVECTOR3;
class Fireball;
class LightSource;
class ParticleSystem;
struct Packet;

class GameState : public State
{
	public:   
		~GameState();
		static GameState* Instance(void) {return &I;}
		void Init(void);
		void DeInit(void);
		void Render(long time);
		void Update(long time);
		void Load(void);
		void Unload(void);
		void KeyJustPressed(int key);
		void KeyJustReleased(int key);
		void MouseButtonJustPressed(int button);
		void MouseButtonJustReleased(int button);
		void IncreasePlayersExperience(void); // Give both players 1 experience point
		void RestartGame(void); // Clear out everything and reset variables to start the game over again without leaving the state
		void ResetCastleHealth(void); // Reset castle healths to maximum
		void ResetPlayers(void); // Reset players and player spells to initial values
		Fireball* CreateFireball(D3DXVECTOR3 rayOrg, D3DXVECTOR3 rayDir, int team); // Create a fireball with the specified settings
		Demon * CreateDemon(D3DXVECTOR3 pointOnGround, int team, bool calledFromNetwork); // Create a demon with the specified settings
		TerrainMesh* GetTerrainMesh(void) {return &terrainMesh;} // Get a pointer to the terrain
		BasicDataStructures::List<Demon*>* GetDemonList(void) {return &demonList;} // Get a pointer to the list of demons
		Player* GetNonUserPlayer(void);
		void GivePerkToPlayer(int perk, int team, bool calledFromNetwork); // Give the specified perk enum to the player on the specified team
		bool ProcessPacket(Packet* p, unsigned char packetIdentifier); // Handle an incoming packet
		void SetCastleHealth(int index, float health); // Set the castle with the specified index to the specified health

	protected:
		GameState();
		static GameState I;

		// Internal game functions
		void UpdateCamera(long time);
		void HandleLevelUp(void);
		bool CanChoosePerk(void);
		bool CanLevelUp(void);
		void DamageCastle(int index, float damage);
		void FillPerksArray(void);
		void DoRestart(void);
		void ClearNetworkObjects(void);

		// Network functions
		void TransmitSetCastleHealth(void);
		void TransmitSummonCreature(D3DXVECTOR3 pointOnGround, ObjectID objectId, unsigned char team);
		void TransmitChoosePerk(unsigned char perk, unsigned char additionalData1, unsigned char additionalData2);
		void TransmitCreateFireball(D3DXVECTOR3 rayOrg, D3DXVECTOR3 rayDir, float throwPower);
		void TransmitChatMessage(const char* msg);
		void ReceiveSummonCreature(Packet *p);
		void ReceiveKillCreature(Packet *p);
		void ReceiveSetCreatureTeam(Packet *p);
		void ReceiveChatMessage(Packet *p);
		void ReceiveSetCastleHealth(Packet *p);
		void ReceiveCreateFireball(Packet *p);
		void ReceiveChoosePerk(Packet *p);
		void PositionLightedSphere(float *x, float *z);
		void GetFireballStartPoint(D3DXVECTOR3 *fireballStartPoint, D3DXVECTOR3 *rayDir);

		void AddChatMessage(char *name, char *message);

		TerrainMesh terrainMesh;
		BasicDataStructures::List<Demon*> demonList;
		BasicDataStructures::List<Fireball*> fireballList;
		BasicDataStructures::List<ParticleSystem*> fireballParticleList;
		FileLoadedVisibleEntity enemyCastle;
		Bitmap backdrop;
		BitmapWidget levelUp;
		LightSource *castleDoorLight;
		float lastTransmitSetCastleHealth;
		long timeToNextCastleHealthTransmission;
		char gameMessage[300];
		Player players[2]; // Two players in the game
		float castleHealth[2]; // Each castle has a certain amount of health
		enum {GAME_IN_PROGRESS, GAME_WON, GAME_LOST, RESTART_WAITING_ON_REMOTE_SYSTEM, RESTART_WAITING_ON_LOCAL_SYSTEM, CONNECTION_LOST} currentState;
		Text centeredMessage, bottomMessage;
		ChatMessageContainer chatMessageContainer;
		TextInputWidget userChatMessageInput;
		TextWidget perkWidgets[NUMBER_OF_PERKS_TO_DRAW];
		int perks[NUMBER_OF_PERKS_TO_DRAW], numberOfPerksToChooseFrom;
		LightedSphere lightedSphere;
};

#define gameState GameState::Instance()

#endif