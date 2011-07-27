/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScreenOverlayBatchedBatched.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SCREENOVERLAYBATCHED_H_
#define __INC_SCREENOVERLAYBATCHED_H_


#include "BaseClass.h"
#include "Prerequisities.h"
#include "ScreenOverlay.h"
#include "TypeInfo.h"
#include "UtilitiesTypes.h"
#include "Vector2.h"


namespace nGENE
{
	/** Class for efficient rendering of many overlays at once. Batched overlays 
		share materials and some of the settings.
	*/
	class nGENEDLL ScreenOverlayBatched: public ScreenOverlay
	{
		EXPOSE_TYPE
	private:
		uint m_nCurrentIndex;					///< What is the index of current element?
		uint m_nGSMask;
		CULL_MODE m_OldCullMode;
		FILL_MODE m_OldFillMode;

		IndexedBuffer* m_pIVB;					///< Buffer holding index data

		SVertex2D* m_pVertices;					///< Pointer to vertex data
		ushort* m_pIndices;						///< Pointer to index data

		const uint m_kBatchSize;				/**< What is the batch size? */

		Vector2 m_vecOffset;					///< Offset for UV coordinates


		void lock();
		void unlock();
		void flush();


		ScreenOverlayBatched(const ScreenOverlayBatched& src);
		ScreenOverlayBatched& operator=(const ScreenOverlayBatched& rhs);

	public:
		ScreenOverlayBatched(const RECT_FLOAT& _rect, uint _batchSize=300);
		ScreenOverlayBatched(float _x, float _y, float _width, float _height,
							 uint _batchSize=300);
		virtual ~ScreenOverlayBatched();

		virtual void init();
		virtual void cleanup();

		virtual void begin();
		virtual void end();

		/** Sets offset for UV coordinates.
			@par
				This might be useful if you want to sample in the texels'
				centres instead on the 'border'.
		*/
		virtual void setOffset(const Vector2& _offset);

		/// This functions do nothing in case of batched overlay.
		virtual void render();
		virtual void render(const Vector2& _position, Real _width, Real _height,
							const SRect<Real>& _src,
							const Colour& _colour=Colour::COLOUR_WHITE);
	};



	inline void ScreenOverlayBatched::setOffset(const Vector2& _offset)
	{
		m_vecOffset = _offset;
	}
//----------------------------------------------------------------------
}


#endif