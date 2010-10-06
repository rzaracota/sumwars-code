#include "gettext.h"
#include "debug.h"


#ifdef WIN32
    #include <windows.h>
#endif

std::string Gettext::m_locale;

bool Gettext::m_changed;

void Gettext::init(const char* locale)
{
	bindtextdomain ("defend_dwarfenwall","./translation/");
	bind_textdomain_codeset ("defend_dwarfenwall","UTF-8");

	bindtextdomain ("fortify_dwarfenwall","./translation/");
	bind_textdomain_codeset ("fortify_dwarfenwall","UTF-8");

	bindtextdomain ("herb_collector","./translation/");
	bind_textdomain_codeset ("herb_collector","UTF-8");

	bindtextdomain ("joringsbridge","./translation/");
	bind_textdomain_codeset ("joringsbridge","UTF-8");

	bindtextdomain ("maylons_grave","./translation/");
	bind_textdomain_codeset ("maylons_grave","UTF-8");

	bindtextdomain ("menu","./translation/");
	bind_textdomain_codeset ("menu","UTF-8");

	bindtextdomain ("rescue_willard","./translation/");
	bind_textdomain_codeset ("rescue_willard","UTF-8");

	bindtextdomain ("rob_the_robber","./translation/");
	bind_textdomain_codeset ("rob_the_robber","UTF-8");

	bindtextdomain ("sumwars","./translation/");
	bind_textdomain_codeset ("sumwars","UTF-8");

	bindtextdomain ("testquest","./translation/");
	bind_textdomain_codeset ("testquest","UTF-8");

	bindtextdomain ("the_ambush_of_the_undead","./translation/");
	bind_textdomain_codeset ("the_ambush_of_the_undead","UTF-8");

	bindtextdomain ("the_ambush","./translation/");
	bind_textdomain_codeset ("the_ambush","UTF-8");

	bindtextdomain ("the_besieged_farm","./translation/");
	bind_textdomain_codeset ("the_besieged_farm","UTF-8");

	bindtextdomain ("the_goblinslayer","./translation/");
	bind_textdomain_codeset ("the_goblinslayer","UTF-8");

	bindtextdomain ("the_poolmonster","./translation/");
	bind_textdomain_codeset ("the_poolmonster","UTF-8");

	bindtextdomain ("the_trader","./translation/");
	bind_textdomain_codeset ("the_trader","UTF-8");

	bindtextdomain ("tutorial","./translation/");
	bind_textdomain_codeset ("tutorial","UTF-8");

	textdomain ("menu");

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

    DEBUGX("set new language %s",locale.c_str());

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
                DEBUGX("current language (GetEnvironmentVariable) %s",lang);
                DEBUGX("current language (getenv) %s",getenv("LANGUAGE"));

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


