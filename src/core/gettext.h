/*
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

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
		 * \brief initializes Gettext
		 * \param locale Language
		 * \param pathToTranslationFiles Path to the translation files
		 */
		static void init(const char* locale, std::string pathToTranslationFiles);

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

