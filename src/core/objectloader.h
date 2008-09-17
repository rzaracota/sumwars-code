#ifndef OBJECTLOADER_H
#define OBJECTLOADER_H


#include "world.h"
#include <map>
#include <list>
#include "monsterbase.h"

#include "../tinyxml/tinyxml.h"
#include <string>
#include <cstring>
using namespace std;


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
	list<MonsterBasicData*>* loadObjects(const char* pFilename);
	
	
	private:
	
	/**
	 * \fn int generateObjects(TiXmlElement* pElement, string element)
	 * \brief Legt Objekte im Speicher anhand von Daten an, die als Parameter uebergeben wurden. Diese Daten stammen aus einer XML-Datei.
	 * \param pElement Zeiger auf ein Element der XML-Datei
	 * \param element Name des Elements
	 * \return Anzahl der Attribute des bearbeiteten Elements
	 */
	int generateObjects(TiXmlElement* pElement, string element);
	
	/**
	 * \fn void searchXml( TiXmlNode* pParent )
	 * \brief Durchmustert eine XML-Datei und sucht nach Elementen.
	 * \param pParent Zeiger auf eine XML-Datei
	 */
	void searchXml(TiXmlNode* pParent);
	
	
	MonsterBasicData* m_monster_data;
	list<MonsterBasicData*>* m_monster_list;
	
	float m_size_prob[4];
	int m_min_level[2];
	int m_max_level[2];
	float m_magic_prob[2];
	float m_magic_power[2];
};


#endif

