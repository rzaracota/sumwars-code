#ifndef EVENTLOADER_H
#define EVENTLOADER_H


#include <map>
#include <list>

#include "../tinyxml/tinyxml.h"
#include <string>
#include <cstring>


struct TempEvent
{
	std::string trigger;
	std::string once;
	std::string region;
	const char* effect;
	const char* condition;
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
	bool loadEvent(const char* pFilename, std::list<TempEvent*> &temp_event_list);
	
	
	private:
	
	/**
	 * \fn int generateWorldData(TiXmlElement* pElement, std::string element)
	 * \brief Legt FixedObjectData im Speicher anhand von Daten an, die als Parameter uebergeben wurden. Diese Daten stammen aus einer XML-Datei.
	 * \param pElement Zeiger auf ein Element der XML-Datei
	 * \param element Name des Elements
	 * \return Anzahl der Attribute des bearbeiteten Elements
	 */
	int generateEvent(TiXmlElement* pElement, std::string element, std::list<TempEvent*> &temp_event_list);
	
	/**
	 * \fn void searchWorldData( TiXmlNode* pParent )
	 * \brief Durchmustert eine XML-Datei und sucht nach Elementen.
	 * \param pParent Zeiger auf eine XML-Datei
	 */
	void searchEvent(TiXmlNode* pParent, std::list<TempEvent*> &temp_event_list);
	
	
	/**
	 * \var WorldData* m_object_data
	 * \brief Daten zu Objekten
	 */
	TempEvent* m_temp_event;
};


#endif

