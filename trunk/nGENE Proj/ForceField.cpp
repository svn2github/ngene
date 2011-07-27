/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ForceField.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ForceField.h"
#include "ForceFieldShapesGroup.h"
#include "Matrix3x3.h"
#include "PhysXMapping.h"
#include "PhysicsActor.h"


#pragma warning(push)
#pragma warning(disable:4245)
#pragma warning(disable:4512)
	#include "NxForceField.h"
	#include "NxPhysics.h"
#pragma warning(pop)


namespace nGENE
{
	// Initialize static members
	TypeInfo ForceField::Type(L"ForceField", &BaseClass::Type);


	ForceField::ForceField(NxScene* _scene, FORCE_FIELD_DESC& _desc):
		m_pScene(_scene),
		m_pForceField(NULL),
		m_Desc(_desc)
	{
	}
//----------------------------------------------------------------------
	ForceField::~ForceField()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void ForceField::init()
	{
		NxForceFieldDesc fieldDesc;
		NxForceFieldLinearKernelDesc	kernelDesc;
		NxForceFieldLinearKernel*		linearKernel;

		// Description of the kernel
		FORCE_FIELD_KERNEL_DESC* pKernel = m_Desc.kernel;
		if(pKernel)
		{
			kernelDesc.constant = PhysXMapping::vector3ToNxVec3(pKernel->constant);
			kernelDesc.noise = PhysXMapping::vector3ToNxVec3(pKernel->noise);
			kernelDesc.falloffLinear = PhysXMapping::vector3ToNxVec3(pKernel->linearFalloff);
			kernelDesc.falloffQuadratic = PhysXMapping::vector3ToNxVec3(pKernel->quadraticFalloff);
			kernelDesc.torusRadius = pKernel->torusRadius;

			kernelDesc.positionMultiplier = PhysXMapping::matrix3x3ToNxMat33(pKernel->positionMultiplier);
			kernelDesc.positionTarget = PhysXMapping::vector3ToNxVec3(pKernel->positionTarget);

			kernelDesc.velocityMultiplier = PhysXMapping::matrix3x3ToNxMat33(pKernel->velocityMultiplier);
			kernelDesc.velocityTarget = PhysXMapping::vector3ToNxVec3(pKernel->velocityTarget);
		}

		if(kernelDesc.isValid())
			linearKernel = m_pScene->createForceFieldLinearKernel(kernelDesc);
		else
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Description of force field kernel is invalid");
			return;
		}


		fieldDesc.rigidBodyType = PhysXMapping::ffiToNxForceFieldType(m_Desc.rigidBodyInteraction);
		fieldDesc.softBodyType = PhysXMapping::ffiToNxForceFieldType(m_Desc.softBodyInteraction);
		fieldDesc.fluidType = PhysXMapping::ffiToNxForceFieldType(m_Desc.fluidInteraction);
		fieldDesc.actor = NULL;
		fieldDesc.coordinates = PhysXMapping::ffcToNxForceFieldCoordinates(m_Desc.coordinates);
		fieldDesc.kernel = linearKernel;
		fieldDesc.pose.id();
		fieldDesc.flags = PhysXMapping::ffvsToNxForceFieldFlags(m_Desc.volumeScalingFlags);

		if(fieldDesc.isValid())
			m_pForceField = m_pScene->createForceField(fieldDesc);
		else
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Description of force field is invalid");
			return;
		}
	}
//----------------------------------------------------------------------
	void ForceField::cleanup()
	{
		if(m_vGroups.size())
		{
			for(uint i = 0; i < m_vGroups.size(); ++i)
				NGENE_DELETE(m_vGroups[i]);
			m_vGroups.clear();
		}
		m_pScene->releaseForceField(*m_pForceField);
	}
//----------------------------------------------------------------------
	void ForceField::update()
	{
	}
//----------------------------------------------------------------------
	void ForceField::addShapeGroup(ForceFieldShapesGroup* _group)
	{
		m_pForceField->addShapeGroup(*_group->getShapeGroup());
		m_vGroups.push_back(_group);
	}
//----------------------------------------------------------------------
	void ForceField::setTransform(const Matrix4x4& _transform)
	{
		m_pForceField->setPose(PhysXMapping::matrix4x4ToNxMat34(_transform));
	}
//----------------------------------------------------------------------
	void ForceField::setPosition(const Vector3& _position)
	{
		Matrix4x4 mat;
		mat._m41 = _position.x;
		mat._m42 = _position.y;
		mat._m43 = _position.z;

		m_pForceField->setPose(PhysXMapping::matrix4x4ToNxMat34(mat));
	}
//----------------------------------------------------------------------
	void ForceField::attachNode(PhysicsActor* _actor)
	{
		m_pForceField->setActor(_actor->getActor());
	}
//----------------------------------------------------------------------
}