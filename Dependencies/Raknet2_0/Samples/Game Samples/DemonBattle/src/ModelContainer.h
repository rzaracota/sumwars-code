#ifndef __MODEL_CONTAINER_H
#define __MODEL_CONTAINER_H

#include <windows.h>
#include "md2model.h"
#include "AnimationGenerator.h"
#include "ModelEnum.h"
#include "XFile.h"

class ModelContainer
{
public:
	ModelContainer();
	~ModelContainer();
	static ModelContainer* Instance() {return &I;}
	void LoadAll(void);
	void UnloadAll(void);
	void LoadMD2Model(int i);
	void UnloadMD2Model(int i);
	void LoadXModel(int i);
	void UnloadXModel(int i);
	void LoadTexture(int i);
	void UnloadAllTextures(void);

	MD2Model models[NUMBER_OF_MODELS];
	XFile xModels[NUMBER_OF_XMODELS];
	LPDIRECT3DTEXTURE9 textures[NUMBER_OF_TEXTURES];
	AnimationGenerator animations[NUMBER_OF_MODELS];

private:
	static ModelContainer I;
};

#define modelContainer ModelContainer::Instance()

#endif