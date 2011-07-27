/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PhysicsActorWrapper.h
Version:	0.01
---------------------------------------------------------------------------
*/

#ifndef __INC_RIGIDBODYWRAPPER_H_
#define __INC_RIGIDBODYWRAPPER_H_


#include "nGENE.h"
#include "NodeWrapper.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;


	/// Wrapper for nGENE's rigid body class.
	ref class PhysicsActorWrapper: public SceneNode
	{
	private:
		PhysicsActor* m_pRigid;

		static int s_nRigidsCount;

	public:
		PhysicsActorWrapper(Node* _node, const wstring& _type, bool _dynamic)
		{
			uint index = 0;
			PhysicsWorld* pWorld = Physics::getSingleton().getWorld(index);

			PhysicsMaterial* physMat = pWorld->createPhysicsMaterial(L"testPhysMat");
			physMat->setRestitution(0.5f);
			physMat->setDynamicFriction(0.5f);
			physMat->setStaticFriction(0.5f);

			wostringstream buffer;
			buffer << "PhysicsActor";
			buffer << s_nRigidsCount;
			wstring name = buffer.str();

			PhysicsActor::BODY_TYPE bodyType = _dynamic ? PhysicsActor::BT_DYNAMIC : PhysicsActor::BT_STATIC;

			m_pRigid = pWorld->createPhysicsActor(_type, nGENE::Vector3(15.0f, 3.0f, 0.5f), bodyType);
			m_pRigid->attachNode((NodeVisible*)_node);
			m_pRigid->setShapeMaterial(0, *physMat);
			pWorld->addPhysicsActor(name, m_pRigid);


			this->Name = "PhysicsActor" + (s_nRigidsCount);
			this->Key = "PhysicsActor" + (s_nRigidsCount++);

			ImageIndex = 1;

			frmSceneGraph::graph->addNode(this);
		}

		/*PhysicsActorWrapper(Camera* _camera, String^ _name):
			NodeWrapper(_camera, _name, 1)
		{
			m_pCamera = _camera;

			Engine::getSingleton().setActiveCamera(m_pCamera);

			m_pCamera->requestUpdate();
			m_pCamera->updateCamera();

			m_FOV = Maths::radToDeg(m_pCamera->getFrustum().getFOV());
		}*/

		[Browsable(true), CategoryAttribute("Physics"),
		DescriptionAttribute("Specifies body mass")]
		property Real Mass
		{
			Real get()
			{
				return m_pRigid->getMass();
			}

			void set(Real value)
			{
				m_pRigid->setMass(value);
			}
		}
	};
}


#endif