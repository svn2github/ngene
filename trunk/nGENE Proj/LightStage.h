/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		LightStage.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_LIGHTSTAGE_H_
#define __INC_LIGHTSTAGE_H_


#include "Prerequisities.h"
#include "RenderStage.h"


namespace nGENE
{
	/** Light rendering stage.
	*/
	class nGENEDLL LightStage: public RenderStage
	{
	private:
		/// Quad for rendering post-process effects
		ScreenOverlay* m_pPostProcessQuad;

		/// Texture used for High Definition Rendering.
		ITexture* m_pHDR;

		/// Specifies whether HDR rendering should be used.
		bool m_bUseHDR;

	public:
		LightStage();
		virtual ~LightStage();

		void init();
		void cleanup();

		/** This method is used for geometry stage of deferred shading.
			@remarks
				The standard material plays role of diffuse buffer, but
				several other rendering passes are applied (eg. normals,
				specular) to enhance visual quality of the final image.
		*/
		virtual void render();

		/// Specifies whether HDR rendering should be used.
		void setUseHDR(bool _value);
		/// Toggles HDR rendering.
		void toggleHDR();
		/// Checks if HDR is enabled.
		bool isUsingHDR() const;
	};



	inline void LightStage::setUseHDR(bool _value)
	{
		m_bUseHDR = _value;
	}
//----------------------------------------------------------------------
	inline void LightStage::toggleHDR()
	{
		m_bUseHDR = m_bUseHDR ? false : true;
	}
//----------------------------------------------------------------------
	inline bool LightStage::isUsingHDR() const
	{
		return m_bUseHDR;
	}
//----------------------------------------------------------------------
}


#endif