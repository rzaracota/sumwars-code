#ifndef CHARCONV_H
#define CHARCONV_H
#include "raknet/RakNetTypes.h"
#include "raknet/BitStream.h"
#include "raknet/GetTime.h"
#include "raknet/MessageIdentifiers.h"
#include <algorithm>
#include <string>
#include <cstring>
#include <iostream>

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
	CharConv();
	
	/**
	 * \fn CharConv(int dummy)
	 * \brief Erzeugt ein Objekt mit leerem Bitstream (Standard ist, dass eine Kennung und ein Zeitstempel geschrieben wird)
	 * \param dummy Dummy Parameter um eine unterschiedliche Signatur zu erzeugen
	 */
	CharConv(int dummy);
	
	/**
	 * \fn CharConv(std::iostream* stream)
	 * \param stream Datenstrom, in den Daten geschrieben werden
	 */
	CharConv(std::iostream* stream)
	{
		m_stream = stream;
	}
	
	/**
	 * \fn CharConv(unsigned char* data, unsigned int size)
	 * \brief erstellt ein neues Objekt auf der Basis der angegebenen Daten
	 * \param data zugrunde liegende Daten
	 * \param size Laenge der Zeichenkette data
	 */
	CharConv(unsigned char* data, unsigned int size);
	
	/**
	 * \fn CharConv(Packet* packet)
	 * \brief erstellt ein neues Objekt aus einem ueber das Netzwerk erhaltenen Datenpaket
	 * \param packet Datenpaket
	 */
	CharConv(Packet* packet);
	
	
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
	void toBuffer(const char* data, unsigned int size);
	
	
	/**
	 * \fn void fromBuffer(char* data, unsigned int size)
	 * \brief Liest Daten als String aus dem Puffer
	 * \param data  Ausgabeparameter fuer die Daten
	 * \param size Anzahl der Zeichen die gelesen werden
	 */
	void fromBuffer(char* data, unsigned int size);
	
	/**
	 * \fn void toBuffer(std::string s, unsigned int size)
	 * \brief Schreibt aus dem String die ersten size Zeichen in den Puffer
	 * \param s String
	 * \param size Anzahl Zeichen
	 */
	void toBuffer(std::string s, unsigned int size);
	
	
	/**
	 * \fn void fromBuffer(std::string& s, unsigned int size)
	 * \brief Liest aus dem String size Zeichen und schreibt sie in den String
	 * \param s String
	 * \param size Anzahl Zeichen
	 */
	void fromBuffer(std::string& s, unsigned int size);
	
	/**
	 * \fn void toBuffer(char c)
	 * \brief Schreibt einen char in den Buffer
	 * \param c char
	 */
	void toBuffer(char c);
	
	
	/**
	 * \fn void fromBuffer(char &c)
	 * \brief liest einen char aus dem Puffer
	 * \param c char
	 */
	void fromBuffer(char &c);
	
	/**
	 * \fn void toBuffer(T data)
	 * \brief Schreibt Daten in den Puffer
	 * \param data zu schreibende Daten
	 */
	template<class T>
	void toBuffer(T data)
	{
		if (m_stream ==0)
		{
			m_bitstream.Write(data);
		}
		else
		{
			(*m_stream) << data << " ";
		}
	}
	
	/**
	 * \fn void fromBuffer(T &data)
	 * \brief Liest Daten aus dem Puffer
	 * \param data Ausgabeparameter fuer die Daten
	 */
	template<class T>
	void fromBuffer(T &data)
	{
		if (m_stream ==0)
		{
			m_bitstream.Read(data);
		}
		else
		{
			(*m_stream) >> data;
		}
	}
	
	/**
	 * \fn void toBuffer(std::string s)
	 * \brief Schreibt einen String in den Buffer, indem zuerst die Laenge und dann die Daten geschrieben werden
	 * \param s String
	 */
	void toBuffer(std::string s);
	
	
	/**
	 * \fn void fromBuffer(std::string s)
	 * \brief liest einen String aus dem Puffer
	 * \param s String
	 */
	void fromBuffer(std::string& s);
	
	
	
	
	/**
	 * \fn RakNet::BitStream* getBitStream()
	 * \brief Gibt den Bitstream der die Daten enthaelt aus
	 */
	RakNet::BitStream* getBitStream()
	{
		return &m_bitstream;
	}
	
	/**
	 * \fn std::iostream* getStream()
	 * \brief  Gibt den Stream aus
	 */
	std::iostream* getStream()
	{
		return m_stream;
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
	
	/**
	 * \fn void printNewline()
	 * \brief Schreibt Newline (Debugging)
	 */
	void printNewline()
	{
		if (m_stream !=0)
			(*m_stream) << "\n";
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
		
		/**
		 * \var std::iostream* m_stream
		 * \brief Ein/Ausgabestrom - wenn nicht auf 0 gesetzt wird dieser Strom verwendet
		 */
		std::iostream* m_stream;
	
};



#endif // CHARCONV_H
