/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		GUIFont.cpp
Version:	0.10
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "GUIFont.h"
#include "StateManager.h"
#include "TextureManager.h"
#include "VertexBufferManager.h"
#include "VertexBuffer.h"
#include "VertexDeclaration.h"
#include "XMLParser.h"



/// @todo Remove this
#include "GUIManager.h"
#include "Material.h"
#include "RenderTechnique.h"
#include "ITexture.h"
///


namespace nGENE
{
	GUIFont::GUIFont(const wstring& _fontDefFile, const wstring& _textureFile, uint _batchSize):
		m_pTexture(NULL),
		m_kBatchSize(_batchSize),
		m_nCurrentIndex(0),
		m_pVB(NULL),
		m_pIVB(NULL),
		m_pVertices(NULL),
		m_pIndices(NULL)
	{
		parse(_fontDefFile, _textureFile);
	}
//----------------------------------------------------------------------
	GUIFont::~GUIFont()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void GUIFont::cleanup()
	{
		NGENE_DELETE_ARRAY(m_Desc.chars);
	}
//----------------------------------------------------------------------
	void GUIFont::parse(const wstring& _fontDefFile, const wstring& _textureFile)
	{
		IFile* file = FileManager::getSingleton().openFile(_fontDefFile.c_str(), nGENE::OPEN_READ);
		if(!file)
			return;

		wstring temp = file->getBuffer();
		FileManager::getSingleton().closeFile(_fontDefFile.c_str());

		XMLDocument* pDocument = XMLParser::getSingleton().parse(temp);
		XMLDocument* pChild = pDocument->getFirstChild();
		XMLDocument* pTemp = NULL;

		string tagName;
		while(pChild)
		{
			tagName = pChild->getTagName();
			if(tagName == "common")
				parseFont(pChild);
			else if(tagName == "pages" && _textureFile.empty())
				parsePage(pChild);
			else if(tagName == "chars")
				parseChars(pChild);

			pTemp = pChild;
			pChild = pChild->getNextSibling();
			NGENE_DELETE(pTemp);
		}

		NGENE_DELETE(pDocument);

		if(!_textureFile.empty())
			m_pTexture = TextureManager::getSingleton().createTextureFromFile(_textureFile, _textureFile);


		createBuffers();
	}
//----------------------------------------------------------------------
	void GUIFont::parseFont(XMLDocument* _node)
	{
		string key = _node->getAttribute("lineHeight");
		m_Desc.lineHeight = atoi(key.c_str());

		key = _node->getAttribute("base");
		m_Desc.base = atoi(key.c_str());

		key = _node->getAttribute("scaleW");
		m_Desc.width = atoi(key.c_str());

		key = _node->getAttribute("scaleH");
		m_Desc.height = atoi(key.c_str());

		key = _node->getAttribute("pages");
		m_Desc.pages = atoi(key.c_str());
		if(m_Desc.pages != 1)
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__,
				__WFUNCTION__, __LINE__, L"Only one page per font is currently supported.");
		}
	}
//----------------------------------------------------------------------
	void GUIFont::parsePage(XMLDocument* _node)
	{
		XMLDocument* pChild = _node->getFirstChild();
		XMLDocument* pTemp = NULL;

		while(pChild)
		{
			if(pChild->getTagName() == "page")
			{
				string key = pChild->getAttribute("file");
				wstring filename(key.begin(), key.end());
				m_pTexture = TextureManager::getSingleton().createTextureFromFile(filename, filename);
			}

			pTemp = pChild;
			pChild = pChild->getNextSibling();
			NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	void GUIFont::parseChars(XMLDocument* _node)
	{
		string key = _node->getAttribute("count");
		uint count = atoi(key.c_str());
		///
		count = 256;
		///
		m_Desc.chars = new CHAR_DESC[count];

		XMLDocument* pChild = _node->getFirstChild();
		XMLDocument* pTemp = NULL;
		uint i = 0;

		while(pChild)
		{
			if(pChild->getTagName() == "char")
			{
				key = pChild->getAttribute("id");
				i = atoi(key.c_str());


				key = pChild->getAttribute("x");
				m_Desc.chars[i].x = atoi(key.c_str());

				key = pChild->getAttribute("y");
				m_Desc.chars[i].y = atoi(key.c_str());

				key = pChild->getAttribute("width");
				m_Desc.chars[i].width = atoi(key.c_str());

				key = pChild->getAttribute("height");
				m_Desc.chars[i].height = atoi(key.c_str());

				key = pChild->getAttribute("xoffset");
				m_Desc.chars[i].xOffset = atoi(key.c_str());

				key = pChild->getAttribute("yoffset");
				m_Desc.chars[i].yOffset = atoi(key.c_str());

				key = pChild->getAttribute("xadvance");
				m_Desc.chars[i].xAdvance = atoi(key.c_str());

				key = pChild->getAttribute("page");
				m_Desc.chars[i].page = atoi(key.c_str());
			}

			pTemp = pChild;
			pChild = pChild->getNextSibling();
			NGENE_DELETE(pTemp);
		}
	}
//----------------------------------------------------------------------
	void GUIFont::createBuffers()
	{
		VertexBufferManager& manager = VertexBufferManager::getSingleton();


		INDEXEDBUFFER_DESC ivbDesc;
		ivbDesc.indicesNum = m_kBatchSize * 6;
		ivbDesc.type = IVB_DYNAMIC;
		m_pIVB = manager.createIndexedBuffer(ivbDesc);

		VERTEXBUFFER_DESC vbDesc;
		vbDesc.verticesNum = m_kBatchSize * 4;
		vbDesc.primitivesNum = m_kBatchSize * 2;
		vbDesc.primitivesType = PT_TRIANGLELIST;
		vbDesc.type = VB_DYNAMIC;
		vbDesc.indices = m_pIVB;
		vbDesc.vertexSize = sizeof(SVertex2D);
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"Declaration2D");
		m_pVB = manager.createVertexBuffer(vbDesc);
	}
//----------------------------------------------------------------------
	float GUIFont::getTextWidth(const wstring& _text, uint _size) const
	{
		float result = 0;
		float size = (float)_size / (float)m_Desc.lineHeight;
		for(uint i = 0; i < _text.length(); ++i)
			result += getCharacterWidth(_text[i], _size);

		return result;
	}
//----------------------------------------------------------------------
	void GUIFont::begin()
	{
		Renderer& renderer = Renderer::getSingleton();

		// Save current render state
		RENDER_STATE renderState = renderer.getRenderState();
		StateManager::getSingleton().push(renderState);


		if(renderState.culling != CULL_NONE)
			renderer.setCullingMode(CULL_NONE);
		if(!(renderState.fillMode & FILL_SOLID))
			renderer.setFillMode(FILL_SOLID);

		renderer.set2DMode();

		// Lock buffers
		lock();
	}
//----------------------------------------------------------------------
	void GUIFont::end()
	{
		for(uint i = 0; i < m_vTexts.size(); ++i)
			render(m_vTexts[i]);
		m_vTexts.clear();

		// Draw anything left
		if(m_nCurrentIndex)
			flush();
		unlock();


		Renderer& renderer = Renderer::getSingleton();

		// Restore previous render state
		RENDER_STATE renderState = StateManager::getSingleton().pop();
		if(renderState.culling != CULL_NONE)
			renderer.setCullingMode(renderState.culling);
		if(!(renderState.fillMode & FILL_SOLID))
			renderer.setFillMode(renderState.fillMode);

		renderer.restore3DMode();
	}
//----------------------------------------------------------------------
	void GUIFont::render(const STextDesc& _text)
	{
		uint index1 = (m_nCurrentIndex << 2);
		uint index2 = m_nCurrentIndex * 6;

		float currentX = _text.x;
		float currentY = _text.y;
		float size = (float)_text.size / (float)m_Desc.lineHeight;

		Vector3 Pos;

		for(uint i = 0; i < _text.text.length(); ++i)
		{
			// Flush as the batch is at its maximum size
			if(m_nCurrentIndex == m_kBatchSize)
			{
				flush();
				
				index1 = (m_nCurrentIndex << 2);
				index2 = m_nCurrentIndex * 6;
			}

			wchar_t c = _text.text[i];
			if(c == L'\n')
			{
				currentY += m_Desc.lineHeight;
				currentX = _text.x;
			}

			CHAR_DESC& desc = m_Desc.chars[c];

			float x1 = currentX + float(desc.xOffset) * size;
			float y1 = currentY + float(desc.yOffset) * size;
			float x2 = currentX + float(desc.xOffset + desc.width) * size;
			float y2 = currentY + float(desc.yOffset + desc.height) * size;

			uint base = index1;

			m_pVertices[index1].position = Vector3(x1, y1, 0.5f);
			m_pVertices[index1].colour = _text.colour;
			m_pVertices[index1++].texCoord = Vector2((float)desc.x / (float)m_Desc.width, (float)desc.y / (float)m_Desc.height);
			m_pVertices[index1].position = Vector3(x2, y1, 0.5f);
			m_pVertices[index1].colour = _text.colour;
			m_pVertices[index1++].texCoord = Vector2((float)(desc.x + desc.width) / (float)m_Desc.width, (float)desc.y / (float)m_Desc.height);
			m_pVertices[index1].position = Vector3(x1, y2, 0.5f);
			m_pVertices[index1].colour = _text.colour;
			m_pVertices[index1++].texCoord = Vector2((float)desc.x / (float)m_Desc.width, (float)(desc.y + desc.height) / (float)m_Desc.height);
			m_pVertices[index1].position = Vector3(x2, y2, 0.5f);
			m_pVertices[index1].colour = _text.colour;
			m_pVertices[index1++].texCoord = Vector2((float)(desc.x + desc.width) / (float)m_Desc.width, (float)(desc.y + desc.height) / (float)m_Desc.height);

			m_pIndices[index2++] = base;
			m_pIndices[index2++] = base + 1;
			m_pIndices[index2++] = base + 3;
			m_pIndices[index2++] = base;
			m_pIndices[index2++] = base + 3;
			m_pIndices[index2++] = base + 2;


			++m_nCurrentIndex;

			// Move cursor
			currentX += float(desc.xAdvance) * size;
		}
	}
//----------------------------------------------------------------------
	void GUIFont::drawText(const wstring& _text, uint _x, uint _y, uint _size, const Colour& _colour)
	{
		STextDesc text = {_x, _y, _size, _colour.getDwordARGB(), _text};
		m_vTexts.push_back(text);
	}
//----------------------------------------------------------------------
	void GUIFont::flush()
	{
		unlock();


		Matrix4x4 newWorld;
		Renderer& renderer = Renderer::getSingleton();

		renderer.setWorldMatrix(newWorld);
		renderer.set2DMode();


		Material* pMaterial = GUIManager::getSingleton().getActiveMaterial();
		pMaterial->bind();


		m_pVB->prepare();

		RenderTechnique* pTechnique = pMaterial->getActiveRenderTechnique();
		for(uint i = 0; i < pTechnique->getPassesNum(); ++i)
		{
			RenderPass& pass = pTechnique->getRenderPass(i);
			// Scene pass, so skip it
			if(pass.getRenderingOrder() == DO_SCENE || !pass.isEnabled())
				continue;
			pass.bind();
			pass.update();

			m_pTexture->bind(0);

			m_pVB->render((m_nCurrentIndex << 2), (m_nCurrentIndex << 1));

			pass.unbind();
		}

		pMaterial->unbind();


		lock();

		m_nCurrentIndex = 0;
	}
//----------------------------------------------------------------------
	void GUIFont::lock()
	{
		m_pVB->lock(0, 0, (void**)(&m_pVertices));
		m_pIVB->lock(0, 0, (void**)(&m_pIndices));
	}
//----------------------------------------------------------------------
	void GUIFont::unlock()
	{
		// Unlock buffers
		m_pVB->unlock();
		m_pIVB->unlock();
	}
//----------------------------------------------------------------------
}