#ifndef OBJECTLOADER_H
#define OBJECTLOADER_H


#include <map>
#include <list>
#include <vector>
#include "monsterbase.h"
#include "action.h"


#include "../tinyxml/tinyxml.h"
#include <string>
#include <cstring>


/**
 * \struct ItemMeshData
 * \brief Enthaelt die Daten, die zum Laden des Meshs fuer ein Monster notwendig sind
 */
struct MonsterMeshData
{
	/**
	 * \var std::string m_subtype
	 * \brief Subtyp des Monsters
	 */
	std::string m_subtype;
	
	/**
	 * \var string m_mesh
	 * \brief Name des Meshes
	 */
	std::string m_mesh;
	
	/**
	 * \fn std::map<Action::ActionType, std::list<std::string> > m_animations
	 * \brief Animationen des Monsters sortiert nach Typ der Aktion
	 */
	std::map<Action::ActionType, std::vector<std::string> > m_animations;
};

/**
 * \struct FixedObjectData
 * \brief Struktur mit den Daten von fixenObjekten
 */
struct FixedObjectData
{
	/**
	* \var short m_layer
	* \brief Ebene in der sich das Objekt befindet
	*/
	short m_layer;

	/**
	* \var Shape m_shape
	* \brief Form des Objektes
	*/
	Shape m_shape;
};


/**
 * \struct ItemMeshData
 * \brief Enthaelt die Daten, die zum Laden des Meshs fuer ein Monster notwendig sind
 */
struct FixedObjectMeshData
{
	/**
	 * \var std::string m_subtype
	 * \brief Subtyp des festen Objekts
	 */
	std::string m_subtype;
	
	/**
	 * \var string m_mesh
	 * \brief Name des Meshes
	 */
	std::string m_mesh;
	
	
};


struct TempMonsterGroup
{
	int number;
	float prob;
	WorldObject::TypeInfo::ObjectSubtype subtype;
};


/**
 * \class ObjectLoader
 * \brief Laedt Objecte aus einer XML-Datei
 */
class ObjectLoader
{
	public:
	
	/**
	 * \fn void loadMonsterBasicData(const char* pFilename)
	 * \brief Sorgt fuer das Anlegen von MonsterBasicData, die in einer XML-Datei gespeichert sind
	 * \param pFilename Pfad zu einer XML-Datei, die Objekte enthaelt
	 * \return Liste der geladenen Items
	 */
	std::list<MonsterBasicData*>* loadMonsterBasicData(const char* pFilename);
	
	/**
	 * \fn void loadMonsterMeshData(const char* pFilename)
	 * \brief Sorgt fuer das Anlegen von MonsterMeshData, die in einer XML-Datei gespeichert sind
	 * \param pFilename Pfad zu einer XML-Datei, die Monster enthaelt
	 * \return Liste der geladenen Monster-Meshes
	 */
	std::list<MonsterMeshData*>* loadMonsterMeshData(const char* pFilename);
	
	/**
	 * \fn void loadFixedObjectData(const char* pFilename)
	 * \brief Sorgt fuer das Anlegen von FixedObjectData, die in einer XML-Datei gespeichert sind
	 * \param pFilename Pfad zu einer XML-Datei, die feste Objekte enthaelt
	 * \return Liste der geladenen festen Objekte
	 */
	bool loadFixedObjectData(const char* pFilename, std::list<FixedObjectData*>* &object_list, std::list<std::string>* &subtype_list);
	
	/**
	 * \fn void loadFixedObjectMeshData(const char* pFilename)
	 * \brief Sorgt fuer das Anlegen von FixedObjectMeshData, die in einer XML-Datei gespeichert sind
	 * \param pFilename Pfad zu einer XML-Datei, die Monster enthaelt
	 * \return Liste der geladenen FixedObject-Meshes
	 */
	std::list<FixedObjectMeshData*>* loadFixedObjectMeshData(const char* pFilename);
	
	bool loadMonsterGroup(const char* pFilename, std::list<MonsterGroup*> &monster_group_list, std::list<std::string> &name_list);
	
	private:
	
	/**
	 * \fn int generateMonsterBasicData(TiXmlElement* pElement, std::string element)
	 * \brief Legt Monster im Speicher anhand von Daten an, die als Parameter uebergeben wurden. Diese Daten stammen aus einer XML-Datei.
	 * \param pElement Zeiger auf ein Element der XML-Datei
	 * \param element Name des Elements
	 * \return Anzahl der Attribute des bearbeiteten Elements
	 */
	int generateMonsterBasicData(TiXmlElement* pElement, std::string element);
	
	/**
	 * \fn void searchMonsterBasicData( TiXmlNode* pParent )
	 * \brief Durchmustert eine XML-Datei und sucht nach Elementen.
	 * \param pParent Zeiger auf eine XML-Datei
	 */
	void searchMonsterBasicData(TiXmlNode* pParent);
	
	/**
	 * \fn int generateMonsterMeshData(TiXmlElement* pElement, std::string element)
	 * \brief Legt MonsterMeshData im Speicher anhand von Daten an, die als Parameter uebergeben wurden. Diese Daten stammen aus einer XML-Datei.
	 * \param pElement Zeiger auf ein Element der XML-Datei
	 * \param element Name des Elements
	 * \return Anzahl der Attribute des bearbeiteten Elements
	 */
	int generateMonsterMeshData(TiXmlElement* pElement, std::string element);
	
	/**
	 * \fn void searchMonsterMeshData( TiXmlNode* pParent )
	 * \brief Durchmustert eine XML-Datei und sucht nach Elementen.
	 * \param pParent Zeiger auf eine XML-Datei
	 */
	void searchMonsterMeshData(TiXmlNode* pParent);
	
	/**
	 * \fn int generateFixedObjectData(TiXmlElement* pElement, std::string element)
	 * \brief Legt FixedObjectData im Speicher anhand von Daten an, die als Parameter uebergeben wurden. Diese Daten stammen aus einer XML-Datei.
	 * \param pElement Zeiger auf ein Element der XML-Datei
	 * \param element Name des Elements
	 * \return Anzahl der Attribute des bearbeiteten Elements
	 */
	int generateFixedObjectData(TiXmlElement* pElement, std::string element, std::list<FixedObjectData*>* object_list, std::list<std::string>* subtype_list);
	
	/**
	 * \fn void searchFixedObjectData( TiXmlNode* pParent )
	 * \brief Durchmustert eine XML-Datei und sucht nach Elementen.
	 * \param pParent Zeiger auf eine XML-Datei
	 */
	void searchFixedObjectData(TiXmlNode* pParent, std::list<FixedObjectData*>* object_list, std::list<std::string>* subtype_list);
	
	/**
	 * \fn int generateFixedObjectMeshData(TiXmlElement* pElement, std::string element)
	 * \brief Legt FixedObjectMeshData im Speicher anhand von Daten an, die als Parameter uebergeben wurden. Diese Daten stammen aus einer XML-Datei.
	 * \param pElement Zeiger auf ein Element der XML-Datei
	 * \param element Name des Elements
	 * \return Anzahl der Attribute des bearbeiteten Elements
	 */
	int generateFixedObjectMeshData(TiXmlElement* pElement, std::string element);
	
	/**
	 * \fn void searchFixedObjectMeshData( TiXmlNode* pParent )
	 * \brief Durchmustert eine XML-Datei und sucht nach Elementen.
	 * \param pParent Zeiger auf eine XML-Datei
	 */
	void searchFixedObjectMeshData(TiXmlNode* pParent);
	
	int generateMonsterGroup(TiXmlElement* pElement, std::string element, std::list<MonsterGroup*> &monster_group_list, std::list<std::string> &name_list);
	
	void searchMonsterGroup(TiXmlNode* pParent, std::list<MonsterGroup*> &monster_group_list, std::list<std::string> &name_list);
	
	
	/**
	 * \var MonsterBasicData* m_monster_data
	 * \brief Daten der Monster
	 */
	MonsterBasicData* m_monster_data;
	/**
	 * \var std::list<MonsterBasicData*>* m_monster_list
	 * \brief Liste mit Daten der Monster
	 */
	std::list<MonsterBasicData*>* m_monster_list;
	
	/**
	 * \var MonsterMeshData* m_monster_mesh_data
	 * \brief Daten zu den Meshes der Monster
	 */
	MonsterMeshData* m_monster_mesh_data;
	/**
	 * \var std::list<MonsterMeshData*>* m_monster_mesh_list
	 * \brief Liste mit Daten zu den Meshes der Monster
	 */
	std::list<MonsterMeshData*>* m_monster_mesh_list;
	
	/**
	 * \var FixedObjectData* m_object_data
	 * \brief Daten zu Objekten
	 */
	FixedObjectData* m_object_data;
	/**
	 * \var std::list<FixedObjectData*>* m_object_list
	 * \brief Liste mit Daten zu Objekten
	 */
	//std::list<FixedObjectData*>* m_object_list;
	
	/**
	 * \var FixedObjectMeshData* m_fixed_object_mesh_data
	 * \brief Daten zu den Meshes der festen Objekte
	 */
	FixedObjectMeshData* m_fixed_object_mesh_data;
	/**
	 * \var std::list<FixedObjectMeshData*>* m_fixed_object_mesh_list
	 * \brief Liste mit Daten zu den Meshes der festen Objekte
	 */
	std::list<FixedObjectMeshData*>* m_fixed_object_mesh_list;
	
	MonsterGroup* m_monster_group;
	
	TempMonsterGroup m_temp_monster_group;
	
	
	/**
	 * \var float m_size_prob[4]
	 * \brief Wahrscheinlichkeit mit der ein Item einer bestimmten Groesse gedroppt wird (Zwischenspeicher-Variable)
	 */
	float m_size_prob[4];
	/**
	 * \var int m_min_level[4]
	 * \brief minimales Level, das das gedroppte Item haben muss (Zwischenspeicher-Variable)
	 */
	int m_min_level[4];
	/**
	 * \var int m_max_level[4]
	 * \brief maximales Level das das gedroppte Item haben darf (Zwischenspeicher-Variable)
	 */
	int m_max_level[4];
	/**
	 * \var float m_magic_prob[4]
	 * \brief Chance, das der Gegenstand magisch wird (Zwischenspeicher-Variable)
	 */
	float m_magic_prob[4];
	/**
	 * \var float m_magic_power[4]
	 * \brief Staerke der Verzauberung, falls der Gegenstand magisch wird (Zwischenspeicher-Variable)
	 */
	float m_magic_power[4];
	
	/**
	 * \var float m_extend[2]
	 * \brief Ausdehnung des Objekt-Grundrechteckes (Zwischenspeicher-Variable)
	 */
	float m_extent[2];
	/**
	 * \var float m_radius
	 * \brief Radius des Objekt-Grundkreises (Zwischenspeicher-Variable)
	 */
	float m_radius;
	
	/**
	 * \var string m_subtype
	 * \brief Subtyp des Objektes (Zwischenspeicher-Variable)
	 */
	std::string m_subtype;
	
	/**
	 * \var Action::ActionType m_action
	 * \brief Typ der Aktion (Zwischenspeicher-Variable)
	 */
	Action::ActionType m_action;

};


#endif

