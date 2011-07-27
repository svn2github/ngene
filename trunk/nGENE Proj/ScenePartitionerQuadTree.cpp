/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		ScenePartitionerQuadTree.cpp
Version:	0.12
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "AABB.h"
#include "Frustum.h"
#include "ICullable.h"
#include "Node.h"
#include "Ray.h"
#include "ScenePartitionerQuadTree.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo ScenePartitionerQuadTree::Type(L"ScenePartitionerQuadTree", &ScenePartitioner::Type);

	const float SQuadTreeNode::NodeY = 10000.0f;


	ScenePartitionerQuadTree::ScenePartitionerQuadTree()
	{
		m_Description.bounds.left = -1000.0f;
		m_Description.bounds.right = 1000.0f;
		m_Description.bounds.top = -1000.0f;
		m_Description.bounds.bottom = 1000.0f;
		m_Description.minimumNodeSize = 10.0f;

		init();
	}
//----------------------------------------------------------------------
	ScenePartitionerQuadTree::ScenePartitionerQuadTree(const QUAD_TREE_DESC& _desc):
		m_Description(_desc)
	{
		init();
	}
//----------------------------------------------------------------------
	ScenePartitionerQuadTree::~ScenePartitionerQuadTree()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void ScenePartitionerQuadTree::init()
	{
		m_RootNode.NodeBound.left = m_Description.bounds.left;
		m_RootNode.NodeBound.right = m_Description.bounds.right;
		m_RootNode.NodeBound.top = m_Description.bounds.top;
		m_RootNode.NodeBound.bottom = m_Description.bounds.bottom;
		m_RootNode.pParent = NULL;
		m_RootNode.pTree = this;
	}
//----------------------------------------------------------------------
	void ScenePartitionerQuadTree::cleanup()
	{
		m_RootNode.cleanup();
	}
//----------------------------------------------------------------------
	void ScenePartitionerQuadTree::addNode(ICullable* _node)
	{
		AABB* pVolume = _node->getBoundingBox();

		float x = Maths::abs <float> (pVolume->getMaximum().x - pVolume->getMinimum().x);
		float z = Maths::abs <float> (pVolume->getMaximum().z - pVolume->getMinimum().z);

		// Check if both x and z radii are correct. Otherwise - exit.
		if(!Maths::floatCompare(x, 0.0f) ||
		   !Maths::floatCompare(z, 0.0f))
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__,
				__LINE__, L"None of the node's bounding volumes dimensions can be 0");
			return;
		}

		QuadTreeNode* pNode = &m_RootNode;

		Vector3& min = pVolume->getMinimum();
		Vector3& max = pVolume->getMaximum();


		while(pNode)
		{
			RECT_FLOAT& NodeBound = pNode->NodeBound;

			float xSize = Maths::abs <float>(NodeBound.left - NodeBound.right) / 2.0f;
			float zSize = Maths::abs <float>(NodeBound.top - NodeBound.bottom) / 2.0f;

			// Check if a node will fit into any of the child nodes
			if((x <= xSize && z <= zSize) &&
			   (xSize >= m_Description.minimumNodeSize && zSize >= m_Description.minimumNodeSize))
			{
				// Create four child nodes, if they do not exist.
				// If at least one child exists, four of them do.
				if(!pNode->pTopLeft)
				{
					pNode->pTopLeft = new QuadTreeNode();
					pNode->pTopRight = new QuadTreeNode();
					pNode->pBottomLeft = new QuadTreeNode();
					pNode->pBottomRight = new QuadTreeNode();

					pNode->pTopLeft->NodeBound = NodeBound;
					pNode->pTopLeft->NodeBound.bottom = NodeBound.top + zSize;
					pNode->pTopLeft->NodeBound.right = NodeBound.left + xSize;
					pNode->pTopLeft->pParent = pNode;
					pNode->pTopLeft->pTree = this;

					pNode->pTopRight->NodeBound = NodeBound;
					pNode->pTopRight->NodeBound.bottom = NodeBound.top + zSize;
					pNode->pTopRight->NodeBound.left = NodeBound.left + xSize;
					pNode->pTopRight->pParent = pNode;
					pNode->pTopRight->pTree = this;

					pNode->pBottomLeft->NodeBound = NodeBound;
					pNode->pBottomLeft->NodeBound.top = NodeBound.top + zSize;
					pNode->pBottomLeft->NodeBound.right = NodeBound.left + xSize;
					pNode->pBottomLeft->pParent = pNode;
					pNode->pBottomLeft->pTree = this;

					pNode->pBottomRight->NodeBound = NodeBound;
					pNode->pBottomRight->NodeBound.top = NodeBound.top + zSize;
					pNode->pBottomRight->NodeBound.left = NodeBound.left + xSize;
					pNode->pBottomRight->pParent = pNode;
					pNode->pBottomRight->pTree = this;
				}


				// Top-left
				if(min.x >= pNode->pTopLeft->NodeBound.left && max.x <= pNode->pTopLeft->NodeBound.right &&
				   min.z >= pNode->pTopLeft->NodeBound.top && max.z <= pNode->pTopLeft->NodeBound.bottom)
				{
					pNode = pNode->pTopLeft;
				}
				// Top-right
				else if(min.x >= pNode->pTopRight->NodeBound.left && max.x <= pNode->pTopRight->NodeBound.right &&
				   min.z >= pNode->pTopRight->NodeBound.top && max.z <= pNode->pTopRight->NodeBound.bottom)
				{
					pNode = pNode->pTopRight;
				}
				// Bottom-left
				else if(min.x >= pNode->pBottomLeft->NodeBound.left && max.x <= pNode->pBottomLeft->NodeBound.right &&
				   min.z >= pNode->pBottomLeft->NodeBound.top && max.z <= pNode->pBottomLeft->NodeBound.bottom)
				{
					pNode = pNode->pBottomLeft;
				}
				// Bottom-right
				else if(min.x >= pNode->pBottomRight->NodeBound.left && max.x <= pNode->pBottomRight->NodeBound.right &&
				   min.z >= pNode->pBottomRight->NodeBound.top && max.z <= pNode->pBottomRight->NodeBound.bottom)
				{
					pNode = pNode->pBottomRight;
				}
				else
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
	void ScenePartitionerQuadTree::removeNode(ICullable* _node)
	{
		QuadTreeNode* pNode = m_RootNode.find(_node);
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
	void ScenePartitionerQuadTree::intersect(Ray& _ray, bool _perTriangle)
	{
		m_vIntersections.clear();

		m_RootNode.intersect(_ray, _perTriangle);
	}
//----------------------------------------------------------------------
	void ScenePartitionerQuadTree::update()
	{
		QuadTreeNode* pNode = &m_RootNode;

		pNode->updateHierarchy();
	}
//----------------------------------------------------------------------
	void ScenePartitionerQuadTree::testSceneVisibility()
	{
		QuadTreeNode* pNode = &m_RootNode;

		if(m_pFrustum)
		{
			m_nNodesVisibleCount = 0;
			pNode->update(m_pFrustum);
		}
	}
//----------------------------------------------------------------------
	ICullable* ScenePartitionerQuadTree::getClosest()
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
	void ScenePartitionerQuadTree::intersects(ICullable* _cullable, float _t)
	{
		m_vIntersections.push_back(make_pair(_cullable, _t));
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	SQuadTreeNode::SQuadTreeNode():
		pParent(NULL),
		pTopLeft(NULL),
		pTopRight(NULL),
		pBottomLeft(NULL),
		pBottomRight(NULL)
	{
	}
//----------------------------------------------------------------------
	void SQuadTreeNode::update(Frustum* _frustum, CULLING_RESULT _cull)
	{
		Frustum& frustum = *_frustum;

		QuadTreeNode* pNode = this;

		if(_cull == CR_UNKNOWN || _cull == CR_INTERSECT)
		{
			Vector3 vecMin = Vector3(pNode->NodeBound.left, -NodeY, pNode->NodeBound.bottom);
			Vector3 vecMax = Vector3(pNode->NodeBound.right, NodeY, pNode->NodeBound.top);

			AABB aabb(vecMin, vecMax);
			CULLING_RESULT result = frustum.testAABB(aabb);
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
		if(pNode->pTopLeft)
		{
			pNode->pTopLeft->update(_frustum, pNode->cull);
			pNode->pTopRight->update(_frustum, pNode->cull);
			pNode->pBottomLeft->update(_frustum, pNode->cull);
			pNode->pBottomRight->update(_frustum, pNode->cull);
		}
	}
//----------------------------------------------------------------------
	void SQuadTreeNode::updateHierarchy()
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
		if(pTopLeft)
		{
			pTopLeft->updateHierarchy();
			pTopRight->updateHierarchy();
			pBottomLeft->updateHierarchy();
			pBottomRight->updateHierarchy();
		}
	}
//----------------------------------------------------------------------
	SQuadTreeNode* SQuadTreeNode::find(ICullable* _node)
	{
		for(uint i = 0; i < vNodes.size(); ++i)
		{
			if(vNodes[i] == _node)
				return this;
		}

		if(pTopLeft)
		{
			SQuadTreeNode* pResult = NULL;

			if((pResult = pTopLeft->find(_node))) return pResult;
			if((pResult = pTopRight->find(_node))) return pResult;
			if((pResult = pBottomLeft->find(_node))) return pResult;
			if((pResult = pBottomRight->find(_node))) return pResult;
		}

		return NULL;
	}
//----------------------------------------------------------------------
	void SQuadTreeNode::intersect(Ray& _ray, bool _perTriangle)
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

		if(pTopLeft)
		{
			// Top left
			AABB box;
			intersectChild(pTopLeft, _ray, box, _perTriangle);

			// Top right
			intersectChild(pTopRight, _ray, box, _perTriangle);

			// Bottom left
			intersectChild(pBottomLeft, _ray, box, _perTriangle);

			// Bottom right
			intersectChild(pBottomRight, _ray, box, _perTriangle);
		}
	}
//----------------------------------------------------------------------
	void SQuadTreeNode::intersectChild(SQuadTreeNode* _child, Ray& _ray, AABB& _box, bool _perTriangle)
	{
		_box.setAABB(Vector3(_child->NodeBound.left, -NodeY, _child->NodeBound.top),
					 Vector3(_child->NodeBound.right, NodeY, _child->NodeBound.bottom));
		bool intersects = _ray.intersectAABB(_box);
		if(intersects)
		{
			_child->intersect(_ray, _perTriangle);
		}
	}
//----------------------------------------------------------------------
	void SQuadTreeNode::cleanup()
	{
		// Remove all nodes
		vNodes.clear();

		if(pTopLeft)
		{
			pTopLeft->cleanup();
			NGENE_DELETE(pTopLeft);

			pTopRight->cleanup();
			NGENE_DELETE(pTopRight);

			pBottomLeft->cleanup();
			NGENE_DELETE(pBottomLeft);

			pBottomRight->cleanup();
			NGENE_DELETE(pBottomRight);
		}
	}
//----------------------------------------------------------------------
}