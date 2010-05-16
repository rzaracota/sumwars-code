#ifndef _BASE_3D_H
#define _BASE_3D_H

#pragma once

#include <windows.h>
#include <windowsx.h>
#include <D3d9types.h>
#include "D3DHeaders.h"
#include "vector"

	// The modellvertex for D3D
	struct MODELVERTEX 
	{
		D3DXVECTOR3   m_vecPos;
		D3DCOLOR      m_dwDiffuse;
		D3DXVECTOR2   m_vecTex;
	} ;

class Base3D
{
public:

	#define D3DFVF_MODELVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0) )

	struct SMesh
	{
		std::vector <MODELVERTEX> vertex;
	};

protected:
	long m_vertices, m_triangles;
};

#endif