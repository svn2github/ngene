/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		RenderPass.h
Version:	0.09
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_RENDERPASS_H_
#define __INC_RENDERPASS_H_


#include "HashTable.h"
#include "Prerequisities.h"
#include "TextureSampler.h"
#include "TypeInfo.h"
#include "XMLNode.h"


namespace nGENE
{
	/** Sometimes post-processing materials require some passes
		to be run during regular scene rendering. In order to accomplish
		such functionality pass can have PASS_ORDER set to PO_SCENE.
	*/
	enum DRAW_ORDER
	{
		DO_PRE,
		DO_SCENE,
		DO_POST
	};

	/** Render pass.
		@remarks
			Render pass is single rendering call rendering
			batched geometry.
	*/
	class nGENEDLL RenderPass: public BaseClass, public XMLNode
	{
		EXPOSE_TYPE
	private:
		wstring m_stName;					///< Name of the pass - has to be unique

		ShaderInstance* m_pVertexShader;	///< Vertex shader
		ShaderInstance* m_pPixelShader;		///< Pixel shader
		ShaderInstance* m_pGeometryShader;	///< Geometry shader

		/** Render Target for the pass. As nGENE supports MRT multiple
			 render targets are available per render pass */
		vector <TextureSampler> m_vRenderTargets;

		vector <TextureSampler> m_vSamplers;///< Textures used by the render pass

		DRAW_ORDER m_Order;					///< Rendering order used by pass.

		uint m_nRunCount;					///< How many times render pass should be rendered?

		bool m_bHasLighting;				///< Is light turned on?
		bool m_bIsEnabled;					///< Is rendering pass enabled


		void saveSamplers();
		void saveShaders();

	public:
		RenderPass();
		RenderPass(const RenderPass& src);
		explicit RenderPass(const wstring& _name);
		~RenderPass();

		/// Assignment operator.
		RenderPass& operator=(const RenderPass& rhs);

		void init();
		void cleanup();

		void parse(XMLDocument* _node);
		void load() {}
		void save(File* _file);

		/// Binds render pass to the rendering pipeline.
		void bind();
		/// Unbinds render pass.
		void unbind();

		/// Updates render pass.
		void update();

		/// Returns name of the pass.
		const wstring& getName() const;

		/// Returns render target associated with this render pass.
		TextureSampler& getRenderTarget(uint _index=0);
		/// Returns number of render targets used by this pass.
		uint getRenderTargetsNum() const;

		/// Adds existing sampler to the render pass.
		void addTextureSampler(const TextureSampler& _sampler);
		TextureSampler& getTextureSampler(uint _index=0);
		/// Returns number of texture samplers used by this pass.
		uint getTextureSamplersNum() const;

		DRAW_ORDER getRenderingOrder() const;

		ShaderInstance* getVertexShader() const;
		ShaderInstance* getGeometryShader() const;
		ShaderInstance* getPixelShader() const;

		/** Sets render count.
			@param
				_count this number tells how many times the rendering
				of this render pass should be performed. As an example
				think of shells and fins texturing technique where each
				shell (being literally geometry) has to be rendered
				several times to achieve desired effect i.e. nicely looking
				fur.
		*/
		void setRenderCount(uint _count);
		uint getRenderCount() const;

		/** Specifies whether pass is enabled or not.
			@remarks
				Only enabled passes are rendered.
		*/
		void setEnabled(bool _value);
		bool isEnabled() const;
	};



	inline const wstring& RenderPass::getName() const
	{
		return m_stName;
	}
//----------------------------------------------------------------------
	inline DRAW_ORDER RenderPass::getRenderingOrder() const
	{
		return m_Order;
	}
//----------------------------------------------------------------------
	inline ShaderInstance* RenderPass::getVertexShader() const
	{
		return m_pVertexShader;
	}
//----------------------------------------------------------------------
	inline ShaderInstance* RenderPass::getGeometryShader() const
	{
		return m_pGeometryShader;
	}
//----------------------------------------------------------------------
	inline ShaderInstance* RenderPass::getPixelShader() const
	{
		return m_pPixelShader;
	}
//----------------------------------------------------------------------
	inline void RenderPass::setRenderCount(uint _count)
	{
		m_nRunCount = _count;
	}
//----------------------------------------------------------------------
	inline uint RenderPass::getRenderCount() const
	{
		return m_nRunCount;
	}
//----------------------------------------------------------------------
	inline void RenderPass::setEnabled(bool _value)
	{
		m_bIsEnabled = _value;
	}
//----------------------------------------------------------------------
	inline bool RenderPass::isEnabled() const
	{
		return m_bIsEnabled;
	}
//----------------------------------------------------------------------
}


#endif