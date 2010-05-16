#ifndef __D3DHEADERS_H
#define __D3DHEADERS_H

#include <windows.h>
#include <D3d9types.h>
#include <D3dx9math.h>

const FLOAT PI           =  3.14159265358979323846f; // Pi
const FLOAT PI_MUL_2     =  6.28318530717958623200f; // 2 * Pi
const FLOAT PI_DIV_2     =  1.57079632679489655800f; // Pi / 2
const FLOAT PI_DIV_4     =  0.78539816339744827900f; // Pi / 4


typedef struct {
   D3DXVECTOR3 vMin;       // Kleinster Eckpunkt der Box
   D3DXVECTOR3 vMax;       // Grösster Eckpunkt der Box
   D3DXVECTOR3 vMittelpkt; // Mittelpunkt (Zentrum) der Box
   D3DXVECTOR3 vPos;	   //Muss per Hand gesetzt werden
} SBoundingBox;


// Vertex Struktur (untransformiert, unbeleuchtet)
struct D3DVERTEX
{
	D3DVERTEX () {vN.x=vN.y=vN.z=0;};
	D3DVERTEX (D3DXVECTOR3& pos) 
	{
		x=pos.x;y=pos.y;z=pos.z;
	}
   float     x;  // D3DFVF_XYZ
   float     y; 
   float     z; 
   D3DVECTOR vN; // D3DFVF_NORMAL
   float     tu; // D3DFVF_TEX1
   float     tv;
} ;

//Vertex Struktur (beleuchtet, transformiert)
struct D3DTLVERTEX
{
	float x, y,z ,rhw;
	DWORD color;
	float tu,tv;
};

#define D3DFVF_D3DTLVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE| D3DFVF_TEX1)
#define D3DFVF_VERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

#endif
