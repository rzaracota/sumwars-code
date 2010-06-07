// All the paths to the various sounds, sound textures, and animation files here

#ifndef __SOUND_ENUM_H
#define __SOUND_ENUM_H

enum soundNames
{
	MOUSE_OVER_WIDGET_SOUND,
	SHOOT_FIREBALL_SOUND,
	GAIN_LEVEL_SOUND,
	SELECTED_PERK_SOUND,
	DEMON_SUMMON_SOUND,
	ENEMY_LOSES_SOUND,
	FIREBALL_EXPLOSION_SOUND, // disabled
	RECEIVE_CHAT_MESSAGE_SOUND,
	NUMBER_OF_SOUNDS,
};

extern const char soundPaths[NUMBER_OF_SOUNDS][40];
extern int _maximumConcurrentPlayInstances[NUMBER_OF_SOUNDS];

#endif
