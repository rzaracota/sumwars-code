#ifndef CHARCONV_H
#define CHARCONV_H
#include "raknet/RakNetTypes.h"
#include "raknet/BitStream.h"

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
		
	}
	
	/**
	 * \fn CharConv(Packet* packet)
	 * \brief erstellt ein neues Objekt aus einem ueber das Netzwerk erhaltenen Datenpaket
	 * \param packet Datenpaket
	 */
	CharConv(Packet* packet)
	: m_bitstream(packet->data, packet->length, false)
	{
		
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
	 * \fn void toBuffer(char* data, unsigned int size)
	 * \brief Schreibt Daten in den Puffer
	 * \param data zu schreibende Daten
	 * \param size Laenge der zu schreibenden Daten
	 */
	void toBuffer(char* data, unsigned int size)
	{
		m_bitstream.Write(data,size);
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
	 * \fn RakNet::BitStream* getBitStream()
	 * \brief Gibt den Bitstream der die Daten enthaelt aus
	 */
	RakNet::BitStream* getBitStream()
	{
		return &m_bitstream;
	}
			
	
	private:
		/**
		 * \var RakNet::BitStream m_bitstream
		 * \brief enthaelt die Daten als Zeichenkette
		 */
		RakNet::BitStream m_bitstream;
	
};






#endif // CHARCONV_H
