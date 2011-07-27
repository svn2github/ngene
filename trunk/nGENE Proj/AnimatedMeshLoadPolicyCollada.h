/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		AnimatedMeshLoadPolicyCollada.h
Version:	0.03
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_ANIMATEDMESHLOADPOLICYCOLLADA_H_
#define __INC_ANIMATEDMESHLOADPOLICYCOLLADA_H_


#include "Prerequisities.h"
#include "MeshLoadPolicyCollada.h"
#include "SkeletalAnimationController.h"
#include "Surface.h"
#include "TypeInfo.h"
#include "Matrix4x4.h"


namespace nGENE
{
	/** Animated mesh loader for the collada format.
	*/
	class nGENEDLL AnimatedMeshLoadPolicyCollada: public MeshLoadPolicyCollada
	{
		EXPOSE_TYPE
	private:
		struct SVertexInfluence
		{
			uint bonesNum;
			vector <uint> bonesIndices;
			vector <Real> weights;
		};

		struct SAnimationChannel
		{
			string source;
			string targetBone;
			string targetValue;
		};

		struct SAnimationSampler
		{
			vector <SColladaSemantic> semantics;
		};

		Skeleton m_Skeleton;					///< This mesh skeleton

		HashTable <string, SAnimationSampler> m_Samplers;

		vector <SVertexInfluence> m_Influences;	///< Bones influence on vertices
		vector <SAnimationChannel> m_Channels;	///< Animation channels set
		vector <KeyFrameAnimationSet> m_Sets;	///< Animations sets
		vector <Real> m_vWeights;				///< Array of bone weights

		Matrix4x4 m_matTransform;
		bool m_bMoveToCentre;


		/// Processes single animation.
		void processAnimation(XMLDocument* _node);
		/// Processes animation sampler.
		void processSampler(XMLDocument* _node, const string& _id);
		/// Processes single controller.
		void processController(XMLDocument* _node, const string& _id);
		/// Processes skin.
		void processSkin(XMLDocument* _node, const string& _id);
		/// Processes joints.
		void processJoints(XMLDocument* _node);
		/// Processes bind poses.
		void processBindPoses(XMLDocument* _node);
		/// Processes weights.
		void processWeightsArray(XMLDocument* _node);
		/// Processes weights.
		void processWeights(XMLDocument* _node);

		void processNodeType(const string& _type, SModel* _model, const Matrix4x4& _transform,
			vector <Surface>& _surfaces, const string& _nodeName, const string& _parentName,
			const string& _parentType, const Matrix4x4* _parentTransform);

		void loadAnimations(XMLDocument* _node, vector <Surface>& _surfaces);
		void loadControllers(XMLDocument* _node, vector <Surface>& _surfaces);

		void createAnimations();
		void createMesh(SModel* _model, STriangleSet* _set, Surface& _surface,
			const Matrix4x4& _transform);

	protected:
		~AnimatedMeshLoadPolicyCollada() {}

	public:
		AnimatedMeshLoadPolicyCollada();

		/** Loads contents of the file.
			@remarks
				Simply call it on host application to obtain
				all mesh data contained in the file.
		*/
		vector <Surface> loadMesh(const wstring& _fileName, const Matrix4x4& _transform,
			bool _moveToCentre);

		/// Returns reference to the underlying skeleton object.
		Skeleton& getSkeleton();

		/// Returns number of animations sets.
		uint getAnimationSetsNum() const;

		/// Returns animation set with the given index.
		KeyFrameAnimationSet& getAnimationSet(uint _index);
	};



	inline Skeleton& AnimatedMeshLoadPolicyCollada::getSkeleton()
	{
		return m_Skeleton;
	}
//----------------------------------------------------------------------
	inline uint AnimatedMeshLoadPolicyCollada::getAnimationSetsNum() const
	{
		return m_Sets.size();
	}
//----------------------------------------------------------------------
	inline KeyFrameAnimationSet& AnimatedMeshLoadPolicyCollada::getAnimationSet(uint _index)
	{
		return m_Sets[_index];
	}
//----------------------------------------------------------------------
}

#endif