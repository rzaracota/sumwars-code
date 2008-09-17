

#include "mapgenerator.h"

using namespace std;




 
	

void MapGenerator::drawMap(list<MapArea*>* tmp)
{
	int i,j;
	int map[MAPSIZE][MAPSIZE];
	MapArea* ma;
	
	for (i=0;i<MAPSIZE;i++)
		for (j=0;j<MAPSIZE;j++)
			map[i][j]=-1;
	
	list<MapArea*>::iterator iter;
	for (iter=tmp->begin();iter!=tmp->end();iter++)
	{
		ma = *iter;
		map[(ma->x+MAPSIZE*10000)%MAPSIZE][(ma->y+MAPSIZE*10000)%MAPSIZE]=ma->content;
	}
			
	cout << "\n\n";
	for (i=0;i<MAPSIZE;i++)
	{	
		cout << "|";
		for (j=0;j<MAPSIZE;j++)
			{
				switch(map[j][i])
				{
					case -1: cout << "  "; break;
					case 0: cout << ".."; break;
					case 1: cout << "##"; break;
					case 2: cout << "^"; break;
					case 3: cout << "v"; break;
				}
				//cout << f.margin_dist[0] << f.margin_dist[1] << f.margin_dist[2] << f.margin_dist[3] ;
				//cout << f.type;
			}
		cout << "|\n"; 
	}
	cout << "\n\n";
}



// Zeigt den Dungeon in Konsolengraphik an
void MapGenerator::showDungeon(QuadTree<MapArea>* dungeonmap)
{
	int x=0, y=0;
	char c=' ';
	list<MapArea*>* tmp = new list<MapArea*>;
	
	while (c != '0')
	{
		tmp->clear();
		dungeonmap->getElementsInRect(x*MAPSIZE,y*MAPSIZE,(x+1)*MAPSIZE-1,(y+1)*MAPSIZE-1,NULL,NULL,tmp);
		cout << "Koordinaten (" <<x<<","<<y<<")\n";
		if (tmp->empty())
		{
			cout << "keine Daten\n\n";
		}
		else
			drawMap(tmp);
		cout << "2,4,6,8 fuer nach sued,west, ost, nord\n";
		cout << "+,- fuer nach oben, nach unten\n";
		cout << "0 fuer ende\n\n";
		cin >> c;
		if (c=='6')
			x++;
		if (c=='4')
			x--;
		if (c=='2')
			y++;
		if (c=='8' )
			y--;
		/*if (c=='+')
			z++;
		if (c=='-' )
			z--;
		*/
		
	}
}



MapArea* MapGenerator::getField(QuadTree<MapArea>* dungeonmap, int x, int y, bool expand)
{

	static int counter =10;
	MapArea* ret = dungeonmap->getElementAt(x,y);
	MapArea* ma;
	if (expand && ret==0)
	{
		ret= new MapArea(x,y,counter++);
		dungeonmap->insertElement(ret);
		
	}
	
	
	if (expand)
	{
		Field* f;
		for (int i=-1;i<=1;i++)
		{
			for (int j=-1;j<=1;j++)
			{
				if (dungeonmap->getElementAt(x+i,y+j)==0)
				{
					ma = new MapArea(x+i,y+j,counter++);
					dungeonmap->insertElement(ma);
				}
			}
		}
	}
	
	return ret;
}
 
// Berechnet im Dungeon Einzelkarte an Position x,y,z
// markiert alle Karten zu denen Wege fuehren als zu bearbeiten
// schiebt neue Karten in die Taskliste
// n Anzahl der bisher bearbeiteten Karten
// max Obergrenze fuer die Anzahl 
  void MapGenerator::calcDungeon(QuadTree<MapArea>* dungeonmap,int max )
{

	char c;
		
	// Queue fuer die Felder
	queue<Coords>* coordq = new queue<Coords>;
	
	// testen der freien Richtungen
	// finden der Eingangspunkte in die Karte
	// 0..5 = nord, ost, sued, west, oben, unten
	
	
	
	
	// Einbauen  der Stadt als Startpunkt
	int i,j;
	MapArea* field;
	
	for (i=0;i<5;i++)
		for(j=0;j<5;j++)
		{
			field=getField(dungeonmap,i,j);
			if (i % 4 ==0 || j%4==0)
				field->content=1;
			else
				field->content=0;
			field->town = true;
		}
		field=getField(dungeonmap,2,3);	
		field->towncenter = true;
		field->trader = true;
		field=getField(dungeonmap,2,2);
		field->trader = true;
		
		field=getField(dungeonmap,4,2);	
	field->content=0;
	field->dist=1;
	
	Coords coord;
	coord.x=4;
	coord.y=2;
	coordq->push(coord);
	
	
	
	//return;
	// Berechnen der Karte
	
	int nf, nb,todo,t,nfges ;
	int adj[4][2];
	int xf,yf,xa,ya;
	float prob,psum;
	int dist;
	int counter =0;
	
	
	// Solange es noch abzuarbeitende Felder gibt
	while (!coordq->empty())
	{
		nf=0;
		nb=0;
		todo =0;
		
		// Feld aus der Queue holen
		coord = coordq->front();
		xf = coord.x;
		yf = coord.y;
		dist=coord.dist;
		coordq->pop();
		
		//cout << "calc field: " << xf << " " << yf <<"\n";
		
		// benachbarte Felder absuchen
		// Anzahl freie und blockierte zaehlen
		// noch zu belegende finden
		for (i=0;i<=3;i++)
		{
			xa = xf + direction[i][0];
			ya = yf + direction[i][1];
			
			//cout << "view neighbour: " << xa << " " << ya <<"\n";
			
			
			field = getField(dungeonmap,xa,ya);
			t=field->content;
			if (t==-1)
			{
				adj[todo][0]=xa;
				adj[todo][1]=ya;
				todo++;
			}
			else
			{
				if (t==1)
				{
					nb++;
				}
				else
				{
					nf++;
				}
			}
		}
		// betrachten der Nachbarfelder beendet
		
		
		if (todo==0)
			continue;
		
		// Berechnen wie viele Felder insgesamt frei werden sollen
		psum =0;
		prob = (rand()*1.0) / (RAND_MAX);
		
		nfges = min(nf-1,0);
		
		
		while (prob>psum && (nfges-nf)< todo)
		{
			
			psum += FIELDPROB[nf][nb][nfges];
			//cout <<"psum nfges " << psum <<" "<<nfges<<"\n";
			nfges++;
		}
		
		
		// Diese Zeile sorgt dafür, dass der letzte Weg nicht geschlossen wird
		// solange weniger als max/2 Karten gezeichnet sind
		if (coordq->empty() && nfges==nf && counter<(max/2))
			nfges++;
		
		if (counter>max)
			nfges=nf;
		
		
		//cout << "nf nb nfges todo " << nf <<" "<<nb << " "<<nfges<<" "<<todo<<"\n";
		//cout << "prob "<<prob<< "\n";
		
		// Felder eintragen
		i=4-nf-nb;
		
		while (i>0)
		{
			j = (rand() % i);
			xa = adj[j][0];
			ya = adj[j][1];
			field = getField(dungeonmap,xa,ya);
			if (i<=nfges-nf)
			{
				// Feld als frei eintragen
				
				field->content =0;
				field->dist=dist;
				
				// in die Queue schieben
				coord.x=xa;
				coord.y=ya;
				coord.prev_x =xf;
				coord.prev_y =yf;
				coord.dist = dist+1;
				coordq->push(coord);
				
			}
			else
			{
				// Feld als blockiert eintragen
				field->content=1;
			}
			
			adj[j][0]=adj[i-1][0];
			adj[j][1]=adj[i-1][1];
			
			i--;
		}
		
		counter++;
		
//  		//showDungeon(dungeonmap);
	}
	// Ende while Schleife leeren der Queue
	delete coordq;
}

void MapGenerator::turn(int &x, int &y,int t)
{
	int tmp;
	if (t==1)
	{
		tmp = x;
		x =(FIELD_STRETCH-1-y);
		y=tmp;
	}
	if (t==2)
	{
		x =(FIELD_STRETCH-1-x);
		y =(FIELD_STRETCH-1-y);
	}
	if (t==3)
	{
		tmp = y;
		y =(FIELD_STRETCH-1-x);
		x=tmp;
	}
}	

 bool MapGenerator::check(int x,int y,World* world)
{
	bool b[8];
	int n[8][2]={{-1,-1},{0,-1},{1,-1},{-1,0},{1,0},{-1,1},{0,1},{1,1}};
	WorldObject* wo;
	for (int i=0;i<=7;i++)
	{
		wo = world->getWorldObjectAt((float)x+n[i][0],(float) y+n[i][1]);
		if (wo!=0)
		{
			b[i]=false;
		}
		else
		{
			b[i]=true;
		}
	}
	
	/*
	if (x == 195 && y == -19)
	{
		printf("%d %d\n",x,y);
		printf("%d%d%d\n",(int) b[0],(int) b[1],(int) b[2]);
		printf("%d*%d\n",(int) b[3],(int) b[4]);
		printf("%d%d%d\n",(int) b[5],(int) b[6],(int) b[7]);
			
	}
	*/
	bool ret1 = (b[1] && b[2] && b[0]) || (b[6] && b[7] && b[5]) || !(b[1] || b[2] || b[0]) || !(b[6] || b[7] || b[0]);
	bool ret2 =	(b[0] && b[3] && b[5]) || (b[2] && b[4] && b[7]) || !(b[0] || b[3] || b[5]) || !(b[2] || b[4] || b[7]);
	return ret1 & ret2;
}
	
	
void MapGenerator::insertElement(World* world,int i, int cx, int cy,int x, int y,int (*obj)[FIELD_STRETCH])
{
	static int ind=100;
	float nx = x*FIELD_STRETCH+cx;
	float ny = y*FIELD_STRETCH+cy;
	obj[cx][cy]=i;
	WorldObject* wo;
	if (i<=11)
		 wo = new FixedObject(world,nx,ny,ind++,IND_OBJECTS[i] );
	else
	{
		// Brunnen
		if (i==FOUNTAIN)
		{
			Fountain* f = new Fountain(world,nx,ny,ind++);
			f->init(1);	// Level
			wo = f;
		}
		
		// Spawnpoint
		if (i==SPAWNPOINT)
		{
			Spawnpoint* s= new Spawnpoint(world,nx,ny,ind++);
			s->init(SUBTYPE_GOBLIN,1,2,10,1000);	
			wo=s;
		}
		
		// Kiste
		if (i==CHEST)
		{
			Chest* c = new Chest(world,nx,ny,ind++,60000);
			c->setLevel(1);
			wo = c;
		}
		
		if (i==TRADER)
		{
			Trader* tr = new Trader(world, nx,ny,ind++,SUBTYPE_UNIVERSAL_TRADER);
			wo = tr;
		}
	}
	world->insertWorldObject(wo);
}

void MapGenerator::stretchDungeon(QuadTree<MapArea>* dungeonmap, World* world)
{	
	int i,j,ki,kj,k;
	int n,sum;
	int x,y;
	int a=1,u,v[4];
	int ind=100;
	list<MapArea*>::iterator iter;
	list<MapArea*>* fieldlist= new list<MapArea*>;
	dungeonmap->getElements(fieldlist);
	MapArea* field;
	MapArea* field2;
	// Fuer alle Teilkarten
	for (iter=fieldlist->begin();iter!=fieldlist->end();iter++)
	{
		field = (*iter);
		x=field->x;
		y=field->y;
		
		DEBUG4("calc field %i %i\n",x,y);
		sum=0;
		n=1;
		
		// Berechnen des Wertes des anliegenden 2x2 Feldes
		for (ki=0;ki<=1;ki++)
			for(kj=0;kj<=1;kj++)
			{
				field2 = getField(dungeonmap,x+kj,y+ki,false);
				if (field2)
					sum += (field2->content +1)*n;
				n *=3;
				
			}
		int cx,cy;
		// Wert!=0 wenn nicht alle vier Felder leer
		if (sum !=0)
		{
			// Berechnen ob an der Stelle Begrenzungen gezogen werden muessen
			field->sum=sum;
			//printf("calc %i %i res: %i\n",x,y,sum);
			field->type = SUM_TYPE[sum];
			field->turn=SUM_TURN[sum];

			// Wenn Begrenzungen gezogen werden muessen
			if (field->type!=0)
			{
				//field->margin_dist[0]=1+rand() % (FIELD_STRETCH-2);
				u=-1;
				
				// Fuer alle vier Richtungen den Austrittspunkt der Kanten bestimmen
				for (k=0;k<4;k++)
				{
					v[k]=0;
					field2 = getField(dungeonmap,x+direction[k][0],y+direction[k][1],false);
					//if (field2)
						//printf("neigh %i %i,  ntype %i type %i dir %i turn %i \n",x+direction[k][0],y+direction[k][1],field2->type,field->type,k,field->turn);
					if (field2 !=0 && field2->type!=0 && TYPE_DIR[field->type][(k+4-field->turn)%4]==1)
					{
						field->margin_dist[k]=field2->margin_dist[(k+2)%4];
						u=field->margin_dist[k];
						//printf( "copying dir %i\n",k);
					}
					else
					{
						v[k]=1;
						
						field->margin_dist[k]=1+rand() %(FIELD_STRETCH-2);
						//field->margin_dist[k] =a;
						//a=(a+1)%10;
					}
				}
				if (u!=-1 && field->type==2)
				{
					for (k=0;k<4;k++)
					{
						if (v[k]==1)
						{
							field->margin_dist[k]=u;
						}
					}
				}
				
			}
			// Ende der Berechnung der Begrenzungen
			
			int obj[FIELD_STRETCH][FIELD_STRETCH];
			for (i=0;i<FIELD_STRETCH;i++ )
				for (j=0;j<FIELD_STRETCH;j++ )
				{
					obj[i][j]=-1;
				}
			
				
			// Begrenzungen zeichnen
			DEBUG4("Zeichnen der Begrenzung %i",field->type);
			/*for (int l=0;l<4;l++)
			{
				cout << field->margin_dist[l] << " ";
			}
			cout << "\n";*/
			if (field->type==-1)
			{
				for (i=0;i<	FIELD_STRETCH;i++)
					for (j=0;j<=FIELD_STRETCH;j++)
					{	
						obj[i][j]=0;
					}	
			}
			
			if (field->type==1 || field->type==3)
			{
				// Ecke
				
				int b1 = field->margin_dist[field->turn];
				int b2= field->margin_dist[(3+field->turn)%4];
				//cout << b1<<b2<<"\n";
				if (field->turn==1 || field->turn==2)
					b2 =(FIELD_STRETCH-1-b2);
				if (field->turn>1)
					b1 =(FIELD_STRETCH-1-b1);
				
				
				//cout << b1<<b2<<"\n";
				// Waagerechte
				for (k=0;k<b1;k++)
				{
					cx=k;
					cy=b2;
					turn(cx,cy,field->turn);
					insertElement(world,1+(1+field->type+field->turn)%4,cx,cy,x,y,obj);
					
				}
				
				// Ecke
				cx=b1;
				cy=b2;
				turn(cx,cy,field->turn);
				insertElement(world,5+(2+field->turn)%4,cx,cy,x,y,obj);
				
				// Senkrechte
				for (k=0;k<b2;k++)
				{
					cx=b1;
					cy=k;
					turn(cx,cy,field->turn);
					insertElement(world,1+(2+field->type+field->turn)%4,cx,cy,x,y,obj);
				}
				
				// betretbare Felder festlegen
				if (field->type==1)
				{
					for (i=b1+1;i<	FIELD_STRETCH;i++)
						for (j=0;j<=b2;j++)
						{	
							cx=i;
							cy=j;
							turn(cx,cy,field->turn);
							obj[cx][cy]=0;
						}
					
					for (i=0;i<FIELD_STRETCH;i++)
						for (j=b2+1;j<FIELD_STRETCH;j++)
						{
							cx=i;
							cy=j;
							turn(cx,cy,field->turn);
							obj[cx][cy]=0;
						}
				}
				else
				{
					for (i=0;i<	b1;i++)
						for (j=0;j<b2;j++)
						{	
							cx=i;
							cy=j;
							turn(cx,cy,field->turn);
							obj[cx][cy]=0;
						}
				}
				
			}
			
			
			

			if (field->type==2)
			{
				// Gerade
				int b1 = field->margin_dist[(1+field->turn)%4];
				int b2 = field->margin_dist[(3+field->turn)%4];
				
				if (field->turn==1 || field->turn==2)
				{
					b1 =(FIELD_STRETCH-1-b1);
					b2 =(FIELD_STRETCH-1-b2);
				}
				
				if (b1 == b2)
				{
					
					// einfache Waagerechte
					for (k=0;k<FIELD_STRETCH;k++)
					{
						cx=k;
						cy=b1;
						turn(cx,cy,field->turn);
						insertElement(world,1+field->turn,cx,cy,x,y,obj);
					}
					// betretbare Felder festlegen
					for (i=0;i<FIELD_STRETCH;i++)
						for (j=b1+1;j<FIELD_STRETCH;j++)
						{	
							cx=i;
							cy=j;
							turn(cx,cy,field->turn);
							obj[cx][cy]=0;
						}
				}
				else
				{
					int c = rand() % FIELD_STRETCH;
					
					// Waagerechte 1
					for (k=0;k<c;k++)
					{
						cx=k;
						cy=b2;
						turn(cx,cy,field->turn);
						insertElement(world,1+field->turn,cx,cy,x,y,obj);
					}
					
					//Ecke 1
					cx=c;
					cy=b2;
					turn(cx,cy,field->turn);
					if (b1<b2)
						insertElement(world,5+(2+field->turn)%4,cx,cy,x,y,obj);
					else
						insertElement(world,5+(1+field->turn)%4,cx,cy,x,y,obj);
					
					// Senkrechte
					for (k=min(b1,b2)+1;k<max(b1,b2);k++)
					{
						cx=c;
						cy=k;
						turn(cx,cy,field->turn);
						if (b1<b2)
							insertElement(world,1+(1+field->turn)%4,cx,cy,x,y,obj);
						else
							insertElement(world,1+(3+field->turn)%4,cx,cy,x,y,obj);
					}
					
					//Ecke 2
					cx=c;
					cy=b1;
					turn(cx,cy,field->turn);
					if (b1<b2)
						insertElement(world,5+field->turn,cx,cy,x,y,obj);
					else
						insertElement(world,5+(3+field->turn)%4,cx,cy,x,y,obj);
					
					// Waagerechte 1
					for (k=c+1;k<FIELD_STRETCH;k++)
					{
						cx=k;
						cy=b1;
						turn(cx,cy,field->turn);
						insertElement(world,1+field->turn,cx,cy,x,y,obj);
					}
					
					
					// betretbare Felder festlegen
					for (i=0;i<c;i++)
						for (j=b2+1;j<FIELD_STRETCH;j++)
						{	
							cx=i;
							cy=j;
							turn(cx,cy,field->turn);
							if (obj[cx][cy]==-1)
							obj[cx][cy]=0;
						}
						
					for (i=c;i<	FIELD_STRETCH;i++)
						for (j=b1+1;j<FIELD_STRETCH;j++)
						{	
							cx=i;
							cy=j;
							turn(cx,cy,field->turn);
							if (obj[cx][cy]==-1)
							obj[cx][cy]=0;
						}
				}
			}
			
			
			

			if (field->type==4)
			{
				int b1 = field->margin_dist[(0+field->turn)%4];
				int b2 = field->margin_dist[(1+field->turn)%4];
				int b3 = field->margin_dist[(2+field->turn)%4];
				int b4 = field->margin_dist[(3+field->turn)%4];
				
				/*printf("bx: %i %i %i %i\n",b1,b2,b3,b4);
				printf("turn: %i\n",field->turn);
				*/
				if (field->turn==1 || field->turn==2)
				{
					b2 =(FIELD_STRETCH-1-b2);
					b4 =(FIELD_STRETCH-1-b4);
				}
				
				if (field->turn>1)
				{
					b1 =(FIELD_STRETCH-1-b1);
					b3 =(FIELD_STRETCH-1-b3);
				}
				
				
				int r=rand() %2;
				
				if (b1<b3 || b4<b2 || r==0)
				{
					// Waagerechte
					for (k=0;k<b1;k++)
					{
						cx=k;
						cy=b4;
						turn(cx,cy,field->turn);
						insertElement(world,1+(2+field->turn)%4,cx,cy,x,y,obj);
					
					}
				
					// Ecke
					cx=b1;
					cy=b4;
					turn(cx,cy,field->turn);
					insertElement(world,5+(2+field->turn)%4,cx,cy,x,y,obj);
				
					// Senkrechte
					for (k=0;k<b4;k++)
					{
						cx=b1;
						cy=k;
						turn(cx,cy,field->turn);
						insertElement(world,1+(3+field->turn)%4,cx,cy,x,y,obj);
					}
					
					// betretbare Felder festlegen
					for (i=0;i<b1;i++)
						for (j=0;j<b4;j++)
						{	
							cx=i;
							cy=j;
							turn(cx,cy,field->turn);
							obj[cx][cy]=0;
						}
					
				}
				else
				{
					int ex = rand() % b3;
					int ey = rand() % b2;
					
					// Waagerechte
					for (k=0;k<ex;k++)
					{
						cx=k;
						cy=b4;
						turn(cx,cy,field->turn);
						insertElement(world,1+(2+field->turn)%4,cx,cy,x,y,obj);
					
					}
				
					// Ecke
					cx=ex;
					cy=b4;
					turn(cx,cy,field->turn);
					insertElement(world,5+(2+field->turn)%4,cx,cy,x,y,obj);
					
					// Senkrechte
					for (k=b4-1;k>ey;k--)
					{
						cx=ex;
						cy=k;
						turn(cx,cy,field->turn);
						insertElement(world,1+(3+field->turn)%4,cx,cy,x,y,obj);
					}
					
					// Ecke
					cx=ex;
					cy=ey;
					turn(cx,cy,field->turn);
					insertElement(world,5+(0+field->turn)%4,cx,cy,x,y,obj);
					
					// Waagerechte
					for (k=ex+1;k<b1;k++)
					{
						cx=k;
						cy=ey;
						turn(cx,cy,field->turn);
						insertElement(world,1+(2+field->turn)%4,cx,cy,x,y,obj);
					
					}
					
					// Ecke
					cx=b1;
					cy=ey;
					turn(cx,cy,field->turn);
					insertElement(world,5+(2+field->turn)%4,cx,cy,x,y,obj);
					
					// Senkrechte
					for (k=0;k<ey;k++)
					{
						cx=b1;
						cy=k;
						turn(cx,cy,field->turn);
						insertElement(world,1+(3+field->turn)%4,cx,cy,x,y,obj);
					}
					
					// betretbare Felder festlegen
					for (i=0;i<	ex;i++)
						for (j=0;j<b4;j++)
						{	
							cx=i;
							cy=j;
							turn(cx,cy,field->turn);
							if (obj[cx][cy]==-1)
							obj[cx][cy]=0;
						}
						
					for (i=ex;i<b1;i++)
						for (j=0;j<ey;j++)
						{	
							cx=i;
							cy=j;
							turn(cx,cy,field->turn);
							if (obj[cx][cy]==-1)
								obj[cx][cy]=0;
						}
				}
				
				// zweiter Linienzug...
				
				if (b1<b3 || b4<b2 || r==1)
				{
					// Waagerechte
					for (k=FIELD_STRETCH-1;k>b3;k--)
					{
						cx=k;
						cy=b2;
						turn(cx,cy,field->turn);
						insertElement(world,1+(0+field->turn)%4,cx,cy,x,y,obj);
					
					}
				
					// Ecke
					cx=b3;
					cy=b2;
					turn(cx,cy,field->turn);
					insertElement(world,5+(0+field->turn)%4,cx,cy,x,y,obj);
				
					// Senkrechte
					for (k=FIELD_STRETCH-1;k>b2;k--)
					{
						cx=b3;
						cy=k;
						turn(cx,cy,field->turn);
						insertElement(world,1+(1+field->turn)%4,cx,cy,x,y,obj);
					}
					
					// betretbare Felder festlegen
					for (i=b3+1;i<FIELD_STRETCH;i++)
						for (j=b2+1;j<FIELD_STRETCH;j++)
						{	
							cx=i;
							cy=j;
							turn(cx,cy,field->turn);
							obj[cx][cy]=0;
						}
				}
				else
				{
					int ex = FIELD_STRETCH-1-rand() % (FIELD_STRETCH-b1-1);
					int ey = FIELD_STRETCH-1-rand() % (FIELD_STRETCH-b4-1);
					
					// Waagerechte
					for (k=FIELD_STRETCH-1;k>ex;k--)
					{
						cx=k;
						cy=b2;
						turn(cx,cy,field->turn);
						insertElement(world,1+(0+field->turn)%4,cx,cy,x,y,obj);
					
					}
				
					// Ecke
					cx=ex;
					cy=b2;
					turn(cx,cy,field->turn);
					insertElement(world,5+(0+field->turn)%4,cx,cy,x,y,obj);
					
					// Senkrechte
					for (k=b2+1;k<ey;k++)
					{
						cx=ex;
						cy=k;
						turn(cx,cy,field->turn);
						insertElement(world,1+(1+field->turn)%4,cx,cy,x,y,obj);
					}
					
					// Ecke
					cx=ex;
					cy=ey;
					turn(cx,cy,field->turn);
					insertElement(world,5+(2+field->turn)%4,cx,cy,x,y,obj);
					
					// Waagerechte
					for (k=b3+1;k<ex;k++)
					{
						cx=k;
						cy=ey;
						turn(cx,cy,field->turn);
						insertElement(world,1+(0+field->turn)%4,cx,cy,x,y,obj);
					
					}
					
					// Ecke
					cx=b3;
					cy=ey;
					turn(cx,cy,field->turn);
					insertElement(world,5+(0+field->turn)%4,cx,cy,x,y,obj);
					
					// Senkrechte
					for (k=FIELD_STRETCH-1;k>ey;k--)
					{
						cx=b3;
						cy=k;
						turn(cx,cy,field->turn);
						insertElement(world,1+(1+field->turn)%4,cx,cy,x,y,obj);
					}
					
					// betretbare Felder festlegen
					for (i=b3+1;i<	ex;i++)
						for (j=ey+1;j<FIELD_STRETCH;j++)
						{	
							cx=i;
							cy=j;
							turn(cx,cy,field->turn);
							if (obj[cx][cy]==-1)
								obj[cx][cy]=0;
						}
						
					for (i=ex;i<FIELD_STRETCH;i++)
						for (j=b2+1;j<FIELD_STRETCH;j++)
						{	
							cx=i;
							cy=j;
							turn(cx,cy,field->turn);
							if (obj[cx][cy]==-1)
							obj[cx][cy]=0;
						}
					
				}

				
			}
			
			/*
			printf("type %d\n",field->type);
			for (int ii=FIELD_STRETCH-1;ii>=0;ii--)
			{
				for (int jj=0;jj<FIELD_STRETCH;jj++)
					printf("%2d",obj[jj][ii]);
				printf("\n");	
			}
			printf("\n");
			}
			*/
			
			int nx,ny;
			// Towncenter, Brunnen einfuegen
			if (field->towncenter)
			{
				i=0;
				do
				{
					nx=rand()% FIELD_STRETCH;
					ny=rand()% FIELD_STRETCH;
					i++;
				}
				while (i<1000 && (obj[nx][ny]!=0 || !check(x*FIELD_STRETCH+nx,y*FIELD_STRETCH+ny,world)));
				
				if (i<1000)
				{
					insertElement(world,TOWNCENTER,nx,ny,x,y,obj);
				}
				
				i=0;
				do
				{
					nx=rand()% FIELD_STRETCH;
					ny=rand()% FIELD_STRETCH;
					i++;
				}
				while (i<1000 && (obj[nx][ny]!=0 || !check(x*FIELD_STRETCH+nx,y*FIELD_STRETCH+ny,world)));
				
				if (i<1000)
				{
					insertElement(world,FOUNTAIN,nx,ny,x,y,obj);
				}
				
			}
			
			if (field->type !=0 && !field->town && rand()*1.0 / RAND_MAX < FOUNTAIN_PROB[field->type])
			{
				i=0;
				do
				{
					nx=rand()% FIELD_STRETCH;
					ny=rand()% FIELD_STRETCH;
					i++;
				}
				while (i<3 && (obj[nx][ny]!=0 || !check(x*FIELD_STRETCH+nx,y*FIELD_STRETCH+ny,world)));
				
				if (i<3)
				{
					insertElement(world,FOUNTAIN,nx,ny,x,y,obj);
				}
			}

			if (field->trader)
			{
				i=0;
				do
				{
					nx=rand()% FIELD_STRETCH;
					ny=rand()% FIELD_STRETCH;
					i++;
				}
				while (i<1000 && (obj[nx][ny]!=0 || !check(x*FIELD_STRETCH+nx,y*FIELD_STRETCH+ny,world)));
				
				if (i<1000)
				{
					insertElement(world,TRADER,nx,ny,x,y,obj);
				}
			};
				
			// Kisten einfuegen
			if (field->type!=0 && !field->town && rand()*1.0 / RAND_MAX < CHEST_PROB[field->type])
			{
				i=0;
				do
				{
					nx=rand()% FIELD_STRETCH;
					ny=rand()% FIELD_STRETCH;
					i++;
				}
				while (i<3 && (obj[nx][ny]!=0 || !check(x*FIELD_STRETCH+nx,y*FIELD_STRETCH+ny,world)));
				
				if (i<3)
				{
					insertElement(world,CHEST,nx,ny,x,y,obj);
				}
			}
			
			//Spawnpoint einfuegen
			if (field->type!=0 && !field->town && rand()*1.0 / RAND_MAX < SPAWNPOINT_PROB[field->type])
			{
				i=0;
				do
				{
					nx=rand()% FIELD_STRETCH;
					ny=rand()% FIELD_STRETCH;
					i++;
				}
				while (i<10 && (obj[nx][ny]!=0 || !check(nx,ny,world)));
				
				if (i<10)
				{
					insertElement(world,SPAWNPOINT,nx,ny,x,y,obj);
				}
				
				for (i=0;i<4;i++)
				{
					nx=rand()% FIELD_STRETCH;
					ny=rand()% FIELD_STRETCH;
					if (rand()*1.0/RAND_MAX < TOTEM_PROB && obj[nx][ny]==0 && check(x*FIELD_STRETCH+nx,y*FIELD_STRETCH+ny,world))
						 insertElement(world,TOTEM,nx,ny,x,y,obj);
				}
			}
			
			char dummy;
			float p = TREE_PROB[0];
			if (field->town)
				p= TREE_PROB[1];
			// Baueme einfuegen
			for (i=0;i<FIELD_STRETCH*FIELD_STRETCH;i++)
			{
				nx=rand()% FIELD_STRETCH;
				ny=rand()% FIELD_STRETCH;
				if ((rand()*1.0)/RAND_MAX < p && obj[nx][ny]<=0 && check(x*FIELD_STRETCH+nx,y*FIELD_STRETCH+ny,world))
				{	
					
					insertElement(world,TREE,nx,ny,x,y,obj);
					
					//printf("%d %d\n",nx,ny);
					if (x*FIELD_STRETCH+nx==195 && y*FIELD_STRETCH+ny==-19 && false)
					{
						for (int ii=FIELD_STRETCH;ii>=0;ii--)
						{
							for (int jj=0;jj<FIELD_STRETCH;jj++)
								printf("%2d",obj[jj][ii]);
							printf("\n");	
						}
						//cin >> dummy;
					}	
					
				}
			}
		}
		// Ende der Berechnung der relevanten Felder
	}
}

void MapGenerator::createMap(World* world)
{
	long sec;
	time(&sec);
	//printf("time %i \n",sec);
	//srand(sec);			// Zufallszahlen initialisieren;
	srand(1192953949);
	QuadTree<MapArea>* dungeonmap = new QuadTree<MapArea>(0,0,1,1);
	int max = 100; // Anzahl Felder
	calcDungeon(  dungeonmap,max);
	stretchDungeon(dungeonmap,world);
	
	//showDungeon(dungeonmap);
	delete dungeonmap;
}

/*int main()
{
	
	return 0;
}*/


