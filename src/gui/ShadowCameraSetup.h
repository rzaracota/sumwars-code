//
// C++ Interface: ShadowCameraSetup
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
#ifndef EMBEROGRESHADOWCAMERASETUP_H
#define EMBEROGRESHADOWCAMERASETUP_H

#include <OgreShadowCameraSetup.h>

namespace Ogre
{
	class PSSMShadowCameraSetup;
}

/**
 *
 * @brief This takes care of the setup of the pssm shadow camera.
 *
 * Apart from setting up the camera it also listens for changes to the varconf settings dealing with the shadows and update the shadow settings accordingly.
 *
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class ShadowCameraSetup
{
public:
    /**
     * @brief Ctor.
     * @param sceneMgr The scene manager to which we want to apply our shadow camera.
     */
    ShadowCameraSetup(Ogre::SceneManager& sceneMgr);

    /**
     * @brief Dtor.
     * This will actually not reset the shadows.
     */
    virtual ~ShadowCameraSetup();

protected:
    /**
     * @brief Sets up the shadows.
     * A couple of settings are however omitted in this method as they are instead set in the Config_* methods.
     * @return
     */
    virtual bool setup();

    /**
     * @brief The scene manager which we want to apply our shadow camera to.
     */
    Ogre::SceneManager& mSceneMgr;

    /**
     * @brief The pssm shadow camera setup.
     */
    Ogre::PSSMShadowCameraSetup* mPssmSetup;

    /**
     * @brief A reference to the shared pointer, which guarantees that the mPssmSetup field isn't deleted inside Ogre.
     */
    Ogre::ShadowCameraSetupPtr mSharedCameraPtr;
private:

	/**
	 * @brief Sets the shadow texture size.
	 * @param textureSize
	 */
	void Config_ShadowTextureSize(int textureSize);

	/**
	 * @brief Sets the shadow split points.
	 * @param a
	 * @param b
	 * @param c
	 * @param d
	 */
	void Config_ShadowSplitPoints(int a, int b, int c, int d);

	/**
	 * @brief Sets the shadow split padding, a float.
	 * @param section
	 * @param key
	 * @param variable
	 */
	void Config_ShadowSplitPadding(float padding);

	/**
	 * @brief Sets the shadow optimal adjust factors (as a space separated series of floats, for example "1 1 1").
	 * @param a
	 * @param b
	 * @param c
	 */
	void Config_ShadowOptimalAdjustFactors(float a, float b, float c);

	/**
	 * @brief Sets the shadow use aggressive focus region, as a boolean.
	 * @param useAggressiveFocusRegion
	 */
	void Config_ShadowUseAggressiveFocusRegion(bool useAggressiveFocusRegion);

	/**
	 * @brief Sets the shadow far distance, as a float.
	 * @param dist
	 */
	void Config_ShadowFarDistance(float dist);

	/**
	 * @brief Sets whether back faces should be rendered, as a boolean.
	 * @param renderBackfaces
	 */
	void Config_ShadowRenderBackfaces(bool renderBackfaces);

};
#endif
