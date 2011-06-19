#ifndef CONTENTEDITOR_TAB_H
#define CONTENTEDITOR_TAB_H

#include <OgreMeshManager.h>
#include "CEGUI/CEGUI.h"
#include "debug.h"
#include "mathhelper.h"

/**
 * \brief Base class for all content editor components.
 */
class ContentEditorTab
{
	public:
		
		/**
		 * \brief Initialises the panel
		 * \param parent parent CEGUI window
		 */
		virtual void init(CEGUI::Window* parent)
		{
			m_no_cegui_events = false;
		}
		
		/**
		* \brief Updates the content of the tab
		* Function is called once each tick.
		*/
		virtual void update()
		{
		};
		
	protected:
		/**
		 * \brief Sets the text of a CEGUI window
		 * \param windowname Name of the window
		 * \param text new text of the window
		 */
		void setWindowText(std::string windowname, std::string text);
		
		/**
		 * \brief returns the text of a CEGUI window
		 * \param windowname Name of the window
		 * \return text of the window
		 */
		std::string getWindowText(std::string windowname);
		
		/**
		 * \brief Sets the value of a CEGUI spinner
		 * \param windowname Name of the window
		 * \param value new value
		 */
		void setSpinnerValue(std::string windowname, double value);
		
		/**
		 * \brief Returns the value of a CEGUI spinner
		 * \param windowname Name of the window
		 * \return current spinner value
		 */
		double getSpinnerValue(std::string windowname, double value);
		
		/**
		 * \brief Sets the state of a CEGUI Checkbox
		 * \param windowname Name of the window
		 * \param state new state
		 */
		void setCheckboxSelected(std::string windowname, bool state);
		
		/**
		 * \brief Returns the state of a CEGUI Checkbox
		 * \param windowname Name of the window
		 * \return state
		 */
		bool getCheckboxSelected(std::string windowname);
		
		/**
		 * \brief returns the selected Item of a combobox
		 * \param windowname Name of the window
		 * \param def default string returned if no item is selected
		 * \return text of the selected item
		 */
		std::string getComboboxSelection(std::string windowname, std::string def="");
		
		/**
		 * \brief Sets the cursor in a CEGUI MultiLineEditbox at the specified position
		 * \param windowname Name of the window
		 * \param row cursor row 
		 * \param col cursor column
		 */
		void setMultiLineEditboxCursor(std::string windowname, int row, int col);
		
		void getNodeBounds(const Ogre::SceneNode* node, Ogre::Vector3& minimum, Ogre::Vector3& maximum, int level=1);
		
	protected:
		/**
		 * \brief while set to true, all CEGUI events will be ignored
		 * This allows setting some GUI elements in event handlers without getting infinite loops due.
		 */
		bool m_no_cegui_events;
		
};


#endif