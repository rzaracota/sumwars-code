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

#ifndef SUMWARSHELPER_H
#define SUMWARSHELPER_H

#include "config.h"

#include <physfs.h>
#include <OgreString.h>

// The standard string
#include <string>
// The standard container for stl.
#include <vector>
// The used singleton manager.
#include "OgreSingleton.h"

/**
 * \brief Helper class. Used as a namespace to store utility static functions.
 * Functions provided here relate to parsing of the resolution string found in
 * various OGRE drivers and the OS specific path parsing to avoid adding #if/#ifdef clauses
 * all over the code.
 */
class SumwarsHelper
		: public Ogre::Singleton <SumwarsHelper>
{
protected:
	// A list of supported features.
	std::vector <std::string> supportedFeatures_;

public:
	/**
	 * Constructor; will call the init function.
	 */
	SumwarsHelper ();

	/**
	 * \brief Initialize the helper; this will also reload the list of supported features.
	 */
	void init ();


	/**
	 * \brief Search in the list of stored features for a specific one.
	 * \param featureName The name of the feature to look for.
	 */
	bool hasFeature (const std::string& featureName) const;


	/**
	 * \brief Retrieve the preload file name. Should not be used if the preload feature is not available.
	 */
	std::string getPreloadFileName () const;

	/**
	 * \brief Get the path to use for storage.
	 */
	const std::string& getStorageBasePath () const;


	/**
	 * \fn static Ogre::String gameDataPath()
	 *
	 * \brief Returns the path where the game data is stored (e.g.,
	 * /usr/share/sumwars in Unix)
	 */
	static Ogre::String gameDataPath()
	{
		Ogre::String path = userPath();

#if defined (__unix__)
		// redefine if necessary
		path = SUMWARS_PREFIX "/" SUMWARS_SHARE_DIR;
#elif defined (__APPLE__)
		// redefine if necessary
#elif defined (_WIN32)
		// redefine if necessary
#endif

		return path;
	}

	/**
	 * \fn static Ogre::String gameDocPath()
	 *
	 * \brief Returns the path where the game documentation is stored (e.g.,
	 * /usr/share/doc/sumwars in Unix)
	 */
	static Ogre::String gameDocPath()
	{
		Ogre::String path = userPath();

#if defined (__unix__)
		// redefine if necessary
		path = SUMWARS_PREFIX "/" SUMWARS_DOC_DIR;
#elif defined (__APPLE__)
		// redefine if necessary
#elif defined (_WIN32)
		// redefine if necessary
#endif

		return path;
	}


	/**
	 * \fn static Ogre::String userPath()
	 * \brief Returns the writable sumwars directory in the users directory
	 */
	static Ogre::String userPath();

	/**
	 * \fn static Ogre::String savePath()
	 * \brief Returns the directory with sumwars save files
	 */
	static Ogre::String savePath();


	/**
	 * \brief Gets the updated resolution string from a base resolution string.
	 * Purpose: manage differences between Direct3D and OpenGL resolution strings.
	 *
	 * \param initialString The source resolution string.
	 * \param newWidth The new width to use.
	 * \param newHeight The new height to use.
	 * \return The full updated name of the entry for the resolution.
	 * \author Augustin Preda (if anything doesn't work, he's the one to bash).
	 */
	static std::string getUpdatedResolutionString (const std::string& initialString, int newWidth, int newHeight);

	/**
	 * \brief Gets the native resolution on the desktop.
	 * Purpose: Setting resolution on initial startup
	 *
	 * \return Full native desktop resolution
	 * \author Stefan Stammberger (if anything doesn't work, he's the one to bash).
	 */
	static std::string getNativeResolutionString ();

	/**
	 * \brief Parse a basic resolution string and get the width and height.
	 * Purpose: manage differences between Direct3D and OpenGL resolution strings.
	 *
	 * \param initialString The source resolution string.
	 * \param[out] newWidth The width to read.
	 * \param[out] newHeight The height to read.
	 * \return Nothing.
	 * \author Augustin Preda (if anything doesn't work, he's the one to bash).
	 */
	static void getSizesFromResolutionString (const std::string& initialString, int& videoModeWidth, int& videoModeHeight);

#if defined (__APPLE__)
	// TODO: copied function here from application.h; Investigate integration with userPath function.
	// TODO: investigate removal of original function from application.h

	/**
	 * \fn Ogre::String macPath()
	 * \brief Returns the path to the Resources directory on mac.
	 */
	Ogre::String macPath();
#endif

};
#endif // SUMWARSHELPER_H
