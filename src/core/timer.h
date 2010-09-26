#ifndef TIMER_H
#define TIMER_H

//#include <sys/time.h>
#include "OgreTimer.h"

/**
 * \class Timer
 * \brief Klasse zum Zeit messen
 */
class Timer
{
	
	public:
		
	/**
	 * \fn void start()
	 * \brief Startet den Timer
	 */
	void start()
	{
		mTimer = Ogre::Timer();
	}
		
	/**
	 * \fn float getTime()
	 * \brief Gibt vergangene Zeit in ms aus
	 */
	float getTime()
	{
        return (float)mTimer.getMilliseconds();
	}
	
	private:
	/**
	 * \var timeval m_tv
	 * \brief Speichert die Zeit zum Zeitpunkt des letzten Aufrufes von Start
	 */
	Ogre::Timer mTimer;
};


#endif

