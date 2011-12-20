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

/**
 * File info: 
 *      the file was started from the Worldforge Ember client similarly named file. It was adapted and greatly changed.
 * 
 * List of contributors:
 * @author Erik Hjortsberg (original Ember client file).
 * @author Augustin Preda.
 */
#ifndef SHADOWCAMERASETUP_H
#define SHADOWCAMERASETUP_H

#include <OgreShadowCameraSetup.h>

#include <OgreRTShaderSystem.h>

// Forward declarations.
namespace Ogre
{
	class PSSMShadowCameraSetup;
}


/**
 * @brief This takes care of the setup of the pssm shadow camera.
 * The PSSM is difficult to set up due to the large amount of parameters that need tinkering with.
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
    ShadowCameraSetup(Ogre::SceneManager& sceneMgr, Ogre::RTShader::ShaderGenerator* rtssPtr = 0);

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

	/**
	 * @brief The Shader generator instance.
	 */
	Ogre::RTShader::ShaderGenerator* mShaderGenerator;

private:

	/**
	 * Set m_shadow_camera_setup_type to:
	 * 0 = Default
	 * 1 = LiSPSMShadowCameraSetup
	 * 2 = FocusedShadowCameraSetup
	 * 3 = PlaneOptimalShadowCameraSetup
	 * 4 = PSSM
	 * This is kept as an integer value only temporary. It should be turned into an enum and read from a config file.
	 */
	int m_shadow_camera_setup_type;

	/**
	 * Configure function:
	 * calculate the split points for the PSSM
	 * @param splitCount The amount of split points to use. Recommended value : 3
	 * @param nearDistance The closest distance to calculate from. Recommended value : the camera near clip distance.
	 * @param farDistance The furthest point to calculate to. Recommended value: larger than the shadow far distance.
	 */
	void configAndCalculateSplitPoints (size_t splitCount, Ogre::Real nearDistance, Ogre::Real farDistance);

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
#endif // SHADOWCAMERASETUP_H
