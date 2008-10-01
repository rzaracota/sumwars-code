#ifndef ITEMLOADER_H
#define ITEMLOADER_H

#include "item.h"
#include "itemfactory.h"
#include <map>
#include "dropslot.h"
#include <list>

#include "../tinyxml/tinyxml.h"
#include <string>
#include <cstring>
using namespace std;

/**
 * \struct ItemMeshData
 * \brief Enthaelt die Daten, die zum Laden des Meshs fuer einen Gegenstand notwendig sind
 */
struct ItemMeshData
{
	/**
	 * \var string m_subtype
	 * \brief Subtyp des Gegenstandes
	 */
	string m_subtype;
	
	/**
	 * \var string m_mesh
	 * \brief Name des Meshes
	 */
	string m_mesh;
};

/**
 * \struct DropChanceData
 * \brief Enthaelt die Daten fuer die Einstellung der DropChance
 */
struct DropChanceData
{
	int m_level;
	float m_probability;
	Item::Size m_size;
	string m_subtype;
	Item::Type m_type;
};


/**
 * \class ItemLoader
 * \brief Laedt Items aus einer XML-Datei
 */
class ItemLoader
{
	public:
	
	/**
	 * \fn ~ItemLoader()
	 * \brief Destruktor
	 */
	~ItemLoader();
	
	/**
	 * \fn void loadItemBasicData(const char* pFilename)
	 * \brief Sorgt fuer das Anlegen von Items, die in einer XML-Datei gespeichert sind
	 * \param pFilename Pfad zu einer XML-Datei, die Items enthaelt
	 * \return Liste der geladenen Items
	 */
	list<ItemBasicData*>* loadItemBasicData(const char* pFilename);
	
	list<DropChanceData*>* loadDropChanceData(const char* pFilename);
	
	/**
	 * \fn void loadItemMeshData(const char* pFilename)
	 * \brief Sorgt fuer das Anlegen von ItemMeshData, die in einer XML-Datei gespeichert sind
	 * \param pFilename Pfad zu einer XML-Datei, die Items enthaelt
	 * \return Liste der geladenen Items
	 */
	list<ItemMeshData*>* loadItemMeshData(const char* pFilename);
	
	
	private:
	
	/**
	 * \fn int generateItemBasicData(TiXmlElement* pElement, string element)
	 * \brief Legt Items im Speicher anhand von Daten an, die als Parameter uebergeben wurden. Diese Daten stammen aus einer XML-Datei.
	 * \param pElement Zeiger auf ein Element der XML-Datei
	 * \param element Name des Elements
	 * \return Anzahl der Attribute des bearbeiteten Elements
	 */
	int generateItemBasicData(TiXmlElement* pElement, string element);
	
	int generateDropChanceData(TiXmlElement* pElement, string element);
	
	/**
	 * \fn int generateItemMeshData(TiXmlElement* pElement, string element)
	 * \brief Legt ItemMeshData im Speicher anhand von Daten an, die als Parameter uebergeben wurden. Diese Daten stammen aus einer XML-Datei.
	 * \param pElement Zeiger auf ein Element der XML-Datei
	 * \param element Name des Elements
	 * \return Anzahl der Attribute des bearbeiteten Elements
	 */
	int generateItemMeshData(TiXmlElement* pElement, string element);
	
	/**
	 * \fn void searchItemBasicData( TiXmlNode* pParent )
	 * \brief Durchmustert eine XML-Datei und sucht nach Elementen.
	 * \param pParent Zeiger auf eine XML-Datei
	 */
	void searchItemBasicData(TiXmlNode* pParent);
	
	void searchDropChanceData(TiXmlNode* pParent);
	
	/**
	 * \fn void searchItemMeshData( TiXmlNode* pParent )
	 * \brief Durchmustert eine XML-Datei und sucht nach Elementen.
	 * \param pParent Zeiger auf eine XML-Datei
	 */
	void searchItemMeshData(TiXmlNode* pParent);
	
	
	ItemBasicData* m_item_data;
	list<ItemBasicData*>* m_item_list;
	
	DropChanceData* m_drop_chance_data;
	list<DropChanceData*>* m_drop_chance_data_list;
	
	ItemMeshData* m_item_mesh_data;
	list<ItemMeshData*>* m_item_mesh_list;
	
	float m_weapon_mod[31];
};


#endif

