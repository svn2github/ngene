/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIManager.cpp
Version:	0.08
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUIManager.h"
#include "DeviceRender.h"
#include "GUICursor.h"
#include "GUIFont.h"
#include "GUITextField.h"
#include "GUIWindow.h"
#include "ITexture.h"
#include "Material.h"
#include "ScreenOverlay.h"


namespace nGENE
{
	GUIManager::GUIManager(uint _width, uint _height):
		m_pActiveMaterial(NULL),
		m_pInactiveMaterial(NULL),
		m_pOverlay(NULL),
		m_pFont(NULL),
		m_pDragged(NULL),
		m_pControlOver(NULL),
		m_pActiveWindow(NULL),
		m_pActiveCursor(NULL),
		m_nWidth(_width),
		m_nHeight(_height),
		m_bCursor(true)
	{
		GUITextField::fillCharTable();
	}
//----------------------------------------------------------------------
	GUIManager::~GUIManager()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void GUIManager::init()
	{
		ListenerRegistry <DeviceEvent>::addListener(this);
		ListenerRegistry <KeyboardEvent>::addListener(this);
		ListenerRegistry <MouseEvent>::addListener(this);
	}
//----------------------------------------------------------------------
	void GUIManager::reset()
	{
		m_vWindows.clear();
	}
//----------------------------------------------------------------------
	void GUIManager::cleanup()
	{
		ListenerRegistry <DeviceEvent>::removeListener(this);
		ListenerRegistry <KeyboardEvent>::removeListener(this);
		ListenerRegistry <MouseEvent>::removeListener(this);

		NGENE_DELETE(m_pOverlay);
	}
//----------------------------------------------------------------------
	GUIWindow* GUIManager::createWindow(dword _style)
	{
		GUIWindow* result = new GUIWindow(_style);
		
		return result;
	}
//----------------------------------------------------------------------
	void GUIManager::addWindow(GUIWindow* _window, bool _active)
	{
		m_vWindows.push_back(_window);
		if(m_vWindows.size() == 1 || _active)
			m_pActiveWindow = _window;
	}
//----------------------------------------------------------------------
	void GUIManager::removeWindow(GUIWindow* _window)
	{
		vector <GUIWindow*>::iterator iter = m_vWindows.begin();
		vector <GUIWindow*>::iterator iterEnd = m_vWindows.end();
		
		for(iter = m_vWindows.begin(); iter != iterEnd; ++iter)
		{
			if((*iter) == _window)
				break;
		}

		if(m_pActiveWindow == _window)
			m_pActiveWindow = NULL;
		if(m_pDragged == _window)
			m_pDragged = NULL;
		
		if(iter != iterEnd)
			m_vWindows.erase(iter);
	}
//----------------------------------------------------------------------
	void GUIManager::setActiveMaterial(Material* _material)
	{
		m_pActiveMaterial = _material;

		uint nIndex = 0;
		TEXTURE_DESC& desc = _material->getActiveRenderTechnique()->getRenderPass(0).getTextureSampler(nIndex).getTexture()->getTextureDesc(); 
		Vector2 vecOffset(0.5f / (float)desc.width, 0.5f / (float)desc.height);
		m_pOverlay->setOffset(vecOffset);
	}
//----------------------------------------------------------------------
	Material* GUIManager::getActiveMaterial() const
	{
		return m_pActiveMaterial;
	}
//----------------------------------------------------------------------
	void GUIManager::setInactiveMaterial(Material* _material)
	{
		m_pInactiveMaterial = _material;
	}
//----------------------------------------------------------------------
	Material* GUIManager::getInactiveMaterial() const
	{
		return m_pInactiveMaterial;
	}
//----------------------------------------------------------------------
	void GUIManager::handleEvent(const DeviceEvent& _evt)
	{
		if(_evt.type == DET_CREATED)
			onDeviceCreated();
	}
//----------------------------------------------------------------------
	void GUIManager::handleEvent(const KeyboardEvent& _evt)
	{
		if(m_pActiveWindow)
			m_pActiveWindow->handleEvent(_evt);
	}
//----------------------------------------------------------------------
	void GUIManager::handleEvent(const MouseEvent& _evt)
	{
		if(_evt.type == MET_MOVE_X || _evt.type == MET_MOVE_Y)
		{
			if(m_pDragged)
			{
				Vector2 pos(_evt.mousePosition.x - m_pDragged->m_vecAnchor.x,
							_evt.mousePosition.y - m_pDragged->m_vecAnchor.y);
				
				Maths::clamp(pos.x, 0.0f, Renderer::getSingleton().getDisplayMode().width - m_pDragged->getWidth());
				Maths::clamp(pos.y, 0.0f, Renderer::getSingleton().getDisplayMode().height - m_pDragged->getHeight());

				m_pDragged->setPosition(pos);
			}

			// Update cursor position
			if(m_pActiveCursor && m_bCursor)
			{
				m_pActiveCursor->setPosition(_evt.mousePosition);
			}
		}
		else
		{
			if(_evt.type == MET_BUTTON_LEFT)
			{
				if(!(_evt.nvalue & 0x80))
				{
					if(m_pDragged)
						m_pDragged = NULL;
				}

				GUIWindow* pWnd = findWindow(_evt.mousePosition.x,
											 _evt.mousePosition.y);

				m_pActiveWindow = (pWnd ? pWnd : NULL);
			}
		}

		if(m_pActiveWindow)
			m_pActiveWindow->handleEvent(_evt);
	}
//----------------------------------------------------------------------
	void GUIManager::resizeWindows()
	{
		SDisplayMode displayMode = Renderer::getSingleton().getDisplayMode();
		if((m_nWidth != displayMode.width) || (m_nHeight != displayMode.height))
		{
			for(uint i = 0; i < m_vWindows.size(); ++i)
			{
				m_vWindows[i]->changeResolution(m_nWidth,
												m_nHeight,
												displayMode.width,
												displayMode.height);
			}

			m_nWidth = displayMode.width;
			m_nHeight = displayMode.height;
		}
	}
//----------------------------------------------------------------------
	void GUIManager::render()
	{
		if(!m_pFont)
			return;

		resizeWindows();

		//Renderer::getSingleton().setScissorTestEnabled(true);

		// Render inactive windows first
		m_pOverlay->setMaterial(m_pInactiveMaterial);

		m_pFont->begin();

			for(uint i = 0; i < m_vWindows.size(); ++i)
			{
				GUIWindow* pWnd = m_vWindows[i];
				if(m_pActiveWindow == pWnd)
					continue;				

				renderWindow(pWnd);
			}

		// Render active window
		if(m_pActiveWindow)
		{
			m_pOverlay->setMaterial(m_pActiveMaterial);
			renderWindow(m_pActiveWindow);
		}

		m_pFont->end();

		//Renderer::getSingleton().setScissorTestEnabled(false);

		if(m_pActiveCursor && m_bCursor)
		{
			Vector2 vecPos(0.0f, 0.0f);
			m_pActiveCursor->render(m_pOverlay, vecPos);
		}
	}
//----------------------------------------------------------------------
	void GUIManager::renderWindow(GUIWindow* _wnd)
	{
		if(_wnd->isVisible())
		{
			SRect <uint> rect(_wnd->getPosition().x,
							  _wnd->getPosition().y,
							  _wnd->getPosition().x + _wnd->getWidth(),
							  _wnd->getPosition().y + _wnd->getHeight());

			//Renderer::getSingleton().setScissorRectangle(rect);
			_wnd->render(m_pOverlay, _wnd->getPosition());
		}
	}
//----------------------------------------------------------------------
	GUIWindow* GUIManager::findWindow(uint _x, uint _y)
	{
		for(uint i = 0; i < m_vWindows.size(); ++i)
		{
			GUIWindow* pTemp = m_vWindows[i];
			if(_x > pTemp->getPosition().x && _y > pTemp->getPosition().y)
			{
				if((_x < pTemp->getPosition().x + pTemp->getWidth()) && (_y < pTemp->getPosition().y + pTemp->getHeight()))
				{
					if(pTemp->isVisible() && pTemp->isEnabled())
						return pTemp;
				}
			}
		}

		return NULL;
	}
//----------------------------------------------------------------------
	void GUIManager::setActiveWindow(GUIWindow* _window)
	{
		if(!_window)
		{
			m_pActiveWindow = NULL;
			return;
		}

		/// Check if this belongs to the manager
		uint i = 0;
		for(i = 0; i < m_vWindows.size(); ++i)
		{
			if(_window == m_vWindows[i])
				break;
		}

		if(i == m_vWindows.size())
			return;

		m_pActiveWindow = _window;
	}
//----------------------------------------------------------------------
	void GUIManager::onDeviceCreated()
	{
		m_pOverlay = new ScreenOverlayBatched(RECT_FLOAT(0.0f,
														 0.0f,
														 1.0f,
														 1.0f));
	}
//----------------------------------------------------------------------
}