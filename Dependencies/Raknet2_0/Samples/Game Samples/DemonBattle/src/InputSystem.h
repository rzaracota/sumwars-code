#ifndef __INPUT_SYSTEM_H
#define __INPUT_SYSTEM_H

#include <dinput.h>
#include <windows.h>
#include "toolz.h"
#include "d3dapp.h"

//extern BOOL g_bWork;
class InputSystem
{
public :
	static InputSystem* Instance(void) {return &I;}

	//Aktualisiert die Mausdaten
	BOOL UpdateMouse (long time);

	//Initialisiert DirectInput objekt
	BOOL Init (HWND, HINSTANCE,int nResX,int nResY);

	//Maus initialisiern und belegen
	BOOL InitMouse (float nSpeed =1);

	//Keyboard initialisieren und belegen
	BOOL InitKeyboard ();

	void ShutDown ();

	CDirectToolz m_toolz;

	//x und y pos der Maus in Pixel
	int m_posX,m_posY;

	BOOL UpdateKeyboard (long time);

	unsigned long GetKeyStateTime(int key) {return keyStateTime[key];}
	unsigned long GetMouseStateTime(int button) {return mouseButtonStateTime[button];}
	int KeyPressed(int key) {return g_pD3DApp->isActiveFromWindowsMessage && (m_KeysDown[key] & 0x80);}
	int KeyReleased(int key) {return   g_pD3DApp->isActiveFromWindowsMessage && (!(m_KeysDown[key] & 0x80));}
	bool MouseButtonPressed(int button) {return  g_pD3DApp->isActiveFromWindowsMessage && mouseButtonDown[button];}
	bool MouseButtonReleased(int button) {return  g_pD3DApp->isActiveFromWindowsMessage && mouseButtonDown[button]==false;}
	bool KeyJustPressed(int key) {if ((m_KeysDown[key] & 0x80) && keyStateTime[key]==0L &&  g_pD3DApp->isActiveFromWindowsMessage) return true; else return false;}
	bool KeyJustReleased(int key) {if (!(m_KeysDown[key] & 0x80) && keyStateTime[key]==0L &&  g_pD3DApp->isActiveFromWindowsMessage) return true; else return false;}
	bool MouseButtonJustPressed(int button) {if (mouseButtonDown[button] && mouseButtonStateTime[button]==0L &&  g_pD3DApp->isActiveFromWindowsMessage) return true; else return false;}
	bool MouseButtonJustReleased(int button) {if (mouseButtonDown[button]==false && mouseButtonStateTime[button]==0L &&  g_pD3DApp->isActiveFromWindowsMessage) return true; else return false;}
	int MouseX(void) const {return m_posX;}
	int MouseY(void) const {return m_posY;}
	void SetMouseSensitivity(float multiplier) {mouseSensitivity=multiplier;}

	//Auflösung
	int m_nResX,m_nResY;

	//Position der Maus, reicht von -n bis n
	//d.h. die Mauspos wird nicht durch die Auflösung eingeschränkt
	int m_posX2, m_posY2;

	float m_speed2; //S.o.
private:
	InputSystem ();
	DIMOUSESTATE m_state;

	//Hiermit wird die Mausbewegung multipliziert
	float mouseSensitivity;

	// contains keyboard state table
	UCHAR m_KeysDown[256];
	unsigned long keyStateTime[256];
	unsigned long mouseButtonStateTime[3];
	bool mouseButtonDown[3];

	//HWND+Hinstance
	HWND m_hwnd;
	HINSTANCE m_hInst;

	LPDIRECTINPUTDEVICE8 m_lpDIMouse;
	LPDIRECTINPUTDEVICE8 m_lpDIKeyboard;
	LPDIRECTINPUT8		 m_lpDirectInput;

	static InputSystem I;
};

#define inputSystem InputSystem::Instance()

#endif