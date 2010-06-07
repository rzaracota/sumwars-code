#ifndef __LIGHTED_SPHERE_H
#define __LIGHTED_SPHERE_H

//#include <D3d.h>
#include "VisibleEntity.h"
#include "LightSource.h"

#define LIGHTED_SPHERE_RADIUS 10.0f

class LightedSphere : public VisibleEntity
{
public:
	LightedSphere();
	~LightedSphere();
	void Load(void);
	void Unload(void);
	void Render(long time);
	void Update(long time);
	void Init(void);
	void Deinit(void);
protected:
	float impactPointLightTimer;
	LPD3DXMESH m_pSphereMesh;          // Representation of point light
	LightSource *impactPointLight;
};

#endif