#include "sound.h"

#include "elementattrib.h"
#include <OgreResourceGroupManager.h>

std::multimap<SoundName, Sound> SoundSystem::m_sounds;

std::map<std::string, SoundObject*> SoundSystem::m_sound_objects;

float SoundSystem::m_sound_volume = 0.0f;
ALCdevice * SoundSystem::m_device;
ALCcontext* SoundSystem::m_context;

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
	Ogre::FileInfoListPtr files;
	Ogre::FileInfoList::iterator it;
	std::string filename;
	files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("sound",file);
	
	for (it = files->begin(); it != files->end(); ++it)
	{
		filename = it->archive->getName();
		filename += "/";
		filename += it->filename;
		
		DEBUGX("loading file %s",filename.c_str());
		
		int buffernr;
		buffernr = alutCreateBufferFromFile(filename.c_str());
		if (buffernr !=  AL_NONE)
		{
			m_sounds.insert(std::make_pair(sname,buffernr));
			
		}
		else
		{
			ERRORMSG( "Could not load %s",filename.c_str());
		}
	}
	checkErrors();
}

void SoundSystem::loadSoundData(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();

	if (loadOkay)
	{
		loadSoundInfos(&doc);
	}
	else
	{
		ERRORMSG("Failed to load file %s", pFilename);
	}
}

void SoundSystem::loadSoundInfos(TiXmlNode* node)
{
	TiXmlNode* child;
	if (node->Type()==TiXmlNode::ELEMENT && !strcmp(node->Value(), "Sound"))
	{
		ElementAttrib attr;
		attr.parseElement(node->ToElement());
		
		std::string name,file;
		attr.getString("name",name);
		
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			if (child->Type()==TiXmlNode::ELEMENT)
			{
				attr.parseElement(child->ToElement());
				if (!strcmp(child->Value(), "Soundfile"))
				{
					attr.getString("source",file);
					DEBUGX("loading sound file %s for sound %s",file.c_str(), name.c_str());
					loadSoundFile(file, name);
				}
			}
		}
	}
	else
	{
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			loadSoundInfos(child);
		}
	}
}

void SoundSystem::init()
{
	m_device = 0;
	m_context = 0;
	// variante 1, geht unter ubuntu 9.10 nicht
	/*
    m_device = alcOpenDevice("Generic Software");
    m_context = alcCreateContext(m_device, NULL);
    alcMakeContextCurrent(m_context);
	alutInitWithoutContext(0,0);
	*/
	
	// variante 2, geht unter win nicht
	//alutInit(0,0);
	
	// variante 3: zu testen
	/*
	const ALCchar *defaultDevice = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
	m_device = alcOpenDevice(defaultDevice);
	m_context = alcCreateContext(m_device, NULL);
	alcMakeContextCurrent(m_context);
	alutInitWithoutContext(0,0);
	*/
	
	// variante 4: zu testen
	
	m_device = alcOpenDevice(NULL);
    m_context = alcCreateContext(m_device, NULL);
    alcMakeContextCurrent(m_context);
	alutInitWithoutContext(NULL,NULL);
	
	alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
	checkErrors();
}

void SoundSystem::setListenerPosition(Vector pos)
{
	ALfloat listenerPos[3]={0.0,0.0,0.0};
	listenerPos[0] = pos.m_x;
	listenerPos[1] = pos.m_y;

	alListenerfv(AL_POSITION,listenerPos);
	checkErrors();
}

void SoundSystem::cleanup()
{
	std::multimap<SoundName, Sound>::iterator it = m_sounds.begin();
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


void SoundSystem::setSoundVolume(float vol)
{
	m_sound_volume = vol;
	
	// neue Lautstarke fuer existierende Soundobjekte
	std::map<std::string, SoundObject*>::iterator it;
	for (it = m_sound_objects.begin(); it != m_sound_objects.end(); ++it)
	{
		it->second->setVolume(vol);
	}
}

SoundObject* SoundSystem::createSoundObject(std::string name)
{
	if (m_sound_objects.count(name)>0)
	{
		ERRORMSG("Sound object with name %s already exists",name.c_str());
		return getSoundObject(name);
	}
	DEBUGX("creating sound object with name %s",name.c_str());

	SoundObject* so;
	so = new SoundObject(name);
	so->setVolume(m_sound_volume);
	m_sound_objects.insert(std::make_pair(name,so));
	checkErrors();
	
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
		ERRORMSG("Cant delete Sound Object %s",name.c_str());
	}
}

void SoundSystem::deleteSoundObject(SoundObject* object)
{
	deleteSoundObject(object->getName());
}


bool SoundSystem::getSourceHandle(ALuint &handle)
{
	bool success = true;
	
	alGenSources(1, &handle);
	if (alGetError() != AL_NO_ERROR)
	{
		success = false;
	}
	else
	{
		//alSourcei(m_handle, AL_LOOPING, AL_TRUE);
		alSourcef(handle, AL_REFERENCE_DISTANCE, 5);
		//alSourcef(m_handle, AL_ROLLOFF_FACTOR, 0.5);
		alSourcef(handle,AL_MAX_DISTANCE , 20);
	}
	return success;
}

bool SoundSystem::checkErrors()
{
	int error = alGetError();

	if(error != AL_NO_ERROR)
	{
		ERRORMSG("AL error %i: %s",error, alGetString(error)) ;
		return true;
	}
	return false;
}

SoundObject::SoundObject(std::string name, Vector pos)
{
	/*
	m_handle_valid = SoundSystem::getSourceHandle(m_handle);
	setPosition(pos);
	SoundSystem::checkErrors();
	*/
	m_handle_valid = false;
	m_sound_name = "";
	m_name = name;
	m_global_volume = 1.0;
	m_volume = 1.0;
}

SoundObject::~SoundObject()
{
	if (m_handle_valid)
	{
		alDeleteSources(1,&m_handle);
	}
}

void SoundObject::setPosition(Vector pos)
{
	m_position = pos;
	

	if (m_handle_valid)
	{
		ALfloat spos[3]={0.0,0.0,0.0};
		spos[0] = pos.m_x;
		spos[1] = pos.m_y;
	
		alSourcefv(m_handle, AL_POSITION, spos);
	}
}

Vector SoundObject::getPosition()
{
	return m_position;
}

void SoundObject::setSound(Sound snd)
{
	// if this object has no valid handle, try to get a valid one
	if (!m_handle_valid)
	{
		m_handle_valid = SoundSystem::getSourceHandle(m_handle);
		setPosition(m_position);
		setVolume(m_volume);
	}
	
	if (m_handle_valid)
	{
		alSourcei(m_handle, AL_BUFFER, snd);
		SoundSystem::checkErrors();
	}
	else
	{
		m_sound_name = "";
	}
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
	if (m_handle_valid)
	{
		alSourcePlay(m_handle);
		SoundSystem::checkErrors();
	}
}

void SoundObject::stop()
{
	if (m_handle_valid)
	{
		alSourceStop(m_handle);
		SoundSystem::checkErrors();
	}
	m_sound_name="";
}

void SoundObject::rewind()
{
	if (m_handle_valid)
	{
		alSourceRewind(m_handle);
		SoundSystem::checkErrors();
	}
}

void SoundObject::update()
{
	if (m_handle_valid)
	{
		int state;
		alGetSourcei(m_handle,AL_SOURCE_STATE,&state);

		if (state == AL_STOPPED)
		{
			m_sound_name = "";
			alDeleteSources(1, &m_handle);
			m_handle_valid = false;
		}
	}
}


