#ifndef REGION_H
#define REGION_H

#include "matrix2d.h"
#include "serverwobject.h"
#include "tiles.h"
#include "dropitem.h"
#include "event.h"


#include <string>
#include <list>
#include <map>
#include <set>
#include <math.h>
#include <cstdlib>
#include <algorithm>

class Gridunit;


using namespace std;

struct NBRegion
{
	/**
	 * \var m_region_id
	 * \brief ID der Region
	 */
	short m_region_id;
	
	/**
	 * \var m_dx
	 * \brief Aenderung der x-Koordinate beim Uebertritt in diese Region
	 */
	float m_dx;
	
	/**
	 * \var m_dy
	 * \brief Aenderung der y-Koordinate beim Uebertritt in diese Region
	 */
	float m_dy;
};

/**
 * \class Region
 * \brief Eine Region ist ein rechteckiges Teilstueck der Spielwelt. Monster und Geschosse koennen nicht zwischen Regionen wechseln
 */
class Region
{
	public:
		/**
		 * \fn Region(short dimx, short dimy, short id, bool server)
		 * \brief Konstruktor
		 * \param dimx Ausdehnung in x-Richtung
	 	 * \param dimy Ausdehnung in y-Richtung
	 	 */
		Region(short dimx, short dimy, short id, World* world, bool server);
		
		
		/**
		 * \fn ~Region()
		 * \brief Destruktor
		 */
		~Region();
		
		/**
		 * \fn insertSWObject (ServerWObject* object, float x, float y)
		 * \brief F&uuml;gt ServerWObject ein
		 * \param object Zeiger auf das Objekt, welches eingef&uuml;gt werden soll
		 * \return bool, der angibt, ob die Operation erfolgreich war
		 * 
		 * F&uuml;gt das ServerWObject in die internen Datenstrukturen ein. Wenn das Einf&uuml;gen erfolgreich war, so wird true zur&uuml;ckgegeben, sonst false.
		 * Wenn ein NULL-Zeiger &uuml;bergeben wird, so wird false ausgegeben.
		 */
		bool  insertSWObject (ServerWObject* object, float x, float y);
		
		/**
		 * \fn bool getSWObjectsInShape( Shape* shape,  list<ServerWObject*>* result,short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL, ServerWObject* omit=0, bool empty_test = false );
		 * \brief Sucht ServerWObjects innerhalb eines Gebietes
		 * \param shape Form des Gebietes
		 * \param layer Ebene in der gesucht wird
		 * \param result Liste, an die die gefundenen Objekte angehangen werden
		 * \param omit Objekt, das ausgelassen wird
		 * \param empty_test wenn true, wird nach dem ersten gefundenen Objekt abgebrochen
		 * \param group Gruppen die durchsucht werden sollen
		 * \return bool, der angibt, ob die Operation erfolgreich war
		 * 
		 * Alle gefundenen Objekte werden an das Ende der Liste result angehangen.
		 * Wenn f&uuml;r selector ein NULL-Zeiger &uuml;bergeben wird, so werden die Objekte nicht selektiert.
		 * Wenn f&uuml;r result ein NULL-Zeiger &uuml;bergeben wird, oder ein anderer Fehler auftritt, so wird false zur&uuml;ckgegeben, sonst true.
 		 */
		bool getSWObjectsInShape( Shape* shape,  list<ServerWObject*>* result,short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL, ServerWObject* omit=0, bool empty_test = false );
		
		/**
		 * \fn bool getFreePlace(Shape* shape, short layer, float& x, float&y)
		 * \brief Sucht nach einem freien Platz fuer ein Objekt moeglichst nahe an den angegebenen Koordinaten
		 * \param shape Form des Objekts
		 * \param layer Ebene des Objekts
		 * \param x Eingabe: Zielpunkt, Ausgabe: tatsaechlich gefundener Ort
		 * \param y Eingabe: Zielpunkt, Ausgabe: tatsaechlich gefundener Ort
		 * \return true, wenn ein freier Platz gefunden wurde, sonst false
		 */
		bool getFreePlace(Shape* shape, short layer, float& x, float&y);
		
		/**
		 * \fn bool addObjectsInShapeFromGridunit(Shape* shape, Gridunit* gu, list<ServerWObject*>* result, short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL,ServerWObject* omit=0, bool empty_test = false )
		 * \brief Fuegt alle Objekte aus der Gridunit, die sich mit dem Gebiet befinden zu der Liste hinzu
		 * \param layer Ebene in der gesucht wird
		 * \param result Liste, an die die gefundenen Objekte angehangen werden
		 * \param omit Objekt, das ausgelassen wird
		 * \param empty_test wenn true, wird nach dem ersten gefundenen Objekt abgebrochen
		 * \param group Gruppen die durchsucht werden sollen
		 * \return bool, der angibt, ob die Operation erfolgreich war
		 **/
		bool addObjectsInShapeFromGridunit(Shape* shape, Gridunit* gu, list<ServerWObject*>* result, short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL,ServerWObject* omit=0, bool empty_test = false );
		
		/**
		 * \fn bool addObjectsOnLineFromGridunit(float xstart, float ystart, float xend,float yend, Gridunit* gu, list<ServerWObject*>* result, short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL,ServerWObject* omit=0, bool empty_test = false )
		 * \brief Fuegt alle Objekte aus der Gridunit, die sich auf der Linie befinden zu der Liste hinzu
		 *  \param layer Ebene in der gesucht wird
		 * \param result Liste, an die die gefundenen Objekte angehangen werden
		 * \param omit Objekt, das ausgelassen wird
		 * \param empty_test wenn true, wird nach dem ersten gefundenen Objekt abgebrochen
		 * \param group Gruppen die durchsucht werden sollen
		 * \return bool, der angibt, ob die Operation erfolgreich war
		 **/
		bool addObjectsOnLineFromGridunit(float xstart, float ystart, float xend,float yend, Gridunit* gu, list<ServerWObject*>* result, short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL,ServerWObject* omit=0, bool empty_test = false );
		
		/**
		 * \fn bool changeObjectGroup(ServerWObject* object,WorldObject::Group group )
		 * \brief Veraendert die Gruppe des Objektes
		 * \param object Objekt fuer das die Gruppe geaendert werden soll
		 * \param group neue Gruppe des Objektes
		 */
		bool changeObjectGroup(ServerWObject* object,WorldObject::Group group );
		
		/**
		 * \fn ServerWObject* getSWObject ( int id)
		 * \brief Sucht Objekt anhand seiner ID heraus
		 * \return Objekt, Nullzeiger wenn das Objekt nicht existiert
		 */
		ServerWObject* getSWObject ( int id);
		
		/** 
		 * \fn ServerWObject* getSWObjectAt(float x_coordinate, float y_coordinate, short layer=WorldObject::Geomety::LAYER_ALL, short group = WorldObject::GROUP_ALL);
		 * \brief Sucht ServerWObject an den gegebenen Koordinaten
		 * \param x_coordinate x-Koordinate des Punktes
		 * \param y_coordinate y-Koordinate des Punktes
		 * \param layer Ebene in der gesucht wird
		 * \param group Gruppen die durchsucht werden sollen
		 * \return Zeiger auf das ServerWObject, wenn sich an dem Punkt eines befindet, sonst NULL
		 */
		ServerWObject* getSWObjectAt(float x_coordinate, float y_coordinate, short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL);
		
		/**
		 * \fn void getSWObjectsOnLine( float xstart, float ystart, float xend, float yend,  list<ServerWObject*>* result,short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL, ServerWObject* omit=0 )
		 * \brief Sucht alle Objekte die auf der angegebenen Linie liegen
		 * \param xstart x-Koordinate Startpunkt
		 * \param ystart y-Koordinate Startpunkt
		 * \param xend x-Koordinate Endpunkt
		 * \param yend y-Koordinate Endpunkt
		 * \param result Liste, an die die gefundenen Objekte angehangen werden
		 * \param layer Ebene in der gesucht wird
		 * \param group Gruppen die durchsucht werden sollen
		 * \param omit Objekt, das ausgelassen wird
		 */
		void getSWObjectsOnLine( float xstart, float ystart, float xend, float yend,  list<ServerWObject*>* result,short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL, ServerWObject* omit=0 );
		
		/**
		 * \fn void getProjectilesOnScreen(float center_x,float center_y, list<DmgProjectile*>* result)
		 * \brief Gibt alle Projektile auf dem Bildschirm aus
		 * \param center_x x-Koordinate Zentrum
		 * \param center_y y-Koordinate Zentrum
		 * \param result Liste in die die Objekte eingeordnet werden
		 */
		void getProjectilesOnScreen(float center_x,float center_y, list<DmgProjectile*>* result);
	
		/**
		 * \fn bool  insertProjectile(DmgProjectile* object, float x, float y)
		 * \brief Fuegt ein Projektil ein
		 * \param object Zeiger auf das Objekt, welches eingef&uuml;gt werden soll
		 * \return bool, der angibt, ob die Operation erfolgreich war
		 *
		 */
		bool  insertProjectile(DmgProjectile* object, float x, float y);
		
		/**
		 * \fn DmgProjectile* getProjectile(int id)
		 * \brief Gibt das Projektil mit der angegebenen ID aus
		 */
		DmgProjectile* getProjectile(int id);
		
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
		 * \fn void createObjectFromString(CharConv* cv, map<int,ServerWObject*>* players)
		 * \brief liest die Daten zu einem Objekt aus dem Bitstream und erzeugt es
		 * \param cv Bitstream aus dem die Daten gelesen werden
		 * \param players Spielerdaten (Spieler werden nicht neu erzeugt sondern nur in die Region verschoben)
		 */
		void createObjectFromString(CharConv* cv, map<int,ServerWObject*>* players);
		
		/**
		 * \fn void createProjectileFromString(CharConv* cv)
		 * \brief liest die Daten zu einem Projektil aus dem Bitstream und erzeugt es
		 */
		void createProjectileFromString(CharConv* cv);
		
		/**
		 * \fn void createItemFromString(CharConv* cv, int id)
		 * \brief liest die Daten zu einem Gegenstand aus dem Bitstream und erzeugt es
		 */
		void createItemFromString(CharConv* cv, int id);
		
		/**
		 * \fn void deleteProjectile(DmgProjectile* proj)
		 * \brief Entfernt das Projektil aus der Region
		 */
		void deleteProjectile(DmgProjectile* proj);
		
		/**
		 * \fn update(float time)
		 * \brief Lässt für alle Objekte in der Welt die angegebene Zeitspanne verstreichen
		 * \param time Zeit um die in der Welt vergeht in Millisekunden
		 */
		void update(float time);
		
		/**
		 * \fn bool dropItem(Item*, float x, float y)
		 * \brief Laesst Item in der Region fallen
		 * \param item Gegenstand
		 * \param x x-Koordinate
		 * \param y y-Koordinate
		 */
		bool dropItem(Item* item, float x, float y);
		
		/**
		 * \fn bool deleteItemAt(float x, float y)
		 * \brief Entfernt Item an der angegebenen Stelle aus der Region
		 */
		bool deleteItemAt(float x, float y);
		
		/**
		 * \fn Item* getItemAt(float x, float y)
		 * \brief Gibt Item an der angegebenen Position aus. Gibt NULL aus, wenn dort kein Item ist
		 */
		Item* getItemAt(float x, float y);
		
		/**
		 * \fn bool deleteItem(int id)
		 * \brief Entfernt Item aus der Region
		 */
		bool deleteItem(int id);
		
		/**
		 * \fn Item* getItem(int id)
		 * \brief Gibt Item aus. Gibt NULL aus, wenn dort kein Item ist
		 */
		Item* getItem(int id);
		
		/**
		 * \fn void getItemsOnScreen(float center_x,float center_y, list<DropItem*>* result)
		 * \brief Fuegt alle Gegenstaende auf dem Bildschirm in die Liste eine
		 * \param center_x x-Koordinate Zentrum
		 * \param center_y y-Koordinate Zentrum
		 * \param result Liste in die die Objekte eingeordnet werden
		 */
		void getItemsOnScreen(float center_x,float center_y, list<DropItem*>* result);
		
		/**
		 * \fn void getRegionData(CharConv* cv)
		 * \brief Schreibt alle Objekte, Projektile, Items und Tiles in einen String
		 * \param buf Eingabepuffer
		 * \return Zeiger hinter den beschriebenen Bereich
		 */
		void getRegionData(CharConv* cv);
		
		/**
		 * \fn void setRegionData(CharConv* cv)
		 * \brief Liest die Objekte, Projektile, Items und Tiles aus einem String ein
		 */
		void setRegionData(CharConv* cv, map<int,ServerWObject*>* players);
		
		/**
		 * \fn  setTile(Tile tile,short x, short y)
		 * \brief setzt an der angegebenen Position das Tile
		 * \param tile Tile
		 * \param x Position
		 * \param y Position
		 */
		void setTile(Tile tile,short x, short y);
		
		/**
		 * \fn map<int,ServerWObject*>* getPlayers()
		 * \brief Gibt alle Spieler in der Region aus
		 */
		map<int,ServerWObject*>* getPlayers()
		{
			return m_players;
		}
		
		
		/**
		 * \fn map<int,DropItem*>* getDropItems()
		 * \brief Gibt alle auf dem Boden liegenden Items aus
		 */
		map<int,DropItem*>* getDropItems()
		{
			return m_drop_items;
		}
		
		/**
		 * \fn map<int,DmgProjectile*>* getProjectiles()
		 */
		map<int,DmgProjectile*>* getProjectiles()
		{
			return m_projectiles;
		}
		
		/**
		 * \fn void insertEvent(Event &event)
		 * \brief Fuegt ein neues Event in die Eventliste ein
		 */
		void insertEvent(Event &event)
		{
			m_events->push_back(event);
		}
		
		/**
		 * \fn list<Event>* getEvents()
		 */
		list<Event>* getEvents()
		{
			return m_events;
		}
				
	private:
	/**
	 * \var m_dimx
	 * \brief Ausdehnung der Region in x-Richtung
	 */
	short m_dimx;
		
	/**
		* \var m_dimy
		* \brief Ausdehnung der Region in y-Richtung
		*/
	short m_dimy;
	
	/**
		* \var m_data_grid
		* \brief Das Array, welches die Daten der Region enthaelt. Jeder Eintrag im Array entspricht einem 4*4 Quadrat
		*/
	Matrix2d<Gridunit>* m_data_grid;
	
	/**
		* \var m_neighbour_region[4]
		* \brief Gibt die vier Nachbarregionen an
		*/
	NBRegion m_neighbour_region[4];
	
	/**
	 * \var Matrix2d<char> m_tiles
	 * \brief Matrix der Tiles
	 */
	Matrix2d<char>* m_tiles;
	
	/**
	 * \var m_players
	 * Liste der Spieler in der Region
	 */
	map<int,ServerWObject*>* m_players;
	
	/**
	 * \var map<int, DmgProjectile>* m_projectiles
	 * \brief Liste aller Projektile in der Region
	 */
	map<int,DmgProjectile*>* m_projectiles;
	
	/**
	 * \var map<int,ServerWObject*>* m_object_bintree
	 * \brief organisiert alle nicht statischen Objekte in einem Bin&auml;rbaum
	 */
	map<int,ServerWObject*>* m_objects;
	
	/**
	 * \var map<int,ServerWObject*>* m_object_bintree
	 * \brief organisiert statische Objekte in einem Bin&auml;rbaum
	 */
	map<int,ServerWObject*>* m_static_objects;
	
	/**
	 * \var map<int,DropItem*>* m_drop_items
	 * \brief Liste der Gegenstaende, die auf dem Boden liegen
	 */
	map<int,DropItem*>* m_drop_items;
	
	/**
	/* \var short m_id
	 * \brief Nummer der Region
	 */
	short m_id;
	
	/**
	 * \var std::queue<Event> m_events
	 * \brief Liste der lokalen Events beim aktuellen update
	 */
	list<Event>* m_events;
	
	/**
	 * \var bool m_server
	 * \brief true, wenn der Rechner der Server ist
	 */
	bool m_server;
	
	/**
	 * \var World* m_world
	 * \brief Zeiger auf die Welt
	 */
	World* m_world;
	
};

#include "gridunit.h"

#endif
