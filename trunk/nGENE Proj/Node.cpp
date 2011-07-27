/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Node.cpp
Version:	0.21
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Node.h"
#include "AABB.h"
#include "IAnimationController.h"
#include "ISerializer.h"
#include "OBB.h"
#include "TaskManager.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo Node::Type(L"Node", &BaseClass::Type);


	Node::Node():
		m_pParent(NULL),
		m_pNextSibling(NULL),
		m_pPrevSibling(NULL),
		m_pChildren(NULL),
		m_pCreator(NULL),
		m_nChildrenNum(0),
		m_vecPosition(Vector3::ZERO_VECTOR),
		m_vecScale(Vector3::ONE_VECTOR),
		m_UpVector(UV_Y),
		m_bEnabled(true),
		m_bHasChanged(true)
	{
	}
//----------------------------------------------------------------------
	Node::~Node()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void Node::init()
	{
	}
//----------------------------------------------------------------------
	void Node::cleanup()
	{
		// Stop animation threads
		for(uint i = 0; i < m_vControllers.size(); ++i)
			m_vControllers[i]->stopAnimation();

		// Delete all children nodes
		if(m_pChildren)
		{
			Node* pNode = m_pChildren;
			while(pNode)
			{
				Node* pTemp = pNode;
				pNode = pNode->getNextSibling();

				// If dynamic memory allocation was used, free it
				if(pTemp->m_bIsAllocatedOnHeap)
				{
					NGENE_DELETE(pTemp);
				}
				else
					pTemp->cleanup();
			}
		}

		m_nChildrenNum = 0;
		m_pCreator = NULL;
		m_pChildren = NULL;
		m_pPrevSibling = NULL;
		m_pNextSibling = NULL;
		m_pParent = NULL;
	}
//----------------------------------------------------------------------
	void Node::removeChild(Node* _child)
	{
		// Check if it is really our child
		Node* pNode = m_pChildren;
		bool bOurChild = false;
		while(pNode && !bOurChild)
		{
			if(pNode == _child)
				bOurChild = true;
			else
				pNode = pNode->getNextSibling();
		}

		// No... not really
		if(!bOurChild)
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Node cannot be removed. Not a child");
			return;
		}

		// Remove all children
		Node* pNodeChild = _child->m_pChildren;
		while(pNodeChild)
		{
			Node* pTemp = pNodeChild;
			pNodeChild = pNodeChild->getNextSibling();
			_child->removeChild(pTemp);
		}

		pNode->m_pCreator->removeFromSceneGraph(pNode->m_stName);

		// Change node hierarchy
		if(pNode->m_pPrevSibling)
			pNode->m_pPrevSibling->m_pNextSibling = pNode->m_pNextSibling;
		if(pNode->m_pNextSibling)
			pNode->m_pNextSibling->m_pPrevSibling = pNode->m_pPrevSibling;

		if(pNode == m_pChildren)
			m_pChildren = pNode->m_pNextSibling;

		// Free memory
		removeSubtask(pNode);
		TaskManager::getSingleton().removeTask(pNode);
		if(pNode->m_bIsAllocatedOnHeap)
		{
			NGENE_DELETE(pNode);
		}
		else
			pNode->cleanup();

		if(!--m_nChildrenNum)
			m_pChildren = NULL;
	}
//----------------------------------------------------------------------
	void Node::update()
	{
		// If node is disabled, return
		if(!m_bEnabled)
			return;

		// First see if controllers updated us...
		for(uint i = 0; i < m_vControllers.size(); ++i)
			m_vControllers[i]->retrieveData();

		// If parent has changed, we have for sure changed as well
		if(m_pParent)
		{
			if(m_pParent->hasChanged())
				m_bHasChanged = true;
		}

		// Apply transformations
		if(m_bHasChanged)
			applyTransform();

		onUpdate();
	}
//----------------------------------------------------------------------
	void Node::onEndUpdate()
	{
		if(m_bHasChanged)
			m_bHasChanged = false;
	}
//----------------------------------------------------------------------
	void Node::onUpdate()
	{
		// Do nothing...
	}
//----------------------------------------------------------------------
	void Node::initDependencies(Task* _task)
	{
		addSubtask(_task);
	}
//----------------------------------------------------------------------
	void Node::addChild(const wstring& _name, Node& _child)
	{
		// Error, node cannot be added
		if(!_child.getCreator()->addToSceneGraph(_name, &_child))
			return;

		Node* temp = m_pChildren;
		m_pChildren = &_child;

		_child.setNextSibling(temp);
		_child.setParent(this);
		_child.applyTransform();
		_child.m_bIsAllocatedOnHeap = false;

		if(temp)
			temp->setPrevSibling(m_pChildren);

		++m_nChildrenNum;


		initDependencies(&_child);
	}
//----------------------------------------------------------------------
	void Node::addChild(const wstring& _name, Node* _child)
	{
		// Error, node cannot be added
		if(!_child->getCreator()->addToSceneGraph(_name, _child))
			return;

		Node* temp = m_pChildren;
		m_pChildren = _child;

		_child->setNextSibling(temp);
		_child->setParent(this);
		_child->applyTransform();
		_child->m_bIsAllocatedOnHeap = true;

		if(temp)
			temp->setPrevSibling(m_pChildren);

		++m_nChildrenNum;


		initDependencies(_child);
	}
//----------------------------------------------------------------------
	void Node::translate(const nGENE::Vector3& _translate)
	{
		m_vecPosition.add(_translate);

		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void Node::translate(Real _x, Real _y, Real _z)
	{
		m_vecPosition.add(Vector3(_x, _y, _z));

		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void Node::setPositionWorld(const Vector3& _pos)
	{
		Matrix4x4 matTransform = m_matTransform;
		Vector3 parentPos;
		if(m_pParent)
			parentPos = m_pParent->getPositionWorld();
		matTransform.setTranslation(parentPos);
		matTransform.invert();
		Vector4 pos(_pos, 1.0f);
		pos = pos * matTransform;

		setPosition(pos.x, pos.y, pos.z);
	}
//----------------------------------------------------------------------
	void Node::setPositionWorld(Real _x, Real _y, Real _z)
	{
		Vector3 parentPos;

		if(m_pParent)
			parentPos = m_pParent->getPositionWorld();

		Matrix4x4 matTransform = m_matTransform;
		matTransform.setTranslation(parentPos);
		matTransform.invert();
		Vector4 pos(_x, _y, _z, 1.0f);
		pos = pos * matTransform;

		setPosition(pos.x, pos.y, pos.z);
	}
//----------------------------------------------------------------------
	Vector3 Node::getPositionWorld()
	{
		return Vector3(m_matTransform.getTranslation());
	}
//----------------------------------------------------------------------
	Quaternion Node::getRotationWorld()
	{
		Quaternion quatParent;
		if(m_pParent)
			quatParent = m_pParent->getRotation();

		return (getRotation() * quatParent);
	}
//----------------------------------------------------------------------
	void Node::applyTransform()
	{
		// Get the parental node's matrix
		Matrix4x4 matParent;
		if(m_pParent)
			matParent = m_pParent->getTransform();
			
		Matrix4x4 matTrans;
		Matrix4x4 matScale;
		Matrix4x4 matRot;

		matTrans.translate(m_vecPosition);
		matRot = m_quatOrientation.toMatrix();
		matScale.scale(m_vecScale);

		// Combine transformations
		matScale.multiply(matRot);
		matScale.multiply(matTrans);

		m_matTransform = multiply(matScale, matParent);
	}
//----------------------------------------------------------------------
	void Node::setRotation(const Vector3& _rot)
	{
		m_vecRotation = _rot;
		Matrix4x4 matRot;
		matRot.rotateYawPitchRoll(_rot);

		m_quatOrientation.fromMatrix(matRot);
	}
//----------------------------------------------------------------------
	void Node::setRotation(Real _yaw, Real _pitch, Real _roll)
	{
		m_vecRotation.set(_pitch, _yaw, _roll);
		Matrix4x4 matRot;
		matRot.rotateYawPitchRoll(_yaw, _pitch, _roll);

		m_quatOrientation.fromMatrix(matRot);
	}
//----------------------------------------------------------------------
	void Node::setRotation(const Vector3& _axis, const Real _angle)
	{
		m_quatOrientation.set(_axis, _angle);
		m_quatOrientation.normalize();

		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void Node::setRotation(const Quaternion& _quat)
	{
		m_quatOrientation = _quat;
		m_quatOrientation.normalize();

		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void Node::setPosition(const Vector3& _pos)
	{
		m_vecPosition = _pos;

		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void Node::setScale(Real _x, Real _y, Real _z)
	{
		m_vecScale.set(_x, _y, _z);

		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void Node::setScale(const Vector3& _scale)
	{
		m_vecScale = _scale;

		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void Node::setScale(Real _scale)
	{
		m_vecScale.set(_scale, _scale, _scale);

		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void Node::setPosition(Real _x, Real _y, Real _z)
	{
		m_vecPosition = Vector3(_x, _y, _z);

		m_bHasChanged = true;
	}
//----------------------------------------------------------------------
	void Node::addToPartitioner(ScenePartitioner* _partitioner)
	{
	}
//----------------------------------------------------------------------
	void Node::removeFromPartitioner(ScenePartitioner* _partitioner)
	{
	}
//----------------------------------------------------------------------
	void Node::serialize(ISerializer* _serializer,
						 bool _serializeType,
						 bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		if(_serializeType) _serializer->addObject(this->Type);
			
			Property <wstring> prName(m_stName);
			_serializer->addProperty("Name", prName);

			Property <Vector3> prPosition(m_vecPosition);
			_serializer->addProperty("Position", prPosition);
			Property <Vector3> prRotation(m_vecRotation);
			_serializer->addProperty("Rotation", prRotation);
			Property <Vector3> prScale(m_vecScale);
			_serializer->addProperty("Scale", prScale);

			Property <Quaternion> prOrientation(m_quatOrientation);
			_serializer->addProperty("Orientation", prOrientation);

			Property <bool> prEnabled(m_bEnabled);
			_serializer->addProperty("Enabled", prEnabled);

			wstring upVector = (m_UpVector == UV_Y ? L"Y" : L"Z");
			Property <wstring> prUpVector(upVector);
			_serializer->addProperty("Up", prUpVector);

			// Serialize children
			if(_serializeChildren)
			{
				Node* pNodeChild = m_pChildren;
				while(pNodeChild)
				{
					pNodeChild->serialize(_serializer, true, _serializeChildren);
					pNodeChild = pNodeChild->getNextSibling();
				}
			}

		if(_serializeType) _serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void Node::deserialize(ISerializer* _serializer)
	{
		Property <wstring> prName(m_stName);
		_serializer->getProperty("Name", prName);

		Property <Vector3> prPosition(m_vecPosition);
		_serializer->getProperty("Position", prPosition);
		Property <Vector3> prRotation(m_vecRotation);
		_serializer->getProperty("Rotation", prRotation);
		Property <Vector3> prScale(m_vecScale);
		_serializer->getProperty("Scale", prScale);

		Property <Quaternion> prOrientation(m_quatOrientation);
		_serializer->getProperty("Orientation", prOrientation);

		Property <bool> prEnabled(m_bEnabled);
		_serializer->getProperty("Enabled", prEnabled);

		wstring upVector;
		Property <wstring> prUpVector(upVector);
		_serializer->getProperty("Up", prUpVector);
		if(upVector == L"Z")
			m_UpVector = UV_Z;

		m_bHasChanged = true;

		setCreator(_serializer->getSceneManager());

		init();

		// Add node to the scene manager
		if(_serializer->getParent())
			((Node*)_serializer->getParent())->addChild(getName(), this);
		else
			m_pCreator->init(this);
	}
//----------------------------------------------------------------------
	void Node::addAnimationController(IAnimationController* _controller)
	{
		_controller->setNode(this);
		_controller->init();
		
		addSubtask(_controller);

		m_vControllers.push_back(_controller);
	}
//----------------------------------------------------------------------
	void Node::removeAnimationController(uint _index)
	{
		Assert(_index < m_vControllers.size(), "Animation controller index out of bounds");

		m_vControllers[_index]->stopAnimation();
		removeSubtask(m_vControllers[_index]);

		m_vControllers.erase(m_vControllers.begin() + _index);
	}
//----------------------------------------------------------------------
	void Node::removeAnimationController(IAnimationController* _controller)
	{
		/// Check if the controller belongs to this Node
		uint i = 0;
		for(i = 0; i < m_vControllers.size(); ++i)
		{
			if(m_vControllers[i] == _controller)
				break;
		}

		if(i != m_vControllers.size())
		{
			m_vControllers[i]->stopAnimation();
			removeChildTask(m_vControllers[i]);

			m_vControllers.erase(m_vControllers.begin() + i);
		}
	}
//----------------------------------------------------------------------
	IAnimationController* Node::getAnimationController(uint _index) const
	{
		Assert(_index < m_vControllers.size(), "Animation controller index out of bounds");

		return m_vControllers[_index];
	}
//----------------------------------------------------------------------
	bool Node::isEnabled() const
	{
		if(!m_bEnabled)
			return false;

		// Check if this node should be REALLY enabled...
		if(m_pParent)
			return m_pParent->isEnabled();
		else
			return true;
	}
//----------------------------------------------------------------------
}