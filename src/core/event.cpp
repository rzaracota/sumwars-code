#include "event.h"
#include "debug.h"

Trigger::Trigger(TriggerType type)
{
	m_type = type,
	m_lua_variables = "trigger = {} \n";
}

void Trigger::addVariable(std::string name, int value)
{
	std::stringstream stream;
	stream <<"trigger."<< name << " = " << value << "\n";
	m_lua_variables += stream.str();
}

void Trigger::addVariable(std::string name, bool value)
{
	std::stringstream stream;
	stream <<"trigger."<< name << " = ";
	if (value)
	{
		 stream << "true"; 
	}
	else
	{
		 stream << "false"; 
	}
	
	stream<< "\n";
	m_lua_variables += stream.str();
}

void Trigger::addVariable(std::string name, float value)
{
	std::stringstream stream;
	stream <<"trigger."<< name << " = " << value << "\n";
	m_lua_variables += stream.str();
}

void Trigger::addVariable(std::string name, std::string value)
{
	std::stringstream stream;
	stream <<"trigger."<< name << " = \"" << value << "\"\n";
	m_lua_variables += stream.str();
}

Event::~Event()
{
}

