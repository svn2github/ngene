/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Frustum.cpp
Version:	0.13
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Frustum.h"
#include "AABB.h"
#include "ISerializer.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo Frustum::Type(L"Frustum", &BaseClass::Type);


	Frustum::Frustum():
		m_fNearPlane(1.0f),
		m_fFarPlane(1000.0f),
		m_fAspect(4.0f / 3.0f),
		m_FOV(Maths::PI / 4.0f),
		m_vecEyePos(Vector3::ZERO_VECTOR),
		m_vecForward(Vector3::UNIT_Z),
		m_vecRight(Vector3::UNIT_X),
		m_vecUp(Vector3::UNIT_Y),
		m_bChanged(true)
	{
		init();
	}
//----------------------------------------------------------------------
	Frustum::~Frustum()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void Frustum::init()
	{
		setPerspective(m_FOV, m_fAspect, m_fNearPlane, m_fFarPlane);
		update();
	}
//----------------------------------------------------------------------
	void Frustum::cleanup()
	{
	}
//----------------------------------------------------------------------
	void Frustum::update()
	{
		// No need to update
		if(!m_bChanged)
			return;

		// Operate on frustum projection matrix
		m_matProjection.perspective(m_FOV, m_fAspect, m_fNearPlane, m_fFarPlane);

		// Set frustum perspective
		setPerspective(m_FOV, m_fAspect, m_fNearPlane, m_fFarPlane);

		m_bChanged = false;
	}
//----------------------------------------------------------------------
	void Frustum::reset()
	{
		m_bChanged = true;
	}
//----------------------------------------------------------------------
	void Frustum::setPerspective(const float _fov, const float _aspect, const float _near, const float _far)
	{
		m_FOV = _fov;
		m_fAspect = _aspect;
		m_fNearPlane = _near;
		m_fFarPlane = _far;

		// Calculate additional factors
		m_fUFactor = Maths::ftan(m_FOV * 0.5f);
		m_fRFactor = m_fUFactor * m_fAspect;

		m_fUSphereFactor = 1.0f / Maths::fcos(m_FOV * 0.5f);
		m_fRSphereFactor = 1.0f / Maths::fcos(Maths::fatan(m_fRFactor));

		m_bChanged = true;
	}
//----------------------------------------------------------------------
	void Frustum::setNear(float _near)
	{
		m_fNearPlane = _near;
		m_bChanged = true;
	}
//----------------------------------------------------------------------
	void Frustum::setFar(float _far)
	{
		m_fFarPlane = _far;
		m_bChanged = true;
	}
//----------------------------------------------------------------------
	void Frustum::setFOV(float _fov)
	{
		m_FOV = _fov;
		m_bChanged = true;
	}
//----------------------------------------------------------------------
	void Frustum::setAspect(float _aspect)
	{
		m_fAspect = _aspect;
		m_bChanged = true;
	}
//----------------------------------------------------------------------
	bool Frustum::testPoint(const Vector3 &_vec)
	{
		Vector3 OP = _vec - m_vecEyePos;

		// Test against near/far plane
		float forward = OP.dotProduct(m_vecForward);
		if((forward < m_fNearPlane) || (forward > m_fFarPlane))
			return false;

		// Test against right/left plane
		float right = OP.dotProduct(m_vecRight);
		float rLimit = m_fRFactor * forward;
		if((right < -rLimit) || (right > rLimit))
			return false;

		// Test against up/bottom plane
		float up = OP.dotProduct(m_vecUp);
		float uLimit = m_fUFactor * forward;
		if((up < -uLimit) || (up > uLimit))
			return false;


		// Point is visible
		return true;
	}
//----------------------------------------------------------------------
	bool Frustum::testSphere(const Vector3& _centre, float _radius)
	{
		Vector3 OP = _centre - (m_vecEyePos);

		// Test against near/far plane
		float forward = OP.dotProduct(m_vecForward);
		if((forward < m_fNearPlane - _radius) || (forward > m_fFarPlane + _radius))
			return false;

		// Test against right/left plane
		float d = _radius * m_fRSphereFactor;
		float right = OP.dotProduct(m_vecRight);
		float rLimit = m_fRFactor * forward + d;
		if((right < -rLimit) || (right > rLimit))
			return false;

		// Test against up/bottom plane
		d = _radius * m_fUSphereFactor;
		float up = OP.dotProduct(m_vecUp);
		float uLimit = m_fUFactor * forward + d;
		if((up < -uLimit) || (up > uLimit))
			return false;

		// Sphere is visible
		return true;
	}
//----------------------------------------------------------------------
	CULLING_RESULT Frustum::testAABB(AABB& _box)
	{
		// First do an early-out test by approximating AABB is a sphere
		Vector3 vecRadius = _box.getCentre() - _box.getMaximum();
		float fRadius = vecRadius.getLength();

		if(!testSphere(_box.getCentre(), fRadius))
			return CR_INVISIBLE;


		// Now test the box itself
		const Vector3& vecMin = _box.getMinimum();
		const Vector3& vecMax = _box.getMaximum();

		Vector3 P;
		uint nOutOfLeft  = 0;
		uint nOutOfRight = 0;
		uint nOutOfFar	 = 0;
		uint nOutOfNear	 = 0;
		uint nOutOfTop	 = 0;
		uint nOutOfBottom= 0;
		bool bIsInRightTest;
		bool bIsInUpTest;
		bool bIsInFrontTest;

		Vector3 vecCorners[2];
		vecCorners[0] = vecMin - (m_vecEyePos);
		vecCorners[1] = vecMax - (m_vecEyePos);

		for(uint i = 0; i < 8; ++i)
		{
			bIsInRightTest = bIsInUpTest = bIsInFrontTest = false;
			P.x = vecCorners[i & 1].x;
			P.y = vecCorners[(i >> 2) & 1].y;
			P.z = vecCorners[(i >> 1) & 1].z;

			float r = m_vecRight.dotProduct(P);
			float u = m_vecUp.dotProduct(P);
			float f = m_vecForward.dotProduct(P);

			if(r < -m_fRFactor * f)
				++nOutOfLeft;
			else if (r > m_fRFactor * f)
				++nOutOfRight;
			else
				bIsInRightTest = true;
			
			if(u < -m_fUFactor * f)
				++nOutOfBottom;
			else if(u > m_fUFactor * f)
				++nOutOfTop;
			else bIsInUpTest = true;

			if(f < m_fNearPlane)
				++nOutOfNear;		
			else if(f > m_fFarPlane)
				++nOutOfFar;
			else
				bIsInFrontTest = true;

			// Box is contained within all limits
			if(bIsInRightTest && bIsInFrontTest && bIsInUpTest)
				return CR_VISIBLE;
		}

		if(nOutOfLeft == 8 || nOutOfRight == 8 || nOutOfFar == 8 ||
		   nOutOfNear == 8 || nOutOfTop == 8 || nOutOfBottom == 8)
		{
			return CR_INVISIBLE;
		}
		else if(!nOutOfLeft && !nOutOfRight &&
				!nOutOfFar && !nOutOfNear &&
				!nOutOfTop && !nOutOfBottom)
		{
			return CR_VISIBLE;
		}
		else
		{
			return CR_INTERSECT;
		}
	}
//----------------------------------------------------------------------
	void Frustum::setVectors(Vector3& _pos, Vector3& _forward, Vector3& _right, Vector3& _up)
	{
		m_vecEyePos = _pos;
		m_vecForward = _forward;
		m_vecRight = _right;
		m_vecUp = _up;

		m_bChanged = true;
	}
//----------------------------------------------------------------------
	void Frustum::getFrustumPoints(nGENE::Vector4* _points)
	{
		Matrix4x4 matView;
		matView.lookAt(m_vecEyePos,
					   m_vecEyePos + m_vecForward,
					   m_vecUp);

		getFrustumPointsLocal(_points);

		matView.invert();
		matView.transpose();

		// Transform all points by the frustum matrix
		for(uint i = 0; i < 8; ++i)
			_points[i] = matView.multiply(_points[i]);
	}
//----------------------------------------------------------------------
	void Frustum::getFrustumPointsLocal(nGENE::Vector4* _points)
	{
		float tanHalfFOV = Maths::ftan(m_FOV * 0.5f);

		float y = m_fNearPlane * tanHalfFOV;
		float x = m_fAspect * y;
		_points[0].set(-x, -y, m_fNearPlane, 1.0f);
		_points[1].set(x, -y, m_fNearPlane, 1.0f);
		_points[2].set(x, y, m_fNearPlane, 1.0f);
		_points[3].set(-x, y, m_fNearPlane, 1.0f);

		y = m_fFarPlane * tanHalfFOV;
		x = m_fAspect * y;
		_points[4].set(-x, -y, m_fFarPlane, 1.0f);
		_points[5].set(x, -y, m_fFarPlane, 1.0f);
		_points[6].set(x, y, m_fFarPlane, 1.0f);
		_points[7].set(-x, y, m_fFarPlane, 1.0f);
	}
//----------------------------------------------------------------------
	void Frustum::getFrustumPlanes(Plane* _planes)
	{
		Vector4 temp[8];
		getFrustumPoints(temp);

		Vector3 points[8];
		for(uint i = 0; i < 8; ++i)
			points[i].set(temp[i].x, temp[i].y, temp[i].z);


		// Near
		_planes[0].setPlane(points[0], points[1], points[2]);
		// Far
		_planes[1].setPlane(points[4], points[6], points[5]);
		// Left
		_planes[2].setPlane(points[0], points[3], points[4]);
		// Right
		_planes[3].setPlane(points[1], points[5], points[2]);
		// Up
		_planes[4].setPlane(points[2], points[6], points[3]);
		// Down
		_planes[5].setPlane(points[0], points[4], points[1]);
	}
//----------------------------------------------------------------------
	void Frustum::getFrustumPlanesLocal(Plane* _planes)
	{
		Vector4 temp[8];
		getFrustumPointsLocal(temp);

		Vector3 points[8];
		for(uint i = 0; i < 8; ++i)
			points[i].set(temp[i].x, temp[i].y, temp[i].z);


		// Near
		_planes[0].setPlane(points[0], points[1], points[2]);
		// Far
		_planes[1].setPlane(points[4], points[6], points[5]);
		// Left
		_planes[2].setPlane(points[0], points[3], points[4]);
		// Right
		_planes[3].setPlane(points[1], points[5], points[2]);
		// Up
		_planes[4].setPlane(points[2], points[6], points[3]);
		// Down
		_planes[5].setPlane(points[0], points[4], points[1]);
	}
	//----------------------------------------------------------------------
}