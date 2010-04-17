#include "music.h"

#include <iostream>
#include "debug.h"

void MusicManager::play(std::string file)
{
	try
	{
		m_ogg_stream.open(file);
		if(!m_ogg_stream.playback())
		{
			ERRORMSG("Ogg refused to play file %s.",file.c_str());
			return;
		}
		
		m_musicfile = file;
		m_is_playing = true;
	}
	catch (std::string error)
	{
		ERRORMSG("Ogg errormessage: %s",error.c_str());
	}
}

void MusicManager::update()
{
	if (!m_is_playing)
		return;
	
	bool active = m_ogg_stream.update();
			
	if (active)
	{
		if(!m_ogg_stream.playing())
		{
			if(!m_ogg_stream.playback())
			{
				stop();
				ERRORMSG("Ogg abruptly stopped playing file %s",m_musicfile.c_str());
			}
			else
			{
				stop();
				ERRORMSG("Ogg stream playing file %s was interrupted",m_musicfile.c_str());
			}
		}
	}
	
	if (!active && m_musicfile != "")
	{
		stop();
	}
	
}

void MusicManager::stop()
{
	if (!m_is_playing)
		return;
	
	try
	{
		m_is_playing = false;
		m_ogg_stream.release();
	}
	catch (std::string msg)
	{
		
	}
}
