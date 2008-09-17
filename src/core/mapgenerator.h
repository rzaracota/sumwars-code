/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Hans Wulf

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
 * \file mapgenerator.h
 * \defgroup MapGenerator \ Kartenerstellung
 * \ingroup MapGenerator
 * \brief Klasse zum generieren der Spielwelt
 * \author Hans
 * \version 1.0
 * \date 2007/05/28
 * \note Beinhaltet die Klasse MapGenerator
 */
#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H
#include <string>
#include <stdio.h>
#include <iostream>
#include <queue>
#include <math.h>
#include <stdlib.h>
#include <map>
#include <list>

#include "world.h"
#include "fixedobject.h"
#include "constants.h"
#include "quadtree.h"
#include "quadtree.cpp"
#include "spawnpoint.h"
#include "fountain.h"
#include "chest.h"
#include "trader.h"

// Kantenlaenge einer einzelnen Karte
#define MAPSIZE 20

// FIELDPROB[f][b][n] = Wahrscheinlichkeit, dass ein Feld mit bereits b blockierten und f freien Nachbarfeldern insgesamt n+1 freie Nachbarn hat
// Summe einer Vierergruppe muss immer 1 betragen

// hier is die Stelle zum dran rumbasteln ;)
// immer nur genau eine Variante einkommentieren ;)

// Variante fuer verwinkelte langezogene Karte
const float FIELDPROB[4][4][4]={{{0.7,0.2,0.1,0},{0.7,0.2,0.1,0},{0.8,0.2,0,0},{1,0,0,0}}, /* 0 freie */
	{{0.05,0.7,0.2,0.05},{0.05,0.7,0.25,0},{0.05,0.95,0,0},{0,1,0,0}}, /* 1 freie */
	{{0,0.4,0.3,0.3},{0,0.5,0.5,0},{0,0,1,0},{0,0,0,0}},	/* 2 freie */
	{{0,0,0.5,0.5},	{0,0,0,1},{0,0,0,0},{0,0,0,0}}};	/* 3 freie */
	
// Variante fuer viele Sackgassen
//	const float FIELDPROB[4][4][4]={{{0.7,0.2,0.1,0},{0.7,0.2,0.1,0},{0.8,0.2,0,0},{1,0,0,0}}, /* 0 freie*/
//		{{0.1,0.7,0.15,0.05},{0.1,0.7,0.1,0},{0.1,0.9,0,0},{0,1,0,0}}, /* 1 freie */
//		{{0,0.2,0.6,0.2},{0,0.2,0.8,0},{0,0,1,0},{0,0,0,0}},	/* 2 freie */
//		{{0,0,0.5,0.5},	{0,0,0,1},{0,0,0,0},{0,0,0,0}}};	/* 3 freie */
	
// Variante fuer große Hoehlen	
//const float FIELDPROB[4][4][4]={{{0.7,0.2,0.1,0},{0.7,0.2,0.1,0},{0.8,0.2,0,0},{1,0,0,0}}, /* 0 freie*/
//		{{0.05,0.65,0.1,0.2},{0.05,0.6,0.35,0},{0.05,0.95,0,0},{0,1,0,0}}, /* 1 freie */
//		{{0,0.3,0.1,0.6},{0,0.2,0.8,0},{0,0,1,0},{0,0,0,0}},	/* 2 freie */
//		{{0,0,0.2,0.8},	{0,0,0,1},{0,0,0,0},{0,0,0,0}}};	/* 3 freie */
	


// Anmerkungen:
// 0 freie Felder: nur möglich bei einer Leiter
// 1 freie Felder: Standardfall (das Feld von dem man kommt ist das eine freie)
// 2 freie Felder: Treffpunkt von zwei Gängen
	
	// Array mit den Aenderungen der Koordinaten in Richtung Nord, Ost, Sued, West
	const int direction[4][2]={{0,-1},{1,0},{0,1},{-1,0}};
	
	// Anzahl Felder, in die ein einzelnes Feld der schematischen Karte übersetzt wird.
	const int FIELD_STRETCH = 8;
	
	const int SUM_TYPE[81] = {0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,3,0,
					0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,-1,1, 0,1,2, 0,0,0, 0,1,2, 3,4,3,
	  				0,0,0, 0,0,3, 0,0,0, 0,0,3, 0,1,4, 0,2,3, 0,0,0, 0,2,3, 0,3,0};
	  
	const int SUM_TURN[81]= {0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
		  			0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,1,0, 0,0,0, 0,3,3, 2,0,2,
		 			0,0,0, 0,0,1, 0,0,0, 0,0,3, 0,2,1, 0,1,3, 0,0,0, 0,2,1, 0,0,0};
					
	const int TYPE_DIR[5][4] = {{0,0,0,0},{1,0,0,1},{0,1,0,1},{1,0,0,1},{1,1,1,1}};
		 			
	const int IND_OBJECTS[12] = {0,SUBTYPE_PALISADE_FRONT,SUBTYPE_PALISADE_DOWN ,SUBTYPE_PALISADE_FRONT,SUBTYPE_PALISADE_DOWN ,SUBTYPE_PALISADE_CORNER_NW,SUBTYPE_PALISADE_CORNER_NE,SUBTYPE_PALISADE_CORNER_SE,SUBTYPE_PALISADE_CORNER_SW,SUBTYPE_TREE,SUBTYPE_TOWN_CENTER,SUBTYPE_TOTEM};
	
	const float TREE_PROB[2] = {0.1,0.03};
	const float CHEST_PROB[5]={0.01,0.05,0.1,0.3,0.3};
	const float FOUNTAIN_PROB[5]={0.1,0.01,0.01,0.001,0.001};
	const float TOTEM_PROB=0.3;
	const float SPAWNPOINT_PROB[5]={0.6,0.5,0.3,0.2,0.2};
	
	enum OBJECTS {TREE = 9, TOWNCENTER = 10, FOUNTAIN = 100,TOTEM =11 , SPAWNPOINT = 101, CHEST  = 102, TRADER = 103};
	
struct MapArea
{
	MapArea(int x, int y, int id)
	{
		this->x=x;
		this->y=y;
		content=-1;
		type=0;
		turn=0;
		this->id=id;
		town= towncenter=trader=false;
	}
	bool inside,town, towncenter,trader;
	
	int content;
	int margin_dist[4];
	int turn;
	int type;
	int sum;
	int dist;
	int x,y;
	int id;
	
	float getCoordinateX()
	{
		return x;
	}
	float getCoordinateY()
	{
		return y;
	}
	
	
	
	int getId()
	{
		return id;
	}
	
};


// Struktur fuer die Informationen eines Feldes
struct Coords
{
	int x,y; 			// Koordinaten
	int dist;
	int prev_x, prev_y;	// Koordinaten des vorrangegangenen Feldes
};

	
	
// Array zum speichern einer Karte
// -1 = noch nicht belegt, 0 = frei, 1=blockiert, 2=Leiter nach oben, 3= Leiter nach unten

/*
struct Gamemap
{
	MapArea gmap[MAPSIZE][MAPSIZE];
	int x,y;
};

// Array zum speichern einen Dungeons
// 9x9x9 Array von einzelnen Karten
class Dungeonmap
{
	private:
		map<int,Gamemap*>* gmaps;
		int num_maps;
		//map<int,Gamemap*>::iterator prev_map;
		
	public:
	Dungeonmap();
	~Dungeonmap();
	
	Gamemap* getGamemap(int x, int y);
	
	map<int,Gamemap*>* getGamemaps();

	
	MapArea* getMapArea(int x, int y,bool create=true);
	
};

class DynMatrix
{
	
	private:
		int dim;
		int* data;
		
	public:
	DynMatrix(int dim);

	
	~DynMatrix();
	
	
	int* ind(int x, int y);
	
};
*/
class MapGenerator{
	
	
	public:
		 static void createMap(World* world);
		 MapGenerator(){};
	
		
	private:
		static void drawMap(list<MapArea*>* tmp);
		static void showDungeon(QuadTree<MapArea>* dungeonmap);
		static MapArea* getField(QuadTree<MapArea>* dungeonmap, int x, int y, bool expand=true);
		static void calcDungeon(QuadTree<MapArea>* dungeonmap,int max );
		static void turn(int &x, int &y,int t);
		static bool check(int x,int y,World* world);
		static void stretchDungeon(QuadTree<MapArea>* dungeonmap, World* world);
		static void insertElement(World* world,int i, int cx, int cy,int x, int y,int (*obj)[FIELD_STRETCH]);
};

#endif //CHEST_H
