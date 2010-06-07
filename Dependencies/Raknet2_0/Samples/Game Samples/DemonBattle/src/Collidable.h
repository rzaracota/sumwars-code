#ifndef __COLLIDABLE_H
#define __COLLIDABLE_H

#include <D3dx9math.h>

#include "ArrayList.h"
class TerrainMesh;
class FileLoadedVisibleEntity;

// To make an object collidable, include an instance of this class, then in the constructor of the owner call SetOwner(this)
class Collidable
{
public:
	Collidable();
	~Collidable();
	void SetOwner(FileLoadedVisibleEntity* Owner);
	FileLoadedVisibleEntity* GetOwner(void) {return owner;}

	// Is the specified location within the bounding box of the owner?
	bool IsInBoundingBox(D3DXVECTOR3 loc);

	// Returns a pointer to the object collided into or 0 if none
	static FileLoadedVisibleEntity *CollidableObjectCollision(D3DXVECTOR3 loc);

	static BasicDataStructures::List<Collidable*> collidablesList;

protected:
	FileLoadedVisibleEntity* owner;
};

#endif
