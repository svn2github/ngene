/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		KeyFrame.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_KEYFRAME_H_
#define __INC_KEYFRAME_H_


#include "Prerequisities.h"
#include "Quaternion.h"
#include "Vector3.h"


namespace nGENE
{
	/** Single key of a key frame animation.
	*/
	class nGENEDLL KeyFrame
	{
	public:
		Quaternion m_quatOrientation;			///< Rotation quaternion
		Vector3 m_vecPosition;					///< Position vector
		Vector3 m_vecScale;						///< Scale vector

		Real m_fStage;

	public:
		KeyFrame();
		~KeyFrame();

		/** Sets stage of the animation.
			@param
				_stage value ranging 0.0 to 1.0 where 0.0 means start
				and 1.0 end of the animation.
		*/
		void setStage(Real _stage);
		Real getStage() const;

		/// Sets rotation for this key.
		void setRotation(const Quaternion& _orientation);
		Quaternion& getRotation();

		/// Sets position for this key.
		void setPosition(const Vector3& _position);
		Vector3& getPosition();

		/// Sets scale for this key.
		void setScale(const Vector3& _scale);
		Vector3& getScale();
	};



	inline void KeyFrame::setStage(Real _stage)
	{
		m_fStage = _stage;
	}
//----------------------------------------------------------------------
	inline void KeyFrame::setRotation(const Quaternion& _orientation)
	{
		m_quatOrientation = _orientation;
	}
//----------------------------------------------------------------------
	inline Quaternion& KeyFrame::getRotation()
	{
		return m_quatOrientation;
	}
//----------------------------------------------------------------------
	inline void KeyFrame::setPosition(const Vector3& _position)
	{
		m_vecPosition = _position;
	}
//----------------------------------------------------------------------
	inline Vector3& KeyFrame::getPosition()
	{
		return m_vecPosition;
	}
//----------------------------------------------------------------------
	inline void KeyFrame::setScale(const Vector3& _scale)
	{
		m_vecScale = _scale;
	}
//----------------------------------------------------------------------
	inline Vector3& KeyFrame::getScale()
	{
		return m_vecScale;
	}
//----------------------------------------------------------------------
	inline Real KeyFrame::getStage() const
	{
		return m_fStage;
	}
//----------------------------------------------------------------------
}


#endif