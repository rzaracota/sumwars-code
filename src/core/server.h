#ifndef SERVER_H
#define SERVER_H

#include <unistd.h>
#include "debug.h"
#include "servernetwork.h"
#include <list>


#include "world.h"
//#include "mapgenerator.h"

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
//#include <GL/gl.h>
//#include <GL/glut.h>

// includes zum spaeter wieder rausnehmen...
#include "warrior.h"
#include "mage.h"
#include "archer.h"
#include "priest.h"
#include "fixedobject.h"
#include "monster.h"
#include "goblin.h"
#include "spawnpoint.h"

#include "objectfactory.h"

#include "timer.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <pthread.h>

/**
 * \class Server
 * \brief Klasse fuer einen Spielserver
 */
class Server
{
	public:
	
	/**
	 * \struct StartArgs
	 * \brief Argumentliste fuer den Start des Serverthreads
	 */
	struct StartArgs
	{
		Server* m_server;
	};
		
	/**
	 * \fn  Server()
	 * \brief Konstruktor
	 */
	Server();
	
	/**
	 * \fn ~Server()
	 * \brief Destruktor
	 */
	~Server();
	
	/**
	 * \fn void init(int max_slots)
	 * \brief initialisiert den Server
	 */
	void init(int max_slots);
	
	/**
	 * \fn void run()
	 * \brief  Startet den Server
	 */
	void run();
	
	/**
	 * \fn void loop()
	 * \brief die Hauptspielschleife
	 */
	void loop();
	
	/**
	 * \fn void stop()
	 * \brief Stoppt den Server
	 */
	void stop();
	
	/**
	 * \fn void acceptLogins()
	 * \brief Fuegt Spieler die sich neu eingeloggt haben ins Spiel ein
	 */
	void acceptLogins();
	
	/**
	 * \fn void updateLogins()
	 * \brief Aktualisiert die Liste der eingehenden Logins
	 */
	 void updateLogins();
	
	 /**
	  * \fn static void* startServer(void* arg)
	  * \brief Startet den Server in einem eigenen Thread
	  */
	static void* startServer(void* arg);
	
	private:
	/**
	 * \var bool m_stop
	 * \brief gibt an, ob der Server gestoppt werden soll
	 */
	bool m_stop;
	
	/**
	 * \var pthread_t m_server_thread
	 * \brief Thread Handel fuer den Thread des Servers
	 */
	pthread_t m_server_thread;
	
	/**
	 * \var World *m_world
	 * \brief Zeiger auf die Welt
	 */
	World *m_world;
	
	/**
	 * \var ServerNetwork * m_network
	 * \brief Zeiger auf das Netzwerk
	 */
	ServerNetwork * m_network;

	/**
	 * \var list<int> m_logins
	 * \brief Liste der Spieler die sich gerade einloggen wollen
	 */
	list<int> m_logins;
};


#endif

