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

/**
 * \struct ItemMeshData
 * \brief Enthaelt die Daten, die zum Laden des Meshs fuer einen Gegenstand notwendig sind
 */
struct ItemMeshData
{
	/**
	 * \var std::string m_subtype
	 * \brief Subtyp des Gegenstandes
	 */
	std::string m_subtype;
	
	/**
	 * \var std::string m_mesh
	 * \brief Name des Meshes
	 */
	std::string m_mesh;
};

/**
 * \struct DropChanceData
 * \brief Enthaelt die Daten fuer die Einstellung der DropChance
 */
struct DropChanceData
{
	/**
	 * \var Item::Type m_type
	 * \brief Typ des Items
	 */
	Item::Type m_type;
	
	/**
	 * \var Item::Subtype m_subtype
	 * \brief Subtype des Items
	 */
	Item::Subtype m_subtype;

	/**
	 * \var int m_level
	 * \brief Level des Items
	 */
	int m_level;
	
	/**
	 * \var float m_probability
	 * \brief relative Dropchance
	 */
	float m_probability;
	
	/**
	 * \var Item::Size m_size
	 * \brief Groesse des Items
	 */
	Item::Size m_size;
};

struct ItemImageData
{
	/**
	 * \var std::string m_subtype
	 * \brief Subtyp des Gegenstandes
	 */
	std::string m_subtype;
	
	/**
	 * \var std::string m_image
	 * \brief Bild des Meshes
	 */
	std::string m_image;
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
	 * \fn std::list<ItemBasicData*>* loadItemBasicData(const char* pFilename)
	 * \brief Sorgt fuer das Anlegen von Items, die in einer XML-Datei gespeichert sind
	 * \param pFilename Pfad zu einer XML-Datei, die Items enthaelt
	 * \return Liste der geladenen Items
	 */
	std::list<ItemBasicData*>* loadItemBasicData(const char* pFilename);
	
	/**
	 * \fn std::list<DropChanceData*>* loadDropChanceData(const char* pFilename)
	 * \brief Sorgt fuer das Anlegen von Daten zur Dropchance der Items, die in einer XML-Datei gespeichert sind
	 * \param pFilename Pfad zu einer XML-Datei, die Dtopchance-Daten der Items enthaelt
	 * \return Liste der geladenen Items
	 */
	std::list<DropChanceData*>* loadDropChanceData(const char* pFilename);
	
	/**
	 * \fn std::list<ItemMeshData*>* loadItemMeshData(const char* pFilename)
	 * \brief Sorgt fuer das Anlegen von ItemMeshData, die in einer XML-Datei gespeichert sind
	 * \param pFilename Pfad zu einer XML-Datei, die Items enthaelt
	 * \return Liste der geladenen Items
	 */
	std::list<ItemMeshData*>* loadItemMeshData(const char* pFilename);
	
	bool loadItemImageData(const char* pFilename, std::list<ItemImageData*> &item_image_data_list);
	
	private:
	
	/**
	 * \fn int generateItemBasicData(TiXmlElement* pElement, std::string element)
	 * \brief Legt Items im Speicher anhand von Daten an, die als Parameter uebergeben wurden. Diese Daten stammen aus einer XML-Datei.
	 * \param pElement Zeiger auf ein Element der XML-Datei
	 * \param element Name des Elements
	 * \return Anzahl der Attribute des bearbeiteten Elements
	 */
	int generateItemBasicData(TiXmlElement* pElement, std::string element);
	
	/**
	 * \fn void searchItemBasicData( TiXmlNode* pParent )
	 * \brief Durchmustert eine XML-Datei und sucht nach Elementen.
	 * \param pParent Zeiger auf eine XML-Datei
	 */
	void searchItemBasicData(TiXmlNode* pParent);
	
	/**
	 * \fn int generateDropChanceData(TiXmlElement* pElement, std::string element);
	 * \brief Legt Dropchance-Daten der Items im Speicher anhand von Daten an, die als Parameter uebergeben wurden. Diese Daten stammen aus einer XML-Datei.
	 * \param pElement Zeiger auf ein Element der XML-Datei
	 * \param element Name des Elements
	 * \return Anzahl der Attribute des bearbeiteten Elements
	 */
	int generateDropChanceData(TiXmlElement* pElement, std::string element);
	
	/**
	 * \fn void searchDropChanceData(TiXmlNode* pParent);
	 * \brief Durchmustert eine XML-Datei und sucht nach Elementen.
	 * \param pParent Zeiger auf eine XML-Datei
	 */
	void searchDropChanceData(TiXmlNode* pParent);
	
	/**
	 * \fn int generateItemMeshData(TiXmlElement* pElement, std::string element)
	 * \brief Legt ItemMeshData im Speicher anhand von Daten an, die als Parameter uebergeben wurden. Diese Daten stammen aus einer XML-Datei.
	 * \param pElement Zeiger auf ein Element der XML-Datei
	 * \param element Name des Elements
	 * \return Anzahl der Attribute des bearbeiteten Elements
	 */
	int generateItemMeshData(TiXmlElement* pElement, std::string element);
	
	/**
	 * \fn void searchItemMeshData( TiXmlNode* pParent )
	 * \brief Durchmustert eine XML-Datei und sucht nach Elementen.
	 * \param pParent Zeiger auf eine XML-Datei
	 */
	void searchItemMeshData(TiXmlNode* pParent);
	
	int generateItemImageData(TiXmlElement* pElement, std::string element, std::list<ItemImageData*> &item_image_data_list);
	
	void searchItemImageData(TiXmlNode* pParent, std::list<ItemImageData*> &item_image_data_list);
	
	
	/**
	 * \var ItemBasicData* m_item_data
	 * \brief Basisdaten der Items
	 */
	ItemBasicData* m_item_data;
	/**
	 * \var std::list<ItemBasicData*>* m_item_list
	 * \brief Liste mit Basisdaten der Items
	 */
	std::list<ItemBasicData*>* m_item_list;
	
	/**
	 * \var DropChanceData* m_drop_chance_data
	 * \brief DropChance-Daten der Items
	 */
	DropChanceData* m_drop_chance_data;
	/**
	 * \var std::list<DropChanceData*>* m_drop_chance_data_list
	 * \brief Liste mit DropChance-Daten der Items
	 */
	std::list<DropChanceData*>* m_drop_chance_data_list;
	
	/**
	 * \var ItemMeshData* m_item_mesh_data
	 * \brief Daten zu den Meshes der Items
	 */
	ItemMeshData* m_item_mesh_data;
	/**
	 * \var std::list<ItemMeshData*>* m_item_mesh_list
	 * \brief Liste mit Daten zu den Meshes der Items
	 */
	std::list<ItemMeshData*>* m_item_mesh_list;
	
	ItemImageData* m_item_image_data;
	
	/**
	 * \var float m_weapon_mod[31]
	 * \brief Modifikationen der Items (FIXME Nicht mehr benoetigt, da jetzt aus XML gelesen. Zu Testzwecken vorerst noch nicht geloescht.)
	 */
	float m_weapon_mod[31];
};


#endif

