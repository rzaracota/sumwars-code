#ifndef D_TOOLZ
#define D_TOOLZ


#include <windows.h>

class CDirectToolz
{
public :
	CDirectToolz ();

	//Für Kollisionsabfragen
	BOOL PtInRect (int x, int y, RECT& r) const;

	//Erschafft ein normales Windowsfenster
	BOOL CreateNormalWindow (char name[], HINSTANCE h);

	//Runden
	int Runden (double n) const;

	//Info in Datei schreiben
	BOOL FTrace ( LPCSTR msg, LPCSTR logFile ="log.log", BOOL bDelete=FALSE ) const;

	//Macht ne Fehlermeldung+Trace
	void ErrorBox (LPCSTR msg) const;
	
	//X und Y : Jeweilige Startwerte
	//w : Breite
	//h : Höhe
	//Testen ob sich 2 Rechtecke überlappen
	int CollisionTest(int x1, int y1, int w1, int h1, 
                   	  int x2, int y2, int w2, int h2);
  
  
  	// this function computes the distance from 0,0 to x,y with 3.5% error
  	//BSP : x1 = 100, x2 = 200, y1 = 200, y2 = 300
  	//FastDistance (x1-x2,y1-y2);
  	int FastDistance_2D(int DeltAx, int DeltaA);

	char* ErrorToString(HRESULT error);
  	                 
	HWND m_hwnd;
	HINSTANCE m_hInst;

	float SIN_LOOK[360], COS_LOOK[360];
};

#endif