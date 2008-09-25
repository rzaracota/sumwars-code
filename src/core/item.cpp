#include "item.h"


string Item::getName()
{
    #ifndef WIN32
        return (gettext((getString()).c_str()));
    #else
        return getString();
    #endif
}


string Item::getString()
{
	return m_subtype;
}


void Item::toString(CharConv* cv)
{

	cv->toBuffer((char) m_type);
	char stmp[11];
	stmp[10] = '\0';
	strncpy(stmp,m_subtype.c_str(),10);
	cv->toBuffer(stmp,10);


}

void Item::fromString(CharConv* cv)
{
	// Daten werden extern eingelesen
}



