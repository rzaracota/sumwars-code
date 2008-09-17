#ifndef MATRIX2D_H
#define MATRIX2D_H

#include "debug.h"
#include <stdio.h>
#include <string.h>

/**
 * \class Matrix2d
 * \brief Klasse fuer ein zweidimensionales Feld
 */
template <class Element>
class Matrix2d
{

	private:
		/**
 		 * \var int m_dimx
 		 * \brief TODO
 		 */
		int m_dimx;
		
        /**
         * \var int m_dimy
         * \brief TODO
         */
		int m_dimy;
		
		/**
 		 * \var Element* m_data
 		 * \brief TODO
 		 */
		Element* m_data;
		
	public:
	/**
 	 * fn Matrix2D(int dimx, int dimy)
 	 * \brief Konstruktor
 	 * \param dimx Breite des Feldes
 	 * \param dimy Laenge des Feldes
	 */
	Matrix2d(int dimx, int dimy)
	{
		m_dimx = dimx,m_dimy = dimy;
		m_data = new Element[dimx*dimy];
		DEBUG5("creating Matrix2d at %p",m_data);
	}
	
	
	/**
 	 * \fn ~Matrix2d()
 	 * \brief Destruktor
 	 */
	~Matrix2d()
	{
		delete[] m_data;
	}
	
	
	/**
	 * \fn Element* ind(int x, int y)
	 * \brief TODO
	 * \param x TODO
	 * \param y TODO
	 */
	Element* ind(int x, int y)
	{
		return &(m_data[x*m_dimy +y]);
	}
	
	/**
	 * \fn void clear()
	 * \brief Schreibt 0 in alle Elemente des Feldes
	 */
	void clear()
	{
		memset(m_data,0,sizeof(Element)*m_dimx*m_dimy);
	}
	
			
};
	

#endif
