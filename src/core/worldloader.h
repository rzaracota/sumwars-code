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




/**
 * \class TemplateLoader
 * \brief Laedt verschiedene Templates aus einer XML-Datei
 */
class WorldLoader
{
	public:
	
	
	
	/**
	 * \fn static bool loadNPCData(const char* pFilename)
	 * \brief Liest die Daten zu NPCs aus einer XML Datei
	 * \param pFilename Name der XML Datei
	 */
	static bool loadNPCData(const char* pFilename);
	
	/**
	 * \fn static loadQuestsData(const char* pFilename)
	 * \brief Liest die Daten zu Quests aus einer XML Datei
	 * \param pFilename Name der XML Datei
	 */
	static bool loadQuestsData(const char* pFilename);
	
	/**
	 * \fn static bool loadRegionData(const char* pFilename)
	 * \brief Laedt die Daten zu den Regionen aus der XML
	 * \param pFilename Name der XML Datei
	 */
	static bool loadRegionData(const char* pFilename);
	
	/**
	 * \fn static void loadEvent( TiXmlNode* node, Event *ev, TriggerType &type)
	 * \brief Laedt die Daten zu einem Event aus einem XML Knoten
	 * \param node Knoten der die Daten enthaelt
	 * \param ev Event in das die Daten geschrieben werden
	 * \param type Typ des Triggers, der das Event ausloesen soll (Ausgabeparameter)
	 */
	static void loadEvent( TiXmlNode* node, Event *ev, TriggerType &type);
	
	private:
	
	/**
	 * \fn static bool loadRegions(TiXmlNode* node)
	 * \brief Lade die Daten zu Regionen aus einer XML Datei
	 * \param node XMl Knoten
	 */
		static bool loadRegions(TiXmlNode* node);
		
	/**
	* \fn static bool loadRegion(TiXmlNode* node, RegionData* rdata)
	 * \brief Laedt die Daten zu einer Region aus einem XML Knoten
	 * \param node Knoten der die Daten enthaelt
	 * \param rdata Datenstruktur in die die Daten geschrieben werden
	 */
	static bool loadRegion(TiXmlNode* node, RegionData* rdata);
	
	/**
	 * \fn static void loadFraction( TiXmlNode* node)
	 * \brief Laedt Daten zu einer Fraktion aus der XML Datei
	 */
	static void loadFraction( TiXmlNode* node);
	
	/**
	 * \fn static void loadNPC( TiXmlNode* node)
	 * \brief Laedt die Daten zu einem NPC aus einem Knoten
	 * \param node XML Knoten
	 */
	static void loadNPC( TiXmlNode* node);
	
	/**
	 * \fn static void loadQuests(TiXmlNode* node)
	 * \brief Liest die Daten zu Quests aus einer XML Datei
	 * \param node XML Knoten
	 */
	static void loadQuests(TiXmlNode* node);
	
	/**
	 * \fn static void loadQuest(TiXmlNode* node, Quest* quest)
	 * \brief Laedt die Daten zu einem Quest
	 * \param node Knoten, der die Daten enthaelt
	 * \param quest Klasse, in die die Daten geschrieben werden
	 */
	static void loadQuest(TiXmlNode* node, Quest* quest);
	
	private:
		/**
		 * \brief Name der Datei die gerade gelesen wird
		 */
		static std::string m_filename;
	

};


#endif

