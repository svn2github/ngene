/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeVolumetric.h
Version:	0.06
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_NODEVOLUMETRIC_H_
#define __INC_NODEVOLUMETRIC_H_


#include "Frustum.h"
#include "AABB.h"
#include "NodeVisible.h"
#include "OBB.h"
#include "Prerequisities.h"
#include "TypeInfo.h"

#include "NodeDebugFrustum.h"

using nGENE::Debug::NodeDebugFrustum;


namespace nGENE
{
namespace Nature
{
	/** Volumetric light shaft.
		@par
			The method used to render light shafts is slicing planes
			technique.
		@remarks
			Even though it is possible to add it as a child of any node
			type, it is highly recommended to bind it to the NodeLight
			as when deserializing it is assumed this way.
	*/
	class NodeVolumetric: public NodeVisible
	{
		EXPOSE_TYPE
	protected:
		Frustum m_Frustum;					///< Light frustum
		VertexBuffer* m_pVB;				///< Pointer to the VertexBuffer object
		Material* m_pVolumetricMat;			///< Pointer to the Material object
		NodeLight* m_pLight;				///< Light to which this volume is attached
		NodeDebugFrustum* m_pDebug;			///< Debug frustum

		Colour m_Colour;					///< Colour of the shaft

		AABB m_AABB;						///< Volumetric effect helper AABB
		OBB m_OBB;							///< Volumetric effect helper OBB

		uint m_nMaxSlices;					///< Maximum number of slices

		dword m_dwPrevTime;

		Real m_fFarPlane;					///< Far plane distance

		Matrix4x4 m_matLightView;			///< Light view matrix
		Matrix4x4 m_matLightProj;			///< Light projection matrix
		Matrix4x4 m_matFrustumWorld;
		Matrix4x4 m_matTexture;				///< Projective texturing matrix

		Vector4 m_ptFrustum[8];				///< Frustum points

		bool m_bIsShadowUsed;				///< Is shadow used?
		bool m_bIsCookieUsed;				///< Is cookie/gobo used?
		bool m_bIsNoiseUsed;				///< Is scrolling noise used?
		bool m_bShowDebug;					///< Is debug frustum enabled?


		void beginRender();
		void endRender();
		void updateAABB();

		dword mapShellToColourChannel(uint _shell);

		/// Updates volumetric effect.
		void onUpdate();

	public:
		NodeVolumetric(NodeLight* _light, uint _slicesNum=100);
		NodeVolumetric(const NodeVolumetric& src);
		NodeVolumetric(uint _slicesNum=100);
		~NodeVolumetric();
		NodeVolumetric& operator=(const NodeVolumetric& rhs);

		void init();

		/// Sets light.
		void setLight(NodeLight* _light);
		/// Returns light associated with this volumetric effect.
		NodeLight* getLight() const;

		/// Sets far plane distance.
		void setFar(Real _far);
		/// Returns far plane distance.
		Real getFar() const;

		/// Specifies whether shadow map is used.
		void setShadowUsed(bool _value);
		/// Checks if shadow map is used.
		bool isShadowUsed() const;

		/// Specifies whether cookie/gobo texture is used.
		void setCookieUsed(bool _value);
		/// Checks if cookie texture is used.
		bool isCookieUsed() const;

		/// Specifies whether noise texture is used.
		void setNoiseUsed(bool _value);
		/// Checks if noise texture is used.
		bool isNoiseUsed() const;

		/// Specifies whether debug object is visible.
		void setShowDebug(bool _value);
		bool isShowDebug() const;

		/// Sets colour of the light shaft.
		void setColour(const Colour& _colour);
		/// Returns colour of the light shaft.
		Colour& getColour();


		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};



	inline NodeLight* NodeVolumetric::getLight() const
	{
		return m_pLight;
	}
//----------------------------------------------------------------------
	inline Real NodeVolumetric::getFar() const
	{
		return m_fFarPlane;
	}
//----------------------------------------------------------------------
	inline void NodeVolumetric::setShadowUsed(bool _value)
	{
		m_bIsShadowUsed = _value;
	}
//----------------------------------------------------------------------
	inline bool NodeVolumetric::isShadowUsed() const
	{
		return m_bIsShadowUsed;
	}
//----------------------------------------------------------------------
	inline void NodeVolumetric::setCookieUsed(bool _value)
	{
		m_bIsCookieUsed = _value;
	}
//----------------------------------------------------------------------
	inline bool NodeVolumetric::isCookieUsed() const
	{
		return m_bIsCookieUsed;
	}
//----------------------------------------------------------------------
	inline void NodeVolumetric::setNoiseUsed(bool _value)
	{
		m_bIsNoiseUsed = _value;
	}
//----------------------------------------------------------------------
	inline bool NodeVolumetric::isNoiseUsed() const
	{
		return m_bIsNoiseUsed;
	}
//----------------------------------------------------------------------
	inline bool NodeVolumetric::isShowDebug() const
	{
		return m_bShowDebug;
	}
//----------------------------------------------------------------------
	inline void NodeVolumetric::setColour(const Colour& _colour)
	{
		m_Colour = _colour;
	}
//----------------------------------------------------------------------
	inline Colour& NodeVolumetric::getColour()
	{
		return m_Colour;
	}
//----------------------------------------------------------------------
}
}


#endif