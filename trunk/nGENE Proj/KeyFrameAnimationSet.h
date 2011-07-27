/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		KeyFrameAnimationSet.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_KEYFRAMEANIMATIONSET_H_
#define __INC_KEYFRAMEANIMATIONSET_H_


#include "KeyFrameAnimation.h"
#include "Prerequisities.h"
#include "Quaternion.h"
#include "Vector3.h"


namespace nGENE
{
	/** Set of key frame animations.
		@par
			Animation set is the main building block of animation.
			Skeletal key frame animation can have many animation sets
			active at a time. This is due to the fact that nGENE supports
			animation blending. For each animation set one have to specify
			weight (ranging 0.0 to 1.0). Weights of all active animation
			set have to sum up to 1.0 in the majority of cases (unless
			animations are blending-ready).
	*/
	class nGENEDLL KeyFrameAnimationSet
	{
	protected:
		/// Hash table containing all animations.
		HashTable <BONE*, KeyFrameAnimation> m_Animations;
		/// Hash table containing weights for all bones.
		HashTable <BONE*, Real> m_Weights;

		bool m_bEnabled;				///< Specifies whether it is enabled

		Real m_fWeight;					///< Weight to be used with animation blending

		wstring m_stName;				///< Name of the set (eg. 'walk' or 'jump')

	public:
		KeyFrameAnimationSet();

		/** Adds animation to the set.
			@param _bone bone to which animation applies.
			@param _animation animation itself.
		*/
		void addAnimation(BONE* _bone, KeyFrameAnimation& _animation);

		/** Performs key frame animation.
			@param _delta time difference since last update.
		*/
		void animate(Real _delta);

		/// Specifies whether this animation set is enabled.
		void setEnabled(bool _value);

		/// Checks if the animation set is enabled.
		bool isEnabled() const;

		/// Sets weight of this animation set.
		void setWeight(Real _weight);
		/// Sets weight for a given bone.
		void setWeight(BONE* _bone, Real _weight);

		/// Returns weight of the animation set.
		Real getWeight() const;
		/// Returns weight for a given bone.
		Real getWeight(BONE* _bone);

		/// Returns KeyFrameAnimation for a given BONE object.
		KeyFrameAnimation* getKeyFrameAnimation(BONE* _bone);

		/// Sets name of this set.
		void setName(const wstring& _name);
		/// Returns name of this set.
		wstring& getName();
	};



	inline void KeyFrameAnimationSet::setEnabled(bool _value)
	{
		m_bEnabled = _value;
	}
//----------------------------------------------------------------------
	inline bool KeyFrameAnimationSet::isEnabled() const
	{
		return m_bEnabled;
	}
//----------------------------------------------------------------------
	inline void KeyFrameAnimationSet::setWeight(Real _weight)
	{
		m_fWeight = _weight;

		HashTable <BONE*, Real>::iterator iter;
		for(iter = m_Weights.begin(); iter != m_Weights.end(); ++iter)
			iter->second = m_fWeight;
	}
//----------------------------------------------------------------------
	inline void KeyFrameAnimationSet::setWeight(BONE* _bone, Real _weight)
	{
		// Bone is not animated by this set
		if(m_Weights.find(_bone) == m_Weights.end())
			return;

		m_Weights[_bone] = _weight;
	}
//----------------------------------------------------------------------
	inline Real KeyFrameAnimationSet::getWeight() const
	{
		return m_fWeight;
	}
//----------------------------------------------------------------------
	inline Real KeyFrameAnimationSet::getWeight(BONE* _bone)
	{
		// Bone is not animated by this set, so return 0
		if(m_Weights.find(_bone) == m_Weights.end())
			return 0.0f;

		return m_Weights[_bone];
	}
//----------------------------------------------------------------------
	inline void KeyFrameAnimationSet::setName(const wstring& _name)
	{
		m_stName = _name;
	}
//----------------------------------------------------------------------
	inline wstring& KeyFrameAnimationSet::getName()
	{
		return m_stName;
	}
//----------------------------------------------------------------------
}


#endif