#include "fraction.h"

Fraction::Relation Fraction::getRelation(Id id)
{
	std::map<Id, Relation>::iterator it;
	it = m_relations.find(id);
	if (it == m_relations.end())
		return m_relations[DEFAULT];
			
	return it->second;
}

