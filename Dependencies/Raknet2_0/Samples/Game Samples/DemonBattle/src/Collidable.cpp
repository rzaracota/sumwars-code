#include <assert.h>
#include "Collidable.h"
#include "TerrainMesh.h"
#include "FileLoadedVisibleEntity.h"

//#include "MemoryManager.h"

BasicDataStructures::List<Collidable*> Collidable::collidablesList;

Collidable::Collidable()
{
	collidablesList.insert(this);
	owner=0;
}

Collidable::~Collidable()
{
	unsigned long index =collidablesList.getIndexOf(this);
	if (index != MAX_UNSIGNED_LONG)
		collidablesList.del(index);
}

bool Collidable::IsInBoundingBox(D3DXVECTOR3 loc)
{
	assert(owner);

	return loc.x >= owner->GetX() + owner->GetXMin() && loc.x <= owner->GetX() + owner->GetXMax() &&
		loc.y >= owner->GetY() + owner->GetYMin() && loc.y <= owner->GetY() + owner->GetYMax() &&
		loc.z >= owner->GetZ() + owner->GetZMin() && loc.z <= owner->GetZ() + owner->GetZMax();
}


void Collidable::SetOwner(FileLoadedVisibleEntity* Owner)
{
	assert(Owner);
	owner=Owner;
}

// Returns a pointer to the object collided into or 0 if none
FileLoadedVisibleEntity *Collidable::CollidableObjectCollision(D3DXVECTOR3 loc)
{
	for (unsigned long i=0; i < collidablesList.size(); i++)
	{
		if (collidablesList[i]->IsInBoundingBox(loc))
			return collidablesList[i]->GetOwner();
	}

	return 0;
}