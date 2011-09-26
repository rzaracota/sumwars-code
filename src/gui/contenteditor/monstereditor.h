#ifndef MONSTER_EDITOR_TAB_H
#define MONSTER_EDITOR_TAB_H

#include "contenteditortab.h"
#include "worldobject.h"
#include "monster.h"
#include "objectloader.h"

/**
 * \brief Editor class for Monster XML
 */
class MonsterEditor : public ContentEditorTab
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
		 * \brief is called if the list of all Monsters has been modified
		 */
		void updateAllMonsterList();
		
		/**
		 * \param evt CEGUI event arguments
		 * \brief Called upon changing any element in the Monster tab
		 */
		bool onMonsterModified(const CEGUI::EventArgs& evt);
		
		
		/**
		 * \param evt CEGUI event arguments
		 * \brief Called upon submitting XML for Monster
		 */
		bool onMonsterXMLModified(const CEGUI::EventArgs& evt);
		
		/**
		 * \param evt CEGUI event arguments
		 * \brief Called upon clicking create Monster
		 */
		bool onMonsterCreate(const CEGUI::EventArgs& evt);
		
		
		/**
		 * \param evt CEGUI event arguments
		 * \brief Called upon clicking the Copy Data from existing monster Button
		 */
		bool onCopyData(const CEGUI::EventArgs& evt);
		
				
		/**
		 * \param evt CEGUI event arguments
		 * \brief Called upon clicking the delete all objects Button
		 */
		bool onDelAllObjects(const CEGUI::EventArgs& evt);
		
		/**
		 * \param evt CEGUI event arguments
		 * \brief Called upon clicking the get player position Button
		 */
		bool onGetPlayerPosition(const CEGUI::EventArgs& evt);
		
		
		/**
		 * \brief updates the content of the Monster XML editor
		 */
		void updateMonsterXML();
		
		/**
		 * \brief updates the content of the Monster XML editor
		 */
		void updateMonsterEditor();
		
		/**
		 * \brief marks if the monster data has been modified
		 */
		bool m_modified_monster;
		
		/**
		 * \brief marks that the XML of the monsters data has been modified
		 */
		bool m_modified_monster_xml;
		
		/**
		 * \brief marks whether the list of all monsters is dirty
		 */
		bool m_modified_monster_list;
		
		/**
		 * \brief Monster currently edited
		 */
		MonsterBasicData m_edited_monster;
		
		/**
		 * \brief XML representation of the  Monster edited with this window
		 */
		TiXmlDocument m_monster_xml; 
		
		/**
		 * \brief unique ID for monsters
		 */
		int m_unique_id;
		
		/**
		 * \brief List of all created objects
		 * The first integer is the region ID, the second is the object ID
		 */
		std::list< std::pair<int,int> > m_created_objects;
};


#endif