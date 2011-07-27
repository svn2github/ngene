/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ForceFieldTypes.h
Version:	0.02
Info:		This file contains various types definitions which
			are used by the engine to describe force fields.
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FORCEFIELDTYPES_H_
#define __INC_FORCEFIELDTYPES_H_


#include "Matrix3x3.h"
#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"
#include "Vector3.h"


namespace nGENE
{
	/// Type of interaction between force field and other actors.
	enum FORCE_FIELD_INTERACTION
	{
		FFI_NONE,							///< No interaction at all
		FFI_GRAVITATION,					///< Force is proportional to mass of the actor
		FFI_OTHER							///< Force is not scaling with the mass of the actor
	};

	/// Coordinates space for force field.
	enum FORCE_FIELD_COORDINATES
	{
		FFC_CARTESIAN,
		FFC_SPHERICAL,
		FFC_CYLINDRICAL,
		FFC_TOROIDAL
	};

	/// Specifies if force scales with volume for given actors type.
	enum FORCE_FIELD_VOLUME_SCALING
	{
		FFVS_RIGID_BODY = (1<<0),
		FFVS_SOFT_BODY	= (1<<1),
		FFVS_CLOTH		= (1<<2),
		FFVS_FLUID		= (1<<3)
	};

	/// Descriptor of the force field kernel function.
	typedef struct SForceFieldKernelDesc
	{
		Vector3 constant;								///< Constant force part
		Vector3 noise;									///< Noise force part

		Vector3 linearFalloff;							///< Linear falloff
		Vector3 quadraticFalloff;						///< Quadratic falloff

		Real torusRadius;								/**< Torus radius for FFC_TOROIDAL toroidal
															 coordinate space of a force field */

		Matrix3x3 positionMultiplier;					///< Multiplier of position
		Vector3 positionTarget;							///< Position target

		Matrix3x3 velocityMultiplier;					///< Multiplier of velocity
		Vector3 velocityTarget;							///< Velocity target

		SForceFieldKernelDesc():
			torusRadius(0.0f)
		{
		}
	} FORCE_FIELD_KERNEL_DESC;

	/// Descriptor of the character controller.
	typedef struct SForceFieldDesc
	{
		FORCE_FIELD_INTERACTION rigidBodyInteraction;	///< Type of interaction with rigid bodies
		FORCE_FIELD_INTERACTION softBodyInteraction;	///< Type of interaction with soft bodies
		FORCE_FIELD_INTERACTION fluidInteraction;		///< Type of interaction with fluids
		FORCE_FIELD_INTERACTION clothInteraction;		///< Type of interaction with cloth

		FORCE_FIELD_COORDINATES coordinates;			///< Coordinate space used by force field

		dword volumeScalingFlags;						/**< Flags specifying for what actor types
															 volumetric scaling is enabled */

		FORCE_FIELD_KERNEL_DESC* kernel;				///< Pointer to kernel description

		SForceFieldDesc():
			rigidBodyInteraction(FFI_OTHER),
			softBodyInteraction(FFI_OTHER),
			fluidInteraction(FFI_OTHER),
			clothInteraction(FFI_OTHER),
			coordinates(FFC_CARTESIAN),
			volumeScalingFlags(FFVS_RIGID_BODY | FFVS_SOFT_BODY | FFVS_CLOTH | FFVS_FLUID),
			kernel(NULL)
		{
		}
	} FORCE_FIELD_DESC;
}


#endif