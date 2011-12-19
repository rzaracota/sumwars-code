//
// C++ Implementation: ShaderManager
//
// Description:
//
//
// Author: Alexey Torkhov <atorkhov@gmail.com>, (C) 2009
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ShaderManager.h"

#include "ShadowCameraSetup.h"

#include "debug.h"

#include <list>
#include <exception>
#include <algorithm>
#include <cctype>

#include <OgreMaterialManager.h>
#include <OgreMaterial.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
/**
 * @brief A shader setup instance which envelops a scene manager and handles the shadow camera setup for that manager.
 */
class ShaderSetupInstance 
{
private:

	Ogre::SceneManager& mSceneManager;

	/**
	 * @brief Takes care of the setup of the pssm shadow camera. Owned by this instance.
	 */
	ShadowCameraSetup* mShadowCameraSetup;

	/**
	 * @brief The shader generator
	 */
	Ogre::RTShader::ShaderGenerator* mShaderGeneratorPtr;

public:

	ShaderSetupInstance (Ogre::SceneManager& sceneManager,
						Ogre::RTShader::ShaderGenerator* generatorPtr)
		: mSceneManager(sceneManager)
		, mShadowCameraSetup(0)
		, mShaderGeneratorPtr (generatorPtr)
	{

	}

	~ShaderSetupInstance()
	{
		delete mShadowCameraSetup;
	}

	void setPSSMShadows()
	{
		delete mShadowCameraSetup;
		mShadowCameraSetup = new ShadowCameraSetup(mSceneManager, mShaderGeneratorPtr);
	}

	void setNoShadows()
	{
		delete mShadowCameraSetup;
		mShadowCameraSetup = 0;
		mSceneManager.setShadowTechnique(Ogre::SHADOWTYPE_NONE);
		//This will make any other camera setup delete itself (unless held by another shared pointer).
		mSceneManager.setShadowCameraSetup(Ogre::ShadowCameraSetupPtr());
	}
};

ShaderManager::ShaderManager () 
	: mGraphicsLevel (LEVEL_DEFAULT)
	, mBestGraphicsLevel (LEVEL_DEFAULT)
{
	mGraphicSchemes[LEVEL_DEFAULT]		= std::string("Default");
	mGraphicSchemes[LEVEL_LOW]			= std::string("Low");
	mGraphicSchemes[LEVEL_MEDIUM]		= std::string("Medium");
	mGraphicSchemes[LEVEL_HIGH]			= std::string("High");
	mGraphicSchemes[LEVEL_EXPERIMENTAL]	= std::string("Experimental");

	init();
}

void ShaderManager::init()
{
	DEBUG ("Shader Manager is initializing...");
#if 0
	// We normally want to check base materials
	std::list<std::string> materialsToCheck;
	materialsToCheck.push_back("/base/simple");
	materialsToCheck.push_back("/base/normalmap");
	materialsToCheck.push_back("/base/normalmap/specular");

	bool supported;

	// Iterate schemes from best to worst
	for (std::map<GraphicsLevel, std::string>::reverse_iterator I = mGraphicSchemes.rbegin(); I != mGraphicSchemes.rend(); ++I) 
	{
		Ogre::MaterialManager::getSingleton().setActiveScheme(I->second);

		supported = true;
		for (std::list<std::string>::iterator J = materialsToCheck.begin(); J != materialsToCheck.end(); ++J) 
		{
			supported &= checkMaterial(*J, I->second);
			// Break when found first unsupported material, no need to check others
			if (!supported) 
			{
				break;
			}
		}

		// Found some supported sheme, ok
		if (supported) 
		{
			mBestGraphicsLevel = I->first;
			break;
		}
	}

	// No scheme is supported, something wrong with graphics
	if (!supported) 
	{
		LOGGER(Log::LOGLEVEL_INFO, "No schemes are supported");
		throw "No schemes is supported, something wrong with graphics";
	}

	// Don't start in experimental level
	mGraphicsLevel = (mBestGraphicsLevel < LEVEL_EXPERIMENTAL) ? mBestGraphicsLevel : LEVEL_HIGH;

	// TODO adapt to sumwars
	/*GraphicsLevel configLevel = getLevelByName(std::string(EmberServices::getSingleton().getConfigService().getValue("graphics", "level")));
	if (configLevel <= mBestGraphicsLevel) {
		mGraphicsLevel = configLevel;
	}

	setGraphicsLevel(mGraphicsLevel);*/
#endif
	// Force currently to the "high" quality settings.
	mBestGraphicsLevel = LEVEL_HIGH;
	mGraphicsLevel = LEVEL_HIGH;
	setGraphicsLevel (mGraphicsLevel);
}

bool ShaderManager::checkMaterial(const std::string& materialName, const std::string& schemeName)
{
	return true;
#if 0
	// OGRE scheme is switched in caller
	Ogre::MaterialPtr material = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().load(materialName, "General"));
	if (material->getNumSupportedTechniques() == 0) 
	{
		std::stringstream str;
		str << "The material '" << material->getName() << "' has no supported techniques with scheme " << schemeName << ". The reason for this is: \n" << material->getUnsupportedTechniquesExplanation();
		LOGGER(Log::LOGLEVEL_INFO, str.str().c_str());
		str.str("");
		return false;
	}

	std::stringstream str;
	str << "The material '" << material->getName() << "' has " << material->getNumSupportedTechniques() << " supported techniques out of " << material->getNumTechniques();
	LOGGER(Log::LOGLEVEL_INFO, str.str().c_str());
	str.str("");

	// Check that we use desired scheme, but not fallbacked to default
	if (material->getBestTechnique()->getSchemeName() != schemeName) {
		str << "The material '" << material->getName() << "' has best supported scheme " << material->getBestTechnique()->getSchemeName() << ". Was looking for " << schemeName;
		LOGGER(Log::LOGLEVEL_INFO, str.str().c_str());
		str.str("");
		return false;
	}

	str << "The material '" << material->getName() << "' supported with scheme " << schemeName;
	LOGGER(Log::LOGLEVEL_INFO, str.str().c_str());
	str.str("");
	return true;
#endif
}

ShaderManager::~ShaderManager()
{
	for (ShaderSetupStore::const_iterator I = mShaderSetups.begin(); I != mShaderSetups.end(); ++I)
	{
		delete I->second;
	}
}

ShaderManager::GraphicsLevel ShaderManager::getGraphicsLevel()
{
	return mGraphicsLevel;
}


ShaderManager::GraphicsLevel ShaderManager::getLevelByName(const std::string &level)
{
	std::string levelString = level;
	std::transform(levelString.begin(), levelString.end(), levelString.begin(), (int(*)(int)) std::tolower);

	for (std::map<GraphicsLevel, std::string>::iterator I = mGraphicSchemes.begin(); I != mGraphicSchemes.end(); ++I) {
		std::string scheme = I->second;
		std::transform(scheme.begin(), scheme.end(), scheme.begin(), (int(*)(int)) std::tolower);
		if (levelString == scheme) {
			return I->first;
		}
	}

	return LEVEL_DEFAULT;
}

void ShaderManager::registerSceneManager(Ogre::SceneManager* sceneManager)
{
	ShaderSetupInstance* instance = new ShaderSetupInstance(*sceneManager, 0);
	mShaderSetups.insert(ShaderSetupStore::value_type(sceneManager, instance));
	//setGraphicsLevel(mGraphicsLevel); 
}


void ShaderManager::registerSceneManager(Ogre::SceneManager* sceneManager, Ogre::RTShader::ShaderGenerator* shaderGeneratorPtr)
{
	ShaderSetupInstance* instance = new ShaderSetupInstance(*sceneManager, shaderGeneratorPtr);
	mShaderSetups.insert(ShaderSetupStore::value_type(sceneManager, instance));
	//setGraphicsLevel(mGraphicsLevel); 
}

void ShaderManager::deregisterSceneManager(Ogre::SceneManager* sceneManager)
{
	ShaderSetupStore::iterator I = mShaderSetups.find(sceneManager);
	if (I != mShaderSetups.end()) {
		delete I->second;
		mShaderSetups.erase(I);
	}
}

ShaderManager::GraphicsLevel ShaderManager::setGraphicsLevel(ShaderManager::GraphicsLevel newLevel)
{
	if (newLevel > mBestGraphicsLevel) 
	{
		std::stringstream str;
		str << "Cannot set graphics level " << mGraphicSchemes[newLevel];
		LOGGER(Log::LOGLEVEL_INFO, str.str().c_str());
		return mGraphicsLevel;
	}
	
	std::stringstream str;
	str << "Using graphics level " << mGraphicSchemes[newLevel];
	LOGGER(Log::LOGLEVEL_INFO, str.str().c_str());
	
	Ogre::MaterialManager::getSingleton().setActiveScheme(mGraphicSchemes[newLevel]);
		
	Ogre::RenderWindow* window = Ogre::Root::getSingleton().getAutoCreatedWindow();
	for (int i = 0; i < window->getNumViewports(); ++i) 
		window->getViewport(i)->setMaterialScheme("High");
		//window->getViewport(i)->setMaterialScheme(mGraphicSchemes[newLevel]);


	switch (newLevel) 
	{
	case LEVEL_EXPERIMENTAL:
	case LEVEL_HIGH:
		setPSSMShadows();
		break;

	case LEVEL_MEDIUM:
	case LEVEL_LOW:
	case LEVEL_DEFAULT:
		setNoShadows();
		break;
	}
	mGraphicsLevel = newLevel;

	return mGraphicsLevel;
}

void ShaderManager::setPSSMShadows()
{
	for (ShaderSetupStore::const_iterator I = mShaderSetups.begin(); I != mShaderSetups.end(); ++I)
	{
		I->second->setPSSMShadows();
	}
}

void ShaderManager::setNoShadows()
{
	for (ShaderSetupStore::const_iterator I = mShaderSetups.begin(); I != mShaderSetups.end(); ++I)
	{
		I->second->setNoShadows();
	}
}

