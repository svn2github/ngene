/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointDistance.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "JointDistance.h"
#include "JointEnumerator.h"
#include "PhysXMapping.h"
#include "PhysicsActor.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxScene.h"
	#include "NxDistanceJointDesc.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo JointDistance::Type(L"JointDistance", &Joint::Type);


	JointDistance::JointDistance(NxScene* _scene, SJointDesc& _desc):
		Joint(_scene, _desc),
		m_pDistanceDesc(new NxDistanceJointDesc())
	{
	}
//----------------------------------------------------------------------
	JointDistance::~JointDistance()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void JointDistance::init()
	{
		m_pDistanceDesc->actor[0] = m_JointDesc.actor1 ? m_JointDesc.actor1->getActor() : NULL;
		m_pDistanceDesc->actor[1] = m_JointDesc.actor2 ? m_JointDesc.actor2->getActor() : NULL;
		m_pDistanceDesc->localAnchor[0] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localAnchor1);
		m_pDistanceDesc->localAnchor[1] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localAnchor2);
		m_pDistanceDesc->localAxis[0] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localDirection1);
		m_pDistanceDesc->localAxis[1] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localDirection2);
		SDistanceJointDesc& desc = ((SDistanceJointDesc&)m_JointDesc);
		m_pDistanceDesc->minDistance = desc.minDistance;
		m_pDistanceDesc->maxDistance = desc.maxDistance;
		m_pDistanceDesc->spring.spring = desc.spring.springCoeff;
		m_pDistanceDesc->spring.damper = desc.spring.damperCoeff;
		m_pDistanceDesc->flags = desc.flags;


		if(!m_pDistanceDesc->isValid())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Distance joint description is invalid");
			return;
		}

		if(m_JointDesc.globalDirection != Vector3::ZERO_VECTOR)
			m_pDistanceDesc->setGlobalAxis(PhysXMapping::vector3ToNxVec3(m_JointDesc.globalDirection));
		if(m_JointDesc.globalAnchor != Vector3::ZERO_VECTOR)
			m_pDistanceDesc->setGlobalAnchor(PhysXMapping::vector3ToNxVec3(m_JointDesc.globalAnchor));

		m_pJoint = m_pScene->createJoint(*m_pDistanceDesc);
	}
//----------------------------------------------------------------------
	void JointDistance::cleanup()
	{
		NGENE_DELETE(m_pDistanceDesc);
	}
//----------------------------------------------------------------------
	NxJointDesc* JointDistance::getJointDesc()
	{
		return (static_cast<NxJointDesc*>(m_pDistanceDesc));
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	JointDistanceFactory::JointDistanceFactory()
	{
	}
//----------------------------------------------------------------------
	JointDistanceFactory::~JointDistanceFactory()
	{
	}
//----------------------------------------------------------------------
	Joint* JointDistanceFactory::createJoint(NxScene* _scene, JOINT_DESC& _desc)
	{
		return (new JointDistance(_scene, _desc));
	}
//----------------------------------------------------------------------
}