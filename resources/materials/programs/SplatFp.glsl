/*
	Splatting fragment shader
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

// Splatting part is based on older Ember shader written by Erik Hjortsberg

// Conditional defines are disabled because of bug in OGRE 1.6.0 GLSL preprocessor - http://www.ogre3d.org/forums/viewtopic.php?f=4&t=46837

// Number of lights
// 3 is resonable number to support
//#ifndef NUM_LIGHTS
// #define NUM_LIGHTS	3
//#endif

// Either have shadows or not
// Supports 3 lights
//#ifndef SHADOW
// #define SHADOW			1
//#endif

#define PCF				1

// Number of splatting layers, should be set from application
//#ifndef NUM_LAYERS
//#define NUM_LAYERS	1
//#endif

// Either use normal mapping or not
//#ifndef NORMAL_MAP
#define NORMAL_MAP		0
//#endif

// Number of parallax steps - works when normal mapping is on as it needs height map
// 3 is ok for small heights (scale < 0,05)
// 5-7 for bigger ones
//#ifndef PARALLAX_STEPS
//#define PARALLAX_STEPS	0
//#endif

// Either show specular highlights or not
// Could be disabled for better speed
//#ifndef SPECULAR
#define SPECULAR		0
//#endif

// Other formula for calculating specular, could save few instructions
//#ifndef SPECULAR_PHONG
#define SPECULAR_PHONG	0
//#endif

// Eye is at (0, 0, 0) or at (0, 0, infinity) in eye space, could save few instructions when not in normal mapping mode
//#ifndef EYE_AT_ORIGIN
#define EYE_AT_ORIGIN	1
//#endif


// Fog factor
varying float fog;

///The number of active lights. This can be less than NUM_LIGHTS depending on the scene.
uniform float numberOfActiveLights;

#if SHADOW
#define LINEAR_RANGE 0

// Shadow maps
uniform sampler2D shadowMap0;
uniform sampler2D shadowMap1;
uniform sampler2D shadowMap2;
uniform sampler2D shadowMap3;
uniform sampler2D shadowMap4;

// Shadow world view projection matrix
//uniform mat4 shadowWorldViewProjectionMatrix0;
//uniform mat4 shadowWorldViewProjectionMatrix1;
//uniform mat4 shadowWorldViewProjectionMatrix2;
//uniform mat4 shadowWorldViewProjectionMatrix3;
//uniform mat4 shadowWorldViewProjectionMatrix4;

uniform vec4 inverseShadowMapSize0;
uniform vec4 inverseShadowMapSize1;
uniform vec4 inverseShadowMapSize2;
uniform vec4 inverseShadowMapSize3;
uniform vec4 inverseShadowMapSize4;

uniform float fixedDepthBias;
uniform float gradientClamp;
uniform float gradientScaleBias;

uniform vec4 pssmSplitPoints;

// Shadow texture coordinates
varying vec4 shadowTexCoord0;
varying vec4 shadowTexCoord1;
varying vec4 shadowTexCoord2;
varying vec4 shadowTexCoord3;
varying vec4 shadowTexCoord4;
#endif

#if NUM_LAYERS > 0
#if NUM_LAYERS > 8
uniform vec4 scales[3];
#else
#if NUM_LAYERS > 4
uniform vec4 scales[2];
#else
uniform vec4 scales[1];
#endif
#endif
uniform sampler2D baseTextureDiffuse;
#if NUM_LAYERS > 1
uniform sampler2D coverageTexture1;
#endif
#if NUM_LAYERS > 1
uniform sampler2D splat1TextureDiffuse;
#endif
#if NUM_LAYERS > 2
uniform sampler2D splat2TextureDiffuse;
#endif
#if NUM_LAYERS > 3
uniform sampler2D splat3TextureDiffuse;
#endif
#if NUM_LAYERS > 4
uniform sampler2D splat4TextureDiffuse;
#endif
#if NUM_LAYERS > 5
uniform sampler2D coverageTexture2;
#endif
#if NUM_LAYERS > 5
uniform sampler2D splat5TextureDiffuse;
#endif
#if NUM_LAYERS > 6
uniform sampler2D splat6TextureDiffuse;
#endif
#if NUM_LAYERS > 7
uniform sampler2D splat7TextureDiffuse;
#endif
#if NUM_LAYERS > 8
uniform sampler2D splat8TextureDiffuse;
#endif
#if NUM_LAYERS > 9
uniform sampler2D coverageTexture3;
#endif
#if NUM_LAYERS > 9
uniform sampler2D splat9TextureDiffuse;
#endif
#if NUM_LAYERS > 10
uniform sampler2D splat10TextureDiffuse;
#endif
#if NUM_LAYERS > 11
uniform sampler2D splat11TextureDiffuse;
#endif
#endif

// Fragment position in eye space
varying vec3 eyePosition;

// Normal in eye space
varying vec3 eyeNormal;

// Light attenuation, packed to vector
varying vec3 attenuation;
//#define ATTENUATION_I attenuation[i]

#if NUM_LAYERS > 0
vec4 splatting(vec2 texCoord)
{
	vec4 diffuseColour;
	diffuseColour = texture2D(baseTextureDiffuse, texCoord * scales[0].x);
#if NUM_LAYERS > 1
	diffuseColour = (mix(diffuseColour, texture2D(splat1TextureDiffuse, texCoord * scales[0].y), texture2D(coverageTexture1, texCoord).w));
#endif
#if NUM_LAYERS > 2
	diffuseColour = (mix(diffuseColour, texture2D(splat2TextureDiffuse, texCoord * scales[0].z), texture2D(coverageTexture1, texCoord).x));
#endif
#if NUM_LAYERS > 3
	diffuseColour = (mix(diffuseColour, texture2D(splat3TextureDiffuse, texCoord * scales[0].w), texture2D(coverageTexture1, texCoord).y));
#endif
#if NUM_LAYERS > 4
	diffuseColour = (mix(diffuseColour, texture2D(splat4TextureDiffuse, texCoord * scales[1].x), texture2D(coverageTexture1, texCoord).z));
#endif
#if NUM_LAYERS > 5
	diffuseColour = (mix(diffuseColour, texture2D(splat5TextureDiffuse, texCoord * scales[1].y), texture2D(coverageTexture2, texCoord).w));
#endif
#if NUM_LAYERS > 6
	diffuseColour = (mix(diffuseColour, texture2D(splat6TextureDiffuse, texCoord * scales[1].z), texture2D(coverageTexture2, texCoord).x));
#endif
#if NUM_LAYERS > 7
	diffuseColour = (mix(diffuseColour, texture2D(splat7TextureDiffuse, texCoord * scales[1].w), texture2D(coverageTexture2, texCoord).y));
#endif
#if NUM_LAYERS > 8
	diffuseColour = (mix(diffuseColour, texture2D(splat8TextureDiffuse, texCoord * scales[2].x), texture2D(coverageTexture2, texCoord).z));
#endif
#if NUM_LAYERS > 9
	diffuseColour = (mix(diffuseColour, texture2D(splat9TextureDiffuse, texCoord * scales[2].y), texture2D(coverageTexture3, texCoord).w));
#endif
#if NUM_LAYERS > 10
	diffuseColour = (mix(diffuseColour, texture2D(splat10TextureDiffuse, texCoord * scales[2].z), texture2D(coverageTexture3, texCoord).x));
#endif
#if NUM_LAYERS > 11
	diffuseColour = (mix(diffuseColour, texture2D(splat11TextureDiffuse, texCoord * scales[2].w), texture2D(coverageTexture3, texCoord).y));
#endif
	return diffuseColour;
}
#endif

void lighting  (in gl_LightSourceParameters light,
#if NORMAL_MAP
				in vec3 L,
#endif
				in vec3 N,
				in float attenuation,
				in float shadowing,
				inout vec4 diffuse
#if SPECULAR
				, in vec3 V
				, inout vec4 specular
#endif
#if SPECULAR_PHONG
				, in vec3 R
				, inout vec4 specular
#endif
				)
{
	// Compute vector from surface to light position
#if !NORMAL_MAP
	vec3 L = normalize(light.position.xyz - eyePosition * light.position.w);
#endif

	float NdotL = max(0.0, dot(N, L));
	diffuse += light.diffuse * NdotL * attenuation * shadowing;

#if SPECULAR
	vec3 H = normalize(L + V);

	float NdotH = max(0.0, dot(N, H));
	if (NdotL > 0.0) {
		specular += light.specular * pow(NdotH, gl_FrontMaterial.shininess) * attenuation * shadowing;
	} else {
		specular += 0.0;
	}
#endif
#if SPECULAR_PHONG
	// Phong specular power is dot(reflect(L, N), V), replacing this with equal equation dot(reflect(V, N), L)
	float RdotL = max(0.0, dot(R, L));
	if (NdotL > 0.0) {
		// To accomodate substitution of half vector with reflect vector, dividing shininess by 4
		specular += light.specular * pow(RdotL, gl_FrontMaterial.shininess / 4.0) * attenuation * shadowing;
	}
#endif
}

#if SHADOW

float shadowPCF(in sampler2D shadowMap, in vec4 shadowTexCoord, vec2 inverseShadowMapSize)
{
	// point on shadowmap
#if LINEAR_RANGE
	shadowTexCoord.xy = shadowTexCoord.xy / shadowTexCoord.w;
#else
	shadowTexCoord = shadowTexCoord / shadowTexCoord.w;
#endif

#if !PCF
	float depth = texture2D(shadowMap, shadowTexCoord.xy).x * (1.0 - fixedDepthBias);
	return (depth > shadowTexCoord.z) ? 1.0 : 0.0;
#else

	// Do not shade too far away objects
	if (abs(shadowTexCoord.z) > 1.0)
	{
		return 1.0;
	}

	float centerdepth = texture2D(shadowMap, shadowTexCoord.xy).x;

	// gradient calculation
  	float pixeloffset = inverseShadowMapSize.x;
	vec4 depths = vec4(
		texture2D(shadowMap, shadowTexCoord.xy + vec2(-pixeloffset, 0)).x,
		texture2D(shadowMap, shadowTexCoord.xy + vec2(+pixeloffset, 0)).x,
		texture2D(shadowMap, shadowTexCoord.xy + vec2(0, -pixeloffset)).x,
		texture2D(shadowMap, shadowTexCoord.xy + vec2(0, +pixeloffset)).x);

	vec2 differences = abs( depths.yw - depths.xz );
	float gradient = min(gradientClamp, max(differences.x, differences.y));
	float gradientFactor = gradient * gradientScaleBias;

	// visibility function
	float depthAdjust = gradientFactor - (fixedDepthBias * centerdepth);
	//depthAdjust =  -fixedDepthBias * centerdepth;
	float finalCenterDepth = centerdepth + depthAdjust;

	// use depths from prev, calculate diff
	depths += depthAdjust;
	float final = (finalCenterDepth > shadowTexCoord.z) ? 1.0 : 0.0;
	final += (depths.x > shadowTexCoord.z) ? 1.0 : 0.0;
	final += (depths.y > shadowTexCoord.z) ? 1.0 : 0.0;
	final += (depths.z > shadowTexCoord.z) ? 1.0 : 0.0;
	final += (depths.w > shadowTexCoord.z) ? 1.0 : 0.0;
	final *= 0.2;

	return final;
#endif
}

float shadowPSSM()
{
	float depth = gl_TexCoord[0].p;
	float shadowing = 0.0;

	if (depth <= pssmSplitPoints.y)
	{
		shadowing = shadowPCF(shadowMap0, shadowTexCoord0, inverseShadowMapSize0.xy);
	}
	else if (depth <= pssmSplitPoints.z)
	{
		shadowing = shadowPCF(shadowMap1, shadowTexCoord1, inverseShadowMapSize1.xy);
	}
	else
	{
		shadowing = shadowPCF(shadowMap2, shadowTexCoord2, inverseShadowMapSize2.xy);
	}

	return shadowing;
}

vec3 shadowPSSMDebug()
{
	float depth = gl_TexCoord[0].p;
	float shadowing = 0.0;
	vec3 splitColour;
	float showLayer = 0.0;

	if (depth <= pssmSplitPoints.y && showLayer == 0.0 || showLayer == 1.0)
	{
		splitColour = vec3(1.0, 0.0, 0.0);
		shadowing = shadowPCF(shadowMap0, shadowTexCoord0, inverseShadowMapSize0.xy);
	}
	else if (depth <= pssmSplitPoints.z && showLayer == 0.0 || showLayer == 2.0)
	{
		splitColour = vec3(0.0, 1.0, 0.0);
		shadowing = shadowPCF(shadowMap1, shadowTexCoord1, inverseShadowMapSize1.xy);
	}
	else
	{
		splitColour = vec3(0.0, 0.0, 1.0);
		shadowing = shadowPCF(shadowMap2, shadowTexCoord2, inverseShadowMapSize2.xy);
	}

	return vec3(1.0) - (vec3(1.0) - splitColour) * (1.0 - shadowing);
}

vec3 shadow3Debug()
{
	float shadowing1, shadowing2, shadowing3;

	shadowing1 = shadowPCF(shadowMap0, shadowTexCoord0, inverseShadowMapSize0.xy);
	shadowing2 = shadowPCF(shadowMap1, shadowTexCoord1, inverseShadowMapSize1.xy);
	shadowing3 = shadowPCF(shadowMap2, shadowTexCoord2, inverseShadowMapSize2.xy);

	return vec3(1.0) - ((1.0 - shadowing1) * (1.0 - vec3(1.0, 0.0, 0.0)) +
						(1.0 - shadowing2) * (1.0 - vec3(0.0, 1.0, 0.0)) +
						(1.0 - shadowing3) * (1.0 - vec3(0.0, 0.0, 1.0)));
}
#endif

void main()
{
	// Exit if fully in fog
//	if (fog == 0.0)
//	{
//		gl_FragColor = gl_Fog.color;
//		return;
//	}

	vec2 uv = gl_TexCoord[0].st;

	vec4 diffuseColour = splatting(uv);

	// Renormalize interpolated normal, required for good quality specular highlight
	vec3 N = normalize(eyeNormal);

//If we're using shadows, we'll iterate through all of the lights and look up against the shadow textures etc.
//If we're not however, we'll just use the first light (normally the sun) and make the whole lightning model much simpler.
	// Accumulates diffuse light colour
	vec4 diffuse = vec4(0.0);

	// Loop through lights, compute contribution from each
	for (int i = 0; i < NUM_LIGHTS && i < int(numberOfActiveLights); i++)
	{
		float shadowing = 1.0;

#if SHADOW
		// Use PSSM only for first directional light
		if (gl_LightSource[0].position.w == 0.0)
		{
			if (i == 0)
			{
				shadowing = shadowPSSM();
			}
			else if (i == 1)
			{
				shadowing = shadowPCF(shadowMap3, shadowTexCoord3, inverseShadowMapSize3.xy);
			}
			else if (i == 2)
			{
				shadowing = shadowPCF(shadowMap4, shadowTexCoord4, inverseShadowMapSize4.xy);
			}
		}
		else
		{
			if (i == 0)
			{
				shadowing = shadowPCF(shadowMap0, shadowTexCoord0, inverseShadowMapSize0.xy);
			}
			else if (i == 1)
			{
				shadowing = shadowPCF(shadowMap1, shadowTexCoord1, inverseShadowMapSize1.xy);
			}
			else if (i == 2)
			{
				shadowing = shadowPCF(shadowMap2, shadowTexCoord2, inverseShadowMapSize2.xy);
			}
		}
#endif
		float curAttenuation = attenuation[i];

		lighting(gl_LightSource[i]
#if NORMAL_MAP
			, L
#endif
			, N, curAttenuation /*ATTENUATION_I*/
			, shadowing
			, diffuse
#if SPECULAR
			, V, specular
#endif
#if SPECULAR_PHONG
			, R, specular
#endif
		);
	}

	vec3 colour = vec3(gl_LightModel.ambient * diffuseColour +
#if SPECULAR || SPECULAR_PHONG
		specularColour * specular +
#endif
		diffuseColour * diffuse)
		// /1000.0 + shadowPSSMDebug()
		// /1000.0 + shadow3Debug()
		;

#if FOG
	gl_FragColor.rgb = mix(gl_Fog.color.rgb, colour, fog);
#else
	gl_FragColor.rgb = colour;
#endif

	gl_FragColor.a = diffuseColour.a;
}
