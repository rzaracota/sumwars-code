#include "projectile.h"

Projectile::Projectile(World* w,ProjectileType type, WorldObject::TypeInfo::Fraction fr, int id)
{
	m_id = id;
	m_type = type;
	m_timer =0;
	m_geometry.m_layer = WorldObject::Geometry::LAYER_AIR;
	
	m_world =w;
	m_last_hit_object_id =0;
	m_flags =0;
	m_state = FLYING;
	m_timer_limit = 1000;
	m_counter =0;
	m_goal_object =0;


	float r=0.1;
	// Radius, Timerlaufzeit und Status je nach Typ setzen
	switch(type)
	{
		case FIRE_BOLT: r=0.1; break;
		case ICE_BOLT: r=0.1; break;
		case LIGHTNING:
			r=0.15;
			m_state=STABLE;
			m_timer_limit = 250;
			break;
		case FIRE_BALL:
			r=0.2;
			m_max_radius = 1.5;
			m_flags = EXPLODES;
			break;
		case FIRE_WALL:
			r=0;
			m_state=GROWING;
			m_timer_limit = 200;
			m_max_radius = 2;
			break;
		case FIRE_WAVE:
			r=0;
			m_state=GROWING;
			m_timer_limit = 1000;
			m_max_radius = 7;
			break;
		case ICE_RING:
			r=0;
			m_state=GROWING;
			m_timer_limit = 800;
			m_max_radius = 4;
			break;
		case FREEZE:
			r=4;
			m_state=STABLE;
			m_timer_limit = 300;
			break;
		case BLIZZARD:
			r=3;
			m_state=STABLE;
			m_timer_limit = 500;
			break;
		case THUNDERSTORM:
			r=5;
			m_state=STABLE;
			m_timer_limit = 300;
			break;
		case CHAIN_LIGHTNING:
			r=0.15;
			m_flags = BOUNCING;
			break;
		case STATIC_SHIELD:
			r=1.5;
			m_state=STABLE;
			m_timer_limit = 150;
			case ARROW: r=0.1; break;
		case GUIDED_ARROW:
			r=0.1;
			m_timer_limit = 1000;
			break;
		case EXPLOSION:
		case ICE_EXPLOSION:
		case WIND_EXPLOSION:
		case FIRE_EXPLOSION:
			m_timer_limit = 200;
			m_state=EXPLODING;
			break;

		case LIGHT_BEAM:
		case DIVINE_BEAM:
		case ELEM_EXPLOSION:
		case ACID:
		case HYPNOSIS:
			r=0.2;
			m_timer_limit = 200;
			m_state=STABLE;
			break;

		default:
			break;
	}


	m_geometry.m_radius =r;
	m_creator_fraction = fr;

}

bool Projectile::update(float time)
{
	DEBUG5("projektile type %i coord %f %f time %f , %f/ %f", m_type,getGeometry()->m_coordinate_x,getGeometry()->m_coordinate_y,time,m_timer, m_timer_limit);
	DEBUG5("speed %f %f",m_speed_x,m_speed_y);

	// Zeit die beim aktuellen Durchlauf der Schleife verbraucht wird
	float dtime;
	// true, wenn Timer die Grenze erreicht
	bool lim = false;
	// Liste der getroffenen Objekte
	std::list<WorldObject*> hitobj;
	std::list<WorldObject*>::iterator i;
	WorldObject* hit;

	// aktuelle Position
	float x;
	float y;
	// zuletzt getroffenes Objekt (ID)
	float dir[2],d;
	while (time>0)
	{
		hitobj.clear();
		dtime = time;
		x = getGeometry()->m_coordinate_x;
		y = getGeometry()->m_coordinate_y;

		// Timer erhoehen
		if (dtime > m_timer_limit-m_timer)
		{
			// Grenze erreicht
			lim = true;
			dtime =m_timer_limit-m_timer;
			m_timer = m_timer_limit;
		}
		else
		{
			m_timer += dtime;
		}
		time -= dtime;


		switch (m_state)
		{
			case FLYING:
				// Status fliegend behandeln
				handleFlying(dtime);

				break;

			case EXPLODING:
				DEBUG5("exploding");
				// Radius vergroessern
				m_geometry.m_radius += (m_max_radius)*dtime/(m_timer_limit);

				// Wenn Timer Limit erreicht
				if (m_timer >= m_timer_limit)
				{
					m_state = DESTROYED;

					if (m_world->isServer())
					{
						Shape s;
						s.m_coordinate_x = x;
						s.m_coordinate_y = y;
						s.m_type = Shape::CIRCLE;
						s.m_radius = m_geometry.m_radius;

						// Alle Objekte im Explosionsradius suchen
						hitobj.clear();
						m_world->getObjectsInShape(&s,m_region,&hitobj,getGeometry()->m_layer,WorldObject::CREATURE,0);
						for (i=hitobj.begin();i!=hitobj.end();++i)
						{
							// Schaden austeilen
							(*i)->takeDamage(&m_damage);
						}

						if (m_flags & MULTI_EXPLODES)
						{
							// Flag mehrfach explodierend gesetzt
							DEBUG("multiexploding");
							d= 1/sqrt(sqr(m_speed_x)+sqr(m_speed_y));
							dir[0] = m_speed_x * d;
							dir[1] = m_speed_y * d;

							// Schaden halbieren
							Damage dmg;
							memcpy(&dmg,&m_damage,sizeof(Damage));
							for (int i=0;i<4;i++)
							{
								dmg.m_multiplier[i] *= 0.5;
							}

							// vier neue Projektile erzeugen
							Projectile* pr;
							pr = new Projectile(m_world,m_type,m_creator_fraction, m_world->getValidProjectileId());
							memcpy(pr->getDamage(),&dmg,sizeof(Damage));
							pr->setFlags(Projectile::EXPLODES);
							pr->setMaxRadius(1);
							m_world->insertProjectile(pr,x+dir[0]*s.m_radius,y+dir[1]*s.m_radius,m_region);

							pr = new Projectile(m_world,m_type,m_creator_fraction, m_world->getValidProjectileId());
							memcpy(pr->getDamage(),&dmg,sizeof(Damage));
							pr->setFlags(Projectile::EXPLODES);
							pr->setMaxRadius(1);
							m_world->insertProjectile(pr,x-dir[1]*s.m_radius,y+dir[0]*s.m_radius,m_region);

							pr = new Projectile(m_world,m_type,m_creator_fraction, m_world->getValidProjectileId());
							memcpy(pr->getDamage(),&dmg,sizeof(Damage));
							pr->setFlags(Projectile::EXPLODES);
							pr->setMaxRadius(1);
							m_world->insertProjectile(pr,x-dir[0]*s.m_radius,y-dir[1]*s.m_radius,m_region);

							pr = new Projectile(m_world,m_type,m_creator_fraction, m_world->getValidProjectileId());
							memcpy(pr->getDamage(),&dmg,sizeof(Damage));
							pr->setFlags(Projectile::EXPLODES);
							pr->setMaxRadius(1);
							m_world->insertProjectile(pr,x+dir[1]*s.m_radius,y-dir[0]*s.m_radius,m_region);

						}
					}
				}
				break;

			case GROWING:
				// Status wachsend behandeln
				handleGrowing(dtime);

				break;

			case STABLE:
				// Status stabil behandeln
				handleStable(dtime);

				break;

			case DESTROYED:
				DEBUG5("destroyed");
				time =0;
				break;

			case VANISHING:
				// Status verschwindend
				if (m_type == FIRE_WALL)
				{
					// Radius reduzieren
					m_geometry.m_radius -= 2* dtime/(m_timer_limit);
				}
				if (m_timer >= m_timer_limit)
				{
					m_state = DESTROYED;
				}



			default:
				time =0;
				break;

		}
	}

	if (m_type == FIRE_WALL && m_world->timerLimit(1))
	{
		// Typ Feuersaeule

		// Flache der Feuersaeule
		Shape s;
		s.m_coordinate_x = m_geometry.m_coordinate_x;
		s.m_coordinate_y = m_geometry.m_coordinate_y;
		s.m_type = Shape::CIRCLE;
		s.m_radius = m_geometry.m_radius;

		// Objekte suchen die die Saeule beruehren
		m_world->getObjectsInShape(&s,m_region,&hitobj,getGeometry()->m_layer,WorldObject::CREATURE,0);
		for (i=hitobj.begin();i!=hitobj.end();++i)
		{
			// Schaden austeilen
			hit = (*i);
			float m = m_damage.m_multiplier[Damage::FIRE];
			m_damage.m_multiplier[Damage::FIRE] *= 500;
			hit->takeDamage(&m_damage);
			m_damage.m_multiplier[Damage::FIRE] = m;
		}
	}

	DEBUG5("update end");
	return true;

}

void Projectile::handleFlying(float dtime)
{
	list<WorldObject*> hitobj;
	list<WorldObject*>::iterator i;
	WorldObject* hit;
	float x;
	float y;
	float xnew, ynew;
	x = getGeometry()->m_coordinate_x;
	y = getGeometry()->m_coordinate_y;
	float dir[2],d;
	float rnew,rmin;
	float x2,y2;
	int lid;
	float newdir[2];
	float ndir[0],sdir[2];

	if (m_type == GUIDED_ARROW)
	{
		// Lenkpfeil

		// Zielobjekt
		hit =0;

		if (m_goal_object!=0)
		{
			// Zielobjekt ist per ID vorgegeben, Objekt von der Welt holen
			hit = m_world->getObject(m_goal_object,getRegion());
		}

		if (hit==0)
		{
			// kein Zielobjekt vorhanden, automatisch eines suchen

			// Kreis um aktuelle Position mit Umkreis 5
			Shape s;
			s.m_coordinate_x = x;
			s.m_coordinate_y = y;
			s.m_type = Shape::CIRCLE;
			s.m_radius = 5;

			// neue Flugrichtung
			newdir[0]=0;
			newdir[1]=0;

			// alle Objekte im Kreis suchen
			m_world->getObjectsInShape(&s,m_region,&hitobj,getGeometry()->m_layer, WorldObject::CREATURE,0);

			// alle Objekte als potentielle Ziele loeschen, die dem Erschaffer des Projektils nicht feindlich gesinnt sind
			if (!hitobj.empty())
			{
				i = hitobj.begin();
				hit=(*i);
				while (!hitobj.empty() && m_world->getRelation(m_creator_fraction,hit) != WorldObject::HOSTILE)
				{
					i=hitobj.erase(i);
					if (i!=hitobj.end())
						hit=(*i);
				}
			}

			// Suchen jenes Zieles welches am besten zu erreichen ist
			rmin = 3*sqr(s.m_radius);
			if (!hitobj.empty())
			{
				// normieren der aktuellen Geschwindigkeit
				d= 1/sqrt(sqr(m_speed_x)+sqr(m_speed_y));
				sdir[0]= m_speed_x *d;
				sdir[1]= m_speed_y *d;

				DEBUG5("old speed %f %f",m_speed_x,m_speed_y);
				// Durchmustern der potentiellen Ziele
				for (i=hitobj.begin();i!=hitobj.end();++i)
				{

					hit= (*i);
					// TODO: die nicht feindlichen sollten eigentlich schon rausgefiltern sein...
					if (m_world->getRelation(m_creator_fraction,hit) != WorldObject::HOSTILE)
						continue;

					//DEBUG("calc obj %i",hit->getId());
					// Koordinaten des Potentiellen Zieles
					x2 = hit->getGeometry()->m_shape.m_coordinate_x;
					y2 = hit->getGeometry()->m_shape.m_coordinate_y;

					// Richtung von aktueller Position zum Ziel, normiert
					dir[0] = x2-x;
					dir[1] = y2-y;
					d = 1/sqrt(sqr(dir[0]) + sqr(dir[1]));
					ndir[0] = dir[0]*d;
					ndir[1] = dir[1]*d;

					//DEBUG("dir %f %f",dir[0],dir[1]);
					// effektiver Abstand: normaler Abstand minus 4* skalarprodukt(aktuelle Flugrichtung, Zielrichtung)
					// Ziele fuer die eine scharfe Wendung noetig ist, werden schlechter bewertet
					rnew = -4*(ndir[0]*sdir[0]+ndir[1]*sdir[1]);
					rnew += sqr(dir[0]) + sqr(dir[1]);

					//DEBUG("rnew %f",rnew);
					// Wenn aktuelles Objekt besser ist als bisher bestes
					if (rnew<rmin)
					{
						// neue Richtung setzen
						rmin = rnew;
						newdir[0]=dir[0];
						newdir[1]=dir[1];

					}

				}
			}
		}
		else
		{
			// es gibt ein Zielobjekt
			// Richtung ist Richtung zum Zielobjekt
			newdir[0] = hit->getGeometry()->m_shape.m_coordinate_x-x;
			newdir[1] = hit->getGeometry()->m_shape.m_coordinate_y-y;

		}

		d = sqr(newdir[0]) + sqr(newdir[1]);
		if (d>0)
		{
			// normieren der Zielrichtung
			d= 0.01/sqrt(d);
			newdir[0]*=d;
			newdir[1]*=d;

			float p =0.3*dtime / 50;
			if (p>1)
				p=1;
			DEBUG5("p = %f",p);
			DEBUG5("newdir %f %f",newdir[0],newdir[1]);
			// Neue Richtung ergibt sich aus Linearkombination von aktueller Richtung und Zielrichtung
			m_speed_x = (1-p)*m_speed_x+(p)*newdir[0];
			m_speed_y = (1-p)*m_speed_y+(p)*newdir[1];
			DEBUG5("new speed %f %f",m_speed_x,m_speed_y);

			// neue Richtung normieren
			d = sqr(m_speed_x) + sqr(m_speed_y);
			if (d>0)
			{
				d= 0.01/sqrt(d);
				m_speed_x *=d;
				m_speed_y *=d;

			}
			else
			{
				m_speed_x=newdir[0];
				m_speed_y=newdir[1];
			}

			if (m_world->timerLimit(0))
			{
				m_event_mask |= Event::DATA_SPEED;
			}
		}
		hitobj.clear();

		if (m_speed_y!=0 || m_speed_x !=0)
		{
			m_geometry.m_angle = atan2(m_speed_y,m_speed_x);
		}
	}

	if (m_timer >= m_timer_limit)
	{
		DEBUG5("destroyed after timeout");
		m_state = DESTROYED;
	}

	// neue Koordinaten nach Ablauf des Zeitquantums
	xnew = x+m_speed_x*dtime;
	ynew = y+m_speed_y*dtime;



	// Objekt an der aktuellen Position suchen
	DEBUG5("getting objects on Line %f %f %f %f",x,y,xnew,ynew);
	m_world->getObjectsOnLine(x,y,xnew,ynew,m_region,&hitobj,getGeometry()->m_layer,WorldObject::CREATURE | WorldObject::FIXED,0);

	// Alle Objekte herausfiltern die verbuendet sind, sowie das zuletzt gerade getroffene Objekt
	if (!hitobj.empty())
	{
		i = hitobj.begin();
		hit = (*i);

		while (!hitobj.empty() && (m_world->getRelation(m_creator_fraction,hit) == WorldObject:: ALLIED || hit->getId() == m_last_hit_object_id ))
		{
			i=hitobj.erase(i);
			if (i!=hitobj.end())
				hit=(*i);
		}
	}

	if (!hitobj.empty())
	{
		DEBUG5("hit object");
		i = hitobj.begin();
		hit = (*i);
		//DEBUG("hit object %p",hit);

		//DEBUG("hit object %i at %f %f",hit->getId(),hit->getGeometry()->m_shape.m_coordinate_x,hit->getGeometry()->m_shape.m_coordinate_y);
		if (!(m_flags & (EXPLODES | MULTI_EXPLODES) ))
		{
			// erstem getroffenen Objekt Schaden zufuegen

			DEBUG5("deal damage %p",hit);
			DEBUG5("deal damage to %i",(*i)->getId());
			if (m_flags & PIERCING)
			{
				// Projektil fliegt weiter
				m_last_hit_object_id = hit->getId();
			}
			else
			{
				// Projektil wird zerstoert
				DEBUG5("hit obj %i",hit->getId());
				m_state = DESTROYED;
				m_timer=0;
			}
			if (m_world->isServer())
			{
				hit->takeDamage(&m_damage);
			}

		}
		else
		{
			// Projektil explodiert
			// Status auf explodierend setzen, Radius, Timer setzen
			DEBUG5("exploding");
			m_state = EXPLODING;
			if (m_type == FIRE_BALL || m_type == FIRE_ARROW)
				m_type = FIRE_EXPLOSION;
			else if (m_type == ICE_ARROW)
				m_type = ICE_EXPLOSION;
			else if (m_type == WIND_ARROW)
				m_type = WIND_EXPLOSION;
			else
				m_type = EXPLOSION;

			m_max_radius = 1.5;

			m_timer=0;
			m_timer_limit=200;

			m_event_mask |= Event::DATA_PROJ_STATE | Event::DATA_TYPE | Event::DATA_MAX_RADIUS;
		}

		// true, wenn das Projektil zu einem weiteren Ziel weiterspringt
		bool bounce = false;
		if (m_flags & BOUNCING)
			bounce = true;

		if (m_flags & PROB_BOUNCING && m_world->isServer())
		{
			// zufaelliges weiterspringen, Chance 50%
			if (rand()<RAND_MAX*0.5)
			{
				DEBUG("prob bounce");
				bounce = true;
			}
		}

		if (hit->getTypeInfo()->m_type != WorldObject::TypeInfo::TYPE_FIXED_OBJECT && bounce)
		{
			// Projektil hat ein Lebewesen getroffen, springt weiter
			m_state = FLYING;
			float speed = sqrt(sqr(m_speed_x)+sqr(m_speed_y));

			// Kreis mit Radius 5 um aktuelle Position
			Shape s;
			s.m_coordinate_x = m_geometry.m_coordinate_x;
			s.m_coordinate_y = m_geometry.m_coordinate_y;
			s.m_type = Shape::CIRCLE;
			s.m_radius = 5;

			// Alle Objekte im Kreis suchen
			hitobj.clear();
			m_world->getObjectsInShape(&s,m_region,&hitobj,WorldObject::Geometry::LAYER_AIR,WorldObject::CREATURE,0);
			rmin = sqr(s.m_radius);
			lid = hit->getId();
			hit =0;

			for (i=hitobj.begin();i!=hitobj.end();++i)
			{
				// Durchmustern der potentiellen Ziele
				DEBUG5("testing obj %i, lid %i",(*i)->getId(),lid);

				// letztes getroffendes Objekt ausschliessen
				if (lid==(*i)->getId())
					continue;

				// alle nicht feindlich gesinnten Objekte ausschliessen
				if (m_world->getRelation(m_creator_fraction,(*i)) != WorldObject::HOSTILE)
					continue;

				// kein zurueckspringen zu dem davor zuletzt getroffenen Objekt
				if (m_last_hit_object_id== (*i)->getId())
					continue;

				// Abstand zur aktuellen Position ermitteln
				x2 = (*i)->getGeometry()->m_shape.m_coordinate_x;
				y2 = (*i)->getGeometry()->m_shape.m_coordinate_y;

				rnew = sqr(x2-xnew)+sqr(y2-ynew);
				DEBUG5("radius %f",rnew);
				// Das Objekt herraussuchen, welches den minimalen Abstand aufweist
				if (rnew < rmin)
				{
					rmin = rnew;
					hit = (*i);
				}
			}

			m_last_hit_object_id = lid;
			if (hit!=0)
			{
				// Es wurde ein Ziel gefunden
				DEBUG5("next obj %i",hit->getId());
				DEBUG5("counter %i",m_counter);

				// Neue Richtung ist Vektor von aktueller Position zum neuen Ziel
				x2 = hit->getGeometry()->m_shape.m_coordinate_x;
				y2 = hit->getGeometry()->m_shape.m_coordinate_y;
				dir[0] = x2-xnew;
				dir[1] = y2-ynew;
				d = 1/sqrt(sqr(dir[0])+sqr(dir[1]));
				dir[0] *=d;
				dir[1] *=d;
				m_speed_x = dir[0] *speed;
				m_speed_y = dir[1] *speed;

				m_event_mask |= Event::DATA_SPEED | Event::DATA_PROJ_STATE;

				DEBUG5("koord %f %f to %f %f",xnew,ynew,x2,y2);
				DEBUG5("dir %f %f",dir[0],dir[1]);
				m_timer =0;

				// bei Kettenblitz Schaden pro Sprung um 20% reduzieren
				if (m_type == CHAIN_LIGHTNING)
					m_damage.m_multiplier[Damage::AIR] *= 0.8;

				if (m_speed_y!=0 || m_speed_x !=0)
				{
					m_geometry.m_angle = atan2(m_speed_y,m_speed_x);
				}
			}
			else
			{
				// kein Ziel gefunden, Projektil zerstoeren
				m_state = DESTROYED;
			}

			// Zaehler, bei 5 Spruengen Projektil zerstoeren
			m_counter ++;
			if (m_counter>=5)
			{
				m_state = DESTROYED;
			}
		}

	}
	// neue Koordinaten setzen
	getGeometry()->m_coordinate_x = xnew;
	getGeometry()->m_coordinate_y = ynew;

}

void Projectile::handleGrowing(float dtime)
{
	list<WorldObject*> hitobj;
	list<WorldObject*>::iterator i;
	WorldObject* hit;
	float x;
	float y;
	float rnew,rmin;
	float rold;
	int lid;

	// aktuelle Position
	x = getGeometry()->m_coordinate_x;
	y = getGeometry()->m_coordinate_y;

	rold = m_geometry.m_radius;
	// Radius erhoehen
	m_geometry.m_radius += m_max_radius* dtime/(m_timer_limit);

	if ((m_type == FIRE_WAVE || m_type == ICE_RING) && m_world->isServer())
	{
		// Schaden an die neu getroffenen Lebewesen austeilen
		rnew = m_geometry.m_radius;
		// Flaeche die der Kreis nach dem Update ueberdeckt
		Shape s;
		s.m_coordinate_x = m_geometry.m_coordinate_x;
		s.m_coordinate_y = m_geometry.m_coordinate_y;
		s.m_type = Shape::CIRCLE;
		s.m_radius = rnew;

		// Alle Objekte suchen die sich in dem Kreis befinden
		m_world->getObjectsInShape(&s,m_region,&hitobj,getGeometry()->m_layer,WorldObject::CREATURE,0);
		lid = m_last_hit_object_id;
		DEBUG5("last hit id = %i",lid);
		rmin =0;

		// Flaeche die vor dem Update ueberdeckt wurde minus 10%
		s.m_radius = rold-(rnew-rold)*0.1;

		// Schaden wird nur an die Objekte ausgeteilt, die im neuen, aber nicht im alten Kreis sind
		if (!hitobj.empty())
		{
			for (i=hitobj.begin();i!=hitobj.end();++i)
			{
				hit = (*i);
				DEBUG5("covering obj %i",hit->getId());

				// Kein Schaden an nicht feindliche Objekte austeilen
				if (m_world->getRelation(m_creator_fraction,hit) != WorldObject::HOSTILE)
					continue;

				// kein Schaden an Objekte austeilen, die sich im inneren Kreis befinden
				if (World::intersect(&(hit->getGeometry()->m_shape),&s))
				{
					DEBUG5("is inside inner circle");
					continue;
				}

				// kein Schaden an das zuletzt getroffene Objekt austeilen
				if (hit->getId() == lid)
				{
					DEBUG5("is last hit obj %i",hit->getId());
					continue;
				}

				// Schaden austeilen
				hit->takeDamage(&m_damage);
				m_last_hit_object_id = hit->getId();
			}
		}
	}

	if (m_timer >= m_timer_limit)
	{
		// Timer abgelaufen

		// Standardverhalten Projektil zerstoeren
		m_state = DESTROYED;

		if (m_type == FIRE_WALL)
		{
			// fuer Feuersaeule: Uebergang in stabilen Zustand
			m_timer =0;
			m_timer_limit=5000;
			m_state = STABLE;

			m_event_mask |= Event::DATA_PROJ_STATE | Event::DATA_PROJ_TIMER;
		}
	}

}

void Projectile::handleStable(float dtime)
{

	list<WorldObject*> hitobj;
	list<WorldObject*>::iterator i;
	WorldObject* hit;

	// aktuelle Position
	float x;
	float y;
	x = getGeometry()->m_coordinate_x;
	y = getGeometry()->m_coordinate_y;

	float dir[2];
	float x2,y2;
	Projectile* pr;

	if (m_timer >= m_timer_limit && m_world->isServer())
	{
		// Timer Limit erreicht
		m_state = DESTROYED;

		if (m_type == BLIZZARD)
		{
			// Flaeche die vom Blizzard bedeckt wird
			Shape s;
			s.m_coordinate_x = m_geometry.m_coordinate_x;
			s.m_coordinate_y = m_geometry.m_coordinate_y;
			s.m_type = Shape::CIRCLE;
			s.m_radius = m_geometry.m_radius;

			// Alle Objekte in der Flaeche suchen
			m_world->getObjectsInShape(&s,m_region,&hitobj,getGeometry()->m_layer,WorldObject::CREATURE,0);
			for (i=hitobj.begin();i!=hitobj.end();++i)
			{
				// Schaden austeilen
				hit = (*i);
				hit->takeDamage(&m_damage);
			}

			// Schaden wird in 5 Wellen austeilt
			if (m_counter <5)
			{
				// naechste Welle: Timer zuruecksetzen, Zaehler erhoehen
				m_counter ++;
				m_timer =0;
				m_state = STABLE;

			}
		}

		if (m_type == FREEZE || m_type == STATIC_SHIELD)
		{
			// vom Zauber ueberdeckte Flaeche
			Shape s;
			s.m_coordinate_x = m_geometry.m_coordinate_x;
			s.m_coordinate_y = m_geometry.m_coordinate_y;
			s.m_type = Shape::CIRCLE;
			s.m_radius = m_geometry.m_radius;

			// Alle Objekte in der Flaeche suchen
			m_world->getObjectsInShape(&s,m_region,&hitobj,getGeometry()->m_layer,WorldObject::CREATURE,0);
			for (i=hitobj.begin();i!=hitobj.end();++i)
			{
				// Schaden austeilen
				hit = (*i);
				hit->takeDamage(&m_damage);
			}
		}

		if (m_type ==  THUNDERSTORM)
		{
			// Punkt in der Flaeche des Zaubers auswuerfeln
			dir[0] = m_geometry.m_radius;
			dir[1] = m_geometry.m_radius;

			while (sqr(dir[0]) + dir[1] > sqr(m_geometry.m_radius))
			{
				dir[0] = m_geometry.m_radius - 2*m_geometry.m_radius*rand()*1.0/RAND_MAX;
				dir[1] = m_geometry.m_radius - 2*m_geometry.m_radius*rand()*1.0/RAND_MAX;
			}

			// Kreis um den ausgewuerfelten Punkt mit Radius 1.5
			Shape s;
			s.m_coordinate_x = dir[0]+x;
			s.m_coordinate_y = dir[1]+y;
			s.m_type = Shape::CIRCLE;
			s.m_radius = 1.5;
			hitobj.clear();

			// Alle Objekte in dem Kreis suchen
			m_world->getObjectsInShape(&s,m_region,&hitobj,getGeometry()->m_layer,WorldObject::CREATURE,0);

			DEBUG5("center %f %f",s.m_coordinate_x,s.m_coordinate_y);

			// Alle nicht feindlichen Objekte aus der Liste entfernen
			if (!hitobj.empty())
			{
				i = hitobj.begin();
				hit = (*i);
				while (i != hitobj.end() && m_world->getRelation(m_creator_fraction,hit) != WorldObject::HOSTILE)
				{
					i=hitobj.erase(i);
					if (i!=hitobj.end())
						hit=(*i);

				}
			}

			if (!hitobj.empty())
			{
				// Ziel gefunden
				i = hitobj.begin();
				hit = (*i);

				DEBUG5("hit obj %i",hit->getId());

				// Position des gefundenen Zieles
				x2 = hit->getGeometry()->m_shape.m_coordinate_x;
				y2 = hit->getGeometry()->m_shape.m_coordinate_y;

				// beim Ziel Projektil vom Typ Blitz erzeugen
				pr = new Projectile(m_world,Projectile::LIGHTNING,m_creator_fraction, m_world->getValidProjectileId());
				memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
				m_world->insertProjectile(pr,x2,y2,m_region);
			}

			// Gewitter besteht aus 30 Blitzen
			if (m_counter <30)
			{
				// naechster Blitz: Timer zuruecksetzen
				m_counter ++;
				m_timer =0;
				m_state = STABLE;

			}

		}

		if (m_type == FIRE_WALL)
		{
			// Zauber Feuersaeule
			// Uebergang zum Zustand verschwindend
			m_timer =0;
			m_timer_limit=200;
			m_state = VANISHING;
			m_event_mask |= Event::DATA_PROJ_STATE | Event::DATA_PROJ_TIMER;
		}

		if (m_type == LIGHTNING || m_type ==LIGHT_BEAM  || m_type ==ELEM_EXPLOSION || m_type ==ACID || m_type ==DIVINE_BEAM  || m_type ==HYPNOSIS)
		{
			// Objekt an der Stelle suchen an der der Zauber wirkt
			hit = m_world->getObjectAt( getGeometry()->m_coordinate_x,getGeometry()->m_coordinate_y,m_region,getGeometry()->m_layer );

			if (hit !=0)
			{
				// Ziel gefunden, Schaden austeilen
				m_last_hit_object_id = hit->getId();
				hit->takeDamage(&m_damage);
			}
		}

	}
}


void Projectile::toString(CharConv* cv)
{
	cv->toBuffer((char) m_type);
	cv->toBuffer((char) m_creator_fraction);
	cv->toBuffer( m_id);
	cv->toBuffer((char) m_state);
	cv->toBuffer(m_geometry.m_coordinate_x);
	cv->toBuffer(m_geometry.m_coordinate_y);
	cv->toBuffer(m_geometry.m_radius);
	cv->toBuffer(m_geometry.m_angle);
	cv->toBuffer(m_geometry.m_layer);
	cv->toBuffer(m_timer);
	cv->toBuffer(m_timer_limit);
	cv->toBuffer(m_speed_x);
	cv->toBuffer(m_speed_y);
	cv->toBuffer(m_flags);
	cv->toBuffer(m_max_radius);
	cv->toBuffer(m_goal_object);

}

void Projectile::fromString(CharConv* cv)
{
	// Typ, Fraktion und ID werden schon vorher eingelesen..

	char tmp;
	cv->fromBuffer<char>(tmp);
	m_state = (ProjectileState) tmp;
	cv->fromBuffer<float>(m_geometry.m_coordinate_x);
	cv->fromBuffer<float>(m_geometry.m_coordinate_y);
	cv->fromBuffer<float>(m_geometry.m_radius);
	cv->fromBuffer<float>(m_geometry.m_angle);
	cv->fromBuffer<short>(m_geometry.m_layer);
	cv->fromBuffer<float>(m_timer);
	cv->fromBuffer<float>(m_timer_limit);
	cv->fromBuffer(m_speed_x);
	cv->fromBuffer(m_speed_y);
	cv->fromBuffer(m_flags);
	cv->fromBuffer(m_max_radius);
	cv->fromBuffer(m_goal_object);
}

void Projectile::writeEvent(Event* event, CharConv* cv)
{
	if (event->m_data & Event::DATA_SPEED)
	{
		cv->toBuffer(m_geometry.m_coordinate_x);
		cv->toBuffer(m_geometry.m_coordinate_y);
		cv->toBuffer(m_speed_x);
		cv->toBuffer(m_speed_y);
	}

	if (event->m_data & Event::DATA_PROJ_STATE)
	{
		cv->toBuffer((char) m_state);
	}

	if (event->m_data & Event::DATA_GOAL_OBJECT)
	{
		cv->toBuffer(m_goal_object);
	}

	if (event->m_data & Event::DATA_TYPE)
	{
		cv->toBuffer((char) m_type);
	}

	if (event->m_data & Event::DATA_PROJ_TIMER)
	{
		cv->toBuffer(m_timer_limit);
	}

	if (event->m_data & Event::DATA_MAX_RADIUS)
	{
		cv->toBuffer(m_max_radius);
		cv->toBuffer<float>(m_geometry.m_radius);
	}
}

void Projectile::processEvent(Event* event, CharConv* cv)
{
	if (event->m_data & Event::DATA_SPEED)
	{
		cv->fromBuffer(m_geometry.m_coordinate_x);
		cv->fromBuffer(m_geometry.m_coordinate_y);
		cv->fromBuffer(m_speed_x);
		cv->fromBuffer(m_speed_y);

		if (m_speed_y!=0 || m_speed_x !=0)
		{
			m_geometry.m_angle = atan2(m_speed_y,m_speed_x);
		}
	}

	if (event->m_data & Event::DATA_PROJ_STATE)
	{
		char ctmp;
		cv->fromBuffer(ctmp);
		m_state = (ProjectileState) ctmp;
	}

	if (event->m_data & Event::DATA_GOAL_OBJECT)
	{
		cv->fromBuffer(m_goal_object);
	}

	if (event->m_data & Event::DATA_TYPE)
	{
		char ctmp;
		cv->fromBuffer(ctmp);
		m_type = (ProjectileType) ctmp;
	}

	if (event->m_data & Event::DATA_PROJ_TIMER)
	{
		cv->fromBuffer(m_timer_limit);
		m_timer =0;
	}

	if (event->m_data & Event::DATA_MAX_RADIUS)
	{
		cv->fromBuffer(m_max_radius);
		cv->fromBuffer<float>(m_geometry.m_radius);
	}
}

