/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ForceField.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_FORCEFIELD_H_
#define __INC_FORCEFIELD_H_


#include "BaseClass.h"
#include "ForceFieldTypes.h"
#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"
#include "TypeInfo.h"
#include "Vector3.h"


namespace nGENE
{
	/** ForceField description.
	*/
	class nGENEDLL ForceField: public BaseClass
	{
		EXPOSE_TYPE
	protected:
		NxScene* m_pScene;					///< Pointer to PhysX scene
		NxForceField* m_pForceField;		///< Pointer to PhysX force field

		/// Dimensions of the ForceField
		Vector3 m_vecDimensions;

		/// Position of ForceField in Actor's space.
		Vector3 m_vecLocalPos;

		FORCE_FIELD_DESC m_Desc;			///< Description of the force field

		/// Vector of shapes groups.
		vector <ForceFieldShapesGroup*> m_vGroups;

	public:
		ForceField(NxScene* _scene, FORCE_FIELD_DESC& _desc);
		virtual ~ForceField();

		virtual void init();
		virtual void cleanup();

		/** Updates force field.
			@remarks
				If you want your force field to change in time, to simulate
				waves or changing wind for example, overload this method
				in your derived classes.
		*/
		virtual void update();

		/// Adds group of shapes.
		virtual void addShapeGroup(ForceFieldShapesGroup* _group);

		/// Sets transformation.
		virtual void setTransform(const Matrix4x4& _mat);

		/** Sets position of the force field.
			@remarks
				Using this method causes previous transformations to be
				lost.
		*/
		virtual void setPosition(const Vector3& _position);

		/// Binds actor to this force field.
		virtual void attachNode(PhysicsActor* _actor);
	};
}


#endif