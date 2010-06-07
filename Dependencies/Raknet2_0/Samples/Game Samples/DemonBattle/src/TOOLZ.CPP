#include "toolz.h"
#include <stdlib.h>
#include <stdio.h>

#include <math.h>
#include "ManagerState.h"

//#include "MemoryManager.h"

extern LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int CDirectToolz::Runden (double n) const
{	
	int z, rest;
	rest = (int) (n);
	n -= (double) (rest);
	if (n >= 0.5) z = 1;
	if (n < 0.5) z = 0;
	z+=rest;
	return (z);
}//Runden

BOOL CDirectToolz::FTrace ( LPCSTR msg, LPCSTR logFile, BOOL bDelete ) const
{
	#ifdef _DEBUG
	FILE *datei;
	if (bDelete)
		datei = fopen (logFile,"w");
	else
		datei = fopen ( logFile,"a");

	if (datei != NULL)
	{
		fwrite (msg, strlen (msg),1,datei);
		fclose (datei);		
		return TRUE;
	}
	else
	{
		return FALSE;
	}
#else
	return FALSE;
#endif

}//FTrace

void CDirectToolz::ErrorBox (LPCSTR msg) const
{
	MessageBox (m_hwnd,msg,"Fehler!",MB_ICONSTOP);
	FTrace ("\n");
	FTrace (msg);
}

BOOL CDirectToolz::PtInRect (int x, int y, RECT& r) const
{
	return (x >r.left && x < r.right &&
			y >r.top  && y < r.bottom);

}

BOOL CDirectToolz::CreateNormalWindow (char  name[], HINSTANCE h)
{
	m_hInst = h;

	//Ab hier wird das Fenster erstellt
	WNDCLASS		wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = m_hInst;
	wndclass.hIcon = LoadIcon (NULL,IDI_APPLICATION);
	wndclass.hCursor = LoadCursor (NULL,IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = name;

	RegisterClass (&wndclass);

#ifdef _DEBUG
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, name, name,
        WS_POPUP, 0, 0, 640,
        480, NULL, NULL, m_hInst, NULL);
		manager->SaveResolution(640,480);
#else
	m_hwnd = CreateWindowEx(WS_EX_TOPMOST, name, name,
        WS_POPUP, 0, 0, GetSystemMetrics (SM_CXSCREEN),
        GetSystemMetrics (SM_CYSCREEN), NULL, NULL, m_hInst, NULL);
		manager->SaveResolution(GetSystemMetrics (SM_CXSCREEN),GetSystemMetrics (SM_CYSCREEN));
#endif

	if (m_hwnd == NULL || m_hInst == NULL)
		return FALSE;

#ifdef _DEBUG
	ShowWindow (m_hwnd, SW_SHOWNORMAL);
#else
	//Fenster ist erstellt
	ShowWindow (m_hwnd, SW_SHOWMAXIMIZED);
#endif
	UpdateWindow (m_hwnd);

	return TRUE;
}

int CDirectToolz::CollisionTest(int x1, int y1, int w1, int h1, 
                   int x2, int y2, int w2, int h2) 
{
	// this function tests if the two rects overlap
	
	// get the radi of each rect
	int width1  = (w1>>1) - (w1>>3);
	int height1 = (h1>>1) - (h1>>3);
	
	int width2  = (w2>>1) - (w2>>3);
	int height2 = (h2>>1) - (h2>>3);
	
	// compute center of each rect
	int cx1 = x1 + width1;
	int cy1 = y1 + height1;
	
	int cx2 = x2 + width2;
	int cy2 = y2 + height2;
	
	// compute deltas
	int dx = abs(cx2 - cx1);
	int dy = abs(cy2 - cy1);
	
	// test if rects overlap
	if (dx < (width1+width2) && dy < (height1+height2))
	   return(1);
	else
	// else no collision
	return(0);

} // end Collision_Test

int CDirectToolz::FastDistance_2D(int x, int y)
{
	// this function computes the distance from 0,0 to x,y with 3.5% error
	
	// first compute the absolute value of x,y
	x = abs(x);
	y = abs(y);
	
	// compute the minimum of x,y
	int mn = x<y ? x:y;
	
	// return the distance
	return(x+y-(mn>>1)-(mn>>2)+(mn>>4));

} // end Fast_Distance_2D

CDirectToolz::CDirectToolz ()
{
	for (int winkel=0;winkel<360;winkel++)
	{
		float rad_winkel = winkel*(3.14159f/180);
		
		SIN_LOOK[winkel] = sinf(rad_winkel);
		COS_LOOK[winkel] = cosf(rad_winkel);	
	}
}