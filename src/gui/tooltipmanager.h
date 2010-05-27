#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <list>
#include <map>

#include "CEGUI.h"
#include "OgreSingleton.h"

class Tooltip
{
public:
	Tooltip(CEGUI::Window *parent, std::string name, float fadeInTime, float fadeOutTime, float visibilityTime, float targetAlpha = 0);

	void create(std::string msg, CEGUI::UVector2 position, CEGUI::UVector2 size, CEGUI::Font *font);
	void fadeOut();

	std::string getName() { return m_Name; }
	
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

	float m_Alpha;
};

class TooltipManager : public Ogre::Singleton<TooltipManager>
{
public:
	TooltipManager ( );
	void createTooltip ( std::list<std::string> list, float timeVisible = 0.0f, CEGUI::UVector2 position = CEGUI::UVector2(), CEGUI::Font *font = 0);

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
	std::map<std::string, Tooltip*> m_Tooltips;
	float m_timeVisible;
	float m_fadeInTime;
	float m_fadeOutTime;
	int m_toolTipsCreatedCount;
    CEGUI::Font* m_DefaultFont;
};

#endif

