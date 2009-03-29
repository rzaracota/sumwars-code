#ifndef TOOLTIP_H
#define TOOLTIP_H

#include "CEGUI.h"

class TextWrapTooltip : public CEGUI::Tooltip
{
	public:
		TextWrapTooltip(const CEGUI::String &type, const CEGUI::String &name);
		
		virtual CEGUI::Size getTextSize_impl() const;
};

class TextWrapTooltipFactory : public CEGUI::WindowFactory
{
	public:
		TextWrapTooltipFactory();
		
		virtual CEGUI::Window * createWindow (const CEGUI::String &name)
		{
			return new TextWrapTooltip(d_type,name);
		}
		
		virtual void  destroyWindow (CEGUI::Window *window)
		{
			delete window;
		}
};

#endif

