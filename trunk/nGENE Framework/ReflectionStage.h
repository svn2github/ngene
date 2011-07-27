/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ReflectionStage.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_REFLECTIONSTAGE_H_
#define __INC_REFLECTIONSTAGE_H_


#include "HashTable.h"
#include "RenderStage.h"
#include "Prerequisities.h"
#include "Plane.h"


namespace nGENE
{
namespace Nature
{
	/** Stage for rendering shadows.
		@par
			In this stage, reflection is performed. Note that
			this feature is external to the nGENE's core.
		@par
			Only stages addind through addRenderStage() method
			are reflected. So you have to add manually ones which
			you want to reflect. Order of adding them is crucial,
			so make sure it is correct.
	*/
	class ReflectionStage: public RenderStage
	{
	private:
		uint m_nReflectionMapSize;

		ScreenOverlay* m_pScreenQuad;					///< Quad for rendering post-process effects
		Material* m_pReflectionMaterial;				///< Pointer to the reflection material

		Real m_fReflectionDistance;						///< Reflection distance
		Plane m_Plane;									///< Reflection plane

		static int s_nRequestsNum;						///< Number of requests

		ITexture* m_pReflectionMap;
		ITexture* m_pBackBuffer;

		bool m_bIsReady;


		void prepare();

	public:
		/** Creates reflection stage.
			@param
				_textureSize size of the square reflection map being used. The larger
				the map, the higher quality of the reflections and lower performance.
				Defaults to 512.
		*/
		ReflectionStage(uint _textureSize=512);
		~ReflectionStage();

		void init();

		void render();

		/** Sets reflection plane.
			@remarks
				A plane has to be normalized i.e. |a + b + c| = 1.
		*/
		void setReflectionPlane(Real _a, Real _b, Real _c, Real _d);

		/// Sets reflections distance.
		void setReflectionDistance(Real _distance);


		/// Requests reflection (increases internal counter).
		static void requestReflection();
		/// Release reflection (decreases internal counter).
		static void releaseReflection();
	};



	inline void ReflectionStage::setReflectionPlane(Real _a, Real _b, Real _c, Real _d)
	{
		m_Plane.setPlane(_a, _b, _c, _d);
	}
//----------------------------------------------------------------------
	inline void ReflectionStage::setReflectionDistance(Real _distance)
	{
		m_fReflectionDistance = _distance;
	}
//----------------------------------------------------------------------
	inline void ReflectionStage::requestReflection()
	{
		++s_nRequestsNum;
	}
//----------------------------------------------------------------------
	inline void ReflectionStage::releaseReflection()
	{
		--s_nRequestsNum;
	}
//----------------------------------------------------------------------
}
}


#endif