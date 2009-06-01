#ifndef REGION_H
#define REGION_H

#include "event.h"
#include "matrix2d.h"
#include "worldobject.h"
#include "dropitem.h"
#include "networkstruct.h"
#include "projectile.h"
#include "objectfactory.h"
#include "dialogue.h"
#include "minimap.h"



#include <string>
#include <list>
#include <map>
#include <set>
#include <math.h>
#include <cstdlib>
#include <algorithm>

class Gridunit;
class Region;

typedef std::string RegionName;

typedef std::pair<RegionName, LocationName> RegionLocation;

/**
 * \enum ExitDirection
 * \brief Verschiedene Richtungen in denen man die Region verlassen kann
 */
enum ExitDirection
{
	SOUTH = 3,
	NORTH = 2,
	WEST = 0, 
	EAST = 1,
};

/**
 * \struct RegionExit
 * \brief Struktur die einen Ausgang aus der Region beschreibt
 */
struct RegionExit
{
	/**
	 * \var Shape m_shape
	 * \brief Form der Flaeche, in der der Spieler stehen muss um die Region zu verlassen
	 */
	Shape m_shape;
	
	/**
	 * \var LocationName m_exit_name
	 * \brief Name des Ortes, an dem sich der Ausgang befindet
	 */
	LocationName m_exit_name;
	
	/**
	 * \var std::string m_destination_region
	 * \brief Name der Zielregion
	 */
	std::string m_destination_region;
	
	/**
	 * \var LocationName m_destination_location
	 * \brief Name des Zielpunktes in der Zielregion
	 */
	LocationName m_destination_location;
};

/**
 * \class RegionData
 * \brief enthaelt alle Daten die noetig sind um die Region zu erstellen
 */
class RegionData
{
	public:
		
		/**
		 * \struct ObjectGroupTemplateSet
		 * \brief Struktur fuer eine Gruppe von Objekten die mehrmals in die Region eingefuegt werden soll
		 */
		struct ObjectGroupTemplateSet
		{
			/**
			 * \var ObjectGroupTemplateName m_group_name
			 * \brief Name der Gruppe
			 */
			ObjectGroupTemplateName m_group_name;
			
			/**
			 * \var int m_number
			 * \brief Anzahl wie oft man versuchen soll, eine Gruppe eingefuegen
			 */
			int m_number;
			
			/**
			 * \var float m_probability
			 * \brief Wahrscheinlichkeit mit der eine Gruppe eingefuegt wird
			 * es werden m_number viele Orte ausgewaehlt, aber fuer jeden nur mit dieser Warscheinlichkeit wirklich die Gruppe eingefuegt
			 */
			float m_probability;
		};
		
		/**
		 * \fn struct NamedObjectGroupTemplate
		 * \brief Struktur fuer eine Objektgruppe deren Instanz mit einem Namen versehen werden soll
		 */
		struct NamedObjectGroupTemplate
		{
			/**
			 * \var ObjectGroupTemplateName m_group_name
			 * \brief Name der Gruppe
			 */
			ObjectGroupTemplateName m_group_name;
			
			/**
			 * \var std::string m_name
			 * \brief Name der Gruppe
			 */
			std::string m_name;
		};
		
		/**
		 * \fn struct SpawnGroup
		 * \brief Gruppe von Monstern die in der Region auftaucht
		 */
		struct SpawnGroup
		{
			/**
			 * \var MonsterGroupName m_monsters
			 * \brief Name der Monstergruppe
			 */
			MonsterGroupName m_monsters;
			
			/**
			 * \var int m_number
			 * \brief Anzahl wie oft die Gruppe auftaucht
			 */
			int m_number;
			
		};
		
		/**
		 * \fn RegionData()
		 * \brief Konstruktor
		 */
		RegionData();
		
		/**
		 * \fn ~RegionData()
		 * \brief Destruktor
		 */
		~RegionData();
		
		/**
		 * \fn void addObjectGroupTemplate(ObjectGroupTemplateName m_group_name, int prio, int number =1, float probability=1.0)
		 * \brief Fuegt eine neue Objektgruppe ein
		 * \param group_name Name der Gruppe
		 * \param prio Prioritaet der Gruppe
		 * \param number Anzahl wie oft die Gruppe eingefuegt wird
		 * \param probability Wahrscheinlichkeit mit der die Gruppe eingefuegt wird
		 */
		void addObjectGroupTemplate(ObjectGroupTemplateName group_name, int prio=0, int number =1, float probability=1.0);
		
		/**
		 * \fn void  addNamedObjectGroupTemplate(ObjectGroupTemplateName group_name, std::string name, int prio=0)
		 * \brief Fuegt eine neue Objektgruppe ein
		 * \param group_name Name der Gruppe
		 * \param name Name unter dem die Gruppe eingefuegt wird
		 * \param prio Prioritaet der Gruppe
		 */
		void addNamedObjectGroupTemplate(ObjectGroupTemplateName group_name, std::string name, int prio=0)
		{
			NamedObjectGroupTemplate og;
			og.m_name = name;
			og.m_group_name = group_name;
			m_named_object_groups.insert( std::make_pair( prio, og));
		}
		
		/**
		 * \fn void addSpawnGroup(MonsterGroupName group_name, int number =1)
		 * \brief Fuegt eine neue Monstergruppe zu den in der Region auftauchenden Monstern hinzu
		 * \param group_name Name der Monstergruppe
		 * \param number Gibt an wie oft die Gruppe auftauchen soll
		 */
		void addSpawnGroup(MonsterGroupName group_name, int number =1)
		{
			SpawnGroup sg;
			sg.m_monsters = group_name;
			sg.m_number =number;
			m_monsters.push_back(sg);
		}
		
		/**
		 * \fn void addEnvironment(float maxheight, EnvironmentName env)
		 * \brief Fuegt eine neue Umgebung fuer die Region ein
		 * \param maxheight maximale Hoehe bis zu der diese Umgebung verwendet wird
		 * \param env Name der Umgebung
		 */
		void addEnvironment(float maxheight, EnvironmentName env)
		{
			m_environments.push_back(std::make_pair(maxheight,env));
		}
		
		/**
		 * \fn void addExit(RegionExit exit)
		 * \brief Fuegt einen Ausgang hinzu
		 * \param exit Ausgang
		 */
		void addExit(RegionExit exit)
		{
			m_exits.push_back(exit);
		}
		
		/**
		 * \fn void addEvent(TriggerType type, Event* event)
		 * \brief Fuegt ein neues Event hinzu
		 * \param trigger Typ des Triggers durch den das Event ausgeloest wird
		 * \param event Event
		 */
		void addEvent(TriggerType trigger, Event* event);
		
		/**
		 * \var short m_id
		 * \brief Nummer der Region
		 */
		short m_id;
		
		/**
		 * \var std::string m_name
		 * \brief Name der Region
		 */
		std::string m_name;
		
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
		 * \var float m_area_percent
		 * \brief Gibt Anteil der Flaeche an, die tatsaechlich zugaenglich sein soll
		 */
		float m_area_percent;
		
		/**
		 * \var float m_complexity
		 * \brief Komplexitaet der Berandung (Wert zwischen 0 und 1)
		 */
		float m_complexity;
		
		/**
		 * \var int m_granularity
		 * \brief Groesse der groessten Strukturen - muss eine Zweierpotenz sein
		 */
		int m_granularity;
		
		/**
		 * \var bool m_exit_directions[4]
		 * \brief Richtungen in denen ein Ausgang aus der Region erstellt wird
		 */
		bool m_exit_directions[4];
		
		/**
		 * \var std::list<RegionExit> m_region_exits
		 * \brief Liste der Ausgaenge aus der Region
		 */
		std::list<RegionExit> m_exits;
		
		/**
		 * \var std::multimap<int,ObjectGroupTemplateSet> m_object_groups
		 * \brief die Patterns sortiert nach einer Prioritaet
		 */
		std::multimap<int,ObjectGroupTemplateSet> m_object_groups;
		
		/**
		 * \var std::multimap<int, NamedObjectGroupTemplate > m_named_object_groups
		 * \brief Objektgruppen mit einem Namen. Diese Objektgruppen werden immer zuerst eingefuegt und sind verpflichtend
		 */
		std::multimap<int, NamedObjectGroupTemplate > m_named_object_groups;
		
		/**
		 * \var std::list<std::pair<float, EnvironmentName> > m_environments
		 * \brief Liste von moeglichen Umgebungen mit ihren Hoehenlinien
		 */
		std::list<std::pair<float, EnvironmentName> > m_environments;
		
		
		/**
		 * \var list<SpawnGroup> m_monsters
		 * \brief Liste der Monster die in der Region auftauchen
		 */
		std::list<SpawnGroup> m_monsters;
		
		
		/**
		 * \var LocationName m_revive_location
		 * \brief Ort an dem in dieser Region gestorbene Helden wiedererweckt werden
		 */
		LocationName m_revive_location;
		
		/**
		 * \var std::multimap<TriggerType, Event*> m_events
		 * \brief Events der Region
		 */
		std::multimap<TriggerType, Event*> m_events;
		
		/**
		 * \var std::string m_ground_material
		 * \brief Material mit dem der Boden gerendert wird
		 */
		std::string m_ground_material;
		
		/**
		 * \var ObjectGroupTemplateName m_region_template
		 * \brief Ist gesetzt, wenn die komplette Region aus einem Template besteht
		 */
		ObjectGroupTemplateName m_region_template;
		
		/**
		 * \var float m_ambient_light[3]
		 * \brief Werte fuer ambientes Licht
		 */
		float m_ambient_light[3];
		
		/**
		 * \var float m_hero_light[3]
		 * \brief Werte fuer das Spotlicht des Helden
		 */
		float m_hero_light[3];
		
		/**
		 * \var float m_directional_light[3]
		 * \brief Werte fuer das Licht von Sonne/Mond
		 */
		float m_directional_light[3];
		
		/**
		 * \var bool m_has_waypoint
		 * \brief auf true gesetzt, wenn die Region einen Wegpunkt hat
		 */
		bool m_has_waypoint;
};

/**
 * \class RegionCamera
 * \brief Zeigt an, aus welcher Position die Szene bei Zwischenszenen betrachtet wird
 */
class RegionCamera
{
	public:
		/**
		* \struct Position
		* \brief Speichert ein Position der Kamera
		*/
		struct Position
		{
			/**
			* \var float m_distance
			* \brief Abstand zum Spieler
			*/
			float m_distance;
			
			/**
			* \var float m_theta
			* \brief Winkel zum Boden
			*/
			float m_theta;
			
			/**
			* \var float m_phi
			* \brief Drehwinkel um die z-Achse
			*/
			float m_phi;
		
			/**
			* \var Vector m_focus
			* \brief Vector, auf den die Kamera fokussiert ist
			*/
			Vector m_focus;
		};
	
		/**
		 * \fn void addPosition(Position& pos, float time)
		 * \brief Fuegt zur Kamerabewegung einen weiteren Punkt hinzu
		 * \param pos Position die erreicht werden soll
		 * \param time Zeit in ms, die vergeht, bis ausgehend von der vorhergehenden Position die angegebenen Position erreicht wird
		 */
		void addPosition(Position& pos, float time);
		
	
		/**
		 * \fn void update(float time)
		 * \brief aktualisiert die Kamerastellung nachdem eine gewisse Zeit vergangen ist
		 * \param time Zeit in ms
		 */
		void update(float time);
		
		/**
		 * \fn void toString(CharConv* cv)
		 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
		 * \param cv Ausgabepuffer
		 */
		void toString(CharConv* cv);
			
			
		/**
		 * \fn void fromString(CharConv* cv)
		 * \brief Erzeugt das Objekt aus einem String
		 * \param cv Eingabepuffer
		 */
		void fromString(CharConv* cv);
		
		
		/**
		 * \var Position m_position
		 * \brief aktuelle Position
		 */
		Position m_position;
		
		/**
		 * \var std::list<std::pair<Position,float> > m_next_positions
		 * \brief Positionen, die als naechstes eingenommen werden
		 */
		std::list<std::pair<Position,float> > m_next_positions;
		
		/**
		 * \var  Region* m_region
		 * \brief Region zu der die Kamera gehoert
		 */
		Region* m_region;
};

	
/**
 * \class Region
 * \brief Eine Region ist ein rechteckiges Teilstueck der Spielwelt. Monster und Geschosse koennen nicht zwischen Regionen wechseln
 */
class Region
{
	public:
		/**
	 * \fn Region(short dimx, short dimy, short id, std::string name, RegionData* data=0)
		 * \brief Konstruktor
		 * \param dimx Ausdehnung in x-Richtung
	 	 * \param dimy Ausdehnung in y-Richtung
		 * \param id ID der Region
		 * \param name Name der Region
		 * \param data Daten zur Initialisierung
	 	 */
		Region(short dimx, short dimy, short id, std::string name, RegionData* data=0);


		/**
		 * \fn ~Region()
		 * \brief Destruktor
		 */
		~Region();

		/**
		 * \fn insertObject (WorldObject* object, Vector pos, float angle=0, bool collision_test =false)
		 * \brief Fuegt ein bereits existierendes WorldObject ein
		 * \param object Zeiger auf das Objekt, welches eingefuegt werden soll
		 * \param pos Position an der das Objekt eingefuegt wird
		 * \param angle Drehwinkel des Objektes
		 * \param collision_test wenn auf true gesetzt, wird geprueft ob das Objekt mit einem anderen kollidiert und die Positions in dem Fall leicht geaendert
		 * \return bool, der angibt, ob die Operation erfolgreich war
		 */
		bool insertObject (WorldObject* object, Vector pos, float angle=0, bool collision_test =false);
		
		/**
		 * \fn int createObject(ObjectTemplateType generictype, Vector pos, float angle=0, WorldObject::State state = WorldObject::STATE_ACTIVE)
		 * \brief Erstellt ein neues Objekt und fuegt es an der angegebenen Stelle ein
		 * \param type Typ des neuen Objekts
		 * \param generictype Subtyp des neuen Objekts, kann generisch sein
		 * \param pos Position an der das Objekt eingefuegt wird
		 * \param angle Drehwinkel des Objektes
		 * \param state Zustand in dem das Objekt erzeugt wird
		 * \return ID des neu erstellten Objekts
		 */
		int createObject(ObjectTemplateType generictype, Vector pos, float angle=0, WorldObject::State state = WorldObject::STATE_NONE);
		
		/**
		 * \fn void createObjectGroup(ObjectGroupTemplateName templname, Vector position, float angle=0, std::string name="" )
		 * \brief erzeugt eine Gruppe von Objekten und fuegt sie in die Region ein
		 * \param templname Name des Templates aus dem die Gruppe erzeugt wird
		 * \param position Ort an dem die Gruppe eingefuegt wird
		 * \param angle Drehwinkel mit dem die Gruppe eingefuegt wird
		 * \param Name des Ortes
		 */
		void createObjectGroup(ObjectGroupTemplateName templname, Vector position, float angle=0, std::string name ="");
		
		/**
		 * \fn void createMonsterGroup(MonsterGroupName mgname, Vector position, float radius=3)
		 * \brief erzeugt an der angegebenen Stelle eine Monstergruppe
		 * \param mgname Name der Monstergruppe
		 * \param position Ort an dem die Gruppe erzeugt wird
		 * \param radius Umkreis in dem die Monster verteilt werden
		 */
		void createMonsterGroup(MonsterGroupName mgname, Vector position, float radius=3);

		/**
		 * \fn bool getObjectsInShape( Shape* shape,  WorldObjectList* result,short layer=WorldObject::LAYER_ALL, short group = WorldObject::GROUP_ALL, WorldObject* omit=0, bool empty_test = false );
		 * \brief Sucht WorldObjects innerhalb eines Gebietes
		 * \param shape Form des Gebietes
		 * \param layer Ebene in der gesucht wird
		 * \param result Liste, an die die gefundenen Objekte angehangen werden
		 * \param omit Objekt, das ausgelassen wird
		 * \param empty_test wenn true, wird nach dem ersten gefundenen Objekt abgebrochen
		 * \param group Gruppen die durchsucht werden sollen
		 * \return bool, der angibt, ob die Operation erfolgreich war
		 *
 		 */
		bool getObjectsInShape( Shape* shape,  WorldObjectList* result,short layer=WorldObject::LAYER_ALL, short group = WorldObject::GROUP_ALL, WorldObject* omit=0, bool empty_test = false );

		/**
		 * \fn bool getFreePlace(Shape* shape, short layer, Vector& pos, WorldObject* omit=0)
		 * \brief Sucht nach einem freien Platz fuer ein Objekt moeglichst nahe an den angegebenen Koordinaten
		 * \param shape Form des Objekts
		 * \param layer Ebene des Objekts
		 * \param pos Eingabe: Zielpunkt, Ausgabe, tatsaechlich gefundener Ort
		 * \param omit Objekt, das ausgelassen wird
		 * \return true, wenn ein freier Platz gefunden wurde, sonst false
		 */
		bool getFreePlace(Shape* shape, short layer, Vector& pos, WorldObject* omit=0);

		/**
		 * \fn bool addObjectsInShapeFromGridunit(Shape* shape, Gridunit* gu, WorldObjectList* result, short layer=WorldObject::LAYER_ALL, short group = WorldObject::GROUP_ALL,WorldObject* omit=0, bool empty_test = false )
		 * \brief Fuegt alle Objekte aus der Gridunit, die sich mit dem Gebiet befinden zu der Liste hinzu
		 * \param layer Ebene in der gesucht wird
		 * \param result Liste, an die die gefundenen Objekte angehangen werden
		 * \param omit Objekt, das ausgelassen wird
		 * \param empty_test wenn true, wird nach dem ersten gefundenen Objekt abgebrochen
		 * \param group Gruppen die durchsucht werden sollen
		 * \param shape Es werden nur Objekte hinzugefuegt die diese Flaeche beruehren
		 * \param gu Gridunit aus der Objekte hinzugefuegt werden
		 * \return bool, der angibt, ob die Operation erfolgreich war
		 **/
		bool addObjectsInShapeFromGridunit(Shape* shape, Gridunit* gu, WorldObjectList* result, short layer=WorldObject::LAYER_ALL, short group = WorldObject::GROUP_ALL,WorldObject* omit=0, bool empty_test = false );

		/**
		 * \fn bool addObjectsOnLineFromGridunit(Line& line, Gridunit* gu, WorldObjectList* result, short layer=WorldObject::LAYER_ALL, short group = WorldObject::GROUP_ALL,WorldObject* omit=0, bool empty_test = false )
		 * \brief Fuegt alle Objekte aus der Gridunit, die sich in einer Flaeche befinden zu der Liste hinzu (intern)
		 *  \param layer Ebene in der gesucht wird
		 * \param result Liste, an die die gefundenen Objekte angehangen werden
		 * \param omit Objekt, das ausgelassen wird
		 * \param empty_test wenn true, wird nach dem ersten gefundenen Objekt abgebrochen
		 * \param group Gruppen die durchsucht werden sollen
		 * \param line Linie
		 * \param gu Gridunit aus der Objekte hinzugefuegt werden
		 * \return bool, der angibt, ob die Operation erfolgreich war
		 **/
		bool addObjectsOnLineFromGridunit(Line& line, Gridunit* gu, WorldObjectList* result, short layer=WorldObject::LAYER_ALL, short group = WorldObject::GROUP_ALL,WorldObject* omit=0, bool empty_test = false );

		/**
		 * \fn bool changeObjectGroup(WorldObject* object,WorldObject::Group group )
		 * \brief Veraendert die Gruppe des Objektes
		 * \param object Objekt fuer das die Gruppe geaendert werden soll
		 * \param group neue Gruppe des Objektes
		 */
		bool changeObjectGroup(WorldObject* object,WorldObject::Group group );

		/**
		 * \fn WorldObject* getObject ( int id)
		 * \brief Sucht Objekt anhand seiner ID heraus
		 * \param id ID des Objekts
		 * \return Objekt, Nullzeiger wenn das Objekt nicht existiert
		 */
		WorldObject* getObject ( int id);

		/**
		 * \fn WorldObject* getObjectAt(Vector pos, short layer=WorldObject::LAYER_ALL, short group = WorldObject::GROUP_ALL);
		 * \brief Sucht WorldObject an den gegebenen Koordinaten
		 * \param pos Ort an dem gesucht wird
		 * \param layer Ebene in der gesucht wird
		 * \param group Gruppen die durchsucht werden sollen
		 * \return Zeiger auf das WorldObject, wenn sich an dem Punkt eines befindet, sonst NULL
		 */
		WorldObject* getObjectAt(Vector pos, short layer=WorldObject::LAYER_ALL, short group = WorldObject::GROUP_ALL);

		/**
		 * \fn void getObjectsOnLine( Line & line,  WorldObjectList* result,short layer=WorldObject::Geometry::LAYER_ALL, short group = WorldObject::GROUP_ALL, WorldObject* omit=0 )
		 * \brief Sucht alle Objekte die auf der angegebenen Linie liegen
		 * \param line Linie auf der gesucht wird
		 * \param result Liste, an die die gefundenen Objekte angehangen werden
		 * \param layer Ebene in der gesucht wird
		 * \param group Gruppen die durchsucht werden sollen
		 * \param omit Objekt, das ausgelassen wird
		 */
		void getObjectsOnLine( Line& line,  WorldObjectList* result,short layer=WorldObject::LAYER_ALL, short group = WorldObject::GROUP_ALL, WorldObject* omit=0 );


		/**
		 * \fn bool  insertProjectile(Projectile* object, Vector pos)
		 * \brief Fuegt ein Projektil ein
		 * \param object Zeiger auf das Objekt, welches eingefuegt werden soll
		 * \param pos Position an der das Objekt eingefuegt wird
		 * \return bool, der angibt, ob die Operation erfolgreich war
		 *
		 */
		bool  insertProjectile(Projectile* object, Vector pos);

		/**
		 * \fn Projectile* getProjectile(int id)
		 * \brief Gibt das Projektil mit der angegebenen ID aus
		 */
		Projectile* getProjectile(int id);

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
		 * \fn bool deleteObject(int id)
		 * \brief Loescht das Objekt mit der angegebenen ID
		 */
		bool deleteObject(int id);
		
		 /**
		 * \fn moveObject(WorldObject* object, Vector newpos)
		 * \brief verschiebt ein WorldObject an den Punkt (x,y)
		 * \param object Zeiger auf das Objekt, welches verschoben werden soll
		 * \param newpos neue Koordinaten des Objekts
		 * \return bool, welcher angibt, ob das Verschieben erfolgreich war
		  */
		bool moveObject(WorldObject* object, Vector newpos);

		/**
		 * \fn void createObjectFromString(CharConv* cv, WorldObjectMap* players)
		 * \brief liest die Daten zu einem Objekt aus dem Bitstream und erzeugt es
		 * \param cv Bitstream aus dem die Daten gelesen werden
		 * \param players Spielerdaten (Spieler werden nicht neu erzeugt sondern nur in die Region verschoben)
		 */
		void createObjectFromString(CharConv* cv, WorldObjectMap* players);

		/**
		 * \fn void createProjectileFromString(CharConv* cv)
		 * \brief liest die Daten zu einem Projektil aus dem Bitstream und erzeugt es
		 */
		void createProjectileFromString(CharConv* cv);

		/**
		 * \fn void createItemFromString(CharConv* cv)
		 * \brief liest die Daten zu einem Gegenstand aus dem Bitstream und erzeugt es
		 */
		void createItemFromString(CharConv* cv);

		/**
		 * \fn void deleteProjectile(Projectile* proj)
		 * \brief Entfernt das Projektil aus der Region
		 */
		void deleteProjectile(Projectile* proj);

		/**
		 * \fn update(float time)
		 * \brief Lässt für alle Objekte in der Welt die angegebene Zeitspanne verstreichen
		 * \param time Zeit um die in der Welt vergeht in Millisekunden
		 */
		void update(float time);

		/**
		 * \fn bool dropItem(Item* item, Vector pos)
		 * \brief Laesst Item in der Region fallen
		 * \param item Gegenstand
		 * \param pos Ort an dem der Gegenstand fallen gelassen wird
		 */
		bool dropItem(Item* item, Vector pos);

		/**
		 * \fn bool dropItem(Item::Subtype subtype, Vector pos,int magic_power=0)
		 * \brief Erzeugt ein Item mit dem angegebenen Typ und laesst es fallen
		 * \param subtype Typ des Gegenstandes
		 * \param pos Position an der der Gegenstand fallen gelassen wird
		 * \param magic_power Verzauberung des Gegenstandes
		 */
		bool dropItem(Item::Subtype subtype, Vector pos, int magic_power=0);
		
		/**
		 * \fn Item* getItemAt(Vector pos)
		 * \param pos Ort an dem gesucht wird
		 * \brief Gibt Item an der angegebenen Position aus. Gibt NULL aus, wenn dort kein Item ist
		 */
		Item* getItemAt(Vector pos);

		/**
		 * \fn bool deleteItem(int id, bool delitem)
		 * \brief Entfernt Item aus der Region
		 * \param id ID des items
		 * \param delitem Wenn true, wird das Item geloescht, sonst nur aus der Region entfernt
		 */
		bool deleteItem(int id, bool delitem=false);

		/**
		 * \fn Item* getItem(int id)
		 * \brief Gibt Item aus. Gibt NULL aus, wenn dort kein Item ist
		 */
		Item* getItem(int id);

		/**
		 * \fn Item* getDropItem(int id)
		 * \brief Gibt Item mit den Information wo es liegt aus. Gibt NULL aus, wenn dort kein Item ist
		 */
		DropItem* getDropItem(int id);

		/**
		 * \fn void getRegionData(CharConv* cv)
		 * \brief Schreibt alle Objekte, Projektile, Items und Tiles in einen String
		 * \param cv Eingabepuffer
		 * \return Zeiger hinter den beschriebenen Bereich
		 */
		void getRegionData(CharConv* cv);

		/**
		 * \fn void setRegionData(CharConv* cv, WorldObjectMap* players)
		 * \brief Liest die Objekte, Projektile, Items und Tiles aus einem String ein
		 * \param cv Eingabepuffer
		 * \param players Liste der Spieler (die Spieler werden nicht neu erzeugt, sondern nur eingefuegt)
		 */
		void setRegionData(CharConv* cv, WorldObjectMap* players);

		/**
		 * \fn void getRegionCheckData(CharConv* cv)
		 * \brief Gibt die Daten aus, mit denen sich das Inventar der Welt ueberpruefen laesst
		 * \param cv Eingabepuffer
		 */
		void getRegionCheckData(CharConv* cv);
		
		/**
		 * \fn void checkRegionData(CharConv* cv)
		 * \brief Prueft die Daten der Region mit Hilfe der im Puffer vorliegenden Daten
		 * \param cv Ausgabepuffer
		 */
		void checkRegionData(CharConv* cv);
		

		/**
		 * \fn Minimap* createMinimap()
		 * \brief Erzeugt eine Minimap fuer die Region
		 */
		Minimap* createMinimap()
		{
			return new Minimap(m_dimx,m_dimy);
		}
		
		/**
		 * \fn WorldObjectMap* getPlayers()
		 * \brief Gibt alle Spieler in der Region aus
		 */
		WorldObjectMap* getPlayers()
		{
			return m_players;
		}


		/**
		 * \fn DropItemMap* getDropItems()
		 * \brief Gibt alle auf dem Boden liegenden Items aus
		 */
		DropItemMap* getDropItems()
		{
			return m_drop_items;
		}

		/**
		 * \fn ProjectileMap* getProjectiles()
		 * \return Gibt alle Projektile in der Region aus
		 */
		ProjectileMap* getProjectiles()
		{
			return m_projectiles;
		}
		
		/**
		 * \fn GameObjectMap& getGameObjects()
		 * \brief Gibt die Datenstruktur mit allen Objekten aus
		 */
		GameObjectMap& getGameObjects()
		{
			return m_game_objects;
		}
		
		/**
		 * \fn WorldObjectMap* getStaticObjects()
		 * \brief Gibt die statischen Objekte aus
		 */
		WorldObjectMap* getStaticObjects()
		{
			return m_static_objects;
		}

		/**
		 * \fn void insertNetEvent(NetEvent &event)
		 * \brief Fuegt ein neues NetEvent in die NetEventliste ein
		 */
		void insertNetEvent(NetEvent &event)
		{
			m_netevents->push_back(event);
		}
		
		/**
		 * \fn void insertEnvironment(float maxheight, EnvironmentName env)
		 * \brief Fuegt eine neue Umgebung in die Region ein
		 * \param maxheight maximale Hoehe bis zu der diese Umgebung verwendet wird
		 * \param env Name der Umgebung
		 */
		void insertEnvironment(float maxheight, EnvironmentName env)
		{
			m_environments.insert(std::make_pair(maxheight,env));
		}

		/**
		 * \fn NetEventList* getNetEvents()
		 * \brief Gibt die Liste der NetEvents aus
		 */
		NetEventList* getNetEvents()
		{
			return m_netevents;
		}
		
		/**
		 * \fn Matrix2d<float>* getHeight()
		 * \brief Gibt die Hoehenkarte aus
		 */
		Matrix2d<float>* getHeight()
		{
			return m_height;
		}
		
		/**
		 * \fn EnvironmentName getEnvironment(Vector pos)
		 * \brief Gibt die Umgebung aus, die an der angegebenen Stelle vorherrscht
		 * \param pos Ort
		 */
		EnvironmentName getEnvironment(Vector pos);
		
		/**
		 * \fn void addLocation(LocationName name, Vector pos)
		 * \brief Fuegt einen neuen Ort ein
		 * \param name Name des Ortes
		 * \param pos Koordinaten des Ortes
		 */
		void addLocation(LocationName name, Vector pos)
		{
			m_locations.insert(std::make_pair(name,pos));
		}
		
		/**
		 * \fn bool hasLocation(LocationName loc)
		 * \brief Gibt an, ob die Region einen Ort mit dem angegebenen Namen hat
		 * \param loc Ort
		 */
		bool hasLocation(LocationName loc)
		{
			return (m_locations.count(loc) > 0);
		}
		
		/**
		 * \fn Vector getLocation(LocationName name)
		 * \brief Gibt die Position eines Ortes aus
		 * \param name Name des Ortes
		 */
		Vector getLocation(LocationName name);
		
		/**
		 * \fn void addArea(AreaName name, Shape area)
		 * \brief Fuegt eine neue Flaeche ein
		 */
		void addArea(AreaName name, Shape area)
		{
			m_areas.insert(std::make_pair(name,area));	
		}
		
		/**
		 * \fn Shape getArea(AreaName name)
		 * \brief Gibt die Flaeche mit dem angegebenen Name aus
		 */
		Shape getArea(AreaName name);
		
		/**
		 * \fn std::string getName()
		 * \brief Gibt den Name der Region aus
		 */
		std::string getName()
		{
			return m_name;
		}
		
		/**
		 * \fn void addExit(RegionExit exit)
		 * \brief Fuegt einen Ausgang hinzu
		 * \param exit Ausgang
		 */
		void addExit(RegionExit exit);
		
		/**
		 * \fn short getId()
		 * \brief Gibt die ID der Region aus
		 */
		short getId()
		{
			return m_id;
		}
		
		/**
		 * \fn void setReviveLocation(LocationName loc)
		 * \brief Setzt den Ort an dem in dieser Region gestorbene Helden wiederbelebt werden
		 * \param loc Name des Ortes
		 */
		void setReviveLocation(LocationName loc)
		{
			m_revive_location = loc;
			
		}
		
		/**
		 * \fn void insertTrigger(Trigger* trigger)
		 * \brief Fuegt einen neuen Trigger in die Liste der aktuell aktivierten Trigger ein
		 * \param trigger eingefuegter Trigger
		 */
		void insertTrigger(Trigger* trigger);
		
		/**
		 * \fn void insertTimedTrigger(Trigger* trigger)
		 * \brief Fuegt einen Trigger ein, der zeitverzoegert aktiv wird
		 * \param trigger eingefuegter Trigger
		 * \param time Zeit
		 */
		void insertTimedTrigger(Trigger* trigger, float time);
		
		/**
		 * \fn void addEvent(TriggerType type, Event* event)
		 * \brief Fuegt ein neues Event hinzu
		 * \param trigger Typ des Triggers durch den das Event ausgeloest wird
		 * \param event Event
		 */
		void addEvent(TriggerType trigger, Event* event);
		
		/**
		 * \fn Damage& getDamageObject(std::string name)
		 * \brief Gibt das Schadensobjekt mit dem angegebenen Name aus. Wenn das Objekt noch nicht existierte wird es angelegt
		 * \param name Name
		 */
		Damage& getDamageObject(std::string name)
		{
			return m_damage_objects[name];
		}
		
		/**
		 * \fn void setCutsceneMode(bool mode)
		 * \brief Setzt den Cutscene Modus
		 * \param mode Modus
		 */
		void setCutsceneMode(bool mode);
		
		/**
		 * \fn bool getCutsceneMode()
		 * \brief Gibt aus, ob die sich die Region aktuell im Cutscene Modus befindet
		 */
		bool getCutsceneMode()
		{
			return m_cutscene_mode;
		}
		
		/**
		 * \fn std::multimap<TriggerType, Event*>& getEvents()
		 * \brief Gibt Referenz auf die Eventliste aus
		 */
		std::multimap<TriggerType, Event*>& getEvents()
		{
			return m_events;
		}
		
		/**
		 * \fn RegionCamera& getCamera()
		 * \brief Gibt die Kamera aus
		 */
		RegionCamera& getCamera()
		{
			return m_camera;
		}
		
		/**
		 * \fn Dialogue* getDialogue(int id)
		 * \brief Gibt zu der die ID den Dialog aus
		 * \param id ID
		 */
		Dialogue* getDialogue(int id);
		
		/**
		 * \fn void insertDialogue(Dialogue* dia)
		 * \brief Fuegt einen neuen Dialog ein
		 * \param dia Dialog
		 */
		void insertDialogue(Dialogue* dia);
		
		/**
		 * \fn short getDimX()
		 * \brief Dimension in x-Richtung
		 */
		short getDimX()
		{
			return m_dimx;
		}
		
		/**
		 * \fn short getDimY()
		 * \brief Dimension in y-Richtung
		 */
		short getDimY()
		{
			return m_dimy;
		}
		
		/**
		 * \fn void setGroundMaterial(std::string material)
		 * \brief Setzt das Material des Bodens
		 * \param material Material
		 */
		void setGroundMaterial(std::string material)
		{
			m_ground_material = material;
		}
		
		/**
		 * \fn std::string getGroundMaterial()
		 * \brief Gibt das Material fuer den Boden aus
		 */
		std::string getGroundMaterial()
		{
			return m_ground_material;
		}
		
		/**
		 * \fn float* getAmbientLight()
		 * \brief Gibt ambiente Lichtstaerke aus
		 */
		float* getAmbientLight()
		{
			return m_ambient_light;
		}
		
		/**
		 * \fn float* getHeroLight()
		 * \brief Gibt Lichtstaerke des Heldeslichtes aus
		 */
		float* getHeroLight()
		{
			return m_hero_light;
		}
		
		/**
		 * \fn float* getDirectionalLight()
		 * \brief Gibt Lichtstaerke des gerichteten Lichts aus
		 */
		float* getDirectionalLight()
		{
			return m_directional_light;
		}
		
		/**
		 * \fn Vector& getWaypointLocation()
		 * \brief Gibt den Ort des Wegpunktes aus
		 */
		Vector& getWaypointLocation()
		{
			return m_waypoint_location;
		}
		
		/**
		 * \fn void insertPlayerTeleport(int playerid, RegionLocation regionloc)
		 * \brief Fuegt einen Spieler ein, der sich aus der Region per Wegpunkt entfernt
		 */
		void insertPlayerTeleport(int playerid, RegionLocation regionloc)
		{
			m_teleport_players.insert(std::make_pair(playerid,regionloc));
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
		 * \var Matrix2d<float>* m_height
		 * \brief Hoehenkarte in 4x4 Kaestchen
		 */
		Matrix2d<float>* m_height;
		
		/**
		 * \var std::map<float, EnvironmentName> m_environments
		 * \brief Liste von moeglichen Umgebungen mit ihren Hoehenlinien (Obergrenzen)
		 */
		std::map<float, EnvironmentName> m_environments;
	
		/**
		* \var m_players
		* Liste der Spieler in der Region
		*/
		WorldObjectMap* m_players;
	
		/**
		* \var ProjectileMap* m_projectiles
		* \brief Liste aller Projektile in der Region
		*/
		ProjectileMap* m_projectiles;
	
		/**
		* \var WorldObjectMap* m_objects
		* \brief organisiert alle nicht statischen Objekte in einem Bin&auml;rbaum
		*/
		WorldObjectMap* m_objects;
	
		/**
		 * \var WorldObjectMap* m_static_objects;
		 * \brief organisiert statische Objekte in einem Bin&auml;rbaum
		 */
		WorldObjectMap* m_static_objects;
		
		/**
		 * \var WorldObjectMap m_large_objects
		 * \brief Speichert alle Objekte, die so gross sind, dass sie fuer die Kollisionsbehandlung gesondert behandelt werden muessen
		 */
		WorldObjectMap m_large_objects;
	
		/**
		* \var DropItemMap* m_drop_items
		* \brief Liste der Gegenstaende, die auf dem Boden liegen
		*/
		DropItemMap* m_drop_items;
	
		/**
		* \var DropItemMap* m_drop_item_locations
		* \brief Orte der Gegenstaende, die auf dem Boden liegen
		*/
		DropItemMap* m_drop_item_locations;
	
		/**
		 * \var GameObjectMap m_game_objects
		 * \brief Datenstruktur mit allen (nicht statischen) Spielobjekten der Region
		 */
		GameObjectMap m_game_objects;
	
		/**
		* \var short m_id
		* \brief Nummer der Region
		*/
		short m_id;
		
		/**
		 * \var std::string m_name
		 * \brief Name der Region
		 */
		std::string m_name;
		
		/**
		 * \var bool m_cutscene_mode
		 * \brief Gibt an, ob die Region sich aktuell im Cutszene Modus befindet. In diesem Modus werden nur per Script gesetzte Aktionen ausgefuehrt
		 */
		bool m_cutscene_mode;
	
		/**
		* \var NetEventList* m_netevents
		* \brief Liste der lokalen NetEvents beim aktuellen update
		*/
		NetEventList* m_netevents;
		
		/**
		 * \var std::map<LocationName, Vector> m_locations
		 * \brief Liste der Orte
		 */
		std::map<LocationName, Vector> m_locations;
		
		/**
		 * \var std::map<AreaName, Shape> m_areas
		 * \brief Liste der Orte
		 */
		std::map<AreaName, Shape> m_areas;
		
		/**
		 * \var std::list<RegionExit> m_region_exits
		 * \brief Liste der Ausgaenge aus der Region
		 */
		std::list<RegionExit> m_exits;
		
		/**
		 * \var LocationName m_revive_location
		 * \brief Ort an dem in dieser Region gestorbene Helden wiedererweckt werden
		 */
		LocationName m_revive_location;
		
		/**
		 * \var std::list<Trigger*> m_triggers
		 * \brief Trigger die waehrend dem aktuellen Event ausgeloest wurden
		 */
		std::list<Trigger*> m_triggers;
		
		/**
		 * \var std::list<std::pair<float, Trigger*> > m_timed_trigger
		 * \brief Liste der Trigger die mit Zeitverzoegerung ausgeloest werden
		 */
		std::list<std::pair<float, Trigger*> > m_timed_trigger;
		
		/**
		 * \var std::multimap<TriggerType, Event*> m_events
		 * \brief Liste der registrierten Events, aufgelistet nach dem Trigger durch den sie ausgeloest werden
		 */
		std::multimap<TriggerType, Event*> m_events;
		
		/**
		 * \var std::map<std::string,Damage> m_damage_objects
		 * \brief Liste der Schadensobjekte
		 */
		std::map<std::string,Damage> m_damage_objects;
		
		/**
		 * \var std::map<int, Dialogue*> m_dialogues
		 * \brief Liste der Dialoge
		 */
		std::map<int, Dialogue*> m_dialogues;
		
		/**
		 * \var std::map<int,RegionLocation>  m_teleport_players;
		 * \brief Liste der Spieler die sich aus der Region per Wegpunkt entfernen
		 */
		std::map<int,RegionLocation>  m_teleport_players;
		
		/**
		 * \var RegionCamera m_camera
		 * \brief Kameraposition von der aus die Region bei Zwischenszenen gesehen wird
		 */
		RegionCamera m_camera;
		
		/**
		 * \var std::string m_ground_material
		 * \brief Material mit dem der Boden gerendert wird
		 */
		std::string m_ground_material;
		
		/**
		 * \var float m_ambient_light[3]
		 * \brief Werte fuer ambientes Licht
		 */
		float m_ambient_light[3];
		
		/**
		 * \var float m_hero_light[3]
		 * \brief Werte fuer das Spotlicht des Helden
		 */
		float m_hero_light[3];
		
		/**
		 * \var float m_directional_light[3]
		 * \brief Werte fuer das Licht von Sonne/Mond
		 */
		float m_directional_light[3];
		
		/**
		 * \var Vector m_waypoint_location
		 * \brief Position des Wegpunktes
		 */
		Vector m_waypoint_location;

};

#include "gridunit.h"

#endif
