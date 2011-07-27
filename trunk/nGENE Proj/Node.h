/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Node.h
Version:	0.20
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_NODE_H_
#define __INC_NODE_H_


#include "BaseClass.h"
#include "ISerializable.h"
#include "Maths.h"
#include "Matrix4x4.h"
#include "Prerequisities.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Task.h"
#include "TypeInfo.h"


namespace nGENE
{
	/// Up vector of the Node - important for animation.
	enum UP_VECTOR
	{
		UV_Y,
		UV_Z
	};

	/** Base node class.
		@par
			Nodes are objects which build a 3D world in nGENE Tech applications.
		@par
			It is important to notice than node owns its children. It means that
			if a child node has been allocated on the heap it is its parent
			who is responsible for freeing the memory - not the client application!
		@remarks
			It is important to note that all the Nodes in nGENE Tech are managed
			by SceneManager. Even if you create Nodes by other means than
			calling appropriate SceneManager method you have to assign them to
			one of the existing SceneManagers by calling Node::setCreator() and
			passing pointer to this manager as an argument.
	*/
	class nGENEDLL Node: public BaseClass, public ISerializable,
		public Task
	{
		EXPOSE_TYPE
	protected:
		/** Pointer to the parental Node. Thanks to that
			you can easily create nodes hierarchy.
		*/
		Node* m_pParent;
		/// Next sibling in the nodes hierarchy.
		Node* m_pNextSibling;
		/// Previous sibling in the nodes hierarchy.
		Node* m_pPrevSibling;

		/// Pointer to the first child
		Node* m_pChildren;

		/// Pointer to the SceneManager which created the node.
		SceneManager* m_pCreator;

		/// Number of children of this Node
		uint m_nChildrenNum;

		Vector3 m_vecPosition;					///< Node's position
		Vector3 m_vecRotation;					///< Rotation in Euler angles
		Vector3 m_vecScale;						///< Node's scale
		Quaternion m_quatOrientation;			///< Node's orientation
		Matrix4x4 m_matTransform;				///< All transformations in one matrix

		UP_VECTOR m_UpVector;					///< Up vector of the Node

		bool m_bIsInSceneGraph;
		bool m_bEnabled;						/**< Is node enabled? Parameters of disabled
													 node are not being calculated. */
		bool m_bHasChanged;						///< Has node changed since last frame?
		bool m_bIsAllocatedOnHeap;				///< Is node allocated on heap?

		wstring m_stName;						///< Node's name

		/// Animation controllers to use for this node.
		vector <IAnimationController*> m_vControllers;


		/// Called in update function to change update() behaviour.
		virtual void onUpdate();

		/// Builds Task dependencies.
		void initDependencies(Task* _task);

	public:
		Node();
		virtual ~Node();


		virtual void init();
		virtual void cleanup();

		/// Updates the node.
		void update();
		void onEndUpdate();

		/** Specifies whether node is enabled or not.
			@remarks
				Note that setting Node's 'enabled' attribute to false will
				also disable all its children i.e. prevent them from updating
				and in case of NodeVisible type - of rendering. Setting
				this property to true does not influence child nodes.
		*/
		virtual void setEnabled(bool _value);
		/// Toggles enabled attribute.
		virtual void toggleEnabled();
		/** Checks whether node is enabled.
			@remarks
				Only enabled nodes are updated and (in case
				of NodeVisible type objects) rendered.
		*/
		virtual bool isEnabled() const;

		/// Checks if the node changed since last frame.
		bool hasChanged() const;


		/// Sets Node's name.
		void setName(const wstring& _name);
		/// Returns Node's name.
		wstring& getName();

		/** Sets the node's parental node.
			@remarks
				By creating son-parent relations between nodes
				we get nodes hierarchy.
		*/
		void setParent(Node* _parent);
		/// Returns Node's parent Node.
		Node* getParent() const;

		/// Sets pointer to the next sibling object.
		void setNextSibling(Node* _sib);
		/// Returns next sibling of the node.
		Node* getNextSibling() const;
		/// Sets previous sibling of the node.
		void setPrevSibling(Node* _sib);
		/// Returns previous sibling of the node.
		Node* getPrevSibling() const;

		/// Adds child to the Node.
		void addChild(const wstring& _name, Node& _child);
		/// Adds child to the Node.
		void addChild(const wstring& _name, Node* _child);
		/** Returns pointer to the last child added.
			@remarks
				To obtain pointers to the rest of the children use getNextSibling()
				method on the child Node object.
		*/
		Node* getChild() const;

		/// Removes specified child.
		void removeChild(Node* _child);

		/// Returns total number of Node's children.
		uint getChildrenNum() const;

		/// Moves node by the given vector.
		virtual void translate(const Vector3& _translate);
		/// Moves node by the given vector.
		virtual void translate(Real _x, Real _y, Real _z);

		/// Sets node local position.
		virtual void setPosition(const Vector3& _pos);
		/// Sets node local position.
		virtual void setPosition(Real _x, Real _y, Real _z);
		/// Returns node position.
		Vector3& getPositionLocal();

		/// Sets node position in world space.
		virtual void setPositionWorld(const Vector3& _pos);
		/// Sets node position in world space.
		virtual void setPositionWorld(Real _x, Real _y, Real _z);
		/// Returns position in world space coordinates.
		Vector3 getPositionWorld();

		Quaternion getRotationWorld();

		/// Returns world space transform of this Node.
		const Matrix4x4& getTransform() const;
		/// All transformations are applied to the node building its matrix.
		void applyTransform();

		/** Lets you scale the object non-uniformly.
			@remarks
				You can scale node by each axis independently. You have to
				notice that scaling factor above 1.0 causes nodes being
				enlarged; below 1.0 - made smaller. Use 1.0 if you want no changes.
		*/
		void setScale(Real _x, Real _y, Real _z);
		/** Lets you scale the object non-uniformly.
			@remarks
				You can scale node by each axis independently. You have to
				notice that scaling factor above 1.0 causes nodes being
				enlarged; below 1.0 - made smaller. Use 1.0 if you want no changes.
		*/
		void setScale(const Vector3& _scale);
		/// Uniform scaling of the node.
		void setScale(Real _scale);
		/// Returns scale factors for this node.
		Vector3& getScale() {return m_vecScale;}

		/** Rotates node around all axes. Actually it is rotation using Euler angles.
			@param _rot Euler angles: x - Pitch, y - Yaw, z - Roll.
			@remarks
				In case of rotation functions note that following convention
				is used in nGENE: yaw stands for rotation around z axis, pitch - x
				and roll - y axis.
		*/
		virtual void setRotation(const Vector3& _rot);
		/** Rotates node around all axes. Actually it is rotation using Euler angles.
			@remarks
				In case of rotation functions note that following convention
				is used in nGENE: yaw stands for rotation around z axis, pitch - x
				and roll - y axis.
		*/
		virtual void setRotation(Real _yaw, Real _pitch, Real _roll);
		virtual void setRotation(const Vector3& _axis, const Real _angle);
		virtual void setRotation(const Quaternion& _quat);
		/// Returns Node's orientation.
		virtual Quaternion& getRotation();

		/// Sets scene creator (i.e. SceneManager it belongs to).
		void setCreator(SceneManager* _creator);
		/// Returns SceneManager which owns this Node.
		SceneManager* getCreator() const;


		//// Adds node to the specified partitioner.
		virtual void addToPartitioner(ScenePartitioner* _partitioner);
		//// Removes node from the specified partitioner.
		virtual void removeFromPartitioner(ScenePartitioner* _partitioner);


		/// Serializes node object.
		virtual void serialize(ISerializer* _serializer, bool _serializeType, bool _serializeChildren);
		/// Deserializes node object.
		virtual void deserialize(ISerializer* _serializer);

		/// Adds new animation controller for this node.
		virtual void addAnimationController(IAnimationController* _controller);
		/// Removes animation controller with the given index.
		virtual void removeAnimationController(uint _index);
		/// Removes given animation controller.
		virtual void removeAnimationController(IAnimationController* _controller);
		/// Returns controller with the given index for this node.
		virtual IAnimationController* getAnimationController(uint _index) const;

		/// Sets up vector of the Node.
		virtual void setUpVector(UP_VECTOR _up);
		/// Returns up vector of the Node.
		UP_VECTOR getUpVector() const;
	};



	inline void Node::toggleEnabled()
	{
		m_bEnabled = (m_bEnabled ? false : true);
	}
//----------------------------------------------------------------------
	inline void Node::setParent(Node* _parent)
	{
		m_pParent = _parent;
	}
//----------------------------------------------------------------------
	inline Node* Node::getParent() const
	{
		return m_pParent;
	}
//----------------------------------------------------------------------
	inline Node* Node::getChild() const
	{
		return m_pChildren;
	}
//----------------------------------------------------------------------
	inline void Node::setEnabled(bool _value)
	{
		m_bEnabled = _value;
	}
//----------------------------------------------------------------------
	inline void Node::setCreator(SceneManager* _creator)
	{
		m_pCreator = _creator;
	}
//----------------------------------------------------------------------
	inline SceneManager* Node::getCreator() const
	{
		return m_pCreator;
	}
//----------------------------------------------------------------------
	inline const Matrix4x4& Node::getTransform() const
	{
		return m_matTransform;
	}
//----------------------------------------------------------------------
	inline bool Node::hasChanged() const
	{
		return m_bHasChanged;
	}
//----------------------------------------------------------------------
	inline void Node::setName(const wstring& _name)
	{
		m_stName = _name;
	}
//----------------------------------------------------------------------
	inline wstring& Node::getName()
	{
		return m_stName;
	}
//----------------------------------------------------------------------
	inline void Node::setNextSibling(Node* _sib)
	{
		m_pNextSibling = _sib;
	}
//----------------------------------------------------------------------
	inline Node* Node::getNextSibling() const
	{
		return m_pNextSibling;
	}
//----------------------------------------------------------------------
	inline void Node::setPrevSibling(Node* _sib)
	{
		m_pPrevSibling = _sib;
	}
//----------------------------------------------------------------------
	inline Node* Node::getPrevSibling() const
	{
		return m_pPrevSibling;
	}
//----------------------------------------------------------------------
	inline uint Node::getChildrenNum() const
	{
		return m_nChildrenNum;
	}
//----------------------------------------------------------------------
	inline Vector3& Node::getPositionLocal()
	{
		return m_vecPosition;
	}
//----------------------------------------------------------------------
	inline Quaternion& Node::getRotation()
	{
		return m_quatOrientation;
	}
//----------------------------------------------------------------------
	inline void Node::setUpVector(UP_VECTOR _up)
	{
		m_UpVector = _up;
	}
//----------------------------------------------------------------------
	inline UP_VECTOR Node::getUpVector() const
	{
		return m_UpVector;
	}
//----------------------------------------------------------------------
}


#endif