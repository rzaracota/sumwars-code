#include "InputSystem.h"
#include "..\Common\Include\d3dutil.h"
#include "ManagerState.h"

//#include "MemoryManager.h"


CDirectToolz		g_toolz;
BOOL g_bWork = TRUE;

InputSystem InputSystem::I;

InputSystem::InputSystem ()
{
	m_lpDIKeyboard = NULL;
	m_lpDirectInput = NULL;
	m_lpDIMouse = NULL;

	memset(keyStateTime, 1, 256 * sizeof(unsigned long));
	ZeroMemory(m_KeysDown, 256 * sizeof(UCHAR));
	memset(mouseButtonStateTime, 1, 3 * sizeof(unsigned long));
	ZeroMemory(mouseButtonDown, 3 * sizeof(bool));
}

void InputSystem::ShutDown ()
{
	if (m_lpDIMouse)
	{
		m_lpDIMouse->Unacquire ();
		m_lpDIMouse->Release ();
		m_lpDIMouse = NULL;
	}
	if (m_lpDIKeyboard)
	{
		m_lpDIKeyboard->Unacquire ();
		m_lpDIKeyboard->Release();
		m_lpDIKeyboard = NULL;
	}
	if (m_lpDirectInput)
	{
		m_lpDirectInput->Release ();
		m_lpDirectInput = NULL;
	} 
}

BOOL InputSystem::InitKeyboard ()
{
	// this function initializes the keyboard device

	// create the keyboard device  

	if (FAILED(m_lpDirectInput->CreateDevice(GUID_SysKeyboard, &m_lpDIKeyboard, NULL)))
	{
		m_toolz.ErrorBox ("Fehler beim erstellen des Keyboards");
		return FALSE;
	}

	// set cooperation level
	if (m_lpDIKeyboard->SetCooperativeLevel(m_hwnd, 
					 DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)!=DI_OK)
	{
		m_toolz.ErrorBox ("Fehler beim setzen des Kooperationslevel fürs Keybord");
		return FALSE;
	}


	// set data format
	if (m_lpDIKeyboard->SetDataFormat(&c_dfDIKeyboard)!=DI_OK)
	{
		m_toolz.ErrorBox ("Fehler beim setzen des DataFormats fürs Keyboard");
		return FALSE;
	}

	// acquire the keyboard
	if (m_lpDIKeyboard->Acquire()!=DI_OK)
	{
		m_toolz.ErrorBox ("Fehler bei Keyboard->Acquire");
		return FALSE;
	}

	// return success
	return TRUE;
}

BOOL InputSystem::UpdateKeyboard (long time)
{
	if (!g_bWork) return FALSE;

	UCHAR tempKeysDown[256];
	HRESULT result;
	result = m_lpDIKeyboard->GetDeviceState(256, (LPVOID)tempKeysDown) == DI_OK;
	if (FAILED(result))
		return result;

	// Compare the last state to the current state.
	for (int key=0; key<256; key++)
	{
		if ((m_KeysDown[key] & 0x80) != (tempKeysDown[key] & 0x80))
			keyStateTime[key]=0L; // If the state is different reset the timer for it.
		else
			keyStateTime[key]+=time; // Otherwise increment the timer for it
	}

	// Record the new state
	memcpy(m_KeysDown, tempKeysDown, 256 * sizeof(UCHAR));

	return result;
}


BOOL InputSystem::UpdateMouse (long time)
{
	if (!g_bWork) return FALSE;
	if (FAILED(m_lpDIMouse->GetDeviceState(sizeof(m_state), &m_state)))
		return FALSE;

//		Die Daten aus dem DIMOUSESTATE-Typ 
//		LONG lX die relative Bewegung der Maus auf der X-Achse
//		LONG lY dito für die Y-Achse
//		LONG lZ die relative Bewegung des Mausrads
//		BYTE rgbButtons[..] der Status der einzelnen Tasten (eine Taste ist gedrückt, wenn das höchste Bit gesetzt ist)

	bool tempMouseButtonDown[3];

	for (int i=0; i < 3; i++)
		if(m_state.rgbButtons[i] & 0x80)
			tempMouseButtonDown[i]=true;
		else
			tempMouseButtonDown[i]=false;

	// Compare the last state to the current state.
	for (int button=0; button<3; button++)
		if (tempMouseButtonDown[button] != mouseButtonDown[button])
			mouseButtonStateTime[button]=0L; // If the state is different reset the timer for it.
		else
			mouseButtonStateTime[button]+=time; // Otherwise increment the timer for it

	// Record the new state
	memcpy(mouseButtonDown, tempMouseButtonDown, 3 * sizeof(bool));

//	 if( applicationPointer->m_bWindowed)
  //  {
        POINT ptCursor;
        GetCursorPos( &ptCursor );
		ScreenToClient( m_hWnd, &ptCursor );
		m_posX=ptCursor.x;
		m_posY=ptCursor.y;
//	}

//	 else
//	{
		//Y-Bewegung
//		m_posY += (int)(m_state.lY*mouseSensitivity);
//		m_posY2+= (int)(m_state.lY*m_speed2);

		if(m_posY < 0)
		m_posY = 0;
		//if(m_posY >= m_nResY)
		// m_posY = m_nResY- 1;

		if(m_posY >= (int)manager->GetScreenHeight())
		m_posY = (int)manager->GetScreenHeight() - 1;
		

		//X-Bewegung
//		m_posX+= (int)(m_state.lX*mouseSensitivity);
//		m_posX2+= (int)(m_state.lX*m_speed2);

		if(m_posX< 0)
		m_posX= 0;
		//if(m_posX>= m_nResX)
		// m_posX= m_nResX- 1;
		
		if(m_posX>= (int)manager->GetScreenWidth())
		m_posX= (int)manager->GetScreenWidth() - 1;
//	}


	return TRUE;
}

BOOL InputSystem::Init (HWND h, HINSTANCE hi, int nResX,int nResY)
{
	m_hInst = hi;
	m_hwnd = h;
	m_nResX = nResX;
	m_nResY = nResY;
	m_toolz.m_hInst = m_hInst;
	m_toolz.m_hwnd = m_hwnd;

	if (!IsWindow (m_hwnd))
	{
		m_toolz.ErrorBox ("Keine gültige Hwnd in DirectInput::Init");
		return FALSE;
	}

	if (m_hInst == 0)
	{
		m_toolz.ErrorBox ("HInst == 0");
		return FALSE;
	}

	//DInput Objekt erstellen
	if (FAILED( DirectInput8Create(hi, DIRECTINPUT_VERSION, 
        IID_IDirectInput8, (void**)&m_lpDirectInput, NULL)))
	{
		m_toolz.ErrorBox ("Fehler beim erstellen des DirectInput Objektes");
		return FALSE;
	}
	return TRUE;
}

BOOL InputSystem::InitMouse (float nSpeed)
{
	mouseSensitivity = nSpeed;

	//Die Maus initialisieren:
	if (FAILED(m_lpDirectInput->CreateDevice(GUID_SysMouse, &m_lpDIMouse, NULL)))
	{
		m_toolz.ErrorBox ("Fehler beim erschaffen des Mausobjektes");
		return FALSE;
	}
	
	//das Datenformat setzen. 
	if (FAILED(m_lpDIMouse->SetDataFormat(&c_dfDIMouse)))
	{
		m_toolz.ErrorBox ("Fehler beim setzen des Datenformates");
		return FALSE;
	}

	//Nun setzt man den Kooperativitätslevel, bei dem es mehrere Möglichkeiten gibt. 
	if (FAILED(m_lpDIMouse->SetCooperativeLevel(m_hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		m_toolz.ErrorBox ("Fehler beim setzen des Kooperationslevels");
		return FALSE;
	}

	//Jetzt noch schnell die Maus belegen:
	if (FAILED(m_lpDIMouse->Acquire()))
	{
		m_toolz.ErrorBox ("Fehler beim belegen der Maus");
		return FALSE;
	}

	//So, das war's auch schon mit der Initialisierung. 
	return TRUE;
}
