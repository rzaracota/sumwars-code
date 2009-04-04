#include "tooltip.h"

TextWrapTooltip::TextWrapTooltip(const CEGUI::String &type, const CEGUI::String &name)
	: CEGUI::Tooltip(type,name)
{
}

CEGUI::Size TextWrapTooltip::getTextSize_impl() const
{
	CEGUI::Font* fnt = getFont();

	if (fnt)
	{
		CEGUI::Rect area(CEGUI::System::getSingleton().getRenderer()->getRect());

		// get required size of the tool tip according to the text extents.
		float width = PixelAligned(fnt->getFormattedTextExtent(d_text, area, CEGUI::LeftAligned));
		
		CEGUI::Vector2 maxsize = getMaxSize().asAbsolute(area.getSize());
		
		// check that size does not exceed max size
		if (width > maxsize.d_x)
		{
			width = maxsize.d_x;
		}
		
		area.setWidth(width-6);
		float height = PixelAligned(fnt->getFormattedLineCount(d_text, area, CEGUI::WordWrapCentred) * fnt->getLineSpacing());
		
		return CEGUI::Size(width+6, height);
	}
	else
	{
		return CEGUI::Size(0,0);
	}
	return CEGUI::Tooltip::getTextSize_impl();
}

TextWrapTooltipFactory::TextWrapTooltipFactory()
	: CEGUI::WindowFactory::WindowFactory("TextWrapTooltip")
{
	
}

