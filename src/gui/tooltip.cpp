#include "tooltip.h"
#include <iostream>

Tooltip::Tooltip ( CEGUI::Window* parent, std::string name, float fadeInTime, float fadeOutTime, float visibilityTime, float targetAlpha, TooltipType type)
{
    m_FadeInTime = fadeInTime;
    m_FadeOutTime = fadeOutTime;
    m_VisibilityTime = visibilityTime;
    m_Parent = parent;
    m_Name = name;
    m_FadingIn = true;
    m_FadingOut = false;
    m_Visible = false;
    m_Type = type;
    m_Child = NULL;

    m_CurrentFadeInTime = 0;
    m_CurrentVisibleTime = 0;
    m_CurrentFadeOutTime = 0;
    m_IsDead = false;

    if (targetAlpha == 0)
        m_Alpha = parent->getAlpha();
    else
        m_Alpha = targetAlpha;

    if (m_FadeInTime > 0.0f)
        m_FadeInStepPerMS = m_Alpha / m_FadeInTime;
    else
        m_FadeInStepPerMS = 0.0f;

    if (m_FadeOutTime > 0.0f)
        m_FadeOutStepPerMS = m_Alpha / m_FadeOutTime;
    else
        m_FadeOutStepPerMS = 0.0f;

}

void Tooltip::create(std::string msg, CEGUI::UVector2 position, CEGUI::UVector2 size, CEGUI::Font *font)
{
    using namespace CEGUI;
    FrameWindow* tt = static_cast<FrameWindow*>(WindowManager::getSingletonPtr()->createWindow( (CEGUI::utf8*)"SumwarsTooltip", (CEGUI::utf8*)m_Name.c_str() ));
    m_CEGUIWindow = tt;

    tt->setText((CEGUI::utf8*)msg.c_str());

    tt->setPosition( position );

    tt->setSize( size );
    tt->setAlpha(0.0f);
    assert(m_Parent);
    m_Parent->addChildWindow(tt);
}


void Tooltip::fadeOut()
{	
    m_Visible = false;
	m_FadingIn = false;
	m_FadingOut = true;
}


void Tooltip::update ( float timeSinceLastUpdate )
{
    if (m_FadingIn)
    {
        m_CurrentFadeInTime += timeSinceLastUpdate;

        float steps = timeSinceLastUpdate * m_FadeInStepPerMS;
        float newAlpha = m_CEGUIWindow->getAlpha() + steps;
        m_CEGUIWindow->setAlpha(newAlpha);

        if (m_CurrentFadeInTime > m_FadeInTime)
        {
            m_FadingIn = false;
            m_Visible = true;
        }

    }
    else if (m_Visible && m_VisibilityTime != 0)
    {
        m_CurrentVisibleTime += timeSinceLastUpdate;

        if (m_CurrentVisibleTime > m_VisibilityTime)
        {
            m_Visible = false;
            m_FadingOut = true;
        }
    }
    else if (m_FadingOut)
    {
        m_CurrentFadeOutTime += timeSinceLastUpdate;
        

        float steps = timeSinceLastUpdate * m_FadeOutStepPerMS;
        float newAlpha = m_CEGUIWindow->getAlpha() - steps;
		m_CEGUIWindow->setAlpha(newAlpha);
        if (m_FadeOutTime < m_CurrentFadeOutTime)
        {
			m_IsDead = true;
            m_Parent->removeChildWindow(m_Name);
            CEGUI::WindowManager::getSingleton().destroyWindow(m_Name);
        }
    }
}
