#ifndef FIXEDBASE_H
#define FIXEDBASE_H

#include "geometry.h"
#include "../tinyxml/tinyxml.h"

/**
 * \struct FixedObjectData
 * \brief Struktur mit den Daten von fixenObjekten
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
	
	/**
	 * \brief Writes the DataStructure to the XML node
	 * \param node XML node
	 * This function also deals with XML trees that are already written. In this case, only the difference is written.
	 */
	void writeToXML(TiXmlNode* node);
	
	/**
	 * \brief assignment operator
	 * \param other object to be copied from
	 */
	void operator=(const FixedObjectData& other);
};

#endif

