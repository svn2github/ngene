/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		SceneManagerFactory.cpp
Version:	0.01
---------------------------------------------------------------------------
*/

#include "PrecompiledHeaders.h"
#include "SceneManagerFactory.h"


namespace nGENE
{
	SceneManagerFactory::SceneManagerFactory()
	{
	}
//----------------------------------------------------------------------
	SceneManagerFactory::~SceneManagerFactory()
	{
	}
//----------------------------------------------------------------------
	SceneManager* SceneManagerFactory::createSceneManager(SCENE_MANAGER_TYPE _type) const
	{
		return (new SceneManager());
	}
//----------------------------------------------------------------------
}