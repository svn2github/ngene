/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointPointOnLine.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "JointPointOnLine.h"
#include "JointEnumerator.h"
#include "PhysXMapping.h"
#include "PhysicsActor.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxScene.h"
	#include "NxPointOnLineJointDesc.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo JointPointOnLine::Type(L"JointPointOnLine", &Joint::Type);


	JointPointOnLine::JointPointOnLine(NxScene* _scene, SJointDesc& _desc):
		Joint(_scene, _desc),
		m_pPointOnLineDesc(new NxPointOnLineJointDesc())
	{
	}
//----------------------------------------------------------------------
	JointPointOnLine::~JointPointOnLine()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void JointPointOnLine::init()
	{
		m_pPointOnLineDesc->actor[0] = m_JointDesc.actor1 ? m_JointDesc.actor1->getActor() : NULL;
		m_pPointOnLineDesc->actor[1] = m_JointDesc.actor2 ? m_JointDesc.actor2->getActor() : NULL;
		m_pPointOnLineDesc->localAnchor[0] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localAnchor1);
		m_pPointOnLineDesc->localAnchor[1] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localAnchor2);
		m_pPointOnLineDesc->localAxis[0] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localDirection1);
		m_pPointOnLineDesc->localAxis[1] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localDirection2);

		if(!m_pPointOnLineDesc->isValid())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Point-on-Line joint description is invalid");
			return;
		}

		if(m_JointDesc.globalDirection != Vector3::ZERO_VECTOR)
			m_pPointOnLineDesc->setGlobalAxis(PhysXMapping::vector3ToNxVec3(m_JointDesc.globalDirection));
		if(m_JointDesc.globalAnchor != Vector3::ZERO_VECTOR)
			m_pPointOnLineDesc->setGlobalAnchor(PhysXMapping::vector3ToNxVec3(m_JointDesc.globalAnchor));

		m_pJoint = m_pScene->createJoint(*m_pPointOnLineDesc);
	}
//----------------------------------------------------------------------
	void JointPointOnLine::cleanup()
	{
		NGENE_DELETE(m_pPointOnLineDesc);
	}
//----------------------------------------------------------------------
	NxJointDesc* JointPointOnLine::getJointDesc()
	{
		return (static_cast<NxJointDesc*>(m_pPointOnLineDesc));
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	JointPointOnLineFactory::JointPointOnLineFactory()
	{
	}
//----------------------------------------------------------------------
	JointPointOnLineFactory::~JointPointOnLineFactory()
	{
	}
//----------------------------------------------------------------------
	Joint* JointPointOnLineFactory::createJoint(NxScene* _scene, JOINT_DESC& _desc)
	{
		return (new JointPointOnLine(_scene, _desc));
	}
//----------------------------------------------------------------------
}