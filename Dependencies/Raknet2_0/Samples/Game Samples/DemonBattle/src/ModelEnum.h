// All the paths to the various models, model textures, and animation files here

#ifndef __MODEL_ENUM_H
#define __MODEL_ENUM_H

enum modelNames
{
	DEMON_MODEL,CD2000_MODEL,NUMBER_OF_MODELS
};

enum xModelNames
{
	CASTLE_MODEL,NUMBER_OF_XMODELS
};

const char modelPaths[NUMBER_OF_MODELS][40] = 
{
	"graphic\\models\\bauul.md2",
	"graphic\\models\\cd2000.md2"
};

const char xModelPaths[NUMBER_OF_XMODELS][40] = 
{
	"graphic\\models\\castle.x"
};

const float axisToRotateAround[NUMBER_OF_MODELS][3] = 
{
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};

const float amountToRotateInRadians[NUMBER_OF_MODELS] = 
{
	-3.1415927f/2.0f,
	-3.1415927f/2.0f
};

enum textureNames
{
	DEMON_TEXTURE_1,
	DEMON_TEXTURE_2,
	CD2000_TEXTURE,
	TERRAIN_TEXTURE_1,
	PARTICLE_TEXTURE_1,	
	NUMBER_OF_TEXTURES
};

const char texturePaths[NUMBER_OF_TEXTURES][40] = 
{
	"graphic\\models\\bauul1.jpg",
	"graphic\\models\\bauul2.jpg",
	"graphic\\models\\CD2000.jpg",
	"graphic\\JPG\\Terrain\\ground.jpg",
	"graphic\\JPG\\particle.jpg",
};

#endif
