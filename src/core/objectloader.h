#ifndef OBJECTLOADER_H
#define OBJECTLOADER_H


#include <map>
#include <list>
#include "monsterbase.h"

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
};

/**
 * \struct FixedObjectData
 * \brief Struktur mit den Daten von festen Objekten
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
 * \class ObjectLoader
 * \brief Laedt Objecte aus einer XML-Datei
 */
class ObjectLoader
{
	public:
	
	/**
	 * \fn void loadObjects(const char* pFilename)
	 * \brief Sorgt fuer das Anlegen von Objekten, die in einer XML-Datei gespeichert sind
	 * \param pFilename Pfad zu einer XML-Datei, die Objekte enthaelt
	 * \return Liste der geladenen Items
	 */
	std::list<MonsterBasicData*>* loadObjects(const char* pFilename);
	
	/**
	 * \fn void loadMonsterMeshData(const char* pFilename)
	 * \brief Sorgt fuer das Anlegen von MonsterMeshData, die in einer XML-Datei gespeichert sind
	 * \param pFilename Pfad zu einer XML-Datei, die Monster enthaelt
	 * \return Liste der geladenen Monster
	 */
	std::list<MonsterMeshData*>* loadMonsterMeshData(const char* pFilename);
	
	private:
	
	/**
	 * \fn int generateObjects(TiXmlElement* pElement, std::string element)
	 * \brief Legt Objekte im Speicher anhand von Daten an, die als Parameter uebergeben wurden. Diese Daten stammen aus einer XML-Datei.
	 * \param pElement Zeiger auf ein Element der XML-Datei
	 * \param element Name des Elements
	 * \return Anzahl der Attribute des bearbeiteten Elements
	 */
	int generateObjects(TiXmlElement* pElement, std::string element);
	
	/**
	 * \fn void searchXml( TiXmlNode* pParent )
	 * \brief Durchmustert eine XML-Datei und sucht nach Elementen.
	 * \param pParent Zeiger auf eine XML-Datei
	 */
	void searchXml(TiXmlNode* pParent);
	
	/**
	 * \fn int generateMonsterMeshData(TiXmlElement* pElement, std::string element)
	 * \brief Legt ItemMonsterData im Speicher anhand von Daten an, die als Parameter uebergeben wurden. Diese Daten stammen aus einer XML-Datei.
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
	
	
	MonsterBasicData* m_monster_data;
	std::list<MonsterBasicData*>* m_monster_list;
	
	MonsterMeshData* m_monster_mesh_data;
	std::list<MonsterMeshData*>* m_monster_mesh_list;
	
	float m_size_prob[4];
	int m_min_level[2];
	int m_max_level[2];
	float m_magic_prob[2];
	float m_magic_power[2];
};


#endif

