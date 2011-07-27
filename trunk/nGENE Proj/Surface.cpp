/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Surface.cpp
Version:	0.21
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Surface.h"
#include "AABB.h"
#include "Material.h"
#include "MaterialLibrary.h"
#include "MaterialManager.h"
#include "NodeVisible.h"
#include "OBB.h"
#include "Ray.h"
#include "VertexBufferManager.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo Surface::Type(L"Surface", &ICullable::Type);


	Surface::Surface():
		m_pMaterial(NULL),
		m_pNode(NULL),
		m_bEnabled(true),
		m_bLightable(true),
		m_bFlippedNormals(false),
		m_bFlippedOrder(false),
		onUpdateAABB(NULL),
		onBeginRender(NULL),
		onEndRender(NULL)
	{
		m_bPickable = true;
	}
//----------------------------------------------------------------------
	Surface::Surface(const wstring& _name):
		m_pMaterial(NULL),
		m_pNode(NULL),
		m_bEnabled(true),
		m_bLightable(true),
		m_bFlippedNormals(false),
		m_bFlippedOrder(false),
		m_stName(_name),
		onUpdateAABB(NULL),
		onBeginRender(NULL),
		onEndRender(NULL)
	{
		m_bPickable = true;
	}
//----------------------------------------------------------------------
	Surface::Surface(const Surface& src)
	{
		m_stName = src.m_stName;
		m_pMaterial = src.m_pMaterial;
		m_pNode = src.m_pNode;
		m_bEnabled = src.m_bEnabled;
		m_bLightable = src.m_bLightable;
		m_bFlippedNormals = src.m_bFlippedNormals;
		m_bFlippedOrder = src.m_bFlippedOrder;
		m_vecTexCoordMult = src.m_vecTexCoordMult;

		m_bPickable = src.m_bPickable;

		m_pVBs = src.m_pVBs;
		for(uint i = 0; i < m_pVBs.size(); ++i)
		{
			VertexBuffer* pVB = m_pVBs[i].second;
			if(pVB)
				pVB->setReferenceCount(pVB->getReferenceCount() + 1);
		}

		onUpdateAABB = src.onUpdateAABB;
		onBeginRender = src.onBeginRender;
		onEndRender = src.onEndRender;
	}
//----------------------------------------------------------------------
	Surface& Surface::operator=(const Surface& rhs)
	{
		if(this == &rhs)
			return (*this);

		m_stName = rhs.m_stName;
		m_pMaterial = rhs.m_pMaterial;
		m_pNode = rhs.m_pNode;
		m_bEnabled = rhs.m_bEnabled;
		m_bLightable = rhs.m_bLightable;
		m_bFlippedNormals = rhs.m_bFlippedNormals;
		m_bFlippedOrder = rhs.m_bFlippedOrder;
		m_vecTexCoordMult = rhs.m_vecTexCoordMult;

		m_bPickable = rhs.m_bPickable;

		for(uint i = 0; i < m_pVBs.size(); ++i)
			destroyVB(i);
		m_pVBs.clear();

		m_pVBs = rhs.m_pVBs;
		for(uint i = 0; i < m_pVBs.size(); ++i)
		{
			VertexBuffer* pVB = m_pVBs[i].second;
			if(pVB)
				pVB->setReferenceCount(pVB->getReferenceCount() + 1);
		}

		onUpdateAABB = rhs.onUpdateAABB;
		onBeginRender = rhs.onBeginRender;
		onEndRender = rhs.onEndRender;

		return (*this);
	}
//----------------------------------------------------------------------
	Surface::~Surface()
	{
		for(uint i = 0; i < m_pVBs.size(); ++i)
			destroyVB(i);
		m_pVBs.clear();

		m_pVBs.clear();
		m_pNode = NULL;
		m_pMaterial = NULL;

		onUpdateAABB = NULL;
		onBeginRender = NULL;
		onEndRender = NULL;
	}
//----------------------------------------------------------------------
	void Surface::render()
	{
		// No material, so we are not rendering it
		if(!m_pMaterial || !m_pNode->isEnabled() || !m_bEnabled || m_bCulled || !m_pVBs.size())
			return;

		// Begin rendering
		if(onBeginRender)
			onBeginRender();


		// Render surface
		for(uint i = 0; i < m_pVBs.size(); ++i)
		{
			VERTEX_STREAM& stream = m_pVBs[i];

			stream.second->setStreamIndex(stream.first);
			stream.second->prepare();
		}
		
		m_pVBs[0].second->render();


		// End rendering
		if(onEndRender)
			onEndRender();
	}
//----------------------------------------------------------------------
	void Surface::setIndexedBuffer(IndexedBuffer* _ivb, uint _index)
	{
		m_pVBs[_index].second->setIndexedBuffer(_ivb);
	}
//----------------------------------------------------------------------
	IndexedBuffer* Surface::getIndexedBuffer(uint _index) const
	{
		return m_pVBs[_index].second->getIndexedBuffer();
	}
//----------------------------------------------------------------------
	void Surface::setMaterial(Material* _material)
	{
		// Get rid of the previous material
		if(m_pMaterial)
			m_pMaterial->removeSurface(this);

		m_pMaterial = _material;
		if(m_pMaterial)
			m_pMaterial->addSurface(this);
	}
//----------------------------------------------------------------------
	void Surface::prepare()
	{
		Renderer::getSingleton().setWorldMatrix(m_pNode->getTransform());
	}
//----------------------------------------------------------------------
	void Surface::calculateBoundingBox()
	{
		if(onUpdateAABB)
			onUpdateAABB();
		else
		{
			if(m_pVBs.size() == 1)
			{
				m_pOBB->construct(m_pVBs[0].second);
				Vector3 temp = m_pOBB->getMaximum() - m_pOBB->getMinimum();
				if(!Maths::floatCompare(temp.getLength(), 0.0f))
				{
					m_pOBB->setOBB(Vector3(-1.0f, -1.0f, -1.0f),
								   Vector3(1.0f, 1.0f, 1.0f));
				}

				m_pAABB->construct(*m_pOBB);
			}
			else if(m_pVBs.size() > 1)
			{
				m_pOBB->construct(m_pVBs[0].second);
				Vector3 temp = m_pOBB->getMaximum() - m_pOBB->getMinimum();
				if(!Maths::floatCompare(temp.getLength(), 0.0f))
				{
					m_pOBB->setOBB(Vector3(-1.0f, -1.0f, -1.0f),
								   Vector3(1.0f, 1.0f, 1.0f));
				}

				m_pAABB->construct(*m_pOBB);

				OBB obb;
				AABB aabb;
				Vector3 curMin = m_pAABB->getMinimum();
				Vector3 curMax = m_pAABB->getMaximum();
				for(uint i = 1; i < m_pVBs.size(); ++i)
				{
					obb.construct(m_pVBs[i].second);
					obb.setTransform(m_pNode->getTransform());
					Vector3 temp = obb.getMaximum() - obb.getMinimum();
					if(!Maths::floatCompare(temp.getLength(), 0.0f))
					{
						obb.setOBB(Vector3(-1.0f, -1.0f, -1.0f),
								   Vector3(1.0f, 1.0f, 1.0f));
					}

					aabb.construct(obb);

					Vector3 tmpMin = aabb.getMinimum();
					Vector3 tmpMax = aabb.getMaximum();

					if(tmpMin.x < curMin.x)
						curMin.x = tmpMin.x;
					if(tmpMin.y < curMin.y)
						curMin.y = tmpMin.y;
					if(tmpMin.z < curMin.z)
						curMin.z = tmpMin.z;

					if(tmpMax.x > curMax.x)
						curMax.x = tmpMax.x;
					if(tmpMax.y > curMax.y)
						curMax.y = tmpMax.y;
					if(tmpMax.z > curMax.z)
						curMax.z = tmpMax.z;
				}

				m_pAABB->setAABB(curMin, curMax);
			}
		}
	}
//----------------------------------------------------------------------
	void Surface::updateBoundingBox()
	{
		if(onUpdateAABB)
			onUpdateAABB();
		else
		{
			// If we have dynamic buffer we have to update bounding boxes every frame
			if(m_pVBs[0].second->getDesc().type == VB_DYNAMIC)
			{
				calculateBoundingBox();
				return;
			}

			if(m_pVBs.size() == 1)
			{
				if(m_pOBB)
					m_pOBB->setTransform(m_pNode->getTransform());
				if(m_pAABB)
					m_pAABB->construct(*m_pOBB);
			}
			else
			{
				calculateBoundingBox();
			}
		}
	}
//----------------------------------------------------------------------
	void Surface::setVertexBuffer(VertexBuffer* _vb, uint _index)
	{
		_vb->setReferenceCount(_vb->getReferenceCount() + 1);

		if(_index < m_pVBs.size())
		{
			destroyVB(_index);
			m_pVBs.erase(m_pVBs.begin() + _index);
		}

		VERTEX_STREAM stream = make_pair(_index, _vb);

		m_pVBs.push_back(stream);

		calculateBoundingBox();
	}
//----------------------------------------------------------------------
	void Surface::flipNormals()
	{
		for(uint i = 0; i < m_pVBs.size(); ++i)
		{
			VertexBuffer* pVB = m_pVBs[i].second;

			dword dwVerticesNum = pVB->getVerticesNum();
			SVertex* pVertices;
			pVB->lock(0, dwVerticesNum, (void**)&pVertices);

			for(uint i = 0; i < dwVerticesNum; ++i)
				pVertices[i].vecNormal.negate();

			pVB->unlock();
		}

		m_bFlippedNormals = m_bFlippedNormals ? false : true;
	}
//----------------------------------------------------------------------
	void Surface::flipWindingOrder()
	{
		for(uint i = 0; i < m_pVBs.size(); ++i)
		{
			VertexBuffer* pVB = m_pVBs[i].second;

			IndexedBuffer* pIVB = pVB->getIndexedBuffer();
			dword dwIndicesNum = pIVB->getIndicesNum();

			if(dwIndicesNum > 65536 && pIVB->is32Bit())
			{
				uint* pIndices = new uint[dwIndicesNum];
				pIVB->getIndexData(pIndices, 0, dwIndicesNum);

				for(uint i = 0; i < dwIndicesNum; i += 3)
				{
					uint temp = pIndices[i];
					pIndices[i] = pIndices[i + 2];
					pIndices[i + 2] = temp;
				}

				pIVB->setIndexData(pIndices, 0, dwIndicesNum);

				NGENE_DELETE_ARRAY(pIndices);
			}
			else
			{
				ushort* pIndices = new ushort[dwIndicesNum];
				pIVB->getIndexData(pIndices, 0, dwIndicesNum);

				for(uint i = 0; i < dwIndicesNum; i += 3)
				{
					ushort temp = pIndices[i];
					pIndices[i] = pIndices[i + 2];
					pIndices[i + 2] = temp;
				}

				pIVB->setIndexData(pIndices, 0, dwIndicesNum);

				NGENE_DELETE_ARRAY(pIndices);
			}
		}

		m_bFlippedOrder = m_bFlippedOrder ? false : true;
	}
//----------------------------------------------------------------------
	void Surface::serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren)
	{
		if(_serializeType) _serializer->addObject(this->Type);
			
			Property <bool> prLightable(m_bLightable);
			_serializer->addProperty("Lightable", prLightable);
			Property <bool> prEnabled(m_bEnabled);
			_serializer->addProperty("Enabled", prEnabled);
			Property <bool> prPickable(m_bPickable);
			_serializer->addProperty("Pickable", prPickable);

			Property <bool> prFlippedOrder(m_bFlippedOrder);
			_serializer->addProperty("FlippedOrder", prFlippedOrder);
			Property <bool> prFlippedNormals(m_bFlippedNormals);
			_serializer->addProperty("FlippedNormals", prFlippedNormals);

			Property <Vector2> prTexMult(m_vecTexCoordMult);
			_serializer->addProperty("TexCoordMult", prTexMult);

			Property <wstring> prName(m_stName);
			_serializer->addProperty("Name", prName);

			// Serialize material data
			if(m_pMaterial)
			{
				Property <wstring> prMat(m_pMaterial->getName());
				_serializer->addProperty("Material", prMat);
				Property <wstring> prLib(m_pMaterial->getMaterialLibrary()->getName());
				_serializer->addProperty("Library", prLib);
			}
		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void Surface::deserialize(ISerializer* _serializer)
	{
		Property <bool> prLightable(m_bLightable);
		_serializer->getProperty("Lightable", prLightable);
		Property <bool> prEnabled(m_bEnabled);
		_serializer->getProperty("Enabled", prEnabled);
		Property <bool> prPickable(m_bPickable);
		_serializer->getProperty("Pickable", prPickable);

		Property <Vector2> prTexMult(m_vecTexCoordMult);
		_serializer->getProperty("TexCoordMult", prTexMult);

		Property <wstring> prName(m_stName);
		_serializer->getProperty("Name", prName);

		wstring stLib;
		wstring stMat;
		Property <wstring> prLib(stLib);
		Property <wstring> prMat(stMat);
		_serializer->getProperty("Library", prLib);
		_serializer->getProperty("Material", prMat);
		if(stLib != L"" && stMat != L"")
		{
			m_pMaterial = MaterialManager::getSingleton().getLibrary(stLib)->getMaterial(stMat);
		}

		// Flip winding order?
		bool blnTmp;
		Property <bool> prFlippedOrder(blnTmp);
		_serializer->getProperty("FlippedOrder", prFlippedOrder);
		if(blnTmp)
			flipWindingOrder();			

		// Flip normals?
		Property <bool> prFlippedNormals(blnTmp);
		_serializer->getProperty("FlippedNormals", prFlippedNormals);
		if(blnTmp)
			flipNormals();
	}
//----------------------------------------------------------------------
	bool Surface::tryToPick(const Ray& _ray)
	{
		Ray ray = _ray;
		ray.moveToObjectSpace(m_pNode->getTransform());
		Vector3 dir = ray.getDirection() * 2000.0f;
		ray.setDirection(dir);

		return ray.intersectSurface(this);
	}
//----------------------------------------------------------------------
	void Surface::destroyVB(uint _index)
	{
		VertexBuffer* pVB = m_pVBs[_index].second;

		if(pVB)
		{
			pVB->setReferenceCount(pVB->getReferenceCount() - 1);
			if(!pVB->getReferenceCount())
				VertexBufferManager::getSingleton().removeVertexBuffer(pVB);
		}
	}
//----------------------------------------------------------------------
}