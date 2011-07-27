/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		RenderPass.cpp
Version:	0.16
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "RenderPass.h"
#include "ITexture.h"
#include "ShaderInstance.h"
#include "ShaderManager.h"
#include "Surface.h"
#include "TextureManager.h"
#include "TextureSampler.h"
#include "XMLDocument.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo RenderPass::Type(L"RenderPass", NULL);


	RenderPass::RenderPass():
		m_stName(L""),
		m_pVertexShader(NULL),
		m_pPixelShader(NULL),
		m_pGeometryShader(NULL),
		m_Order(DO_POST),
		m_nRunCount(1),
		m_bIsEnabled(true)
	{
		init();
	}
//----------------------------------------------------------------------
	RenderPass::RenderPass(const wstring& _name):
		m_stName(_name),
		m_pVertexShader(NULL),
		m_pPixelShader(NULL),
		m_pGeometryShader(NULL),
		m_Order(DO_POST),
		m_nRunCount(1),
		m_bIsEnabled(true)
	{
		init();
	}
//----------------------------------------------------------------------
	RenderPass::RenderPass(const RenderPass& src):
		BaseClass(src),
		XMLNode(src)
	{
		m_bHasLighting = src.m_bHasLighting;
		m_bIsEnabled = src.m_bIsEnabled;

		m_Order = src.m_Order;
		m_nRunCount = src.m_nRunCount;

		m_pVertexShader = src.m_pVertexShader ? new ShaderInstance(*src.m_pVertexShader) : NULL;
		m_pGeometryShader = src.m_pGeometryShader ? new ShaderInstance(*src.m_pGeometryShader) : NULL;
		m_pPixelShader = src.m_pPixelShader ? new ShaderInstance(*src.m_pPixelShader) : NULL;

		m_stName = src.m_stName;
		m_stValue = src.m_stValue;

		m_vRenderTargets = src.m_vRenderTargets;
		m_vSamplers = src.m_vSamplers;
	}
//----------------------------------------------------------------------
	RenderPass& RenderPass::operator=(const RenderPass& rhs)
	{
		if(this == &rhs)
			return (*this);

		BaseClass::operator=(rhs);
		XMLNode::operator=(rhs);

		m_bHasLighting = rhs.m_bHasLighting;
		m_bIsEnabled = rhs.m_bIsEnabled;

		m_Order = rhs.m_Order;
		m_nRunCount = rhs.m_nRunCount;

		m_pVertexShader = rhs.m_pVertexShader ? new ShaderInstance(*rhs.m_pVertexShader) : NULL;
		m_pGeometryShader = rhs.m_pGeometryShader ? new ShaderInstance(*rhs.m_pGeometryShader) : NULL;
		m_pPixelShader = rhs.m_pPixelShader ? new ShaderInstance(*rhs.m_pPixelShader) : NULL;

		m_stName = rhs.m_stName;
		m_stValue = rhs.m_stValue;

		m_vRenderTargets = rhs.m_vRenderTargets;
		m_vSamplers = rhs.m_vSamplers;
		
		return (*this);
	}
//----------------------------------------------------------------------
	RenderPass::~RenderPass()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void RenderPass::init()
	{
	}
//----------------------------------------------------------------------
	void RenderPass::cleanup()
	{
		NGENE_DELETE(m_pVertexShader);
		NGENE_DELETE(m_pGeometryShader);
		NGENE_DELETE(m_pPixelShader);

		if(!m_vSamplers.empty())
			m_vSamplers.clear();

		if(!m_vRenderTargets.empty())
			m_vRenderTargets.clear();
	}
//----------------------------------------------------------------------
	void RenderPass::bind()
	{
		// Not active so return
		if(!m_bIsEnabled)
			return;

		ShaderManager& manager = ShaderManager::getSingleton();
		if(m_pVertexShader)
			m_pVertexShader->bind();
		else
			manager.unbindShader(SHADER_VERTEX);

		if(m_pGeometryShader)
			m_pGeometryShader->bind();
		else
			manager.unbindShader(SHADER_GEOMETRY);

		if(m_pPixelShader)
			m_pPixelShader->bind();
		else
			manager.unbindShader(SHADER_PIXEL);


		for(uint i = 0; i < m_vRenderTargets.size(); ++i)
			m_vRenderTargets[i].bind();
		for(uint i = 0; i < m_vSamplers.size(); ++i)
			m_vSamplers[i].bind();
	}
//----------------------------------------------------------------------
	void RenderPass::unbind()
	{
		// Not active so return
		if(!m_bIsEnabled)
			return;

		if(m_pVertexShader)
			m_pVertexShader->unbind();
		if(m_pGeometryShader)
			m_pGeometryShader->unbind();
		if(m_pPixelShader)
			m_pPixelShader->unbind();
		for(uint i = 0; i < m_vRenderTargets.size(); ++i)
			m_vRenderTargets[i].unbind();
		for(uint i = 0; i < m_vSamplers.size(); ++i)
			m_vSamplers[i].unbind();
	}
//----------------------------------------------------------------------
	void RenderPass::update()
	{
		// Not active so return
		if(!m_bIsEnabled)
			return;

		if(m_pVertexShader)
			m_pVertexShader->update();
		if(m_pGeometryShader)
			m_pGeometryShader->update();
		if(m_pPixelShader)
			m_pPixelShader->update();
	}
//----------------------------------------------------------------------
	void RenderPass::save(File* _file)
	{
		XMLNode::setName("pass");

		Attribute atName;
		atName.first = "name";
		atName.second = string(RenderPass::m_stName.begin(),
			RenderPass::m_stName.end());

		addAttribute(atName);

		// Save textures
		vector <XMLNode*> vTextures;
		for(uint i = 0; i < m_vSamplers.size(); ++i)
		{
			TextureSampler& sampler = m_vSamplers[i];
			wstring temp = sampler.getTextureName();
			string stTextureName(temp.begin(), temp.end());

			XMLNode* pNode = TextureManager::getSingleton().saveTexture(sampler.getTexture(),
				stTextureName);
			//addChild(pNode);

			vTextures.push_back(pNode);
		}

		// Save render targets
		for(uint i = 0; i < m_vRenderTargets.size(); ++i)
		{
			TextureSampler& sampler = m_vRenderTargets[i];
			wstring temp = sampler.getTextureName();
			string stTextureName(temp.begin(), temp.end());

			XMLNode* pNode = TextureManager::getSingleton().saveTexture(sampler.getTexture(),
				stTextureName);
			//addChild(pNode);

			vTextures.push_back(pNode);
		}

		// Save samplers
		saveSamplers();

		// Save shaders
		saveShaders();


		XMLNode::save(_file);


		// Now release some memory we used
		if(!vTextures.empty())
		{
			for(uint i = 0; i < vTextures.size(); ++i)
				NGENE_DELETE(vTextures[i]);
		}
	}
//----------------------------------------------------------------------
	void RenderPass::saveSamplers()
	{
		// Save samplers
		for(uint i = 0; i < m_vSamplers.size(); ++i)
			addChild(&m_vSamplers[i]);

		// Save render targets
		for(uint i = 0; i < m_vRenderTargets.size(); ++i)
			addChild(&m_vRenderTargets[i]);
	}
//----------------------------------------------------------------------
	void RenderPass::saveShaders()
	{
		// Save vertex shader
		if(m_pVertexShader)
			addChild(m_pVertexShader);

		// Save geometry shader
		if(m_pGeometryShader)
			addChild(m_pGeometryShader);

		// Save pixel shader
		if(m_pPixelShader)
			addChild(m_pPixelShader);
	}
//----------------------------------------------------------------------
	void RenderPass::parse(XMLDocument* _node)
	{
		XMLDocument* pNode = _node;

		// Parse node data
		string key = pNode->getAttribute("order");
		if(key.compare("scene") == 0)
			m_Order = DO_SCENE;
		if(key.compare("post") == 0)
			m_Order = DO_POST;

		key = pNode->getAttribute("run_count");
		if(key.compare("") != 0)
			m_nRunCount = atoi(key.c_str());


		// Parse child nodes
		XMLDocument* pChild = pNode->getFirstChild();
		XMLDocument* pTemp = NULL;
		ShaderManager& manager = ShaderManager::getSingleton();

		while(pChild)
		{
			string tagName = pChild->getTagName();

			// Load vertex shader
			if(tagName == "vertex_shader")
			{
				string file = pChild->getAttribute("file");
				wstring shadFile(file.begin(), file.end());
				string name = pChild->getAttribute("name");
				wstring shadName = shadFile;
				if(name.compare("") != 0)
					wstring shadName(name.begin(), name.end());


				if(!manager.findShader(shadName))
				{
					SHADER_DESC desc;
					desc.fileName = shadFile;
					desc.name = shadName;
					desc.type = SHADER_VERTEX;

					Shader* pVertexShader = manager.createShader(desc);
					pVertexShader->parse(pChild);
				}
				
				m_pVertexShader = new ShaderInstance();
				m_pVertexShader->setShader(manager.getShader(shadName));
				m_pVertexShader->parse(pChild);
			}
			// Load geometry shader
			else if(tagName == "geometry_shader")
			{
				string file = pChild->getAttribute("file");
				wstring shadFile(file.begin(), file.end());
				string name = pChild->getAttribute("name");
				wstring shadName = shadFile;
				if(name.compare("") != 0)
					wstring shadName(name.begin(), name.end());

				if(!manager.findShader(shadName))
				{
					SHADER_DESC desc;
					desc.fileName = shadName;
					desc.name = shadName;
					desc.type = SHADER_GEOMETRY;

					Shader* pGeometryShader = manager.createShader(desc);
					pGeometryShader->parse(pChild);
				}

				m_pGeometryShader = new ShaderInstance();
				m_pGeometryShader->setShader(manager.getShader(shadName));
				m_pGeometryShader->parse(pChild);
			}
			// Load pixel shader
			else if(tagName == "pixel_shader")
			{
				string file = pChild->getAttribute("file");
				wstring shadFile(file.begin(), file.end());
				string name = pChild->getAttribute("name");
				wstring shadName = shadFile;
				if(name.compare("") != 0)
					wstring shadName(name.begin(), name.end());

				if(!manager.findShader(shadName))
				{
					SHADER_DESC desc;
					desc.fileName = shadName;
					desc.name = shadName;
					desc.type = SHADER_PIXEL;

					Shader* pPixelShader = manager.createShader(desc);
					pPixelShader->parse(pChild);
				}
				
				m_pPixelShader = new ShaderInstance();
				m_pPixelShader->setShader(manager.getShader(shadName));
				m_pPixelShader->parse(pChild);
			}
			// Load/create texture
			else if(tagName == "texture")
			{
				TextureSampler sampler;
				sampler.parse(pChild);
				if((!sampler.isForceSet()) && sampler.getTexture()->getTextureDesc().usage == TU_RENDERTARGET)
					m_vRenderTargets.push_back(sampler);
				else
					m_vSamplers.push_back(sampler);
			}
			// Create sampler
			else if(tagName == "sampler")
			{
				TextureSampler sampler;
				sampler.parse(pChild);
				if((!sampler.isForceSet()) && sampler.getTexture()->getTextureDesc().usage == TU_RENDERTARGET)
					m_vRenderTargets.push_back(sampler);
				else
					m_vSamplers.push_back(sampler);
			}

			pTemp = pChild;
			pChild = pChild->getNextSibling();
			NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	TextureSampler& RenderPass::getRenderTarget(uint _index)
	{
		Assert(_index < m_vRenderTargets.size(), "Index out of bounds");

		return m_vRenderTargets[_index];
	}
//----------------------------------------------------------------------
	uint RenderPass::getRenderTargetsNum() const
	{
		return m_vRenderTargets.size();
	}
//----------------------------------------------------------------------
	void RenderPass::addTextureSampler(const TextureSampler& _sampler)
	{
		m_vSamplers.push_back(_sampler);
	}
//----------------------------------------------------------------------
	TextureSampler& RenderPass::getTextureSampler(uint _index)
	{
		if(_index > m_vSamplers.size() - 1)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Index: %d out of bound: %d", _index, m_vSamplers.size() - 1);
			_index = m_vSamplers.size() - 1;
		}

		return m_vSamplers[_index];
	}
//----------------------------------------------------------------------
	uint RenderPass::getTextureSamplersNum() const
	{
		return m_vSamplers.size();
	}
//----------------------------------------------------------------------
}
