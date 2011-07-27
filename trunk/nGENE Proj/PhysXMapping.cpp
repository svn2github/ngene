/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PhysXMapping.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "PhysXMapping.h"
#include "ForceFieldTypes.h"
#include "Matrix3x3.h"

#include "NxExtended.h"
#include "NxForceField.h"


namespace nGENE
{
	Vector3 PhysXMapping::nxVec3ToVector3(const NxVec3& _vec)
	{
		return Vector3(_vec.x, _vec.y, _vec.z);
	}
//----------------------------------------------------------------------
	Vector3 PhysXMapping::nxVec3ToVector3(const NxExtendedVec3& _vec)
	{
		return Vector3(_vec.x, _vec.y, _vec.z);
	}
//----------------------------------------------------------------------
	NxVec3 PhysXMapping::vector3ToNxVec3(const Vector3& _vec)
	{
		return NxVec3(_vec.x, _vec.y, _vec.z);
	}
//----------------------------------------------------------------------
	NxExtendedVec3 PhysXMapping::vector3ToNxExtendedVec3(const Vector3& _vec)
	{
		return NxExtendedVec3(_vec.x, _vec.y, _vec.z);
	}
//----------------------------------------------------------------------
	Quaternion PhysXMapping::nxQuatToQuaternion(const NxQuat& _quat)
	{
		return Quaternion(_quat.x, _quat.y, _quat.z, _quat.w);
	}
//----------------------------------------------------------------------
	NxQuat PhysXMapping::quaternionToNxQuat(const Quaternion& _quat)
	{
		NxQuat result;
		result.setXYZW(_quat.x, _quat.y, _quat.z, _quat.w);

		return result;
	}
//----------------------------------------------------------------------
	NxMat33 PhysXMapping::matrix3x3ToNxMat33(const Matrix3x3& _mat)
	{
		NxMat33 mat(NxVec3(_mat._m11, _mat._m12, _mat._m13),
					NxVec3(_mat._m21, _mat._m22, _mat._m23),
					NxVec3(_mat._m31, _mat._m32, _mat._m33));

		return mat;
	}
//----------------------------------------------------------------------
	NxMat34 PhysXMapping::matrix4x4ToNxMat34(const Matrix4x4& _mat)
	{
		NxMat33 matRot(NxVec3(_mat._m11, _mat._m12, _mat._m13),
					   NxVec3(_mat._m21, _mat._m22, _mat._m23),
					   NxVec3(_mat._m31, _mat._m32, _mat._m33));
		NxVec3 vecTrans(_mat._m41, _mat._m42, _mat._m43);

		NxMat34 mat(matRot, vecTrans);

		return mat;
	}
//----------------------------------------------------------------------
	NxForceFieldType PhysXMapping::ffiToNxForceFieldType(const FORCE_FIELD_INTERACTION& _interaction)
	{
		switch(_interaction)
		{
		case FFI_NONE: return NX_FF_TYPE_NO_INTERACTION;
		case FFI_GRAVITATION: return NX_FF_TYPE_GRAVITATIONAL;
		case FFI_OTHER: return NX_FF_TYPE_OTHER;
		default: return NX_FF_TYPE_OTHER;
		}
	}
//----------------------------------------------------------------------
	NxForceFieldCoordinates PhysXMapping::ffcToNxForceFieldCoordinates(const FORCE_FIELD_COORDINATES& _coordinates)
	{
		switch(_coordinates)
		{
		case FFC_CARTESIAN: return NX_FFC_CARTESIAN;
		case FFC_SPHERICAL: return NX_FFC_SPHERICAL;
		case FFC_CYLINDRICAL: return NX_FFC_CYLINDRICAL;
		case FFC_TOROIDAL: return NX_FFC_TOROIDAL;
		default: return NX_FFC_CARTESIAN;
		}
	}
//----------------------------------------------------------------------
	uint PhysXMapping::ffvsToNxForceFieldFlags(uint _scaling)
	{
		uint result = 0;
		if(_scaling & FFVS_RIGID_BODY)
			result |= NX_FFF_VOLUMETRIC_SCALING_RIGIDBODY;
		if(_scaling & FFVS_SOFT_BODY)
			result |= NX_FFF_VOLUMETRIC_SCALING_SOFTBODY;
		if(_scaling & FFVS_CLOTH)
			result |= NX_FFF_VOLUMETRIC_SCALING_CLOTH;
		if(_scaling & FFVS_FLUID)
			result |= NX_FFF_VOLUMETRIC_SCALING_FLUID;

		return result;
	}
//----------------------------------------------------------------------
}
