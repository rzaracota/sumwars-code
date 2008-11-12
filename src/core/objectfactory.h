#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H



#include <map>
#include "monsterbase.h"
#include "objectloader.h"


#include "../tinyxml/tinyxml.h"
#include <string>
#include <list>


class World;




/**
 * Bezeichnung fuer eine Umgebung
 */
typedef std::string EnvironmentName;

/**
 * Liste von Typbezeichnungen fuer Objekte
 */
typedef std::list<WorldObject::TypeInfo::ObjectSubtype> WorldObjectTypeList;

/**
 * Name fuer ein Objekt in einem Template
 * kann entweder ein Objektname sein oder ein generischer Name
 * generische Name beginnen mit $
 */
typedef std::string ObjectTemplateType;

/**
 * \class ObjectTemplate
 * \brief Platzhalter fuer ein Objekt. Das Objekt das daraus tatsaechlich erzeugt wird ist von der Umgebung abhaengig
 */
class ObjectTemplate
{
	private:
		/**
	 * \var std::map<EnvironmentName, WorldObjectTypeList > m_env_objects
	 * \brief Gibt fuer jede Umgebung eine Liste von Objekten an, die den Platzhalter ersetzen
		 */
		std::map<EnvironmentName, WorldObjectTypeList > m_env_objects;
		
	
	public:
		
		/**
		 * \var WorldObject::TypeInfo::ObjectType m_type
		 * \brief Basistyp des Objekts
		 */
		WorldObject::TypeInfo::ObjectType m_type;
		
		/**
		 * \fn void addObject(EnvironmentName env, WorldObject::TypeInfo::ObjectSubtype object)
		 * \brief Fuegt fuer die Umgebung ein Objekt hinzu
		 * \param env Name der Umgebung
		 * \param object Typ des Objekts
		 */
		void addObject(EnvironmentName env, WorldObject::TypeInfo::ObjectSubtype object)
		{
			m_env_objects[env].push_back(object);
		}
		
		/**
		 * \fn WorldObject::TypeInfo::ObjectSubtype getObject(EnvironmentName env)
		 * \brief Gibt fuer eine gegebene Umgebung ein passendes Objekt aus
		 * \param env Umgebung
		 */
		WorldObject::TypeInfo::ObjectSubtype getObject(EnvironmentName env);
};


/**
 * Name einer Gruppe von Objekten
 */
typedef std::string ObjectGroupTemplateName;

/**
 * Name eines Ortes
 */
typedef std::string LocationName;


/**
 * \class ObjectGroupTemplate
 * \brief Gruppe von Objekten in einer vorgegebenen Anordnung
 */
class ObjectGroupTemplate
{
	public:
		/**
	 * \struct GroupObject
	 * \brief Objekt das Bestandteil der Gruppe ist
		 */
		struct GroupObject
		{
			/**
			 * \var ObjectTemplateType m_type
			 * \brief Typ des Objektes. Beginnt mit $ fuer generische Objekte
			 */
			ObjectTemplateType m_type;
			
			/**
			 * \var Vector m_center
			 * \brief Ortsvektor des Mittelpunktes relativ zum Mittelpunkt des Templates
			 */
			Vector m_center;
			
			/**
			 * \var float m_angle
			 * \brief Drehwinkel
			 */
			float m_angle;
			
			/**
			 * \var float m_probability
			 * \brief Chance, dass das Objekt eingesetzt wird
			 */
			float m_probability;
			
			/**
			 * \var bool m_prob_angle
			 * \brief Wenn auf true gesetzt, wird der winkel zufaellig gesetzt
			 */
			bool m_prob_angle;
		};
	
		/**
		 * \fn Shape* getShape()
		 * \brief Gibt die Form aus
		 */
		Shape* getShape()
		{
			return &m_shape;
		}
		
		/**
		 * \fn std::list<GroupObject>* getObjects()
		 * \brief Gibt die Objekte aus
		 */
		std::list<GroupObject>* getObjects()
		{
			return &m_objects;
		}
		
		/**
		 * \fn std::map<std::string, Vector>* getLocations()
		 * \brief Gibt die Orte aus
		 */
		std::map<std::string, Vector>* getLocations()
		{
			return &m_locations;
		}
		
		/**
		 * \fn void addLocation(LocationName name, Vector pos)
		 * \brief Fuegt einen benannten Ort in das Template ein
		 * \param name Name des Ortes
		 * \param pos Position relativ zum Mittelpunkt
		 */
		void addLocation(LocationName name, Vector pos)
		{
			m_locations[name] = pos;
		}
		
		/**
		 * \fn void addWaypoint(Vector pos)
		 * \brief Fuegt einen Wegpunkt in das Template ein
		 * \param pos Position
		 */
		void addWaypoint(Vector pos)
		{
			m_waypoint = pos;
		}
		
		/**
		 * \fn void addObject(GroupObject gobj)
		 * \brief Fuegt ein Objekt dem Template hinzu
		 * \param gobj Objekt
		 */
		void addObject(GroupObject gobj)
		{
			m_objects.push_back(gobj);
		}
		
		/**
		 * \fn void addObject(ObjectTemplateType objtype, Vector pos, float angle=0, float probability = 1.0)
		 * \brief Fuegt ein neues Objekt in die Gruppe ein
		 * \param objtype Typ des eingefuegten Objekts
		 * \param pos Position relativ zum Mittelpunkt
		 * \param angle Drehwinkel
		 * \param m_probability Wahrscheinlichkeit, dass das Objekt vertreten ist
		 */
		void addObject(ObjectTemplateType objtype, Vector pos, float angle=0, float probability =1.0);
	
	private:
		/**
	 * \var Shape m_shape
	 * \brief Form der Gruppe
		 */
		Shape m_shape;
		
		/**
		 * \var Vector m_waypoint
		 * \brief der Punkt an dem das Template an das Wegenetz angebunden wird
		 */
		Vector m_waypoint;
		
		/**
		 * \var std::list<GroupObject> m_objects
		 * \brief Liste der Objekte in der Gruppe
		 */
		std::list<GroupObject> m_objects;
		
		/**
		 * \var std::map<LocationName, Vector> m_locations
		 * \brief Orte mit Namen
		 */
		std::map<LocationName, Vector> m_locations;
};




/**
 * \class ObjectFactory
 * \brief Erzeugt Objekte vom Typ WorldObject
 */
class ObjectFactory
{
	public:
		
	/**
	 * \fn static WorldObject* createObject(WorldObject::TypeInfo::ObjectType type, WorldObject::TypeInfo::ObjectSubtype subtype, int id=0)
	 * \brief Erzeugt ein Objekt des angefordertens Typs/Subtyps
	 * \param type Typ des Objektes
	 * \param subtype Subtyp des Objektes
	 * \param id ID des Objektes. Wenn id==0 wird eine neue ID generiert
	 */
	static WorldObject* createObject(WorldObject::TypeInfo::ObjectType type, WorldObject::TypeInfo::ObjectSubtype subtype, int id=0);
	
	
	/**
	 * \fn static WorldObject::TypeInfo::ObjectSubtype getObjectType(ObjectTemplateType generictype, EnvironmentName env)
	 * \brief erzeugt aus einem ObjektTemplate einen Objekttyp passend zu seiner Umgebung
	 * \param generictype generischer Typ des Objekts
	 * \param env Umgebung
	 */
	static WorldObject::TypeInfo::ObjectSubtype getObjectType(ObjectTemplateType generictype, EnvironmentName env);
	
	/**
	 * \fn static ObjectGroupTemplate* getObjectGroupTemplate(ObjectGroupTemplateName name)
	 * \brief sucht zu dem betreffenden Name das passende Template heraus
	 * \param name Name der Objektgruppe
	 */
	static ObjectGroupTemplate* getObjectGroupTemplate(ObjectGroupTemplateName name);
	
	/**
	 * \fn static MonsterGroup* getMonsterGroup(MonsterGroupName name)
	 * \brief Gibt die Monstergruppe zu dem angegebenen Name aus
	 */
	static MonsterGroup* getMonsterGroup(MonsterGroupName name);
	
	/**
	 * \fn Laedt die Daten zu Monstern aus der Datei
	 * \param file Monster XML Datei
	 */
	static void loadMonsterData(std::string file);
	
	/**
	 * \fn static void loadFixedObjectData(std::string file)
	 * \brief Laedt Daten zu festen Objekten aus der Datei
	 * \param file Objekte XML Datei
	 */
	static void loadFixedObjectData(std::string file);
	
	/**
	 * \fn static void loadObjectTemplates(std::string file)
	 * \brief Laedt Daten zu festen Objekten aus der Datei
	 * \param file Objekttemplate XML Datei
	 */
	static void loadObjectTemplates(std::string file);
	
	/**
	 * \fn static void loadObjectGroupTemplates(std::string file)
	 * \brief Laedt Daten zu festen Objektgruppen aus der Datei
	 * \param file Objektgruppentemplate XML Datei
	 */
	static void loadObjectGroupTemplates(std::string file);
	
	/**
	 * \fn static init()
	 * \brief registriert die Daten der Monster
	 */
	static void init();
	
	/**
	 * \fn static void cleanup()
	 * \brief Gibt den Speicher aller Daten frei
	 */
	static void cleanup();
	
	private:	
	/**
	 * \var static std::map<WorldObject::TypeInfo::ObjectSubtype, MonsterBasicData*> m_monster_data
	 * \brief Basisdaten zu den Monstern
	 */
	static std::map<WorldObject::TypeInfo::ObjectSubtype, MonsterBasicData*> m_monster_data;
	
	/**
	 * \var static std::map<WorldObject::TypeInfo::ObjectSubtype, FixedObjectData*> m_fixed_object_data
	 * \brief Basisdaten zu den festen Objekten
	 */
	static std::map<WorldObject::TypeInfo::ObjectSubtype, FixedObjectData*> m_fixed_object_data;
	
	/**
	 * \var static std::map<ObjectTemplateType, ObjectTemplate*> m_object_templates
	 * \brief Datenbank fuer die Objekttemplates, indexiert nach Typ
	 */
	static std::map<ObjectTemplateType, ObjectTemplate*> m_object_templates;
	
	/**
	 * \var static std::map<ObjectGroupTemplateName, ObjectGroupTemplate*> m_object_group_templates
	 * \brief Datenbank fuer die Objektgruppen indexiert nach Name
	 */
	static std::map<ObjectGroupTemplateName, ObjectGroupTemplate*> m_object_group_templates;
	
	/**
	 * \var static std::map< MonsterGroupName, MonsterGroup*>  m_monster_groups
	 * \brief Liste von Monstern die von Spawnpoints erzeugt werden
	 */
	static std::map< MonsterGroupName, MonsterGroup*>  m_monster_groups;
	
	/**
	 * \fn static registerMonster(WorldObject::TypeInfo::ObjectSubtype subtype, MonsterBasicData* data)
	 * \brief Registriert die Daten fuer ein bestimmten Monstertyp
	 * \param subtype Subtyp des Monsters
	 * \param data Daten des Monsters
	 */
	static void registerMonster(WorldObject::TypeInfo::ObjectSubtype subtype, MonsterBasicData* data);
	
	/**
	 * \fn static void registerFixedObject(WorldObject::TypeInfo::ObjectSubtype subtype, FixedObjectData* data)
	 * \brief Registriert die Daten fuer ein bestimmten Monstertyp
	 * \param subtype Subtyp des Objektes
	 * \param data Daten des festen Objekts
	 */
	static void registerFixedObject(WorldObject::TypeInfo::ObjectSubtype subtype, FixedObjectData* data);
	
	/**
	 * \fn static registerObjectTemplate(ObjectTemplateType type, ObjectTemplate* templ)
	 * \brief Registriert ein neues Objekttemplate
	 * \param type Typ des Templates
	 * \param templ die Daten
	 */
	static void registerObjectTemplate(ObjectTemplateType type, ObjectTemplate* templ);
	
	
	/**
	 * \fn static registerObjectGroupTemplate(ObjectGroupTemplateName name, ObjectGroupTemplate* data)
	 * \brief Registriert ein neues Template fuer eine Objektgruppe
	 * \param name der Name des Templates
	 * \param data die Daten
	 */
	static void registerObjectGroupTemplate(ObjectGroupTemplateName name, ObjectGroupTemplate* data);
	
	/**
	 * \fn static void registerMonsterGroup(MonsterGroupName name, MonsterGroup data)
	 * \brief Registriert eine Gruppe von Monstern
	 * \param name Name der Gruppe
	 * \param group Daten
	 */
	static void registerMonsterGroup(MonsterGroupName name, MonsterGroup* data);
	
	
};



#endif

