/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointSphere.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "JointSphere.h"
#include "JointEnumerator.h"
#include "PhysXMapping.h"
#include "PhysicsActor.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxScene.h"
	#include "NxSphericalJointDesc.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo JointSphere::Type(L"JointSphere", &Joint::Type);


	JointSphere::JointSphere(NxScene* _scene, SJointDesc& _desc):
		Joint(_scene, _desc),
		m_pSphereDesc(new NxSphericalJointDesc())
	{
	}
//----------------------------------------------------------------------
	JointSphere::~JointSphere()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void JointSphere::init()
	{
		m_pSphereDesc->actor[0] = m_JointDesc.actor1 ? m_JointDesc.actor1->getActor() : NULL;
		m_pSphereDesc->actor[1] = m_JointDesc.actor2 ? m_JointDesc.actor2->getActor() : NULL;
		m_pSphereDesc->localAnchor[0] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localAnchor1);
		m_pSphereDesc->localAnchor[1] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localAnchor2);
		m_pSphereDesc->localAxis[0] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localDirection1);
		m_pSphereDesc->localAxis[1] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localDirection2);

		SSphereJointDesc& desc = ((SSphereJointDesc&)m_JointDesc);
		m_pSphereDesc->swingAxis = PhysXMapping::vector3ToNxVec3(desc.swingAxis);
		m_pSphereDesc->flags = desc.flags;

		m_pSphereDesc->twistLimit.high.value = desc.twistLimit.highValue;
		m_pSphereDesc->twistLimit.high.restitution = desc.twistLimit.highRestitution;
		m_pSphereDesc->twistLimit.low.value = desc.twistLimit.lowValue;
		m_pSphereDesc->twistLimit.low.restitution = desc.twistLimit.lowRestitution;

		m_pSphereDesc->swingLimit.value = desc.swingLimit.value;
		m_pSphereDesc->swingLimit.restitution = desc.swingLimit.restitution;

		m_pSphereDesc->twistSpring.spring = desc.twistSpring.springCoeff;
		m_pSphereDesc->twistSpring.damper = desc.twistSpring.damperCoeff;

		m_pSphereDesc->swingSpring.spring = desc.swingSpring.springCoeff;
		m_pSphereDesc->swingSpring.damper = desc.swingSpring.damperCoeff;

		m_pSphereDesc->projectionDistance = desc.projectionDistance;
		m_pSphereDesc->projectionMode = NX_JPM_POINT_MINDIST;

		if(!m_pSphereDesc->isValid())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Spherical joint description is invalid");
			return;
		}

		if(m_JointDesc.globalDirection != Vector3::ZERO_VECTOR)
			m_pSphereDesc->setGlobalAxis(PhysXMapping::vector3ToNxVec3(m_JointDesc.globalDirection));
		if(m_JointDesc.globalAnchor != Vector3::ZERO_VECTOR)
			m_pSphereDesc->setGlobalAnchor(PhysXMapping::vector3ToNxVec3(m_JointDesc.globalAnchor));

		m_pJoint = m_pScene->createJoint(*m_pSphereDesc);
	}
//----------------------------------------------------------------------
	void JointSphere::cleanup()
	{
		NGENE_DELETE(m_pSphereDesc);
	}
//----------------------------------------------------------------------
	NxJointDesc* JointSphere::getJointDesc()
	{
		return (static_cast<NxJointDesc*>(m_pSphereDesc));
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	JointSphereFactory::JointSphereFactory()
	{
	}
//----------------------------------------------------------------------
	JointSphereFactory::~JointSphereFactory()
	{
	}
//----------------------------------------------------------------------
	Joint* JointSphereFactory::createJoint(NxScene* _scene, JOINT_DESC& _desc)
	{
		return (new JointSphere(_scene, _desc));
	}
//----------------------------------------------------------------------
}