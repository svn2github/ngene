/*
---------------------------------------------------------------------------
This source file is part of nGENE Tech.

Copyright (c) 2006- Wojciech Toman

This program is free software.

File:		SoundManagerEnumerator.h
Version:	0.01
---------------------------------------------------------------------------
*/

#pragma once
#ifndef __INC_SOUNDMANAGERENUMERATOR_H_
#define __INC_SOUNDMANAGERENUMERATOR_H_


#include "FMODSoundManager.h"
#include "HashTable.h"
#include "Prerequisities.h"
#include "Singleton.h"


namespace nGENE
{
	/** ISoundManager Enumerator class.
		@remarks
			Enumerators lets you easily extend nGENE Tech by just adding
			factories of new type to the enumerator. Then by specifying
			the type name you can create objects of any available type
			in the enumerator.
	*/
	class nGENEDLL SoundManagerEnumerator: public Singleton <SoundManagerEnumerator>
	{
	protected:
		/// All factories in the nGENE
		HashTable <wstring, SoundManagerFactory*> m_pFactories;

		// DefaultFactories
		FMODSoundManagerFactory m_FMODFactory;

	public:
		SoundManagerEnumerator();
		~SoundManagerEnumerator();

		/** Adds new factory to the enumerator.
			@remarks
				Factory has to be added to the enumerator prior to
				using it.
		*/
		void addFactory(SoundManagerFactory& _factory, const wstring& _typeName);
		/** Adds new factory to the enumerator.
			@remarks
				Factory has to be added to the enumerator prior to
				using it.
		*/
		void addFactory(SoundManagerFactory* _factory, const wstring& _typeName);

		/// Removes factory for the given type.
		void removeFactory(const wstring& _typeName);

		/// Creates ISoundManager object.
		ISoundManager* create(const wstring& _typeName);

		/// Returns names of the sound managers.
		vector <wstring> getNamesList();
	};
}


#endif