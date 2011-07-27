/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ShadowStage.h
Version:	0.06
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SHADOWSTAGE_H_
#define __INC_SHADOWSTAGE_H_


#include "HashTable.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "RenderStage.h"
#include "Prerequisities.h"
#include "Vector3.h"


namespace nGENE
{
namespace Nature
{
	/** Stage for rendering shadows.
		@remarks
			In this stage, shadow mapping is performed. Note that
			this feature is external to the nGENE's core.
		@par
			For directional lights orthogonal projection is being used.
	*/
	class ShadowStage: public RenderStage
	{
	private:
		uint m_nShadowMapPrevID;						///< ID of the last shadow map

		Material* m_pShadowMapMaterial;					///< Pointer to the shadow mapping material
		Material* m_pShadowMaterial;					///< Pointer to the shadow material
		ScreenOverlay* m_pScreenQuad;					///< Quad for rendering post-process effects

		Matrix4x4 m_matTexAdj;							///< Texture matrix
		Matrix4x4 m_matViewOld;							///< Old view matrix

		ShaderConstant* m_pDepthThreshold;				///< Depth limit
		ShaderConstant* m_pShadowMatrix;				///< Shadow matrix
		ShaderConstant* m_pTexelSize;					///< Size of the texel

		Vector3 m_vecShadowOffset;						///< Shadow offset

		HashTable <uint, ITexture*> m_pShadowMaps;		///< Pointers to shadow maps

		float m_fRange;
		float m_fDistance;


		/** Renders shadow depth map (or shadow map for short) 
			to a texture.
		*/
		void renderShadowMap();
		/// Renders shadow for given params.
		void renderShadow(Vector3& _pos, Vector3& _forward, Vector3& _right, Vector3& _up,
						  NodeLight* _light, uint _counter=0);

	public:
		/// Creates shadow stage.
		ShadowStage();
		~ShadowStage();

		void init();

		void render();

		/** Sets shadowing technique.
			@remarks
				At present there are a few requirements for valid shadow material.
				The most important is that in 0th render pass in pixel shader 
				there must be constant named "matTexture". It is responsible for 
				projecting shadow onto the scene. It is defined as standard 
				(Light View Projection Matrix) * (Texture Matrix).
		*/
		void setShadowMaterial(Material* _material);

		/// Sets range for directional lights.
		void setRange(Real _range);
		/// Returns range for directional lights.
		Real getRange() const;

		/// Sets depth range for directional lights.
		void setDistance(Real _distance);
		/// Returns depth range for directional lights.
		Real getDistance() const;

		/// Sets shadow offset.
		void setShadowOffset(const Vector3& _offset);
		/// Returns shadow offset.
		Vector3& getShadowOffset();
	};



	inline void ShadowStage::setRange(Real _range)
	{
		m_fRange = _range;
	}
//----------------------------------------------------------------------
	inline Real ShadowStage::getRange() const
	{
		return m_fRange;
	}
//----------------------------------------------------------------------
	inline void ShadowStage::setDistance(Real _distance)
	{
		m_fDistance = _distance;
	}
//----------------------------------------------------------------------
	inline Real ShadowStage::getDistance() const
	{
		return m_fDistance;
	}
//----------------------------------------------------------------------
	inline void ShadowStage::setShadowOffset(const Vector3& _offset)
	{
		m_vecShadowOffset = _offset;
	}
//----------------------------------------------------------------------
	inline Vector3& ShadowStage::getShadowOffset()
	{
		return m_vecShadowOffset;
	}
//----------------------------------------------------------------------
}
}


#endif