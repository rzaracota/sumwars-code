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
	 * \fn  virtual void update()
	 * \brief Fuehrt fuer den Client-Slot die Abarbeitung durch
	*/
	virtual void update();

	/**
	 * \fn virtual int numberSlotMessages(int slot=0)
	 * \brief Anzahl der Packete im Empfangspuffer
	 * \return Anzahl der Packete
	 *
	 * Ist der Slot ungeultig wird Null geliefert, sonst die Anzahl der Packete im Empfangspuffers
	 */
	virtual int numberSlotMessages(int slot=0);


	/**
	 * \fn virtual void popSlotMessage(Packet* &data, int slot=0)
	 * \brief Fuehrt net_pop_slot_message auf den Client-Slots
	 * \param data Puffer fuer die zu kopierenden Daten
	 * \param size Groesse des Datenblocks
	 * \return Status der Methode
	 *
	 */
	virtual void popSlotMessage( Packet* &data, int slot=0);


	/**
	 * \fnvirtual  void pushSlotMessage( RakNet::BitStream * data,int slot=0, PacketPriority prio= HIGH_PRIORITY,PacketReliability reliability = RELIABLE )
	 * \brief Fuehrt net_push_slot_message auf einen Server-Slots aus
	 * \param data Puffer fuer die zu kopierenden Daten
	 * \param slot Slot des Empfaengers
	 * \return Status der Methode
	 *
	 */
	virtual void pushSlotMessage( RakNet::BitStream * data, int slot=0, PacketPriority prio= HIGH_PRIORITY,PacketReliability reliability = RELIABLE) ;

	/**
	 * \fn virtual NetStatus getSlotStatus( int slot=0 )
	 * \brief Liefert den Status eines Server-Slots
	 * \param slot Slotnummer des Servers
	 * \return Status des Slots
	 *
	 */
	virtual NetStatus getSlotStatus( int slot=0 );



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


	/**
	 * \var SystemAddress m_server_address
	 * \brief Adresse des Servers
	 */
	SystemAddress m_server_address;
};

#endif

