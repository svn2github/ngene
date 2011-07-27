/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointPrism.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "JointPrism.h"
#include "JointEnumerator.h"
#include "PhysXMapping.h"
#include "PhysicsActor.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxScene.h"
	#include "NxPrismaticJointDesc.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo JointPrism::Type(L"JointPrism", &Joint::Type);


	JointPrism::JointPrism(NxScene* _scene, SJointDesc& _desc):
		Joint(_scene, _desc),
		m_pPrismDesc(new NxPrismaticJointDesc())
	{
	}
//----------------------------------------------------------------------
	JointPrism::~JointPrism()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void JointPrism::init()
	{
		m_pPrismDesc->actor[0] = m_JointDesc.actor1 ? m_JointDesc.actor1->getActor() : NULL;
		m_pPrismDesc->actor[1] = m_JointDesc.actor2 ? m_JointDesc.actor2->getActor() : NULL;
		m_pPrismDesc->localAnchor[0] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localAnchor1);
		m_pPrismDesc->localAnchor[1] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localAnchor2);
		m_pPrismDesc->localAxis[0] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localDirection1);
		m_pPrismDesc->localAxis[1] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localDirection2);

		if(!m_pPrismDesc->isValid())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Prismatic joint description is invalid");
			return;
		}

		if(m_JointDesc.globalDirection != Vector3::ZERO_VECTOR)
			m_pPrismDesc->setGlobalAxis(PhysXMapping::vector3ToNxVec3(m_JointDesc.globalDirection));
		if(m_JointDesc.globalAnchor != Vector3::ZERO_VECTOR)
			m_pPrismDesc->setGlobalAnchor(PhysXMapping::vector3ToNxVec3(m_JointDesc.globalAnchor));

		m_pJoint = m_pScene->createJoint(*m_pPrismDesc);
	}
//----------------------------------------------------------------------
	void JointPrism::cleanup()
	{
		NGENE_DELETE(m_pPrismDesc);
	}
//----------------------------------------------------------------------
	NxJointDesc* JointPrism::getJointDesc()
	{
		return (static_cast<NxJointDesc*>(m_pPrismDesc));
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	JointPrismFactory::JointPrismFactory()
	{
	}
//----------------------------------------------------------------------
	JointPrismFactory::~JointPrismFactory()
	{
	}
//----------------------------------------------------------------------
	Joint* JointPrismFactory::createJoint(NxScene* _scene, JOINT_DESC& _desc)
	{
		return (new JointPrism(_scene, _desc));
	}
//----------------------------------------------------------------------
}