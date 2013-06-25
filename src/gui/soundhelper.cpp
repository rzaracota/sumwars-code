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

//
// Includes
//

// Own header
#include "soundhelper.h"

// Debugging helper
#include "debug.h"

// Resource management and control for Ogre.
#include "OgreArchive.h"
#include "OgreResourceGroupManager.h"

// Sound library
#include "gussound.h"


// Allow a shorter name for the sound manager.
using gussound::SoundManager;



///
/// Load a file (E.g. an XML file) containing sound group definitions.
///
bool SoundHelper::loadSoundGroupsFromFile (const std::string& fileName)
{
	// TODO: get extension from file. Create parsing implementation based on extension (E.g. XML).
	// Current assumption: the file is an XML file.

	// Load XML document.
	TiXmlDocument doc (fileName.c_str ());
	bool loadOkay = doc.LoadFile();

	if (! loadOkay)
	{
		// Failed to load.
		ERRORMSG ("Failed to load file %s", fileName.c_str ());
		return false;
	}

	loadSoundInfos (&doc);


	return true;
}


///
/// Load content from an XML Node.
///
void SoundHelper::loadSoundInfos (TiXmlNode* node)
{
	// File was loaded... time to parse the contents.
	TiXmlNode* child;
	if (node->Type () == TiXmlNode::TINYXML_ELEMENT && !strcmp (node->Value (), "Sound"))
	{
		ElementAttrib attr;
		attr.parseElement (node->ToElement ());
		
		std::string groupName;
		float probability;

		attr.getString ("name", groupName);
		attr.getFloat ("probability", probability, 1.0);

		// Add the group to the repository.
		SoundManager::getPtr ()->addSoundGroup (groupName);

		for (child = node->FirstChild (); child != 0; child = child->NextSibling ())
		{
			if (child->Type () == TiXmlNode::TINYXML_ELEMENT)
			{
				attr.parseElement (child->ToElement ());
				if (! strcmp (child->Value (), "Soundfile"))
				{
					std::string fileName;
					int weight;

					attr.getString ("source", fileName);
					attr.getInt ("weight", weight, 1.0);
                    
					DEBUGX ("loading sound file %s for sound %s", fileName.c_str (), groupName.c_str ());

					// Add/load the sound file
					try
					{
						std::string fullPath = getNameWithPathForSoundFile (fileName);
						SoundManager::getPtr ()->getRepository ()->addSound (fileName.c_str ()  // name
																		, fullPath.c_str ()  // file & path
																		, true // load entire sound into memory? don't do it for songs! only for small sounds.
																		, gussound::GSC_Effect  // a category for the sound; you can change the volume for all sounds in a category.
																		, false); // want to make sure this sound is only played from one source at one time?
					}
					catch (std::exception&)
					{
						// An exception is thrown if the sound already exists.
						// That's ok for us, as we're using the file name as the sound name, so if it exists, it means it's already in the system.
					}

					// Assign the sound file to the group.
					SoundManager::getPtr ()->getSoundGroup (groupName)->addSound (fileName.c_str (), weight);
				}
			}
		}
	}
	else
	{
		// Run through the entire tree
		for ( child = node->FirstChild(); child != 0; child = child->NextSibling())
		{
			loadSoundInfos(child);
		}
	}
}



///
/// Return the full path to the music file (passing through Ogre resource management), from the received file name.
///
std::string SoundHelper::getNameWithPathForMusicTrack (const std::string& trackName)
{
	std::string result (trackName);

	Ogre::FileInfoListPtr files;
	Ogre::FileInfoList::iterator it;
	files = Ogre::ResourceGroupManager::getSingleton ().findResourceFileInfo ("music", trackName);

	it = files->begin ();
	if (it != files->end ())
	{
		std::string filename;
		result = it->archive->getName ();
		result += "/";
		result += it->filename;
	}

	return result;
}



///
/// Add a track to a playlist by using only the file name (without the path).
///
void SoundHelper::addPlaylistTrackByShortName (const std::string& playlistName, const std::string& shortFileName)
{
	std::string fileName;
	try
	{
		fileName = SoundHelper::getNameWithPathForMusicTrack (shortFileName);
		// One could call the following function to add an "anonymous" track (with a name such as "track_01").
		SoundManager::getPtr ()->addPlaylistTrack (playlistName, fileName);
	}
	catch (std::exception& e)
	{
		DEBUG ("Caught exception while trying to add to playlist [%s] track file [%s]: %s", playlistName.c_str (), fileName.c_str (), e.what ());
	}
}




///
/// Return the full path to the sound file (passing through Ogre resource management), from the received file name.
///
std::string SoundHelper::getNameWithPathForSoundFile (const std::string& soundFile)
{
	std::string result (soundFile);

	Ogre::FileInfoListPtr files;
	Ogre::FileInfoList::iterator it;
	files = Ogre::ResourceGroupManager::getSingleton ().findResourceFileInfo ("sound", soundFile);

	it = files->begin ();
	if (it != files->end ())
	{
		std::string filename;
		result = it->archive->getName ();
		result += "/";
		result += it->filename;
	}

	return result;
}



///
/// Play a sound for the current player, at his location. (Basically a 2D sound).
///
void SoundHelper::playAmbientSoundGroup (const std::string& soundGroupID)
{
	std::string soundToPlay;
	soundToPlay = SoundManager::getPtr ()->getSoundGroup (soundGroupID)->getRandomSound ();
	SoundManager::getPtr ()->getRepository ()->getSound (soundToPlay)->play2D ();
}



///
/// Play a sound group at a given location.
///
void SoundHelper::playSoundGroupAtPosition (const std::string& soundGroupID, double posX, double posY, double posZ)
{
	try
	{
		DEBUG ("Will try to play sound group [%s]", soundGroupID.c_str ());

		std::string soundToPlay;
		DEBUG ("Checking if valid");
		soundToPlay = SoundManager::getPtr ()->getSoundGroup (soundGroupID)->getRandomSound ();
		DEBUG ("Chosen sound file from group [%s]",  soundToPlay.c_str ());

		SoundManager::getPtr ()->getRepository ()->getSound (soundToPlay)->play3D (posX, posY, posZ);
	}
	catch (std::exception& e)
	{
		DEBUG ("Caught exception while trying to play sound group [%s]: %s", soundGroupID.c_str (), e.what ());
	}
}