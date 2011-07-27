/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		SkeletalAnimationController.h
Version:	0.07
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SKELETALANIMATIONCONTROLLER_H_
#define __INC_SKELETALANIMATIONCONTROLLER_H_


#include "IAnimationController.h"
#include "Prerequisities.h"
#include "KeyFrameAnimationSet.h"
#include "NodeVisible.h"
#include "Skeleton.h"
#include "ShaderConstant.h"
#include "Timer.h"


namespace nGENE
{
	/** Skeletal animation controller.
		@par
			It support animation blending out of the box by using
			weights for KeyFrameAnimationSet objects.
	*/
	template <typename LoaderPolicy>
	class SkeletalAnimationController: public IAnimationController
	{
	protected:
		NodeMesh <LoaderPolicy>* m_pMesh;		///< Pointer to the mesh object
		Skeleton* m_pSkeleton;					///< Pointer to the skeleton object

		float m_fAnimationSpeed;				///< Animation speed

		Semaphore m_FillCount;
		Semaphore m_EmptyCount;

		/// Vector holding all animation sets.
		vector <KeyFrameAnimationSet> m_AnimationSets;
		list <float*> m_pMatrices;

		ShaderConstant* m_pConstantMatrices;
		ShaderConstant* m_pConstantMatricesShadow;


		/// Normalizes weights for animation sets.
		void normalizeWeights();

	public:
		/** SkeletalAnimationController constructor.
			@param _skeleton pointer to the skeleton object to animate.
			@param _maximum size of buffer used by the algorithm.
		*/
		SkeletalAnimationController(Skeleton* _skeleton, uint _bufferSize=3);
		
		/** Sets node.
			@remarks
				_node must be of NodeMesh <LoaderPolicy> type.
		*/
		virtual void setNode(NodeMesh <LoaderPolicy>* _node);
		
		/** Sets node.
			@remarks
				_node must be of NodeMesh <LoaderPolicy> type.
		*/
		virtual void setNode(Node* _node);

		virtual void retrieveData();

		/// Returns pointer to the skeleton associated with this controller.
		Skeleton* getSkeleton();

		/** Adds new animation set to this skeletal animation.
			@param _set animation set to be added.
		*/
		void addAnimationSet(KeyFrameAnimationSet& _set);

		/** Returns animation set with the given index.
			@param
				_index index of the desired animation set. Have to fall in
				[0; animationSetsCount) range.
			@returns
				KeyFrameAnimationSet* pointer to the desired animation set or
				NULL if the _index was invalid.
		*/
		KeyFrameAnimationSet* getAnimationSet(uint _index);
		/** Returns animation set with the given name.
			@param
				_name name of the desired animation set.
			@returns
				KeyFrameAnimationSet* pointer to the desired animation set or
				NULL if the set with such a name was not found.
		*/
		KeyFrameAnimationSet* getAnimationSet(const wstring& _name);

		/** Sets weight of the given animation set.
			@remarks
				This method is preferred in most cases to the direct calling
				of KeyFrameAnimationSet::setWeight() as it takes care of normalizing
				weights, i.e. it prevents situation in which sum of weights
				does not equal 1.0 what could result in strange behaviour
				of the animated mesh.
			@param _index index of the animation set.
			@param _weight for the animation set.
		*/
		void setAnimationSetWeight(uint _index, Real _weight);
		/** Sets weight of the given animation set.
			@remarks
				This method is preferred in most cases to the direct calling
				of KeyFrameAnimationSet::setWeight() as it takes care of normalizing
				weights, i.e. it prevents situation in which sum of weights
				does not equal 1.0 what could result in strange behaviour
				of the animated mesh.
			@param _name name of the animation set.
			@param _weight for the animation set.
		*/
		void setAnimationSetWeight(const wstring& _name, Real _weight);

		/// Sets speed of the animation.
		void setAnimationSpeed(float _speed);
		/// Returns speed of the animation.
		float getAnimationSpeed() const;

		void init();

		/// Performs skeletal animation.
		virtual void update();

		/// Returns number of KeyFrameAnimationSets in this controller.
		uint getAnimationSetsNum() const;

		/// Returns number of underlying Skeleton bones.
		uint getBonesNum() const;

		/// Returns bone from the Skeleton object with the given index.
		BONE* getBone(uint _index) const;

		/// Returns bone from the Skeleton object with the given name.
		BONE* getBone(const wstring& _name) const;

		/// Sets pointers to some of the variables.
		void setConstants(ShaderConstant* _matrices, ShaderConstant* _shadowMatrices);
	};



	template <typename LoaderPolicy>
	void SkeletalAnimationController <LoaderPolicy>::init()
	{
		m_EmptyCount.down();

		bool atLeastOneEnabled = false;
		for(uint i = 0; i < m_AnimationSets.size(); ++i)
		{
			if(m_AnimationSets[i].isEnabled())
			{
				atLeastOneEnabled = true;
				break;
			}
		}

		if(atLeastOneEnabled)
			m_pSkeleton->prepareForUpdate();

		// First animate all the bones
		for(uint i = 0; i < m_AnimationSets.size(); ++i)
			m_AnimationSets[i].animate(0.0f);

		// Then process all the bones
		if(atLeastOneEnabled)
		{
			m_pSkeleton->update();

			float* pMatrices = new float[12 * m_pSkeleton->getBonesNum()];
			float* pDest = pMatrices;

			for(uint i = 0; i < m_pSkeleton->getBonesNum(); ++i)
			{
				Matrix4x4 mat = m_pSkeleton->getBone(i)->offsetTransform * m_pSkeleton->getBone(i)->transform;

				float source[12];
				int index = 0;
				for(int k = 0; k < 4; ++k)
				{
					for(int l = 0; l < 3; ++l)
						source[index++] = mat._m[k][l];
				}

				memcpy(pDest, source, 12 * sizeof(float));
				pDest += 12;		
			}


			m_pMatrices.push_back(pMatrices);
		}

		m_FillCount.up(1);

		bool bOldAnimating = m_bAnimating;
		m_bAnimating = true;

		retrieveData();

		m_bAnimating = bOldAnimating;
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	void SkeletalAnimationController <LoaderPolicy>::update()
	{
		if(!m_bStopped)
		{
			if(!m_bAnimating)
			{
				m_nPrevTime = Engine::getSingleton().getTimer().getMilliseconds();
				return;
			}

			m_EmptyCount.down();
			
			dword now = Engine::getSingleton().getTimer().getMilliseconds();
			float diff = (float)(now - m_nPrevTime);

			bool atLeastOneEnabled = false;
			for(uint i = 0; i < m_AnimationSets.size(); ++i)
			{
				if(m_AnimationSets[i].isEnabled())
				{
					atLeastOneEnabled = true;
					break;
				}
			}

			if(atLeastOneEnabled)
				m_pSkeleton->prepareForUpdate();

			// First animate all the bones
			for(uint i = 0; i < m_AnimationSets.size(); ++i)
				m_AnimationSets[i].animate(diff * m_fAnimationSpeed);

			// Then process all the bones
			if(atLeastOneEnabled)
			{
				m_pSkeleton->update();

				float* pMatrices = new float[12 * m_pSkeleton->getBonesNum()];
				float* pDest = pMatrices;

				for(uint i = 0; i < m_pSkeleton->getBonesNum(); ++i)
				{
					Matrix4x4 mat = m_pSkeleton->getBone(i)->offsetTransform * m_pSkeleton->getBone(i)->transform;

					float source[12];
					int index = 0;
					for(int k = 0; k < 4; ++k)
					{
						for(int l = 0; l < 3; ++l)
							source[index++] = mat._m[k][l];
					}

					memcpy(pDest, source, 12 * sizeof(float));
					pDest += 12;		
				}


				m_pMatrices.push_back(pMatrices);
			}
			else
				m_bFinished = true;

			m_FillCount.up(1);

			m_nPrevTime = now;
		}
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	SkeletalAnimationController <LoaderPolicy>::SkeletalAnimationController(Skeleton* _skeleton, uint _bufferSize):
	m_pMesh(NULL),
		m_pSkeleton(_skeleton),
		m_fAnimationSpeed(1.0f),
		m_FillCount(0, _bufferSize),
		m_EmptyCount(_bufferSize, _bufferSize),
		m_pConstantMatrices(NULL),
		m_pConstantMatricesShadow(NULL)
	{
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	void SkeletalAnimationController <LoaderPolicy>::setNode(NodeMesh <LoaderPolicy>* _node)
	{
		m_pMesh = _node;
		IAnimationController::setNode(m_pMesh);
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	void SkeletalAnimationController <LoaderPolicy>::setNode(Node* _node)
	{
		m_pMesh = (NodeMesh <LoaderPolicy>*)_node;
		IAnimationController::setNode(m_pMesh);
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	void SkeletalAnimationController <LoaderPolicy>::retrieveData()
	{
		// Not animating so try to not lock whole engine :)
		if(!m_bAnimating)
			return;

		if(!m_FillCount.tryDown())
			return;

			float* pMatrices = m_pMatrices.front();
			m_pMatrices.pop_front();

		m_EmptyCount.up(1);

		uint count = m_pSkeleton->getBonesNum() * 12;
		if(m_pConstantMatrices)
			m_pConstantMatrices->setDefaultValue(pMatrices, count);
		if(m_pConstantMatricesShadow)
			m_pConstantMatricesShadow->setDefaultValue(pMatrices, count);

		NGENE_DELETE_ARRAY(pMatrices);
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	Skeleton* SkeletalAnimationController <LoaderPolicy>::getSkeleton()
	{
		return m_pSkeleton;
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	void SkeletalAnimationController <LoaderPolicy>::addAnimationSet(KeyFrameAnimationSet& _set)
	{
		m_AnimationSets.push_back(_set);

		normalizeWeights();
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	void SkeletalAnimationController <LoaderPolicy>::normalizeWeights()
	{
		// Nothing to normalize
		if(!m_AnimationSets.size())
			return;

		// Normalize weights
		Real fSum = 0.0f;
		for(uint i = 0; i < m_AnimationSets.size(); ++i)
			fSum += m_AnimationSets[i].getWeight();

		if(fSum > 0.0f)
		{
			Real fInvSum = 1.0f / fSum;
			for(uint i = 0; i < m_AnimationSets.size(); ++i)
			{
				KeyFrameAnimationSet& set = m_AnimationSets[i];
				set.setWeight(set.getWeight() * fInvSum);
			}
		}
		else
			m_AnimationSets[0].setWeight(1.0f);
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	void SkeletalAnimationController <LoaderPolicy>::setAnimationSetWeight(uint _index, Real _weight)
	{
		Assert(_index < m_AnimationSets.size(), "Animation set index out of bounds");

		m_AnimationSets[_index].setWeight(_weight);

		normalizeWeights();
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	void SkeletalAnimationController <LoaderPolicy>::setAnimationSetWeight(const wstring& _name, Real _weight)
	{
		KeyFrameAnimationSet* pSet = getAnimationSet(_name);
		if(!pSet)
			return;

		pSet->setWeight(_weight);

		normalizeWeights();
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	KeyFrameAnimationSet* SkeletalAnimationController <LoaderPolicy>::getAnimationSet(uint _index)
	{
		if(_index >= m_AnimationSets.size())
			return NULL;

		return &m_AnimationSets[_index];
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	KeyFrameAnimationSet* SkeletalAnimationController <LoaderPolicy>::getAnimationSet(const wstring& _name)
	{
		for(uint i = 0; i < m_AnimationSets.size(); ++i)
		{
			if(m_AnimationSets[i].getName() == _name)
				return &m_AnimationSets[i];
		}

		return NULL;
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	void SkeletalAnimationController <LoaderPolicy>::setAnimationSpeed(float _speed)
	{
		m_fAnimationSpeed = _speed;
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	float SkeletalAnimationController <LoaderPolicy>::getAnimationSpeed() const
	{
		return m_fAnimationSpeed;
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	uint SkeletalAnimationController <LoaderPolicy>::getAnimationSetsNum() const
	{
		return m_AnimationSets.size();
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	uint SkeletalAnimationController <LoaderPolicy>::getBonesNum() const
	{
		return m_pSkeleton->getBonesNum();
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	BONE* SkeletalAnimationController <LoaderPolicy>::getBone(uint _index) const
	{
		return m_pSkeleton->getBone(_index);
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	BONE* SkeletalAnimationController <LoaderPolicy>::getBone(const wstring& _name) const
	{
		return m_pSkeleton->getBone(_name);
	}
//----------------------------------------------------------------------
	template <typename LoaderPolicy>
	void SkeletalAnimationController <LoaderPolicy>::setConstants(ShaderConstant* _matrices, ShaderConstant* _shadowMatrices)
	{
		m_pConstantMatrices = _matrices;
		m_pConstantMatricesShadow = _shadowMatrices;
	}
//----------------------------------------------------------------------
}


#endif