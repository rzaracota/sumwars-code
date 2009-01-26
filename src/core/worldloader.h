#ifndef WORLDLOADER_H
#define WORLDLOADER_H


#include <map>
#include <list>
#include "region.h"
#include "objectfactory.h"
#include "event.h"
#include "dialogue.h"

#include "../tinyxml/tinyxml.h"
#include <string>
#include <cstring>

/**
 * \class ElementAttrib
 * \brief Helferklasse zum zwischenspeichert der Attribute eines Elements
 */
class ElementAttrib
{
	public:
		/**
		 * \fn void parseElement(TiXmlElement* elem)
		 * \brief Parst die Attribute des Elements
		 * \param elem XML Element
		 */
		void parseElement(TiXmlElement* elem);
	
		/**
		 * \fn void getString(std::string attrib, std::string& data, std::string def ="")
		 * \brief Gibt ein Attribut als String aus.
		 * \param attrib Name des Attributs
		 * \param data Ausgabeparameter fuer die Daten
		 * \param def String, der ausgegeben wird, wenn das Attribut nicht existiert
		 */
		void getString(std::string attrib, std::string& data, std::string def ="");
	
		/**
		 * \fn void getInt(std::string attrib, int& data, int def =0)
		 * \brief Gibt ein Attribut als Integer aus.
		 * \param attrib Name des Attributs
		 * \param data Ausgabeparameter fuer die Daten
		 * \param def Zahl, die ausgegeben wird, wenn das Attribut nicht existiert oder sich nicht zu int konvertieren laesst
		 */
		void getInt(std::string attrib, int& data, int def =0);
	
	
		/**
		 * \fn void getInt(std::string attrib, int& data, int def =0)
		 * \brief Gibt ein Attribut als Float aus.
		 * \param attrib Name des Attributs
		 * \param data Ausgabeparameter fuer die Daten
		 * \param def Zahl, die ausgegeben wird, wenn das Attribut nicht existiert oder sich nicht zu float konvertieren laesst
		 */
		void getFloat(std::string attrib, float& data, float def =0);
	
	private:
		/**
		 * \var std::map<std::string, TiXmlAttribute*> m_attribs
		 * \brief Map zum zwischenspeichern der Attribute
		 */
		std::map<std::string, TiXmlAttribute*> m_attribs;
};

struct TempReviveLocation
{
	std::string region;
	std::string location;
};

struct TempNameObjectGroup
{
	void init()
	{
		prio = 0;
	}
	ObjectGroupTemplateName group_name;
	std::string name;
	int prio;
};

struct TempObjectGroup
{
	void init()
	{
		prio=0;
		number=1;
		probability=1.0;
	}
	ObjectGroupTemplateName group_name;
	int prio;
	int number;
	float probability;
};

struct TempEnvironment
{
	float maxheight;
	EnvironmentName env;
};

struct TempSpawnGroup
{
	void init()
	{
		number=1;
	}
	MonsterGroupName group_name;
	int number;
};

struct TempExit
{
	LocationName destination_location;
	std::string destination_region;
	LocationName exit_name;
	Shape::ShapeType type;
	float extent_x;
	float extent_y;
	float radius;
};


/**
 * \class TemplateLoader
 * \brief Laedt verschiedene Templates aus einer XML-Datei
 */
class WorldLoader
{
	public:
	
	/**
	 * \fn void loadRegionData(const char* pFilename)
	 * \brief Sorgt fuer das Anlegen von FixedObjectData, die in einer XML-Datei gespeichert sind
	 * \param pFilename Pfad zu einer XML-Datei, die feste Objekte enthaelt
	 * \return Liste der geladenen festen Objekte
	 */
	bool loadRegionData(const char* pFilename, std::list<RegionData*> &region_list);
	
	/**
	 * \fn bool loadNPCData(const char* pFilename)
	 * \brief Liest die Daten zu NPC's aus einer XML Datei
	 * \param pFilename Name der XML Datei
	 */
	bool loadNPCData(const char* pFilename);
	
	private:
	
	/**
	 * \fn void loadEvent( TiXmlNode* node, Event *ev, TriggerType &type)
	 * \brief Laedt die Daten zu einem Event aus einem XML Knoten
	 * \param node Knoten der die Daten enthaelt
	 * \param ev Event in das die Daten geschrieben werden
	 * \param type Typ des Triggers, der das Event ausloesen soll (Ausgabeparameter)
	 */
	void loadEvent( TiXmlNode* node, Event *ev, TriggerType &type);
	
	/**
	 * \fn void loadNPC( TiXmlNode* node)
	 * \brief Laedt die Daten zu einem NPC aus einem Knoten
	 * \param node XML Knoten
	 */
	void loadNPC( TiXmlNode* node);
	
	/**
	 * \fn int generateWorldData(TiXmlElement* pElement, std::string element)
	 * \brief Legt FixedObjectData im Speicher anhand von Daten an, die als Parameter uebergeben wurden. Diese Daten stammen aus einer XML-Datei.
	 * \param pElement Zeiger auf ein Element der XML-Datei
	 * \param element Name des Elements
	 * \return Anzahl der Attribute des bearbeiteten Elements
	 */
	int generateRegionData(TiXmlNode* pParent, TiXmlElement* pElement, std::string element, std::list<RegionData*> &region_list);
	
	/**
	 * \fn void searchWorldData( TiXmlNode* pParent )
	 * \brief Durchmustert eine XML-Datei und sucht nach Elementen.
	 * \param pParent Zeiger auf eine XML-Datei
	 */
	void searchRegionData(TiXmlNode* pParent, std::list<RegionData*> &region_list);
	
	
	void loadRegionData(RegionData rdata, TiXmlElement* pElement);
	
	/**
	 * \var WorldData* m_object_data
	 * \brief Daten zu Objekten
	 */
	RegionData* m_region_data;
	
	/**
	 * \var std::list<FixedObjectData*>* m_object_list
	 * \brief Liste mit Daten zu Objekten
	 */
	//std::list<FixedObjectData*>* m_object_list;
	
	TempReviveLocation m_temp_revive_location;
	TempNameObjectGroup m_temp_name_object_group;
	TempObjectGroup m_temp_object_group;
	TempEnvironment m_temp_environment;
	TempSpawnGroup m_spawn_group;
	TempExit m_temp_exit;
	
	/*TempShape m_temp_shape;
	TempWaypoint m_temp_waypoint;
	TempLocation m_temp_location;
	TempGroupObject m_temp_group_object;
	std::string m_current_environment_name;
*/
};


#endif

