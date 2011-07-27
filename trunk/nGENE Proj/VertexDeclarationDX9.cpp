/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		VertexDeclarationDX9.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "DeviceRender.h"
#include "DX9Mapping.h"
#include "VertexDeclarationDX9.h"


namespace nGENE
{
	VertexDeclarationDX9::VertexDeclarationDX9():
		m_pVertexDecl(NULL)
	{
	}
//----------------------------------------------------------------------
	VertexDeclarationDX9::~VertexDeclarationDX9()
	{
		NGENE_RELEASE(m_pVertexDecl);
	}
//----------------------------------------------------------------------
	void VertexDeclarationDX9::addVertexElement(const VERTEX_ELEMENT& _element)
	{
		VertexDeclaration::addVertexElement(_element);
		init();
	}
//----------------------------------------------------------------------
	void VertexDeclarationDX9::addVertexElement(SVertexElement* _array, uint _count)
	{
		VertexDeclaration::addVertexElement(_array, _count);
		init();
	}
//----------------------------------------------------------------------
	void VertexDeclarationDX9::init()
	{
		if(m_pVertexDecl)
			NGENE_RELEASE(m_pVertexDecl);

		IDirect3DDevice9* pDevice = reinterpret_cast<IDirect3DDevice9*>
			(Renderer::getSingleton().getDeviceRender().getDevicePtr());

		dword dwElementsNum = m_Elements.size();
		D3DVERTEXELEMENT9* pElements = new D3DVERTEXELEMENT9[dwElementsNum + 1];
		for(uint i = 0; i < dwElementsNum; ++i)
			pElements[i] = DX9Mapping::svertexElementToD3DVERTEXELEMENT9(m_Elements[i]);

		// Add dummy element at the end
		addDummyElement(&pElements[dwElementsNum]);

		// Create vertex declaration
		pDevice->CreateVertexDeclaration(pElements, &m_pVertexDecl);


		NGENE_DELETE_ARRAY(pElements);
	}
//----------------------------------------------------------------------
	void VertexDeclarationDX9::bind()
	{
		IDirect3DDevice9* pDevice = reinterpret_cast<IDirect3DDevice9*>
			(Renderer::getSingleton().getDeviceRender().getDevicePtr());

		pDevice->SetVertexDeclaration(m_pVertexDecl);
	}
//----------------------------------------------------------------------
	void VertexDeclarationDX9::addDummyElement(D3DVERTEXELEMENT9* _element)
	{
		_element->Method= 0;
		_element->Offset= 0;
		_element->Stream= 0xFF;
		_element->Type = D3DDECLTYPE_UNUSED;
		_element->Usage = 0;
		_element->UsageIndex = 0;
	}
//----------------------------------------------------------------------
}
