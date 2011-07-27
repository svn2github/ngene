/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		VertexBufferManager.cpp
Version:	0.10
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "IndexedBufferDX9.h"
#include "VertexBufferDX9.h"
#include "VertexBufferManager.h"
#include "VertexDeclaration.h"
#include "VertexDeclarationDX9.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo VertexBufferManager::Type(L"VertexBufferManager", &BaseClass::Type);


	VertexBufferManager::VertexBufferManager():
		m_pVertexDeclFactoryDX9(NULL),
		m_pVertexBufferFactoryDX9(NULL),
		m_pIndexedBufferFactoryDX9(NULL),
		m_nVBIndex(0),
		m_nIVBIndex(0),
		m_pActiveVB(NULL),
		m_pActiveIVB(NULL),
		m_pActiveVertexDecl(NULL)
	{
	}
//----------------------------------------------------------------------
	VertexBufferManager::~VertexBufferManager()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void VertexBufferManager::init()
	{
		// Add default factories
		m_pVertexDeclFactoryDX9 = new VertexDeclarationDX9Factory();
		addVertexDeclarationFactory(*m_pVertexDeclFactoryDX9, API_DIRECTX9C);

		m_pVertexBufferFactoryDX9 = new VertexBufferDX9Factory();
		addVertexBufferFactory(*m_pVertexBufferFactoryDX9, API_DIRECTX9C);

		m_pIndexedBufferFactoryDX9 = new IndexedBufferDX9Factory();
		addIndexedBufferFactory(*m_pIndexedBufferFactoryDX9, API_DIRECTX9C);

		
		// Create default vertex declarations
		createDefaultDeclarations();
	}
//----------------------------------------------------------------------
	void VertexBufferManager::createDefaultDeclarations()
	{
		// Create default vertex declaration
		VERTEX_ELEMENT elements[4] =
		{
			{0, 0,  0, 3, VEU_POSITION},
			{0, 12, 0, 3, VEU_NORMAL},
			{0, 24, 0, 4, VEU_COLOUR},
			{0, 28, 0, 2, VEU_TEXCOORD}
		};

		createVertexDeclaration(L"Default", elements, 4);

		// Create simple non-lightable vertex declaration
		VERTEX_ELEMENT elementsSimple[2] =
		{
			{0, 0,  0, 3, VEU_POSITION},
			{0, 12, 0, 4, VEU_COLOUR}
		};

		createVertexDeclaration(L"NonLightable", elementsSimple, 2);

		// Create vertex 2D declaration
		VERTEX_ELEMENT elements2D[3] =
		{
			{0, 0,  0, 3, VEU_POSITION},
			{0, 12, 0, 4, VEU_COLOUR},
			{0, 16, 0, 2, VEU_TEXCOORD}
		};

		createVertexDeclaration(L"Declaration2D", elements2D, 3);

		// Create vertex skinning declaration
		VERTEX_ELEMENT elementsSkinning[6] =
		{
			{0, 0,  0, 3, VEU_POSITION},
			{0, 12, 0, 3, VEU_NORMAL},
			{0, 24, 0, 4, VEU_COLOUR},
			{0, 28, 0, 2, VEU_TEXCOORD},
			{0, 36, 0, 4, VEU_BLEND_WEIGHTS},
			{0, 52, 0, 4, VEU_BLEND_INDICES}
		};

		createVertexDeclaration(L"DeclarationSkinning", elementsSkinning, 6);
	}
//----------------------------------------------------------------------
	void VertexBufferManager::cleanup()
	{
		if(!m_VBs.empty())
		{
			HashTable <uint, VertexBuffer*>::iterator iter;
			for(iter = m_VBs.begin(); iter != m_VBs.end(); ++iter)
				NGENE_DELETE(iter->second);

			m_VBs.clear();
		}

		if(!m_IVBs.empty())
		{
			HashTable <uint, IndexedBuffer*>::iterator iter;
			for(iter = m_IVBs.begin(); iter != m_IVBs.end(); ++iter)
				NGENE_DELETE(iter->second);

			m_IVBs.clear();
		}

		if(!m_pVertexDecls.empty())
		{
			HashTable <wstring, VertexDeclaration*>::iterator iter;
			for(iter = m_pVertexDecls.begin(); iter != m_pVertexDecls.end(); ++iter)
				NGENE_DELETE(iter->second);

			m_pVertexDecls.clear();
		}

		if(!m_pVertexDeclFactories.empty())
			m_pVertexDeclFactories.clear();

		if(!m_pVertexBufFactories.empty())
			m_pVertexBufFactories.clear();

		if(!m_pIndexedBufFactories.empty())
			m_pIndexedBufFactories.clear();

		NGENE_DELETE(m_pVertexDeclFactoryDX9);
		NGENE_DELETE(m_pVertexBufferFactoryDX9);
		NGENE_DELETE(m_pIndexedBufferFactoryDX9);
	}
//----------------------------------------------------------------------
	void VertexBufferManager::addVertexDeclarationFactory(VertexDeclarationFactory& _factory, const API_TYPE& _api)
	{
		// If factory already exists, return
		if(m_pVertexDeclFactories.find(_api) != m_pVertexDeclFactories.end())
			return;

		m_pVertexDeclFactories[_api] = &_factory;
	}
//----------------------------------------------------------------------
	void VertexBufferManager::removeVertexDeclarationFactory(const API_TYPE& _api)
	{
		m_pVertexDeclFactories.erase(_api);
	}
//----------------------------------------------------------------------
	void VertexBufferManager::addVertexBufferFactory(VertexBufferFactory& _factory, const API_TYPE& _api)
	{
		// If factory already exists, return
		if(m_pVertexBufFactories.find(_api) != m_pVertexBufFactories.end())
			return;

		m_pVertexBufFactories[_api] = &_factory;
	}
//----------------------------------------------------------------------
	void VertexBufferManager::removeVertexBufferFactory(const API_TYPE& _api)
	{
		m_pVertexBufFactories.erase(_api);
	}
//----------------------------------------------------------------------
	void VertexBufferManager::addIndexedBufferFactory(IndexedBufferFactory& _factory, const API_TYPE& _api)
	{
		// If factory already exists, return
		if(m_pIndexedBufFactories.find(_api) != m_pIndexedBufFactories.end())
			return;

		m_pIndexedBufFactories[_api] = &_factory;
	}
//----------------------------------------------------------------------
	void VertexBufferManager::removeIndexedBufferFactory(const API_TYPE& _api)
	{
		m_pIndexedBufFactories.erase(_api);
	}
//----------------------------------------------------------------------
	VertexBuffer* VertexBufferManager::createVertexBuffer(const VERTEXBUFFER_DESC& _desc)
	{
		// Invalid buffer, so return
		if(!_desc.isValid())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Invalid vertex buffer description");
			return NULL;
		}

		// Choose appropriate vertex buffer type
		VertexBuffer* vb = NULL;
		API_TYPE api = Engine::getSingleton().getRenderingAPI();

		if(m_pVertexBufFactories.find(api) == m_pVertexBufFactories.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Vertex buffer factory does not exist!");
			return NULL;
		}
		
		vb = m_pVertexBufFactories[api]->create(_desc);

		// Set buffer settings
		vb->init();
		if(_desc.vertices && _desc.verticesNum)
			vb->setVertexData(_desc.vertices, 0, _desc.verticesNum);

		// Add vertex buffer to the table
		vb->setVBIndex(m_nVBIndex);
		m_VBs[m_nVBIndex++] = vb;

		return vb;
	}
//----------------------------------------------------------------------
	IndexedBuffer* VertexBufferManager::createIndexedBuffer(const INDEXEDBUFFER_DESC& _desc)
	{
		// Choose appropriate indexed buffer type
		IndexedBuffer* ivb = NULL;
		API_TYPE api = Engine::getSingleton().getRenderingAPI();

		if(m_pIndexedBufFactories.find(api) == m_pIndexedBufFactories.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Index buffer factory does not exist!");
			return NULL;
		}
		
		ivb = m_pIndexedBufFactories[api]->create(_desc);

		// Set buffer settings
		ivb->init();
		if(_desc.indices && _desc.indicesNum)
			ivb->setIndexData(_desc.indices, 0, _desc.indicesNum);

		// Add indexed buffer to the table
		ivb->setIVBIndex(m_nIVBIndex);
		m_IVBs[m_nIVBIndex++] = ivb;

		return ivb;
	}
//----------------------------------------------------------------------
	VertexDeclaration* VertexBufferManager::createVertexDeclaration(const wstring& _name, VERTEX_ELEMENT* _elements, uint _count)
	{
		// Choose appropriate vertex declaration type
		VertexDeclaration* pvdecl = NULL;
		API_TYPE api = Engine::getSingleton().getRenderingAPI();

		if(m_pVertexDeclFactories.find(api) == m_pVertexDeclFactories.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Vertex declaration factory does not exist!");
			return NULL;
		}

		// Create new input device
		pvdecl = m_pVertexDeclFactories[api]->create();
		pvdecl->addVertexElement(_elements, _count);

		// Add declaration to the table
		m_pVertexDecls[_name] = pvdecl;

		return pvdecl;
	}
//----------------------------------------------------------------------
	void VertexBufferManager::removeVertexBuffer(uint _key)
	{
		// Vertex Buffer doesn't exist
		if(m_VBs.find(_key) == m_VBs.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Vertex buffer with key: %d does not exist. It cannot be deleted", _key);
			return;
		}

		NGENE_DELETE(m_VBs[_key]);
		m_VBs.erase(_key);
	}
//----------------------------------------------------------------------
	void VertexBufferManager::removeVertexBuffer(VertexBuffer* _vb)
	{
		if(_vb->getVBIndex() > -1)
			removeVertexBuffer(_vb->getVBIndex());
		else
			NGENE_DELETE(_vb);
	}
//----------------------------------------------------------------------
	void VertexBufferManager::removeIndexedBuffer(uint _key)
	{
		// Indexed Buffer doesn't exist
		if(m_IVBs.find(_key) == m_IVBs.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Indexed buffer with key: %d does not exist. It cannot be deleted", _key);
			return;
		}

		NGENE_DELETE(m_IVBs[_key]);
		m_IVBs.erase(_key);
	}
//----------------------------------------------------------------------
	void VertexBufferManager::removeIndexedBuffer(IndexedBuffer* _ivb)
	{
		if(_ivb->getIVBIndex() > -1)
			removeIndexedBuffer(_ivb->getIVBIndex());
		else
			NGENE_DELETE(_ivb);
	}
//----------------------------------------------------------------------
	void VertexBufferManager::removeVertexDeclaration(const wstring& _name)
	{
		// Vertex declaration doesn't exist
		if(m_pVertexDecls.find(_name) == m_pVertexDecls.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Vertex declaration with name: %d does not exist. It cannot be deleted", _name);
			return;
		}

		NGENE_DELETE(m_pVertexDecls[_name]);
		m_pVertexDecls.erase(_name);
	}
//----------------------------------------------------------------------
	VertexDeclaration* VertexBufferManager::getVertexDeclaration(const wstring& _name)
	{
		// Vertex declaration doesn't exist
		if(m_pVertexDecls.find(_name) == m_pVertexDecls.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Vertex declaration with name: %d does not exist", _name);
			return NULL;
		}

		return m_pVertexDecls[_name];
	}
//----------------------------------------------------------------------
	bool VertexBufferManager::findVertexDeclaration(const wstring& _name)
	{
		// Vertex declaration doesn't exist
		if(m_pVertexDecls.find(_name) == m_pVertexDecls.end())
			return false;

		return true;
	}
//----------------------------------------------------------------------
	VertexBuffer* VertexBufferManager::mergeVertexBuffers(VertexBuffer* _buf1, VertexBuffer* _buf2)
	{
		// Vertex buffer doesn't exist
		if(!_buf1 || !_buf2)
			return NULL;

		// Both keys are equal
		if(_buf1 == _buf2)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"You cannot merge buffer with itself");
			return NULL;
		}

		VERTEXBUFFER_DESC& desc1 = _buf1->getDesc();
		VERTEXBUFFER_DESC& desc2 = _buf2->getDesc();

		// Create new buffer
		if((desc1.primitivesType == desc2.primitivesType) && (desc1.type == desc2.type) &&
		   (desc1.vertexSize == desc2.vertexSize))
		{
			dword vertexSize = desc1.vertexSize;

			VERTEXBUFFER_DESC newDesc;
			newDesc.indices = mergeIndexedBuffers(desc1.indices, desc2.indices);
			newDesc.primitivesNum = desc1.primitivesNum + desc2.primitivesNum;
			newDesc.primitivesType = desc1.primitivesType;
			newDesc.type = desc1.type;
			newDesc.vertexSize = vertexSize;
			newDesc.verticesNum = desc1.verticesNum + desc2.verticesNum;
			
			char* data1 = new char[desc1.verticesNum * vertexSize];
			char* data2 = new char[desc2.verticesNum * vertexSize];

			_buf1->getVertexData(data1, 0, desc1.verticesNum);
			_buf2->getVertexData(data2, 0, desc2.verticesNum);

			char* newData = new char[newDesc.verticesNum * vertexSize];
			char* dest = newData;

			memcpy(dest, data1, desc1.verticesNum * vertexSize);
			dest += desc1.verticesNum * vertexSize;
			memcpy(dest, data2, desc2.verticesNum * vertexSize);

			newDesc.vertices = newData;

			VertexBuffer* newBuf = createVertexBuffer(newDesc);


			NGENE_DELETE_ARRAY(data1);
			NGENE_DELETE_ARRAY(data2);
			NGENE_DELETE_ARRAY(newData);


			return newBuf;
		}

		return NULL;
	}
//----------------------------------------------------------------------
	IndexedBuffer* VertexBufferManager::mergeIndexedBuffers(IndexedBuffer* _buf1, IndexedBuffer* _buf2)
	{
		// Vertex buffer doesn't exist
		if(!_buf1 || !_buf2)
			return NULL;

		// Both keys are equal
		if(_buf1 == _buf2)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"You cannot merge buffer with itself");
			return NULL;
		}


		INDEXEDBUFFER_DESC& desc1 = _buf1->getDesc();
		INDEXEDBUFFER_DESC& desc2 = _buf2->getDesc();

		// Create new buffer
		if((desc1.indexSize == desc2.indexSize) && (desc1.is32bit == desc2.is32bit))
		{
			dword indexSize = desc1.indexSize;

			INDEXEDBUFFER_DESC newDesc;
			newDesc.indicesNum = desc1.indicesNum + desc2.indicesNum;
			newDesc.indexSize = indexSize;
			newDesc.is32bit = desc1.is32bit;
			
			char* data1 = new char[desc1.indicesNum * indexSize];
			char* data2 = new char[desc2.indicesNum * indexSize];

			_buf1->getIndexData(data1, 0, desc1.indicesNum);
			_buf2->getIndexData(data2, 0, desc2.indicesNum);

			char* newData = new char[newDesc.indicesNum * indexSize];
			char* dest = newData;

			memcpy(dest, data1, desc1.indicesNum * indexSize);
			dest += desc1.indicesNum * indexSize;
			memcpy(dest, data2, desc2.indicesNum * desc2.indexSize);

			newDesc.indices = newData;

			IndexedBuffer* newBuf = createIndexedBuffer(newDesc);


			NGENE_DELETE_ARRAY(data1);
			NGENE_DELETE_ARRAY(data2);
			NGENE_DELETE_ARRAY(newData);


			return newBuf;
		}

		return NULL;
	}
//----------------------------------------------------------------------
	void VertexBufferManager::bindVertexBuffer(VertexBuffer* _buffer)
	{
		if(!_buffer)
			m_pActiveVB = NULL;

		if(_buffer != m_pActiveVB)
		{
			m_pActiveVB = _buffer;
			m_pActiveVB->bind();
		}
	}
//----------------------------------------------------------------------
	void VertexBufferManager::bindIndexedBuffer(IndexedBuffer* _buffer)
	{
		if(!_buffer)
			m_pActiveIVB = NULL;

		if(_buffer != m_pActiveIVB)
		{
			m_pActiveIVB = _buffer;
			m_pActiveIVB->bind();
		}
	}
//----------------------------------------------------------------------
	void VertexBufferManager::bindVertexDeclaration(VertexDeclaration* _declare)
	{
		if(!_declare)
			m_pActiveVertexDecl = NULL;

		if(_declare != m_pActiveVertexDecl)
		{
			m_pActiveVertexDecl = _declare;
			m_pActiveVertexDecl->bind();
		}
	}
//----------------------------------------------------------------------
}
