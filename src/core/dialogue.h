#ifndef DIALOGUE_H
#define DIALOGUE_H

#include "debug.h"
#include "event.h"
#include "creaturestruct.h"

class Region;


/**
 * \class TopicList
 * \brief Klasse fuer die Liste der Themen, ueber die ein NPC reden kann
 */
class TopicList
{
	public:
		
		/**
		 * \fn ~TopicList()
		 * \brief Destruktor
		 */
		~TopicList();
		
		/**
		 * \fn void addTopic(std::string topic, event* speech)
		 * \brief Fuegt der Liste ein Thema hinzu
		 * \param topic das Thema
		 * \param speech das Gespraech zu dem Thema
		 */
		void addTopic(std::string topic, Event* speech);
		
		/**
		 * \fn void addStartTopic(std::string text, std::string topic)
		 * \brief Fuegt ein Thema als Startthema hinzu
		 * \param topic Thema
		 * \param text Text mit dem das Thema angesprochen wird
		 */
		void addStartTopic(std::string text, std::string topic);
		
		/**
		 * \fn Event* getSpeakTopic(std::string topic)
		 * \brief Gibt Gespraech zu einem Thema aus#
		 * \param topic Thema
		 */
		Event* getSpeakTopic(std::string topic);
		
		/**
		 * \fn std::list< std::pair<std::string, std::string> >& getStartTopics()
		 * \brief Gibt die Liste der Startthemen aus
		 */
		std::list< std::pair<std::string, std::string> >& getStartTopics()
		{
			return m_start_topics;
		}
				
	private:
		/**
	 	* \var std::map < std::string, Event*> m_topics
		* \brief Liste der Themen
		 */
		std::map < std::string, Event*> m_topics;
		
		/**
		 * \var std::list< std::pair<std::string, std::string> > m_start_topics
		 * \brief Liste der Themen, mit denen ein Gespraech beginnen kann (Text,Thema)
		 */
		std::list< std::pair<std::string, std::string> > m_start_topics;
};

/**
 * \class Dialogue
 * \brief Klasse fuer einen Gespraech zwischen mehreren Spielern / NPC's
 */
class Dialogue
{
	public:
		/**
		 * \fn Dialogue(Region* region, std::string topic_base="global")
		 * \brief Konstruktor
		 * \param region Region, in der das Gespraech stattfindet
		 * \param id ID des Dialogs
		 * \param topic_base Name des NPC, aus dessen Repertoire die Topics genommen werden
		 */
		Dialogue(Region* region, std::string topic_base="global");
		
		/**
		 * \fn ~Dialogue()
		 * \brief Destruktor
		 */
		~Dialogue();
		
		/**
		 * \fn void init()
		 * \brief Initialisierung
		 */
		static void init();
		
		/**
		 * \fn void addSpeaker(int id, std::string refname)
		 * \brief Fuegt der Gespraech einen Sprecher hinzu
		 * \param id ID des Sprechers
		 * \param refname Name unter dem der Sprecher angesprochen wird
		 */
		void addSpeaker(int id, std::string refname="");
		
		/**
		 * \fn void speak(std::string refname, std::string text, float time = 500)
		 * \brief Fuegt einen neuen gesprochenen Text ein
		 * \param refname Name unter dem der Sprecher referenziert wird
		 * \param text gesprochener Text
		 * \param time Zeit, die der Text angezeigt wird
		 */
		void speak(std::string refname, std::string text, float time = 500);
		
		/**
		 * \fn void addQuestion(std::string text)
		 * \brief Fuegt eine Frage ein. Die Frage wird bei dem Spieler mit der ID  m_main_player_id angezeigt
		 * \param text Text der Frage
		 */
		void addQuestion(std::string text);
		
		
		/**
		 * \fn void setTopicBase(std::string topic_base)
		 * \brief Setzt Repertoire der Topics
		 * \param topic_base global, wenn kein NPC dabei ist, sonst Name des NPC
		 */
		void setTopicBase(std::string topic_base);
		
		/**
		 * \fn void addAnswer(std::string text, std::string topic)
		 * \brief Fuegt fuer die Frage eine Antwort ein
		 * \param text Text der Antwort
		 * \param topic Thema zu dem gesprungen wird, wenn der Spieler diese Antwort waehlt
		 */
		void addAnswer(std::string text, std::string topic);
		
		/**
		 * \fn bool checkTopic(std::string topic)
		 * \brief prueft, ob das angegebene Thema angesprochen werden kann
		 */
		bool checkTopic(std::string topic);
		
		/**
		 * \fn void changeTopic(std::string topic)
		 * \brief Setzt das Thema auf das angegebene Thema
		 * Themen mit besonderen Bedeutungen sind 'start' und 'end'
		 */
		void changeTopic(std::string topic);
		
		/**
		 * \fn void update(float time)
		 * \brief Aktualisiert den Dialog, nachdem eine gewisse Zeitspanne vergangen ist
		 * \param time Zeit in ms
		 */
		void update(float time);
		
		/**
		 * \fn int getSpeaker(std::string refname)
		 * \brief Gibt die ID des Sprechers aus, der ueber den angegebenen Name angesprochen wird
		 * \param refname Referenzname
		 */
		int getSpeaker(std::string refname);
		
		/**
		 * \fn void cleanup()
		 *  \brief Gibt allokierten Speicher frei
		 */
		void cleanup();
		
		/**
		 * \fn bool isFinished()
		 * \brief Gibt aus, ob der Dialog beendet wurde
		 */
		bool isFinished()
		{
			return m_finished;
		}
		
		/**
		 * \fn int getId()
		 * \brief Gibt die ID aus
		 */
		int getId()
		{
			return m_id;
		}
		
		/**
		 * \fn static TopicList& getTopicList(std::string topic_base)
		 * \brief Gibt die Themenliste zu einem NPC aus
		 * \param topic_base Name des NPC
		 */
		static TopicList& getTopicList(std::string topic_base)
		{
			return m_topics[topic_base];
		}
	
	private:
		
		/**
		 * \var int m_main_player_id
		 * \brief ID des Spielers, dem die Fragen gestellt werden
		 */
		int m_main_player_id;
		
		/**
		 * \var std::string m_topic_base
		 * \brief Name des NPC, aus dessen Repertoire die Topics genommen werden
		 */
		std::string m_topic_base;
		
		/**
		 * \var std::map<std::string, int> m_speaker
		 * \brief Liste aller Sprecher
		 */
		std::map<std::string, int> m_speaker;
		
		/**
		 * \var  Region* m_region
		 * \brief Region, in der das Gespraech stattfindet
		 */
		Region* m_region;
		
		/**
		 * \var std::list<std::pair< std::string, CreatureSpeakText > > m_speech
		 * \brief Liste von gesprochenen Aussagen
		 */
		std::list<std::pair< std::string, CreatureSpeakText > > m_speech;
		
		/**
		 * \var bool m_finished
		 * \brief auf true gesetzt, wenn das Gespraech beendet ist
		 */
		bool m_finished;
		
		/**
		 * \var bool m_started
		 * \brief true, bevor der erste Text angezeigt wird
		 */
		bool m_started;
		
		/**
		 * \var int m_id
		 * \brief ID des Dialogs
		 */
		int m_id;
		
		/**
		 * \var static std::map<std::string , TopicList > m_topics
		 * \brief Liste der Themen fuer alle NPC's
		 */
		static std::map<std::string , TopicList > m_topics;
		
};

#endif

