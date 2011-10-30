/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "sumwarshelper.h"


std::string SumwarsHelper::getUpdatedResolutionString (const std::string& initialString, int newWidth, int newHeight)
{
	std::string returnValue;
	returnValue = initialString;

	std::string sLeft, sRight;
	int nPos = returnValue.find (" ");
	if (nPos == std::string::npos)
	{
		// some error...
		return "";
	}

	sLeft = returnValue.substr (0, nPos);
	sRight= returnValue.substr (nPos + 3); // + 3 chars for " x "
	nPos = sRight.find (" ");
	std::string sAux;

	if (nPos == std::string::npos)
	{
		// we don't have a colour depth.
		sAux = sRight;
		sRight = "";
	}
	else
	{
		sAux = sRight.substr(0, nPos);
		sRight = sRight.substr(nPos);
	}

	// Use a string stream to parse the needed string.
	std::stringstream ss;
	ss << newWidth << " x " << newHeight << sRight;
	returnValue = ss.str ();

	return returnValue;
}


void SumwarsHelper::getSizesFromResolutionString (const std::string& initialString, int& videoModeWidth, int& videoModeHeight)
{
	std::string tempString;
	tempString = initialString;

	std::string sLeft, sRight;
	int nPos = tempString.find (" ");
	if (nPos == std::string::npos)
	{
		// some error...
		return;
	}

	sLeft = tempString.substr (0, nPos);
	sRight= tempString.substr (nPos + 3); // + 3 chars for " x "
	nPos = sRight.find (" ");
	std::string sAux;

	if (nPos == std::string::npos)
	{
		// we don't have a colour depth.
		sAux = sRight;
		sRight = "";
	}
	else
	{
		sAux = sRight.substr(0, nPos);
		sRight = sRight.substr(nPos);
	}

	videoModeWidth = atoi (sLeft.c_str ());
	videoModeHeight = atoi (sAux.c_str ());
}
