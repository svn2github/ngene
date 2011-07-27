/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIWindow.cpp
Version:	0.08
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUIWindow.h"
#include "GUIFont.h"
#include "GUIManager.h"
#include "ScreenOverlay.h"
#include "TextureManager.h"


namespace nGENE
{
	GUIWindow::GUIWindow(dword _style):
		m_bMovable(true),
		m_bIsDragged(false),
		m_Style(_style),
		m_nPrevTime(0),
		m_nPassedTime(0),
		m_nSlideInTime(1000),
		m_bSlideIn(false)
	{
		if(_style & WND_CAPTION)
		{
			m_vecPosition.y -= 32.0f;
			m_fHeight += 32.0f;

			if(_style & WND_CONTROLS)
			{
				m_btnClose.setPosition(Vector2(10, 2));
				m_btnClose.setHeight(24);
				m_btnClose.setWidth(32);
				m_btnClose.setBorder(true);
				m_btnClose.onClick.bind(this, &GUIWindow::_ButtonClose_Click);

				m_imgClose.setHeight(24);
				m_imgClose.setWidth(24);
				m_imgClose.setPosition(4, 0);
				//m_imgClose.setImage(TextureManager::getSingleton().createTextureFromFile(L"close_icon", L"close_icon.png"));

				m_vControls.push_back(&m_btnClose);
			}
		}
	}
//----------------------------------------------------------------------
	GUIWindow::~GUIWindow()
	{
	}
//----------------------------------------------------------------------
	void GUIWindow::render(ScreenOverlay* _overlay, Vector2& _position)
	{
		// Slide in window
		if(m_bSlideIn)
		{
			dword now = Engine::getSingleton().getTimer().getMilliseconds();
			dword diff = now - m_nPrevTime;
			m_nPassedTime += diff;

			if(m_nPassedTime >= m_nSlideInTime)
			{
				m_nPassedTime = m_nSlideInTime;
				m_bSlideIn = false;
			}

			float fOpacity = Maths::lerp(0.0f, 1.0f, (float)m_nPassedTime / (float)m_nSlideInTime);
			setOpacity(fOpacity);

			m_nPrevTime = now;
		}

		_overlay->begin();

		Colour clr = Colour::COLOUR_WHITE;
		clr.setAlpha(clr.getAlpha() * m_fOpacity);

		Vector2 position = _position;
		float height = m_fHeight;
		if(m_Style & WND_CAPTION)
		{
			_overlay->render(position,
							 m_fWidth,
							 32.0f,
							 SRect<Real>(0.0625f, 0.0625f, 0.125f, 0.09375f),
							 clr);

			clr = Colour::COLOUR_BLACK;
			GUIManager::getSingleton().getFont()->drawText(m_stCaption,
														   position.x,
														   position.y,
														   24,
														   clr);
			position.y += 32.0f;
			height -= 32.0f;
		}

		if(m_Style)
		{
			clr = m_Colour;
			clr.setAlpha(clr.getAlpha() * m_fOpacity);

			_overlay->render(position,
							 m_fWidth,
							 height,
							 SRect<Real>(0.0f, 0.0625f, 0.0625f, 0.125f),
							 clr);
		}

		if(m_Style & WND_BORDER)
		{
			clr = Colour::COLOUR_BLACK;
			clr.setAlpha(clr.getAlpha() * m_fOpacity);

			_overlay->render(Vector2(position.x, position.y), 2.0f, height, SRect<Real>(0.0f, 0.125f, 0.00390625f, 0.1875f), clr);
			_overlay->render(Vector2(position.x + m_fWidth - 2.0f, position.y), 2.0f, height, SRect<Real>(0.0f, 0.125f, 0.00390625f, 0.1875f), clr);
			_overlay->render(Vector2(position.x + 2.0f, position.y + height - 2.0f), m_fWidth - 4.0f, 2.0f, SRect<Real>(0.0f, 0.1875f, 0.0625f, 0.19140625f), clr);
		}


		if(m_Style & WND_CONTROLS)
		{
			if(!m_imgClose.getImage())
			{
				m_imgClose.setImage(TextureManager::getSingleton().createTextureFromFile(L"close_icon", L"close_icon.png"));
				m_btnClose.setImage(&m_imgClose);
				m_btnClose.setPosition(getWidth() - m_btnClose.getWidth() - 10,
									   0);
			}
		}

		_overlay->end();


		// Render all child controls
		_overlay->begin();
			GUIControlContainer::render(_overlay, _position);
		_overlay->end();

		if(onPaint)
			onPaint();
	}
//----------------------------------------------------------------------
	void GUIWindow::addControl(GUIControl* _control)
	{
		GUIControlContainer::addControl(_control);
		
		if(m_Style & WND_CAPTION)
		{
			Vector2 pos = _control->getPosition();
			pos.y += 32.0f;
			_control->setPosition(pos);
		}
	}
//----------------------------------------------------------------------
	void GUIWindow::handleEvent(const KeyboardEvent& _evt)
	{
		if(_evt.isKeyPressed(KC_TAB))
		{
			int i = 0;
			int count = static_cast<int>(m_vControls.size());
			for(i = 0; i < count; ++i)
			{
				if(m_vControls[i] == m_pFocusedControl)
					break;
			}

			if(_evt.isKeyDown(KC_LSHIFT))
			{
				if(--i < 0)
					i = count - 1;
			}
			else
			{
				if(++i == count)
					i = 0;
			}

			setFocusedControl(m_vControls[i]);
		}

		if(m_pFocusedControl)
			m_pFocusedControl->keyboardDown(_evt);
	}
//----------------------------------------------------------------------
	void GUIWindow::handleEvent(const MouseEvent& _evt)
	{
		Real x = _evt.mousePosition.x;
		Real y = _evt.mousePosition.y;

		GUIControl* pControl = findControl(x, y);
		GUIControl* pLast = GUIManager::getSingleton().getControlOver();

		if(pControl != pLast)
		{
			if(pControl)
			{
				pControl->mouseRollIn(x, y);
				pControl->setState(STATE_MOUSE_OVER);
			}
			if(pLast)
			{
				pLast->mouseRollOut(x, y);
				pLast->setState(STATE_NONE);
			}

			GUIManager::getSingleton().setControlOver(pControl ? pControl : this);
		}


		if(_evt.type == MET_BUTTON_LEFT)
		{
			if(_evt.nvalue & 0x80)
			{
				if(m_bMovable && (m_Style & WND_CAPTION) && !pControl)
				{
					if(y <= m_vecPosition.y + 32.0f)
					{
						m_vecAnchor.x = x - m_vecPosition.x;
						m_vecAnchor.y = y - m_vecPosition.y;
						drag();
					}
				}
				else
				{
					if(pControl)
					{
						pControl->mouseDown(x, y);
						pControl->setState(STATE_MOUSE_DOWN);
					}
				}
			}
			else
			{
				if(pControl)
				{
					if(pControl->getState() == STATE_MOUSE_DOWN)
					{
						pControl->mouseClick(x, y);
						pControl->setState(STATE_NONE);

						setFocusedControl(pControl);
					}
					pControl->mouseRelease(x, y);
				}
				else
					setFocusedControl(NULL);
				
				if(m_bIsDragged)
					drop();
			}
		}
		else if(_evt.type == MET_MOVE_X || _evt.type == MET_MOVE_Y)
		{
			if(pControl)
				pControl->mouseOver(x, y);
		}
	}
//----------------------------------------------------------------------
	GUIControl* GUIWindow::findControl(uint _x, uint _y)
	{
		// We're going from top control down
		for(uint i = m_vControls.size() - 1; i > 0; --i)
		{
			GUIControl* pTemp = m_vControls[i];
			Vector2 position = pTemp->getPosition() + getPosition();
			if(_x > position.x && _y > position.y)
			{
				if((_x < position.x + pTemp->getWidth()) && (_y < position.y + pTemp->getHeight()))
				{
					if(pTemp->isVisible() && pTemp->isEnabled())
						return pTemp;
				}
			}
		}

		return NULL;
	}
//----------------------------------------------------------------------
	void GUIWindow::drag()
	{
		m_bIsDragged = true;
		GUIManager::getSingleton().drag(this);
	}
//----------------------------------------------------------------------
	void GUIWindow::drop()
	{
		m_bIsDragged = false;
		//GUIManager::getSingleton().drag(NULL);
	}
//----------------------------------------------------------------------
	void GUIWindow::activate()
	{
		GUIManager::getSingleton().setActiveWindow(this);
	}
//----------------------------------------------------------------------
	void GUIWindow::_ButtonClose_Click(uint _x, uint _y)
	{
		setVisible(false);
	}
//----------------------------------------------------------------------
	void GUIWindow::slideIn(dword _slideTime)
	{
		show();

		m_nSlideInTime = _slideTime;
		m_nPassedTime = 0;
		m_nPrevTime = Engine::getSingleton().getTimer().getMilliseconds();
		m_bSlideIn = true;
	}
//----------------------------------------------------------------------
	void GUIWindow::activateOrDeactivate()
	{
		GUIManager* mgr = GUIManager::getSingletonPtr();
		if(!mgr)
			return;

		if(m_bVisible)
		{
			GUIWindow* pActive = mgr->getActiveWindow();
			if(!pActive)
				mgr->setActiveWindow(this);
		}
		else
		{
			GUIWindow* pActive = mgr->getActiveWindow();
			if(pActive == this)
				mgr->setActiveWindow(NULL);
		}
	}
//----------------------------------------------------------------------
	void GUIWindow::setVisible(bool _value)
	{
		GUIControl::setVisible(_value);

		activateOrDeactivate();
	}
//----------------------------------------------------------------------
	void GUIWindow::toggleVisible()
	{
		GUIControl::toggleVisible();

		activateOrDeactivate();
	}
//----------------------------------------------------------------------
	void GUIWindow::show()
	{
		GUIControl::show();

		GUIManager* mgr = GUIManager::getSingletonPtr();
		if(!mgr)
			return;

		GUIWindow* pActive = mgr->getActiveWindow();
		if(!pActive)
			mgr->setActiveWindow(this);
	}
//----------------------------------------------------------------------
	void GUIWindow::hide()
	{
		GUIControl::hide();

		GUIManager* mgr = GUIManager::getSingletonPtr();
		if(!mgr)
			return;

		GUIWindow* pActive = mgr->getActiveWindow();
		if(pActive == this)
			mgr->setActiveWindow(NULL);
	}
//----------------------------------------------------------------------
}