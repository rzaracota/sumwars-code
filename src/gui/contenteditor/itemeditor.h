#ifndef ITEM_EDITOR_TAB_H
#define ITEM_EDITOR_TAB_H

#include "contenteditortab.h"
#include "worldobject.h"
#include "item.h"
#include "itemloader.h"

/**
 * \brief Editor class for Item XML
 */
class ItemEditor : public ContentEditorTab
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
		 * \brief is called if the list of all Items has been modified
		 */
		void updateAllItemList();
		
		/**
		 * \param evt CEGUI event arguments
		 * \brief Called upon changing any element in the Item tab
		 */
		bool onItemModified(const CEGUI::EventArgs& evt);
		
		/**
		 * \param evt CEGUI event arguments
		 * \brief Called upon changing any element in the Weapon tab
		 */
		bool onWeaponModified(const CEGUI::EventArgs& evt);
		
		/**
		 * \param evt CEGUI event arguments
		 * \brief Called upon changing any element in the Consume tab
		 */
		bool onConsumeEffectsModified(const CEGUI::EventArgs& evt);
		
		/**
		 * \param evt CEGUI event arguments
		 * \brief Called upon changing any element in the Equip tab
		 */
		bool onEquipEffectsModified(const CEGUI::EventArgs& evt);
		
		/**
		 * \param evt CEGUI event arguments
		 * \brief Called upon submitting XML for Item
		 */
		bool onItemXMLModified(const CEGUI::EventArgs& evt);
		
		/**
		 * \param evt CEGUI event arguments
		 * \brief Called upon clicking create Item and drop Button
		 */
		bool onItemCreateDrop(const CEGUI::EventArgs& evt);
		
		/**
		 * \param evt CEGUI event arguments
		 * \brief Called upon clicking create Item in Inventory Button
		 */
		bool onItemCreateInInventory(const CEGUI::EventArgs& evt);
		
		/**
		 * \param evt CEGUI event arguments
		 * \brief Called upon clicking the Copy Data from existing item Button
		 */
		bool onCopyData(const CEGUI::EventArgs& evt);
		
		
		/**
		 * \brief updates the content of the Item XML editor
		 */
		void updateItemXML();
		
		/**
		 * \brief updates the content of the Item XML editor
		 */
		void updateItemEditor();
		
		/**
		 * \brief creates an item based on the XML data
		 * \return created Item
		 */
		Item* createItem();
		
		/**
		 * \brief marks if the item data has been modified
		 */
		bool m_modified_item;
		
		/**
		 * \brief marks that the XML of the item data has been modified
		 */
		bool m_modified_item_xml;
		
		/**
		 * \brief marks whether the list of all items is dirty
		 */
		bool m_modified_item_list;
		
		/**
		 * \brief Item currently edited
		 */
		ItemBasicData m_edited_item;
		
		/**
		 * \brief XML representation of the  Item edited with this window
		 */
		TiXmlDocument m_item_xml; 
		
		/**
		 * \brief unique ID for items
		 */
		int m_unique_id;
};


#endif