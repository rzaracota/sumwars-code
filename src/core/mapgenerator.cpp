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
	while (!success)
	{
		// Speicher anfordern
		MapGenerator::createMapData(&mdata,rdata);

		// grundlegende Karte anfertigen
		MapGenerator::createBaseMap(&mdata,rdata);

		// ermitteln wo die Objektgruppen platziert werden koennen
		MapGenerator::createTemplateMap(&mdata,rdata);

		// Objektgruppen platzieren
		success = MapGenerator::insertGroupTemplates(&mdata,rdata);

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

	// Berandungen einfuegen
	MapGenerator::createBorder(&mdata,rdata);

	// Ausgaenge erzeugen
	MapGenerator::createExits(&mdata,rdata);

	// Monster einfuegen
	MapGenerator::insertSpawnpoints(&mdata,rdata);

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
	mdata->m_region = new Region(rdata->m_dimx,rdata->m_dimy,rdata->m_id,rdata->m_name);
	mdata->m_template_index_map = new Matrix2d<int>(rdata->m_dimx,rdata->m_dimy);
}

void MapGenerator::createBaseMap(MapData* mdata, RegionData* rdata)
{

	// Umgebungen in die Region einfuegen

	std::list<std::pair<float, EnvironmentName> >::iterator et;
	for (et = rdata->m_environments.begin(); et != rdata->m_environments.end(); ++et)
	{
		mdata->m_region->insertEnvironment(et->first,et->second);
	}

	float size = rdata->m_area_percent;

	int dimx = rdata->m_dimx/2;
	int dimy = rdata->m_dimy/2;

	// temporaere Karte anlegen
	Matrix2d<float>* hmap = new Matrix2d<float>(dimx, dimy);

	hmap->clear();

	// Karte erzeugen mit Perlin Noise
	// erhaltenes Feld wird als Hoehenkarte interpretiert
	// alle Felder die unterhalb eines kritischen Niveaus liegen sind begehbar
	createPerlinNoise(hmap,dimx, dimy, rdata->m_granularity, rdata->m_complexity,true);


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

			// Dimension pruefen, die Randfelder werden auch generell nicht gewaehlt
			if (nbx<1 || nby<1 || nbx >= dimx-1 || nby >= dimy-1)
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
	//std::cout << "area percent: "<<cnt*1.0/(dimx*dimy)<<"\n";

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

	// Umgebungskarte generieren
	createPerlinNoise(mdata->m_region->getHeight(), rdata->m_dimx, rdata->m_dimy,4 , 0.4,false);

	delete hmap;
}

 void MapGenerator::createTemplateMap(MapData* mdata, RegionData* rdata)
{
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
		}
	}
}

bool MapGenerator::insertGroupTemplates(MapData* mdata, RegionData* rdata)
{
	// obligatorische Objektgruppen einfuegen

	std::multimap<int, RegionData::NamedObjectGroupTemplate >::reverse_iterator it;
	ObjectGroupTemplate* templ;
	Shape s;
	Vector pos;
	bool succ;
	LocationName locname, areaname;
	for (it = rdata->m_named_object_groups.rbegin(); it != rdata->m_named_object_groups.rend(); ++it)
	{
		// Objektgruppe anhand des Namens suchen
		templ = ObjectFactory::getObjectGroupTemplate(it->second.m_group_name);
		if (templ ==0)
		{
			ERRORMSG("unknown object group %s",it->second.m_group_name.c_str());
			continue;
		}

		// Grundform der Gruppe kopieren
		s.m_type = templ->getShape()->m_type;
		s.m_extent = templ->getShape()->m_extent;
		s.m_radius = templ->getShape()->m_radius;

		
		// Ort fuer das Template suchen
		succ = getTemplatePlace(mdata,&s,pos);
		s.m_center = pos;

		if (succ == false)
		{
			// Obligatorisches Template konnte nicht platziert werden
			return false;
		}

		// Objektgruppe einfuegen
		// TODO: Winkel ?
		DEBUG5("placing group %s at %f %f",it->second.m_group_name.c_str(), pos.m_x, pos.m_y);
		mdata->m_region->createObjectGroup(it->second.m_group_name,pos,0);

		// Mittelpunkt eintragen
		locname = it->second.m_name;
		locname += ":center";
		mdata->m_region->addLocation(locname,pos);
		
		// Flaeche eintragen
		areaname = it->second.m_name;
		areaname += ":area";
		mdata->m_region->addArea(areaname,s);
	}


	// fakultative Objektgruppen einfuegen
	std::multimap<int, RegionData::ObjectGroupTemplateSet >::reverse_iterator jt;
	for (jt = rdata->m_object_groups.rbegin(); jt != rdata->m_object_groups.rend(); ++jt)
	{
		templ = ObjectFactory::getObjectGroupTemplate(jt->second.m_group_name);
		if (templ ==0)
		{
			ERRORMSG("unknown object group %s",jt->second.m_group_name.c_str());
			continue;
		}

		// Grundform der Gruppe kopieren
		memcpy(&s , templ->getShape(), sizeof(Shape));

		for (int i=0; i< jt->second.m_number; i++)
		{
			if (Random::random() > jt->second.m_probability)
			{
				continue;
			}

			// Ort fuer das Template suchen
			succ = getTemplatePlace(mdata,&s,pos);

			if (succ == false)
			{
				// Template konnte nicht platziert werden
				break;
			}

			// Objektgruppe einfuegen
			// TODO: Winkel ?
			DEBUG5("placing group %s at %f %f",jt->second.m_group_name.c_str(), pos.m_x, pos.m_y);
			mdata->m_region->createObjectGroup(jt->second.m_group_name,pos,0);
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


			if (*(mdata->m_base_map->ind(i,j)) >=1 )
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
					if (exit[k][0]*nb[k][0] + exit[k][1]*nb[k][1] == i*nb[k][0] + j*nb[k][1] && Random::randi(exitcount[k]) ==0)
					{
						exit[k][0] =i;
						exit[k][1] =j;
						exitcount[k] ++;
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


	// fuer jedes markierte werden die 4 Nachbarfelder angesehen
	// die 4 Informationen werden als 4bit Zahl interpretiert
	// es gibt 6 verschiedene Konstellationen fuer die Nachbarfelder
	// die 4bit Zahl dient als Index um die Konstellation und den Drehwinkel zu erhalten

	// Liste mit den 6 verschiedenen Moeglichenkeiten welche Nachbarfelder besetzt sind
	std::string borders[6] = {"border(side)","border(corner)","border(twoside)","border(twocorner)","border(filled)","border(single_block)"};

	// Maske dafuer, welche Situation vorliegt
	int bmask[16]={4,0,0,2,0,1,1,3,0,1,1,3,2,3,3,5};

	// Maske fuer die Rotationswinkel in Schritten von 90 Grad
	int rotmask[16] = {0,0,2,0,3,0,3,0,1,1,2,2,1,1,3,0};
	int mask =0;
	int nbi, nbj;

	ObjectGroupTemplateName templ;
	float angle;

	bool skip;

	Matrix2d<char> maskmap(hdimx,hdimy);
	maskmap.clear();

	for (int i=0; i<hdimx; i++)
	{
		for (int j=0; j< hdimy;j++)
		{
			skip = false;
			if (*(mdata->m_base_map->ind(i,j)) != -1)
			{
				continue;
			}

			mask =0;
			// Bitmaske aufbauen
			for (int k=0; k<4; k++)
			{
				nbi = i + nb[k][0];
				nbj = j + nb[k][1];

				mask *=2;
				if ( nbi<0 || nbj<0 || nbi>=hdimx || nbj>=hdimy)
				{
					continue;
				}

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

			if (skip)
				continue;

			// Template und dessen Winkel bestimmen
			templ = borders[bmask[mask]];
			angle = rotmask[mask] *PI/2;

			DEBUG5("placing type (%i %i) %i %i %s",i,j,mask, bmask[mask]+1, borders[bmask[mask]].c_str());
			mdata->m_region->createObjectGroup(templ,Vector(i*8+4,j*8+4),angle);
			//std::cout << "\n";

			*(maskmap.ind(i,j)) = bmask[mask]+1;

		}
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
	for (st = rdata->m_monsters.begin(); st != rdata->m_monsters.end(); ++st)
	{
		DEBUG5("%s x %i",st->m_monsters.c_str(),st->m_number);
		for (int i=0; i< st->m_number; i++)
		{
			int r = Random::randi(points.size());
			wo = new Spawnpoint(st->m_monsters, World::getWorld()->getValidId());

			mdata->m_region->insertObject(wo,Vector(points[r].first*8+4, points[r].second*8+4));

			DEBUG5("placing spawnpoint for %s at %i %i",st->m_monsters.c_str(), points[r].first*8+4, points[r].second*8+4);

			points[r] = points.back();
			points.resize(points.size() -1);

		}
	}
}


void MapGenerator::createExits(MapData* mdata, RegionData* rdata)
{
	std::list<RegionExit>::iterator it;
	Vector pos;
	for (it = rdata->m_exits.begin(); it != rdata->m_exits.end(); ++it)
	{
		mdata->m_region->addExit(*it);

		// Bei den Ausgaengen keine Monster
		pos = mdata->m_region->getLocation(it->m_exit_name);
		*(mdata->m_base_map->ind(int(pos.m_x/8), int(pos.m_y/8))) = 2;

	}
}

bool MapGenerator::getTemplatePlace(MapData* mdata, Shape* shape, Vector & place)
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
	float* weightx = new float[dimx];
	float* weighty = new float[dimy];
	int freqy,px,py;
	float ampl =1;

	// Perlin Noise Algorithmus
	for (int freq = startfreq; freq <= dimx;freq*=2)
	{
		freqy = (freq * dimy)/dimx;

		// anlegen der Zufallszahlen fuer die aktuelle Frequenz
		for (int i=0;i<=freq;i++)
		{
			for (int j=0;j<=freqy;j++)
			{
				*(tmp->ind(i,j)) = Random::random()* ampl;

			}

		}

		// Interpolation
		px = dimx/freq;
		float invpx = 1.0/ px;
		py = dimy/freqy;
		float invpy = 1.0/ py;

		for (int k=0;k<px;k++)
		{
			weightx[k] = k*invpx;
		}

		for (int l=0;l<py;l++)
		{
			weighty[l] = l*invpy;
		}

		for (int i=0;i<freq;i++)
		{
			for (int j=0;j<freqy;j++)
			{
				for (int k=0;k<px;k++)
				{
					for (int l=0;l<py;l++)
					{

						*(data->ind(i*px+k,j*py+l)) += (*(tmp->ind(i,j))*(1-weightx[k])*(1-weighty[l])
								+ *(tmp->ind(i+1,j))*weightx[k]*(1-weighty[l])
								+ *(tmp->ind(i+1,j+1))*weightx[k]*weighty[l]
								+ *(tmp->ind(i,j+1))*(1-weightx[k])*weighty[l]) ;
					}
				}
			}
		}


		ampl *= persistance;
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
		float bnd = 5;
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

	delete[] weightx;
	delete[] weighty;
	delete tmp;
}



