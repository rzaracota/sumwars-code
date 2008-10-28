#ifndef TEMPLATELOADER_H
#define TEMPLATELOADER_H


#include <map>
#include <list>
//#include "monsterbase.h" // TODO wird hier moeglicherweise nicht gebraucht

#include "../tinyxml/tinyxml.h"
#include <string>
#include <cstring>


/**
 * \struct WorldData
 * \brief Enthaelt die Daten, die zum Laden von World-Templates notwendig sind
 */
struct WorldData
{
	// TODO
};


/**
 * \class TemplateLoader
 * \brief Laedt verschiedene Templates aus einer XML-Datei
 */
class TemplateLoader
{
	public:
	
	/**
	 * \fn void loadWorldData(const char* pFilename)
	 * \brief Sorgt fuer das Anlegen von FixedObjectData, die in einer XML-Datei gespeichert sind
	 * \param pFilename Pfad zu einer XML-Datei, die feste Objekte enthaelt
	 * \return Liste der geladenen festen Objekte
	 */
	bool loadWorldData(const char* pFilename, std::list<WorldData*>* &world_list, std::list<std::string>* &subtype_list);
	
	private:
	
	/**
	 * \fn int generateWorldData(TiXmlElement* pElement, std::string element)
	 * \brief Legt FixedObjectData im Speicher anhand von Daten an, die als Parameter uebergeben wurden. Diese Daten stammen aus einer XML-Datei.
	 * \param pElement Zeiger auf ein Element der XML-Datei
	 * \param element Name des Elements
	 * \return Anzahl der Attribute des bearbeiteten Elements
	 */
	int generateWorldData(TiXmlElement* pElement, std::string element, std::list<WorldData*>* world_list, std::list<std::string>* subtype_list);
	
	/**
	 * \fn void searchWorldData( TiXmlNode* pParent )
	 * \brief Durchmustert eine XML-Datei und sucht nach Elementen.
	 * \param pParent Zeiger auf eine XML-Datei
	 */
	void searchWorldData(TiXmlNode* pParent, std::list<WorldData*>* world_list, std::list<std::string>* subtype_list);
	
	/**
	 * \var WorldData* m_object_data
	 * \brief Daten zu Objekten
	 */
	WorldData* m_world_data;
	/**
	 * \var std::list<FixedObjectData*>* m_object_list
	 * \brief Liste mit Daten zu Objekten
	 */
	//std::list<FixedObjectData*>* m_object_list;
	

};


#endif

