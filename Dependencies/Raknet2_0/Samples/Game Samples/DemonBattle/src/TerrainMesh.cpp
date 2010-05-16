#include "TerrainMesh.h"
#include <assert.h>
#include "Math.h"
#include "rand.h"
#include "ModelContainer.h"
#include "..\Common\Include\d3dutil.h"
#include "User.h"

const DWORD COLORVERTEX::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;

//#include "MemoryManager.h"

TerrainMesh::TerrainMesh()
{

    m_dwNumGroundVertices  = (GROUND_GRIDSIZE + 1) * (GROUND_GRIDSIZE + 1);
    m_dwNumGroundIndices   = (GROUND_GRIDSIZE * GROUND_GRIDSIZE) * 6;
    m_pGroundVB            = NULL;
    m_pGroundIB            = NULL;
    m_planeGround          = D3DXPLANE( 0.0f, 1.0f, 0.0f, 0.0f );

	// Set the material
//	D3DUtil_InitMaterial (mat, .6f,.6f,.6f,.6f);
	
	D3DUtil_InitMaterial (mat, 1,1,1,1);
	/*
	mat.Diffuse.r=.5;
	mat.Diffuse.g=.5;
	mat.Diffuse.b=.5;
	mat.Diffuse.a=.5;
		*/
	mat.Ambient.r=.6f;
	mat.Ambient.g=.6f;
	mat.Ambient.b=.6f;
	mat.Ambient.a=.6f;

	mat.Diffuse.r = mat.Specular.r = 1.0f;
    mat.Diffuse.g = mat.Specular.g = 1.0f;
    mat.Diffuse.b = mat.Specular.b = 1.0f;
    mat.Diffuse.a = mat.Specular.b = 1.0f;
}

TerrainMesh::~TerrainMesh()
{
	Destroy();
}

HRESULT TerrainMesh::Generate(void)
{
	HRESULT hr;

    // Create the index buffer
    WORD* pIndices, *pIndicesI;
    hr = m_pd3dDevice->CreateIndexBuffer( m_dwNumGroundIndices*sizeof(WORD),
                                            D3DUSAGE_WRITEONLY,
                                            D3DFMT_INDEX16, D3DPOOL_MANAGED,
                                            &m_pGroundIB, NULL);
    if( FAILED(hr) )
        return E_FAIL;

    // Fill the index buffer
    m_pGroundIB->Lock( 0, m_dwNumGroundIndices*sizeof(WORD), (void**)&pIndices, 0 );
    if( FAILED(hr) )
        return E_FAIL;

	pIndicesI=pIndices;

    // Fill in indices
    for( DWORD z = 0; z < GROUND_GRIDSIZE; z++ )
    {
        for( DWORD x = 0; x < GROUND_GRIDSIZE; x++ )
        {
            DWORD vtx = x + z * (GROUND_GRIDSIZE+1);
			/*
            *pIndices++ = (WORD)( vtx + 1 );
            *pIndices++ = (WORD)( vtx + 0 );
            *pIndices++ = (WORD)( vtx + 0 + (GROUND_GRIDSIZE+1) );
            *pIndices++ = (WORD)( vtx + 1 );
            *pIndices++ = (WORD)( vtx + 0 + (GROUND_GRIDSIZE+1) );
            *pIndices++ = (WORD)( vtx + 1 + (GROUND_GRIDSIZE+1) );
			*/
			*pIndicesI++ = (WORD)( vtx + 1 );
            *pIndicesI++ = (WORD)( vtx + 0 + (GROUND_GRIDSIZE+1) );
			*pIndicesI++ = (WORD)( vtx + 0 );
            
            *pIndicesI++ = (WORD)( vtx + 1 );
			*pIndicesI++ = (WORD)( vtx + 1 + (GROUND_GRIDSIZE+1) );
            *pIndicesI++ = (WORD)( vtx + 0 + (GROUND_GRIDSIZE+1) );
            
        }
    }

// Create vertex buffer for ground object
	/*
    hr = m_pd3dDevice->CreateVertexBuffer( m_dwNumGroundVertices*sizeof(COLORVERTEX),
                                            D3DUSAGE_WRITEONLY, D3DFVF_COLORVERTEX,
                                            D3DPOOL_MANAGED, &m_pGroundVB );
											*/

	 hr = m_pd3dDevice->CreateVertexBuffer( m_dwNumGroundVertices*sizeof(COLORVERTEX),
                                               D3DUSAGE_WRITEONLY, COLORVERTEX::FVF,
                                               D3DPOOL_MANAGED, &m_pGroundVB, NULL );

    if( FAILED(hr) )
        return E_FAIL;

    // Fill vertex buffer
    COLORVERTEX* pVertices, *pVerticesI;
    if( FAILED( m_pGroundVB->Lock( 0, 0, (void**)&pVertices, NULL ) ) )
        return hr;

	// Do this so we can maintain the original value of pVertices while still using ++ to change the pointer
	pVerticesI=pVertices;

    // Fill in vertices
    for( DWORD zz = 0; zz <= GROUND_GRIDSIZE; zz++ )
    {
        for( DWORD xx = 0; xx <= GROUND_GRIDSIZE; xx++ )
        {
            pVerticesI->v.x   = GROUND_WIDTH * (xx/(FLOAT)GROUND_GRIDSIZE-0.5f);
            pVerticesI->v.y   = 0.0f;
            pVerticesI->v.z   = GROUND_HEIGHT * (zz/(FLOAT)GROUND_GRIDSIZE-0.5f);
            pVerticesI->color = GROUND_COLOR;
            pVerticesI->tu    = xx*GROUND_TILE/(FLOAT)GROUND_GRIDSIZE;
            pVerticesI->tv    = zz*GROUND_TILE/(FLOAT)GROUND_GRIDSIZE;
			pVerticesI->normal = D3DXVECTOR3(0.0f,1.0f,0.0f);
            pVerticesI++;
        }
    }

	// Not necessary with flat ground.  Uncomment if I make the ground unflat later
	/*
	// Generate the face normals so we can get the vertex normals
	D3DXVECTOR3 faceNormal;
	assert(m_dwNumGroundIndices % 3 == 0); // Evenly divisible

	// Go through every 3 indices and get the face normal for the triangle it forms
	for (int i=0; i < (int)m_dwNumGroundIndices/3; i++)
	{
		D3DXVECTOR3 a,b,c, bma, cma;
		a = pVertices[pIndices[i*3 + 0]].v;
		b = pVertices[pIndices[i*3 + 1]].v;
		c = pVertices[pIndices[i*3 + 2]].v;
		bma = b - a;
		cma = c - a;

		// This does faceNormals[i] = Cross(b-a,c-a); where a,b,c are the vertices for that face
		D3DXVec3Cross(&faceNormal, &cma, &bma);

		// For each vertex comprising this face...
		for(int j=0;j<3;j++)
		{
			// ...get position of this vertex...
			D3DXVECTOR3 vloc = pVertices[pIndices[i*3+j]].v;
			// ... and go through the list of vertices.  If the locations match then add this face normal
			for(int ii2=0;ii2<(int)m_dwNumGroundVertices;ii2++)
			{
				if(pVertices[ii2].v == vloc)
					pVertices[ii2].normal += faceNormal;
			}
		}
	}

	// go through all the faces again, if any vertex is facing opposite its polygon or its zero make it equal to the polygon
	for (int i=0; i < (int)m_dwNumGroundIndices/3; i++)
	{
		D3DXVECTOR3 a,b,c, bma, cma;
		a = pVertices[pIndices[i*3 + 0]].v;
		b = pVertices[pIndices[i*3 + 1]].v;
		c = pVertices[pIndices[i*3 + 2]].v;
		bma = b - a;
		cma = c - a;

		// This does faceNormals[i] = Cross(b-a,c-a); where a,b,c are the vertices for that face
		D3DXVec3Cross(&faceNormal, &cma, &bma);

		if(D3DXVec3LengthSq(&pVertices[pIndices[i*3 + 0]].v) < 0.0001f) pVertices[pIndices[i*3 + 0]].normal = faceNormal;
		if(D3DXVec3LengthSq(&pVertices[pIndices[i*3 + 1]].v) < 0.0001f) pVertices[pIndices[i*3 + 0]].normal = faceNormal;
		if(D3DXVec3LengthSq(&pVertices[pIndices[i*3 + 2]].v) < 0.0001f) pVertices[pIndices[i*3 + 0]].normal = faceNormal;
		if(D3DXVec3Dot(&pVertices[pIndices[i*3 + 0]].normal,&faceNormal) < 0) pVertices[pIndices[i*3 + 0]].normal = faceNormal;
		if(D3DXVec3Dot(&pVertices[pIndices[i*3 + 1]].normal,&faceNormal) < 0) pVertices[pIndices[i*3 + 0]].normal = faceNormal;
		if(D3DXVec3Dot(&pVertices[pIndices[i*3 + 2]].normal,&faceNormal) < 0) pVertices[pIndices[i*3 + 0]].normal = faceNormal;
	}

	// normalize every vertex
	for (int i=0; i < (int)m_dwNumGroundVertices; i++)
	{
		D3DXVec3Normalize(&pVertices[i].normal, &pVertices[i].normal);			
	}
	*/

    m_pGroundIB->Unlock();
    m_pGroundVB->Unlock();


	return S_OK;
}

void TerrainMesh::Destroy(void)
{
    SAFE_RELEASE( m_pGroundVB );
    SAFE_RELEASE( m_pGroundIB );
}

void TerrainMesh::Render(BOOL m_bDrawReflection)
{
	// Set the world matrix
    D3DXMATRIX matWorld;
    D3DXMatrixIdentity( &matWorld );
    m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	m_pd3dDevice->SetMaterial (&mat);

  // Draw the ground
    if( m_bDrawReflection )
    {
        m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
        m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
    }


	/*
    //m_pd3dDevice->SetTransform( D3DTS_VIEW, &m_matView );
	m_pd3dDevice->SetTexture( 0, modelContainer->textures[TERRAIN_TEXTURE_1] );
    m_pd3dDevice->SetFVF( D3DFVF_COLORVERTEX );
    m_pd3dDevice->SetStreamSource( 0, m_pGroundVB, sizeof(COLORVERTEX) );
    m_pd3dDevice->SetIndices( m_pGroundIB, 0L );
    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, m_dwNumGroundVertices, 0, m_dwNumGroundIndices/3 );
	*/

	m_pd3dDevice->SetTransform( D3DTS_VIEW, user->camera.GetViewMatrix() );
    m_pd3dDevice->SetTexture( 0, modelContainer->textures[TERRAIN_TEXTURE_1] );
    m_pd3dDevice->SetFVF( COLORVERTEX::FVF );
    m_pd3dDevice->SetStreamSource( 0, m_pGroundVB, 0, sizeof(COLORVERTEX) );
    m_pd3dDevice->SetIndices( m_pGroundIB );
    m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_dwNumGroundVertices, 0, m_dwNumGroundIndices/3 );
}

int TerrainMesh::GetHeight(float x, float z)
{
	return 0;
}