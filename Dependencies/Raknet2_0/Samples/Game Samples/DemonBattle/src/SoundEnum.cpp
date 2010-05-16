#include "SoundEnum.h"

const char soundPaths[NUMBER_OF_SOUNDS][40] = 
{
	"sounds\\mouseOverWidget.wav",
	"sounds\\shootFireball.wav",
	"sounds\\gainLevel.wav",
	"sounds\\selectedPerk.wav",
	"sounds\\demonSummon.wav",
	"sounds\\enemyLoses.wav",
	"sounds\\fireballExplosion.wav",
	"sounds\\receiveChatMessage.wav"
};

int _maximumConcurrentPlayInstances[NUMBER_OF_SOUNDS] =
{
	6,
	3,
	1,
	1,
	3,
	1,
	6,
	3
};