#include "tooltipmanager.h"
#include "ceguiutility.h"

template<> TooltipManager* Ogre::Singleton<TooltipManager>::ms_Singleton = 0;

Tooltip::Tooltip ( CEGUI::Window* parent, std::string name, float fadeInTime, float fadeOutTime, float visibilityTime, float targetAlpha )
{
	m_FadeInTime = fadeInTime;
	m_FadeOutTime = fadeOutTime;
	m_VisibilityTime = visibilityTime;
	m_Parent = parent;
	m_Name = name;
	m_Visible = m_FadingOut = false;
	m_FadingIn = true;

	if(targetAlpha == 0)
		m_Alpha = parent->getAlpha();
	else
		m_Alpha = targetAlpha;
}


void Tooltip::update ( float timeSinceLastUpdate )
{
	if(m_FadingIn)
	{
		m_CurrentFadeInTime += timeSinceLastUpdate;

		if(m_CurrentFadeInTime > m_FadeInTime)
		{
			m_FadingIn = false;
			m_Visible = true;
		}

	}
	else if(m_Visible)
	{
		m_CurrentVisibleTime += timeSinceLastUpdate;
		
		if(m_CurrentVisibleTime > m_VisibilityTime)
		{
			m_Visible = false;
			m_FadingOut = true;
		}
	}
	else
	{
		m_CurrentFadeOutTime += timeSinceLastUpdate;

		if(m_FadeOutTime > m_CurrentFadeOutTime)
		{
			m_Parent->removeChildWindow(m_Name);
			CEGUI::WindowManager::getSingleton().destroyWindow(m_Name);
		}
	}
}

TooltipManager::TooltipManager ()
{
	m_Parent = 0;
    m_Position = CEGUI::UVector2();
    m_CEGUIWinMgr = CEGUI::WindowManager::getSingletonPtr();
    m_CEGUISystem = CEGUI::System::getSingletonPtr();
	m_DefaultFont = m_CEGUISystem->getDefaultFont();
}

void TooltipManager::setParent(CEGUI::Window* parent)
{
	m_Parent = parent;
}


void TooltipManager::createTooltip ( std::string windowName, std::list<std::string> list, float timeVisible, CEGUI::UVector2 position, CEGUI::Font *font)
{
    CEGUI::FrameWindow* tt = static_cast<CEGUI::FrameWindow*>(m_CEGUIWinMgr->createWindow( (CEGUI::utf8*)"SumwarsTooltip", (CEGUI::utf8*)windowName.c_str() ));
	std::string msg;
    CEGUI::UVector2 size;
	
	if(font)
		size = CEGUIUtility::getWindowSizeForText(list, font, msg);
	else
		size = CEGUIUtility::getWindowSizeForText(list, m_DefaultFont, msg);
	
    tt->setText(msg);

    if (position == CEGUI::UVector2())
        tt->setPosition( m_Position );
    else
        tt->setPosition( position );

    tt->setSize( size );
    tt->setAlpha(0.9f);
	assert(m_Parent);
    m_Parent->addChildWindow(tt);

	m_Tooltips[windowName] = tt;
}

void TooltipManager::setPosition(const CEGUI::UVector2& position)
{
    m_Position = position;
}

CEGUI::UVector2 TooltipManager::getPosition()
{
    return m_Position;
}


void TooltipManager::setVisibleTime ( float timeVisible )
{
    m_timeVisible = timeVisible;
}

float TooltipManager::getVisibleTime()
{
    return m_timeVisible;
}

void TooltipManager::setFadeInTime ( float timeToFadeIn )
{
    m_fadeInTime = timeToFadeIn;
}

float TooltipManager::getFadeInTime()
{
    return m_fadeInTime;
}

void TooltipManager::setFadeOutTime ( float timeToFadeOut )
{
    m_fadeOutTime = timeToFadeOut;
}

float TooltipManager::getFadeOutTime()
{
    return m_fadeOutTime;
}

void TooltipManager::update(float timeSinceLastUpdate)
{

}

TooltipManager* TooltipManager::getSingletonPtr(void)
{
    return ms_Singleton;
}

TooltipManager& TooltipManager::getSingleton(void)
{
    assert( ms_Singleton );
    return ( *ms_Singleton );
}
