Fraction:: getRelation(int id)
{
	std::map<int, Relation>::iterator it;
	it = m_relations.find(id);
	if (it == m_relations.end())
		return NEUTRAL;
			
	return it->second;
}