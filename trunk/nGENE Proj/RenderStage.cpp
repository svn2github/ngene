/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		RenderStage.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "RenderStage.h"


namespace nGENE
{
	RenderStage::RenderStage():
		m_pRenderer(NULL),
		m_bEnabled(true),
		m_bIsEmbeddable(false),
		m_nPriority(0)
	{
	}
//----------------------------------------------------------------------
	RenderStage::~RenderStage()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void RenderStage::init()
	{
	}
//----------------------------------------------------------------------
	void RenderStage::cleanup()
	{
	}
//----------------------------------------------------------------------
	void RenderStage::setPriority(uint _priority)
	{
		m_nPriority = _priority;
		if(m_pRenderer)
			m_pRenderer->sortStages();
	}
//----------------------------------------------------------------------
	void RenderStage::beginRender()
	{
	}
//----------------------------------------------------------------------
	void RenderStage::onBeginFrame()
	{
	}
//----------------------------------------------------------------------
	void RenderStage::endRender()
	{
	}
//----------------------------------------------------------------------
	void RenderStage::addRenderStage(RenderStage* _stage)
	{
		if(_stage->isEmbeddable())
			m_vEmbeddedStages.push_back(_stage);
	}
//----------------------------------------------------------------------
	uint RenderStage::getRenderStagesNum() const
	{
		return m_vEmbeddedStages.size();
	}
//----------------------------------------------------------------------
	void RenderStage::removeRenderStage(uint _index)
	{
		if(_index >= m_vEmbeddedStages.size())
			return;

		m_vEmbeddedStages.erase(m_vEmbeddedStages.begin() + _index);
	}
//----------------------------------------------------------------------
}
