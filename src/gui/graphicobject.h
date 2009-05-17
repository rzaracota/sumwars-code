#ifndef GRAPHIC_OBJECT_H
#define GRAPHIC_OBJECT_H

#include <string>
#include "gameobject.h"


/**
 * \class GraphicObject
 * \brief Klasse fuer alle gerenderten Objekte
 */
class GraphicObject
{
	public:
		typedef std::string Type;
	
	private:
		
		/**
		 * \var Type m_type
		 * \brief Typ des Objekts
		 */
		Type m_type;
		
		/**
		 * \var GameObject* m_object
		 * \brief zugrundeliegendes Objekt im Spiel
		 */
		GameObject* m_object;
		
		/**
		 * \var std::map<std::string, Ogre::MovableObject*> m_attached_objects
		 * \brief Ogre Objekte, die zu dem GraphicsObjekt gehoeren
		 */
		std::map<std::string, Ogre::MovableObject*> m_attached_objects;
		
		/**
		 * \var std::map<std::string, GraphicObject*> m_subobjects
		 * \brief UnterObjekte des aktuellen GraphicsObjekts
		 */
		std::map<std::string, GraphicObject*> m_subobjects;
};


#endif