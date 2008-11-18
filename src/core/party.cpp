#include "party.h"
#include "world.h"
#include "player.h"

void Party::init(int id)
{
	
	m_leader_id =0;
	m_id = id;
	m_relations[id] = WorldObject::ALLIED;
}


Party::~Party()
{
}

void Party::clear()
{
	m_members.clear();
	m_candidates.clear();
	m_relations.clear();
	
	m_leader_id =0;
}

void Party::addMember(int id)
{
	WorldObject* player = World::getWorld()->getPlayer(id);
	if (player ==0)
		return;
	
	DEBUG("adding member %i to party %i",id, m_id);
	if (m_members.empty())
	{
		m_leader_id = id;
	}
	m_members.insert(id);
	
	player->setFraction((WorldObject::Fraction) (getId() + WorldObject::FRAC_PLAYER_PARTY));
	
	if (World::getWorld()->isServer())
	{
		
		Event event;
		event.m_id = id;
		event.m_type = Event::PLAYER_PARTY_CHANGED;
		
		World::getWorld()->insertEvent(event);
	}
	
	
}

void Party::addCandidate(int id)
{
	DEBUG("adding candidate %i to party %i",id, m_id);
	Player* pl = static_cast<Player*>( World::getWorld()->getPlayer(id));
	if (pl !=0)
	{
		pl->setCandidateParty(getId());
	}
	m_candidates.insert(id);
	
	if (World::getWorld()->isServer())
	{
		
		Event event;
		event.m_id = id;
		event.m_type = Event::PLAYER_PARTY_CANDIDATE;
		
		World::getWorld()->insertEvent(event);
	}
}

void Party::removeMember(int id)
{
	DEBUG("removing member %i from party %i",id, m_id);

	m_members.erase(id);
	DEBUG("number of members %i",m_members.size());
}

void Party::removeCandidate(int id)
{	
	DEBUG("removing candidate %i from party %i",id, m_id);
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

void Party::setRelation(int id, WorldObject::Relation rel)
{
	m_relations[id] = rel;
	
	if (World::getWorld()->isServer())
	{
		
		Event event;
		event.m_id = id;
		event.m_data = getId();
		event.m_type = Event::PARTY_RELATION_CHANGED;
		
		World::getWorld()->insertEvent(event);
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
	std::map<int, WorldObject::Relation>::iterator jt;
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
	std::map<int, WorldObject::Relation>::iterator jt;
	char rel, pid;
	for (int i=0; i<nr_rel; i++)
	{
		cv->fromBuffer<char>(pid);
		cv->fromBuffer<char>(rel);
		m_relations[pid] = (WorldObject::Relation) rel;
		
	}
}







