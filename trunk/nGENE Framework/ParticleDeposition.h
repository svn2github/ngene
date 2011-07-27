/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ParticleDeposition.h
Version:	0.02
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_PARTICLEDEPOSITION_H_
#define __INC_PARTICLEDEPOSITION_H_


#include "Prerequisities.h"
#include "BrownianTree.h"
#include "Vector2.h"


namespace nGENE
{
namespace Nature
{
	/** Default positioning policy for particle deposition.
		@par
			It is very simple policy - once point is set it
			will stay the same until it is manually changed.
	*/
	class ParticlePositionPolicyConstant
	{
	private:
		Vector2 m_vecPosition;
		Vector2 m_vecLimit;

	public:
		/// Sets position of the element.
		void setPosition(Vector2& _position)
		{
			if(_position.x >= 0 &&
			   _position.x < m_vecLimit.x &&
			   _position.y >= 0 &&
			   _position.y < m_vecLimit.y)
			{
				m_vecPosition = _position;
			}
		}

		/// Sets limits for position.
		void setLimit(Vector2& _limit)
		{
			m_vecLimit = _limit;
		}

		/// Returns next position.
		Vector2 getNextPosition()
		{
			return m_vecPosition;
		}
	};

	/** Volcanic area.
		@par
			It is very simple policy - several points are chosen.
	*/
	class ParticlePositionPolicyPoints
	{
	private:
		Vector2 m_vecLimit;

		vector <Vector2> m_StartPoints;

	public:
		/// Adds new starting point.
		void setPosition(Vector2& _position)
		{
			m_StartPoints.push_back(_position);
		}

		/// Adds new starting point.
		void addPosition(Vector2& _position)
		{
			m_StartPoints.push_back(_position);
		}

		/// Sets limits for position.
		void setLimit(Vector2& _limit)
		{
			m_vecLimit = _limit;
		}

		/// Returns next position.
		Vector2 getNextPosition()
		{
			uint index = rand() % m_StartPoints.size() + 1;

			return m_StartPoints[index];
		}
	};

	/** Positioning particle for mountain ridge generation.
	*/
	class ParticlePositionPolicyMountain
	{
	private:
		Vector2 m_vecPosition;			///< Position of the central vent
		Vector2 m_vecLimit;				///< Position limit

		uint m_nCurrentSeed;			///< Current seed
		uint m_nTreeIterations;			///< Number of iterations for Brownian tree generation

		/// All seeds
		vector <BrownianTree::TREE_SEED> m_vSeeds;

	public:
		ParticlePositionPolicyMountain():
			m_nCurrentSeed(0),
			m_nTreeIterations(6000)
		{
		}

		/// Sets limits for position.
		void setLimit(Vector2& _limit)
		{
			m_vecLimit = _limit;
		}

		/// Sets number of tree walkers
		void setWalkersNum(uint _value)
		{
			m_nTreeIterations = _value;
		}

		/// Sets position of the element.
		void setPosition(Vector2& _position)
		{
			if(_position.x >= 0 &&
			   _position.x < m_vecLimit.x &&
			   _position.y >= 0 &&
			   _position.y < m_vecLimit.y)
			{
				m_vecPosition = _position;

				BrownianTree tree;
				BrownianTree::TREE_SEED seed = make_pair((uint)_position.x, (uint)_position.y);
				vector <BrownianTree::TREE_SEED> seeds;
				seeds.push_back(seed);
				uint width = static_cast<uint>(m_vecLimit.x);
				uint height = static_cast<uint>(m_vecLimit.y);
				bool* pTmp = tree(seeds, width, height, m_nTreeIterations);

				// Add all seeds
				for(uint i = 0; i < height; ++i)
				{
					for(uint j = 0; j < width; ++j)
					{
						if(pTmp[i * width + j])
						{
							BrownianTree::TREE_SEED seed(j, i);
							m_vSeeds.push_back(seed);
						}
					}
				}

				NGENE_DELETE_ARRAY(pTmp);
			}
		}

		/// Returns next position.
		Vector2 getNextPosition()
		{
			Vector2 pos(m_vSeeds[m_nCurrentSeed].first,
						m_vSeeds[m_nCurrentSeed].second);

			++m_nCurrentSeed %= m_vSeeds.size();

			return pos;
		}
	};

	/** Positioning particle for volcanoes generation.
		@par
			This policy tries to reproduce volcano creation. It does
			so by simulating lava streams flow from the crater downwards.
	*/
	class ParticlePositionPolicyVolcano
	{
	private:
		Vector2 m_vecPosition;			///< Position of the central vent
		Vector2 m_vecLimit;				///< Position limit
		Vector2 m_vecCurrentPosition;	///< Current drop position;

		uint m_nStreamsCount;			///< Number of lava streams
		uint m_nMaximumLength;			///< Maximum lava stream length
		uint m_nCurrentLength;			///< Current stream length
		uint m_nCurrentStream;			///< Current stream index

		Vector2 m_vecCurrentDir;		///< Current stream direction

		vector <uint> m_vLengths;		///< Lengths of all lava streams
		vector <Vector2> m_vDirections;	///< Directions of all lava streams

	public:
		ParticlePositionPolicyVolcano():
			m_nStreamsCount(1),
			m_nMaximumLength(10),
			m_nCurrentStream(0),
			m_nCurrentLength(0)
		{
		}

		/// Sets limits for position.
		void setLimit(Vector2& _limit)
		{
			m_vecLimit = _limit;
		}

		/// Sets position of the element.
		void setPosition(Vector2& _position)
		{
			if(_position.x >= 0 &&
			   _position.x < m_vecLimit.x &&
			   _position.y >= 0 &&
			   _position.y < m_vecLimit.y)
			{
				m_vecPosition = _position;
				m_vecCurrentPosition = m_vecPosition;
			}
		}

		/// Sets maximum lava stream length.
		void setMaximumStreamLength(uint _length)
		{
			m_nMaximumLength = _length;
		}

		/// Sets number of lava streams.
		void setStreamsCount(uint _count)
		{
			m_nStreamsCount = _count;
			for(uint i = 0; i < m_nStreamsCount; ++i)
			{
				m_vLengths.push_back(20 + rand() % m_nMaximumLength);
				Vector2 dir(Maths::perlinNoiseGenerator(rand(), rand(), rand()),
							Maths::perlinNoiseGenerator(rand(), rand(), rand()));
				dir.normalize();
				m_vDirections.push_back(dir);
			}

			m_vecCurrentDir = m_vDirections[0];
		}

		/// Returns next position.
		Vector2 getNextPosition()
		{
			Vector2 pos = m_vecCurrentPosition;

			if(++m_nCurrentLength == m_vLengths[m_nCurrentStream])
			{
				m_nCurrentLength = 0;
				m_nCurrentStream = (++m_nCurrentStream) % m_nStreamsCount;
				m_vecCurrentPosition = m_vecPosition;
				m_vecCurrentDir = m_vDirections[m_nCurrentStream];
			}
			else
			{
				Vector2 tmp = m_vecCurrentPosition;
				Vector2 newDir = Vector2(Maths::perlinNoiseGenerator(rand(), rand(), rand()),
										 Maths::perlinNoiseGenerator(rand(), rand(), rand())) * 0.125f +
								 m_vecCurrentDir;

				
				newDir.normalize();
				m_vecCurrentDir = newDir;

				tmp += newDir;

				if(tmp.x >= 0 &&
				   tmp.x < m_vecLimit.x &&
				   tmp.y >= 0 &&
				   tmp.y < m_vecLimit.y)
				{
					m_vecCurrentPosition = tmp;
				}
				else
					m_nCurrentLength = m_vLengths[m_nCurrentStream] - 1;
			}

			return pos;
		}
	};

	/** Default deformation policy class.
		@par
			It does not change anything in terrain appearance.
	*/
	class ParticleDeformPolicyNone
	{
	public:
		byte deform(byte _value)
		{
			return _value;
		}
	};

	/** Volcano deformation policy class.
		@par
			It creates crater, caldera or whatever you call it.
	*/
	class ParticleDeformPolicyVolcano
	{
	private:
		byte m_nCraterBegin;			///< Beginning of the crater

	public:
		/// Sets height at which crater begins
		void setCraterHeight(byte _height)
		{
			m_nCraterBegin = _height;
		}

		byte deform(byte _value)
		{
			if(_value <= m_nCraterBegin)
				return _value;
			else
			{
				int diff = _value - m_nCraterBegin;
				if(diff > m_nCraterBegin)
					diff = m_nCraterBegin - 1;
				return (m_nCraterBegin - diff);
			}
		}
	};

	/** Deforming policy which 'flip' the heights.
		@par
			Useful for lakes/rivers/valleys creation.
	*/
	class ParticleDeformPolicyFlip
	{
	public:
		byte deform(byte _value)
		{
			return (255 - _value);
		}
	};


	/** Helper class used to generate height map texture.
		@par
			This class uses particle deposition method to generate
			height-map
	*/
	template <typename ParticlePositionPolicy=ParticlePositionPolicyConstant, typename ParticleDeformPolicy=ParticleDeformPolicyNone>
	class ParticleDeposition: public ParticlePositionPolicy, public ParticleDeformPolicy
	{
	private:
		bool up(float* pHeights, uint _width, uint _height, uint _seed, uint& y, uint& x, uint i, uint elevationThreshold);
		bool upLeft(float* pHeights, uint _width, uint _height, uint _seed, uint& y, uint& x, uint i, uint elevationThreshold);
		bool upRight(float* pHeights, uint _width, uint _height, uint _seed, uint& y, uint& x, uint i, uint elevationThreshold);
		bool down(float* pHeights, uint _width, uint _height, uint _seed, uint& y, uint& x, uint i, uint elevationThreshold);
		bool downLeft(float* pHeights, uint _width, uint _height, uint _seed, uint& y, uint& x, uint i, uint elevationThreshold);
		bool downRight(float* pHeights, uint _width, uint _height, uint _seed, uint& y, uint& x, uint i, uint elevationThreshold);
		bool left(float* pHeights, uint _width, uint _height, uint _seed, uint& y, uint& x, uint i, uint elevationThreshold);
		bool right(float* pHeights, uint _width, uint _height, uint _seed, uint& y, uint& x, uint i, uint elevationThreshold);

	public:
		/// Generates new height map texture.
		nGENE::byte* generate(uint _width, uint _height, uint _seed, uint iterations, uint minSearchRadius=1, uint maxSearchRadius=1,
			uint minElevation=0, uint maxElevation=0);
	};



	template <typename ParticlePositionPolicy, typename ParticleDeformPolicy>
	byte* ParticleDeposition <typename ParticlePositionPolicy, typename ParticleDeformPolicy>::generate(uint _width, uint _height, uint _seed,
			uint iterations, uint minSearchRadius, uint maxSearchRadius, uint minElevation, uint maxElevation)
	{	
		float* pHeights = new float[_width * _height];

		memset(pHeights, 0, _width * _height * sizeof(float));

		uint particlesCount = iterations;
		uint searchRadius = minSearchRadius;
		uint elevationThreshold = minElevation;
		uint radiusRandomness = maxSearchRadius - minSearchRadius;
		uint elevationRandomness = maxElevation - minElevation;
		srand(_seed);
		float max = 0.0f;
		uint t[8] = {1, 2, 3, 4, 5, 6, 7, 8};
		for(uint i = 0; i < particlesCount; ++i)
		{
			Vector2 position = getNextPosition();
			uint x = (uint)position.x;
			uint y = (uint)position.y;

			if(radiusRandomness)
				searchRadius = minSearchRadius + rand() % (radiusRandomness + 1);
			if(elevationRandomness)
				elevationThreshold = minElevation + rand() % (elevationRandomness + 1);

			Maths::permutation(t, 0, 8);


			bool run = false;
			do
			{
				if(radiusRandomness)
					searchRadius = minSearchRadius + rand() % (radiusRandomness + 1);
				if(elevationRandomness)
					elevationThreshold = minElevation + rand() % (elevationRandomness + 1);

				Maths::permutation(t, 0, 8);

				run = false;
				uint j = minSearchRadius;
				while(!run)
				{
					for(uint k = 0; k < 8; ++k)
					{
						if(t[k] == 1)
						{
							if(up(pHeights, _width, _height, _seed, y, x, j, elevationThreshold))
							{
								run = true;
								break;
							}
						}
						if(t[k] == 2)
						{
							if(down(pHeights, _width, _height, _seed, y, x, j, elevationThreshold))
							{
								run = true;
								break;
							}
						}
						if(t[k] == 3)
						{
							if(left(pHeights, _width, _height, _seed, y, x, j, elevationThreshold))
							{
								run = true;
								break;
							}
						}
						if(t[k] == 4)
						{
							if(right(pHeights, _width, _height, _seed, y, x, j, elevationThreshold))
							{
								run = true;
								break;
							}
						}
						if(t[k] == 5)
						{
							if(upLeft(pHeights, _width, _height, _seed, y, x, j, elevationThreshold))
							{
								run = true;
								break;
							}
						}
						if(t[k] == 6)
						{
							if(upRight(pHeights, _width, _height, _seed, y, x, j, elevationThreshold))
							{
								run = true;
								break;
							}
						}
						if(t[k] == 7)
						{
							if(downLeft(pHeights, _width, _height, _seed, y, x, j, elevationThreshold))
							{
								run = true;
								break;
							}
						}
						if(t[k] == 8)
						{
							if(downRight(pHeights, _width, _height, _seed, y, x, j, elevationThreshold))
							{
								run = true;
								break;
							}
						}
					}

					if(run)
						break;

					if(++j > searchRadius)
					{
						break;
						run = false;
					}
				}
			} while(run);

			
			pHeights[y * _width + x] += 1.0f;
			if(pHeights[y * _width + x] > max)
				max = pHeights[y * _width + x];
		}

		
		nGENE::byte* pData = new nGENE::byte[_width * _height];
		
		for(uint i = 0; i < _width * _height; ++i)
		{
			if(max > 255.0f)
				pData[i] = nGENE::byte((pHeights[i] / max) * 255.0f);
			else
				pData[i] = nGENE::byte(pHeights[i]);

			pData[i] = deform(pData[i]);
		}


		NGENE_DELETE(pHeights);

		return pData;
	}
//----------------------------------------------------------------------
	template <typename ParticlePositionPolicy, typename ParticleDeformPolicy>
	bool ParticleDeposition <typename ParticlePositionPolicy, typename ParticleDeformPolicy>::up(float* pHeights, uint _width, uint _height, uint _seed, uint& y, uint& x, uint i, uint elevationThreshold)
	{
		if(y >= i)
		{
			if(pHeights[y * _width + x] > pHeights[(y - i) * _width + x] + elevationThreshold)
			{
				y -= i;
				return true;
			}
		}

		return false;
	}
//----------------------------------------------------------------------
	template <typename ParticlePositionPolicy, typename ParticleDeformPolicy>
	bool ParticleDeposition <typename ParticlePositionPolicy, typename ParticleDeformPolicy>::upLeft(float* pHeights, uint _width, uint _height, uint _seed, uint& y, uint& x, uint i, uint elevationThreshold)
	{
		if(y >= i && x >= i)
		{
			if(pHeights[y * _width + x] > pHeights[(y - i) * _width + (x - i)] + elevationThreshold)
			{
				y -= i;
				x -= i;
				return true;
			}
		}

		return false;
	}
//----------------------------------------------------------------------
	template <typename ParticlePositionPolicy, typename ParticleDeformPolicy>
	bool ParticleDeposition <typename ParticlePositionPolicy, typename ParticleDeformPolicy>::upRight(float* pHeights, uint _width, uint _height, uint _seed, uint& y, uint& x, uint i, uint elevationThreshold)
	{
		if(y >= i && x < _width - 1)
		{
			if(pHeights[y * _width + x] > pHeights[(y - i) * _width + (x + i)] + elevationThreshold)
			{
				y -= i;
				x += i;
				return true;
			}
		}

		return false;
	}
//----------------------------------------------------------------------
	template <typename ParticlePositionPolicy, typename ParticleDeformPolicy>
	bool ParticleDeposition <typename ParticlePositionPolicy, typename ParticleDeformPolicy>::down(float* pHeights, uint _width, uint _height, uint _seed, uint& y, uint& x, uint i, uint elevationThreshold)
	{
		if(y < _height - i)
		{
			if(pHeights[y * _width + x] > pHeights[(y + i) * _width + x] + elevationThreshold)
			{
				y += i;
				return true;
			}
		}

		return false;
	}
//----------------------------------------------------------------------
	template <typename ParticlePositionPolicy, typename ParticleDeformPolicy>
	bool ParticleDeposition <typename ParticlePositionPolicy, typename ParticleDeformPolicy>::downLeft(float* pHeights, uint _width, uint _height, uint _seed, uint& y, uint& x, uint i, uint elevationThreshold)
	{
		if(y < _height - i && x >= i)
		{
			if(pHeights[y * _width + x] > pHeights[(y + i) * _width + (x - i)] + elevationThreshold)
			{
				y += i;
				x -= i;
				return true;
			}
		}

		return false;
	}
//----------------------------------------------------------------------
	template <typename ParticlePositionPolicy, typename ParticleDeformPolicy>
	bool ParticleDeposition <typename ParticlePositionPolicy, typename ParticleDeformPolicy>::downRight(float* pHeights, uint _width, uint _height, uint _seed, uint& y, uint& x, uint i, uint elevationThreshold)
	{
		if(y < _height - i && x < _width - 1)
		{
			if(pHeights[y * _width + x] > pHeights[(y + i) * _width + (x + i)] + elevationThreshold)
			{
				y += i;
				x += i;
				return true;
			}
		}

		return false;
	}
//----------------------------------------------------------------------
	template <typename ParticlePositionPolicy, typename ParticleDeformPolicy>
	bool ParticleDeposition <typename ParticlePositionPolicy, typename ParticleDeformPolicy>::left(float* pHeights, uint _width, uint _height, uint _seed, uint& y, uint& x, uint i, uint elevationThreshold)
	{
		if(x >= i)
		{
			if(pHeights[y * _width + x] > pHeights[y * _width + (x - i)] + elevationThreshold)
			{
				x -= i;
				return true;
			}
		}

		return false;
	}
//----------------------------------------------------------------------
	template <typename ParticlePositionPolicy, typename ParticleDeformPolicy>
	bool ParticleDeposition <typename ParticlePositionPolicy, typename ParticleDeformPolicy>::right(float* pHeights, uint _width, uint _height, uint _seed, uint& y, uint& x, uint i, uint elevationThreshold)
	{
		if(x < _width - i)
		{
			if(pHeights[y * _width + x] > pHeights[y * _width + (x + i)] + elevationThreshold)
			{
				x += i;
				return true;
			}
		}

		return false;
	}
//----------------------------------------------------------------------
}
}

#endif