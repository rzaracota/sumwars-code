#include "charconv.h"
#include "debug.h"

CharConv::CharConv()
	: m_bitstream()
{
	m_timestamp = RakNet::GetTime();
	m_bitstream.Write((char) ID_TIMESTAMP);
	m_bitstream.Write(m_timestamp);
	m_stream =0;
}

CharConv::CharConv(unsigned char* data, unsigned int size)
	: m_bitstream(data, size, false)
{
	m_timestamp = RakNet::GetTime();
	m_stream=0;
}

CharConv::CharConv(Packet* packet)
	: m_bitstream(packet->data, packet->length, false)
{
	if (packet->data[0] == ID_TIMESTAMP)
	{
		char tmp;
		m_bitstream.Read(tmp);
		m_bitstream.Read(m_timestamp);
	}
	else
	{
		m_timestamp = RakNet::GetTime();
	}
	m_stream=0;
}

void CharConv::toBuffer(const char* data, unsigned int size)
{
	if (m_stream ==0)
	{
		m_bitstream.Write(data,size);
	}
	else
	{
		m_stream->write(data,size);
	}
}

void CharConv::fromBuffer(char* data, unsigned int size)
{
	if (m_stream ==0)
	{
		m_bitstream.Read(data,size);
	}
	else
	{
		m_stream->read(data,size);
	}
}

void CharConv::toBuffer(std::string s, unsigned int size)
{
	if (m_stream ==0)
	{
		char * data = new char[size];
		memset(data,0,size);
		memcpy(data,s.data(), std::min(size,s.size()));
		toBuffer(data, size);
		delete data;
	}
	else
	{
		(*m_stream) << s << " ";
	}
}

void CharConv::fromBuffer(std::string& s, unsigned int size)
{
	if (m_stream == 0)
	{
		char* data = new char[size];
		fromBuffer(data,size);
		s.assign(data,size);
		delete data;
	}
	else
	{
		(*m_stream) >> s;
	}
}

void CharConv::toBuffer(std::string s)
{
	if (m_stream ==0)
	{
		toBuffer<int>(s.size());
		toBuffer(s.data(), s.size());
	}
	else
	{
		(*m_stream) << s << " ";
	}
}

void CharConv::fromBuffer(std::string& s)
{
	if (m_stream ==0)
	{
		int len;
		fromBuffer<int>(len);
		char * data = new char[len];
		fromBuffer(data,len);
		s.assign(data,len);
		delete data;
	}
	else
	{
		(*m_stream) >> s ;
	}
}

