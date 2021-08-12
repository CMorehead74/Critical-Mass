//--------------------------------------------------------------------------------
// Written by Justin Murphy
//--------------------------------------------------------------------------------
#include "InputManager.h"
#include <windowsx.h> // for proper mouse coordinates when working with dual monitors
//--------------------------------------------------------------------------------
CInputManager* CInputManager::s_pInstance = 0;
//--------------------------------------------------------------------------------
CInputManager*  CInputManager::GetInstance()
{
	if (!s_pInstance)
		assert(0); // "call to CInputManager::GetInstance() before being created"
	return s_pInstance;
}
//--------------------------------------------------------------------------------
void CInputManager::CreateInstance()
{
	if(!s_pInstance)
		s_pInstance = new CInputManager();
}
//--------------------------------------------------------------------------------
void CInputManager::DestroyInstance()
{
	if(s_pInstance)
		delete s_pInstance;
	s_pInstance = NULL;
}
//--------------------------------------------------------------------------------
CInputManager::CInputManager()
{
	// clear all the keys and mouse info
	ZeroMemory(&m_Keys[0],256*sizeof(tKey));
	ZeroMemory(&m_tMouse,sizeof(m_tMouse));
	m_bAnyKeyEnabled = false;
}
//--------------------------------------------------------------------------------
CInputManager::~CInputManager()
{
}
//--------------------------------------------------------------------------------
void CInputManager::ActivateKey(const unsigned char VKcode)
{
	if (m_Keys[VKcode].unRefCount == 0)
	{
		// create a key and associated events
		m_ActiveKeys.push_back(VKcode);
		/*EVENTID emptyName;
		emptyName.clear();
		m_ActiveKeyPressEvents.push_back(emptyName);
		m_ActiveKeyDownEvents.push_back(emptyName);
		m_ActiveKeyReleaseEvents.push_back(emptyName);*/
	}

	m_Keys[VKcode].unRefCount++;
}
//--------------------------------------------------------------------------------
void CInputManager::DeActivateKey(const unsigned char VKcode)
{
	//ensure the key is active
	if(m_Keys[VKcode].unRefCount > 0)
	{
		m_Keys[VKcode].unRefCount--;
		if(m_Keys[VKcode].unRefCount == 0)
		{
			for(unsigned int i = 0; i < m_ActiveKeys.size(); i++)
			{
				if(m_ActiveKeys[i] == VKcode)
				{
					// remove from active list
					m_ActiveKeys.erase(m_ActiveKeys.begin() + i);
					// reset actual key
					m_Keys[VKcode].nKeyState = KEY_CLEAR;
					m_Keys[VKcode].fHeldDuration = 0.f;
					// remove event set
					/*m_ActiveKeyPressEvents.erase(m_ActiveKeyPressEvents.begin() + i);
					m_ActiveKeyDownEvents.erase(m_ActiveKeyDownEvents.begin() + i);
					m_ActiveKeyReleaseEvents.erase(m_ActiveKeyReleaseEvents.begin() + i);*/
					return;
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------
//EVENTID CInputManager::BindKeyEvent(const unsigned char VKcode, eKeyStates keyState, EVENTID EventToThrow)
//{	
//	assert(keyState < KEY_PRESS || keyState > KEY_RELEASE);
//	unsigned int unActiveKeyIndex = 0;
//	// locate proper key
//	for(; unActiveKeyIndex < m_ActiveKeys.size(); unActiveKeyIndex++)
//		if(m_ActiveKeys[unActiveKeyIndex] == VKcode)
//			break;
//
//	if(unActiveKeyIndex == m_ActiveKeys.size())
//		ActivateKey(VKcode);
//
//	switch(keyState)
//	{
//	case KEY_PRESS:
//		{
//			if(m_ActiveKeyPressEvents[unActiveKeyIndex].empty())
//				m_ActiveKeyPressEvents[unActiveKeyIndex] = EventToThrow;
//			return m_ActiveKeyPressEvents[unActiveKeyIndex];
//		}
//		break;
//	case KEY_DOWN:
//		{
//			if(m_ActiveKeyDownEvents[unActiveKeyIndex].empty())
//				m_ActiveKeyDownEvents[unActiveKeyIndex] = EventToThrow;
//			return m_ActiveKeyDownEvents[unActiveKeyIndex];
//		}
//		break;
//	case KEY_RELEASE:
//		{
//			if(m_ActiveKeyReleaseEvents[unActiveKeyIndex].empty())
//				m_ActiveKeyReleaseEvents[unActiveKeyIndex] = EventToThrow;
//			return m_ActiveKeyReleaseEvents[unActiveKeyIndex];
//		}
//		break;
//	};
//	return EventToThrow;
//}
////--------------------------------------------------------------------------------
//EVENTID CInputManager::BindKeyPressEvent(const unsigned char VKcode, EVENTID EventToThrow)
//{	
//	unsigned int unActiveKeyIndex = 0;
//	// locate proper key
//	for(; unActiveKeyIndex < m_ActiveKeys.size(); unActiveKeyIndex++)
//		if(m_ActiveKeys[unActiveKeyIndex] == VKcode)
//			break;
//	
//	if(unActiveKeyIndex == m_ActiveKeys.size())
//		ActivateKey(VKcode);
//
//	if(m_ActiveKeyPressEvents[unActiveKeyIndex].empty())
//		m_ActiveKeyPressEvents[unActiveKeyIndex] = EventToThrow;
//	return m_ActiveKeyPressEvents[unActiveKeyIndex];
//}
////--------------------------------------------------------------------------------
//EVENTID CInputManager::BindKeyDownEvent(const unsigned char VKcode, EVENTID EventToThrow)
//{
//	unsigned int unActiveKeyIndex = 0;
//	// locate proper key
//	for(; unActiveKeyIndex < m_ActiveKeys.size(); unActiveKeyIndex++)
//		if(m_ActiveKeys[unActiveKeyIndex] == VKcode)
//			break;
//	
//	if(unActiveKeyIndex == m_ActiveKeys.size())
//		ActivateKey(VKcode);
//
//	if(m_ActiveKeyDownEvents[unActiveKeyIndex].empty())
//		m_ActiveKeyDownEvents[unActiveKeyIndex] = EventToThrow;
//	return m_ActiveKeyDownEvents[unActiveKeyIndex];
//}
////--------------------------------------------------------------------------------
//EVENTID CInputManager::BindKeyReleaseEvent(const unsigned char VKcode, EVENTID EventToThrow)
//{
//	unsigned int unActiveKeyIndex = 0;
//	// locate proper key
//	for(; unActiveKeyIndex < m_ActiveKeys.size(); unActiveKeyIndex++)
//		if(m_ActiveKeys[unActiveKeyIndex] == VKcode)
//			break;
//	
//	if(unActiveKeyIndex == m_ActiveKeys.size())
//		ActivateKey(VKcode);
//
//	if(m_ActiveKeyReleaseEvents[unActiveKeyIndex].empty())
//		m_ActiveKeyReleaseEvents[unActiveKeyIndex] = EventToThrow;
//	return m_ActiveKeyReleaseEvents[unActiveKeyIndex];
//}
////--------------------------------------------------------------------------------
//void CInputManager::UnBindKeyEvent(const unsigned char VKcode, eKeyStates keyState)
//{
//	assert(keyState < KEY_PRESS || keyState > KEY_RELEASE);
//	// locate proper key
//	for(unsigned int i = 0; i < m_ActiveKeys.size(); i++)
//		if(m_ActiveKeys[i] == VKcode)
//		{
//			switch(keyState)
//			{
//			case KEY_PRESS:
//				m_ActiveKeyPressEvents[i].clear();
//				break;
//			case KEY_DOWN:
//				m_ActiveKeyDownEvents[i].clear();
//				break;
//			case KEY_RELEASE:
//				m_ActiveKeyReleaseEvents[i].clear();
//				break;
//			};
//		}
//}
//--------------------------------------------------------------------------------
void CInputManager::KeyDown(const unsigned char VKcode)
{
	if (m_Keys[VKcode].nKeyState == KEY_CLEAR || m_Keys[VKcode].nKeyState == KEY_UP)
		m_Keys[VKcode].nKeyState = KEY_PRESS;

	if (m_bAnyKeyEnabled)
	{
		if (m_LastKeyState.nKeyState != KEY_PRESS || m_ucLastKeyCode != VKcode)
		{
			m_LastKeyState.nKeyState = KEY_PRESS;
			m_ucLastKeyCode = VKcode;
		}
	}
}
//--------------------------------------------------------------------------------
void CInputManager::KeyUp(unsigned char VKcode)
{
	if (m_Keys[VKcode].nKeyState > KEY_CLEAR)
	{
		m_Keys[VKcode].nKeyState = KEY_RELEASE;
		m_Keys[VKcode].fHeldDuration = 0.f;
	}

	if (m_bAnyKeyEnabled)
	{
		if (m_LastKeyState.nKeyState == KEY_CLEAR)
		{
			m_LastKeyState.nKeyState = KEY_UP;
			m_ucLastKeyCode = VKcode;
		}
	}
}
//--------------------------------------------------------------------------------
bool CInputManager::HandleWindowMessages(const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
	switch (uMsg)
	{
		// left mouse button
	case WM_LBUTTONDOWN:
		{
			KeyDown(VK_LBUTTON);
			break;
		}
	case WM_LBUTTONUP:
		{
			KeyUp(VK_LBUTTON);
			break;
		}
		// right mouse button
	case WM_RBUTTONDOWN:
		{
			KeyDown(VK_RBUTTON);
			break;
		}
	case WM_RBUTTONUP:
		{
			KeyUp(VK_RBUTTON);
			break;
		}
		// middle mouse button
	case WM_MBUTTONDOWN:
		{
			KeyDown(VK_MBUTTON);
			break;
		}
	case WM_MBUTTONUP:
		{
			KeyUp(VK_MBUTTON);
			break;
		}
		// X mouse button
	case WM_XBUTTONDOWN:
		{
			switch (LOWORD(wParam))
			{
			case MK_XBUTTON1:
				{
					KeyDown(VK_XBUTTON1);
					break;
				}
			case MK_XBUTTON2:
				{
					KeyDown(VK_XBUTTON2);
					break;
				}
			}
			break;
		}
	case WM_XBUTTONUP:
		{
			switch (wParam)
			{
			case MK_XBUTTON1:
				{
					KeyUp(VK_XBUTTON1);
					break;
				}
			case MK_XBUTTON2:
				{
					KeyUp(VK_XBUTTON2);
					break;
				}
			}
			break;
		}
		// mousewheel
	case WM_MOUSEWHEEL:
		{
			if ((short)(HIWORD(wParam)) > 0)
			{
				ScrollUp();
			}
			else
			{
				ScrollDown();
			}
			break;
		}
		// keyboard
	case WM_KEYDOWN:
		{
			KeyDown(unsigned char(wParam & 0xFF));
			break;
		}
	case WM_KEYUP:
		{
			KeyUp(unsigned char(wParam & 0xFF));
			//if(m_bAnyKeyEnabled)
			//		EventManager()->SendEventInstant("KeyTyped",new CGeneralEventArgs<unsigned char>(unsigned char(wParam & 0xFF)));
			break;
		}
	case WM_MOUSEMOVE:
		{
			m_tMouse.x = GET_X_LPARAM(lParam);
			m_tMouse.y = GET_Y_LPARAM(lParam);
			break;
		}
	case WM_SYSKEYDOWN:
		{
			// full screen
			KeyDown(VK_LMENU);
			KeyDown(unsigned char(wParam & 0xFF));
			if ((unsigned char(wParam & 0xFF)) == VK_RETURN&&!(lParam&0xA0))
			{
				//EventManager()->SendEvent("OnFullScreen",0,CEventManager::eEventInstant);
			}

			else if ((unsigned char(wParam & 0xFF)) == VK_F4&&!(lParam&0xA0))
				PostQuitMessage(0);
			else 
				break;
		}
	case WM_SYSKEYUP:
		{
			if (lParam & 0x2000000)
			{
				KeyUp(VK_LMENU);
				m_Keys[wParam & 0xFF].nKeyState = KEY_DOWN;
			}
			else
			{
				KeyUp(unsigned char(wParam & 0xFF));
			}
			break;
		}
	case WM_NCMOUSELEAVE:
		{
			// reset the mouse buttons if it leaves the window bounds
			KeyUp(VK_LBUTTON);
			KeyUp(VK_RBUTTON);
			KeyUp(VK_MBUTTON);
			KeyUp(VK_XBUTTON1);
			KeyUp(VK_XBUTTON2);
			break;
		}
	default:
		{
			return true;
		}
	}
	return false;
}
//--------------------------------------------------------------------------------
void CInputManager::ResetInput()
{
	for(unsigned int i = 0; i < m_ActiveKeys.size(); i++)
	{
		KeyUp(m_ActiveKeys[i]);
	}
}
//--------------------------------------------------------------------------------
void CInputManager::UpdateKeyStates(const float fDeltaTime)
{
	// any-key handling
	if (m_bAnyKeyEnabled)
	{
		switch (m_LastKeyState.nKeyState)
		{
		case KEY_PRESS:
			{
				//EventManager()->SendEventInstant("AnyKeyPressed",new CGeneralEventArgs<unsigned char>(m_ucLastKeyCode));
				m_LastKeyState.nKeyState = KEY_DOWN;
				m_LastKeyState.fHeldDuration = 0.f;
				break;
			}
		case KEY_DOWN:
			{
				m_LastKeyState.fHeldDuration += fDeltaTime;
				//EventManager()->SendEventInstant("AnyKeyDown",new CGeneralEventArgs2<unsigned char,float>(m_ucLastKeyCode,m_LastKeyState.fHeldDuration));
				break;
			}
		case KEY_UP:
			{
				//EventManager()->SendEventInstant("AnyKeyReleased",new CGeneralEventArgs<unsigned char>(m_ucLastKeyCode));
				m_LastKeyState.nKeyState = KEY_CLEAR;
				break;
			}

		}
	}

	for(unsigned int i = 0; i < m_ActiveKeys.size(); i++)
	{
		switch(m_Keys[m_ActiveKeys[i]].nKeyState)
		{
		case KEY_PRESS:
			{
				m_Keys[m_ActiveKeys[i]].nKeyState = KEY_DOWN;
				//if(!m_ActiveKeyPressEvents[i].empty())
				//	EventManager()->SendEventInstant(m_ActiveKeyPressEvents[i]);
				break;
			}
		case KEY_DOWN:
			{
				m_Keys[m_ActiveKeys[i]].fHeldDuration += fDeltaTime;
				//if(!m_ActiveKeyDownEvents[i].empty())
				//	EventManager()->SendEventInstant(m_ActiveKeyDownEvents[i],new CGeneralEventArgs<float>(m_Keys[m_ActiveKeys[i]].fHeldDuration));
				break;
			}
		case KEY_RELEASE:
			{
				m_Keys[m_ActiveKeys[i]].nKeyState = KEY_UP;
				//if(!m_ActiveKeyReleaseEvents[i].empty())
				//	EventManager()->SendEventInstant(m_ActiveKeyReleaseEvents[i]);
				break;
			}
		}
	}
	// update mouse position as well
	m_tMouse.prevX = m_tMouse.x;
	m_tMouse.prevY = m_tMouse.y;
}
//--------------------------------------------------------------------------------
const CInputManager::eKeyStates CInputManager::GetKeyState(const unsigned char VKcode) const 
{
	return m_Keys[VKcode].nKeyState; 
}
//--------------------------------------------------------------------------------
const bool CInputManager::GetKeyDown(const unsigned char VKcode) const 
{ 
	return (m_Keys[VKcode].nKeyState == KEY_DOWN) ? true : false; 
}
//--------------------------------------------------------------------------------
const bool CInputManager::GetKeyUp(const unsigned char VKcode) const 
{ 
	return (m_Keys[VKcode].nKeyState == KEY_UP) ? true : false; 
}
//--------------------------------------------------------------------------------
const bool CInputManager::GetKeyRelease(const unsigned char VKcode)	const
{ 
	return (m_Keys[VKcode].nKeyState == KEY_RELEASE) ? true : false;
}
//--------------------------------------------------------------------------------
const bool CInputManager::GetKeyPress(const unsigned char VKcode) const 
{ 
	return (m_Keys[VKcode].nKeyState == KEY_PRESS) ? true : false;
}
//--------------------------------------------------------------------------------