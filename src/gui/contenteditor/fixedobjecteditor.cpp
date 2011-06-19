
#include "graphicmanager.h"

#include "fixedobjecteditor.h"
#include "world.h"
#include "renderinfoeditor.h"
#include "contenteditor.h"


void FixedObjectEditor::init(CEGUI::Window* parent)
{
	ContentEditorTab::init(parent);
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	CEGUI::PushButton* detectCircleButton = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("FOTab/Prop/DetectCircleButton"));
	CEGUI::PushButton* detectRectButton = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("FOTab/Prop/DetectRectButton"));
	
	CEGUI::Spinner* radiusSpinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("FOTab/Prop/RadiusSpinner"));
	CEGUI::Spinner* widthSpinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("FOTab/Prop/WidthSpinner"));
	CEGUI::Spinner* depthSpinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("FOTab/Prop/DepthSpinner"));
	
	CEGUI::RadioButton* circleButton = static_cast<CEGUI::RadioButton*>(win_mgr.getWindow("FOTab/Prop/CircleCheckbox"));
	CEGUI::RadioButton* rectButton = static_cast<CEGUI::RadioButton*>(win_mgr.getWindow("FOTab/Prop/RectangleCheckbox"));
	
	radiusSpinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&FixedObjectEditor::onFixedObjectModified, this));
	widthSpinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&FixedObjectEditor::onFixedObjectModified, this));
	depthSpinner->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&FixedObjectEditor::onFixedObjectModified, this));
	
	circleButton->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&FixedObjectEditor::onFixedObjectModified, this));
	rectButton->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&FixedObjectEditor::onFixedObjectModified, this));
	circleButton->setSelected(true);
	
	detectCircleButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&FixedObjectEditor::onFixedObjectAutodetectSize, this));
	detectRectButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&FixedObjectEditor::onFixedObjectAutodetectSize, this));
	
	CEGUI::Combobox* layerSelector = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("FOTab/Prop/LayerSelector"));
	layerSelector->addItem(new CEGUI::ListboxTextItem("Normal"));
	layerSelector->addItem(new CEGUI::ListboxTextItem("Base"));
	layerSelector->addItem(new CEGUI::ListboxTextItem("Air"));
	layerSelector->addItem(new CEGUI::ListboxTextItem("NoCollision"));
	
	layerSelector->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&FixedObjectEditor::onFixedObjectModified, this));
	
	CEGUI::PushButton* createFOButton = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("FOTab/XML/CreateButton"));
	createFOButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&FixedObjectEditor::onFixedObjectCreate, this));
	
	// init the internal data
	TiXmlElement * fixed_root = new TiXmlElement("Object");  
	m_fixed_object_xml.LinkEndChild( fixed_root );  
	fixed_root->SetAttribute("subtype","EditorFixedObject");
	
	TiXmlElement * fixed_ri = new TiXmlElement("RenderInfo");  
	fixed_ri->SetAttribute("name","EditorRenderInfo");
	fixed_root->LinkEndChild(fixed_ri);
}

void FixedObjectEditor::update()
{
	if (m_modified_fixed_object)
	{
		updateFixedObjectXML();
	}
	
	if (m_modified_fixed_object_xml)
	{
		updateFixedObjectEditor();
	}
	
	m_modified_fixed_object = false;
	m_modified_fixed_object_xml = false;
}

void FixedObjectEditor::updateFixedObjectXML()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	// update the XML representation
	m_edited_fixed_object.writeToXML(m_fixed_object_xml.FirstChildElement());
	
	// write to the editor
	TiXmlPrinter printer;
	m_fixed_object_xml.Accept(&printer);
	
	CEGUI::MultiLineEditbox* editor = static_cast<CEGUI::MultiLineEditbox*>(win_mgr.getWindow("FOTab/XML/FOXMLEditbox"));
	editor->setText(printer.CStr());
}

void FixedObjectEditor::updateFixedObjectEditor()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	CEGUI::Combobox* layerSelector = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("FOTab/Prop/LayerSelector"));
	
	CEGUI::Spinner* radiusSpinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("FOTab/Prop/RadiusSpinner"));
	CEGUI::Spinner* widthSpinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("FOTab/Prop/WidthSpinner"));
	CEGUI::Spinner* depthSpinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("FOTab/Prop/DepthSpinner"));
	
	CEGUI::RadioButton* circleButton = static_cast<CEGUI::RadioButton*>(win_mgr.getWindow("FOTab/Prop/CircleCheckbox"));
	CEGUI::RadioButton* rectButton = static_cast<CEGUI::RadioButton*>(win_mgr.getWindow("FOTab/Prop/RectangleCheckbox"));
	
	// adjust the spinner values
	if (circleButton->getSelectedButtonInGroup() == circleButton)
	{
		radiusSpinner->setCurrentValue(m_edited_fixed_object.m_shape.m_radius);
		circleButton->setSelected(true);
	}
	else
	{
		widthSpinner->setCurrentValue(m_edited_fixed_object.m_shape.m_extent.m_x*2);
		depthSpinner->setCurrentValue(m_edited_fixed_object.m_shape.m_extent.m_y*2);
		rectButton->setSelected(true);
	}
	
	// set the layer combobox
	std::string layer = "Normal";
	if (m_edited_fixed_object.m_layer == WorldObject::LAYER_BASE)
		layer = "Base";
	else if (m_edited_fixed_object.m_layer == WorldObject::LAYER_AIR)
		layer = "Air";
	else if (m_edited_fixed_object.m_layer == WorldObject::LAYER_NOCOLLISION)
		layer == "NoCollision";
	
	CEGUI::ListboxItem* selection = layerSelector->findItemWithText(CEGUI::String(layer),0);
	if (selection != 0)
	{
		int id = selection->getID();
		layerSelector->setSelection(id,id);
	}
}


bool FixedObjectEditor::onFixedObjectModified(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	CEGUI::RadioButton* CircleButton = static_cast<CEGUI::RadioButton*>(win_mgr.getWindow("FOTab/Prop/CircleCheckbox"));
	CEGUI::RadioButton* RectButton = static_cast<CEGUI::RadioButton*>(win_mgr.getWindow("FOTab/Prop/RectangleCheckbox"));
	
	CEGUI::Spinner* radiusSpinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("FOTab/Prop/RadiusSpinner"));
	CEGUI::Spinner* widthSpinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("FOTab/Prop/WidthSpinner"));
	CEGUI::Spinner* depthSpinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("FOTab/Prop/DepthSpinner"));
	
	CEGUI::Combobox* layerSelector = static_cast<CEGUI::Combobox*>(win_mgr.getWindow("FOTab/Prop/LayerSelector"));
	
	if (CircleButton->getSelectedButtonInGroup() == CircleButton)
	{
		m_edited_fixed_object.m_shape.m_type = Shape::CIRCLE;
		m_edited_fixed_object.m_shape.m_radius = radiusSpinner->getCurrentValue();
	}
	else
	{
		m_edited_fixed_object.m_shape.m_type = Shape::RECT;
		m_edited_fixed_object.m_shape.m_extent.m_x = 0.5*widthSpinner->getCurrentValue();
		m_edited_fixed_object.m_shape.m_extent.m_y = 0.5*depthSpinner->getCurrentValue();
	}
	
	std::string layer = layerSelector->getText().c_str();
	if (layer == "Base")
		m_edited_fixed_object.m_layer = WorldObject::LAYER_BASE;
	else if (layer == "Air")
		m_edited_fixed_object.m_layer = WorldObject::LAYER_AIR;
	else if (layer == "NoCollision")
		m_edited_fixed_object.m_layer = WorldObject::LAYER_NOCOLLISION;
	else
		m_edited_fixed_object.m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
	
	m_modified_fixed_object = true;
}

bool FixedObjectEditor::onFixedObjectAutodetectSize(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
	static_cast<const CEGUI::MouseEventArgs&>(evt);
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	CEGUI::PushButton* detectCircleButton = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("FOTab/Prop/DetectCircleButton"));
	CEGUI::PushButton* detectRectButton = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("FOTab/Prop/DetectRectButton"));
	
	CEGUI::Spinner* radiusSpinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("FOTab/Prop/RadiusSpinner"));
	CEGUI::Spinner* widthSpinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("FOTab/Prop/WidthSpinner"));
	CEGUI::Spinner* depthSpinner =  static_cast<CEGUI::Spinner*>(win_mgr.getWindow("FOTab/Prop/DepthSpinner"));
	
	// calculate the size of the bounding box
	RenderInfoEditor* ri_editor = dynamic_cast<RenderInfoEditor*>(ContentEditor::getSingleton().getComponent("RIEditor"));
	Ogre::SceneNode* topnode = ri_editor->getEditedGraphicObject()->getTopNode();
	topnode->_updateBounds();
	
	Ogre::Vector3 bbox_min(1000,1000,1000);
	Ogre::Vector3 bbox_max(-1000,-1000,-1000);
	
	getNodeBounds(topnode,bbox_min,bbox_max);
	
	double size_x = bbox_max[0] - bbox_min[0];
	double size_z = bbox_max[2] - bbox_min[2];
	
	// set spinner values according to boundingbox
	if (we.window == detectCircleButton)
	{
		radiusSpinner->setCurrentValue(0.5*sqrt(size_x*size_x + size_z*size_z));
	}
	else
	{
		widthSpinner->setCurrentValue(size_x);
		depthSpinner->setCurrentValue(size_z);
	}
	
	m_modified_fixed_object = true;
}

bool FixedObjectEditor::onFixedObjectXMLModified(const CEGUI::EventArgs& evt)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::MultiLineEditbox* editor = static_cast<CEGUI::MultiLineEditbox*>(win_mgr.getWindow("FOTab/XML/FOXMLEditbox"));
	
	// Parse the editor text to XML
	// use temporary XML document for recovering from errors
	TiXmlDocument ri_temp_xml;
	ri_temp_xml.Parse(editor->getText().c_str());
	
	if (!ri_temp_xml.Error())
	{
		// copy the first Element to the real Renderinfo
		// first, remove the old root
		TiXmlElement* oldroot = m_fixed_object_xml.RootElement();
		if (oldroot != 0)
		{
			m_fixed_object_xml.RemoveChild(oldroot);
		}
		m_fixed_object_xml.LinkEndChild(ri_temp_xml.RootElement()->Clone());
		
		// parse the XML to the fixed object data
		m_modified_fixed_object_xml = true;
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

bool FixedObjectEditor::onFixedObjectCreate(const CEGUI::EventArgs& evt)
{
	// reparse and update the FixedObject Data
	ObjectLoader::loadObject(m_fixed_object_xml.FirstChildElement(),true);
	
	World* world = World::getWorld();
	if (world == 0)
		return true;
	
	WorldObject* player = world->getLocalPlayer();
	if (player == 0)
		return true;
	Region* region = player->getRegion();
	
	Vector pos = player->getShape()->m_center;
	
	float angle = 0.0;
	float height = 0.0;
	
	DEBUG("creating object");
	region->createObject("EditorFixedObject", pos,angle, height,WorldObject::STATE_AUTO);
}

