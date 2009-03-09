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
	m_topics.insert(std::make_pair(topic,speech));
	
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
				itm->m_price = (int) (itm->m_price * m_cost_multiplier);
				
				// einfuegen und erfolg testen
				if (equ->insertItem(itm,false) == Equipement::NONE)
				{
					delete itm;
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



Dialogue::Dialogue(Region* region, std::string topic_base)
{
	m_id = World::getWorld()->getValidId();
	m_region = region;
	m_topic_base = topic_base;
	m_main_player_id =0;
	m_finished = false;
	
	m_started = true;
	m_trade = false;
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

void Dialogue::addSpeaker(int id, std::string refname)
{
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
		ERRORMSG("cant add %i: %s to dialogue: has already dialogue",id, refname.c_str());
		return;
	}
	
	cr->setDialogue(m_id);
	m_speaker.insert(std::make_pair(refname,id));
	
	if (m_main_player_id ==0 && cr->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_PLAYER)
	{
		m_main_player_id = cr->getId();
		m_speaker["main_player"] = m_main_player_id;
	}
	
}

void Dialogue::speak(std::string refname, std::string text, float time)
{
	CreatureSpeakText txt;
	txt.m_text = text;
	txt.m_time = time;
	
	m_speech.push_back(std::make_pair(refname,txt));
}

void Dialogue::addQuestion(std::string text)
{
	speak("main_player",text,0);
	
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
#ifndef WIN32
		m_speech.back().second.m_answers.push_back(std::make_pair(dgettext("event",text.c_str()),topic));
#else
		m_speech.back().second.m_answers.push_back(std::make_pair(text.c_str(),topic));
#endif
	}
	
	
}

void Dialogue::setTopicBase(std::string topic_base)
{
	m_topic_base = topic_base;
}

bool Dialogue::checkTopic(std::string topic)
{
	if (topic == "start" || topic == "end")
		return true;
	
	Event* st = m_topics[m_topic_base].getSpeakTopic(topic);
	
	if (st !=0)
	{
		return st->checkCondition();
	}
	
	return false;
}

void Dialogue::changeTopic(std::string topic)
{
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
	
	Event* st;
	
	if (topic == "start")
	{
		addQuestion(gettext("which Topic"));
		
		// Alle Antworten hinzufuegen, deren Themen freigeschaltet sind
		
		std::list< std::pair<std::string, std::string> >& lst = m_topics[m_topic_base].getStartTopics();
		std::list< std::pair<std::string, std::string> >::iterator jt;
		for (jt = lst.begin(); jt != lst.end(); ++jt)
		{
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
			
			// Aenderung eingetreten
			WorldObject* wo;
			Creature* cr;
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
			while (cst==0 && !m_speech.empty())
			{
				wo = m_region->getObject( getSpeaker(m_speech.front().first));
				
				if (wo ==0 || !wo->isCreature())
				{
					DEBUG("cant speak text %s %s",m_speech.front().first.c_str(),m_speech.front().second.m_text.c_str());
					m_speech.pop_front();
					continue;
				}
				
				cst = &(m_speech.front().second);

			}
			
			
			
			if (cst ==0 || m_speech.empty())
			{
				m_finished = true;
				return;
			}
			
			DEBUG5("spoken text %s",cst->m_text.c_str());
			
			if (!cst->m_answers.empty() && cst->m_text == "#change_topic#")
			{
				changeTopic(cst->m_answers.front().second);
				return;
			}
			
			// naechsten Text sprechen
			cr = static_cast<Creature*>(wo);
			cr->speakText(*cst);
			
		}
		
	}
	
	if (m_speech.empty() && !m_trade)
	{
		m_finished = true;
	}
}

