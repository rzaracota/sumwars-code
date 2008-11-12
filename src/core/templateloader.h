#ifndef TEMPLATELOADER_H
#define TEMPLATELOADER_H


#include <map>
#include <list>
#include "region.h"
#include "objectfactory.h"

#include "../tinyxml/tinyxml.h"
#include <string>
#include <cstring>


// FIXME eventuell gar nicht benoetigt
/**
 * \struct WorldData
 * \brief Enthaelt die Daten, die zum Laden von World-Templates notwendig sind
 */
/*struct WorldData
{
	// TODO
};*/
// END FIXME

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

struct TempShape
{
	Shape::ShapeType type;
	float radius;
	float extent_x;
	float extent_y;
	float angle;
};

struct TempWaypoint
{
	float pos_x;
	float pos_y;
};

struct TempLocation
{
	std::string name;
	float pos_x;
	float pos_y;
};

struct TempGroupObject
{
	float angle;
	float center_x;
	float center_y;
	bool prob_angle;
	float probability;
	ObjectTemplateType type;
};

/**
 * \class TemplateLoader
 * \brief Laedt verschiedene Templates aus einer XML-Datei
 */
class TemplateLoader
{
	public:
	
	/**
	 * \fn void loadRegionData(const char* pFilename)
	 * \brief Sorgt fuer das Anlegen von FixedObjectData, die in einer XML-Datei gespeichert sind
	 * \param pFilename Pfad zu einer XML-Datei, die feste Objekte enthaelt
	 * \return Liste der geladenen festen Objekte
	 */
	//bool loadWorldData(const char* pFilename, std::list<WorldData*>* &world_list, std::list<std::string>* &subtype_list);
	bool loadRegionData(const char* pFilename, std::list<RegionData*> &region_list);

	bool loadObjectTemplate(const char* pFilename, std::list<ObjectTemplate*> &object_template_list, std::list<std::string> &name_list);
	
	bool loadObjectGroupTemplate(const char* pFilename, std::list<ObjectGroupTemplate*> &object_group_template_list, std::list<std::string> &name_list);
	
	
	private:
	
	/**
	 * \fn int generateWorldData(TiXmlElement* pElement, std::string element)
	 * \brief Legt FixedObjectData im Speicher anhand von Daten an, die als Parameter uebergeben wurden. Diese Daten stammen aus einer XML-Datei.
	 * \param pElement Zeiger auf ein Element der XML-Datei
	 * \param element Name des Elements
	 * \return Anzahl der Attribute des bearbeiteten Elements
	 */
	int generateRegionData(TiXmlElement* pElement, std::string element, std::list<RegionData*> &region_list);
	
	/**
	 * \fn void searchWorldData( TiXmlNode* pParent )
	 * \brief Durchmustert eine XML-Datei und sucht nach Elementen.
	 * \param pParent Zeiger auf eine XML-Datei
	 */
	void searchRegionData(TiXmlNode* pParent, std::list<RegionData*> &region_list);
	
	int generateObjectTemplate(TiXmlElement* pElement, std::string element, std::list<ObjectTemplate*> &object_template_list, std::list<std::string> &name_list);
	
	void searchObjectTemplate(TiXmlNode* pParent, std::list<ObjectTemplate*> &object_template_list, std::list<std::string> &name_list);
	
	int generateObjectGroupTemplate(TiXmlElement* pElement, std::string element, std::list<ObjectGroupTemplate*> &object_template_group_list, std::list<std::string> &name_list);
	
	void searchObjectGroupTemplate(TiXmlNode* pParent, std::list<ObjectGroupTemplate*> &object_group_template_list, std::list<std::string> &name_list);
	
	/**
	 * \var WorldData* m_object_data
	 * \brief Daten zu Objekten
	 */
	RegionData* m_region_data;
	
	ObjectTemplate* m_object_template;
	
	ObjectGroupTemplate* m_object_group_template;
	
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
	
	TempShape m_temp_shape;
	TempWaypoint m_temp_waypoint;
	TempLocation m_temp_location;
	TempGroupObject m_temp_group_object;
	std::string m_current_environment_name;

};


#endif

