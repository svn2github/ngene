/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointPointInPlane.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "JointPointInPlane.h"
#include "JointEnumerator.h"
#include "PhysXMapping.h"
#include "PhysicsActor.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxScene.h"
	#include "NxPointInPlaneJointDesc.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo JointPointInPlane::Type(L"JointPointInPlane", &Joint::Type);


	JointPointInPlane::JointPointInPlane(NxScene* _scene, SJointDesc& _desc):
		Joint(_scene, _desc),
		m_pPointInPlaneDesc(new NxPointInPlaneJointDesc())
	{
	}
//----------------------------------------------------------------------
	JointPointInPlane::~JointPointInPlane()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void JointPointInPlane::init()
	{
		m_pPointInPlaneDesc->actor[0] = m_JointDesc.actor1 ? m_JointDesc.actor1->getActor() : NULL;
		m_pPointInPlaneDesc->actor[1] = m_JointDesc.actor2 ? m_JointDesc.actor2->getActor() : NULL;
		m_pPointInPlaneDesc->localAnchor[0] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localAnchor1);
		m_pPointInPlaneDesc->localAnchor[1] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localAnchor2);
		m_pPointInPlaneDesc->localAxis[0] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localDirection1);
		m_pPointInPlaneDesc->localAxis[1] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localDirection2);

		if(!m_pPointInPlaneDesc->isValid())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Point-in-Plane joint description is invalid");
			return;
		}

		if(m_JointDesc.globalDirection != Vector3::ZERO_VECTOR)
			m_pPointInPlaneDesc->setGlobalAxis(PhysXMapping::vector3ToNxVec3(m_JointDesc.globalDirection));
		if(m_JointDesc.globalAnchor != Vector3::ZERO_VECTOR)
			m_pPointInPlaneDesc->setGlobalAnchor(PhysXMapping::vector3ToNxVec3(m_JointDesc.globalAnchor));

		m_pJoint = m_pScene->createJoint(*m_pPointInPlaneDesc);
	}
//----------------------------------------------------------------------
	void JointPointInPlane::cleanup()
	{
		NGENE_DELETE(m_pPointInPlaneDesc);
	}
//----------------------------------------------------------------------
	NxJointDesc* JointPointInPlane::getJointDesc()
	{
		return (static_cast<NxJointDesc*>(m_pPointInPlaneDesc));
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	JointPointInPlaneFactory::JointPointInPlaneFactory()
	{
	}
//----------------------------------------------------------------------
	JointPointInPlaneFactory::~JointPointInPlaneFactory()
	{
	}
//----------------------------------------------------------------------
	Joint* JointPointInPlaneFactory::createJoint(NxScene* _scene, JOINT_DESC& _desc)
	{
		return (new JointPointInPlane(_scene, _desc));
	}
//----------------------------------------------------------------------
}