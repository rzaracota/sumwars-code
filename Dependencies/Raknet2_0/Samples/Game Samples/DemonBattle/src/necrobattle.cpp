#define STRICT
#include <math.h>
#include <stdio.h>
#include <D3DX9.h>
#include "D3DApp.h"
//#include "D3DFont.h"
#include "D3DUtil.h"
#include "DXUtil.h"
#include "..\resource.h"

#include "ParticleSystem.h"
#include "ModelContainer.h"
#include "TextManager.h"
#include "MainMenuState.h"
#include "CreditsState.h"
#include "InstructionsState.h"
#include "GameState.h"
#include "HostGameState.h"
#include "JoinGameState.h"
#include "rand.h"
#include "GetTime.h"
#include "InputSystem.h"
#include "User.h"
#include "RakServerInterface.h"
#include "RakClientInterface.h"
#include "ParticleSystemManager.h"
#include "DirectSound.h"
#include "SoundContainer.h"
#include <D3dx9math.h>
#include "DemonBattleMultiplayer.h"

extern RakServerInterface* rakServer;
extern RakClientInterface* rakClient;

//#include "MemoryManager.h"

//-----------------------------------------------------------------------------
// Name: class CMyD3DApplication
// Desc: Application class. The base class (CD3DApplication) provides the 
//       generic functionality needed in all Direct3D samples. CMyD3DApplication 
//       adds functionality specific to this sample program.
//-----------------------------------------------------------------------------
class CMyD3DApplication : public CD3DApplication
{
		/*
    CD3DFont*          m_pFont;
    CD3DFont*          m_pFontSmall;


    // Ground stuff
    LPDIRECT3DTEXTURE9 m_pGroundTexture;
    D3DXPLANE          m_planeGround;

    LPDIRECT3DVERTEXBUFFER9 m_pGroundVB;
    LPDIRECT3DINDEXBUFFER9  m_pGroundIB;
    DWORD              m_dwNumGroundIndices;
    DWORD              m_dwNumGroundVertices;

    // Particle stuff
    LPDIRECT3DTEXTURE9 m_pParticleTexture;
    ParticleSystem*   m_pParticleSystem;
    DWORD              m_dwNumParticlesToEmit;
   

    // Variables for determining view position
    D3DXVECTOR3        m_vPosition;
    D3DXVECTOR3        m_vVelocity;
    FLOAT              m_fYaw;
    FLOAT              m_fYawVelocity;
    FLOAT              m_fPitch;
    FLOAT              m_fPitchVelocity;
    D3DXMATRIX         m_matView;
    D3DXMATRIX         m_matOrientation;
*/
	 BOOL               m_bCanDoAlphaBlend;
	 bool doRender;
	 BYTE               m_bKey[256];
	 
    DWORD              m_dwParticleColor;
    BOOL               m_bAnimateEmitter;

	 
    BOOL               m_bDrawHelp;
    BOOL               m_bDrawReflection;
	long elapsedTime, lastTime;

    HRESULT ConfirmDevice( D3DCAPS9*, DWORD, D3DFORMAT );

protected:
    HRESULT OneTimeSceneInit();
    HRESULT InitDeviceObjects();
    HRESULT RestoreDeviceObjects();
    HRESULT InvalidateDeviceObjects();
    HRESULT DeleteDeviceObjects();
    HRESULT Render();
    HRESULT FrameMove();
    HRESULT FinalCleanup();
    VOID    UpdateCamera();

public:
    LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    CMyD3DApplication();
};


//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: Called once per frame, the call is the entry point for animating
//       the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::FrameMove()
{
	unsigned long time = (long)/*m_fElapsedTime*/ (DXUtil_Timer(TIMER_GETABSOLUTETIME) * 1000.0f);
	if (lastTime==0L)
		elapsedTime=0L;
	else
		elapsedTime = time - lastTime;
	lastTime=time;
	

	/*
	if (lastTime !=0)
		elapsedTime = time - lastTime;
	else
		elapsedTime = 30;

	lastTime=time;
*/

	if (isActiveFromWindowsMessage==false && (rakServer->IsActive()==false && rakClient->IsConnected()==false))
	{
		elapsedTime=0L;
		textManager->ShowMessageEx(-1, "PAUSED", (manager->GetScreenWidth()-250) / 2, (manager->GetScreenHeight()-30)/2, 64, "Arial", 255, 0, 0, 255);
	}

	manager->GetState()->Update(elapsedTime);
	user->Update(elapsedTime);
	DemonBattleMultiplayer::Instance()->ProcessPackets();

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Called once per frame, the call is the entry point for 3d
//       rendering. This function sets up render states, clears the
//       viewport, and renders the scene.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::Render()
{
   if( doRender==false || FAILED( m_pd3dDevice->BeginScene() ) )
        return S_OK;

    // Clear the viewport
     m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
                         0x00000000, 1.0f, 0L );

	manager->GetState()->Render(elapsedTime);
	textManager->Render(elapsedTime);
	user->Render(elapsedTime);

    // End the scene.
    m_pd3dDevice->EndScene();
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point to the program. Initializes everything, and goes into a
//       message-processing loop. Idle time is used to render the scene.
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
    CMyD3DApplication d3dApp;

	ShowCursor (0);

    if( FAILED( d3dApp.Create( hInst ) ) )
        return 0;	


    return d3dApp.Run();
}




//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Called when the app is exiting, or the device is being changed,
//       this function deletes any device dependent objects.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::DeleteDeviceObjects()
{
	
	modelContainer->UnloadAll();
	textManager->Unload();
	mainMenuState->Unload();
	hostGameState->Unload();
	joinGameState->Unload();
	creditsState->Unload();
	instructionsState->Unload();
	gameState->Unload();
	particleSystemManager->Unload();


	/*
    m_pFont->DeleteDeviceObjects();
    m_pFontSmall->DeleteDeviceObjects();


	//isLoaded=false;

    SAFE_RELEASE( m_pGroundTexture );
    SAFE_RELEASE( m_pParticleTexture );

    SAFE_RELEASE( m_pGroundVB );
    SAFE_RELEASE( m_pGroundIB );
*/
    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: CMyD3DApplication()
// Desc: Application constructor. Sets attributes for the app.
//-----------------------------------------------------------------------------
CMyD3DApplication::CMyD3DApplication()
{
    m_strWindowTitle    = "DemonBattle";
//    m_bUseDepthBuffer   = TRUE;
	elapsedTime = lastTime = 0;
	/*
    m_pFont                = new CD3DFont( _T("Arial"), 12, D3DFONT_BOLD );
    m_pFontSmall           = new CD3DFont( _T("Arial"),  9, D3DFONT_BOLD );


    m_pParticleSystem      = new ParticleSystem( 512, 2048);
    m_pParticleTexture     = NULL;
    m_dwNumParticlesToEmit = 10;
    m_bAnimateEmitter      = FALSE;
    m_dwParticleColor      = COLOR_WHITE;

    m_pGroundTexture       = NULL;
    m_dwNumGroundVertices  = (GROUND_GRIDSIZE + 1) * (GROUND_GRIDSIZE + 1);
    m_dwNumGroundIndices   = (GROUND_GRIDSIZE * GROUND_GRIDSIZE) * 6;
    m_pGroundVB            = NULL;
    m_pGroundIB            = NULL;
    m_planeGround          = D3DXPLANE( 0.0f, 1.0f, 0.0f, 0.0f );

    m_bDrawReflection      = FALSE;
    m_bCanDoAlphaBlend     = FALSE;
    m_bDrawHelp            = FALSE;

    ZeroMemory( m_bKey, 256 );
    m_vPosition      = D3DXVECTOR3( 0.0f, 3.0f,-4.0f );
    m_vVelocity      = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    m_fYaw           = 0.03f;
    m_fYawVelocity   = 0.0f;
    m_fPitch         = 0.5f;
    m_fPitchVelocity = 0.0f;
    D3DXMatrixTranslation( &m_matView, 0.0f, 0.0f, 10.0f );
    D3DXMatrixTranslation( &m_matOrientation, 0.0f, 0.0f, 0.0f );
	*/
}




//-----------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: Called during initial app startup, this function performs all the
//       permanent initialization.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::OneTimeSceneInit()
{
	directSound->Load();
	soundContainer->LoadAll();

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: UpdateCamera()
// Desc: 
//-----------------------------------------------------------------------------
VOID CMyD3DApplication::UpdateCamera()
{
	/*
    FLOAT fElapsedTime;

    if( m_fElapsedTime > 0.0f )
        fElapsedTime = m_fElapsedTime;
    else
        fElapsedTime = 0.05f;

    FLOAT fSpeed        = 3.0f*fElapsedTime;
    FLOAT fAngularSpeed = 1.0f*fElapsedTime;

    // De-accelerate the camera movement (for smooth motion)
    m_vVelocity      *= 0.9f;
    m_fYawVelocity   *= 0.9f;
    m_fPitchVelocity *= 0.9f;

    // Process keyboard input
    if( m_bKey[VK_RIGHT] )    m_vVelocity.x    += fSpeed; // Slide Right
    if( m_bKey[VK_LEFT] )     m_vVelocity.x    -= fSpeed; // Slide Left
    if( m_bKey[VK_UP] )       m_vVelocity.y    += fSpeed; // Slide Up
    if( m_bKey[VK_DOWN] )     m_vVelocity.y    -= fSpeed; // Slide Down
    if( m_bKey['W'] )         m_vVelocity.z    += fSpeed; // Move Forward
    if( m_bKey['S'] )         m_vVelocity.z    -= fSpeed; // Move Backward
    if( m_bKey['E'] )         m_fYawVelocity   += fSpeed; // Turn Right
    if( m_bKey['Q'] )         m_fYawVelocity   -= fSpeed; // Turn Left
    if( m_bKey['Z'] )         m_fPitchVelocity += fSpeed; // Turn Down
    if( m_bKey['A'] )         m_fPitchVelocity -= fSpeed; // Turn Up
    if( m_bKey[VK_ADD] )      if( m_dwNumParticlesToEmit < 10 ) m_dwNumParticlesToEmit++;
    if( m_bKey[VK_SUBTRACT] ) if( m_dwNumParticlesToEmit > 0 )  m_dwNumParticlesToEmit--;

    // Update the position vector
    D3DXVECTOR3 vT = m_vVelocity * fSpeed;
    D3DXVec3TransformNormal( &vT, &vT, &m_matOrientation );
    m_vPosition += vT;
    if( m_vPosition.y < 1.0f )
        m_vPosition.y = 1.0f;

    // Update the yaw-pitch-rotation vector
    m_fYaw   += fAngularSpeed * m_fYawVelocity;
    m_fPitch += fAngularSpeed * m_fPitchVelocity;
    if( m_fPitch < 0.0f )      m_fPitch = 0.0f;
    if( m_fPitch > D3DX_PI/2 ) m_fPitch = D3DX_PI/2;

    // Set the view matrix
    D3DXQUATERNION qR;
    D3DXQuaternionRotationYawPitchRoll( &qR, m_fYaw, m_fPitch, 0.0f );
    D3DXMatrixAffineTransformation( &m_matOrientation, 1.25f, NULL, &qR, &m_vPosition );
    D3DXMatrixInverse( &m_matView, NULL, &m_matOrientation );
	*/
}


//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initialize scene objects.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InitDeviceObjects()
{
	
/*
    HRESULT hr;
    // Create textures
    if( FAILED( D3DUtil_CreateTexture( m_pd3dDevice, _T("Ground2.bmp"),
                                       &m_pGroundTexture ) ) )
        return D3DAPPERR_MEDIANOTFOUND;

    if( FAILED( D3DUtil_CreateTexture( m_pd3dDevice, _T("Particle.bmp"),
                                       &m_pParticleTexture ) ) )
        return D3DAPPERR_MEDIANOTFOUND;

    // Set up the fonts and textures
    m_pFont->InitDeviceObjects( m_pd3dDevice );
    m_pFontSmall->InitDeviceObjects( m_pd3dDevice );
*/
    // Check if we can do the reflection effect
    m_bCanDoAlphaBlend = (m_d3dCaps.SrcBlendCaps & D3DPBLENDCAPS_SRCALPHA) &&
                         (m_d3dCaps.DestBlendCaps & D3DPBLENDCAPS_INVSRCALPHA);

    if( m_bCanDoAlphaBlend )
        m_bDrawReflection = TRUE;
/*
    // Create ground object
    {
        // Create vertex buffer for ground object
        hr = m_pd3dDevice->CreateVertexBuffer( m_dwNumGroundVertices*sizeof(COLORVERTEX),
                                               D3DUSAGE_WRITEONLY, D3DFVF_COLORVERTEX,
                                               D3DPOOL_MANAGED, &m_pGroundVB );
        if( FAILED(hr) )
            return E_FAIL;

        // Fill vertex buffer
        COLORVERTEX* pVertices;
        if( FAILED( m_pGroundVB->Lock( 0, 0, (BYTE**)&pVertices, NULL ) ) )
            return hr;

        // Fill in vertices
        for( DWORD zz = 0; zz <= GROUND_GRIDSIZE; zz++ )
        {
            for( DWORD xx = 0; xx <= GROUND_GRIDSIZE; xx++ )
            {
                pVertices->v.x   = GROUND_WIDTH * (xx/(FLOAT)GROUND_GRIDSIZE-0.5f);
                pVertices->v.y   = 0.0f;
                pVertices->v.z   = GROUND_HEIGHT * (zz/(FLOAT)GROUND_GRIDSIZE-0.5f);
                pVertices->color = GROUND_COLOR;
                pVertices->tu    = xx*GROUND_TILE/(FLOAT)GROUND_GRIDSIZE;
                pVertices->tv    = zz*GROUND_TILE/(FLOAT)GROUND_GRIDSIZE;
                pVertices++;
            }
        }

        m_pGroundVB->Unlock();

        // Create the index buffer
        WORD* pIndices;
        hr = m_pd3dDevice->CreateIndexBuffer( m_dwNumGroundIndices*sizeof(WORD),
                                              D3DUSAGE_WRITEONLY,
                                              D3DFMT_INDEX16, D3DPOOL_MANAGED,
                                              &m_pGroundIB );
        if( FAILED(hr) )
            return E_FAIL;

        // Fill the index buffer
        m_pGroundIB->Lock( 0, m_dwNumGroundIndices*sizeof(WORD), (BYTE**)&pIndices, 0 );
        if( FAILED(hr) )
            return E_FAIL;

        // Fill in indices
        for( DWORD z = 0; z < GROUND_GRIDSIZE; z++ )
        {
            for( DWORD x = 0; x < GROUND_GRIDSIZE; x++ )
            {
                DWORD vtx = x + z * (GROUND_GRIDSIZE+1);
                *pIndices++ = (WORD)( vtx + 1 );
                *pIndices++ = (WORD)( vtx + 0 );
                *pIndices++ = (WORD)( vtx + 0 + (GROUND_GRIDSIZE+1) );
                *pIndices++ = (WORD)( vtx + 1 );
                *pIndices++ = (WORD)( vtx + 0 + (GROUND_GRIDSIZE+1) );
                *pIndices++ = (WORD)( vtx + 1 + (GROUND_GRIDSIZE+1) );
            }
        }

        m_pGroundIB->Unlock();
    }
*/
	
			
	if (!inputSystem->Init (m_hWnd,m_hInstance,(int)manager->GetScreenWidth(),(int)manager->GetScreenHeight()))
		return FALSE;
	if (!inputSystem->InitMouse (1))
		return FALSE;
	if (!inputSystem->InitKeyboard ())
		return FALSE;

	// Center the mouse
	inputSystem->m_posX = manager->GetScreenWidth()/2;
	inputSystem->m_posY = manager->GetScreenHeight()/2;
	seedMT(timeGetTime());
	GetTime::Instance()->Init();

	FLOAT fAspect = ((FLOAT)(int)manager->GetScreenWidth()) / (int)manager->GetScreenHeight();
	user->camera.SetProjParams(D3DX_PI/4, fAspect, 0.1f, VIEW_DISTANCE);

	manager->SetState(mainMenuState);


    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::RestoreDeviceObjects()
{
//	HRESULT hr;

	/*
	   // Set the world matrix
    D3DXMATRIXA16 matWorld;
    D3DXMatrixIdentity( &matWorld );
    m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

    // Set projection matrix
    D3DXMATRIXA16 matProj;
    FLOAT fAspect = ((FLOAT)manager->GetScreenWidth()) / manager->GetScreenHeight();
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, 0.1f, 100.0f );
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

    // Set the world matrix
    D3DXMATRIX matWorld;
    D3DXMatrixIdentity( &matWorld );
    m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	
	D3DXMATRIX matProj;
	FLOAT fAspect = ((FLOAT)manager->GetScreenWidth()) / (FLOAT)manager->GetScreenHeight();
	//user->camera.SetProjParams(D3DX_PI/4.5f, fAspect, 1.0f, VIEW_DISTANCE);
	user->camera.SetProjParams(D3DX_PI/3.0f, fAspect, 1.0f, VIEW_DISTANCE);
	user->camera.SetProjectionMatrix();
	*/

   // Set the world matrix
    D3DXMATRIXA16 matWorld;
    D3DXMatrixIdentity( &matWorld );
    m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

    // Set projection matrix
	D3DXMATRIX matProj;
	FLOAT fAspect = ((FLOAT)manager->GetScreenWidth()) / (FLOAT)manager->GetScreenHeight();
	//user->camera.SetProjParams(D3DX_PI/4.5f, fAspect, 1.0f, VIEW_DISTANCE);
	user->camera.SetProjParams(D3DX_PI/3.0f, fAspect, 1.0f, VIEW_DISTANCE);
	user->camera.SetProjectionMatrix();

	user->camera.SetViewMatrix();

	
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
    m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
    m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

    m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
    m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING,  TRUE );
	m_pd3dDevice->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE( 0.95, 0.95, 0.95, 1.0 ) ); // ARGB
    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,  D3DCULL_CW );
    m_pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
/*
	
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	*/

	modelContainer->LoadAll();
	textManager->Initialize(m_pd3dDevice);
	mainMenuState->Load();
	hostGameState->Load();
	joinGameState->Load();
	creditsState->Load();
	instructionsState->Load();
	gameState->Load();
	user->Load();
	manager->GetState()->Init();

	doRender=true;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::InvalidateDeviceObjects()
{
 //   m_pFont->InvalidateDeviceObjects();
   // m_pFontSmall->InvalidateDeviceObjects();
  //  m_pParticleSystem->InvalidateDeviceObjects();

	modelContainer->UnloadAll();
	textManager->Unload();
	mainMenuState->Unload();
	hostGameState->Unload();
	joinGameState->Unload();
	creditsState->Unload();
	instructionsState->Unload();
	gameState->Unload();
	user->Unload();
	particleSystemManager->Unload();
    
	doRender=false;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: FinalCleanup()
// Desc: Called before the app exits, this function gives the app the chance
//       to cleanup after itself.
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::FinalCleanup()
{
		/*
    SAFE_DELETE( m_pFont );
    SAFE_DELETE( m_pFontSmall );

    SAFE_DELETE( m_pGroundTexture );
    SAFE_DELETE( m_pParticleTexture );
    SAFE_DELETE( m_pParticleSystem );
*/
	soundContainer->UnloadAll();
	directSound->Unload();

	FILE *fp;
	fp = fopen("d3dsettings.cfg", "wb");
	if (fp)
	{
		putc(!m_d3dSettings.IsWindowed, fp);
		fclose(fp);
	}
	
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: ConfirmDevice()
// Desc: Called during device intialization, this code checks the device
//       for some minimum set of capabilities
//-----------------------------------------------------------------------------
HRESULT CMyD3DApplication::ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior,
                                          D3DFORMAT Format )
{
    // Make sure device can do ONE:ONE alphablending
    if( 0 == ( pCaps->SrcBlendCaps & D3DPBLENDCAPS_ONE ) )
        return E_FAIL;
    if( 0 == ( pCaps->DestBlendCaps & D3DPBLENDCAPS_ONE ) )
        return E_FAIL;

    // Make sure HW TnL devices can do point sprites
    if( (dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING ) ||
        (dwBehavior & D3DCREATE_MIXED_VERTEXPROCESSING ) )
    {
        if( pCaps->MaxPointSize <= 1.0f )
            return E_FAIL;
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: Message proc function to handle key and menu input
//-----------------------------------------------------------------------------
LRESULT CMyD3DApplication::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam,
                                    LPARAM lParam )
{
/*
    // Record key presses
    if( WM_KEYDOWN == uMsg )
    {
        m_bKey[wParam] = 1;
    }

    // Perform commands when keys are rleased
    if( WM_KEYUP == uMsg )
    {
        m_bKey[wParam] = 0;

        switch( wParam )
        {
            case VK_F1:
                m_bDrawHelp = !m_bDrawHelp;
                return 1;

            case 'R':
                if( m_bCanDoAlphaBlend )
                    m_bDrawReflection = !m_bDrawReflection;
                break;

            case VK_F3:
                m_bAnimateEmitter = !m_bAnimateEmitter;
                break;

            case VK_F4:
                if( ++m_dwParticleColor >= NUM_COLORS )
                    m_dwParticleColor = COLOR_WHITE;
                break;
        }
    }
*/
    return CD3DApplication::MsgProc( hWnd, uMsg, wParam, lParam );
}


