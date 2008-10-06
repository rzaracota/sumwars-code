/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Hans Wulf

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef WORLD_H
#define WORLD_H



#include <sys/time.h>

#include <string>
#include <list>
#include <map>
#include <math.h>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <queue>

#include "worldobject.h"
#include "projectile.h"
#include "trade.h"
#include "servernetwork.h"
#include "clientnetwork.h"
#include "region.h"
#include "pathfind.h"
#include "party.h"
#include "itemfactory.h"
#include "objectfactory.h"
#include "event.h"

/**
 * \def WORLD_MAX_REGIONS
 * \brief Anzahl maximaler Regionen die gleichzeitig im Speicher sind
 * \def sqr(X)
 * \brief Quadratfunktion
 */
#define WORLD_MAX_REGIONS 100

#define sqr(X) ((X) * (X))

/**
 * \file world.h
 * \defgroup world \ Welt
 * \brief Deklaration der World
 * \author Hans Wulf
 * \version 1.0
 * \date 2007/05/28
 */

/**
 * Liste von Slotnommern auf denen neue Logins eingegangen sind
 */
typedef std::list<int> LoginList;

/**
 * \class World
 * \brief Hauptobjekt der Anwendung
 *
 * Dieses Objekt ist das Hauptobjekt der Anwendung. Die wesentliche Aufgabe liegt in der Verwaltung der WorldObject Objekte.
 */
class World {
//Public stuff
public:
	//Fields
	//Constructors
	/**
	 * \fn World(bool server)
	 * \param server Gibt an, ob der Rechner der Server ist
	 * \brief Konstruktor
	 *
	 * Legt ein neues World Objekt an
	 */
	World(bool server);
	
	
	/**
	* \fn ~World()
	 * \brief Destruktor
	 */
	~World();
	
	
	
	/**
	 * \fn bool init()
	 * \brief initialisiert die Welt
	 * \return true, wenn kein Fehler aufgetreten ist, sonst false
	 */
	bool init();
	
	/**
	 * \fn void createRegion(short region)
	 * \brief Erzeugt die Region mit der angegebenen Nummer
	 * \param region Nummer der Region, die erzeugt werden soll
	 */
	void createRegion(short region);
	
	
	/**
	 * \fn void acceptLogins()
	 * \brief Fuegt Spieler die sich neu eingeloggt haben ins Spiel ein
	 */
	void acceptLogins();
	
	/**
	 * \fn void updateLogins()
	 * \brief Aktualisiert die Liste der eingehenden Logins
	 */
	void updateLogins();
	
	//Accessor Methods
	/**
	 * \fn short insertRegion(Region* region, int rnr)
	 * \brief Fuegt eine neue Region in die Welt ein
	 * \param region Einzufuegende Region
	 * \param rnr ID der Region
	 * \return gibt die ID der Region an, die an allen Objekten in der Region gespeichert wird. Im Falle eines Fehlers wird -1 zurueckgegeben.
	 */
	short insertRegion(Region* region, int rnr);
	
	/**
	 * \fn bool insertPlayerIntoRegion(WorldObject* player, short region)
	 * \brief Versucht einen Spieler in eine Region einzufuegen
	 */
	bool insertPlayerIntoRegion(WorldObject* player, short region);

	//Operations
	/**
	 * \fn static bool intersect(Shape* s1, Shape* s2)
	 * \brief Gibt true aus, wenn die angegebenen Formen sich ueberschneiden, sonst false
	 * \param s1 erste Form
	 * \param s2 zweite Form
	 */
	static bool intersect(Shape* s1, Shape* s2);


	/**
	 * \fn Trade* getTrade( int id)
	 * \brief Sucht Handelsvorgang anhand seiner ID
	 * \param id ID des gesuchten Handelsvorganges
	 * \return Zeiger auf den gefundenen Handel
	 *
	 * W&auml;hlt einen Handel anhand seiner ID aus. Gibt einen Zeiger auf das Handelsobjekt zur&uuml;ck, beziehungsweise NULL wenn das Objekt nicht gefunden wurde.
	 */
	Trade*  getTrade ( int id);


	/**
	 * \fn int newTrade(int trader1_id, int trader2_id)
	 * \brief Legt einen neuen Handelsvorgang an
	 * \param trader1_id ID des ersten Handelspartners
	 * \param trader2_id ID des zweiten Handelsparters
	 * \return ID des neu angelegten Handelsvorgangs
	 * Legt einen neuen Handelsvorgang an und gibt dessen ID aus. Wenn das anlegen des Handelsvorganges nicht erfolgreich war, so wird 0 ausgegeben
	 */
	int newTrade(int trader1_id, int trader2_id);

	/**
	 * \fn WorldObject* getObject ( int id, short rid)
	 * \brief Sucht WorldObject anhand seiner ID
	 * \param id ID des gesuchten WorldObjects
	 * \param rid ID der Region
	 * \return Zeiger auf das gefundene WorldObject
	 *
	 * W&auml;hlt ein WorldObject anhand seiner ID aus. Gibt einen Zeiger auf das Objekt zur&uuml;ck, beziehungsweise NULL wenn das Objekt nicht gefunden wurde.
	 */
	WorldObject* getObject ( int id, short rid);


	/**
	 * \fn bool getObjectsInShape( Shape* shape, short region, WorldObjectList* result,short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL, WorldObject* omit=0 )
	 * \brief Sucht WorldObjects innerhalb eines Gebietes
	 * \param shape Form des Gebietes
	 * \param layer Ebene in der gesucht wird
	 * \param region Region in der gesucht wird
	 * \param result Liste, an die die gefundenen Objekte angehangen werden
	 * \param layer Ebene in der gesucht wird
	 * \param group Gruppen die durchsucht werden sollen
	 * \param omit Objekt, das ausgelassen wird
	 * \return bool, der angibt, ob die Operation erfolgreich war
	 *
	 * Alle gefundenen Objekte werden an das Ende der Liste result angehangen.
	 * Wenn f&uuml;r selector ein NULL-Zeiger &uuml;bergeben wird, so werden die Objekte nicht selektiert.
	 * Wenn f&uuml;r result ein NULL-Zeiger &uuml;bergeben wird, oder ein anderer Fehler auftritt, so wird false zur&uuml;ckgegeben, sonst true.
	 */
	bool getObjectsInShape( Shape* shape, short region, WorldObjectList* result,short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL, WorldObject* omit=0 );


	/**
	 * \fn WorldObject* getObjectAt(float x_coordinate, float y_coordinate,  short region,short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL)
	 * \brief Sucht WorldObject an den gegebenen Koordinaten
	 * \param x_coordinate x-Koordinate des Punktes
	 * \param y_coordinate y-Koordinate des Punktes
	 * \param layer Ebene in der gesucht wird
	 * \param region Region in der gesucht wird
	 * \param layer Ebene in der gesucht wird
	 * \param group Gruppen die durchsucht werden sollen
	 * \return Zeiger auf das WorldObject, wenn sich an dem Punkt eines befindet, sonst NULL
	 */
	WorldObject* getObjectAt(float x_coordinate, float y_coordinate,  short region,short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL);

	/**
	 * \fn void getObjectsOnLine( float xstart, float ystart, float xend, float yend,  short region, WorldObjectList* result,short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL, WorldObject* omit=0 )
	 * \brief Sucht alle Objekte die auf der angegebenen Linie liegen
	 * \param xstart x-Koordinate Startpunkt
	 * \param ystart y-Koordinate Startpunkt
	 * \param xend x-Koordinate Endpunkt
	 * \param yend y-Koordinate Endpunkt
	 * \param region Region in der gesucht wird
	 * \param result Liste, an die die gefundenen Objekte angehangen werden
	 * \param layer Ebene in der gesucht wird
	 * \param group Gruppen die durchsucht werden sollen
	 * \param omit Objekt, das ausgelassen wird
	 */
	void getObjectsOnLine( float xstart, float ystart, float xend, float yend,  short region, WorldObjectList* result,short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL, WorldObject* omit=0 );

	/**
	 * \fn void getProjectilesOnScreen(float center_x,float center_y,short region, std::list<Projectile*>* result)
	 * \brief Gibt alle Projektile auf dem Bildschirm aus
	 * \param center_x x-Koordinate Zentrum
	 * \param center_y y-Koordinate Zentrum
	 * \param region Region
	 * \param result Liste in die die Objekte eingeordnet werden
	 */
	void getProjectilesOnScreen(float center_x,float center_y,short region, std::list<Projectile*>* result);


	/**
	 * \fn bool  insertObject (WorldObject* object, float x, float y, short region)
	 * \brief F&uuml;gt WorldObject ein
	 * \param object Zeiger auf das Objekt, welches eingef&uuml;gt werden soll
	 * \param x x-Koordinate
	 * \param y y-Koordinate
	 * \param region Region in der sich das Projektil befindet
	 * \return bool, der angibt, ob die Operation erfolgreich war
	 *
	 * F&uuml;gt das WorldObject in die internen Datenstrukturen ein. Wenn das Einf&uuml;gen erfolgreich war, so wird true zur&uuml;ckgegeben, sonst false.
	 * Wenn ein NULL-Zeiger &uuml;bergeben wird, so wird false ausgegeben.
	 */
	bool  insertObject (WorldObject* object, float x, float y, short region);
	
	/**
	 * \fn bool insertPlayer(WorldObject* player, int slot = NOSLOT)
	 * \brief Fuegt einen neuen Spieler hinzu
	 */
	bool insertPlayer(WorldObject* player, int slot= NOSLOT);

	/**
	 * \fn bool  insertProjectile(Projectile* object, float x, float y, short region)
	 * \brief Fuegt ein Projektil ein
	 * \param object Zeiger auf das Objekt, welches eingef&uuml;gt werden soll
	 * \param x x-Koordinate
	 * \param y y-Koordinate
	 * \param region Region in der sich das Projektil befindet
	 * \return bool, der angibt, ob die Operation erfolgreich war
	 *
	 */
	bool  insertProjectile(Projectile* object, float x, float y, short region);

	
	
	/**
	 * \fn deleteObject(WorldObject* object)
	 * \brief L&ouml;scht WorldObject
	 * \param object Zeiger auf das Objekt, welches gel&ouml;scht werden soll
	 * \return bool, der angibt, ob die Operation erfolgreich war
	 *
	 * L&ouml;scht das WorldObject aus den internen Datenstrukturen. Das Objekt selbst wird nicht gel&ouml;scht, es wird nur die Einbindung in die Datenstrukturen entfernt. Wenn das L&ouml;schen erfolgreich war, so wird true zur&uuml;ckgegeben, sonst false.
	 * Wenn ein NULL-Zeiger &uuml;bergeben wird, so wird false ausgegeben.
	 */
	bool  deleteObject (WorldObject* object);

	 /**
	  * \fn moveObject(WorldObject* object, float x, float y)
	  * \brief verschiebt ein WorldObject an den Punkt (x,y)
	  * \param object Zeiger auf das Objekt, welches verschoben werden soll
	  * \param x x-Koordinate des Zielpunktes, zu dem das Objekt verschoben werden soll
	  * \param y y-Koordinate des Zielpunktes, zu dem das Objekt verschoben werden soll
	  * \return bool, welcher angibt, ob das Verschieben erfolgreich war
	  */
	bool moveObject(WorldObject* object, float x, float y);

	/**
	 * \fn static bool lineIntersect(float xstart, float ystart, float xend,float yend ,float dir[2],Shape* s)
	 * \brief Prueft ob sich die Form auf der Linie befindet
	 * \param xstart x-Koordinate Startpunkt
	 * \param ystart y-Koordinate Startpunkt
	 * \param xend x-Koordinate Endpunkt
	 * \param yend y-Koordinate Endpunkt
	 * \param dir Richtung der Linie
	 * \param s Form, gegen die geprueft wird
	 * \return true, wenn die Linie die Form schneidet
	 */
	static bool lineIntersect(float xstart, float ystart, float xend,float yend ,float dir[2],Shape* s);

	/**
	 * \fn bool calcBlockmat(PathfindInfo * pathinfo)
	 * \brief Berechnet die Matrix der blockierten Felder
	 * \param pathinfo Wegfindeinformation fuer die die Matrix benoetigt wird
	 */
	bool calcBlockmat(PathfindInfo * pathinfo);

	/**
	 * \fn bool calcPotential(PathfindInfo* pathinfo)
	 * \brief Berechnet auf Basis der Blockadematrix das Potentialfeld
	 * \param pathinfo Wegfindeinformation fuer die das Potential berechnet wird
	 */
	bool calcPotential(PathfindInfo* pathinfo);

	/**
	 * \fn void calcPathDirection(PathfindInfo* pathinfo, float x_coordinate, float y_coordinate, float dir[2])
	 * \brief Berechnet die Bewegungsrichtung auf Basis der gegebenen Wegfindeinformation
	 * \param pathinfo verwendete Wegfindeinformation
	 * \param x_coordinate Standpunkt, x-Koordinate
	 * \param y_coordinate Standpunkt, y-Koordinate
	 * \param dir erhaltene Bewegungsrichtung
	 */
	void calcPathDirection(PathfindInfo* pathfinfo, float x_coordinate, float y_coordinate, float dir[2]);



	/**
	 * \fn getValidId()
	 * \brief Gibt eine zul&auml;ssige ID f&uuml;r ein WorldObject aus
	 * \return zul&auml;ssige ID f&uuml;r ein WorldObject
	 */
	int getValidId();

	/**
	 * \fn int getValidProjectileId()
	 * \brief Gibt eine zulaessige ID fuer ein Projektil zurueck
	 */
	int getValidProjectileId();


	/**
	 * \fn update(float time)
	 * \brief Laesst für alle Objekte in der Welt die angegebene Zeitspanne verstreichen
	 * \param time Zeit um die in der Welt vergeht in Millisekunden
	 */
	void update(float time);
	
	/**
	 * \fn void updatePlayers()
	 * \brief Aktualisiert die Spielerobjekte
	 */
	void updatePlayers();
	
	/**
	 * \fn void writeEvent(Region* region, Event* event, CharConv* cv)
	 * \brief Schreib die Daten zu dem Event in den Bitstream
	 * \param region Region in der das Event aufgetreten ist
	 * \param event Event das geschrieben wird
	 * \param cv Bitstream zum Schreiben
	 */
	void writeEvent(Region* region, Event* event, CharConv* cv);
	
	/**
	 * \fn bool processEvent(Region* region,CharConv* cv)
	 * \brief Liest ein Event aus dem Bitstream und fuehrt es aus
	 * \param region Region in der das Event aufgetreten ist
	 * \param cv Bitstream
	 */
	bool processEvent(Region* region,CharConv* cv);

	/**
	 * \fn void calcBlockArray(PathfindInfo* p)
	 * \brief Berechnet fuer die durch p spezifizierte Wegfindeaufgabe das Feld mit den blockierten Feldern
	 * \param p Struktur mit den Daten zur Wegfindung
	 */
	void calcBlockArray(PathfindInfo* p);

	/**
	* \fn void setNetwork(ServerNetwork * network)
	* \param network Zeiger auf das Netzwerk-Objekt
	* \brief Setzt Zeiger auf das Netzwerk-Objekt
	*/

	void setNetwork(ServerNetwork * network)
	{
		m_network = network;
	};


	/**
	 * \fn 	Region* getRegion(int rid)
	 * \brief Gibt die Region aus
	 * \param rid ID der Region
	 */
	Region* getRegion(int rid)
	{
		return m_regions[rid];
	}

	/**
	 * \fn Network* getNetwork()
	 *\return network Zeiger auf das Netzwerk-Objekt
	 * \brief Gibt Zeiger auf das Netzwerk-Objekt aus
	 */

	Network* getNetwork()
	{
		return m_network;
	}

	/**
	 * \fn Party* getParty(WorldObject::TypeInfo::Fraction frac)
	 * \brief Gibt die Party zurueck, welche zu der angegebenen Fraktion gehoert
	 * \param frac Fraktion eines Spielers
	 */
	Party* getParty(WorldObject::TypeInfo::Fraction frac)
	{
		return &(m_parties[frac - WorldObject::TypeInfo::FRAC_PLAYER_PARTY]);
	}

	/**
	 * \fn WorldObject* getLocalPlayer()
	 * \brief Gibt den Spieler aus, der sich an dem Rechner befindet, auf dem diese Welt simuliert wird
	 */
	WorldObject* getLocalPlayer()
	{
		return m_local_player;
	}

	/**
	 * \fn Party* getEmptyParty()
	 * \brief Gibt eine Party aus, die bisher keine Mitglieder hat
	 */
	Party* getEmptyParty();

	/**
	 * \fn Party::Relation getRelation(WorldObject::TypeInfo::Fraction frac, WorldObject* wo)
	 * \brief Gibt die Beziehung eines Lebewesens zu einer Fraktion
	 * \param frac Fraktion
	 * \param wo Objekt
	 */
	WorldObject::Relation getRelation(WorldObject::TypeInfo::Fraction frac, WorldObject* wo);

	/**
	 * \brief Gibt die Distanz zwischen den beiden Flaechen quadriert aus
	 * \param s1 erste Flaeche
	 * \param s2  zweite Flaeche
	 */
	static float getDistance(Shape& s1, Shape& s2);

	/**
	 * \fn void handleSavegame(CharConv *cv, int slot=LOCAL_SLOT)
	 * \brief Behandelt den Empfang eines Savegames
	 * \param cv Eingabepuffer
	 * \param slot Slot ueber den das Savegame empfangen wurde. Wenn das Savegame nicht ueber das Netzwerk uebertragen wurde -1
	 */
	void handleSavegame(CharConv *cv , int slot=LOCAL_SLOT);
	
	/**
	 * \fn void handleCommand(ClientCommand* cmd, int slot=LOCAL_SLOT, float delay=0)
	 * \brief Behandelt ein erhaltenes Kommmando
	 * \param cmd Kommando
	 * \param slot Slot ueber den das Kommando empfangen wurde. Wenn das Kommando nicht ueber das Netzwerk uebertragen wurde -1
	 * \param delay Millisekunden die das Kommando verspaetet erhalten wurde
	 */
	void handleCommand(ClientCommand* cmd, int slot=LOCAL_SLOT, float delay=0);
	
	/**
	 * \fn void handleDataRequest(ClientDataRequest* request, int slot  = LOCAL_SLOT)
	 * \brief Behandelt eine Anfrage nach Daten vom Client
	 * \param request beschreibt welche Daten gefordert werden
	 * \param slot Ziel der Daten
	 */
	void handleDataRequest(ClientDataRequest* request, int slot  = LOCAL_SLOT);

	/**
	 * \fn void insertEvent(Event &event)
	 * \brief Fuegt ein neues Event in die Eventliste ein
	 */
	void insertEvent(Event &event);
	
	/**
	 * \fn bool isServer()
	 * \brief gibt true aus, wenn die Welt der Server ist
	 */
	bool isServer()
	{
		return m_server;
	}
	
	/**
	 * \fn bool timerLimit(int i)
	 * \param i Nummer des Timers
	 * \brief Gibt true aus, wenn der Timer beim aktuellen update das Limit erreicht hat
	 */
	bool timerLimit(int i)
	{
		return m_timer_limit[i];
	}
	
	
			

//Private stuff
private:
	//Fields

	/**
	 * \var bool m_server
	 * \brief true, wenn der Rechner der Server ist
	 */
	bool m_server;
	
	/**
	 * \var m_regions
	 * \brief Speichert die Spielwelt in Form von Regionen. Eine Region ist ein rechteckiger Ausschnitt der Spielwelt
	 */
	Region* m_regions[WORLD_MAX_REGIONS];


	/**
	 * \var std::map<int, Trade* >* m_trades
	 * \brief Speichert alle Handelsvorgänge
	 */
	std::map<int, Trade* >* m_trades;


	/**
	 * \var Party* m_parties
	 * \brief Liste alles Parties
	 */
	Party* m_parties;

	/**
	 * \var m_player_slots
	 * \brief Liste der Spieler in der Welt mit ihren Slots
	 */
	WorldObjectMap* m_player_slots;
	
	/**
	 * \var WorldObjectMap* m_players
	 * \brief Liste der Spieler in der Welt sortiert nach ID
	 */
	WorldObjectMap* m_players;
	

	/**
	 * \var int m_max_nr_players
	 * \brief Gibt die maximale Anzahl Spieler an
	 */
	int m_max_nr_players;

	/**
	 * \var Network* m_network
	 * \brief Netzwerkanbindung
	 */
	Network* m_network;
	
	/**
	 * \var WorldObject* m_local_player
	 * \brief lokaler Spieler
	 */
	WorldObject* m_local_player;
	
	/**
	 * \var LoginList m_logins
	 * \brief Liste der Spieler die sich gerade einloggen wollen
	 */
	LoginList m_logins;
	
	/**
	 * \var EventList m_events
	 * \brief Liste der globalen Events beim aktuellen update
	 */
	EventList* m_events;
	
	/**
	 * \var float m_timer[3]
	 * \brief Timer a 250, 500 und 1000 ms
	 */
	float m_timer[3];
	
	/**
	 * \var  bool m_timer_limit[3]
	 * \brief ist true, wenn der betreffende Timer gerade in dem Tick abgelaufen ist
	 */
	bool m_timer_limit[3];
};
#endif //WORLD_H
