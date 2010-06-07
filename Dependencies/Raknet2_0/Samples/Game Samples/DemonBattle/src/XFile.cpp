#include "XFile.h"
#include "..\Common\Include\d3dutil.h"
#include <assert.h>

//#include "MemoryManager.h"

XFile::XFile()
{
}

XFile::~XFile()
{

}

void XFile::Load(const char* filename)
{
	XModel = LoadXFile(filename);
}

void XFile::Unload(void)
{
//	if (XModel.pMeshMaterials)
//	{
	//	delete [] XModel.pMeshMaterials;
	//	XModel.pMeshMaterials=0;
//	}

	/*
		for (int i=0; i < XModel.dwNumMaterials; i++)
		{
			delete XModel.pMeshTextures[i];
			XModel.pMeshTextures[i]=0;
		}
*/

//	if (XModel.pMeshTextures)
	//{
		// Unloading mid-game causes mem corruption in XModel.pMeshTextures and I don't know why.
		for (int i=0; i < (int)XModel.dwNumMaterials; i++)
		{
			if (XModel.pMeshTextures[i])
			{
				XModel.pMeshTextures[i]->Release();
				XModel.pMeshTextures[i]=0;
			}
		}

//		delete [] XModel.pMeshTextures;
//		XModel.pMeshTextures=0;
	//}

	if (XModel.pMesh)
	{
		XModel.pMesh->Release();
		// is this a memory leak?
		XModel.pMesh=0;
	}
}

void XFile::Render(void)
{
	assert(isLoaded());
	RenderX(XModel);
}
