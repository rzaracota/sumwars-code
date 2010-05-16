#include <assert.h>
#include "FileLoadedVisibleEntity.h"
#include "D3DHeaders.h"
#include "User.h"
#include "ManagerState.h"
#include <D3dx9math.h>
#include "..\Common\Include\d3dutil.h"
#include "TerrainMesh.h"
#include "Math.h"
#include "GameState.h"

//#include "MemoryManager.h"

//List<FileLoadedVisibleEntity*> FileLoadedVisibleEntity::visibleEntitiesList;

FileLoadedVisibleEntity::FileLoadedVisibleEntity()
{
	 remoteAnimation=0; remoteTexture=0; /*visibleEntitiesList.insert(this);*/
	 remoteXFile=0;
}


void FileLoadedVisibleEntity::Render(long time)
{
	VisibleEntity::Render(time);

	if (remoteAnimation && remoteTexture && remoteModel)
	{
		m_pd3dDevice->SetTexture(0,remoteTexture);
		m_pd3dDevice->SetFVF (D3DFVF_MODELVERTEX);
		
		m_pd3dDevice->SetMaterial (&mat);


		m_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, //Typ	
											remoteModel->GetTriangleCount(),		  //Anzahl
											*(remoteAnimation->GetMeshDataPointer()),		  //Pointer auf Daten
											sizeof(MODELVERTEX));  //Größe Vertex
	}
	else if (remoteXFile)
	{
		assert(remoteXFile->isLoaded());
		remoteXFile->Render();
	}
	else
		manager->LogWrite("Error in FileLoadedVisibleEntity::Render, attempting to render an object that was not loaded");
}

void FileLoadedVisibleEntity::Update(long time)
{
}

// Note that you can use a remote mesh/texture or an xFile, not both.
void FileLoadedVisibleEntity::SetRemoteAnimation(AnimationGenerator* RemoteAnimation, MD2Model* model)
{
	assert(RemoteAnimation);
	assert(model);
	assert(remoteXFile==0);
	remoteAnimation=RemoteAnimation;
	remoteModel = model;
}

// Note that you can use a remote mesh/texture or an xFile, not both.
void FileLoadedVisibleEntity::SetRemoteTexture(LPDIRECT3DTEXTURE9 RemoteTexture)
{
	assert(RemoteTexture);
	assert(remoteXFile==0);
	remoteTexture=RemoteTexture;
}

// Note that you can use a remote mesh/texture or an xFile, not both.
void FileLoadedVisibleEntity::SetXFile(XFile* xFile)
{
	assert(remoteAnimation==0);
	assert(remoteTexture==0);
	assert(xFile!=0);
	remoteXFile=xFile;
}

void FileLoadedVisibleEntity::AdjustHeightToGround(void)
{
	oMatrix._42 = gameState->GetTerrainMesh()->GetHeight(oMatrix._41, oMatrix._43) - GetYMin();
}

float FileLoadedVisibleEntity::GetXMax(int animationIndex) const {if (animationIndex>=0) return remoteModel->xMax[remoteAnimation->GetStartingKeyframeForAnimationIndex(animationIndex)] * scale; return remoteModel->xMax[remoteAnimation->GetStartingKeyframeForAnimationIndex()] * scale;}
float FileLoadedVisibleEntity::GetXMin(int animationIndex) const {if (animationIndex>=0) return remoteModel->xMin[remoteAnimation->GetStartingKeyframeForAnimationIndex(animationIndex)] * scale; return remoteModel->xMin[remoteAnimation->GetStartingKeyframeForAnimationIndex()] * scale;}
float FileLoadedVisibleEntity::GetYMax(int animationIndex) const {if (animationIndex>=0) return remoteModel->yMax[remoteAnimation->GetStartingKeyframeForAnimationIndex(animationIndex)] * scale; return remoteModel->yMax[remoteAnimation->GetStartingKeyframeForAnimationIndex()] * scale;}
float FileLoadedVisibleEntity::GetYMin(int animationIndex) const {if (animationIndex>=0) return remoteModel->yMin[remoteAnimation->GetStartingKeyframeForAnimationIndex(animationIndex)] * scale; return remoteModel->yMin[remoteAnimation->GetStartingKeyframeForAnimationIndex()] * scale;}
float FileLoadedVisibleEntity::GetZMax(int animationIndex) const {if (animationIndex>=0) return remoteModel->zMax[remoteAnimation->GetStartingKeyframeForAnimationIndex(animationIndex)] * scale; return remoteModel->zMax[remoteAnimation->GetStartingKeyframeForAnimationIndex()] * scale;}
float FileLoadedVisibleEntity::GetZMin(int animationIndex) const {if (animationIndex>=0) return remoteModel->zMin[remoteAnimation->GetStartingKeyframeForAnimationIndex(animationIndex)] * scale; return remoteModel->zMin[remoteAnimation->GetStartingKeyframeForAnimationIndex()] * scale;}
