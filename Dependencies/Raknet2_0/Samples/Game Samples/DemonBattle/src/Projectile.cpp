#include "Projectile.h"
//#include "MemoryManager.h"

Projectile::Projectile()
{
	//acceleration.x=0.0f; acceleration.y=.00098f; acceleration.z=0.0f;
}

Projectile::~Projectile()
{
}

void Projectile::GetNextLoc(float time, D3DXVECTOR3* vector)
{
	//x0 + v0t + 1/2 at*t

	// x0
	*vector=position0;

	// v0t
	(*vector)+=velocity*(float)time;
    
	// 1/2 at*t
	(*vector)+=.5f * acceleration * (float)(time * time);
}

// Sets loc to GetNextLoc based on time
void Projectile::Update(long time)
{
	D3DXVECTOR3 nextLoc;
	elapedTime+=time;
	GetNextLoc(elapedTime/1000.0f, &nextLoc);
	oMatrix._41=nextLoc.x; oMatrix._42=nextLoc.y; oMatrix._43=nextLoc.z;

}

void Projectile::Render(long time)
{
}

void Projectile::Start(void)
{
	position0.x=oMatrix._41;
	position0.y=oMatrix._42;
	position0.z=oMatrix._43;
    
	acceleration0=acceleration;
	velocity0=velocity;
	elapedTime=0;
}
