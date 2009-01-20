#ifndef CHARCONV_H
#define CHARCONV_H
#include "raknet/RakNetTypes.h"
#include "raknet/BitStream.h"
#include "raknet/GetTime.h"
#include "raknet/MessageIdentifiers.h"
#include <algorithm>
#include <string>
#include <cstring>

/**
 * \class CharConv
 * \brief Klasse fuer die Konvertierung von Datenstrukturen in eine Kette aus char und umgekehrt
 */
class CharConv
{
	public:
	
	/**
	 * \fn CharConv()
	 * \brief Konstruktor
	 */
	CharConv()
	: m_bitstream()
	{
		m_timestamp = RakNet::GetTime();
		m_bitstream.Write((char) ID_TIMESTAMP);
		m_bitstream.Write(m_timestamp);
	}
	
	/**
	 * \fn CharConv(unsigned char* data, unsigned int size)
	 * \brief erstellt ein neues Objekt auf der Basis der angegebenen Daten
	 * \param data zugrunde liegende Daten
	 * \param size Laenge der Zeichenkette data
	 */
	CharConv(unsigned char* data, unsigned int size)
	: m_bitstream(data, size, false)
	{
		m_timestamp = RakNet::GetTime();
	}
	
	/**
	 * \fn CharConv(Packet* packet)
	 * \brief erstellt ein neues Objekt aus einem ueber das Netzwerk erhaltenen Datenpaket
	 * \param packet Datenpaket
	 */
	CharConv(Packet* packet)
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
		
	}
	
	/**
	 * \fn void backToStart()
	 * \brief setzt den internen Lesezeiger zurueck an den Anfang
	 */
	void backToStart()
	{
		m_bitstream.SetWriteOffset(0);
	}
	
	/**
	 * \fn void reset()
	 * \brief Loescht die zugrunde liegenden Daten
	 */
	void reset()
	{
		m_bitstream.Reset();
	}
	
	/**
	 * \fn void toBuffer(const char* data, unsigned int size)
	 * \brief Schreibt Daten in den Puffer
	 * \param data zu schreibende Daten
	 * \param size Laenge der zu schreibenden Daten
	 */
	void toBuffer(const char* data, unsigned int size)
	{
		m_bitstream.Write(data,size);
	}
	
	/**
	 * \fn void fromBuffer(char* data, unsigned int size)
	 * \brief Liest Daten als String aus dem Puffer
	 * \param data  Ausgabeparameter fuer die Daten
	 * \param size Anzahl der Zeichen die gelesen werden
	 */
	void fromBuffer(char* data, unsigned int size)
	{
		m_bitstream.Read(data,size);
	}
	
	/**
	 * \fn void toBuffer(std::string s, unsigned int size)
	 * \brief Schreibt aus dem String die ersten size Zeichen in den Puffer
	 * \param s String
	 * \param size Anzahl Zeichen
	 */
	void toBuffer(std::string s, unsigned int size)
	{
		char * data = new char[size];
		memset(data,0,size);
		memcpy(data,s.data(), std::min(size,s.size()));
		toBuffer(data, size);
		delete data;
	}
	
	/**
	 * \fn void fromBuffer(std::string s, unsigned int size)
	 * \brief Liest aus dem String size Zeichen und schreibt sie in den String
	 * \param s String
	 * \param size Anzahl Zeichen
	 */
	void fromBuffer(std::string s, unsigned int size)
	{
		char* data = new char[size];
		fromBuffer(data,size);
		s.assign(data,size);
		delete data;
	}
	
	/**
	 * \fn void toBuffer(T data)
	 * \brief Schreibt Daten in den Puffer
	 * \param data zu schreibende Daten
	 */
	template<class T>
	void toBuffer(T data)
	{
		m_bitstream.Write(data);
	}
	
	/**
	 * \fn void fromBuffer(T &data)
	 * \brief Liest Daten aus dem Puffer
	 * \param data Ausgabeparameter fuer die Daten
	 */
	template<class T>
	void fromBuffer(T &data)
	{
		m_bitstream.Read(data);
	}
	
	/**
	 * \fn void toBuffer(std::string s)
	 * \brief Schreibt einen String in den Buffer, indem zuerst die Laenge und dann die Daten geschrieben werden
	 * \param s String
	 */
	void toBuffer(std::string s)
	{
		toBuffer<int>(s.size());
		toBuffer(s.data(), s.size());
	}
	
	/**
	 * \fn void fromBuffer(std::string s)
	 * \brief liest einen String aus dem Puffer
	 * \param s String
	 */
	void fromBuffer(std::string s)
	{
		int len;
		fromBuffer<int>(len);
		char * data = new char[len];
		fromBuffer(data,len);
		s.assign(data,len);
		delete data;
	}
	
	
	
	/**
	 * \fn RakNet::BitStream* getBitStream()
	 * \brief Gibt den Bitstream der die Daten enthaelt aus
	 */
	RakNet::BitStream* getBitStream()
	{
		return &m_bitstream;
	}
	
	/**
	 * \fn int readBits()
	 * \brief Gibt die Anzahl der bisher gelesenen Bits aus
	 */
	int readBits()
	{
		return m_bitstream.GetReadOffset();
	}
	
	/**
	 * \fn int writeBits()
	 * \brief Gibt die Anzahl der bisher geschriebenen Bits aus
	 */
	int writeBits()
	{
		return m_bitstream.GetNumberOfBitsUsed();
	}
	
	/**
	 * \fn float getDelay()
	 * \brief Gibt die Verzoegerung des Paketes in Millisekunden an
	 */
	float getDelay()
	{
		return std::max(0.0f,static_cast<float>(RakNet::GetTime())-static_cast<float>(m_timestamp));
	}
	
	/**
	 * \fn unsigned long getTimestamp()
	 * \brief Gibt den Zeitstempel des Paketes aus
	 */
	unsigned long getTimestamp()
	{
		return m_timestamp;
	}
	
	
	private:
		/**
		 * \var RakNet::BitStream m_bitstream
		 * \brief enthaelt die Daten als Zeichenkette
		 */
		RakNet::BitStream m_bitstream;
		
		/**
		 * \var unsigned long m_timestamp
		 * \brief Zeit zur das Paket erstellt bzw erhalten wurde
		 */
		unsigned long m_timestamp;
	
};






#endif // CHARCONV_H
