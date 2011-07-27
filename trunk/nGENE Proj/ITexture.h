/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ITexture.h
Version:	0.10
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_ITEXTURE_H_
#define __INC_ITEXTURE_H_


#include "BaseClass.h"
#include "TextureTypes.h"
#include "TypeInfo.h"


namespace nGENE
{
	/** Texture pure interface.
	*/
	class nGENEDLL ITexture: public BaseClass
	{
		EXPOSE_TYPE
	protected:
		/** Description of the texture.
			@par
				It is essential that this structure is properly
				filled by subclasses.
			@par
				Also important thing to note is _level parameter which
				occurs in many method declarations. It stands for level
				of the texture.
		*/
		TEXTURE_DESC m_TextureDesc;

		float m_fRatioWidth;				///< Width scaling factor
		float m_fRatioHeight;				///< Height scaling factor

	public:
		ITexture();
		virtual ~ITexture();

		virtual void init()=0;
		virtual void cleanup()=0;

		/** Creates empty texture.
			@remarks
				This method creates new resource, being empty at the beginning. This
				method is especially useful for creating render targets.
			@param
				_width width of the texture.
			@param
				_height height of the texture.
			@param
				_depth this parameter makes sense only with TT_3D texture type as it
				specifies 3rd dimension of the texture. Must be non-zero.
		*/
		virtual void create(uint _width=512, uint _height=512, TEXTURE_TYPE _type=TT_2D,
			TEXTURE_USAGE _usage=TU_NORMAL, TEXTURE_FORMAT _format=TF_A8R8G8B8, uint _depth=1)=0;

		/// Creates texture from file.
		virtual void createFromFile(const wstring& _fileName, TEXTURE_TYPE _type=TT_2D,
			TEXTURE_USAGE _usage=TU_NORMAL, TEXTURE_FORMAT _format=TF_DEFAULT)=0;

		/** Activates texture.
			@param
				_textureUnit index of texture unit, this texture should be bound to.
			@param
				_forceSet in case of render targets textures by default they are
				set to render into them. However as you finish this process
				you will sometimes need to put this texture onto some object.
				To do this set this parameter to true.
			@param
				_clearTarget in case of render targets textures specifies whether it
				should be cleared or not. You can specify BUFFER_DEPTH and CB_TARGET flags.
		*/
		virtual void bind(uint _textureUnit=0, bool _forceSet=false,
			dword _clearFlag=CB_TARGET)=0;
		/// Deactivates texture.
		virtual void unbind()=0;

		/// Sets image data of the texture.
		virtual void setData(void* _data, uint _level=0) const=0;
		virtual void setDataFloat(void* _data, uint size, uint _level=0) const=0;

		/** Returns image data of the texture.
			@remarks
				This function allocates buffer for the data. Client
				function has to free memory on its own.
		*/
		virtual void* getData(uint _level=0) const=0;

		/// Returns texture description.
		virtual TEXTURE_DESC& getTextureDesc();

		/// Sets ratio for width and height.
		void setRatio(Real _x, Real _y);

		/// Returns texture bit depth.
		virtual uint getBitDepth(uint _level=0) const=0;

		/// Clears render target.
		virtual void clearRenderTarget()=0;
	};



	inline TEXTURE_DESC& ITexture::getTextureDesc()
	{
		return m_TextureDesc;
	}
//----------------------------------------------------------------------
	inline void ITexture::setRatio(Real _x, Real _y)
	{
		m_fRatioWidth = _x;
		m_fRatioHeight = _y;
	}
//----------------------------------------------------------------------
}


#endif
