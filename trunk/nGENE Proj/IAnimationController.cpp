/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		IAnimationController.cpp
Version:	0.06
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "IAnimationController.h"


namespace nGENE
{
	IAnimationController::IAnimationController():
		m_bAnimating(false),
		m_bStopped(true),
		m_bFinished(false),
		m_pNode(NULL)
	{
	}
//----------------------------------------------------------------------
	IAnimationController::~IAnimationController()
	{
		stopAnimation();
	}
//----------------------------------------------------------------------
	void IAnimationController::setNode(Node* _node)
	{
		m_pNode = _node;
	}
//----------------------------------------------------------------------
	void IAnimationController::startAnimation()
	{
		m_bAnimating = true;
		m_bStopped = false;

		m_nPrevTime = Engine::getSingleton().getTimer().getMilliseconds();
	}
//----------------------------------------------------------------------
	void IAnimationController::init()
	{
	}
//----------------------------------------------------------------------
	bool IAnimationController::isAnimating()
	{
		return m_bAnimating;
	}
//----------------------------------------------------------------------
	void IAnimationController::stopAnimation()
	{
		m_bAnimating = false;
		m_bStopped = true;
	}
//----------------------------------------------------------------------
	void IAnimationController::pauseAnimation()
	{
		m_bAnimating = false;
	}
//----------------------------------------------------------------------
	void IAnimationController::unpauseAnimation()
	{
		m_bAnimating = true;
	}
//----------------------------------------------------------------------
	void IAnimationController::togglePauseAnimation()
	{
		if(m_bAnimating)
			pauseAnimation();
		else
			unpauseAnimation();
	}
//----------------------------------------------------------------------
	void IAnimationController::serialize(ISerializer* _serializer,
										 bool _serializeType,
										 bool _serializeChildren)
	{
		// Not serializable, so return
		if(!m_bIsSerializable)
			return;
	}
//----------------------------------------------------------------------
}