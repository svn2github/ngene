/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		RendererEnumerator.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_RENDERERENUMERATOR_H_
#define __INC_RENDERERENUMERATOR_H_


#include "HashTable.h"
#include "Prerequisities.h"
#include "Singleton.h"


namespace nGENE
{
	/** Renderer Enumerator class.
		@remarks
			Enumerators lets you easily extend nGENE Tech by just adding
			factories of new type to the enumerator. Then by specifying
			the type name you can create objects of any available type
			in the enumerator.
	*/
	class nGENEDLL RendererEnumerator: public Singleton <RendererEnumerator>
	{
	private:
		/// Hash table containing all factories
		HashTable <wstring, RendererFactory*> m_pFactories;

		// Default factories:

		/// DirectX 9.0 factory
		RendererDX9Factory* m_pDX9;
		/// DirectX 10.0 factory
		RendererDX10Factory* m_pDX10;

	public:
		RendererEnumerator();
		virtual ~RendererEnumerator();

		void init();
		void cleanup();

		/// Registers new factory.
		void addFactory(RendererFactory& _factory, const wstring& _typeName);
		/// Registers new factory.
		void addFactory(RendererFactory* _factory, const wstring& _typeName);

		/// Removes factory.
		void removeFactory(const wstring& _typeName);

		/// Creates new renderer.
		Renderer* create(const wstring& _typeName, bool _windowed,
			uint _renderWndsNum=1, bool _multiThreading=false);
		/// Destroys existing renderer.
		void destroy(Renderer* _renderer);

		/// Returns names of the renderers.
		vector <wstring> getNamesList();
	};
}


#endif