/*
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#include "CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h"
#include "CEGUI/RendererModules/Ogre/CEGUIOgreTexture.h"
#include "CEGUI/RendererModules/Ogre/CEGUIOgreResourceProvider.h"
#include "scene.h"

#include "minimapwindow.h"

#include "OgreResource.h"

MinimapWindow::MinimapWindow (Document* doc)
	:Window(doc)
{
	m_region_id = -1;
		
	DEBUGX("setup main menu");
	// GUI Elemente erzeugen

	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	

	// Rahmen fuer das Menue Savegame auswaehlen
	
	
	CEGUI::FrameWindow* minimap = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "MinimapWindow");
	m_window = minimap;
	
	minimap->setPosition(CEGUI::UVector2(cegui_reldim(0.15f), cegui_reldim( 0.05f))); //0.0/0.8
	minimap->setSize(CEGUI::UVector2(cegui_reldim(0.7f), cegui_reldim( 0.8f))); //1.0/0.2
	minimap->setProperty("FrameEnabled","false");
	minimap->setProperty("TitlebarEnabled","false");
	minimap->setProperty("CloseButtonEnabled","false");
 	minimap->setAlpha (0.0);
	minimap->setMousePassThroughEnabled(true);
	
	
	CEGUI::Window* label;
	
	label = win_mgr.createWindow("TaharezLook/StaticText", "RegionNameLabel");
	minimap->addChildWindow(label);
	label->setProperty("FrameEnabled", "false");
	label->setProperty("BackgroundEnabled", "false");
	label->setProperty("HorzFormatting", "HorzCentred");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.01)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim( 0.06f)));
	label->setVisible(true);
	label->setAlwaysOnTop(true);
	label->setMousePassThroughEnabled(true);
	label->setID(0);
	label->setText( "test" );
	label->setInheritsAlpha (false);
	label->setAlpha(0.6);
	
	label = win_mgr.createWindow("TaharezLook/StaticImage", "MinimapImage");
	minimap->addChildWindow(label);
	label->setProperty("FrameEnabled", "false");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.00f), cegui_reldim( 0.00)));
	label->setSize(CEGUI::UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
	label->setMousePassThroughEnabled(true);
	label->setProperty("Image", "set:minimap image:minimap_img"); 
	label->setProperty("BackgroundColours", "tl:44444444 tr:44444444 bl:44444444 br:44444444"); 
	label->setInheritsAlpha (false);
	label->setAlpha(1.0);
	
	/*
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("RttMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::Technique *technique = material->createTechnique();
	technique->createPass();
	material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	material->getTechnique(0)->getPass(0)->createTextureUnitState("minimap_tex");
	
	Ogre::Overlay* overlay = Ogre::OverlayManager::getSingleton().create("minimapOverlay");
	Ogre::OverlayContainer* container = (Ogre::OverlayContainer*) Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "minimapContainer");
	overlay->add2D(container);
	container->setMaterialName("RttMat");
	container->setDimensions(0.8, 0.8);
	//textArea->setMetricsMode(GMM_PIXELS);
	container->setPosition(0.1, 0.1);
	overlay->show();
	*/
}

void MinimapWindow::update()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::FrameWindow* minimap = (CEGUI::FrameWindow*) win_mgr.getWindow("MinimapWindow");
	
	Player* player = m_document->getLocalPlayer();
	if (player ==0)
		return;
	
	Region* region = player->getRegion();
	if (region ==0)
		return;
	
	WorldObjectMap * players = region->getPlayers();
	WorldObjectMap::iterator it;
	WorldObject* pl;
	// Anzahl Fenster
	static int ncount =0;
	
	CEGUI::Window* label;
	std::ostringstream stream;
	
	int cnt =0;
	float relx, rely;
	float alpha;
	for (it = players->begin(); it != players->end(); ++it)
	{
		pl = it->second;
		stream.str("");
		stream << "PlayerMinimapImage"<<cnt;
		
		if (cnt >= ncount)
		{
			label = win_mgr.createWindow("TaharezLook/StaticImage", stream.str());
			minimap->addChildWindow(label);
			label->setProperty("FrameEnabled", "false");
			label->setProperty("BackgroundEnabled", "true");
			label->setProperty("BackgroundColours", "tl:00000000 tr:00000000 bl:00000000 br:00000000"); 
			label->setSize(CEGUI::UVector2(cegui_reldim(0.03f), cegui_reldim( 0.03f)));
			label->setMousePassThroughEnabled(true);
			label->setProperty("Image", "set:TaharezLook image:CloseButtonNormal"); 
			label->setInheritsAlpha (false);
			label->setAlwaysOnTop(true);
			
			DEBUG("creating Window!");
			
			ncount ++;
		}
		else
		{
			label = win_mgr.getWindow(stream.str());
		}
		
		alpha = 0.5;
		if (pl == player)
		{
			alpha = 1.0;
		}
		if (label->getAlpha() != alpha)
		{
			label->setAlpha(alpha);
		}
		
		if (region->getDimX()>region->getDimY())
		{
			relx =pl->getShape()->m_center.m_x/ region->getDimX()/4.0f - 0.015;
			rely =(pl->getShape()->m_center.m_y +2*(region->getDimX()-region->getDimY()))/ region->getDimX()/4.0f - 0.015;
		}
		else
		{
			relx =(pl->getShape()->m_center.m_x +2*(region->getDimY()-region->getDimX()))/ region->getDimY()/4 - 0.015;
			rely =pl->getShape()->m_center.m_y/ region->getDimY()/4 - 0.015;
		}
		
		DEBUGX("relx %f  rely %f",relx,rely);
		
		label->setPosition(CEGUI::UVector2(cegui_reldim(relx), cegui_reldim(rely)));
		label->setVisible(true);
		cnt++;
	}
	
	// restliche Label verstecken
	for (; cnt <ncount; cnt++)
	{
		stream.str("");
		stream << "PlayerMinimapImage";
		stream << cnt;
			
		label = win_mgr.getWindow(stream.str());
		label->setVisible(false);
	}
	
	label = win_mgr.getWindow("RegionNameLabel");
	
	CEGUI::utf8* ctext = (CEGUI::utf8*) dgettext("sumwars",region->getName().c_str());
	if (label->getText() != ctext)
	{
		label->setText( ctext );
	}
	
}

