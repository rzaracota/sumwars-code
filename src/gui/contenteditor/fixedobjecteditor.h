#ifndef FIXEDOBJECT_EDITOR_TAB_H
#define FIXEDOBJECT_EDITOR_TAB_H

#include "contenteditortab.h"
#include "worldobject.h"
#include "objectloader.h"

/**
 * \brief Editor class for FixedObject XML
 */
class FixedObjectEditor : public ContentEditorTab
{
	public:
		/**
		 * \brief Initialises the panel
		 * \param parent parent CEGUI window
		 */
		virtual void init(CEGUI::Window* parent);
		
		/**
		 * \brief Updates the content of the tab
		 * Function is called once each tick.
		 */
		virtual void update();
		
	private:
		
		/**
		 * \param evt CEGUI event arguments
		 * \brief Called upon changing any element FixedObject tab
		 */
		bool onFixedObjectModified(const CEGUI::EventArgs& evt);
		
		/**
		 * \param evt CEGUI event arguments
		 * \brief Called upon submitting XML for FixedObject
		 */
		bool onFixedObjectXMLModified(const CEGUI::EventArgs& evt);
		
		/**
		 * \param evt CEGUI event arguments
		 * \brief Called upon clicking the autodetect size button
		 */
		bool onFixedObjectAutodetectSize(const CEGUI::EventArgs& evt);
		
		/**
		 * \param evt CEGUI event arguments
		 * \brief Called upon clicking create Fixed Object Button
		 */
		bool onFixedObjectCreate(const CEGUI::EventArgs& evt);
		
		/**
		 * \brief updates the content of the FixedObject XML editor
		 */
		void updateFixedObjectXML();
		
		/**
		 * \brief updates the content of the FixedObject XML editor
		 */
		void updateFixedObjectEditor();
		
		
		
		/**
		 * \brief marks if the fixed object data has been modified
		 */
		bool m_modified_fixed_object;
		
		/**
		 * \brief marks that the XML of the fixed object data has been modified
		 */
		bool m_modified_fixed_object_xml;
		
		/**
		 * \brief FixedObject currently edited
		 */
		FixedObjectData m_edited_fixed_object;
		
		/**
		 * \brief XML representation of the  FixedObject edited with this window
		 */
		TiXmlDocument m_fixed_object_xml; 
};


#endif