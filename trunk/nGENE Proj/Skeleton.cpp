/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		Skeleton.cpp
Version:	0.02
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "Skeleton.h"


namespace nGENE
{
	Skeleton::Skeleton()
	{
	}
//----------------------------------------------------------------------
	Skeleton::~Skeleton()
	{
		if(m_vBones.size())
		{
			for(uint i = 0; i < m_vBones.size(); ++i)
				NGENE_DELETE(m_vBones[i]);
			m_vBones.clear();
		}
	}
//----------------------------------------------------------------------
	void Skeleton::addBone(BONE* _bone)
	{
		if(!m_vBones.size())
			m_pRoot = _bone;
		m_vBones.push_back(_bone);
	}
//----------------------------------------------------------------------
	BONE* Skeleton::getBone(uint _index)
	{
		if(_index >= m_vBones.size())
			return NULL;

		return m_vBones[_index];
	}
//----------------------------------------------------------------------
	BONE* Skeleton::getBone(const string& _name)
	{
		for(uint i = 0; i < m_vBones.size(); ++i)
		{
			if(_name == m_vBones[i]->name)
				return m_vBones[i];
		}

		return NULL;
	}
//----------------------------------------------------------------------
	void Skeleton::update()
	{
		updateBone(m_pRoot);
	}
//----------------------------------------------------------------------
	void Skeleton::calculateInitialPose()
	{
		initialPose(m_pRoot);
	}
//----------------------------------------------------------------------
	void Skeleton::initialPose(BONE* _bone)
	{
		if(_bone->parent)
			_bone->transform = _bone->localTransform * _bone->parent->transform;
		else
			_bone->transform = _bone->localTransform;

		// Recursively update children
		BONE* pChild = _bone->children;
		while(pChild)
		{
			initialPose(pChild);
			pChild = pChild->nextSibling;
		}
	}
//----------------------------------------------------------------------
	void Skeleton::updateBone(BONE* _bone)
	{
		if(_bone->parent)
			_bone->transform = _bone->localTransform * _bone->parent->transform;
		else
			_bone->transform = _bone->localTransform;

		BONE* pChild = _bone->children;
		while(pChild)
		{
			updateBone(pChild);
			pChild = pChild->nextSibling;
		}
	}
//----------------------------------------------------------------------
	void Skeleton::prepareForUpdate()
	{
		for(uint i = 0; i < m_vBones.size(); ++i)
			m_vBones[i]->localTransform.zero();
	}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
	void SSkeletonBone::print(ostream& _result, uint _level)
	{
		for(uint i = 0; i < _level; ++i)
			_result << "-";
		_result << name << endl;

		SSkeletonBone* pBone = children;
		while(pBone)
		{
			pBone->print(_result, _level + 1);
			pBone = pBone->nextSibling;
		}
	}
//----------------------------------------------------------------------
}
