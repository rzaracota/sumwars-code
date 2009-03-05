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
		 * \fn void resetShift()
		 *  \brief Setzt Verschiebung auf 0
		 */
		void resetShift()
		{
			m_shift = Vector(0,0);
		}
		
	private:
	/**
	 * \var Ogre::Camera* m_minimap_camera
	 * \brief Kamera mit der die Minimap erzeugt wird
	 */
	Ogre::Camera* m_minimap_camera;
	
	/**
	*  \var Ogre::RenderTarget* m_minimap_rt
	* \brief Rendertarget for the minimap
	*/
	Ogre::RenderTarget* m_minimap_rt;
	
	/**
	 * \var Vector m_shift
	 * \brief Wert um den die Karte verschoben ist
	 */
	Vector m_shift;
	
	/**
	 * \var short m_region_id
	 * \brief ID der Region, die gerade angezeigt wird
	 */
	short m_region_id;
	
	
	
	
};

#endif

