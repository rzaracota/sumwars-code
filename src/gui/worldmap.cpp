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

#include "worldmap.h"

// Utility for CEGUI cross-version compatibility
#include "ceguiutility.h"


Worldmap::Worldmap (Document* doc, const std::string& ceguiSkinName)
	: Window (doc)
	, m_ceguiSkinName (ceguiSkinName)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	// The World map window and holder
	CEGUI::Window* worldmap = (CEGUI::FrameWindow*) CEGUIUtility::loadLayoutFromFile ("worldmapwindow.layout");
	if (!worldmap)
	{
		DEBUG ("WARNING: Failed to load [%s]", "worldmapwindow.layout");
	}

	CEGUI::Window* worldmap_holder = CEGUIUtility::loadLayoutFromFile ("worldmapwindow_holder.layout");
	if (!worldmap_holder)
	{
		DEBUG ("WARNING: Failed to load [%s]", "worldmapwindow_holder.layout");
	}
	
	CEGUI::Window* wndHolder = CEGUIUtility::getWindowForLoadedLayoutEx (worldmap_holder, "WorldmapWindow_Holder");
	CEGUI::Window* wndHolderAux = CEGUIUtility::getWindowForLoadedLayoutEx (worldmap_holder, "WorldmapWindow_Holder_aux");
	CEGUI::Window* wndHeldWindow = CEGUIUtility::getWindowForLoadedLayoutEx (worldmap, "WorldmapWindow");
	if (wndHolder && wndHeldWindow && wndHolderAux)
	{
		CEGUIUtility::addChildWidget (wndHolderAux, wndHeldWindow);
	}
	else
	{
		if (!wndHolder) DEBUG ("ERROR: Unable to get the window holder for char screen.");
		if (!wndHeldWindow) DEBUG ("ERROR: Unable to get the window for the worldmap screen.");
	}

	m_window = worldmap_holder;
}

void Worldmap::update()
{
	Player* player = m_document->getLocalPlayer();
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::FrameWindow* worldmap = (CEGUI::FrameWindow*) CEGUIUtility::getWindowForLoadedLayoutEx (m_window, "WorldmapWindow_Holder_aux/WorldmapWindow");
	
	static int ncount =0;
	static bool tpset = false;
	
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
			label = win_mgr.createWindow (CEGUIUtility::getWidgetWithSkin (m_ceguiSkinName, "StaticImage"), stream.str());
			CEGUIUtility::addChildWidget (worldmap, label);

			label->setProperty("FrameEnabled", "false");
			label->setProperty("BackgroundEnabled", "false");
			//label->setProperty("BackgroundColours", "tl:00000000 tr:00000000 bl:00000000 br:00000000"); 
			CEGUIUtility::setWidgetSizeRel (label, 0.02f, 0.02f);
			label->setProperty("Image", CEGUIUtility::getImageNameWithSkin ("SumWarsExtras", "WaypointMark")); 
			label->setInheritsAlpha (false);
			label->setAlwaysOnTop(true);
			label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Worldmap::onWaypointClicked, this));
			
			ncount ++;
		}
		else
		{
			label = CEGUIUtility::getWindowForLoadedLayoutEx (m_window, stream.str());
		}
		
		pos = it->second.m_world_coord;
		
		label->setID(it->first);
		label->setPosition(CEGUI::UVector2(cegui_reldim(pos.m_x), cegui_reldim(pos.m_y)));
		label->setVisible(true);
		label->setTooltipText((CEGUI::utf8*) dgettext("sumwars",it->second.m_name.c_str()));
		cnt++;
	}
	
	
	// restliche Label verstecken
	for (; cnt <ncount; cnt++)
	{
		stream.str("");
		stream << "WaypointImage";
		stream << cnt;
			
		label = CEGUIUtility::getWindowForLoadedLayoutEx (m_window, stream.str());
		label->setVisible(false);
	}
	RegionLocation& portal = player->getPortalPosition();
	if (portal.first != "")
	{
		short id = World::getWorld()->getRegionId(portal.first);
		it = winfos.find(id);
		
		if (it != winfos.end())
		{
		
			stream.str("");
			stream << "TownPortalImage";
			
			if (tpset==false)
			{
				label = win_mgr.createWindow (CEGUIUtility::getWidgetWithSkin (m_ceguiSkinName, "StaticImage"), stream.str());
				CEGUIUtility::addChildWidget (worldmap, label);

				label->setProperty("FrameEnabled", "false");
				label->setProperty("BackgroundEnabled", "true");

				if (label->isPropertyPresent ("BackgroundColours"))
				{
					label->setProperty("BackgroundColours", "tl:00000000 tr:00000000 bl:00000000 br:00000000"); 
				}
				else if (label->isPropertyPresent ("BackgroundColour"))
				{
					label->setProperty("BackgroundColour", "00000000");
				}
				CEGUIUtility::setWidgetSizeRel (label, 0.023f, 0.023f);

				label->setProperty("Image", CEGUIUtility::getImageNameWithSkin ("SumWarsExtras", "TownPortalSymbol"));
				label->setInheritsAlpha (false);
				label->setAlwaysOnTop(true);
				label->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Worldmap::onWaypointClicked, this));
				
				tpset = true;
			}
			else
			{
				label = CEGUIUtility::getWindowForLoadedLayoutEx (m_window, stream.str());
			}
			
			
			pos = it->second.m_world_coord;
			
			label->setID(-999);
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
			stream << dgettext("sumwars","Town Portal") << "\n";
			stream << dgettext("sumwars",it->second.m_name.c_str());
			label->setTooltipText((CEGUI::utf8*) stream.str().c_str());
		}
		else
		{
			ERRORMSG("region %s has no world position", portal.first.c_str());
		}
	}
	else {
		if(tpset == true) {
			stream.str("");
			stream << "TownPortalImage";
			label = CEGUIUtility::getWindowForLoadedLayoutEx (m_window, stream.str());
			label->setVisible(false);
		}

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
	
	DEBUGX("waypoint %i clicked",id);
	m_document->onButtonWaypointClicked(id);
	
	return true;
}

