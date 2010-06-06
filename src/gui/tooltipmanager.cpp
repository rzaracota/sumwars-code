#include "tooltipmanager.h"
#include "ceguiutility.h"

template<> TooltipManager* Ogre::Singleton<TooltipManager>::ms_Singleton = 0;

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

    tt->setText(msg);

    tt->setPosition( position );

    tt->setSize( size );
    tt->setAlpha(0.0f);
    assert(m_Parent);
    m_Parent->addChildWindow(tt);
}


void Tooltip::fadeOut()
{
    m_Visible = false;
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
    else if (m_Visible)
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
        m_IsDead = true;

        float steps = timeSinceLastUpdate * m_FadeOutStepPerMS;
        float newAlpha = m_CEGUIWindow->getAlpha() - steps;
        m_CEGUIWindow->setAlpha(newAlpha);

        if (m_FadeOutTime < m_CurrentFadeOutTime)
        {
            m_Parent->removeChildWindow(m_Name);
            CEGUI::WindowManager::getSingleton().destroyWindow(m_Name);
        }
    }
}


TooltipManager::TooltipManager ()
{
    m_toolTipsCreatedCount = 0;
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


void TooltipManager::createTooltip ( std::list<std::string> list, float timeVisible, CEGUI::UVector2 position, CEGUI::Font *font, Tooltip::TooltipType type)
{

    std::string msg;
    CEGUI::UVector2 size;
    std::ostringstream windowName;
    windowName << "Tooltip__" << m_toolTipsCreatedCount;
    CEGUI::Font *tempFont = m_DefaultFont;

    if (font)
    {
        size = CEGUIUtility::getWindowSizeForText(list, font, msg);
        tempFont = font;
    }
    else
        size = CEGUIUtility::getWindowSizeForText(list, m_DefaultFont, msg);

    if (type == Tooltip::Main)
    {
        Tooltip *tt = new Tooltip(m_Parent, windowName.str(), m_fadeInTime, m_fadeOutTime, m_timeVisible, 0.9f);
        fadeAllOut();
        m_CurrentMain = tt;
        tt->create(msg, position, size, tempFont);
        m_Tooltips[windowName.str()] = tt;
        m_toolTipsCreatedCount++;
    }
    else
    {
        Tooltip *temp = m_CurrentMain->getChild();
        if (temp)
		{
            while (temp)
            {
                if (!temp->getChild())
                {
                    Tooltip *tt = new Tooltip(m_Parent, windowName.str(), m_fadeInTime, m_fadeOutTime, m_timeVisible, 0.9f);
                    CEGUI::UVector2 pos = temp->getCEGUIWindow()->getPosition();
                    pos.d_x += m_CurrentMain->getCEGUIWindow()->getWidth();
                    tt->create(msg, pos, size, tempFont);
                    temp->setChild(tt);
                    m_Tooltips[windowName.str()] = tt;
                    m_toolTipsCreatedCount++;
					break;
                }
                temp = temp->getChild();
            }
		}
        else
        {
            Tooltip *tt = new Tooltip(m_Parent, windowName.str(), m_fadeInTime, m_fadeOutTime, m_timeVisible, 0.9f);
            CEGUI::UVector2 pos = m_CurrentMain->getCEGUIWindow()->getPosition();
            pos.d_x += m_CurrentMain->getCEGUIWindow()->getWidth();
            tt->create(msg, pos, size, tempFont);
            m_CurrentMain->setChild(tt);
            m_Tooltips[windowName.str()] = tt;
            m_toolTipsCreatedCount++;

        }
    }
}


void TooltipManager::fadeAllOut()
{
    std::map<std::string, Tooltip*>::iterator iter;
    for (iter = m_Tooltips.begin(); iter != m_Tooltips.end(); iter++)
    {
        Tooltip *tt = iter->second;

        if (!tt->isDead())
            tt->fadeOut();
    }
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
    std::map<std::string, Tooltip*>::iterator iter;
    for (iter = m_Tooltips.begin(); iter != m_Tooltips.end(); iter++)
    {
        Tooltip *tt = iter->second;

        if (tt->isDead())
        {
            CEGUI::WindowManager::getSingleton().destroyWindow(tt->getName());
            m_Tooltips.erase(iter);
            delete tt;
        }
        else
            tt->update(timeSinceLastUpdate);
    }
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
