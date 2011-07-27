/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Skeleton.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SKELETON_H_
#define __INC_SKELETON_H_


#include "BoundingVolume.h"
#include "Prerequisities.h"
#include "Quaternion.h"
#include "Vector3.h"


namespace nGENE
{
	/// Single bone structure.
	typedef struct nGENEDLL SSkeletonBone
	{
		uint ID;
		string name;

		Matrix4x4 localTransform;				///< Bone local transform
		Matrix4x4 transform;					///< Bone transform
		Matrix4x4 offsetTransform;				///< Bone offset transform

		Quaternion orientation;					///< Bone orientation
		Vector3 translate;						///< Bone translation vector
		Vector3 scale;							///< Bone scaling vector
		Real length;

		Real minAngle;
		Real maxAngle;

		SSkeletonBone* parent;					///< Pointer to the parent bone
		SSkeletonBone* children;				///< Pointer to the children bones
		SSkeletonBone* nextSibling;				///< Pointer to the next sibling of the bone
		SSkeletonBone* prevSibling;				///< Pointer to the previous sibling of the bone

		SSkeletonBone():
			ID(0),
			parent(NULL),
			children(NULL),
			nextSibling(NULL),
			prevSibling(NULL)
		{
		}

		/** Prints the hierarchy of bones starting at this bone down.
			@param _result stream to which hierarchy should be printed.
			@param
				_level starting level of hierarchy. It modifies formatting only.
				Defaults to 0.
		*/
		void print(ostream& _result, uint _level=0);
	} BONE;


	/// Skeleton class.
	class nGENEDLL Skeleton
	{
	private:
		// Hide assignment operator and copy constructor to prevent
		// it from being used.
		Skeleton(const Skeleton& src);
		Skeleton& operator=(const Skeleton& rhs);

	protected:
		vector <BONE*> m_vBones;					///< Vector of bones
		BONE* m_pRoot;								///< Root bone

		/** Calculates initial pose for hierarchy starting at the given bone.
			@param _bone subtree root.
		*/
		void initialPose(BONE* _bone);

		/** Updates bones subtree starting at the given bone.
			@param _bone root of the subtree to be updated.
		*/
		void updateBone(BONE* _bone);

	public:
		Skeleton();
		virtual ~Skeleton();

		/// Adds new bone to the skeleton.
		void addBone(BONE* _bone);
		
		/// Returns bone with the given index.
		BONE* getBone(uint _index);

		/// Returns bone with the given name.
		BONE* getBone(const string& _name);

		/// Sets root bone of the skeleton.
		void setRoot(BONE* _root);

		/// Returns skeleton root.
		BONE* getRoot();

		/// Returns number of bones.
		uint getBonesNum() const;

		/// Prepares for update.
		void prepareForUpdate();

		/// Updates all bones information.
		void update();

		/// Calculates initial positioning of all the bones of the skeleton.
		void calculateInitialPose();
	};



	inline void Skeleton::setRoot(BONE* _root)
	{
		m_pRoot = _root;
	}
//----------------------------------------------------------------------
	inline BONE* Skeleton::getRoot()
	{
		return m_pRoot;
	}
//----------------------------------------------------------------------
	inline uint Skeleton::getBonesNum() const
	{
		return m_vBones.size();
	}
//----------------------------------------------------------------------
}


#endif