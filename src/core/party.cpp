#include "party.h"


void Party::init(char nr_parties, char id)
{
	m_members = new int[nr_parties];
	m_candidates = new int[nr_parties];
	m_relations = new WorldObject::Relation[nr_parties];
	int i;
	for (i=0;i<nr_parties;i++)
	{
		m_members[i]=0;
		m_candidates[i]=0;
		m_relations[i]=WorldObject::NEUTRAL;
		//m_relations[i]=WorldObject::HOSTILE;
	}
	m_relations[id] = WorldObject::ALLIED;
	m_nr_parties = nr_parties;
	m_nr_members =0;
	m_nr_candidates = 0;
	m_id = id;
}


Party::~Party()
{
	delete[] m_members;
	delete[] m_candidates;
	delete[] m_relations;
}

void Party::clear()
{
	int i;
	m_nr_members =0;
	m_nr_candidates = 0;
	for (i=0;i<m_nr_parties;i++)
	{
		m_members[i]=0;
		m_candidates[i]=0;
		m_relations[i]=WorldObject::NEUTRAL;
		//m_relations[i]=WorldObject::HOSTILE;
	}
	m_relations[m_id] = WorldObject::ALLIED;
}

void Party::addMember(int id)
{
	if (m_nr_members == m_nr_parties)
	{
		ERRORMSG("mehr Spieler in der Party als anwesend sein koennen");
		return;
	}
	m_members[m_nr_members] = id;
	m_nr_members++;
}

void Party::addCandidate(int id)
{
	if (m_nr_candidates == m_nr_parties)
	{
		ERRORMSG("mehr Spieler als Bewerber als anwesend sein koennen");
		return;
	}
	m_candidates[m_nr_candidates] = id;
	m_nr_candidates++;
}

void Party::removeMember(int id)
{
	int i;
	for (i=0;i<m_nr_members;i++)
	{
		if (m_members[i]==id)
			break;
	}
	if (i==m_nr_members)
	{
		DEBUG("Spieler nicht in der Party");
	}
	else
	{
		for (;i<m_nr_members-1;i++)
		{
			m_members[i] = m_members[i+1];
		}
		m_members[m_nr_members-1]=0;
		m_nr_members--;
	}
}

void Party::removeCandidate(int id)
{
	int i;
	for (i=0;i<m_nr_candidates;i++)
	{
		if (m_candidates[i]==id)
			break;
	}
	if (i==m_nr_candidates)
	{
		DEBUG("Spieler nicht unter den Bewerbern");
	}
	else
	{
		for (;i<m_nr_candidates-1;i++)
		{
			m_candidates[i] = m_candidates[i+1];
		}
		m_candidates[m_nr_candidates-1]=0;
		m_nr_candidates--;
	}
}


void Party::acceptCandidate(int id)
{
	removeCandidate(id);
	addMember(id);
}

void Party::toString(CharConv* cv)
{
	
	int i;
	cv->toBuffer(m_id);
	cv->toBuffer(m_nr_parties);
	cv->toBuffer(m_nr_members);
	for (i=0;i<m_nr_members;i++)
		cv->toBuffer(m_members[i]);
	cv->toBuffer(m_nr_candidates);
	for (i=0;i<m_nr_candidates;i++)
		cv->toBuffer(m_candidates[i]);
	for (i=0;i<m_nr_parties;i++)
		cv->toBuffer((char) m_relations[i]);

}

void Party::fromString(CharConv* cv)
{
	
	int i;
	cv->fromBuffer<char>(m_id);
	cv->fromBuffer<char>(m_nr_parties);
	m_members = new int[m_nr_parties];
	m_candidates = new int[m_nr_parties];
	m_relations = new WorldObject::Relation[m_nr_parties];
	cv->fromBuffer<char>(m_nr_members);
	for (i=0;i<m_nr_members;i++)
	{
		cv->fromBuffer<int>(m_members[i]);
	}
	cv->fromBuffer<char>(m_nr_candidates);
	for (i=0;i<m_nr_candidates;i++)
	{
		cv->fromBuffer<int>(m_candidates[i]);
	}
	char tmp;
	for (i=0;i<m_nr_parties;i++)
	{
		cv->fromBuffer<char>(tmp);
		m_relations[i] = (WorldObject::Relation) tmp;
	}

}







