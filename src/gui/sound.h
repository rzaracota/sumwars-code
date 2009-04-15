
#include <AL/al.h>
#include <AL/alut.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>

#include "geometry.h"
#include "projectile.h"

/**
 * \brief Name eines Sounds
 */
typedef std::string SoundName;

/**
 * \brief Name einer Situation in der ein Sound abgespielt wird
 */
typedef std::string SoundTarget;

/**
 * \brief OpenAL ID eines Sounds
 */
typedef ALuint Sound;

class SoundObject;

/**
 * \class SoundSystem
 * \brief Basisklasse fuer Sound
 */
class SoundSystem
{
	public:
		/**
		 * \fn static void init()
		 * \brief initialisiert das Soundsystem
		 */
		static void init();
		
		/**
		 * \fn static Sound getSound(SoundName sname)
		 * \brief Gibt zu einem gegebenen Name einen Sound aus
		 * \param sname Name des Sounds
		 */
		static Sound getSound(SoundName sname);
		
		/**
		 * \fn static void loadSoundFile(std::string file, SoundName sname)
		 * \brief Laedt ein Soundfile und registriert den Sound unter einen gegebenen Name
		 */
		static void loadSoundFile(std::string file, SoundName sname);
		
		/**
		 * \fn static void setListenerPosition(Vector pos)
		 * \brief Setzt die Position an der der Schall gehoehrt wird
		 */
		static void setListenerPosition(Vector pos);
		
		/**
		 * \fn static void cleanup()
		 * \brief gibt Speicher frei
		 */
		static void cleanup();
		
		/**
		 * \fn static void clearObjects()
		 * \brief Loescht alle SoundObjekte
		 */
		static void clearObjects();
		
		/**
		 * \fn SoundObject* getSoundObject(std::string name)
		 * \brief Gibt das SoundObjekt mit dem angegebenen Name aus
		 * \param name Name
		 */
		static SoundObject* getSoundObject(std::string name);
		
		/**
		 * \fn static void createSoundObject(std::string name)
		 * \brief Erzeugt ein SoundObjekt mit dem angegebenen Name
		 * \param name Name
		 */
		static SoundObject* createSoundObject(std::string name);
		
		/**
		 * \fn static void deleteSoundObject(std::string name)
		 * \brief Loescht ein SoundObjekt mit dem angegebenen Name
		 */
		static void deleteSoundObject(std::string name);
		
		/**
		 * \fn static SoundName getSoundName(SoundTarget target)
		 * \brief Gibt fuer die gegebene Situation den richtigen Sound aus
		 * \param target Name der Situation
		 */
		static SoundName getSoundName(SoundTarget target);
		
		/**
		 * \fn static void registerSound(SoundTarget target, SoundName name)
		 * \brief Registriert fuer eine gegebene Situation einen Sound
		 * \param target  Name der Situation
		 * \param name Name des Sounds
		 */
		static void registerSound(SoundTarget target, SoundName name);
		
		/**
		 * \fn static SoundName getProjectileSound(Projectile::Subtype ptype)
		 * \brief Gibt den Sound fuer ein bestimmtes Projektil aus
		 * \param ptype Typ des Projektils
		 */
		static SoundName getProjectileSound(Projectile::Subtype ptype);
		
		/**
		 * \fn static void setSoundVolume(float vol)
		 * \brief Stellt die Lautstaerke ein
		 * \param vol Lautstaerke (0-1)
		 */
		static void setSoundVolume(float vol);
	
	private:
		
		/**
	 * \fn static std::map<GameObject::Subtype, SoundName> m_projectile_sounds
		 * \brief Sounds die abgespielt werden, wenn Projektile gestartet werden
		 */
		static std::map<GameObject::Subtype, SoundName> m_projectile_sounds;
		
		/**
		 * \var static std::multimap<SoundName, Sound> m_sounds
		 * \brief Bildet die Name der Sounds auf die OpenAL IDs ab
		 */
		static std::multimap<SoundName, Sound> m_sounds;
		
		/**
		 * \var static std::map<SoundTarget, SoundName> m_sounds_targets
		 * \brief Speichert fuer Situationen, in denen Sounds abgespielt werden den Name des Sounds
		 */
		static std::map<SoundTarget, SoundName> m_sounds_targets;
		
		/**
		 * \var static std::map<std::string, SoundObject*> m_sound_objects
		 * \brief Schallquellen sortiert nach Name
		 */
		static std::map<std::string, SoundObject*> m_sound_objects;
		
};

/**
 * \class SoundObject
 * \brief Schallquelle
 */
class SoundObject
{
	public:
		/**
		 * \fn SoundObject(Vector pos = Vector(0,0))
		 * \brief Konstruktor
		 * \param pos Position
		 */
		SoundObject(Vector pos = Vector(0,0));
		
		/**
		 * \fn ~SoundObject()
		 * \brief Destruktor
		 */
		~SoundObject();
	
		/**
		 * \fn void setPosition(Vector pos)
		 * \brief Setzt die Position der Schallquelle
		 * \param pos neue Position
		 */
		void setPosition(Vector pos);
		
		/**
		 * \fn Vector getPosition()
		 * \brief Gibt die Position der Schallquelle aus
		 */
		Vector getPosition();
		
		/**
		 * \fn void setSound(Sound snd)
		 * \brief Setzt den abgespielten Sound der Schallquelle
		 * \param snd Sound ID
		 */
		void setSound(Sound snd);
		
		/**
		 * \fn void setSound(SoundName sname)
		 * \brief Setzt den abgespielten Sound
		 * \param sname Name des Sounds
		 */
		void setSound(SoundName sname);
		
		/**
		 * \fn void setVolume(float vol)
		 * \brief Setzt die Lautstaerke der Schallquelle
		 * \param vol Lautstaerke (0-1)
		 */
		void setVolume(float vol);
		
		/**
		 * \fn void play()
		 * \brief Startet das Abspielen des eingestellten Sounds
		 */
		void play();
		
		/**
		 * \fn void setPlayOffset(float ms, float tol)
		 * \brief Setzt den Zeitpunkt des Sounds der gerade abgespielt wird
		 * \param ms Zeitpunkt in ms seit Beginn
		 * \param tol Toleranzwert - wenn der aktuelle Zustand weniger als um tol abweicht wird nichts geaendert
		 */
		void setPlayOffset(float ms, float tol);
		
		/**
		 * \fn void stop()
		 * \brief Stoppt das Abspielen des eingestellten Sounds
		 */
		void stop();
		
		/**
		 * \fn void rewind()
		 * \brief Spult den abgespielten Sound zurueck
		 */
		void rewind();
		
		/**
		 * \fn SoundName getSoundName()
		 * \brief Gibt den Name des aktuell abgespielten Sounds aus
		 */
		SoundName getSoundName()
		{
			return m_sound_name;
		}
		
		/**
		 * \fn void update()
		 * \brief aktualisiert internen Status
		 */
		void update();
		
	private:
		/**
		 * \var ALuint m_handle
		 * \brief OpenAL handle fuer die Soundquelle
		 **/
		ALuint m_handle;
		
		/**
		 * \var SoundName m_sound_name
		 * \brief Name des aktuellen Sounds
		 */
		SoundName m_sound_name;
	
	
};

