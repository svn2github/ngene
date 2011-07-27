/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeLightning.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_NODELIGHTNING_H_
#define __INC_NODELIGHTNING_H_


#include "NodeVisible.h"
#include "TypeInfo.h"


namespace nGENE
{
namespace Nature
{
	/// A single segment of the lightning bolt.
	struct SLightningSegment
	{
		Vector3 start;
		Vector3 end;
		uint branchID;
		uint ID;
		Real alpha;
		Real width;
	};

	/** Lightning effect.
		@par
			Lightnings are created using middle point method as described
			http://wtomandev.blogspot.com/2009/09/storm-is-coming.html.
	*/
	class NodeLightning: public NodeVisible
	{
		EXPOSE_TYPE
	protected:
		VertexBuffer* m_pVB;
		IndexedBuffer* m_pIVB;

		VertexBuffer* m_pVB2;
		IndexedBuffer* m_pIVB2;

		SVertex* m_pVertices1;
		SVertex* m_pVertices2;

		Vector3 m_vecStartPoint;					///< Start point of the lightning
		Vector3 m_vecEndPoint;						///< End point of the lightning

		uint m_nIterationsNum;						///< Number of algorithm iterations
		uint m_nAdditionalBranches;					///< Number of branches

		Real m_fMinBranchLength;					///< Minimum length of the branch
		Real m_fOffset;
		Real m_fBranchSpread;						///< Spread of branches
		Real m_fWidth;								///< Width of the main branch

		Real m_fCurrentAlpha;
		Real m_fCurrentAlpha2;
		Real m_fMaxAlpha2;

		Real m_fMaxFlash;

		dword m_dwPrevTime;
		dword m_dwUpdateFrequency;
		dword m_dwPause;
		dword m_dwNumberOfRuns;
		dword m_dwNumberOfRunsLeft;

		dword m_dwTotal;
		dword m_dwTotal2;

		bool m_bStarted;

		Colour m_Colour;							///< Lightning colour

		HashTable <uint, Vector3> m_BranchesStart;


		/// Update geometry of the lightning.
		void updateGeometry(const vector<SLightningSegment>& _segments, VertexBuffer** _VB, IndexedBuffer** _IVB,
			float _alpha, Surface* _surface, SVertex** _vertices);
		/// Generate lightning segments
		void generate(vector <SLightningSegment>& _segments);

		void beginRender1();
		void beginRender2();

		void updateAABB1();
		void updateAABB2();

	public:
		NodeLightning();
		NodeLightning(const NodeLightning& src);
		~NodeLightning();

		NodeLightning& NodeLightning::operator=(const NodeLightning& rhs);

		void init();
		void cleanup();

		void onUpdate();

		/// Sets lightning start point.
		void setStartPoint(const Vector3& _start);
		/// Returns lightning start point.
		Vector3& getStartPoint();

		/// Sets lightning end point.
		void setEndPoint(const Vector3& _end);
		/// Returns lightning end point.
		Vector3& getEndPoint();

		/// Sets number of segments divisions (iterations).
		void setSegmentsNum(uint _value);
		/// Return number of iterations.
		uint getSegmentsNum() const;

		/// Sets number of branches.
		void setBranchesNum(uint _value);
		/// Returns number of branches.
		uint getBranchesNum() const;

		/// Sets minimum length of the branch.
		void setMinBranchLength(Real _length);
		/// Returns minimum length of the branch.
		Real getMinBranchLength() const;

		/// Sets offset.
		void setOffset(Real _offset);
		/// Returns offset.
		Real getOffset() const;

		/// Sets offset.
		void setBranchSpread(Real _spread);
		/// Returns offset.
		Real getBranchSpread() const;

		/// Set width of the main branch.
		void setWidth(Real _width);
		/// Returns width of the main branch.
		Real getWidth() const;

		/// Sets how often in milliseconds lightning will be updated.
		void setUpdateFrequency(dword _frequency);
		/// Returns how often in milliseconds lightning will be updated.
		dword getUpdateFrequency() const;

		/// Sets lightning colour.
		void setColour(const Colour& _colour);
		/// Returns lightning colour.
		Colour& getColour();

		/// Sets flash strength.
		void setMaxFlash(Real _value);
		/// Returns flash strength.
		Real getMaxFlash() const;

		/// Sets transparency of the second bolt.
		void setBolt2Alpha(Real _alpha);
		/// Returns transparency of the second bolt.
		Real getBolt2Alpha() const;

		/// Sets maximum number of runs (0 means no limit)
		void setNumberOfRuns(dword _value);
		/// Returns number of runs (0 means no limit).
		dword getNumberOfRuns() const;

		/// Returns number of runs left.
		dword getNumberOfRunsLeft() const;

		/// Sets delay between first and second hit (in milliseconds).
		void setPause(dword _value);
		/// Returns delay between first and second hit (in milliseconds).
		dword getPause() const;

		/// Checks if finished emitting.
		bool hasFinished() const;

		void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		void deserialize(ISerializer* _serializer);
	};



	inline void NodeLightning::setStartPoint(const Vector3& _start)
	{
		m_vecStartPoint = _start;
	}
//----------------------------------------------------------------------
	inline Vector3& NodeLightning::getStartPoint()
	{
		return m_vecStartPoint;
	}
//----------------------------------------------------------------------
	inline void NodeLightning::setEndPoint(const Vector3& _end)
	{
		m_vecEndPoint = _end;
	}
//----------------------------------------------------------------------
	inline Vector3& NodeLightning::getEndPoint()
	{
		return m_vecEndPoint;
	}
//----------------------------------------------------------------------
	inline void NodeLightning::setSegmentsNum(uint _value)
	{
		m_nIterationsNum = _value;
	}
//----------------------------------------------------------------------
	inline uint NodeLightning::getSegmentsNum() const
	{
		return m_nIterationsNum;
	}
//----------------------------------------------------------------------
	inline void NodeLightning::setBranchesNum(uint _value)
	{
		m_nAdditionalBranches = _value;
	}
//----------------------------------------------------------------------
	inline uint NodeLightning::getBranchesNum() const
	{
		return m_nAdditionalBranches;
	}
//----------------------------------------------------------------------
	inline void NodeLightning::setMinBranchLength(Real _length)
	{
		m_fMinBranchLength = _length;
	}
//----------------------------------------------------------------------
	inline Real NodeLightning::getMinBranchLength() const
	{
		return m_fMinBranchLength;
	}
//----------------------------------------------------------------------
	inline void NodeLightning::setOffset(Real _offset)
	{
		m_fOffset = _offset;
	}
//----------------------------------------------------------------------
	inline Real NodeLightning::getOffset() const
	{
		return m_fOffset;
	}
//----------------------------------------------------------------------
	inline void NodeLightning::setWidth(Real _width)
	{
		m_fWidth = _width;
	}
//----------------------------------------------------------------------
	inline Real NodeLightning::getWidth() const
	{
		return m_fWidth;
	}
//----------------------------------------------------------------------
	inline void NodeLightning::setUpdateFrequency(dword _frequency)
	{
		m_dwUpdateFrequency = _frequency;
		m_dwPause = m_dwUpdateFrequency / 2;

		m_dwTotal = 0;
		m_dwTotal2 = 0;
		m_bStarted = false;
	}
//----------------------------------------------------------------------
	inline dword NodeLightning::getUpdateFrequency() const
	{
		return m_dwUpdateFrequency;
	}
//----------------------------------------------------------------------
	inline void NodeLightning::setColour(const Colour& _colour)
	{
		m_Colour = _colour;
	}
//----------------------------------------------------------------------
	inline Colour& NodeLightning::getColour()
	{
		return m_Colour;
	}
//----------------------------------------------------------------------
	inline void NodeLightning::setMaxFlash(Real _value)
	{
		m_fMaxFlash = _value;
	}
//----------------------------------------------------------------------
	inline Real NodeLightning::getMaxFlash() const
	{
		return m_fMaxFlash;
	}
//----------------------------------------------------------------------
	inline void NodeLightning::setPause(dword _value)
	{
		m_dwPause = _value;

		m_dwTotal = 0;
		m_dwTotal2 = 0;
		m_bStarted = false;
	}
//----------------------------------------------------------------------
	inline dword NodeLightning::getPause() const
	{
		return m_dwPause;
	}
//----------------------------------------------------------------------
	inline void NodeLightning::setNumberOfRuns(dword _value)
	{
		m_dwNumberOfRuns = _value;
		m_dwNumberOfRunsLeft = _value * 2;

		m_dwTotal = 0;
		m_dwTotal2 = 0;
		m_bStarted = false;
	}
//----------------------------------------------------------------------
	inline dword NodeLightning::getNumberOfRuns() const
	{
		return m_dwNumberOfRuns;
	}
//----------------------------------------------------------------------
	inline void NodeLightning::setBolt2Alpha(Real _alpha)
	{
		m_fMaxAlpha2 = _alpha;
	}
//----------------------------------------------------------------------
	inline Real NodeLightning::getBolt2Alpha() const
	{
		return m_fMaxAlpha2;
	}
//----------------------------------------------------------------------
	inline void NodeLightning::setBranchSpread(Real _spread)
	{
		m_fBranchSpread = _spread;
	}
//----------------------------------------------------------------------
	inline Real NodeLightning::getBranchSpread() const
	{
		return m_fBranchSpread;
	}
//----------------------------------------------------------------------
	inline dword NodeLightning::getNumberOfRunsLeft() const
	{
		return m_dwNumberOfRunsLeft;
	}
//----------------------------------------------------------------------
	inline bool NodeLightning::hasFinished() const
	{
		if(m_dwNumberOfRuns && !m_dwNumberOfRunsLeft)
			return true;
		else
			return false;
	}
//----------------------------------------------------------------------
}
}


#endif