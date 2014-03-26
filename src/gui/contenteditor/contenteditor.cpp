/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Utility for CEGUI cross-version compatibility
#include "ceguiutility.h"

#include "contenteditor.h"
#include "OgreRoot.h"
#include "debug.h"
#include "config.h"

#ifdef CEGUI_07
#include "CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h"
#else
#include "CEGUI/RendererModules/Ogre/Renderer.h"
#endif

#include <OgreHardwarePixelBuffer.h>
#include <OgreMeshManager.h>
#include <OISMouse.h>

#include "graphicmanager.h"
#include "worldobject.h"
#include "objectloader.h"
#include "world.h"

#include "renderinfoeditor.h"
#include "fixedobjecteditor.h"
#include "itemeditor.h"
#include "monstereditor.h"
#include "gameinfotab.h"

using namespace CEGUI;

template<> ContentEditor* Ogre::Singleton<ContentEditor>::SUMWARS_OGRE_SINGLETON = 0;

void ContentEditor::init(bool visible)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	//CEGUI::System* guiSystem = System::getSingletonPtr();
	CEGUI::WindowManager* winManager = WindowManager::getSingletonPtr();
  CEGUI::Window* gameScreen = CEGUIUtility::getWindow("GameScreen");
	m_lastVisibilitySwitch = 0;
	
	m_rootWindow = winManager->createWindow("TaharezLook/FrameWindow", "ContentEditor");
	m_rootWindow->setPosition(UVector2(UDim(0.025f, 0.0f), UDim(0.025f, 0.0f)));
  CEGUIUtility::setWidgetSizeRel(m_rootWindow, 0.9f, 0.85f);
	m_rootWindow->setText((CEGUI::utf8*)"Content Editor");
  CEGUIUtility::addChildWidget (gameScreen, m_rootWindow);
	
	CEGUI::Window* rootWindow;
	rootWindow = CEGUIUtility::loadLayoutFromFile ("contenteditor.layout");
	rootWindow->setPosition(UVector2(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f)));
  CEGUIUtility::setWidgetSizeRel(rootWindow, 1.0f, 1.0f);
  CEGUIUtility::addChildWidget (m_rootWindow, rootWindow);
	
	// initialisation of the selected tabs for more intuitive use
  CEGUI::TabControl* widget = static_cast<CEGUI::TabControl*>(CEGUIUtility::getWindow("ObjectInfoTabControl"));
  if (widget)
  {
    widget->setSelectedTab("RenderInfoTab");
  }

  widget = static_cast<CEGUI::TabControl*>(CEGUIUtility::getWindow("RenderInfoTabControl"));
  if (widget)
  {
    widget->setSelectedTab("RITab/BasicMesh");
  }

  widget = static_cast<CEGUI::TabControl*>(CEGUIUtility::getWindow("FixedObjectTabControl"));
  if (widget)
  {
    widget->setSelectedTab("FOTab/Properties");
  }

  widget = static_cast<CEGUI::TabControl*>(CEGUIUtility::getWindow("MonsterTabControl"));
  if (widget)
  {
    widget->setSelectedTab("MonsterTab/Properties");
  }

	// create SceneManager for renderering images for the content editor
	Ogre::SceneManager* editor_scene_mng = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC,"EditorSceneManager");
	editor_scene_mng->setAmbientLight(Ogre::ColourValue(1,1,1));
	
	// camera filming the content of the the editor scene
	Ogre::Camera* editor_camera = editor_scene_mng->createCamera("editor_camera");
	editor_camera->setNearClipDistance(0.1);
	editor_camera->setAspectRatio(1.0);
	Ogre::SceneNode *mainEditorNode = editor_scene_mng->getRootSceneNode()->createChildSceneNode("MainEditorCameraNode");
	Ogre::SceneNode *editorNode = mainEditorNode->createChildSceneNode("EditorCameraNode");
	Ogre::SceneNode *editorCamPitchNode = editorNode->createChildSceneNode("EditorCamerapitchNode");
	editorCamPitchNode->attachObject(editor_camera);
	mainEditorNode->setFixedYawAxis(true);
	editorNode->setFixedYawAxis(true);
	editorCamPitchNode->setFixedYawAxis(true);
	
	
    // texture that is created from the camera image
	Ogre::TexturePtr editor_texture = Ogre::TextureManager::getSingleton().createManual( "editor_tex",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
   512, 512, 0, Ogre::PF_R8G8B8A8, Ogre::TU_RENDERTARGET );
	
   // connection texture and camera via RenderTarget
	Ogre::RenderTarget* editor_rt = editor_texture->getBuffer()->getRenderTarget();
	editor_rt->setAutoUpdated(false);
	Ogre::Viewport *editor_view = editor_rt->addViewport(editor_camera );
	editor_view->setClearEveryFrame( true );
	editor_view->setOverlaysEnabled (false);
	editor_view->setBackgroundColour(Ogre::ColourValue(0,0,0,1.0) );
	editor_rt->update();

	// get the OgreRenderer from CEGUI and create a CEGUI texture from the Ogre texture
	CEGUI::OgreRenderer* renderer_ptr = static_cast<CEGUI::OgreRenderer*>(CEGUI::System::getSingleton().getRenderer());
  CEGUI::String imageName("editor_img");

#ifdef CEGUI_07
	// create a CEGUI Image from the Texture
  CEGUI::Texture& editor_ceguiTex = renderer_ptr->createTexture(editor_texture);
    
	CEGUI::Imageset& editor_textureImageSet = CEGUI::ImagesetManager::getSingleton().create("editor_imageset", editor_ceguiTex);

	editor_textureImageSet.defineImage(
      imageName,
			CEGUI::Point( 0.0f, 0.0f ),
			CEGUI::Size( editor_ceguiTex.getSize().d_width, editor_ceguiTex.getSize().d_height ),
			CEGUI::Point( 0.0f, 0.0f ) );
#else
  CEGUI::Texture &editor_cegui_texture = renderer_ptr->createTexture (editor_texture->getName (), editor_texture);
	{
		CEGUI::TextureTarget*   d_textureTarget;
		CEGUI::BasicImage*      d_textureTargetImage;
		d_textureTarget = renderer_ptr->createTextureTarget();
		d_textureTargetImage = static_cast<CEGUI::BasicImage*>(&CEGUI::ImageManager::getSingleton().create("BasicImage", imageName));
		d_textureTargetImage->setTexture(&editor_cegui_texture);
		d_textureTargetImage->setArea(
        CEGUI::Rectf(
            0, 
            0, 
            editor_cegui_texture.getSize ().d_width,
            editor_cegui_texture.getSize ().d_height));
	}
#endif

	// place the image in a the CEGUI label
	CEGUI::Window* label = CEGUIUtility::getWindow("RITab/BM/meshPreview");

  CEGUI::String content_editor_image_name(CEGUIUtility::getImageNameWithSkin("editor_tex", "editor_img"));
	label->setProperty("Image", content_editor_image_name); // prev: "set:editor_imageset image:editor_img"
	label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&ContentEditor::onPreviewWindowMouseDown, this));
  label->subscribeEvent(CEGUIUtility::EventMouseLeavesWindowArea(), CEGUI::Event::Subscriber(&ContentEditor::onPreviewWindowMouseUp, this));
	label->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&ContentEditor::onPreviewWindowMouseUp, this));
	label->subscribeEvent(CEGUI::Window::EventMouseWheel, CEGUI::Event::Subscriber(&ContentEditor::onPreviewWindowScrollWheel, this));
	
	CEGUI::PushButton* closebutton = static_cast<CEGUI::PushButton*>(CEGUIUtility::getWindow("CloseButton"));
	closebutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ContentEditor::onClose, this));
	
	closebutton = static_cast<CEGUI::FrameWindow*>(m_rootWindow)->getCloseButton();
	closebutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ContentEditor::onClose, this));
	
	m_components["FOEditor"] = new FixedObjectEditor();
	m_components["RIEditor"] = new RenderInfoEditor();
	m_components["ItemEditor"] = new ItemEditor();
	m_components["MonsterEditor"] = new MonsterEditor();
  m_components["GameInfoTab"] = new GameInfoTab();
	
	std::map<std::string, ContentEditorTab*>::iterator it;
	for (it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->init(m_rootWindow);
	}
	
	if(!visible)
  {
		m_rootWindow->setVisible(visible);
  }

	updateTranslation();
}

void ContentEditor::toggleVisibility()
{
	if((Ogre::Root::getSingleton().getTimer()->getMilliseconds() - m_lastVisibilitySwitch) > 500)
	{
		m_rootWindow->setVisible(!m_rootWindow->isVisible());
		m_lastVisibilitySwitch = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
	}
}


void ContentEditor::update(OIS::Keyboard *keyboard, OIS::Mouse *mouse)
{
	if(m_leftMouseDown)
	{
		Ogre::SceneManager *editor_scene_mng = Ogre::Root::getSingleton().getSceneManager("EditorSceneManager");
		Ogre::Radian rotX(mouse->getMouseState().X.rel * 0.05f);
		Ogre::Radian rotY(mouse->getMouseState().Y.rel * 0.05f);
		
		Ogre::SceneNode *editorNode = editor_scene_mng->getSceneNode("EditorCameraNode");
		Ogre::SceneNode *editorCamPitchNode = editor_scene_mng->getSceneNode("EditorCamerapitchNode");
		editorNode->yaw(-rotX);
		editorCamPitchNode->roll(rotY);
	}
	else if(m_rightMouseDown)
	{
		Ogre::SceneManager *editor_scene_mng = Ogre::Root::getSingleton().getSceneManager("EditorSceneManager");
		Ogre::Camera *cam = editor_scene_mng->getCamera("editor_camera");
		
		Ogre::Vector3 vec = Ogre::Vector3(0.0f, 0.0f, mouse->getMouseState().Y.rel * 0.1f);
		cam->moveRelative(vec);
	}
	
	if(keyboard->isKeyDown(OIS::KC_LCONTROL) && keyboard->isKeyDown(OIS::KC_K))
		toggleVisibility();
	
	std::map<std::string, ContentEditorTab*>::iterator it;
	for (it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->update();
	}
}

void ContentEditor::fullUpdateComponent(std::string name)
{
	if (m_components.count(name) == 0)
		return;
	
	m_components[name]->setUpdateBaseContent();
}

bool ContentEditor::handleCloseWindow(const CEGUI::EventArgs& e)
{
	m_rootWindow->hide();
	return true;
}

void ContentEditor::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;

	textdomain("tools");

	label = CEGUIUtility::getWindow("RenderInfoTab");
	label->setText((CEGUI::utf8*) gettext("Render Info"));
	
	label = CEGUIUtility::getWindow("RITab/BasicMesh");
	label->setText((CEGUI::utf8*) gettext("Mesh"));
	
	label = CEGUIUtility::getWindow("RITab/BM/MainMeshLabel");
	label->setText((CEGUI::utf8*) gettext("Select main mesh:"));
	
	label = CEGUIUtility::getWindow("RITab/SubMesh");
	label->setText((CEGUI::utf8*) gettext("Submeshes"));
	
	label = CEGUIUtility::getWindow("RITab/SubMesh/AddSubMeshButton"); 
	label->setText((CEGUI::utf8*) gettext("Add Submesh"));
	
	label = CEGUIUtility::getWindow("RITab/SM/NewSubmeshLabel"); 
	label->setText((CEGUI::utf8*) gettext("New submesh:"));
	
	label = CEGUIUtility::getWindow("RITab/SM/NewSMNameLabel"); 
	label->setText((CEGUI::utf8*) gettext("submesh name:"));
	
	label = CEGUIUtility::getWindow("RITab/SM/EditSMLabel"); 
	label->setText((CEGUI::utf8*) gettext("Edit submesh:"));
	
	label = CEGUIUtility::getWindow("RITab/SM/AttachMeshLabel");
	label->setText((CEGUI::utf8*) gettext("Attach to:"));
	
	label = CEGUIUtility::getWindow("RITab/SM/BoneLabel"); 
	label->setText((CEGUI::utf8*) gettext("at bone:"));
	
	label = CEGUIUtility::getWindow("RITab/SM/RotateLabel");
	label->setText((CEGUI::utf8*) gettext("Rotate(x,y,z):"));
	
	label = CEGUIUtility::getWindow("RITab/SM/ScaleLabel");
	label->setText((CEGUI::utf8*) gettext("Scale:"));
	
	label = CEGUIUtility::getWindow("RITab/SM/OffsetLabel");
	label->setText((CEGUI::utf8*) gettext("Offset (x,y,z):"));
	
	label = CEGUIUtility::getWindow("RITab/SubMesh/DelSubMeshButton"); 
	label->setText((CEGUI::utf8*) gettext("Delete Submesh"));
	
	label = CEGUIUtility::getWindow("RITab/Animations"); 
	label->setText((CEGUI::utf8*) gettext("Animations"));
	
	label = CEGUIUtility::getWindow("RITab/XML"); 
	label->setText((CEGUI::utf8*) gettext("XML"));
	
	label = CEGUIUtility::getWindow("RITab/XML/SubmitButton"); 
	label->setText((CEGUI::utf8*) gettext("Submit XML"));
	
	label = CEGUIUtility::getWindow("FixedObjectTab"); 
	label->setText((CEGUI::utf8*) gettext("FixedObject"));
	
	label = CEGUIUtility::getWindow("FOTab/Properties"); 
	label->setText((CEGUI::utf8*) gettext("Properties"));
	
	label = CEGUIUtility::getWindow("FOTab/Prop/ShapeLabel"); 
	label->setText((CEGUI::utf8*) gettext("Shape:"));
	
	label = CEGUIUtility::getWindow("FOTab/Prop/CircleLabel"); 
	label->setText((CEGUI::utf8*) gettext("Circle"));
	
	label = CEGUIUtility::getWindow("FOTab/Prop/RadiusLabel"); 
	label->setText((CEGUI::utf8*) gettext("Radius:"));
	
	label = CEGUIUtility::getWindow("FOTab/Prop/DetectCircleButton"); 
	label->setText((CEGUI::utf8*) gettext("Autodetect"));
	
	label = CEGUIUtility::getWindow("FOTab/Prop/RectangleLabel");
	label->setText((CEGUI::utf8*) gettext("Rectangle"));
	
	label = CEGUIUtility::getWindow("FOTab/Prop/WidthLabel");
	label->setText((CEGUI::utf8*) gettext("Width:"));
	
	label = CEGUIUtility::getWindow("FOTab/Prop/DepthLabel");
	label->setText((CEGUI::utf8*) gettext("Depth:"));
	
	label = CEGUIUtility::getWindow("FOTab/Prop/DetectRectButton");
	label->setText((CEGUI::utf8*) gettext("Autodetect"));
	
	label = CEGUIUtility::getWindow("FOTab/Prop/CollisionLabel");
	label->setText((CEGUI::utf8*) gettext("Collision type"));
	
	label = CEGUIUtility::getWindow("FOTab/Properties/CopyDataLabel");
	label->setText((CEGUI::utf8*) gettext("Copy data from:"));
	
	label = CEGUIUtility::getWindow("FOTab/Properties/CopyDataButton");
	label->setText((CEGUI::utf8*) gettext("Copy"));
	
	label = CEGUIUtility::getWindow("FOTab/Create");
	label->setText((CEGUI::utf8*) gettext("Create Object"));
	
	label = CEGUIUtility::getWindow("FOTab/XML/CreateButton");
	label->setText((CEGUI::utf8*) gettext("Create Object"));
	
	label = CEGUIUtility::getWindow("FOTab/Create/PosLabel");
	label->setText((CEGUI::utf8*) gettext("Position:"));
	
	label = CEGUIUtility::getWindow("FOTab/Create/GetPlPosButton");
	label->setText((CEGUI::utf8*) gettext("Get Player Position"));
	
	label = CEGUIUtility::getWindow("FOTab/Create/AngleLabel");
	label->setText((CEGUI::utf8*) gettext("Angle:"));
	
	label = CEGUIUtility::getWindow("FOTab/Create/DelAllButton");
	label->setText((CEGUI::utf8*) gettext("Delete all objects"));
	
	label = CEGUIUtility::getWindow("FOTab/XML");
	label->setText((CEGUI::utf8*) gettext("XML"));
	
	label = CEGUIUtility::getWindow("FOTab/XML/SubmitButton");
	label->setText((CEGUI::utf8*) gettext("Submit XML"));
	
	label = CEGUIUtility::getWindow("ItemTab");
	label->setText((CEGUI::utf8*) gettext("Item"));
	
	label = CEGUIUtility::getWindow("ItemTab/General");
	label->setText((CEGUI::utf8*) gettext("General"));
	
	label = CEGUIUtility::getWindow("ItemTab/Properties/TypeLabel");
	label->setText((CEGUI::utf8*) gettext("Type:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Properties/SizeLabel");
	label->setText((CEGUI::utf8*) gettext("Size:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Properties/CopyDataLabel");
	label->setText((CEGUI::utf8*) gettext("Copy data from:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Properties/CopyDataButton");
	label->setText((CEGUI::utf8*) gettext("Copy"));
	
	label = CEGUIUtility::getWindow("ItemTab/General/NameLabel");
	label->setText((CEGUI::utf8*) gettext("Name:"));
	
	label = CEGUIUtility::getWindow("ItemTab/General/PriceLabel");
	label->setText((CEGUI::utf8*) gettext("Value:"));
	
	label = CEGUIUtility::getWindow("ItemTab/General/EnchantRangeLabel");
	label->setText((CEGUI::utf8*) gettext("Enchant min:"));
	
	label = CEGUIUtility::getWindow("ItemTab/General/EnchantMaxLabel");
	label->setText((CEGUI::utf8*) gettext("max:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Equip");
	label->setText((CEGUI::utf8*) gettext("Equip"));
	
	label = CEGUIUtility::getWindow("ItemTab/Equip/HealthLabel");
	label->setText((CEGUI::utf8*) gettext("Health:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Equip/AttrLabel");
	label->setText((CEGUI::utf8*) gettext("Attribute:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Equip/StrengthLabel");
	label->setText((CEGUI::utf8*) gettext("Strength:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Equip/DexterityLabel");
	label->setText((CEGUI::utf8*) gettext("Dexterity:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Equip/MagicPowerLabel");
	label->setText((CEGUI::utf8*) gettext("Magic Power:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Equip/WillpowerLabel");
	label->setText((CEGUI::utf8*) gettext("Willpower:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Equip/ResistancesLabel");
	label->setText((CEGUI::utf8*) gettext("Resistances:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Equip/PhysResLabel");
	label->setText((CEGUI::utf8*) gettext("Physical:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Equip/FireResLabel");
	label->setText((CEGUI::utf8*) gettext("Fire:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Equip/IceResLabel");
	label->setText((CEGUI::utf8*) gettext("Ice:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Equip/AirResLabel");
	label->setText((CEGUI::utf8*) gettext("Air:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Equip/OtherLabel");
	label->setText((CEGUI::utf8*) gettext("Other:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Equip/ArmorLabel");
	label->setText((CEGUI::utf8*) gettext("Armor:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Equip/BlockLabel");
	label->setText((CEGUI::utf8*) gettext("Block:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Equip/AttackLabel");
	label->setText((CEGUI::utf8*) gettext("Precision:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Equip/PowerLabel");
	label->setText((CEGUI::utf8*) gettext("Power:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon");
	label->setText((CEGUI::utf8*) gettext("Weapon"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/TypeLabel");
	label->setText((CEGUI::utf8*) gettext("Type:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/DamageLabel");
	label->setText((CEGUI::utf8*) gettext("Damage:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/PhysLabel");
	label->setText((CEGUI::utf8*) gettext("Physical:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/PhysToLabel");
	label->setText((CEGUI::utf8*) gettext("-"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/PhysMultLabel");
	label->setText((CEGUI::utf8*) gettext("x"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/FireLabel");
	label->setText((CEGUI::utf8*) gettext("Fire:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/FireToLabel");
	label->setText((CEGUI::utf8*) gettext("-"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/FireMultLabel");
	label->setText((CEGUI::utf8*) gettext("x"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/IceLabel");
	label->setText((CEGUI::utf8*) gettext("Ice:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/IceToLabel");
	label->setText((CEGUI::utf8*) gettext("-"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/IceMultLabel");
	label->setText((CEGUI::utf8*) gettext("x"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/AirLabel");
	label->setText((CEGUI::utf8*) gettext("Air:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/AirToLabel");
	label->setText((CEGUI::utf8*) gettext("-"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/AirMultLabel");
	label->setText((CEGUI::utf8*) gettext("x"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/PrecisionLabel");
	label->setText((CEGUI::utf8*) gettext("Precision:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/PowerLabel");
	label->setText((CEGUI::utf8*) gettext("Power:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/RangeLabel");
	label->setText((CEGUI::utf8*) gettext("Range:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/SpeedLabel");
	label->setText((CEGUI::utf8*) gettext("Speed:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/TwohandedLabel");
	label->setText((CEGUI::utf8*) gettext("Twohanded weapon"));
	
	label = CEGUIUtility::getWindow("ItemTab/Weapon/CritPercentLabel");
	label->setText((CEGUI::utf8*) gettext("Crit. Hits:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Consume");
	label->setText((CEGUI::utf8*) gettext("Consume"));
	
	label = CEGUIUtility::getWindow("ItemTab/Consume/TypeLabel");
	label->setText((CEGUI::utf8*) gettext("Health:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Consume/CureLabel");
	label->setText((CEGUI::utf8*) gettext("Status heal and immunity time:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Consume/BlindLabel");
	label->setText((CEGUI::utf8*) gettext("Blind:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Consume/PoisonedLabel");
	label->setText((CEGUI::utf8*) gettext("Poisoned:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Consume/BerserkLabel");
	label->setText((CEGUI::utf8*) gettext("Berserk:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Consume/ConfusedLabel");
	label->setText((CEGUI::utf8*) gettext("Confused:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Consume/MuteLabel");
	label->setText((CEGUI::utf8*) gettext("Mute:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Consume/ParalyzedLabel");
	label->setText((CEGUI::utf8*) gettext("Paralyzed:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Consume/FrozenLabel");
	label->setText((CEGUI::utf8*) gettext("Frozen:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Consume/BurningLabel");
	label->setText((CEGUI::utf8*) gettext("Burning:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Create");
	label->setText((CEGUI::utf8*) gettext("Create Item"));
	
	label = CEGUIUtility::getWindow("ItemTab/Create/EnchantLabel");
	label->setText((CEGUI::utf8*) gettext("Enchant:"));
	
	label = CEGUIUtility::getWindow("ItemTab/Create/CreateInventoryButton");
	label->setText((CEGUI::utf8*) gettext("Create in inventory"));
	
	label = CEGUIUtility::getWindow("ItemTab/Create/CreateDropButton");
	label->setText((CEGUI::utf8*) gettext("Create an drop"));
	
	label = CEGUIUtility::getWindow("ItemTab/XML");
	label->setText((CEGUI::utf8*) gettext("XML"));
	
	label = CEGUIUtility::getWindow("ItemTab/XML/SubmitButton");
	label->setText((CEGUI::utf8*) gettext("Submit XML"));
	
	label = CEGUIUtility::getWindow("CloseButton");
	label->setText((CEGUI::utf8*) gettext("Close"));
	
	textdomain("menu");
}

bool ContentEditor::onPreviewWindowMouseDown(const CEGUI::EventArgs& evt)
{
#if (OGRE_VERSION < ((1 << 16) | (9 << 8) | 0))
    Ogre::TexturePtr tex = Ogre::TextureManager::getSingletonPtr()->getByName("editor_tex");
#else
    Ogre::TexturePtr tex = Ogre::TextureManager::getSingletonPtr()->getByName("editor_tex").staticCast<Ogre::Texture>();
#endif
    tex.getPointer()->getBuffer()->getRenderTarget()->setAutoUpdated(true);
	
	const CEGUI::MouseEventArgs* args = static_cast<const CEGUI::MouseEventArgs*>(&evt);
	if(args->button == CEGUI::LeftButton)
		m_leftMouseDown = true;
	if(args->button == CEGUI::RightButton)
		m_rightMouseDown = true;

	return true;
}

bool ContentEditor::onPreviewWindowMouseUp(const CEGUI::EventArgs& evt)
{
#if (OGRE_VERSION < ((1 << 16) | (9 << 8) | 0))
    Ogre::TexturePtr tex = Ogre::TextureManager::getSingletonPtr()->getByName("editor_tex", "General");
#else
    Ogre::TexturePtr tex = Ogre::TextureManager::getSingletonPtr()->getByName("editor_tex", "General").staticCast<Ogre::Texture>();
#endif
	tex.getPointer()->getBuffer()->getRenderTarget()->setAutoUpdated(false);
	
	const CEGUI::MouseEventArgs* args = static_cast<const CEGUI::MouseEventArgs*>(&evt);
	if(args->button == CEGUI::LeftButton)
		m_leftMouseDown = false;
	if(args->button == CEGUI::RightButton)
		m_rightMouseDown = false;

	return true;
}

bool ContentEditor::onPreviewWindowScrollWheel(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs* args = static_cast<const CEGUI::MouseEventArgs*>(&evt);
	
	Ogre::SceneManager *editor_scene_mng = Ogre::Root::getSingleton().getSceneManager("EditorSceneManager");
	Ogre::Camera *cam = editor_scene_mng->getCamera("editor_camera");

	Ogre::Vector3 vec = Ogre::Vector3(0.0f, 0.0f, args->wheelChange * 0.02f);
	cam->moveRelative(vec);


#if (OGRE_VERSION < ((1 << 16) | (9 << 8) | 0))
    Ogre::TexturePtr tex = Ogre::TextureManager::getSingletonPtr()->getByName("editor_tex", "General");
#else
    Ogre::TexturePtr tex = Ogre::TextureManager::getSingletonPtr()->getByName("editor_tex", "General").staticCast<Ogre::Texture>();
#endif
	tex.getPointer()->getBuffer()->getRenderTarget()->update();

	return true;
}


ContentEditor* ContentEditor::getSingletonPtr(void)
{
	return SUMWARS_OGRE_SINGLETON;
}

ContentEditor& ContentEditor::getSingleton(void)
{
	assert( SUMWARS_OGRE_SINGLETON );
	return ( *SUMWARS_OGRE_SINGLETON );
}

ContentEditorTab* ContentEditor::getComponent(std::string name)
{
	if (m_components.count(name) == 0)
		return 0;
	
	return m_components[name];
}

bool ContentEditor::onClose(const CEGUI::EventArgs& evt)
{
	m_rootWindow->setVisible(!m_rootWindow->isVisible());
	m_lastVisibilitySwitch = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
	return true;
}

