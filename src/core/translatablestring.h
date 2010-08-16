#ifndef TRANSLATABLE_STRING
#define TRANSLATABLE_STRING

#include <string>
#include <vector>
#include "charconv.h"
#include "gettext.h"

/**
 * \brief Class for a string, that should be translated with gettext
 */
class TranslatableString
{
	public:
		/**
		 * \brief Default constructor
		 */
		TranslatableString();
		
		/**
		 * \brief Copy constructor
		 * \param other other TranslatableString
		 */
		TranslatableString(const TranslatableString& other)
		{
			*this = other;
		}
		
		/**
		 * \brief Construktor that creates the text from a string
		 * \param text The raw text (untranslated)
		 * \param domain gettext domain for translation
		 * Marked as explicit to avoid unexpected type construction
		 */
		explicit TranslatableString(std::string text, std::string domain = "")
		{
			m_text = text;
			m_domain = domain;
		}
		
		/**
		 * \brief Sets the Text of the string
		 * \param text The text to be translated
		 * \param substrings list of substrings, that should be inserted into the string after translation
		 * If the string contains some markers of the syntax $1, $2 and so one, these are replaced by the substrings. \n
		 * The $$ marker is replaced by a single dollar sign
		 */
		void setText(std::string text, std::vector<std::string> substrings);
		
		/**
		 * \brief Sets the text of the string
		 * \param text The text to be translated
		 */
		void operator=(std::string text);
		
		/**
		 * \brief Comparison operator
		 * \param other other translatable string
		 */
		bool operator<(const TranslatableString& other) const
		{
			return m_text < other.m_text;
		}
		
		
		/**
		 * \brief Returns the estimated length of the string
		 */
		int size();
		
		/**
		 * \brief comparison operator
		 * \param string normal string
		 * This only compares the text to the (untranslated) m_text!
		 */
		bool operator==(std::string text)
		{
			return (m_text == text);
		}
		
		/**
		 * \brief Sets the domain of the text
		 */
		void setTextDomain(std::string domain);
		
		/**
		 * \brief Returns the translated string
		 */
		const std::string& getTranslation();
		
		/**
		 * \brief Copy operator
		 * \param other other translable string
		 */
		void operator=(const TranslatableString& other);
		
		/**
		 * \brief Writes the object to the buffer
		 * \param cv Buffer
		 */
		void toString(CharConv* cv);
		
		
		/**
		 * \brief Retrieves the object content from the buffer
		 * \param cv buffer
		 */
		void fromString(CharConv* cv);
		
	private:
		/**
		 * \brief current Gettext domain, that is used for translation
		 */
		std::string m_domain;
		
		/**
		 * \brief the text to be translated
		 */
		std::string m_text;
		
		/**
		 * \brief List of substrings, that are inserted into the text after translation
		 */
		std::vector<std::string> m_substrings;
		
		/**
		 * \brief The translation of the text (internal buffer);
		 */
		std::string m_translation;
		
		/**
		 * \brief Is set to true, if the string is translated (the m_translation member is correct)
		 */
		bool m_is_translated;
		
		/**
		 * \brief The locale used for the translation
		 */
		std::string m_locale;
};

#endif