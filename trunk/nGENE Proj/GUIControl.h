/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIControl.h
Version:	0.08
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUICONTROL_H_
#define __INC_GUICONTROL_H_


#include "Colour.h"
#include "FastDelegate.h"
#include "Prerequisities.h"
#include "TypeInfo.h"
#include "UtilitiesTypes.h"
#include "Vector2.h"
#include "Thread.h"


namespace nGENE
{
	/// Enumeration describing control state.
	enum CONTROL_STATE
	{
		STATE_NONE,
		STATE_MOUSE_OVER,
		STATE_MOUSE_DOWN
	};

	/** Part of a gui widget graphical representation.
		@par
			Each widget type consists of at least one element but may
			contain arbitrary number of them.
	*/
	struct SGUIElement
	{
		SRect <Real> coordinates;				///< Element texture coordinates
		Vector2 position;						///< Position in widget local space
	};

	/** Base class for all GUI elements.
		@remarks
			Events are handled using "delegates mechanism" provided by
			Don Clugston's library (available on-line for free at
			http://www.codeproject.com/KB/cpp/FastDelegate.aspx).
	*/
	class nGENEDLL GUIControl
	{
		EXPOSE_TYPE
	protected:
		GUIControl* m_pParent;				///< Parental control

		Colour m_Colour;					///< Fore colour of the control
		Colour m_MouseOverColour;			///< Fore colour of the control when mouse pointer is over it

		Vector2 m_vecPosition;				///< Position on the screen

		float m_fHeight;					///< Height of the control
		float m_fWidth;						///< Width of the control
		float m_fOpacity;					///< Opacity level (1.0 by default)

		uint m_nTabIndex;					///< Tab stop index

		bool m_bEnabled;					///< Is control enabled?
		bool m_bVisible;					///< Is control visible?
		bool m_bHasFocus;					///< Has control focus?
		bool m_bHasTabStop;					///< Is tabstop enabled for this control?

		CONTROL_STATE m_State;				///< State of the control

		/// Graphical elements of the widget
		static vector <SGUIElement> m_vElements;


		void renderElements(ScreenOverlay* _overlay);


	public:
		// Here go delegates
		typedef fastdelegate::FastDelegate2 <uint, uint> ON_CLICK_DELEGATE;
		typedef fastdelegate::FastDelegate0 <> ON_PAINT_DELEGATE;
		typedef fastdelegate::FastDelegate1 <const KeyboardEvent&> ON_KEYBOARD_DELEGATE;

		// Mouse events
		ON_CLICK_DELEGATE onClick;
		ON_CLICK_DELEGATE onMouseDown;
		ON_CLICK_DELEGATE onMouseRollIn;
		ON_CLICK_DELEGATE onMouseRollOut;
		ON_CLICK_DELEGATE onMouseOver;
		ON_CLICK_DELEGATE onMouseRelease;

		// Keyboard events
		ON_KEYBOARD_DELEGATE onKeyboard;

		// Other events
		ON_PAINT_DELEGATE onPaint;
		ON_PAINT_DELEGATE onFocus;
		ON_PAINT_DELEGATE onLostFocus;
	
	public:
		GUIControl();
		virtual ~GUIControl();

		virtual void changeResolution(uint _oldWidth, uint _oldHeight,
			uint _width, uint _height);

		/// Sets control height.
		virtual void setHeight(Real _height);
		/// Returns control height.
		Real getHeight() const;

		/// Sets control width.
		virtual void setWidth(Real _width);
		/// Returns control width.
		Real getWidth() const;

		/** Sets size of the control.
			@param
				_size size of the control:
				* x component - width.
				* y component - height.
		*/
		virtual void setSize(const Vector2& _size);
		/// Sets size of the control.
		virtual void setSize(float _width, float _height);

		/// Sets position of this control within container.
		virtual void setPosition(const Vector2& _position);
		/// Sets position of this control within container.
		virtual void setPosition(Real _x, Real _y);
		/// Returns position of this control relative to its container.
		Vector2& getPosition();
		/// Returns absolute position of this control.
		Vector2 getAbsolutePosition();

		/// Sets parent of this control.
		virtual void setParent(GUIControl* _parent);
		/// Returns parent control of this control.
		GUIControl* getParent() const;

		/// Sets control face colour.
		void setColour(const Colour& _colour);
		/// Returns control face colour.
		Colour& getColour();

		/// Sets control face colour when mouse is over control.
		void setMouseOverColour(const Colour& _colour);
		/// Returns control face colour when mouse is over control.
		Colour& getMouseOverColour();

		/// Specifies whether the control is enabled or not.
		virtual void setEnabled(bool _value);
		/// Toggles enabled state of this control.
		virtual void toggleEnabled();
		/// Checks if this control is enabled.
		virtual bool isEnabled() const;

		/// Sets visibility of the control.
		virtual void setVisible(bool _value);
		/// Toggles visibility of the control.
		virtual void toggleVisible();
		/// Shows control.
		virtual void show();
		/// Hides control.
		virtual void hide();
		/// Checks if this control is visible.
		bool isVisible() const;

		/// Checks if control has focus.
		bool hasFocus() const;

		void setTabStop(bool _value);
		bool hasTabStop() const;

		void setTabIndex(uint _index);
		uint getTabIndex() const;

		/** Sets opacity of the control.
			@remarks
				It doesn't change the control colour.
		*/
		virtual void setOpacity(float _opacity);
		/// Returns opacity of the control.
		float getOpacity() const;

		/// Renders the control on the screen
		virtual void render(ScreenOverlay* _overlay, Vector2& _position)=0;

		/// Generates a key down event.
		virtual void keyboardDown(const KeyboardEvent& _evt);

		virtual void mouseDown(uint _x, uint _y);
		virtual void mouseClick(uint _x, uint _y);
		virtual void mouseRollIn(uint _x, uint _y);
		virtual void mouseRollOut(uint _x, uint _y);
		virtual void mouseOver(uint _x, uint _y);
		virtual void mouseRelease(uint _x, uint _y);

		virtual void gettingFocus();
		virtual void loosingFocus();

		void setState(CONTROL_STATE _state);
		CONTROL_STATE getState() const;

		/// Add GUI element for this class.
		static void addGUIElement(SGUIElement& _element);
	};



	inline void GUIControl::setHeight(Real _height)
	{
		m_fHeight = _height;
	}
//----------------------------------------------------------------------
	inline Real GUIControl::getHeight() const
	{
		return m_fHeight;
	}
//----------------------------------------------------------------------
	inline void GUIControl::setWidth(Real _width)
	{
		m_fWidth = _width;
	}
//----------------------------------------------------------------------
	inline Real GUIControl::getWidth() const
	{
		return m_fWidth;
	}
//----------------------------------------------------------------------
	inline void GUIControl::setPosition(const Vector2& _position)
	{
		m_vecPosition = _position;
	}
//----------------------------------------------------------------------
	inline void GUIControl::setPosition(Real _x, Real _y)
	{
		m_vecPosition.x = _x;
		m_vecPosition.y = _y;
	}
//----------------------------------------------------------------------
	inline Vector2& GUIControl::getPosition()
	{
		return m_vecPosition;
	}
//----------------------------------------------------------------------
	inline void GUIControl::setParent(GUIControl* _parent)
	{
		m_pParent = _parent;
	}
//----------------------------------------------------------------------
	inline GUIControl* GUIControl::getParent() const
	{
		return m_pParent;
	}
//----------------------------------------------------------------------
	inline void GUIControl::setEnabled(bool _value)
	{
		m_bEnabled = _value;
	}
//----------------------------------------------------------------------
	inline void GUIControl::toggleEnabled()
	{
		m_bEnabled = m_bEnabled ? false : true;
	}
//----------------------------------------------------------------------
	inline bool GUIControl::isEnabled() const
	{
		return m_bEnabled;
	}
//----------------------------------------------------------------------
	inline void GUIControl::setVisible(bool _value)
	{
		m_bVisible = _value;
	}
//----------------------------------------------------------------------
	inline void GUIControl::toggleVisible()
	{
		m_bVisible = m_bVisible ? false : true;
	}
//----------------------------------------------------------------------
	inline void GUIControl::show()
	{
		m_bVisible = true;
	}
//----------------------------------------------------------------------
	inline void GUIControl::hide()
	{
		m_bVisible = false;
	}
//----------------------------------------------------------------------
	inline bool GUIControl::isVisible() const
	{
		return m_bVisible;
	}
//----------------------------------------------------------------------
	inline bool GUIControl::hasFocus() const
	{
		return m_bHasFocus;
	}
//----------------------------------------------------------------------
	inline void GUIControl::setTabStop(bool _value)
	{
		m_bHasTabStop = _value;
	}
//----------------------------------------------------------------------
	inline bool GUIControl::hasTabStop() const
	{
		return m_bHasTabStop;
	}
//----------------------------------------------------------------------
	inline void GUIControl::setTabIndex(uint _index)
	{
		m_nTabIndex = _index;
	}
//----------------------------------------------------------------------
	inline uint GUIControl::getTabIndex() const
	{
		return m_nTabIndex;
	}
//----------------------------------------------------------------------
	inline void GUIControl::setColour(const Colour& _colour)
	{
		m_Colour = _colour;
	}
//----------------------------------------------------------------------
	inline Colour& GUIControl::getColour()
	{
		return m_Colour;
	}
//----------------------------------------------------------------------
	inline void GUIControl::setMouseOverColour(const Colour& _colour)
	{
		m_MouseOverColour = _colour;
	}
//----------------------------------------------------------------------
	inline Colour& GUIControl::getMouseOverColour()
	{
		return m_MouseOverColour;
	}
//----------------------------------------------------------------------
	inline void GUIControl::keyboardDown(const KeyboardEvent& _evt)
	{
		if(onKeyboard)
			onKeyboard(_evt);
	}
//----------------------------------------------------------------------
	inline void GUIControl::mouseDown(uint _x, uint _y)
	{
		if(onMouseDown)
			onMouseDown(_x, _y);
	}
//----------------------------------------------------------------------
	inline void GUIControl::mouseClick(uint _x, uint _y)
	{
		if(onClick)
			onClick(_x, _y);
	}
//----------------------------------------------------------------------
	inline void GUIControl::mouseOver(uint _x, uint _y)
	{
		if(onMouseOver)
			onMouseOver(_x, _y);
	}
//----------------------------------------------------------------------
	inline void GUIControl::mouseRollIn(uint _x, uint _y)
	{
		if(onMouseRollIn)
			onMouseRollIn(_x, _y);
	}
//----------------------------------------------------------------------
	inline void GUIControl::mouseRollOut(uint _x, uint _y)
	{
		if(onMouseRollOut)
			onMouseRollOut(_x, _y);
	}
//----------------------------------------------------------------------
	inline void GUIControl::mouseRelease(uint _x, uint _y)
	{
		if(onMouseRelease)
			onMouseRelease(_x, _y);
	}
//----------------------------------------------------------------------
	inline void GUIControl::gettingFocus()
	{
		if(onFocus)
			onFocus();
	}
//----------------------------------------------------------------------
	inline void GUIControl::loosingFocus()
	{
		if(onLostFocus)
			onLostFocus();
	}
//----------------------------------------------------------------------
	inline void GUIControl::setState(CONTROL_STATE _state)
	{
		m_State = _state;
	}
//----------------------------------------------------------------------
	inline CONTROL_STATE GUIControl::getState() const
	{
		return m_State;
	}
//----------------------------------------------------------------------
	inline void GUIControl::setSize(const Vector2& _size)
	{
		setWidth(_size.x);
		setHeight(_size.y);
	}
//----------------------------------------------------------------------
	inline void GUIControl::setSize(float _width, float _height)
	{
		setWidth(_width);
		setHeight(_height);
	}
//----------------------------------------------------------------------
	inline void GUIControl::setOpacity(float _opacity)
	{
		m_fOpacity = _opacity;
	}
//----------------------------------------------------------------------
	inline float GUIControl::getOpacity() const
	{
		return m_fOpacity;
	}
//----------------------------------------------------------------------
}


#endif