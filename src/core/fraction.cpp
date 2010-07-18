#include "fraction.h"



Fraction::Relation Fraction::getRelation(Id id)
{
	std::map<Id, Relation>::iterator it;
	it = m_relations.find(id);
	if (it == m_relations.end())
		return m_relations[DEFAULT];
			
	return it->second;
}

void Fraction::toString(CharConv* cv)
{
	cv->toBuffer(m_id);
	cv->toBuffer(m_type);
	cv->toBuffer(m_name);
	
	cv->toBuffer(static_cast<int>(m_relations.size()));
	
	
	std::map<Id, Relation>::iterator it;
	for (it = m_relations.begin(); it != m_relations.end(); ++it)
	{
		cv->toBuffer(it->first);
		cv->toBuffer(static_cast<char>(it->second));
	}
}

void Fraction::fromString(CharConv* cv)
{
	cv->fromBuffer(m_name);
	
	int nr;
	cv->fromBuffer(nr);
	char tmp;
	Fraction::Id id;
	for (int i=0; i<nr; i++)
	{
		cv->fromBuffer(id);
		cv->fromBuffer(tmp);
		m_relations[id] = (Relation) tmp;
	}
}

