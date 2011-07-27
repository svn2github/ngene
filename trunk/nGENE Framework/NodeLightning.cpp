/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		NodeLightning.cpp
Version:	0.04
---------------------------------------------------------------------------
*/

#include "NodeLightning.h"
#include "AABB.h"
#include "Material.h"
#include "MaterialLibrary.h"
#include "MaterialManager.h"
#include "OBB.h"
#include "Shader.h"
#include "ShaderConstant.h"
#include "ShaderInstance.h"
#include "Timer.h"
#include "Vector3.h"
#include "VertexBufferManager.h"

#include "Camera.h"


namespace nGENE
{
namespace Nature
{
	// Initialize static members
	TypeInfo NodeLightning::Type(L"NodeLightning", &NodeVisible::Type);



	class BoltSegmentComparator
	{
	public:
		bool operator()(const SLightningSegment& _segment1, const SLightningSegment& _segment2)
		{
			if(_segment1.branchID == _segment2.branchID)
				return (_segment1.ID < _segment2.ID);
			else
				return (_segment1.branchID < _segment2.branchID);
		}
	};




	NodeLightning::NodeLightning():
		m_pVB(NULL),
		m_pIVB(NULL),
		m_vecStartPoint(0.0f, 30.0f, 0.0f),
		m_vecEndPoint(0.0f, 0.0f, 0.0f),
		m_nIterationsNum(6),
		m_nAdditionalBranches(6),
		m_fMinBranchLength(5.0f),
		m_fOffset(6.0f),
		m_fBranchSpread(12.0f),
		m_fWidth(0.2f),
		m_fCurrentAlpha(1.0f),
		m_fCurrentAlpha2(0.0f),
		m_dwUpdateFrequency(500),
		m_dwTotal(0),
		m_Colour(Colour::COLOUR_WHITE),
		m_bStarted(false),
		m_fMaxFlash(1.0f),
		m_fMaxAlpha2(1.0f),
		m_dwPause(250),
		m_dwNumberOfRuns(0),
		m_dwNumberOfRunsLeft(0),
		m_pVertices1(NULL),
		m_pVertices2(NULL)
	{
	}
//----------------------------------------------------------------------
	NodeLightning::NodeLightning(const NodeLightning& src):
		NodeVisible(src)
	{
		m_pVB = src.m_pVB;
		m_pIVB = src.m_pIVB;

		m_pVB2 = src.m_pVB2;
		m_pIVB2 = src.m_pIVB2;

		m_vecStartPoint = src.m_vecStartPoint;
		m_vecEndPoint = src.m_vecEndPoint;

		m_nIterationsNum = src.m_nIterationsNum;
		m_nAdditionalBranches = src.m_nAdditionalBranches;

		m_fMinBranchLength = src.m_fMinBranchLength;
		m_fOffset = src.m_fOffset;
		m_fBranchSpread = src.m_fBranchSpread;
		m_fWidth = src.m_fWidth;

		m_fCurrentAlpha = src.m_fCurrentAlpha;
		m_fCurrentAlpha2 = src.m_fCurrentAlpha2;
		m_fMaxFlash = src.m_fMaxFlash;
		m_fMaxAlpha2 = src.m_fMaxAlpha2;

		m_dwPrevTime = src.m_dwPrevTime;
		m_dwTotal = src.m_dwTotal;
		m_dwTotal2 = src.m_dwTotal2;
		m_dwUpdateFrequency = src.m_dwUpdateFrequency;
		m_dwPause = src.m_dwPause;
		m_dwNumberOfRuns = src.m_dwNumberOfRuns;
		m_dwNumberOfRunsLeft = src.m_dwNumberOfRunsLeft;

		m_bStarted = src.m_bStarted;

		m_Colour = src.m_Colour;

		m_BranchesStart = src.m_BranchesStart;

		m_vSurfaces[0]->onBeginRender.bind(this, &NodeLightning::beginRender1);
		m_vSurfaces[0]->onUpdateAABB.bind(this, &NodeLightning::updateAABB1);

		m_vSurfaces[1]->onBeginRender.bind(this, &NodeLightning::beginRender2);
		m_vSurfaces[1]->onUpdateAABB.bind(this, &NodeLightning::updateAABB2);

		m_pVertices1 = NULL;
		m_pVertices2 = NULL;

		Node::m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	NodeLightning& NodeLightning::operator=(const NodeLightning& rhs)
	{
		if(this == &rhs)
			return (*this);

		NodeVisible::operator=(rhs);


		m_pVB = rhs.m_pVB;
		m_pIVB = rhs.m_pIVB;

		m_pVB2 = rhs.m_pVB2;
		m_pIVB2 = rhs.m_pIVB2;

		m_vecStartPoint = rhs.m_vecStartPoint;
		m_vecEndPoint = rhs.m_vecEndPoint;

		m_nIterationsNum = rhs.m_nIterationsNum;
		m_nAdditionalBranches = rhs.m_nAdditionalBranches;

		m_fMinBranchLength = rhs.m_fMinBranchLength;
		m_fOffset = rhs.m_fOffset;
		m_fBranchSpread = rhs.m_fBranchSpread;
		m_fWidth = rhs.m_fWidth;

		m_fCurrentAlpha = rhs.m_fCurrentAlpha;
		m_fCurrentAlpha2 = rhs.m_fCurrentAlpha2;
		m_fMaxFlash = rhs.m_fMaxFlash;
		m_fMaxAlpha2 = rhs.m_fMaxAlpha2;

		m_dwPrevTime = rhs.m_dwPrevTime;
		m_dwTotal = rhs.m_dwTotal;
		m_dwTotal2 = rhs.m_dwTotal2;
		m_dwUpdateFrequency = rhs.m_dwUpdateFrequency;
		m_dwPause = rhs.m_dwPause;
		m_dwNumberOfRuns = rhs.m_dwNumberOfRuns;
		m_dwNumberOfRunsLeft = rhs.m_dwNumberOfRunsLeft;

		m_bStarted = rhs.m_bStarted;

		m_Colour = rhs.m_Colour;

		m_BranchesStart = rhs.m_BranchesStart;

		m_vSurfaces[0]->onBeginRender.bind(this, &NodeLightning::beginRender1);
		m_vSurfaces[0]->onUpdateAABB.bind(this, &NodeLightning::updateAABB1);

		m_vSurfaces[1]->onBeginRender.bind(this, &NodeLightning::beginRender2);
		m_vSurfaces[1]->onUpdateAABB.bind(this, &NodeLightning::updateAABB2);

		m_pVertices1 = NULL;
		m_pVertices2 = NULL;


		Node::m_bHasChanged = true;


		return (*this);
	}
//----------------------------------------------------------------------
	NodeLightning::~NodeLightning()
	{
		NGENE_DELETE_ARRAY(m_pVertices1);
		NGENE_DELETE_ARRAY(m_pVertices2);
	}
//----------------------------------------------------------------------
	void NodeLightning::generate(vector <SLightningSegment>& _segments)
	{
		uint nextFree = 0;
		uint ID = 0;
		uint additionalBranches = m_nAdditionalBranches;

		float offset = m_fOffset;
		float spread = m_fBranchSpread;


		m_BranchesStart[nextFree++] = m_vecStartPoint;
		SLightningSegment s = {m_vecStartPoint, m_vecEndPoint, 0, ID++, 1.0f, m_fWidth};
		_segments.push_back(s);


		for(uint i = 0; i < m_nIterationsNum; ++i)
		{
			uint scount = _segments.size();
			for(uint j = 0; j < scount; ++j)
			{
				SLightningSegment s = _segments[j];

				Vector3 midPoint = (s.start - s.end) * 0.5f + s.end;
				Vector3 temp = s.start - s.end;
				temp.normalize();

				// Choose direction
				if(rand() % 10 < 5)
					temp.set(-temp.y, temp.x, temp.z);
				else
					temp.set(temp.x, temp.z, -temp.y);
				temp.normalize();
				Vector3 off = (Maths::perlinNoiseGenerator(rand() % 100, rand() % 100, rand() % 100) * offset) * temp;
				midPoint += off;

				SLightningSegment s1 = {s.start, midPoint, s.branchID, ID++, s.alpha, s.width};
				SLightningSegment s2 = {midPoint, s.end, s.branchID, ID++, s.alpha, s.width};

				// Insert both _segments
				_segments.push_back(s1);
				_segments.push_back(s2);

				// Split
				bool split = false;
				if(rand() % 10 < 8)
					split = true;
				if(additionalBranches && split)
				{
					Vector3 dir = midPoint - s.start;
					float maxLength = dir.length();
					if(dir.y > 0)
						dir.y *= -1;
					dir.x += (Maths::perlinNoiseGenerator(rand() % 100, rand() % 100, rand() % 100) * spread);
					dir.z += (Maths::perlinNoiseGenerator(rand() % 100, rand() % 100, rand() % 100) * spread);
					dir.normalize();

					float branchLength = (abs(Maths::perlinNoiseGenerator(rand() % 100, rand() % 100, rand() % 100)) * maxLength);
					if(branchLength + m_fMinBranchLength <= maxLength)
						branchLength += m_fMinBranchLength;
					Vector3 branchEnd = midPoint + dir * branchLength;
					m_BranchesStart[nextFree] = midPoint;
					SLightningSegment s3 = {midPoint, branchEnd, nextFree++, ID++, s.alpha * 0.85f, s.width * 0.75f};

					_segments.push_back(s3);

					--additionalBranches;
				}
			}

			_segments.erase(_segments.begin(), _segments.begin() + scount);

			offset *= 0.5f;
			spread *= 0.5f;
		}

		BoltSegmentComparator comparator;
		std::sort(_segments.begin(), _segments.end(), comparator);
	}
//----------------------------------------------------------------------
	void NodeLightning::onUpdate()
	{
		NodeVisible::onUpdate();

		// Not updating as maximum number of lightnings were created
		if(m_dwNumberOfRuns && !m_dwNumberOfRunsLeft)
		{
			if(Maths::floatCompare(m_fCurrentAlpha, 0.0f) == 0 &&
				Maths::floatCompare(m_fCurrentAlpha2, 0.0f) == 0)
			{
				m_fCurrentAlpha = 0.0f;
				m_fCurrentAlpha2= 0.0f;

				return;
			}
			else
			{
				m_fCurrentAlpha = 0.0f;
				m_fCurrentAlpha2= 0.0f;

				vector <SLightningSegment> segments;
				generate(segments);

				updateGeometry(segments, &m_pVB, &m_pIVB, m_fCurrentAlpha, m_vSurfaces[0], &m_pVertices1);
				updateGeometry(segments, &m_pVB2, &m_pIVB2, m_fCurrentAlpha2, m_vSurfaces[1], &m_pVertices2);
			}
		}

		dword now = Engine::getSingleton().getTimer().getMilliseconds();
		dword diff = now - m_dwPrevTime;
		m_dwTotal += diff;
		m_dwTotal2 += diff;


		// Update first bolt
		if(!m_dwNumberOfRuns || m_dwNumberOfRunsLeft >= 2)
		{
			if(m_dwTotal >= m_dwUpdateFrequency)
			{
				if(m_dwNumberOfRuns)
					--m_dwNumberOfRunsLeft;

				if(!m_dwNumberOfRuns || m_dwNumberOfRunsLeft >= 2)
				{
					vector <SLightningSegment> segments;

					m_fCurrentAlpha = 1.0f;
					generate(segments);


					updateGeometry(segments, &m_pVB, &m_pIVB, m_fCurrentAlpha, m_vSurfaces[0], &m_pVertices1);
				}

				m_dwTotal -= m_dwUpdateFrequency;
			}
			else
			{
				float t = (float)m_dwTotal / (float)m_dwUpdateFrequency;
				m_fCurrentAlpha = Maths::lerp(1.0f, 0.0f, t);
			}
		}

		if(!m_bStarted)
		{
			if(m_dwTotal >= m_dwPause)
			{
				m_bStarted = true;
				m_dwTotal2 = 0;
			}
		}

		// Update second bolt
		if(!m_dwNumberOfRuns || m_dwNumberOfRunsLeft >= 1)
		{
			if(m_bStarted)
			{
				if(m_dwTotal2 >= m_dwUpdateFrequency)
				{
					if(m_dwNumberOfRuns)
						--m_dwNumberOfRunsLeft;

					if(!m_dwNumberOfRuns || m_dwNumberOfRunsLeft >= 1)
					{
						vector <SLightningSegment> segments;

						m_fCurrentAlpha2 = 1.0f;
						generate(segments);


						updateGeometry(segments, &m_pVB2, &m_pIVB2, m_fCurrentAlpha2, m_vSurfaces[1], &m_pVertices2);
					}

					m_dwTotal2 -= m_dwUpdateFrequency;
				}
				else
				{
					float t = (float)m_dwTotal2 / (float)m_dwUpdateFrequency;
					m_fCurrentAlpha2 = Maths::lerp(1.0f, 0.0f, t);
				}
			}
		}

		
		ShaderConstant* pConstant = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"lightning")->getActiveRenderTechnique()->getRenderPass(3).getPixelShader()->getConstant("flashFactor");
		float flash = m_fCurrentAlpha * m_fMaxFlash;
		if(pConstant)
			pConstant->setDefaultValue(&flash);

		m_dwPrevTime = now;
	}
//----------------------------------------------------------------------
	void NodeLightning::updateGeometry(const vector<SLightningSegment>& _segments,
		VertexBuffer** _VB, IndexedBuffer** _IVB,
		float _alpha, Surface* _surface, SVertex** _vertices)
	{
		NGENE_DELETE_ARRAY(*_vertices);

		uint count = _segments.size();
		dword dwNumVertices = count * 4;

		*_vertices = new SVertex[dwNumVertices];
		SVertex* pVertex = *_vertices;

		VertexBuffer* pVB = *_VB;
		IndexedBuffer* pIVB = *_IVB;

		uint bufferSize = pVB->getDesc().verticesNum;
		if(bufferSize < dwNumVertices)
		{
			VERTEXBUFFER_DESC desc = pVB->getDesc();

			desc.verticesNum = dwNumVertices;
			desc.vertices = NULL;
			*_VB = VertexBufferManager::getSingleton().createVertexBuffer(desc);
			pVB = *_VB;
		}


		Vector3 end1;
		Vector3 end2;
		Vector3 normal;
		bool bNewBranch = true;
		for(uint i = 0; i < count; ++i)
		{
			SLightningSegment s = _segments[i];

			Colour col = m_Colour;
			byte alpha = (byte)(s.alpha * _alpha * 255.0f);
			col.setAlpha(alpha);

			if(i)
			{
				if(s.branchID != _segments[i - 1].branchID)
					bNewBranch = true;
			}


			if(bNewBranch)
			{
				Vector3 start = m_BranchesStart[s.branchID];

				normal =  s.start - s.end;
				normal.set(-normal.y, normal.x, normal.z);
				normal.normalize();

				pVertex[i * 4 + 0].vecPosition = start + s.width * normal;
				pVertex[i * 4 + 1].vecPosition = start - s.width * normal;

				bNewBranch = false;
			}
			else
			{
				pVertex[i * 4 + 0].vecPosition = end1;
				pVertex[i * 4 + 1].vecPosition = end2;
			}

			if(i < count - 1)
			{
				normal =  s.start - s.end;
				Vector3 normal2 = _segments[i + 1].start - _segments[i + 1].end;
				normal = (normal + normal2) * 0.5f;
				normal.set(-normal.y, normal.x, normal.z);
				normal.normalize();
			}

			end1 = s.end + s.width * normal;
			end2 = s.end - s.width * normal;
			pVertex[i * 4 + 2].vecPosition = end1;
			pVertex[i * 4 + 3].vecPosition = end2;

			pVertex[i * 4 + 0].dwColour = col;
			pVertex[i * 4 + 1].dwColour = col;
			pVertex[i * 4 + 2].dwColour = col;
			pVertex[i * 4 + 3].dwColour = col;

			pVertex[i * 4 + 0].vecTexCoord = Vector2(0.0f, 0.0f);
			pVertex[i * 4 + 1].vecTexCoord = Vector2(1.0f, 0.0f);
			pVertex[i * 4 + 2].vecTexCoord = Vector2(0.0f, 1.0f);
			pVertex[i * 4 + 3].vecTexCoord = Vector2(1.0f, 1.0f);
		}

		SVertex* pVertices;

		pVB->lock(0, dwNumVertices, (void**)&pVertices);
			memcpy(pVertices, pVertex, dwNumVertices * sizeof(SVertex));
		pVB->unlock();

		pVB->setPrimitivesNum(count * 2);

		// Set indices
		dword dwNumIndices = count * 6;
		ushort* pIndices;

		bufferSize = pVB->getDesc().verticesNum;
		if(bufferSize < dwNumIndices)
		{
			INDEXEDBUFFER_DESC desc = pIVB->getDesc();

			desc.indicesNum = dwNumIndices;
			desc.indices = NULL;
			*_IVB = VertexBufferManager::getSingleton().createIndexedBuffer(desc);
			pIVB = *_IVB;
			
			pVB->setIndexedBuffer(pIVB);
			_surface->setVertexBuffer(pVB);
		}

		pIVB->lock(0, dwNumIndices, (void**)&pIndices);

		ushort begin = 0;

		for(uint i = 0; i < dwNumIndices; i += 6)
		{
			pIndices[i + 0] = begin + 3;
			pIndices[i + 1] = begin + 1;
			pIndices[i + 2] = begin;

			pIndices[i + 3] = begin + 2;
			pIndices[i + 4] = begin + 3;
			pIndices[i + 5] = begin;

			begin += 4;
		}

		pIVB->unlock();

		_surface->updateBoundingBox();
	}
//----------------------------------------------------------------------
	void NodeLightning::init()
	{
		vector <SLightningSegment> segments;

		// First generate lightning...
		generate(segments);

		// Create surfaces
		uint count = segments.size();
		dword dwNumVertices = count * 4;
		dword dwNumIndices = count * 6;


		VertexBufferManager& manager = VertexBufferManager::getSingleton();

		INDEXEDBUFFER_DESC ivbDesc;
		ivbDesc.type = IVB_DYNAMIC;
		ivbDesc.discardData = true;
		ivbDesc.indices = NULL;
		ivbDesc.indicesNum = dwNumIndices;
		m_pIVB = manager.createIndexedBuffer(ivbDesc);

		VERTEXBUFFER_DESC vbDesc;
		vbDesc.type = VB_DYNAMIC;
		vbDesc.discardData = true;
		vbDesc.vertices = NULL;
		vbDesc.verticesNum = dwNumVertices;
		vbDesc.primitivesNum = count * 2;
		vbDesc.primitivesType = PT_TRIANGLELIST;
		vbDesc.indices = m_pIVB;
		vbDesc.vertexDeclaration = manager.getVertexDeclaration(L"Default");
		m_pVB = manager.createVertexBuffer(vbDesc);

		// First surface
		Surface temp;
		temp.setMaterial(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"lightning"));
		temp.setVertexBuffer(m_pVB);
		addSurface(L"Bolt1", temp);


		m_pIVB2 = manager.createIndexedBuffer(ivbDesc);
		m_pVB2 = manager.createVertexBuffer(vbDesc);

		// Second surface
		Surface temp2;
		temp2.setMaterial(MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"lightning"));
		temp2.setVertexBuffer(m_pVB2);
		addSurface(L"Bolt2", temp2);


		// Bind delegates
		m_vSurfaces[0]->onBeginRender.bind(this, &NodeLightning::beginRender1);
		m_vSurfaces[0]->onUpdateAABB.bind(this, &NodeLightning::updateAABB1);

		m_vSurfaces[1]->onBeginRender.bind(this, &NodeLightning::beginRender2);
		m_vSurfaces[1]->onUpdateAABB.bind(this, &NodeLightning::updateAABB2);


		updateGeometry(segments, &m_pVB, &m_pIVB, 0.0f, m_vSurfaces[0], &m_pVertices1);
		updateGeometry(segments, &m_pVB2, &m_pIVB2, m_fCurrentAlpha2, m_vSurfaces[1], &m_pVertices2);


		calculateBoundingBox();


		MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"lightning")->setPriority(1000);


		m_dwPrevTime = Engine::getSingleton().getTimer().getMilliseconds();
	}
//----------------------------------------------------------------------
	void NodeLightning::cleanup()
	{
		NodeVisible::cleanup();
	}
//----------------------------------------------------------------------
	void NodeLightning::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
		NodeVisible::serialize(_serializer, false, _serializeChildren);

		Property <Vector3> prStartPoint(m_vecStartPoint);
		_serializer->addProperty("StartPoint", prStartPoint);
		Property <Vector3> prEndPoint(m_vecEndPoint);
		_serializer->addProperty("EndPoint", prEndPoint);

		Property <uint> prSegmentsNum(m_nIterationsNum);
		_serializer->addProperty("SegmentsNum", prSegmentsNum);
		Property <uint> prBranches(m_nAdditionalBranches);
		_serializer->addProperty("Branches", prBranches);

		Property <dword> prUpdate(m_dwUpdateFrequency);
		_serializer->addProperty("UpdateFrequency", prUpdate);
		Property <dword> prPause(m_dwPause);
		_serializer->addProperty("Pause", prPause);
		Property <dword> prRuns(m_dwNumberOfRuns);
		_serializer->addProperty("RunsNum", prRuns);

		Property <float> prOffset(m_fOffset);
		_serializer->addProperty("Offset", prOffset);
		Property <float> prSpread(m_fBranchSpread);
		_serializer->addProperty("BranchSpread", prSpread);
		Property <float> prWidth(m_fWidth);
		_serializer->addProperty("Width", prWidth);
		Property <float> prFlash(m_fMaxFlash);
		_serializer->addProperty("Flash", prFlash);
		Property <float> prAlpha2(m_fMaxAlpha2);
		_serializer->addProperty("Alpha2", prAlpha2);
		Property <float> prMinBranchLength(m_fMaxAlpha2);
		_serializer->addProperty("MinBranchLength", prMinBranchLength);

		Property <Colour> prColour(m_Colour);
		_serializer->addProperty("Colour", prColour);

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void NodeLightning::deserialize(ISerializer* _serializer)
	{
		Property <Vector3> prStartPoint(m_vecStartPoint);
		_serializer->getProperty("StartPoint", prStartPoint);
		Property <Vector3> prEndPoint(m_vecEndPoint);
		_serializer->getProperty("EndPoint", prEndPoint);

		Property <uint> prSegmentsNum(m_nIterationsNum);
		_serializer->getProperty("SegmentsNum", prSegmentsNum);
		Property <uint> prBranches(m_nAdditionalBranches);
		_serializer->getProperty("Branches", prBranches);

		Property <dword> prUpdate(m_dwUpdateFrequency);
		_serializer->getProperty("UpdateFrequency", prUpdate);
		Property <dword> prPause(m_dwPause);
		_serializer->getProperty("Pause", prPause);
		Property <dword> prRuns(m_dwNumberOfRuns);
		_serializer->getProperty("RunsNum", prRuns);

		Property <float> prOffset(m_fOffset);
		_serializer->getProperty("Offset", prOffset);
		Property <float> prSpread(m_fBranchSpread);
		_serializer->getProperty("BranchSpread", prSpread);
		Property <float> prWidth(m_fWidth);
		_serializer->getProperty("Width", prWidth);
		Property <float> prFlash(m_fMaxFlash);
		_serializer->getProperty("Flash", prFlash);
		Property <float> prAlpha2(m_fMaxAlpha2);
		_serializer->getProperty("Alpha2", prAlpha2);
		Property <float> prMinBranchLength(m_fMinBranchLength);
		_serializer->getProperty("MinBranchLength", prMinBranchLength);

		Property <Colour> prColour(m_Colour);
		_serializer->getProperty("Colour", prColour);

		NodeVisible::deserialize(_serializer);
	}
//----------------------------------------------------------------------
	void NodeLightning::beginRender1()
	{
		Shader* pShader = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"lightning")->getActiveRenderTechnique()->getRenderPass(0).getPixelShader()->getShader();
		float flash = m_fCurrentAlpha * m_fMaxFlash;

		pShader->setFloat("alphaFactor", m_fCurrentAlpha);

		//pShader = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"lightning")->getActiveRenderTechnique()->getRenderPass(3).getPixelShader()->getShader();
		//pShader->setFloat("flashFactor", flash);
		//if(pConstant)
		//	pConstant->setDefaultValue(&m_fCurrentAlpha);
		//pConstant = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"lightning")->getActiveRenderTechnique()->getRenderPass(3).getPixelShader()->getConstant("flashFactor");
		//float flash = m_fCurrentAlpha * m_fMaxFlash;
		//if(pConstant)
		//	pConstant->setDefaultValue(&flash);
	}
//----------------------------------------------------------------------
	void NodeLightning::beginRender2()
	{
		Shader* pShader = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"lightning")->getActiveRenderTechnique()->getRenderPass(0).getPixelShader()->getShader();
		float flash = m_fCurrentAlpha2 * m_fMaxFlash;

		float alpha = m_fCurrentAlpha2 * m_fMaxAlpha2;
		pShader->setFloat("alphaFactor", alpha);

		//pShader = MaterialManager::getSingleton().getLibrary(L"default")->getMaterial(L"lightning")->getActiveRenderTechnique()->getRenderPass(3).getPixelShader()->getShader();
		//pShader->setFloat("flashFactor", flash);
	}
//----------------------------------------------------------------------
	void NodeLightning::updateAABB1()
	{
		Surface* pSurface = m_vSurfaces[0];
		if(pSurface)
		{
			Matrix4x4 mat;
			mat.translate(m_vecPosition);

			OBB obb;
			obb.construct(m_pVertices1,
						  m_pVB->getPrimitivesNum() * 2,
						  m_pVB->getDesc().vertexSize,
						  0);

			AABB aabb;

			obb.setTransform(mat);
			aabb.construct(obb);

			pSurface->setOBB(obb);
			pSurface->setAABB(aabb);
		}
	}
//----------------------------------------------------------------------
	void NodeLightning::updateAABB2()
	{
		Surface* pSurface = m_vSurfaces[1];
		if(pSurface)
		{
			Matrix4x4 mat;
			mat.translate(m_vecPosition);

			OBB obb;
			obb.construct(m_pVertices2,
						  m_pVB2->getPrimitivesNum() * 2,
						  m_pVB2->getDesc().vertexSize,
						  0);
			AABB aabb;

			obb.setTransform(mat);
			aabb.construct(obb);

			pSurface->setOBB(obb);
			pSurface->setAABB(aabb);
		}
	}
//----------------------------------------------------------------------
}
}