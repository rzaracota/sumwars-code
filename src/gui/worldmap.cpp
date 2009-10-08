#include "worldmap.h"


Worldmap::Worldmap(Document* doc)
	:Window(doc)
{
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	

	// Rahmen fuer das Menue Savegame auswaehlen
	
	
	CEGUI::FrameWindow* worldmap = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "WorldmapWindow");
	m_window = worldmap;
	
	worldmap->setPosition(CEGUI::UVector2(cegui_reldim(0.15f), cegui_reldim( 0.05f))); //0.0/0.8
	worldmap->setSize(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim( 0.8f))); //1.0/0.2
	worldmap->setProperty("FrameEnabled","false");
	worldmap->setProperty("TitlebarEnabled","false");
	worldmap->setProperty("CloseButtonEnabled","false");
	worldmap->setAlpha (0.0);
	
	CEGUI::Window* label;
	label = win_mgr.createWindow("TaharezLook/StaticImage", "WorldmapImage");
	worldmap->addChildWindow(label);
	label->setProperty("FrameEnabled", "false");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.00f), cegui_reldim( 0.00)));
	label->setSize(CEGUI::UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
	label->setMousePassThroughEnabled(true);
	label->setInheritsAlpha (false);
	label->setAlpha(1.0);
	
}

void Worldmap::update()
{
	Player* player = m_document->getLocalPlayer();
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::FrameWindow* worldmap = (CEGUI::FrameWindow*) win_mgr.getWindow("WorldmapWindow");
	
	static int ncount =0;
	
	std::map<short,WaypointInfo>& winfos = World::getWorld()->getWaypointData();
	std::map<short,WaypointInfo>::iterator it;
	
	std::ostringstream stream;
	int cnt =0;
	
	CEGUI::Window* label;
	Vector pos;
	
	// Schleife ueber alle Wegpunkte
	for (it = winfos.begin(); it != winfos.end(); ++it)
	{
		// nur Wegpunkte die der Spieler schon gefunden hat
		if (!player->checkWaypoint(it->first))
			continue;
		
		stream.str("");
		stream << "WaypointImage"<<cnt;
		
		if (cnt >= ncount)
		{
			label = win_mgr.createWindow("TaharezLook/StaticImage", stream.str());
			worldmap->addChildWindow(label);
			label->setProperty("FrameEnabled", "false");
			label->setProperty("BackgroundEnabled", "true");
			label->setProperty("BackgroundColours", "tl:00000000 tr:00000000 bl:00000000 br:00000000"); 
			label->setSize(CEGUI::UVector2(cegui_reldim(0.02f), cegui_reldim( 0.02f)));
			label->setProperty("Image", "set:TaharezLook image:RadioButtonNormal"); 
			label->setInheritsAlpha (false);
			label->setAlwaysOnTop(true);
			label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Worldmap::onWaypointClicked, this));
			
			ncount ++;
		}
		else
		{
			label = win_mgr.getWindow(stream.str());
		}
		
		pos = it->second.m_world_coord;
		
		label->setID(it->first);
		label->setPosition(CEGUI::UVector2(cegui_reldim(pos.m_x), cegui_reldim(pos.m_y)));
		label->setVisible(true);
		label->setTooltipText((CEGUI::utf8*) dgettext("sumwars_xml",it->second.m_name.c_str()));
		cnt++;
	}
	
	RegionLocation& portal = player->getPortalPosition();
	if (portal.first != "")
	{
		short id = World::getWorld()->getRegionId(portal.first);
		it = winfos.find(id);
		
		if (it != winfos.end())
		{
		
			stream.str("");
			stream << "WaypointImage"<<cnt;
			
			if (cnt >= ncount)
			{
				label = win_mgr.createWindow("TaharezLook/StaticImage", stream.str());
				worldmap->addChildWindow(label);
				label->setProperty("FrameEnabled", "false");
				label->setProperty("BackgroundEnabled", "true");
				label->setProperty("BackgroundColours", "tl:00000000 tr:00000000 bl:00000000 br:00000000"); 
				label->setSize(CEGUI::UVector2(cegui_reldim(0.03f), cegui_reldim( 0.03f)));
				label->setProperty("Image", "set:TaharezLook image:RadioButtonMark"); 
				label->setInheritsAlpha (false);
				label->setAlwaysOnTop(true);
				label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Worldmap::onWaypointClicked, this));
				
				ncount ++;
			}
			else
			{
				label = win_mgr.getWindow(stream.str());
			}
			
			
			pos = it->second.m_world_coord;
			
			label->setID(-1);
			if (player->checkWaypoint(id))
			{
				label->setPosition(CEGUI::UVector2(cegui_reldim(pos.m_x+0.015f), cegui_reldim(pos.m_y)));
			}
			else
			{
				label->setPosition(CEGUI::UVector2(cegui_reldim(pos.m_x), cegui_reldim(pos.m_y)));				
			}
			label->setVisible(true);
			std::stringstream stream;
			stream << dgettext("sumwars_xml","Town Portal") << "\n";
			stream << dgettext("sumwars_xml",it->second.m_name.c_str());
			label->setTooltipText((CEGUI::utf8*) stream.str().c_str());
			
			cnt++;
		}
		else
		{
			ERRORMSG("region %s has no world position", portal.first.c_str());
		}
	}
	
	// restliche Label verstecken
	for (; cnt <ncount; cnt++)
	{
		stream.str("");
		stream << "WaypointImage";
		stream << cnt;
			
		label = win_mgr.getWindow(stream.str());
		label->setVisible(false);
	}
}

void Worldmap::updateTranslation()
{
	
}


bool Worldmap::onWaypointClicked(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();
	
	DEBUG5("waypoint %i clicked",id);
	m_document->onButtonWaypointClicked(id);
	
	return true;
}

