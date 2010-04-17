#pragma once

#include <string>

#include "ogg.h"

class MusicManager
{
	public:
		/**
		 * \brief Konstruktor
		 */
		MusicManager()
		{
			m_musicfile = "";
			m_is_playing = false;
			m_music_volume = 1;
		}
		
		/**
		 * \brief Destruktor
		 */
		~MusicManager()
		{
			if (m_is_playing)
			{
				stop();
			}
		}
		
		/**
		 * \brief Spielt eine Musikdatei im .ogg Format ab
		 * \param file Musikdatei
		 * Spielt eine Musikdatei im .ogg Format ab. Die evtl zuvor laufende Musik wird gestoppt
		 */
		void play(std::string file);
		
		/**
		 * \brief Stoppt das Abspielen der Musik
		 */
		void stop();
		
		
		/**
		 * \brief Aktualisierung, muss regelmaessig aufgerufen werden
		 */
		void update();
			
		/**
		 * \brief Gibt die einzige Instanz der Klasse aus
		 */
		static MusicManager& instance()
		{
			static MusicManager inst;
			return inst;
		}
		
		/**
		 * \brief Gibt aus, ob gerade Musik gespielt wird
		 */
		bool isPlaying()
		{
			return m_is_playing;
		}
		
		/**
		 * \fn void setMusicVolume(float vol)
		 * \brief Stellt die Lautstaerke ein
		 * \param vol Lautstaerke (0-1)
		 */
		void setMusicVolume(float vol)
		{
			m_music_volume = vol;
			m_ogg_stream.setVolume(vol);
		}

		/**
		 * \fn float getMusicVolume()
		 * \brief Gibt die Lautstaerke des Sounds aus
		 */
		float getMusicVolume()
		{
			return m_music_volume;
		}
		
	private:
		
		/**
		 * \brief aktuell abgespielete Musikdatei
		 */
		std::string m_musicfile;
		
		/**
		 * \brief Gibt an, ob gerade eine Datei abgespielt wird
		 */
		bool m_is_playing;
		
		/**
		 * \brief Hilfsklasse fuer das Abspielen von ogg Dateien
		 */
		ogg_stream m_ogg_stream;
		
		/**
		 * \brief Lautstaerke
		 */
		float m_music_volume;
	
};

