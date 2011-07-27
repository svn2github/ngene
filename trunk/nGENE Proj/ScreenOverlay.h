/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScreenOverlay.h
Version:	0.06
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SCREENOVERLAY_H_
#define __INC_SCREENOVERLAY_H_


#include "BaseClass.h"
#include "Prerequisities.h"
#include "TypeInfo.h"
#include "UtilitiesTypes.h"
#include "Vector2.h"


namespace nGENE
{
	/** Base class for screen overlay.
		@remarks
			Screen overlay is a 2D shape drawn on the screen,
			in front of any geometry. Moreover, screen overlay's
			position and size are defined in screen space.
	*/
	class nGENEDLL ScreenOverlay: public BaseClass, public IRenderable
	{
		EXPOSE_TYPE
	protected:
		Vector2 m_vecPosition;				///< Position of the overlay's top-left corner
		float m_fWidth;						///< Width of the overlay
		float m_fHeight;					///< Height of the overlay

		uint m_nGSMask;
		CULL_MODE m_OldCullMode;
		FILL_MODE m_OldFillMode;
		
		VertexBuffer* m_pVB;				///< It holds overlay vertices

		Material* m_pMaterial;				///< Pointer to material data

	public:
		ScreenOverlay(const RECT_FLOAT& _rect);
		ScreenOverlay(float _x, float _y, float _width, float _height);
		virtual ~ScreenOverlay();

		virtual void init();
		virtual void cleanup();

		virtual void begin();
		virtual void end();

		virtual void render();
		virtual void render(const Vector2& _position, Real _width, Real _height,
							const SRect<Real>& _src,
							const Colour& _colour=Colour::COLOUR_WHITE);

		void setMaterial(Material* _material);
		Material* getMaterial() const;

		void setPosition(const Vector2& _position);

		void setWidth(float _width);
		void setHeight(float _height);
	};



	inline void ScreenOverlay::setPosition(const Vector2& _position)
	{
		m_vecPosition = _position;
	}
//----------------------------------------------------------------------
	inline void ScreenOverlay::setWidth(float _width)
	{
		m_fWidth = _width;
	}
//----------------------------------------------------------------------
	inline void ScreenOverlay::setHeight(float _height)
	{
		m_fHeight = _height;
	}
//----------------------------------------------------------------------
}


#endif