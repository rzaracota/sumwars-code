#ifndef OBJECTLOADER_H
#define OBJECTLOADER_H


#include <map>
#include <list>
#include <vector>
#include "monsterbase.h"
#include "action.h"
#include "elementattrib.h"


#include "../tinyxml/tinyxml.h"
#include <string>
#include <cstring>







/**
 * \class ObjectLoader
 * \brief Laedt Objecte aus einer XML-Datei
 */
class ObjectLoader
{
	public:
	


	
	// neuer Monsterdata loader
	
	/**
	 * \fn loadMonsterData(const char* pFilename);
	 * \brief Liest die Daten zu Monstern aus einer XML Datei
	 * \param pFilename Name der XML Datei
	 */
	static bool loadMonsterData(const char* pFilename);
	
	/**
	 * \fn bool loadMonsterTiXmlNode* node)
	 * \brief Liest die Daten zu einem Monster aus dem Knoten
	 * \param node XML Knoten
	 */
	static bool loadMonster(TiXmlNode* node);
	
	// neuer FixedObject loader
	
	/**
	 * \fn loadFixedObjectData(const char* pFilename);
	 * \brief Liest die Daten zu festen Objekten aus einer XML Datei
	 * \param pFilename Name der XML Datei
	 */
	static bool loadFixedObjectData(const char* pFilename);
	
	/**
	 * \fn bool loadFixedObject(TiXmlNode* node)
	 * \brief Liest die Daten zu einem festen Object aus dem Knoten
	 * \param node XML Knoten
	 */
	static bool loadFixedObject(TiXmlNode* node);
	
	private:
	

};


#endif

