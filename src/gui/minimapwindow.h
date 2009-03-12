#ifndef MINIMAPWINDOW_H
#define MINIMAPWINDOW_H

#include "Ogre.h"

#include "window.h"


/**
 * \class MinimapWindow
 * \brief Fenster Questinformationen
 */
class MinimapWindow : public Window
{
	public:
	/**
	 * \fn MinimapWindow (Document* doc)
	 * \brief Konstruktor
	 */
		MinimapWindow (Document* doc);
	
		/**
		 * \fn virtual void update()
		 * \brief aktualisiert den Inhalt des Fensters
		 */
		virtual void update();
		
		/**
		 * \fn virtual void updateTranslation
		 * \brief aktualisiert die Uebersetzungen
		 */
		virtual void updateTranslation()
		{
			
		}

		
	private:
	
	
	/**
	 * \var short m_region_id
	 * \brief ID der Region, die gerade angezeigt wird
	 */
	short m_region_id;
	
	
	
	
};

#endif

