#include "variable.h"

Variable::Variable(int i)
{
	m_type = "int";
	setData(i);
}

Variable::Variable(float f)
{
	m_type = "float";
	setData(f);
}

void Variable::setData(int i)
{
	std::stringstream stream;
	stream << i;
	m_data = stream.str();
	
}

void Variable::setData(float f)
{
	std::stringstream stream;
	stream << f;
	m_data = stream.str();	
}

