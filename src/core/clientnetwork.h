#ifdef CLIENTNETWORK_H
#else
#define CLIENTNETWORK_H

#include "network.h"
#include <queue>
#include "debug.h"
#include "charconv.h"
#include <unistd.h>
#ifdef WIN32
#include <windows.h>
#endif


/**
 * \class ClientNetwork
 * \brief Klasse fuer die Repraesentation der Netzwerkschnittstelle auf der Clientseite
 */
class ClientNetwork : public Network
{
	public:
	/**
	 * \fn ClientNetwork()
	 * \brief Konstruktor
	 */
	ClientNetwork();

	/**
	 * \fn virtual  ~ClientNetwork()
	 * \brief Destruktor
	 */
	virtual ~ClientNetwork();


	/**
	 * \fn  serverConnect( char* hostname, int req_port );
	 * \brief Versucht einen Login am Server und handelt alle Verbindungen aus
	 * \param hostname Serveradresse
	 * \param req_port Authentifikationsport des Servers
	 * \return Status der Methode
	 *
	 * Verbindet sich zum auth_port des Servers hostname und Versuch eine Anmeldung mit username
	 * und password. Schlägt die Anmeldung fehl, weil der Nutzername und/oder das Passwort
	 * falsch ist wird NET_AUTH_FAILD zurueckgegeben, wenn alle Server-Slots voll sind wird
	 * NET_SLOTS_FULL zurueckgegeben oder wenn sonst ein Fehler auftritt NET_ERROR.
	 * Bei ein geglueckten Anmeldungen wird NET_CONNECTED geliefert.
	 */
	void  serverConnect( char* hostname, int req_port );

	/**
	 * \fn NetStatus serverDisconnect()
	 * \brief Schliesst die eventuelle Verbindungen zum Server
	 */
	void serverDisconnect();
	
	/**
	 * \fn void kill()
	 * \brief Schliesst das die Netzwerkverbindung
	 */
	virtual void kill();


	/**
	 * \fn  virtual void update()
	 * \brief Fuehrt fuer den Client-Slot die Abarbeitung durch
	*/
	virtual void update();

	/**
	 * \fn virtual int numberSlotMessages(int slot=0)
	 * \brief Anzahl der Packete im Empfangspuffer
	 * \param slot Nummer des Slots dessen Paketanzahl abgefragt wird -  beim Client immer 0
	 * \return Anzahl der Packete
	 *
	 * Ist der Slot ungeultig wird Null geliefert, sonst die Anzahl der Packete im Empfangspuffers
	 */
	virtual int numberSlotMessages(int slot=0);


	/**
	 * \fn virtual void popSlotMessage(Packet* &data, int slot=0)
	 * \brief Fuehrt net_pop_slot_message auf den Client-Slots
	 * \param data Puffer fuer die zu kopierenden Daten
	 * \param slot Nummer des Slots aus dem ein Paket entnommen wird -  beim Client immer 0
	 * \return Status der Methode
	 *
	 */
	virtual void popSlotMessage( Packet* &data, int slot=0);


	/**
	 * \fn virtual  void pushSlotMessage( RakNet::BitStream * data,int slot=0, PacketPriority prio= HIGH_PRIORITY,PacketReliability reliability = RELIABLE )
	 * \brief Fuehrt net_push_slot_message auf einen Server-Slots aus
	 * \param data Puffer fuer die zu kopierenden Daten
	 * \param slot Slot des Empfaengers
	 * \param prio Prioritaet mit der das Paket gesendet wird
	 * \param reliability Verlaesslichkeit mit der das Paket gesendet wird
	 */
	virtual void pushSlotMessage( RakNet::BitStream * data, int slot=0, PacketPriority prio= HIGH_PRIORITY,PacketReliability reliability = RELIABLE) ;

	/**
	 * \fn virtual NetStatus getSlotStatus( int slot=0 )
	 * \brief Liefert den Status eines Server-Slots
	 * \param slot Slotnummer des Servers (immer 0)
	 * \return Status des Slots
	 *
	 */
	virtual NetStatus getSlotStatus( int slot=0 );
	
	/**
	 * \fn void deallocatePacket(Packet* packet)
	 * \param packet zu loeschendes Paket
	 * \brief Loescht ein nicht mehr benoetigtes Paket. Um Speicherlecks zu vermeiden sollten alle Paket auf mit dieser Funktion entfernt werden
	 */
	virtual void deallocatePacket(Packet* packet)
	{
		m_peer->DeallocatePacket(packet);
	}



	private:
	/**
	 * \var PacketQueue m_received_packets
	 * \brief Liste der empfangenen Packete
	 */
	PacketQueue m_received_packets;



	/**
	 * \var NetStatus m_status
	 * \brief Status der Netzwerkschnittstelle
	 */
	NetStatus m_status;

	RakClientInterface* m_peer;

	/**
	 * \var PlayerID m_server_address
	 * \brief Adresse des Servers
	 */
	PlayerID m_server_address;
};

#endif

