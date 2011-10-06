/*
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

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

