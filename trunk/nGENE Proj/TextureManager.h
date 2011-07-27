/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		TextureManager.h
Version:	0.09
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_TEXTUREMANAGER_H_
#define __INC_TEXTUREMANAGER_H_


#include "HashTable.h"
#include "Prerequisities.h"
#include "Singleton.h"
#include "TextureTypes.h"


namespace nGENE
{
	/** Lets you load and unload texture files, create blank ones,
		set them as render targets etc.
	*/
	class nGENEDLL TextureManager: public Singleton <TextureManager>
	{
	private:
		/// HashTable holding all textures.
		HashTable <wstring, ITexture*> m_pTextures;
		/// HashTable holding all not yet loaded textures.
		HashTable <wstring, XMLDocument*> m_pWaitingTextures;


		/// Parses texture object.
		ITexture* parseTexture(XMLDocument* _node);
		
		/// Parses texture format.
		TEXTURE_FORMAT parseFormat(const string& _data);

		/// Called when not loaded texture is accessed.
		void textureAccessed(const wstring& _name);

	public:
		TextureManager();
		virtual ~TextureManager();

		void cleanup();

		/// Creates new empty texture.
		ITexture* createTexture(const wstring& _name, uint _width=512, uint _height=512,
			TEXTURE_TYPE _type=TT_2D, TEXTURE_USAGE _usage=TU_NORMAL, TEXTURE_FORMAT _format=TF_A8R8G8B8);
		/// Creates new empty texture.
		ITexture* createTexture(const wstring& _name, const TEXTURE_DESC& _desc);

		/// Loads texture from file.
		ITexture* createTextureFromFile(const wstring& _name, const wstring& _fileName, TEXTURE_TYPE _type=TT_2D,
			TEXTURE_USAGE _usage=TU_NORMAL, TEXTURE_FORMAT _format=TF_DEFAULT);
		/// Loads texture from file.
		ITexture* createTextureFromFile(const wstring& _name, const TEXTURE_DESC& _desc);
		
		/// Creates texture from XML node.
		ITexture* createTextureFromXML(XMLDocument* _node, bool _onDemand=true);

		/** Binds texture to the pipeline.
			@remarks
				The result of bind function is strongly dependent on texture's
				TEXTURE_USAGE, eg. iff it is TU_RENDERTARGET new render
				texture will be set for rendering.
		*/
		void bind(const wstring& _name);

		/// Returns existing texture.
		ITexture* getTexture(const wstring& _name);
		/** Sets texture to a given one.
			@par
				This way you can replace existing texture with a different one.
		*/
		void setTexture(const wstring& _name, ITexture* _texture);

		/** Checks if specified texture already exists.
			@returns
				bool if the texture is found the function returns true and
				false otherwise.
		*/
		bool findTexture(const wstring& _name);

		/// Removes given texture.
		void removeTexture(const wstring& _name);
		/// Removes given texture.
		void removeTexture(ITexture* _texture);


		/// Saves texture object.
		XMLNode* saveTexture(ITexture* _texture, const string& _name);
		string saveFormat(TEXTURE_FORMAT _format);
	};
}


#endif