/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TextureSampler.cpp
Version:	0.12
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "ITexture.h"
#include "TextureManager.h"
#include "TextureSampler.h"
#include "XMLDocument.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo TextureSampler::Type(L"TextureSampler", NULL);


	TextureSampler::TextureSampler():
		m_pTexture(NULL),
		m_nTextureUnit(0),
		m_FilterMin(FILTER_ANISOTROPIC),
		m_FilterMag(FILTER_TRILINEAR),
		m_FilterMip(FILTER_TRILINEAR),
		m_AddressMode(TAM_CLAMP),
		m_dwClearFlags(CB_TARGET),
		m_bForceSet(true),
		m_bIsEnabled(true)
	{
	}
//----------------------------------------------------------------------
	TextureSampler::~TextureSampler()
	{
		m_pTexture = NULL;
	}
//----------------------------------------------------------------------
	void TextureSampler::save(File* _file)
	{
		XMLNode::setName("sampler");

		Attribute atUnit;
		stringstream buffer;
		buffer << m_nTextureUnit;
		atUnit.first = "sampler";
		atUnit.second = buffer.str();

		Attribute atFilterMin;
		atFilterMin.first = "filter_min";
		atFilterMin.second = saveFiltering(m_FilterMin);

		Attribute atFilterMag;
		atFilterMag.first = "filter_mag";
		atFilterMag.second = saveFiltering(m_FilterMag);

		Attribute atFilterMip;
		atFilterMip.first = "filter_mip";
		atFilterMip.second = saveFiltering(m_FilterMip);

		Attribute atAddressing;
		atAddressing.first = "addressing_mode";
		atAddressing.second = saveAddressing(m_AddressMode);

		Attribute atBindAsTexture;
		buffer.str("");
		buffer << m_bForceSet;
		atBindAsTexture.first = "bind_as_texture";
		atBindAsTexture.second = buffer.str();

		Attribute atClearTarget;
		buffer.str("");
		buffer << ((m_dwClearFlags & CB_TARGET) >> 3);
		atClearTarget.first = "clear_target";
		atClearTarget.second = buffer.str();

		Attribute atClearDepth;
		buffer.str("");
		buffer << ((m_dwClearFlags & CB_DEPTH) >> 1);
		atClearDepth.first = "clear_depth";
		atClearDepth.second = buffer.str();

		Attribute atTextureName;
		atTextureName.first = "name";
		atTextureName.second.assign(m_stTextureName.begin(), m_stTextureName.end());

		/// @todo remove this
		TEXTURE_DESC& desc = m_pTexture->getTextureDesc();

		
		if(desc.filename != L"")
		{
			Attribute atFileName;
			atFileName.first = "file";
			atFileName.second.assign(desc.filename.begin(),
				desc.filename.end());

			addAttribute(atFileName);
		}

		Attribute atHeight;
		buffer.str("");
		buffer << desc.height;
		atHeight.first = "height_ratio";
		atHeight.second = buffer.str();

		Attribute atWidth;
		buffer.str("");
		buffer << desc.width;
		atWidth.first = "width_ratio";
		atWidth.second = buffer.str();

		Attribute atFormat;
		atFormat.first = "format";
		atFormat.second = TextureManager::getSingleton().saveFormat(desc.format);

		Attribute atUsage;
		atUsage.first = "usage";
		switch(desc.usage)
		{
		case TU_RENDERTARGET: atUsage.second = "render_target";
			break;
		default: atUsage.second = "normal";
			break;
		}
		///


		addAttribute(atUnit);
		addAttribute(atFilterMin);
		addAttribute(atFilterMag);
		addAttribute(atFilterMip);
		addAttribute(atAddressing);
		addAttribute(atBindAsTexture);
		addAttribute(atClearTarget);
		addAttribute(atClearDepth);
		addAttribute(atTextureName);

		/// @todo remove this
		addAttribute(atHeight);
		addAttribute(atWidth);
		addAttribute(atFormat);
		addAttribute(atUsage);
		///


		XMLNode::save(_file);
	}
//----------------------------------------------------------------------
	string TextureSampler::saveFiltering(TEXTURE_FILTER _filter)
	{
		switch(_filter)
		{
		case FILTER_NEAREST: return "FILTER_NEAREST";
		case FILTER_TRILINEAR: return "FILTER_TRILINEAR";
		case FILTER_ANISOTROPIC: return "FILTER_ANISOTROPIC";
		default: return "FILTER_NONE";
		}
	}
//----------------------------------------------------------------------
	string TextureSampler::saveAddressing(TEXTURE_ADDRESSING_MODE _mode)
	{
		switch(_mode)
		{
		case TAM_CLAMP: return "ADDRESS_CLAMP";
		case TAM_BORDER: return "ADDRESS_BORDER";
		case TAM_MIRROR: return "ADDRESS_MIRROR";
		default: return "ADDRESS_WRAP";
		}
	}
//----------------------------------------------------------------------
	void TextureSampler::parse(XMLDocument* _node)
	{
		XMLDocument* pNode = _node;

		// Parse data (independent from texture file)
		string stTextureUnit = pNode->getAttribute("sampler");
		if(!stTextureUnit.compare("") == 0)
			m_nTextureUnit = atoi(stTextureUnit.c_str());

		if(pNode->getAttribute("filter_min") != "")
			m_FilterMin = parseFiltering(pNode->getAttribute("filter_min"));
		if(pNode->getAttribute("filter_mag") != "")
			m_FilterMag = parseFiltering(pNode->getAttribute("filter_mag"));
		if(pNode->getAttribute("filter_mip") != "")
			m_FilterMip = parseFiltering(pNode->getAttribute("filter_mip"));
		if(pNode->getAttribute("addressing_mode") != "")
			m_AddressMode = parseAddressing(pNode->getAttribute("addressing_mode"));
		if(pNode->getAttribute("bind_as_texture") != "")
		{
			string key = pNode->getAttribute("bind_as_texture");
			if(key.compare("1") == 0 || key.compare("true") == 0)
				m_bForceSet = true;
			else if(key.compare("0") == 0 || key.compare("false") == 0)
				m_bForceSet = false;
		}
		if(pNode->getAttribute("clear_target") != "")
		{
			string key = pNode->getAttribute("clear_target");
			if(key.compare("1") == 0 || key.compare("true") == 0)
				m_dwClearFlags |= CB_TARGET;
			else if(key.compare("0") == 0 || key.compare("false") == 0)
				m_dwClearFlags &= ~CB_TARGET;
		}
		if(pNode->getAttribute("clear_depth") != "")
		{
			string key = pNode->getAttribute("clear_depth");
			if(key.compare("1") == 0 || key.compare("true") == 0)
				m_dwClearFlags |= CB_DEPTH;
			else if(key.compare("0") == 0 || key.compare("false") == 0)
				m_dwClearFlags &= ~CB_DEPTH;
		}
		

		// Parse texture data
		TextureManager& texManager = TextureManager::getSingleton();
		string name = pNode->getAttribute("name");
		m_stTextureName.assign(name.begin(), name.end());

		ITexture* pTexture = NULL;
		if(!texManager.findTexture(m_stTextureName))
		{
			if(pNode->getAttribute("bind_as_texture") == "")
				m_bForceSet = false;
			
			// If we came that far, it has to be loaded NOW
			pTexture = texManager.createTextureFromXML(pNode, false);
		}
		else
		{
			// Texture already exists so just grab it
			pTexture = texManager.getTexture(m_stTextureName);

			if(pNode->getAttribute("usage") == "render_target" && pNode->getAttribute("bind_as_texture") == "")
				m_bForceSet = false;
		}

		
		m_pTexture = pTexture;
	}
//----------------------------------------------------------------------
	TEXTURE_FILTER TextureSampler::parseFiltering(const string& _data)
	{
		if(_data.compare("FILTER_NONE") == 0)
			return FILTER_NONE;
		else if(_data.compare("FILTER_NEAREST") == 0)
			return FILTER_NEAREST;
		else if(_data.compare("FILTER_TRILINEAR") == 0)
			return FILTER_TRILINEAR;
		else if(_data.compare("FILTER_ANISOTROPIC") == 0)
			return FILTER_ANISOTROPIC;
		else
			return FILTER_UNKNOWN;
	}
//----------------------------------------------------------------------
	TEXTURE_ADDRESSING_MODE TextureSampler::parseAddressing(const string& _data)
	{
		if(_data.compare("ADDRESS_WRAP") == 0)
			return TAM_WRAP;
		else if(_data.compare("ADDRESS_CLAMP") == 0)
			return TAM_CLAMP;
		else if(_data.compare("ADDRESS_BORDER") == 0)
			return TAM_BORDER;
		else if(_data.compare("ADDRESS_MIRROR") == 0)
			return TAM_MIRROR;
		else
			return TAM_WRAP;
	}
//----------------------------------------------------------------------
	void TextureSampler::bind()
	{
		// Skip as it is disabled.
		if(!m_bIsEnabled)
			return;

		m_pTexture->bind(m_nTextureUnit, m_bForceSet, m_dwClearFlags);

		// Bind sampler filtering
		Renderer& renderer = Renderer::getSingleton();
		if(renderer.getTextureFilter(m_nTextureUnit, FILTER_MIN) != m_FilterMin)
			renderer.setTextureFilter(m_nTextureUnit, FILTER_MIN, m_FilterMin);
		if(renderer.getTextureFilter(m_nTextureUnit, FILTER_MAG) != m_FilterMag)
			renderer.setTextureFilter(m_nTextureUnit, FILTER_MAG, m_FilterMag);
		if(renderer.getTextureFilter(m_nTextureUnit, FILTER_MIP) != m_FilterMip)
			renderer.setTextureFilter(m_nTextureUnit, FILTER_MIP, m_FilterMip);

		// Bind texture addressing mode
		if(renderer.getTextureAddressingMode(m_nTextureUnit) != m_AddressMode)
			renderer.setTextureAddressingMode(m_nTextureUnit, m_AddressMode);
	}
//----------------------------------------------------------------------
	void TextureSampler::unbind()
	{
		// Skip as it is disabled.
		if(!m_bIsEnabled)
			return;

		m_pTexture->unbind();
	}
//----------------------------------------------------------------------
	void TextureSampler::setTexture(const wstring& _name)
	{
		m_pTexture = TextureManager::getSingleton().getTexture(_name);
	}
//----------------------------------------------------------------------
}