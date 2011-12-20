//
// C++ Implementation: ShadowCameraSetup
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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

#include "ShadowCameraSetup.h"

#include "debug.h"

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreShadowCameraSetupPSSM.h>


ShadowCameraSetup::ShadowCameraSetup (Ogre::SceneManager& sceneMgr, Ogre::RTShader::ShaderGenerator* rtssPtr)
	: mSceneMgr (sceneMgr)
	, m_shadow_camera_setup_type (4)
	, mShaderGenerator (rtssPtr)
{
	DEBUG ("Shadow camera setup ctor - entered");
	// Set m_shadow_camera_setup_type to:
	// 0 = Default
	// 1 = LiSPSMShadowCameraSetup
	// 2 = FocusedShadowCameraSetup
	// 3 = PlaneOptimalShadowCameraSetup
	// 4 = PSSM
	try
	{
		setup ();
	}
	catch (Ogre::Exception & e)
	{
		ERRORMSG ("Exception caught while setting up the camera shadow: %s", e.what ());
	}

	//Config_ShadowTextureSize(1024);
	if (m_shadow_camera_setup_type == 4)
	{
		DEBUG ("Applying custom PSSM settings...");
		Config_ShadowSplitPadding (2.0f);
		Config_ShadowOptimalAdjustFactors (1.0f, 1.0f, 1.0f);

		configAndCalculateSplitPoints (3, 1.0f, 120);
		//Config_ShadowSplitPoints(1, 15, 50, 180);

		//Config_ShadowUseAggressiveFocusRegion(true);

		// Make the RTTS aware of the PSSM setup.
		if (mPssmSetup && mShaderGenerator)
		{
			// Grab the scheme render state.												
			Ogre::RTShader::RenderState* schemRenderState = mShaderGenerator->getRenderState (Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
			// Assume a lighting model of SSLM_PerVertexLighting
			Ogre::RTShader::SubRenderState* perPerVertexLightModel = mShaderGenerator->createSubRenderState (Ogre::RTShader::FFPLighting::Type);
			schemRenderState->addTemplateSubRenderState (perPerVertexLightModel);	

			Ogre::RTShader::SubRenderState* subRenderState = mShaderGenerator->createSubRenderState (Ogre::RTShader::IntegratedPSSM3::Type);	
			Ogre::RTShader::IntegratedPSSM3* pssm3SubRenderState = static_cast<Ogre::RTShader::IntegratedPSSM3*>(subRenderState);

			const Ogre::PSSMShadowCameraSetup::SplitPointList& srcSplitPoints = mPssmSetup->getSplitPoints ();
			Ogre::RTShader::IntegratedPSSM3::SplitPointList dstSplitPoints;

			for (unsigned int i = 0; i < srcSplitPoints.size (); ++ i)
			{
				dstSplitPoints.push_back (srcSplitPoints[i]);
			}

			pssm3SubRenderState->setSplitPoints (dstSplitPoints);
			schemRenderState->addTemplateSubRenderState (subRenderState);
		}
	}

	Config_ShadowFarDistance (70.0f);
	Config_ShadowRenderBackfaces (true);
	DEBUG ("Shadow Camera Setup ctor - exit");
}


ShadowCameraSetup::~ShadowCameraSetup()
{
	if (m_shadow_camera_setup_type == 4)
	{
		// Make the RTTS aware of the PSSM setup.
		if (mPssmSetup && mShaderGenerator)
		{
			// Grab the scheme render state.												
			Ogre::RTShader::RenderState* schemRenderState = mShaderGenerator->getRenderState (Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

			Ogre::RTShader::SubRenderStateList myList = schemRenderState->getTemplateSubRenderStateList();
			for (Ogre::RTShader::SubRenderStateList::iterator it = myList.begin (); it != myList.end (); ++ it)
			{
				schemRenderState->removeTemplateSubRenderState (*it);
			}
		}
	}
	DEBUG ("Shadow Camera Setup dtor");
}

bool ShadowCameraSetup::setup()
{
	DEBUG ("Setting up the shadow camera...");
	// Need to detect D3D or GL for best depth shadowmapping.
	bool isOpenGL;
	if (Ogre::Root::getSingleton().getRenderSystem()->getName().find("GL") != Ogre::String::npos)
	{
		isOpenGL = true;
	}
	else
	{
		isOpenGL = false;
	}

	//mSceneMgr.setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED); // TODO: move setting from main menu.

	mSceneMgr.setShadowTextureCount (3);
	mSceneMgr.setShadowTextureSelfShadow (true);
	// If we can use texture sizes BIGGER than the window size, specify the settings...
	// This difference may come up most often due to the D3D/OpenGL support.
	if (Ogre::Root::getSingletonPtr ()->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_HWRENDER_TO_TEXTURE))
	{
		mSceneMgr.setShadowTextureSettings (1024, 2);
	}
	else
	{
		mSceneMgr.setShadowTextureSettings (256, 2);
	}

	mSceneMgr.setShadowTextureCountPerLightType (Ogre::Light::LT_DIRECTIONAL, 3);
	//mSceneMgr.setShadowTextureCountPerLightType(Ogre::Light::LT_POINT, 1);
	//mSceneMgr.setShadowTextureCountPerLightType(Ogre::Light::LT_SPOTLIGHT, 1);

	if (isOpenGL)
	{
		// GL performs much better if you pick half-float format
		mSceneMgr.setShadowTexturePixelFormat (Ogre::PF_FLOAT16_R);
	}
	else
	{
		// D3D is the opposite - if you ask for PF_FLOAT16_R you
		// get an integer format instead! You can ask for PF_FLOAT16_GR
		// but the precision doesn't work well
		mSceneMgr.setShadowTexturePixelFormat (Ogre::PF_FLOAT32_R);
	}

	// TODO: make configurable.
	mSceneMgr.setShadowTextureCasterMaterial("PSSM/shadow_caster");

	switch (m_shadow_camera_setup_type)
	{
	case 1:
		mSharedCameraPtr = Ogre::ShadowCameraSetupPtr(OGRE_NEW Ogre::LiSPSMShadowCameraSetup ());
		break;
	case 2:
		mSharedCameraPtr = Ogre::ShadowCameraSetupPtr(OGRE_NEW Ogre::FocusedShadowCameraSetup ());
		break;
	case 3:
		// A plane is needed to initialize the camera by.
		//Ogre::ShadowCameraSetupPtr(OGRE_NEW Ogre::PlaneOptimalShadowCameraSetup (mPlane));
		break;
	case 4:
		mPssmSetup = OGRE_NEW Ogre::PSSMShadowCameraSetup();
		mSharedCameraPtr = Ogre::ShadowCameraSetupPtr(mPssmSetup);
		break;
	case 0:
	default:
		mSharedCameraPtr = Ogre::ShadowCameraSetupPtr(OGRE_NEW Ogre::DefaultShadowCameraSetup ());
		break;
	}
	
	
	mSceneMgr.setShadowCameraSetup(mSharedCameraPtr);

	DEBUG ("Successfully set up the shadow camera.");
	return true;
}

void ShadowCameraSetup::configAndCalculateSplitPoints (size_t splitCount, Ogre::Real nearDistance, Ogre::Real farDistance)
{
	try 
	{
		DEBUG ("Calculating shadow camera PSSM splitpoints for count=%d, near-far dist = %.2f, %.2f", splitCount, nearDistance, farDistance);
		mPssmSetup->calculateSplitPoints (splitCount, nearDistance, farDistance);
	}
	catch (const std::exception& ex) 
	{
		ERRORMSG ("Caught exception when setting shadow texture size: [%s]", ex.what ());
	}
}



void ShadowCameraSetup::Config_ShadowTextureSize (int textureSize)
{	
	try 
	{
		mSceneMgr.setShadowTextureSize (textureSize);
	}
	catch (const std::exception& ex) 
	{
		ERRORMSG ("Caught exception when setting shadow texture size: [%s]", ex.what ());
	}
}

void ShadowCameraSetup::Config_ShadowSplitPoints (int a, int b, int c, int d)
{
	try 
	{
		Ogre::PSSMShadowCameraSetup::SplitPointList splitPointList = mPssmSetup->getSplitPoints();
		splitPointList[0] = a;
		splitPointList[1] = b;
		splitPointList[2] = c;
		splitPointList[3] = d;
		mPssmSetup->setSplitPoints(splitPointList);

	} 
	catch (const std::exception& ex) 
	{
		ERRORMSG ("Caught exception when setting shadow split points: [%s]", ex.what());
	}
}

void ShadowCameraSetup::Config_ShadowSplitPadding (float padding)
{
	try 
	{
		mPssmSetup->setSplitPadding(padding);
	}
	catch (const std::exception& ex) 
	{
		ERRORMSG("Caught exception when setting shadow split padding: [%s]", ex.what());
	}
}

void ShadowCameraSetup::Config_ShadowOptimalAdjustFactors (float a, float b, float c)
{
	try 
	{
		mPssmSetup->setOptimalAdjustFactor(0, a);
		mPssmSetup->setOptimalAdjustFactor(1, b);
		mPssmSetup->setOptimalAdjustFactor(2, c);

		mPssmSetup->setUseSimpleOptimalAdjust (false);
	} 
	catch (const std::exception& ex) 
	{
		ERRORMSG("Caught exception when setting optimal shadow adjust factors: [%s]", ex.what());
	}
}

void ShadowCameraSetup::Config_ShadowUseAggressiveFocusRegion (bool useAggressiveFocusRegion)
{
	try 
	{
		mPssmSetup->setUseAggressiveFocusRegion(useAggressiveFocusRegion);
	} 
	catch (const std::exception& ex)
	{
		ERRORMSG("Caught exception when shadow use aggressive focus region: [%s]", ex.what());
	}
}

void ShadowCameraSetup::Config_ShadowFarDistance (float dist)
{
	try 
	{
		mSceneMgr.setShadowFarDistance(dist);
	} 
	catch (const std::exception& ex)
	{
		ERRORMSG("Caught exception when setting shadow far distance: [%s]", ex.what());
	}
}

void ShadowCameraSetup::Config_ShadowRenderBackfaces (bool renderBackfaces)
{
	try
	{
		mSceneMgr.setShadowCasterRenderBackFaces(renderBackfaces);
	} 
	catch (const std::exception& ex) 
	{
		ERRORMSG("Caught exception when setting shadow render back faces: [%s]", ex.what());
	}
}
