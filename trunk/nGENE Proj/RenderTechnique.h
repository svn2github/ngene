/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		RenderTechnique.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_RENDERTECHNIQUE_H_
#define __INC_RENDERTECHNIQUE_H_


#include "HashTable.h"
#include "Prerequisities.h"
#include "TextureSampler.h"
#include "RenderPass.h"
#include "TypeInfo.h"
#include "XMLNode.h"


namespace nGENE
{
	/** Render technique.
		@par
			A technique is a collection of passes.
	*/
	class nGENEDLL RenderTechnique: public BaseClass, public XMLNode
	{
		EXPOSE_TYPE
	private:
		wstring m_stName;					///< Name of the technique - has to be unique
		Material* m_pMaterial;				///< Pointer to the Material object

		vector <RenderPass> m_vPasses;		///< Vector of RenderPass objects

	public:
		RenderTechnique();
		~RenderTechnique();


		void init();
		void cleanup();

		void parse(XMLDocument* _node);
		void load() {}
		void save(File* _file);

		/// Binds RenderTechnique to the rendering pipeline.
		void bind();
		/// Unbinds RenderTechnique.
		void unbind();

		/// Updates RenderTechnique.
		void update();

		/// Sets name of the technique.
		void setTechniqueName(const wstring& _name);
		/// Returns name of the technique.
		const wstring& getName() const;

		/// Adds new render pass to the material.
		void addRenderPass(RenderPass& _pass);
		/** Performs rendering of the material.
			@remarks
				Rendering of the technique is built of binding,
				rendering and finally unbinding its render
				passes.
		*/
		void render(RenderQueue* _queue);
		/// Returns render pass with the given index.
		RenderPass& getRenderPass(uint _index);

		/// Returns number of render passes.
		uint getPassesNum() const;

		/// Sets material for this technique.
		void setMaterial(Material* _material);
		/// Returns material for this technique.
		Material* getMaterial() const;
	};



	inline const wstring& RenderTechnique::getName() const
	{
		return m_stName;
	}
//----------------------------------------------------------------------
	inline RenderPass& RenderTechnique::getRenderPass(uint _index)
	{
		return m_vPasses[_index];
	}
//----------------------------------------------------------------------
	inline uint RenderTechnique::getPassesNum() const
	{
		return m_vPasses.size();
	}
//----------------------------------------------------------------------
	inline void RenderTechnique::setMaterial(Material* _material)
	{
		m_pMaterial = _material;
	}
//----------------------------------------------------------------------
	inline Material* RenderTechnique::getMaterial() const
	{
		return m_pMaterial;
	}
//----------------------------------------------------------------------
	inline void RenderTechnique::setTechniqueName(const wstring& _name)
	{
		m_stName = _name;
	}
//----------------------------------------------------------------------
}


#endif