/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Camera2D.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_CAMERA2D_H_
#define __INC_CAMERA2D_H_


#include "Camera.h"
#include "Prerequisities.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Camera class used in 2D space.
		@remarks
			This camera class is especially usefull in creating side scroll
			games (eg. 2D platform games) and isometric games (RTS or RPG).
		@par
			It is important to notice that you should not use Camera2D together
			with Camera3D objects as the first uses pixel coordinates.
	*/
	class nGENEDLL Camera2D: public Camera
	{
		EXPOSE_TYPE
	public:
		/** This enumeration specifies which axes will be available for movement.
			The axis not specified is locked. Note that exactly one axis must be
			locked.
		*/
		enum CAMERA_LOCK_AXES
		{
			CLA_X_Y,
			CLA_X_Z,
			CLA_Y_Z
		};

	private:
		Real m_fVelocity;						///< Camera scroll speed
		Real m_fZoom;							///< Camera zoom level
		Real m_fDistance;						///< Distance from the target point

		CAMERA_LOCK_AXES m_Axes;				///< Axes in which movement is possible

	protected:
		/// Updates all camera vectors.
		virtual void updateAxes();

	public:
		Camera2D();
		virtual ~Camera2D();

		virtual void init();
		virtual void cleanup();

		/// Updates camera.
		virtual void updateCamera();

		/// Scrolls the camera.
		virtual void scroll(const Vector2& _direction);

		/// Sets camera scroll speed.
		void setVelocity(Real _velocity);
		/// Returns camera velocity.
		Real getVelocity() const;

		/** Sets camera zoom level.
			@param
				_zoom is the zoom level for the camera. Values greater than 1 cause
				zoom in, whilst those less than 1 zoom out. Values smaller then
				0 will cause flip in y axis. Although I doubt it, you can use it for
				some purpose.
		*/
		void setZoom(Real _zoom);
		/// Returns camera zoom.
		Real getZoom() const;

		/// Sets distance from the target point.
		void setDistance(Real _distance);
		/// Returns distance from the target point.
		Real getDistance() const;

		void setAxesLock(const CAMERA_LOCK_AXES& _axes);
		CAMERA_LOCK_AXES& getAxesLock();

		virtual void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		virtual void deserialize(ISerializer* _serializer);
	};


	/** Camera factory to be used for creating Camera2D objects.
	*/
	class nGENEDLL Camera2DFactory: public CameraFactory
	{
	public:
		Camera2DFactory();
		virtual ~Camera2DFactory();

		Camera* createCamera() {return new Camera2D();}
	};



	inline void Camera2D::setVelocity(Real _velocity)
	{
		m_fVelocity = _velocity;
	}
//----------------------------------------------------------------------
	inline Real Camera2D::getVelocity() const
	{
		return m_fVelocity;
	}
//----------------------------------------------------------------------
	inline Real Camera2D::getZoom() const
	{
		return m_fZoom;
	}
//----------------------------------------------------------------------
	inline Real Camera2D::getDistance() const
	{
		return m_fDistance;
	}
//----------------------------------------------------------------------
	inline void Camera2D::setAxesLock(const Camera2D::CAMERA_LOCK_AXES& _axes)
	{
		m_Axes = _axes;
	}
//----------------------------------------------------------------------
	inline Camera2D::CAMERA_LOCK_AXES& Camera2D::getAxesLock()
	{
		return m_Axes;
	}
//----------------------------------------------------------------------
}


#endif