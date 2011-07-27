/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CharacterController.h
Version:	0.07
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "CharacterController.h"
#include "CollisionListener.h"
#include "ListenerRegistry.h"
#include "Node.h"
#include "PhysXMapping.h"
#include "PhysicsActor.h"
#include "Shape.h"
#include "ShapeCapsule.h"
#include "ShapeEnumerator.h"


#pragma warning(push)
#pragma warning(disable:4245)
#pragma warning(disable:4512)
	#include "NxActor.h"
	#include "NxController.h"
	#include "NxControllerManager.h"
	#include "NxPhysics.h"
	#include "NxCapsuleController.h"
	#include "NxBoxController.h"
#pragma warning(pop)


namespace nGENE
{
	/// Report for controller-environment collisions.
	class nGENEDLL ControllerHitReport: public NxUserControllerHitReport
	{
	private:
		const dword m_kNumberOfPoints;		///< Maximum number of collision points
		dword m_nPointIndex;				///< Index of current point

		NxVec3* m_vecPoints;


		ControllerHitReport(const ControllerHitReport& src);
		ControllerHitReport& operator=(const ControllerHitReport& rhs);

	public:
		ControllerHitReport(dword _numberOfPoints=100);
		virtual ~ControllerHitReport();

		virtual NxControllerAction onShapeHit(const NxControllerShapeHit& hit);
		virtual NxControllerAction onControllerHit(const NxControllersHit& hit);
	} g_HitReport;
//----------------------------------------------------------------------
//----------------------------------------------------------------------

	// Initialize static members
	TypeInfo CharacterController::Type(L"CharacterController", &BaseClass::Type);


	CharacterController::CharacterController(NxScene* _scene, CONTROLLER_DESC& _desc):
		m_pNode(NULL),
		m_pShape(NULL),
		m_pScene(_scene),
		m_pActor(NULL),
		m_pController(NULL),
		m_vecMovementDir(Vector3::ZERO_VECTOR),
		m_bMoving(false),
		m_bJumping(false),
		m_bFlying(false),
		m_bNormalizeDir(true),
		m_bOrientationAppliesToMovement(true),
		m_Desc(_desc)
	{
	}
//----------------------------------------------------------------------
	CharacterController::~CharacterController()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void CharacterController::initCapsuleShaped()
	{
		NxCapsuleControllerDesc desc;
		desc.position.x		= 0.0f;
		desc.position.y		= 0.0f;
		desc.position.z		= 0.0f;
		desc.radius			= m_Desc.radius;
		desc.height			= m_Desc.height;
		desc.upDirection	= NX_Y;
		desc.slopeLimit		= m_Desc.slopeLimit;
		desc.skinWidth		= 0.1f;
		desc.stepOffset		= m_Desc.obstacleLimit;
		desc.callback		= &g_HitReport;
		desc.userData		= this;


		if(desc.isValid())
		{
			NxControllerManager* pManager = Physics::getSingleton().getControllerManager();
			m_pController = pManager->createController(m_pScene, desc);
			m_pActor = m_pController->getActor();

			NxShape* const* shapes = m_pActor->getShapes();
			for(uint i = 0; i < m_pActor->getNbShapes(); ++i)
			{
				NxCapsuleShape* pCapsule = shapes[i]->isCapsule();
				if(pCapsule)
				{
					m_pShape = ShapeEnumerator::getSingleton().create(L"Capsule");

					Vector3 dimensions(pCapsule->getRadius(), pCapsule->getHeight(), 0.0f);
					m_pShape->setDimensions(dimensions);
					m_pShape->setCharacterController(this);

					pCapsule->userData = m_pShape;
					pCapsule->setGroup(PhysicsActor::GROUP_CHARACTER_CONTROLLER);
				}
				else
				{
					shapes[i]->userData = NULL;
				}
			}
		}
		else
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Invalid character controller description");
			return;
		}
	}
//----------------------------------------------------------------------
	void CharacterController::initBoxShaped()
	{
		NxBoxControllerDesc desc;
		desc.position.x		= 0.0f;
		desc.position.y		= 0.0f;
		desc.position.z		= 0.0f;
		desc.extents		= NxVec3(m_Desc.width * 0.5f, m_Desc.height * 0.5f, m_Desc.length * 0.5f);
		desc.upDirection	= NX_Y;
		desc.slopeLimit		= m_Desc.slopeLimit;
		desc.skinWidth		= 0.1f;
		desc.stepOffset		= m_Desc.obstacleLimit;
		desc.callback		= &g_HitReport;
		desc.userData		= this;


		if(desc.isValid())
		{
			NxControllerManager* pManager = Physics::getSingleton().getControllerManager();
			m_pController = pManager->createController(m_pScene, desc);
			m_pActor = m_pController->getActor();

			NxShape* const* shapes = m_pActor->getShapes();
			for(uint i = 0; i < m_pActor->getNbShapes(); ++i)
			{
				NxBoxShape* pBox = shapes[i]->isBox();
				if(pBox)
				{
					m_pShape = ShapeEnumerator::getSingleton().create(L"Box");

					Vector3 dimensions = PhysXMapping::nxVec3ToVector3(pBox->getDimensions());
					m_pShape->setDimensions(dimensions);
					m_pShape->setCharacterController(this);

					pBox->userData = m_pShape;
					pBox->setGroup(PhysicsActor::GROUP_CHARACTER_CONTROLLER);
				}
				else
				{
					shapes[i]->userData = NULL;
				}
			}
		}
		else
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Invalid character controller description");
			return;
		}
	}
//----------------------------------------------------------------------
	void CharacterController::init()
	{
		if(m_Desc.shape == CCS_CAPSULE)
			initCapsuleShaped();
		else if(m_Desc.shape == CCS_BOX)
			initBoxShaped();
	}
//----------------------------------------------------------------------
	void CharacterController::cleanup()
	{
		NGENE_DELETE(m_pShape);

		m_pScene->releaseActor(*m_pActor);

		NxControllerManager* pManager = Physics::getSingleton().getControllerManager();
		pManager->releaseController(*m_pController);
	}
//----------------------------------------------------------------------
	void CharacterController::attachNode(Node* _node)
	{
		m_pNode = _node;

		if(m_pNode)
		{
			Vector3 vecPos = m_pNode->getPositionWorld();
			m_pController->setPosition(PhysXMapping::vector3ToNxExtendedVec3(vecPos));

			m_quatNodeOrientation = m_pNode->getRotation();
		}
	}
//----------------------------------------------------------------------
	void CharacterController::update(Real _delta)
	{
		// No node attached, so return
		if(!m_pNode || m_vecMovementDir.length() < 0.001f)
			return;


		NxVec3 disp;
		if(!m_bFlying)
			m_pScene->getGravity(disp);
		else
		{
			disp.x = 0.0f;
			disp.y = 0.0f;
			disp.z = 0.0f;
		}

		if(m_bMoving)
		{
			if(!m_bFlying)
				m_vecMovementDir.y = 0.0f;
			if(m_bOrientationAppliesToMovement)
				m_vecMovementDir = m_quatOrientation * m_vecMovementDir;
			if(m_bNormalizeDir)
				m_vecMovementDir.normalize();

			float velocity = m_Desc.velocity;

			disp.x += m_vecMovementDir.x * velocity;
			if(m_bFlying)
				disp.y += m_vecMovementDir.y * velocity;
			disp.z += m_vecMovementDir.z * velocity;
			m_bMoving = false;
		}

		NxU32 collisionFlags;
		NxVec3 movement = disp * _delta;
		
		// We're making hops!
		if(m_bJumping && !m_bFlying)
		{
			m_fJumpTime += _delta;
			Real heightDelta = disp.y * m_fJumpTime * m_fJumpTime + m_fJumpHeight * m_fJumpTime;
			heightDelta = (heightDelta - disp.y) * _delta;
			if(heightDelta != 0.0f)
				movement.y += heightDelta;
		}

		
		m_pController->move(movement,
							m_Desc.collisionMask,
							m_Desc.minDistance,
							collisionFlags,
							m_Desc.sharpness);
		
		// We're not jumping anymore
		if(collisionFlags & NXCC_COLLISION_DOWN)
			m_bJumping = false;



		Vector3 vecPos = PhysXMapping::nxVec3ToVector3(m_pController->getPosition());
		m_pNode->setPosition(vecPos);
		m_pNode->setRotation(m_quatOrientation * m_quatNodeOrientation);


		m_vecMovementDir = Vector3::ZERO_VECTOR;
	}
//----------------------------------------------------------------------
	void CharacterController::setPosition(const Vector3& _position)
	{
		m_pController->setPosition(PhysXMapping::vector3ToNxExtendedVec3(_position));

		if(m_pNode)
			m_pNode->setPosition(_position);
	}
//----------------------------------------------------------------------
	void CharacterController::setPosition(Real _x, Real _y, Real _z)
	{
		Vector3 pos(_x, _y, _z);
		m_pController->setPosition(PhysXMapping::vector3ToNxExtendedVec3(pos));

		if(m_pNode)
			m_pNode->setPosition(_x, _y, _z);
	}
//----------------------------------------------------------------------
	void CharacterController::jump(Real _force)
	{
		if(m_bJumping)
			return;

		m_fJumpTime = 0.0f;
		m_fJumpHeight = _force;
		m_bJumping = true;
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	ControllerHitReport::ControllerHitReport(dword _numberOfPoints):
		m_kNumberOfPoints(_numberOfPoints),
		m_nPointIndex(0)
	{
		m_vecPoints = new NxVec3[m_kNumberOfPoints];
	}
//----------------------------------------------------------------------
	ControllerHitReport::~ControllerHitReport()
	{
		NGENE_DELETE_ARRAY(m_vecPoints);
	}
//----------------------------------------------------------------------
	NxControllerAction ControllerHitReport::onShapeHit(const NxControllerShapeHit& hit)
	{
		if(hit.shape)
		{
			NxCollisionGroup group = hit.shape->getGroup();
			if(group == PhysicsActor::GROUP_COLLIDABLE_PUSHABLE)
			{
				NxActor& actor = hit.shape->getActor();
				if(actor.isDynamic())
				{
					if((m_vecPoints[m_nPointIndex].x != hit.worldPos.x) ||
					   (m_vecPoints[m_nPointIndex].y != hit.worldPos.y) ||
					   (m_vecPoints[m_nPointIndex].z != hit.worldPos.z))
					{
						m_vecPoints[m_nPointIndex].x = hit.worldPos.x;
						m_vecPoints[m_nPointIndex].y = hit.worldPos.y;
						m_vecPoints[m_nPointIndex].z = hit.worldPos.z;
						if(++m_nPointIndex == m_kNumberOfPoints)
							m_nPointIndex = 0;
					}

					if(hit.dir.y == 0.0f)
					{
						NxF32 coeff = actor.getMass() * hit.length * 10.0f;
						actor.addForceAtLocalPos(hit.dir * coeff,
												 NxVec3(0.0f, 0.0f, 0.0f),
												 NX_IMPULSE);
					}
				}
			}

			// Emit nGENE event
			ControllerCollisionEvent evt;
			evt.controller = (CharacterController*)hit.controller->getUserData();
			evt.actor = (PhysicsActor*)hit.shape->getActor().userData;
			evt.motionDir = PhysXMapping::nxVec3ToVector3(hit.dir);
			evt.motionLength = hit.length;
			evt.contactPoints.push_back(PhysXMapping::nxVec3ToVector3(hit.worldPos));
			evt.contactNormals.push_back(PhysXMapping::nxVec3ToVector3(hit.worldNormal));

			// Handle event
			ListenerRegistry <ControllerCollisionEvent>::handleEvent(evt);
		}

		return NX_ACTION_NONE;
	}
//----------------------------------------------------------------------
	NxControllerAction ControllerHitReport::onControllerHit(const NxControllersHit& hit)
	{
		// Emit nGENE event
		ControllerControllerCollisionEvent evt;
		evt.controller1 = (CharacterController*)hit.controller->getUserData();
		evt.controller2 = (CharacterController*)hit.other->getUserData();

		// Handle event
		ListenerRegistry <ControllerControllerCollisionEvent>::handleEvent(evt);

		return NX_ACTION_NONE;
	}
//----------------------------------------------------------------------
	SControllerDesc::SControllerDesc():
		radius(0.5f),
		height(0.5f),
		width(0.5f),
		length(0.5f),
		slopeLimit(0.7f),
		obstacleLimit(0.5f),
		velocity(2.0f),
		sharpness(1.0f),
		minDistance(0.000001f),
		collisionMask((1 << PhysicsActor::GROUP_COLLIDABLE_NON_PUSHABLE) | 
					  (1 << PhysicsActor::GROUP_COLLIDABLE_PUSHABLE)),
		shape(CCS_CAPSULE)
	{
	}
	//----------------------------------------------------------------------
}