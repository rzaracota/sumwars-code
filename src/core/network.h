#ifdef NETWORK_H
#else
#define NETWORK_H

#include "raknet/MessageIdentifiers.h"
#include "raknet/RakNetworkFactory.h"
#include "raknet/RakPeerInterface.h"
#include "raknet/RakNetTypes.h"
#include "raknet/BitStream.h"
#include "debug.h"
#include <queue>

#define REQ_PORT 5331

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
		Network();
		
		/**
		 * \fn NetStatus init( int auth_port )
		 * \brief Initialisiert den Server, oeffnet die Netzwerkschnittstelle
		 * \param auth_port gibt den Port an, auf dem sich Clients anmelden koennen
		 * \return Status der Methode
		 *
		 */
		virtual NetStatus init( int auth_port=REQ_PORT )
		{
			return NET_OK;
		};
		
		/**
		 * \fn void kill()
		 * \brief Schliesst das die Netzwerkverbindung
		 */
		virtual void kill();
		
		/**
		 * \fn  void update()
		 * \brief Sendet / Empfaengt die Daten
		 */
		virtual void update()=0;
		
		/**
		 * \fn unsigned char getPacketIdentifier(Packet *p)
		 * \brief Extrahier aus einem Packet die Identifikationsnummer
		 */
		unsigned char getPacketIdentifier(Packet *p);
		
		/**
		 * \fn int numberSlotMessages()
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
		 * \param size Groesse des Datenblocks
		 * \return Status der Methode
		 *
		 */
		virtual void popSlotMessage( Packet* &data, int slot=0) =0;
		
		/**
		 * \fn void pushSlotMessage( RakNet::BitStream * data,int slot=0, PacketPriority prio= HIGH_PRIORITY,PacketReliability reliability = RELIABLE )
		 * \brief Fuehrt net_push_slot_message auf einen Server-Slots aus
		 * \param data Puffer fuer die zu kopierenden Daten
		 * \param slot Slot des Empfaengers
		 * \return Status der Methode
		 *
		 */
		virtual void pushSlotMessage( RakNet::BitStream * data, int slot=0, PacketPriority prio= HIGH_PRIORITY,PacketReliability reliability = RELIABLE) =0;
		
		/**
		 * \fn void deallocatePacket(Packet* packet)
		 * \brief Loescht ein nicht mehr benoetigtes Paket. Um Speicherlecks zu vermeiden sollten alle Paket auf mit dieser Funktion entfernt werden
		 */
		void deallocatePacket(Packet* packet)
		{
			m_peer->DeallocatePacket(packet);
		}
		
		/**
		 * \fn NetStatus getSlotStatus( int slot=0 )
		 * \brief Liefert den Status eines Server-Slots
		 * \param slot Slotnummer des Servers
		 * \return Status des Slots
		 *
		 */
		virtual NetStatus getSlotStatus( int slot=0 ) =0;
		
		
	protected:
		/**
		 * \var RakPeerInterface* m_peer
		 * \brief Netzwerkschnittstelle von RakNet
		 */
		RakPeerInterface* m_peer;
		
};

typedef std::queue<Packet*> PacketQueue;

#endif

