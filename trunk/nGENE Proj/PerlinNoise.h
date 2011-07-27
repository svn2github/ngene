/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		PerlinNoise.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PERLINNOISE_H_
#define __INC_PERLINNOISE_H_


#include "Prerequisities.h"


namespace nGENE
{
	/** This class defines n-dimensional Perlin noise.
		@remarks
			To achieve "multi-dimensolity" of the noise template
			recusion is used extensively.
	*/
	template <uint N=1>
	class PerlinNoise
	{
	protected:
		PerlinNoise <N-1>* m_pElements;
		uint m_nSize;

		void copyFrom(const PerlinNoise <N>& src);

	public:
		PerlinNoise(uint _size=32);
		PerlinNoise(const PerlinNoise <N>& src);
		~PerlinNoise();

		PerlinNoise <N>& operator=(const PerlinNoise <N>& rhs);

		/// Setups perlin noise.
		void setup();
		double smooth(vector <uint>& _indices);
		Real& get(vector <uint>& _indices);

		void resize(uint _size);
		uint getSize() const;

		PerlinNoise <N-1>& operator[](int _index);
		const PerlinNoise <N-1>& operator[](int _index) const;
	};

	template <>
	class PerlinNoise <1>
	{
	protected:
		Real* m_pElements;
		uint m_nSize;

		void copyFrom(const PerlinNoise <1>& src);

	public:
		PerlinNoise(uint _size=32);
		PerlinNoise(const PerlinNoise <1>& src);
		~PerlinNoise();

		PerlinNoise <1>& operator=(const PerlinNoise <1>& rhs);

		void setup();
		double smooth(vector <uint>& _indices);

		void resize(uint _size);
		uint getSize() const;

		Real& get(vector <uint>& _indices);

		Real& operator[](int _index);
		const Real& operator[](int _index) const;
	};



	template <uint N>
	PerlinNoise <N>::PerlinNoise(uint _size):
		m_nSize(_size)
	{
		m_pElements = new PerlinNoise <N-1>[m_nSize];
		for(uint i = 0; i < m_nSize; ++i)
			m_pElements[i].resize(_size);
	}
//----------------------------------------------------------------------
	template <uint N>
	PerlinNoise <N>::PerlinNoise(const PerlinNoise <N>& src)
	{
		copyFrom(src);
	}
//----------------------------------------------------------------------
	template <uint N>
	PerlinNoise <N>::~PerlinNoise()
	{
		NGENE_DELETE_ARRAY(m_pElements);
	}
//----------------------------------------------------------------------
	template <uint N>
	PerlinNoise <N>& PerlinNoise <N>::operator=(const PerlinNoise <N>& rhs)
	{
		if(this == &rhs)
			return (*this);

		delete[] m_pElements;
		copyFrom(rhs);

		return (*this);
	}
//----------------------------------------------------------------------
	template <uint N>
	void PerlinNoise <N>::copyFrom(const PerlinNoise <N>& src)
	{
		m_nSize = src.m_nSize;
		m_pElements = new PerlinNoise <N-1>[m_nSize];
		for(uint i = 0; i < m_nSize; ++i)
			m_pElements[i] = src.m_pElements[i];
	}
//----------------------------------------------------------------------
	template <uint N>
	void PerlinNoise <N>::resize(uint _size)
	{
		PerlinNoise <N-1>* newElements = new PerlinNoise <N-1>[_size];
		for(uint i = 0; i < _size && i < m_nSize; ++i)
		{
			newElements[i] = m_pElements[i];
			newElements[i].resize(_size);
		}

		m_nSize = _size;
		delete[] m_pElements;
		m_pElements = newElements;
	}
//----------------------------------------------------------------------
	template <uint N>
	uint PerlinNoise <N>::getSize() const
	{
		return m_nSize;
	}
//----------------------------------------------------------------------
	template <uint N>
	void PerlinNoise <N>::setup()
	{
		for(uint i = 0; i < m_nSize; ++i)
			m_pElements[i].setup();
	}
//----------------------------------------------------------------------
	template <uint N>
	double PerlinNoise <N>::smooth(vector <uint>& _indices)
	{
		uint index = _indices.front();
		_indices.erase(_indices.begin());

		vector <uint> inds1 = _indices;
		vector <uint> inds2 = _indices;
		vector <uint> inds3 = _indices;

		double result = (m_pElements[index + 0].smooth(inds1) +
						 m_pElements[index + 1].smooth(inds2) +
						 m_pElements[index - 1].smooth(inds3)) / 3.0f;

		return result;
	}
//----------------------------------------------------------------------
	template <uint N>
	Real& PerlinNoise <N>::get(vector <uint>& _indices)
	{
		uint index = _indices.front();
		_indices.erase(_indices.begin());

		Real& result = m_pElements[index].get(_indices);

		return result;
	}
//----------------------------------------------------------------------
	template <uint N>
	PerlinNoise <N-1>& PerlinNoise <N>::operator[](int _index)
	{
		return m_pElements[_index];
	}
//----------------------------------------------------------------------
	template <uint N>
	const PerlinNoise <N-1>& PerlinNoise <N>::operator[](int _index) const
	{
		return m_pElements[_index];
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	inline PerlinNoise <1>::PerlinNoise(uint _size):
		m_nSize(_size)
	{
		m_pElements = new Real[m_nSize];
	}
//----------------------------------------------------------------------
	inline PerlinNoise <1>::PerlinNoise(const PerlinNoise <1>& src)
	{
		copyFrom(src);
	}
//----------------------------------------------------------------------
	inline PerlinNoise <1>::~PerlinNoise()
	{
		NGENE_DELETE_ARRAY(m_pElements);
	}
//----------------------------------------------------------------------
	inline PerlinNoise <1>& PerlinNoise <1>::operator=(const PerlinNoise <1>& rhs)
	{
		if(this == &rhs)
			return (*this);

		delete[] m_pElements;
		copyFrom(rhs);

		return (*this);
	}
//----------------------------------------------------------------------
	inline void PerlinNoise <1>::copyFrom(const PerlinNoise <1>& src)
	{
		m_nSize = src.m_nSize;
		m_pElements = new Real[m_nSize];
		for(uint i = 0; i < m_nSize; ++i)
			m_pElements[i] = src.m_pElements[i];
	}
//----------------------------------------------------------------------
	inline void PerlinNoise <1>::resize(uint _size)
	{
		Real* newElements = new Real[_size];
		for(uint i = 0; i < _size && i < m_nSize; ++i)
			newElements[i] = m_pElements[i];
		
		m_nSize = _size;
		delete m_pElements;
		m_pElements = newElements;
	}
//----------------------------------------------------------------------
	inline uint PerlinNoise <1>::getSize() const
	{
		return m_nSize;
	}
//----------------------------------------------------------------------
	inline void PerlinNoise <1>::setup()
	{
		for(uint i = 0; i < m_nSize; ++i)
		{
			m_pElements[i] = Maths::perlinNoiseGenerator(i, 0, rand());
		}
	}
//----------------------------------------------------------------------
	inline double PerlinNoise <1>::smooth(vector <uint>& _indices)
	{
		uint index = _indices[0];
		_indices.erase(_indices.begin());

		double result = 0.0f;
		result = (m_pElements[index + 0] +
				  m_pElements[index + 1] +
				  m_pElements[index - 1]) / 3.0f;

		return result;
	}
//----------------------------------------------------------------------
	inline Real& PerlinNoise <1>::get(vector <uint>& _indices)
	{
		uint index = _indices.front();
		_indices.erase(_indices.begin());

		Real& result = m_pElements[index];

		return result;
	}
//----------------------------------------------------------------------
	inline Real& PerlinNoise <1>::operator[](int _index)
	{
		return m_pElements[_index];
	}
//----------------------------------------------------------------------
	inline const Real& PerlinNoise <1>::operator[](int _index) const
	{
		return m_pElements[_index];
	}
//----------------------------------------------------------------------
}


#endif