/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		SceneManager.h
Version:	0.22
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SCENEMANAGER_H_
#define __INC_SCENEMANAGER_H_


#include "AutoPointer.h"
#include "BaseClass.h"
#include "HashTable.h"
#include "ISerializable.h"
#include "Maths.h"
#include "Prerequisities.h"
#include "Task.h"
#include "TypeInfo.h"
#include "Vector2.h"


namespace nGENE
{
	/** Manages scene's geometry in such a way rendering is speeded up.
		@par
			You can create multiple scene managers. That gives you
			opportunity to take advantage of different solutions
			what can be used eg. to seamlessly link in- and outdoor
			scenes.
		@par
			It is essential that the user removes all nodes created,
			on his own. Otherwise it will be taken care by the nGENE
			no sooner than at exit. Besides, deleting a node leads
			to deleting all its children.
	*/
	class nGENEDLL SceneManager: public BaseClass, public ISerializable,
		public Task
	{
		EXPOSE_TYPE
	protected:
		// Enumerators/factories and stuff like that
		AutoPointer <CameraEnumerator> m_pCameraEnumerator;

		/** It is the root of the scene graph of this SceneManager.
			@remarks
				Root node cannot be created or deleted manually.
				However, its attributes are fully modifiable.
		*/
		Node* m_pRootNode;

		/** HashTable containing all nodes in the scene belonging
			to this SceneManager.
		*/
		HashTable <wstring, Node*> m_pSceneNodes;

		/** HashTable containing all cameras in the scene.
			@remarks
				Although only one camera could be marked active
				at the time, you can have multiple ones in your
				scene and switch between them whenever you want.
		*/
		HashTable <wstring, Camera*> m_pCameras;


		ScenePartitioner* m_pPartitioner;		///< Scene partitioner

	public:
		SceneManager();
		virtual ~SceneManager();

		/// Initializes scene manager.
		virtual void init();
		/** Initializes scene manager.
			@param
				_root new root node of the manager. It has to be allocated
				on the heap.
		*/
		virtual void init(Node* _root);

		/// Frees allocated memory, removes all nodes.
		virtual void cleanup();

		/** Creates new camera.
			@remarks
				To create a new camera you have to provide camera's type name
				(several to choose between) and a key it will have in the
				scene-graph.
		*/
		virtual Camera* createCamera(const wstring& _typeName, const wstring& _key);


		/** Updates SceneManager.
		*/
		virtual void update();

		/// Creates new renderable Node.
		NodeVisible* createNodeVisible();

		/// Creates new teapot visualization.
		PrefabTeapot* createTeapot();
		
		/// Creates new plane visualization.
		PrefabPlane* createPlane();
		/// Creates new plane visualization.
		PrefabPlane* createPlane(Plane& _plane, const Vector2& _texCoordMul=Vector2(100.0f, 100.0f),
			Real _sideLength=4000.0f);

		/** Creates new box prefab.
			@remarks
				Box dimensions have to be specified. If they are below
				zero absolute value is automatically taken.
		*/
		PrefabBox* createBox(Real _length=1.0f, Real _height=1.0f, Real _width=1.0f,
			const Vector2& _texCoordMul=Vector2::ONE_VECTOR);
		PrefabBox* createBox(Vector3& _size, const Vector2& _texCoordMul=Vector2::ONE_VECTOR);
		
		/** Creates new sphere prefab.
			@param _slices number of cuts along radius.
			@param _stacks number of cuts along side.
			@param _radius radius of the sphere.
			@param _texCoordMult modifier by which texture coordinates are multiplied.
		*/
		PrefabSphere* createSphere(uint _slices=10, uint _stacks=10, Real _radius=1.0f,
			const Vector2& _texCoordMul=Vector2::ONE_VECTOR,
			Real _phi=Maths::PI);

		/** Creates new cylinder prefab.
			@param _slices number of cuts along radius.
			@param _stacks number of cuts along side.
			@param _radius radius of the cylinder.
			@param _height height of the cylinder.
			@param _closed if set to true cylinder will have top and bottom caps.
			@param _texCoordMult modifier by which texture coordinates are multiplied.
		*/
		PrefabCylinder* createCylinder(uint _slices=10, uint _stacks=10, Real _radius=1.0f,
			Real _height=1.0f, bool _closed=true,
			const Vector2& _texCoordMul=Vector2::ONE_VECTOR);
		
		/// Creates new capsule prefab.
		PrefabCapsule* createCapsule(uint _slices=10, uint _stacks=10, Real _radius=1.0f,
			Real _height=1.0f, const Vector2& _texCoordMul=Vector2::ONE_VECTOR);

		/** Creates new pyramid prefab.
			@param _height height of the pyramid.
			@param _side length of base side.
			@param _texCoordMult modifier by which texture coordinates are multiplied.
		*/
		PrefabPyramid* createPyramid(Real _height=1.0f, Real _side=1.0f,
			const Vector2& _texCoordMul=Vector2::ONE_VECTOR);

		/// Creates new torus prefab.
		PrefabTorus* createTorus(uint _radialSlices=10, uint _circleSlices=15, Real _innerRadius=0.25f,
			Real _outerRadius=1.0f, const Vector2& _texCoordMul=Vector2::ONE_VECTOR);

		/// Creates new arrow node.
		PrefabArrow* createArrow(Real _length=1.0f, Real _endLength=0.1f);

		/// Creates new axis rods.
		PrefabAxisRods* createAxisRods(Real _length=1.0f, Real _endLength=0.1f);

		/// Creates new set of billboards.
		NodeBillboardSet* createBillboardSet();

		/** Creates new light and adds it to the scene.
			@remarks
				As all lights are run in hardware their number is
				not limited at all.
		*/
		NodeLight* createLight(const LIGHT_TYPE& _lightType);

		/** Creates new terrain node.
			@param _desc description of the terrain object.
		*/
		NodeTerrain* createTerrain(const TERRAIN_DESC& _desc);

		/** Creates mesh from file.
			@par
				This is a template function. To properly load and process
				3D mesh file you have to specify loader policy being simply
				the 3D file format loader.
			@par
				Provided policies are:
				* MeshLoadPolicy3dsFile - for loading .3ds file,
				* MeshLoadPolicyXFile - for loading .x files,
				* MeshLoadPolicyCollada - for loading Collada format.
			@param
				_name name of the file to load.
		*/
		template <typename LoaderPolicy>
		NodeMesh <LoaderPolicy>* createMesh(const wstring& _name);

		/** Creates mesh from file.
			@par
				This is a template function. To properly load and process
				3D mesh file you have to specify loader policy being simply
				the 3D file format loader.
			@par
				Provided policies are:
				* MeshLoadPolicy3dsFile - for loading .3ds file,
				* MeshLoadPolicyXFile - for loading .x files,
				* MeshLoadPolicyCollada - for loading Collada format.
			@param
				_name name of the file to load.
			@param
				_transform default transform to apply to vertices.
		*/
		template <typename LoaderPolicy>
		NodeMesh <LoaderPolicy>* createMesh(const wstring& _name, const Matrix4x4& _transform);

		/** Creates mesh from file.
			@par
			This is a template function. To properly load and process
			3D mesh file you have to specify loader policy being simply
			the 3D file format loader.
			@par
				Provided policies are:
				* MeshLoadPolicy3dsFile - for loading .3ds file,
				* MeshLoadPolicyXFile - for loading .x files,
				* MeshLoadPolicyCollada - for loading Collada format.
			@param
				_name name of the file to load.
			@param
				_transform default transform to apply to vertices.
			@param
				_moveToCenter moves the mesh to the center of local coordinate system.
		*/
		template <typename LoaderPolicy>
		NodeMesh <LoaderPolicy>* createMesh(const wstring& _name, const Matrix4x4& _transform,
			bool _moveToCenter);

		/// Creates new particle system.
		NodeParticleSystem* createParticleSystem();

		/// Creates new sound.
		NodeSound* createSound(const wstring& _name, const SOUND_DESC& _desc);


		/// Returns the Root of Scene Graph.
		Node* getRootNode() const;
		/** Returns Node with the given name or false
			when it does not exist. */
		Node* getNode(const wstring& _name);
		HashTable <wstring, Node*>& getNodes();
		/// Returns total number of nodes in the scene graph.
		uint getNodesNum() const;


		/** Adds node to the Scene Graph.
			@param _name name of the node. It has to be unique.
			@param _node pointer to the node object.
			@returns
				bool true when adding a node succeeded and false on
				failure.
			@remarks
				No two nodes can have same name in the scene graph. If
				you provide a name under which another node is found false
				will be returned.
		*/
		bool addToSceneGraph(const wstring& _name, Node* _node);

		/// Removes node from Scene Graph.
		void removeFromSceneGraph(const wstring& _name);

		/// Set scene partitioner.
		void setScenePartitioner(ScenePartitioner* _partitioner);
		/// Returns pointer to the scene partitioner object.
		ScenePartitioner* getScenePartitioner() const;

		/// Call this to make visibility test on all nodes.
		void testVisibility();


		virtual void serialize(ISerializer* _serializer,
							   bool _serializeType,
							   bool _serializeChildren);
		virtual void deserialize(ISerializer* _serializer);
	};



	template <typename LoaderPolicy>
	NodeMesh <LoaderPolicy>* SceneManager::createMesh(const wstring& _name)
	{
		NodeMesh <LoaderPolicy>* pResult = new NodeMesh <LoaderPolicy> (_name);
		pResult->setCreator(this);

		return pResult;
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	NodeMesh <LoaderPolicy>* SceneManager::createMesh(const wstring& _name,
		const Matrix4x4& _transform)
	{
		NodeMesh <LoaderPolicy>* pResult = new NodeMesh <LoaderPolicy> (_name,
			_transform);
		pResult->setCreator(this);

		return pResult;
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	NodeMesh <LoaderPolicy>* SceneManager::createMesh(const wstring& _name,
		const Matrix4x4& _transform, bool _moveToCenter)
	{
		NodeMesh <LoaderPolicy>* pResult = new NodeMesh <LoaderPolicy> (_name,
			_transform, _moveToCenter);
		pResult->setCreator(this);

		return pResult;
	}
//----------------------------------------------------------------------
	inline ScenePartitioner* SceneManager::getScenePartitioner() const
	{
		return m_pPartitioner;
	}
//----------------------------------------------------------------------
	inline HashTable <wstring, Node*>& SceneManager::getNodes()
	{
		return m_pSceneNodes;
	}
//----------------------------------------------------------------------
	inline uint SceneManager::getNodesNum() const
	{
		return m_pSceneNodes.size();
	}
//----------------------------------------------------------------------
	inline Node* SceneManager::getRootNode() const
	{
		return m_pRootNode;
	}
//----------------------------------------------------------------------
}


#endif