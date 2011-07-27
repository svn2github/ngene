/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PhysXMapping.h
Version:	0.05
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PHYSXMAPPING_H_
#define __INC_PHYSXMAPPING_H_


#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"
#include "Quaternion.h"


namespace nGENE
{
	/** Provides conversions between nGENE Tech and Ageia PhysX
		types.
		@remarks
			These conversions are necessary as nGENE uses its own
			conventions which are not entirely compatible with
			those of PhysX.
	*/
	class PhysXMapping
	{
	public:
		/// Converts PhysX NxVec3 into nGENE's Vector3.
		static Vector3 nxVec3ToVector3(const NxVec3& _vec);
		/// Converts PhysX NxExtendedVec3 into nGENE's Vector3.
		static Vector3 nxVec3ToVector3(const NxExtendedVec3& _vec);

		/// Converts nGENE Vector3 to NxVec3 of PhysX.
		static NxVec3 vector3ToNxVec3(const Vector3& _vec);
		/// Converts nGENE Vector3 to NxExtendedVec3 of PhysX.
		static NxExtendedVec3 vector3ToNxExtendedVec3(const Vector3& _vec);

		/// Converts nGENE Matrix3x3 to NxMat33 of PhysX.
		static NxMat33 matrix3x3ToNxMat33(const Matrix3x3& _mat);
		/// Converts nGENE Matrix4x4 to NxMat34 of PhysX.
		static NxMat34 matrix4x4ToNxMat34(const Matrix4x4& _mat);

		/// Converts PhysX NxQuat into nGENE's Quaternion.
		static Quaternion nxQuatToQuaternion(const NxQuat& _quat);
		/// Converts nGENE Quaternion to NxQuat of PhysX.
		static NxQuat quaternionToNxQuat(const Quaternion& _quat);

		/// Converts between FORCE_FIELD_INTERACTION and NxForceField enums.
		static NxForceFieldType ffiToNxForceFieldType(const FORCE_FIELD_INTERACTION& _interaction);
		/// Converts between FORCE_FIELD_COORDINATES and NxForceFieldCoordinates.
		static NxForceFieldCoordinates ffcToNxForceFieldCoordinates(const FORCE_FIELD_COORDINATES& _coordinates);
		/// Converts between FORCE_FIELD_VOLUME_SCALING and uint.
		static uint ffvsToNxForceFieldFlags(uint _scaling);
	};
}


#endif