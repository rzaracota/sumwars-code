#include "ceguiutility.h"
#include "CEGUI.h"

std::string CEGUIUtility::stripColors(const std::string &input)
{
	std::string output = input;
	
	std::string::size_type pos = output.find("[");
	while(pos != std::string::npos)
	{
		output.erase(pos,pos+19);
		pos = output.find("[");
	}
	
	return output;
}

CEGUI::UVector2 CEGUIUtility::getWindowSizeForText(std::list<std::string> list, CEGUI::Font *font, std::string &added)
{
	using namespace CEGUI;
	
	float textWidth = 0.0f;
	float textHeight = 0.0f;
	Size screenSize = CEGUI::System::getSingleton().getRenderer()->getDisplaySize();
	
	added = "";
	
	std::list<std::string>::iterator iter = list.begin();
	while (iter != list.end())
	{
		std::string s = *iter;
		added += s;
		float tempwidth = font->getTextExtent(stripColors(s).c_str()) ;
		if (tempwidth > textWidth)
			textWidth = tempwidth;
		
		iter++;
	}
	
	textHeight = list.size() * font->getFontHeight();
	
	return UVector2(UDim((textWidth/screenSize.d_width) + 0.05f, 0), UDim(textHeight / screenSize.d_height, 0) );
}