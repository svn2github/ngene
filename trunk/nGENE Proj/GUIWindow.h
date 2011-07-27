/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIWindow.h
Version:	0.08
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUIWINDOW_H_
#define __INC_GUIWINDOW_H_


#include "GUIControl.h"
#include "GUIControlContainer.h"
#include "GUIButton.h"
#include "GUIImage.h"
#include "Prerequisities.h"


namespace nGENE
{
	/// Enumeration of window style
	enum WINDOW_STYLE
	{
		WND_TRANSPARENT = 0,
		WND_BORDER		= (1<<0),
		WND_CAPTION		= (1<<1),
		WND_CONTROLS	= (1<<2)
	};

	/** Window control.
		@remarks
			All controls must belong to one window or another. Windows can
			be child of other windows as well.
	*/
	class nGENEDLL GUIWindow: public GUIControlContainer
	{
	private:
		GUIButton m_btnClose;						///< Close button
		GUIImage m_imgClose;						///< Close icon

		wstring m_stCaption;						///< Caption of the element

		bool m_bMovable;							///< Can window be moved?
		bool m_bIsDragged;							///< Is window currently dragged?
		bool m_bSlideIn;							///< Is sliding in?

		dword m_Style;								///< Style of the window

		dword m_nPrevTime;
		dword m_nPassedTime;
		dword m_nSlideInTime;						///< Time of sliding in


		void _ButtonClose_Click(uint _x, uint _y);
		void activateOrDeactivate();

	public:
		GUIWindow(dword _style);
		virtual ~GUIWindow();

		virtual void addControl(GUIControl* _control);
		virtual void render(ScreenOverlay* _overlay, Vector2& _position);

		virtual void handleEvent(const KeyboardEvent& _evt);
		virtual void handleEvent(const MouseEvent& _evt);

		/** Finds control under given point.
			@return
				GUIControl* pointer to the enabled and visible control
				or null if no such control was found.
		*/
		virtual GUIControl* findControl(uint _x, uint _y);

		/// Sets visibility of the control.
		void setVisible(bool _value);
		/// Toggles visibility of the control.
		void toggleVisible();

		/// Shows control.
		void show();
		/// Hides control.
		void hide();

		/// Shows a window by sliding the window in.
		void slideIn(dword _slideTime=1000);

		/// Makes this window active (having focus).
		void activate();

		/// Sets window bar caption.
		void setCaption(const wstring& _caption);
		/// Returns window bar caption.
		const wstring& getCaption() const;

		/// Specifies whether this window can be dragged.
		void setMovable(bool _value);
		/// Checks if this window can be dragged.
		bool isMovable() const;


		/// @todo I don't like this...
		void drag();
		void drop();
		Vector2 m_vecAnchor;
		///
	};



	inline void GUIWindow::setCaption(const wstring& _caption)
	{
		m_stCaption = _caption;
	}
//----------------------------------------------------------------------
	inline const wstring& GUIWindow::getCaption() const
	{
		return m_stCaption;
	}
//----------------------------------------------------------------------
	inline void GUIWindow::setMovable(bool _value)
	{
		m_bMovable = _value;
	}
//----------------------------------------------------------------------
	inline bool GUIWindow::isMovable() const
	{
		return m_bMovable;
	}
//----------------------------------------------------------------------
}


#endif