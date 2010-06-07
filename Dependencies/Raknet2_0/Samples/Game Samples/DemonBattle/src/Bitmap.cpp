#include "Bitmap.h"
#include <assert.h>
#include "ManagerState.h"
#include "D3DHeaders.h"
#include "..\Common\Include\d3dutil.h"

//#include "MemoryManager.h"


#include <D3D9.h>

Bitmap::~Bitmap()
{
	Free();
}

Bitmap::Bitmap()
{
	bitmapLoaded=false;
	surface=0;
}

void Bitmap::Load(char* file)
{
	if (!surface)
	{
		D3DXGetImageInfoFromFile(file, &imageInfo);
		m_pd3dDevice->CreateOffscreenPlainSurface(imageInfo.Width, imageInfo.Height, m_d3dsdBackBuffer.Format, D3DPOOL_SYSTEMMEM, &surface, NULL);
	}

	if (SUCCEEDED(D3DXLoadSurfaceFromFile(surface, NULL, NULL, file, NULL, D3DX_FILTER_NONE, 0, NULL)))
		bitmapLoaded=true;
	
	assert(surface);

	assert(bitmapLoaded);
}

void Bitmap::Free(void)
{
	if (surface)
	{
		surface->Release();
		surface=0;
	}

	bitmapLoaded=false;
}

void Bitmap::Render(int x, int y)
{
	assert(surface);
	assert(bitmapLoaded);

	RECT source;

		if (y<0)
			y=0;

		if (x<0)
			x=0;

	if ((int)manager->GetScreenWidth()-x > (int)imageInfo.Width)
	{	
		source.left=0;
		source.right=imageInfo.Width;
	}
	else
	{
		source.left = (imageInfo.Width - ((int)manager->GetScreenWidth()-x)) / 2;
		source.right= (imageInfo.Width + ((int)manager->GetScreenWidth()-x)) / 2;
	}

	if ((int)manager->GetScreenHeight()-y > (int)imageInfo.Height)
	{
		source.top=0;
		source.bottom=imageInfo.Height;
	}
	else
	{
		source.top = (imageInfo.Height - ((int)manager->GetScreenHeight()-y)) / 2;
		source.bottom= (imageInfo.Height + ((int)manager->GetScreenHeight()-y)) / 2;
	}

	POINT dest;
	dest.x=x; dest.y=y;
	
	// Store render target surface desc
	LPDIRECT3DSURFACE9 m_BackBuffer;			// The backbuffer
    m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &m_BackBuffer );
	m_pd3dDevice->UpdateSurface(surface, &source, m_BackBuffer, &dest);
	m_BackBuffer->Release();
}

void Bitmap::RenderTiled()
{
	assert(surface);
	assert(bitmapLoaded);

	LPDIRECT3DSURFACE9 m_BackBuffer;			// The backbuffer
	m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &m_BackBuffer );

	RECT source;

	source.left=0;
	source.top=0;
	source.bottom=imageInfo.Height;

	POINT dest;
	dest.x=0; dest.y=0;

	while (dest.y < (int)manager->GetScreenHeight())
	{
		source.right=imageInfo.Width;

		if (dest.y + imageInfo.Height > (int)manager->GetScreenHeight())
			source.bottom = (int)manager->GetScreenHeight() - dest.y;
		
		while (dest.x < (int)manager->GetScreenWidth())
		{
            if (dest.x + imageInfo.Width > (int)manager->GetScreenWidth())
				source.right = (int)manager->GetScreenWidth() - dest.x;

			m_pd3dDevice->UpdateSurface(surface, &source, m_BackBuffer, &dest);

			dest.x += imageInfo.Width;
		}

		dest.y += imageInfo.Height;
	}
	
	m_BackBuffer->Release();
}