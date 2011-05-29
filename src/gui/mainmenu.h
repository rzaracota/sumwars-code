#ifndef MAINMENU_H
#define MAINMENU_H

#include "OgreRoot.h"
#include "window.h"

class SavegameList;

/**
 * \class MainMenu
 * \brief Fenster Charakter Info
 */
class MainMenu : public Window , public Ogre::FrameListener
{
public:
    /**
    * \fn MainMenu (Document* doc)
    * \brief Constructor
     */
    MainMenu ( Document* doc );

    /**
     * \fn virtual void update()
     * \brief Updates the window contents
     */
    virtual void update();

    /**
     * \fn virtual void updateTranslation
     * \brief updates the translations
     */
    virtual void updateTranslation();


    /**
     * \fn virtual bool frameStarted (const FrameEvent &evt)
     * \brief Called when a frame is about to begin rendering.
     */
    virtual bool frameStarted ( const Ogre::FrameEvent &evt );

    /**
     * \fn virtual bool frameStarted (const FrameEvent &evt)
     * \brief Called just after a frame has been rendered.
     */
    virtual bool frameEnded ( const Ogre::FrameEvent &evt );


private:

    /**
     * \fn bool createSavegameList()
     * \brief Creates the savegame list in main menu
     */
    void createSavegameList();

    /**
     * \fn bool onStartSinglePlayer(const CEGUI::EventArgs& evt)
     * \brief Handles start of the SinglePlayer mode
     */
    bool onStartSinglePlayer ( const CEGUI::EventArgs& evt );

    /**
     * \fn bool onStartMultiPlayer(const CEGUI::EventArgs& evt)
     * \brief Handles start of the MultiPlayer mode when joining a game
     */
    bool onStartMultiPlayer ( const CEGUI::EventArgs& evt );

    /**
     * \fn bool onStartMultiPlayerHost(const CEGUI::EventArgs& evt)
     * \brief Handles start of the MultiPlayer mode when hosting a game
     */
    bool onStartMultiPlayerHost ( const CEGUI::EventArgs& evt );

    /**
     * \fn bool onShowCredits(const CEGUI::EventArgs& evt)
     * \brief Handles clicks on the credits button
     */
    bool onShowCredits ( const CEGUI::EventArgs& evt );

    /**
     * \fn bool onShown(const CEGUI::EventArgs& evt)
     * \brief Handles events when window is shown
     */
    bool onShown ( const CEGUI::EventArgs& evt );

    /**
     * \fn bool onHidden(const CEGUI::EventArgs& evt)
     * \brief Handles events when window is hidden
     */
    bool onHidden ( const CEGUI::EventArgs& evt );


    /**
     * \fn bool onShowOptions(const CEGUI::EventArgs& evt)
     * \brief Handles clicks on the Options button
     */
    bool onShowOptions ( const CEGUI::EventArgs& evt );

    /**
     * \fn bool onQuitGameHost(const CEGUI::EventArgs& evt)
     * \brief Handles clicks on quit hosting a multiplayer game
     */
    bool onQuitGameHost ( const CEGUI::EventArgs& evt );

    /**
     * \fn void createScene()
     * \brief Creates the background scene
     */
    void createScene();

    /**
     * \fn void createCharacterMenu()
     * \brief Creates the GUI for character selection
     */

    void createCharacterMenu();

    /**
     * \var Ogre::SceneNode *m_mainNode;
     * \brief A link to the MainMenu Ogre::SceneNode
     */
    Ogre::SceneNode *m_mainNode;

    /**
     * \var Ogre::SceneManager *m_sceneMgr;
     * \brief A pointer to the MainMenu Ogre::SceneManager
     */
    Ogre::SceneManager *m_sceneMgr;

    /**
     *\var bool m_sceneCreated;
     *\brief Sets to true if the scene was successfully created
     */
    bool m_sceneCreated;

    /**
     * \var SaveGameList m_saveGameList;
     * \brief A pointer to the savegame list window
     */
    SavegameList *m_saveGameList;
};

#endif
