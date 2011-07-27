/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeFur.h
Version:	0.04
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_NODEFUR_H_
#define __INC_NODEFUR_H_


#include "NodeVisible.h"
#include "PreethamScatter.h"
#include "Prerequisities.h"


namespace nGENE
{
namespace Nature
{
	/** Fur texture.
	*/
	class FurTexture
	{
	private:
		static uint s_TexturesCount;

		uint m_nSize;
		uint m_nNumLayers;

		vector <ITexture*> m_vTextures;

	public:
		FurTexture();
		~FurTexture();
		
		void init(uint _seed, uint _size, uint _num, uint _density);
		void cleanup();

		ITexture* getTexture(uint _index);
	};

	/** Node to be used with shells and fins texturing method.
		@remarks
			Note that this technique should not be used with long hair
			as the results would be rather not convincing.
	*/
	class NodeFur: public NodeVisible
	{
	private:
		/// Set of 2D slices for rendering fur.
		FurTexture m_Texture;

		uint m_nLayersNum;						///< Number of layers
		uint m_nTextureSize;					///< Dimension of the texture
		uint m_nDensity;						///< Fur density
		uint m_nSeed;							///< Seed for generator

		/** Length of the fur. The longer the fur, the more
			layers will be required to present fur in a convincing
			manner.
		*/
		Real m_fFurLength;

		Material* m_pFurMat;					///< Pointer to the fur material

	public:
		/** NodeFur constructor.
			@param
				_layersNum number of shells. The greater the value, the better
				quality of the result image is. However, it comes at performance
				drop.
			@param
				_textureSize fur texture size.
			@param
				_density density of the fur texture.
		*/
		NodeFur(uint _layersNum=20, uint _textureSize=128, uint _density=6000, uint _seed=383832);
		~NodeFur();

		void init();

		void addSurface(const wstring& _name, Surface& _surf);

		/// Sets fur length.
		void setFurLength(Real _length);
		/// Returns fur length.
		Real getFurLength() const;

		/** Sets number of layers.
			@remarks
				Causes NodeFur object recreation.
		*/
		void setLayersNum(uint _layers);
		uint getLayersNum() const;

		/** Sets fur texture size.
			@remarks
				Causes NodeFur object recreation.
		*/
		void setTextureSize(uint _size);
		/// Returns fur texture size.
		uint getTextureSize() const;

		/** Sets fur density.
			@remarks
				Causes NodeFur object recreation.
		*/
		void setFurDensity(uint _density);
		/// Returns fur density.
		uint getFurDensity() const;


		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};



	inline Real NodeFur::getFurLength() const
	{
		return m_fFurLength;
	}
//----------------------------------------------------------------------
	inline uint NodeFur::getLayersNum() const
	{
		return m_nLayersNum;
	}
//----------------------------------------------------------------------
	inline uint NodeFur::getFurDensity() const
	{
		return m_nDensity;
	}
//----------------------------------------------------------------------
	inline uint NodeFur::getTextureSize() const
	{
		return m_nTextureSize;
	}
//----------------------------------------------------------------------
}
}

#endif