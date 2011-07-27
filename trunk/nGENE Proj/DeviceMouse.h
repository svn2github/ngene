/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		DeviceMouse.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_DEVICEMOUSE_H_
#define __INC_DEVICEMOUSE_H_

#define DIRECTINPUT_VERSION 0x0800


#include <dinput.h>

#include "InputDevice.h"
#include "Vector3.h"


namespace nGENE
{
	/// Type of the mouse event
	enum MOUSE_EVENT_TYPE
	{
		MET_BUTTON_LEFT,
		MET_BUTTON_RIGHT,
		MET_BUTTON_MIDDLE,
		MET_BUTTON_ADDITIONAL_1,
		MET_BUTTON_ADDITIONAL_2,
		MET_BUTTON_ADDITIONAL_3,
		MET_BUTTON_ADDITIONAL_4,
		MET_BUTTON_ADDITIONAL_5,
		MET_MOVE_X,
		MET_MOVE_Y,
		MET_MOVE_Z
	};

	/// Structure describing single mouse event.
	struct MouseEvent
	{
		MOUSE_EVENT_TYPE type;				///< Type of event
		union {
			Real fvalue;
			uint nvalue;
		};

		Vector2 mousePosition;				///< Position of the mouse cursor
	};


	/** Abstract Mouse device class.
		@remarks
			You have to provide own implementation for every
			API used.
	*/
	class nGENEDLL DeviceMouse: public InputDevice
	{
	protected:
		Real m_fSensitivity;					///< Sensitivity of the mouse

	public:
		DeviceMouse();
		virtual ~DeviceMouse();

		/// Creates the mouse device.
		virtual void createDevice()=0;
		/// Retrieves input from the mouse device.
		virtual void retrieveData()=0;

		virtual MouseEvent& getEvent(uint _index)=0;
		virtual dword getEventsCount() const=0;

		/** Sets mouse sensitivity.
			@param
				_value mouse sensitivity should be in 0 - 10 range.
		*/
		void setSensitivity(Real _value);
		Real getSensitivity() const;
	};



	inline void DeviceMouse::setSensitivity(Real _value)
	{
		Maths::clamp <Real>(_value, 0.0f, 10.0f);
		m_fSensitivity = _value;
	}
//----------------------------------------------------------------------
	inline Real DeviceMouse::getSensitivity() const
	{
		return m_fSensitivity;
	}
//----------------------------------------------------------------------
}


#endif