#include "gettext.h"
#include "debug.h"


#ifdef WIN32
    #include <windows.h>
#endif

std::string Gettext::m_locale;

bool Gettext::m_changed;

void Gettext::init(const char* locale)
{

	bindtextdomain ("sumwars-sumwars","./translation/");
	bind_textdomain_codeset ("sumwars-sumwars","UTF-8");

	bindtextdomain ("sumwars-events","./translation/");
	bind_textdomain_codeset ("sumwars-events","UTF-8");

	bindtextdomain ("sumwars-xml","./translation/");
	bind_textdomain_codeset ("sumwars-xml","UTF-8");

	textdomain ("sumwars-sumwars");

	Gettext::setLocale(locale);
}

const char* Gettext::getLocale()
{
	return m_locale.c_str();
}

void Gettext::setLocale(const char* loc)
{
    std::string locale;
    if (loc != 0)
    {
         locale = loc;
    }
    else
    {
        locale = "";
    }

    DEBUG("set new language %s",locale.c_str());

	if (locale != m_locale)
	{
	    #ifdef WIN32
            if (locale != "")
            {
                std::string win_locale(locale, 0, 2);
                std::string env = "LANGUAGE=" + win_locale;

                _putenv(env.c_str());
                SetEnvironmentVariable("LANGUAGE", win_locale.c_str());

                char lang[50];
                GetEnvironmentVariable("LANGUAGE",lang,50);
                DEBUG("current language (GetEnvironmentVariable) %s",lang);
                DEBUG("current language (getenv) %s",getenv("LANGUAGE"));

               m_locale = locale;
               m_changed = true;
               return;
            }

        #endif
        std::string extensions[3] = {".utf-8",".UTF-8",""};

        // try all possible extensions
        char* ret;
        bool success = false;
        std::string locale_ext;
        for (int i=0; i<3; i++)
        {
            locale_ext = locale;
            locale_ext += extensions[i];
            ret = setlocale( LC_MESSAGES, locale_ext.c_str() );
            if (ret !=0)
            {
                m_locale = locale_ext;
                m_changed = true;
                success = true;
                DEBUGX("setting locale to %s",ret);
                break;
            }
        }


		if (!success)
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


