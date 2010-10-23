#ifndef TOOLTIP_H
#define TOOLTIP_H

#include "CEGUI/CEGUI.h"

class Tooltip
{
public:
	/**
	 * \enum Tooltiptype
	 * \brief Tooltip types
	 */
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
#endif

