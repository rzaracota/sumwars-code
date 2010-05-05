#ifndef GETTEXT_H
#define GETTEXT_H


#ifdef gettext
    #undef gettext
#endif
#include <libintl.h>


#include <locale.h>
#include <string>

/**
 * \class Gettext
 * \brief Hilfsklasse fuer die Internationalisierung
 */
class Gettext
{
	public:
		/**
		 * \fn static void init(const char* locale)
		 * \brief initialisiert Gettext
		 * \param locale Sprache
		 */
		static void init(const char* locale);

		/**
		 * \fn static const char* getLocale()
		 * \brief Gibt die aktuelle Sprache aus
		 */
		static const char* getLocale();

		/**
		 * \fn static void setLocale(const char* locale)
		 * \brief Setzt die aktuelle Sprache
		 * \param locale Sprache
		 */
		static void setLocale(const char* locale);

		/**
		 * \fn static bool getLocaleChanged()
		 * \brief Gibt aus, ob die Sprache geaendert wurde. Das Bit wird dabei automatisch auf false zurueckgesetzt
		 */
		static bool getLocaleChanged();

	private:
		/**
		 * \var std::string m_locale
		 * \brief aktuelle Sprache
		 */
		static std::string m_locale;

		/**
		 * \var static bool m_changed
		 * \brief Auf true gesetzt, wenn die Sprache gerade geaendert wurde (bis zum ersten Aufruf von getLocalChanged)
		 */
		static bool m_changed;
};
#endif

