/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		RenderQueue.cpp
Version:	0.06
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "NodeVisible.h"
#include "RenderPass.h"
#include "RenderQueue.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo RenderQueue::Type(L"RenderQueue", &BaseClass::Type);


	RenderQueue::RenderQueue()
	{
		init();
	}
//----------------------------------------------------------------------
	RenderQueue::~RenderQueue()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void RenderQueue::push_back(Surface* _node)
	{
		m_pNodes.push_back(_node);
	}
//----------------------------------------------------------------------
	void RenderQueue::init()
	{
	}
//----------------------------------------------------------------------
	void RenderQueue::cleanup()
	{
		clear();
	}
//----------------------------------------------------------------------
	void RenderQueue::clear()
	{
		m_pNodes.clear();
	}
//----------------------------------------------------------------------
	void RenderQueue::render(RenderPass* _pass)
	{
		Surface* pNode = NULL;
		for(uint i = 0; i < m_pNodes.size(); ++i)
		{
			pNode = m_pNodes[i];
			if(!pNode->isCulled() && pNode->isEnabled() && pNode->getNode()->isEnabled() && pNode->getMaterialPtr())
			{
				pNode->prepare();

				uint count = _pass->getRenderCount();
				for(uint j = 0; j < count; ++j)
				{
					_pass->update();
					pNode->render();
				}
			}
		}
	}
//----------------------------------------------------------------------
	uint RenderQueue::getSurfacesNum() const
	{
		uint result = 0;
		for(uint i = 0; i < m_pNodes.size(); ++i)
		{
			if(!m_pNodes[i]->isCulled() && m_pNodes[i]->isEnabled())
				++result;
		}

		return result;
	}
//----------------------------------------------------------------------
}