#include "gettext.h"
#include "debug.h"

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

std::string Gettext::m_locale;

bool Gettext::m_changed;

void Gettext::init(const char* locale, std::string pathToTranslationFiles)
{
	bindtextdomain ("defend_dwarfenwall",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("defend_dwarfenwall","UTF-8");

	bindtextdomain ("fortify_dwarfenwall",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("fortify_dwarfenwall","UTF-8");

	bindtextdomain ("herb_collector",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("herb_collector","UTF-8");

	bindtextdomain ("joringsbridge",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("joringsbridge","UTF-8");

	bindtextdomain ("maylons_grave",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("maylons_grave","UTF-8");

	bindtextdomain ("menu",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("menu","UTF-8");

	bindtextdomain ("rescue_willard",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("rescue_willard","UTF-8");

	bindtextdomain ("rob_the_robber",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("rob_the_robber","UTF-8");

	bindtextdomain ("sumwars",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("sumwars","UTF-8");

	bindtextdomain ("testquest",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("testquest","UTF-8");

	bindtextdomain ("the_ambush_of_the_undead",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("the_ambush_of_the_undead","UTF-8");

	bindtextdomain ("the_ambush",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("the_ambush","UTF-8");

	bindtextdomain ("the_besieged_farm",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("the_besieged_farm","UTF-8");

	bindtextdomain ("the_goblinslayer",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("the_goblinslayer","UTF-8");

	bindtextdomain ("the_poolmonster",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("the_poolmonster","UTF-8");

	bindtextdomain ("the_trader",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("the_trader","UTF-8");

	bindtextdomain ("tutorial",pathToTranslationFiles.c_str());
	bind_textdomain_codeset ("tutorial","UTF-8");

	textdomain ("menu");

#ifdef WIN32
	char loc[100];
	GetLocaleInfo(LOCALE_USER_DEFAULT,
						  LOCALE_SISO639LANGNAME,
						  loc, sizeof(loc));
	Gettext::setLocale(loc);
#else
	Gettext::setLocale(locale);
#endif


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
			setenv("LANG", locale_ext.c_str(), 1);     // fixes gettext problems with Ubuntu
			setenv("LANGUAGE", locale_ext.c_str(), 1); // fixes gettext problems with Ubuntu
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


