#ifdef NETWORK_H
#else
#define NETWORK_H

#include "debug.h"


/*
#include "raknet/MessageIdentifiers.h"
#include "raknet/RakNetworkFactory.h"
#include "raknet/RakPeerInterface.h"
#include "raknet/RakNetTypes.h"
#include "raknet/BitStream.h"
*/
#ifndef NO_RAKNET
#include "raknet/PacketEnumerations.h"
#include "raknet/RakNetworkFactory.h"
#include "raknet/RakPeerInterface.h"
#include "raknet/NetworkTypes.h"
#include "raknet/BitStream.h"
#include "raknet/RakClientInterface.h"
#include "raknet/RakServerInterface.h"

#else // #ifndef NO_RAKNET
	// dummy classes to preserve the interface of Network classes
	class Packet
	{
	};
	
	namespace RakNet
	{
		class BitStream
		{
			public:
			char* GetData()
			{
				ERRORMSG("Called RakNet lib in NO_RAKNET build");
				return 0;
			}
		};
	}
	
	class RakServerInterface {};
	class RakClientInterface {};
	class PlayerID {};
	
	enum PacketPriority
	{
		SYSTEM_PRIORITY,   //!< System priority is for system related messaging.  Don't use it.
  HIGH_PRIORITY,   //!< Those message are handle first
  MEDIUM_PRIORITY,   //!< Message relativly important
  LOW_PRIORITY,   //!< Not critical information
  NUMBER_OF_PRIORITIES
	};
/**
	 * This define the reliability behaviour to apply to a packet
	 * 
	 * @note  Note to self: I write this with 3 bits in the stream!
	 *
 */

	enum PacketReliability
	{
		UNRELIABLE,   //!< Send packet not reliable and not sequenced
  UNRELIABLE_SEQUENCED,  //!< Send packet not reliable but sequenced
  RELIABLE,   //!< Send packet reliable
  RELIABLE_ORDERED,   //!< Send packet reliable respecting ordering
  RELIABLE_SEQUENCED //!< Send packet reliable respecting sequenced
	};

#endif

#define ID_USER_PACKET_ENUM  80


#include <queue>



/**
 * \enum NetStatus
 * \brief Aufzaehlung von Zustaenden der Netzwerkschnittstelle
 */
enum NetStatus
{
	NET_REJECTED = -5,
 NET_SLOTS_FULL=-4,	/*!< Server-Slots sind voll */
 NET_TIMEOUT=-3,		/*!< Zeit fuer Operation ist */
 NET_CLOSE=-2,		/*!< Verbindung wurde geschlossen */
 NET_ERROR=-1,		/*!< Ein Fehler ist aufgetreten */
 NET_OK=0,		/*!< Operation erfolgreich abgeschlossen */
 NET_QUEUE_EMPTY,	/*!< Puffer ist leer */
 NET_CONNECTED		/*!< Verbindung hergestellt */
};

/**
 * \enum SpecialSlot
 * \brief besondere Slotnummern
 */
enum SpecialSlot
{
	LOCAL_SLOT = -1,
	NOSLOT = -2,
};

/**
 * \class Network
 * \brief Basisklasse fuer Netzwerkinterface
 */
class Network
{
	public:
		/**
		 * \fn Network()
		 * \brief Konstruktor
		 */
		Network()
		{

		}

		/**
		 * \fn virtual ~Network()
		 * \brief Destruktor
		 */
		virtual ~Network()
		{

		}


		/**
		 * \fn NetStatus init( int auth_port )
		 * \brief Initialisiert den Server, oeffnet die Netzwerkschnittstelle
		 * \param auth_port gibt den Port an, auf dem sich Clients anmelden koennen
		 * \return Status der Methode
		 *
		 */
		virtual NetStatus init( int auth_port )
		{
			return NET_OK;
		};

		/**
		 * \fn void kill()
		 * \brief Schliesst das die Netzwerkverbindung
		 */
		virtual void kill() =0;

		/**
		 * \fn  void update()
		 * \brief Sendet / Empfaengt die Daten
		 */
		virtual void update()=0;

		/**
		 * \fn unsigned char getPacketIdentifier(Packet *p)
		 * \param p Paket
		 * \brief Extrahier aus einem Packet die Identifikationsnummer
		 */
		unsigned char getPacketIdentifier(Packet *p);

		/**
		 * \fn virtual int numberSlotMessages(int slot=0) =0;
		 * \param slot Nummer des Slots dessen Paketanzahl abgefragt wird
		 * \brief Anzahl der Packete im Empfangspuffer
		 * \return Anzahl der Packete
		 *
		 * Ist der Slot ungeultig wird Null geliefert, sonst die Anzahl der Packete im Empfangspuffers
		 */
		virtual int numberSlotMessages(int slot=0) =0;


		/**
		 * \fn void popSlotMessage(Packet* &data, int slot=0)
		 * \brief Fuehrt net_pop_slot_message auf den Client-Slots
		 * \param data Puffer fuer die zu kopierenden Daten
		 * \param slot Nummer des Slots aus dem ein Paket entnommen wird
		 */
		virtual void popSlotMessage( Packet* &data, int slot=0) =0;

		/**
		 * \fn void pushSlotMessage( RakNet::BitStream * data,int slot=0, PacketPriority prio= HIGH_PRIORITY,PacketReliability reliability = RELIABLE )
		 * \brief Fuehrt net_push_slot_message auf einen Server-Slots aus
		 * \param data Puffer fuer die zu kopierenden Daten
		 * \param slot Slot des Empfaengers
		 * \param prio Prioritaet mit der das Paket gesendet wird
		 * \param reliability Verlaesslichkeit mit der das Paket gesendet wird
		 *
		 */
		virtual void pushSlotMessage( RakNet::BitStream * data, int slot=0, PacketPriority prio= HIGH_PRIORITY,PacketReliability reliability = RELIABLE) =0;

		/**
		 * \fn void deallocatePacket(Packet* packet)
		 * \param packet zu loeschendes Paket
		 * \brief Loescht ein nicht mehr benoetigtes Paket. Um Speicherlecks zu vermeiden sollten alle Paket auf mit dieser Funktion entfernt werden
		 */
		virtual void deallocatePacket(Packet* packet) =0;

		/**
		 * \fn NetStatus getSlotStatus( int slot=0 )
		 * \brief Liefert den Status eines Server-Slots
		 * \param slot Slotnummer
		 * \return Status des Slots
		 *
		 */
		virtual NetStatus getSlotStatus( int slot=0 ) =0;



};

/**
 * Warteschlange von Netzwerkpaketen
 */
typedef std::queue<Packet*> PacketQueue;

#endif

