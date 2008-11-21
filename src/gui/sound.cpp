#include "sound.h"


std::multimap<SoundName, Sound> SoundSystem::m_sounds;

std::map<std::string, SoundObject*> SoundSystem::m_sound_objects;

std::map<SoundTarget, SoundName> SoundSystem::m_sounds_targets;

std::map<int, SoundName> SoundSystem::m_projectile_sounds;

Sound SoundSystem::getSound(SoundName sname)
{
	// Iteratoren auf den ersten und letzten Sound mit dem gewuenschten Name
	std::multimap<SoundName, Sound>::iterator it,jt;
	it = m_sounds.lower_bound(sname);
	jt = m_sounds.upper_bound(sname);
	
	// NONE ausgeben, wenn kein Sound vorhanden
	if (m_sounds.count(sname) ==0)
	{
		return AL_NONE;
	}
	
	// unter allen Sounds gleichmaessig einen auswuerfeln
	Sound snd = it->second;
	
	int count =2;
	++it;
	while (it != jt)
	{
		if (rand() %count ==0)
		{
			snd = it->second;
		}
		count ++;
		++it;
		
	}
	
	return snd;
	
	
}

void SoundSystem::loadSoundFile(std::string file, SoundName sname)
{
	int buffernr;
	buffernr = alutCreateBufferFromFile(file.c_str());
	if (buffernr !=  AL_NONE)
	{
		m_sounds.insert(std::make_pair(sname,buffernr));
	}
	else
	{
		ERRORMSG( "could not load %s",file.c_str());
	}
}

void SoundSystem::init()
{
	alutInit(0,0);
	
	alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
	
	
	// Soundfiles laden
	SoundSystem::loadSoundFile("../resources/sound/arrow.wav", "arrow");
	SoundSystem::loadSoundFile("../resources/sound/cast_ice.wav", "cast_ice");
	SoundSystem::loadSoundFile("../resources/sound/fire_cast.wav", "cast_fire");
	SoundSystem::loadSoundFile("../resources/sound/thunder_cast1.wav", "cast_thunder");
	SoundSystem::loadSoundFile("../resources/sound/thunder_cast2.wav", "cast_thunder");
	SoundSystem::loadSoundFile("../resources/sound/thunder.wav", "thunder");
	
	SoundSystem::loadSoundFile("../resources/sound/dog_atk.wav", "dog_attack");
	SoundSystem::loadSoundFile("../resources/sound/dog_die.wav", "dog_die");
	
	SoundSystem::loadSoundFile("../resources/sound/lich_atk.wav", "lich_attack");
	SoundSystem::loadSoundFile("../resources/sound/lich_die.wav", "lich_die");
	SoundSystem::loadSoundFile("../resources/sound/lich_hit.wav", "lich_hit");
	
	SoundSystem::loadSoundFile("../resources/sound/monster_atk.wav", "monster_attack");
	SoundSystem::loadSoundFile("../resources/sound/monster_die.wav", "monster_die");
	SoundSystem::loadSoundFile("../resources/sound/monster_hit.wav", "monster_hit");
	
	SoundSystem::loadSoundFile("../resources/sound/hero_hit.wav", "hero_hit");
	SoundSystem::loadSoundFile("../resources/sound/hero_die.wav", "hero_die");
	
	SoundSystem::loadSoundFile("../resources/sound/sword_atk.wav", "sword");
	
	SoundSystem::loadSoundFile("../resources/sound/drob_wood.wav", "drop_wood");
	SoundSystem::loadSoundFile("../resources/sound/drop_metall.wav", "drop_metal");
	SoundSystem::loadSoundFile("../resources/sound/drop_potion.wav", "drop_potion");
	
	// Ereignisse auf Soundfiles mappen
	SoundSystem::registerSound("short_sw:attack", "sword");
	SoundSystem::registerSound("short_sw:drop", "drop_metal");
	SoundSystem::registerSound("battle_axe:attack", "sword");
	SoundSystem::registerSound("battle_axe:drop", "drop_metal");
	SoundSystem::registerSound("heal_1:drop", "drop_potion");
	SoundSystem::registerSound("heal_2:drop", "drop_potion");
	SoundSystem::registerSound("wood_sh:drop", "drop_wood");
	
	
	SoundSystem::registerSound("goblin:hit", "monster_hit");
	SoundSystem::registerSound("goblin:die", "monster_die");
	SoundSystem::registerSound("goblin:attack", "monster_attack");

	SoundSystem::registerSound("lich:hit", "lich_hit");
	SoundSystem::registerSound("lich:die", "lich_die");
	SoundSystem::registerSound("lich:attack", "lich_attack");
	
	SoundSystem::registerSound("gob_dog:hit", "monster_hit");
	SoundSystem::registerSound("gob_dog:die", "dog_die");
	SoundSystem::registerSound("gob_dog:attack", "dog_attack");
	
	SoundSystem::registerSound("hero:die", "hero_die");
	SoundSystem::registerSound("hero:hit", "hero_hit");
	
	SoundSystem::registerSound("arrow", "arrow");
	SoundSystem::registerSound("cast_fire", "cast_fire");
	SoundSystem::registerSound("cast_ice", "cast_ice");
	SoundSystem::registerSound("cast_thunder", "cast_thunder");
	SoundSystem::registerSound("thunder", "thunder");
	
	
	m_projectile_sounds[Projectile::ARROW] = "arrow";
	m_projectile_sounds[Projectile::GUIDED_ARROW] = "arrow";
	m_projectile_sounds[Projectile::FIRE_ARROW] = "arrow";
	m_projectile_sounds[Projectile::ICE_ARROW] = "arrow";
	m_projectile_sounds[Projectile::WIND_ARROW] = "arrow";
	
	m_projectile_sounds[Projectile::MAGIC_ARROW] = "cast_fire";
	m_projectile_sounds[Projectile::FIRE_BOLT] = "cast_fire";
	m_projectile_sounds[Projectile::FIRE_BALL] = "cast_fire";
	m_projectile_sounds[Projectile::FIRE_WALL] = "cast_fire";
	m_projectile_sounds[Projectile::FIRE_WAVE] = "cast_fire";
	m_projectile_sounds[Projectile::FIRE_WAVE] = "cast_fire";
	m_projectile_sounds[Projectile::EXPLOSION] = "cast_fire";
	m_projectile_sounds[Projectile::FIRE_EXPLOSION] = "cast_fire";
	
	m_projectile_sounds[Projectile::ICE_BOLT] = "cast_ice";
	m_projectile_sounds[Projectile::BLIZZARD] = "cast_ice";
	m_projectile_sounds[Projectile::ICE_RING] = "cast_ice";
	m_projectile_sounds[Projectile::FREEZE] = "cast_ice";
	m_projectile_sounds[Projectile::ICE_EXPLOSION] = "cast_ice";
	
	
	m_projectile_sounds[Projectile::LIGHTNING] = "cast_thunder";
	m_projectile_sounds[Projectile::THUNDERSTORM] = "thunder";
	m_projectile_sounds[Projectile::CHAIN_LIGHTNING] = "cast_thunder";
	m_projectile_sounds[Projectile::STATIC_SHIELD] = "cast_thunder";
	m_projectile_sounds[Projectile::WIND_EXPLOSION] = "cast_thunder";
	
	
	m_projectile_sounds[Projectile::LIGHT_BEAM] = "cast_fire";
	m_projectile_sounds[Projectile::ELEM_EXPLOSION] = "cast_fire";
	m_projectile_sounds[Projectile::ACID] = "cast_fire";
	m_projectile_sounds[Projectile::DIVINE_BEAM] = "cast_fire";
	m_projectile_sounds[Projectile::HYPNOSIS] = "cast_fire";

 
}

void SoundSystem::setListenerPosition(Vector pos)
{
	ALfloat listenerPos[3]={0.0,0.0,0.0};
	listenerPos[0] = pos.m_x;
	listenerPos[1] = pos.m_y;
	
	alListenerfv(AL_POSITION,listenerPos);	
}

void SoundSystem::cleanup()
{
	std::map<SoundName, Sound>::iterator it;
	for (it = m_sounds.begin(); it != m_sounds.end(); ++it)
	{
		alDeleteBuffers(1,&(it->second));
	}
	m_sounds.clear();
	
	clearObjects();
}

void SoundSystem::clearObjects()
{
	std::map<std::string, SoundObject*>::iterator it;
	for (it = m_sound_objects.begin(); it != m_sound_objects.end(); ++it)
	{
		delete it->second;
	}
	
	m_sound_objects.clear();
}


SoundObject* SoundSystem::createSoundObject(std::string name)
{
	if (m_sound_objects.count(name)>0)
	{
		ERRORMSG("sound object with name %s already exists",name.c_str());
		return getSoundObject(name);
	}
	
	SoundObject* so;
	so = new SoundObject();
	m_sound_objects.insert(std::make_pair(name,so));
	return so;
	
}

SoundObject* SoundSystem::getSoundObject(std::string name)
{
	std::map<std::string, SoundObject*>::iterator it;
	it = m_sound_objects.find(name);
	if (it ==m_sound_objects.end())
	{
		return 0;
	}
		
	return it->second;
}

void SoundSystem::deleteSoundObject(std::string name)
{
	std::map<std::string, SoundObject*>::iterator it;
	it = m_sound_objects.find(name);
	if (it !=m_sound_objects.end())
	{
		delete it->second;
		m_sound_objects.erase(it);
	}
	else
	{
		ERRORMSG("cant delete Sound Object %s",name.c_str());
	}
}

SoundName SoundSystem::getSoundName(SoundTarget target)
{
	static std::map<SoundTarget, SoundName>::iterator it;
	
	it = m_sounds_targets.find(target);
	if (it == m_sounds_targets.end())
	{
		return "";
	}
	
	return it->second;
}

void SoundSystem::registerSound(SoundTarget target, SoundName name)
{
	m_sounds_targets.insert(std::make_pair(target,name));
}

SoundName SoundSystem::getProjectileSound(Projectile::ProjectileType ptype)
{
	std::map<int, SoundName>::iterator it;
	
	it = m_projectile_sounds.find((int) ptype);
	if (it != m_projectile_sounds.end())
	{
		return it->second;
	}
	return "";
}



SoundObject::SoundObject(Vector pos)
{
	alGenSources(1, &m_handle);
	setPosition(pos);
	
	// zum testen
	//alSourcei(m_handle, AL_LOOPING, AL_TRUE);
	alSourcef(m_handle, AL_REFERENCE_DISTANCE, 5);
	//alSourcef(m_handle, AL_ROLLOFF_FACTOR, 0.5);
	alSourcef(m_handle,AL_MAX_DISTANCE , 20);
	
	m_sound_name = "";
}

SoundObject::~SoundObject()
{
	alDeleteSources(1,&m_handle);
}

void SoundObject::setPosition(Vector pos)
{
	ALfloat spos[3]={0.0,0.0,0.0};
	spos[0] = pos.m_x;
	spos[1] = pos.m_y;
	
	alSourcefv(m_handle, AL_POSITION, spos);
}

Vector SoundObject::getPosition()
{
	ALfloat spos[3]={0.0,0.0,0.0};
	alGetSourcefv(m_handle, AL_POSITION, spos);
	
	return Vector(spos[0],spos[1]);
}

void SoundObject::setSound(Sound snd)
{
	alSourcei(m_handle, AL_BUFFER, snd);
}

void SoundObject::setSound(SoundName sname)
{
	update();
	
	if (m_sound_name != "")
	{
		return;
	}
	
	if (sname == "")
	{
		m_sound_name = "";
		return;	
	}
	
	
	Sound snd = SoundSystem::getSound(sname);
	if (snd != AL_NONE)
	{
		m_sound_name =sname;
		setSound(snd);
		play();
	}
}
	
void SoundObject::play()
{
	alSourcePlay(m_handle);
}

void SoundObject::stop()
{
	alSourceStop(m_handle);
	m_sound_name="";
}

void SoundObject::rewind()
{
	alSourceRewind(m_handle);
}

void SoundObject::update()
{
	int state;
	alGetSourcei(m_handle,AL_SOURCE_STATE,&state);
	
	if (state == AL_STOPPED)
	{
		m_sound_name = "";
	}
}


