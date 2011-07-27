/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		JointHinge.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "JointHinge.h"
#include "JointEnumerator.h"
#include "PhysXMapping.h"
#include "PhysicsActor.h"


#pragma warning(push)
#pragma warning(disable:4512)
	#include "NxScene.h"
	#include "NxRevoluteJoint.h"
	#include "NxRevoluteJointDesc.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo JointHinge::Type(L"JointHinge", &Joint::Type);


	JointHinge::JointHinge(NxScene* _scene, SJointDesc& _desc):
		Joint(_scene, _desc),
		m_pHingeDesc(new NxRevoluteJointDesc()),
		m_pHinge(NULL)
	{
	}
//----------------------------------------------------------------------
	JointHinge::~JointHinge()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void JointHinge::init()
	{
		m_pHingeDesc->actor[0] = m_JointDesc.actor1 ? m_JointDesc.actor1->getActor() : NULL;
		m_pHingeDesc->actor[1] = m_JointDesc.actor2 ? m_JointDesc.actor2->getActor() : NULL;
		m_pHingeDesc->localAnchor[0] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localAnchor1);
		m_pHingeDesc->localAnchor[1] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localAnchor2);
		m_pHingeDesc->localAxis[0] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localDirection1);
		m_pHingeDesc->localAxis[1] = PhysXMapping::vector3ToNxVec3(m_JointDesc.localDirection2);

		if(!m_pHingeDesc->isValid())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Hinge joint description is invalid");
			return;
		}

		if(m_JointDesc.globalDirection != Vector3::ZERO_VECTOR)
			m_pHingeDesc->setGlobalAxis(PhysXMapping::vector3ToNxVec3(m_JointDesc.globalDirection));
		if(m_JointDesc.globalAnchor != Vector3::ZERO_VECTOR)
			m_pHingeDesc->setGlobalAnchor(PhysXMapping::vector3ToNxVec3(m_JointDesc.globalAnchor));

		m_pJoint = m_pScene->createJoint(*m_pHingeDesc);
		m_pHinge = (NxRevoluteJoint*)m_pJoint;
	}
//----------------------------------------------------------------------
	void JointHinge::cleanup()
	{
		NGENE_DELETE(m_pHingeDesc);
	}
//----------------------------------------------------------------------
	NxJointDesc* JointHinge::getJointDesc()
	{
		return (static_cast<NxJointDesc*>(m_pHingeDesc));
	}
//----------------------------------------------------------------------
	void JointHinge::setMotor(const MOTOR_DESC& _motor)
	{
		NxMotorDesc motor(_motor.targetVelocity, _motor.maxForce, _motor.freeSpin);
		m_pHinge->setMotor(motor);
	}
//----------------------------------------------------------------------
	MOTOR_DESC JointHinge::getMotor()
	{
		MOTOR_DESC motorDesc;
		NxMotorDesc motor;
		if(m_pHinge->getMotor(motor))
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
	void JointHinge::setLimits(const JOINT_LIMITS_PAIR_DESC& _limits)
	{
		NxJointLimitPairDesc limits;
		limits.low.value = _limits.lowValue;
		limits.low.restitution = _limits.lowRestitution;
		limits.high.value = _limits.highValue;
		limits.high.restitution = _limits.highRestitution;

		m_pHinge->setLimits(limits);
	}
//----------------------------------------------------------------------
	void JointHinge::setMotorEnabled(bool _value)
	{
		uint flags = m_pHinge->getFlags();
		if(_value)
			flags |= NX_RJF_MOTOR_ENABLED;
		else
			flags &= ~NX_RJF_MOTOR_ENABLED;

		m_pHinge->setFlags(flags);
	}
//----------------------------------------------------------------------
	bool JointHinge::isMotorEnabled()
	{
		return ((m_pHinge->getFlags() & NX_RJF_MOTOR_ENABLED) ? true : false);
	}
//----------------------------------------------------------------------
	void JointHinge::setSpringEnabled(bool _value)
	{
		uint flags = m_pHinge->getFlags();
		if(_value)
			flags |= NX_RJF_SPRING_ENABLED;
		else
			flags &= ~NX_RJF_SPRING_ENABLED;

		m_pHinge->setFlags(flags);
	}
//----------------------------------------------------------------------
	bool JointHinge::isSpringEnabled()
	{
		return ((m_pHinge->getFlags() & NX_RJF_SPRING_ENABLED) ? true : false);
	}
//----------------------------------------------------------------------
	void JointHinge::setLimitsEnabled(bool _value)
	{
		uint flags = m_pHinge->getFlags();
		if(_value)
			flags |= NX_RJF_LIMIT_ENABLED;
		else
			flags &= ~NX_RJF_LIMIT_ENABLED;

		m_pHinge->setFlags(flags);
	}
//----------------------------------------------------------------------
	bool JointHinge::isLimitsEnabled()
	{
		return ((m_pHinge->getFlags() & NX_RJF_LIMIT_ENABLED) ? true : false);
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	JointHingeFactory::JointHingeFactory()
	{
	}
//----------------------------------------------------------------------
	JointHingeFactory::~JointHingeFactory()
	{
	}
//----------------------------------------------------------------------
	Joint* JointHingeFactory::createJoint(NxScene* _scene, JOINT_DESC& _desc)
	{
		return (new JointHinge(_scene, _desc));
	}
//----------------------------------------------------------------------
}