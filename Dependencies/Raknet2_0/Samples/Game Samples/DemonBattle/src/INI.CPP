#include "ini.h"

#include <assert.h>

//#include "MemoryManager.h"

BOOL CIniFile::WriteString (std::string section,std::string key,std::string value)
{
	return (WritePrivateProfileString ( section.c_str(), key.c_str(),value.c_str(),m_strFileName.c_str()));
}


BOOL CIniFile::GetIntValue (int& value,std::string section,std::string key,int def)
{
	value = GetPrivateProfileInt ( section.c_str(), key.c_str(), def, m_strFileName.c_str());
	return TRUE;
}

BOOL CIniFile::GetStringValue (std::string& buffer,std::string section,std::string key)
{
	char* cBuffer = new char[256];
	
	int read=GetPrivateProfileString (section.c_str(),key.c_str(), NULL, cBuffer, 255, m_strFileName.c_str());
	buffer = cBuffer;

	delete []cBuffer;

	if (!read)
	{
//		g_toolz.ErrorBox ("Fehler beim lesen aus der Inifile :");
//		g_toolz.ErrorBox (section.c_str());
//		g_toolz.ErrorBox (key.c_str());
		assert (0);
	}
	return read != 0;
}
