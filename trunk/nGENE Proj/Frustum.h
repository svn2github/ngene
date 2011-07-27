/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Frustum.h
Version:	0.08
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FRUSTUM_H_
#define __INC_FRUSTUM_H_


#include "ISerializable.h"
#include "Matrix4x4.h"
#include "Plane.h"
#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	/// Type defining if a shape is contained withing a frustum
	enum CULLING_RESULT
	{
		CR_UNKNOWN,						///< It is not yet known if shape is visible
		CR_VISIBLE,						///< A shape is fully contained
		CR_INTERSECT,					///< A shape intersects frustum
		CR_INVISIBLE					///< A shape is outside frustum
	};

	/** Frustum is the area visible from the camera's eye.
		@remarks
			Although most frequently frustums are used with cameras 
			you can as well use them for other purposes, eg.
			light cones.
		@remarks
			Although as tradinionally frustum class lets you cull the
			invisible geometry, the method itself is not traditional
			at all. It uses 'radar' approach described in GPU Gems 5
			by Frank Puig Placeres in his article titled: "Improved
			Frustum Culling". It is much more effective than regular
			way of doing this as no planes calculus is involved.
	*/
	class nGENEDLL Frustum: public BaseClass
	{
		EXPOSE_TYPE
	private:
		float m_fNearPlane;						///< Near plane distance
		float m_fFarPlane;						///< Far plane distance
		float m_fAspect;						///< Aspect

		float m_FOV;							///< Field of view

		float m_fRFactor;
		float m_fUFactor;

		float m_fRSphereFactor;
		float m_fUSphereFactor;

		// Pointers to the appropriate vectors - in case of View Frustum
		// these are Camera's vectors.
		Vector3 m_vecEyePos;
		Vector3 m_vecForward;
		Vector3 m_vecRight;
		Vector3 m_vecUp;


		/// Frustum projection matrix.
		Matrix4x4 m_matProjection;


		/// Has frustum changed recently
		bool m_bChanged;

	public:
		Frustum();
		virtual ~Frustum();

		void init();
		void cleanup();

		/** Updates the frustum.
			@remarks
				When the frustum is updated all planes are re-calculated
				regarding to the new camera position.
		*/
		void update();

		/// Frustum reset.
		void reset();

		/// Checks if frustum has changed since last time.
		bool hasChanged() const;
		
		/// Checks if a point is inside frustum.
		bool testPoint(const Vector3& _vec);
		/// Checks if a sphere is inside frustum.
		virtual bool testSphere(const Vector3& _centre, float _radius);

		/** Checks if an AABB is inside frustum.
		*/
		virtual CULLING_RESULT testAABB(AABB& _box);


		/// Sets near plane distance.
		virtual void setNear(float _near);
		virtual float getNear() const {return m_fNearPlane;}

		/// Sets far plane distance.
		virtual void setFar(float _far);
		virtual float getFar() const {return m_fFarPlane;}

		/// Sets field of view angle.
		virtual void setFOV(float _fov);
		virtual float getFOV() const {return m_FOV;}

		/// Sets acpect ratio.
		virtual void setAspect(float _aspect);
		virtual float getAspect() const {return m_fAspect;}

		/// Sets all parameters at once.
		virtual void setPerspective(const float _fov, const float _aspect, const float _near, const float _far);
		/// Sets frustum vectors.
		virtual void setVectors(Vector3& _pos, Vector3& _forward, Vector3& _right, Vector3& _up);
		
		Vector3& getEyePos();
		Vector3& getForward();
		Vector3& getRight();
		Vector3& getUp();

		/// Returns frustum projection matrix.
		Matrix4x4& getProjectionMatrix();

		/** Returns 8 points describing the frustum in world space.
			@param
				_points [out] array of points being returned.
		*/
		void getFrustumPoints(Vector4* _points);
		/** Returns 8 points describing the frustum in eye space.
			@param
			_points [out] array of points being returned.
		*/
		void getFrustumPointsLocal(Vector4* _points);

		/// Returns 6 planes describing the frustum in world space.
		void getFrustumPlanes(Plane* _planes);
		/// Returns 6 planes describing the frustum in local space.
		void getFrustumPlanesLocal(Plane* _planes);
	};



	inline Matrix4x4& Frustum::getProjectionMatrix()
	{
		return m_matProjection;
	}
//----------------------------------------------------------------------
	inline Vector3& Frustum::getEyePos()
	{
		return m_vecEyePos;
	}
//----------------------------------------------------------------------
	inline Vector3& Frustum::getForward()
	{
		return m_vecForward;
	}
//----------------------------------------------------------------------
	inline Vector3& Frustum::getRight()
	{
		return m_vecRight;
	}
//----------------------------------------------------------------------
	inline Vector3& Frustum::getUp()
	{
		return m_vecUp;
	}
//----------------------------------------------------------------------
	inline bool Frustum::hasChanged() const
	{
		return m_bChanged;
	}
//----------------------------------------------------------------------
}


#endif