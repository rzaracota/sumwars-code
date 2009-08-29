#include "party.h"
#include "world.h"
#include "player.h"

void Party::init(int id)
{
	
	m_leader_id =0;
	m_id = id;
	m_relations[id] = Fraction::ALLIED;
}


Party::~Party()
{
	clear();
}

void Party::clear()
{
	m_members.clear();
	m_candidates.clear();
	m_relations.clear();
	
	m_leader_id =0;
	
	std::map<short,Minimap*>::iterator it;
	for (it = m_minimaps.begin(); it != m_minimaps.end(); ++it)
	{
		delete it->second;
	}
	m_minimaps.clear();

}

void Party::updateMinimaps()
{
	WorldObject* wo;
	Minimap* mm;
	std::set<int>::iterator it;

	int rid;
	
	for (it = m_members.begin(); it != m_members.end(); ++it)
	{
		wo = World::getWorld()->getPlayer(*it);
		// nur aktive Spieler
		if (wo !=0 && wo->getRegion()!=0 && wo->getState() == WorldObject::STATE_ACTIVE )
		{
			rid = wo->getRegion()->getId();
			mm = getMinimap(rid);
			
			mm->update(wo->getShape()->m_center);
		}
	}
}

Minimap* Party::getMinimap(short rid)
{
	DEBUG5("get Minimap for %i",rid);
	std::map<short,Minimap*>::iterator mt;
	
	mt = m_minimaps.find(rid);
			
	if (mt == m_minimaps.end())
	{
		// Minimap noch nicht vorhanden -> erzeugen
		DEBUG("creating minimap for region %i",rid);
		mt = m_minimaps.insert(std::make_pair(rid,World::getWorld()->getRegion(rid)->createMinimap())).first;
	}
	return mt->second;
}

void Party::addMember(int id)
{
	Player* player = static_cast<Player*>(World::getWorld()->getPlayer(id));
	if (player ==0)
		return;
	
	DEBUG5("adding member %i to party %i",id, m_id);
	if (m_members.empty())
	{
		m_leader_id = id;
	}
	m_members.insert(id);
	
	Party* p = player->getParty();
	if (p!=0 && p!=this)
	{
		while (!p->getCandidates().empty())
		{
			p->removeCandidate(*(p->getCandidates().begin()));
		}
		
		// Sichtbereich des neuen Spielers mit dem der Party vereinigen 
		std::map<short,Minimap*>& othermmaps = p->getMinimaps();
		std::map<short,Minimap*>::iterator mt;
		Minimap* mmap;
		for (mt = othermmaps.begin(); mt != othermmaps.end(); ++mt)
		{
			
			mmap = getMinimap(mt->first);
			mmap->merge(*(mt->second));
		}
	}
	
	
	
	
	player->setFraction(getId());
	
	if (World::getWorld()->isServer())
	{
		
		NetEvent event;
		event.m_id = id;
		event.m_type = NetEvent::PLAYER_PARTY_CHANGED;
		
		World::getWorld()->insertNetEvent(event);
	}
	
	
}

void Party::addCandidate(int id)
{
	DEBUG5("adding candidate %i to party %i",id, m_id);
	Player* pl = static_cast<Player*>( World::getWorld()->getPlayer(id));
	if (pl !=0)
	{
		pl->setCandidateParty(getId());
	}
	m_candidates.insert(id);
	
	if (World::getWorld()->isServer())
	{
		
		NetEvent event;
		event.m_id = id;
		event.m_type = NetEvent::PLAYER_PARTY_CANDIDATE;
		
		World::getWorld()->insertNetEvent(event);
	}
}

void Party::removeMember(int id)
{
	DEBUG5("removing member %i from party %i",id, m_id);

	m_members.erase(id);
	DEBUG5("number of members %i",m_members.size());
	if (id == m_leader_id)
	{
		m_leader_id = *(m_members.begin());
	}
}

void Party::removeCandidate(int id)
{	
	DEBUG5("removing candidate %i from party %i",id, m_id);
	Player* pl = static_cast<Player*>( World::getWorld()->getPlayer(id));
	if (pl !=0)
	{
		pl->setCandidateParty(-1);
	}
	m_candidates.erase(id);
}


void Party::acceptCandidate(int id)
{
	removeCandidate(id);
	addMember(id);
}

void Party::setRelation(int id, Fraction::Relation rel)
{
	m_relations[id] = rel;
	
	if (World::getWorld()->isServer())
	{
		
		NetEvent event;
		event.m_id = id;
		event.m_data = getId();
		event.m_type = NetEvent::PARTY_RELATION_CHANGED;
		
		World::getWorld()->insertNetEvent(event);
	}
}

void Party::toString(CharConv* cv)
{
	
	cv->toBuffer<char>(m_id);
	cv->toBuffer<char>(getNrMembers());
	
	std::set<int>::iterator it;
	for (it = m_members.begin(); it != m_members.end(); ++it)
	{
		cv->toBuffer(*it);
	}
	
	cv->toBuffer<char>(getNrCandidates());
	for (it = m_candidates.begin(); it != m_candidates.end(); ++it)
	{
		cv->toBuffer(*it);
	}
	
	cv->toBuffer<char>(m_relations.size());
	std::map<int, Fraction::Relation>::iterator jt;
	for (jt = m_relations.begin(); jt != m_relations.end(); ++jt)
	{
		cv->toBuffer<char>(jt->first);
		cv->toBuffer<char>(jt->second);
	}

}

void Party::fromString(CharConv* cv)
{
	char mid;
	cv->fromBuffer<char>(mid);
	m_id = mid;
	char nr_member, nr_cand, nr_rel;
	int id;
	
	
	cv->fromBuffer<char>(nr_member);
	std::set<int>::iterator it;
	for (int i=0; i<nr_member; i++)
	{
		cv->fromBuffer(id);
		addMember(id);
	}
	
	cv->fromBuffer<char>(nr_cand);
	for (int i=0; i<nr_cand; i++)
	{
		cv->fromBuffer(id);
		addCandidate(id);
	}
	
	cv->fromBuffer<char>(nr_rel);
	std::map<int, Fraction::Relation>::iterator jt;
	char rel, pid;
	for (int i=0; i<nr_rel; i++)
	{
		cv->fromBuffer<char>(pid);
		cv->fromBuffer<char>(rel);
		m_relations[pid] = (Fraction::Relation) rel;
		
	}
}







