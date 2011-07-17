#ifndef DEBUGCAMERA_H
#define DEBUGCAMERA_H

#include "debugtab.h"
#include "CEGUI/CEGUIWindow.h"

namespace Ogre
{
	class SceneNode;
}

class DebugCameraTab : public CEGUI::Window, public DebugTab
{

public:
    DebugCameraTab(const CEGUI::String& type, const CEGUI::String& name);
	virtual void update(OIS::Keyboard *keyboard, OIS::Mouse *mouse);
	
	virtual void initialiseComponents(void );

	/**
	 * \param evt CEGUI event arguments
	 * \brief Called upon clicking a mouse button down to the preview window
	 */
	bool onDebugWindowMouseDown(const CEGUI::EventArgs& evt);
	
	/**
	 * \param evt CEGUI event arguments
	 * \brief Called upon releasing a mouse button to the preview window
	 */
	bool onDebugWindowMouseUp(const CEGUI::EventArgs& evt);
	
protected:
	bool onPlayerCameraModeChanged(const CEGUI::EventArgs& evt);
	
private:
	CEGUI::Window *m_tabLayout;
	
	/**
	 * \brief Sets the debug camera and the Rendertarget up
	 */
	void setupCamera();
	
	/**
	 * \brief While true, the user rotates the camera in the preview window
	 */
	bool m_leftMouseDown;
	
	/**
	 * \brief While true, the user zooms the camera in the preview window
	 */
	bool m_rightMouseDown;
	
	/**
	 * \brief Main camera node
	 */
	Ogre::SceneNode *cameraNode;
	
	/**
	 * \brief Camera Yaw node
	 */
	Ogre::SceneNode *cameraYawNode;
	
	/**
	 * \brief Camera Pitch node
	 */
	Ogre::SceneNode *cameraPitchNode;
		
	/**
	 * \brief Camera Roll node
	 */
	Ogre::SceneNode *cameraRollNode;
	
public:
	static CEGUI::String WidgetTypeName;
};

#endif // GUIDEBUGTAB_H
