/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		SceneManagerFactory.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SCENEMANAGERFACTORY_H_
#define __INC_SCENEMANAGERFACTORY_H_


#include "SceneManager.h"
#include "Singleton.h"


namespace nGENE
{
	/** Factory object creating SceneManager objects.
	*/
	class nGENEDLL SceneManagerFactory: public Singleton <SceneManagerFactory>
	{
	public:
		/** Type of the manager - it determines the algorithms being used
			to optimize rendering process.
			@todo
				Change this into enumerator.
		*/
		enum SCENE_MANAGER_TYPE
		{
			SMT_UNKNOWN,		///< Optimizations used are unknown
			SMT_GENERIC,		/**< No special optimizations are used.
									 Although it might seem to be very bad
									 solution, in some cases it may turn out
									 to be quite efficient eg. when there are
									 multiple outdoor and indoor areas in the
									 same world. */
			SMT_OUTDOOR,		/**< Used for rendering terrain. Mainly quad-tree
									 is used as an optimization method. */
			SMT_INDOOR,			/**< Portal/Sector system is used */
			SMT_SEAMLESS
		};

	public:
		SceneManagerFactory();
		virtual ~SceneManagerFactory();

		/** Creates new SceneManager object of defined type. Multiple SceneManagers
			can be used at the same time.
		*/
		virtual SceneManager* createSceneManager(SCENE_MANAGER_TYPE type) const;
	};
}


#endif