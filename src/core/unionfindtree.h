#ifndef UNIONFINDTREE_H
#define UNIONFINDTREE_H

#include <list>
#include <vector>
#include <iostream>
#include <iomanip>
/**
 * \class UnionFindTree
 * \brief Implementation eines Unionfind Baumes
 */
class UnionFindTree
{
	public:
		/**
		 * \fn UnionFindTree(int dim)
		 * \brief Konstruktor, legt einen neuen Baum an, bestehend aus nur einelementigen Mengen
		 * \param dim Anzahl der Elemente
		 */
		UnionFindTree(int dim);
		
		/**
		 * \fn ~UnionFindTree()
		 * \brief Destruktor
		 */
		~UnionFindTree();
		
		/**
		 * \fn int find(int index)
		 * \brief Gibt die Wurzel des Teilbaumes, das Element index enthaelt aus
		 * \param index Element
		 */
		int find(int index);
		
		/**
		 * \fn void merge(int index1, index2)
		 * \brief Vereinigt die beiden Mengen die die beiden Elemente enthalten
		 * \param index1 Element der ersten Menge
		 * \param index2 Element der zweiten Menge
		 */
		void merge(int index1, int index2);
		
		/**
		 * \fn int getCardinality(int index)
		 * \brief Gibt die Groesse der Menge, die das Element index enthaelt aus
		 * \param index Element
		 */
		int getCardinality(int index);
		
		/**
		 * \fn void getElements(int index, std::list<int>& result)
		 * \brief Gibt die Elemente der Menge, die das Element index enthaelt aus
		 * \param index Element
		 * \param result Liste in die die Elemente eingefuegt werden
		 */
		void getElements(int index, std::list<int>& result);
		
		/*
		void print()
		{
			for (int i=0; i<m_dim; i++)
				std::cout << std::setw(3) << m_data[i];
			std::cout <<"\n";
			for (int i=0; i<m_dim; i++)
				std::cout << std::setw(3) << m_cardinality[i];
			std::cout <<"\n";
		}
		*/
		
	private:
		/**
		 * \var m_dim
		 * \brief Anzahl der Elemente
		 */
		int m_dim;
		
		/**
		 * \var int *m_data
		 * \brief Baum als Array
		 */
		int* m_data;
		
		/**
		 * \var int m_cardinality
		 * \brief Kardinalitaet der Teilmengen
		 */
		int* m_cardinality;
};
	









#endif

