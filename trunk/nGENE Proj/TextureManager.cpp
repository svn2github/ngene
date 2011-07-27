/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TextureManager.cpp
Version:	0.11
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "TextureManager.h"
#include "ITexture.h"
#include "TextureDX9.h"
#include "TextureDX10.h"
#include "XMLDocument.h"
#include "XMLNode.h"


namespace nGENE
{
	TextureManager::TextureManager()
	{
	}
//----------------------------------------------------------------------
	TextureManager::~TextureManager()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void TextureManager::cleanup()
	{
		if(!m_pTextures.empty())
		{
			HashTable <wstring, ITexture*>::iterator iter;
			for(iter = m_pTextures.begin(); iter != m_pTextures.end(); ++iter)
				NGENE_DELETE(iter->second);

			m_pTextures.clear();
		}

		if(!m_pWaitingTextures.empty())
		{
			HashTable <wstring, XMLDocument*>::iterator iter;
			for(iter = m_pWaitingTextures.begin(); iter != m_pWaitingTextures.end(); ++iter)
				NGENE_DELETE(iter->second);

			m_pWaitingTextures.clear();
		}
	}
//----------------------------------------------------------------------
	ITexture* TextureManager::createTexture(const wstring& _name, uint _width, uint _height,
		TEXTURE_TYPE _type, TEXTURE_USAGE _usage, TEXTURE_FORMAT _format)
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Creating texture: %ls", _name.c_str());

		// If texture exists, return it
		if(m_pTextures.find(_name) != m_pTextures.end())
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Texture: %ls already exists", _name.c_str());
			return m_pTextures[_name];
		}

		// Create new texture
		switch(Engine::getSingleton().getRenderingAPI())
		{
		case API_DIRECTX9C: m_pTextures[_name] = new TextureDX9();
							m_pTextures[_name]->create(_width, _height, _type, _usage, _format);
							break;

		case API_DIRECTX10: m_pTextures[_name] = new TextureDX10();
							m_pTextures[_name]->create(_width, _height, _type, _usage, _format);
							break;
		}

		return m_pTextures[_name];
	}
//----------------------------------------------------------------------
	ITexture* TextureManager::createTexture(const wstring& _name, const TEXTURE_DESC& _desc)
	{
		return (createTexture(_name, _desc.width, _desc.height, _desc.type,
			_desc.usage, _desc.format));
	}
//----------------------------------------------------------------------
	ITexture* TextureManager::createTextureFromFile(const wstring& _name,
		const wstring& _fileName, TEXTURE_TYPE _type, TEXTURE_USAGE _usage,
		TEXTURE_FORMAT _format)
	{
		// If texture exists, return it
		if(m_pTextures.find(_name) != m_pTextures.end())
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Texture: %ls already exists", _name.c_str());
			return m_pTextures[_name];
		}

		// Create new texture
		switch(Engine::getSingleton().getRenderingAPI())
		{
		case API_DIRECTX9C: m_pTextures[_name] = new TextureDX9();
							m_pTextures[_name]->createFromFile(_fileName,
															   _type,
															   _usage,
															   _format);
							break;
		case API_DIRECTX10: m_pTextures[_name] = new TextureDX10();
							m_pTextures[_name]->createFromFile(_fileName,
															   _type,
															   _usage,
															   _format);
							break;
		}

		return m_pTextures[_name];
	}
//----------------------------------------------------------------------
	ITexture* TextureManager::createTextureFromFile(const wstring& _name,
		const TEXTURE_DESC& _desc)
	{
		return (createTextureFromFile(_name, _desc.filename, _desc.type,
									  _desc.usage, _desc.format));
	}
//----------------------------------------------------------------------
	void TextureManager::bind(const wstring& _name)
	{
		// If texture doesn't exist, exit
		if(m_pTextures.find(_name) == m_pTextures.end())
		{
			if(m_pWaitingTextures.find(_name) == m_pWaitingTextures.end())
			{
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Texture: %ls does not exist", _name.c_str());
				return;
			}

			textureAccessed(_name);
		}

		m_pTextures[_name]->bind();
	}
//----------------------------------------------------------------------
	ITexture* TextureManager::getTexture(const wstring& _name)
	{
		if(m_pTextures.find(_name) == m_pTextures.end())
		{
			if(m_pWaitingTextures.find(_name) == m_pWaitingTextures.end())
			{
				Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
					L"Texture: %ls does not exist!", _name.c_str());
				return NULL;
			}

			textureAccessed(_name);
		}

		return m_pTextures[_name];
	}
//----------------------------------------------------------------------
	void TextureManager::setTexture(const wstring& _name, ITexture* _texture)
	{
		m_pTextures[_name] = _texture;
	}
//----------------------------------------------------------------------
	bool TextureManager::findTexture(const wstring& _name)
	{
		if(m_pTextures.find(_name) == m_pTextures.end())
		{
			if(m_pWaitingTextures.find(_name) == m_pWaitingTextures.end())
				return false;
			else
				textureAccessed(_name);
		}

		return true;
	}
//----------------------------------------------------------------------
	void TextureManager::removeTexture(const wstring& _name)
	{
		HashTable <wstring, ITexture*>::iterator iter = m_pTextures.find(_name);
		if(iter == m_pTextures.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Texture: %ls does not exist!", _name.c_str());
			return;
		}

		NGENE_DELETE(iter->second);
		m_pTextures.erase(iter);
	}
//----------------------------------------------------------------------
	void TextureManager::removeTexture(ITexture* _texture)
	{
		HashTable <wstring, ITexture*>::iterator iter;
		for(iter = m_pTextures.begin(); iter != m_pTextures.end(); ++iter)
		{
			if(iter->second == _texture)
				break;
		}

		if(iter == m_pTextures.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Texture does not exist!");
			return;
		}

		NGENE_DELETE(iter->second);
		m_pTextures.erase(iter);
	}
//----------------------------------------------------------------------
	ITexture* TextureManager::createTextureFromXML(XMLDocument* _node, bool _onDemand)
	{
		if(_onDemand)
		{
			string name = _node->getAttribute("name");
			wstring stName(name.begin(), name.end());
			m_pWaitingTextures[stName] = _node;

			return NULL;
		}
		else
		{
			return parseTexture(_node);
		}
	}
//----------------------------------------------------------------------
	void TextureManager::textureAccessed(const wstring& _name)
	{
		if(m_pWaitingTextures.find(_name) == m_pWaitingTextures.end())
			return;

		parseTexture(m_pWaitingTextures[_name]);

		// Release memory
		NGENE_DELETE(m_pWaitingTextures[_name]);
		m_pWaitingTextures.erase(_name);
	}
//----------------------------------------------------------------------
	ITexture* TextureManager::parseTexture(XMLDocument* _node)
	{
		XMLDocument* pNode = _node;
		ITexture* pTexture = NULL;


		const SDisplayMode& dispMode = Renderer::getSingleton().getDisplayMode();


		string name = pNode->getAttribute("name");
		wstring stName(name.begin(), name.end());

		// Texture format
		string stFormat = pNode->getAttribute("format");
		TEXTURE_FORMAT format = parseFormat(stFormat);

		// Texture usage
		string stUsage = pNode->getAttribute("usage");
		TEXTURE_USAGE usage = TU_NORMAL;
		if(stUsage.compare("render_target") == 0)
			usage = TU_RENDERTARGET;

		// Texture file data
		string stFile = pNode->getAttribute("file");
		if(stFile.compare("") == 0 || usage  == TU_RENDERTARGET)
		{
			uint nWidth = dispMode.width;
			uint nHeight= dispMode.height;
			string stWidth = pNode->getAttribute("width_ratio");
			string stHeight= pNode->getAttribute("height_ratio");
			float ratio_x = 1.0f;
			float ratio_y = 1.0f;
			float ratio = 1.0f;
			if(stWidth.compare(""))
			{
				ratio = atof(stWidth.c_str());
				if(ratio <= 2.0f && stWidth.find("px") == string::npos)
				{
					nWidth = FastFloat::floatToInt(dispMode.width * ratio);
					ratio_x = ratio;
				}
				else
				{
					nWidth = FastFloat::floatToInt(ratio);
					ratio_x = 0.0f;
				}
			}
			if(stHeight.compare(""))
			{
				ratio = atof(stHeight.c_str());
				if(ratio <= 2.0f && stHeight.find("px") == string::npos)
				{
					nHeight = FastFloat::floatToInt(dispMode.height * ratio);
					ratio_y = ratio;
				}
				else
				{
					nHeight = FastFloat::floatToInt(ratio);
					ratio_y = 0.0f;
				}
			}

			pTexture = createTexture(stName, nWidth, nHeight,
				TT_2D, usage, format);
			pTexture->setRatio(ratio_x, ratio_y);
		}
		else
		{
			wstring stFileName(stFile.begin(), stFile.end());
			pTexture = createTextureFromFile(stName, stFileName, TT_2D,
				usage, format);
		}


		return pTexture;
	}
//----------------------------------------------------------------------
	TEXTURE_FORMAT TextureManager::parseFormat(const string& _data)
	{
		TEXTURE_FORMAT format = TF_A8R8G8B8;
		if(_data.compare("g16r16f") == 0)
			format = TF_G16R16F;
		else if(_data.compare("a8r8g8b8") == 0)
			format = TF_A8R8G8B8;
		else if(_data.compare("a2r10g10b10") == 0)
			format = TF_A2R10G10B10;
		else if(_data.compare("r8g8b8") == 0)
			format = TF_R8G8B8;
		else if(_data.compare("a1r5g5b5") == 0)
			format = TF_A1R5G5B5;
		else if(_data.compare("r5g5b5") == 0)
			format = TF_R5G5B5;
		else if(_data.compare("r5g6b5") == 0)
			format = TF_R5G6B5;
		else if(_data.compare("a16b16g16r16f") == 0)
			format = TF_A16B16G16R16F;
		else if(_data.compare("g32r32f") == 0)
			format = TF_G32R32F;
		else if(_data.compare("r32f") == 0)
			format = TF_R32F;
		else if(_data.compare("a132b32g32r32f") == 0)
			format = TF_A32B32G32R32F;
		else if(_data.compare("r16f") == 0)
			format = TF_R16F;
		else if(_data.compare("a8") == 0)
			format = TF_A8;

		return format;
	}
//----------------------------------------------------------------------
	string TextureManager::saveFormat(TEXTURE_FORMAT _format)
	{
		switch(_format)
		{
		case TF_G16R16F: return "g16r16f";
		case TF_A8R8G8B8: return "a8r8g8b8";
		case TF_A2R10G10B10: return "a2r10g10b10";
		case TF_R8G8B8: return "r8g8b8";
		case TF_A1R5G5B5: return "a1r5g5b5";
		case TF_R5G5B5: return "r5g5b5";
		case TF_R5G6B5: return "r5g6b5";
		case TF_A16B16G16R16F: return "a16b16g16r16f";
		case TF_G32R32F: return "g32r32f";
		case TF_R32F: return "r32f";
		case TF_A32B32G32R32F: return "a132b32g32r32f";
		case TF_R16F: return "r16f";
		case TF_A8: return "a8";
		default: return "a8r8g8b8";
		}
	}
//----------------------------------------------------------------------
	XMLNode* TextureManager::saveTexture(ITexture* _texture, const string& _name)
	{
		XMLNode* pNode = new XMLNode("texture");
		TEXTURE_DESC& desc = _texture->getTextureDesc();

		XMLNode::Attribute atName;
		atName.first = "name";
		atName.second = _name;

		if(desc.filename != L"")
		{
			XMLNode::Attribute atFileName;
			atFileName.first = "file";
			atFileName.second.assign(desc.filename.begin(),
				desc.filename.end());

			pNode->addAttribute(atFileName);
		}

		XMLNode::Attribute atHeight;
		stringstream buffer;
		buffer << desc.height;
		atHeight.first = "height_ratio";
		atHeight.second = buffer.str();

		XMLNode::Attribute atWidth;
		buffer.str("");
		buffer << desc.width;
		atWidth.first = "width_ratio";
		atWidth.second = buffer.str();

		XMLNode::Attribute atFormat;
		atFormat.first = "format";
		atFormat.second = saveFormat(desc.format);

		XMLNode::Attribute atUsage;
		atUsage.first = "usage";
		switch(desc.usage)
		{
		case TU_RENDERTARGET: atUsage.second = "render_target";
			break;
		default: atUsage.second = "normal";
			break;
		}


		pNode->addAttribute(atName);
		pNode->addAttribute(atHeight);
		pNode->addAttribute(atWidth);
		pNode->addAttribute(atFormat);
		pNode->addAttribute(atUsage);


		return pNode;
	}
//----------------------------------------------------------------------
}