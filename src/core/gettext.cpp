#include "gettext.h"
#include "debug.h"

char* Gettext::m_locale;

bool Gettext::m_changed;

void Gettext::init(const char* locale)
{
	m_locale = setlocale( LC_MESSAGES, locale );
	if (m_locale == 0)
	{
		m_locale = setlocale( LC_MESSAGES, "");
	}
		
	bindtextdomain ("sumwars_sumwars","./translation/");
	bind_textdomain_codeset ("sumwars_sumwars","UTF-8");
	
	bindtextdomain ("sumwars_events","./translation/");
	bind_textdomain_codeset ("sumwars_events","UTF-8");
	
	bindtextdomain ("sumwars_xml","./translation/");
	bind_textdomain_codeset ("sumwars_xml","UTF-8");
	
	textdomain ("sumwars_sumwars");
}

const char* Gettext::getLocale()
{
	return m_locale;
}

void Gettext::setLocale(const char* locale)
{
	if (locale != m_locale)
	{
		char* ret = setlocale( LC_MESSAGES, locale );
		if (ret !=0)
		{
			m_locale = ret;
			m_changed = true;
			DEBUGX("setting locale to %s",ret);
		}
		else
		{
			DEBUGX("setting locale %s not succesful",locale);
		}
	}
}

bool Gettext::getLocaleChanged()
{
	bool ret = m_changed;
	m_changed = false;
	return ret;
}


