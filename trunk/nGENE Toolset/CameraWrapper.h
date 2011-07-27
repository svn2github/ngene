/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		CameraWrapper.h
Version:	0.06
---------------------------------------------------------------------------
*/

#ifndef __INC_CAMERAWRAPPER_H_
#define __INC_CAMERAWRAPPER_H_


#include "nGENE.h"
#include "NodeWrapper.h"

#include <sstream>


namespace nGENEToolset
{
	using namespace System;


	/// Wrapper for nGENE's camera.
	ref class CameraWrapper: public NodeWrapper
	{
	private:
		Camera* m_pCamera;

		static int s_nCameraCount;

	public:
		CameraWrapper()
		{
			wostringstream buffer;
			buffer << "Camera";
			buffer << s_nCameraCount;
			wstring text = buffer.str();

			SceneManager* sm = Engine::getSingleton().getSceneManager(0);
			m_pNode = sm->createCamera(L"CameraFirstPerson", text);
			m_pCamera = static_cast<Camera*>(m_pNode);
			m_pCamera->setPosition(nGENE::Vector3(0.0f, 3.0f, -5.0f));
			m_pCamera->setUp(nGENE::Vector3::UNIT_Y);
			m_pCamera->getFrustumPtr()->setNear(0.1f);
			m_pCamera->getFrustumPtr()->setFar(4000.0f);
			if(frmSceneGraph::graph->getActive() != nullptr)
				frmSceneGraph::graph->getActive()->getnGENENode()->addChild(text, m_pCamera);

			Engine::getSingleton().setActiveCamera(m_pCamera);

			m_pCamera->requestUpdate();
			m_pCamera->updateCamera();

			m_Position = gcnew Vector3(&m_pNode->getPositionLocal());
			m_Dimensions = gcnew Vector3(&m_pCamera->getScale());
			m_Rotation = gcnew Quaternion(&m_pCamera->getRotation());

			//m_PositionWorld->setVector(m_pNode->getPositionWorld());


			this->Name = "Camera" + (s_nCameraCount);
			this->Key = "Camera" + (s_nCameraCount++);

			ImageIndex = 1;


			frmSceneGraph::graph->addNode(this);
		}

		CameraWrapper(Camera* _camera, String^ _name):
			NodeWrapper(_camera, _name, 1)
		{
			m_pCamera = _camera;
			s_nCameraCount++;

			Engine::getSingleton().setActiveCamera(m_pCamera);

			m_pCamera->requestUpdate();
			m_pCamera->updateCamera();
		}

		[Browsable(true), CategoryAttribute("Camera"),
		DescriptionAttribute("Velocity of camera movement")]
		property float Velocity
		{
			float get()
			{
				CameraFirstPerson* cam = static_cast<CameraFirstPerson*>(m_pCamera);

				return cam->getVelocity();
			}

			void set(float value)
			{
				CameraFirstPerson* cam = static_cast<CameraFirstPerson*>(m_pCamera);

				cam->setVelocity(value);
			}
		}

		[Browsable(true), CategoryAttribute("Camera"),
		DescriptionAttribute("Camera Field Of View")]
		property float FOV
		{
			float get()
			{
				return Maths::radToDeg(m_pCamera->getFrustum().getFOV());
			}

			void set(float value)
			{
				m_pCamera->getFrustum().setFOV(Maths::degToRad(value));
				m_pCamera->requestUpdate();
				m_pCamera->getFrustum().update();
			}
		}

		[Browsable(true), CategoryAttribute("Camera"),
		DescriptionAttribute("Camera aspect ratio")]
		property float Aspect
		{
			float get()
			{
				return m_pCamera->getFrustum().getAspect();
			}

			void set(float value)
			{
				m_pCamera->getFrustum().setAspect(value);
				m_pCamera->requestUpdate();
				m_pCamera->getFrustum().update();
			}
		}

		[Browsable(true), CategoryAttribute("Camera"),
		DescriptionAttribute("Near clipping plane")]
		property float NearPlane
		{
			float get()
			{
				return m_pCamera->getFrustum().getNear();
			}

			void set(float value)
			{
				m_pCamera->getFrustum().setNear(value);
				m_pCamera->requestUpdate();
				m_pCamera->getFrustum().update();
			}
		}

		[Browsable(true), CategoryAttribute("Camera"),
		DescriptionAttribute("Far clipping plane")]
		property float FarPlane
		{
			float get()
			{
				return m_pCamera->getFrustum().getFar();
			}

			void set(float value)
			{
				m_pCamera->getFrustum().setFar(value);
				m_pCamera->requestUpdate();
				m_pCamera->getFrustum().update();
			}
		}
	};
}


#endif