#include "event.h"

Trigger::Trigger(TriggerType type)
{
	m_type = type,
	m_lua_variables = "";
}

void Trigger::addVariable(std::string name, int value)
{
	std::stringstream stream;
	stream << name << " = " << value << "\n";
	m_lua_variables += stream.str();
}

void Trigger::addVariable(std::string name, float value)
{
	std::stringstream stream;
	stream << name << " = " << value << "\n";
	m_lua_variables += stream.str();
}

void Trigger::addVariable(std::string name, std::string value)
{
	std::stringstream stream;
	stream << name << " = \"" << value << "\"\n";
	m_lua_variables += stream.str();
}

