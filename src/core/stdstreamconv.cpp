#include "stdstreamconv.h"



void StdStreamConv::toBuffer(std::string s)
{
	// empty string and strings with spaces must be modified to be able to retrieve correctly
	// empty string -> "#empty#"
	// space -> "#~#"
	// Lets hope nobody tries to send a string with #~# inside
	if (s=="")
	{
		(*m_stream) << "#empty# ";
	}
	else
	{
		int pos = s.find(' ');
		while (pos != std::string::npos)
		{
			s.replace(pos,1,"#~#");
			pos = s.find(' ',pos+1);
		}
		(*m_stream) << s << " ";
	}
}

void StdStreamConv::fromBuffer(std::string& s)
{
	(*m_stream) >> s ;
	if (s == "#empty#")
	{
		s="";
	}
		
	int pos = s.find("#~#");
	while (pos != std::string::npos)
	{
		s.replace(pos,3," ");
		pos = s.find("#~#",pos+1);
	}

}

