#include "dialogue.h"
#include "world.h"
#include "region.h"
#include "creature.h"
#include "itemfactory.h"

std::map<std::string , TopicList > Dialogue::m_topics;

std::map<std::string, NPCTrade > Dialogue::m_npc_trades;


TopicList::~TopicList()
{
	std::map < std::string, Event*>::iterator it;
	for (it = m_topics.begin(); it != m_topics.end(); ++it)
	{
		delete it->second;
	}
}

void TopicList::addTopic(std::string topic, Event* speech)
{
	if (m_topics.count(topic) != 0)
	{
			WARNING("duplicate topic %s",topic.c_str());
	}
	else
	{
		m_topics.insert(std::make_pair(topic,speech));
	}

}

void TopicList::addStartTopic(std::string text, std::string topic)
{
	m_start_topics.push_back(std::make_pair(text,topic));
}

Event* TopicList::getSpeakTopic(std::string topic)
{
	std::map < std::string, Event*>::iterator it;

	it = m_topics.find(topic);
	if (it == m_topics.end())
		return 0;

	return it->second;
}



void NPCTrade::TradeObject::operator=(TradeObject& other)
{
	m_subtype = other.m_subtype;
	m_number = other.m_number;
	m_number_magical = other.m_number_magical;
	m_min_enchant = other.m_min_enchant;
	m_max_enchant = other.m_max_enchant;

}

NPCTrade::NPCTrade()
	:	m_trade_objects()
{
	m_cost_multiplier= 1.0;
	m_refresh_time = 36000000; // 10 min
	m_refresh_timer.start();
}

bool NPCTrade::checkRefresh(Equipement* &equ)
{
	// update, wenn das Inventar noch garnicht existiert oder zu alt ist
	if (equ ==0 || m_refresh_timer.getTime() > m_refresh_time)
	{
		if (equ == 0)
		{
			equ = new Equipement(100,100,100);
		}
		equ->clear();

		Item* itm=0;
		Item::Type type;
		std::list<TradeObject>::iterator it;
		// Schleife ueber alle handelbaren Objekte
		for (it = m_trade_objects.begin(); it != m_trade_objects.end(); ++it)
		{
			// nicht magische Objekte erzeugen
			for (int i=0; i< it->m_number; ++i)
			{
				type = ItemFactory::getBaseType(it->m_subtype);
				itm = ItemFactory::createItem(type,it->m_subtype);
				if (itm != 0)
				{
					itm->m_price = (int) (itm->m_price * m_cost_multiplier);
	
					// einfuegen und erfolg testen
					if (equ->insertItem(itm,false) == Equipement::NONE)
					{
						delete itm;
					}
				}
				else
				{
					ERRORMSG("could not create Item %s",it->m_subtype.c_str());
				}
			}

			// magische Objekte erzeugen
			float magic;
			for (int i=0; i< it->m_number_magical; ++i)
			{
				type = ItemFactory::getBaseType(it->m_subtype);
				magic = Random::randrangef(it->m_min_enchant, it->m_max_enchant);
				itm = ItemFactory::createItem(type,it->m_subtype,0,magic,Item::MAGICAL);
				itm->m_price = (int) (itm->m_price * m_cost_multiplier);

				// einfuegen und erfolg testen
				if (equ->insertItem(itm,false) == Equipement::NONE)
				{
					delete itm;
				}
			}
		}

		return true;
	}
	return false;
}



Dialogue::Dialogue(Region* region, std::string topic_base,int id)
{
	if (id ==0)
	{
		m_id = World::getWorld()->getValidId();
	}
	else
	{
		m_id = id;
	}
	m_region = region;
	m_topic_base = topic_base;
	m_main_player_id =0;
	m_finished = false;

	m_started = true;
	m_trade = false;
	m_nr_players =0;
	
	for (int i=0; i<4; i++)
	{
		m_active_speaker[i] =0;
	}
	m_active = true;
}

Dialogue::~Dialogue()
{
	std::map<std::string, int>::iterator it;

	WorldObject* wo;
	Creature* cr;
	for (it = m_speaker.begin(); it != m_speaker.end(); ++it)
	{
		wo = m_region->getObject(it->second);
		if (wo !=0 && wo->isCreature())
		{
			cr = static_cast<Creature*>(wo);
			cr ->clearSpeakText();
			cr ->setDialogue(0);
		}
	}
}

void Dialogue::init()
{
}

void Dialogue::cleanup()
{
	m_topics.clear();
	m_npc_trades.clear();
}

void Dialogue::addSpeaker(int id, std::string refname, bool force)
{
	if (id ==0)
		return;
	
	// Pruefen, ob die Kreatur existiert
	WorldObject* wo =0;
	wo = m_region->getObject(id);
	if (wo ==0 || !wo->isCreature())
	{
		ERRORMSG("cant add %i: %s to dialogue: not a creature", id, refname.c_str());
		return;
	}

	Creature *cr =  static_cast<Creature*>(wo);
	if (refname =="")
	{
		refname = cr->getRefName();
	}

	// Pruefen, ob die Kreatur nicht schon einen Dialog hat
	if (cr->getDialogueId() != 0)
	{
		if (cr->getDialogueId()!= m_id)
		{
			if (!force)
			{
				ERRORMSG("cant add %i: %s to dialogue: has already dialogue",id, refname.c_str());
				return;
			}
			else
			{
				cr->getDialogue()->removeSpeaker(cr->getId());
				cr->setDialogue(m_id);
			}
		}
	}
	else
	{
		cr->setDialogue(m_id);
	}

	m_speaker.insert(std::make_pair(refname,id));

	if (cr->getType() == "PLAYER")
	{
		if (m_main_player_id ==0 )
		{
			m_main_player_id = cr->getId();
			m_speaker["main_player"] = m_main_player_id;
		}
	}
	
	// ggf Spielerstatus erstellen
	std::map<int,SpeakerState>::iterator it;
	it = m_speaker_state.find(id);
	if (it == m_speaker_state.end())
	{
		SpeakerState& state = m_speaker_state[id];
		state.m_id = id;
		state.m_emotion = "";
		state.m_position = UNKNOWN;
		state.m_visible = false;
		state.m_text_visible = false;
		
		cr->getSpeakText().m_emotion="";
		if (cr->getType() == "PLAYER")
		{
			m_nr_players ++;
		}
	}

}

void Dialogue::removeSpeaker(int id)
{
	DEBUG5("remove %i",id);
	WorldObject* wo =0;
	wo = m_region->getObject(id);
	Creature* cr = dynamic_cast<Creature*>(wo);
	if (cr == 0 || cr->getDialogueId() != m_id)
		return;
	
	setSpeakerPosition(id, NONE);
	
	if (cr->getType() == "PLAYER")
	{
		m_nr_players --;
	}
	
	while (!m_speech.empty() && getSpeaker(m_speech.front().first) == id)
	{
		m_started = true;
		m_speech.pop_front();
	}
	
	// alle Sprecherrollen des Spielers loeschen
	std::map<std::string, int>::iterator it;
	for (it = m_speaker.begin(); it != m_speaker.end(); )
	{
		if (it->second == id)
		{
			m_speaker.erase(it++);
		}
		else
		{
			++it;
		}
	}
	
	// Status des Spielers loeschen
	m_speaker_state.erase(id);
	
	cr->setDialogue(0);
	cr->clearSpeakText();
}

void Dialogue::speak(std::string refname, std::string text, std::string emotion,float time)
{
	CreatureSpeakText txt;
	txt.m_text = text;
	txt.m_time = time;
	txt.m_emotion = emotion;

	m_speech.push_back(std::make_pair(refname,txt));
}

void Dialogue::addQuestion(std::string text)
{
	speak("main_player",text,"");

}

void Dialogue::addAnswer(std::string text, std::string topic)
{
	if (m_speech.empty() || m_speech.back().first != "main_player")
	{
		ERRORMSG("cannot add answer: last speech must be question");
	}

	if (checkTopic(topic))
	{
		DEBUG5("adding answer %s %s",text.c_str(), topic.c_str());
		m_speech.back().second.m_answers.push_back(std::make_pair(dgettext("event",text.c_str()),topic));

	}


}

void Dialogue::setTopicBase(std::string topic_base)
{
	m_topic_base = topic_base;
}

bool Dialogue::checkTopic(std::string topic)
{
	if (topic == "start" || topic == "end" || topic == "abort" || topic =="#change_topic#" || topic =="#jump_topic#")
		return true;

	Event* st = m_topics[m_topic_base].getSpeakTopic(topic);

	if (st !=0)
	{
		DEBUG5("checking Topic %s",topic.c_str());
		return st->checkCondition();
	}

	return false;
}

void Dialogue::changeTopic(std::string topic)
{
	if (topic =="abort")
	{
		if (m_topic == "start")
		{
			topic = "end";
		}
		else
		{
			return;
		}
	}
	
	
	if (topic == "end")
	{
		// Gespraech beenden
		m_finished = true;

		if (m_trade)
		{
			Creature* player = dynamic_cast<Creature*>( m_region->getObject(m_main_player_id));
			if (player != 0)
			{
				player->setTradePartner(0);
			}
		}
		return ;
	}

	m_speech.clear();
	m_started = true;

	m_topic = topic;
	
	Event* st;

	if (topic == "start")
	{
		Creature* npc = dynamic_cast<Creature*>( m_region->getObject( getSpeaker(m_topic_base) ) );
		if (npc ==0)
		{
			addQuestion(gettext("which Topic"));
		}
		else
		{
			addQuestion(npc->getRefName());
		}

		// Alle Antworten hinzufuegen, deren Themen freigeschaltet sind

		std::list< std::pair<std::string, std::string> >& lst = m_topics[m_topic_base].getStartTopics();
		std::list< std::pair<std::string, std::string> >::iterator jt;
		for (jt = lst.begin(); jt != lst.end(); ++jt)
		{
			if (jt->second == "start_dialogue")
			{
				st = m_topics[m_topic_base].getSpeakTopic(jt->second);
				if (st->checkCondition())
				{
					EventSystem::setRegion(m_region);
					st->doEffect();
				}
				continue;
			}
			if (checkTopic(jt->second))
			{
				DEBUG5("added answer %s %s",jt->first.c_str(),jt->second.c_str());
				addAnswer(jt->first, jt->second);
			}
		}
		return;
	}

	if (topic == "trade")
	{
		// Pruefen, dass der NPC wirklich handeln kann
		if (!canTrade(m_topic_base))
		{
			m_finished = true;
			return ;
		}

		// Handelspartner und Basisinformationen einholen
		NPCTrade& tradeinfo = getNPCTrade(m_topic_base);
		Creature* npc = dynamic_cast<Creature*>( m_region->getObject( getSpeaker(m_topic_base) ) );
		Creature* player = dynamic_cast<Creature*>( m_region->getObject(m_main_player_id));

		if (npc==0 || player ==0)
		{
			m_finished = true;
			return ;
		}

		// evtl Inventar auffrischen
		Equipement* equ = npc->getEquipement();
		tradeinfo.checkRefresh(equ);

		if (npc->getEquipement() ==0)
		{
			npc->setEquipement(equ);
		}

		m_trade = true;


		NetEvent event;
		event.m_type =  NetEvent::TRADER_INVENTORY_REFRESH;
		event.m_data = 0;
		event.m_id = npc->getId();

		m_region->insertNetEvent(event);


		// Handelspartner setzen
		npc->setTradePartner(player->getId());
		npc->getTradeInfo().m_price_factor = tradeinfo.m_cost_multiplier;
		player->setTradePartner(npc->getId());
	}

	st = m_topics[m_topic_base].getSpeakTopic(topic);

	if (st ==0)
	{
		ERRORMSG("topic %s:%s existiert nicht",m_topic_base.c_str(),topic.c_str());
		m_finished = true;
		return ;
	}

	if (st->checkCondition())
	{
		EventSystem::setRegion(m_region);
		st->doEffect();
	}

}

int Dialogue::getSpeaker(std::string refname)
{
	std::map<std::string, int>::iterator it;
	it = m_speaker.find(refname);
	if (it == m_speaker.end())
		return 0;

	return it->second;
}

Dialogue::Position Dialogue::calcSpeakerPosition(int id)
{
	WorldObject* wo =0;
	wo = m_region->getObject(id);
	if (wo ==0 || !wo->isCreature())
	{
		ERRORMSG("Speaker %i is not a creature", id);
		return Dialogue::UNKNOWN;
	}
	
	Position base = UPPER_RIGHT;
	Position alt = UPPER_LEFT;
	
	if (wo->getType() == "PLAYER")
	{
		base = LOWER_LEFT;
		alt = LOWER_RIGHT;
	}
	
	// zuerst Basisposition bevorzugen
	// zuerst unbelegte Positionen Bevorzugen
	if (m_active_speaker[base] == 0)
		return base;
	
	if (m_active_speaker[alt] == 0)
		return alt;
	
	if (m_speaker_state[m_active_speaker[base]].m_text_visible == false)
		return base;
	
	return alt;
}

void Dialogue::setSpeakerPosition(int id, Position pos)
{
	if (pos<NONE || pos>AUTOMATIC || pos == UNKNOWN || m_speaker_state.count(id) == 0)
		return;
	
	m_event_mask =1;
	
	if (pos == AUTOMATIC)
		pos = calcSpeakerPosition(id);
	
	// Spieler entfernen
	// bisherige Position ermitteln:
	Position oldpos = m_speaker_state[id].m_position;
	if (oldpos != UNKNOWN && pos != oldpos)
	{
		m_active_speaker[oldpos] =0;
	}
	
	// Sprecher der bisher auf der Position pos war verdraengen
	if (pos != NONE)
	{
		int prev_speaker = m_active_speaker[pos];
		if (prev_speaker != 0 && prev_speaker != id)
		{
			m_speaker_state[prev_speaker].m_visible = false;
			m_speaker_state[prev_speaker].m_text_visible = false;
		}
					
		// neuen Sprecher aktivieren
		m_speaker_state[id].m_visible = true;
		m_speaker_state[id].m_position = pos;
		m_active_speaker[pos] = id;
	}
	else
	{
		m_speaker_state[id].m_visible = false;
		m_speaker_state[id].m_text_visible = false;
	}
	
	DEBUG5("set speaker %i at position %i",id,pos);
	
}

Dialogue::SpeakerState* Dialogue::getSpeakerState(Position pos)
{
	if (pos<0 || pos >=4)
		return 0;
	
	if (m_active_speaker[pos] == 0)
		return 0;
	
	return &(m_speaker_state[m_active_speaker[pos]]);
}

void Dialogue::update(float time)
{

	if (m_finished)
	{

		return;
	}

	float stime = time;
	CreatureSpeakText* cst;

	// Zeitverlauf berechnen
	while (stime >0 && !m_speech.empty())
	{
		cst = &(m_speech.front().second);

		// Fragen bleiben generell stehen
		// Handel ebenso
		if (!m_started && ! cst->m_answers.empty() || m_trade)
		{
			stime =0;
			break;
		}

		if (!m_started && cst->m_time > stime)
		{
			cst->m_time -= stime;
			stime =0;
		}
		else
		{
			
			// bei inaktivem Dialog keine neuen Saetze erzeugen
			if (!m_active)
				return;
			
			m_player_skips.clear();
			
			// Aenderung eingetreten
			WorldObject* wo=0;
			Creature* cr=0;
			if (!m_started)
			{
				stime -= cst->m_time;

				// aktuellen Sprecher ermitteln
				wo = m_region->getObject( getSpeaker(m_speech.front().first));

				// fuer diesen Specher aktuellen Text zuruecksetzen
				if (wo !=0 && wo->isCreature())
				{
					cr = static_cast<Creature*>(wo);
					cr->clearSpeakText();
				}

				m_speech.pop_front();
			}
			m_started = false;

			// Naechsten zulaessigen Text suchen
			cst =0;
			int id;
			Position pos=UNKNOWN;
			while (cst==0 && !m_speech.empty())
			{
				// Change Topic braucht keinen Sprecher
				if (m_speech.front().second.m_text == "#change_topic#")
				{
					cst = &(m_speech.front().second);
					break;
				}
				
				if (m_speech.front().second.m_text == "#execute#")
				{
					EventSystem::doString(m_speech.front().second.m_emotion.c_str());
					m_speech.pop_front();
					continue;
				}
				
				id = getSpeaker(m_speech.front().first);
				wo = m_region->getObject(id );
				cr = static_cast<Creature*>(wo);

				if (cr ==0 && m_speech.front().first!="nobody")
				{
					DEBUG5("cant speak text %s %s",m_speech.front().first.c_str(),m_speech.front().second.m_text.c_str());
					m_speech.pop_front();
					continue;
				}
				
				cst = &(m_speech.front().second);
				
				if (cst->m_text == "#emotion#")
				{
					cr->getSpeakText().m_emotion = cst->m_emotion;
					m_speaker_state[id].m_emotion = cst->m_emotion;
					m_event_mask =1;
					m_speech.pop_front();
					cst =0;
					continue;
				}
				
				if (cst->m_text == "#position#")
				{
 					std::string posstr = cst->m_emotion;
					Position pos = UNKNOWN;
					if (posstr == "none")
						pos = NONE;
					else if (posstr == "upper_left")
						pos = UPPER_LEFT;
					else if (posstr == "upper_right")
						pos = UPPER_RIGHT;
					else if (posstr == "lower_left")
						pos = LOWER_LEFT;
					else if (posstr == "lower_right")
						pos = LOWER_RIGHT;
					
					setSpeakerPosition(id,pos);
					
					m_event_mask =1;
					cst =0;
					m_speech.pop_front();
					continue;
				}
				
				// Springt ein Topic an, setzt danach aber mit dem alten fort
				if (!cst->m_answers.empty() && cst->m_text == "#jump_topic#")
				{
					Event* st;
				
					st = m_topics[m_topic_base].getSpeakTopic(cst->m_answers.front().second);
					if (st->checkCondition())
					{
						EventSystem::setRegion(m_region);
						st->doEffect();
					}
				
					m_speech.pop_front();
					cst =0;
					continue;
				}

				
				// Falls die Position des Spielers im Dialog noch unbekannt: Position berechnen
				pos = m_speaker_state[id].m_position;
				if (pos == UNKNOWN)
				{
					pos = calcSpeakerPosition(id);
				}
				
				if (pos == UNKNOWN)
				{
					continue;
				}
				
				
			}

			if (cst ==0 || m_speech.empty())
			{
				m_finished = true;
				return;
			}

			// geht direkt zum naechsten Topic
			if (!cst->m_answers.empty() && cst->m_text == "#change_topic#")
			{
				changeTopic(cst->m_answers.front().second);
				return;
			}
			else if (cst->m_text == "#change_topic#")
			{
				DEBUG("change topic with empty topic list");
				return;
			}

			// naechsten Text sprechen
			DEBUG5("spoken text %s",cst->m_text.c_str());

			if (cr !=0)
			{
				cr->speakText(*cst);
				
				setSpeakerPosition(id,pos);
				for (int i=0; i<4; i++)
				{
					if (m_active_speaker[i] != 0)
					{
						if (i== pos)
						{
							m_speaker_state[m_active_speaker[i]].m_text_visible = true;
						}
						else
						{
							m_speaker_state[m_active_speaker[i]].m_text_visible = false;
						}
					}
				}
				
				// Emotion setzen
				if (cst->m_emotion != "")
				{
					m_speaker_state[id].m_emotion = cst->m_emotion;
				}
				
				cr->getSpeakText().m_emotion = m_speaker_state[id].m_emotion;
				m_event_mask =1;
			}

		}

	}

	if (m_speech.empty() && !m_trade)
	{
		m_finished = true;
	}
}

void Dialogue::toString(CharConv* cv)
{
	// nur die relevanten Daten werden geschrieben: die aktuell sichtbaren 4 Spieler
	cv->toBuffer(m_id);
	for (int i=0; i<4; i++)
	{
		cv->toBuffer(m_active_speaker[i]);
		if (m_active_speaker[i] != 0)
		{
			SpeakerState& state = m_speaker_state[m_active_speaker[i]];
			cv->toBuffer(state.m_id);
			cv->toBuffer(state.m_emotion);
			cv->toBuffer<short>(state.m_position);
			cv->toBuffer(state.m_visible);
			cv->toBuffer(state.m_text_visible);
		}
	}
}

void Dialogue::fromString(CharConv* cv)
{
	for (int i=0; i<4; i++)
	{
		cv->fromBuffer(m_active_speaker[i]);
		if (m_active_speaker[i] != 0)
		{
			short tmp;
			SpeakerState& state = m_speaker_state[m_active_speaker[i]];
			cv->fromBuffer(state.m_id);
			cv->fromBuffer(state.m_emotion);
			cv->fromBuffer<short>(tmp);
			state.m_position = (Position) tmp;
			cv->fromBuffer(state.m_visible);
			cv->fromBuffer(state.m_text_visible);
		}
	}
}

void Dialogue::skipText(int id)
{
	m_player_skips.insert(id);
	DEBUG5("players %i skips %i",m_nr_players, m_player_skips.size());
	if ((int) m_player_skips.size() == m_nr_players)
	{
		// aktuellen Text ueberspringen
		CreatureSpeakText* cst;
		if (!m_speech.empty())
		{
			cst = &(m_speech.front().second);
			// Fragen und Handel kann nicht so uebersprungen werden
			if (!cst->m_answers.empty() || m_trade)
				return;
			
			cst->m_time =0;
		}
	}
}

