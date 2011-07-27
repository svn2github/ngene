/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		KeyFrameAnimation.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_KEYFRAMEANIMATION_H_
#define __INC_KEYFRAMEANIMATION_H_


#include "KeyFrame.h"
#include "Prerequisities.h"
#include "Quaternion.h"
#include "Vector3.h"


namespace nGENE
{
	/** Animation based on key frames.
		@par
			Key frame animation object is associated with a single bone.
			Animation consists of keys of different type:
			* rotation keys
			* translation keys
			* scaling keys
			Between keys the values are interpolated using different methods
			(linear for translation and scaling and slerp for rotation).
	*/
	class nGENEDLL KeyFrameAnimation
	{
	private:
		vector <KeyFrame> m_vRotationKeys;
		vector <KeyFrame> m_vScaleKeys;
		vector <KeyFrame> m_vPositionKeys;

		uint m_nCurrentRotKey;
		uint m_nCurrentScaleKey;
		uint m_nCurrentPosKey;

		Quaternion m_quatCurrentRot;
		Vector3 m_vecCurrentScale;
		Vector3 m_vecCurrentPos;

		float m_fTime;					///< Total animation time
		float m_fPassedTime;			///< Time which already passed

	public:
		KeyFrameAnimation();

		/// Adds new rotation key.
		void addRotationKey(KeyFrame& _frame);
		/// Returns rotation key with the given index.
		KeyFrame& getRotationKey(uint _index);

		/// Adds new scale key.
		void addScaleKey(KeyFrame& _frame);
		/// Returns scale key with the given index.
		KeyFrame& getScaleKey(uint _index);

		/// Adds new position key.
		void addPositionKey(KeyFrame& _frame);
		/// Returns position key with the given index.
		KeyFrame& getPositionKey(uint _index);

		/** Sets total animation length.
			@param _value length of the animation in milliseconds.
		*/
		void setAnimationLength(Real _value);

		/** Performs animation.
			@param _delta time difference since previous update.
		*/
		void animate(const Real& _delta);

		/// Returns current rotation quaternion.
		Quaternion& getCurrentRotation();

		/// Returns current scale vector.
		Vector3& getCurrentScale();

		/// Returns current position vector.
		Vector3& getCurrentPosition();

		/// Returns number of rotation keys.
		uint getRotationKeysNum() const;
		/// Returns number of scale keys.
		uint getScaleKeysNum() const;
		/// Returns number of position keys.
		uint getPositionKeysNum() const;
	};



	inline void KeyFrameAnimation::setAnimationLength(Real _value)
	{
		m_fTime = _value;
	}
//----------------------------------------------------------------------
	inline Quaternion& KeyFrameAnimation::getCurrentRotation()
	{
		return m_quatCurrentRot;
	}
//----------------------------------------------------------------------
	inline Vector3& KeyFrameAnimation::getCurrentScale()
	{
		return m_vecCurrentScale;
	}
//----------------------------------------------------------------------
	inline Vector3& KeyFrameAnimation::getCurrentPosition()
	{
		return m_vecCurrentPos;
	}
//----------------------------------------------------------------------
	inline uint KeyFrameAnimation::getRotationKeysNum() const
	{
		return m_vRotationKeys.size();
	}
//----------------------------------------------------------------------
	inline uint KeyFrameAnimation::getScaleKeysNum() const
	{
		return m_vScaleKeys.size();
	}
//----------------------------------------------------------------------
	inline uint KeyFrameAnimation::getPositionKeysNum() const
	{
		return m_vPositionKeys.size();
	}
//----------------------------------------------------------------------
	inline KeyFrame& KeyFrameAnimation::getRotationKey(uint _index)
	{
		Assert(_index < m_vRotationKeys.size(), "Rotation key index out of bounds");

		return m_vRotationKeys[_index];
	}
//----------------------------------------------------------------------
	inline KeyFrame& KeyFrameAnimation::getScaleKey(uint _index)
	{
		Assert(_index < m_vScaleKeys.size(), "Scale key index out of bounds");

		return m_vScaleKeys[_index];
	}
//----------------------------------------------------------------------
	inline KeyFrame& KeyFrameAnimation::getPositionKey(uint _index)
	{
		Assert(_index < m_vPositionKeys.size(), "Position key index out of bounds");

		return m_vPositionKeys[_index];
	}
//----------------------------------------------------------------------
}


#endif