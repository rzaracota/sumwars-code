#ifndef OBJECTLOADER_H
#define OBJECTLOADER_H


#include <map>
#include <list>
#include <vector>
#include "monsterbase.h"
#include "playerbase.h"
#include "action.h"
#include "elementattrib.h"
#include "projectilebase.h"
#include "treasurebase.h"

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
	
	/**
	 * \fn static bool loadCreatureBaseAttr(TiXmlNode* node, CreatureBaseAttr& attr)
	 * \brief Laedt die Daten der Struktur aus dem Knoten
	 * \param node XML Knoten
	 * \param attr eingelesene Struktur
	 **/
	static bool loadCreatureBaseAttr(TiXmlNode* node, CreatureBaseAttr& attr);
	
	/**
	 * \fn static bool loadShape(TiXmlNode* node, Shape& shape)
	 * \brief laedt Daten zu einer Flaeche aus einem Knoten
	 * \param node XML Knoten
	 * \param shape eingelesene Struktur
	 **/
	static bool loadShape(TiXmlNode* node, Shape& shape);
	
	/**
	 * \fn loadObjectData(const char* pFilename);
	 * \brief Liest die Daten zu Objekten aus einer XML Datei
	 * \param pFilename Name der XML Datei
	 */
	static bool loadObjectData(const char* pFilename);
	
	/**
	 * \fn bool loadFixedObject(TiXmlNode* node)
	 * \brief Liest die Daten zu einem Object aus dem Knoten
	 * \param node XML Knoten
	 */
	static bool loadObject(TiXmlNode* node);
	
	/**
	 * \fn loadProjectileData(const char* pFilename);
	 * \brief Liest die Daten zu Projectilen aus einer XML Datei
	 * \param pFilename Name der XML Datei
	 */
	static bool loadProjectileData(const char* pFilename);
	
	/**
	 * \fn bool loadProjectile(TiXmlNode* node)
	 * \brief Liest die Daten zu einem Projectil aus dem Knoten
	 * \param node XML Knoten
	 */
	static bool loadProjectile(TiXmlNode* node);
	
	/**
	 * \fn loadPlayerData(const char* pFilename);
	 * \brief Liest die Daten zu einer Spielerklasse aus einer XML Datei
	 * \param pFilename Name der XML Datei
	 */
	static bool loadPlayerData(const char* pFilename);
	
	/**
	 * \fn static bool loadEmotionSet(TiXmlNode* node)
	 * \brief Liest die Daten zu einem Emotionset aus einem Knoten
	 * \param node XML Knoten
	 */
	static bool loadEmotionSet(TiXmlNode* node);
	
	/**
	 * \fn bool loadPlayerTiXmlNode* node)
	 * \brief Liest die Daten zu einem Spielerklasse aus dem Knoten
	 * \param node XML Knoten
	 */
	static bool loadPlayer(TiXmlNode* node);
	private:
		/**
		 * \brief Name der Datei die gerade gelesen wird
		 */
		static std::string m_filename;
	

};


#endif

