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

VariableRef::VariableRef()
{
	m_type = "";
	m_data_ptr = 0;
}

VariableRef::VariableRef(int &i)
{
	m_type = "int";
	m_data_ptr = (void*) &i;
}

VariableRef::VariableRef(short &s)
{
	m_type = "short";
	m_data_ptr = (void*) &s;
}

VariableRef::VariableRef(char &c)
{
	m_type = "char";
	m_data_ptr = (void*) &c;
}

VariableRef::VariableRef(float &f)
{
	m_type = "float";
	m_data_ptr = (void*) &f;
}

VariableRef::VariableRef(std::string &s)
{
	m_type = "string";
	m_data_ptr = (void*) &s;
}

VariableRef::VariableRef(Variable &v)
{
	m_type = "var";
	m_data_ptr = (void*) &v;
}

void VariableRef::setVariable(int &i)
{
	m_type = "int";
	m_data_ptr = (void*) &i;
}

void VariableRef::setVariable(short &s)
{
	m_type = "short";
	m_data_ptr = (void*) &s;
}

void VariableRef::setVariable(char &c)
{
	m_type = "char";
	m_data_ptr = (void*) &c;
}

void VariableRef::setVariable(float &f)
{
	m_type = "float";
	m_data_ptr = (void*) &f;
}

void VariableRef::setVariable(std::string &s)
{
	m_type = "string";
	m_data_ptr = (void*) &s;
}

void VariableRef::setVariable(Variable &v)
{
	m_type = "var";
	m_data_ptr = (void*) &v;
}


void VariableRef::operator=(std::string& s)
{
	if (!isValid())
	{
		ERRORMSG("Tried to use invalid reference");
		return;
	}
	
	if (m_type == "string")
	{
		*(static_cast<std::string*>(m_data_ptr)) = s;
	}
	else if (m_type == "var")
	{
		static_cast<Variable*>(m_data_ptr)->setData(s);
	}
	else
	{
		std::stringstream stream(s);
		if (m_type =="int")
		{
			stream >> *(static_cast<int*>(m_data_ptr));
		}
		else if (m_type =="short")
		{
			stream >> *(static_cast<short*>(m_data_ptr));
		}
		else if (m_type =="float")
		{
			stream >> *(static_cast<float*>(m_data_ptr));
		}
		else if (m_type =="char")
		{
			int i;
			stream >> i;
			*(static_cast<char*>(m_data_ptr)) = (char) i;
		}
		else
		{
			ERRORMSG("unknown data type %s",m_type.c_str());
		}
		
		if (stream.fail())
		{
			ERRORMSG("failed to read datatype %s from string %s",m_type.c_str(), s.c_str());
		}
		
	}
}

std::string VariableRef::print()
{
	if (!isValid())
	{
		ERRORMSG("Tried to use invalid reference");
		return "";
	}
	
	if (m_type == "string")
	{
		return *(static_cast<std::string*>(m_data_ptr));
	}
	else if (m_type == "var")
	{
		return static_cast<Variable*>(m_data_ptr)->getData();
	}
	else
	{
		std::stringstream stream("");
		if (m_type =="int")
		{
			stream << *(static_cast<int*>(m_data_ptr));
		}
		else if (m_type =="short")
		{
			stream << *(static_cast<short*>(m_data_ptr));
		}
		else if (m_type =="float")
		{
			stream << *(static_cast<float*>(m_data_ptr));
		}
		else if (m_type =="char")
		{
			int i = (int) *(static_cast<char*>(m_data_ptr));
			stream << i;
		}
		else
		{
			ERRORMSG("unknown data type %s",m_type.c_str());
		}
		
		if (stream.fail())
		{
			ERRORMSG("failed to write datatype %s",m_type.c_str());
		}
		return stream.str();
		
	}
}

