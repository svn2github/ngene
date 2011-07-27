/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		KeyFrameAnimation.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "KeyFrameAnimation.h"


namespace nGENE
{
	// Used in sorting key frames by their stage value.
	class KeyFramesComparator
	{
	public:
		bool operator()(const KeyFrame& lhs, const KeyFrame& rhs)
		{
			return (lhs.getStage() < rhs.getStage());
		}
	};


	KeyFrameAnimation::KeyFrameAnimation():
		m_nCurrentRotKey(0),
		m_nCurrentScaleKey(0),
		m_nCurrentPosKey(0),
		m_fPassedTime(0.0f),
		m_fTime(0.0f)
	{
	}
//----------------------------------------------------------------------
	void KeyFrameAnimation::addRotationKey(KeyFrame& _frame)
	{
		m_vRotationKeys.push_back(_frame);

		// Sort rotation keys
		if(m_vRotationKeys.size() > 1)
		{
			KeyFramesComparator comparator;
			std::sort(m_vRotationKeys.begin(), m_vRotationKeys.end(), comparator);
		}
	}
//----------------------------------------------------------------------
	void KeyFrameAnimation::addScaleKey(KeyFrame& _frame)
	{
		m_vScaleKeys.push_back(_frame);

		// Sort scale keys
		if(m_vScaleKeys.size() > 1)
		{
			KeyFramesComparator comparator;
			std::sort(m_vScaleKeys.begin(), m_vScaleKeys.end(), comparator);
		}
	}
//----------------------------------------------------------------------
	void KeyFrameAnimation::addPositionKey(KeyFrame& _frame)
	{
		m_vPositionKeys.push_back(_frame);

		// Sort position keys
		if(m_vPositionKeys.size() > 1)
		{
			KeyFramesComparator comparator;
			std::sort(m_vPositionKeys.begin(), m_vPositionKeys.end(), comparator);
		}
	}
//----------------------------------------------------------------------
	void KeyFrameAnimation::animate(const Real& _delta)
	{
		m_fPassedTime += _delta;
		if(m_fPassedTime >= m_fTime)
			m_fPassedTime -= m_fTime;

		Real _stage = m_fPassedTime / m_fTime;

		// Animate rotation
		uint i;
		for(i = m_nCurrentRotKey; i < m_vRotationKeys.size(); ++i)
		{
			if((i + 1) == m_vRotationKeys.size())
				break;

			if(_stage >= m_vRotationKeys[i].getStage() && _stage < m_vRotationKeys[i + 1].getStage())
			{
				m_nCurrentRotKey = i;
				break;
			}
		}

		if(i == m_vRotationKeys.size() - 1)
			m_nCurrentRotKey = 0;

		float t = (_stage - m_vRotationKeys[m_nCurrentRotKey].getStage()) /
				  (m_vRotationKeys[m_nCurrentRotKey + 1].getStage() - m_vRotationKeys[m_nCurrentRotKey].getStage());
		m_quatCurrentRot = slerp(m_vRotationKeys[m_nCurrentRotKey].getRotation(),
			m_vRotationKeys[m_nCurrentRotKey + 1].getRotation(),
			t);

		// Animate scale
		for(i = m_nCurrentScaleKey; i < m_vScaleKeys.size(); ++i)
		{
			if((i + 1) == m_vScaleKeys.size())
				break;

			if(_stage >= m_vScaleKeys[i].getStage() && _stage < m_vScaleKeys[i + 1].getStage())
			{
				m_nCurrentScaleKey = i;
				break;
			}
		}

		if(i == m_vScaleKeys.size() - 1)
			m_nCurrentScaleKey = 0;

		t = (_stage - m_vScaleKeys[m_nCurrentScaleKey].getStage()) /
			(m_vScaleKeys[m_nCurrentScaleKey + 1].getStage() - m_vScaleKeys[m_nCurrentScaleKey].getStage());
		Vector3& startScale = m_vScaleKeys[m_nCurrentScaleKey].getScale();
		Vector3& endScale = m_vScaleKeys[m_nCurrentScaleKey + 1].getScale();
		m_vecCurrentScale = Maths::lerp <Vector3> (startScale, endScale, t);

		// Animate position
		for(i = m_nCurrentPosKey; i < m_vPositionKeys.size(); ++i)
		{
			if((i + 1) == m_vPositionKeys.size())
				break;

			if(_stage >= m_vPositionKeys[i].getStage() && _stage < m_vPositionKeys[i + 1].getStage())
			{
				m_nCurrentPosKey = i;
				break;
			}
		}

		if(i == m_vPositionKeys.size() - 1)
			m_nCurrentPosKey = 0;

		t = (_stage - m_vPositionKeys[m_nCurrentPosKey].getStage()) /
			(m_vPositionKeys[m_nCurrentPosKey + 1].getStage() - m_vPositionKeys[m_nCurrentPosKey].getStage());
		Vector3& startPos = m_vPositionKeys[m_nCurrentPosKey].getPosition();
		Vector3& endPos = m_vPositionKeys[m_nCurrentPosKey + 1].getPosition();
		m_vecCurrentPos = Maths::lerp <Vector3> (startPos, endPos, t);
	}
//----------------------------------------------------------------------
}
