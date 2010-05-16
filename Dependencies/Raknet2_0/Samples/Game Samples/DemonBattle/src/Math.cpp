#include <math.h>
#include "Math.h"

//#include "MemoryManager.h"

// (y-i)^2 = b^2 * (1 - (((x-h)^2)/a^2) - (((z-j)^2)/c^2) )
// h,i,j is center of ellipse (For this function we'll assume i == 0)
// a,b,c is sqrt(length) into respective axis x,y,z
// x,y,z are positions (y returned by this function)

float EllipseHeight(float a, float b, float c, float h, float j, float x, float z)
{
	float value = b*b * (1 - (((x-h)*(x-h))/(a*a)) - (((z-j)*(z-j))/(c*c)) );
	if (value < 0)
		return 0.0f;
	else
		return (float)sqrt(value);
}

void LookAtMatrix(const D3DXVECTOR3 &from, const D3DXVECTOR3 &to, const D3DXVECTOR3 &up, D3DXMATRIX &m)
{
	//D3DXMATRIX m;
	//D3DXMatrixIdentity(&m);

	// Get the z basis vector, which points straight ahead. This is the
	// difference from the eyepoint to the lookat point.
	D3DXVECTOR3 vView = to - from;
	
	float fLength = D3DXVec3Length(&vView);
	if(fLength < 1e-6f) return;
	
	// Normalize the z basis vector
	vView /= fLength;
	
	// Get the dot product, and calculate the projection of the z basis
	// vector onto the up vector. The projection is the y basis vector.
	float fDotProduct = D3DXVec3Dot(&up,&vView);
	
	D3DXVECTOR3 vUp = up - vView * fDotProduct;
	
	// If this vector has near-zero length because the input specified a
	// bogus up vector, let's try a default up vector
	if(1e-6f > (fLength = D3DXVec3Length(&vUp)))
	{
		vUp = D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) - vView * vView.y;
		
		// If we still have near-zero length, resort to a different axis.
		if(1e-6f > (fLength = D3DXVec3Length(&vUp)))
		{
			vUp = D3DXVECTOR3( 0.0f, 0.0f, 1.0f ) - vView * vView.z;			
			if(1e-6f > (fLength = D3DXVec3Length(&vUp))) return;
		}
	}
	
	// Normalize the y basis vector
	vUp /= fLength;
	
	// The x basis vector is found simply with the cross product of the y
	// and z basis vectors
	D3DXVECTOR3 vRight;
	D3DXVec3Cross(&vRight, &vUp, &vView);
	
	// Start building the matrix. The first three rows contains the basis
	// vectors used to rotate the view to point at the lookat point
	m._11 = vRight.x;    m._21 = vUp.x;    m._31 = vView.x;
	m._12 = vRight.y;    m._22 = vUp.y;    m._32 = vView.y;
	m._13 = vRight.z;    m._23 = vUp.z;    m._33 = vView.z;

	
		// Start building the matrix. The first three rows contains the basis
	// vectors used to rotate the view to point at the lookat point
//	m.x.x = vRight.x;    m.y.x = vUp.x;    m.z.x = vView.x;
//	m.x.y = vRight.y;    m.y.y = vUp.y;    m.z.y = vView.y;
//	m.x.z = vRight.z;    m.y.z = vUp.z;    m.z.z = vView.z;

	
	// Do the translation values (rotations are still about the eyepoint)
//	m.w.x = - D3DXVec3Dot(&from, &vRight);
//	m.w.y = - D3DXVec3Dot(&from, &vUp);
//	m.w.z = - D3DXVec3Dot(&from, &vView);
}

D3DXMATRIX ProjectionMatrix(const float near_plane, // Distance to near clipping 
                                         // plane
                 const float far_plane,  // Distance to far clipping 
                                         // plane
                 const float fov_horiz,  // Horizontal field of view 
                                         // angle, in radians
                 const float fov_vert)   // Vertical field of view 
                                         // angle, in radians
{
    float    h, w, Q;
 
    w = (float)1/(float)tan(fov_horiz*0.5);  // 1/tan(x) == cot(x)
    h = (float)1/(float)tan(fov_vert*0.5);   // 1/tan(x) == cot(x)
    Q = far_plane/(far_plane - near_plane);
 
    D3DXMATRIX ret;
    ZeroMemory(&ret, sizeof(ret));

    ret(0, 0) = w;
    ret(1, 1) = h;
    ret(2, 2) = Q;
    ret(3, 2) = -Q*near_plane;
    ret(2, 3) = 1;
    return ret;
}   // End of ProjectionMatrix

bool RayPlaneIntersection(D3DXPLANE *plane, D3DXVECTOR3* out, D3DXVECTOR3 *rayOrg, D3DXVECTOR3 *rayDir)
{
	// To calculate this take the equation of a ray, which is r0 + rdt and plug it into the equation of a plane, Ax + By + Cd + D = 0
	// You get A(r0x + rdxt) + B(r0y + rdyt) + C(r0z + rdzt) + D = 0
	// = Ar0x + Ardxt + Br0y + Brdyt + Cr0z + Crdzt + D = 0
	// = Ar0x + Br0y + Cr0z + Ardxt + Brdyt + Crdzt + D = 0
	// = PLANE_NORMAL dot r0 + PLANE_NORMAL dot rd * t + D = 0
	// Now solve for t
	// PLANE_NORMAL dot rd * t = - (PLANE_NORMAL dot r0 + D)
	// t = - (PLANE_NORMAL dot r0 + D) / PLANE_NORMAL dot rd
	// given that PLANE_NORMAL dot rd !=0

	// Dot product of plane normal and ray direction
	float demoninator = D3DXVec3Dot(&D3DXVECTOR3(plane->a, plane->b, plane->c), rayDir);
	if (demoninator==0)
		return false; // No intersection point

	float numerator = -1.0f * (D3DXVec3Dot(&D3DXVECTOR3(plane->a, plane->b, plane->c), rayOrg) + plane->d);
	float distance = numerator / demoninator;
	// If distance < 0 then the ray direction has to be negative, which means that the ray origin is behind the plane
	if (distance < 0.0f)
		return false;

	// Otherwise the intersection point is the ray origin + ray direction * distance
	*out = *rayOrg + *rayDir * distance;

	return true;
}
