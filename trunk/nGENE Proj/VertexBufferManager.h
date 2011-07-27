/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		VertexBufferManager.h
Version:	0.10
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_VERTEXBUFFERMANAGER_H_
#define __INC_VERTEXBUFFERMANAGER_H_


#include "BaseClass.h"
#include "HashTable.h"
#include "IndexedBuffer.h"
#include "Prerequisities.h"
#include "Singleton.h"
#include "TypeInfo.h"
#include "VertexBuffer.h"
#include "VertexBufferTypes.h"


namespace nGENE
{
	/** This class lets you oversee work of the vertex buffer in
		the application.
		@remarks
			Note that unless you provide your own mechanisms, you
			should not create vertex buffer by direct instance
			creation. It would be not efficient as such buffers
			could not be handled properly by the VertexBufferManager.
	*/
	class nGENEDLL VertexBufferManager: public BaseClass, public Singleton <VertexBufferManager>
	{
		EXPOSE_TYPE
	private:
		/// Hash table containing all factories for VertexDeclarations.
		HashTable <API_TYPE, VertexDeclarationFactory*> m_pVertexDeclFactories;
		/// Hash table containing all factories for VertexBuffers.
		HashTable <API_TYPE, VertexBufferFactory*> m_pVertexBufFactories;
		/// Hash table containing all factories for IndexedBuffers.
		HashTable <API_TYPE, IndexedBufferFactory*> m_pIndexedBufFactories;

		// Default factories:

		VertexDeclarationDX9Factory* m_pVertexDeclFactoryDX9;
		VertexBufferDX9Factory* m_pVertexBufferFactoryDX9;
		IndexedBufferDX9Factory* m_pIndexedBufferFactoryDX9;


		/** Structure holding current vertex buffers.
			@remarks
				Using HashTable structure to hold data is useful
				because we can easily control the buffers by just
				calling them by their keys. Besides it is more
				effective than regular map or multi-map structure.
		*/
		HashTable <uint, VertexBuffer*> m_VBs;

		/** Structure holding current indexed buffers.
			@remarks
				Using HashTable structure to hold data is useful
				because we can easily control the buffers by just
				calling them by their keys. Besides it is more
				effective than regular map or multi-map structure.
		*/
		HashTable <uint, IndexedBuffer*> m_IVBs;

		/// Hash table holding all vertex declarations.
		HashTable <wstring, VertexDeclaration*> m_pVertexDecls;

		/// Counter to specify index of the vertex buffer.
		uint m_nVBIndex;
		/// Counter to specify index of the indexed buffer.
		uint m_nIVBIndex;


		/// Active vertex buffer.
		VertexBuffer* m_pActiveVB;
		/// Active indexed buffer.
		IndexedBuffer* m_pActiveIVB;
		/// Active vertex declaration.
		VertexDeclaration* m_pActiveVertexDecl;


		void createDefaultDeclarations();

	public:
		VertexBufferManager();
		~VertexBufferManager();

		void init();
		void cleanup();

		/// Registers new factory for VertexDeclaration.
		void addVertexDeclarationFactory(VertexDeclarationFactory& _factory,
			const API_TYPE& _api);
		/// Removes factory.
		void removeVertexDeclarationFactory(const API_TYPE& _api);
		/// Registers new factory for VertexBuffer.
		void addVertexBufferFactory(VertexBufferFactory& _factory,
			const API_TYPE& _api);
		/// Removes factory.
		void removeVertexBufferFactory(const API_TYPE& _api);
		/// Registers new factory for IndexedBuffer.
		void addIndexedBufferFactory(IndexedBufferFactory& _factory,
			const API_TYPE& _api);
		/// Removes factory.
		void removeIndexedBufferFactory(const API_TYPE& _api);

		/** Adds new vertex buffer.
			@param
				_desc description of the vertex buffer.
		*/
		VertexBuffer* createVertexBuffer(const VERTEXBUFFER_DESC& _desc);

		/** Adds new indexed buffer.
			@param
				_indicesNum integer number specifying number of indices.
			@param
				_data data containing indices information.
		*/
		IndexedBuffer* createIndexedBuffer(const INDEXEDBUFFER_DESC& _desc);

		/** Creates new vertex declaration.
			@param
				_name name of the vertex declaration.
			@param
				_elements array of elements to be added to the vertex declaration.
			@count
				_count number of elements in the _elements array.
		*/
		VertexDeclaration* createVertexDeclaration(const wstring& _name,
			VERTEX_ELEMENT* _elements, uint _count);

		/** Removes existing vertex buffer.
			@param
				_key key of the vertex buffer to be removed.
		*/
		void removeVertexBuffer(uint _key);
		/// Removes vertex buffer by its pointer.
		void removeVertexBuffer(VertexBuffer* _vb);

		/** Removes existing indexed buffer.
			@param
				_key key of the indexed buffer to be removed.
		*/
		void removeIndexedBuffer(uint _key);
		/// Removes index buffer by its pointer.
		void removeIndexedBuffer(IndexedBuffer* _ivb);

		/** Removes existing vertex declaration.
			@param
				_name name of the vertex declaration to be removed.
		*/
		void removeVertexDeclaration(const wstring& _name);
	
		/** Merges two vertex buffers into one.
			@remarks
				Binding can be useful in case of having multiple buffers
				sharing vertex format and material and each containing
				a small amount of vertices. It is more effective to make one
				buffer of them.
			@param
				_buf1 first buffer.
			@param
				_buf2 second buffer.
			@param
				_remove if true than both source buffers will be remove. Otherwise
				they will be left untouched.
		*/
		VertexBuffer* mergeVertexBuffers(VertexBuffer* _buf1, VertexBuffer* _buf2);

		/** Merges two index buffers into one.
			@param
				_buf1 first buffer.
			@param
				_buf2 second buffer.
			@param
				_remove if true than both source buffers will be remove. Otherwise
				they will be left untouched.
		*/
		IndexedBuffer* mergeIndexedBuffers(IndexedBuffer* _buf1, IndexedBuffer* _buf2);

		/// Returns reference to the given VertexBuffer.
		VertexBuffer& getVertexBuffer(uint _key);
		/// Returns pointer to the given VertexBuffer.
		VertexBuffer* getVertexBufferPtr(uint _key);

		/// Get current index for vertex buffers.
		uint getVBIndex() const;
		/// Get current index for index buffers.
		uint getIVBIndex() const;

		VertexDeclaration* getVertexDeclaration(const wstring& _name);
		bool findVertexDeclaration(const wstring& _name);

		/// Returns number of VertexBuffer objects.
		uint getVertexBuffersNum() const;
		/// Returns number of IndexBuffer objects.
		uint getIndexedBuffersNum() const;

		/** Binds given vertex buffer.
			@remarks
				It is better way to go then manually calling VertexBuffer::bind() method,
				as vertex buffer is set only if it is different from active one.
		*/
		void bindVertexBuffer(VertexBuffer* _buffer);
		/** Binds given indexed buffer.
			@remarks
				It is better way to go then manually calling IndexedBuffer::bind() method,
				as indexed buffer is set only if it is different from active one.
		*/
		void bindIndexedBuffer(IndexedBuffer* _buffer);
		/** Binds given vertex declaration.
			@remarks
				It is better way to go then manually calling VertexDeclaration::bind() method,
				as vertex declaration is set only if it is different from active one.
		*/
		void bindVertexDeclaration(VertexDeclaration* _declare);
	};



	inline uint VertexBufferManager::getVBIndex() const
	{
		return m_nVBIndex;
	}
//----------------------------------------------------------------------
	inline uint VertexBufferManager::getIVBIndex() const
	{
		return m_nIVBIndex;
	}
//----------------------------------------------------------------------
	inline uint VertexBufferManager::getVertexBuffersNum() const
	{
		return m_VBs.size();
	}
//----------------------------------------------------------------------
	inline uint VertexBufferManager::getIndexedBuffersNum() const
	{
		return m_IVBs.size();
	}
//----------------------------------------------------------------------
	inline VertexBuffer& VertexBufferManager::getVertexBuffer(uint _key)
	{
		return *m_VBs[_key];
	}
//----------------------------------------------------------------------
	inline VertexBuffer* VertexBufferManager::getVertexBufferPtr(uint _key)
	{
		return m_VBs[_key];
	}
//----------------------------------------------------------------------
}


#endif
