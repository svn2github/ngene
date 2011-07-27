/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Material.cpp
Version:	0.27
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Material.h"
#include "MaterialLibrary.h"
#include "RenderTechnique.h"
#include "RendererTypes.h"
#include "ShaderManager.h"
#include "StateManager.h"
#include "TextureManager.h"
#include "XMLDocument.h"

#include <sstream>


namespace nGENE
{
	// Initialize static members
	TypeInfo Material::Type(L"Material", NULL);


	Material::Material():
		m_pData(NULL),
		m_pMaterialLib(NULL),
		m_stName(L""),
		m_Ambient(Colour::COLOUR_BLACK),
		m_Diffuse(Colour::COLOUR_BLACK),
		m_Specular(Colour::COLOUR_BLACK),
		m_fEmissive(0.0f),
		m_fSpecularPower(0.0f),
		m_dwAlphaTestRef(1),
		m_FillMode(FILL_SOLID),
		m_bTwoSided(false),
		m_bZWrite(true),
		m_bZBuffer(true),
		m_fZBias(0.0f),
		m_bCastShadow(true),
		m_bTransparent(false),
		m_bAlphaTest(false),
		m_bLightable(true),
		m_bEnabled(true),
		m_bFullscreenScissor(false),
		m_DrawOrder(DO_SCENE),
		m_BlendOP(BO_ADD),
		m_SrcBlend(BLEND_ONE),
		m_DestBlend(BLEND_ONE),
		m_pParent(NULL),
		m_pActiveTechnique(NULL),
		m_nPriority(0),
		m_bCreated(false)
	{
	}
//----------------------------------------------------------------------
	Material::Material(const wstring& _name):
		m_pData(NULL),
		m_pMaterialLib(NULL),
		m_stName(_name),
		m_Ambient(Colour::COLOUR_BLACK),
		m_Diffuse(Colour::COLOUR_BLACK),
		m_Specular(Colour::COLOUR_BLACK),
		m_fEmissive(0.0f),
		m_fSpecularPower(0.0f),
		m_dwAlphaTestRef(1),
		m_FillMode(FILL_SOLID),
		m_bTwoSided(false),
		m_bZWrite(true),
		m_bZBuffer(true),
		m_fZBias(0.0f),
		m_bCastShadow(true),
		m_bTransparent(false),
		m_bAlphaTest(false),
		m_bLightable(false),
		m_bFullscreenScissor(false),
		m_bEnabled(true),
		m_DrawOrder(DO_SCENE),
		m_BlendOP(BO_ADD),
		m_SrcBlend(BLEND_ONE),
		m_DestBlend(BLEND_ONE),
		m_pParent(NULL),
		m_pActiveTechnique(NULL),
		m_nPriority(0),
		m_bCreated(false)
	{
	}
//----------------------------------------------------------------------
	Material::~Material()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void Material::cleanup()
	{
		if(m_pParent)
			m_pParent = NULL;

		if(!m_vTechniques.empty())
		{
			m_vTechniques.clear();
			m_pActiveTechnique = NULL;
		}

		NGENE_DELETE(m_pData);
	}
//----------------------------------------------------------------------
	void Material::bind()
	{
		materialAccessed();


		Renderer::getSingleton().setMaterial(*this);


		Renderer& renderer = Renderer::getSingleton();
		RENDER_STATE renderState = renderer.getRenderState();

		// Save current render state
		StateManager::getSingleton().push(renderState);


		if(m_bTransparent != renderState.alphaBlend)
			renderer.setAlphaBlendEnabled(m_bTransparent);
		if(m_bAlphaTest != renderState.isAlphaTest)
			renderer.setAlphaTestEnabled(m_bAlphaTest);
		if(m_dwAlphaTestRef != renderState.alphaTestRef)
			renderer.setAlphaTestReference(m_dwAlphaTestRef);
		if(m_SrcBlend != static_cast<uint>(renderState.source))
			renderer.setSourceBlending(static_cast<BLEND_TYPE>(m_SrcBlend));
		if(m_DestBlend != static_cast<uint>(renderState.destination))
			renderer.setDestinationBlending(static_cast<BLEND_TYPE>(m_DestBlend));
		if(m_BlendOP != renderState.blendOP)
			renderer.setBlendOperation(m_BlendOP);
		if(m_bZWrite != renderState.zWrite)
			renderer.setZWriteEnabled(m_bZWrite);
		if(m_bZBuffer != renderState.zBuffer)
			renderer.setZEnabled(m_bZBuffer);
		if(m_bTwoSided && renderState.culling != CULL_NONE)
			renderer.setCullingMode(CULL_NONE);
		if(m_fZBias != renderState.zBias)
			renderer.setZBias(m_fZBias);
		if(m_FillMode != renderState.fillMode)
			renderer.setFillMode(m_FillMode);
		if(m_bFullscreenScissor && !renderState.scissorTest)
		{
			SRect <uint> rect(0,
							  0,
							  renderer.getDisplayMode().width,
							  renderer.getDisplayMode().height);
			renderer.setScissorRectangle(rect);
			renderer.setScissorTestEnabled(m_bFullscreenScissor);
		}

		m_pActiveTechnique->bind();
	}
//----------------------------------------------------------------------
	void Material::unbind()
	{
		m_pActiveTechnique->unbind();


		Renderer& renderer = Renderer::getSingleton();
		RENDER_STATE renderState = StateManager::getSingleton().pop();


		if(m_bTwoSided && renderState.culling != CULL_NONE)
			renderer.setCullingMode(renderState.culling);
	}
//----------------------------------------------------------------------
	void Material::render(RenderQueue* _queue)
	{
		materialAccessed();

		if(!m_bEnabled)
			return;

		m_pActiveTechnique->render(_queue);
	}
//----------------------------------------------------------------------
	void Material::save(File* _file)
	{
		XMLNode::setName("material");

		// Add attributes
		Attribute atName;
		atName.first = "name";
		atName.second.assign(m_stName.begin(), m_stName.end());

		Attribute atTwoSided;
		ostringstream buffer;
		buffer << m_bTwoSided;
		atTwoSided.first = "two_sided";
		atTwoSided.second = buffer.str();

		Attribute atAmbient;
		buffer.str("");
		buffer << m_Ambient.toString();
		atAmbient.first = "ambient";
		atAmbient.second = buffer.str();

		Attribute atDiffuse;
		buffer.str("");
		buffer << m_Diffuse.toString();
		atDiffuse.first = "diffuse";
		atDiffuse.second = buffer.str();

		Attribute atSpecular;
		buffer.str("");
		buffer << m_Specular.toString();
		atSpecular.first = "specular";
		atSpecular.second = buffer.str();

		Attribute atEmissive;
		buffer.str("");
		buffer << m_fEmissive;
		atEmissive.first = "emissive_factor";
		atEmissive.second = buffer.str();

		Attribute atSpecularPower;
		buffer.str("");
		buffer << m_fSpecularPower;
		atSpecularPower.first = "specular_power";
		atSpecularPower.second = buffer.str();

		Attribute atZBias;
		buffer.str("");
		buffer << m_fZBias;
		atZBias.first = "z_bias";
		atZBias.second = buffer.str();

		Attribute atOrder;
		buffer.str("");
		switch(m_DrawOrder)
		{
		case DO_POST: buffer << "post";
			break;
		case DO_PRE: buffer << "pre";
			break;
		default: buffer << "scene";
			break;
		}
		atOrder.first = "order";
		atOrder.second = buffer.str();

		Attribute atBlendOp;
		buffer.str("");
		switch(m_BlendOP)
		{
		case BO_ADD: buffer << "add";
			break;
		case BO_SUBTRACT: buffer << "subtract";
			break;
		case BO_REVSUBTRACT: buffer << "revsubtract";
			break;
		case BO_MIN: buffer << "min";
			break;
		case BO_MAX: buffer << "max";
			break;
		}
		atBlendOp.first = "blend_operation";
		atBlendOp.second = buffer.str();

		Attribute atSrcBlend;
		buffer.str("");
		buffer << m_SrcBlend;
		atSrcBlend.first = "src_blend";
		atSrcBlend.second = buffer.str();

		Attribute atDestBlend;
		buffer.str("");
		buffer << m_DestBlend;
		atDestBlend.first = "dest_blend";
		atDestBlend.second = buffer.str();

		Attribute atAlphaTestRef;
		buffer.str("");
		buffer << m_dwAlphaTestRef;
		atAlphaTestRef.first = "alpha_test_ref";
		atAlphaTestRef.second = buffer.str();

		Attribute atFillMode;
		buffer.str("");
		switch(m_FillMode)
		{
		case FILL_DOT: buffer << "point";
			break;
		case FILL_WIREFRAME: buffer << "wireframe";
			break;
		default: buffer << "solid";
			break;
		}
		atFillMode.first = "fill_mode";
		atFillMode.second = buffer.str();

		Attribute atZWrite;
		buffer.str("");
		buffer << m_bZWrite;
		atZWrite.first = "z_write";
		atZWrite.second = buffer.str();

		Attribute atZBuffer;
		buffer.str("");
		buffer << m_bZBuffer;
		atZBuffer.first = "z_buffer";
		atZBuffer.second = buffer.str();

		Attribute atCastShadow;
		buffer.str("");
		buffer << m_bCastShadow;
		atCastShadow.first = "cast_shadow";
		atCastShadow.second = buffer.str();

		Attribute atTransparent;
		buffer.str("");
		buffer << m_bTransparent;
		atTransparent.first = "transparent";
		atTransparent.second = buffer.str();

		Attribute atAlphaTest;
		buffer.str("");
		buffer << m_bAlphaTest;
		atAlphaTest.first = "alpha_test";
		atAlphaTest.second = buffer.str();

		Attribute atLightable;
		buffer.str("");
		buffer << m_bLightable;
		atLightable.first = "lightable";
		atLightable.second = buffer.str();

		Attribute atScissor;
		buffer.str("");
		buffer << m_bFullscreenScissor;
		atScissor.first = "scissor";
		atScissor.second = buffer.str();


		addAttribute(atName);
		addAttribute(atTwoSided);
		addAttribute(atAmbient);
		addAttribute(atDiffuse);
		addAttribute(atSpecular);
		addAttribute(atEmissive);
		addAttribute(atSpecularPower);
		addAttribute(atZBias);
		addAttribute(atOrder);
		addAttribute(atBlendOp);
		addAttribute(atSrcBlend);
		addAttribute(atDestBlend);
		addAttribute(atAlphaTestRef);
		addAttribute(atFillMode);
		addAttribute(atZWrite);
		addAttribute(atZBuffer);
		addAttribute(atCastShadow);
		addAttribute(atTransparent);
		addAttribute(atAlphaTest);
		addAttribute(atLightable);
		addAttribute(atScissor);


		// Save techniques
		for(uint i = 0; i < m_vTechniques.size(); ++i)
			addChild(&m_vTechniques[i]);

		// Save
		XMLNode::save(_file);
	}
//----------------------------------------------------------------------
	void Material::createMaterial(XMLDocument* _node)
	{
		if(m_bCreated)
			return;

		SharedObject sharedObj(&m_CriticalSection);

		// Double guardian
		if(m_bCreated)
			return;

		XMLDocument* pNode = _node;

		// Parse data

		// Info about colours
		string key = pNode->getAttribute("emissive_factor");
		if(key.compare("") != 0)
			m_fEmissive = atof(key.c_str());

		key = pNode->getAttribute("diffuse");
		if(key.compare("") != 0)
			m_Diffuse.fromString(key);

		key = pNode->getAttribute("ambient");
		if(key.compare("") != 0)
			m_Ambient.fromString(key);

		key = pNode->getAttribute("specular");
		if(key.compare("") != 0)
			m_Specular.fromString(key);


		// Other attributes
		key = pNode->getAttribute("two_sided");
		if(key.compare("1") == 0 || key.compare("true") == 0)
			m_bTwoSided = true;
		else if(key.compare("0") == 0 || key.compare("false") == 0)
			m_bTwoSided = false;

		key = pNode->getAttribute("transparent");
		if(key.compare("1") == 0 || key.compare("true") == 0)
			m_bTransparent = true;
		else if(key.compare("0") == 0 || key.compare("false") == 0)
			m_bTransparent = false;

		key = pNode->getAttribute("lightable");
		if(key.compare("1") == 0 || key.compare("true") == 0)
			m_bLightable = true;
		else if(key.compare("0") == 0 || key.compare("false") == 0)
			m_bLightable = false;

		key = pNode->getAttribute("alpha_test");
		if(key.compare("1") == 0 || key.compare("true") == 0)
			m_bAlphaTest = true;
		else if(key.compare("0") == 0 || key.compare("false") == 0)
			m_bAlphaTest = false;

		key = pNode->getAttribute("alpha_test_ref");
		if(key.compare("") != 0)
			m_dwAlphaTestRef = static_cast<uint>(atoi(key.c_str()));

		key = pNode->getAttribute("src_blend");
		if(key.compare("") != 0)
			m_SrcBlend = static_cast<BLEND_TYPE>(atoi(key.c_str()));

		key = pNode->getAttribute("dest_blend");
		if(key.compare("") != 0)
			m_DestBlend = static_cast<BLEND_TYPE>(atoi(key.c_str()));

		key = pNode->getAttribute("z_write");
		if(key.compare("1") == 0 || key.compare("true") == 0)
			m_bZWrite = true;
		else if(key.compare("0") == 0 || key.compare("false") == 0)
			m_bZWrite = false;

		key = pNode->getAttribute("z_buffer");
		if(key.compare("1") == 0 || key.compare("true") == 0)
			m_bZBuffer = true;
		else if(key.compare("0") == 0 || key.compare("false") == 0)
			m_bZBuffer = false;

		key = pNode->getAttribute("z_bias");
		if(key.compare("") != 0)
			m_fZBias = atof(key.c_str());

		key = pNode->getAttribute("cast_shadow");
		if(key.compare("1") == 0 || key.compare("true") == 0)
			m_bCastShadow = true;
		else if(key.compare("0") == 0 || key.compare("false") == 0)
			m_bCastShadow = false;

		key = pNode->getAttribute("scissor");
		if(key.compare("1") == 0 || key.compare("true") == 0)
			m_bFullscreenScissor = true;
		else if(key.compare("0") == 0 || key.compare("false") == 0)
			m_bFullscreenScissor = false;

		// Get blending operation
		key = pNode->getAttribute("blend_operation");
		if(key.compare("add") == 0)
			m_BlendOP = BO_ADD;
		else if(key.compare("subtract") == 0)
			m_BlendOP = BO_SUBTRACT;
		else if(key.compare("revsubtract") == 0)
			m_BlendOP = BO_REVSUBTRACT;
		else if(key.compare("min") == 0)
			m_BlendOP = BO_MIN;
		else if(key.compare("max") == 0)
			m_BlendOP = BO_MAX;

		// Get draw order
		key = pNode->getAttribute("order");
		if(key.compare("post") == 0)
			m_DrawOrder = DO_POST;
		else if(key.compare("pre") == 0)
			m_DrawOrder = DO_PRE;
		else if(key.compare("scene") == 0)
			m_DrawOrder = DO_SCENE;

		// Get fill mode
		key = pNode->getAttribute("fill_mode");
		if(key.compare("point") == 0)
			m_FillMode = FILL_DOT;
		else if(key.compare("wireframe") == 0)
			m_FillMode = FILL_WIREFRAME;
		else if(key.compare("solid") == 0)
			m_FillMode = FILL_SOLID;
		else
			m_FillMode = FILL_SOLID;


		m_vTechniques.clear();


		// Parse child nodes
		XMLDocument* pChild = pNode->getFirstChild();
		XMLDocument* pTemp = NULL;
		while(pChild)
		{
			string tagName = pChild->getTagName();

			// Parse render techniques
			if(tagName == "technique")
			{
				string name = pChild->getAttribute("name");
				wstring techniqueName;
				techniqueName.assign(name.begin(), name.end());

				RenderTechnique technique;
				technique.setMaterial(this);
				technique.setTechniqueName(techniqueName);
				technique.parse(pChild);

				/// Check if there are no techniques with the same name
				vector <RenderTechnique>::iterator iter, iter2;
				for(iter = m_vTechniques.begin(); iter != m_vTechniques.end(); ++iter)
				{
					wstring name = iter->getName();
					if(name == techniqueName)
						break;
				}

				if(iter == m_vTechniques.end())
				{
					m_vTechniques.push_back(technique);
				}
				else
				{
					Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
						L"Material: %ls have more than one technique with a name: %ls",
						Material::m_stName.c_str(), techniqueName.c_str());

					return;
				}
			}
			// Parse child materials
			else if(tagName == "material")
			{
				Material childMat(*this);
				MaterialLibrary* pMatLib = getMaterialLibrary();

				childMat.setParent(this);
				childMat.setMaterialLibrary(pMatLib);
				childMat.parse(pChild);
				pMatLib->addMaterial(childMat.getName(), childMat);
			}

			pTemp = pChild;
			pChild = pChild->getNextSibling();
			NGENE_DELETE(pTemp);
		}

		if(m_vTechniques.empty())
		{
			RenderTechnique technique;
			technique.setTechniqueName(L"default");
			addRenderTechnique(technique);
		}

		// Set first technique as active
		m_pActiveTechnique = &m_vTechniques[0];


		m_bCreated = true;
	}
//----------------------------------------------------------------------
	void Material::parse(XMLDocument* _node)
	{
		if(m_pMaterialLib->isLoadedOnDemand())
		{
			string key = _node->getAttribute("name");
			Material::m_stName.assign(key.begin(), key.end());

			m_pData = _node;
		}
		else
		{
			string key = _node->getAttribute("name");
			Material::m_stName.assign(key.begin(), key.end());

			createMaterial(_node);
		}
	}
//----------------------------------------------------------------------
	void Material::addRenderTechnique(RenderTechnique& _technique)
	{
		materialAccessed();

		m_vTechniques.push_back(_technique);

		// Set first technique as active
		m_pActiveTechnique = &m_vTechniques[0];
	}
//----------------------------------------------------------------------
	void Material::setActiveRenderTechnique(uint _index)
	{
		materialAccessed();

		if(_index >= m_vTechniques.size())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"No technique with index: %d for material: %ls",
				_index, Material::m_stName.c_str());
			return;
		}

		m_pActiveTechnique = &m_vTechniques[_index];
	}
//----------------------------------------------------------------------
	void Material::setActiveRenderTechnique(RenderTechnique* _technique)
	{
		materialAccessed();

		if(_technique)
			m_pActiveTechnique = _technique;
	}
//----------------------------------------------------------------------
	bool Material::hasRenderTechnique(const wstring& _name)
	{
		materialAccessed();

		for(uint i = 0; i < m_vTechniques.size(); ++i)
		{
			if(m_vTechniques[i].getName() == _name)
				return true;
		}

		return false;
	}
//----------------------------------------------------------------------
	RenderTechnique* Material::getRenderTechnique(const wstring& _name)
	{
		materialAccessed();

		for(uint i = 0; i < m_vTechniques.size(); ++i)
		{
			if(m_vTechniques[i].getName() == _name)
				return &m_vTechniques[i];
		}

		return NULL;
	}
//----------------------------------------------------------------------
	RenderTechnique* Material::getActiveRenderTechnique()
	{
		materialAccessed();

		return m_pActiveTechnique;
	}
//----------------------------------------------------------------------
	void Material::materialAccessed()
	{
		// On-demand resource loading is turned on
		createMaterial(m_pData);
		NGENE_DELETE(m_pData);
	}
//----------------------------------------------------------------------
}