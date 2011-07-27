/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointFixed.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "JointFixed.h"
#include "JointEnumerator.h"
#include "PhysXMapping.h"
#include "PhysicsActor.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxScene.h"
	#include "NxFixedJointDesc.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo JointFixed::Type(L"JointFixed", &Joint::Type);


	JointFixed::JointFixed(NxScene* _scene, SJointDesc& _desc):
		Joint(_scene, _desc),
		m_pFixedDesc(new NxFixedJointDesc())
	{
	}
//----------------------------------------------------------------------
	JointFixed::~JointFixed()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void JointFixed::init()
	{
		m_pFixedDesc->actor[0] = m_JointDesc.actor1 ? m_JointDesc.actor1->getActor() : NULL;
		m_pFixedDesc->actor[1] = m_JointDesc.actor2 ? m_JointDesc.actor2->getActor() : NULL;

		if(!m_pFixedDesc->isValid())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Fixed joint description is invalid");
			return;
		}

		if(m_JointDesc.globalDirection != Vector3::ZERO_VECTOR)
			m_pFixedDesc->setGlobalAxis(PhysXMapping::vector3ToNxVec3(m_JointDesc.globalDirection));
		if(m_JointDesc.globalAnchor != Vector3::ZERO_VECTOR)
			m_pFixedDesc->setGlobalAnchor(PhysXMapping::vector3ToNxVec3(m_JointDesc.globalAnchor));

		m_pJoint = m_pScene->createJoint(*m_pFixedDesc);
	}
//----------------------------------------------------------------------
	void JointFixed::cleanup()
	{
		NGENE_DELETE(m_pFixedDesc);
	}
//----------------------------------------------------------------------
	NxJointDesc* JointFixed::getJointDesc()
	{
		return (static_cast<NxJointDesc*>(m_pFixedDesc));
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	JointFixedFactory::JointFixedFactory()
	{
	}
//----------------------------------------------------------------------
	JointFixedFactory::~JointFixedFactory()
	{
	}
//----------------------------------------------------------------------
	Joint* JointFixedFactory::createJoint(NxScene* _scene, JOINT_DESC& _desc)
	{
		return (new JointFixed(_scene, _desc));
	}
//----------------------------------------------------------------------
}