#ifndef FIXEDBASE_H
#define FIXEDBASE_H

#include "geometry.h"

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
};

#endif

