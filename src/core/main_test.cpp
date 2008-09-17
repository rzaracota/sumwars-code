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

#include "world.h"
#include "serverwobject.h"
#include "creature.h"
#include "fixedobject.h"
#include "warrior.h"
#include "goblin.h"

#include <iostream>

#define hex(a) ((a)>=10) ? (a)-10+'a' : (a)+'0'

using namespace std;

const short nlayers[9] = {WorldObject::Geometry::LAYER_DEAD ,
		WorldObject::Geometry::LAYER_FIXED & WorldObject::Geometry::LAYER_BASE,
		WorldObject::Geometry::LAYER_FIXED & WorldObject::Geometry::LAYER_BASE & WorldObject::Geometry::LAYER_AIR,
		WorldObject::Geometry::LAYER_CREATURE & WorldObject::Geometry::LAYER_AIR ,
		WorldObject::Geometry::LAYER_CREATURE & WorldObject::Geometry::LAYER_BASE,
		WorldObject::Geometry::LAYER_CREATURE & WorldObject::Geometry::LAYER_BASE & WorldObject::Geometry::LAYER_AIR,
		WorldObject::Geometry::LAYER_PLAYER & WorldObject::Geometry::LAYER_BASE & WorldObject::Geometry::LAYER_AIR,
		WorldObject::Geometry::LAYER_MONSTER & WorldObject::Geometry::LAYER_AIR,
		WorldObject::Geometry::LAYER_MONSTER & WorldObject::Geometry::LAYER_AIR & WorldObject::Geometry::LAYER_BASE};
		
const short slayers[10] = {
	WorldObject::Geometry::LAYER_DEAD,
	WorldObject::Geometry::LAYER_BASE,
 	WorldObject::Geometry::LAYER_AIR,
  	WorldObject::Geometry::LAYER_AIR | WorldObject::Geometry::LAYER_BASE,
	WorldObject::Geometry::LAYER_FIXED , 
 	WorldObject::Geometry::LAYER_CREATURE, 
  	WorldObject::Geometry::LAYER_FIXED & WorldObject::Geometry::LAYER_AIR, 
	WorldObject::Geometry::LAYER_MONSTER & WorldObject::Geometry::LAYER_AIR, 
 	WorldObject::Geometry::LAYER_MONSTER & WorldObject::Geometry::LAYER_BASE,
 	(WorldObject::Geometry::LAYER_MONSTER | WorldObject::Geometry::LAYER_PLAYER) & (WorldObject::Geometry::LAYER_BASE | WorldObject::Geometry::LAYER_AIR)
};
	

int main(int argc, char *argv[])
{
	char c,c2;
	
	World* g_world=new World();
	float x,y, radius,x1,y1, x2, y2,x_path,y_path, pathlength;
	Action::ActionType act = Action::ATTACK;
	int i,j,key;
	unsigned int id;
	ServerWObject* wo=0,*wo2=0;
	Creature* cr=0;
	Player* pl =0;
	CreatureBaseAttr* basm =0;
	
	WorldObject::Geometry* wob;
	Shape sho,shs,shall;
	sho.m_type = Shape::CIRCLE;
	sho.m_radius =1;
	shall.m_type = Shape::RECT;
	shall.m_coordinate_x = 16;
	shall.m_coordinate_y = 16;
	shall.m_extent_x = 16;
	shall.m_extent_y = 16;
	
	shs.m_coordinate_x = 5.5;
	shs.m_coordinate_y = 5.3;
	shs.m_type = Shape::RECT;
	shs.m_extent_x =1.1;
	shs.m_extent_y = 1.9;
	
	short layer = WorldObject::Geometry::LAYER_ALL;
	Shape* sp;
	Gridunit* gu;
	
	Region* reg = new Region(8,8);
	short rid = g_world->insertRegion(reg);
	
	// Elemente die von Anfang an eingefügt werden
	
	////////////
	// der Spieler
	
	//wo2 = wo = cr= new Warrior(g_world,1);
	//g_world->insertSWObject(wo, 11,5,rid);
	
	
	wo = new FixedObject(g_world,32);
	wob = wo->getGeometry();
	sp = &(wob->m_shape);
	wob->m_layer = (WorldObject::Geometry::LAYER_FIXED & WorldObject::Geometry::LAYER_BASE & WorldObject::Geometry::LAYER_AIR);
	sp->m_type = Shape::RECT;
	sp->m_extent_x = 1.0;
	sp->m_extent_y = 2;
	g_world->insertSWObject(wo, 4,6,rid);
	
	 	
	wo = new FixedObject(g_world,100);
	wob = wo->getGeometry();
	sp = &(wob->m_shape);
	wob->m_layer = (WorldObject::Geometry::LAYER_FIXED & WorldObject::Geometry::LAYER_BASE & WorldObject::Geometry::LAYER_AIR);
	sp->m_type = Shape::CIRCLE;
	sp->m_extent_x = 1.51;
	sp->m_extent_y = 2.51;
	sp->m_radius = 2.8;
	g_world->insertSWObject(wo, 9,13,rid);
		
	wo = new Goblin(g_world,20);
	g_world->insertSWObject(wo, 13,8,rid);
	
	wo = cr=new Goblin(g_world,21);
	basm = cr->getBaseAttrMod();
	basm->m_max_health = 300;
	cr->getDynAttr()->m_health = 300;
	g_world->insertSWObject(wo, 14.2,8.2,rid);
		
	wo = cr=new Goblin(g_world,22);
	basm = cr->getBaseAttrMod();
	basm->m_strength = 200;
	g_world->insertSWObject(wo, 14.5,6.7,rid);

	wo = cr=new Goblin(g_world,23);
	basm = cr->getBaseAttrMod();
	basm->m_dexterity = 200;
	g_world->insertSWObject(wo, 14.8,5.7,rid);

	wo =cr= new Goblin(g_world,24);
	basm = cr->getBaseAttrMod();
	basm->m_magic_power = 200;
	g_world->insertSWObject(wo, 14,10.9,rid);

	wo = cr=new Goblin(g_world,25);
	basm = cr->getBaseAttrMod();
	basm->m_willpower = 200;
	g_world->insertSWObject(wo, 17,8.2,rid);
		
	wo = cr=new Goblin(g_world,26);
	basm = cr->getBaseAttrMod();
		// blocken
		//basm->m_abilities[1]=0x0100;
		// Turm in der Schlacht
		//basm->m_abilities[1]=0x0200;
	basm->m_block = 44;
	basm->m_armor = 50;
	g_world->insertSWObject(wo, 7,5,rid);
		
	wo = cr=new Goblin(g_world,27);
	basm = cr->getBaseAttrMod();
	basm->m_block = 200;
	g_world->insertSWObject(wo, 18.5,7,rid);

	wo = cr=new Goblin(g_world,28);
	basm = cr->getBaseAttrMod();
	basm->m_armor = 200;
	g_world->insertSWObject(wo, 17.2,6.6,rid);

	wo = cr=new Goblin(g_world,29);
	basm = cr->getBaseAttrMod();
	basm->m_resistances[Damage::PHYSICAL] = 50;
	g_world->insertSWObject(wo, 19.9,6.9,rid);
		

	wo = cr=new Goblin(g_world,30);
	basm = cr->getBaseAttrMod();
	basm->m_resistances[Damage::FIRE] = 50;
	g_world->insertSWObject(wo, 16.2,3,rid);
	
	wo = cr=new Goblin(g_world,31);
	basm = cr->getBaseAttrMod();
	basm->m_resistances[Damage::AIR] = 50;
	g_world->insertSWObject(wo, 19.2,3.1,rid);
		
	wo = cr=new Goblin(g_world,32);
	basm = cr->getBaseAttrMod();
	basm->m_resistances[Damage::ICE] = 50;
	g_world->insertSWObject(wo, 21,5,rid);
	
	
	
	// Zeiger auf Liste von WorldObject - Anfragen welche Objekte in einem Gebiet sind werden in dieser Form ausgegeben
	list<ServerWObject*>* res= new list<ServerWObject*>;
	
	// Iterator zum durchmustern einer solchen Liste
	list<ServerWObject*>::iterator iter;
	

	bool result;
	//wo =wo2;	
	
	/*
	cr = (Creature*) wo;
	Command* com = cr->getCommand();
	com->m_type = Action::WALK;
	com->m_goal_object_id=0;
	com->m_goal_coordinate_x = 8;
	com->m_goal_coordinate_y = 4;
	*/
	
	
	c='0';
	while (c !='x' )
	{
		if (wo!=0)
		{
			wob = wo->getGeometry();
			printf("\n aktuelles WorldObject:\n");
			printf("WO: x: %f y: %f ID: %i layer: %x\n", wob->m_shape.m_coordinate_x,wob->m_shape.m_coordinate_y, wo->getId(),wob->m_layer);
			
		};
		printf("\n1: WorldObject einfügen \n2: Grid anzeigen \n3: update \n4: alle Elemente anzeigen \n5: Element nach key suchen \n6: ElementInShape \n7: WorldObject bewegen\n8: ObjectShape aendern \n9: SuchShape aendern\n0: aktuelles Element löschen \na: Aktion auswaehlen \nc: Kommando erteilen\ni: intersect \nl: ElementOnLine \n \nx: beenden \n");
		
		cin >> c;
		if (c=='1')
		{
			cout << "x\n";
			cin >> x;
			cout << "y\n";
			cin >> y;
			cout << "ID \n";
			cin >> id;
			wo = new ServerWObject(g_world,id);
			wob = wo->getGeometry();
			sp = &(wob->m_shape);
			sp->m_type = sho.m_type;
			sp->m_radius = sho.m_radius;
			sp->m_extent_x = sho.m_extent_x;
			sp->m_extent_y = sho.m_extent_y;
			printf("0: WorldObject::Geometry::LAYER_DEAD 				\
					\n1: WorldObject::Geometry::LAYER_FIXED & WorldObject::Geometry::LAYER_BASE	 \
					\n2: WorldObject::Geometry::LAYER_FIXED & WorldObject::Geometry::LAYER_BASE & WorldObject::Geometry::LAYER_AIR \
					\n3. WorldObject::Geometry::LAYER_CREATURE & WorldObject::Geometry::LAYER_AIR \
					\n4. WorldObject::Geometry::LAYER_CREATURE & WorldObject::Geometry::LAYER_BASE \
					\n5. WorldObject::Geometry::LAYER_CREATURE & WorldObject::Geometry::LAYER_BASE & WorldObject::Geometry::LAYER_AIR\
					\n6. WorldObject::Geometry::LAYER_PLAYER & WorldObject::Geometry::LAYER_BASE & WorldObject::Geometry::LAYER_AIR \
					\n7. WorldObject::Geometry::LAYER_MONSTER & WorldObject::Geometry::LAYER_AIR \
					\n8. WorldObject::Geometry::LAYER_MONSTER &WorldObject::Geometry::LAYER_AIR &  WorldObject::Geometry::LAYER_BASE\n");
			int l;
			cin >> l;
			wob->m_layer = nlayers[l];
			g_world->insertSWObject(wo, x,y,rid);
			
			
		}
		else if (c=='2')
		{
			cout << "i\n";
			cin >> i;
			cout << "j\n";
			cin >> j;
			
			gu = reg->m_data_grid->ind(i,j);
			cout << "Objekte in WorldObject::Geometry::LAYER_DEAD: \n";
			for (i=0;i<gu->getObjectNr(WorldObject::Geometry::LAYER_DEAD);i++)
			{
				wo = gu->getObjects(WorldObject::Geometry::LAYER_DEAD)[i];
				wob = wo->getGeometry();
				printf("WO: x: %f y: %f ID: %i layer: %x\n", wob->m_shape.m_coordinate_x,wob->m_shape.m_coordinate_y, wo->getId(),wob->m_layer);
			}
			printf("\n");
			cout << "Objekte in WorldObject::Geometry::LAYER_FIXED:\n";
			for (i=0;i<gu->getObjectNr(WorldObject::Geometry::LAYER_FIXED);i++)
			{
				wo = gu->getObjects(WorldObject::Geometry::LAYER_FIXED)[i];
				wob = wo->getGeometry();
				printf("WO: x: %f y: %f ID: %i layer: %x\n", wob->m_shape.m_coordinate_x,wob->m_shape.m_coordinate_y, wo->getId(),wob->m_layer);
			}
			printf("\n");
			cout << "Objekte in WorldObject::Geometry::LAYER_CREATURE:\n";
			for (i=0;i<gu->getObjectNr(WorldObject::Geometry::LAYER_CREATURE);i++)
			{
				wo = gu->getObjects(WorldObject::Geometry::LAYER_CREATURE)[i];
				wob = wo->getGeometry();
				printf("WO: x: %f y: %f ID: %i layer: %x\n", wob->m_shape.m_coordinate_x,wob->m_shape.m_coordinate_y, wo->getId(),wob->m_layer);
			}
			printf("\n");
			
			
		}
		else if (c=='3')
		{
			// update aufrufen
			
			float time;
			int i,n;
			 cout << "Zeitdifferenz\n";
			 cin >> time;
			 n=1;
			 //cout << "Anzahl der Updates\n";
			 //cin >> n;
			 
			 for(i=0;i<n;i++)
			 {
			 	g_world->update(time);
			 }
		}
		else if (c=='4')
		{
			// Alle Objekte anzeigen
			
			// Suche nach allen Objecten wird einfach über Suche in einem Kreis mit riesigem Umkreis gemacht - brauchen wir so eine Funktion (ausserhalb der g_world, die ihre Objekte über den Binärbaum durchmustern kann)
			
			WorldObject* wo2;
			// Löschen der Ergebnisliste
			res->clear();
			
			// Aufruf mit x=0, y=0, Radius= riesig, selector= NULL (keine Selektierung)
			result= g_world->getSWObjectsInShape(&shall,WorldObject::Geometry::LAYER_ALL,rid,res);
			
			if (result== false)
			{
				cout << "\n Es ist ein Fehler aufgetreten";
			}
			else
			{
				// Durchmustern der STL Liste
				for (iter =res->begin(); iter!=res->end(); iter ++)
				{
					// WorldObjectpointer aus dem Iterator holen
					// die leicht sonderbare Schreibweise entsteht durch den überladenen * Operator für den Iterator
					wo2 = *iter;
					wob = wo2->getGeometry();
				
					printf("Objekt: %f %f ID: %i layer: %x\n",wob->m_shape.m_coordinate_x,wob->m_shape.m_coordinate_y,wo2->getId()
					, wob->m_layer);	
					
					if (wo2->getTypeInfo()->m_type!=WorldObject::TypeInfo::TYPE_FIXED_OBJECT)
					{
						Creature* cr2 = (Creature*) wo2;
						CreatureBaseAttr* cba = cr2->getBaseAttrMod();
						CreatureDynAttr* cda = cr2->getDynAttr();
						/*printf("     HP: %i / %i\n", pl2->getHealth(), pl2->getMaxHealth());
						printf("     LVL: %i\n", pl2->getLevel());
						printf("     EXP: %i / %i\n", pl2->getExperience(), pl2->getMaxExperience());
						printf("     ATT: %i DEF: %i ARMOR %i \n",pl2->getAttack(),pl2->getDefense(), pl2->getArmor());
						*/
					}
					
				}
			}
			
		}	
		else if (c=='5')
		{
			// Suchen eines WorldObjects anhand seines Keys
			cout << "id: ";
			cin >> key;
			wo=g_world->getSWObject(key)	;
			
			// nach solchen Abfragen immer auf NULL prüfen
			if (wo == 0)
				cout << "\nObjekt nicht gefunden\n";
		}	
		else if (c=='6')
		{
			// Element in Shape
			ServerWObject* wo2;
			// Löschen der Ergebnisliste
			res->clear();
			
			printf(" \
			\n0: WorldObject::Geometry::LAYER_DEAD, \
			\n1: WorldObject::Geometry::LAYER_BASE, \
			\n2: WorldObject::Geometry::LAYER_AIR, \
 			\n3:WorldObject::Geometry::LAYER_AIR | WorldObject::Geometry::LAYER_BASE, \
 			\n4: WorldObject::Geometry::LAYER_FIXED ,  \
 			\n5: WorldObject::Geometry::LAYER_CREATURE, \
 			\n6: WorldObject::Geometry::LAYER_FIXED & WorldObject::Geometry::LAYER_AIR, \
 			\n7: WorldObject::Geometry::LAYER_MONSTER & WorldObject::Geometry::LAYER_AIR, \
 			\n8: WorldObject::Geometry::LAYER_MONSTER & WorldObject::Geometry::LAYER_BASE \
 			\n9: (WorldObject::Geometry::LAYER_MONSTER | WorldObject::Geometry::LAYER_PLAYER) & (WorldObject::Geometry::LAYER_BASE | WorldObject::Geometry::LAYER_AIR) \n");
			
			int l;
			cin >> l;
			
			// Aufruf mit x=0, y=0, Radius= riesig, selector= NULL (keine Selektierung)
			result= g_world->getSWObjectsInShape(&shs,slayers[l],rid,res);
			
			if (result== false)
			{
				cout << "\n Es ist ein Fehler aufgetreten";
			}
			else
			{
				// Durchmustern der STL Liste
				for (iter =res->begin(); iter!=res->end(); iter ++)
				{
					// WorldObjectpointer aus dem Iterator holen
					// die leicht sonderbare Schreibweise entsteht durch den überladenen * Operator für den Iterator
					wo2 = *iter;
					wob = wo2->getGeometry();
				
					printf("Objekt: %f %f key: %i layer: %x\n",wob->m_shape.m_coordinate_x,wob->m_shape.m_coordinate_y,wo->getId(),wob->m_layer);	
					/*
					if (wo2->getObjectType().getObjectType()!=OBJECTTYPE_FIXED_OBJECT)
					{
					Player* pl2 = (Player*) wo2;
					printf("     HP: %i / %i\n", pl2->getHealth(), pl2->getMaxHealth());
					printf("     LVL: %i\n", pl2->getLevel());
					printf("     EXP: %i / %i\n", pl2->getExperience(), pl2->getMaxExperience());
					printf("     ATT: %i DEF: %i ARMOR %i \n",pl2->getAttack(),pl2->getDefense(), pl2->getArmor());
				}
					*/
				}
			}
		}
		else if (c=='7')
		{
			// WorldObject modifizieren
			if (wo !=0)
			{
				
				// Koordinaten neu setzen, die alten vorher sichern
				x1=wob->m_shape.m_coordinate_x;
				y1=wob->m_shape.m_coordinate_y;
				printf("\nx (aktuell %f): ", wob->m_shape.m_coordinate_x);
				cin >> x;
				printf("y (aktuell %f): ",wob->m_shape.m_coordinate_y) ;
				cin >> y;
				
				wo->moveTo(x,y);
				
			}
			else
			{
				cout << "\n kein WorldObject ausgewählt \n";
			}
		}
		else if (c=='8')
		{
			printf("1: Kreis 2: Rechteck\n");
			cin >> c2;
			if (c2=='1')
			{
				sho.m_type = Shape::CIRCLE;
				printf("radius:\n");
				cin >> sho.m_radius;
			} 
			else
			{
				sho.m_type = Shape::RECT;
				printf("Ausdehnung x:\n");
				cin >> sho.m_extent_x;
				printf("Ausdehnung y:\n");
				cin >> sho.m_extent_y;
				
			}
				
			
			
			
		}
		else if (c=='9')
		{
			printf("\nx: ");
			cin >> x;
			printf("y: ") ;
			cin >> y;
			shs.m_coordinate_x =x;
			shs.m_coordinate_y =y;
			printf("1: Kreis 2: Rechteck\n");
			cin >> c2;
			if (c2=='1')
			{
				shs.m_type = Shape::CIRCLE;
				printf("radius:\n");
				cin >> shs.m_radius;
			} 
			else
			{
				shs.m_type = Shape::RECT;
				printf("Ausdehnung x:\n");
				cin >> shs.m_extent_x;
				printf("Ausdehnung y:\n");
				cin >> shs.m_extent_y;
				
			}
			
		}
		else if (c=='0')
		{
			
			// Testen der Element Löschen Funktion
			
			result= g_world->deleteSWObject(wo);
			
			// Testen des Resultats noch weniger vergessen als bei den obigen Fällen ;)
			if (result==true)
			{
				cout << "\n Löschen erfolgreich\n";
				wo=0;
			}
			else
			{
				cout << "\nLöschen nicht erfolgreich\n";
			}
			
		}
		
		else if (c=='c')
		{
			if (wo!=0 & wo->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_PLAYER)
			{
				pl = (Player*) wo;
				cout << "x\n";
				cin >> x;
				cout << "y\n";
				cin >> y;
				wo2 = g_world->getSWObjectAt(x, y, WorldObject::Geometry::LAYER_ALL, rid);
				ClientCommand com;
				com.m_coordinate_x = x;
				com.m_coordinate_y = y;
				com.m_action = act;
				com.m_button = LEFT_MOUSE_BUTTON;
				if (wo2!=0)
				{
					com.m_id = wo2->getId();
					com.m_action = act;
				}
				else
				{
					com.m_id = 0;
					com.m_action = Action::WALK;
				}
				
				pl->onClientCommand(&com);
				
				/*
				Command* com = cr->getCommand();
				
				if (wo2!=0)
				{
					com->m_type = Action::ATTACK;
					com->m_goal_object_id = wo2->getId();
					cout << "Zielobject "<<wo2->getId() << "\n";
				}
				else
				{
					com->m_type = Action::WALK;
					//DEBUG("goal : %f %f",x,y);
					com->m_goal_coordinate_x = x;
					com->m_goal_coordinate_y = y;
				}
				*/
				
			}
		}
		else if (c=='i')
		{
			if (g_world->intersect(&(wo->getGeometry()->m_shape), &shs))
				printf("ueberschneiden sich\n");
		}
		else if (c=='a')
		{
			cout << "Aktionsnummer angeben\n";
			int a;
			cin >> a;
			act = (Action::ActionType) a;
		}
		else if (c=='l')
		{
			float xs,xe,ys,ye;
			res->clear();
			cout << "x start\n";
			cin>> xs;
			cout << "y start\n";
			cin>> ys;
			cout << "x end\n";
			cin>> xe;
			cout << "y end\n";
			cin>> ye;
			
			g_world->getSWObjectsOnLine(xs,ys,xe,ye,WorldObject::Geometry::LAYER_AIR,rid,res);
			
			for (iter =res->begin(); iter!=res->end(); iter ++)
			{
				wo2 = *iter;
				wob = wo2->getGeometry();
				
				printf("Objekt: %f %f key: %i layer: %x\n",wob->m_shape.m_coordinate_x,wob->m_shape.m_coordinate_y,wo2->getId(),wob->m_layer);	
			}
			
		}
		else if (c=='x')
		{
			//Beenden
		}
		else
		{
			//Typo
			printf("Sie haben sich vertippt! x eingeben zum Beenden, andere Eingabe zum Fortfahren!\n");
			cin >> c;
		}
	}	
	
	return 1;
}


