/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PrerequisitiesPhysX.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PREREQUISITIESPHYSX_H_
#define __INC_PREREQUISITIESPHYSX_H_


#pragma warning(push)
#pragma warning(disable:4512)


class NxActor;
class NxBoxForceFieldShapeDesc;
class NxBoxShapeDesc;
class NxCapsuleForceFieldShapeDesc;
class NxCapsuleShapeDesc;
class NxController;
class NxControllerManager;
class NxCylindricalJointDesc;
class NxDistanceJointDesc;
class NxForceField;
class NxForceFieldDesc;
class NxForceFieldShapeDesc;
class NxFixedJointDesc;
class NxHeightField;
class NxHeightFieldDesc;
class NxHeightFieldShapeDesc;
class NxForceFieldShapeGroup;
class NxJoint;
class NxJointDesc;
class NxPointInPlaneJointDesc;
class NxPointOnLineJointDesc;
class NxPrismaticJointDesc;
class NxPulleyJoint;
class NxPulleyJointDesc;
class NxRevoluteJointDesc;
class NxSphericalJoint;
class NxSphericalJointDesc;
class NxMat33;
class NxMat34;
class NxMaterial;
class NxPlaneShapeDesc;
class NxPhysicsSDK;
class NxRevoluteJoint;
class NxQuat;
class NxScene;
class NxShape;
class NxShapeDesc;
class NxSphereForceFieldShapeDesc;
class NxSphereShapeDesc;
class NxTriangleMeshDesc;
class NxTriangleMeshShapeDesc;
class NxVec3;


struct NxControllersHit;
struct NxControllerShapeHit;
struct NxExtendedVec3;


enum NxControllerAction;
enum NxForceFieldCoordinates;
enum NxForceFieldType;
enum NxMemoryType;


typedef __int64				NxI64;
typedef signed int			NxI32;
typedef signed short		NxI16;
typedef signed char			NxI8;

typedef unsigned __int64	NxU64;
typedef unsigned int		NxU32;
typedef unsigned short		NxU16;
typedef unsigned char		NxU8;

typedef float				NxF32;
typedef double				NxF64;


#if (_MSC_VER>=1000)
	#define NX_INLINE __forceinline
#endif

#pragma warning(pop)

#endif