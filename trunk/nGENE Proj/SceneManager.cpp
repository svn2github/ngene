/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		SceneManager.cpp
Version:	0.24
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "SceneManager.h"
#include "Billboard.h"
#include "CameraEnumerator.h"
#include "NodeLight.h"
#include "NodeParticleSystem.h"
#include "NodeSound.h"
#include "NodeTerrain.h"
#include "PrefabArrow.h"
#include "PrefabAxisRods.h"
#include "PrefabBox.h"
#include "PrefabCapsule.h"
#include "PrefabCylinder.h"
#include "PrefabPlane.h"
#include "PrefabPyramid.h"
#include "PrefabSphere.h"
#include "PrefabTeapot.h"
#include "PrefabTorus.h"
#include "ScenePartitioner.h"


namespace nGENE
{
	// Initialize static members
	TypeInfo SceneManager::Type(L"SceneManager", &BaseClass::Type);


	SceneManager::SceneManager():
		m_pCameraEnumerator(new CameraEnumerator()),
		m_pRootNode(NULL),
		m_pPartitioner(NULL)
	{
		init();
	}
//----------------------------------------------------------------------
	SceneManager::~SceneManager()
	{
		cleanup();
	}
//----------------------------------------------------------------------
	void SceneManager::init()
	{
		// Root node has to be created, otherwise nothing could be rendered
		m_pRootNode = new Node();
		m_pRootNode->setName(L"SceneRoot");
		m_pRootNode->setCreator(this);

		addSubtask(m_pRootNode);
		if(m_pPartitioner)
			m_pPartitioner->addDependency(m_pRootNode);
	}
//----------------------------------------------------------------------
	void SceneManager::init(Node* _root)
	{
		// Root node has to be created, otherwise nothing could be rendered
		m_pRootNode = _root;

		if(m_pPartitioner)
			m_pPartitioner->init();

		addSubtask(m_pRootNode);
		if(m_pPartitioner)
			m_pPartitioner->addDependency(m_pRootNode);
	}
//----------------------------------------------------------------------
	void SceneManager::cleanup()
	{
		Renderer::getSingleton().removeLights();

		if(m_pRootNode)
		{
			if(m_pPartitioner)
				m_pRootNode->removeChildTask(m_pPartitioner);
			removeSubtask(m_pRootNode);
		}

		NGENE_DELETE(m_pRootNode);

		m_pSceneNodes.clear();
		m_pCameras.clear();

		if(m_pPartitioner)
			m_pPartitioner->cleanup();
	}
//----------------------------------------------------------------------
	void SceneManager::update()
	{
		// Do nothing at present - necessary for task management
	}
//----------------------------------------------------------------------
	Camera* SceneManager::createCamera(const wstring& _typename, const wstring& _key)
	{
		m_pCameras[_key] = m_pCameraEnumerator->create(_typename);
		m_pCameras[_key]->setCreator(this);

		return m_pCameras[_key];
	}
//----------------------------------------------------------------------
	NodeVisible* SceneManager::createNodeVisible()
	{
		NodeVisible* result = new NodeVisible();
		result->setCreator(this);
		result->init();

		return result;
	}
//----------------------------------------------------------------------
	NodeLight* SceneManager::createLight(const LIGHT_TYPE& _lightType)
	{
		NodeLight* result = new NodeLight(_lightType);
		result->setCreator(this);
		result->init();

		return result;
	}
//----------------------------------------------------------------------
	NodeTerrain* SceneManager::createTerrain(const TERRAIN_DESC& _desc)
	{
		NodeTerrain* result = new NodeTerrain(_desc);
		result->setCreator(this);
		result->init();

		return result;
	}
//----------------------------------------------------------------------
	NodeParticleSystem* SceneManager::createParticleSystem()
	{
		NodeParticleSystem* result = new NodeParticleSystem();
		result->setCreator(this);
		result->init();

		return result;
	}
//----------------------------------------------------------------------
	NodeSound* SceneManager::createSound(const wstring& _name, const SOUND_DESC& _desc)
	{
		NodeSound* result = new NodeSound(_name, _desc);
		result->setCreator(this);
		result->init();

		return result;
	}
//----------------------------------------------------------------------
	PrefabTeapot* SceneManager::createTeapot()
	{
		PrefabTeapot* result = new PrefabTeapot();
		result->setCreator(this);
		result->init();

		return result;
	}
//----------------------------------------------------------------------
	PrefabPlane* SceneManager::createPlane()
	{
		PrefabPlane* result = new PrefabPlane();
		result->setCreator(this);
		result->init();

		return result;
	}
//----------------------------------------------------------------------
	PrefabPlane* SceneManager::createPlane(Plane& _plane, const Vector2& _texCoordMul,
		Real _sideLength)
	{
		PrefabPlane* result = new PrefabPlane(_plane, _sideLength, _texCoordMul);
		result->setCreator(this);
		result->init();

		return result;
	}
//----------------------------------------------------------------------
	PrefabBox* SceneManager::createBox(Real _length, Real _height, Real _width,
		const Vector2& _texCoordMul)
	{
		// Dimensions may require negating
		_length = FastFloat::fabs(_length);
		_height = FastFloat::fabs(_height);
		_width  = FastFloat::fabs(_width);

		// Find maximum and minimum points of AABB
		Vector3 vecMax(_length * 0.5f,
					   _height * 0.5f,
					   _width  * 0.5f);
		Vector3 vecMin = -vecMax;

		AABB aabb(vecMin, vecMax);

		PrefabBox* result = new PrefabBox(aabb, _texCoordMul);
		result->setCreator(this);
		result->init();

		return result;
	}
//----------------------------------------------------------------------
	PrefabBox* SceneManager::createBox(Vector3& _size,
		const Vector2& _texCoordMul)
	{
		// Dimensions may require negating
		_size.x = FastFloat::fabs(_size.x);
		_size.y = FastFloat::fabs(_size.y);
		_size.z = FastFloat::fabs(_size.z);

		// Find maximum and minimum points of AABB
		Vector3 vecMax(_size.x * 0.5f,
					   _size.y * 0.5f,
					   _size.z * 0.5f);
		Vector3 vecMin = -vecMax;

		AABB aabb(vecMin, vecMax);

		PrefabBox* result = new PrefabBox(aabb, _texCoordMul);
		result->setCreator(this);
		result->init();

		return result;
	}
//----------------------------------------------------------------------
	PrefabSphere* SceneManager::createSphere(uint _slices, uint _stacks, Real _radius,
		const Vector2& _texCoordMul, Real _phi)
	{
		PrefabSphere* result = new PrefabSphere(_slices, _stacks, _radius,
			_texCoordMul, _phi);
		result->setCreator(this);
		result->init();

		return result;
	}
//----------------------------------------------------------------------
	PrefabCylinder* SceneManager::createCylinder(uint _slices, uint _stacks, Real _radius,
		Real _height, bool _closed, const Vector2& _texCoordMul)
	{
		PrefabCylinder* result = new PrefabCylinder(_slices, _stacks, _radius,
			_height, _closed, _texCoordMul);
		result->setCreator(this);
		result->init();

		return result;
	}
//----------------------------------------------------------------------
	PrefabCapsule* SceneManager::createCapsule(uint _slices, uint _stacks, Real _radius,
		Real _height, const Vector2& _texCoordMul)
	{
		PrefabCapsule* result = new PrefabCapsule(_slices, _stacks, _height,
			_radius, _texCoordMul);
		result->setCreator(this);
		result->init();

		return result;
	}
//----------------------------------------------------------------------
	PrefabPyramid* SceneManager::createPyramid(Real _height, Real _side, const Vector2& _texCoordMul)
	{
		PrefabPyramid* result = new PrefabPyramid(_height, _side, _texCoordMul);
		result->setCreator(this);
		result->init();

		return result;
	}
//----------------------------------------------------------------------
	PrefabTorus* SceneManager::createTorus(uint _radialSlices, uint _circleSlices, Real _innerRadius,
		Real _outerRadius, const Vector2& _texCoordMul)
	{
		PrefabTorus* result = new PrefabTorus(_radialSlices, _circleSlices, _innerRadius,
			_outerRadius, _texCoordMul);
		result->setCreator(this);
		result->init();

		return result;
	}
//----------------------------------------------------------------------
	PrefabArrow* SceneManager::createArrow(Real _length, Real _endLength)
	{
		PrefabArrow* result = new PrefabArrow(_length, _endLength);
		result->setCreator(this);
		result->init();

		return result;
	}
//----------------------------------------------------------------------
	PrefabAxisRods* SceneManager::createAxisRods(Real _length, Real _endLength)
	{
		PrefabAxisRods* result = new PrefabAxisRods(_length, _endLength);
		result->setCreator(this);
		result->init();

		return result;
	}
//----------------------------------------------------------------------
	NodeBillboardSet* SceneManager::createBillboardSet()
	{
		NodeBillboardSet* result = new NodeBillboardSet();
		result->setCreator(this);
		result->init();

		return result;
	}
//----------------------------------------------------------------------
	Node* SceneManager::getNode(const wstring& _name)
	{
		if(m_pSceneNodes.find(_name) == m_pSceneNodes.end())
		{
			Log::log(LET_ERROR, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Node: %s not found.", _name.c_str());
			return NULL;
		}

		return m_pSceneNodes[_name];
	}
//----------------------------------------------------------------------
	bool SceneManager::addToSceneGraph(const wstring& _name, Node* _node)
	{
		Log::log(LET_EVENT, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
			L"Adding node to the scene: %ls", _name.c_str());

		// Such a node already exists
		if(m_pSceneNodes.find(_name) != m_pSceneNodes.end())
		{
			Log::log(LET_WARNING, L"nGENE", __WFILE__, __WFUNCTION__, __LINE__,
				L"Node with a name: %ls already exists in the scene. Not adding", _name.c_str());
			return false;
		}

		m_pSceneNodes[_name] = _node;
		m_pSceneNodes[_name]->setName(_name);

		// Add node to partitioner
		if(m_pPartitioner)
			_node->addToPartitioner(m_pPartitioner);

		return true;
	}
//----------------------------------------------------------------------
	void SceneManager::removeFromSceneGraph(const wstring& _name)
	{
		if(!getNode(_name))
			return;

		Node* pNode = m_pSceneNodes[_name];

		// Remove node from partitioner
		if(m_pRootNode == pNode)
			removeSubtask(pNode);
		if(m_pPartitioner)
			pNode->removeFromPartitioner(m_pPartitioner);
		m_pSceneNodes.erase(_name);
	}
//----------------------------------------------------------------------
	void SceneManager::setScenePartitioner(ScenePartitioner* _partitioner)
	{
		if(m_pPartitioner)
		{
			if(m_pRootNode)
				m_pRootNode->removeChildTask(m_pPartitioner);
		}

		m_pPartitioner = _partitioner;

		if(m_pRootNode)
			m_pPartitioner->addDependency(m_pRootNode);
	}
//----------------------------------------------------------------------
	void SceneManager::testVisibility()
	{
		if(m_pPartitioner)
			m_pPartitioner->testSceneVisibility();
	}
//----------------------------------------------------------------------
	void SceneManager::serialize(ISerializer* _serializer,
								 bool _serializeType,
								 bool _serializeChildren)
	{
		if(!m_bIsSerializable)
			return;

		_serializer->addObject(this->Type);
			//_serializer->addProperty <String> ("Name", "");//String(m_stName));

			// @todo Serialize partitioner

			// Serialize nodes
			if(m_pRootNode)
				m_pRootNode->serialize(_serializer, true, _serializeChildren);
		_serializer->endObject(this->Type);
	}
//----------------------------------------------------------------------
	void SceneManager::deserialize(ISerializer* _serializer)
	{
		/// @todo Read attributes
		/// ...
		/// ...
	}
//----------------------------------------------------------------------
}
