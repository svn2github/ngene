/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointCylinder.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "JointCylinder.h"
#include "JointEnumerator.h"
#include "PhysXMapping.h"
#include "PhysicsActor.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxScene.h"
	#include "NxCylindricalJointDesc.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo JointCylinder::Type(L"JointCylinder", &Joint::Type);


	JointCylinder::JointCylinder(NxScene* _scene, SJointDesc& _desc):
		Joint(_scene, _desc),
		m_pCylinderDesc(new NxCylindricalJointDesc())
	{
	}
//----------------------------------------------------------------------
	JointCylinder::~JointCylinder()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void JointCylinder::init()
	{
		m_pCylinderDesc->actor[0] = m_JointDesc.actor1 ? m_JointDesc.actor1->getActor() : NULL;
		m_pCylinderDesc->actor[1] = m_JointDesc.actor2 ? m_JointDesc.actor2->getActor() : NULL;
		m_pCylinderDesc->localAnchor[0] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localAnchor1);
		m_pCylinderDesc->localAnchor[1] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localAnchor2);
		m_pCylinderDesc->localAxis[0] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localDirection1);
		m_pCylinderDesc->localAxis[1] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localDirection2);

		if(!m_pCylinderDesc->isValid())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Cylindrical joint description is invalid");
			return;
		}

		if(m_JointDesc.globalDirection != Vector3::ZERO_VECTOR)
			m_pCylinderDesc->setGlobalAxis(PhysXMapping::vector3ToNxVec3(m_JointDesc.globalDirection));
		if(m_JointDesc.globalAnchor != Vector3::ZERO_VECTOR)
			m_pCylinderDesc->setGlobalAnchor(PhysXMapping::vector3ToNxVec3(m_JointDesc.globalAnchor));

		m_pJoint = m_pScene->createJoint(*m_pCylinderDesc);
	}
//----------------------------------------------------------------------
	void JointCylinder::cleanup()
	{
		NGENE_DELETE(m_pCylinderDesc);
	}
//----------------------------------------------------------------------
	NxJointDesc* JointCylinder::getJointDesc()
	{
		return (static_cast<NxJointDesc*>(m_pCylinderDesc));
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	JointCylinderFactory::JointCylinderFactory()
	{
	}
//----------------------------------------------------------------------
	JointCylinderFactory::~JointCylinderFactory()
	{
	}
//----------------------------------------------------------------------
	Joint* JointCylinderFactory::createJoint(NxScene* _scene, JOINT_DESC& _desc)
	{
		return (new JointCylinder(_scene, _desc));
	}
//----------------------------------------------------------------------
}