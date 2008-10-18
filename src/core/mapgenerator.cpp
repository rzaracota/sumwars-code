#include "mapgenerator.h"



bool operator<(WeightedLocation first, WeightedLocation second)
{
	return first.m_value< second.m_value;
}



void MapGenerator::createMapData(MapData* mdata, RegionData* rdata)
{
	mdata->m_base_map = new Matrix2d<char>(rdata->m_dimx,rdata->m_dimy);
	mdata->m_environment = new Matrix2d<float>(rdata->m_dimx,rdata->m_dimy);

}

void MapGenerator::createBaseMap(MapData* mdata, RegionData* rdata)
{
	float size = rdata->m_area_percent;
			
	int dimx = rdata->m_dimx;
	int dimy = rdata->m_dimy;
	
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
	
	//std::cout << "area percent: "<<cnt*1.0/(dimx*dimy)<<"\n";
	
	// nochmal 5 Runden Floodfill um den Rand zu ermitteln
	
	// Marker einfuegen
	int markercnt =0;
	borderqu.push(std::make_pair(-1,-1));
	
	
	while (!borderqu.empty() && markercnt <5)
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
	createPerlinNoise(mdata->m_environment, dimx, dimy,4 , 0.4,false);
}

void MapGenerator::createBorder(MapData* mdata, RegionData* rdata)
{
	
	// Delta zu Nachbarfeldern
	int nb[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
	int dnb[8][2] = {{-1,0},{1,0},{0,-1},{0,1},{-1,1},{1,1},{1,-1},{-1,-1}};
	
	// Matrix mit 8x8 Felder, Felder werden markiert, 
	Matrix2d<char> bmap(rdata->m_dimx/2, rdata->m_dimy/2);
	
	//for (int i=0; i< rdata->m_dimx
}

void MapGenerator::createPerlinNoise(Matrix2d<float> *data, int dimx, int dimy,int startfreq, float persistance, bool bounds)
{
	Matrix2d<float>* tmp= new Matrix2d<float>(dimx+1, dimy+1);
	tmp->clear();
	float* weightx = new float[dimx];
	float* weighty = new float[dimy];
	int freqy,px,py,x,y;
	float ampl =1;
	float amp = ampl;
	
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
		float bnd = 0.1;
		float dist;
		for (int i=0;i<dimx;i++)
		{
			for (int j=0;j<dimy;j++)
			{
				dist = (bnd + std::max(fabs(i-dimx/2)/dimx,fabs(j-dimy/2)/dimy)-0.5);
				if (dist>0)
				{
					*(data->ind(i,j)) = *(data->ind(i,j))*(bnd-dist)/bnd + 1*dist/bnd;
				}
			}
		}
	}
	
	delete weightx;
	delete weighty;
	delete tmp;		
}



