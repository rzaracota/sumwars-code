#include "CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h"
#include <OgreHardwarePixelBuffer.h>
#include <OgreMeshManager.h>

#include "graphicmanager.h"
#include "itemeditor.h"
#include "renderinfoeditor.h"
#include "contenteditor.h"
#include "player.h"

void ItemEditor::init(CEGUI::Window* parent)
{
	ContentEditorTab::init(parent);
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	CEGUI::PushButton* xmlsubmitButton = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("ItemTab/XML/SubmitButton"));
	xmlsubmitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ItemEditor::onItemXMLModified, this));
	
	CEGUI::PushButton* createDropButton = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("ItemTab/Create/CreateDropButton"));
	createDropButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ItemEditor::onItemCreateDrop, this));
	
	CEGUI::PushButton* createInvButton = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("ItemTab/Create/CreateInventoryButton"));
	createInvButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ItemEditor::onItemCreateInInventory, this));
	
	CEGUI::Spinner* enchantMin =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("ItemTab/General/EnchantMinSpinner"));
	enchantMin->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&ItemEditor::onItemModified, this));
	
	CEGUI::Spinner* enchantMax =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("ItemTab/General/EnchantMaxSpinner"));
	enchantMax->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&ItemEditor::onItemModified, this));
	
	CEGUI::Spinner* price =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("ItemTab/General/PriceSpinner"));
	price->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&ItemEditor::onItemModified, this));
	
	CEGUI::Editbox* nameBox = static_cast<CEGUI::Editbox*>(win_mgr.getWindow("ItemTab/General/NameBox"));
	nameBox->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&ItemEditor::onItemModified, this));
	
	CEGUI::Combobox* typeSelector = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("ItemTab/Properties/TypeBox"));
	typeSelector->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&ItemEditor::onItemModified, this));
	typeSelector->addItem(new CEGUI::ListboxTextItem("Armor"));
	typeSelector->addItem(new CEGUI::ListboxTextItem("Weapon"));
	typeSelector->addItem(new CEGUI::ListboxTextItem("Helmet"));
	typeSelector->addItem(new CEGUI::ListboxTextItem("Gloves"));
	typeSelector->addItem(new CEGUI::ListboxTextItem("Ring"));
	typeSelector->addItem(new CEGUI::ListboxTextItem("Amulet"));
	typeSelector->addItem(new CEGUI::ListboxTextItem("Shield"));
	typeSelector->addItem(new CEGUI::ListboxTextItem("Potion"));
	typeSelector->setText("Armor");
	typeSelector->setSelection(0,0);
	
	
	CEGUI::Combobox* sizeSelector = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("ItemTab/Properties/SizeBox"));
	sizeSelector->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&ItemEditor::onItemModified, this));
	sizeSelector->addItem(new CEGUI::ListboxTextItem("Big"));
	sizeSelector->addItem(new CEGUI::ListboxTextItem("Medium"));
	sizeSelector->addItem(new CEGUI::ListboxTextItem("Small"));
	sizeSelector->setText("Big");
	sizeSelector->setSelection(0,0);
	
	CEGUI::PushButton* copyfoButton = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("ItemTab/Properties/CopyDataButton"));
	copyfoButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ItemEditor::onCopyData, this));
	
	// init the internal data
	TiXmlElement * item_root = new TiXmlElement("Item");  
	m_item_xml.LinkEndChild( item_root );  
	item_root->SetAttribute("subtype","EditorItem");
	
	TiXmlElement * item_image = new TiXmlElement("Image");  
	item_image->SetAttribute("image","set:noMedia.png image:full_image");
	item_root->LinkEndChild(item_image);
	
	TiXmlElement * item_ri = new TiXmlElement("RenderInfo");  
	item_ri->SetAttribute("name","EditorRenderInfo");
	item_root->LinkEndChild(item_ri);
	
	// init the data
	m_edited_item.m_subtype = "EditorItem";
	
	m_update_base_content = true;
	m_modified_item = true;
	
	// create camera for item photos
	Ogre::SceneManager* editor_scene_mng = Ogre::Root::getSingleton().getSceneManager("EditorSceneManager");
	Ogre::Camera* item_camera = editor_scene_mng->createCamera("item_camera");
	item_camera->setNearClipDistance(0.1);
	item_camera->setAspectRatio(1.0);
	item_camera->setPosition(Ogre::Vector3(0,1,0));
	item_camera->lookAt(Ogre::Vector3(0,0,0));
	
	m_unique_id = 1;

}

void ItemEditor::update()
{
	if (m_update_base_content == true)
	{
		updateAllItemList();
		m_update_base_content = false;
	}
	
	if (m_modified_item)
	{
		updateItemXML();
	}
	
	if (m_modified_item_xml)
	{
		updateItemEditor();
	}
	
	m_modified_item = false;
	m_modified_item_xml = false;
}

void ItemEditor::updateAllItemList()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	CEGUI::Combobox* copyItemSelector = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("ItemTab/Properties/CopyDataBox"));
	
	// Fill list of all FixedObjects
	const std::map<Item::Subtype,ItemBasicData*>& all_items = ItemFactory::getAllItemData();
	std::map<Item::Subtype,ItemBasicData*>::const_iterator it;
	for (it = all_items.begin(); it != all_items.end(); ++it)
	{
		copyItemSelector->addItem(new CEGUI::ListboxTextItem(it->first.c_str()));
	}
}

bool ItemEditor::onItemModified(const CEGUI::EventArgs& evt)
{
	if (m_no_cegui_events)
		return true;
	m_no_cegui_events = true;
	
	m_edited_item.m_price = (int) getSpinnerValue("ItemTab/General/PriceSpinner",1);
	m_edited_item.m_min_enchant = getSpinnerValue("ItemTab/General/EnchantMinSpinner",1);
	m_edited_item.m_max_enchant = getSpinnerValue("ItemTab/General/EnchantMaxSpinner",1);
	if (m_edited_item.m_max_enchant < m_edited_item.m_min_enchant)
	{
		m_edited_item.m_max_enchant = m_edited_item.m_min_enchant;
		setSpinnerValue("ItemTab/General/EnchantMaxSpinner",m_edited_item.m_max_enchant);
	}
	m_edited_item.m_name = getWindowText("ItemTab/General/NameBox","EditorItem");
	
	std::string type = getWindowText("ItemTab/Properties/TypeBox","Weapon");
	m_edited_item.m_type = Item::WEAPON;
	if (type == "Armor") m_edited_item.m_type = Item::ARMOR;
	else if (type == "Weapon") m_edited_item.m_type = Item::WEAPON;
	else if (type == "Helmet") m_edited_item.m_type = Item::HELMET;
	else if (type == "Gloves") m_edited_item.m_type = Item::GLOVES;
	else if (type == "Ring") m_edited_item.m_type = Item::RING;
	else if (type == "Amulet") m_edited_item.m_type = Item::AMULET;
	else if (type == "Shield") m_edited_item.m_type = Item::SHIELD;
	else if (type == "Potion") m_edited_item.m_type = Item::POTION;

	std::string size = getWindowText("ItemTab/Properties/SizeBox","Big");
	m_edited_item.m_size = Item::BIG;
	if (size == "Big") m_edited_item.m_size = Item::BIG;
	else if (size == "Medium") m_edited_item.m_size = Item::MEDIUM;
	else if (size == "Small") m_edited_item.m_size = Item::SMALL;

	m_no_cegui_events = false;
	m_modified_item = true;
	
	return true;
}

bool ItemEditor::onItemXMLModified(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::MultiLineEditbox* editor = static_cast<CEGUI::MultiLineEditbox*>(win_mgr.getWindow("ItemTab/XML/ItemXMLEditbox"));
	
	// Parse the editor text to XML
	// use temporary XML document for recovering from errors
	TiXmlDocument item_temp_xml;
	item_temp_xml.Parse(editor->getText().c_str());
	
	if (!item_temp_xml.Error())
	{
		// first, remove the old root
		TiXmlElement* oldroot = m_item_xml.RootElement();
		if (oldroot != 0)
		{
			m_item_xml.RemoveChild(oldroot);
		}
		// clone the first Element
		m_item_xml.LinkEndChild(item_temp_xml.RootElement()->Clone());
		
		// parse the XML to the fixed object data
		std::string subtype = ItemLoader::loadItem(m_item_xml.FirstChildElement(),true);
		// copy to the local Data structure
		ItemBasicData* data = ItemFactory::getItemBasicData(subtype);
		if (data != 0)
		{
			m_edited_item = *data;
		}
		
		m_modified_item_xml = true;
	}
	else
	{
		// XML parse error
		// set the cursor to the position of the first error
		int err_row = item_temp_xml.ErrorRow();
		int err_col = item_temp_xml.ErrorCol();
		
		setMultiLineEditboxCursor("ItemTab/XML/ItemXMLEditbox",err_row,err_col);
	}
	
	return true;
}

Item* ItemEditor::createItem()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	// create an item picture
	std::stringstream idstream;
	idstream << m_unique_id;
	
	Ogre::SceneManager* editor_scene_mng = Ogre::Root::getSingleton().getSceneManager("EditorSceneManager");
	Ogre::Camera* item_camera = editor_scene_mng->getCamera("item_camera");
	
	// texture that is created from the camera image
	Ogre::TexturePtr item_texture = Ogre::TextureManager::getSingleton().createManual( std::string("item_tex_") + idstream.str(),
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
   256, 256, 0, Ogre::PF_R8G8B8A8, Ogre::TU_RENDERTARGET );
	
   // connection texture and camera via RenderTarget
	Ogre::RenderTarget* item_rt = item_texture->getBuffer()->getRenderTarget();
	item_rt->setAutoUpdated(false);
	Ogre::Viewport *item_view = item_rt->addViewport(item_camera );
	item_view->setClearEveryFrame( false );
	item_view->setOverlaysEnabled (false);
	item_view->setBackgroundColour(Ogre::ColourValue(0,0,1,1.0) );

	// create a CEGUI Image from the Texture
    CEGUI::Texture& item_ceguiTex = static_cast<CEGUI::OgreRenderer*>(CEGUI::System::getSingleton().getRenderer())->createTexture(item_texture);
    
	CEGUI::Imageset& item_textureImageSet = CEGUI::ImagesetManager::getSingleton().create(std::string("item_imageset_") + idstream.str(), item_ceguiTex);

	item_textureImageSet.defineImage( "item_img_"+ idstream.str(),
			CEGUI::Point( 0.0f, 0.0f ),
			CEGUI::Size( item_ceguiTex.getSize().d_width, item_ceguiTex.getSize().d_height ),
			CEGUI::Point( 0.0f, 0.0f ) );
	
	item_rt->update();
	
	CEGUI::Window* label = win_mgr.getWindow("ItemTab/BM/itemPreview");

	// reparse and update the FixedObject Data
	// create a unique renderinfo (to avoid that the object is modified by the editor after creation)
	RenderInfoEditor* ri_editor = dynamic_cast<RenderInfoEditor*>(ContentEditor::getSingleton().getComponent("RIEditor"));
	std::string unique_ri = ri_editor->getUniqueRenderinfo();
	// temporarily replace the renderinfo name
	TiXmlElement * item_ri = m_item_xml.RootElement()->FirstChildElement("RenderInfo");
	if (item_ri == 0)
		return 0;
	std::string name = item_ri->Attribute("name");
	item_ri->SetAttribute("name",unique_ri.c_str());
	
	// temporarily replace the image name
	TiXmlElement * item_image = m_item_xml.RootElement()->FirstChildElement("Image");
	if (item_image == 0)
		return 0;
	std::string image = item_image->Attribute("image");
	
	if (image == "set:noMedia.png image:full_image")
	{
		std::stringstream itemimage;
		itemimage <<  "set:item_imageset_" << idstream.str()<< " " << "image:item_img_" << idstream.str();
		DEBUG("item %s", itemimage.str().c_str());
		itemimage.str("");
		itemimage << "set:editor_imageset image:editor_img";
		item_image->SetAttribute("image",itemimage.str().c_str());
		label->setProperty("Image", itemimage.str().c_str());
		
		DEBUG("image info %s %s %i",item_textureImageSet.getName().c_str(),("item_img_"+ idstream.str()).c_str(), item_textureImageSet.isImageDefined("item_img_"+ idstream.str()));
	}

	// make the item subtype unique by adding a number
	std::string plain_subtype = m_item_xml.RootElement()->Attribute("subtype");
	std::stringstream stream;
	stream << plain_subtype << m_unique_id;
	m_unique_id++;
	m_item_xml.RootElement()->SetAttribute("subtype",stream.str().c_str());
	
	std::string subtype = ItemLoader::loadItem(m_item_xml.FirstChildElement(),true);
	
	// reset the changed attributes
	item_ri->SetAttribute("name",name.c_str());
	m_item_xml.RootElement()->SetAttribute("subtype",plain_subtype.c_str());
	item_image->SetAttribute("image",image.c_str());
	
	float magic = getSpinnerValue("ItemTab/Create/EnchantSpinner",0);
	Item::Type type = ItemFactory::getBaseType(subtype);
	Item* item = ItemFactory::createItem(type, subtype, 0, magic);
	return item;
}

bool ItemEditor::onItemCreateDrop(const CEGUI::EventArgs& evt)
{
	World* world = World::getWorld();
	if (world == 0)
		return true;
	
	WorldObject* player = world->getLocalPlayer();
	if (player == 0)
		return true;
	Region* region = player->getRegion();
	
	// if the position is set to default, use the player position
	Vector pos = player->getShape()->m_center;
	
	// create the object
	Item* item = createItem();
	if (item == 0)
		return true;
	region->dropItem(item,pos);
	
	return true;
}

bool ItemEditor::onItemCreateInInventory(const CEGUI::EventArgs& evt)
{
	World* world = World::getWorld();
	if (world == 0)
		return true;
	
	WorldObject* player = world->getLocalPlayer();
	if (player == 0)
		return true;

	Item* item = createItem();
	if (item == 0)
		return true;
	static_cast<Player*>(player)->insertItem(item);
	return true;
}

bool ItemEditor::onCopyData(const CEGUI::EventArgs& evt)
{
	std::string objname	= getComboboxSelection("ItemTab/Properties/CopyDataBox", "");
	if (objname == "")
		return true;
	
	ItemBasicData* data = ItemFactory::getItemBasicData(objname);
	data->m_subtype = "EditorItem";
	
	if (data == 0)
		return true;
	
	m_edited_item = *data;
	
	m_modified_item = true;
	m_modified_item_xml = true;
	
	return true;
}

void ItemEditor::updateItemXML()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	// update the XML representation
	m_edited_item.writeToXML(m_item_xml.FirstChildElement());
	
	// write to the editor
	TiXmlPrinter printer;
	m_item_xml.Accept(&printer);
	
	CEGUI::MultiLineEditbox* editor = static_cast<CEGUI::MultiLineEditbox*>(win_mgr.getWindow("ItemTab/XML/ItemXMLEditbox"));
	editor->setText(printer.CStr());
}

void ItemEditor::updateItemEditor()
{
	m_no_cegui_events = true;
	
	setSpinnerValue("ItemTab/General/PriceSpinner",m_edited_item.m_price);
	setSpinnerValue("ItemTab/General/EnchantMinSpinner",m_edited_item.m_min_enchant);
	setSpinnerValue("ItemTab/General/EnchantMaxSpinner",m_edited_item.m_max_enchant);
	setWindowText("ItemTab/General/NameBox",m_edited_item.m_name);
	
	Item::Type type = m_edited_item.m_type;
	if (type == Item::ARMOR) setComboboxSelection("ItemTab/Properties/TypeBox","Armor");
	else if (type == Item::WEAPON) setComboboxSelection("ItemTab/Properties/TypeBox","Weapon");
	else if (type == Item::HELMET) setComboboxSelection("ItemTab/Properties/TypeBox","Helmet");
	else if (type == Item::GLOVES) setComboboxSelection("ItemTab/Properties/TypeBox","Gloves");
	else if (type == Item::RING) setComboboxSelection("ItemTab/Properties/TypeBox","Ring");
	else if (type == Item::AMULET) setComboboxSelection("ItemTab/Properties/TypeBox","Amulet");
	else if (type == Item::SHIELD) setComboboxSelection("ItemTab/Properties/TypeBox","Shield");
	else if (type == Item::POTION) setComboboxSelection("ItemTab/Properties/TypeBox","Potion");
	
	Item::Size size = m_edited_item.m_size;
	if (size == Item::BIG) setComboboxSelection("ItemTab/Properties/SizeBox","Big");
	else if (size == Item::MEDIUM) setComboboxSelection("ItemTab/Properties/SizeBox","Medium");
	else if (size == Item::SMALL) setComboboxSelection("ItemTab/Properties/SizeBox","Small");

	m_no_cegui_events = false;
}



