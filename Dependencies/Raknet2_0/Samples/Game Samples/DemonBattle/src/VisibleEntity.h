#ifndef __VISIBLE_ENTITY_H
#define __VISIBLE_ENTITY_H

#include "Entity.h"
#include "Base3D.h"

class VisibleEntity : public Entity
{
	public:
	VisibleEntity();
	virtual ~VisibleEntity();
	virtual void Render(long time);
	virtual void SetLocation(float x, float y, float z) {oMatrix._41=x; oMatrix._42=y; oMatrix._43=z;}
	virtual void Update(long time)=0;
	virtual void SetMaterial(float r, float g, float b, float a);
	virtual void SetRotationYawPitchRoll(float yaw, float pitch, float roll);
	virtual void LookAt(const D3DXVECTOR3& lookAt);

	protected:
	void MultiplyInScale(void);
	float scale;

	// The material properties for this object
	D3DMATERIAL9 mat;

};

#endif