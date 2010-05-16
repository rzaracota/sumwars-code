#ifndef __ENTITY_H
#define __ENTITY_H

#include <D3dx9math.h>

class Entity
{
public:
	Entity() {D3DXMatrixIdentity(&oMatrix);}
	virtual ~Entity() {}
	virtual void SetLocation(float x, float y, float z) {oMatrix._41=x; oMatrix._42=y; oMatrix._43=z;}
	virtual void GetLocation(float &x, float &y, float &z) const {x=oMatrix._41; y=oMatrix._42; z=oMatrix._43;}
	virtual float GetX(void) const {return oMatrix._41;}
	virtual float GetY(void) const {return oMatrix._42;}
	virtual float GetZ(void) const {return oMatrix._43;}
protected:
		// public for easy access
	D3DXMATRIX oMatrix;
};

#endif