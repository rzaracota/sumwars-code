#include "dialogue.h"
#include "world.h"
#include "region.h"
#include "creature.h"

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

std::map<std::string , TopicList > Dialogue::m_topics;





Dialogue::Dialogue(Region* region, std::string topic_base)
{
	m_id = World::getWorld()->getValidId();
	m_region = region;
	m_topic_base = topic_base;
	m_main_player_id =0;
	m_finished = false;
	
	m_started = true;
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
	Event* top;
			
	top = new Event;
	top->setEffect("speak('peasant', 'test1',800) ; speak('player','antwort1',800)");
	getTopicList("peasant").addTopic("topic1",top);
	getTopicList("peasant").addStartTopic("das erste Thema", "topic1");
	
	top = new Event;
	top->setEffect(" neues =0; \
			speak('player', 'frage2',1000) ; \
			speak('peasant','antwort2.1',1000); \
			speak('peasant','antwort2.2',1000); \
			addQuestion('und jetzt?'); \
			addAnswer('nochmal das ganze','start'); \
			addAnswer('was neues','topic3'); \
			addAnswer('was ganz neues ?','topic4'); \
			addAnswer('ciao','end'); ");
	getTopicList("peasant").addTopic("topic2",top);
	getTopicList("peasant").addStartTopic("dumme Fragen stellen", "topic2");
	
	top = new Event;
	top->setEffect("speak('player', 'Gibts was neues?',1000) \
			speak('peasant','es gibt nix neues',1000); \
			speak('peasant','',1000); \
			speak('peasant','echt nicht',1000); \
			changeTopic('start'); \
			neues = 1");
	getTopicList("peasant").addTopic("topic3",top);
			
				
	
	top = new Event;
	top->setCondition("return (neues==1)");
	top->setEffect("speak('player', 'Und Jetzt?',1000) \
			speak('peasant','du nervst!',1000)");
	getTopicList("peasant").addTopic("topic4",top);
	
	
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
	if (cr->getDialogue() != 0)
	{
		ERRORMSG("cant add %i: %s to dialogue: has already dialogue",id, refname.c_str());
		return;
	}
	
	cr->setDialogue(m_id);
	m_speaker.insert(std::make_pair(refname,id));
	
	if (m_main_player_id ==0 && cr->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_PLAYER)
	{
		m_main_player_id = cr->getId();
	}
	
}

void Dialogue::speak(std::string refname, std::string text, float time)
{
	CreatureSpeakText txt;
	txt.m_text = text;
	txt.m_time = 500;
	
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
		m_speech.back().second.m_answers.push_back(std::make_pair(text,topic));
	}
	
	
}

bool Dialogue::checkTopic(std::string topic)
{
	
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
		return ;
	}
	
	Event* st;
	
	if (topic == "start")
	{
		addQuestion("");
		
		// Alle Antworten hinzufuegen, deren Themen freigeschaltet sind
		
		std::list< std::pair<std::string, std::string> >& lst = m_topics[m_topic_base].getStartTopics();
		std::list< std::pair<std::string, std::string> >::iterator jt;
		for (jt = lst.begin(); jt != lst.end(); ++jt)
		{
			if (checkTopic(jt->second))
			{
				addAnswer(jt->first, jt->second);
			}
		}
		return;
	}
	
	st = m_topics[m_topic_base].getSpeakTopic(topic);
	
	if (st ==0)
	{
		ERRORMSG("topic %s:%s existiert nicht",m_topic_base.c_str(),topic.c_str());
		m_finished = true;
		return ;
	}
	
	if (!st->checkCondition())
	{
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
		return;
	
	float stime = time;
	CreatureSpeakText* cst;
	
	// Zeitverlauf berechnen
	while (stime >0 && !m_speech.empty())
	{
		cst = &(m_speech.front().second);
		
		// Fragen bleiben generell stehen
		if (! cst->m_answers.empty())
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
			
			// Naechsten zulaessigen Text suchen
			cst =0;
			while (cst==0 && !m_speech.empty())
			{
				wo = m_region->getObject( getSpeaker(m_speech.front().first));
				
				if (wo ==0 || !wo->isCreature())
				{
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
	
	if (m_speech.empty())
	{
		m_finished = true;
	}
}

