#ifndef SKILLTREE_H
#define SKILLTREE_H

#include "window.h"
#include "OISKeyboard.h"
/**
 * \class SkillTree
 * \brief Fenster Faehigkeitenbaum
 */
class SkillTree : public Window
{
	public:
		/**
		 * \fn SkillTree (Document* doc, OIS::Keyboard *keyboard)
		 * \param keyboard OIS keyboard
		 * \brief Konstruktor
		 */
		SkillTree (Document* doc, OIS::Keyboard *keyboard);
	
		/**
		 * \fn virtual void update()
		 * \brief aktualisiert den Inhalt des Fensters
		 */
		virtual void update();
		
		/**
		 * \fn virtual void updateTranslation
		 * \brief aktualisiert die Uebersetzungen
		 */
		virtual void updateTranslation();

		
	private:
		/**
		 * \fn bool onAbilityHover(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Eintritt der Maus in Bereich ueber einer Faehigkeit
		 */
		bool onAbilityHover(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onAbilityHoverLeave(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Verlassen des Bereichs ueber einer Faehigkeit
		 */
		bool onAbilityHoverLeave(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn void updateAbilityTooltip(unsigned int pos)
		 * \brief Aktualisiert den Tooltip fuer die Faehigkeit ueber der die Maus ist
		 */
		void updateAbilityTooltip(unsigned int pos);
		
		/**
		 * \fn bool onSkillMouseClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Mausklick auf eine Faehigkeit im Skilltree
		 */
		bool onSkillMouseClicked(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onSkillLearnMouseClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Mausklick auf den Lernen Button zu einer Faehigkeit im Skilltree
		 */
		bool onSkillLearnMouseClicked(const CEGUI::EventArgs& evt);
		
		
		/**
		 * \var OIS::Keyboard *m_keyboard
		 * \brief Repraesentation der Tastatur
		 */
		OIS::Keyboard *m_keyboard;
		
		/**
		 * \var int m_player_id
		 * \brief ID des Spielers
		 */
		int m_player_id;
		
		/**
		 * \brief number of skilltree tabs
		 */
		int m_nr_tabs;
		
		/**
		 * \brief number of skills
		 */
		int m_nr_skills;
		
		/**
		 * \brief number of dependency connections
		 */
		int m_nr_dependencies;
		
		/**
		 * \brief number of shortkey labels
		 */
		int m_shortkey_labels;
};

#endif
