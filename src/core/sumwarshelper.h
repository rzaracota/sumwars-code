/*
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef SUMWARSHELPER_H
#define SUMWARSHELPER_H

#include "physfs.h"
#include "OgreString.h"
// Return the path to where files should be stored on mac

class SumwarsHelper
{
	/**
	 * \fn static Ogre::String userPath()
	 * \brief Returns the writable sumwars directory in the users directory
	 */
public:
	static Ogre::String userPath()
	{
		Ogre::String path = PHYSFS_getUserDir();
#ifdef __APPLE__
		path.append("/Library/Application Support/Sumwars");
#else
		path.append(".sumwars");
#endif
		return path;
	}
};
#endif // SUMWARSHELPER_H
