#ifndef QUESTINFO_H
#define QUESTINFO_H

#include "Ogre.h"

#include "window.h"
#include "listitem.h"


/**
 * \class QuestInfo
 * \brief Fenster Questinformationen
 */
class QuestInfo : public Window
{
	public:
		/**
	 * \fn QuestInfo (Document* doc)
	 * \brief Konstruktor
		 */
		QuestInfo (Document* doc);
	
		/**
		 * \fn virtual void update()
		 * \brief aktualisiert den Inhalt des Fensters
		 */
		virtual void update();
		
	private:
	/**
	 * \fn bool onQuestSelected(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Auswahl einer Klasse
	 */
	bool onQuestSelected(const CEGUI::EventArgs& evt);
	
	/**
	 * \fn bool onLookSelected(const CEGUI::EventArgs& evt)
	 * \brief Behandelt Auswahl eines anderen Filters
	 */
	bool onFilterSelected(const CEGUI::EventArgs& evt);
	
	/**
	 * \fn void updateDescription()
	 * \brief aktualisiert die Beschreibung
	 */
	void updateDescription();
	
	/**
	 * \fn std::string m_questname
	 * \brief Name des aktuell angezeigten Quests
	 */
	std::string m_questname;
	
	
};

#endif
