/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Camera.h
Version:	0.12
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_CAMERA_H_
#define __INC_CAMERA_H_


#include "AutoPointer.h"
#include "BaseClass.h"
#include "CameraFactory.h"
#include "Matrix4x4.h"
#include "Node.h"
#include "Prerequisities.h"
#include "TypeInfo.h"
#include "Vector3.h"


namespace nGENE
{
	/** Base camera class.
		@remarks
			Via Camera class interface you can control what the
			end-user (usually that means Player) sees on the screen.
			This class is capable of controlling view and projection
			matrices.
		@par
			The camera is built up from three vectors: forward (points
			towards camera's target), up (points up) and right (points
			to the right of the camera). So it is convenient to think of
			a camera as of a 3-dimensional coordinate system being
			rotated in relation to camera's orientation:
			^   ^          Where the vectors are (from the leftmost one):
			|  /		   up, forward and right.
			| /
			|/
			+-------->
		@par
			As the camera is really a special kind of node, another nodes
			can be simply assigned to it, thus creating useful effects like
			light coming from the observer or displaying weapon or
			cross-hair in the first person shooting games.
	*/
	class nGENEDLL Camera: public Node
	{
		EXPOSE_TYPE
	protected:
		AutoPointer <Frustum> m_pFrustum;		///< Camera frustum

		Matrix4x4 m_matView;					///< Camera view matrix
		
		Vector3 m_vecForward;					///< Camera's direction vector
		Vector3 m_vecUp;						///< Camera's up vector
		Vector3 m_vecRight;						///< Camera's right vector

		bool m_bChanged;						///< Has camera changed since previous frame?

		dword m_lPrevTime;


		/// Updates all camera vectors.
		virtual void updateAxes();
		virtual void updateFrustum();

	public:
		Camera();
		virtual ~Camera();

		virtual void init();
		virtual void cleanup();

		/// Updates camera.
		virtual void updateCamera();

		void setRotation(const Vector3& _rot);
		void setRotation(const Vector3& _axis, const Real _angle);
		void setRotation(const Quaternion& _quat);

		virtual void setPosition(const Vector3& _pos);
		virtual void setPosition(Real _x, Real _y, Real _z);

		/// Sets the camera direction.
		void setForward(const Vector3& _forward);
		Vector3& getForward() {return m_vecForward;}

		/// Sets the up vector.
		void setUp(const Vector3& _up);
		Vector3& getUp() {return m_vecUp;}

		Vector3& getRight() {return m_vecRight;}

		/// Gets the viewing-frustum of the camera.
		Frustum& getFrustum() {return *(m_pFrustum.getPointer());}
		Frustum* getFrustumPtr() const {return m_pFrustum.getPointer();}

		/// Requests immediate update of the camera.
		void requestUpdate() {m_bChanged=true;}
		/// Checks if camera has moved or rotated since last frame.
		bool hasMoved() const {return m_bChanged;}

		Matrix4x4& getViewMatrix() {return m_matView;}

		virtual void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		virtual void deserialize(ISerializer* _serializer);
	};

	/// Factory to be used for Camera class objects creation.
	class nGENEDLL DefaultCameraFactory: public CameraFactory
	{
	public:
		DefaultCameraFactory();
		virtual ~DefaultCameraFactory();

		Camera* createCamera() {return new Camera();}
	};
}


#endif
