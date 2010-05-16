#ifndef __MATH_H
#define __MATH_H

#include <windows.h>
#include <D3dx9math.h>

extern float EllipseHeight(float a, float b, float c, float h, float j, float x, float z);
void LookAtMatrix(const D3DXVECTOR3 &from, const D3DXVECTOR3 &to, const D3DXVECTOR3 &up, D3DXMATRIX &m);
D3DXMATRIX ProjectionMatrix(const float near_plane, const float far_plane, const float fov_horiz, const float fov_vert);

// Returns the intersection of the given ray and plane.  Returns false if coplanar or negative distance, true and the output in out otherwise
bool RayPlaneIntersection(D3DXPLANE *plane, D3DXVECTOR3* out, D3DXVECTOR3 *rayOrg, D3DXVECTOR3 *rayDir);

#endif