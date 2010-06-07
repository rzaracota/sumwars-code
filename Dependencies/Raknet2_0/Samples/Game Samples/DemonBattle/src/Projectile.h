#ifndef __PROJECTILE_H
#define __PROJECTILE_H

#include <D3dx9math.h>
#include "FileLoadedVisibleEntity.h"


class Projectile : public Entity
{
public:
	Projectile();
	virtual ~Projectile();
	virtual void Start(void);
	virtual void GetNextLoc(float time, D3DXVECTOR3* vector);
	virtual void Update(long time); // Sets loc to GetNextLoc based on time
	virtual void Render(long time); // calls FileLoadedVisibleEntity::Render
	virtual void SetVelocity(D3DXVECTOR3 &Velocity) {velocity=Velocity;}
	virtual void SetAcceleration(D3DXVECTOR3 &Acceleration) {acceleration=Acceleration;}
	virtual D3DXVECTOR3& GetVelocity(void) {return velocity;}
	virtual D3DXVECTOR3& GetAcceleration(void) {return acceleration;}
protected:
	D3DXVECTOR3 velocity, acceleration, velocity0, acceleration0, position0;
	int elapedTime;
};

#endif