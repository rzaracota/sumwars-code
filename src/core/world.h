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

using namespace std;

#include <string>
#include <list>
#include <map>
#include <math.h>
#include <cstdlib>
#include <algorithm>
#include <iostream>

#include "serverwobject.h"
#include "dmgprojectile.h"
#include "trade.h"
#include "servernetwork.h"
#include "clientnetwork.h"
#include "region.h"
#include "pathfind.h"
#include "party.h"
#include "itemfactory.h"
#include "objectfactory.h"

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
//Forward Declarations

/**
 * \class World
 * \brief Hauptobjekt der Anwendung
 *
 * Dieses Objekt ist das Hauptobjekt der Anwendung. Die wesentliche Aufgabe liegt in der Verwaltung der ServerWObject Objekte.
 */
class World {
//Public stuff
public:
	//Fields
	//Constructors
	/**
	 * \fn World(bool server)
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
	 * \return gibt die ID der Region an, die an allen Objekten in der Region gespeichert wird. Im Falle eines Fehlers wird -1 zurueckgegeben.
	 */
	short insertRegion(Region* region, int rnr);
	
	/**
	 * \fn bool insertPlayerIntoRegion(ServerWObject* player, short region)
	 * \brief Versucht einen Spieler in eine Region einzufuegen
	 */
	bool insertPlayerIntoRegion(ServerWObject* player, short region);

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
	 * \fn ServerWObject*  getSWObject ( int id);
	 * \brief Sucht ServerWObject anhand seiner ID
	 * \param id ID des gesuchten ServerWObjects
	 * \param rid ID der Region
	 * \return Zeiger auf das gefundene ServerWObject
	 *
	 * W&auml;hlt ein ServerWObject anhand seiner ID aus. Gibt einen Zeiger auf das Objekt zur&uuml;ck, beziehungsweise NULL wenn das Objekt nicht gefunden wurde.
	 */
	ServerWObject* getSWObject ( int id, short rid);


	/**
	 * \fn bool getSWObjectsInShape( Shape* shape, short region, list<ServerWObject*>* result,short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL, ServerWObject* omit=0 )
	 * \brief Sucht ServerWObjects innerhalb eines Gebietes
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
	bool getSWObjectsInShape( Shape* shape, short region, list<ServerWObject*>* result,short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL, ServerWObject* omit=0 );


	/**
	 * \fn ServerWObject* getSWObjectAt(float x_coordinate, float y_coordinate,  short region,short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL)
	 * \brief Sucht ServerWObject an den gegebenen Koordinaten
	 * \param x_coordinate x-Koordinate des Punktes
	 * \param y_coordinate y-Koordinate des Punktes
	 * \param layer Ebene in der gesucht wird
	 * \param region Region in der gesucht wird
	 * \param layer Ebene in der gesucht wird
	 * \param group Gruppen die durchsucht werden sollen
	 * \return Zeiger auf das ServerWObject, wenn sich an dem Punkt eines befindet, sonst NULL
	 */
	ServerWObject* getSWObjectAt(float x_coordinate, float y_coordinate,  short region,short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL);

	/**
	 * \fn getSWObjectsOnLine( float xstart, float ystart, float xend, float yend,  short region, list<ServerWObject*>* result,,short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL ServerWObject* omit=0 )
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
	void getSWObjectsOnLine( float xstart, float ystart, float xend, float yend,  short region, list<ServerWObject*>* result,short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL, ServerWObject* omit=0 );

	/**
	 * \fn void getProjectilesOnScreen(float center_x,float center_y,short region, list<DmgProjectile*>* result)
	 * \brief Gibt alle Projektile auf dem Bildschirm aus
	 * \param center_x x-Koordinate Zentrum
	 * \param center_y y-Koordinate Zentrum
	 * \param region Region
	 * \param result Liste in die die Objekte eingeordnet werden
	 */
	void getProjectilesOnScreen(float center_x,float center_y,short region, list<DmgProjectile*>* result);

	/* *
	 * \fn getClosestFreeSquare(float x_coordinate, float y_coordinate, float &x_ret_coordinate, float &y_ret_coordinate)
	 * \brief Sucht den freien Punkt, der sich am nächsten zum eingegebenen Punkt befindet
	 * \param x_coordinate x-Koordinate des Punktes
	 * \param y_coordinate y-Koordinate des Punktes
	 * \param x_ret_coordinate x-Koordinate des gefundenen Punktes
	 * \param y_ret_coordinate y-Koordinate des gefundenen Punktes
	 * \return gibt an, ob ein Punkt gefunden wurde.
	 *
	 * Findet den Punkt, welcher sich am naechsten an dem eingegebenen Punkt befindet. Ist der Startpunkt selbst frei, so wird dieser zurückgegeben. Die Suche überschreitet ausgehend vom den Startpunkt zwar andere Spieler, aber keine festen Objekte. Es wird als ein Punkt gefunden, der vom Startpunkt aus prinzipiell erreichbar ist, lediglich der Weg ist durch andere Spieler blockiert  Es werden nur Punkte gefunden, die eine maximale Distanz zum Startpunkt von 10 haben
	 */
	//bool getClosestFreeSquare(float x_coordinate, float y_coordinate, float &x_ret_coordinate, float &y_ret_coordinate);


	/**
	 * \fn insertSWObject(ServerWObject* object)
	 * \brief F&uuml;gt ServerWObject ein
	 * \param object Zeiger auf das Objekt, welches eingef&uuml;gt werden soll
	 * \return bool, der angibt, ob die Operation erfolgreich war
	 *
	 * F&uuml;gt das ServerWObject in die internen Datenstrukturen ein. Wenn das Einf&uuml;gen erfolgreich war, so wird true zur&uuml;ckgegeben, sonst false.
	 * Wenn ein NULL-Zeiger &uuml;bergeben wird, so wird false ausgegeben.
	 */
	bool  insertSWObject (ServerWObject* object, float x, float y, short region);
	
	/**
	 * \fn bool insertPlayer(ServerWObject* player, int slot = NOSLOT)
	 * \brief Fuegt einen neuen Spieler hinzu
	 */
	bool insertPlayer(ServerWObject* player, int slot= NOSLOT);

	/**
	 * \fn bool  insertProjectile(DmgProjectile* object, float x, float y, short region)
	 * \brief Fuegt ein Projektil ein
	 * \param object Zeiger auf das Objekt, welches eingef&uuml;gt werden soll
	 * \return bool, der angibt, ob die Operation erfolgreich war
	 *
	 */
	bool  insertProjectile(DmgProjectile* object, float x, float y, short region);

	
	
	/**
	 * \fn deleteServerWObject(ServerWObject* object)
	 * \brief L&ouml;scht ServerWObject
	 * \param object Zeiger auf das Objekt, welches gel&ouml;scht werden soll
	 * \return bool, der angibt, ob die Operation erfolgreich war
	 *
	 * L&ouml;scht das ServerWObject aus den internen Datenstrukturen. Das Objekt selbst wird nicht gel&ouml;scht, es wird nur die Einbindung in die Datenstrukturen entfernt. Wenn das L&ouml;schen erfolgreich war, so wird true zur&uuml;ckgegeben, sonst false.
	 * Wenn ein NULL-Zeiger &uuml;bergeben wird, so wird false ausgegeben.
	 */
	bool  deleteSWObject (ServerWObject* object);

	 /**
	  * \fn moveServerWObject(ServerWObject* object, float x, float y)
	  * \brief verschiebt ein ServerWObject an den Punkt (x,y)
	  * \param object Zeiger auf das Objekt, welches verschoben werden soll
	  * \param x x-Koordinate des Zielpunktes, zu dem das Objekt verschoben werden soll
	  * \param y y-Koordinate des Zielpunktes, zu dem das Objekt verschoben werden soll
	  * \return bool, welcher angibt, ob das Verschieben erfolgreich war
	  */
	bool moveSWObject(ServerWObject* object, float x, float y);

	/**
	 * \fn bool lineIntersect(float xstart, float xstart, float xend,float yend float dir[2],Shape* s)
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
	 * \fn void calcPathDirection(PathfindInfo* pathfinfo, float x_coordinate, float y_coordinate, float dir[2])
	 * \brief Berechnet die Bewegungsrichtung auf Basis der gegebenen Wegfindeinformation
	 * \param pathinfo verwendete Wegfindeinformation
	 * \param x_coordinate Standpunkt, x-Koordinate
	 * \param y_coordinate Standpunkt, y-Koordinate
	 * \param dir erhaltene Bewegungsrichtung
	 */
	void calcPathDirection(PathfindInfo* pathfinfo, float x_coordinate, float y_coordinate, float dir[2]);



	/**
	 * \fn getValidId()
	 * \brief Gibt eine zul&auml;ssige ID f&uuml;r ein ServerWObject aus
	 * \return zul&auml;ssige ID f&uuml;r ein ServerWObject
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
	 * \fn ServerWObject* getLocalPlayer()
	 * \brief Gibt den Spieler aus, der sich an dem Rechner befindet, auf dem diese Welt simuliert wird
	 */
	ServerWObject* getLocalPlayer()
	{
		return m_local_player;
	}

	/**
	 * \fn Party* getEmptyParty()
	 * \brief Gibt eine Party aus, die bisher keine Mitglieder hat
	 */
	Party* getEmptyParty();

	/**
	 * \fn Party::Relation getRelation(WorldObject::TypeInfo::Fraction frac, ServerWObject* wo)
	 * \brief Gibt die Beziehung eines Lebewesens zu einer Fraktion
	 * \param frac Fraktion
	 * \param wo Objekt
	 */
	WorldObject::Relation getRelation(WorldObject::TypeInfo::Fraction frac, ServerWObject* wo);

	/**
	 * \brief Gibt die Distanz zwischen den beiden Flaechen quadriert aus
	 * \param s1 erste Flaeche
	 * \param s2  zweite Flaeche
	 */
	static float getDistance(Shape& s1, Shape& s2);

	/**
	 * \fn void handleSavegame(CharConv *cv, int slot=LOCAL_SLOT)
	 * \brief Behandelt den Empfang eines Savegames
	 * \param data Savegame
	 * \param slot Slot ueber den das Savegame empfangen wurde. Wenn das Savegame nicht ueber das Netzwerk uebertragen wurde -1
	 */
	void handleSavegame(CharConv *cv , int slot=LOCAL_SLOT);
	
	/**
	 * \fn void handleCommand(ClientCommand* cmd, int slot=LOCAL_SLOT)
	 * \brief Behandelt ein erhaltenes Kommmando
	 * \param cmd Kommando
	 * \param slot Slot ueber den das Kommando empfangen wurde. Wenn das Kommando nicht ueber das Netzwerk uebertragen wurde -1
	 */
	void handleCommand(ClientCommand* cmd, int slot=LOCAL_SLOT);
	
	/**
	 * \fn void handleDataRequest(ClientDataRequest* request, int slot  = LOCAL_SLOT)
	 * \brief Behandelt eine Anfrage nach Daten vom Client
	 * \param request beschreibt welche Daten gefordert werden
	 * \param slot Ziel der Daten
	 */
	void handleDataRequest(ClientDataRequest* request, int slot  = LOCAL_SLOT);

	// debug only


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
	 * \var map<int, Trade* >* m_trades
	 * \brief Speichert alle Handelsvorgänge
	 */
	map<int, Trade* >* m_trades;


	/**
	 * \var Party* m_parties
	 * \brief Liste alles Parties
	 */
	Party* m_parties;

	/**
	 * \var m_players
	 * \brief Liste der Spieler in der Welt mit ihren Slots
	 */
	map<int,ServerWObject*>* m_player_slots;
	
	/**
	 * \var map<int,ServerWObject*>* m_players
	 * \brief Liste der Spieler in der Welt sortiert nach ID
	 */
	map<int,ServerWObject*>* m_players;
	

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
	 * \var ServerWObject* m_local_player
	 */
	ServerWObject* m_local_player;
	
	/**
	 * \var list<int> m_logins
	 * \brief Liste der Spieler die sich gerade einloggen wollen
	 */
	list<int> m_logins;
};
#endif //WORLD_H
