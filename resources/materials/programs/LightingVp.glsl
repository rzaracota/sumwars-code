/*
	Lighting vertex shader
	Copyright (C) 2009  Alexey Torkhov

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Based on OGRE samples
// Some ideas were borrowed from Arseny Kapoulkine's blog - http://zeuxcg.blogspot.com/2007/10/my-own-lighting-shader-with-blackjack.html

// Conditional defines are disabled because of bug in OGRE 1.6.0 GLSL preprocessor - http://www.ogre3d.org/forums/viewtopic.php?f=4&t=46837

// Type of fog
//#if !defined(FOG_NONE) && !defined(FOG_LINEAR) && !defined(FOG_EXP) && !defined(FOG_EXP2)
//#define FOG_NONE		1
//#endif
//#ifndef FOG_NONE
#define FOG_NONE		0
//#endif
//#ifndef FOG_LINEAR
#define FOG_LINEAR		0
//#endif
//#ifndef FOG_EXP
#define FOG_EXP			0
//#endif
//#ifndef FOG_EXP2
//#define FOG_EXP2		0
//#endif

// Number of lights
// 3 is resonable number to support
//#ifndef NUM_LIGHTS
#define NUM_LIGHTS		3
//#endif

// Either have shadows or not
// Supports 3 lights
//#ifndef SHADOW
//#define SHADOW			0
//#endif

// Either use normal mapping or not
//#ifndef NORMAL_MAP
//#define NORMAL_MAP		0
//#endif

// Number of parallax steps - works when normal mapping is on as it needs height map
// 3 is ok for small heights (scale < 0,05)
// 5-7 for bigger ones
//#ifndef PARALLAX_STEPS
//#define PARALLAX_STEPS	0
//#endif


// Fog factor
varying float fog;

///The number of active lights. This can be less than NUM_LIGHTS depending on the scene.
uniform float numberOfActiveLights;

#if SHADOW
#define LINEAR_RANGE 0

// Shadow maps
//uniform sampler2D shadowMap0;
//uniform sampler2D shadowMap1;
//uniform sampler2D shadowMap2;
//uniform sampler2D shadowMap3;
//uniform sampler2D shadowMap4;

// Shadow world view projection matrix
uniform mat4 shadowWorldViewProjectionMatrix0;
uniform mat4 shadowWorldViewProjectionMatrix1;
uniform mat4 shadowWorldViewProjectionMatrix2;
uniform mat4 shadowWorldViewProjectionMatrix3;
uniform mat4 shadowWorldViewProjectionMatrix4;

//uniform vec4 inverseShadowMapSize0;
//uniform vec4 inverseShadowMapSize1;
//uniform vec4 inverseShadowMapSize2;
//uniform vec4 inverseShadowMapSize3;
//uniform vec4 inverseShadowMapSize4;

//uniform float fixedDepthBias;
//uniform float gradientClamp;
//uniform float gradientScaleBias;

//uniform vec4 pssmSplitPoints;

// Shadow texture coordinates
varying vec4 shadowTexCoord0;
varying vec4 shadowTexCoord1;
varying vec4 shadowTexCoord2;
varying vec4 shadowTexCoord3;
varying vec4 shadowTexCoord4;

#if LINEAR_RANGE
uniform vec4 shadowDepthRange0;
uniform vec4 shadowDepthRange1;
uniform vec4 shadowDepthRange2;
uniform vec4 shadowDepthRange3;
uniform vec4 shadowDepthRange4;
#endif
#endif

#if !NORMAL_MAP
// Fragment position in eye space
varying vec3 eyePosition;

// Normal in eye space
varying vec3 eyeNormal;

// Light attenuation, packed to vector
varying vec3 attenuation;
//#define ATTENUATION_I attenuation[i]

#else // NORMAL_MAP
// Normal+height map texture
//uniform sampler2D normalHeightMap;

#if PARALLAX_STEPS > 0
// Scale and bias for parallax
// Having scale = 0,05 - 0,1 provides BIG visual depth
// Bias = - scale/2  shifts texture equally by depth
// Bias = - scale    is also good to have holes or scratches on flat surface
uniform vec2 scaleBias;
#endif

// Light vector in tangent space (not normalized to keep linearity)
// 4-th component contains attenuation value
varying vec4 lightDir[NUM_LIGHTS];
//#define ATTENUATION_I lightDir[i].w

// Eye direction in tangent space
varying vec3 eyeDir;

// Tangent and binormal provided by application
attribute vec3 tangent;
attribute vec3 binormal;
#endif // NORMAL_MAP

#if WAVE
uniform float time;
uniform float frequency;
uniform vec4 direction;
#endif


// We're intentionally not using a normal supplied by Ogre, and instead are using the gl_Normal supplied by OpenGL.
// This is because we were getting incorrect normal when the Ogre one's were used. This flies in the face of what this attribute is supposed to fix, i.e. providing a more robust way to get the normal which works on all drivers. But there you go.
// attribute vec3 normal; <-- intentionally not used, but kept here to point out the fact


void main()
{
	vec4 position = gl_Vertex;
#if WAVE
	if (gl_MultiTexCoord0.y == 0.0) 
	{ 
		position += direction * sin(time + gl_Vertex.x * frequency);
	} 	
#endif
	// Pass-through default components
	gl_Position    = gl_ModelViewProjectionMatrix * position;
	//gl_FrontColor  = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;

#if !NORMAL_MAP
	// Transform vertex to eye space
	eyePosition = vec3(gl_ModelViewMatrix * gl_Vertex);

	// Transform normal to eye space
	eyeNormal = gl_NormalMatrix * gl_Normal;
#else
	vec3 N = normalize(gl_NormalMatrix * gl_Normal);
	vec3 T = normalize(gl_NormalMatrix * tangent);
	// Seems that texture space is left-handed
	vec3 B = cross(T, N);
	//vec3 B = normalize(gl_NormalMatrix * attrBitangent);

	vec3 eyePosition = vec3(gl_ModelViewMatrix * gl_Vertex);
	vec3 eye = -eyePosition;

	// Multiply eye direction by TBN matrix to transform it to tangent space
	// eyeDir = mat3(T, B, N) * eye;
	eyeDir = vec3(dot(eye, T), dot(eye, B), dot(eye, N));
#endif

#if SHADOW
	// Set coordinates on shadow textures
	shadowTexCoord0 = shadowWorldViewProjectionMatrix0 * gl_Vertex;
	shadowTexCoord1 = shadowWorldViewProjectionMatrix1 * gl_Vertex;
	shadowTexCoord2 = shadowWorldViewProjectionMatrix2 * gl_Vertex;
	shadowTexCoord3 = shadowWorldViewProjectionMatrix3 * gl_Vertex;
	shadowTexCoord4 = shadowWorldViewProjectionMatrix4 * gl_Vertex;

#if LINEAR_RANGE
	shadowTexCoord0.z = (shadowTexCoord0.z - shadowDepthRange0.x) * shadowDepthRange0.w;
	shadowTexCoord1.z = (shadowTexCoord1.z - shadowDepthRange1.x) * shadowDepthRange1.w;
	shadowTexCoord2.z = (shadowTexCoord2.z - shadowDepthRange2.x) * shadowDepthRange2.w;
	shadowTexCoord3.z = (shadowTexCoord3.z - shadowDepthRange3.x) * shadowDepthRange3.w;
	shadowTexCoord4.z = (shadowTexCoord4.z - shadowDepthRange4.x) * shadowDepthRange4.w;
#endif

	// Pass depth for PSSM
	gl_TexCoord[0].p = gl_Position.z;
#endif

//We can't use a loop here, since that will break on ATI cards

#if NUM_LIGHTS > 0
	if (numberOfActiveLights > 0.0) {
		// Compute vector from surface to light position
		vec3 L = gl_LightSource[0].position.xyz - eyePosition * gl_LightSource[0].position.w;

		float curAttenuation = 1.0;
		// Attenuation only for non-directional lights
		if (gl_LightSource[0].position.w > 0.0)
		{
			// Compute distance between surface and light position
			float d = length(L);

			// Compute attenuation per vertex
			curAttenuation =  1.0 / (gl_LightSource[0].constantAttenuation +
									 gl_LightSource[0].linearAttenuation * d +
									 gl_LightSource[0].quadraticAttenuation * d * d);
		}
#if !NORMAL_MAP
		attenuation[0] = curAttenuation;
#else
		// Multiply light direction by TBN matrix to transform it to tangent space
		// lightDir[i].xyz = mat3(T, B, N) * L;
		lightDir[0] = vec4(dot(L, T), dot(L, B), dot(L, N), curAttenuation);
#endif		

	}
#endif

#if NUM_LIGHTS > 1
	if (numberOfActiveLights > 1.0) {
		// Compute vector from surface to light position
		vec3 L = gl_LightSource[1].position.xyz - eyePosition * gl_LightSource[1].position.w;

		float curAttenuation = 1.0;
		// Attenuation only for non-directional lights
		if (gl_LightSource[1].position.w > 0.0)
		{
			// Compute distance between surface and light position
			float d = length(L);

			// Compute attenuation per vertex
			curAttenuation =  1.0 / (gl_LightSource[1].constantAttenuation +
									 gl_LightSource[1].linearAttenuation * d +
									 gl_LightSource[1].quadraticAttenuation * d * d);
		}
#if !NORMAL_MAP
		attenuation[1] = curAttenuation;
#else
		// Multiply light direction by TBN matrix to transform it to tangent space
		// lightDir[i].xyz = mat3(T, B, N) * L;
		lightDir[1] = vec4(dot(L, T), dot(L, B), dot(L, N), curAttenuation);
#endif		

	}
#endif

#if NUM_LIGHTS > 2
	if (numberOfActiveLights > 2.0) {
		// Compute vector from surface to light position
		vec3 L = gl_LightSource[2].position.xyz - eyePosition * gl_LightSource[2].position.w;

		float curAttenuation = 1.0;
		// Attenuation only for non-directional lights
		if (gl_LightSource[2].position.w > 0.0)
		{
			// Compute distance between surface and light position
			float d = length(L);

			// Compute attenuation per vertex
			curAttenuation =  1.0 / (gl_LightSource[2].constantAttenuation +
									 gl_LightSource[2].linearAttenuation * d +
									 gl_LightSource[2].quadraticAttenuation * d * d);
		}
#if !NORMAL_MAP
		attenuation[2] = curAttenuation;
#else
		// Multiply light direction by TBN matrix to transform it to tangent space
		// lightDir[i].xyz = mat3(T, B, N) * L;
		lightDir[2] = vec4(dot(L, T), dot(L, B), dot(L, N), curAttenuation);
#endif		

	}
#endif



#if FOG_NONE
	fog = 1.0;
#else
	// Calculating fog factor per vertex, no visual difference with per fragment
	//gl_FogFragCoord = gl_Position.z;
	//If the fog colour alpha is null, we consider the fog disabled
	if (gl_Fog.color.a != 0.0)
	{
#if FOG_LINEAR
		fog = clamp((gl_Fog.end - gl_Position.z) * gl_Fog.scale, 0.0, 1.0);
#endif
#if FOG_EXP
		fog = clamp(exp(- gl_Fog.density * gl_Position.z), 0.0, 1.0);
#endif
#if FOG_EXP2
		fog = clamp(exp(- gl_Fog.density * gl_Fog.density * gl_Position.z * gl_Position.z), 0.0, 1.0);
#endif
	}
	else
	{
		fog = 1.0;
	}
#endif

}
