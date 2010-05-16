#ifndef __X_FILE_H
#define __X_FILE_H

#include <windows.h>
#include "D3DHeaders.h"
#include "..\Common\Include\d3dutil.h"

class XFile
{
public:
	XFile();
	~XFile();
	void Load(const char* filename);
	void Unload(void);
	void Render(void);
	bool isLoaded(void) {return XModel.pMesh!=0 && *(XModel.pMeshTextures)!=0 && XModel.pMeshMaterials!=0;}
protected:
	X3DMODELL XModel;
};

#endif