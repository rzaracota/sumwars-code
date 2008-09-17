/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Hans Wulf, Michael Kempf, Daniel Erler

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
/**
 * \file constants.h
 * \defgroup Constants \ Konstanten
 * \brief Stellt globale Konstanten zur Verf&uuml;gung
 * \author Hans Wulf, Michael Kempf, Daniel Erler
 * \version 1.0
 * \date 2007/05/28
 * \note Beinhaltet etliche Defines und einige Enums, wie ActionType oder Mode ...
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

//Konstantendefinitionen
	//allgemeine Konstanten ohne Kategorie
	/**
	 * \def EPSILON
	 * \brief Konstante fuer Maschinengenauigkeit
	 */
	#define EPSILON 0.00001
	
	

	
	

	//Konstanten mit Bezug zur Stadt
	/**
	 * \def TOWN_RADIUS
	 * \brief Konstante die angibt, in welchem Abstand um das Stadtzentrum das Gebiet als Stadt gilt
	 */
	#define TOWN_RADIUS 6

	
	/**
	 * \enum Mode
	 * \brief Beschreibt den Modus, in dem sich ein Spieler befindet
	 */
	enum Mode {MODE_AGRESSIVE, MODE_DEFENSIVE};
	
	/**
	 * \enum ItemLocationType
	 * \brief Beschreibt den Standort eines Items im Handelsfenster 
	 */
	enum ItemLocationType {PLAYER_INVENTORY, PLAYER_OFFER, TRADER_INVENTORY, TRADER_OFFER};
	
	/**
	 * \enum NotebookTab
	 * \brief Beschreibt die Registerkarte der GUI
	 */
	enum NotebookTab {TAB_FIELD=0, TAB_CHARACTER, TAB_INVENTORY, TAB_TRADE};

#endif //CONSTANTS_H
