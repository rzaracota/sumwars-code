#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>

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
		gettimeofday(&m_tv, NULL);
	}
		
	/**
	 * \fn float getTime()
	 * \brief Gibt vergangene Zeit in ms aus
	 */
	float getTime()
	{
		timeval tv2;
		gettimeofday(&tv2, NULL);
		
		return (tv2.tv_sec-m_tv.tv_sec)*1000.0 + (tv2.tv_usec-m_tv.tv_usec)*0.001;
   
	}
	
	private:
	/**
	 * \var timeval m_tv
	 * \brief Speichert die Zeit zum Zeitpunkt des letzten Aufrufes von Start
	 */
	timeval m_tv;
	 
	 
};


#endif

