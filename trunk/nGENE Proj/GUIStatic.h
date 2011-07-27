/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIStatic.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_GUISTATIC_H_
#define __INC_GUISTATIC_H_


#include "GUIText.h"
#include "Prerequisities.h"


namespace nGENE
{
	/** Static text control.
		@remarks
			Static text, also known as label, cannot be changed
			by user directly.
	*/
	class nGENEDLL GUIStatic: public GUIText
	{
	public:
		GUIStatic();
		virtual ~GUIStatic();

		virtual void render(ScreenOverlay* _overlay, Vector2& _position);
		
		/// Sets caption of this label.
		void setCaption(const wstring& _caption);
		/// Returns caption of this label.
		const wstring& getCaption() const;
	};



	inline void GUIStatic::setCaption(const wstring& _caption)
	{
		m_stText = _caption;
	}
//----------------------------------------------------------------------
	inline const wstring& GUIStatic::getCaption() const
	{
		return m_stText;
	}
//----------------------------------------------------------------------
}


#endif