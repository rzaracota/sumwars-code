#include "mapgenerator.h"
#include "world.h"


bool operator<(WeightedLocation first, WeightedLocation second)
{
	return first.m_value< second.m_value;
}

Region* MapGenerator::createRegion(RegionData* rdata)
{
	// temporaere Daten fuer die Erzeugung der Region
	MapGenerator::MapData mdata;

	bool success = false;
	int counter =0;
	while (!success)
	{
		counter ++;
		if (counter > 10)
			return 0;
		
		// Speicher anfordern
		MapGenerator::createMapData(&mdata,rdata);
		

		// grundlegende Karte anfertigen
		if (rdata->m_region_template =="")
		{
			MapGenerator::createBaseMap(&mdata,rdata);
		}
		
		// Umgebungen in die Region einfuegen
		std::list<std::pair<float, EnvironmentName> >::iterator et;
		for (et = rdata->m_environments.begin(); et != rdata->m_environments.end(); ++et)
		{
			mdata.m_region->insertEnvironment(et->first,et->second);
		}
		
		// Umgebungskarte generieren
		createPerlinNoise(mdata.m_region->getHeight(), rdata->m_dimx, rdata->m_dimy,std::min(rdata->m_dimx,rdata->m_dimy)/4 , 0.4,false);

		if (rdata->m_region_template =="")
		{
			// ermitteln wo die Objektgruppen platziert werden koennen
			MapGenerator::createTemplateMap(&mdata,rdata);
	
			// Objektgruppen platzieren
			success = MapGenerator::insertGroupTemplates(&mdata,rdata);
		}
		else
		{
			DEBUG("region template %s",rdata->m_region_template.c_str());
			// Region besteht aus einer einzelnen Objektgruppe
			// Objektgruppe anhand des Namens suchen
			ObjectGroup* templ;
			templ = ObjectFactory::getObjectGroup(rdata->m_region_template);
			if (templ ==0)
			{
				ERRORMSG("unknown object group %s",rdata->m_region_template.c_str());
				return 0;
			}

			Vector pos(rdata->m_dimx*2, rdata->m_dimy*2);
			mdata.m_region->createObjectGroup(rdata->m_region_template,pos,0);
			success = true;
		}
		
		// Events kopieren
		std::multimap<TriggerType, Event*>::iterator it;
		Event* ev;
		for (it = rdata->m_events.begin(); it != rdata ->m_events.end(); ++it)
		{
			ev = new Event(*(it->second));
			mdata.m_region->addEvent(it->first,ev);
		}
		
		mdata.m_region->setReviveLocation(rdata->m_revive_location);
		mdata.m_region->setGroundMaterial(rdata->m_ground_material);
		
		// Wenn der Versuch nicht erfolgreich war alles loeschen und von vorn beginnen
		if (!success)
		{
			DEBUG("not successful");

			delete mdata.m_base_map;
			delete mdata.m_template_map;
			delete mdata.m_template_index_map;
			delete mdata.m_region;
		}
	}

	if (rdata->m_region_template =="")
	{
		// Berandungen einfuegen
		MapGenerator::createBorder(&mdata,rdata);
	}
	
	// Ausgaenge erzeugen
	MapGenerator::createExits(&mdata,rdata);
	
	if (rdata->m_region_template =="")
	{
		// Monster einfuegen
		MapGenerator::insertSpawnpoints(&mdata,rdata);
	}

	// Speicher freigeben
	delete mdata.m_base_map;
	delete mdata.m_template_map;
	delete mdata.m_template_index_map;
	
	return mdata.m_region;
}

void MapGenerator::createMapData(MapData* mdata, RegionData* rdata)
{
	mdata->m_base_map = new Matrix2d<char>(rdata->m_dimx/2,rdata->m_dimy/2);
	mdata->m_base_map->clear();
	mdata->m_template_map = new Matrix2d<char>(rdata->m_dimx,rdata->m_dimy);
	mdata->m_template_map->clear();
	mdata->m_region = new Region(rdata->m_dimx,rdata->m_dimy,rdata->m_id,rdata->m_name,rdata);
	mdata->m_template_index_map = new Matrix2d<int>(rdata->m_dimx,rdata->m_dimy);
	mdata->m_template_places.clear();
	mdata->m_border.clear();
}

void MapGenerator::createBaseMap(MapData* mdata, RegionData* rdata)
{

	

	float size = rdata->m_area_percent;

	// Karte wird in 8x8 Felder erzeugt, Region rechnet aber in 4x4 Gridunits
	int dimx = rdata->m_dimx/2;
	int dimy = rdata->m_dimy/2;

	// temporaere Karte anlegen
	Matrix2d<float>* hmap = new Matrix2d<float>(dimx, dimy);

	hmap->clear();

	// Karte erzeugen mit Perlin Noise
	// erhaltenes Feld wird als Hoehenkarte interpretiert
	// alle Felder die unterhalb eines kritischen Niveaus liegen sind begehbar
	createPerlinNoise(hmap,dimx, dimy, rdata->m_granularity/2, rdata->m_complexity,true);


	// Delta zu Nachbarfeldern
	int nb[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
	int dnb[8][2] = {{-1,0},{1,0},{0,-1},{0,1},{-1,1},{1,1},{1,-1},{-1,-1}};

	// Suchen eine Niveaus das eine hinreichend grosse zusammenhaengende Flaeche erzeugt
	// Idee: alle Felder werden in einer PriorityQueue eingeordnet
	// alle Felder werden in einen UnionFind Baum eingeordnet
	// von kleinster Höhe beginnend werden die Felder jeweils mit dem tiefstliegenden Nachbarfeld verschmolzen

	std::priority_queue<WeightedLocation> fields_queue;
	WeightedLocation loc;
	for (int i=0; i< dimx; i++)
	{
		for (int j=0; j< dimy; j++)
		{
			loc.m_x = i;
			loc.m_y = j;
			loc.m_value = 1- (*hmap)[i][j];

			fields_queue.push(loc);
		}
	}

	UnionFindTree uftree(dimx*dimy);

	float height;
	int x,y,nbx, nby;

	// Solange *Wasserstand* erhoehen, bis eine genuegend grosse zusammenhaengende Flaeche gefunden wurde
	while (1)
	{
		// oberstes Feld aus der Queue nehmen
		loc = fields_queue.top();
		fields_queue.pop();

		height = 1-loc.m_value;
		x = loc.m_x;
		y = loc.m_y;

		// Schleife ueber die Nachbarfelder
		for (int i=0; i<4; i++)
		{
			nbx = x + nb[i][0];
			nby = y + nb[i][1];

			// Dimension pruefen
			if (nbx<0 || nby<0 || nbx >= dimx || nby >= dimy)
				continue;

			if ( (*hmap)[nbx][nby] <= height)
			{
				uftree.merge(nbx*dimy+nby,x*dimy+y);
			}
		}

		// Testen ob eine hinreichend große Flaeche entstanden ist
		if (uftree.getCardinality(x*dimy+y) >= dimx*dimy*size)
		{
			break;
		}

	}

	// kleiner Aufschlag um sehr enge Durchgaenge zu vermeiden
	height += 0.01;
	
	/*
	// Debugging
	DEBUG("height %f",height);
	for (int j=0; j< dimy; j++)
	{
		for (int i=0; i< dimx; i++)
		{
			if ((*hmap)[i][j]<height)
			{
				std::cout << " ";
			}
			else
			{
				std::cout << "X";
			}
		}
		std::cout << "\n";
	}
	*/
	
	// Floodfill um die Flaeche und deren Rand zu finden
	// die besuchten Felder werden in border mit 1 markiert
	// es werden nur Felder besucht, die unterhalb der gefundenen Hoehe liegen
	int cnt=0;
	std::queue<std::pair<int,int> > qu;
	std::pair<int,int> point;

	// Queue mit Felder die zum Rand gehoeren
	std::queue<std::pair<int,int> > borderqu;

	qu.push(std::make_pair(x,y));
	(*(mdata->m_base_map))[x][y]=1;

	// Solange die Queue nicht leer ist

	while (!qu.empty())
	{
		// Feld entnehmen
		point = qu.front();
		qu.pop();

		x = point.first;
		y = point.second;

		// Schleife ueber die Nachbarfelder
		for (int i=0; i<4; i++)
		{
			nbx = x + nb[i][0];
			nby = y + nb[i][1];

			// Dimension pruefen
			if (nbx<0 || nby<0 || nbx >= dimx || nby >= dimy)
				continue;

			// nur noch nicht besuchte Felder
			if ((*(mdata->m_base_map))[nbx][nby] != 0)
				continue;

			// Felder oberhalb der Obergrenze zum Rand hinzufuegen
			if ((*hmap)[nbx][nby] > height)
			{
				borderqu.push(std::make_pair(nbx,nby));
				(*(mdata->m_base_map))[nbx][nby]=-1;

				mdata->m_border.push_back(std::make_pair(nbx,nby));
				continue;
			}

			// Feld hinzufuegen
			qu.push(std::make_pair(nbx,nby));
			// als besucht markieren
			(*(mdata->m_base_map))[nbx][nby]=1;

			cnt++;
		}
	}
	
	//std::list<std::pair<int,int> >::iterator it;
	//for (
	
	
	// nochmal 3 Runden Floodfill um den Rand zu ermitteln

	// Marker einfuegen
	int markercnt =0;
	borderqu.push(std::make_pair(-1,-1));


	while (!borderqu.empty() && markercnt <3)
	{
		// Feld entnehmen
		point = borderqu.front();
		borderqu.pop();

		x = point.first;
		y = point.second;

		// Testen ob der Marker entnommen wurde
		if (x==-1)
		{
			markercnt ++;
			borderqu.push(std::make_pair(-1,-1));
			continue;
		}

		// Schleife ueber die Nachbarfelder
		for (int i=0; i<8; i++)
		{
			nbx = x + dnb[i][0];
			nby = y + dnb[i][1];

			// Dimension pruefen
			if (nbx<0 || nby<0 || nbx >= dimx || nby >= dimy)
				continue;

			// nur noch nicht besuchte Felder
			if ((*(mdata->m_base_map))[nbx][nby] != 0)
				continue;


			// Feld hinzufuegen
			borderqu.push(std::make_pair(nbx,nby));
			// als besucht markieren
			(*(mdata->m_base_map))[nbx][nby]=-1;
		}
	}
	

	delete hmap;
}

 void MapGenerator::createTemplateMap(MapData* mdata, RegionData* rdata)
{
	Matrix2d<int>  markval(rdata->m_dimx,rdata->m_dimy);
	markval.clear();
	
	// Delta zu Nachbarfeldern
	int nb[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};

	int hdimx = rdata->m_dimx/2;
	int hdimy = rdata->m_dimy/2;

	int dnb[8][2] = {{-1,0},{1,0},{0,-1},{0,1},{-1,1},{1,1},{1,-1},{-1,-1}};

	// moegliche Randfelder in den Nachbarfeldern
	int nbr[4][4] = {{1,0,1,1},{0,0,0,1},{0,1,1,1},{0,0,1,0}};

	// Rand finden und in ein doppelt so genaues Feld uebertragen
	std::queue<std::pair<int,int> > qu;
	std::list<std::pair<int,int> >::iterator it;

	// Schleife ueber die Randfelder
	for (it = mdata->m_border.begin(); it != mdata->m_border.end(); ++it)
	{
		int i,j,nbi,nbj;
		int nbif,nbjf;
		i = it->first;
		j = it->second;

		// der Rand selber wird mit -1 markiert
		// ein Randfeld wird in dieser Matrix ein 2x2 Block
		*(mdata->m_template_map->ind(i*2,j*2))= -1;
		*(mdata->m_template_map->ind(i*2+1,j*2))= -1;
		*(mdata->m_template_map->ind(i*2+1,j*2+1))= -1;
		*(mdata->m_template_map->ind(i*2,j*2+1))= -1;

		// die 8 angrenzenden Felder werden mit 1 markiert, wenn sie frei sind
		for (int k=0; k<4; k++)
		{
			nbi = i + nb[k][0];
			nbj = j + nb[k][1];

			if (nbi<1 || nbj <1 || nbi >=hdimx-1 || nbj >= hdimy-1)
				continue;

			if (*(mdata->m_base_map->ind(nbi,nbj)) >=1)
			{
				for (int l=0; l<4; l+=2)
				{
					nbif = nbi*2+nbr[k][l];
					nbjf = nbj*2+nbr[k][l+1];

					if (*(mdata->m_template_map->ind(nbif,nbjf)) == 0)
					{
						qu.push(std::make_pair(nbif,nbjf));
						*(mdata->m_template_map->ind(nbif,nbjf)) = 1;

						mdata->m_template_places[1].push_back(nbif * 10000 + nbjf);
						*(mdata->m_template_index_map->ind(nbif,nbjf)) = mdata->m_template_places[1].size()-1;
						
						
					}

					markval[nbif][nbjf] =1;
				}

			}
			else
			{
				*(mdata->m_template_map->ind(nbi*2+nbr[k][0],nbj*2+nbr[k][1])) = -1;
				*(mdata->m_template_map->ind(nbi*2+nbr[k][2],nbj*2+nbr[k][3])) = -1;
			}

		}
	}

	// Floodfill mit 8-Adjazenz um Ort mit viel Abstand zum Rand zu ermitteln
	qu.push(std::make_pair(-1,-1));
	int markercnt =2;
	std::pair<int,int> point;
	int x,y,nbx,nby;
	
	while (!qu.empty())
	{
		// Feld entnehmen
		point = qu.front();
		qu.pop();

		x = point.first;
		y = point.second;

		// Testen ob der Marker entnommen wurde
		if (x==-1)
		{
			if (qu.empty())
				break;

			markercnt ++;
			qu.push(std::make_pair(-1,-1));
			continue;
		}

		// Schleife ueber die Nachbarfelder
		for (int i=0; i<8; i++)
		{
			nbx = x + dnb[i][0];
			nby = y + dnb[i][1];

			// Dimension pruefen
			if (nbx<2 || nby<2 || nbx >= rdata->m_dimx-2 || nby >= rdata->m_dimy-2)
				continue;

			// nur noch nicht besuchte Felder
			if ((*(mdata->m_template_map))[nbx][nby] != 0)
				continue;


			// Feld hinzufuegen
			qu.push(std::make_pair(nbx,nby));

			// als besucht markieren
			(*(mdata->m_template_map))[nbx][nby]= markercnt;
			mdata->m_template_places[markercnt].push_back(nbx*10000+nby);
			*(mdata->m_template_index_map->ind(nbx,nby)) = mdata->m_template_places[markercnt].size() -1;
			
			markval[nbx][nby] = markercnt;
		}
	}
	
	/*
	for (int i=0; i< rdata->m_dimx; i++)
	{
		for (int j=0; j<rdata->m_dimy; j++)
		{
			std::cout << markval[i][j] << " ";
		}
		std::cout << "\n";
	}
	
	std::map< int, std::vector<int> >::iterator mt;
	for (mt = mdata->m_template_places.begin(); mt != mdata->m_template_places.end(); ++mt)
	{
		DEBUG("templates size %i: %i",mt->first, mt->second.size());
	}
	*/
}

bool MapGenerator::insertGroupTemplates(MapData* mdata, RegionData* rdata)
{
	// obligatorische Objektgruppen einfuegen

	std::multimap<int, RegionData::NamedObjectGroup >::reverse_iterator it;
	ObjectGroup* templ;
	Shape s;
	Vector pos;
	bool succ;
	LocationName locname, areaname;
	for (it = rdata->m_named_object_groups.rbegin(); it != rdata->m_named_object_groups.rend(); ++it)
	{
		// Objektgruppe anhand des Namens suchen
		templ = ObjectFactory::getObjectGroup(it->second.m_group_name);
		if (templ ==0)
		{
			ERRORMSG("unknown object group %s",it->second.m_group_name.c_str());
			continue;
		}

		// Grundform der Gruppe kopieren
		s.m_type = templ->getShape()->m_type;
		s.m_extent = templ->getShape()->m_extent;
		s.m_radius = templ->getShape()->m_radius;
		s.m_angle = templ->getShape()->m_angle + it->second.m_angle;

		DEBUG("template angle %f",s.m_angle);
		
		// Ort fuer das Template suchen
		succ = getTemplatePlace(mdata,&s,pos);
		s.m_center = pos;

		if (succ == false)
		{
			// Obligatorisches Template konnte nicht platziert werden
			DEBUG("could not place template %s",it->second.m_group_name.c_str());
			/*
			int hdimx = rdata->m_dimx/2;
			int hdimy = rdata->m_dimy/2;
			for (int i=0; i<hdimx; i++)
			{
				for (int j=0; j< hdimy;j++)
				{
					if (*(mdata->m_base_map->ind(i,j)) != -1)
					{
						std::cout << "  ";
					}
					else
					{
						std::cout << "# ";
					}
				}
				std::cout << "\n";
			}
			*/
			return false;
		}

		// Objektgruppe einfuegen
		// TODO: Winkel ?
		DEBUG5("placing group %s at %f %f",it->second.m_group_name.c_str(), pos.m_x, pos.m_y);
		mdata->m_region->createObjectGroup(it->second.m_group_name,pos,s.m_angle, it->second.m_name);

		
	}
	
	if (rdata->m_has_waypoint)
	{
		// Wegpunkt einfuegen
		templ = ObjectFactory::getObjectGroup("waypoint_templ");
		memcpy(&s , templ->getShape(), sizeof(Shape));
		succ = getTemplatePlace(mdata,&s,pos);
		if (succ == false)
		{
			// Wegpunkt konnte nicht platziert werden
			DEBUG("could not place waypoint");
			return false;
		}
		mdata->m_region->createObjectGroup("waypoint_templ",pos,0);
	}
	
	// fakultative Objektgruppen einfuegen
	std::multimap<int, RegionData::ObjectGroupSet >::reverse_iterator jt;
	for (jt = rdata->m_object_groups.rbegin(); jt != rdata->m_object_groups.rend(); ++jt)
	{
		templ = ObjectFactory::getObjectGroup(jt->second.m_group_name);
		if (templ ==0)
		{
			ERRORMSG("unknown object group %s",jt->second.m_group_name.c_str());
			continue;
		}

		float angle =0;
		for (int i=0; i< jt->second.m_number; i++)
		{
			if (Random::random() > jt->second.m_probability)
			{
				continue;
			}
			memcpy(&s , templ->getShape(), sizeof(Shape));
			
			// Drehwinkel ermitteln
			// Kreise beliebig Rechtecke nur um 90°
			if (s.m_type == Shape::CIRCLE)
			{
				angle = 2*3.14159*Random::random();
			}
			else
			{
				int n = Random::randi(4);
				if (n%2 == 1)
				{
					std::swap(s.m_extent.m_x, s.m_extent.m_y);
				}
				angle = 3.14159*(n*90.0)/180.0;
			}

			// Ort fuer das Template suchen
			succ = getTemplatePlace(mdata,&s,pos,jt->second.m_decoration );

			if (succ == false)
			{
				// Template konnte nicht platziert werden
				break;
			}

			// Objektgruppe einfuegen
			DEBUG5("placing group %s at %f %f",jt->second.m_group_name.c_str(), pos.m_x, pos.m_y);
			mdata->m_region->createObjectGroup(jt->second.m_group_name,pos,angle);
		}
	}
	
	

	return true;
}


void MapGenerator::createBorder(MapData* mdata, RegionData* rdata)
{
	// Delta zu Nachbarfeldern
	int nb[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};

	int hdimx = rdata->m_dimx/2;
	int hdimy = rdata->m_dimy/2;

	// Orte fuer den Ausgang in den vier Richtungen
	int exit[4][2];
	int exitcount[4];
	for (int i=0; i<4; i++)
	{
		exit[i][0] = nb[i][0]*-1000;
		exit[i][1] = nb[i][1]*-1000;
		exitcount[i] =1;
	}

	// Ausgaenge suchen
	for (int i=0; i<hdimx; i++)
	{
		for (int j=0; j< hdimy;j++)
		{

			int rnd;
			if (*(mdata->m_base_map->ind(i,j)) >=1 )
			{
				// Testen ob das Feld als Ausgang in Frage kommt
				// Also am noerdlichsten, westlichsten usw liegt
				for (int k=0; k<4; k++)
				{
					if (exit[k][0]*nb[k][0] + exit[k][1]*nb[k][1] < i*nb[k][0] + j*nb[k][1])
					{
						exit[k][0] =i;
						exit[k][1] =j;
						exitcount[k] =1;
					}
					
					if (exit[k][0]*nb[k][0] + exit[k][1]*nb[k][1] == i*nb[k][0] + j*nb[k][1])
					{
						rnd = Random::randi(exitcount[k]);
						if (rnd ==0)
						{
							exit[k][0] =i;
							exit[k][1] =j;
						}
						exitcount[k] ++;
					}
				}
			}

		}
	}

	// Ausgaenge platzieren
	for (int k=0; k<4; k++)
	{
		std::string dirname[4] = {"west","east","north","south"};
		std::string locname;
		if (rdata->m_exit_directions[k])
		{
			int i = exit[k][0];
			int j = exit[k][1];


			locname = "entry_";
			locname += dirname[k];
			mdata->m_region->addLocation(locname,Vector(i*8+4,j*8+4));
            *(mdata->m_base_map->ind(i,j)) = 2;

			i+= nb[k][0];
			j+= nb[k][1];
			locname = "exit_";
			locname += dirname[k];
			mdata->m_region->addLocation(locname,Vector(i*8+4,j*8+4));

			while (i>=0 && j>=0 && i< hdimx && j<hdimy)
			{
				*(mdata->m_base_map->ind(i,j)) = 2;
				i+= nb[k][0];
				j+= nb[k][1];
			}
		}
	}


	
	// fuer jedes markierte Feld werden die 4 Nachbarfelder angesehen
	// die 4 Informationen werden als 4bit Zahl interpretiert
	// blockierte Felder erhalten eine 0, freie eine 1
	//
	// es gibt 6 verschiedene Konstellationen fuer die Nachbarfelder
	// die 4bit Zahl dient als Index um die Konstellation und den Drehwinkel zu erhalten
	//
	// Aufbau der Bitmaske:
	// 
	//  D
	// A*B
	//  C
	// => Bitmaske ABCD
	
	// Liste mit den 6 verschiedenen Moeglichenkeiten welche Nachbarfelder besetzt sind
	// Form der 6 Template (+ frei, # blockiert):
	//
	// ?+?   ?+?   ?+?   ?+?   ?#?   ?+?
	// ###   ##+   ###   ##+   ###   +#+
	// ?#?   ?#?   ?+?   ?+?   ?#?   ?+?
	std::string borders[6] = {"border(side)","border(corner)","border(twoside)","border(twocorner)","border(filled)","border(single_block)"};

	// Maske dafuer, welche Situation vorliegt
	int bmask[16]={4,0,0,2,0,1,1,3,0,1,1,3,2,3,3,5};

	// Maske fuer die Rotationswinkel in Schritten von 90 Grad
	int rotmask[16] = {0,0,2,0,3,0,3,0,1,1,2,2,1,1,3,0};
	int mask =0;
	int nbi, nbj;

	// Abstaende der Diagonalen
	int diag[4][2] ={{-1,-1},{-1,1},{1,1},{1,-1}};
	
	// Fuer die Diagonalen wird eine aehnliche Bitmaske erstellt:
	// // eine 1 wird gesetzt, wenn das Feld selbst frei ist, die beiden Felder daneben aber besetzt
	//
	// B C
	//  *
	// A D
	// => Bitmaske ABCD
	
	// Liste mit den verschiedenen Moeglichenkeiten welche Nachbarfelder besetzt sind
	// Form der Template (+ frei, # blockiert):
	//
	// ###   ###   ##+   +##   +#+   +#+
	// ###   ###   ###   ###   ###   ###
	// ###   ##+   ##+   ##+   ##+   +#+
	
	std::string smallcorners[8] = {"","(one_smallcorner)","(two_smallcorner)","(diag_smallcorner)","(three_smallcorner)","(four_smallcorner)"};
		
	int diagbmask[16]=    {0,1,1,2,1,3,2,4, 1,2,3,4,2,4,4,5};
	int diagrotmask[16] = {0,0,1,0,2,0,1,0, 3,3,3,3,2,2,1,0};
	
	int diagi, diagj;
	int dmask;
	
	ObjectGroupName templ,diagtempl;
	float angle;

	bool skip;

	
	for (int j=0; j< hdimy;j++)
	{
		for (int i=0; i<hdimx; i++)
		{
			skip = false;
			if (*(mdata->m_base_map->ind(i,j)) != -1)
			{
				//std::cout << "  ";
				continue;
			}

			mask =0;
			dmask =0;
			
			// Bitmasken aufbauen
			for (int k=0; k<4; k++)
			{
				nbi = i + nb[k][0];
				nbj = j + nb[k][1];

				mask *=2;
				dmask  *= 2;
				
				if ( nbi>=0 && nbj>=0 && nbi<hdimx && nbj<hdimy)
				{
					if (*(mdata->m_base_map->ind(nbi,nbj)) >= 1)
					{
						mask +=1;
					}

				// Wenn eines der Nachbarfelder im *leeren Raum* liegt
				// dann keine Objekte setzen
					if (*(mdata->m_base_map->ind(nbi,nbj)) == 0)
					{
						skip = true;
					}
				}

			
				
				// Bitmaske fuer Diagonalen
				diagi = i + diag[k][0];
				diagj = j + diag[k][1];
				
				if ( diagi>=0 && diagj>=0 && diagi<hdimx && diagj<hdimy)
				{
					if (*(mdata->m_base_map->ind(diagi,diagj)) >= 1 &&
						*(mdata->m_base_map->ind(diagi,j)) == -1 &&
						*(mdata->m_base_map->ind(i,diagj)) == -1)
					{
						dmask +=1;
					}
				}

			}
			
			//std::cout << bmask[dmask] <<" ";
			if (skip)
				continue;

			// Template und dessen Winkel bestimmen
			templ = borders[bmask[mask]];
			angle = rotmask[mask] *PI/2;

			diagtempl = smallcorners[diagbmask[dmask]];
			float diagangle = diagrotmask[dmask] *PI/2;
			
			// gemeinsames Template aus Innenecken und Rand herstellen
			if (templ == "border(filled)" && diagtempl !="")
			{
				templ ="border";
				templ += diagtempl;
				angle = diagangle;
			}
			else if (templ == "border(side)" && diagtempl == "(one_smallcorner)")
			{
				if (angle == diagangle)
				{
					templ = "border(side)(right_smallcorner)";
				}
				else
				{
					templ = "border(side)(left_smallcorner)";
				}
			}
			else
			{
				templ += diagtempl;
			}
			
			DEBUG5("placing type (%i %i) %s",i,j,templ.c_str());
			
			mdata->m_region->createObjectGroup(templ,Vector(i*8+4,j*8+4),angle);
			
			// TODO Templates mergen ?
			/*
			if (diagbmask[dmask] != 0)
			{
				diagtempl = "border(";
				diagtempl += smallcorners[diagbmask[dmask]];
				diagtempl += ")";
				
				DEBUG5("placing type (%i %i) %i %s",i,j,diagbmask[dmask], diagtempl.c_str());
				
				mdata->m_region->createObjectGroup(diagtempl,Vector(i*8+4,j*8+4),diagangle);
			}
			*/
		}
		//std::cout << "\n";
	}



}

void MapGenerator::insertSpawnpoints(MapData* mdata, RegionData* rdata)
{
	// Punkte an denen Spawnpoints möglich sind
	std::vector< std::pair<int,int> > points;

	int hdimx = rdata->m_dimx/2;
	int hdimy = rdata->m_dimy/2;

	// moegliche Orte fuer Spawnpoints ermitteln
	for (int i=0; i<hdimx; i++)
	{
		for (int j=0; j< hdimy;j++)
		{
			// nur erreichbare Felder
			if (*(mdata->m_base_map->ind(i,j)) !=1)
			{
				continue;
			}

			points.push_back(std::make_pair(i,j));
		}
	}


	// Spawnpoints platzieren
	std::list<RegionData::SpawnGroup>::iterator st;
	WorldObject* wo;
	bool stop = false;
	for (st = rdata->m_monsters.begin(); st != rdata->m_monsters.end() &&!stop; ++st)
	{
		// aufhoeren, wenn keine Orte mehr vorhanden
		
		DEBUG5("%s x %i",st->m_monsters.c_str(),st->m_number);
		for (int i=0; i< st->m_number; i++)
		{
			int r = Random::randi(points.size());
			wo = new Spawnpoint(st->m_monsters);

			mdata->m_region->insertObject(wo,Vector(points[r].first*8+4, points[r].second*8+4));

			DEBUG5("placing spawnpoint for %s at %i %i",st->m_monsters.c_str(), points[r].first*8+4, points[r].second*8+4);
			
			points[r] = points.back();
			points.resize(points.size() -1);
			if (points.size() ==0)
			{
				stop = true;
				break;
			}
		}
	}
}


void MapGenerator::createExits(MapData* mdata, RegionData* rdata)
{
	int dimx = rdata->m_dimx/2;
	int dimy = rdata->m_dimy/2;
	
	std::list<RegionExit>::iterator it;
	Vector pos;
	for (it = rdata->m_exits.begin(); it != rdata->m_exits.end(); ++it)
	{
		mdata->m_region->addExit(*it);

		// Bei den Ausgaengen keine Monster
		if (rdata->m_region_template =="")
		{
			pos = mdata->m_region->getLocation(it->m_exit_name);
			int cx = int(pos.m_x/8);
			int cy = int(pos.m_y/8);
			for (int i=std::max(0,cx-2); i<std::min(dimx,cx+2); i++)
			{
				for (int j=std::max(0,cy-2); j<std::min(dimy,cy+2); j++)
				{
					if (*(mdata->m_base_map->ind(i,j))==1)
					{
						*(mdata->m_base_map->ind(i,j)) = 2;
					}
				}
			}
		}

	}
	
	if (rdata->m_has_waypoint)
	{
		Vector pos = mdata->m_region->getLocation("WaypointLoc");
		WorldObject* wo =	 new Waypoint();
		mdata->m_region->insertObject(wo,pos);
		mdata->m_region->getWaypointLocation() = pos;
		
		//mdata->m_region->insertObject(wo,pos);
		DEBUG("waypoint at %f %f %i",pos.m_x, pos.m_y,wo->getId());
		
		// Bei den Ausgaengen keine Monster
		if (rdata->m_region_template =="")
		{
			int cx = int(pos.m_x/8);
			int cy = int(pos.m_y/8);
			for (int i=std::max(0,cx-2); i<std::min(dimx,cx+2); i++)
			{
				for (int j=std::max(0,cy-2); j<std::min(dimy,cy+2); j++)
				{
					if (*(mdata->m_base_map->ind(i,j))==1)
					{
						*(mdata->m_base_map->ind(i,j)) = 2;
					}
				}
			}
		}
	}
}

bool MapGenerator::getTemplatePlace(MapData* mdata, Shape* shape, Vector & place, bool deco)
{
	if (shape ==0)
	{
		return false;
	}

	bool success = false;

	// Groesse des Templates in 4x4 Feldern
	Vector ext = shape->getAxisExtent();
	int ex = int(ceil(ext.m_x/2));
	int ey = int(ceil(ext.m_y/2));

	// Gibt an welche wie viele 4x4 Felder in jede Richtung frei sein muessen
	int size = int(std::max(ex, ey))/2 +1;

	std::map< int, std::vector<int> >::iterator mt, mt2;
	std::vector<int>::iterator st;

	int pi,pj;
	int imin,jmin,imax,jmax;
	int pl;

	while (!success)
	{
		int nr;
		// Anzahl der in Frage kommenden Orte ermitteln
		// alle Orte aufsummieren mit Abstandsindex mindens size
		nr =0;
		mt2 = mt = mdata->m_template_places.lower_bound(size);

		while (mt != mdata->m_template_places.end())
		{
			nr += mt->second.size();
			++mt;
		}

		if (nr == 0)
		{
			// kein Platz fuer das Template vorhanden
			return false;
		}
		DEBUG5("%i places for template of size %i",nr,size);
		// Ort auswuerfeln und ermitteln
		nr = Random::randi(nr);
		mt = mt2;
		while (nr >= int(mt->second.size()))
		{
			nr -= mt->second.size();
			++mt;
		}

		pl = mt->second[nr];

		// Flaeche die durch das Template ueberdeckt wird bestimmen
		pi = pl / 10000;
		pj = pl % 10000;
		imin = pi - (ex-1)/2;
		jmin = pj - (ey-1)/2;
		imax = imin + ex-1;
		jmax = jmin + ey-1;
		// Ort eines eventuellen Hindernisses
		int obi = -1, obj= -1;

		// Pruefen ob die Flaeche keine Hindernisse enthaelt
		for (int i = imin; i<= imax; i++)
		{
			for (int j= jmin; j<= jmax; j++)
			{
				if (*(mdata->m_template_map->ind(i,j)) <0)
				{
					obi = i;
					obj = j;
					DEBUG5("found obstacle at %i %i",i,j);
					break;
				}
			}
			if (obi != -1)
			{
				break;
			}
		}

		int midx,vidx;
		int b, bi, bj;

		if (obi != -1)
		{
			// Hindernis gefunden

			// dieses Hindernis verhindert das platzieren der Templates in seiner Umgebung
			// in dieser Umgebung werden die Zahlen fuer den minimalen Abstand zum naechsten Hindernis aktualisiert
			imin = obi - size+1;
			jmin = obj - size+1;
			imax = obi + size-1;
			jmax = obj + size-1;

			int newd;

			// Schleife ueber die Umgebung des Hindernisses
			for (int i = imin; i<= imax; i++)
			{
				for (int j= jmin; j<= jmax; j++)
				{
					newd = (int) std::max(fabs(obi-i) , fabs(obj-j));
					if (newd < *(mdata->m_template_map->ind(i,j)))
					{
						// Feld gefunden dessen Abstandsindex aktualisiert werden muss

						// Indizee an welcher Stelle das Feld in der m_template_places Datenstruktur zu finden ist
						midx = *(mdata->m_template_map->ind(i,j));
						vidx = *(mdata->m_template_index_map->ind(i,j));

						if (midx<=0)
						{
							continue;
						}

						// Entfernen aus der Datenstruktur
						if (mdata->m_template_places[midx].size() >1)
						{
							b = mdata->m_template_places[midx].back();
							mdata->m_template_places[midx][vidx] = b;
							bi = b / 10000;
							bj = b % 10000;
							*(mdata->m_template_index_map->ind(bi,bj)) = vidx;

							mdata->m_template_places[midx].resize(mdata->m_template_places[midx].size() -1);

						}
						else
						{
							mdata->m_template_places.erase(midx);
						}


						// wieder einfuegen an der richtigen Stelle
						mdata->m_template_places[newd].push_back(i*10000+j);
						*(mdata->m_template_map->ind(i,j)) = newd;
						*(mdata->m_template_index_map->ind(i,j)) = mdata->m_template_places[newd].size()-1;
					}

				}
			}

		}
		else
		{
			// Orte die von dem Template ueberdeckt werden aus der Liste der freien Orte entfernen

			// Objekt auf der Template Karte eintragen
			for (int i = imin; i<= imax; i++)
			{
				for (int j= jmin; j<= jmax; j++)
				{
					// Indizee an welcher Stelle das Feld in der m_template_places Datenstruktur zu finden ist
					midx = *(mdata->m_template_map->ind(i,j));
					vidx = *(mdata->m_template_index_map->ind(i,j));

					// Element aus der Liste der freien Ort entfernen
					// letztes Element an die Stelle des geloeschten kopieren
					if (mdata->m_template_places[midx].size() >1)
					{

						// Eintragung in der index Karte korrigieren
						b = mdata->m_template_places[midx].back();
						mdata->m_template_places[midx][vidx] = b;
						bi = b / 10000;
						bj = b % 10000;
						*(mdata->m_template_index_map->ind(bi,bj)) = vidx;

						mdata->m_template_places[midx].resize(mdata->m_template_places[midx].size() -1);

					}
					else
					{
						mdata->m_template_places.erase(midx);
					}

					// auf der Template Karte als blockiert eintragen
					*(mdata->m_template_map->ind(i,j)) = -2;
					
					// wenn nicht dekorativ hier keine Monster platzieren
					if (!deco)
					{
						*(mdata->m_base_map->ind(i/2,j/2)) = 2;
					}

				}
			}

			place.m_x = pi*4+2;
			place.m_y = pj*4+2;

			if (ex %2 ==0)
				place.m_x += 2;
			if (ey %2 ==0)
				place.m_y += 2;

			DEBUG5("found place %i %i",pi,pj);

			success = true;
		}
	}

	return true;
}


void MapGenerator::createPerlinNoise(Matrix2d<float> *data, int dimx, int dimy,int startfreq, float persistance, bool bounds)
{
	Matrix2d<float>* tmp= new Matrix2d<float>(dimx+1, dimy+1);
	tmp->clear();
	float* weight = new float[std::min(dimx,dimy)];
	float ampl =1;
	int dx,dy;
	float invdist;
	
	if (startfreq > dimx || startfreq > dimy)
	{
		startfreq = std::min(dimx,dimy);
	}
	
	DEBUG5("Perlin noise dimx %i dimy %i",dimx,dimy);
	
	// Perlin Noise Algorithmus
	// Aenderung hier: Schleife geht ueber den Abstand zwischen den Stuetzstellen
	for (int dist = startfreq; dist>0 ;dist/=2)
	{
		// Dimension des Zufallszahlenfeldes fuer diesen Abstand
		dx = 2+(dimx-2)/dist;
		dy = 2+(dimy-2)/dist;
		
		DEBUG5("distance %i  tmp array %i %i",dist,dx,dy);
		
		// anlegen der Zufallszahlen fuer die aktuelle Frequenz
		for (int i=0;i<=dx;i++)
		{
			for (int j=0;j<=dy;j++)
			{
				*(tmp->ind(i,j)) = Random::random()* ampl;

			}

		}

		// Interpolation (nur fuer Abstand >1)
		if (dist>1)
		{
			invdist = 1.0f/dist;
	
			for (int k=0;k<dist;k++)
			{
				weight[k] = k*invdist;
			}
	
			
			
			for (int i=0;i<dx;i++)
			{
				for (int j=0;j<dy;j++)
				{
					for (int k=0;k<dist;k++)
					{
						for (int l=0;l<dist;l++)
						{
							if (i*dist+k< dimx && j*dist+l< dimy)
							{
								*(data->ind(i*dist+k,j*dist+l)) += (*(tmp->ind(i,j))*(1-weight[k])*(1-weight[l])
										+ *(tmp->ind(i+1,j))*weight[k]*(1-weight[l])
										+ *(tmp->ind(i+1,j+1))*weight[k]*weight[l]
										+ *(tmp->ind(i,j+1))*(1-weight[k])*weight[l]) ;
							}
						}
					}
				}
			}
		}
		else
		{
			for (int i=0;i<dimx;i++)
			{
				for (int j=0;j<dimy;j++)
				{
					*(data->ind(i,j)) +=*(tmp->ind(i,j));
				}
			}
		}

		ampl *= persistance;
		/*
		if (bounds)
		{
		
			std::cout.width(6);
			std::cout << "\n";
			for (int j=0;j<dy;j++)
			{
				for (int i=0;i<dx;i++)
				{
				
					std::cout << *(tmp->ind(i,j))<<" ";
				}
				std::cout << "\n";
			}
		}
		if (bounds)
		{
		
			std::cout.width(6);
			std::cout << "\n";
			for (int j=0;j<dimy;j++)
			{
				for (int i=0;i<dimx;i++)
				{
					std::cout << *(data->ind(i,j))<<" ";
				}
				std::cout << "\n";
			}
		}
		*/
	}

	// Maximum und Minimum suchen
	float fmax =0,fmin = 1000000;
	for (int i=0;i<dimx;i++)
	{
		for (int j=0;j<dimy;j++)
		{
			if (*(data->ind(i,j)) > fmax)
			{
				fmax = *(data->ind(i,j));
			}
			if (*(data->ind(i,j)) < fmin)
			{
				fmin = *(data->ind(i,j));
			}


		}
	}

	// normieren auf [0..1]
	for (int i=0;i<dimx;i++)
	{
		for (int j=0;j<dimy;j++)
		{
			*(data->ind(i,j)) -= fmin;
			if (fmax-fmin !=0)
			{
				*(data->ind(i,j)) /= (fmax-fmin);
			}
		}
	}
	
	
	// Raender anlegen
	if (bounds)
	{
		float bnd = std::min(5.0f, std::min(dimx/3.0f,dimy/3.0f));
		float dist;
		for (int i=0;i<dimx;i++)
		{
			for (int j=0;j<dimy;j++)
			{
				dist = std::min(std::min(i,dimx-i-1),std::min(j,dimy-j-1));
				if (dist<bnd)
				{
					*(data->ind(i,j)) = *(data->ind(i,j))*(dist)/bnd + (bnd-dist)/bnd;
				}
			}
		}
	}
	
	/*
	//Debugging
	
	std::cout << "\n";
	for (int j=0;j<dimy;j++)
	{
		for (int i=0;i<dimx;i++)
		{
			std::cout << *(data->ind(i,j))<<" ";
		}
		std::cout << "\n";
	}
	*/
	delete[] weight;
	delete tmp;
}



