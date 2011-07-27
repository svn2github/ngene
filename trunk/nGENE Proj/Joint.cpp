/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Joint.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Joint.h"
#include "PhysXMapping.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxJoint.h"
	#include "NxScene.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo Joint::Type(L"Joint", &BaseClass::Type);


	Joint::Joint(NxScene* _scene, SJointDesc& _desc):
		m_pJoint(NULL),
		m_pScene(_scene),
		m_JointDesc(_desc)
	{
	}
//----------------------------------------------------------------------
	Joint::~Joint()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void Joint::init()
	{
	}
//----------------------------------------------------------------------
	void Joint::cleanup()
	{
		m_pScene->releaseJoint(*m_pJoint);
	}
//----------------------------------------------------------------------
	NxJoint* Joint::getJoint() const
	{
		return m_pJoint;
	}
//----------------------------------------------------------------------
	void Joint::setPosition(const Vector3& _vec)
	{
		m_pJoint->setGlobalAnchor(PhysXMapping::vector3ToNxVec3(_vec));
	}
//----------------------------------------------------------------------
	void Joint::setBreakable(Real _force, Real _torque)
	{
		m_pJoint->setBreakable(_force, _torque);
	}
//----------------------------------------------------------------------
	void Joint::getBreakable(Real& _force, Real& _torque)
	{
		m_pJoint->getBreakable(_force, _torque);
	}
//----------------------------------------------------------------------
	void Joint::setDirection(const Vector3& _direction)
	{
		m_pJoint->setGlobalAxis(PhysXMapping::vector3ToNxVec3(_direction));
	}
//----------------------------------------------------------------------
	Vector3 Joint::getDirection()
	{
		return (PhysXMapping::nxVec3ToVector3(m_pJoint->getGlobalAxis()));
	}
//----------------------------------------------------------------------
	bool Joint::isBroken()
	{
		if(m_pJoint->getState() == NX_JS_BROKEN)
			return true;
		else
			return false;
	}
//----------------------------------------------------------------------
}