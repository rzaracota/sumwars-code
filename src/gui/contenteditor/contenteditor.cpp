#include "contenteditor.h"
#include "guiTabs.h"
#include "OgreRoot.h"
#include "debug.h"

#include "CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h"
#include <OgreHardwarePixelBuffer.h>
#include <OgreMeshManager.h>

#include "graphicmanager.h"

using namespace CEGUI;

template<> ContentEditor* Ogre::Singleton<ContentEditor>::ms_Singleton = 0;

void ContentEditor::init(bool visible)
{
	m_no_cegui_events = false;
	m_edited_graphicobject=0;
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	m_guiSystem = System::getSingletonPtr();
	m_winManager = WindowManager::getSingletonPtr();
	m_gameScreen = m_winManager->getWindow("GameScreen");
	m_lastVisibilitySwitch = 0;
	
	m_rootWindow = m_winManager->createWindow("TaharezLook/FrameWindow", "ContentEditor");
	m_rootWindow->setPosition(UVector2(UDim(0.025f, 0.0f), UDim(0.025f, 0.0f)));
	m_rootWindow->setSize(UVector2(UDim(0.9f, 0.0f), UDim(0.85f, 0.0f)));
	m_rootWindow->setText((CEGUI::utf8*)"Content Editor");
	m_gameScreen->addChildWindow(m_rootWindow);
	
	CEGUI::Window* rootWindow;
	rootWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout("ContentEditor.layout");
	rootWindow->setPosition(UVector2(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f)));
	rootWindow->setSize(UVector2(UDim(1.0f, 0.0f), UDim(1.0f, 0.0f)));
	m_rootWindow->addChildWindow(rootWindow);
	
	// create SceneManager for renderering images for the content editor
	Ogre::SceneManager* editor_scene_mng = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC,"EditorSceneManager");
	editor_scene_mng->setAmbientLight(Ogre::ColourValue(1,1,1));
	
	// camera filming the content of the the editor scene
	Ogre::Camera* editor_camera = editor_scene_mng->createCamera("editor_camera");
	editor_camera->setPosition(Ogre::Vector3(3, 1.2,0));
	editor_camera->lookAt(Ogre::Vector3(0,1.2,0));
	editor_camera->setNearClipDistance(0.1);
	editor_camera->setAspectRatio(1.0);
	Ogre::SceneNode *editorNode = editor_scene_mng->getRootSceneNode()->createChildSceneNode("EditorCameraNode");
	editorNode->attachObject(editor_camera);
	
	
	// texture that is creates from the camera image
	Ogre::TexturePtr editor_texture = Ogre::TextureManager::getSingleton().createManual( "editor_tex",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
   512, 512, 0, Ogre::PF_R8G8B8A8, Ogre::TU_RENDERTARGET );

   // connection texture and camera via RenderTarget
	Ogre::RenderTarget* editor_rt = editor_texture->getBuffer()->getRenderTarget();
	editor_rt ->setAutoUpdated(false);
	Ogre::Viewport *editor_view = editor_rt->addViewport(editor_camera );
	editor_view->setClearEveryFrame( true );
	editor_view->setOverlaysEnabled (false);
	editor_view->setBackgroundColour(Ogre::ColourValue(0,0,0,1.0) );
	editor_rt->update();

	// create a CEGUI Image from the Texture
    CEGUI::Texture& editor_ceguiTex = static_cast<CEGUI::OgreRenderer*>(CEGUI::System::getSingleton().getRenderer())->createTexture(editor_texture);
    
	CEGUI::Imageset& editor_textureImageSet = CEGUI::ImagesetManager::getSingleton().create("editor_imageset", editor_ceguiTex);

	editor_textureImageSet.defineImage( "editor_img",
			CEGUI::Point( 0.0f, 0.0f ),
			CEGUI::Size( editor_ceguiTex.getSize().d_width, editor_ceguiTex.getSize().d_height ),
			CEGUI::Point( 0.0f, 0.0f ) );
	
	// place the image in a the CEGUI label
	CEGUI::Window* label = win_mgr.getWindow("RITab/BM/meshPreview");
	label->setProperty("Image", "set:editor_imageset image:editor_img"); 
	
	// add parts to the mesh selectors
	Combobox* selector = static_cast<Combobox*>(WindowManager::getSingleton().getWindow("RITab/BM/MeshSelector"));
	Combobox* subSelector = static_cast<Combobox*>(WindowManager::getSingleton().getWindow("RITab/SubMesh/Selector"));
	
	Ogre::FileInfoListPtr files;
	Ogre::FileInfoList::iterator it;
	std::string file;
	
	files = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("General","*.mesh");
	
	std::list<std::string> filenames;
	for (it = files->begin(); it != files->end(); ++it)
	{
		filenames.push_back(it->filename);
	}
	filenames.sort();
	
	CEGUI::ListboxTextItem* listitem;
	for (std::list<std::string>::iterator it = filenames.begin(); it != filenames.end(); ++it)
	{
		try
		{
			file = *it;
			listitem = new ListboxTextItem(file);
			selector->addItem(listitem);
			listitem = new ListboxTextItem(file);
			subSelector->addItem(listitem);
		}
		catch (Ogre::Exception& e)
		{
			DEBUG("failed with exception %s",e.what());
		}
	}
	
	// wire the GUI
	selector->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&ContentEditor::onMeshSelected, this));
	subSelector->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&ContentEditor::onSubMeshSelected, this));
	
	CEGUI::PushButton* addSubmeshbutton = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("RITab/SubMesh/AddSubMeshButton"));
	addSubmeshbutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ContentEditor::onSubMeshAdded, this));
	
	CEGUI::PushButton* submitRIXMLbutton = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("RITab/XML/SubmitButton"));
	submitRIXMLbutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ContentEditor::onRenderinfoXMLModified, this));
	
	CEGUI::PushButton* delSubmeshbutton = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("RITab/SubMesh/DelSubMeshButton"));
	delSubmeshbutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ContentEditor::onSubMeshDeleted, this));
	
	
	CEGUI::Spinner* rotXspinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMRotateX"));
	CEGUI::Spinner* rotYspinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMRotateY"));
	CEGUI::Spinner* rotZspinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMRotateZ"));
	
	CEGUI::Spinner* posXspinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMOffsetX"));
	CEGUI::Spinner* posYspinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMOffsetY"));
	CEGUI::Spinner* posZspinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMOffsetZ"));
	
	CEGUI::Spinner* scalespinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMScale"));
	
	CEGUI::Combobox* objSelector = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("RITab/SubMesh/EditSMSelector"));
	CEGUI::Combobox* boneobjSelector = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("RITab/SubMesh/AttachMeshSelector"));
	CEGUI::Combobox* boneSelector = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("RITab/SubMesh/BoneSelector"));
	CEGUI::Checkbox* attachCheckbox = static_cast<CEGUI::Checkbox*>(win_mgr.getWindow("RITab/SM/AttachSMCheckbox"));
	
	
	objSelector->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&ContentEditor::onSubObjectSelected, this));
	
	rotXspinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&ContentEditor::onSubMeshModified, this));
	rotYspinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&ContentEditor::onSubMeshModified, this));
	rotZspinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&ContentEditor::onSubMeshModified, this));
	
	posXspinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&ContentEditor::onSubMeshModified, this));
	posYspinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&ContentEditor::onSubMeshModified, this));
	posZspinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&ContentEditor::onSubMeshModified, this));
	
	scalespinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&ContentEditor::onSubMeshModified, this));
	
	boneobjSelector->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&ContentEditor::onSubMeshModified, this));
	boneSelector->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&ContentEditor::onSubMeshModified, this));
	attachCheckbox->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&ContentEditor::onSubMeshModified, this));
	
	
	
	if(!visible)
		m_rootWindow->setVisible(visible);
	
	// init the internal data
	TiXmlElement * renderinfo_root = new TiXmlElement("RenderInfo");  
	m_renderinfo_xml.LinkEndChild( renderinfo_root );  
	renderinfo_root->SetAttribute("name","editor_RI");
}

void ContentEditor::toggleVisibility()
{
	if((Ogre::Root::getSingleton().getTimer()->getMilliseconds() - m_lastVisibilitySwitch) > 500)
	{
		m_rootWindow->setVisible(!m_rootWindow->isVisible());
		m_lastVisibilitySwitch = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
	}
}


void ContentEditor::update(OIS::Keyboard *keyboard)
{
	
	if(keyboard->isKeyDown(OIS::KC_LCONTROL) && keyboard->isKeyDown(OIS::KC_K))
		toggleVisibility();
	
	if (m_modified_renderinfo)
	{
		updateRenderInfoXML();
		updatePreviewImage();
	}
	
	if (m_modified_renderinfo_xml)
	{
		updateSubmeshEditor();
		updatePreviewImage();
	}
	
	m_modified_renderinfo_xml = false;
	m_modified_renderinfo = false;
}

void ContentEditor::updatePreviewImage()
{
	Ogre::SceneManager* old_scene_mng = GraphicManager::getSceneManager();
	Ogre::SceneManager* editor_scene_mng = Ogre::Root::getSingleton().getSceneManager("EditorSceneManager");
	GraphicManager::setSceneManager(editor_scene_mng);
	
	// reparse and update the renderinfo
	GraphicRenderInfo* edited_renderinfo = new GraphicRenderInfo;
	GraphicManager::loadRenderInfo(m_renderinfo_xml.FirstChildElement(), edited_renderinfo);
	GraphicManager::registerRenderInfo("EditorRenderInfo", edited_renderinfo, true);
	
	// if not present: create the graphicObject
	if (m_edited_graphicobject == 0)
	{
		// the ID is just arbitrary...
		m_edited_graphicobject = GraphicManager::createGraphicObject("EditorRenderInfo", "EditedGraphicObject", 123456789);
	}
	m_edited_graphicobject->getTopNode()->setPosition(0,0,0);
	m_edited_graphicobject->update(0);
	
	// update the camera to show the full object
	// first, the subtree needs to be updated
	Ogre::SceneNode* topnode = m_edited_graphicobject->getTopNode();
	topnode->_update(true,false);
	const Ogre::AxisAlignedBox& boundingbox = topnode->_getWorldAABB();
	
	// camera is placed looking along negative X axis, with Y and Z offset
	Ogre::Vector3 bbox_min = boundingbox.getMinimum();
	Ogre::Vector3 bbox_max = boundingbox.getMaximum();
	double center_y = 0.5*(bbox_max[1] + bbox_min[1]);
	double center_z = 0.5*(bbox_max[2] + bbox_min[2]);
	double size_y = bbox_max[1] - bbox_min[1];
	double size_z = bbox_max[2] - bbox_min[2];
	
	double viewsize = MathHelper::Max(size_y, size_z);
	
	Ogre::Camera* editor_camera = editor_scene_mng->getCamera("editor_camera");
	//editor_camera->setPosition(Ogre::Vector3(bbox_max[0] + viewsize/sqrt(2) , center_y,center_z));
	//editor_camera->lookAt(Ogre::Vector3(bbox_max[0], center_y,center_z));
	
	// update the texture
	Ogre::Resource* res= Ogre::TextureManager::getSingleton().createOrRetrieve ("editor_tex",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME).first.getPointer();
	Ogre::Texture* texture = dynamic_cast<Ogre::Texture*>(res);
	Ogre::RenderTarget* target = texture->getBuffer()->getRenderTarget();
	target->update();
	

	
	GraphicManager::setSceneManager(old_scene_mng);
}

void ContentEditor::updateSubmeshEditor(std::string objectname, bool updateList)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	CEGUI::Spinner* rotXspinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMRotateX"));
	CEGUI::Spinner* rotYspinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMRotateY"));
	CEGUI::Spinner* rotZspinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMRotateZ"));
	
	CEGUI::Spinner* posXspinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMOffsetX"));
	CEGUI::Spinner* posYspinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMOffsetY"));
	CEGUI::Spinner* posZspinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMOffsetZ"));

	CEGUI::Spinner* scalespinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMScale"));

	CEGUI::Combobox* objSelector = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("RITab/SubMesh/EditSMSelector"));
	CEGUI::Combobox* boneobjSelector = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("RITab/SubMesh/AttachMeshSelector"));
	CEGUI::Combobox* boneSelector = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("RITab/SubMesh/BoneSelector"));
	CEGUI::Checkbox* attachCheckbox = static_cast<CEGUI::Checkbox*>(win_mgr.getWindow("RITab/SM/AttachSMCheckbox"));
	
	if (objectname == "")
		objectname = objSelector->getText().c_str();
	
	// update the content of the objectSelector
	if (updateList)
	{
		bool objectfound = false;
		bool boneobjfound = false;
		
		// preserve selected boneobj if possible (store the name)
		CEGUI::ListboxItem* boneobjitem = boneobjSelector->getSelectedItem();
		std::string boneobj = "mainmesh";
		if (boneobjitem != 0)
			boneobj = boneobjitem->getText().c_str();
		
		objSelector->resetList();
		boneobjSelector->resetList();
		objSelector->setText("");
		boneobjSelector->setText("");
		
		
		std::list<MovableObjectInfo>& objects = m_edited_renderinfo.getObjects();
		CEGUI::ListboxTextItem* listitem;
		CEGUI::ListboxTextItem* selecteditem;
		for (std::list<MovableObjectInfo>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			if (it->m_type != MovableObjectInfo::ENTITY)
				continue;
			
			listitem = new ListboxTextItem(it->m_objectname);
			objSelector->addItem(listitem);
			
			// automatically select the right item
			if (it->m_objectname == objectname)
			{
				objSelector->setItemSelectState(listitem,true);
				objSelector->setText(objectname);
				objectfound = true;
			}
			
			// also update the list of objects to attach to
			// do not list the object itself
			if (objectname != it->m_objectname)
			{
				listitem = new ListboxTextItem(it->m_objectname);
				boneobjSelector->addItem(listitem);
				
				// automatically select the right object
				if (boneobj == it->m_objectname)
				{
					boneobjSelector->setItemSelectState(listitem,true);
					objSelector->setText(boneobj);
					boneobjfound = true;
				}
			}
		}
		
		// if the whole object is missing, clear all attach selections
		if (!objectfound)
		{
			boneobjSelector->clearAllSelections();
			boneobjSelector->setText("");
			boneSelector->clearAllSelections();
			boneSelector->setText("");
		}
		
		// if just the object to attach to is missing, clear the bone selection
		if (!boneobjfound)
		{
			boneSelector->clearAllSelections();
			boneSelector->setText("");
		}
	}

	m_no_cegui_events = true;

	// get the selected item and the underlying RenderInfo data structure
	CEGUI::ListboxItem* item = objSelector->getSelectedItem();
	MovableObjectInfo* minfo = 0;
	if (item != 0)
		minfo = m_edited_renderinfo.getObject(objectname);
	
	if (minfo == 0)
	{
		objSelector->clearAllSelections();
		objSelector->setText("");
		boneobjSelector->clearAllSelections();
		boneobjSelector->setText("");
		boneSelector->clearAllSelections();
		boneSelector->setText("");
		return;
	}
	
	// update the list of bones if required
	updateBoneList();
	
	// now update all the GUI elements
	// bone and *attached to*
	attachCheckbox->setSelected(minfo->m_bone != "");
	std::string bone;
	std::string boneobj ="";
	int pos = minfo->m_bone.find(':');
	if (pos == -1)
	{
		bone = minfo->m_bone;
		if (bone != "")
		{
			boneobj = "mainmesh";
		}
	}
	else
	{
		boneobj = minfo->m_bone.substr(0,pos);
		bone = minfo->m_bone.substr(pos+1);
	}
	
	// set the selectors of the mesh attached to and the bone to the right state
	if (boneobj != "" && bone != "")
	{
		boneobjSelector->clearAllSelections();
		boneSelector->clearAllSelections();
		boneobjSelector->setText("");
		boneSelector->setText("");
		
		for (int i=0; i<boneobjSelector->getItemCount(); i++)
		{
			if (boneobjSelector->getListboxItemFromIndex(i)->getText() == boneobj)
			{
				boneobjSelector->setSelection(i,i);
				boneobjSelector->setText(boneobj);
			}
		}
		for (int i=0; i<boneSelector->getItemCount(); i++)
		{
			if (boneSelector->getListboxItemFromIndex(i)->getText() == bone)
			{
				boneSelector->setSelection(i,i);
				boneSelector->setText(bone);
			}
		}
	}
	else
	{
		boneobjSelector->clearAllSelections();
		boneSelector->clearAllSelections();
	}
	
	// set the rotation spinners
	rotXspinner->setCurrentValue(minfo->m_rotation[0]);
	rotYspinner->setCurrentValue(minfo->m_rotation[1]);
	rotZspinner->setCurrentValue(minfo->m_rotation[2]);

	// set the position spinners
	posXspinner->setCurrentValue(minfo->m_position[0]);
	posYspinner->setCurrentValue(minfo->m_position[1]);
	posZspinner->setCurrentValue(minfo->m_position[2]);
	
	// set scale spinner
	scalespinner->setCurrentValue(minfo->m_scale);
	
	m_no_cegui_events = false;
}

void ContentEditor::updateBoneList()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Combobox* boneSelector = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("RITab/SubMesh/BoneSelector"));
	CEGUI::Combobox* boneobjSelector = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("RITab/SubMesh/AttachMeshSelector"));
	
	// get the selected object and the underlying RenderInfo data structure
	std::string boneobj = boneobjSelector->getText().c_str();
	
	MovableObjectInfo* minfo = m_edited_renderinfo.getObject(boneobj);

	if (minfo == 0)
	{
		boneSelector->resetList();
		return;
	}
	

	std::string bonesel = boneSelector->getText().c_str();
	
	boneSelector->resetList();
	boneSelector->setText("");
	
	if (minfo->m_type == MovableObjectInfo::ENTITY)
	{
		//get the skeleton of the selected mesh and fill the bone selector combo box
		Ogre::Mesh* mesh = dynamic_cast<Ogre::Mesh*>(Ogre::MeshManager::getSingleton().createOrRetrieve(minfo->m_source,"General").first.getPointer());
		
		Ogre::Skeleton *skel = 0;
		
		if (mesh != 0)
		{
			skel = mesh->getSkeleton().getPointer();
		}
		
		if(skel!=0)
		{
			// iterate the skeleton to get the bones
			Ogre::Skeleton::BoneIterator bit = skel->getBoneIterator();
			std::set<std::string> boneNames;
			
			while (bit.hasMoreElements())
			{
				Ogre::Bone *bone = bit.getNext();
				boneNames.insert(bone->getName());
			}
			
			// add the bones to the combo box
			std::string bone;
			ListboxTextItem* item;
			for (std::set<std::string>::iterator it = boneNames.begin(); it != boneNames.end(); ++it)
			{
				
				bone = *it;
				item = new ListboxTextItem(bone);
				boneSelector->addItem(item);
				// automatically select the right object
				if (bonesel == bone)
				{
					boneSelector->setItemSelectState(item,true);
					boneSelector->setText(bone);
				}
			}
		}
	}
}

void ContentEditor::updateRenderInfoXML()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	// update the XML representation
	m_edited_renderinfo.writeToXML(m_renderinfo_xml.FirstChildElement());
	
	// write to the editor
	TiXmlPrinter printer;
	m_renderinfo_xml.Accept(&printer);
	
	CEGUI::MultiLineEditbox* editor = static_cast<CEGUI::MultiLineEditbox*>(win_mgr.getWindow("RITab/XML/RIXMLEditbox"));
	editor->setText(printer.CStr());
}

bool ContentEditor::handleCloseWindow(const CEGUI::EventArgs& e)
{
	m_rootWindow->hide();
	return true;
}

void ContentEditor::updateTranslation()
{
	
}

bool ContentEditor::onMeshSelected(const CEGUI::EventArgs& evt)
{
	
	
	const CEGUI::MouseEventArgs& we =
	static_cast<const CEGUI::MouseEventArgs&>(evt);
	
	CEGUI::Combobox* cbo = static_cast<CEGUI::Combobox*>(we.window);
	
	CEGUI::ListboxItem* item = cbo->getSelectedItem();
	
	if (item != 0)
	{
		// place the required mesh in the editor scene
		std::string meshname = item->getText().c_str();
		DEBUGX("selected mesh %s",meshname.c_str());
		
		// if the Renderinfo already has a main mesh, edit it
		// otherwise, create it
		MovableObjectInfo* minfo = m_edited_renderinfo.getObject("mainmesh");
		MovableObjectInfo newminfo;
		bool addnew = false;
		
		if (minfo == 0)
		{
			// no mainmesh yet
			minfo = &newminfo;
			addnew = true;
		}
		
		// edit the Object
		minfo->m_type = MovableObjectInfo::ENTITY;
		minfo->m_objectname = "mainmesh";
		minfo->m_source = meshname;
		
		if (addnew)
		{
			m_edited_renderinfo.addObject(*minfo);
		}
		
		m_modified_renderinfo = true;
	}
	
	return true;
}


bool ContentEditor::onSubObjectSelected(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
	static_cast<const CEGUI::MouseEventArgs&>(evt);
	
	CEGUI::Combobox* cbo = static_cast<CEGUI::Combobox*>(we.window);
	
	CEGUI::ListboxItem* item = cbo->getSelectedItem();
	
	if (item != 0)
	{
		// place the required mesh in the editor scene
		std::string meshname = item->getText().c_str();
		DEBUG("selected mesh %s",meshname.c_str());
		
		updateSubmeshEditor(meshname, false);
	}
	
	return true;
}

bool ContentEditor::onSubMeshSelected(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Combobox* subSelector = static_cast<CEGUI::Combobox*>(WindowManager::getSingleton().getWindow("RITab/SubMesh/Selector"));
	CEGUI::Editbox* subMeshNameBox = static_cast<CEGUI::Editbox*>(WindowManager::getSingleton().getWindow("RITab/SM/SMNameEditbox"));
	
	CEGUI::ListboxItem* item = subSelector->getSelectedItem();
	if (item != 0)
	{
		std::string meshname = item->getText().c_str();
		
		// crop the .mesh...
		size_t pos = meshname.find_first_of('.');
		if (pos != std::string::npos)
		{
			meshname.erase(pos);
		}
		subMeshNameBox->setText(meshname);
	}
	return true;
}

bool ContentEditor::onSubMeshAdded(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Combobox* subSelector = static_cast<CEGUI::Combobox*>(WindowManager::getSingleton().getWindow("RITab/SubMesh/Selector"));
	CEGUI::Editbox* subMeshNameBox = static_cast<CEGUI::Editbox*>(WindowManager::getSingleton().getWindow("RITab/SM/SMNameEditbox"));
	
	CEGUI::ListboxItem* item = subSelector->getSelectedItem();
	if (item != 0)
	{
		std::string meshname = item->getText().c_str();
		std::string objectname = subMeshNameBox->getText().c_str();
		
		if (objectname == "")
		{
			// crop the .mesh...
			objectname = meshname;
			size_t pos = objectname.find_first_of('.');
			if (pos != std::string::npos)
			{
				objectname.erase(pos);
			}
		}
		
		// if the Renderinfo already has a an object with this name, edit it
		// otherwise, create it
		MovableObjectInfo* minfo = m_edited_renderinfo.getObject(objectname);
		MovableObjectInfo newminfo;
		bool addnew = false;
		
		if (minfo == 0)
		{
			// no mainmesh yet
			minfo = &newminfo;
			addnew = true;
		}
		
		// edit the Object
		minfo->m_type = MovableObjectInfo::ENTITY;
		minfo->m_objectname = objectname;
		minfo->m_source = meshname;
		
		if (addnew)
		{
			m_edited_renderinfo.addObject(*minfo);
		}
		m_modified_renderinfo = true;
		
		updateSubmeshEditor(objectname, true);
	}
	return true;
}

bool ContentEditor::onSubMeshModified(const CEGUI::EventArgs& evt)
{
	if (m_no_cegui_events)
		return true;
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	// the the GUI parts
	CEGUI::Spinner* rotXspinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMRotateX"));
	CEGUI::Spinner* rotYspinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMRotateY"));
	CEGUI::Spinner* rotZspinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMRotateZ"));
	
	CEGUI::Spinner* posXspinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMOffsetX"));
	CEGUI::Spinner* posYspinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMOffsetY"));
	CEGUI::Spinner* posZspinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMOffsetZ"));
	
	CEGUI::Spinner* scalespinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("RITab/SM/SMScale"));
	
	CEGUI::Combobox* objSelector = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("RITab/SubMesh/EditSMSelector"));
	CEGUI::Combobox* boneobjSelector = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("RITab/SubMesh/AttachMeshSelector"));
	CEGUI::Combobox* boneSelector = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("RITab/SubMesh/BoneSelector"));
	CEGUI::Checkbox* attachCheckbox = static_cast<CEGUI::Checkbox*>(win_mgr.getWindow("RITab/SM/AttachSMCheckbox"));
	
	// get the MovableObjectInfo
	CEGUI::ListboxItem* item = objSelector->getSelectedItem();
	if (item == 0)
		return true;
	
	MovableObjectInfo* minfo = m_edited_renderinfo.getObject(item->getText().c_str());
	if (minfo == 0)
		return true;
	
	// update MovableObjectInfo data
	updateBoneList();
	
	// the bone
	minfo->m_bone = "";
	if (attachCheckbox->isSelected())
	{
		std::string objname="";
		std::string bone="";
		CEGUI::ListboxItem* boneobjitem = boneobjSelector->getSelectedItem();
		if (boneobjitem != 0)
		{
			objname = boneobjitem->getText().c_str();
		}
		CEGUI::ListboxItem* boneitem = boneSelector->getSelectedItem();
		if (boneitem != 0)
		{
			bone = boneitem->getText().c_str();
		}
		
		if (objname != "" && objname != "mainmesh" && bone != "")
		{
			minfo->m_bone += objname;
			minfo->m_bone += ":";
		}
		minfo->m_bone += bone;
	}
	
	// rotation
	minfo->m_rotation[0] = rotXspinner->getCurrentValue();
	minfo->m_rotation[1] = rotYspinner->getCurrentValue();
	minfo->m_rotation[2] = rotZspinner->getCurrentValue();
	
	// position
	minfo->m_position[0] = posXspinner->getCurrentValue();
	minfo->m_position[1] = posYspinner->getCurrentValue();
	minfo->m_position[2] = posZspinner->getCurrentValue();
	
	// scale
	minfo->m_scale = scalespinner->getCurrentValue();

	m_modified_renderinfo = true;
	
	return true;
}

bool ContentEditor::onSubMeshDeleted(const CEGUI::EventArgs& evt)
{
	if (m_no_cegui_events)
		return true;
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Combobox* objSelector = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("RITab/SubMesh/EditSMSelector"));
	
	// get the MovableObjectInfo
	CEGUI::ListboxItem* item = objSelector->getSelectedItem();
	if (item == 0)
		return true;
	
	MovableObjectInfo* minfo = m_edited_renderinfo.getObject(item->getText().c_str());
	if (minfo == 0)
		return true;
	
	// Remove it from the renderinfo
	m_edited_renderinfo.removeObject(minfo->m_objectname);
	
	m_modified_renderinfo = true;
	return true;
}

bool ContentEditor::onRenderinfoXMLModified(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::MultiLineEditbox* editor = static_cast<CEGUI::MultiLineEditbox*>(win_mgr.getWindow("RITab/XML/RIXMLEditbox"));
	
	// Parse the editor text to XML
	// use temporary XML document for recovering from errors
	TiXmlDocument ri_temp_xml;
	ri_temp_xml.Parse(editor->getText().c_str());
	
	if (!ri_temp_xml.Error())
	{
		// copy the first Element to the real Renderinfo
		// first, remove the old root
		TiXmlElement* oldroot = m_renderinfo_xml.RootElement();
		if (oldroot != 0)
		{
			m_renderinfo_xml.RemoveChild(oldroot);
		}
		m_renderinfo_xml.LinkEndChild(ri_temp_xml.RootElement()->Clone());
		
		// parse the XML to the renderinfo
		// firs,t clear the RenderInfo
		m_edited_renderinfo.clear();
		GraphicManager::loadRenderInfo(m_renderinfo_xml.FirstChildElement(), &m_edited_renderinfo);
		
		m_modified_renderinfo_xml = true;
	}
	else
	{
		// XML parse error
		// set the cursor to the position of the first error
		int err_row = ri_temp_xml.ErrorRow();
		int err_col = ri_temp_xml.ErrorCol();
		
		int row =1, col = 1;	
		int pos = 0;	// cursor position found
		const CEGUI::String& text = editor->getText();
		while (pos < text.size())
		{
			// second condition ensures, that cursor is placed on the end,
			// if the row err_row is shorter than err_col for some reason
			if ((row == err_row && col == err_col)
				|| (row > err_row))
			{
				break;
			}
			
			if (text.compare(pos,1,"\n") == 0)
			{
				row++;
				col = 1;
			}
			else
			{
				col ++;
			}
			pos++;
		}
		
		editor->setCaratIndex(pos);
		editor->ensureCaratIsVisible();
		editor->activate();
	}
	return true;
}

ContentEditor* ContentEditor::getSingletonPtr(void)
{
	return ms_Singleton;
}

ContentEditor& ContentEditor::getSingleton(void)
{
	assert( ms_Singleton );
	return ( *ms_Singleton );
}


