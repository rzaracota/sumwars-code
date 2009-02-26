#ifndef WORLDLOADER_H
#define WORLDLOADER_H


#include <map>
#include <list>
#include "region.h"
#include "objectfactory.h"
#include "event.h"
#include "dialogue.h"
#include "quest.h"

#include "../tinyxml/tinyxml.h"
#include <string>
#include <cstring>
#include "elementattrib.h"




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
	 * \brief Liest die Daten zu NPCs aus einer XML Datei
	 * \param pFilename Name der XML Datei
	 */
	bool loadNPCData(const char* pFilename);
	
	/**
	 * \fn loadQuestsData(const char* pFilename)
	 * \brief Liest die Daten zu Quests aus einer XML Datei
	 * \param pFilename Name der XML Datei
	 */
	bool loadQuestsData(const char* pFilename);
	
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
	 * \fn void loadQuests(TiXmlNode* node)
	 * \brief Liest die Daten zu Quests aus einer XML Datei
	 * \param node XML Knoten
	 */
	void loadQuests(TiXmlNode* node);
	
	/**
	 * \fn void loadQuest(TiXmlNode* node, Quest* quest)
	 * \brief Laedt die Daten zu einem Quest
	 * \param node Knoten, der die Daten enthaelt
	 * \param quest Klasse, in die die Daten geschrieben werden
	 */
	void loadQuest(TiXmlNode* node, Quest* quest);
	
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
	
	LocationName m_temp_revive_location;
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

