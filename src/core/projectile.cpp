#include "projectile.h"
#include "world.h"




Projectile::Projectile(Subtype subtype, Damage* dmg, int id)
	: GameObject(id)
{
	if (dmg !=0)
	{
		setFraction(dmg->m_attacker_fraction);
	}
	setSubtype(subtype);
	m_timer =0;
	setLayer(WorldObject::LAYER_AIR);
	setType("PROJECTILE");
	setBaseType(PROJECTILE);
	
	m_last_hit_object_id =0;
	m_flags =0;
	setState(STATE_FLYING,false);
	m_timer_limit = 1500;
	m_counter =0;
	m_goal_object =0;


	float r=0.1;
	// Radius, Timerlaufzeit und Status je nach Typ setzen
	Subtype st = getSubtype();
	if (st == "FIRE_BOLT")
	{
		r=0.1;
	}
	else if (st == "ICE_BOLT")
	{
		r=0.1;
	}
	else if (st == "LIGHTNING")
	{
		r=0.15;
		setState(STATE_STABLE,false);
		m_timer_limit = 250;
	}
	else if (st == "FIRE_BALL")
	{
		r=0.2;
		m_max_radius = 1.5;
		m_flags = EXPLODES;
	}
	else if (st == "FIRE_WALL")
	{
		r=0;
		setState(STATE_GROWING,false);
		m_timer_limit = 200;
		m_max_radius = 2;
	}
	else if (st == "FIRE_WAVE")
	{
		r=0;
		setState(STATE_GROWING,false);
		m_timer_limit = 1000;
		m_max_radius = 7;
	}
	else if (st == "ICE_RING")
	{
		r=0;
		setState(STATE_GROWING,false);
		m_timer_limit = 800;
		m_max_radius = 4;
	}
	else if (st == "FREEZE")
	{
		r=4;
		setState(STATE_STABLE,false);
		m_timer_limit = 300;
	}
	else if (st == "BLIZZARD")
	{
		r=3;
		setState(STATE_STABLE,false);
		m_timer_limit = 500;
	}
	else if (st == "THUNDERSTORM")
	{
		r=5;
		setState(STATE_STABLE,false);
		m_timer_limit = 300;
	}
	else if (st == "CHAIN_LIGHTNING")
	{
		r=0.15;
		m_flags = BOUNCING;
	}
	else if (st == "STATIC_SHIELD")
	{
		r=1.5;
		setState(STATE_STABLE,false);
		m_timer_limit = 150;
	}
	else if (st == "ARROW")
	{
		r=0.1;
	}
	else if (st == "GUIDED_ARROW")
	{
		r=0.1;
		m_timer_limit = 1000;
	}
	else if (st == "")
	{
		r=0.1;
	}
	else if (st == "")
	{
		r=0.1;
	}
	else if (st == "")
	{
		r=0.1;
	}
	else if (st == "EXPLOSION"  ||st == "ICE_EXPLOSION" || st == "WIND_EXPLOSION" || st == "FIRE_EXPLOSION" || st == "ELEM_EXPLOSION")
	{
		m_timer_limit = 200;
		setState(STATE_EXPLODING,false);
	}
	else if (st == "LIGHT_BEAM" || st == "DIVINE_BEAM" || st == "ACID" || st == "HYPNOSIS" )
	{
		r=0.1;
	}
	else if (st == "")
	{
		r=0.1;
	}


	getShape()->m_radius =r;
	setAngle(0);
	if (dmg !=0)
	{
		memcpy(&m_damage, dmg,sizeof(Damage)); 
	}

	clearNetEventMask();
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

	while (time>0.1)
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
		
		if (dtime <=0)
			return false;

		switch (getState())
		{
			case STATE_FLYING:
				// Status fliegend behandeln
				handleFlying(dtime);

				break;

			case STATE_EXPLODING:
				DEBUG5("exploding");
				// Radius vergroessern
				getShape()->m_radius += (m_max_radius)*dtime/(m_timer_limit);

				// Wenn Timer Limit erreicht
				if (m_timer >= m_timer_limit)
				{
					setState(STATE_DESTROYED);

					if (World::getWorld()->isServer())
					{

						// Alle Objekte im Explosionsradius suchen
						hitobj.clear();
						getRegion()->getObjectsInShape(getShape(),&hitobj,getLayer(),WorldObject::CREATURE,0);
						for (i=hitobj.begin();i!=hitobj.end();++i)
						{
							// Schaden austeilen
							(*i)->takeDamage(&m_damage);
						}

						if (m_flags & MULTI_EXPLODES)
						{
							// Flag mehrfach explodierend gesetzt
							DEBUG5("multiexploding");
							Vector dir = getSpeed();
							dir.normalize();
							dir *= getShape()->m_radius;
							
							// Schaden halbieren
							Damage dmg;
							memcpy(&dmg,&m_damage,sizeof(Damage));
							for (int i=0;i<4;i++)
							{
								dmg.m_multiplier[i] *= 0.5;
							}

							// vier neue Projektile erzeugen
							Projectile* pr;
							pr = new Projectile(getSubtype(),&dmg);
							pr->setFlags(Projectile::EXPLODES);
							pr->setMaxRadius(1);
							getRegion()->insertProjectile(pr,getPosition() + dir );
							
							dir.m_x = -dir.m_x;
							pr = new Projectile(getSubtype(),&dmg);
							pr->setFlags(Projectile::EXPLODES);
							pr->setMaxRadius(1);
							getRegion()->insertProjectile(pr,getPosition() + dir);

							dir.m_y = -dir.m_y;
							pr = new Projectile(getSubtype(),&dmg);
							pr->setFlags(Projectile::EXPLODES);
							pr->setMaxRadius(1);
							getRegion()->insertProjectile(pr,getPosition() + dir);

							dir.m_x = -dir.m_x;
							pr = new Projectile(getSubtype(),&dmg);
							pr->setFlags(Projectile::EXPLODES);
							pr->setMaxRadius(1);
							getRegion()->insertProjectile(pr,getPosition() + dir);

						}
					}
				}
				break;

			case STATE_GROWING:
				// Status wachsend behandeln
				handleGrowing(dtime);

				break;

			case STATE_STABLE:
				// Status stabil behandeln
				handleStable(dtime);

				break;

			case STATE_DESTROYED:
				DEBUG5("destroyed");
				setDestroyed(true);
				time =0;
				break;

			case STATE_VANISHING:
				// Status verschwindend
				if (getSubtype() == "FIRE_WALL")
				{
					// Radius reduzieren
					getShape()->m_radius -= 2* dtime/(m_timer_limit);
				}
				if (m_timer >= m_timer_limit)
				{
					setState(STATE_DESTROYED);
				}



			default:
				time =0;
				break;

		}
	}

	if (getSubtype() == "FIRE_WALL" && World::getWorld()->timerLimit(1))
	{
		// Typ Feuersaeule


		// Objekte suchen die die Saeule beruehren
		getRegion()->getObjectsInShape(getShape(),&hitobj,getLayer(),WorldObject::CREATURE,0);
		for (i=hitobj.begin();i!=hitobj.end();++i)
		{
			// Schaden austeilen
			hit = (*i);
			hit->takeDamage(&m_damage);
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
	Vector pos = getPosition(), newpos;
	float rnew,rmin;
	int lid;
	
	Vector speed = getSpeed();

	if (getSubtype() == "GUIDED_ARROW")
	{
		// Lenkpfeil

		// Zielobjekt
		hit =0;

		if (m_goal_object!=0)
		{
			// Zielobjekt ist per ID vorgegeben, Objekt von der Welt holen
			hit = getRegion()->getObject(m_goal_object);
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
			getRegion()->getObjectsInShape(&s,&hitobj,getLayer(), WorldObject::CREATURE,0);

			// alle Objekte als potentielle Ziele loeschen, die dem Erschaffer des Projektils nicht feindlich gesinnt sind
			if (!hitobj.empty())
			{
				i = hitobj.begin();
				hit=(*i);
				while (!hitobj.empty() && World::getWorld()->getRelation(getFraction(),hit) != WorldObject::HOSTILE)
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
				sdir = speed;
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
		float v = speed.getLength();
		
		
		// normieren der Zielrichtung
		newdir.normalize();
		newdir *= v;

		float p =0.3*dtime / 50;
		if (p>1)
			p=1;
		DEBUG5("p = %f",p);
		DEBUG5("newdir %f %f",newdir.m_x,newdir.m_y);
		
		// Neue Richtung ergibt sich aus Linearkombination von aktueller Richtung und Zielrichtung
		speed = speed*(1-p) + newdir*p;
		DEBUG5("new speed %f %f",speed.m_x,speed.m_y);

		// neue Richtung normieren
		speed.normalize();
		speed *= v;

		if (World::getWorld()->timerLimit(0))
		{
			addToNetEventMask(NetEvent::DATA_SPEED);
		}
		
		hitobj.clear();

		setSpeed(speed,false);
		
	}

	if (m_timer >= m_timer_limit)
	{
		DEBUG5("destroyed after timeout");
		setState(STATE_DESTROYED);
	}

	DEBUG5("pos %f %f",pos.m_x, pos.m_y);
	
	// neue Koordinaten nach Ablauf des Zeitquantums
	newpos = pos + getSpeed()*dtime;

	// Linie zwischen alter und neuer Position
	Line line(pos,newpos);


	// Objekt an der aktuellen Position suchen
	getRegion()->getObjectsOnLine(line,&hitobj,getLayer(),WorldObject::CREATURE | WorldObject::FIXED,0);

	// Alle Objekte herausfiltern die verbuendet sind, sowie das zuletzt gerade getroffene Objekt
	if (!hitobj.empty())
	{
		i = hitobj.begin();
		hit = (*i);

		while (!hitobj.empty() && (World::getWorld()->getRelation(getFraction(),hit->getFraction()) == WorldObject:: ALLIED || hit->getId() == m_last_hit_object_id ))
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
				setState(STATE_DESTROYED);
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
			 setState(STATE_EXPLODING);
			 
			if (getSubtype() == "FIRE_BALL" || getSubtype() == "FIRE_ARROW")
				setSubtype("FIRE_EXPLOSION");
			else if (getSubtype() == "ICE_ARROW")
				setSubtype("ICE_EXPLOSION");
			else if (getSubtype() == "WIND_ARROW")
				setSubtype("WIND_EXPLOSION");
			else
				setSubtype("EXPLOSION");

			m_max_radius = 1.5;

			m_timer=0;
			setTimerLimit(200);

			addToNetEventMask(NetEvent::DATA_MAX_RADIUS);
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

		if (hit->isCreature() && bounce)
		{
			Vector speed = getSpeed();
			// Projektil hat ein Lebewesen getroffen, springt weiter
			setState(STATE_FLYING);
			float v = speed.getLength();

			// Kreis mit Radius 5 um aktuelle Position
			Shape s;
			s.m_center = pos;
			s.m_type = Shape::CIRCLE;
			s.m_radius = 5;

			// Alle Objekte im Kreis suchen
			hitobj.clear();
			getRegion()->getObjectsInShape(&s,&hitobj,WorldObject::LAYER_AIR,WorldObject::CREATURE,0);
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
				if (World::getWorld()->getRelation(getFraction(),(*i)) != WorldObject::HOSTILE)
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
				setSpeed(dir *v);

				DEBUG5("dir %f %f",dir.m_x,dir.m_y);
				m_timer =0;

				// bei Kettenblitz Schaden pro Sprung um 20% reduzieren
				if (getSubtype() == "CHAIN_LIGHTNING")
					m_damage.m_multiplier[Damage::AIR] *= 0.8;

			}
			else
			{
				// kein Ziel gefunden, Projektil zerstoeren
				setState(STATE_DESTROYED);
			}

			// Zaehler, bei 5 Spruengen Projektil zerstoeren
			m_counter ++;
			if (m_counter>=5)
			{
				setState(STATE_DESTROYED);
			}
		}

	}
	
	// neue Koordinaten setzen
	setPosition(newpos);

}

void Projectile::handleGrowing(float dtime)
{
	WorldObjectList hitobj;
	WorldObjectList::iterator i;
	WorldObject* hit;
	float rnew,rmin;
	float rold;
	int lid;

	rold = getShape()->m_radius;
	// Radius erhoehen
	getShape()->m_radius += m_max_radius* dtime/(m_timer_limit);

	if ((getSubtype() == "FIRE_WAVE" || getSubtype() == "ICE_RING") && World::getWorld()->isServer())
	{
		// Schaden an die neu getroffenen Lebewesen austeilen
		rnew = getShape()->m_radius;
		
		Shape s;
		s.m_center = getShape()->m_center;
		s.m_type = Shape::CIRCLE;
		s.m_radius = getShape()->m_radius;

		// Alle Objekte suchen die sich in dem Kreis befinden
		getRegion()->getObjectsInShape(getShape(),&hitobj,getLayer(),WorldObject::CREATURE,0);
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
				if (World::getWorld()->getRelation(getFraction(),hit) != WorldObject::HOSTILE)
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
		setState(STATE_DESTROYED);

		if (getSubtype() == "FIRE_WALL")
		{
			// fuer Feuersaeule: Uebergang in stabilen Zustand
			m_timer =0;
			setTimerLimit(5000);
			setState(STATE_STABLE);
		}
	}

}

void Projectile::handleStable(float dtime)
{

	WorldObjectList hitobj;
	WorldObjectList::iterator i;
	WorldObject* hit;

	// aktuelle Position
	Vector pos= getPosition();
	Vector dir;
	Projectile* pr;

	if (m_timer >= m_timer_limit && World::getWorld()->isServer())
	{
		// Timer Limit erreicht
		setState(STATE_DESTROYED);

		if (getSubtype() == "BLIZZARD")
		{

			// Alle Objekte in der Flaeche suchen
			getRegion()->getObjectsInShape(getShape(),&hitobj,getLayer(),WorldObject::CREATURE,0);
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
				setState(STATE_STABLE);

			}
		}

		if (getSubtype() == "FREEZE" || getSubtype() == "STATIC_SHIELD")
		{
			// Alle Objekte in der Flaeche suchen
			getRegion()->getObjectsInShape(getShape(),&hitobj,getLayer(),WorldObject::CREATURE,0);
			for (i=hitobj.begin();i!=hitobj.end();++i)
			{
				// Schaden austeilen
				hit = (*i);
				hit->takeDamage(&m_damage);
			}
		}

		if (getSubtype() ==  "THUNDERSTORM")
		{
			float r = getShape()->m_radius;
			// Punkt in der Flaeche des Zaubers auswuerfeln
			dir = Vector(r, r);

			while (dir.getLength() > r)
			{
				dir.m_x = r*(1 - 2*rand()*1.0/RAND_MAX);
				dir.m_y = r*(1 - 2*rand()*1.0/RAND_MAX);
			}

			// Kreis um den ausgewuerfelten Punkt mit Radius 1.5
			Shape s;
			s.m_center = dir+pos;
			s.m_type = Shape::CIRCLE;
			s.m_radius = 1.5;
			hitobj.clear();

			// Alle Objekte in dem Kreis suchen
			getRegion()->getObjectsInShape(&s,&hitobj,getLayer(),WorldObject::CREATURE,0);

			// Alle nicht feindlichen Objekte aus der Liste entfernen
			if (!hitobj.empty())
			{
				i = hitobj.begin();
				hit = (*i);
				while (i != hitobj.end() && World::getWorld()->getRelation(getFraction(),hit) != WorldObject::HOSTILE)
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
				pr = new Projectile("LIGHTNING",&m_damage, World::getWorld()->getValidId());
				getRegion()->insertProjectile(pr,hit->getShape()->m_center);
			}

			// Gewitter besteht aus 30 Blitzen
			if (m_counter <30)
			{
				// naechster Blitz: Timer zuruecksetzen
				m_counter ++;
				m_timer =0;
				setState(STATE_STABLE);

			}

		}

		if (getSubtype() == "FIRE_WALL")
		{
			// Zauber Feuersaeule
			// Uebergang zum Zustand verschwindend
			m_timer =0;
			setTimerLimit(200);
			setState(STATE_VANISHING);
		}

		if (getSubtype() == "LIGHTNING" || getSubtype() =="LIGHT_BEAM"  || getSubtype() =="ELEM_EXPLOSION" || getSubtype() =="ACID" || getSubtype() =="DIVINE_BEAM"  || getSubtype() =="HYPNOSIS")
		{
			// Objekt an der Stelle suchen an der der Zauber wirkt
			Shape s;
			s.m_center = pos;
			s.m_type = Shape::CIRCLE;
			s.m_radius = 0.5;
			WorldObjectList hits;
			getRegion()->getObjectsInShape( &s,&hits, getLayer() );
			if (!hits.empty())
			{
				hit = hits.front();
			}
			else
			{
				hit =0;
			}
			if (hit !=0)
			{
				// Ziel gefunden, Schaden austeilen
				m_last_hit_object_id = hit->getId();
				hit->takeDamage(&m_damage);
			}
			else
			{
				DEBUG("hit nothing");
			}
		}

	}
}


void Projectile::toString(CharConv* cv)
{
	GameObject::toString(cv);
	
	cv->toBuffer((char) getFraction());
	cv->toBuffer(m_timer);
	cv->toBuffer(m_timer_limit);
	cv->toBuffer(getSpeed().m_x);
	cv->toBuffer(getSpeed().m_y);
	cv->toBuffer(m_flags);
	cv->toBuffer(m_max_radius);
	cv->toBuffer(m_goal_object);

}

void Projectile::fromString(CharConv* cv)
{
	GameObject::fromString(cv);
	
	char tmp;
	cv->fromBuffer<char>(tmp);
	m_fraction = (WorldObject::Fraction) tmp;
	cv->fromBuffer<float>(m_timer);
	cv->fromBuffer<float>(m_timer_limit);
	Vector speed;
	cv->fromBuffer(speed.m_x);
	cv->fromBuffer(speed.m_y);
	setSpeed(speed);
	cv->fromBuffer(m_flags);
	cv->fromBuffer(m_max_radius);
	cv->fromBuffer(m_goal_object);
}

void Projectile::writeNetEvent(NetEvent* event, CharConv* cv)
{
	if (event->m_data & NetEvent::DATA_SPEED)
	{
		cv->toBuffer(getShape()->m_center.m_x);
		cv->toBuffer(getShape()->m_center.m_y);
		cv->toBuffer(getSpeed().m_x);
		cv->toBuffer(getSpeed().m_y);
	}

	if (event->m_data & NetEvent::DATA_GOAL_OBJECT)
	{
		cv->toBuffer(m_goal_object);
	}

	if (event->m_data & NetEvent::DATA_TIMER)
	{
		cv->toBuffer(m_timer_limit);
	}

	if (event->m_data & NetEvent::DATA_MAX_RADIUS)
	{
		cv->toBuffer(m_max_radius);
		cv->toBuffer<float>(getShape()->m_radius);
	}
}

void Projectile::processNetEvent(NetEvent* event, CharConv* cv)
{
	if (event->m_data & NetEvent::DATA_SPEED)
	{
		Vector pos;
		cv->fromBuffer(pos.m_x);
		cv->fromBuffer(pos.m_y);
		setPosition(pos);
		
		Vector speed;
		cv->fromBuffer(speed.m_x);
		cv->fromBuffer(speed.m_y);
		setSpeed(speed);
	}


	if (event->m_data & NetEvent::DATA_GOAL_OBJECT)
	{
		cv->fromBuffer(m_goal_object);
	}

	if (event->m_data & NetEvent::DATA_TIMER)
	{
		cv->fromBuffer(m_timer_limit);
		m_timer =0;
	}

	if (event->m_data & NetEvent::DATA_MAX_RADIUS)
	{
		cv->fromBuffer(m_max_radius);
		cv->fromBuffer<float>(getShape()->m_radius);
	}
}

