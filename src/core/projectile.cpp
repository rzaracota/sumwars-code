#include "projectile.h"
#include "world.h"


Region* Projectile::getRegionPtr()
{
	return World::getWorld()->getRegion(m_region);
}

Projectile::Projectile(ProjectileType type, WorldObject::Fraction fr, int id)
{
	m_id = id;
	m_type = type;
	m_timer =0;
	m_layer = WorldObject::LAYER_AIR;
	
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


	m_shape.m_radius =r;
	m_shape.m_angle =0;
	m_creator_fraction = fr;

}

bool Projectile::update(float time)
{

	// Zeit die beim aktuellen Durchlauf der Schleife verbraucht wird
	float dtime;
	// true, wenn Timer die Grenze erreicht
	bool lim = false;
	// Liste der getroffenen Objekte
	WorldObjectList hitobj;
	WorldObjectList::iterator i;
	WorldObject* hit;

	while (time>0)
	{
		hitobj.clear();
		dtime = time;

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
				m_shape.m_radius += (m_max_radius)*dtime/(m_timer_limit);

				// Wenn Timer Limit erreicht
				if (m_timer >= m_timer_limit)
				{
					m_state = DESTROYED;

					if (World::getWorld()->isServer())
					{

						// Alle Objekte im Explosionsradius suchen
						hitobj.clear();
						getRegionPtr()->getObjectsInShape(&m_shape,&hitobj,m_layer,WorldObject::CREATURE,0);
						for (i=hitobj.begin();i!=hitobj.end();++i)
						{
							// Schaden austeilen
							(*i)->takeDamage(&m_damage);
						}

						if (m_flags & MULTI_EXPLODES)
						{
							// Flag mehrfach explodierend gesetzt
							DEBUG("multiexploding");
							Vector dir = m_speed;
							dir.normalize();
							dir *= m_shape.m_radius;
							
							// Schaden halbieren
							Damage dmg;
							memcpy(&dmg,&m_damage,sizeof(Damage));
							for (int i=0;i<4;i++)
							{
								dmg.m_multiplier[i] *= 0.5;
							}

							// vier neue Projektile erzeugen
							Projectile* pr;
							pr = new Projectile(m_type,m_creator_fraction, World::getWorld()->getValidId());
							memcpy(pr->getDamage(),&dmg,sizeof(Damage));
							pr->setFlags(Projectile::EXPLODES);
							pr->setMaxRadius(1);
							getRegionPtr()->insertProjectile(pr,m_shape.m_center + dir );
							
							dir.m_x = -dir.m_x;
							pr = new Projectile(m_type,m_creator_fraction, World::getWorld()->getValidId());
							memcpy(pr->getDamage(),&dmg,sizeof(Damage));
							pr->setFlags(Projectile::EXPLODES);
							pr->setMaxRadius(1);
							getRegionPtr()->insertProjectile(pr,m_shape.m_center + dir);

							dir.m_y = -dir.m_y;
							pr = new Projectile(m_type,m_creator_fraction, World::getWorld()->getValidId());
							memcpy(pr->getDamage(),&dmg,sizeof(Damage));
							pr->setFlags(Projectile::EXPLODES);
							pr->setMaxRadius(1);
							getRegionPtr()->insertProjectile(pr,m_shape.m_center + dir);

							dir.m_x = -dir.m_x;
							pr = new Projectile(m_type,m_creator_fraction, World::getWorld()->getValidId());
							memcpy(pr->getDamage(),&dmg,sizeof(Damage));
							pr->setFlags(Projectile::EXPLODES);
							pr->setMaxRadius(1);
							getRegionPtr()->insertProjectile(pr,m_shape.m_center + dir);

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
					m_shape.m_radius -= 2* dtime/(m_timer_limit);
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

	if (m_type == FIRE_WALL && World::getWorld()->timerLimit(1))
	{
		// Typ Feuersaeule


		// Objekte suchen die die Saeule beruehren
		getRegionPtr()->getObjectsInShape(&m_shape,&hitobj,m_layer,WorldObject::CREATURE,0);
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
	WorldObjectList hitobj;
	WorldObjectList::iterator i;
	WorldObject* hit;
	Vector dir,newdir(0,0);
	Vector sdir,hitpos,ndir;
	Vector &pos = m_shape.m_center, newpos;
	float rnew,rmin;
	int lid;

	if (m_type == GUIDED_ARROW)
	{
		// Lenkpfeil

		// Zielobjekt
		hit =0;

		if (m_goal_object!=0)
		{
			// Zielobjekt ist per ID vorgegeben, Objekt von der Welt holen
			hit = getRegionPtr()->getObject(m_goal_object);
		}

		if (hit==0)
		{
			// kein Zielobjekt vorhanden, automatisch eines suchen

			// Kreis um aktuelle Position mit Umkreis 5
			Shape s;
			s.m_center = pos;
			s.m_type = Shape::CIRCLE;
			s.m_radius = 5;


			// alle Objekte im Kreis suchen
			getRegionPtr()->getObjectsInShape(&s,&hitobj,m_layer, WorldObject::CREATURE,0);

			// alle Objekte als potentielle Ziele loeschen, die dem Erschaffer des Projektils nicht feindlich gesinnt sind
			if (!hitobj.empty())
			{
				i = hitobj.begin();
				hit=(*i);
				while (!hitobj.empty() && World::getWorld()->getRelation(m_creator_fraction,hit) != WorldObject::HOSTILE)
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
				sdir = m_speed;
				sdir.normalize();

				// Durchmustern der potentiellen Ziele
				for (i=hitobj.begin();i!=hitobj.end();++i)
				{

					hit= (*i);

					// Koordinaten des Potentiellen Zieles
					hitpos = hit->getShape()->m_center;

					// Richtung von aktueller Position zum Ziel, normiert
					dir = hitpos - pos;
					ndir = dir;
					ndir.normalize();

					// effektiver Abstand: normaler Abstand minus 4* skalarprodukt(aktuelle Flugrichtung, Zielrichtung)
					// Ziele fuer die eine scharfe Wendung noetig ist, werden schlechter bewertet
					rnew = -4*(ndir*sdir);
					rnew += sqr(dir.m_x) + sqr(dir.m_y);

					//DEBUG("rnew %f",rnew);
					// Wenn aktuelles Objekt besser ist als bisher bestes
					if (rnew<rmin)
					{
						// neue Richtung setzen
						rmin = rnew;
						newdir = dir;

					}

				}
			}
		}
		else
		{
			// es gibt ein vorgegebenes Zielobjekt
			// Richtung ist Richtung zum Zielobjekt
			newdir = hit->getShape()->m_center - pos;

		}

		// aktuelle absolute Geschwindigkeit
		float v = m_speed.getLength();
		
		
		// normieren der Zielrichtung
		newdir.normalize();
		newdir *= v;

		float p =0.3*dtime / 50;
		if (p>1)
			p=1;
		DEBUG5("p = %f",p);
		DEBUG5("newdir %f %f",newdir.m_x,newdir.m_y);
		
		// Neue Richtung ergibt sich aus Linearkombination von aktueller Richtung und Zielrichtung
		m_speed = m_speed*(1-p) + newdir*p;
		DEBUG5("new speed %f %f",m_speed.m_x,m_speed.m_y);

		// neue Richtung normieren
		m_speed.normalize();
		m_speed *= v;

		if (World::getWorld()->timerLimit(0))
		{
			m_event_mask |= Event::DATA_SPEED;
		}
		
		hitobj.clear();

		m_shape.m_angle = m_speed.angle();
		
	}

	if (m_timer >= m_timer_limit)
	{
		DEBUG5("destroyed after timeout");
		m_state = DESTROYED;
	}

	DEBUG5("pos %f %f",pos.m_x, pos.m_y);
	
	// neue Koordinaten nach Ablauf des Zeitquantums
	newpos = pos + m_speed*dtime;

	// Linie zwischen alter und neuer Position
	Line line(pos,newpos);


	// Objekt an der aktuellen Position suchen
	getRegionPtr()->getObjectsOnLine(line,&hitobj,m_layer,WorldObject::CREATURE | WorldObject::FIXED,0);

	// Alle Objekte herausfiltern die verbuendet sind, sowie das zuletzt gerade getroffene Objekt
	if (!hitobj.empty())
	{
		i = hitobj.begin();
		hit = (*i);

		while (!hitobj.empty() && (World::getWorld()->getRelation(m_creator_fraction,hit) == WorldObject:: ALLIED || hit->getId() == m_last_hit_object_id ))
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
			if (World::getWorld()->isServer())
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

		if (m_flags & PROB_BOUNCING && World::getWorld()->isServer())
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
			float speed = m_speed.getLength();

			// Kreis mit Radius 5 um aktuelle Position
			Shape s;
			s.m_center = pos;
			s.m_type = Shape::CIRCLE;
			s.m_radius = 5;

			// Alle Objekte im Kreis suchen
			hitobj.clear();
			getRegionPtr()->getObjectsInShape(&s,&hitobj,WorldObject::LAYER_AIR,WorldObject::CREATURE,0);
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
				if (World::getWorld()->getRelation(m_creator_fraction,(*i)) != WorldObject::HOSTILE)
					continue;

				// kein zurueckspringen zu dem davor zuletzt getroffenen Objekt
				if (m_last_hit_object_id== (*i)->getId())
					continue;

				// Abstand zur aktuellen Position ermitteln
				hitpos = (*i)->getShape()->m_center - newpos;

				rnew = sqr(hitpos.m_x)+sqr(hitpos.m_y);
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
				dir = hit->getShape()->m_center - newpos;
				dir.normalize();
				m_speed = dir *speed;

				m_event_mask |= Event::DATA_SPEED | Event::DATA_PROJ_STATE;

				DEBUG5("dir %f %f",dir.m_x,dir.m_y);
				m_timer =0;

				// bei Kettenblitz Schaden pro Sprung um 20% reduzieren
				if (m_type == CHAIN_LIGHTNING)
					m_damage.m_multiplier[Damage::AIR] *= 0.8;

				m_shape.m_angle = m_speed.angle();
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
	m_shape.m_center = newpos;

}

void Projectile::handleGrowing(float dtime)
{
	WorldObjectList hitobj;
	WorldObjectList::iterator i;
	WorldObject* hit;
	float rnew,rmin;
	float rold;
	int lid;

	rold = m_shape.m_radius;
	// Radius erhoehen
	m_shape.m_radius += m_max_radius* dtime/(m_timer_limit);

	if ((m_type == FIRE_WAVE || m_type == ICE_RING) && World::getWorld()->isServer())
	{
		// Schaden an die neu getroffenen Lebewesen austeilen
		rnew = m_shape.m_radius;
		
		Shape s;
		s.m_center = m_shape.m_center;
		s.m_type = Shape::CIRCLE;
		s.m_radius = m_shape.m_radius;

		// Alle Objekte suchen die sich in dem Kreis befinden
		getRegionPtr()->getObjectsInShape(&m_shape,&hitobj,m_layer,WorldObject::CREATURE,0);
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
				if (World::getWorld()->getRelation(m_creator_fraction,hit) != WorldObject::HOSTILE)
					continue;

				// kein Schaden an Objekte austeilen, die sich im inneren Kreis befinden
				if (hit->getShape()->intersects(s))
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

	WorldObjectList hitobj;
	WorldObjectList::iterator i;
	WorldObject* hit;

	// aktuelle Position
	Vector& pos= m_shape.m_center;
	Vector dir;
	Projectile* pr;

	if (m_timer >= m_timer_limit && World::getWorld()->isServer())
	{
		// Timer Limit erreicht
		m_state = DESTROYED;

		if (m_type == BLIZZARD)
		{

			// Alle Objekte in der Flaeche suchen
			getRegionPtr()->getObjectsInShape(&m_shape,&hitobj,m_layer,WorldObject::CREATURE,0);
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
			// Alle Objekte in der Flaeche suchen
			getRegionPtr()->getObjectsInShape(&m_shape,&hitobj,m_layer,WorldObject::CREATURE,0);
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
			dir = Vector(m_shape.m_radius , m_shape.m_radius);

			while (dir.getLength() > m_shape.m_radius)
			{
				dir.m_x = m_shape.m_radius*(1 - 2*rand()*1.0/RAND_MAX);
				dir.m_y = m_shape.m_radius*(1 - 2*rand()*1.0/RAND_MAX);
			}

			// Kreis um den ausgewuerfelten Punkt mit Radius 1.5
			Shape s;
			s.m_center = dir+pos;
			s.m_type = Shape::CIRCLE;
			s.m_radius = 1.5;
			hitobj.clear();

			// Alle Objekte in dem Kreis suchen
			getRegionPtr()->getObjectsInShape(&s,&hitobj,m_layer,WorldObject::CREATURE,0);

			// Alle nicht feindlichen Objekte aus der Liste entfernen
			if (!hitobj.empty())
			{
				i = hitobj.begin();
				hit = (*i);
				while (i != hitobj.end() && World::getWorld()->getRelation(m_creator_fraction,hit) != WorldObject::HOSTILE)
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

				// beim Ziel Projektil vom Typ Blitz erzeugen
				pr = new Projectile(Projectile::LIGHTNING,m_creator_fraction, World::getWorld()->getValidId());
				memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
				getRegionPtr()->insertProjectile(pr,hit->getShape()->m_center);
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
			hit = getRegionPtr()->getObjectAt( pos,m_region, m_layer );

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
	cv->toBuffer(m_shape.m_center.m_x);
	cv->toBuffer(m_shape.m_center.m_y);
	cv->toBuffer(m_shape.m_radius);
	cv->toBuffer(m_shape.m_angle);
	cv->toBuffer(m_layer);
	cv->toBuffer(m_timer);
	cv->toBuffer(m_timer_limit);
	cv->toBuffer(m_speed.m_x);
	cv->toBuffer(m_speed.m_y);
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
	cv->fromBuffer<float>(m_shape.m_center.m_x);
	cv->fromBuffer<float>(m_shape.m_center.m_y);
	cv->fromBuffer<float>(m_shape.m_radius);
	cv->fromBuffer<float>(m_shape.m_angle);
	cv->fromBuffer<short>(m_layer);
	cv->fromBuffer<float>(m_timer);
	cv->fromBuffer<float>(m_timer_limit);
	cv->fromBuffer(m_speed.m_x);
	cv->fromBuffer(m_speed.m_y);
	cv->fromBuffer(m_flags);
	cv->fromBuffer(m_max_radius);
	cv->fromBuffer(m_goal_object);
}

void Projectile::writeEvent(Event* event, CharConv* cv)
{
	if (event->m_data & Event::DATA_SPEED)
	{
		cv->toBuffer(m_shape.m_center.m_x);
		cv->toBuffer(m_shape.m_center.m_y);
		cv->toBuffer(m_speed.m_x);
		cv->toBuffer(m_speed.m_y);
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
		cv->toBuffer<float>(m_shape.m_radius);
	}
}

void Projectile::processEvent(Event* event, CharConv* cv)
{
	if (event->m_data & Event::DATA_SPEED)
	{
		cv->fromBuffer(m_shape.m_center.m_x);
		cv->fromBuffer(m_shape.m_center.m_y);
		cv->fromBuffer(m_speed.m_x);
		cv->fromBuffer(m_speed.m_y);

		if (m_speed.m_y!=0 || m_speed.m_x !=0)
		{
			m_shape.m_angle = m_speed.angle();
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
		cv->fromBuffer<float>(m_shape.m_radius);
	}
}

