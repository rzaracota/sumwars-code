#ifndef TOOLTIPMANAGER_H
#define TOOLTIPMANAGER_H

#include <list>
#include <map>

#include "CEGUI/CEGUI.h"
#include "OgreSingleton.h"
#include "tooltip.h"

class TooltipManager : public Ogre::Singleton<TooltipManager>
{
public:
	TooltipManager ( );
	void createTooltip ( std::list<std::string> list, float timeVisible = 0.0f, CEGUI::Font *font = 0, Tooltip::TooltipType type = Tooltip::Main);
	void fadeAllOut();
	
	void setParent(CEGUI::Window *parent);
	
	void setPosition(const CEGUI::UVector2 &position);
	CEGUI::UVector2 getPosition();
	
	void setVisibleTime ( float timeVisible );
	float getVisibleTime();

	void setFadeInTime ( float timeToFadeIn );
	float getFadeInTime();

	void setFadeOutTime ( float timeToFadeOut );
	float getFadeOutTime();

	void update(float timeSinceLastUpdate);

	static TooltipManager& getSingleton(void);
	static TooltipManager* getSingletonPtr(void);
	
private:
	CEGUI::WindowManager *m_CEGUIWinMgr;
	CEGUI::System *m_CEGUISystem;
	CEGUI::Window* m_Parent;
	CEGUI::UVector2 m_Position;
	Tooltip *m_CurrentMain;
	std::map<std::string, Tooltip*> m_Tooltips;
	float m_timeVisible;
	float m_fadeInTime;
	float m_fadeOutTime;
	int m_toolTipsCreatedCount;
    CEGUI::Font* m_DefaultFont;
	CEGUI::String m_ToolTipWindowName;
};

#endif

