/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CharacterController.h
Version:	0.10
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_CHARACTERCONTROLLER_H_
#define __INC_CHARACTERCONTROLLER_H_


#include "BaseClass.h"
#include "Prerequisities.h"
#include "PrerequisitiesPhysX.h"
#include "Quaternion.h"
#include "TypeInfo.h"


namespace nGENE
{
	/// Type of the character controller.
	enum CHARACTER_CONTROLLER_SHAPE
	{
		CCS_CAPSULE,
		CCS_BOX
	};

	/// Descriptor of the character controller.
	typedef struct nGENEDLL SControllerDesc
	{
		Real radius;						///< Radius of the capsule (affects only capsule controller)
		Real height;						///< Height of the capsule/box
		Real width;							///< Width of the controller (affects only box controller)
		Real length;						///< Length of the controller (affects only box controller)

		Real slopeLimit;					///< Maximum slope (cosine of the angle) character can walk against
		Real obstacleLimit;					/**< Maximum obstacle height character 
												 can climb onto */
		Real velocity;						///< Character controller velocity
		Real sharpness;						/**< Used to prevent sudden height changes. Smaller values mean
												 smoother movement, 1.0f means no smoothing. */
		Real minDistance;					///< Minimum distance travelled to consider for movement

		uint collisionMask;					///< Collision mask

		CHARACTER_CONTROLLER_SHAPE shape;	///< Shape of the controller (capsule/box)

		SControllerDesc();
	} CONTROLLER_DESC;


	/** Makes character controlling easier.
		@par
			After creating character controller and before calling any
			of its method invoke init() method to properly initialize
			controller data.
		@remarks
			By default capsule character controller is used.
	*/
	class nGENEDLL CharacterController: public BaseClass
	{
		EXPOSE_TYPE
	private:
		Node* m_pNode;						///< Pointer to Node
		Shape* m_pShape;					///< Pointer to Shape

		NxScene* m_pScene;					///< Pointer to PhysX scene
		NxActor* m_pActor;					///< Pointer to PhysX actor

		NxController* m_pController;		///< Pointer to PhysX character controller

		Vector3 m_vecMovementDir;			///< Direction of movement

		Quaternion m_quatNodeOrientation;	///< Orientation of the controller
		Quaternion m_quatOrientation;		///< Orientation of the controller

		bool m_bMoving;						///< Is controller currently moving?
		bool m_bJumping;					///< Is controller currently jumping?
		bool m_bNormalizeDir;				///< Should movement vector be normalized? True by default
		bool m_bFlying;						///< Is controller in flight mode?
		/** Specifies if the orientation changes direction of movement or only
			changes the rotation of underlying Node.
		*/
		bool m_bOrientationAppliesToMovement;

		Real m_fJumpTime;					///< For how long character is in the air?
		Real m_fJumpHeight;					///< Height of the jump

		CONTROLLER_DESC m_Desc;				///< Description of the controller


		void initCapsuleShaped();
		void initBoxShaped();

	public:
		CharacterController(NxScene* _scene, CONTROLLER_DESC& _desc);
		~CharacterController();

		void init();
		void cleanup();

		/// Updates the character controller.
		void update(Real _delta);

		/// Attaches node to the controller.
		void attachNode(Node* _node);
		/// Returns node attached to the controller.
		Node* getNode() const;

		/// Sets position of the controller.
		void setPosition(Real _x, Real _y, Real _z);
		/// Sets position of the controller.
		void setPosition(const Vector3& _position);

		/// Moves controller in the given direction.
		void move(const Vector3& _direction);

		/** Sets controller orientation.
			@param
				_quat new orientation. Needs not to be normalized.
		*/
		void setRotation(const Quaternion& _quat);
		/** Rotates controller around Y axis - it's up vector.
			@param
				_angle angle in radians by which controller has to
				be rotate.
		*/
		void rotate(Real _angle);
		/// Returns controller orientation.
		Quaternion& getRotation();

		/** Makes a jump.
			@remarks
				_force the greater the value, the higher the jump will be.
		*/
		void jump(Real _force);


		/// Sets CharacterController velocity.
		void setVelocity(Real _velocity);
		/// Returns CharacterController velocity.
		Real getVelocity() const;

		/// Specifies whether movement vector should be normalized.
		void setNormalizeDir(bool _value);
		/// Checks if movement vector is normalized.
		bool isDirNormalized() const;

		/// Sets collision flag for the given group.
		void setCollisionFlag(uint _group, bool _collides);
		/// Checks if controller can collide with a specified group.
		bool isCollidingWithGroup(uint _group);

		/** Specifies whether character controller is in flight mode.
			@par
				Flying controllers are not affected by gravity. Flight
				height can controlled using associated Node's setPosition()
				method or CharacterController move() method.
			@remarks
				Jumping does not work in the flight mode.
		*/
		void setFlying(bool _value);
		/// Checks if the controller is flying.
		bool isFlying() const;

		/** Specifies if the orientation changes direction of movement or only
			changes the rotation of underlying Node.
			@par
				This method is useful when you want to rotate graphics representation
				(i.e. underlying Node object) without rotating the controller itself.
			@param
				_value if set to true the movement direction will change when the
				orientation changes, i.e. it will be rotated.
		*/
		void setOrientationAppliesToMovementDir(bool _value);
		/// Checks if the orientation changes the direction of movement.
		bool isOrientationAppliedToMovementDir() const;
	};



	inline void CharacterController::setVelocity(Real _velocity)
	{
		m_Desc.velocity = _velocity;
	}
//----------------------------------------------------------------------
	inline Real CharacterController::getVelocity() const
	{
		return m_Desc.velocity;
	}
//----------------------------------------------------------------------
	inline void CharacterController::move(const Vector3& _direction)
	{
		m_vecMovementDir += _direction;
		m_bMoving = true;
	}
//----------------------------------------------------------------------
	inline void CharacterController::setRotation(const Quaternion& _quat)
	{
		// Invalid quaternion, so return
		if(!Maths::floatCompare(_quat.x, 0.0f) &&
		   !Maths::floatCompare(_quat.y, 0.0f) &&
		   !Maths::floatCompare(_quat.z, 0.0f) &&
		   !Maths::floatCompare(_quat.w, 0.0f))
			return;

		m_quatOrientation = _quat;
		m_quatOrientation.normalize();
	}
//----------------------------------------------------------------------
	inline void CharacterController::rotate(Real _angle)
	{
		// No need to rotate
		if(!Maths::floatCompare(_angle, 0.0f))
			return;

		Quaternion quat(Vector3::UNIT_Y, _angle);
		m_quatOrientation *= quat;
		m_quatOrientation.normalize();
	}
//----------------------------------------------------------------------
	inline Quaternion& CharacterController::getRotation()
	{
		return m_quatOrientation;
	}
//----------------------------------------------------------------------
	inline Node* CharacterController::getNode() const
	{
		return m_pNode;
	}
//----------------------------------------------------------------------
	inline void CharacterController::setNormalizeDir(bool _value)
	{
		m_bNormalizeDir = _value;
	}
//----------------------------------------------------------------------
	inline bool CharacterController::isDirNormalized() const
	{
		return m_bNormalizeDir;
	}
//----------------------------------------------------------------------
	inline void CharacterController::setCollisionFlag(uint _group, bool _collides)
	{
		if(_collides)
			m_Desc.collisionMask |= (1 << _group);
		else
			m_Desc.collisionMask &= ~(1 << _group);
	}
//----------------------------------------------------------------------
	inline bool CharacterController::isCollidingWithGroup(uint _group)
	{
		return ((m_Desc.collisionMask & (1 << _group)) ? true : false);
	}
//----------------------------------------------------------------------
	inline void CharacterController::setFlying(bool _value)
	{
		m_bFlying = _value;
	}
//----------------------------------------------------------------------
	inline bool CharacterController::isFlying() const
	{
		return m_bFlying;
	}
//----------------------------------------------------------------------
	inline void CharacterController::setOrientationAppliesToMovementDir(bool _value)
	{
		m_bOrientationAppliesToMovement = _value;
	}
//----------------------------------------------------------------------
	inline bool CharacterController::isOrientationAppliedToMovementDir() const
	{
		return m_bOrientationAppliesToMovement;
	}
//----------------------------------------------------------------------
}


#endif