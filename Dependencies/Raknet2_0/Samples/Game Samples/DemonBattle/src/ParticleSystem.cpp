#include "ParticleSystem.h"
#include "ModelContainer.h"
#include "..\Common\Include\d3dutil.h"
#include <assert.h>
#include "rand.h"
#include "TextManager.h"

//#include "MemoryManager.h"

static const float TIME_TO_REACH_END_OF_TAIL=.5f;

const DWORD POINTVERTEX::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	
	D3DXCOLOR ParticleSystem::g_clrColor[NUM_COLORS] =
	{
		D3DXCOLOR( 1.0f,   1.0f,   1.0f,   1.0f ),
		D3DXCOLOR( 1.0f,   0.5f,   0.5f,   .5f ),
		D3DXCOLOR( 0.5f,   1.0f,   0.5f,   1.0f ),
		D3DXCOLOR( 0.125f, 0.5f,   1.0f,   1.0f ),
		D3DXCOLOR( 1.0f, .95f,   .1f,   1.0f )
	};


	DWORD ParticleSystem::g_clrColorFade[NUM_COLORS] =
	{
		D3DXCOLOR( 1.0f,   0.25f,   0.25f,   1.0f ),
		D3DXCOLOR( 1.0f,   0.25f,   0.25f,   1.0f ),
		D3DXCOLOR( 0.25f,  0.75f,   0.25f,   1.0f ),
		D3DXCOLOR( 0.125f, 0.25f,   0.75f,   1.0f ),
		D3DXCOLOR( .5f, .48f,   .05f,   1.0f )
	};


//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
ParticleSystem::ParticleSystem( DWORD dwFlush, DWORD dwDiscard)
{
	m_dwBase         = dwDiscard;
    m_dwFlush        = dwFlush;
	m_dwDiscard      = dwDiscard;

    m_dwParticles    = 0;
    m_dwParticlesLim = 2048;

    m_pParticles     = NULL;
    m_pParticlesFree = NULL;
	m_pVB            = NULL;
	minHeight=0.0f;
	fTime = 0.0f;
}

void ParticleSystem::StartParticle(const D3DXCOLOR &clrEmitColor, const D3DXCOLOR &clrFadeColor, D3DXVECTOR3 vPosition, D3DXVECTOR3 velocity, float MinHeight)
{
	minHeight = MinHeight;
	while( m_pParticles )
    {
        PARTICLE* pSpark = m_pParticles;
        m_pParticles = pSpark->m_pNext;
        delete pSpark;
    }

    while( m_pParticlesFree )
    {
        PARTICLE *pSpark = m_pParticlesFree;
        m_pParticlesFree = pSpark->m_pNext;
        delete pSpark;
    }

	PARTICLE *pParticle;
	pParticle = new PARTICLE;
	pParticle->m_pNext = 0;
	m_pParticles = pParticle;
	m_dwParticles++;

	pParticle->m_bSpark = FALSE;

	pParticle->m_vPos0 = vPosition;
	pParticle->m_vVel0 = velocity;
	pParticle->m_vPos = vPosition;
	pParticle->m_vVel = velocity;

	pParticle->m_clrDiffuse = clrEmitColor;
	pParticle->m_clrFade    = clrFadeColor;

	pParticle->m_fFade      = 1.0f;
	pParticle->m_fTime0     = 0;

	fTime = 0.0f;

}

//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
ParticleSystem::~ParticleSystem()
{
	InvalidateDeviceObjects();

    while( m_pParticles )
    {
        PARTICLE* pSpark = m_pParticles;
        m_pParticles = pSpark->m_pNext;
        delete pSpark;
    }

    while( m_pParticlesFree )
    {
        PARTICLE *pSpark = m_pParticlesFree;
        m_pParticlesFree = pSpark->m_pNext;
        delete pSpark;
    }

}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT ParticleSystem::RestoreDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice )
{
    HRESULT hr;

    // Create a vertex buffer for the particle system.  The size of this buffer
    // does not relate to the number of particles that exist.  Rather, the
    // buffer is used as a communication channel with the device.. we fill in 
    // a bit, and tell the device to draw.  While the device is drawing, we
    // fill in the next bit using NOOVERWRITE.  We continue doing this until 
    // we run out of vertex buffer space, and are forced to DISCARD the buffer
    // and start over at the beginning.

    if(FAILED(hr = pd3dDevice->CreateVertexBuffer( m_dwDiscard * 
		sizeof(POINTVERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS, 
        POINTVERTEX::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL )))
	{
        return E_FAIL;
	}

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT ParticleSystem::InvalidateDeviceObjects()
{
	if (m_pVB)
	{
		m_pVB->Release();
		m_pVB=0;
	}

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT ParticleSystem::Update( FLOAT fSecsPerFrame, DWORD dwNumParticlesToEmit,
                                 const D3DXCOLOR &clrEmitColor,
                                 const D3DXCOLOR &clrFadeColor)
{
   PARTICLE *pParticle, **ppParticle;
    fTime += fSecsPerFrame;

    ppParticle = &m_pParticles;

    while( *ppParticle )
    {
        pParticle = *ppParticle;

        // Calculate new position
        float fT = fTime - pParticle->m_fTime0;
        float fGravity;

        if( pParticle->m_bSpark )
        {
            //fGravity = GRAVITY/20;
			fGravity = GRAVITY*.9;
			//fGravity = 0;
			//pParticle->m_fFade -= fSecsPerFrame * 2.25f;
			pParticle->m_fFade -= fSecsPerFrame * frandomMT() * .5f;//(.05f + frandomMT()*.1f);
        }
        else
        {
			 fGravity = GRAVITY;
            //pParticle->m_fFade -= fSecsPerFrame * 0.25f;
        }

        pParticle->m_vPos    = pParticle->m_vVel0 * fT + pParticle->m_vPos0;
        pParticle->m_vPos.y += (0.5f * fGravity) * (fT * fT);
        pParticle->m_vVel.y  = pParticle->m_vVel0.y + fGravity * fT;

	//	textManager->ShowMessage(-1, "particle position = %f %f %f", pParticle->m_vPos.x, pParticle->m_vPos.y, pParticle->m_vPos.z);

        if( pParticle->m_fFade < 0.0f )
            pParticle->m_fFade = 0.0f;

		// Emit sparks
        if( !pParticle->m_bSpark)
        {
            //  for( int i=0; i<4; i++ )
				for( int i=0; i<2; i++ )
            {
                PARTICLE *pSpark;

                if( m_pParticlesFree )
                {
                    pSpark = m_pParticlesFree;
                    m_pParticlesFree = pSpark->m_pNext;
                }
                else
                {
                    if( NULL == ( pSpark = new PARTICLE ) )
                        return E_OUTOFMEMORY;
                }

                pSpark->m_pNext = pParticle->m_pNext;
                pParticle->m_pNext = pSpark;

                pSpark->m_bSpark  = TRUE;
                pSpark->m_vPos0   = pParticle->m_vPos;
				pSpark->m_vVel0 = pParticle->m_vVel;

				float fT = (float)(fTime - pParticle->m_fTime0 - (.1f +frandomMT()*.8f) * TIME_TO_REACH_END_OF_TAIL);
				
				pSpark->m_vPos0 = pParticle->m_vVel0 * (fTime - pParticle->m_fTime0) + pParticle->m_vPos0;
				pSpark->m_vPos0.y += (0.5f * GRAVITY) * (fT * fT);

				// Doesn't work!
				/*
				// Rotate the angle randomly
				D3DXVECTOR4 result = pSpark->m_vVel0;
				result.w=0;
				D3DXVec4Normalize(&result, &result); // Vector in the direction of the fireball

				D3DXMATRIX ypr;
				
				D3DXMatrixRotationYawPitchRoll(&ypr, 3.0f*D3DX_PI/4.0f + frandomMT()* D3DX_PI/2.0f, 3.0f*D3DX_PI/4.0f + frandomMT()* D3DX_PI/2.0f,3.0f*D3DX_PI/4.0f + frandomMT()* D3DX_PI/2.0f);
				D3DXVec4Transform(&result, &result, &ypr); // Length 1 vector generally in the opposite direction of the fireball
				

				// Add some bit in the opposite direction of the fireball to the velocity
				pSpark->m_vVel0.x +=result.x * frandomMT() * 10.0f;
				pSpark->m_vVel0.y +=result.y * frandomMT() * 10.0f;
				pSpark->m_vVel0.z +=result.z * frandomMT() * 10.0f;
				*/


				// Flip the vectors from 3/4 PI to that plus PI/2
				D3DXVECTOR3 direction;
				float length = D3DXVec3Length(&pSpark->m_vVel0);
				direction.x = pSpark->m_vVel0.x / length;
				direction.y = pSpark->m_vVel0.y / length;
				direction.z = pSpark->m_vVel0.z / length;
				
				pSpark->m_vVel0.x += (float)(sin(asin(direction.x) + 3.0f*D3DX_PI/4.0f + frandomMT()* D3DX_PI/2.0f) * frandomMT() * length * .2f);
				pSpark->m_vVel0.y += (float)(sin(asin(direction.y) + 3.0f*D3DX_PI/4.0f + frandomMT()* D3DX_PI/2.0f) * frandomMT() * length * .2f);
				pSpark->m_vVel0.z += (float)(sin(asin(direction.z) + 3.0f*D3DX_PI/4.0f + frandomMT()* D3DX_PI/2.0f) * frandomMT() * length * .2f);


                D3DXColorLerp( &pSpark->m_clrDiffuse, &pParticle->m_clrFade,
                                &pParticle->m_clrDiffuse, pParticle->m_fFade );

				pSpark->m_vVel = pParticle->m_vVel0;
				pSpark->m_vPos = pParticle->m_vPos0;
                pSpark->m_clrFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
                pSpark->m_fFade   = 1.0f;
                pSpark->m_fTime0  = fTime;
            }
        }

        // Kill old particles
//        if(( pParticle->m_vPos.y < m_fRadius || pParticle->m_bSpark) && pParticle->m_fFade <= 0.0f )
		if(pParticle->m_fFade <= 0.0f || pParticle->m_vPos.y < minHeight)
        {
            

            // Kill particle
            *ppParticle = pParticle->m_pNext;
            pParticle->m_pNext = m_pParticlesFree;
            m_pParticlesFree = pParticle;

            if(!pParticle->m_bSpark)
                m_dwParticles--;
        }
        else
        {
            ppParticle = &pParticle->m_pNext;
        }
    }

	/*
    // Emit new particles
    DWORD dwParticlesEmit = m_dwParticles + dwNumParticlesToEmit;
    while( m_dwParticles < m_dwParticlesLim && m_dwParticles < dwParticlesEmit )
    {
        if( m_pParticlesFree )
        {
            pParticle = m_pParticlesFree;
            m_pParticlesFree = pParticle->m_pNext;
        }
        else
        {
            if( NULL == ( pParticle = new PARTICLE ) )
                return E_OUTOFMEMORY;
        }

        pParticle->m_pNext = m_pParticles;
        m_pParticles = pParticle;
        m_dwParticles++;

        // Emit new particle
        FLOAT fRand1 = ((FLOAT)rand()/(FLOAT)RAND_MAX) * D3DX_PI * 2.0f;
        FLOAT fRand2 = ((FLOAT)rand()/(FLOAT)RAND_MAX) * D3DX_PI * 0.25f;

        pParticle->m_bSpark = FALSE;

        pParticle->m_vPos0 = vPosition;

        pParticle->m_vVel0.x  = cosf(fRand1) * sinf(fRand2) * 2.5f;
        pParticle->m_vVel0.z  = sinf(fRand1) * sinf(fRand2) * 2.5f;
        pParticle->m_vVel0.y  = cosf(fRand1) * cosf(fRand2) * 2.5f;
        //pParticle->m_vVel0.y *= ((FLOAT)rand()/(FLOAT)RAND_MAX) * fEmitVel;

        pParticle->m_vPos = pParticle->m_vPos0;
        pParticle->m_vVel = pParticle->m_vVel0;

        pParticle->m_clrDiffuse = clrEmitColor;
        pParticle->m_clrFade    = clrFadeColor;
        pParticle->m_fFade      = 1.0f;
        pParticle->m_fTime0     = fTime;
    }
	*/

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Renders the particle system using either pointsprites (if supported)
//       or using 4 vertices per particle
//-----------------------------------------------------------------------------
HRESULT ParticleSystem::Render( LPDIRECT3DDEVICE9 pd3dDevice )
{
	 HRESULT hr;
/*
    // Set the render states for using point sprites
    pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, TRUE );
    pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  TRUE );
   // pd3dDevice->SetRenderState( D3DRS_POINTSIZE,     FtoDW(1.00f) );
    pd3dDevice->SetRenderState( D3DRS_POINTSIZE_MIN, FtoDW(0.00f) );
    pd3dDevice->SetRenderState( D3DRS_POINTSCALE_A,  FtoDW(0.00f) );
    pd3dDevice->SetRenderState( D3DRS_POINTSCALE_B,  FtoDW(0.0f) );
    pd3dDevice->SetRenderState( D3DRS_POINTSCALE_C,  FtoDW(1.0f) );

    // Set up the vertex buffer to be rendered
    pd3dDevice->SetStreamSource( 0, m_pVB, sizeof(POINTVERTEX) );
    pd3dDevice->SetFVF( D3DFVF_POINTVERTEX );

    PARTICLE*    pParticle = m_pParticles;
    POINTVERTEX* pVertices;
    DWORD        dwNumParticlesToRender = 0;
*/
	// Set the render states for using point sprites
    pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, TRUE );
    pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  TRUE );
    pd3dDevice->SetRenderState( D3DRS_POINTSIZE,     FtoDW(1.00f) );
    pd3dDevice->SetRenderState( D3DRS_POINTSIZE_MIN, FtoDW(0.00f) );
    pd3dDevice->SetRenderState( D3DRS_POINTSCALE_A,  FtoDW(0.00f) );
    pd3dDevice->SetRenderState( D3DRS_POINTSCALE_B,  FtoDW(0.00f) );
    pd3dDevice->SetRenderState( D3DRS_POINTSCALE_C,  FtoDW(1.00f) );

    // Set up the vertex buffer to be rendered
    pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(POINTVERTEX) );
    pd3dDevice->SetFVF( POINTVERTEX::FVF );

    PARTICLE*    pParticle = m_pParticles;
    POINTVERTEX* pVertices;
    DWORD        dwNumParticlesToRender = 0;

	// Lock the vertex buffer.  We fill the vertex buffer in small
	// chunks, using D3DLOCK_NOOVERWRITE.  When we are done filling
	// each chunk, we call DrawPrim, and lock the next chunk.  When
	// we run out of space in the vertex buffer, we start over at
	// the beginning, using D3DLOCK_DISCARD.

	m_dwBase += m_dwFlush;

	if(m_dwBase >= m_dwDiscard)
		m_dwBase = 0;

	
		if( FAILED( hr = m_pVB->Lock( m_dwBase * sizeof(POINTVERTEX), m_dwFlush * sizeof(POINTVERTEX),
		(void**) &pVertices, m_dwBase ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD ) ) )
    {
		return hr;
    }


    // Render each particle
    while( pParticle )
    {
		
        D3DXVECTOR3 vPos;
        D3DXVECTOR3 vVel;		
		D3DXCOLOR clrDiffuse;
        UINT        dwSteps;

		float timeMod=0.0f;

		vPos = pParticle->m_vPos;

		if (pParticle->m_bSpark==false)
		{
			vVel = pParticle->m_vVel;
			dwSteps=30;

			D3DXColorLerp(&clrDiffuse, &pParticle->m_clrDiffuse, &pParticle->m_clrFade, pParticle->m_fFade);
		}
		else
		{
			vVel = pParticle->m_vVel;
			vVel.x/=80.0f;
			vVel.y/=80.0f;
			vVel.z/=80.0f;
			FLOAT fLength = D3DXVec3LengthSq(&vVel);
			
			if( fLength < 1.0f )        dwSteps = 2;
			else if( fLength <  4.00f ) dwSteps = 3;
			else if( fLength <  9.00f ) dwSteps = 4;
			else if( fLength < 12.25f ) dwSteps = 5;
			else if( fLength < 16.00f ) dwSteps = 6;
			else if( fLength < 20.25f ) dwSteps = 7;
			else                          dwSteps = 8;

			vVel *= -0.04f / (FLOAT)dwSteps;
			D3DXColorLerp(&clrDiffuse, &pParticle->m_clrDiffuse, &pParticle->m_clrFade, (pParticle->m_vPos0.y - pParticle->m_vPos.y) / pParticle->m_vPos0.y);
			
//
//			D3DXColorLerp(&clrDiffuse, &pParticle->m_clrFade, &pParticle->m_clrDiffuse, pParticle->m_fFade);
		}

		// FOr some fucking reason this code won't work inside the for loop
		//pParticle->m_clrDiffuse=g_clrColor[randomMT() % NUM_COLORS];

//		textManager->ShowMessage(-1, "%f", );
		
		/*
		if (pParticle->m_bSpark)
			pd3dDevice->SetRenderState( D3DRS_POINTSIZE,     FtoDW(1.0f) );
		else
			//pd3dDevice->SetRenderState( D3DRS_POINTSIZE,     FtoDW(sqrt((dwSteps-i)*4)));
			pd3dDevice->SetRenderState( D3DRS_POINTSIZE,     FtoDW(10));
*/

        // Render each particle a bunch of times to get a blurring effect
        for( DWORD i = 0; i < dwSteps; i++ )
        {
		//	if (pParticle->m_bSpark==false)
		//		D3DXColorLerp(&clrDiffuse, &pParticle->m_clrFade, &pParticle->m_clrDiffuse, 1.0f - i/dwSteps);

		//	clrDiffuse=pParticle->m_clrDiffuse;

            pVertices->v     = vPos;
            pVertices->color = (DWORD) clrDiffuse;
            pVertices++;

			if (pParticle->m_bSpark==false)
				pd3dDevice->SetRenderState( D3DRS_POINTSIZE,     FtoDW((float)sqrt((dwSteps-i)*4.0f)));
				
            //  if( pParticle->m_bSpark==false || ++dwNumParticlesToRender == m_dwFlush)
			if(++dwNumParticlesToRender == m_dwFlush || pParticle->m_bSpark==false)
		//	  if( ++dwNumParticlesToRender == 1)
            {
                // Done filling this chunk of the vertex buffer.  Lets unlock and
                // draw this portion so we can begin filling the next chunk.

                m_pVB->Unlock();

			//	if (pParticle->m_bSpark)
                if(FAILED(hr = pd3dDevice->DrawPrimitive( D3DPT_POINTLIST, m_dwBase, dwNumParticlesToRender)))
					return hr;

                // Lock the next chunk of the vertex buffer.  If we are at the 
                // end of the vertex buffer, DISCARD the vertex buffer and start
                // at the beginning.  Otherwise, specify NOOVERWRITE, so we can
                // continue filling the VB while the previous chunk is drawing.
				m_dwBase += m_dwFlush;

				if(m_dwBase >= m_dwDiscard)
					m_dwBase = 0;

				if( FAILED( hr = m_pVB->Lock( m_dwBase * sizeof(POINTVERTEX), m_dwFlush * sizeof(POINTVERTEX),
		            (void**) &pVertices, m_dwBase ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD ) ) )
                {
					return hr;
                }

                dwNumParticlesToRender = 0;
            }

			if (pParticle->m_bSpark==false)
			{
				float fT;
				fT = fTime - pParticle->m_fTime0;
			
				fT -= i * TIME_TO_REACH_END_OF_TAIL / (float)dwSteps;
				
				vPos    = pParticle->m_vVel0 * fT + pParticle->m_vPos0;
				vPos.y += (0.5f * GRAVITY) * (fT * fT);
			}
			else
			{
				// Randomly move vPos a bit
				vPos.x = vPos.x - 2.0f + frandomMT() * 4.0f;
				vPos.y = vPos.y - 2.0f + frandomMT() * 4.0f;
				vPos.z = vPos.z - 2.0f + frandomMT() * 4.0f;
			}
        }

        pParticle = pParticle->m_pNext;
    }

    // Unlock the vertex buffer
    m_pVB->Unlock();

    // Render any remaining particles
    if( dwNumParticlesToRender )
    {
        if(FAILED(hr = pd3dDevice->DrawPrimitive( D3DPT_POINTLIST, m_dwBase, dwNumParticlesToRender )))
			return hr;
    }

    // Reset render states
    pd3dDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
    pd3dDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  FALSE );

    return S_OK;
}

void ParticleSystem::DrawParticles(void)
{
	D3DXMATRIX oMatrix;
	D3DXMatrixIdentity(&oMatrix);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &oMatrix);

    // Draw particles
    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
    m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

    m_pd3dDevice->SetTexture(0, modelContainer->textures[PARTICLE_TEXTURE_1] );
    Render( m_pd3dDevice );

    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}

void ParticleSystem::DrawParticleReflections(void)
{
	D3DXMATRIX oMatrix;
	D3DXMatrixIdentity(&oMatrix);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &oMatrix);

	D3DXMATRIX matReflectedView, matView;
	m_pd3dDevice->GetTransform( D3DTS_VIEW, &matView );
	D3DXPLANE ground(0,1,0,0);
    D3DXMatrixReflect( &matReflectedView, &ground );
    D3DXMatrixMultiply( &matReflectedView, &matReflectedView, &matView );        

    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
    m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

    m_pd3dDevice->SetTransform( D3DTS_VIEW, &matReflectedView );
    m_pd3dDevice->SetTexture(0,modelContainer->textures[PARTICLE_TEXTURE_1] );
	Render( m_pd3dDevice );

    m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}