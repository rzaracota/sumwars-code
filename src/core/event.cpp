#include "event.h"

LogicalAnd::~LogicalAnd()
{
	std::list<LogicalExpr*>::iterator it;
	for (it = m_expressions.begin(); it != m_expressions.end(); ++it)
	{
		delete *it;
	}
}


bool LogicalAnd::eval()
{
	std::list<LogicalExpr*>::iterator it;
	for (it = m_expressions.begin(); it != m_expressions.end(); ++it)
	{
		if (!(it->eval()))
		{
			return false;
		}
	}
	
	return true;
}

LogicalOr::~LogicalOr()
{
	std::list<LogicalExpr*>::iterator it;
	for (it = m_expressions.begin(); it != m_expressions.end(); ++it)
	{
		delete *it;
	}
}


bool LogicalOr::eval()
{
	std::list<LogicalExpr*>::iterator it;
	for (it = m_expressions.begin(); it != m_expressions.end(); ++it)
	{
		if (it->eval())
		{
			return true;
		}
	}
	
	return false;
}
