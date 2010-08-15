#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <list>
#include <map>

#include "CEGUI.h"
#include "OgreSingleton.h"

class Tooltip
{
public:
	enum TooltipType
	{
		Main = 1,
		Comparision
	};
	
	Tooltip(CEGUI::Window *parent, std::string name, float fadeInTime, float fadeOutTime, float visibilityTime, float targetAlpha = 0, TooltipType type = Main);

	void create(std::string msg, CEGUI::UVector2 position, CEGUI::UVector2 size, CEGUI::Font *font);
	void fadeOut();

	void setChild(Tooltip *child) { m_Child = child; }
	Tooltip *getChild() { return m_Child; }
	
	std::string getName() { return m_Name; }
	TooltipType getType() { return m_Type; }

	CEGUI::Window *getCEGUIWindow() { return m_CEGUIWindow; }
	
	void update(float timeSinceLastUpdate);
	bool isDead() { return m_IsDead; }
private:
	std::string name;

	float m_CurrentFadeInTime;
	float m_FadeInTime;
	float m_FadeInStepPerMS;
	bool m_FadingIn;

	float m_CurrentVisibleTime;
	float m_VisibilityTime;
	bool m_Visible;
	
	float m_CurrentFadeOutTime;
	float m_FadeOutTime;
	float m_FadeOutStepPerMS;
	bool m_FadingOut;
	
	bool m_IsDead;
	CEGUI::FrameWindow *m_CEGUIWindow;
	CEGUI::Window* m_Parent;
	std::string m_Name;

	Tooltip *m_Child;
	
	float m_Alpha;
    TooltipType m_Type;
};

class TooltipManager : public Ogre::Singleton<TooltipManager>
{
public:
	TooltipManager ( );
	void createTooltip ( std::list<std::string> list, float timeVisible = 0.0f, CEGUI::UVector2 position = CEGUI::UVector2(), CEGUI::Font *font = 0, Tooltip::TooltipType type = Tooltip::Main);
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

