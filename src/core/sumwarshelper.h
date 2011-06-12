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
		path.append("/.sumwars");
#endif
		return path;
	}
};
#endif // SUMWARSHELPER_H
