/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		RenderTechnique.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "RenderTechnique.h"
#include "Material.h"
#include "XMLDocument.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo RenderTechnique::Type(L"RenderTechnique", NULL);


	RenderTechnique::RenderTechnique():
		m_stName(L""),
		m_pMaterial(NULL)
	{
		init();
	}
//----------------------------------------------------------------------
	RenderTechnique::~RenderTechnique()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void RenderTechnique::init()
	{
	}
//----------------------------------------------------------------------
	void RenderTechnique::cleanup()
	{
		
	}
//----------------------------------------------------------------------
	void RenderTechnique::bind()
	{
	}
//----------------------------------------------------------------------
	void RenderTechnique::unbind()
	{
	}
//----------------------------------------------------------------------
	void RenderTechnique::update()
	{
	}
//----------------------------------------------------------------------
	void RenderTechnique::parse(XMLDocument* _node)
	{
		XMLDocument* pNode = _node;

		// Parse child nodes
		XMLDocument* pChild = pNode->getFirstChild();
		XMLDocument* pTemp = NULL;

		while(pChild)
		{
			// Parse render pass
			if(pChild->getTagName() == "pass")
			{
				string name = pChild->getAttribute("name");
				wstring passName;
				passName.assign(name.begin(), name.end());

				RenderPass pass(passName);
				pass.parse(pChild);

				/// @todo If pass with such a name already exists, we have to join
				/// old with this new one.
				vector <RenderPass>::iterator iter, iter2;
				for(iter = m_vPasses.begin(); iter != m_vPasses.end(); ++iter)
				{
					wstring name = iter->getName();
					if(name == passName)
					{
						//						RenderPass pass2;
						//						(*iter) = pass;
						//						iter = m_vPasses.erase(iter);
						//						m_vPasses[i] = pass2;
						//iter2 = iter;
						//m_vPasses.insert(iter2, pass);
						//m_vPasses.erase(m_vPasses.begin() + i, m_vPasses.begin() + i + 1);
						//
						break;
					}
				}

				//RenderPass pass2;
				if(iter == m_vPasses.end())
					m_vPasses.push_back(pass);
				//else
				//	m_vPasses.erase(iter + 1);

				//if(i == m_vPasses.size())
				//	m_vPasses.push_back(pass);
				//if(m_vPasses.size() > 1)
				//	m_vPasses.erase(m_vPasses.begin());
				//	m_vPasses.push_back(pass);
				/*else
				{
				m_vPasses.reserve(1000);
				if(m_vPasses.size() > 1)
				m_vPasses.erase(m_vPasses.begin() + i);
				//m_vPasses.swap(m_vPasses);
				m_vPasses.insert(m_vPasses.begin() + i, pass);
				}*/
			}

			pTemp = pChild;
			pChild = pChild->getNextSibling();
			NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	void RenderTechnique::save(File* _file)
	{
		XMLNode::setName("technique");

		Attribute atName;
		atName.first = "name";
		atName.second = string(RenderTechnique::m_stName.begin(),
			RenderTechnique::m_stName.end());

		addAttribute(atName);

		// Save render passes
		for(uint i = 0; i < m_vPasses.size(); ++i)
			addChild(&m_vPasses[i]);

		// Save
		XMLNode::save(_file);
	}
//----------------------------------------------------------------------
	void RenderTechnique::render(RenderQueue* _queue)
	{
		vector <RenderPass>::iterator iter;
		for(iter = m_vPasses.begin(); iter != m_vPasses.end(); ++iter)
		{
			RenderPass& pass = (*iter);
			// Post processing pass, so skip it
			if((pass.getRenderingOrder() == DO_POST) && (m_pMaterial->getRenderingOrder() == DO_POST))
				continue;

			pass.bind();
				_queue->render(&pass);
			pass.unbind();
		}
	}
//----------------------------------------------------------------------
	void RenderTechnique::addRenderPass(RenderPass& _pass)
	{
		m_vPasses.push_back(_pass);
	}
//----------------------------------------------------------------------
}
