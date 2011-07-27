/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Shape.h
Version:	0.08
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SHAPE_H_
#define __INC_SHAPE_H_


#include "BaseClass.h"
#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"
#include "TypeInfo.h"
#include "Vector3.h"


namespace nGENE
{
	/** Shape description.
		@par
			Shape is used in collision detection algorithms to approximate
			mesh as in the majority of cases precise mesh to mesh collisions
			are too expensive.
		@par
			Shape has also great effect on the falling of the object and
			general behaviour of the actor.
		@par
			Shape can be also made a trigger, i.e. area in 3D space which in
			case of collision notifies of it but doesn't influence colliding
			object's movement.
	*/
	class nGENEDLL Shape: public BaseClass
	{
		EXPOSE_TYPE
	protected:
		/// Pointer to the shape object
		NxShape* m_pShape;

		/// Dimensions of the shape
		Vector3 m_vecDimensions;

		/// Position of shape in Actor's space.
		Vector3 m_vecLocalPos;

		/// Pointer to the PhysicsMaterial object.
		PhysicsMaterial* m_pMaterial;

		/// Pointer to the PhysicsActor object.
		union
		{
			PhysicsActor* m_pActor;
			CharacterController* m_pController;
		};

		/// Specifies whether this shape is a trigger.
		bool m_bTrigger;
		/// Specifies whether this shape belongs to character controller.
		bool m_bBelongsToCharacterController;
		/// Is already created
		bool m_bCreated;

	public:
		Shape();
		virtual ~Shape();

		virtual void init();
		virtual void cleanup();

		/** Sets the dimensions of the shape.
			@remarks
				Although all the time the Vector3 structure is used, it has different
				meanings depending on the type of the shape used, eg. for box it
				describes width, length and height, but for sphere 1st dimension
				stores the radius where is the rest plays no role.
			@par
				As PhysX uses dimensions' radii, the given value has to be divided
				by two.
		*/
		virtual void setDimensions(const Vector3& _dims);
		/// Returns dimensions of the shape.
		Vector3& getDimensions();

		/// Sets local position of the shape.
		virtual void setLocalPos(const Vector3& _pos);

		/// Sets material for this shape.
		virtual void setMaterial(PhysicsMaterial* _material);
		/// Returns material of this shape.
		virtual PhysicsMaterial* getMaterial() const;

		/// Sets whether this shape is a trigger.
		virtual void setTrigger(bool _value);
		/// Checks if this shape is a trigger.
		virtual bool isTrigger() const;

		/// Sets pointer to the PhysicsActor object.
		void setActor(PhysicsActor* _actor);
		/// Returns pointer to the PhysicsActor object.
		PhysicsActor* getActor() const;

		/// Sets pointer to the CharacterController object.
		void setCharacterController(CharacterController* _controller);
		/// Returns pointer to the CharacterController object.
		CharacterController* getCharacterController() const;


		/// Checks if this shape belongs to character controller.
		bool isPartOfCharacterController() const;


		/// Returns PhysX shape description.
		virtual NxShapeDesc* getShapeDesc()=0;


		/// Sets shape pointer.
		void setShape(NxShape* _shape);
	};




	inline void Shape::setDimensions(const Vector3& _dims)
	{
		m_vecDimensions = _dims * 0.5f;
	}
//----------------------------------------------------------------------
	inline Vector3& Shape::getDimensions()
	{
		return m_vecDimensions;
	}
//----------------------------------------------------------------------
	inline void Shape::setMaterial(PhysicsMaterial* _material)
	{
		m_pMaterial = _material;
	}
//----------------------------------------------------------------------
	inline PhysicsMaterial* Shape::getMaterial() const
	{
		return m_pMaterial;
	}
//----------------------------------------------------------------------
	inline void Shape::setTrigger(bool _value)
	{
		m_bTrigger = _value;
	}
//----------------------------------------------------------------------
	inline bool Shape::isTrigger() const
	{
		return m_bTrigger;
	}
//----------------------------------------------------------------------
	inline void Shape::setActor(PhysicsActor* _actor)
	{
		m_pActor = _actor;
		m_bBelongsToCharacterController = false;
	}
//----------------------------------------------------------------------
	inline PhysicsActor* Shape::getActor() const
	{
		return m_pActor;
	}
//----------------------------------------------------------------------
	inline void Shape::setCharacterController(CharacterController* _controller)
	{
		m_pController = _controller;
		m_bBelongsToCharacterController = true;
	}
//----------------------------------------------------------------------
	inline CharacterController* Shape::getCharacterController() const
	{
		return m_pController;
	}
//----------------------------------------------------------------------
	inline bool Shape::isPartOfCharacterController() const
	{
		return m_bBelongsToCharacterController;
	}
//----------------------------------------------------------------------
	inline void Shape::setShape(NxShape* _shape)
	{
		m_pShape = _shape;
		if(m_pShape)
			m_bCreated = true;
	}
//----------------------------------------------------------------------
}


#endif