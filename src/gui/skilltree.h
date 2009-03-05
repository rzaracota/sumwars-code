#ifndef SKILLTREE_H
#define SKILLTREE_H

#include "window.h"

/**
 * \class SkillTree
 * \brief Fenster Faehigkeitenbaum
 */
class SkillTree : public Window
{
	public:
		/**
		 * \fn SkillTree (Document* doc)
		 * \brief Konstruktor
		 */
		SkillTree (Document* doc);
	
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

		
		/**
		 * \fn CEGUI::Window* getCEGUIWindow(int index)
		 * \brief Gibt eines der vier zugrunde liegenden CEGUI Windows aus
		 * \param index Nummer des Fensters
		 */
		CEGUI::Window* getCEGUIWindow(int index)
		{
			return m_windows[index];
		}
		
	private:
		/**
		 * \fn bool onAbilityHover(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Eintritt der Maus in Bereich ueber einer Faehigkeit
		 */
		bool onAbilityHover(const CEGUI::EventArgs& evt);
		
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
		 * \var Zeiger auf die 4 zugrunde liegenden Fenster
		 */
		CEGUI::Window* m_windows[4];
};

#endif
