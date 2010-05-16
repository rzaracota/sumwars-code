#ifndef __PARTICLE_SYSTEM_H
#define __PARTICLE_SYSTEM_H

#include <D3dx9math.h>

enum PARTICLE_COLORS { COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_YELLOW, NUM_COLORS };

#define GRAVITY -45.0f

//-----------------------------------------------------------------------------
// Custom vertex types
//-----------------------------------------------------------------------------



struct POINTVERTEX
{
    D3DXVECTOR3 v;
    D3DCOLOR    color;

    static const DWORD FVF;
};
/*

struct POINTVERTEX
{
    D3DXVECTOR3 v;
    D3DCOLOR    color;
};

#define D3DFVF_POINTVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
*/

// Helper function to stuff a FLOAT into a DWORD argument
inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }

//-----------------------------------------------------------------------------
// Global data for the particles
//-----------------------------------------------------------------------------
struct PARTICLE
{
    BOOL        m_bSpark;     // Sparks are less energetic particles that
                              // are generated where/when the main particles
                              // hit the ground

    D3DXVECTOR3 m_vPos;       // Current position
    D3DXVECTOR3 m_vVel;       // Current velocity

    D3DXVECTOR3 m_vPos0;      // Initial position
    D3DXVECTOR3 m_vVel0;      // Initial velocity
    FLOAT       m_fTime0;     // Time of creation

    D3DXCOLOR   m_clrDiffuse; // Initial diffuse color
    D3DXCOLOR   m_clrFade;    // Faded diffuse color
    FLOAT       m_fFade;      // Fade progression

    PARTICLE*   m_pNext;      // Next particle in list
};




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
class ParticleSystem
{
protected:
    DWORD     m_dwBase;
	DWORD     m_dwFlush;
    DWORD     m_dwDiscard;

    DWORD     m_dwParticles;
    DWORD     m_dwParticlesLim;
    PARTICLE* m_pParticles;
    PARTICLE* m_pParticlesFree;
	float minHeight;

    // Geometry
    LPDIRECT3DVERTEXBUFFER9 m_pVB;

	float fTime;

public:
    ParticleSystem( DWORD dwFlush, DWORD dwDiscard);
	void StartParticle(const D3DXCOLOR &clrEmitColor, const D3DXCOLOR &clrFadeColor, D3DXVECTOR3 vPosition, D3DXVECTOR3 velocity, float MinHeight);
   ~ParticleSystem();

    HRESULT RestoreDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice );
    HRESULT InvalidateDeviceObjects();

    HRESULT Update( FLOAT fSecsPerFrame, DWORD dwNumParticlesToEmit,
                    const D3DXCOLOR &dwEmitColor, const D3DXCOLOR &dwFadeColor);

	void DrawParticles(void);
	void DrawParticleReflections(void);
	bool IsParticlesUpdating(void) const {return m_pParticles!=0;}
	

    HRESULT Render( LPDIRECT3DDEVICE9 pd3dDevice );


	static D3DXCOLOR g_clrColor[NUM_COLORS];
	static DWORD g_clrColorFade[NUM_COLORS];
};

#endif