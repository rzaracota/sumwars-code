#ifndef __ogg_h__
#define __ogg_h__

#include <string>
#include <iostream>
using namespace std;

#ifdef WIN32
	#include <al.h>
#else
	#include <AL/al.h>
#endif

#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>



#define BUFFER_SIZE (4096 * 16)



class ogg_stream
{
    public:
	ogg_stream()
	{
		m_volume = 1;
	}
	
        void open(string path);
        void release();
        void display();
        bool playback();
        bool playing();
        bool update();
		
		void setVolume(float volume)
		{
			if (playing())
			{
				alSourcef (source, AL_GAIN,volume);
			}
			m_volume = volume;
		}

    protected:

        bool stream(ALuint buffer);
        void empty();
        void check();
        string errorString(int code);

    private:

        FILE*           oggFile;
        OggVorbis_File  oggStream;
        vorbis_info*    vorbisInfo;
        vorbis_comment* vorbisComment;

        ALuint buffers[2];
        ALuint source;
        ALenum format;
		float m_volume;
};


#endif // __ogg_h__
