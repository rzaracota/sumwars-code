#pragma once

#include <string>
#include <map>
#include <set>

/**
 * Code fuer eine Taste
 */
typedef int KeyCode;

/**
 * \enum ShortkeyDestination
 * \brief List of Actions that may be triggered by a shortkey
 */
enum ShortkeyDestination
{
	NO_KEY=0,
	SHOW_INVENTORY =1,
	SHOW_CHARINFO =2,
	SHOW_SKILLTREE =3,
	SHOW_PARTYMENU=4,
	SHOW_CHATBOX =5,
	SHOW_CHATBOX_NO_TOGGLE =6,
	SHOW_QUESTINFO =7,
	SHOW_MINIMAP =8,
	SHOW_ITEMLABELS=9,
	CLOSE_ALL=19,
	SWAP_EQUIP=20,
	SHOW_OPTIONS = 21,
	USE_POTION = 30,
	USE_SKILL_LEFT=100,
	USE_SKILL_RIGHT=300,
	CHEAT = 1000,
};

/**
 * Maps Keycodes to the Actions that are triggered by the keycode
 */
typedef std::map<KeyCode,ShortkeyDestination> ShortkeyMap;

/**
 * \brief Stores all Options that may be changed in the Options Menu
 * Manages all Options that may be changed in the Options Menu. In addition, offers some functions store / save them to XML. Some option values are not stored inside the Object itself. 
 */
class Options
{
	public:
		/**
		 * \brief Enumeration of Difficulty settings
		 */
		enum Difficulty
		{
			EASY = 1,
			NORMAL = 2,
			HARD = 3,
			INSANE = 4,
		};
		
		/**
		 * \brief Returns the Instance of the Object (Singleton pattern)
		 */
		static Options* getInstance();
		
		/**
		 * \brief Initializes data structures
		 */
		void init();
		
		/**
		 * Destructor
		 */
		~Options()
		{
			
		};
		
		/**
		 * \brief Sets all options to the default values
		 */
		void setToDefaultOptions();
		
		/**
		 * \brief Writes all Option values to XML file
		 * \return true if successful, else false
		 */
		bool readFromFile(const std::string& filename);
		
		/**
		 * \brief Reads all option values from XML file
		 * \return true if operation was successful, else false
		 */
		bool writeToFile(const std::string& filename);
		
		/**
		 * \fn static void setSoundVolume(float vol)
		 * \brief set the sound volume
		 * \param vol sound volume (0-1)
		 */
		void setSoundVolume(float vol);

		/**
		 * \fn static float getSoundVolume()
		 * \brief get the sound volume
		 * \return sound volume
		 */
		float getSoundVolume();
		
		/**
		 * \brief set music volume
		 * \param vol music volume (0-1)
		 */
		void setMusicVolume(float vol);
		
		/**
		 * \brief returns the music volume
		 * \param vol music volume
		 */
		float getMusicVolume();
		
		/**
		 * \brief Checks if a keycode is a special key
		 * \param kc keycode
		 * Special keys (as Escape and Enter) may not be used as shortkeys
		 * \return true, if \a kc is a special key, false otherwise
		 */
		bool isSpecialKey(KeyCode kc)
		{
			return  m_special_keys.count(kc) > 0;
		}
		
		/**
		 * \brief Returns the locale for internationalization
		 */
		std::string getLocale();
		
		/**
		 * \brief Sets the locale for internationalization
		 * \param locale locale string, en_US for instance
		 */
		void setLocale(const std::string& locale);
		
		/**
		 * \fn bool installShortkey(KeyCode key,ShortkeyDestination dest, bool check_special=true)
		 * \brief Binds keycode \a key to the action specified by \a dest
		 * \param key keycode
		 * \param dest action to be triggered by the key
		 * \return true if the shortkey was successfully set, false otherwise
		 */
		bool setShortkey(KeyCode key,ShortkeyDestination dest);
		
		/**
		 * \fn KeyCode getMappedKey(ShortkeyDestination sd)
		 * \brief Returns the keycode, that triggers the action specified by \a sd
		 * Returns NO_KEY if there is not keycode triggering the action
		 */
		KeyCode getMappedKey(ShortkeyDestination sd);
		
		/**
		 * \brief Returns the action that is mapped to the given key
		 * \param key keycode
		 */
		ShortkeyDestination getMappedDestination(KeyCode key);
		
		/**
		 * \brief Sets the port for network communication
		 * \param port port number
		 */
		void setPort(int port)
		{
			m_port = port;
		}
		
		/**
		 * \brief Returns the port number currently used for network communication
		 */
		int getPort()
		{
			return m_port;
		}
		
		/**
		 * \brief Sets the name of the server host
		 * \param host hostname
		 */
		void setServerHost(const std::string& host)
		{
			m_server_host = host;
		}
		
		/**
		 * \brief Return the name of the server host
		 * \return hostname
		 */
		const std::string& getServerHost()
		{
			return m_server_host;
		}
		
		/**
		 * \brief Sets the maximal number of players that can participate in a game
		 * \param max_nr player limit
		 */
		void setMaxNumberPlayers(int max_nr)
		{
			m_max_players = max_nr;
		}
		
		/**
		 * \brief Returns the maximal number of players that can participate in a game
		 */
		int getMaxNumberPlayers()
		{
			return m_max_players;
		}
		
		/**
		 * \brief Sets the new difficulty
		 * \param difficulty new difficulty setting
		 */
		void setDifficulty(Difficulty difficulty)
		{
			m_difficulty = difficulty;
		}
		
		/**
		 * \brief returns the current difficulty
		 */
		Difficulty getDifficulty()
		{
			return m_difficulty;
		}
		
		/**
		 * \brief Returns the text speed factor
		 */
		float getTextSpeed()
		{
			return m_text_speed;
		}
		
		/**
		 * \brief Sets the text speed factor (factor of 1 is standard speed)
		 * \param text_speed text speed
		 */
		void setTextSpeed(float text_speed)
		{
			m_text_speed = text_speed;
		}
		
		/**
		 * \brief Returns enemy highlight color
		 */
		std::string getEnemyHighlightColor()
		{
			return m_enemy_highlight_color;
		}

		/**
		 * \brief Sets the highlight color for enemies
		 * \param color color (i.e. red, blue, green)
		 */
		void setEnemyHighlightColor(std::string color)
		{
			m_enemy_highlight_color = color;
		}
		
		/**
		 * \brief Sets the value of a debug option
		 * \param valname Name of the option
		 * \param value value as string
		 */
		void setDebugOption(std::string valname, std::string value)
		{
			m_debug_options[valname] = value;
		}
		
		/**
		 * \brief Returns the value of a debug option
		 * Returns the default, if the string is not set
		 * \param valname Name of the option
		 * \param defaultval value that is returned if no value is set
		 */
		std::string getDebugOption(std::string valname, std::string defaultval ="")
		{
			if (m_debug_options.count(valname) == 0)
				return defaultval;
			
			return m_debug_options[valname];
		}
		
		/**
		 * \brief Pushes the named value on the lua stack
		 * \param valname name of the value
		 * \return number of values pushed to the stack
		 */
		int getValue(std::string valname);
		
		/**
		 * \brief Set the named member, value is taken from lua stack
		 * \param valname of the value
		 * \param bool true if successful, else false
		 */
		bool setValue(std::string valname);

	private:
		/**
		 * Constructor
		 */
		Options();
		
		/**
		 * \var std::set<KeyCode> m_special_keys
		 * \brief Set of special key, that may not be used as shortkeys
		 */
		std::set<KeyCode> m_special_keys;
		
		/**
		 * \brief Stores the mapping of keys to actions
		 */
		ShortkeyMap m_shortkey_map;
		
		/**
		 * \var std::string m_server_host
		 * \brief IP des Servers
		 */
		std::string m_server_host;

		/**
		 * \var int m_port
		 * \brief Port ueber den der Netzverkehr laeuft
		 */
		int m_port;

		/**
		 * \var int m_max_players
		 * \brief maximale Anzahl Spieler
		 */
		int m_max_players;
		
		
		/**
		 * \brief difficulty setting
		 */
		Difficulty m_difficulty;
		
		/**
		 * \brief Factor the influences text display times
		 */
		float m_text_speed;
		
		/**
		 *\brief Enemy highlight color
		 */
		std::string m_enemy_highlight_color;
		
		/**
		 * \brief Collection of additional options used for debugging
		 */
		std::map<std::string, std::string> m_debug_options;

};
