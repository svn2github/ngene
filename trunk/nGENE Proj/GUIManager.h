/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIManager.h
Version:	0.05
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUIMANAGER_H_
#define __INC_GUIMANAGER_H_


#include "DeviceKeyboard.h"
#include "DeviceMouse.h"
#include "Listener.h"
#include "Prerequisities.h"
#include "ScreenOverlayBatched.h"
#include "Singleton.h"


namespace nGENE
{
	/** Manages the GUI
		@remarks
			Through this class you can add and remove gui windows
			and handle incoming events.
	*/
	class nGENEDLL GUIManager: public Singleton <GUIManager>,
		public Listener <KeyboardEvent>,
		public Listener <MouseEvent>,
		public Listener <DeviceEvent>
	{
	private:
		vector <GUIWindow*> m_vWindows;

		Material* m_pActiveMaterial;
		Material* m_pInactiveMaterial;

		ScreenOverlayBatched* m_pOverlay;
		GUIFont* m_pFont;
		GUIWindow* m_pDragged;							///< Window being dragged
		GUIControl* m_pControlOver;						///< Control with mouse pointer over it

		GUIWindow* m_pActiveWindow;
		GUICursor* m_pActiveCursor;

		/// Resolution of the screen
		uint m_nWidth;
		uint m_nHeight;

		bool m_bCursor;									///< Is cursor enabled?


		void renderWindow(GUIWindow* _wnd);
		void onDeviceCreated();

		void resizeWindows();

	public:
		GUIManager(uint _width=800, uint _height=600);
		~GUIManager();

		void setBaseGUISize(uint _width, uint _height);

		void init();
		void reset();
		void cleanup();

		/// Creates new window.
		GUIWindow* createWindow(dword _style);

		/// Sets active window.
		void setActiveWindow(GUIWindow* _window);
		/// Returns active window.
		GUIWindow* getActiveWindow() const;

		/// Adds new window.
		void addWindow(GUIWindow* _window, bool _ative=false);
		/// Returns window with the given index.
		GUIWindow* getWindow(uint _index) const;
		/// Removes given window.
		void removeWindow(GUIWindow* _window);

		void setActiveMaterial(Material* _material);
		Material* getActiveMaterial() const;

		void setInactiveMaterial(Material* _material);
		Material* getInactiveMaterial() const;

		void setFont(GUIFont* _font);
		GUIFont* getFont() const;

		void handleEvent(const KeyboardEvent& _evt);
		void handleEvent(const MouseEvent& _evt);
		void handleEvent(const DeviceEvent& _evt);

		/// Sets active cursor.
		void setCursor(GUICursor* _cursor);
		/// Returns active cursor.
		GUICursor* getCursor() const;

		/// Specifies whether cursor is enabled.
		void setCursorEnabled(bool _value);
		/// Checks if the cursor is enabled.
		bool isCursorEnabled() const;


		/** Finds window under given point.
			@return
				GUIWindow* pointer to the enabled and visible window
				or null if no such window was found.
		*/
		GUIWindow* findWindow(uint _x, uint _y);

		void drag(GUIWindow* _control);

		void setControlOver(GUIControl* _control);
		GUIControl* getControlOver() const;

		/// Renders GUI.
		void render();
	};



	inline GUIWindow* GUIManager::getWindow(uint _index) const
	{
		return m_vWindows[_index];
	}
//----------------------------------------------------------------------
	inline void GUIManager::setFont(GUIFont* _font)
	{
		m_pFont = _font;
	}
//----------------------------------------------------------------------
	inline GUIFont* GUIManager::getFont() const
	{
		return m_pFont;
	}
//----------------------------------------------------------------------
	inline void GUIManager::drag(GUIWindow* _control)
	{
		m_pDragged = _control;
	}
//----------------------------------------------------------------------
	inline void GUIManager::setControlOver(GUIControl* _control)
	{
		m_pControlOver = _control;
	}
//----------------------------------------------------------------------
	inline GUIControl* GUIManager::getControlOver() const
	{
		return m_pControlOver;
	}
//----------------------------------------------------------------------
	inline void GUIManager::setCursor(GUICursor* _cursor)
	{
		m_pActiveCursor = _cursor;
	}
//----------------------------------------------------------------------
	inline GUICursor* GUIManager::getCursor() const
	{
		return m_pActiveCursor;
	}
//----------------------------------------------------------------------
	inline void GUIManager::setCursorEnabled(bool _value)
	{
		m_bCursor = _value;
	}
//----------------------------------------------------------------------
	inline bool GUIManager::isCursorEnabled() const
	{
		return m_bCursor;
	}
//----------------------------------------------------------------------
	inline GUIWindow* GUIManager::getActiveWindow() const
	{
		return m_pActiveWindow;
	}
//----------------------------------------------------------------------
	inline void GUIManager::setBaseGUISize(uint _width, uint _height)
	{
		m_nWidth = _width;
		m_nHeight = _height;
	}
//----------------------------------------------------------------------
}


#endif