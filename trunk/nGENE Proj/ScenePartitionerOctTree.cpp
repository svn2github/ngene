/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScenePartitionerOctTree.cpp
Version:	0.03
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "AABB.h"
#include "Frustum.h"
#include "ICullable.h"
#include "Node.h"
#include "Ray.h"
#include "ScenePartitionerOctTree.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo ScenePartitionerOctTree::Type(L"ScenePartitionerOctTree", &ScenePartitioner::Type);


	ScenePartitionerOctTree::ScenePartitionerOctTree()
	{
		m_Description.bounds.setAABB(Vector3(-1000.0f, -1000.0f, -1000.0f),
									 Vector3(1000.0f, 1000.0f, 1000.0f));
		m_Description.minimumNodeSize = 10.0f;

		init();
	}
//----------------------------------------------------------------------
	ScenePartitionerOctTree::ScenePartitionerOctTree(const OCT_TREE_DESC& _desc):
		m_Description(_desc)
	{
		init();
	}
//----------------------------------------------------------------------
	ScenePartitionerOctTree::~ScenePartitionerOctTree()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void ScenePartitionerOctTree::init()
	{
		m_RootNode.NodeBound = m_Description.bounds;
		m_RootNode.pParent = NULL;
		m_RootNode.pTree = this;
	}
//----------------------------------------------------------------------
	void ScenePartitionerOctTree::cleanup()
	{
		m_RootNode.cleanup();
	}
//----------------------------------------------------------------------
	void ScenePartitionerOctTree::addNode(ICullable* _node)
	{
		AABB* pVolume = _node->getBoundingBox();

		float x = Maths::abs <float> (pVolume->getMaximum().x - pVolume->getMinimum().x);
		float y = Maths::abs <float> (pVolume->getMaximum().y - pVolume->getMinimum().y);
		float z = Maths::abs <float> (pVolume->getMaximum().z - pVolume->getMinimum().z);

		// Check if both x, y and z radii are correct. Otherwise - exit.
		if(!Maths::floatCompare(x, 0.0f) ||
		   !Maths::floatCompare(y, 0.0f) ||
		   !Maths::floatCompare(z, 0.0f))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
				__LINE__, L"None of the node's bounding volumes dimensions can be 0");
			return;
		}

		OctTreeNode* pNode = &m_RootNode;


		while(pNode)
		{
			AABB& NodeBound = pNode->NodeBound;

			Vector3& aabbMin = NodeBound.getMinimum();
			Vector3& aabbMax = NodeBound.getMaximum();

			float xSize = Maths::abs <float>(aabbMax.x - aabbMin.x) / 2.0f;
			float ySize = Maths::abs <float>(aabbMax.y - aabbMin.y) / 2.0f;
			float zSize = Maths::abs <float>(aabbMax.z - aabbMin.z) / 2.0f;

			// Check if a node will fit into any of the child nodes
			if((x <= xSize && y <= ySize && z <= zSize) &&
			   (xSize >= m_Description.minimumNodeSize &&
			    ySize >= m_Description.minimumNodeSize &&
				zSize >= m_Description.minimumNodeSize))
			{
				// Create eight child nodes, if they do not exist.
				// If at least one child exists, all of them do.
				if(!pNode->pChildren[0])
				{
					for(uint i = 0; i < 8; ++i)
						pNode->pChildren[i] = new OctTreeNode();

					pNode->pChildren[0]->NodeBound.setAABB(aabbMin, aabbMin + Vector3(xSize, ySize, zSize));
					pNode->pChildren[0]->pParent = pNode;
					pNode->pChildren[0]->pTree = this;

					pNode->pChildren[1]->NodeBound.setAABB(aabbMin + Vector3(xSize, 0.0f, 0.0f), aabbMin + Vector3(2.0f * xSize, ySize, zSize));
					pNode->pChildren[1]->pParent = pNode;
					pNode->pChildren[1]->pTree = this;

					pNode->pChildren[2]->NodeBound.setAABB(aabbMin + Vector3(0.0f, ySize, 0.0f), aabbMin + Vector3(xSize, 2.0f * ySize, zSize));
					pNode->pChildren[2]->pParent = pNode;
					pNode->pChildren[2]->pTree = this;

					pNode->pChildren[3]->NodeBound.setAABB(aabbMin + Vector3(0.0f, 0.0f, zSize), aabbMin + Vector3(xSize, ySize, 2.0f * zSize));
					pNode->pChildren[3]->pParent = pNode;
					pNode->pChildren[3]->pTree = this;

					pNode->pChildren[4]->NodeBound.setAABB(aabbMin + Vector3(xSize, ySize, 0.0f), aabbMin + Vector3(2.0f * xSize, 2.0f * ySize, zSize));
					pNode->pChildren[4]->pParent = pNode;
					pNode->pChildren[4]->pTree = this;

					pNode->pChildren[5]->NodeBound.setAABB(aabbMin + Vector3(xSize, 0.0f, zSize), aabbMin + Vector3(2.0f * xSize, ySize, 2.0f * zSize));
					pNode->pChildren[5]->pParent = pNode;
					pNode->pChildren[5]->pTree = this;

					pNode->pChildren[6]->NodeBound.setAABB(aabbMin + Vector3(0.0f, ySize, zSize), aabbMin + Vector3(xSize, 2.0f * ySize, 2.0f * zSize));
					pNode->pChildren[6]->pParent = pNode;
					pNode->pChildren[6]->pTree = this;

					pNode->pChildren[7]->NodeBound.setAABB(aabbMin + Vector3(xSize, ySize, zSize), aabbMax);
					pNode->pChildren[7]->pParent = pNode;
					pNode->pChildren[7]->pTree = this;
				}


				bool value = false;
				for(uint i = 0; i < 8; ++i)
				{
					if(pNode->pChildren[i]->fits(*pVolume))
					{
						pNode = pNode->pChildren[i];
						value = true;

						break;
					}
				}
				
				if(!value)
				{
					pNode->vNodes.push_back(_node);
					pNode = 0;
					++m_nNodesCount;
				}
			}
			else
			{
				pNode->vNodes.push_back(_node);
				pNode = 0;
				++m_nNodesCount;
			}
		}
	}
//----------------------------------------------------------------------
	void ScenePartitionerOctTree::removeNode(ICullable* _node)
	{
		OctTreeNode* pNode = m_RootNode.find(_node);
		if(pNode)
		{
			uint index = 0;
			for(uint i = 0; i < pNode->vNodes.size(); ++i)
			{
				if(_node == pNode->vNodes[i])
				{
					index = i;
					break;
				}
			}

			pNode->vNodes.erase(pNode->vNodes.begin() + index);
			--m_nNodesCount;
		}
	}
//----------------------------------------------------------------------
	void ScenePartitionerOctTree::intersect(Ray& _ray, bool _perTriangle)
	{
		m_vIntersections.clear();

		m_RootNode.intersect(_ray, _perTriangle);
	}
//----------------------------------------------------------------------
	void ScenePartitionerOctTree::update()
	{
		OctTreeNode* pNode = &m_RootNode;

		pNode->updateHierarchy();
	}
//----------------------------------------------------------------------
	void ScenePartitionerOctTree::testSceneVisibility()
	{
		OctTreeNode* pNode = &m_RootNode;

		if(m_pFrustum)
		{
			m_nNodesVisibleCount = 0;
			pNode->update(m_pFrustum);
		}
	}
//----------------------------------------------------------------------
	ICullable* ScenePartitionerOctTree::getClosest()
	{
		float tmin = 1e9;
		uint index = m_vIntersections.size();
		for(uint i = 0; i < m_vIntersections.size(); ++i)
		{
			if(m_vIntersections[i].second < tmin)
			{
				tmin = m_vIntersections[i].second;
				index = i;
			}
		}

		if(index != m_vIntersections.size())
			return m_vIntersections[index].first;
		return NULL;
	}
//----------------------------------------------------------------------
	void ScenePartitionerOctTree::intersects(ICullable* _cullable, float _t)
	{
		m_vIntersections.push_back(make_pair(_cullable, _t));
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	SOctTreeNode::SOctTreeNode()
	{
		for(uint i = 0; i < 8; ++i)
			pChildren[i] = NULL;
	}
//----------------------------------------------------------------------
	void SOctTreeNode::update(Frustum* _frustum, CULLING_RESULT _cull)
	{
		Frustum& frustum = *_frustum;

		OctTreeNode* pNode = this;

		if(_cull == CR_UNKNOWN || _cull == CR_INTERSECT)
		{
			CULLING_RESULT result = frustum.testAABB(NodeBound);
			pNode->cull = result;
		}
		else
			pNode->cull = _cull;

		if(pNode->vNodes.size() > 0)
		{
			vector <ICullable*>::iterator iter;
			if(pNode->cull == CR_INVISIBLE)
			{
				for(iter = vNodes.begin(); iter != vNodes.end(); ++iter)
				{
					(*iter)->setCulled(true);
				}
			}
			else if(pNode->cull == CR_VISIBLE)
			{
				for(iter = vNodes.begin(); iter != vNodes.end(); ++iter)
				{
					ICullable* pCullable = (*iter);
					if(pCullable->isEnabled())
					{
						pTree->increaseNodeVisibleCount();
						pCullable->setCulled(false);
					}
				}
			}
			// If a node is only partially visible all contained nodes have to be tested
			else
			{
				for(iter = vNodes.begin(); iter != vNodes.end(); ++iter)
				{
					ICullable* pCullable = (*iter);
					if(!pCullable->isEnabled())
						continue;

					AABB& volume = (*pCullable->getBoundingBox());
					CULLING_RESULT result = frustum.testAABB(volume);

					if(result == CR_VISIBLE || result == CR_INTERSECT)
					{
						pTree->increaseNodeVisibleCount();
						pCullable->setCulled(false);
					}
					else
					{
						pCullable->setCulled(true);
					}
				}
			}
		}

		// Update child nodes recursively (if existent)
		if(pNode->pChildren[0])
		{
			for(uint i = 0; i < 8; ++i)
				pNode->pChildren[i]->update(_frustum, pNode->cull);
		}
	}
//----------------------------------------------------------------------
	void SOctTreeNode::updateHierarchy()
	{
		// Look up nodes requiring fixing
		queue <ICullable*> nodes;
		int count = vNodes.size();
		for(int i = 0; i < count; ++i)
		{
			if(vNodes[i]->hasChanged())
			{
				nodes.push(vNodes[i]);
				vNodes.erase(vNodes.begin() + i);
				--count;
				--i;
			}
		}

		while(nodes.size())
		{
			ICullable* pNode = nodes.front();
			pNode->updateBoundingBox();
			
			pTree->addNode(pNode);
			pNode->setChanged(false);

			nodes.pop();
		}

		// Update child nodes recursively (if existent)
		if(pChildren[0])
		{
			for(uint i = 0; i < 8; ++i)
				pChildren[i]->updateHierarchy();
		}
	}
//----------------------------------------------------------------------
	SOctTreeNode* SOctTreeNode::find(ICullable* _node)
	{
		for(uint i = 0; i < vNodes.size(); ++i)
		{
			if(vNodes[i] == _node)
				return this;
		}

		if(pChildren[0])
		{
			SOctTreeNode* pResult = NULL;

			for(uint i = 0; i < 8; ++i)
			{
				if((pResult = pChildren[i]->find(_node))) return pResult;
			}
		}

		return NULL;
	}
//----------------------------------------------------------------------
	void SOctTreeNode::intersect(Ray& _ray, bool _perTriangle)
	{
		// Test ICullables first
		for(uint i = 0; i < vNodes.size(); ++i)
		{
			ICullable* pNode = vNodes[i];
			if(!pNode->isPickable())
				continue;

			AABB& box = *pNode->getBoundingBox();
			Ray ray(_ray.getOrigin(), _ray.getDirection());
			bool intersects = ray.intersectAABB(box);
			if(intersects)
			{
				if(_perTriangle)
				{
					if(!pNode->tryToPick(ray))
						continue;
				}

				float tmin = ray.getTValue();
				if(Maths::floatCompare(tmin, 0.0f) == 1)
				{
					pTree->intersects(pNode, tmin);
				}
			}
		}

		if(pChildren[0])
		{
			AABB box;
			for(uint i = 0; i < 8; ++i)
				intersectChild(pChildren[i], _ray, box, _perTriangle);
		}
	}
//----------------------------------------------------------------------
	void SOctTreeNode::intersectChild(SOctTreeNode* _child, Ray& _ray, AABB& _box, bool _perTriangle)
	{
		_box.setAABB(_child->NodeBound.getMinimum(),
					 _child->NodeBound.getMaximum());
		bool intersects = _ray.intersectAABB(_box);
		if(intersects)
		{
			_child->intersect(_ray, _perTriangle);
		}
	}
//----------------------------------------------------------------------
	bool SOctTreeNode::fits(AABB& _aabb)
	{
		return NodeBound.fits(_aabb);
	}
//----------------------------------------------------------------------
	void SOctTreeNode::cleanup()
	{
		// Remove all nodes
		vNodes.clear();

		if(pChildren[0])
		{
			for(uint i = 0; i < 8; ++i)
			{
				pChildren[i]->cleanup();
				NGENE_DELETE(pChildren[i]);
			}
		}
	}
//----------------------------------------------------------------------
}