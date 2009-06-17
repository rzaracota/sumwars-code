#ifndef SOUND_H
#define SOUND_H

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
#include "gameobject.h"
#include "tinyxml.h"

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
		 * \fn static void loadSoundData(const char* pFilename)
		 * \brief Laedt Daten zu Sounds aus einer XML Datei
		 * \param pFilename Dateiname
		 */
		static void loadSoundData(const char* pFilename);
		
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
		 * \fn static void deleteSoundObject(SoundObject* object)
		 * \brief Loescht das angegebene SoundObjekt
		 * \param object Soundobjekt
		 */
		static void deleteSoundObject(SoundObject* object);
		
		/**
		 * \fn static void setSoundVolume(float vol)
		 * \brief Stellt die Lautstaerke ein
		 * \param vol Lautstaerke (0-1)
		 */
		static void setSoundVolume(float vol);
		
		/**
		 * \fn static float getSoundVolume()
		 * \brief Gibt die Lautstaerke des Sounds aus
		 */
		static float getSoundVolume()
		{
			return m_sound_volume;
		}
	
	private:
		
		static void loadSoundInfos(TiXmlNode* node);
		
		/**
		 * \var static std::multimap<SoundName, Sound> m_sounds
		 * \brief Bildet die Name der Sounds auf die OpenAL IDs ab
		 */
		static std::multimap<SoundName, Sound> m_sounds;
		
		/**
		 * \var static std::map<std::string, SoundObject*> m_sound_objects
		 * \brief Schallquellen sortiert nach Name
		 */
		static std::map<std::string, SoundObject*> m_sound_objects;
		
		/**
		 * \var static float m_sound_volume
		 * \brief Lautstaerke
		 */
		static float m_sound_volume;
		
};

/**
 * \class SoundObject
 * \brief Schallquelle
 */
class SoundObject
{
	public:
		/**
		 * \fn SoundObject(std::string name, Vector pos = Vector(0,0))
		 * \brief Konstruktor
		 * \param pos Position
		 * \param name Name
		 */
		SoundObject(std::string name, Vector pos = Vector(0,0));
		
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
		
		
		/**
		 * \fn std::string getName()
		 * \brief gibt den Name aus
		 */
		std::string getName()
		{
			return m_name;
		}
		
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
	
		/**
		 * \var std::string m_name
		 * \brief Name des Objektes
		 */
		std::string m_name;
};

#endif

