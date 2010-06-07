#ifndef __DEMON_BATTLE_TYPES
#define __DEMON_BATTLE_TYPES

#include "FileLoadedVisibleEntity.h"
#include "..\Common\Include\d3dutil.h"

#pragma pack(1)
struct EnumerationDataStruct
{
	char serverName[40];

};

#define SCDS_MAXIMUM_NAME_LENGTH 40

#pragma pack(1)
struct StaticClientDataStruct
{
	char name[40];
};


#pragma pack(1)
struct KillCreatureStruct
{
	unsigned char typeId;
	ObjectID objectId;
    DamageType dt;
};

#pragma pack(1)
struct SetCreatureTeamStruct
{
	unsigned char typeId;
	ObjectID objectId;
	unsigned char team;
};

#pragma pack(1)
struct SummonCreatureStruct
{
	unsigned char useTimestamp;  // ID_TIMESTAMP
	unsigned long timeStamp;
	unsigned char typeId;
	D3DXVECTOR3 pointOnGround;
	ObjectID objectId;
	unsigned char team;
};

#pragma pack(1)
struct CreateFireballStruct
{
	unsigned char useTimestamp;  // ID_TIMESTAMP
	unsigned long timeStamp;
	unsigned char typeId;
	D3DXVECTOR3 rayOrg;
	D3DXVECTOR3 rayDir;
	float throwPower;
};

#pragma pack(1)
struct ChoosePerkStruct
{
	unsigned char typeId;
	unsigned char perk;
	unsigned char additionalData1;
	unsigned char additionalData2;
};

#pragma pack(1)
struct SetCastleHealthStruct
{
	unsigned char typeId;
	float health;
};

#endif