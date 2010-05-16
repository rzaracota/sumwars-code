#include <assert.h>

#include "ModelContainer.h"
#include "textureman.h"
#include "ini.h"
#include "Demon.h"

//#include "MemoryManager.h"

ModelContainer ModelContainer::I;

ModelContainer::ModelContainer()
{
}

ModelContainer::~ModelContainer()
{
	UnloadAll();
}

void ModelContainer::LoadAll(void)
{
	int i;

	for (i=0; i < NUMBER_OF_MODELS; i++)
		LoadMD2Model(i);

	for (i=0; i < NUMBER_OF_TEXTURES; i++)
		LoadTexture(i);

	for (i=0; i < NUMBER_OF_XMODELS; i++)
		LoadXModel(i);


	// It's convenient to put this here but if I want to load models at different times I'll need to move this to where I load the demon model
	Demon::LoadSharedAnimations();

}

void ModelContainer::UnloadAll(void)
{
	int i;

	for (i=0; i < NUMBER_OF_MODELS; i++)
		UnloadMD2Model(i);

	UnloadAllTextures();

	for (i=0; i < NUMBER_OF_XMODELS; i++)
		UnloadXModel(i);

}

void ModelContainer::LoadXModel(int i)
{
	assert(i < NUMBER_OF_XMODELS);

	// Load the X model
	xModels[i].Load(xModelPaths[i]);
}

void ModelContainer::UnloadXModel(int i)
{
	assert(i < NUMBER_OF_XMODELS);
	xModels[i].Unload();
}

void ModelContainer::LoadMD2Model(int i)
{
	assert(i < NUMBER_OF_MODELS);

	// Load the model with an initial rotation
	models[i].Load(modelPaths[i], D3DXVECTOR3(axisToRotateAround[i][0], axisToRotateAround[i][1],axisToRotateAround[i][2]), amountToRotateInRadians[i]);

	// Load the animation data
	CIniFile file;

	animations[i].SetMD2Model(&models[i]);
/*
	SAnimation a;
	file.SetFileName (animationPaths[i]);

	//read animation data
	file.GetStringValue (a.name, "STAND", "name");
	file.GetIntValue    (a.start,"STAND", "start");
	file.GetIntValue    (a.end,"STAND"  , "end");
	file.GetIntValue    ((int&)a.totalTime,"STAND"  , "totalTime");
	animations[i].animationDataArray.insert(a);

	file.GetStringValue (a.name, "RUN", "name");
	file.GetIntValue    (a.start,"RUN", "start");
	file.GetIntValue    (a.end,"RUN"  , "end");
	file.GetIntValue    ((int&)a.totalTime,"RUN"  , "totalTime");
	animations[i].animationDataArray.insert(a);

	file.GetStringValue (a.name, "JUMP", "name");
	file.GetIntValue    (a.start,"JUMP", "start");
	file.GetIntValue    (a.end,"JUMP"  , "end");
	file.GetIntValue    ((int&)a.totalTime,"JUMP"  , "totalTime");
	animations[i].animationDataArray.insert(a);

	file.GetStringValue (a.name, "ATTACK", "name");
	file.GetIntValue    (a.start,"ATTACK", "start");
	file.GetIntValue    (a.end,"ATTACK"  , "end");
	file.GetIntValue    ((int&)a.totalTime,"ATTACK"  , "totalTime");

	file.GetStringValue (a.name, "DEATH", "name");
	file.GetIntValue    (a.start,"DEATH", "start");
	file.GetIntValue    (a.end,"DEATH"  , "end");
	file.GetIntValue    ((int&)a.totalTime,"DEATH"  , "totalTime");
	animations[i].animationDataArray.insert(a);

	file.GetStringValue (a.name, "PAIN", "name");
	file.GetIntValue    (a.start,"PAIN", "start");
	file.GetIntValue    (a.end,"PAIN"  , "end");
	file.GetIntValue    ((int&)a.totalTime,"PAIN"  , "totalTime");
	animations[i].animationDataArray.insert(a);
*/
	// Init the animation data
	animations[i].RestartAnimation();
}

void ModelContainer::UnloadMD2Model(int i)
{
	assert(i < NUMBER_OF_MODELS);
	models[i].Destroy();
}

void ModelContainer::LoadTexture(int i)
{
	assert(i < NUMBER_OF_TEXTURES);
	textures[i]=textureManager->LoadTexture(texturePaths[i]);
	assert(textures[i]);
}

void ModelContainer::UnloadAllTextures(void)
{
	textureManager->RemoveAll ();
}

