#ifndef _TERRAIN_MESH_H
#define _TERRAIN_MESH_H

#include "Base3D.h"
#include <D3dx9math.h>
#include "DXUtil.h"

/*
struct COLORVERTEX
{
    D3DXVECTOR3 v;
	D3DXVECTOR3 normal;
    D3DCOLOR    color;
    FLOAT       tu;
    FLOAT       tv;
};


#define D3DFVF_COLORVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
*/
struct COLORVERTEX
{
    D3DXVECTOR3 v;
	D3DXVECTOR3 normal;
    D3DCOLOR    color;
    FLOAT       tu;
    FLOAT       tv;

    static const DWORD FVF;
};


//-----------------------------------------------------------------------------
// Global structs and data for the ground object
//-----------------------------------------------------------------------------
#define GROUND_WIDTH     2200.0f
#define GROUND_HEIGHT    2200.0f
#define GROUND_GRIDSIZE  40 // How many squares to have, not the size of each square
#define GROUND_TILE      1
#define GROUND_COLOR     0xcccccccc


#define TEAM_1_CASTLE_POSITION (GROUND_WIDTH/2 - 100)
#define TEAM_2_CASTLE_POSITION (-(GROUND_WIDTH/2 - 100))
#define TEAM_1_CASTLE_HEIGHT 0.0f
#define TEAM_2_CASTLE_HEIGHT 0.0f

// Even rows have (squares + 1) * 2 vertices
// Odd rows have (squares + 1) * 2 + 1 vertices, the one extra vertex is to go 'down' to reach the row below you

class TerrainMesh : public Base3D
{
public:
	TerrainMesh();
	~TerrainMesh();
	HRESULT Generate(void);
	void Destroy(void);
	void Render(BOOL m_bDrawReflection=false);
	int GetHeight(float x, float z);

private:
	float lowestPoint;
	float highestPoint;
	float GetAlpha(float y) {return .5f;/*(y+lowestPoint) / (highestPoint+lowestPoint);*/}

	D3DXPLANE          m_planeGround;

    LPDIRECT3DVERTEXBUFFER9 m_pGroundVB;
    LPDIRECT3DINDEXBUFFER9  m_pGroundIB;
    DWORD              m_dwNumGroundIndices;
    DWORD              m_dwNumGroundVertices;
	D3DMATERIAL9 mat;
};

#endif