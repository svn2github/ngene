/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TextureSampler.h
Version:	0.09
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_TEXTURESAMPLER_H_
#define __INC_TEXTURESAMPLER_H_


#include "Prerequisities.h"
#include "RendererTypes.h"
#include "TypeInfo.h"
#include "XMLNode.h"


namespace nGENE
{
	/** Texture sampler.
		@remarks
			TextureSampler is object which links together
			texture, texture filtering options and texture
			unit.
	*/
	class nGENEDLL TextureSampler: public XMLNode
	{
		EXPOSE_TYPE
	private:
		/// Pointer to the ITexture object.
		ITexture* m_pTexture;

		/// Name of the texture being used by this sampler.
		wstring m_stTextureName;

		/// Texture unit sampler is assigned to.
		uint m_nTextureUnit;

		/// Texture filtering option
		TEXTURE_FILTER m_FilterMin;
		TEXTURE_FILTER m_FilterMag;
		TEXTURE_FILTER m_FilterMip;

		/// Texture addressing mode
		TEXTURE_ADDRESSING_MODE m_AddressMode;

		dword m_dwClearFlags;				///< Specifies whether render target should be cleared

		bool m_bForceSet;
		bool m_bIsEnabled;					/**< Specifies whether sampler is enabled. If not, it
												 will not be bound. */


		string saveFiltering(TEXTURE_FILTER _filter);
		string saveAddressing(TEXTURE_ADDRESSING_MODE _mode);

		TEXTURE_FILTER parseFiltering(const string& _data);
		TEXTURE_ADDRESSING_MODE parseAddressing(const string& _data);

	public:
		TextureSampler();
		~TextureSampler();

		void parse(XMLDocument* _node);
		void load() {}
		void save(File* _file);

		/// Set texture sampler number.
		void setTextureUnit(uint _unit);
		/// Returns texture sampler number.
		uint getTextureUnit() const;

		/// Binds texture sampler.
		void bind();
		void unbind();

		/// Returns pointer to the underlying texture object.
		ITexture* getTexture() const;
		/// Returns name of the texture used by this sampler.
		wstring& getTextureName();
		/// Sets texture for this sampler.
		void setTexture(ITexture* _texture);
		/// Sets texture for this sampler using existing texture.
		void setTexture(const wstring& _name);

		/// Sets addressing mode.
		void setAddressingMode(TEXTURE_ADDRESSING_MODE _mode);

		void setMinFilter(TEXTURE_FILTER _filter);
		void setMagFilter(TEXTURE_FILTER _filter);
		void setMipFilter(TEXTURE_FILTER _filter);

		bool isForceSet() const;

		/// Specifies whether texture sampler is enabled.
		void setEnabled(bool _value);
		/// Checks if texture sampler is enabled.
		bool isEnabled() const;

		/// Allows you to set clear flags.
		void setClearFlags(dword _flags);
		/// Sets specified flag.
		void setClearFlag(CLEAR_BUFFER_TYPE _flag);
		/// Checks if specified flag has been set.
		bool hasClearFlag(CLEAR_BUFFER_TYPE _flag);
		/// Clears specified flag if set.
		void clearClearFlag(CLEAR_BUFFER_TYPE _flag);
		/// Returns clear flags.
		dword getClearFlags() const;
	};



	inline void TextureSampler::setTextureUnit(uint _unit)
	{
		m_nTextureUnit = _unit;
	}
//---------------------------------------------------------------------
	inline uint TextureSampler::getTextureUnit() const
	{
		return m_nTextureUnit;
	}
//---------------------------------------------------------------------
	inline ITexture* TextureSampler::getTexture() const
	{
		return m_pTexture;
	}
//---------------------------------------------------------------------
	inline void TextureSampler::setTexture(ITexture* _texture)
	{
		m_pTexture = _texture;
	}
//---------------------------------------------------------------------
	inline void TextureSampler::setAddressingMode(TEXTURE_ADDRESSING_MODE _mode)
	{
		m_AddressMode = _mode;
	}
//---------------------------------------------------------------------
	inline void TextureSampler::setMinFilter(TEXTURE_FILTER _filter)
	{
		m_FilterMin = _filter;
	}
//---------------------------------------------------------------------
	inline void TextureSampler::setMagFilter(TEXTURE_FILTER _filter)
	{
		m_FilterMag = _filter;
	}
//---------------------------------------------------------------------
	inline void TextureSampler::setMipFilter(TEXTURE_FILTER _filter)
	{
		m_FilterMip = _filter;
	}
//---------------------------------------------------------------------
	inline bool TextureSampler::isForceSet() const
	{
		return m_bForceSet;
	}
//---------------------------------------------------------------------
	inline void TextureSampler::setEnabled(bool _value)
	{
		m_bIsEnabled = _value;
	}
//---------------------------------------------------------------------
	inline bool TextureSampler::isEnabled() const
	{
		return m_bIsEnabled;
	}
//---------------------------------------------------------------------
	inline void TextureSampler::setClearFlags(dword _flags)
	{
		m_dwClearFlags = _flags;
	}
//---------------------------------------------------------------------
	inline dword TextureSampler::getClearFlags() const
	{
		return m_dwClearFlags;
	}
//---------------------------------------------------------------------
	inline bool TextureSampler::hasClearFlag(CLEAR_BUFFER_TYPE _flag)
	{
		return ((_flag & m_dwClearFlags) ? true : false);
	}
//---------------------------------------------------------------------
	inline void TextureSampler::clearClearFlag(CLEAR_BUFFER_TYPE _flag)
	{
		m_dwClearFlags &= ~CB_DEPTH;
	}
//---------------------------------------------------------------------
	inline void TextureSampler::setClearFlag(CLEAR_BUFFER_TYPE _flag)
	{
		m_dwClearFlags |= _flag;
	}
//---------------------------------------------------------------------
	inline wstring& TextureSampler::getTextureName()
	{
		return m_stTextureName;
	}
//---------------------------------------------------------------------
}


#endif