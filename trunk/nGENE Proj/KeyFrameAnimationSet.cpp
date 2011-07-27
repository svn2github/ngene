/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		KeyFrameAnimationSet.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "KeyFrameAnimationSet.h"
#include "Skeleton.h"


namespace nGENE
{
	KeyFrameAnimationSet::KeyFrameAnimationSet():
		m_bEnabled(true),
		m_fWeight(1.0f)
	{
	}
//----------------------------------------------------------------------
	void KeyFrameAnimationSet::addAnimation(BONE* _bone, KeyFrameAnimation& _animation)
	{
		m_Animations[_bone] = _animation;
		m_Weights[_bone] = m_fWeight;
	}
//----------------------------------------------------------------------
	void KeyFrameAnimationSet::animate(Real _delta)
	{
		if(!m_bEnabled)
			return;

		HashTable <BONE*, KeyFrameAnimation>::iterator iter;
		for(iter = m_Animations.begin(); iter != m_Animations.end(); ++iter)
		{
			iter->second.animate(_delta);

			BONE* pBone = iter->first;

			Matrix4x4 matTrans;
			matTrans.translate(iter->second.getCurrentPosition());
			Matrix4x4 matRot = iter->second.getCurrentRotation().toMatrix();
			matRot.transpose();
			Matrix4x4 matScale;
			matScale.scale(iter->second.getCurrentScale());

			pBone->localTransform += (matScale * matRot * matTrans) * m_Weights[pBone];
		}
	}
//----------------------------------------------------------------------
	KeyFrameAnimation* KeyFrameAnimationSet::getKeyFrameAnimation(BONE* _bone)
	{
		// Bone not found, so return NULL
		if(m_Animations.find(_bone) == m_Animations.end())
			return NULL;

		return (&m_Animations[_bone]);
	}
//----------------------------------------------------------------------
}
