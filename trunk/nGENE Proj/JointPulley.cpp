/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointPulley.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "JointPulley.h"
#include "JointEnumerator.h"
#include "PhysXMapping.h"
#include "PhysicsActor.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxScene.h"
	#include "NxPulleyJoint.h"
	#include "NxPulleyJointDesc.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo JointPulley::Type(L"JointPulley", &Joint::Type);


	JointPulley::JointPulley(NxScene* _scene, SJointDesc& _desc):
		Joint(_scene, _desc),
		m_pPulleyDesc(new NxPulleyJointDesc()),
		m_pPulley(NULL)
	{
	}
//----------------------------------------------------------------------
	JointPulley::~JointPulley()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void JointPulley::init()
	{
		m_pPulleyDesc->actor[0] = m_JointDesc.actor1 ? m_JointDesc.actor1->getActor() : NULL;
		m_pPulleyDesc->actor[1] = m_JointDesc.actor2 ? m_JointDesc.actor2->getActor() : NULL;
		m_pPulleyDesc->localAnchor[0] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localAnchor1);
		m_pPulleyDesc->localAnchor[1] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localAnchor2);
		m_pPulleyDesc->localAxis[0] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localDirection1);
		m_pPulleyDesc->localAxis[1] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localDirection2);

		SPulleyJointDesc& desc = ((SPulleyJointDesc&)m_JointDesc);
		m_pPulleyDesc->pulley[0] = PhysXMapping::vector3ToNxVec3(desc.pulley1);
		m_pPulleyDesc->pulley[1] = PhysXMapping::vector3ToNxVec3(desc.pulley2);
		m_pPulleyDesc->distance = desc.distance;
		m_pPulleyDesc->stiffness = desc.stiffness;
		m_pPulleyDesc->ratio = desc.transmission;
		m_pPulleyDesc->flags = desc.flags;

		if(!m_pPulleyDesc->isValid())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Pulley joint description is invalid");
			return;
		}

		if(m_JointDesc.globalDirection != Vector3::ZERO_VECTOR)
			m_pPulleyDesc->setGlobalAxis(PhysXMapping::vector3ToNxVec3(m_JointDesc.globalDirection));
		if(m_JointDesc.globalAnchor != Vector3::ZERO_VECTOR)
			m_pPulleyDesc->setGlobalAnchor(PhysXMapping::vector3ToNxVec3(m_JointDesc.globalAnchor));

		m_pJoint = m_pScene->createJoint(*m_pPulleyDesc);
		m_pPulley = (NxPulleyJoint*)m_pJoint;
	}
//----------------------------------------------------------------------
	void JointPulley::cleanup()
	{
		NGENE_DELETE(m_pPulleyDesc);
	}
//----------------------------------------------------------------------
	NxJointDesc* JointPulley::getJointDesc()
	{
		return (static_cast<NxJointDesc*>(m_pPulleyDesc));
	}
//----------------------------------------------------------------------
	void JointPulley::setMotor(const MOTOR_DESC& _motor)
	{
		NxMotorDesc motor(_motor.targetVelocity, _motor.maxForce, _motor.freeSpin);
		m_pPulley->setMotor(motor);
	}
//----------------------------------------------------------------------
	MOTOR_DESC JointPulley::getMotor()
	{
		MOTOR_DESC motorDesc;
		NxMotorDesc motor;
		if(m_pPulley->getMotor(motor))
		{
			motorDesc.targetVelocity = motor.velTarget;
			motorDesc.maxForce = motor.maxForce;
			motorDesc.freeSpin = (motor.freeSpin != 0);
		}
		else
		{
			motorDesc.targetVelocity = 0.0f;
			motorDesc.maxForce = 0.0f;
			motorDesc.freeSpin = false;
		}

		return motorDesc;
	}
//----------------------------------------------------------------------
	void JointPulley::setMotorEnabled(bool _value)
	{
		uint flags = m_pPulley->getFlags();
		if(_value)
			flags |= NX_PJF_MOTOR_ENABLED;
		else
			flags &= ~NX_PJF_MOTOR_ENABLED;

		m_pPulley->setFlags(flags);
	}
//----------------------------------------------------------------------
	bool JointPulley::isMotorEnabled()
	{
		return ((m_pPulley->getFlags() & NX_PJF_MOTOR_ENABLED) ? true : false);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	JointPulleyFactory::JointPulleyFactory()
	{
	}
//----------------------------------------------------------------------
	JointPulleyFactory::~JointPulleyFactory()
	{
	}
//----------------------------------------------------------------------
	Joint* JointPulleyFactory::createJoint(NxScene* _scene, JOINT_DESC& _desc)
	{
		return (new JointPulley(_scene, _desc));
	}
//----------------------------------------------------------------------
}